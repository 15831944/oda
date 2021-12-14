#! /bin/sh

if test ! -n "${ODADIR}" ; then export ODADIR="/_Oda/trunk" ; fi
if test ! -n "${ODADLLSUFFIX}" ; then export ODADLLSUFFIX="" ; fi
# useless for framework version #if test ! -n "${QTDIR}" ; then export QTDIR="/usr/local/Qt4.5" ; fi
# see Developer/Examples/Qt

if test ! -d "${ODADIR}/Core" ; then
  echo "  Folder ${ODADIR}/Core was not found"
  exit 0
fi

#if test ! -n "${ODABIN}" ; then export ODABIN="${ODADIR}/lib" ; fi
if test ! -n "${ODABIN}" ; then export ODABIN="${ODADIR}/bin" ; fi

TD_CONF_LIST=""
TD_CONF_LIST="${TD_CONF_LIST} macOsX_x86_10.5dll macOsX_x86_10.5dlldbg macOsX_x86_10.5dyn macOsX_x86_10.5dyndbg"
TD_CONF_LIST="${TD_CONF_LIST} macOsX_x86_10.6dll macOsX_x86_10.6dlldbg macOsX_x86_10.6dyn macOsX_x86_10.6dyndbg"
TD_CONF_LIST="${TD_CONF_LIST} macOsX_x86_10.7dll macOsX_x86_10.7dlldbg macOsX_x86_10.7dyn macOsX_x86_10.7dyndbg"

#------

SV_PWD="${PWD}"
DEST="${ODADIR}/Drawing/Examples/Qt"

TD_QT_PRJ_LIST=""
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaFileConverter"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtApp"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtConsole"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtViewSystem"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtOpenGL" 
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtPlotSettingsValidator"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtPropSystem"
#TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtObjects"

#------

for TD_CONF_NAME in $TD_CONF_LIST
do
  TD_CONF_PATH="${ODABIN}/${TD_CONF_NAME}"
  if test ! -d "${TD_CONF_PATH}" ; then continue ; fi
  if test ! -d "${TD_CONF_PATH}/_flg_abs_paths" ; then sh relink_td_qt_xcode.sh -r ; fi
  
  export CONF_NAME=$TD_CONF_NAME
  export TD_PRO_SUFFIX="Release"
  case "${TD_CONF_NAME}" in
    *dbg) export TD_PRO_SUFFIX="Debug" ;;
  esac
  #set >"${TD_CONF_NAME}.txt"

  if test ! -f "${TD_CONF_PATH}/adinit.dat" ; then
    # fix for static library TD_Key without standard prefix (problem of CMAKE building)
    if test -f "${ODADIR}/Kernel/adinit.dat" ; then cp -f "${ODADIR}/Kernel/adinit.dat" "${TD_CONF_PATH}/adinit.dat" ; fi
  fi
  
  if test ! -f "${TD_CONF_PATH}/libTD_Key.a" ; then 
    if test -f "${ODADIR}/lib/${TD_CONF_NAME}/libTD_Key.a" ; then 
      cp -f "${ODADIR}/lib/${TD_CONF_NAME}/libTD_Key.a" "${TD_CONF_PATH}/libTD_Key.a"
    else # for v <= 3.3.1
      # fix for static library TD_Key without standard prefix (problem of CMAKE building)
      cp -f "${ODADIR}/lib/${TD_CONF_NAME}/TD_Key.a" "${TD_CONF_PATH}/libTD_Key.a"
    fi
  fi
  
  PRJDEST="${TD_CONF_PATH}/Examples/Qt"
  if test ! -d "${TD_CONF_PATH}/Examples" ; then mkdir "${TD_CONF_PATH}/Examples" ; fi
  if test ! -d $PRJDEST ; then mkdir $PRJDEST ; fi
  echo "in ${PRJDEST} :"

  for TD_QT_PRJ in $TD_QT_PRJ_LIST
  do
    cp -f $SV_PWD/make_xcode.sh $DEST/$TD_QT_PRJ
    #echo "cd ${DEST}/${TD_QT_PRJ}"
    cd $DEST/$TD_QT_PRJ
  
    echo "  Generate ${TD_QT_PRJ} $TD_PRO_SUFFIX xcodeproj"
    #echo "  >sh make_xcode.sh ${TD_QT_PRJ}${TD_PRO_SUFFIX} ${PRJDEST}/"
    export PRJDEST
    export TD_CONF_NAME
    export PRJ2SRC_LEV_OFFSET=""
    sh make_xcode.sh $TD_QT_PRJ$TD_PRO_SUFFIX "${PRJDEST}/"

      rm -f make_xcode.sh 
    cd $SV_PWD
  done
done

#set >test.txt
