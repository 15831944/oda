
#TARGET = OdaQtConsole$${ODADLLSUFFIX}
TARGET = OdaQtConsole
TEMPLATE = lib

exists(../OdaQtPaths.pri): include(../OdaQtPaths.pri)

mac:!macx: CONFIG += iphone

android|iphone{
  android {
    CONFIG(debug, debug|release) {
        contains(QT_MAJOR_VERSION, 5) {
            LIBS += -L../../../../lib/android_armeabi_r9d
        } else {
            LIBS += -L../../../../lib/android_armeabi_r6bdbg
        }
    } else {
        contains(QT_MAJOR_VERSION, 5) {
            LIBS += -L../../../../lib/android_armeabi_r9d
        } else {
            LIBS += -L../../../../lib/android_armeabi_r6b
        }
    }
    INCLUDEPATH += ../../../../ThirdParty/wchar
  }else {
    DESTDIR = $${ODADIR}/lib/$${TD_CONF_NAME}
    PRJ_DESTDIR = $${ODADIR}/bin/$${TD_CONF_NAME}/Examples/Qt/

    OBJECTS_DIR = $${PRJ_DESTDIR}build
    MOC_DIR = $${PRJ_DESTDIR}build
    UI_DIR = $${PRJ_DESTDIR}build
    RCC_DIR = $${PRJ_DESTDIR}build

    LIBS += -L../../../../lib/$${TD_CONF_NAME}
  }
  CONFIG += staticlib
} else {
#win32|linux|macx

  win32:TARGET_EXT = $${ODADLLSUFFIX}.dll
  unix:!mac:!android {
    CONFIG += plugin # no_plugin_name_prefix
    QMAKE_EXTENSION_PLUGIN = so  
  }

  win32 {
    DESTDIR = $${ODADIR}/exe/$${TD_CONF_NAME}
    #PRJ_DESTDIR = $${ODADIR}/$${QTMAKEPLATFORMS_FLD_NAME}/$$TD_CONF_NAME/ProjectFiles/Examples/Qt/
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
    # test
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
  !win32:LIBS += -L$${ODADIR}/lib/$${TD_CONF_NAME}
}

INCLUDEPATH += $${ODADIR}/Drawing/Examples/Qt/Common
INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Common
INCLUDEPATH += $${ODADIR}/Kernel/Include
INCLUDEPATH += $${ODADIR}/Drawing/Include
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Drawing/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Editor
INCLUDEPATH += $${ODADIR}/Drawing/Extensions/DbConstraints

HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqInterfaces.h
HEADERS += $${ODADIR}/Kernel/Examples/Common/ExDynamicModule.h 
HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqAudit.h
HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqTdSaveAsFilters.h
HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqClipData.h

!exists( $${ODADIR}/Kernel/Include/SysVarPE.h )  {
  HEADERS += $${ODADIR}/Drawing/Examples/Common/SysVarPE.h
  DEFINES += ODA_SYS_VAR_PE_UNLINKED_RXINIT
}
else {
  HEADERS += $${ODADIR}/Kernel/Include/SysVarPE.h
}
!exists( $${ODADIR}/Kernel/Include/PropServices.h )  {
  HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/PropServices.h
  DEFINES += ODA_PROP_SERVICES_UNLINKED_RXINIT
}
else {
  HEADERS += $${ODADIR}/Kernel/Include/PropServices.h
}
!exists( $${ODADIR}/Kernel/Include/ExAppServices.h )  {
  HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/ExAppServices.h
  DEFINES += ODA_EX_APP_SERVICES_UNLINKED_RXINIT
}
else {
  HEADERS += $${ODADIR}/Kernel/Include/ExAppServices.h
}

HEADERS += ConsoleModule.h
HEADERS += ConsoleCommands.h
HEADERS += ConsoleReactors.h
HEADERS += CommandContextImpl.h
HEADERS += OdqConsole.h
HEADERS += OdqConsoleTab.h
HEADERS += OdqCommandLineEdit.h
HEADERS += SysVarPEImpl.h
HEADERS += OdqSysVarDefs.h
HEADERS += OdqGripManager.h
HEADERS += OdqSnapManager.h
HEADERS += OdqDragTracker.h
HEADERS += $${ODADIR}/Drawing/Examples/Editor/ExGripManager.h 
HEADERS += $${ODADIR}/Drawing/Examples/Editor/OSnapManager.h
HEADERS += $${ODADIR}/Drawing/Extensions/DbConstraints/DbAssocManager.h

SOURCES += ConsoleModule.cpp
#macx:DEFINES += ODA_LINKED_WITH_FRAMEWORK_FONDATION
#macx:LIBS += -framework Foundation
SOURCES += ConsoleCommands.cpp
SOURCES += ConsoleReactors.cpp
SOURCES += CommandContextImpl.cpp
SOURCES += OdqConsole.cpp 
SOURCES += OdqConsoleTab.cpp 
SOURCES += OdqCommandLineEdit.cpp
SOURCES += OdqAudit.cpp
SOURCES += SysVarPEImpl.cpp
SOURCES += OdqGripManager.cpp
SOURCES += OdqSnapManager.cpp
SOURCES += OdqDragTracker.cpp
SOURCES += $${ODADIR}/Drawing/Examples/Editor/ExGripManager.cpp
SOURCES += $${ODADIR}/Drawing/Examples/Editor/OSnapManager.cpp

#useLocalDiesel = false
#versionOda=3.4.1
#versionOda=$${ODAVERSION}
##win32|macx:isEqual( versionOda, "3.4.1" ): useLocalDiesel = true
##macx: useLocalDiesel = true
#macx:isEqual( versionOda, "3.4.1" ): useLocalDiesel = true
#!exists( $${ODADIR}/Kernel/Include/DbDiesel.h  ): useLocalDiesel = true
#
#isEqual( useLocalDiesel, true) {
#  DEFINES += ODA_QT_DIESEL_LOCAL_COPY
#  INCLUDEPATH += ./Extensions
#
#  HEADERS += Extensions/DbDiesel.h
#  SOURCES += Extensions/DbDiesel.cpp
#}

#RESOURCES += data/data.qrc
android|iphone {
#Nothing to do
}
else {
  SOURCES += ConsoleAppLoadCommand.cpp
  win32 {
    RC_FILE = data/OdqConsole.rc
    CONFIG -= embed_manifest_dll #CONFIG -= embed_manifest_exe 
  }
  CONFIG += qt
}
contains(QT_MAJOR_VERSION, 5): QT += widgets
contains(QT_MAJOR_VERSION, 5): DEFINES += QT5
win32:contains(QT_MAJOR_VERSION, 5): DEFINES += NOMINMAX

# ----- TD set -----

!win32: CONFIG(debug, debug|release): DEFINES += _DEBUG
DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += TD_CLIENT_BUILD
android|iphone{
  LIBS += -lTD_ExamplesCommon
  LIBS += -lTD_DrawingsExamplesCommon
} else {
  DEFINES += _TOOLKIT_IN_DLL_
  LIBS += -lTD_ExamplesCommon
  LIBS += -lTD_DrawingsExamplesCommon

  !win32:LIBS += -lTD_Key # TODO strange but we have requirement after building via CMAKE

  LIBS += -lTD_Alloc
  LIBS += -lTD_DbRoot
  LIBS += -lTD_Root
  LIBS += -lTD_Db -lTD_DbCore -lTD_DbIO -lTD_DbEntities -lSCENEOE -lACCAMERA -lRText -lATEXT -lISM -lWipeOut
  LIBS += -lDbConstraints
  LIBS += -lTD_Ge

  !win32: LIBS += -lTD_Gi
  macx: LIBS += -lTD_SpatialIndex
  
  win32: isEqual(CONVERTEDBY, "") {
     LIBS += -luser32 # -lkernel32 -lgdi32 ...
  }
}
#DEPENDPATH = $$INCLUDEPATH
#sources.files = $$SOURCES $$HEADERS *.pro
