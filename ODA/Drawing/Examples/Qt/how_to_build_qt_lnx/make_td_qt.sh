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

#------

SV_PWD="${PWD}"
DEST="${ODADIR}/Drawing/Examples/Qt"

TD_QT_PRJ_LIST=""
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaFileConverter"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtApp"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtConsole"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtViewSystem"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtGles"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtOpenGL" 
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtPlotSettingsValidator"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtPropSystem"
#TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtObjects"

#------

for TD_CONF_NAME in $TD_CONF_LIST
do
  TD_CONF_PATH="${ODABIN}/${TD_CONF_NAME}"
  if test -d "${TD_CONF_PATH}"
  then
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
      cp -f $SV_PWD/make_lnx_g++.sh $DEST/$TD_QT_PRJ
      #echo "cd ${DEST}/${TD_QT_PRJ}"
      cd $DEST/$TD_QT_PRJ
    
	    echo "  Generate ${TD_QT_PRJ}${TD_PRO_SUFFIX}/Makefile"
      if test ! -d "${PRJDEST}/${TD_QT_PRJ}${TD_PRO_SUFFIX}" ; then mkdir "${PRJDEST}/${TD_QT_PRJ}${TD_PRO_SUFFIX}" ; fi
	    #echo "  >sh make_lnx_g++.sh.sh Makefile  ${PRJDEST}/${TD_QT_PRJ}${TD_PRO_SUFFIX}/"
      export PRJDEST
      export TD_CONF_NAME
      export PRJ2SRC_LEV_OFFSET="../"
	    sh make_lnx_g++.sh "${TD_QT_PRJ}${TD_PRO_SUFFIX}" "${PRJDEST}/${TD_QT_PRJ}${TD_PRO_SUFFIX}/"
	
      rm -f make_lnx_g++.sh 
	    cd $SV_PWD
    done
  fi
done

#set >test.txt
