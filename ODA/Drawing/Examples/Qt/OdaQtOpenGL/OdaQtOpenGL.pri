#TARGET = OdaQtOpenGL$${ODADLLSUFFIX}
TARGET = OdaQtOpenGL
TEMPLATE = lib

exists(../OdaQtPaths.pri): include(../OdaQtPaths.pri)

#mac:!macx: CONFIG += iphone
#android|iphone {
#  CONFIG += staticlib
#  QT += core gui declarative
#  contains(QT_MAJOR_VERSION, 5) {
#    DEFINES += QT_OPENGL_ES_2
#    DEFINES += TD_OPENGL_ES
#    LIBS += -lGLESv2
#    LIBS += -lEGL
#    LIBS += -landroid
#  } else {
#    DEFINES += QT_OPENGL_ES_1
#    DEFINES += TD_OPENGL_ES
#    #LIBS += -lGLESv1_CM
#  }
#} else{

  win32:TARGET_EXT = $${ODADLLSUFFIX}.txv
  unix:!mac {
    # unixmake2.cpp: } else if(project->isEmpty("QMAKE_HPUX_SHLIB")) {

    CONFIG += plugin no_plugin_name_prefix
    QMAKE_EXTENSION_PLUGIN = txv
  }
  macx {
    CONFIG += plugin no_plugin_name_prefix
    QMAKE_EXTENSION_PLUGIN = txv
  }

  win32 {
    DESTDIR = $${ODADIR}/exe/$${TD_CONF_NAME}
    PRJ_DESTDIR = ""

    # (see result at project property / C/C++ / Output Files / Program Database File Name)
    TMPVAR = $$(QMAKESPEC)
    !isEqual(TMPVAR, ""): QMAKE_CXXFLAGS += /Fd$(IntDir)/

    OBJECTS_DIR = $${PRJ_DESTDIR}build/$$TARGET 
    MOC_DIR = $${PRJ_DESTDIR}build/$$TARGET
    UI_DIR = $${PRJ_DESTDIR}build/$$TARGET
    RCC_DIR = $${PRJ_DESTDIR}build/$$TARGET
  } 
  else {
    DESTDIR = $${ODADIR}/bin/$${TD_CONF_NAME}
    PRJ_DESTDIR = $${ODADIR}/bin/$${TD_CONF_NAME}/Examples/Qt/

    OBJECTS_DIR = $${PRJ_DESTDIR}build
    MOC_DIR = $${PRJ_DESTDIR}build
    UI_DIR = $${PRJ_DESTDIR}build
    RCC_DIR = $${PRJ_DESTDIR}build
  }

  win32:LIBS += -L$${ODADIR}/lib/$${TD_CONF_NAME}
  !win32:LIBS += -L$${ODADIR}/bin/$${TD_CONF_NAME}
  !win32:LIBS += -L$${ODADIR}/lib/$${TD_CONF_NAME}
#}

CONFIG += qt
QT += opengl

INCLUDEPATH += $${ODADIR}/Drawing/Examples/Qt/Common
INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Common
INCLUDEPATH += $${ODADIR}/Kernel/Include
INCLUDEPATH += $${ODADIR}/Drawing/Include
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL

#android|iphone:contains(QT_MAJOR_VERSION, 5) {
## WinGLES2
#INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/sc
#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/Qt/ExGsGLES2LocalContext.cpp
#!android:win32: SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/compatibility/gles2_ext.cpp
#!android:win32: HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/compatibility/gles2_ext.h
#
#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/ExGsGLES2LocalRendition.cpp
#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/ExGsGLES2LocalSharingProvider.cpp
#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/ExGsGLES2MetafileReader.cpp
#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/ExGsGLES2ShadersRuntime.cpp
#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/GLES2Module.cpp
#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/sc/ExGLES2ScShadersCollection.cpp
#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/sc/ExGLES2ScShadGen.cpp
#SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/sc/GeneratedGLES2ShadDefs.cpp
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/ExGsGLES2LocalContext.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/ExGsGLES2LocalGLDefs.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/ExGsGLES2LocalRendition.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/ExGsGLES2LocalSharingProvider.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/ExGsGLES2MetafileReader.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/ExGsGLES2ShadersRuntime.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/local/GLES2Include.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/sc/ExGLES2ScAccumulator.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/sc/ExGLES2ScDefs.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/sc/ExGLES2ScGenForParser.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/sc/ExGLES2ScOptions.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/sc/ExGLES2ScShadersCollection.h
#HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/GLES2/sc/GeneratedGLES2ShadDefs.h
#
#} else {
#OdaQtOpenGL

HEADERS += ExGsQtVectorizeDevice.h # instead ExGsOpenGLVectorizeDevice.h
HEADERS += ExGsQtDevice.h # instead ExGsWin32Device.h
HEADERS += ExGsQtScreenDevice.h 
 
SOURCES += ExGsQtVectorizeDevice.cpp # instead ExGsOpenGLVectorizeDevice.cpp
SOURCES += ExGsQtDevice.cpp # instead ExGsWin32DeviceOpenGL.cpp
SOURCES += OpenGLModule.cpp # instead win/WinOpenGL.cpp

HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/GsOpenGLStreamVectorizer.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/GsOpenGLVectorizer.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/ExOpenGLMetafileReader.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/ExOpenGLMetafileStream.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/ExOpenGLMetafileWriter.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/OpenGLMetafileStream.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/CommonDeviceProps.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/GsOpenGLDefs.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/ExGsHelpers.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/MetafileTransformStack.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/mono_dxt1_compressor.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/TtfFontsCache.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/OpenGL_ES.h
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/OpenGLExtensions.h

SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/GsOpenGLVectorizer.cpp
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/CommonDeviceProps.cpp
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/ExOpenGLMetafileReader.cpp
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/ExOpenGLMetafileStream.cpp
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/ExOpenGLMetafileWriter.cpp
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/OpenGL/GsOpenGLStreamVectorizer.cpp
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/mono_dxt1_compressor.cpp
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/TtfFontsCache.cpp
#}

#RESOURCES += data/data.qrc
win32:!android {
  RC_FILE = data/OdqOpenGL.rc
  CONFIG -= embed_manifest_dll #CONFIG -= embed_manifest_exe 
}

contains(QT_MAJOR_VERSION, 5): QT += widgets
contains(QT_MAJOR_VERSION, 5): QT += core
contains(QT_MAJOR_VERSION, 5): DEFINES += QT5
win32:contains(QT_MAJOR_VERSION, 5): DEFINES += NOMINMAX

# ----- TD set -----

CONFIG(debug, debug|release): DEFINES += _DEBUG
DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += TD_CLIENT_BUILD
#android:DEFINES += OD_ANDROID_TRACEDEFS
android:DEFINES += ANDROID
DEFINES += TD_USE_QT_LIB

#android|iphone {
#    LIBS += -lTD_Alloc
#    LIBS += -lTD_Ge
#    LIBS += -lTD_Gi
#    LIBS += -lTD_Gs
#
#    LIBS += -lTD_DbRoot
#    LIBS += -lTD_Root
#  contains(QT_MAJOR_VERSION, 5){
#    LIBS += -lTD_GLES2
#    DEFINES += TD_USE_QT_LIB
#  } else {
#    LIBS += -lTD_OpenGL
#  }
#} else {
  DEFINES += _TOOLKIT_IN_DLL_

  LIBS += -lTD_Alloc
  LIBS += -lTD_Ge
  LIBS += -lTD_Gi
  LIBS += -lTD_Gs

  LIBS += -lTD_DbRoot
  LIBS += -lTD_Root

  !win32: LIBS += -lTD_Gi
  macx: LIBS += -lTD_SpatialIndex
  
  win32: isEqual(CONVERTEDBY, "") {
    LIBS += -lgdi32 -lopengl32 # -luser32 -lkernel32 -lgdi32 ...
  }
#}

#DEPENDPATH = $$INCLUDEPATH
#sources.files = $$SOURCES $$HEADERS *.pro
