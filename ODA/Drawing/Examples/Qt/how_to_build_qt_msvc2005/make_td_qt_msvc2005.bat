:
: QT examples vcproj 2005 generation
:

:@echo off

if "%ODADIR%"=="" set ODADIR=\_Oda\trunk
if "%ODADLLSUFFIX%"=="" set ODADLLSUFFIX=_3.05src_8
if "%QTDIR%"=="" set QTDIR=C:\Qt\2010.05\qt
if "%QTMAKEPLATFORMS_FLD_NAME%"=="" set QTMAKEPLATFORMS_FLD_NAME=CMakePlatforms\QtPlatforms

set DEST=%ODADIR%\Drawing\Examples\Qt

if not "%ODACFG%"=="" goto end_def_ODACFG
if exist %ODADIR%\exe\vc8dlldbg\OdaMfcApp.exe set ODACFG=vc8dll
if exist %ODADIR%\exe\vc9dlldbg\OdaMfcApp.exe set ODACFG=vc9dll
if exist %ODADIR%\exe\vc10dlldbg\OdaMfcApp.exe set ODACFG=vc10dll
if exist %ODADIR%\exe\vc12dlldbg\OdaMfcApp.exe set ODACFG=vc12dll
if exist %ODADIR%\exe\vc8dll\OdaMfcApp.exe set ODACFG=vc8dll
if exist %ODADIR%\exe\vc9dll\OdaMfcApp.exe set ODACFG=vc9dll
if exist %ODADIR%\exe\vc10dll\OdaMfcApp.exe set ODACFG=vc10dll
if exist %ODADIR%\exe\vc12dll\OdaMfcApp.exe set ODACFG=vc12dll
:end_def_ODACFG

:--------------------

xcopy /S /Y make_vcproj_app.bat %DEST%\OdaFileConverter\*.* >nul:
pushd %DEST%\OdaFileConverter

set TD_CONF_NAME=%ODACFG%dbg
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj_app OdaFileConverterDebug %PRJDEST%\

set TD_CONF_NAME=%ODACFG%
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj_app OdaFileConverterRelease %PRJDEST%\

popd 
del %DEST%\OdaFileConverter\make_vcproj_app.bat

:--------------------

xcopy /S /Y make_vcproj_app.bat %DEST%\OdaQtApp\*.* >nul:
pushd %DEST%\OdaQtApp

set TD_CONF_NAME=%ODACFG%dbg
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj_app OdaQtAppDebug %PRJDEST%\

set TD_CONF_NAME=%ODACFG%
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj_app OdaQtAppRelease %PRJDEST%\

popd 
del %DEST%\OdaQtApp\make_vcproj_app.bat

:--------------------

xcopy /S /Y make_vcproj.bat %DEST%\OdaQtConsole\*.* >nul:
pushd %DEST%\OdaQtConsole

set TD_CONF_NAME=%ODACFG%dbg
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtConsoleDebug %PRJDEST%\

set TD_CONF_NAME=%ODACFG%
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtConsoleRelease %PRJDEST%\

popd 
del %DEST%\OdaQtConsole\make_vcproj.bat

:--------------------

xcopy /S /Y make_vcproj.bat %DEST%\OdaQtViewSystem\*.* >nul:
pushd %DEST%\OdaQtViewSystem

set TD_CONF_NAME=%ODACFG%dbg
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtViewSystemDebug %PRJDEST%\

set TD_CONF_NAME=%ODACFG%
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtViewSystemRelease %PRJDEST%\

popd 
del %DEST%\OdaQtViewSystem\make_vcproj.bat

:--------------------

xcopy /S /Y make_vcproj.bat %DEST%\OdaQtGles\*.* >nul:
pushd %DEST%\OdaQtGles

set TD_CONF_NAME=%ODACFG%dbg
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtGlesDebug %PRJDEST%\

set TD_CONF_NAME=%ODACFG%
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtGlesRelease %PRJDEST%\

popd 
del %DEST%\OdaQtGles\make_vcproj.bat

:--------------------

xcopy /S /Y make_vcproj.bat %DEST%\OdaQtOpenGL\*.* >nul:
pushd %DEST%\OdaQtOpenGL

set TD_CONF_NAME=%ODACFG%dbg
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtOpenGLDebug %PRJDEST%\

set TD_CONF_NAME=%ODACFG%
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtOpenGLRelease %PRJDEST%\

popd 
del %DEST%\OdaQtOpenGL\make_vcproj.bat

:--------------------

xcopy /S /Y make_vcproj.bat %DEST%\OdaQtPlotSettingsValidator\*.* >nul:
pushd %DEST%\OdaQtPlotSettingsValidator

set TD_CONF_NAME=%ODACFG%dbg
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtPlotSettingsValidatorDebug %PRJDEST%\

set TD_CONF_NAME=%ODACFG%
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtPlotSettingsValidatorRelease %PRJDEST%\

popd 
del %DEST%\OdaQtPlotSettingsValidator\make_vcproj.bat

:--------------------

xcopy /S /Y make_vcproj.bat %DEST%\OdaQtPropSystem\*.* >nul:
pushd %DEST%\OdaQtPropSystem

set TD_CONF_NAME=%ODACFG%dbg
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtPropSystemDebug %PRJDEST%\

set TD_CONF_NAME=%ODACFG%
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtPropSystemRelease %PRJDEST%\

popd 
del %DEST%\OdaQtPropSystem\make_vcproj.bat

:--------------------

xcopy /S /Y make_vcproj.bat %DEST%\TigDb\*.* >nul:
pushd %DEST%\TigDb

set TD_CONF_NAME=%ODACFG%dbg
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj TigDbDebug %PRJDEST%\

set TD_CONF_NAME=%ODACFG%
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj TigDbRelease %PRJDEST%\

popd 
del %DEST%\TigDb\make_vcproj.bat

:--------------------
 
:xcopy /S /Y make_vcproj.bat %DEST%\OdaQtObjects\*.* >nul:
:pushd %DEST%\OdaQtObjects

:set TD_CONF_NAME=%ODACFG%dbg
:set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
:set PRJ2SRC_LEV_OFFSET=../../
:if not exist %PRJDEST% mkdir %PRJDEST%

:call make_vcproj OdaQtObjectsDebug %PRJDEST%\

:set TD_CONF_NAME=%ODACFG%
:set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
:set PRJ2SRC_LEV_OFFSET=../../
:if not exist %PRJDEST% mkdir %PRJDEST%

:call make_vcproj OdaQtObjectsRelease %PRJDEST%\

:popd 
:del %DEST%\OdaQtObjects\make_vcproj.bat

:--------------------
