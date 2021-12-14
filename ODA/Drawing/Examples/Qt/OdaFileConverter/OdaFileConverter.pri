TARGET = OdaFileConverter
TEMPLATE = app

exists(../OdaQtPaths.pri): include(../OdaQtPaths.pri)

mac:!macx: CONFIG += iphone

android|iphone {
  # unfortunately Necessitas unsupports such things as $${TD_CONF_NAME}
  #LIBS += -L$${ODADIR}/lib/$${TD_CONF_NAME}
  android {
    CONFIG(debug, debug|release) {
      contains(QT_MAJOR_VERSION, 5) {
        LIBS += -L../../../../lib/android_armeabi_r9d
      } else {
        LIBS += -L../../../../lib/android_armeabi_r6bdbg
      }
    }
    else {
      contains(QT_MAJOR_VERSION, 5) {
        LIBS += -L../../../../lib/android_armeabi_r9d
      } else {
        LIBS += -L../../../../lib/android_armeabi_r6b
      }
    }
    INCLUDEPATH += ../../../../ThirdParty/wchar
  }else {
    LIBS += -L../../../../lib/$${TD_CONF_NAME}
  }
} else {
# win32|linux|macx
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
}

INCLUDEPATH += $${ODADIR}/Drawing/Examples/Qt/Common
INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common
INCLUDEPATH += $${ODADIR}/Drawing/Examples/Common
INCLUDEPATH += $${ODADIR}/Kernel/Include
INCLUDEPATH += $${ODADIR}/Drawing/Include
INCLUDEPATH += $${ODADIR}/Kernel/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/Drawing/Extensions/ExServices
INCLUDEPATH += $${ODADIR}/ThirdParty/activation

HEADERS += $${ODADIR}/Drawing/Examples/Qt/Common/OdqTdSaveAsFilters.h

HEADERS += widget.h
HEADERS += convertor.h

SOURCES += main.cpp
SOURCES += widget.cpp
SOURCES += convertor.cpp

RESOURCES += data/data.qrc

android|iphone {
  QT += declarative
}else{
  win32 {
    RC_FILE = data/OdaFileConverter.rc
    CONFIG -= embed_manifest_exe 
	CONFIG -= embed_manifest_dll
  }
  #else:macx {
  #  dangeous with runtime error : mac you cannot open the application because it may be damaged or incomplete
  #  QMAKE_INFO_PLIST = data/OdaFileConverter.plist   # qmake will copy this file to OdaFileConverter.app/Contents/Info.plist
  #}

  CONFIG += qt
}

contains(QT_MAJOR_VERSION, 5): QT += widgets
contains(QT_MAJOR_VERSION, 5): DEFINES += QT5

# ----- TD set -----

CONFIG(debug, debug|release): DEFINES += _DEBUG
DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += TD_CLIENT_BUILD


android|iphone {
  android {
    !contains(QT_MAJOR_VERSION, 5) {
      SOURCES += mbwc_conv.cpp
    }
  }
  #LIBS += -lTD_Key

  # very strange but unpredictable order is needed for android

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
  android:LIBS += -lTH_Wchar
  mac:LIBS += -lc++
} else {
# win32:!android|linux|macx
  DEFINES += _TOOLKIT_IN_DLL_

  win32 {
    LIBS += -lTD_ExamplesCommon
    LIBS += -lTD_DrawingsExamplesCommon
  }	
  else { 
    #INCLUDEPATH += $${ODADIR}/Kernel/Examples/Common

    SOURCES += $${ODADIR}/Kernel/Extensions/ExServices/OdFileBuf.cpp
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
