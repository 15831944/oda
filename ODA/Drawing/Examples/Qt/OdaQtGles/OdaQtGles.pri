#TARGET = OdaQtGles$${ODADLLSUFFIX}
#TARGET = OdaQtGles
TARGET = OdaQtGLES2
TEMPLATE = lib

exists(../OdaQtPaths.pri): include(../OdaQtPaths.pri)

mac:!macx: CONFIG += iphone

android|iphone {
  CONFIG += staticlib
  QT += core gui declarative

  contains(QT_MAJOR_VERSION, 5) {
    DEFINES += QT_OPENGL_ES_2
    DEFINES += TD_OPENGL_ES
    LIBS += -lGLESv2
    LIBS += -lEGL
    LIBS += -landroid
  } else {
    DEFINES += QT_OPENGL_ES_1
    DEFINES += TD_OPENGL_ES
  }
} else{

  win32:TARGET_EXT = $${ODADLLSUFFIX}.txv
  unix:!mac {
    CONFIG += plugin no_plugin_name_prefix
    QMAKE_EXTENSION_PLUGIN = txv
  }
  macx {
    CONFIG += plugin no_plugin_name_prefix
    QMAKE_EXTENSION_PLUGIN = txv
    DEFINES += TD_OPEN_GLES_MISSING
  }

  win32 {

    DESTDIR = $${ODADIR}/exe/$${TD_CONF_NAME}
    PRJ_DESTDIR = ""

    TMPVAR = $$(QMAKESPEC)
    !isEqual(TMPVAR, ""): QMAKE_CXXFLAGS += /Fd$(IntDir)/

    OBJECTS_DIR = $${PRJ_DESTDIR}build/$$TARGET 
    MOC_DIR = $${PRJ_DESTDIR}build/$$TARGET
    UI_DIR = $${PRJ_DESTDIR}build/$$TARGET
    RCC_DIR = $${PRJ_DESTDIR}build/$$TARGET

  } else {

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

  DEFINES += OD_TRVEC_OLE
}
CONFIG += qt
QT += opengl
#QT += "opengl es2"
contains(QT_MAJOR_VERSION, 5) {
  DEFINES += QT_OPENGL_ES_2
#  #DEFINES += TD_OPENGL_ES
#  LIBS += -lGLESv2
#   LIBS += -lEGL
}
else {
 QT += "opengl es2"
}

INCLUDEPATH += $${ODADIR}/Drawing/Examples/Qt/Common
INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Common
INCLUDEPATH += $${ODADIR}/Kernel/Include
INCLUDEPATH += $${ODADIR}/Drawing/Include
INCLUDEPATH += $${ODADIR}/Kernel/Include/Tr
INCLUDEPATH += $${ODADIR}/Kernel/Include/Tr/vec
INCLUDEPATH += $${ODADIR}/Kernel/Include/Tr/render
INCLUDEPATH += $${ODADIR}/Kernel/Include/Tr/render/gl2
INCLUDEPATH += $${ODADIR}/Kernel/Include/Tr/render/gl2/vec
INCLUDEPATH += $${ODADIR}/Kernel/Include/Tr/render/gl2/compatibility
INCLUDEPATH += $${ODADIR}/Kernel/Include/Tr/extbar
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender/TrLocalRender

!android:!iphone {
  HEADERS += $${ODADIR}/Kernel/Include/Tr/render/gl2/compatibility/gles2_ext.h
  SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/TrGL2/compatibility/gles2_ext.cpp
}
unix:!mac: HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/TrGL2/X11/ExGsXWindowConnection.h
mac: SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/TrGL2/Mac/CocoaOGLOffscreen.mm
mac: SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/TrGL2/Mac/CocoaOGLOnscreen.mm

#HEADERS += $${ODADIR}/Kernel/Source/Tr/sc/TrGL2ScAccumulator.h
#HEADERS += $${ODADIR}/Kernel/Source/Tr/sc/TrGL2ScDefs.h
#HEADERS += $${ODADIR}/Kernel/Source/Tr/sc/TrGL2ScGenForParser.h
#HEADERS += $${ODADIR}/Kernel/Source/Tr/sc/TrGL2ScOptions.h
#HEADERS += $${ODADIR}/Kernel/Source/Tr/sc/TrGL2ScShadersCollection.h
#HEADERS += $${ODADIR}/Kernel/Source/Tr/sc/GeneratedTrGL2ShadDefs.h

#SOURCES += $${ODADIR}/Kernel/Source/Tr/sc/TrGL2ScShadersCollection.cpp
#SOURCES += $${ODADIR}/Kernel/Source/Tr/sc/TrGL2ScShadGen.cpp
#SOURCES += $${ODADIR}/Kernel/Source/Tr/sc/GeneratedTrGL2ShadDefs.cpp

#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrRndDifferentialTransition.cpp
HEADERS += $${ODADIR}/Kernel/Include/Tr/render/TrRndDifferentialTransition.h
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2ExtensionsRegistry.cpp
HEADERS += $${ODADIR}/Kernel/Include/Tr/render/gl2/TrGL2Extensions.h
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2FrameBuffers.cpp
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2FrameBuffers.h
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/TrRndIntersections.cpp
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/TrRndIntersections.h
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2Lineweights.cpp
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/TrGL2LocalMetafileBuilderWCS.cpp
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/TrGL2LocalMetafileBuilderImpl.h 
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/TrGL2LocalMetafileBuilderWCS.h
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2LocalRendition.cpp
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2LocalRendition.h
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/TrRndMetafilePlayer.cpp
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/TrRndMetafilePlayer.h
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2MetafileReader.cpp
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2MetafileReader.h
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2PredefinedTextures.cpp
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/TrRndPrerenderModifiers.cpp
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/TrRndPrerenderModifiers.h
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/TrRndRectsMerger.h
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/TrRndRenderSettings.cpp

HEADERS += $${ODADIR}/Kernel/Include/Tr/render/TrRndRenderSettings.h

#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2ShadersRuntime.cpp
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2ShadersRuntime.h
#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2ShaderState.cpp
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/gl2/TrGL2ShaderState.h

#SOURCES += $${ODADIR}/Kernel/Source/Tr/render/TrRndVisualStyleManager.cpp
#HEADERS += $${ODADIR}/Kernel/Source/Tr/render/TrRndVisualStyleManager.h

HEADERS += $${ODADIR}/Kernel/Include/Tr/render/gl2/GLES2Include.h

HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/TrLocalRender/TrVecLocalRenditionGsClient.h
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/TrLocalRender/TrVecLocalRenditionGsClient.cpp
HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/TrLocalRender/TrVecLocalSharingProvider.h
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/TrLocalRender/TrVecLocalSharingProvider.cpp
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/WinGLES2/GLES2Module.cpp

HEADERS += $${ODADIR}/Kernel/Include/Tr/render/gl2/TrGL2LocalContext.h
SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/TrGL2/Qt/ExGsGLES2LocalContext.cpp

win32:!android:!iphone {
  HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/WinGLES2/win/ExGsGLES2RSTestDialog.h
  SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/WinGLES2/win/ExGsGLES2RSTestDialog.cpp
  HEADERS += $${ODADIR}/Kernel/Extensions/ExRender/WinGLES2/win/ExtWinRes.h
  INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender/WinGLES2/win
  RC_FILE = $${ODADIR}/Kernel/Extensions/ExRender/WinGLES2/win/WinGLES2.rc
  CONFIG -= embed_manifest_dll
  LIBS += -lTrExtBar
}
#win32:!android
#{
#  RC_FILE = data/OdqGles.rc
#  CONFIG -= embed_manifest_dll
#}

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

LIBS += -lTrBase
LIBS += -lTrVec
LIBS += -lTrGL2
LIBS += -lTrRenderBase
LIBS += -lTD_Gs
LIBS += -lTD_Gi
LIBS += -lTD_SpatialIndex
LIBS += -lTD_Ge
LIBS += -lTD_DbRoot
LIBS += -lTD_Root
LIBS += -lTD_Alloc

win32: isEqual(CONVERTEDBY, "") {
  LIBS += -luser32 -lkernel32 -lgdi32 -lopengl32 -lcomdlg32 # -luser32 -lkernel32 -lgdi32 ...
}

DEFINES += TD_USE_QT_LIB

!android:!iphone:DEFINES += _TOOLKIT_IN_DLL_
