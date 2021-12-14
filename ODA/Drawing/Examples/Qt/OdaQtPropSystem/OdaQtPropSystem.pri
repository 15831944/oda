#TARGET = OdaQtPropSystem$${ODADLLSUFFIX}
TARGET = OdaQtPropSystem
TEMPLATE = lib

exists(../OdaQtPaths.pri): include(../OdaQtPaths.pri)

!android {

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
} else {
  CONFIG += staticlib 
}

INCLUDEPATH += $${ODADIR}/Drawing/Examples/Qt/Common
INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Common
INCLUDEPATH += $${ODADIR}/Kernel/Include
INCLUDEPATH += $${ODADIR}/Drawing/Include
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Drawing/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Drawing/Extensions/DbConstraints
INCLUDEPATH += $${ODADIR}/Kernel/DevInclude/DbRoot

HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqInterfaces.h
HEADERS += $${ODADIR}/Kernel/Examples/Common/ExDynamicModule.h
HEADERS += $${ODADIR}/Drawing/Extensions/DbConstraints/DbAssocManager.h

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
!exists( $${ODADIR}/Kernel/Include/SysVarPE.h )  {
  HEADERS += $${ODADIR}/Drawing/Examples/Common/SysVarPE.h
  DEFINES += ODA_SYS_VAR_PE_UNLINKED_RXINIT
}
else {
  HEADERS += $${ODADIR}/Kernel/Include/SysVarPE.h
}

HEADERS += PropSystemModule.h
HEADERS += PropSystemCommands.h
HEADERS += OdqPropSystem.h
HEADERS += OdqPropTab.h
HEADERS += PropSourcesImpl.h
HEADERS += DbXmlPropSources.h
HEADERS += ResBufPropValueConvertors.h

SOURCES += PropSystemModule.cpp
SOURCES += PropSystemCommands.cpp
SOURCES += OdqPropSystem.cpp
SOURCES += OdqPropTab.cpp
SOURCES += PropSourcesImpl.cpp
SOURCES += DbXmlPropSources.cpp
SOURCES += ResBufPropValueConvertors.cpp

#RESOURCES += data/data.qrc
!android {
  win32 {
    RC_FILE = data/OdqPropSystem.rc
    CONFIG -= embed_manifest_dll #CONFIG -= embed_manifest_exe 
  }
  CONFIG += qt
}
contains(QT_MAJOR_VERSION, 5): QT += widgets
contains(QT_MAJOR_VERSION, 5): DEFINES += QT5
win32:contains(QT_MAJOR_VERSION, 5): DEFINES += NOMINMAX

# ----- TD set -----

CONFIG(debug, debug|release): DEFINES += _DEBUG
DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += TD_CLIENT_BUILD
!android: {
  DEFINES += _TOOLKIT_IN_DLL_

  LIBS += -lTD_Alloc
  LIBS += -lTD_DbRoot
  LIBS += -lTD_Root
  LIBS += -lTD_Db -lTD_DbCore -lTD_DbIO -lTD_DbEntities -lSCENEOE -lACCAMERA -lRText -lATEXT -lISM -lWipeOut
  LIBS += -lAcModelDocObj -lDbConstraints
  LIBS += -lTD_Ge
}

#DEPENDPATH = $$INCLUDEPATH
#sources.files = $$SOURCES $$HEADERS *.pro
