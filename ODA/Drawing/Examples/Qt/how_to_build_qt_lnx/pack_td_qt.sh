#! /bin/sh

if test ! -n "${ODADIR}" ; then export ODADIR="/_Oda/trunk" ; fi
if test ! -d "${ODADIR}/Core" ; then
  echo "  Folder ${ODADIR}/Core was not found"
  exit 0
fi
if test ! -n "${ODAVERSION}" ; then export ODAVERSION="3.3" ; fi
 
if test ! -n "$1" ; then TD_ARG="-0" ; else TD_ARG="$1" ; fi
if [ $TD_ARG = "-o" ] ; then
  TD_RPM_DIR=$2
  TD_ARG="-p" ; 
fi
if [ $TD_ARG = "-r" ] ; then 
  TD_RPM_DIR=$2
  if [ $2 = "" ] || test ! -d $2 ; then 
    echo "  Output folder $2 for rpm packages was not found"
  	exit 0
  fi
fi
if [ $TD_ARG = "-p" ] ; then
  TD_PKG_DIR="$2"
  if [ $TD_PKG_DIR = "" ] || test ! -d $TD_PKG_DIR ; then 
    echo "  Output folder ${TD_PKG_DIR} for packages was not found"
  	exit 0
  fi
fi
if [ $TD_ARG != "-p" ] && [ $TD_ARG != "-r" ] ; then
  echo "  using as :"
  echo "    > sh pack_td_qt.sh -p <output_folder_for_packages>"
  echo "        to create output simple packages"
  echo "    > sh pack_td_qt.sh -r <output_folder_for_rpm_packages>"
  echo "        to create rpm packages"
  echo "    > sh pack_td_qt.sh -o <output_folder_for_rpm_packages>"
  echo "        to create rpm and simple packages"
  echo 
fi
#echo " <arg=${TD_ARG}>"
SV_PWD="${PWD}"

#if test ! -n "${ODABIN}" ; then export ODABIN="${ODADIR}/lib" ; fi
if test ! -n "${ODABIN}" ; then export ODABIN="${ODADIR}/bin" ; fi

#------

# for dll-conf only 
TD_CONF_LIST=""
TD_CONF_LIST="${TD_CONF_LIST} lnxX86_3.2dll lnxX86_3.2dlldbg lnxX86_3.2dyn lnxX86_3.2dyndbg" 
TD_CONF_LIST="${TD_CONF_LIST} lnxX86_3.3dll lnxX86_3.3dlldbg lnxX86_3.3dyn lnxX86_3.3dyndbg" 
TD_CONF_LIST="${TD_CONF_LIST} lnxX86_3.4dll lnxX86_3.4dlldbg lnxX86_3.4dyn lnxX86_3.4dyndbg" 
TD_CONF_LIST="${TD_CONF_LIST} lnxX86_4.1dll lnxX86_4.1dlldbg lnxX86_4.1dyn lnxX86_4.1dyndbg" 
TD_CONF_LIST="${TD_CONF_LIST} lnxX86_4.2dll lnxX86_4.2dlldbg lnxX86_4.2dyn lnxX86_4.2dyndbg" 
TD_CONF_LIST="${TD_CONF_LIST} lnxX86_4.4dll lnxX86_4.4dlldbg lnxX86_4.4dyn lnxX86_4.4dyndbg" 
TD_CONF_LIST="${TD_CONF_LIST} lnxX86_4.7dll lnxX86_4.7dlldbg lnxX86_4.7dyn lnxX86_4.7dyndbg" 
TD_CONF_LIST="${TD_CONF_LIST} lnxX86_4.8dll lnxX86_4.8dlldbg lnxX86_4.8dyn lnxX86_4.8dyndbg" 
TD_CONF_LIST="${TD_CONF_LIST} lnxX86_4.9dll lnxX86_4.9dlldbg lnxX86_4.9dyn lnxX86_4.9dyndbg" 
#TD_CONF_LIST="${TD_CONF_LIST} lnxX64_3.4dll lnxX64_3.4dlldbg lnxX64_3.4dyn lnxX64_3.4dyndbg" 
#TD_CONF_LIST="${TD_CONF_LIST} lnxX64_4.1dll lnxX64_4.1dlldbg lnxX64_4.1dyn lnxX64_4.1dyndbg" 
#TD_CONF_LIST="${TD_CONF_LIST} lnxX64_4.2dll lnxX64_4.2dlldbg lnxX64_4.2dyn lnxX64_4.2dyndbg" 

TD_QT_TRG_LIST=""
TD_QT_TRG_LIST="${TD_QT_TRG_LIST} OdaFileConverter"
TD_QT_TRG_LIST="${TD_QT_TRG_LIST} OdaQtApp"

DLLS_SHRT=""
DLLS_SHRT="${DLLS_SHRT} libTD_Alloc.so libTD_DbRoot.so libTD_Root.so"
DLLS_SHRT="${DLLS_SHRT} libTD_Db.so libTD_DbCore.so TD_DbIO.tx TD_DbEntities.tx"
DLLS_SHRT="${DLLS_SHRT} SCENEOE.tx ACCAMERA.tx RText.tx ATEXT.tx ISM.tx WipeOut.tx"
DLLS_SHRT="${DLLS_SHRT} libTD_Ge.so libTD_Gi.so"
DLLS_SHRT="${DLLS_SHRT} libTD_Gs.so libTD_AcisBuilder.so libTD_Br.so libTD_BrepRenderer.so"
DLLS_SHRT="${DLLS_SHRT} ModelerGeometry.tx RecomputeDimBlock.tx ExFieldEvaluator.tx"
DLLS_SHRT="${DLLS_SHRT} libTD_SpatialIndex.so"

#------

DESCRIPTION_OdaFileConverter="OdaFileConverter v${ODAVERSION} package created by ODA."
DESCRIPTION_OdaFileConverter_1="OdaFileConverter is a free application that converts files between the .dwg and .dxf file formats."
#DESCRIPTION_OdaFileConverter_2="This software was created with using ODA SDK for .dwg, .dwf files and Qt."
DESCRIPTION_OdaFileConverter_2="This software was created with using ODA SDK for .dwg files and Nokia Qt."
DESCRIPTION_OdaFileConverter_3="."
DESCRIPTION_OdaFileConverter_4="* Free to download."
#DEPENDS_OdaFileConverter="libc6 (>= 2.12)"
#DEPENDS_OdaFileConverter="$DEPENDS_OdaFileConverter, libdrm2 (>= 2.4.17)"
#DEPENDS_OdaFileConverter="$DEPENDS_OdaFileConverter, libfreetype6 (>= 2.4.0)"
#DEPENDS_OdaFileConverter="$DEPENDS_OdaFileConverter, libgcc1"
#DEPENDS_OdaFileConverter="$DEPENDS_OdaFileConverter, libglib2.0-0 (>= 2.25.16)"
#DEPENDS_OdaFileConverter="$DEPENDS_OdaFileConverter, libstdc++6"
#DEPENDS_OdaFileConverter="$DEPENDS_OdaFileConverter, libuuid1 (>= 2.16)"
#DEPENDS_OdaFileConverter="$DEPENDS_OdaFileConverter, libx11-6, libxcb1, libxext6"
#DEPENDS_OdaFileConverter="$DEPENDS_OdaFileConverter, zlib1g (>= 1:1.2.3.3.dfsg-1)"
DLLS_OdaFileConverter="${DLLS_SHRT}" 
QT_DLLS_OdaFileConverter="libQtCore.so libQtGui.so"
if test ! -f "/usr/bin/rpmbuild" ; then 
  QT_DLLS_OdaFileConverter="${QT_DLLS_OdaFileConverter} libQtXml.so libQtDBus.so"
fi

#------

OUT_NAME_OdaQtApp="ODADrawingsExplorer"
DESCRIPTION_OdaQtApp="ODADrawingsExplorer v${ODAVERSION} package created by ODA."
DESCRIPTION_OdaQtApp_1="ODADrawingsExplorer is a free rendering application, which can render .dwg and .dgn files."
#DESCRIPTION_OdaQtApp_2="This software was created with using ODA SDK® for .dwg files and Nokia Qt."
DESCRIPTION_OdaQtApp_2="This software was created with using ODA SDK for .dwg files and Nokia Qt."
DESCRIPTION_OdaQtApp_3="."
DESCRIPTION_OdaQtApp_4="* Free to download."
#DEPENDS_OdaQtApp="libc6 (>= 2.12)"
#DEPENDS_OdaQtApp="$DEPENDS_OdaQtApp, libdrm2 (>= 2.4.17)"
#DEPENDS_OdaQtApp="$DEPENDS_OdaQtApp, libfreetype6 (>= 2.4.0)"
#DEPENDS_OdaQtApp="$DEPENDS_OdaQtApp, libgcc1"
#DEPENDS_OdaQtApp="$DEPENDS_OdaQtApp, libglib2.0-0 (>= 2.25.16)"
#DEPENDS_OdaQtApp="$DEPENDS_OdaQtApp, libstdc++6"
#DEPENDS_OdaQtApp="$DEPENDS_OdaQtApp, libuuid1 (>= 2.16)"
#DEPENDS_OdaQtApp="$DEPENDS_OdaQtApp, libx11-6, libxcb1, libxext6"
#DEPENDS_OdaQtApp="$DEPENDS_OdaQtApp, zlib1g (>= 1:1.2.3.3.dfsg-1)"
DLLS_OdaQtApp="${DLLS_SHRT}"
DLLS_OdaQtApp="${DLLS_OdaQtApp} libOdaQtConsole.so"
DLLS_OdaQtApp="${DLLS_OdaQtApp} libOdaQtViewSystem.so"
DLLS_OdaQtApp="${DLLS_OdaQtApp} OdaQtGLES2.txv" 
DLLS_OdaQtApp="${DLLS_OdaQtApp} OdaQtOpenGL.txv" 
DLLS_OdaQtApp="${DLLS_OdaQtApp} libOdaQtPropSystem.so"
#DLLS_OdaQtApp="${DLLS_OdaQtApp} OdaQtOpenRL.txv" 
DLLS_OdaQtApp="${DLLS_OdaQtApp} TD_DgnDb.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} libOdaQtPlotSettingsValidator.so"
#DLLS_OdaQtApp="${DLLS_OdaQtApp} OdaQtObjects.tx"
# used for DGN
DLLS_OdaQtApp="${DLLS_OdaQtApp} TG_Db.tx TG_Dgn7IO.tx TG_ModelerGeometry.tx libPSToolkit.so"
DLLS_OdaQtApp="${DLLS_OdaQtApp} TD_DgnImport.tx AcDgnLS.tx"
# temp solution for error during installation about failed dependencies to TG_Db.tx 
#DLLS_OdaQtApp="${DLLS_OdaQtApp} libTG_Db.so" 
DLLS_OdaQtApp="${DLLS_OdaQtApp} libzlib.so" # fix problem of RPM generation after DGN components
# used for caustic
DLLS_OdaQtApp="${DLLS_OdaQtApp} RxRasterServices.tx"  # textures
DLLS_OdaQtApp="${DLLS_OdaQtApp} RasterProcessor.tx" 
DLLS_OdaQtApp="${DLLS_OdaQtApp} PlotStyleServices.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} AcModelDocObj.tx"
# used for TTF fonts for non-Windows platforms
DLLS_OdaQtApp="${DLLS_OdaQtApp} TD_FtFontEngine.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} GripPoints.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} AcIdViewObj.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} TD_DynBlocks.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} TD_RasterExport.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} WinBitmap.txv"
DLLS_OdaQtApp="${DLLS_OdaQtApp} AeciIb.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} ExCommands.tx"
DLLS_OdaQtApp="${DLLS_OdaQtApp} adinit.dat"
# dir_from: dll ... dll :
QT_DLLS_OdaQtApp="libQtCore.so libQtGui.so libQtOpenGL.so libQtXml.so"
if test ! -f "/usr/bin/rpmbuild" ; then 
  QT_DLLS_OdaQtApp="${QT_DLLS_OdaQtApp} libQtDBus.so"
fi
  
#------ 

QT_PATH="/usr/lib"

#------

for TD_CONF_NAME in $TD_CONF_LIST
do
  if test -d "${ODABIN}/${TD_CONF_NAME}"
  then
    TD_PRO_SUFFIX="Release"
    case "${TD_CONF_NAME}" in
      *dbg) TD_PRO_SUFFIX="Debug" ;;
    esac
    #TD_PRO_TYPE="static"
    #case "${TD_CONF_NAME}" in
    #  *dll*) TD_PRO_TYPE="dll" ;;
    #esac
	
    TRGDEST="${ODABIN}/${TD_CONF_NAME}"

    if test -f "${TRGDEST}/OdaQtOpenRL.txv" ; then OUT_NAME_OdaQtApp="ODA_OpenRL_Demo" ; else OUT_NAME_OdaQtApp="ODADrawingsExplorer" ; fi
    
    for TD_QT_TRG in $TD_QT_TRG_LIST
    do
      if test -f "${TRGDEST}/${TD_QT_TRG}" ; then

        if [ $TD_PRO_SUFFIX = "Release" ] && [ "$TD_PKG_DIR" != "" ] ; then

          #DEST_PKG="${TD_PKG_DIR}/${TD_CONF_NAME}/${TD_QT_TRG}"
          DEST_NAME="OUT_NAME_${TD_QT_TRG}"
          eval "DEST_NAME=\$${DEST_NAME}"
          if [ "$DEST_NAME" = "" ] ; then DEST_NAME="${TD_QT_TRG}" ; fi
          FRM_DLLS_LST="DLLS_${TD_QT_TRG}"  
          eval "FRM_DLLS_LST=\$${FRM_DLLS_LST}"
          FRM_QT_DLLS_LST="QT_DLLS_${TD_QT_TRG}"  
          eval "FRM_QT_DLLS_LST=\$${FRM_QT_DLLS_LST}"
          DEST_PKG="${TD_PKG_DIR}/${TD_CONF_NAME}/$DEST_NAME"
          
          echo "  prepare package ${DEST_PKG}"
          if test ! -d $TD_PKG_DIR/$TD_CONF_NAME ; then mkdir $TD_PKG_DIR/$TD_CONF_NAME ; fi
          if test -d $DEST_PKG ; then rm -R $DEST_PKG ; fi
          mkdir $DEST_PKG
          cp -f "${TRGDEST}/${TD_QT_TRG}" "${DEST_PKG}/${DEST_NAME}"
          strip --strip-unneeded "${DEST_PKG}/${DEST_NAME}"
          for DRX_TRG in $FRM_DLLS_LST
          do
            cp  -f "${TRGDEST}/${DRX_TRG}" "${DEST_PKG}"
            if test "${DRX_TRG}" != "adinit.dat" ; then strip --strip-unneeded "${DEST_PKG}/${DRX_TRG}" ; fi
          done
          for DRX_TRG in $FRM_QT_DLLS_LST
          do
            #cp -f /usr/bin/libQtCore.so ...
            cp  -f "${QT_PATH}/${DRX_TRG}" "${DEST_PKG}"
            strip --strip-unneeded "${DEST_PKG}/${DRX_TRG}"
          done
          # copy service common file
          cp -f "${SV_PWD}/data/run_${DEST_NAME}.sh" "${DEST_PKG}/.."
          chmod a+rwx "${DEST_PKG}/../run_${DEST_NAME}.sh"
        fi

        if [ $TD_PRO_SUFFIX = "Release" ] && [ "$TD_RPM_DIR" != "" ] ; then

          if test -f "/usr/bin/rpmbuild" ; then 
            echo "  prepare ${DEST_NAME} rpm package ${TD_RPM_DIR}/${TD_CONF_NAME}"
            DEST_PKG="${TD_RPM_DIR}/${TD_CONF_NAME}/__rpm__${DEST_NAME}"
            DEST_PKG_ROOT="${DEST_PKG}/BUILD_ROOT"
            EXEC_BIN="/usr/local/bin"
            EXEC_NAME="${DEST_NAME}" #TODO #EXEC_NAME="${DEST_NAME}-${ODAVERSION}"
            DEST_PKG_BIN_UP="${DEST_PKG_ROOT}/usr/local"
          else
            echo "  prepare ${DEST_NAME} debian package ${TD_RPM_DIR}/${TD_CONF_NAME}"
            DEST_PKG="${TD_RPM_DIR}/${TD_CONF_NAME}/${DEST_NAME}-${ODAVERSION}"
            DEST_PKG_ROOT="${DEST_PKG}" #DEST_PKG_ROOT="${DEST_PKG}/CONTENTS"
            EXEC_BIN="/usr/bin"
            EXEC_NAME="${DEST_NAME}" #TODO #EXEC_NAME="${DEST_NAME}-${ODAVERSION}"
          fi
          DEST_PKG_BIN="${DEST_PKG_ROOT}${EXEC_BIN}"
          
          #DEST_PKG="${TD_RPM_DIR}/${TD_CONF_NAME}"
          DEST_NAME="OUT_NAME_${TD_QT_TRG}"
          eval "DEST_NAME=\$${DEST_NAME}"
          if [ "$DEST_NAME" = "" ] ; then DEST_NAME="${TD_QT_TRG}" ; fi
          FRM_DLLS_LST="DLLS_${TD_QT_TRG}"  
          eval "FRM_DLLS_LST=\$${FRM_DLLS_LST}"
          FRM_QT_DLLS_LST="QT_DLLS_${TD_QT_TRG}"  
          eval "FRM_QT_DLLS_LST=\$${FRM_QT_DLLS_LST}"

          if test ! -d $TD_RPM_DIR/$TD_CONF_NAME ; then mkdir $TD_RPM_DIR/$TD_CONF_NAME ; fi
          if test -d $DEST_PKG ; then rm -R $DEST_PKG ; fi
          mkdir $DEST_PKG
          if test "$DEST_PKG" != "$DEST_PKG_ROOT" ; then mkdir $DEST_PKG_ROOT ; fi
          mkdir $DEST_PKG_ROOT/usr
          if test "$DEST_PKG_BIN_UP" != "" ; then mkdir $DEST_PKG_BIN_UP ; fi
          mkdir $DEST_PKG_BIN
          mkdir "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}" # .../OdaFileConverter_3.2
          
          #
          # create startup file in ../BUILD_ROOT/usr/local/bin/$DEST_NAME
          EXEC_FILE="${DEST_PKG_BIN}/${EXEC_NAME}"
          #cp -f "${SV_PWD}/data/empty.sh" "$EXEC_FILE" # as way to get executable file without admin rights
          #echo "#! /bin/sh" >>"$EXEC_FILE"
          echo "#! /bin/sh" >"$EXEC_FILE"
          chmod a+rwx "$EXEC_FILE"
          echo "SV_LD_LIBRARY_PATH=\$LD_LIBRARY_PATH" >>"$EXEC_FILE"  
          echo "export LD_LIBRARY_PATH=${EXEC_BIN}/${DEST_NAME}_${ODAVERSION}:\$LD_LIBRARY_PATH" >>"$EXEC_FILE" 
          echo "${EXEC_BIN}/${DEST_NAME}_${ODAVERSION}/${DEST_NAME} \$1 \$2 \$3 \$4 \$5 \$6 \$7 \$8 \$9" >>"$EXEC_FILE" 
          echo "export LD_LIBRARY_PATH=\$SV_LD_LIBRARY_PATH" >>"$EXEC_FILE" 
          
          #cp ../OdaFileConverter/* ../BUILD_ROOT/usr/local/bin/${DEST_NAME}_3.2/
          cp -f "${TRGDEST}/${TD_QT_TRG}" "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}/${DEST_NAME}"
          strip --strip-unneeded "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}/${DEST_NAME}"
          for DRX_TRG in $FRM_DLLS_LST
          do
            cp  -f "${TRGDEST}/${DRX_TRG}" "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}"
            if test "${DRX_TRG}" != "adinit.dat" ; then strip --strip-unneeded "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}/${DRX_TRG}" ; fi
          done
          
          mkdir $DEST_PKG_ROOT/usr/share
          ICON_DIRS="16x16 22x22 24x24 32x32 48x48 64x64"
          mkdir $DEST_PKG_ROOT/usr/share/icons
          mkdir $DEST_PKG_ROOT/usr/share/icons/hicolor
          for ICON_DIR in $ICON_DIRS
          do
            if test -f "${SV_PWD}/data/odalogo_${ICON_DIR}.png" ; then
              mkdir $DEST_PKG_ROOT/usr/share/icons/hicolor/$ICON_DIR
              mkdir $DEST_PKG_ROOT/usr/share/icons/hicolor/$ICON_DIR/apps
              cp -f "${SV_PWD}/data/odalogo_${ICON_DIR}.png" "${DEST_PKG_ROOT}/usr/share/icons/hicolor/$ICON_DIR/apps/${DEST_NAME}.png"
            fi
          done
          
          if test -f "/usr/bin/rpmbuild" ; then 
          
            #         
            if test -f "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}/TG_Db.tx" && test ! -f "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}/libTG_Db.so" ; then
              # to solve RPM check dependence only (I think it's a bug) - will be rename back in post step of installation
              #echo "***************************  ${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}  ************************************************"
              mv "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}/TG_Db.tx" "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}/libTG_Db.so" 
              #ls "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}"
              #echo "***************************"
            fi
            #
            #for DRX_TRG in $FRM_QT_DLLS_LST
            #do
            #  #cp -f /usr/bin/libQtCore.so ...
            #  cp  -f "${QT_PATH}/${DRX_TRG}" "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}"
            #done
          
            #if test -f "$SV_PWD/data/$DEST_NAME.desktop" ; then
              #mkdir $DEST_PKG_ROOT/usr/share
              mkdir $DEST_PKG_ROOT/usr/share/applications
              #cp -f "${SV_PWD}/data/${DEST_NAME}.desktop" "${DEST_PKG_ROOT}/usr/share/applications"
              DESKTOP="${DEST_PKG_ROOT}/usr/share/applications/${DEST_NAME}.desktop"
              echo "[Desktop Entry]" >"$DESKTOP"
              echo "X-SuSE-translate=true" >>"$DESKTOP"
              echo "Type=Application" >>"$DESKTOP"
              echo "Exec=${EXEC_NAME}" >>"$DESKTOP"
              echo "#MimeType=application/${DEST_NAME};" >>"$DESKTOP"
              echo "Icon=${DEST_NAME}" >>"$DESKTOP"
              echo "#X-DocPath=kdevelop/index.html" >>"$DESKTOP"
              echo "Terminal=false" >>"$DESKTOP"
              echo "Name=${EXEC_NAME}" >>"$DESKTOP"
              echo "GenericName=${DEST_NAME} v${ODAVERSION} - Open Design Alliance Inc." >>"$DESKTOP"
              #echo "Categories=Utility;Accessibility;" >>"$DESKTOP"  # "Utility;Application;" - Accessories
              echo "Categories=Graphics;3DGraphics;VectorGraphics;RasterGraphics;Viewer;" >>"$DESKTOP"
              # ® was removed from here because of it is bad-scaled on OpenSUSE desktops 
              echo "Comment=ODA SDK for .dwg files ${ODAVERSION}, Open Design Alliance Inc." >>"$DESKTOP"
              mkdir $DEST_PKG_ROOT/etc
              mkdir $DEST_PKG_ROOT/etc/skel
              mkdir $DEST_PKG_ROOT/etc/skel/Desktop
              cp -f "$DESKTOP" "${DEST_PKG_ROOT}/etc/skel/Desktop"
            #fi

            ###rpmbuild -bb --buildroot /_Oda/out/lnxX86_4.1dll/BUILD_ROOT --target i586 $DEST_NAME.spec
            #rpmbuild -bb --buildroot $DEST_PKG_ROOT --target i386 "${SV_PWD}/data/$DEST_NAME.spec"
            
            SPEC="$DEST_PKG_ROOT/../$DEST_NAME.spec"
            #cp -f "${SV_PWD}/data/$DEST_NAME.spec" "$SPEC"
            echo "# sequense installation : pre install+files post" >"$SPEC" 
            echo "#  sequense uninstallation : preun ... postun" >>"$SPEC" 
            echo "#" >>"$SPEC" 
            echo "# generate to /usr/src/packages/RPMS/i386/${DEST_NAME}-${ODAVERSION}-1.i386.rpm" >>"$SPEC" 
            echo "# install: root> rpm -ivh package_name.rpm" >>"$SPEC" 
            echo "# remove:  root> rpm -ev package_name" >>"$SPEC" 
            echo "# list:    root> rpm -qa package_name" >>"$SPEC" 
            echo "#" >>"$SPEC" 
            echo "# It's better use YaST or" >>"$SPEC" 
            echo "#    Dolphin (to install rpm)" >>"$SPEC" 
            echo "#    and Computer/Install Software (to uninstall rpm)" >>"$SPEC" 
            echo "#  to prevent problem with icon-cache etc" >>"$SPEC" 
            echo "" >>"$SPEC" 
            echo "# spec file for ${DEST_NAME} ${ODAVERSION}" >>"$SPEC" 
            DESCR="DESCRIPTION_${TD_QT_TRG}_1"
            eval "DESCR=\$${DESCR}"
            echo "Summary: ${DESCR}" >>"$SPEC" 
            echo "Name: ${DEST_NAME}" >>"$SPEC" 
            echo "Version: ${ODAVERSION}" >>"$SPEC" 
            echo "Release: 1" >>"$SPEC" 
            echo "Group: Application/Misc" >>"$SPEC" 
            echo "URL: http://www.opendesign.com/guestfiles" >>"$SPEC" 
            echo "Distribution: PCLinux" >>"$SPEC" 
            echo "Vendor: ODA" >>"$SPEC" 
            echo "Packager: ODA <oda@opendesign.com>" >>"$SPEC" 
            echo "License: Other" >>"$SPEC" 
            echo "#Copyright: ODA SDK® for .dwg files ${ODAVERSION}, Open Design Alliance Inc" >>"$SPEC" 
            echo "#Source0: ${DEST_NAME}-${ODAVERSION}.tar.gz" >>"$SPEC" 
            echo "" >>"$SPEC" 
            echo "%description" >>"$SPEC" 
            DESCR="DESCRIPTION_${TD_QT_TRG}_2"
            eval "DESCR=\$${DESCR}"
            echo "  ${DESCR}" >>"$SPEC" 
            echo "" >>"$SPEC" 
            echo "#%pre" >>"$SPEC" 
            echo "" >>"$SPEC" 
            echo "#%install" >>"$SPEC" 
            echo "" >>"$SPEC" 
            echo "%post" >>"$SPEC" 
            echo "chmod 777 /usr/local/bin/${EXEC_NAME}*" >>"$SPEC" 
            echo "chmod 777 /usr/local/bin/${DEST_NAME}_${ODAVERSION}* -R" >>"$SPEC" 
            echo "chmod 666 /etc/skel/Desktop/${DEST_NAME}.desktop" >>"$SPEC" 
            echo "ldconfig" >>"$SPEC" 
            echo "rm -f /home/*/Desktop/${DEST_NAME}.desktop" >>"$SPEC" 
            echo "#cp -f /etc/skel/Desktop/${DEST_NAME}.desktop /home/*/Desktop" >>"$SPEC" 
            echo "find /home/*/Desktop -type d -exec cp -f /etc/skel/Desktop/${DEST_NAME}.desktop '{}' ';'" >>"$SPEC" 
            echo "cp -f /etc/skel/Desktop/${DEST_NAME}.desktop /root/Desktop" >>"$SPEC" 
            echo "echo post completed" >>"$SPEC" 
            echo "#%update-desktop-database-post" >>"$SPEC" 
            echo "#if [ -x update-desktop-database ]; then update-desktop-database &> /dev/null; fi" >>"$SPEC" 
            echo "if test ! -f /usr/local/bin/${DEST_NAME}_${ODAVERSION}/TG_Db.tx && test -f /usr/local/bin/${DEST_NAME}_${ODAVERSION}/libTG_Db.so ; then" >>"$SPEC" 
            echo "  # was renamed to solve RPM check dependence only" >>"$SPEC" 
            echo "  mv /usr/local/bin/${DEST_NAME}_${ODAVERSION}/libTG_Db.so /usr/local/bin/${DEST_NAME}_${ODAVERSION}/TG_Db.tx # rename back to native name " >>"$SPEC" 
            echo "fi" >>"$SPEC" 
            echo "" >>"$SPEC" 
            echo "%files" >>"$SPEC" 
            echo "%defattr(-,root,root,-)" >>"$SPEC" 
            echo "/usr/local/bin/${DEST_NAME}" >>"$SPEC" 
            echo "/usr/local/bin/${DEST_NAME}*/*" >>"$SPEC" 
            echo "#OpenSuse 11.2" >>"$SPEC" 
            echo "#/usr/share/applications/kde4/${DEST_NAME}.desktop" >>"$SPEC" 
            echo "# or" >>"$SPEC" 
            echo "/usr/share/applications/${DEST_NAME}.desktop" >>"$SPEC" 
            echo "/usr/share/icons/hicolor/*/apps/*.png" >>"$SPEC" 
            echo "#" >>"$SPEC" 
            echo "# add desktop shortcuts in ~/Desktop" >>"$SPEC" 
            echo "#/home/xxx/Desktop/${DEST_NAME}.desktop " >>"$SPEC" 
            echo "# for all users :" >>"$SPEC" 
            echo "#/etc/skeleton/Desktop/${DEST_NAME}.desktop" >>"$SPEC" 
            echo "/etc/skel/Desktop/${DEST_NAME}.desktop " >>"$SPEC" 
            echo "" >>"$SPEC" 
            echo "%clean" >>"$SPEC" 
            echo "rm -rf $RPM_BUILD_ROOT" >>"$SPEC" 
            echo "" >>"$SPEC" 
            echo "%preun" >>"$SPEC" 
            echo "#if test -f /usr/local/bin/${DEST_NAME}_${ODAVERSION}/TG_Db.tx ; then" >>"$SPEC" 
            echo "#  rm -f /usr/local/bin/${DEST_NAME}_${ODAVERSION}/TG_Db.tx" >>"$SPEC" 
            echo "#fi" >>"$SPEC" 
            echo "if test -f /usr/local/bin/${DEST_NAME}_${ODAVERSION}/TG_Db.tx && test ! -f /usr/local/bin/${DEST_NAME}_${ODAVERSION}/libTG_Db.so ; then" >>"$SPEC" 
            echo "  # rename back for uninstall" >>"$SPEC" 
            echo "  mv /usr/local/bin/${DEST_NAME}_${ODAVERSION}/TG_Db.tx /usr/local/bin/${DEST_NAME}_${ODAVERSION}/libTG_Db.so" >>"$SPEC" 
            echo "fi" >>"$SPEC" 
            echo "" >>"$SPEC" 
            echo "%postun" >>"$SPEC" 
            echo "rm -f /home/*/Desktop/${DEST_NAME}.desktop" >>"$SPEC" 
            echo "rm -f /root/Desktop/${DEST_NAME}.desktop" >>"$SPEC" 
            echo "#%update-desktop-database-postun" >>"$SPEC" 
            echo "#if [ -x update-desktop-database ]; then update-desktop-database &> /dev/null; fi" >>"$SPEC" 
            echo "rmdir $RPM_BUILD_ROOT/usr/local/bin/${DEST_NAME}_${ODAVERSION}" >>"$SPEC" 
            echo "rm -f $RPM_BUILD_ROOT/usr/local/bin/${EXEC_NAME}" >>"$SPEC" 
            echo "#" >>"$SPEC" 
            echo "#rm -f /home/*/.local/share/applications/${DEST_NAME}.desktop" >>"$SPEC" 
            echo "#rm -f /home/*/.kde4/share/apps/RecentDocuments/${DEST_NAME}*.*" >>"$SPEC" 
            echo "" >>"$SPEC" 
            
            rpmbuild -bb --buildroot $DEST_PKG_ROOT --target i386 "$SPEC"
            
            #how to find RPM? #ODADrawingsExplorer-3.4.0-1.i386.rpm
            #mv /usr/src/packages/RPMS/i386/$DEST_NAME*.rpm "${TD_RPM_DIR}/${TD_CONF_NAME}"
            mv -f "/usr/src/packages/RPMS/i386/${DEST_NAME}-${ODAVERSION}-1.i386.rpm" "${TD_RPM_DIR}/${TD_CONF_NAME}/${DEST_NAME}-${ODAVERSION}.i386.rpm"
            #rm -R "${TD_RPM_DIR}/${TD_CONF_NAME}/__rpm__${DEST_NAME}"
          else  

            #for DRX_TRG in $FRM_QT_DLLS_LST
            #do
            #  cp  -f "${QT_PATH}/${DRX_TRG}" "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}"
            #  strip --strip-unneeded "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}/${DRX_TRG}"
            #done

            #mkdir "${DEST_PKG_ROOT}/usr/share"
            mkdir "${DEST_PKG_ROOT}/usr/share/applications"
            # create shortcut
            DESKTOP="${DEST_PKG_ROOT}/usr/share/applications/${DEST_NAME}_${ODAVERSION}.desktop"
            echo "[Desktop Entry]" >"$DESKTOP"
            echo "Name=${EXEC_NAME}" >>"$DESKTOP"
            echo "Comment=ODA SDK® for .dwg files ${ODAVERSION}, Open Design Alliance Inc." >>"$DESKTOP"
            echo "TryExec=${EXEC_BIN}/${EXEC_NAME}" >>"$DESKTOP"
            echo "Exec=${EXEC_BIN}/${EXEC_NAME} %U" >>"$DESKTOP"
            #echo "Icon=${DEST_NAME}_${ODAVERSION}.png" >>"$DESKTOP"
            echo "Icon=${DEST_NAME}" >>"$DESKTOP"
            echo "Terminal=false" >>"$DESKTOP"
            echo "Type=Application" >>"$DESKTOP"
            #echo "Version=${ODAVERSION}" >>"$DESKTOP"
            #TODO because of Licence questions #echo "MimeType=application/dwg;drawing/dwg;" >>"$DESKTOP"
            #echo "Encoding=UTF-8" >>"$DESKTOP"
            echo "Categories=Graphics;3DGraphics;VectorGraphics;RasterGraphics;Viewer;" >>"$DESKTOP" # "Utility;Application;" - Accessories
            #mkdir "${DEST_PKG_ROOT}/usr/share/doc"
            ##mkdir "${DEST_PKG_ROOT}/usr/share/icons"
            #cp -f "${SV_PWD}/data/odalogo_64x64.png" "${DEST_PKG_ROOT}/usr/share/icons/${DEST_NAME}_${ODAVERSION}.png"
            #mkdir "${DEST_PKG_ROOT}/usr/share/pixmaps"
            #cp -f "${SV_PWD}/data/odalogo_64x64.png" "${DEST_PKG_ROOT}/usr/share/pixmaps/${DEST_NAME}_${ODAVERSION}.png"
            chmod 755 "$DESKTOP"

            if test -f "${SV_PWD}/data/copyright_${DEST_NAME}" ; then
              if test ! -d "${DEST_PKG_ROOT}/usr/share/doc" ; then mkdir "${DEST_PKG_ROOT}/usr/share/doc" ; fi
              if test ! -d "${DEST_PKG_ROOT}/usr/share/doc/${DEST_NAME}" ; then mkdir "${DEST_PKG_ROOT}/usr/share/doc/${DEST_NAME}" ; fi
              cp -f "${SV_PWD}/data/copyright_${DEST_NAME}" "${DEST_PKG_ROOT}/usr/share/doc/${DEST_NAME}/copyright"
            fi
            if test -f "${SV_PWD}/data/readme_${DEST_NAME}" ; then
              if test ! -d "${DEST_PKG_ROOT}/usr/share/doc" ; then mkdir "${DEST_PKG_ROOT}/usr/share/doc" ; fi
              if test ! -d "${DEST_PKG_ROOT}/usr/share/doc/${DEST_NAME}" ; then mkdir "${DEST_PKG_ROOT}/usr/share/doc/${DEST_NAME}" ; fi
              cp -f "${SV_PWD}/data/readme_${DEST_NAME}" "${DEST_PKG_ROOT}/usr/share/doc/${DEST_NAME}/README"
            fi
            
            mkdir "$DEST_PKG/DEBIAN"

            # format version of deb-package
            #echo "2.0" >"${DEST_PKG}/debian-binary"

            # create DEBIAN/control :
            echo "Package: ${DEST_NAME}" >"${DEST_PKG}/DEBIAN/control"
            echo "Version: ${ODAVERSION}" >>"${DEST_PKG}/DEBIAN/control"
            echo "Section: non-free/graphics" >>"${DEST_PKG}/DEBIAN/control"
            echo "Priority: optional" >>"${DEST_PKG}/DEBIAN/control" 
            echo "Architecture: i386" >>"${DEST_PKG}/DEBIAN/control"
            DEPENDS=$(sh ${SV_PWD}/make_depends.sh "${DEST_PKG_BIN}/${DEST_NAME}_${ODAVERSION}")
            if [ "$DEPENDS" = "" ] ; then DEPENDS="DEPENDS_${TD_QT_TRG}" ; eval "DEPENDS=\$${DEPENDS}" ; fi
            if test "$DEPENDS" != "" ; then echo "Depends: ${DEPENDS}" >>"${DEST_PKG}/DEBIAN/control" ; fi
            echo -n "Installed-Size: " >>"${DEST_PKG}/DEBIAN/control"
            cd "${DEST_PKG_ROOT}"
            #  du -sh usr | grep 'usr' | sed s/usr//
            du -s usr | sed s/usr// >>"${DEST_PKG}/DEBIAN/control"
            echo "Maintainer: ODA <oda@opendesign.com>" >>"${DEST_PKG}/DEBIAN/control"
            #echo "Bugs: http://www.opendesign.com/guestfiles" >>"${DEST_PKG}/DEBIAN/control"
            echo "Homepage: http://www.opendesign.com/guestfiles" >>"${DEST_PKG}/DEBIAN/control"
            echo "Type of Package: single binary" #echo "Type of Package : Single"
            
            DESCR="DESCRIPTION_${TD_QT_TRG}"
            eval "DESCR=\$${DESCR}"
            echo "Description: ${DESCR}" >>"${DEST_PKG}/DEBIAN/control"
            DESCR_INDEX_LIST="1 2 3 4 5 6 7 9 10"
            for DESCR_INDEX in $DESCR_INDEX_LIST
            do
              DESCR="DESCRIPTION_${TD_QT_TRG}_${DESCR_INDEX}"
              eval "DESCR=\$${DESCR}"
              if test "$DESCR" != "" ; then 
                echo " ${DESCR}" >>"${DEST_PKG}/DEBIAN/control"
                continue
              fi
              break
            done
            chmod 755 "${DEST_PKG}/DEBIAN/control"
            
            # create DEBIAN/postinst :
            # create lancher on desktop 
            echo "#! /bin/sh" >"${DEST_PKG}/DEBIAN/postinst"
            echo "set -e" >>"${DEST_PKG}/DEBIAN/postinst" 
            echo "" >>"${DEST_PKG}/DEBIAN/postinst"
            #echo "echo \"sh postinst $1\" >>\"/home/${DEST_NAME}.log\"" >>"${DEST_PKG}/DEBIAN/postinst"
            #echo "if [ \"\$1\"!=\"-o\" ]; then exit 0 ; fi" >>"${DEST_PKG}/DEBIAN/postinst"
            echo "for USER in /home/* ; do" >>"${DEST_PKG}/DEBIAN/postinst"
            echo "  if test ! -d \"\$USER/Desktop\" ; then continue ; fi" >>"${DEST_PKG}/DEBIAN/postinst"
            echo "  cp -f /usr/share/applications/${DEST_NAME}_${ODAVERSION}.desktop \"\$USER/Desktop\"" >>"${DEST_PKG}/DEBIAN/postinst"
            echo "  chmod 777 \"\$USER/Desktop/${DEST_NAME}_${ODAVERSION}.desktop\"" >>"${DEST_PKG}/DEBIAN/postinst"
            echo "done" >>"${DEST_PKG}/DEBIAN/postinst"
            echo "exit 0" >>"${DEST_PKG}/DEBIAN/postinst" 
            chmod 755 "${DEST_PKG}/DEBIAN/postinst"
            
            # create DEBIAN/postrm :
            echo "#! /bin/sh" >"${DEST_PKG}/DEBIAN/postrm"
            echo "set -e" >>"${DEST_PKG}/DEBIAN/postrm" 
            echo "" >>"${DEST_PKG}/DEBIAN/postrm"
            #echo "echo \"sh postrm $1\" >>\"/home/${DEST_NAME}.log\"" >>"${DEST_PKG}/DEBIAN/postrm"
            #echo "if [ \"\$1\"!=\"-o\" ] ; then exit 0 ; fi" >>"${DEST_PKG}/DEBIAN/postrm"
            echo "for USER in /home/* ; do" >>"${DEST_PKG}/DEBIAN/postrm"
            echo "  if test ! -f \"\$USER/Desktop/${DEST_NAME}_${ODAVERSION}.desktop\" ; then continue ; fi" >>"${DEST_PKG}/DEBIAN/postrm"
            echo "  rm -f \"\$USER/Desktop/${DEST_NAME}_${ODAVERSION}.desktop\"" >>"${DEST_PKG}/DEBIAN/postrm"
            echo "done" >>"${DEST_PKG}/DEBIAN/postrm"
            echo "exit 0" >>"${DEST_PKG}/DEBIAN/postrm" 
            chmod 755 "${DEST_PKG}/DEBIAN/postrm"

            #sudo aptitude install dpkg-shlibdeps
            
            #fakeroot ls -l "${DEST_PKG}"
            
            #dpkg-deb -b "$DEST_PKG" #"${TD_RPM_DIR}/${TD_CONF_NAME}/${DEST_NAME}-${ODAVERSION}"
            cd "${TD_RPM_DIR}/${TD_CONF_NAME}"
            fakeroot dpkg-deb -b "${DEST_NAME}-${ODAVERSION}" .

            # check result
            #sudo apt-get install lintian    #sudo aptitude install lintian
            DEST_NAME_LOWER="$DEST_NAME"
            DEST_NAME_LOWER=$(echo "$DEST_NAME_LOWER" | tr [:upper:] [:lower:])
            lintian "${DEST_NAME_LOWER}_${ODAVERSION}_i386.deb"
            #lintian -c "${DEST_NAME}_${ODAVERSION}_i386.deb"
           
            #rm -R "${DEST_PKG}"
          fi
          #cd "${SV_PWD}"
         
        fi
      fi
    done
  fi
done
