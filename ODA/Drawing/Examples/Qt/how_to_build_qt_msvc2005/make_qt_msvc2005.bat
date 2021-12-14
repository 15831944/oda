:
: QT vcproj 2005 generation (v4.5.3)
:

:@echo off

if "%ODADIR%"=="" set ODADIR=\_Oda\trunk
if "%QTDIR%"=="" set QTDIR=C:\Qt\2010.05\qt

: QtMaind.lib QtCored4.dll QtGuid4.dll QtOpenGLd4.dll QtScriptd4.dll QtXmld4.dll QtNetworkd4.dll 
: QtMain.lib QtCore4.dll QtGui4.dll QtOpenGL4.dll QtScript4.dll QtXml4.dll QtNetwork4.dll 

: infortunately DESTDIR is defined inside native pro-files like %QTDIR%\lib

xcopy /S /Y make_vcproj.bat %QTDIR%\src\corelib\*.* >nul:
pushd %QTDIR%\src\corelib
call make_vcproj corelib
del make_vcproj.bat
popd 

xcopy /S /Y make_vcproj.bat %QTDIR%\src\gui\*.* >nul:
pushd %QTDIR%\src\gui
call make_vcproj gui
del make_vcproj.bat
popd 

xcopy /S /Y make_vcproj.bat %QTDIR%\src\opengl\*.* >nul:
pushd %QTDIR%\src\opengl
call make_vcproj opengl
del make_vcproj.bat
popd 

xcopy /S /Y make_vcproj.bat %QTDIR%\src\script\*.* >nul:
pushd %QTDIR%\src\script
call make_vcproj script
del make_vcproj.bat
popd 

xcopy /S /Y make_vcproj.bat %QTDIR%\src\xml\*.* >nul:
pushd %QTDIR%\src\xml
call make_vcproj xml
del make_vcproj.bat
popd 

: TODO openssl is needed to compile it
: Win32 OpenSSL http://www.slproweb.com/products/Win32OpenSSL.html
: see also :
: http://code.google.com/p/psi-dev/wiki/msvc2008s
: http://code.google.com/p/psi-dev/wiki/msvc2005
: 
xcopy /S /Y make_vcproj.bat %QTDIR%\src\network\*.* >nul:
pushd %QTDIR%\src\network
call make_vcproj network
del make_vcproj.bat
popd 

xcopy /S /Y make_vcproj.bat %QTDIR%\src\winmain\*.* >nul:
pushd %QTDIR%\src\winmain
call make_vcproj winmain
del make_vcproj.bat
popd 
