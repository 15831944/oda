TEMPLATE = app

exists(../OdaQtPaths.pri): include(../OdaQtPaths.pri)

mac:!macx: CONFIG += iphone

android|iphone {

  TARGET = ODADrawingsExplorer
  DEFINES += TD_USE_QT_LIB
  # unfortunately Necessitas unsupports such things as $${TD_CONF_NAME}
  #LIBS += -L$${ODADIR}/lib/$${TD_CONF_NAME}
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
      contains(QT_MAJOR_VERSION, 5) {
         DEFINES -= QT_OPENGL_ES_2
         DEFINES += TD_OPENGL_ES
         DEFINES += ANDROID
     }
  } else {
    DESTDIR = $${ODADIR}/bin/$${TD_CONF_NAME}
    PRJ_DESTDIR = $${ODADIR}/bin/$${TD_CONF_NAME}/Examples/Qt/

    OBJECTS_DIR = $${PRJ_DESTDIR}build
    MOC_DIR = $${PRJ_DESTDIR}build
    UI_DIR = $${PRJ_DESTDIR}build
    RCC_DIR = $${PRJ_DESTDIR}build
    LIBS += -L$${ODADIR}/lib/$${TD_CONF_NAME}
  }


  QT += core gui declarative
  QT += opengl


} else {
  TARGET = OdaQtApp

  win32 {
    DESTDIR = $${ODADIR}/exe/$${TD_CONF_NAME}
    #PRJ_DESTDIR = $${ODADIR}/$$(QTMAKEPLATFORMS_FLD_NAME)/$$TD_CONF_NAME/ProjectFiles/Examples/Qt/
    PRJ_DESTDIR = ""

    #(see result at project property / C/C++ / Output Files / Program Database File Name):
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

  LIBS += -L$${ODADIR}/lib/$${TD_CONF_NAME}
  !win32:LIBS += -L$${ODADIR}/bin/$${TD_CONF_NAME}
  !win32:LIBS += -L$${ODADIR}/lib/$${TD_CONF_NAME}

}

CONFIG += qt
QT += xml

# Solution for segmentation fault at close application if any drawing was opened On Linux.
# (On Linux only : 
#    With first using of QGlWidget from OdqViewSystem txv invokes 
#    qAddPostRoutine(qt_gl_choose_cmapp) from QtOpenGL to clear this static
#    data by close application(= in destructor ofQApplication 
#    - see also qt_call_post_routines()). Therefore we should keep QtOpenGL
#    from application to prevent it unloading through OdqViewSystem.)
linux-*:QT += opengl

INCLUDEPATH += $${ODADIR}/Drawing/Examples/Qt/Common
INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Common
INCLUDEPATH += $${ODADIR}/Kernel/Include
INCLUDEPATH += $${ODADIR}/Drawing/Include
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Drawing/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExRender
INCLUDEPATH += $${ODADIR}/Drawing/Extensions/PlotSettingsValidator
INCLUDEPATH += $${ODADIR}/ThirdParty/activation

HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqInterfaces.h
HEADERS += $${ODADIR}/Kernel/Examples/Common/ExDynamicModule.h 
HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqAudit.h
HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqClipData.h

!exists( $${ODADIR}/Kernel/Include/ExAppServices.h )  {
  HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/ExAppServices.h
  DEFINES += ODA_EX_APP_SERVICES_UNLINKED_RXINIT
}
else {
  HEADERS += $${ODADIR}/Kernel/Include/ExAppServices.h
}

!exists( $${ODADIR}/Kernel/Include/PropServices.h )  {
  HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/PropServices.h
  DEFINES += ODA_PROP_SERVICES_UNLINKED_RXINIT
}
else {
  HEADERS += $${ODADIR}/Kernel/Include/PropServices.h
}

HEADERS += OdqApplication.h
HEADERS += OdqMainWindow.h
HEADERS += OdqCustomization.h
HEADERS += OdqAppProps.h
HEADERS += OdqPropertyDialog.h
HEADERS += OdqPropertyPalette.h
HEADERS += OdqPropertyPageDialog.h
HEADERS += OdqColorIndexDialog.h
android|iphone {
#Nothing to do
}else{
HEADERS += OdqPlotDialog.h
SOURCES += OdqPlotDialog.cpp
SOURCES += AppPlotCommands.cpp
}
HEADERS += OdqXmlUtils.h
HEADERS += Services.h
HEADERS += AppModule.h
HEADERS += AppCommands.h
HEADERS += ExAppServicesImpl.h
HEADERS += PropServicesImpl.h
HEADERS += ResBufPropValueBaseConvertors.h

SOURCES += main.cpp
SOURCES += OdqApplication.cpp
SOURCES += OdqMainWindow.cpp
SOURCES += OdqCustomization.cpp
SOURCES += OdqAppProps.cpp
SOURCES += OdqXmlUtils.cpp
SOURCES += OdqPropertyDialog.cpp
SOURCES += OdqPropertyPalette.cpp
SOURCES += OdqPropertyPageDialog.cpp
SOURCES += OdqColorIndexDialog.cpp
SOURCES += Services.cpp
SOURCES += AppModule.cpp
SOURCES += AppCommands.cpp 
SOURCES += ExAppServicesImpl.cpp 
SOURCES += PropServicesImpl.cpp

# Cryptographic - support for DWG with password
#win32:!android {
#  # via Window API
#  INCLUDEPATH += $${ODADIR}/Kernel/Extensions/win/Crypt
#  HEADERS += $${ODADIR}/Kernel/Extensions/win/Crypt/WinNTCrypt.h
#  SOURCES += $${ODADIR}/Kernel/Extensions/win/Crypt/WinNTCrypt.cpp
#}
#else {
#  #is not LGPL # See http://www.cryptlib.com for details.
#  # library is missing (should be downloaded from site by users
#  DEFINES += _USE_CRYPTLIB
#  INCLUDEPATH += $${ODADIR}/Kernel/Extensions/Crypt
#  HEADERS += $${ODADIR}/Kernel/Extensions/Crypt/CryptLibCrypt.h
#  SOURCES += $${ODADIR}/Kernel/Extensions/Crypt/CryptLibCrypt.cpp
#}

exists( $${ODADIR}/Caustic/OpenRL/Source/OpenRLKey/OpenRLKey.cpp )  {
  # BmpOpenRL : LIBS += -lTD_OpenRLKey :

  INCLUDEPATH += $${ODADIR}/Caustic/OpenRL/Source/OpenRLKey/include 
  INCLUDEPATH += "$${ODADIR}/Caustic/OpenRL/ThirdParty/OpenRL_SDK/Include"
  HEADERS += $${ODADIR}/Caustic/OpenRL/Source/OpenRLKey/Include/OpenRLKey.h
  HEADERS += $${ODADIR}/Caustic/OpenRL/Source/OpenRLKey/Include/OpenRLKeyPE.h 
  HEADERS += $${ODADIR}/Caustic/OpenRL/Source/OpenRLKey/OpenRLKeyPEImpl.h
  SOURCES += $${ODADIR}/Caustic/OpenRL/Source/OpenRLKey/OpenRLKey.cpp
  SOURCES += $${ODADIR}/Caustic/OpenRL/Source/OpenRLKey/OpenRLKeyPE.cpp

  #7051 - TD_OpenRLKey.lib requires linking with Gs
  #LIBS += -lTD_Gs
}
else {
  DEFINES += ODA_QT_NO_CAUSTIC_OPENRL
}

# QZipReader is private for v 4.5 - 4.6 # TODO use for menu command
exists( $${QTDIR}/src/gui/text/qzip.cpp )  {

  exists( $${QTDIR}/include/Qt/private/qzipreader_p.h ) {
    #DEFINES += Q_AUTOTEST_EXPORT
    #DEFINES -= QT_NO_TEXTODFWRITER
    HEADERS += $${QTDIR}/include/Qt/private/qzipreader_p.h
    HEADERS += $${QTDIR}/include/Qt/private/qzipwriter_p.h
    INCLUDEPATH += $${QTDIR}/src/3rdparty/zlib
    SOURCES += $${QTDIR}/src/gui/text/qzip.cpp
  } else {
    # since 4.7.1
    INCLUDEPATH += $${QTDIR}/src/gui/text
    HEADERS += $${QTDIR}/src/gui/text/qzipreader_p.h
    HEADERS += $${QTDIR}/src/gui/text/qzipwriter_p.h
  }
} 
else {
  DEFINES += ODA_QT_NO_QZIP
}

contains(QT_MAJOR_VERSION, 5): QT += widgets
contains(QT_MAJOR_VERSION, 5): QT += printsupport
win32:contains(QT_MAJOR_VERSION, 5): QT += core
contains(QT_MAJOR_VERSION, 5): DEFINES += QT5
win32:contains(QT_MAJOR_VERSION, 5): DEFINES += NOMINMAX

RESOURCES += data/data.qrc


#TODO for win
# Unfortunately I am not found how to set value $(TargetDir)/TDXdependency.manifest
# of VCPROJ property Manifest Tool/Input and Output/Addition Manifest Files
# via qmake. It is needed to field evaluation via OdaX.
win32:!android {
  RC_FILE = data/OdaQtApp.rc
  CONFIG -= embed_manifest_exe 
  CONFIG -= embed_manifest_dll
# problem with evaluation fields :
#  CONFIG += embed_manifest_exe
#  WINRT_MANIFEST = "$(TargetDir)/TDXdependency.manifest"
#  manifest_file.input = "$(TargetDir)/TDXdependency.manifest"
}
#macx {
#  dangeous with runtime error : mac you cannot open the application because it may be damaged or incomplete
#  QMAKE_INFO_PLIST = data/OdaQtApp.plist   # qmake will copy this file to OdaQtApp.app/Contents/Info.plist
#}

# ----- TD set -----

CONFIG(debug, debug|release): DEFINES += _DEBUG
DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += TD_CLIENT_BUILD

android|iphone {

  #SOURCES += mbwc_conv.cpp
  #LIBS += -lTD_Key

  iphone:DEFINES += OD_GLES2_IOS

  # very strange but unpredictable order is needed for android
  LIBS += -lOdaQtConsole
  LIBS += -lOdaQtViewSystem
  #LIBS += -lOdaQtOpenGL
  LIBS += -lOdaQtGLES2
  contains(QT_MAJOR_VERSION, 5): LIBS += -lTD_GLES2
  LIBS += -lTD_DgnDb
  LIBS += -lTG_Dgn7IO
  LIBS += -lTG_ModelerGeometry
  LIBS += -lPSToolkit
  LIBS += -lTG_Db

  LIBS += -lTD_ExamplesCommon
  LIBS += -lTD_DrawingsExamplesCommon
  LIBS += -lModelerGeometry
  LIBS += -lRecomputeDimBlock
  LIBS += -lExFieldEvaluator
  LIBS += -lTD_BrepRenderer
  LIBS += -lTD_Br
  LIBS += -lTD_AcisBuilder
  LIBS += -lTD_Db -lTD_DbCore -lTD_DbIO -lTD_DbEntities -lSCENEOE -lACCAMERA -lRText -lATEXT -lISM -lWipeOut
  LIBS += -lTD_DbRoot
  LIBS += -lTD_Gs
  LIBS += -lTD_Gi
  LIBS += -lTD_Ge
  LIBS += -lTD_SpatialIndex
  LIBS += -lTD_Root
  LIBS += -lTD_Alloc
  LIBS += -lFreeType
  LIBS += -ltinyxml
  LIBS += -loless

  android: {
    LIBS += -lTH_Wchar
    contains(QT_MAJOR_VERSION, 5) {
        LIBS += -lGLESv2
        LIBS += -lEGL
        LIBS += -landroid
    } else {
       LIBS += -lTD_OpenGL
       LIBS += -lGLESv1_CM
    }
  }
  mac {
    LIBS += -lc++
    LIBS += -lRasterProcessor
    LIBS += -lRxRasterServices
  }
} else {
  DEFINES += _TOOLKIT_IN_DLL_

  win32|linux {
    LIBS += -lTD_ExamplesCommon
    LIBS += -lTD_DrawingsExamplesCommon
  }	
  else { 
    #INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common

    SOURCES += $${ODADIR}/Kernel/Extensions/ExServices/OdFileBuf.cpp

    macx:DEFINES += ODA_LINKED_WITH_FRAMEWORK_FONDATION
    macx:LIBS += -framework Foundation

    SOURCES += $${ODADIR}/Drawing/Extensions/ExServices/ExHostAppServices.cpp
    SOURCES += $${ODADIR}/Drawing/Extensions/ExServices/ExUndoController.cpp
    SOURCES += $${ODADIR}/Drawing/Extensions/ExServices/ExPageController.cpp
    SOURCES += $${ODADIR}/Drawing/Extensions/ExServices/ExDbCommandContext.cpp
    SOURCES += $${ODADIR}/Kernel/Extensions/ExServices/ExStringIO.cpp

    SOURCES += $${ODADIR}/Kernel/Extensions/ExServices/ExKWIndex.cpp
    SOURCES += $${ODADIR}/Kernel/Extensions/ExServices/RxSystemServicesImpl.cpp

    # ExRender :
    SOURCES += $${ODADIR}/Kernel/Extensions/ExRender/BmpTilesGen.cpp
    # OdReadEx :
    SOURCES += $${ODADIR}/Drawing/Examples/OdReadEx/DbDumper.cpp
    SOURCES += $${ODADIR}/Drawing/Examples/OdReadEx/GiWorldDrawDumper.cpp
    SOURCES += $${ODADIR}/Drawing/Examples/OdReadEx/ExProtocolExtension.cpp
    # Common :
    SOURCES += $${ODADIR}/Drawing/Examples/Common/toString.cpp
  }
  SOURCES += $${ODADIR}/Kernel/Extensions/ExServices/ExSystemServices.cpp # was removed from common lib - see build folder

  #win32:LIBS += -ladavapi32 # suspected to be needed for QTCreator + msvc10 toolchain

  LIBS += -lTD_Alloc
  LIBS += -lTD_DbRoot
  LIBS += -lTD_Root
  LIBS += -lTD_Db -lTD_DbCore -lTD_DbIO -lTD_DbEntities -lSCENEOE -lACCAMERA -lRText -lATEXT -lISM -lWipeOut
  LIBS += -lTD_Ge
  LIBS += -lTD_Gi
  # critical for linux :
  LIBS += -lTD_SpatialIndex
  # for Red Hat linux with static TD (feedback from users) :
  linux-*:LIBS += -lTD_Gs
  LIBS += -lTD_Key
  #linux-*:LIBS += -lTD_FT
  win32:LIBS += -lcrypt32
}

#DEPENDPATH = $$INCLUDEPATH
#sources.files = $$SOURCES $$HEADERS *.pro
