
#########################################################################################################################
#                                         !!! ATTENTION !!!                                                             #
#                                This project is not ready                                                              #
#  It is deferred because of serious problem with correct initialization of OpenGLES1.x in current state of Necessitas. #
#  We are awaiting the solution of this problem in beta version of Necessitas to continue work to with this project.    #
#                                                                                                                       #
#########################################################################################################################

CONFIG += ordered
TEMPLATE = subdirs

#cross_compile: CONFIG += nostrip
#confclean.depends += clean
#CONFIG -= qt

#BUILD_PARTS = $$BUILD_PARTS OdaQtApp
#for(PROJECT, $$list($$lower($$unique(BUILD_PARTS)))) {
#  message(priject : $$PROJECT)
#  isEqual(PROJECT, OdaQtApp) { ... }
#}

SUBDIRS += OdaQtConsole
SUBDIRS += OdaQtViewSystem
SUBDIRS += OdaQtOpenGL
#SUBDIRS += OdaQtPropSystem
#SUBDIRS += OdaQtPlotSettingsValidator
#SUBDIRS += OdaQtObjects
SUBDIRS += OdaQtApp

#INSTALLS += ODADrawingsExplorer

# TODO (does not work in a3)
CONFIG(debug, debug|release) {
  #DISTFILES += ../../../bin/android_armeabi_r6bdbg/adinit.dat
  contains(QT_MAJOR_VERSION, 5) {
    TeighaDataFiles.sources = ../../../bin/android_armeabi_r9ddbg/adinit.dat
  } else {
    TeighaDataFiles.sources = ../../../bin/android_armeabi_r6bdbg/adinit.dat
  }
}
else {
  #DISTFILES += ../../../bin/android_armeabi_r6b/adinit.dat
  contains(QT_MAJOR_VERSION, 5) {
    TeighaDataFiles.sources = ../../../bin/android_armeabi_r9d/adinit.dat
  } else {
    TeighaDataFiles.sources = ../../../bin/android_armeabi_r6b/adinit.dat
  }
}
DEPLOYMENT += TeighaDataFiles
#myFiles.sources = path\file1.ext1 path2\file2.ext1 path3\*
#myFiles.path = \some\path\on\device
#someother.sources = C:\additional\files\*
#someother.path = \myFiles\path2
#DEPLOYMENT += myFiles someother

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java

