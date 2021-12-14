#! /bin/sh

if test ! -n "${ODADIR}" ; then export ODADIR="/_Oda/trunk" ; fi
if test ! -n "${ODADLLSUFFIX}" ; then export ODADLLSUFFIX="" ; fi
# useless for framework version #if test ! -n "${QTDIR}" ; then export QTDIR="/usr/local/Qt4.5" ; fi
# see Developer/Examples/Qt

if test ! -d "${ODADIR}/Dgn" ; then
  echo "  Folder ${ODADIR}/Dgn was not found"
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
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} DgnDb"

#------

for TD_CONF_NAME in $TD_CONF_LIST
do
  TD_CONF_PATH="${ODABIN}/${TD_CONF_NAME}"
  if test ! -d "${TD_CONF_PATH}" ; then continue ; fi
  
  export CONF_NAME=$TD_CONF_NAME
  export TD_PRO_SUFFIX="Release"
  case "${TD_CONF_NAME}" in
    *dbg) export TD_PRO_SUFFIX="Debug" ;;
  esac
  #set >"${TD_CONF_NAME}.txt"

  # solution for linking with TG_Db.tx was moved to build_tg_qt_xcode to prevent
  # possible incorrect relinking of TD_DgnDb.tx to libTG_Db.dylib via "relink_td_qt_xcode.sh -r"
  
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
    sh make_xcode.sh $TD_QT_PRJ$TD_PRO_SUFFIX "${PRJDEST}/TD_"

      rm -f make_xcode.sh 
    cd $SV_PWD
  done
done

#set >test.txt
