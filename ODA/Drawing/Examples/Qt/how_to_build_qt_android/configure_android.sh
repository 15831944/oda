#!/bin/sh
#export ODADIR=
#export QTDIR=
#Path to android sdk
#export ANDROID_SDK_ROOT=
#inspite of the name path to android ndk
#export ANDROID_SDK_HOME=
#export ANT_HOME=


print_usage()
{
	echo "Usage: "
	echo "$0 <android_configuration_name>"
	exit 0
}

build_project ()
{
#Build and create android apk
# #1 Project file name
# #2 Project folder name
# #3 Product file name
# #4 Release or debug version
pushd ${ODADIR}/Drawing/Examples/Qt

# Run qmake
${QTDIR}/${ANDROID_ARCH}/bin/qmake.exe $1.pro -r -spec android-g++ 
#"CONFIG+=%QT_RELEASE_DEBUG%"

# Run make
${QTDIR}/../Tools/mingw48_32/bin/mingw32-make.exe

# Go to directory with build results
pushd ${ODADIR}/Drawing/Examples/Qt/$2

# Install built library
${QTDIR}/../Tools/mingw48_32/bin/mingw32-make.exe INSTALL_ROOT=android-build install 

#copy libraries:
cp ../OdaQtViewSystem/libOdaQtViewSystem.a  ${ODADIR}/lib/${ODACFG}
cp ../OdaQtConsole/libOdaQtConsole.a   ${ODADIR}/lib/${ODACFG}
cp ../OdaQtOpenGL/libOdaQtOpenGL.a  ${ODADIR}/lib/${ODACFG}

# Create apk file
# Path to ant should be set --ant D:\MinGW\msys\1.0\opt\apache-ant-1.9.1\bin\ant.bat

${QTDIR}/${ANDROID_ARCH}/bin/androiddeployqt --input android-lib$3.so-deployment-settings.json --output android-build --ant ${ANT_HOME}/bin/ant

cp android-build/bin/QtApp-debug.apk ${ODADIR}/bin/${ODACFG}/$3.apk
rm -rf android-build
rm *.obj

popd
popd
}


if [[ $# -lt 1 ]]
then
  print_usage
fi

# Parameter %1 is the android platform configuration
ODACFG=$1

if [[ $ODACFG == *v7* ]]
then
  ANDROID_ARCH=android_armv7
else
  ANDROID_ARCH=android_armv5
fi

echo $ANDROID_ARCH

# ODADIR shoud be set to as env variable 

if [[ -z "$ODADIR" ]]
then
  echo "Please set up ODADIR (path to main source directory) env variable to continue building. Exiting..."
  exit 0
fi
echo "ODADIR = $ODADIR"

if [[ -z "$QTDIR" ]]
then
  echo "Please set up QTDIR (path to Qt libraries) env variable to continue building. Exiting..."
  exit 0
fi
echo "QTDIR = $QTDIR"

if [[ -z "$ANDROID_SDK_ROOT" ]]
then
  echo "Please set up ANDROID_SDK_ROOT env variable to continue building. Exiting..."
  exit 0
fi
echo "ANDROID_SDK_ROOT = $ANDROID_SDK_ROOT"

if [[ -z "$ANDROID_SDK_HOME" ]]
then
  echo "Please set up ANDROID_SDK_HOME (inspite of the name it's a path to android NDK) env variable to continue building. Exiting..."
  exit 0
fi
echo "ANDROID_SDK_HOME = $ANDROID_SDK_HOME"

QT_RELEASE_DEBUG=release
if [[ $ODACFG == *dbg* ]]
then
  QT_RELEASE_DEBUG=debug
fi

build_project OdaFileConverterAndroid OdaFileConverter ODAFileConverter ${QT_RELEASE_DEBUG}
build_project OdaFileViewerAndroid OdaQtApp ODADrawingsExplorer ${QT_RELEASE_DEBUG}






