
android {
  DEFINES += ANDROID
  DEFINES += ANDROID_NDK_NATIVE
  DEFINES += ANDROID_WCHAR

  QMAKE_CXXFLAGS_EXCEPTIONS_ON = -fexceptions -frtti

  CONFIG += exceptions_on
  CONFIG += warn_off
}

include(./OdaQtGles.pri)
