/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "DFGJITCompiler.h"

#if ENABLE(DFG_JIT)

#include "CodeBlock.h"
#include "DFGOSRExitCompiler.h"
#include "DFGOperations.h"
#include "DFGRegisterBank.h"
#include "DFGSpeculativeJIT.h"
#include "DFGThunks.h"
#include "JSGlobalData.h"
#include "LinkBuffer.h"

namespace JSC { namespace DFG {

void JITCompiler::linkOSRExits()
{
    for (unsigned i = 0; i < codeBlock()->numberOfOSRExits(); ++i) {
        OSRExit& exit = codeBlock()->osrExit(i);
        exit.m_check.initialJump().link(this);
        jitAssertHasValidCallFrame();
        store32(TrustedImm32(i), &globalData()->osrExitIndex);
        exit.m_check.switchToLateJump(patchableJump());
    }
}

void JITCompiler::compileEntry()
{
    // This code currently matches the old JIT. In the function header we need to
    // pop the return address (since we do not allow any recursion on the machine
    // stack), and perform a fast register file check.
    // FIXME: https://bugs.webkit.org/show_bug.cgi?id=56292
    // We'll need to convert the remaining cti_ style calls (specifically the register file
    // check) which will be dependent on stack layout. (We'd need to account for this in
    // both normal return code and when jumping to an exception handler).
    preserveReturnAddressAfterCall(GPRInfo::regT2);
    emitPutToCallFrameHeader(GPRInfo::regT2, RegisterFile::ReturnPC);
    emitPutImmediateToCallFrameHeader(m_codeBlock, RegisterFile::CodeBlock);
}

void JITCompiler::compileBody(SpeculativeJIT& speculative)
{
    // We generate the speculative code path, followed by OSR exit code to return
    // to the old JIT code if speculations fail.

#if DFG_ENABLE(JIT_BREAK_ON_EVERY_FUNCTION)
    // Handy debug tool!
    breakpoint();
#endif
    
    addPtr(TrustedImm32(1), AbsoluteAddress(codeBlock()->addressOfSpeculativeSuccessCounter()));

    bool compiledSpeculative = speculative.compile();
    ASSERT_UNUSED(compiledSpeculative, compiledSpeculative);

    linkOSRExits();

    // Iterate over the m_calls vector, checking for jumps to link.
    bool didLinkExceptionCheck = false;
    for (unsigned i = 0; i < m_exceptionChecks.size(); ++i) {
        Jump& exceptionCheck = m_exceptionChecks[i].m_exceptionCheck;
        if (exceptionCheck.isSet()) {
            exceptionCheck.link(this);
            didLinkExceptionCheck = true;
        }
    }

    // If any exception checks were linked, generate code to lookup a handler.
    if (didLinkExceptionCheck) {
        // lookupExceptionHandler is passed two arguments, exec (the CallFrame*), and
        // the index into the CodeBlock's callReturnIndexVector corresponding to the
        // call that threw the exception (this was set in nonPreservedNonReturnGPR, when
        // the exception check was planted).
        move(GPRInfo::nonPreservedNonReturnGPR, GPRInfo::argumentGPR1);
        move(GPRInfo::callFrameRegister, GPRInfo::argumentGPR0);
#if CPU(X86)
        // FIXME: should use the call abstraction, but this is currently in the SpeculativeJIT layer!
        poke(GPRInfo::argumentGPR0);
        poke(GPRInfo::argumentGPR1, 1);
#endif
        m_calls.append(CallLinkRecord(call(), lookupExceptionHandler));
        // lookupExceptionHandler leaves the handler CallFrame* in the returnValueGPR,
        // and the address of the handler in returnValueGPR2.
        jump(GPRInfo::returnValueGPR2);
    }
}

void JITCompiler::link(LinkBuffer& linkBuffer)
{
    // Link the code, populate data in CodeBlock data structures.
#if DFG_ENABLE(DEBUG_VERBOSE)
    dataLog("JIT code for %p start at [%p, %p). Size = %zu.\n", m_codeBlock, linkBuffer.debugAddress(), static_cast<char*>(linkBuffer.debugAddress()) + linkBuffer.debugSize(), linkBuffer.debugSize());
#endif

    // Link all calls out from the JIT code to their respective functions.
    for (unsigned i = 0; i < m_calls.size(); ++i)
        linkBuffer.link(m_calls[i].m_call, m_calls[i].m_function);

    if (m_codeBlock->needsCallReturnIndices()) {
        m_codeBlock->callReturnIndexVector().reserveCapacity(m_exceptionChecks.size());
        for (unsigned i = 0; i < m_exceptionChecks.size(); ++i) {
            unsigned returnAddressOffset = linkBuffer.returnAddressOffset(m_exceptionChecks[i].m_call);
            CodeOrigin codeOrigin = m_exceptionChecks[i].m_codeOrigin;
            while (codeOrigin.inlineCallFrame)
                codeOrigin = codeOrigin.inlineCallFrame->caller;
            unsigned exceptionInfo = codeOrigin.bytecodeIndex;
            m_codeBlock->callReturnIndexVector().append(CallReturnOffsetToBytecodeOffset(returnAddressOffset, exceptionInfo));
        }
    }

    Vector<CodeOriginAtCallReturnOffset>& codeOrigins = m_codeBlock->codeOrigins();
    codeOrigins.resize(m_exceptionChecks.size());
    
    for (unsigned i = 0; i < m_exceptionChecks.size(); ++i) {
        CallExceptionRecord& record = m_exceptionChecks[i];
        unsigned returnAddressOffset = linkBuffer.returnAddressOffset(m_exceptionChecks[i].m_call);
        codeOrigins[i].codeOrigin = record.m_codeOrigin;
        codeOrigins[i].callReturnOffset = returnAddressOffset;
        record.m_token.assertCodeOriginIndex(i);
    }
    
    m_codeBlock->setNumberOfStructureStubInfos(m_propertyAccesses.size());
    for (unsigned i = 0; i < m_propertyAccesses.size(); ++i) {
        StructureStubInfo& info = m_codeBlock->structureStubInfo(i);
        CodeLocationCall callReturnLocation = linkBuffer.locationOf(m_propertyAccesses[i].m_functionCall);
        info.codeOrigin = m_propertyAccesses[i].m_codeOrigin;
        info.callReturnLocation = callReturnLocation;
        info.patch.dfg.deltaCheckImmToCall = differenceBetweenCodePtr(linkBuffer.locationOf(m_propertyAccesses[i].m_deltaCheckImmToCall), callReturnLocation);
        info.patch.dfg.deltaCallToStructCheck = differenceBetweenCodePtr(callReturnLocation, linkBuffer.locationOf(m_propertyAccesses[i].m_deltaCallToStructCheck));
#if USE(JSVALUE64)
        info.patch.dfg.deltaCallToLoadOrStore = differenceBetweenCodePtr(callReturnLocation, linkBuffer.locationOf(m_propertyAccesses[i].m_deltaCallToLoadOrStore));
#else
        info.patch.dfg.deltaCallToTagLoadOrStore = differenceBetweenCodePtr(callReturnLocation, linkBuffer.locationOf(m_propertyAccesses[i].m_deltaCallToTagLoadOrStore));
        info.patch.dfg.deltaCallToPayloadLoadOrStore = differenceBetweenCodePtr(callReturnLocation, linkBuffer.locationOf(m_propertyAccesses[i].m_deltaCallToPayloadLoadOrStore));
#endif
        info.patch.dfg.deltaCallToSlowCase = differenceBetweenCodePtr(callReturnLocation, linkBuffer.locationOf(m_propertyAccesses[i].m_deltaCallToSlowCase));
        info.patch.dfg.deltaCallToDone = differenceBetweenCodePtr(callReturnLocation, linkBuffer.locationOf(m_propertyAccesses[i].m_deltaCallToDone));
        info.patch.dfg.baseGPR = m_propertyAccesses[i].m_baseGPR;
#if USE(JSVALUE64)
        info.patch.dfg.valueGPR = m_propertyAccesses[i].m_valueGPR;
#else
        info.patch.dfg.valueTagGPR = m_propertyAccesses[i].m_valueTagGPR;
        info.patch.dfg.valueGPR = m_propertyAccesses[i].m_valueGPR;
#endif
        info.patch.dfg.scratchGPR = m_propertyAccesses[i].m_scratchGPR;
        info.patch.dfg.registersFlushed = m_propertyAccesses[i].m_registerMode == PropertyAccessRecord::RegistersFlushed;
    }
    
    m_codeBlock->setNumberOfCallLinkInfos(m_jsCalls.size());
    for (unsigned i = 0; i < m_jsCalls.size(); ++i) {
        CallLinkInfo& info = m_codeBlock->callLinkInfo(i);
        info.callType = m_jsCalls[i].m_callType;
        info.isDFG = true;
        info.callReturnLocation = CodeLocationLabel(linkBuffer.locationOf(m_jsCalls[i].m_slowCall));
        info.hotPathBegin = linkBuffer.locationOf(m_jsCalls[i].m_targetToCheck);
        info.hotPathOther = linkBuffer.locationOfNearCall(m_jsCalls[i].m_fastCall);
    }
    
    MacroAssemblerCodeRef osrExitThunk = globalData()->getCTIStub(osrExitGenerationThunkGenerator);
    CodeLocationLabel target = CodeLocationLabel(osrExitThunk.code());
    for (unsigned i = 0; i < codeBlock()->numberOfOSRExits(); ++i) {
        OSRExit& exit = codeBlock()->osrExit(i);
        linkBuffer.link(exit.m_check.lateJump(), target);
        exit.m_check.correctLateJump(linkBuffer);
    }
    
    codeBlock()->shrinkToFit(CodeBlock::LateShrink);
}

bool JITCompiler::compile(JITCode& entry)
{
    compileEntry();
    SpeculativeJIT speculative(*this);
    compileBody(speculative);

    // Create OSR entry trampolines if necessary.
    speculative.createOSREntries();

    LinkBuffer linkBuffer(*m_globalData, this, m_codeBlock, JITCompilationCanFail);
    if (linkBuffer.didFailToAllocate())
        return false;
    link(linkBuffer);
    speculative.linkOSREntries(linkBuffer);

    entry = JITCode(linkBuffer.finalizeCode(), JITCode::DFGJIT);
    return true;
}

bool JITCompiler::compileFunction(JITCode& entry, MacroAssemblerCodePtr& entryWithArityCheck)
{
    compileEntry();

    // === Function header code generation ===
    // This is the main entry point, without performing an arity check.
    // If we needed to perform an arity check we will already have moved the return address,
    // so enter after this.
    Label fromArityCheck(this);
    // Plant a check that sufficient space is available in the RegisterFile.
    // FIXME: https://bugs.webkit.org/show_bug.cgi?id=56291
    addPtr(TrustedImm32(m_codeBlock->m_numCalleeRegisters * sizeof(Register)), GPRInfo::callFrameRegister, GPRInfo::regT1);
    Jump registerFileCheck = branchPtr(Below, AbsoluteAddress(m_globalData->interpreter->registerFile().addressOfEnd()), GPRInfo::regT1);
    // Return here after register file check.
    Label fromRegisterFileCheck = label();


    // === Function body code generation ===
    SpeculativeJIT speculative(*this);
    compileBody(speculative);

    // === Function footer code generation ===
    //
    // Generate code to perform the slow register file check (if the fast one in
    // the function header fails), and generate the entry point with arity check.
    //
    // Generate the register file check; if the fast check in the function head fails,
    // we need to call out to a helper function to check whether more space is available.
    // FIXME: change this from a cti call to a DFG style operation (normal C calling conventions).
    registerFileCheck.link(this);
    move(stackPointerRegister, GPRInfo::argumentGPR0);
    poke(GPRInfo::callFrameRegister, OBJECT_OFFSETOF(struct JITStackFrame, callFrame) / sizeof(void*));

    CallBeginToken token = beginCall();
    Call callRegisterFileCheck = call();
    notifyCall(callRegisterFileCheck, CodeOrigin(0), token);
    jump(fromRegisterFileCheck);
    
    // The fast entry point into a function does not check the correct number of arguments
    // have been passed to the call (we only use the fast entry point where we can statically
    // determine the correct number of arguments have been passed, or have already checked).
    // In cases where an arity check is necessary, we enter here.
    // FIXME: change this from a cti call to a DFG style operation (normal C calling conventions).
    Label arityCheck = label();
    compileEntry();

    load32(AssemblyHelpers::payloadFor((VirtualRegister)RegisterFile::ArgumentCount), GPRInfo::regT1);
    branch32(AboveOrEqual, GPRInfo::regT1, TrustedImm32(m_codeBlock->numParameters())).linkTo(fromArityCheck, this);
    move(stackPointerRegister, GPRInfo::argumentGPR0);
    poke(GPRInfo::callFrameRegister, OBJECT_OFFSETOF(struct JITStackFrame, callFrame) / sizeof(void*));
    token = beginCall();
    Call callArityCheck = call();
    notifyCall(callArityCheck, CodeOrigin(0), token);
    move(GPRInfo::regT0, GPRInfo::callFrameRegister);
    jump(fromArityCheck);
    
    // Create OSR entry trampolines if necessary.
    speculative.createOSREntries();


    // === Link ===
    LinkBuffer linkBuffer(*m_globalData, this, m_codeBlock, JITCompilationCanFail);
    if (linkBuffer.didFailToAllocate())
        return false;
    link(linkBuffer);
    speculative.linkOSREntries(linkBuffer);
    
    // FIXME: switch the register file check & arity check over to DFGOpertaion style calls, not JIT stubs.
    linkBuffer.link(callRegisterFileCheck, cti_register_file_check);
    linkBuffer.link(callArityCheck, m_codeBlock->m_isConstructor ? cti_op_construct_arityCheck : cti_op_call_arityCheck);

    entryWithArityCheck = linkBuffer.locationOf(arityCheck);
    entry = JITCode(linkBuffer.finalizeCode(), JITCode::DFGJIT);
    return true;
}

} } // namespace JSC::DFG

#endif // ENABLE(DFG_JIT)
