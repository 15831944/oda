:
: copy Qt sample to destination folder
:

:@echo off

if "%ODADIR%"=="" set ODADIR=\_Oda\trunk
if "%ODADLLSUFFIX%"=="" set ODADLLSUFFIX=_3.05src_8

if "%1"=="-p" goto check_path
if "%1"=="-o" goto check_path
if "%1"=="-i" goto check_path
:usage

echo "  using as :"
echo "    > pack_td_qt_msvc2005.bat -p <output_folder_for_packages>"
echo "        to create output packages"
goto end

:check_path
if exist "%2" goto proceed

echo "  output folder %2 for packages was not found"
goto end

:proceed

if not "%ODACFG%"=="" goto end_def_ODACFG
if exist %ODADIR%\exe\vc8dll\OdaMfcApp.exe set ODACFG=vc8dll
if exist %ODADIR%\exe\vc9dll\OdaMfcApp.exe set ODACFG=vc9dll
if exist %ODADIR%\exe\vc10dll\OdaMfcApp.exe set ODACFG=vc10dll
if exist %ODADIR%\exe\vc12dll\OdaMfcApp.exe set ODACFG=vc12dll
:end_def_ODACFG

set INPUT=%ODADIR%\exe\%ODACFG%

set BASEOUTPUT=%2\%ODACFG%
if not exist %BASEOUTPUT% mkdir %BASEOUTPUT%

if not exist %BASEOUTPUT%\install mkdir %BASEOUTPUT%\install >nul:
copy /Y %ODADIR%\Drawing\Examples\Qt\odalogo.ico %BASEOUTPUT%\install\*.* >nul:
set INPUTINST=%ODADIR%\Drawing\Examples\Qt\how_to_build_qt_msvc2005\install
xcopy /S /Y %INPUTINST%\*.* %BASEOUTPUT%\install\*.* >nul:

set OUT_NAME_OdaFileConverter=OdaFileConverter
set OUT_TITLE_OdaFileConverter=Oda File Converter
set OUTPUT=%BASEOUTPUT%\%OUT_NAME_OdaFileConverter%
if exist %OUTPUT% del /S /Q %OUTPUT% >nul:
mkdir %OUTPUT%

: qsed - special simple utility to convert content of vdproj and sln
if not exist %BASEOUTPUT%\install\bin mkdir %BASEOUTPUT%\install\bin
xcopy /Y %INPUT%\QtCore4.dll %BASEOUTPUT%\install\bin\*.* >nul:
pushd %BASEOUTPUT%\install\qsed
call qsed_make_and_build.bat 
popd
echo replace/_3.03_8/%ODADLLSUFFIX%/ >%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
echo replace/3.3.0/%ODAVERSION%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed

: TODO
: qsedres - special simple utility to attune resource strings in execitable file
:           (use to change application name in possible crash dialogs)
if not "%ODACFG%"=="" xcopy /Y %QTDIR%\bin_%ODACFG%dbg\QtCored4.dll %BASEOUTPUT%\install\bin\*.* >nul:
if not "%ODACFG%"=="" xcopy /Y %QTDIR%\lib_%ODACFG%dbg\QtCored4.lib %BASEOUTPUT%\install\qsedres\*.* >nul:
pushd %BASEOUTPUT%\install\qsedres
call qsedres_make_and_build.bat 
popd

if not "%ODACFG%"=="vc10dll" goto else_Runtime_vc10
:
:: old way : Microsoft.Visual.C++.8.0.x86 ==> Microsoft.Visual.C++.10.0.x86
::echo find/PrerequisitesLocation/ replace/2:1/2:2/ find/{EDC2488A-8267-493A-A98E-7D9C3B36CDF3}/ replace/8.0/10.0/ find/8:/ replace/Visual C++ Runtime/Visual C++ 2010 Runtime/ replace/8.0/10.0/ find/}/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
:::echo find/UpgradeCode/ insert_at_next_line/        "AspNetVersion" = "8:4.0.30319.0"/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
:
: #9627
: http://msgroups.net/microsoft.public.vc.mfc/Manifest-Hell-a-thing-of-the-past
:   manifest is no longer necessary for this with VS 2010. The program will 
:   find the DLLs if they're in its own folder
:   Yup, for the VS 2010 DLL's, manifest hell is gone, and we are back to DLL 
:
goto end_Runtime_vc10
:else_Runtime_vc10
:
: remove msvc10 runtime dlls
:   msvcr100.dll
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_92E26A4E259A4E8C945743EE633D134F/ off find/IsolateTo/ find/}/ on   >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
:   msvcp100.dll
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_B18C36D7359E4459B77C53935B513705/ off find/IsolateTo/ find/}/ on   >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
:
:end_Runtime_vc10

if "%ODACFG%"=="vc8dll" goto end_remove_Runtime_vc8
: remove msvc8 item of BootstrapperCfg  - previously renamed to msvc10
:   Microsoft.Visual.C++.8.0.x86
echo find/{EDC2488A-8267-493A-A98E-7D9C3B36CDF3}:Microsoft.Visual.C++.8.0.x86/ off find/ProductCode/ find/}/ on   >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
:end_remove_Runtime_vc8

::rename %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.vdproj %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.vdproj.bak >nul:
copy /Y %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.vdproj %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.vdproj.bak >nul:
del /S /Q %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.vdproj >nul:
copy /Y %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.pfw %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.pfw.bak >nul:
del /S /Q %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.pfw >nul:

:pushd %INPUT%
echo on

copy /Y %INPUT%\OdaFileConverter.exe %OUTPUT%\%OUT_NAME_OdaFileConverter%.exe >nul:
copy /Y %INPUT%\TD_AcisBuilder%ODADLLSUFFIX%.dll %OUTPUT%\TD_AcisBuilder*.* >nul:
copy /Y %INPUT%\TD_Alloc%ODADLLSUFFIX%.dll %OUTPUT%\TD_Alloc*.* >nul:
copy /Y %INPUT%\TD_BrepRenderer%ODADLLSUFFIX%.dll %OUTPUT%\TD_BrepRenderer*.* >nul:
copy /Y %INPUT%\TD_Br%ODADLLSUFFIX%.dll %OUTPUT%\TD_Br*.* >nul:
copy /Y %INPUT%\TD_DbRoot%ODADLLSUFFIX%.dll %OUTPUT%\TD_DbRoot*.* >nul:
copy /Y %INPUT%\TD_Db%ODADLLSUFFIX%.dll %OUTPUT%\TD_Db*.* >nul:
copy /Y %INPUT%\TD_Ge%ODADLLSUFFIX%.dll %OUTPUT%\TD_Ge*.* >nul:
copy /Y %INPUT%\TD_Gi%ODADLLSUFFIX%.dll %OUTPUT%\TD_Gi*.* >nul:
copy /Y %INPUT%\TD_Gs%ODADLLSUFFIX%.dll %OUTPUT%\TD_Gs*.* >nul:
copy /Y %INPUT%\TD_Root%ODADLLSUFFIX%.dll %OUTPUT%\TD_Root*.* >nul:
copy /Y %INPUT%\TD_SpatialIndex%ODADLLSUFFIX%.dll %OUTPUT%\TD_SpatialIndex*.* >nul:
copy /Y %INPUT%\ExFieldEvaluator%ODADLLSUFFIX%.tx %OUTPUT%\ExFieldEvaluator*.* >nul:
copy /Y %INPUT%\ModelerGeometry%ODADLLSUFFIX%.tx %OUTPUT%\ModelerGeometry*.* >nul:
copy /Y %INPUT%\RecomputeDimBlock%ODADLLSUFFIX%.tx %OUTPUT%\RecomputeDimBlock*.* >nul:
xcopy /Y %INPUT%\W3Dtk.dll %OUTPUT%\*.* >nul:
xcopy /Y %INPUT%\WhipTk.dll %OUTPUT%\*.* >nul:
xcopy /Y %INPUT%\TD_Zlib.dll %OUTPUT%\*.* >nul:
xcopy /Y %INPUT%\QtCore4.dll %OUTPUT%\*.* >nul:
xcopy /Y %INPUT%\QtGui4.dll %OUTPUT%\*.* >nul:
if not "%ODACFG%"=="vc10dll" goto end_msvc10_copy_OdaFileConverter
copy /Y %SystemRoot%\System32\msvcr100.dll %OUTPUT%\*.* >nul:
copy /Y %SystemRoot%\System32\msvcp100.dll %OUTPUT%\*.* >nul:   
:end_msvc10_copy_OdaFileConverter

copy /Y %BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed %BASEOUTPUT%\install\bin\tmp.sed >nul:
if not "%ODACFG%"=="vc10dll" goto end_remove_pfw_vcredist_OdaFileConverter
: remove vcredist from .pfw 
echo find/[File 3]/ off find/Disk=0/ on >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
echo replace/Files=3/Files=2/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
: end_remove_pfw_vcredist_OdaFileConverter
echo replace/OdaFileConverter_title/%OUT_TITLE_OdaFileConverter%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
:echo replace/OdaFileConverter/%OUT_NAME_OdaFileConverter%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
: reform guids ProductCode, PackageCode and UpgradeCode for OdaFileConverter
echo reform_guid_by/{1FF14C63-6F95-4CD7-8DD3-7D793595BD5D}/%ODAVERSION%+%OUT_NAME_OdaFileConverter%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
echo reform_guid_by/{95612628-EEAF-4BDC-8927-3E8BE2928CDE}/%ODAVERSION%+%OUT_NAME_OdaFileConverter%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed
echo reform_guid_by/{B5BBBAF9-E33B-494D-8F8B-BFB8B220498B}/%ODAVERSION%+%OUT_NAME_OdaFileConverter%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaFileConverter%.sed

:popd

set OUT_NAME_OdaQtApp=ODADrawingsExplorer
set OUT_TITLE_OdaQtApp=ODA Drawings Explorer
set OUTPUT=%BASEOUTPUT%\%OUT_NAME_OdaQtApp%
if exist %OUTPUT% del /S /Q %OUTPUT% >nul:
mkdir %OUTPUT%

copy /Y %BASEOUTPUT%\install\bin\tmp.sed %BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed >nul:
mkdir %BASEOUTPUT%\install\%OUT_NAME_OdaQtApp% >nul:
copy /Y %BASEOUTPUT%\install\OdaQtApp\OdaQtApp.vdproj %BASEOUTPUT%\install\%OUT_NAME_OdaQtApp%\%OUT_NAME_OdaQtApp%.vdproj.bak >nul:
copy /Y %BASEOUTPUT%\install\OdaQtApp\OdaQtApp.pfw %BASEOUTPUT%\install\%OUT_NAME_OdaQtApp%\%OUT_NAME_OdaQtApp%.pfw.bak >nul:

:pushd %INPUT%
echo on

copy /Y %INPUT%\OdaQtApp.exe %OUTPUT%\%OUT_NAME_OdaQtApp%.exe >nul:
copy /Y %INPUT%\OdaFileConverter.exe %OUTPUT%\OdaFileConverter.exe >nul:
copy /Y %INPUT%\OdaQtConsole%ODADLLSUFFIX%.dll %OUTPUT%\OdaQtConsole*.* >nul:
copy /Y %INPUT%\OdaQtViewSystem%ODADLLSUFFIX%.dll %OUTPUT%\OdaQtViewSystem*.* >nul:
copy /Y %INPUT%\OdaQtOpenGL%ODADLLSUFFIX%.txv %OUTPUT%\OdaQtOpenGL*.* >nul:
copy /Y %INPUT%\OdaQtPlotSettingsValidator%ODADLLSUFFIX%.dll %OUTPUT%\OdaQtPlotSettingsValidator*.* >nul:
copy /Y %INPUT%\OdaQtPropSystem%ODADLLSUFFIX%.dll %OUTPUT%\OdaQtPropSystem*.* >nul:
copy /Y %INPUT%\ExCommands%ODADLLSUFFIX%.tx %OUTPUT%\ExCommands*.* >nul:
copy /Y %INPUT%\TD_AcisBuilder%ODADLLSUFFIX%.dll %OUTPUT%\TD_AcisBuilder*.* >nul:
copy /Y %INPUT%\TD_Alloc%ODADLLSUFFIX%.dll %OUTPUT%\TD_Alloc*.* >nul:
copy /Y %INPUT%\TD_BrepRenderer%ODADLLSUFFIX%.dll %OUTPUT%\TD_BrepRenderer*.* >nul:
copy /Y %INPUT%\TD_Br%ODADLLSUFFIX%.dll %OUTPUT%\TD_Br*.* >nul:
copy /Y %INPUT%\TD_DbRoot%ODADLLSUFFIX%.dll %OUTPUT%\TD_DbRoot*.* >nul:
copy /Y %INPUT%\TD_Db%ODADLLSUFFIX%.dll %OUTPUT%\TD_Db*.* >nul:
copy /Y %INPUT%\TD_Ge%ODADLLSUFFIX%.dll %OUTPUT%\TD_Ge*.* >nul:
copy /Y %INPUT%\TD_Gi%ODADLLSUFFIX%.dll %OUTPUT%\TD_Gi*.* >nul:
copy /Y %INPUT%\TD_Gs%ODADLLSUFFIX%.dll %OUTPUT%\TD_Gs*.* >nul:
copy /Y %INPUT%\TD_Root%ODADLLSUFFIX%.dll %OUTPUT%\TD_Root*.* >nul:
copy /Y %INPUT%\TD_SpatialIndex%ODADLLSUFFIX%.dll %OUTPUT%\TD_SpatialIndex*.* >nul:
copy /Y %INPUT%\ExFieldEvaluator%ODADLLSUFFIX%.tx %OUTPUT%\ExFieldEvaluator*.* >nul:
copy /Y %INPUT%\ModelerGeometry%ODADLLSUFFIX%.tx %OUTPUT%\ModelerGeometry*.* >nul:
copy /Y %INPUT%\RecomputeDimBlock%ODADLLSUFFIX%.tx %OUTPUT%\RecomputeDimBlock*.* >nul:
copy /Y %INPUT%\PlotStyleServices%ODADLLSUFFIX%.tx %OUTPUT%\PlotStyleServices*.* >nul:
copy /Y %INPUT%\AcModelDocObj%ODADLLSUFFIX%.tx %OUTPUT%\AcModelDocObj*.* >nul:
copy /Y %INPUT%\AcIdViewObj%ODADLLSUFFIX%.tx %OUTPUT%\AcIdViewObj*.* >nul:
copy /Y %INPUT%\GripPoints%ODADLLSUFFIX%.tx %OUTPUT%\GripPoints*.* >nul:
copy /Y %INPUT%\RxRasterServices%ODADLLSUFFIX%.tx %OUTPUT%\RxRasterServices*.* >nul:
copy /Y %INPUT%\RasterProcessor%ODADLLSUFFIX%.tx %OUTPUT%\RasterProcessor*.* >nul:
copy /Y %INPUT%\TD_RasterExport%ODADLLSUFFIX%.tx %OUTPUT%\TD_RasterExport*.* >nul:
copy /Y %INPUT%\TD_DynBlocks%ODADLLSUFFIX%.tx %OUTPUT%\TD_DynBlocks*.* >nul:
copy /Y %INPUT%\WinBitmap%ODADLLSUFFIX%.txv %OUTPUT%\WinBitmap*.* >nul:
: 1 tx of TD + 2 dll and 78 tx of TA/TC :
copy /Y %INPUT%\Aec*%ODADLLSUFFIX%.dll %OUTPUT%\*.* >nul:
copy /Y %INPUT%\Aec*%ODADLLSUFFIX%.tx %OUTPUT%\*.* >nul:
:
xcopy /Y %INPUT%\W3Dtk.dll %OUTPUT%\*.* >nul:
xcopy /Y %INPUT%\WhipTk.dll %OUTPUT%\*.* >nul:
xcopy /Y %INPUT%\TD_Zlib.dll %OUTPUT%\*.* >nul:
xcopy /Y %INPUT%\QtCore4.dll %OUTPUT%\*.* >nul:
xcopy /Y %INPUT%\QtGui4.dll %OUTPUT%\*.* >nul:
xcopy /Y %INPUT%\QtOpenGL4.dll %OUTPUT%\*.* >nul:
xcopy /Y %INPUT%\QtXml4.dll %OUTPUT%\*.* >nul:
if not "%ODACFG%"=="vc10dll" goto end_msvc10_copy_ODADrawingsExplorer
copy /Y %SystemRoot%\System32\msvcr100.dll %OUTPUT%\*.* >nul:
copy /Y %SystemRoot%\System32\msvcp100.dll %OUTPUT%\*.* >nul:   
:end_msvc10_copy_ODADrawingsExplorer

if not exist %INPUT%\TD_DgnDb%ODADLLSUFFIX%.tx goto without_dgn

copy /Y %INPUT%\TD_DgnDb%ODADLLSUFFIX%.tx %OUTPUT%\TD_DgnDb*.* >nul:
copy /Y %INPUT%\TG_Db%ODADLLSUFFIX%.tx %OUTPUT%\TG_Db*.* >nul:
copy /Y %INPUT%\TG_Dgn7IO%ODADLLSUFFIX%.tx %OUTPUT%\TG_Dgn7IO*.* >nul:
copy /Y %INPUT%\TG_ModelerGeometry%ODADLLSUFFIX%.tx %OUTPUT%\TG_ModelerGeometry*.* >nul:
copy /Y %INPUT%\PSToolkit%ODADLLSUFFIX%.dll %OUTPUT%\PSToolkit*.* >nul:
copy /Y %INPUT%\TD_DgnImport%ODADLLSUFFIX%.tx %OUTPUT%\TD_DgnImport*.* >nul:
copy /Y %INPUT%\AcDgnLS%ODADLLSUFFIX%.tx %OUTPUT%\AcDgnLS*.* >nul:

goto end_dgn
:without_dgn

: remove dgn dlls
:TD_DgnDb
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_6F87AD8DBF7D4A76AF1D26424E91307E/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:TG_Db
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_5718B33A5D0345D781D0AA6580CE2D83/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:TG_Dgn7IO
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_A58802E8CC9B43958A1A887A3B093B34/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:TG_ModelerGeometry
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_7941185B683C4F03BCCCBA6AA257D3F9/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:PSToolkit
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_2CBE47441B1B4C86A49928E79F94A08B/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:TD_DgnImport
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_71E0D24F3B144E3CBC84BF7DA2DA0006/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AcDgnLS
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_4EF6909B388343008DF87CB527E2697A/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:end_dgn

if not exist %INPUT%\AecBase%ODADLLSUFFIX%.tx goto without_ta_tc
if not exist %INPUT%\AecCivilBase%ODADLLSUFFIX%.tx goto without_tc
goto end_ta_tc
:without_tc

:AeccExtensions.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_5F03613F558F4F6598A5DF216F7C8ACB/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AecCivilBase.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_2422FA3675604882897505650FA29943/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AecCivilBase40.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_B626D4282D9D4313A1AA017B3CDFB9DD/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AecCivilBase50.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_E5CE8A0A5E63420ABF777BA9FC00147B/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AecCivilBase60.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_AFA82F772D104D3A932E2B26C1E012DC/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AecCivilBase70.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_8962CEA9474C419A9D8ACCC7B11C1940/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccLand.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_FF00EE7072DC4728881D39A551E9B91A/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccLand40.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_F72CD4DC497A490F8CFCF75E39D272FD/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccLand50.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_1BCC461B174049D0AB792BE2A4A0F635/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccLand60.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_13B3BB0C51EE40B78D8559EC4B10C505/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccLand70.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_89590FEC24914274B74C003FB0D57EFC/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccNetwork.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_0AAAD231E5644F7ABD70B1C5FF13433A/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccNetwork40.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_4297A7DF06AA4F27A3ECD54A0078DED5/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccNetwork50.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_0A21EB321F6F4BE4B8C9FFCEB686AEAC/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccNetwork60.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_2EEF494A72FC4C85BED09C1C41537073/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccNetwork70.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_FAE4D08E9F404E9FBD9916D0407A5FAA/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccPlanProd.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_ECA83DDFB3B84E3AB9554A092EAE9C66/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccPlanProd40.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_6746294F4DC24535B44C5355EBEE45CF/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccPlanProd50.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_5491DFD169B0499FBBE7AA4C6F219F20/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccPlanProd60.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_AEAF5A1AB3CB424CA24C901EAB756BF8/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccPlanProd70.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_FCD8BD923E9A43059D2AF2D1A5933FE1/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccRoadway.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_6EC3EBCB40F94D93A8F1F325FE1B4C27/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccRoadway40.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_92F5D3CDCD034C43A561754E6699C155/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccRoadway50.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_EAD1DC08543B4458BA7F7DE85DF49A5B/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccRoadway60.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_FE53D7A698634B0F807D23D7A5864DB1/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccRoadway70.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_DFDBF896F59446FB92245F2728F374C2/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccSurvey.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_51B6EF9C21434C0DB1130AED6A59ED43/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccSurvey40.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_8CB0E8310D0C49D0AEBE8346B7BF8F36/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccSurvey50.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_24F68152C9604EC19F7FB712C27BE0E0/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccSurvey60.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_BFFDDFEDA12B49818DFDA49F3246B95E/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccSurvey70.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_A0DA5E99F2AF4C589DBE8E3BA1205181/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccUiLand.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_DA1606DFE3F74E67A638D8BC0F689FA4/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccvBase.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_7CBEA422BF3743A5B692FF986CCED1DD/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccvBase40.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_F6785B46FCA042D8BB7863585B40853B/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccvBase50.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_6E85609B448347F2A07E5A958B15FC70/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccvBase60.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_ADAF57BB0B80400387ACA7956D38ABAA/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:AeccvBase70.tx
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_FB486CE2C4624DA7B450116AB4D2D729/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed

goto end_ta_tc
:without_ta_tc

:AeccNetwork50.tx ... AeccLand.tx (2 * "Aec*.dll" + 78 * "Aec*.tx")
:  (AeciIb.tx is part of TD and skipped here)
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_0A21EB321F6F4BE4B8C9FFCEB686AEAC/ off find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_FF00EE7072DC4728881D39A551E9B91A/ find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed

:end_ta_tc

copy /Y %BASEOUTPUT%\install\bin\ODADrawingsExplorer.sed %BASEOUTPUT%\install\bin\tmp.sed >nul:

if "%ODACFG%"=="vc9dll" goto end_remove_Runtime_vc9_ODADrawingsExplorer
: remove msvc9 item of BootstrapperCfg - needed for caustic only
:   Microsoft.Visual.C++.9.0.x86
echo find/{EDC2488A-8267-493A-A98E-7D9C3B36CDF3}:Microsoft.Visual.C++.9.0.x86/ off find/ProductCode/ find/}/ on   >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:end_remove_Runtime_vc9_ODADrawingsExplorer

: remove vcredist from .pfw 
if not "%ODACFG%"=="vc8dll" goto vc9_remove_vcredist_pfw_ODADrawingsExplorer
: remove vcredist vc9 from .pfw 
echo find/[File 4]/ off find/Disk=0/ on >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
echo replace/Files=4/Files=3/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
goto end_remove_vcredist_pfw_ODADrawingsExplorer
:vc9_remove_vcredist_pfw_ODADrawingsExplorer
if not "%ODACFG%"=="vc9dll" goto vc10_remove_vcredist_pfw_ODADrawingsExplorer
: remove vcredist vc8 from .pfw 
echo find/[File 3]/ off find/Disk=0/ on replace/[File 4]/[File 3]/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
echo replace/Files=4/Files=3/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
goto end_remove_vcredist_pfw_ODADrawingsExplorer
:vc10_remove_vcredist_pfw_ODADrawingsExplorer
: remove vcredist vc8 & vc9 from .pfw 
echo find/[File 3]/ off find/[File 4]/ find/Disk=0/ on >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
echo replace/Files=4/Files=2/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:end_remove_vcredist_pfw_ODADrawingsExplorer

: remove caustic dlls
:OdaQtOpenRL
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_F04532AD756D418C8521EC927841B70C/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:CausticGLUT.dll
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_7583E8C0F5354174B1C6B6B11BE874D3/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:OpenRL.dll
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_F3050218D1804F08AFBF784C445240F5/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:pthreadVC2.dll
echo find/{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE}:_4A8C587CF0F44BAE9CEBF6FFB4A5DCAE/ off find/IsolateTo/ find/}/ on  >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed

echo replace/OdaQtApp_title/%OUT_TITLE_OdaQtApp%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
echo replace/OdaQtApp/%OUT_NAME_OdaQtApp%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
: reform guids ProductCode, PackageCode and UpgradeCode for ODADrawingsExplorer
echo reform_guid_by/{A26F7544-4BA1-43AA-82AD-6D1B3E57B0DA}/%ODAVERSION%+%OUT_NAME_OdaQtApp%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
echo reform_guid_by/{A4B1F541-7C7F-4C66-AD48-AA76149B31D3}/%ODAVERSION%+%OUT_NAME_OdaQtApp%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
echo reform_guid_by/{F9BF7A01-6E7E-4C9C-B63B-4FD583994D02}/%ODAVERSION%+%OUT_NAME_OdaQtApp%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed

if not exist %INPUT%\OdaQtOpenRL%ODADLLSUFFIX%.txv goto without_caustic

set OUT_NAME_OdaQtApp=ODA_OpenRL_Demo
set OUT_TITLE_OdaQtApp=ODA_OpenRL_Demo
set OUTPUT=%BASEOUTPUT%\%OUT_NAME_OdaQtApp%
if exist %OUTPUT% del /S /Q %OUTPUT% >nul:
mkdir %OUTPUT%

copy /Y %BASEOUTPUT%\install\bin\tmp.sed %BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed >nul:
: remove vcredist from .pfw 
if "%ODACFG%"=="vc8dll" goto end_remove_vcredist_pfw_ODA_OpenRL_Demo
: remove vcredist vc8 from .pfw 
echo find/[File 3]/ off find/Disk=0/ on replace/[File 4]/[File 3]/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
echo replace/Files=4/Files=3/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
:end_remove_vcredist_pfw_ODA_OpenRL_Demo

xcopy /Y %BASEOUTPUT%\ODADrawingsExplorer\*.* %OUTPUT%\*.* >nul:
:rename /Y %OUTPUT%\ODADrawingsExplorer.exe %OUTPUT%\%OUT_NAME_OdaQtApp%.exe >nul:
copy /Y %OUTPUT%\ODADrawingsExplorer.exe %OUTPUT%\%OUT_NAME_OdaQtApp%.exe >nul:
del /S /Q %OUTPUT%\ODADrawingsExplorer.exe >nul:
mkdir %BASEOUTPUT%\install\%OUT_NAME_OdaQtApp% >nul:
copy /Y %BASEOUTPUT%\install\OdaQtApp\OdaQtApp.vdproj %BASEOUTPUT%\install\%OUT_NAME_OdaQtApp%\%OUT_NAME_OdaQtApp%.vdproj.bak >nul:
copy /Y %BASEOUTPUT%\install\OdaQtApp\OdaQtApp.pfw %BASEOUTPUT%\install\%OUT_NAME_OdaQtApp%\%OUT_NAME_OdaQtApp%.pfw.bak >nul:

copy /Y %INPUT%\OdaQtOpenRL%ODADLLSUFFIX%.txv %OUTPUT%\OdaQtOpenRL*.* >nul:

pushd %ODADIR%\Caustic\Bin\OpenRL_SDK\Bin\Win32\Release
xcopy /Y CausticGLUT.dll %OUTPUT%\*.* >nul:
xcopy /Y OpenRL.dll %OUTPUT%\*.* >nul:
xcopy /Y pthreadVC2.dll %OUTPUT%\*.* >nul:
popd 

echo replace/OdaQtApp_title/%OUT_TITLE_OdaQtApp%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
echo replace/OdaQtApp/%OUT_NAME_OdaQtApp%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
: reform guids ProductCode, PackageCode and UpgradeCode for ODADrawingsExplorer
echo reform_guid_by/{A26F7544-4BA1-43AA-82AD-6D1B3E57B0DA}/%ODAVERSION%+%OUT_NAME_OdaQtApp%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
echo reform_guid_by/{A4B1F541-7C7F-4C66-AD48-AA76149B31D3}/%ODAVERSION%+%OUT_NAME_OdaQtApp%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed
echo reform_guid_by/{F9BF7A01-6E7E-4C9C-B63B-4FD583994D02}/%ODAVERSION%+%OUT_NAME_OdaQtApp%/ >>%BASEOUTPUT%\install\bin\%OUT_NAME_OdaQtApp%.sed

:without_caustic

:popd

if "%1"=="-p" goto end
: TODO for other path except ..\..\..\..\..\..\..\out\%ODACFG%\OdaFileConverter

:if not "%MSVCCOMMON%"=="" goto end_def_MSVCCOMMON
if exist %ODADIR%\exe\vc8dlldbg\OdaMfcApp.exe set MSVCCOMMON=%VS80COMNTOOLS%..\
if exist %ODADIR%\exe\vc9dlldbg\OdaMfcApp.exe set MSVCCOMMON=%VS90COMNTOOLS%..\
if exist %ODADIR%\exe\vc10dlldbg\OdaMfcApp.exe set MSVCCOMMON=%VS100COMNTOOLS%..\
if exist %ODADIR%\exe\vc8dll\OdaMfcApp.exe set MSVCCOMMON=%VS80COMNTOOLS%..\
if exist %ODADIR%\exe\vc9dll\OdaMfcApp.exe set MSVCCOMMON=%VS90COMNTOOLS%..\
if exist %ODADIR%\exe\vc10dll\OdaMfcApp.exe set MSVCCOMMON=%VS100COMNTOOLS%..\
if exist %ODADIR%\exe\vc12dll\OdaMfcApp.exe set MSVCCOMMON=%VS120COMNTOOLS%..\
:end_def_MSVCCOMMON
: test : installer via vc8
:set MSVCCOMMON=%VS80COMNTOOLS%..\

pushd %BASEOUTPUT%\install\bin

set PFTW="%ProgramFiles%\InstallShield\PackageForTheWeb 4\Pftwwiz.exe"

if not exist %BASEOUTPUT%\install\OdaFileConverter goto end_installer_converter
qsed.exe -f OdaFileConverter.sed %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.vdproj.bak >%BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.vdproj
qsed.exe -f OdaFileConverter.sed %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.pfw.bak >%BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.pfw
pushd %BASEOUTPUT%\install\OdaFileConverter
call "%MSVCCOMMON%Ide\devenv.com" "OdaFileConverter.vdproj" /rebuild
popd
if not exist %PFTW% goto end_installer_converter_with_warning
pushd %BASEOUTPUT%
call %PFTW% %BASEOUTPUT%\install\OdaFileConverter\OdaFileConverter.pfw -a -s 
popd
goto end_installer_converter
:end_installer_converter_with_warning
echo " *** PackageForTheWeb401.exe (http://www.installshield.com/pftw/) should be downloaded and preinstalled to default folder
:end_installer_converter

if not exist %BASEOUTPUT%\install\ODADrawingsExplorer goto end_installer_viewer
qsed.exe -f ODADrawingsExplorer.sed %BASEOUTPUT%\install\ODADrawingsExplorer\ODADrawingsExplorer.vdproj.bak >%BASEOUTPUT%\install\ODADrawingsExplorer\ODADrawingsExplorer.vdproj
qsed.exe -f ODADrawingsExplorer.sed %BASEOUTPUT%\install\ODADrawingsExplorer\ODADrawingsExplorer.pfw.bak >%BASEOUTPUT%\install\ODADrawingsExplorer\ODADrawingsExplorer.pfw
pushd %BASEOUTPUT%\install\ODADrawingsExplorer
call "%MSVCCOMMON%Ide\devenv.com" "ODADrawingsExplorer.vdproj" /rebuild
popd
if not exist %PFTW% goto end_installer_viewer
pushd %BASEOUTPUT%
call %PFTW% %BASEOUTPUT%\install\ODADrawingsExplorer\ODADrawingsExplorer.pfw -a -s 
popd
:end_installer_viewer

if not exist %BASEOUTPUT%\install\ODA_OpenRL_Demo goto end_installer_caustic_demo
qsed.exe -f ODA_OpenRL_Demo.sed %BASEOUTPUT%\install\ODA_OpenRL_Demo\ODA_OpenRL_Demo.vdproj.bak >%BASEOUTPUT%\install\ODA_OpenRL_Demo\ODA_OpenRL_Demo.vdproj
qsed.exe -f ODA_OpenRL_Demo.sed %BASEOUTPUT%\install\ODA_OpenRL_Demo\ODA_OpenRL_Demo.pfw.bak >%BASEOUTPUT%\install\ODA_OpenRL_Demo\ODA_OpenRL_Demo.pfw
pushd %BASEOUTPUT%\install\ODA_OpenRL_Demo
call "%MSVCCOMMON%Ide\devenv.com" "ODA_OpenRL_Demo.vdproj" /rebuild
popd
if not exist %PFTW% goto end_installer_caustic_demo
if not "%ODACFG%"=="vc10dll" goto end_check_vc9redist
set VC9REDIST_TO="%ProgramFiles%\Microsoft SDKs\Windows\v7.0A\Bootstrapper\Packages\vcredist_x86_vc9"
set VC9REDIST_FROM="%ProgramFiles%\Microsoft SDKs\Windows\v6.0A\Bootstrapper\Packages\vcredist_x86"
if exist %VC9REDIST_TO% goto end_check_vc9redist 
mkdir %VC9REDIST_TO% >nul:
xcopy /S /Y %VC9REDIST_FROM%\*.* %VC9REDIST_TO%\*.* >nul:
:end_check_vc9redist
pushd %BASEOUTPUT%
call %PFTW% %BASEOUTPUT%\install\ODA_OpenRL_Demo\ODA_OpenRL_Demo.pfw -a -s 
popd
:end_installer_caustic_demo

cd ..\..
:rmdir /S /Q install
popd

:end
