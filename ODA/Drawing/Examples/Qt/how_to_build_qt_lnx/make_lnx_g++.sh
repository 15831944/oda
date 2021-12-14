#! /bin/sh

if test ! -n "${ODADIR}" ; then export ODADIR="/_Oda/trunk" ; fi

QMAKESPEC=linux-g++
#QMAKESPEC=linux-g++-32
#QMAKESPEC=linux-g++-64 
qmake -o "$2Makefile" "$1".pro

#set >test.txt
