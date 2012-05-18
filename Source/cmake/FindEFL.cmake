INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES (EDJE REQUIRED edje>=1.0.0)
PKG_CHECK_MODULES (ECORE REQUIRED ecore>=1.0.0)
PKG_CHECK_MODULES (ECORE_EVAS REQUIRED ecore-evas>=1.0.0)
PKG_CHECK_MODULES (EFLDEPS REQUIRED
  eina>=1.0.999.63568
  evas>=1.0.0
  ecore>=1.0.999.59763
  ecore-file>=1.0.0
  ecore-evas>=1.0.999.59763
  edje>=1.0.0
  eukit>=1.1.0
  edbus>=1.1.0)
PKG_CHECK_MODULES (EINA REQUIRED eina>=1.0.0)
PKG_CHECK_MODULES (ECORE_X ecore-x>=1.0.0)
PKG_CHECK_MODULES (EVAS REQUIRED evas>=1.0.0)
PKG_CHECK_MODULES (EUKIT REQUIRED eukit>=1.1.0)
PKG_CHECK_MODULES (EDBUS REQUIRED edbus>=1.1.0)

FIND_PROGRAM (EDJE_CC_EXECUTABLE edje_cc)
IF (NOT EDJE_CC_EXECUTABLE)
    MESSAGE (FATAL_ERROR "edje_cc could not be found")
ENDIF ()
