
android {
  DEFINES += ANDROID
  DEFINES += ANDROID_NDK_NATIVE
  DEFINES += ANDROID_WCHAR

  QMAKE_CXXFLAGS_EXCEPTIONS_ON = -fexceptions -frtti
  ##QMAKE_CXXFLAGS_EXCEPTIONS_ON = -fexceptions
  ##QMAKE_CXXFLAGS_RTTI_ON = -frtti

  CONFIG += exceptions_on
  ##CONFIG += rtti_on
  CONFIG += warn_off

  CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
    ##TD_CONF_NAME=%ODACFG%dbg  # does not work
    #TD_CONF_NAME=android_armeabi_r6bdbg
    #ODAFLD=trunk_android_r6b_dbg
  }
  else {
    #TD_CONF_NAME=android_armeabi_r6b
    #ODAFLD=trunk_android_r6b
  }
  #ODAROOT=E:/_Oda
  #ODAVERSION=3.6.0
  #ODADIR=$$(ODAROOT)/$$(ODAFLD)
  ##PRJ2SRC_LEV_OFFSET=../../
  ##PRJDEST=..\..\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
  #exists( ../../../../../_android_oda_qt.pri )  {
  #  #include(../../../../../_android_oda_qt.pri)
  #}

  win32:DEFINES -= win32
  win32:DEFINES -= WIN32
  #win32 { # test
  #  ??? error ??? # unfortunately it is so
  #}

}

include(./OdaQtApp.pri)
