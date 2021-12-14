CONFIG -= debug_and_release # useless for us # TODO unfortunately "-= debug" is not working 
CONFIG += release
#CONFIG += debug

CONFIG += console
QT = core # script ##CONFIG += qt

TARGET = qsed
TEMPLATE = app

DESTDIR = ../bin

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

#HEADERS += 
SOURCES += main.cpp


#DEFINES += UNICODE

#DEPENDPATH = $$INCLUDEPATH
#sources.files = $$SOURCES $$HEADERS *.pro

