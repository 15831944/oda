#TARGET = OdaQtViewSystem$${ODADLLSUFFIX}
TARGET = OdaQtViewSystem
TEMPLATE = lib

exists(../OdaQtPaths.pri): include(../OdaQtPaths.pri)

mac:!macx: CONFIG += iphone

android|iphone {
  CONFIG += staticlib
  QT += core gui declarative

  mac {
    DESTDIR = $${ODADIR}/lib/$${TD_CONF_NAME}
    PRJ_DESTDIR = $${ODADIR}/bin/$${TD_CONF_NAME}/Examples/Qt/
    OBJECTS_DIR = $${PRJ_DESTDIR}build
    MOC_DIR = $${PRJ_DESTDIR}build
    UI_DIR = $${PRJ_DESTDIR}build
    RCC_DIR = $${PRJ_DESTDIR}build
  }

  contains(QT_MAJOR_VERSION, 5) {
    DEFINES -= QT_OPENGL_ES_2
  } else {
    DEFINES += QT_OPENGL_ES_1
  }
  contains(QT_MAJOR_VERSION, 5): DEFINES += TD_OPENGL_ES
  #LIBS += -lGLESv1_CM
}else {
#win32|linux|macx
  win32:TARGET_EXT = $${ODADLLSUFFIX}.dll
  unix:!mac {
    CONFIG += plugin # no_plugin_name_prefix
    QMAKE_EXTENSION_PLUGIN = so  
  }

  win32 {
    DESTDIR = $${ODADIR}/exe/$${TD_CONF_NAME}
    #PRJ_DESTDIR = $${ODADIR}/$$(QTMAKEPLATFORMS_FLD_NAME)/$$TD_CONF_NAME/ProjectFiles/Examples/Qt/
    PRJ_DESTDIR = ""

    # (see result at project property / C/C++ / Output Files / Program Database File Name)
    TMPVAR = $$(QMAKESPEC)
    !isEqual(TMPVAR, ""): QMAKE_CXXFLAGS += /Fd$(IntDir)/
    #QMAKE_CXXFLAGS_DEBUG and QMAKE_CXXFLAGS_RELEASE 

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

  #macx {
  ##  QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Drx/"
  ##  QMAKE_LFLAGS_SONAME = -Wl,-install_name$${LITERAL_WHITESPACE}@executable_path/../Drx/
  #  LITERAL__WHITESPACE = " "
  #  QMAKE_LFLAGS_SONAME = -Wl,-soname,-install_name$$(LITERAL__WHITESPACE)@executable_path/../Drx/##
  #}

  win32:LIBS += -L$${ODADIR}/lib/$${TD_CONF_NAME}
  !win32:LIBS += -L$${ODADIR}/bin/$${TD_CONF_NAME}

}
CONFIG += qt
QT += opengl

contains(QT_MAJOR_VERSION, 5): QT += widgets
contains(QT_MAJOR_VERSION, 5): DEFINES += QT5
win32:contains(QT_MAJOR_VERSION, 5): DEFINES += NOMINMAX

INCLUDEPATH += $${ODADIR}/Drawing/Examples/Qt/Common
INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Common
INCLUDEPATH += $${ODADIR}/Kernel/Include
INCLUDEPATH += $${ODADIR}/Drawing/Include
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Drawing/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Editor

HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqInterfaces.h
HEADERS += $${ODADIR}/Kernel/Examples/Common/ExDynamicModule.h 

HEADERS += ViewSystemModule.h
HEADERS += ViewSystemCommands.h
HEADERS += OdqViewSystem.h
HEADERS += OdqView.h
contains(QT_MAJOR_VERSION, 5) {
  HEADERS += OdqOpenGlRenderArea.h
}
HEADERS += $${ODADIR}/Drawing/Examples/Editor/ExGripManager.h 
#HEADERS += $${ODADIR}/Drawing/Examples/Editor/OSnapManager.h

SOURCES += ViewSystemModule.cpp
SOURCES += ViewSystemCommands.cpp
SOURCES += OdqViewSystem.cpp
SOURCES += OdqView.cpp
SOURCES += $${ODADIR}/Drawing/Examples/Editor/ExGripManager.cpp
#SOURCES += $${ODADIR}/Drawing/Examples/Editor/OSnapManager.cpp

#RESOURCES += data/data.qrc
win32:!android {
  RC_FILE = data/OdqViewSystem.rc
  CONFIG -= embed_manifest_dll #CONFIG -= embed_manifest_exe 
}

# ----- TD set -----

CONFIG(debug, debug|release): DEFINES += _DEBUG
DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += TD_CLIENT_BUILD
android|iphone {
#Nothing to do
} else {
  DEFINES += _TOOLKIT_IN_DLL_

  LIBS += -lTD_Alloc
  LIBS += -lTD_DbRoot
  LIBS += -lTD_Root
  LIBS += -lTD_Db -lTD_DbCore -lTD_DbIO -lTD_DbEntities -lSCENEOE -lACCAMERA -lRText -lATEXT -lISM -lWipeOut
  LIBS += -lTD_Ge

  !win32: LIBS += -lTD_Gi
  win32: LIBS += -lOpengl32
  macx: LIBS += -lTD_SpatialIndex
  
  win32: isEqual(CONVERTEDBY, "") {
     LIBS += -luser32 # -lkernel32 -lgdi32 ...
  }
}

#DEPENDPATH = $$INCLUDEPATH
#sources.files = $$SOURCES $$HEADERS *.pro
