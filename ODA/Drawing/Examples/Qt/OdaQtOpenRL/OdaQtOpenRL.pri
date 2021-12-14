TEMPLATE = lib

exists(../OdaQtPaths.pri): include(../OdaQtPaths.pri)

#TARGET = OdaQtOpenRL$${ODADLLSUFFIX}
TARGET = OdaQtOpenRL

win32:TARGET_EXT = $${ODADLLSUFFIX}.txv
unix:!mac {
  CONFIG += plugin no_plugin_name_prefix
  QMAKE_EXTENSION_PLUGIN = txv
}
macx {
  # via adv params of xcodebuild call :
  CONFIG += plugin no_plugin_name_prefix
  QMAKE_EXTENSION_PLUGIN = txv
}

win32 {
  DESTDIR = $${ODADIR}/exe/$${TD_CONF_NAME}
  #PRJ_DESTDIR = $${ODADIR}/$$(QTMAKEPLATFORMS_FLD_NAME)/$$TD_CONF_NAME/ProjectFiles/Examples/Qt/
  PRJ_DESTDIR = ""

  # (see result at project property / C/C++ / Output Files / Program Database File Name)
  TMPVAR = $$(QMAKESPEC)
  !isEqual(TMPVAR, ""): QMAKE_CXXFLAGS += /Fd$(IntDir)/

  OBJECTS_DIR = $${PRJ_DESTDIR}build/$$TARGET 
  MOC_DIR = $${PRJ_DESTDIR}build/$$TARGET
  UI_DIR = $${PRJ_DESTDIR}build/$$TARGET
  RCC_DIR = $${PRJ_DESTDIR}build/$$TARGET

} else {

  #DESTDIR = $${ODADIR}/bin/$${TD_CONF_NAME}
  DESTDIR = $${ODADIR}/bin/$${TD_CONF_NAME}

  #PRJ_DESTDIR = $${ODADIR}/bin/$${TD_CONF_NAME}/Examples/Qt/
  #PRJ_DESTDIR = $${PRJDEST}/
  PRJ_DESTDIR = $${ODADIR}/bin/$${TD_CONF_NAME}/Examples/Qt/

  OBJECTS_DIR = $${PRJ_DESTDIR}build
  MOC_DIR = $${PRJ_DESTDIR}build
  UI_DIR = $${PRJ_DESTDIR}build
  RCC_DIR = $${PRJ_DESTDIR}build
}


win32:LIBS += -L$${ODADIR}/lib/$${TD_CONF_NAME}
!win32:LIBS += -L$${ODADIR}/bin/$${TD_CONF_NAME}

# -----

# BmpOpenRL :
#win32:LIBS += "-L$${ODADIR}/Caustic/OpenRL/ThirdParty/OpenRL_SDK/Lib/Win32"

INCLUDEPATH += $${ODADIR}/Drawing/Examples/Qt/Common
INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Common
INCLUDEPATH += $${ODADIR}/Kernel/Include
INCLUDEPATH += $${ODADIR}/Drawing/Include
#INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender
#INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL

# BmpOpenRL :
INCLUDEPATH += $${ODADIR}/Caustic/OpenRL/ThirdParty/OpenRL_SDK/Include

# -----

# BmpOpenRL :
SOURCES += $${ODADIR}/Caustic/OpenRL/Source/GsOpenRLDevice.cpp
SOURCES += $${ODADIR}/Caustic/OpenRL/Source/RLBumpMapper.cpp
SOURCES += $${ODADIR}/Caustic/OpenRL/Source/RLImageBkgnd.cpp
SOURCES += $${ODADIR}/Caustic/OpenRL/Source/RLNormalMap.cpp
SOURCES += $${ODADIR}/Caustic/OpenRL/Source/RLNormalMapScale.cpp
SOURCES += $${ODADIR}/Caustic/OpenRL/Source/RLRasterImage.cpp
SOURCES += $${ODADIR}/Caustic/OpenRL/Source/RLShaderCollection.cpp
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/CommonDeviceProps.cpp   # Caustic/OpenRL/Source/CommonDevicePropsOpenRL.cpp

# try to use both :
INCLUDEPATH += $${ODADIR}/Caustic/OpenRL/Source/bmp
HEADERS += $${ODADIR}/Caustic/OpenRL/Source/bmp/ExGsBmpOpenRLDevice.h
SOURCES += $${ODADIR}/Caustic/OpenRL/Source/bmp/ExGsBmpOpenRLDevice.cpp
# :
INCLUDEPATH += $${ODADIR}/Caustic/OpenRL/Source/Qt
HEADERS += $${ODADIR}/Caustic/OpenRL/Source/Qt/ExGsQtOpenRLDevice.h
SOURCES += $${ODADIR}/Caustic/OpenRL/Source/Qt/ExGsQtOpenRLDevice.cpp

#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/win/ExGsWin32Device.cpp # Caustic/OpenRL/Source/bmp/ExGsWin32DeviceOpenRL.cpp
#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/win/ExGsScreenDevice.cpp
# :
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Qt/OdaQtOpenGL
HEADERS += $${ODADIR}/Drawing/Examples/Qt/OdaQtOpenGL/ExGsQtDevice.h
HEADERS += $${ODADIR}/Drawing/Examples/Qt/OdaQtOpenGL/ExGsQtScreenDevice.h
SOURCES += $${ODADIR}/Drawing/Examples/Qt/OdaQtOpenGL/ExGsQtDevice.cpp

#SOURCES += $${ODADIR}/Caustic/OpenRL/Source/bmp/BmpOpenRLModule.cpp
# :
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender # for CommonDeviceProps.h
SOURCES += $${ODADIR}/Caustic/OpenRL/Source/Qt/QtOpenRLModule.cpp 

# was moved to OdaQtApp
## BmpOpenRL : LIBS += -lTD_OpenRLKey :
#INCLUDEPATH += $${ODADIR}/Caustic/OpenRL/Source/OpenRLKey/Include
#SOURCES += $${ODADIR}/Caustic/OpenRL/Source/OpenRLKey/OpenRLKey.cpp
#SOURCES += $${ODADIR}/Caustic/OpenRL/Source/OpenRLKey/OpenRLKeyPE.cpp


#RESOURCES += data/data.qrc
win32 {
  RC_FILE = data/OdqOpenRL.rc
  CONFIG -= embed_manifest_dll #CONFIG -= embed_manifest_exe 
}

# ----- Qt set -----

CONFIG += qt
QT += opengl

contains(QT_MAJOR_VERSION, 5): QT += widgets
contains(QT_MAJOR_VERSION, 5): DEFINES += QT5

# ----- TD set -----

CONFIG(debug, debug|release): DEFINES += _DEBUG
DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += _TOOLKIT_IN_DLL_
DEFINES += TD_CLIENT_BUILD

# -----

LIBS += -lTD_Alloc
LIBS += -lTD_Ge
LIBS += -lTD_Gi
LIBS += -lTD_Gs

LIBS += -lTD_DbRoot
LIBS += -lTD_Root

# BmpOpenRL :
win32:LIBS += -lOpenRL -lVersion

macx {
  LIBS += -framework OpenRL
  LIBS += -framework CausticGLUT
}

!win32 {
  LIBS += -lTD_Gi

  macx {
    LIBS += -lTD_SpatialIndex
  }
}

# -----

#DEPENDPATH = $$INCLUDEPATH
#sources.files = $$SOURCES $$HEADERS *.pro
