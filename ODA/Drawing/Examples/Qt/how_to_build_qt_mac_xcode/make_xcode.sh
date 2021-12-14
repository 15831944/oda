#! /bin/sh

if test ! -n "${ODADIR}" ; then export ODADIR="/_Oda/trunk" ; fi
# useless for framework version #if test ! -n "${QTDIR}" ; then export QTDIR="/usr/local/Qt4.5" ; fi
# see Developer/Examples/Qt
#echo "      >sh make_xcode $1 $2"

#echo "      >qmake -spec macx-xcode -macx -o $2$1.xcodeproj $1.pro"
#echo "      >qmake -macx -o $2$1 $1.pro"

QMAKESPEC=macx-xcode
##### -universal -sdk /Developer/SDKs/MacOSX10.4u.sdk   or # QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
case "${CONF_NAME}" in
*10.4*) QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk ;;
*10.5*) QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk ;;
*10.6*) QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.6.sdk ;;
*10.7*) QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.7.sdk ;;
esac

qmake -o "$2$1" "$1".pro

#if test "$2" != "" && test -f Info.plist ; then mv -f Info.plist "$2Info.plist" ; fi

#set >test.txt
