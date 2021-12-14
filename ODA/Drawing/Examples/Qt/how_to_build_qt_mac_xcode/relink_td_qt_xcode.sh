#! /bin/sh

if test ! -n "${ODADIR}" ; then export ODADIR="/_Oda/trunk" ; fi
# useless for framework version #if test ! -n "${QTDIR}" ; then export QTDIR="/usr/local/Qt4.5" ; fi
# see Developer/Examples/Qt

if test ! -d "${ODADIR}/Core" ; then
  echo "  Folder ${ODADIR}/Core was not found"
  exit 0
fi
if test ! -n "${ODAVERSION}" ; then export ODAVERSION="3.3" ; fi

if test ! -n "$1" ; then TD_ARG="-0" ; else TD_ARG="$1" ; fi
if [ $TD_ARG = "-o" ] ; then TD_ARG="-p" ; fi
if [ $TD_ARG = "-p" ] ; then
  TD_PKG_DIR="$2"
  if [ $TD_PKG_DIR = "" ] || test ! -d $TD_PKG_DIR ; then 
    echo "  Output folder ${TD_PKG_DIR} for packages was not found"
  	exit 0
  fi
fi
if [ $TD_ARG = "-0" ] ; then
  echo "  using as :"
  echo "    > sh relink_td_qt_xcode.sh -r"
  echo "        to attune internal paths - for debugging and executing on place"
  echo "    > sh relink_td_qt_xcode.sh -p <output_folder_for_packages>"
  echo "        to create relative linked packages executable on other mac computers"
  echo 
	exit 0
fi
#echo " <arg=${TD_ARG}>"
SV_PWD="${PWD}"

#------

#if test ! -n "${ODABIN}" ; then export ODABIN="${ODADIR}/lib" ; fi
if test ! -n "${ODABIN}" ; then export ODABIN="${ODADIR}/bin" ; fi

# for dll-conf only 
TD_CONF_LIST=""
TD_CONF_LIST="${TD_CONF_LIST} macOsX_x86_10.5dll macOsX_x86_10.5dlldbg macOsX_x86_10.5dyn macOsX_x86_10.5dyndbg"
TD_CONF_LIST="${TD_CONF_LIST} macOsX_x86_10.6dll macOsX_x86_10.6dlldbg macOsX_x86_10.6dyn macOsX_x86_10.6dyndbg"
TD_CONF_LIST="${TD_CONF_LIST} macOsX_x86_10.7dll macOsX_x86_10.7dlldbg macOsX_x86_10.7dyn macOsX_x86_10.7dyndbg"

TD_QT_TRG_LIST=""
TD_QT_TRG_LIST="${TD_QT_TRG_LIST} OdaFileConverter"
TD_QT_TRG_LIST="${TD_QT_TRG_LIST} OdaQtApp"

DLLS_SHRT=""
DLLS_SHRT="${DLLS_SHRT} libTD_Alloc.dylib libTD_DbRoot.dylib libTD_Root.dylib"
DLLS_SHRT="${DLLS_SHRT} libTD_Db.dylib libTD_DbCore.dylib TD_DbIO.tx TD_DbEntities.tx"
DLLS_SHRT="${DLLS_SHRT} SCENEOE.tx ACCAMERA.tx RText.tx ATEXT.tx ISM.tx WipeOut.tx"
DLLS_SHRT="${DLLS_SHRT} libTD_Ge.dylib libTD_Gi.dylib"
DLLS_SHRT="${DLLS_SHRT} libTD_Gs.dylib libTD_AcisBuilder.dylib libTD_Br.dylib libTD_BrepRenderer.dylib"
DLLS_SHRT="${DLLS_SHRT} ModelerGeometry.tx RecomputeDimBlock.tx ExFieldEvaluator.tx"
DLLS_SHRT="${DLLS_SHRT} libTD_SpatialIndex.dylib"

#------

#OUT_NAMES_OdaFileConverter="OdaFileConverter"
DLLS_OdaFileConverter="${DLLS_SHRT}"
# ver_dir_from.ctx_dir_to: dll ... dll :
FRAMEWORK_DLLS_OdaFileConverter="4.qt: QtCore QtGui :"

#------

OUT_NAMES_OdaQtApp="ODADrawingsExplorer"
DLLS_OdaQtApp="${DLLS_SHRT}"
DLLS_OdaQtApp="${DLLS_OdaQtApp} libOdaQtConsole.dylib"
DLLS_OdaQtApp="${DLLS_OdaQtApp} libOdaQtViewSystem.dylib"
DLLS_OdaQtApp="${DLLS_OdaQtApp} OdaQtOpenGL.txv" 
DLLS_OdaQtApp="${DLLS_OdaQtApp} libOdaQtPropSystem.dylib"
DLLS_OdaQtApp="${DLLS_OdaQtApp} TD_DgnDb.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} libOdaQtPlotSettingsValidator.dylib"
#DLLS_OdaQtApp="${DLLS_OdaQtApp} OdaQtObjects.tx"
# used for DGN
DLLS_OdaQtApp="${DLLS_OdaQtApp} TG_Db.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} TG_Dgn7IO.tx TG_ModelerGeometry.tx libPSToolkit.dylib libzlib.dylib"
DLLS_OdaQtApp="${DLLS_OdaQtApp} TD_DgnImport.tx AcDgnLS.tx"
# used for caustic
DLLS_OdaQtApp="${DLLS_OdaQtApp} RxRasterServices.tx"  # textures
DLLS_OdaQtApp="${DLLS_OdaQtApp} RasterProcessor.tx" 
DLLS_OdaQtApp="${DLLS_OdaQtApp} PlotStyleServices.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} AcModelDocObj.tx"
# used for TTF fonts for non-Windows platforms
#useless for non-Windows now #DLLS_OdaQtApp="${DLLS_OdaQtApp} TD_FtFontEngine.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} GripPoints.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} AcIdViewObj.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} TD_DynBlocks.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} TD_RasterExport.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} WinBitmap.txv"
DLLS_OdaQtApp="${DLLS_OdaQtApp} AeciIb.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} ExCommands.tx"
DATA_OdaQtApp=""
DATA_OdaQtApp="${DATA_OdaQtApp} adinit.dat"
# ver_dir_from.ctx_dir_to: dll ... dll :
FRAMEWORK_DLLS_OdaQtApp=""
FRAMEWORK_DLLS_OdaQtApp="${FRAMEWORK_DLLS_OdaQtApp} 4.qt: QtCore QtGui QtOpenGL QtXml"

if test -d "${ODADIR}/Caustic" ; then 
  OUT_NAMES_OdaQtApp="${OUT_NAMES_OdaQtApp} ODA_OpenRL_Demo"
  DLLS_ODA_OpenRL_Demo="${DLLS_OdaQtApp} OdaQtOpenRL.txv" 
  FRAMEWORK_DLLS_ODA_OpenRL_Demo="${FRAMEWORK_DLLS_OdaQtApp} A.caustic: CausticGLUT OpenRL :"
fi

FRAMEWORK_DLLS_OdaQtApp="${FRAMEWORK_DLLS_OdaQtApp} :"

#------

TD_DLLS="${DLLS_SHRT}"
#TD_DLLS="${TD_DLLS} libTD_Gs.dylib libTD_AcisBuilder.dylib libTD_Br.dylib libTD_BrepRenderer.dylib" # via DLL_SHRT
TD_DLLS="${TD_DLLS} libTD_BmpExport.dylib libTD_Dwf7Export.dylib"
TD_DLLS="${TD_DLLS} libTD_PdfExport.dylib"
TD_DLLS="${TD_DLLS} AcIdViewObj.tx AsdkSmileyDb.tx AutoSurfServices.tx DbCommands.tx TD_Dwf7Import.tx TD_DwfUnderlay.tx"
TD_DLLS="${TD_DLLS} TD_DynBlocks.tx ExEvalWatchers.tx"
TD_DLLS="${TD_DLLS} GripPoints.tx OdCurveFunctions.tx PlotStyleServices.tx"
TD_DLLS="${TD_DLLS} AcModelDocObj.tx"
TD_DLLS="${TD_DLLS} RxRasterServices.tx RasterProcessor.tx" 
TD_DLLS="${TD_DLLS} TD_Ave.tx TD_FtFontEngine.tx TD_STLExport.tx" 
TD_DLLS="${TD_DLLS} TD_SvgExport.tx libTD_PDFToolkit.dylib"
TD_DLLS="${TD_DLLS} ExDynamicBlocks.tx" ##  DummyRecomputeDimBlock.tx" 
TD_DLLS="${TD_DLLS} TD_RasterExport.tx WinBitmap.txv AeciIb.tx"

# DGN
TD_DLLS="${TD_DLLS} TG_Db.tx"
TD_DLLS="${TD_DLLS} TG_Dgn7IO.tx TG_DwgDb.tx TG_ModelerGeometry.tx libPSToolkit.dylib"
TD_DLLS="${TD_DLLS} TD_DgnImport.tx AcDgnLS.tx"
TD_DLLS="${TD_DLLS} libzlib.dylib"

#------

for TD_CONF_NAME in $TD_CONF_LIST
do
  TD_CONF_PATH="${ODABIN}/${TD_CONF_NAME}"
  if test ! -d "${TD_CONF_PATH}" ; then continue ; fi

  TD_PRO_SUFFIX="Release"
  case "${TD_CONF_NAME}" in
    *dbg) TD_PRO_SUFFIX="Debug" ;;
  esac

  TRGDEST="${ODABIN}/${TD_CONF_NAME}"

  # via CMAKE
  #RL_FROM_PATH="${ODABIN}/${TD_CONF_NAME}/"
  RL_FROM_PATH="" 
  
  ######################################################################
  
  # /_Oda/bin/macOsX_x86_10.Xdlldbg/_flg_abs_paths
  if [ $TD_ARG = "-r" ] && test ! -d "${TRGDEST}/_flg_abs_paths" ; then
    echo "  relink set of dylib, tx and txv for absolute paths in ${TRGDEST}"
    
    for DRX_TRG in $TD_DLLS
    do
      # /_Oda/bin/macOsX_x86_10.Xdlldbg/libTD_Db.dylib ...
      RL_TARG="${TRGDEST}/${DRX_TRG}"
    
      if  test ! -f $RL_TARG ; then
        echo "  WARNING: ${RL_TARG} was not found"
        continue
      fi
    
      for DRX in $TD_DLLS
      do
        # ../bin/macOsX_x86_10.Xdlldbg/libTD_Alloc.dylib ...
        RL_FROM="${RL_FROM_PATH}${DRX}" # = "../bin/${TD_CONF_NAME}/${DRX}"
        # /_Oda/bin/macOsX_x86_10.Xdlldbg/libTD_Alloc.dylib ...
        RL_TO="${TRGDEST}/${DRX}"

        if  test ! -f $RL_TO ; then continue ; fi
        
        if [ $DRX_TRG = $DRX ] ; then
          install_name_tool -id $RL_TO $RL_TARG
        else
          install_name_tool -change $RL_FROM $RL_TO $RL_TARG
        fi
      done
    done
   
    # mark as alredy done (long operation)
    mkdir "${TRGDEST}/_flg_abs_paths"
  fi

  ######################################################################

  for TD_QT_TRG in $TD_QT_TRG_LIST
  do
    #echo "${TD_QT_TRG}"
    if test ! -d "${TRGDEST}/${TD_QT_TRG}.app" ; then continue ; fi
    
    if [ $TD_ARG = "-p" ] && [ $TD_PRO_SUFFIX = "Release" ] ; then
      
      DEST_NAMES="OUT_NAMES_${TD_QT_TRG}"
      eval "DEST_NAMES=\$${DEST_NAMES}"
      if [ "$DEST_NAMES" = "" ] ; then DEST_NAMES="${TD_QT_TRG}" ; fi

      for DEST_NAME in $DEST_NAMES
      do
        #echo "DEST_NAME=${DEST_NAME}"

        DEST_PKG="${TD_PKG_DIR}/${TD_CONF_NAME}/$DEST_NAME.app"
        echo "  prepare package ${DEST_PKG} executable on other mac computer"
        if test ! -d $TD_PKG_DIR/$TD_CONF_NAME ; then mkdir $TD_PKG_DIR/$TD_CONF_NAME ; fi
        if test -d $DEST_PKG ; then rm -R $DEST_PKG ; fi
        cp -r "${TRGDEST}/${TD_QT_TRG}.app" "${DEST_PKG}"
        if [ "$TD_QT_TRG" != "$DEST_NAME" ] ; then
          mv -f "${DEST_PKG}/Contents/MacOS/$TD_QT_TRG" "${DEST_PKG}/Contents/MacOS/$DEST_NAME"
        fi
        
        # cp "${TRGDEST}/${DRX}" "${TD_PKG_DIR}/${TD_CONF_NAME}/$DEST_NAME.app/Contents/drx"
        mkdir "${DEST_PKG}/Contents/Resources"
        cp -f "${SV_PWD}/data/odalogo_mac.icns" "${DEST_PKG}/Contents/Resources/odalogo.icns" 
        # service common file
        cp -f "${SV_PWD}/data/fix_attribs.sh" "${TD_PKG_DIR}/${TD_CONF_NAME}"
        
        # Fixed package problem with qt_menu.nib on MacOX X 10.5 (Leopard)
        # cp -R /Library/Frameworks/QtGui.framework/Versions/4/Resources/qt_menu.nib ...
        cp -R "/Library/Frameworks/QtGui.framework/Versions/4/Resources/qt_menu.nib" "${DEST_PKG}/Contents/Resources" 
        cp -f "${SV_PWD}/data/${DEST_NAME}.plist" "${DEST_PKG}/Contents/Info.plist" 
        
        mkdir "${DEST_PKG}/Contents/tx"
        
        DLLS_LST="DLLS_${DEST_NAME}"
        eval "DLLS_LST=\$${DLLS_LST}"
        if [ "$DLLS_LST" = "" ] ; then 
          DLLS_LST="DLLS_${TD_QT_TRG}"
          eval "DLLS_LST=\$${DLLS_LST}"
        fi
        for DRX_TRG in $DLLS_LST # in $DLLS_SHRT #for DRX_TRG in $TD_DLLS
        do
          #echo "DRX_TRG=${DRX_TRG}"
        
          if  test ! -f "${TRGDEST}/${DRX_TRG}" ; then
            echo "  WARNING: ${TRGDEST}/${DRX_TRG} was not found"
            continue
          fi
          cp "${TRGDEST}/${DRX_TRG}" "${DEST_PKG}/Contents/tx"
          for DRX in $DLLS_LST # in $DLLS_SHRT #for DRX in $DLLS_SHRT
          do
            # ../bin/macOsX_x86_10.Xdlldbg/libTD_Alloc.dylib ...
            RL_FROM="${RL_FROM_PATH}${DRX}" # ="../bin/${TD_CONF_NAME}/${DRX}"
            # /_Oda/bin/macOsX_x86_10.Xdlldbg/_flg_abs_paths # /_Oda/bin/macOsX_x86_10.Xdlldbg/libTD_Db.dylib ...
            if test -d "${TRGDEST}/_flg_abs_paths" ; then RL_FROM="${TRGDEST}/${DRX}" ; fi
            # @executable_path/../tx/libTD_Alloc.dylib ...
            RL_TO="@executable_path/../tx/${DRX}"
            RL_TARG="${DEST_PKG}/Contents/tx/${DRX_TRG}"
            if  test ! -f $RL_TARG ; then continue ; fi
            if [ $DRX_TRG = $DRX ] ; then
              install_name_tool -id $DRX_TRG $RL_TARG
            else
              install_name_tool -change $RL_FROM $RL_TO $RL_TARG
            fi
          done
          RL_FROM="${RL_FROM_PATH}${DRX_TRG}" # ="../bin/${TD_CONF_NAME}/${DRX_TRG}" # case before first -r procedure
          # @executable_path/../../../libTD_Alloc.dylib ...
          if test -d "${TRGDEST}/_flg_abs_paths" ; then RL_FROM="@executable_path/../../../${DRX_TRG}" ; fi
          RL_TO="@executable_path/../tx/${DRX_TRG}"
          RL_TARG="${DEST_PKG}/Contents/MacOS/${DEST_NAME}"
          install_name_tool -change $RL_FROM $RL_TO $RL_TARG
          if test -d "${TRGDEST}/_flg_abs_paths" ; then 
            # if app built after  -r procedure
            RL_FROM="${TRGDEST}/${DRX_TRG}"
            install_name_tool -change $RL_FROM $RL_TO $RL_TARG
          fi
        done
        
        DATA_LST="DATA_${TD_QT_TRG}"  
        eval "DATA_LST=\$${DATA_LST}"
        for DATA_TRG in $DATA_LST # adinit.dat
        do
          if  test ! -f "${TRGDEST}/${DATA_TRG}" ; then
            echo "  WARNING: ${TRGDEST}/${DATA_TRG} was not found"
            continue
          fi
          # TODO #cp "${TRGDEST}/${DATA_TRG}" "${DEST_PKG}/Contents/Resources"
          cp "${TRGDEST}/${DATA_TRG}" "${DEST_PKG}/Contents/tx"
        done
        
        LINKED_DLLS_LST=""

        FRM_DLLS_LST="FRAMEWORK_DLLS_${DEST_NAME}"
        eval "FRM_DLLS_LST=\$${FRM_DLLS_LST}"
        if [ "$FRM_DLLS_LST" = "" ] ; then 
          FRM_DLLS_LST="FRAMEWORK_DLLS_${TD_QT_TRG}"
          eval "FRM_DLLS_LST=\$${FRM_DLLS_LST}"
        fi
        for FRM_DLL in $FRM_DLLS_LST
        do
          #echo "FRM_DLL=${FRM_DLL}"
        
          case "${FRM_DLL}" in  # ver_dir_from.ctx_dir_to: dll ... dll :
          *:) # with end marker           # *.*:) 
            if [ "$LINKED_DLLS_LST" != "" ] ; then
              
              mkdir "${DEST_PKG}/Contents/${FRM_SUB_FLD_TO}"
              for QT_TRG in $LINKED_DLLS_LST
              do
                # /Library/Frameworks/QtCore.framework/Versions/4/QtCore ...
                #echo "----copy /Library/Frameworks/$QT_TRG.framework/$FRM_VERSION_FROM/$QT_TRG to $FRM_SUB_FLD_TO"
                cp "/Library/Frameworks/${QT_TRG}.framework/Versions/${FRM_VERSION_FROM}/${QT_TRG}" "${DEST_PKG}/Contents/${FRM_SUB_FLD_TO}"
                for QTDLL in $LINKED_DLLS_LST
                do
                  RL_FROM="${QTDLL}.framework/Versions/${FRM_VERSION_FROM}/${QTDLL}" # QtCore.framework/QtCore.framework/Versions/4/QtCore
                  RL_FROM_FULL="/Library/Frameworks/$RL_FROM" # used for caustic
                  RL_TO="@executable_path/../${FRM_SUB_FLD_TO}/${QTDLL}"
                  RL_TARG="${DEST_PKG}/Contents/${FRM_SUB_FLD_TO}/${QT_TRG}"
                  if [ $QT_TRG = $QTDLL ] ; then
                    install_name_tool -id $QT_TRG $RL_TARG
                  else
                    install_name_tool -change $RL_FROM $RL_TO $RL_TARG
                    install_name_tool -change $RL_FROM_FULL $RL_TO $RL_TARG
                  fi
                done
                
                RL_FROM="${QT_TRG}.framework/Versions/${FRM_VERSION_FROM}/${QT_TRG}" # QtCore.framework/QtCore.framework/Versions/4/QtCore
                RL_FROM_FULL="/Library/Frameworks/$RL_FROM" # used for caustic
                RL_TO="@executable_path/../${FRM_SUB_FLD_TO}/${QT_TRG}"
                
                # relink app in MacOS folde3r
                RL_TARG="${DEST_PKG}/Contents/MacOS/${DEST_NAME}"
                install_name_tool -change $RL_FROM $RL_TO $RL_TARG
                install_name_tool -change $RL_FROM_FULL $RL_TO $RL_TARG

                # relink non-framework dlls  in tx folder
                for DRX_TRG in $DLLS_LST
                do
                  if  test ! -f "${TRGDEST}/${DRX_TRG}" ; then continue ; fi
                  RL_TARG="${DEST_PKG}/Contents/tx/${DRX_TRG}"
                  install_name_tool -change $RL_FROM $RL_TO $RL_TARG
                  install_name_tool -change $RL_FROM_FULL $RL_TO $RL_TARG
                done
               
              done
              LINKED_DLLS_LST=""
            fi
          
            FRM_DLL="${FRM_DLL%:}"
            #echo "FRM_DLL=$FRM_DLL"
            FRM_SUB_FLD_TO="${FRM_DLL#*.}"
            #echo "FRM_SUB_FLD_TO=$FRM_SUB_FLD_TO"
            FRM_VERSION_FROM="${FRM_DLL%.*}"
            #echo "FRM_DLL_VERSION=$FRM_VERSION_FROM"
            continue
            ;;
          esac
          
          LINKED_DLLS_LST="${LINKED_DLLS_LST} $FRM_DLL"
          
        done # for FRM_DLL in $FRM_DLLS_LST
        
        # create simplest installer  "${TD_PKG_DIR}/${TD_CONF_NAME}/$DEST_NAME.dmg"
        hdiutil create "${TD_PKG_DIR}/${TD_CONF_NAME}/${DEST_NAME}_${ODAVERSION}.dmg" -volname "ODA ${DEST_NAME} sample" -fs HFS+ -srcfolder "${DEST_PKG}"
      
      done #       for DEST_NAME in $DEST_NAMES
    fi # if [ $TD_ARG = "-p" ] && [ $TD_PRO_SUFFIX = "Release" ] 
      
    if [ $TD_ARG = "-r" ] ; then
      echo "  relink ${TRGDEST}/${TD_QT_TRG}.app for executable_path"
      echo "  relink ${TRGDEST}/Examples/Qt/build/${TD_PRO_SUFFIX}/${TD_QT_TRG}.app for executable_path"
      
      for DRX in $TD_DLLS
      do
        # ../bin/macOsX_x86_10.Xdlldbg/libTD_Alloc.dylib ...
        RL_FROM="${RL_FROM_PATH}${DRX}" # ="../bin/${TD_CONF_NAME}/${DRX}"
        # @executable_path/../../../libTD_Alloc.dylib ...
        RL_TO="@executable_path/../../../${DRX}"
        # /_Oda/bin/macOsX_x86_10.Xdlldbg/OdaFileConverter.app/Contents/MacOS/OdaFileConverter ...
        RL_TARG="${TRGDEST}/${TD_QT_TRG}.app/Contents/MacOS/${TD_QT_TRG}"
        install_name_tool -change $RL_FROM $RL_TO $RL_TARG

        RL_TO="@executable_path/../../../../../../../${DRX}"
        RL_TARG="${TRGDEST}/Examples/Qt/build/${TD_PRO_SUFFIX}/${TD_QT_TRG}.app/Contents/MacOS/${TD_QT_TRG}"
        install_name_tool -change $RL_FROM $RL_TO $RL_TARG
      done
    fi # if [ $TD_ARG = "-r" ] 
    
  done # for TD_QT_TRG in $TD_QT_TRG_LIST
done # for TD_CONF_NAME in $TD_CONF_LIST
