/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "JSTestActiveDOMObject.h"

#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "JSNode.h"
#include "TestActiveDOMObject.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSTestActiveDOMObject);
/* Hash table */

static const HashTableValue JSTestActiveDOMObjectTableValues[] =
{
    { "excitingAttr", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestActiveDOMObjectExcitingAttr), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestActiveDOMObjectConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSTestActiveDOMObjectTable = { 4, 3, JSTestActiveDOMObjectTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSTestActiveDOMObjectConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSTestActiveDOMObjectConstructorTable = { 1, 0, JSTestActiveDOMObjectConstructorTableValues, 0 };
const ClassInfo JSTestActiveDOMObjectConstructor::s_info = { "TestActiveDOMObjectConstructor", &Base::s_info, &JSTestActiveDOMObjectConstructorTable, 0, CREATE_METHOD_TABLE(JSTestActiveDOMObjectConstructor) };

JSTestActiveDOMObjectConstructor::JSTestActiveDOMObjectConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSTestActiveDOMObjectConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSTestActiveDOMObjectPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSTestActiveDOMObjectConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSTestActiveDOMObjectConstructor, JSDOMWrapper>(exec, &JSTestActiveDOMObjectConstructorTable, jsCast<JSTestActiveDOMObjectConstructor*>(cell), propertyName, slot);
}

bool JSTestActiveDOMObjectConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSTestActiveDOMObjectConstructor, JSDOMWrapper>(exec, &JSTestActiveDOMObjectConstructorTable, jsCast<JSTestActiveDOMObjectConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSTestActiveDOMObjectPrototypeTableValues[] =
{
    { "excitingFunction", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsTestActiveDOMObjectPrototypeFunctionExcitingFunction), (intptr_t)1, NoIntrinsic },
    { "postMessage", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsTestActiveDOMObjectPrototypeFunctionPostMessage), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSTestActiveDOMObjectPrototypeTable = { 4, 3, JSTestActiveDOMObjectPrototypeTableValues, 0 };
const ClassInfo JSTestActiveDOMObjectPrototype::s_info = { "TestActiveDOMObjectPrototype", &Base::s_info, &JSTestActiveDOMObjectPrototypeTable, 0, CREATE_METHOD_TABLE(JSTestActiveDOMObjectPrototype) };

JSObject* JSTestActiveDOMObjectPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTestActiveDOMObject>(exec, globalObject);
}

bool JSTestActiveDOMObjectPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSTestActiveDOMObjectPrototype* thisObject = jsCast<JSTestActiveDOMObjectPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSTestActiveDOMObjectPrototypeTable, thisObject, propertyName, slot);
}

bool JSTestActiveDOMObjectPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSTestActiveDOMObjectPrototype* thisObject = jsCast<JSTestActiveDOMObjectPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSTestActiveDOMObjectPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSTestActiveDOMObject::s_info = { "TestActiveDOMObject", &Base::s_info, &JSTestActiveDOMObjectTable, 0 , CREATE_METHOD_TABLE(JSTestActiveDOMObject) };

JSTestActiveDOMObject::JSTestActiveDOMObject(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<TestActiveDOMObject> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSTestActiveDOMObject::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSTestActiveDOMObject::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSTestActiveDOMObjectPrototype::create(exec->globalData(), globalObject, JSTestActiveDOMObjectPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSTestActiveDOMObject::destroy(JSC::JSCell* cell)
{
    JSTestActiveDOMObject* thisObject = jsCast<JSTestActiveDOMObject*>(cell);
    thisObject->JSTestActiveDOMObject::~JSTestActiveDOMObject();
}

JSTestActiveDOMObject::~JSTestActiveDOMObject()
{
    releaseImplIfNotNull();
}

bool JSTestActiveDOMObject::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSTestActiveDOMObject* thisObject = jsCast<JSTestActiveDOMObject*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSTestActiveDOMObject, Base>(exec, &JSTestActiveDOMObjectTable, thisObject, propertyName, slot);
}

bool JSTestActiveDOMObject::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSTestActiveDOMObject* thisObject = jsCast<JSTestActiveDOMObject*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (!shouldAllowAccessToFrame(exec, thisObject->impl()->frame()))
        return false;
    return getStaticValueDescriptor<JSTestActiveDOMObject, Base>(exec, &JSTestActiveDOMObjectTable, thisObject, propertyName, descriptor);
}

JSValue jsTestActiveDOMObjectExcitingAttr(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSTestActiveDOMObject* castedThis = jsCast<JSTestActiveDOMObject*>(asObject(slotBase));
    if (!castedThis->allowsAccessFrom(exec))
        return jsUndefined();
    UNUSED_PARAM(exec);
    TestActiveDOMObject* impl = static_cast<TestActiveDOMObject*>(castedThis->impl());
    JSValue result = jsNumber(impl->excitingAttr());
    return result;
}


JSValue jsTestActiveDOMObjectConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSTestActiveDOMObject* domObject = jsCast<JSTestActiveDOMObject*>(asObject(slotBase));
    if (!domObject->allowsAccessFrom(exec))
        return jsUndefined();
    return JSTestActiveDOMObject::getConstructor(exec, domObject->globalObject());
}

JSValue JSTestActiveDOMObject::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestActiveDOMObjectConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsTestActiveDOMObjectPrototypeFunctionExcitingFunction(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestActiveDOMObject::s_info))
        return throwVMTypeError(exec);
    JSTestActiveDOMObject* castedThis = jsCast<JSTestActiveDOMObject*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestActiveDOMObject::s_info);
    if (!castedThis->allowsAccessFrom(exec))
        return JSValue::encode(jsUndefined());
    TestActiveDOMObject* impl = static_cast<TestActiveDOMObject*>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    Node* nextChild(toNode(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->excitingFunction(nextChild);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsTestActiveDOMObjectPrototypeFunctionPostMessage(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestActiveDOMObject::s_info))
        return throwVMTypeError(exec);
    JSTestActiveDOMObject* castedThis = jsCast<JSTestActiveDOMObject*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestActiveDOMObject::s_info);
    TestActiveDOMObject* impl = static_cast<TestActiveDOMObject*>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    const String& message(ustringToString(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? UString() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->postMessage(message);
    return JSValue::encode(jsUndefined());
}

static inline bool isObservable(JSTestActiveDOMObject* jsTestActiveDOMObject)
{
    if (jsTestActiveDOMObject->hasCustomProperties())
        return true;
    return false;
}

bool JSTestActiveDOMObjectOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSTestActiveDOMObject* jsTestActiveDOMObject = jsCast<JSTestActiveDOMObject*>(handle.get().asCell());
    if (!isObservable(jsTestActiveDOMObject))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}

void JSTestActiveDOMObjectOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSTestActiveDOMObject* jsTestActiveDOMObject = jsCast<JSTestActiveDOMObject*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsTestActiveDOMObject->impl(), jsTestActiveDOMObject);
    jsTestActiveDOMObject->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TestActiveDOMObject* impl)
{
    return wrap<JSTestActiveDOMObject>(exec, globalObject, impl);
}

TestActiveDOMObject* toTestActiveDOMObject(JSC::JSValue value)
{
    return value.inherits(&JSTestActiveDOMObject::s_info) ? jsCast<JSTestActiveDOMObject*>(asObject(value))->impl() : 0;
}

}
