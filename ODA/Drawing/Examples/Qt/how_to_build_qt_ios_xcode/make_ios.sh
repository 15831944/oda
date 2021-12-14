#!/bin/sh

export TD_CONF_NAME=$1
export TD_SDK_NAME=$2

if [ "$TD_CONF_NAME" = "" ] || [ "$TD_SDK_NAME" = "" ] ; then
echo "$0 <platform> <iOS SDK name>"
echo ""
echo "Available SDKs : "
echo "`ls /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/`"
echo "`ls /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/`"
exit 0
fi

if [ "$ODADIR" = "" ] ;  then
echo "Please set ODADIR environmnet variable: Path to the ODA SDK archive directory"
echo "Example: export ODADIR=/Users/user/main/trunk"
exit 0
fi

if [ "$QTDIR" = "" ] ;  then
echo "Please set QTDIR environmnet variable: Path to Qt libs/bins for the specific platform"
echo "Example: export QTDIR=/Users/user/Qt/5.3/ios"
exit 0
fi

TD_ARCH="-arch armv7 -arch armv7s -arch arm64"
case "${TD_CONF_NAME}" in
*sim*) TD_ARCH="-arch i386 -arch x86_64" ;;
esac

suffix=Release
case "${TD_CONF_NAME}" in
*dbg*) suffix="Debug" ;;
esac

case "${TD_CONF_NAME}" in
*sim*) QMAKE_IPHONE_SDK=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/${TD_SDK_NAME};;
*iphone*) QMAKE_IPHONE_SDK=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/${TD_SDK_NAME} ;;
esac

#Create OdaFileConverter example
make_oda_app_project() {

  echo `pwd`
  pushd ../"$1"
  ${QTDIR}/bin/qmake -o "$2".xcodeproj "$1".pro
  cp "../how_to_build_qt_ios_xcode/data/$2.plist" Info.plist
  xcodebuild -project "$2".xcodeproj -configuration Release -sdk ${QMAKE_IPHONE_SDK}
  pushd ../how_to_build_qt_ios_xcode
}



#Create libraries
make_oda_lib_project() {

  echo `pwd`
  pushd ../"$1"
  ${QTDIR}/bin/qmake -spec macx-xcode  "$1".pro
  xcodebuild -project "$1".xcodeproj -configuration ${suffix} ${TD_ARCH} -sdk ${QMAKE_IPHONE_SDK}
  case "${TD_CONF_NAME}" in
    *sim*) mv Release-iphonesimulator/*.a ${ODADIR}/lib/${TD_CONF_NAME}/  ;;
    *iphone*) mv Release-iphoneos/*.a ${ODADIR}/lib/${TD_CONF_NAME}/  ;;
  esac

  pushd ../how_to_build_qt_ios_xcode
}

if [ ! -f "$ODADIR/bin/$TD_CONF_NAME/adinit.dat" ] ; then
if [ -f "$ODADIR/Kernel/adinit.dat" ] ; then
cp -f "$ODADIR/Kernel/adinit.dat" "$ODADIR/bin/$TD_CONF_NAME/adinit.dat"
fi
fi



make_oda_app_project OdaFileConverter ODAFileConverter
#make_oda_lib_project OdaQtConsole
#make_oda_lib_project OdaQtViewSystem
#make_oda_lib_project OdaQtOpenGL
make_oda_app_project OdaQtApp ODADrawingsExplorer
