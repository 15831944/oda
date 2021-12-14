TARGET = TD_TigDb
TEMPLATE = lib

exists(../OdaQtPaths.pri): include(../OdaQtPaths.pri)

!android {
  win32:TARGET_EXT = $${ODADLLSUFFIX}.tx
  unix:!mac {
    # unixmake2.cpp: } else if(project->isEmpty("QMAKE_HPUX_SHLIB")) {

    CONFIG += plugin no_plugin_name_prefix
    QMAKE_EXTENSION_PLUGIN = tx
  }
  macx {
    CONFIG += plugin no_plugin_name_prefix
    QMAKE_EXTENSION_PLUGIN = tx
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

} else {
  CONFIG += staticlib
}

CONFIG += qt

INCLUDEPATH += $${ODADIR}/Drawing/Examples/Qt/Common
INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common
#INCLUDEPATH += $${ODADIR}/Drawing/Examples/Common
INCLUDEPATH += $${ODADIR}/Kernel/Include
INCLUDEPATH += $${ODADIR}/Drawing/Include
INCLUDEPATH += $${ODADIR}/Drawing/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Kernel

HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqInterfaces.h
HEADERS += $${ODADIR}/Kernel/Examples/Common/ExtDbModule.h
HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/ExtDbModuleBaseImpl.h
HEADERS += $${ODADIR}/Drawing/Extensions/ExServices/ExDbCommandContext.h
HEADERS += TigDbModuleImpl.h
HEADERS += OdqRevSystem.h
HEADERS += OdqRevTab.h
HEADERS += OdqBranchTree.h
HEADERS += OdqRevisionPainter.h
HEADERS += RevisionPainter.h
HEADERS += TigDbCommands.h
HEADERS += RevisionController.h
HEADERS += OdqRevDiffHelper.h

SOURCES += $${ODADIR}/Kernel/Extensions/ExServices/ExKWIndex.cpp
SOURCES += $${ODADIR}/Kernel/Extensions/ExServices/OdFileBuf.cpp
SOURCES += $${ODADIR}/Drawing/Extensions/ExServices/ExDbCommandContext.cpp
SOURCES += TigDbModule.cpp
SOURCES += OdqRevSystem.cpp
SOURCES += OdqRevTab.cpp
SOURCES += OdqBranchTree.cpp
SOURCES += OdqRevisionPainter.cpp
SOURCES += RevisionPainter.cpp
SOURCES += TigDbCommands.cpp
SOURCES += RevisionController.cpp
SOURCES += OdqRevDiffHelper.cpp

win32:!android {
  RC_FILE = data/TigDb.rc
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
android:DEFINES += ANDROID
DEFINES += TD_USE_QT_LIB

!android {
  DEFINES += _TOOLKIT_IN_DLL_

  LIBS += -lTD_Alloc
  LIBS += -lTD_Ge
  LIBS += -lTD_Gi
  LIBS += -lTD_Gs

  LIBS += -lTD_Tf
  LIBS += -lTD_Db -lTD_DbCore -lTD_DbIO -lTD_DbEntities -lSCENEOE -lACCAMERA -lRText -lATEXT -lISM -lWipeOut
  LIBS += -lTD_DbRoot
  LIBS += -lTD_Root
}

#DEPENDPATH = $$INCLUDEPATH
#sources.files = $$SOURCES $$HEADERS *.pro
