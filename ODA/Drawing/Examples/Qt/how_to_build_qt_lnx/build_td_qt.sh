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

TD_QT_PRJ_LIST=""
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaFileConverter"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtApp"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtConsole"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtViewSystem"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtPropSystem"
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtGLES2.txv" 
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtOpenGL.txv" 
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtPlotSettingsValidator"
#TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtObjects.tx"

#------

for TD_CONF_NAME in $TD_CONF_LIST
do
  if test -d "${ODABIN}/${TD_CONF_NAME}"
  then
    export TD_PRO_SUFFIX="Release"
    case "${TD_CONF_NAME}" in
      *dbg) export TD_PRO_SUFFIX="Debug" ;;
    esac
    #set >"${TD_CONF_NAME}.txt"
	
    PRJDEST="${ODABIN}/${TD_CONF_NAME}/Examples/Qt"
	
    for TD_QT_PRJ in $TD_QT_PRJ_LIST
    do
    
      TX_EXT=""
      TX_PREF=""  # TODO
      case "${TD_QT_PRJ}" in 
      TD_*.*) 
        TX_EXT="${TD_QT_PRJ#*.}"
        #echo "TX_EXT=$TX_EXT"
        TX_PREF="TD_"
        #echo "TX_PREF=$TX_PREF"
        TD_QT_PRJ="${TD_QT_PRJ%.*}"
        TD_QT_PRJ="${TD_QT_PRJ#TD_}"
        #echo "TX_NAME=$TD_QT_PRJ"
        ;;
      *.*) 
        TX_EXT="${TD_QT_PRJ#*.}"
        #echo "TX_EXT=$TX_EXT"
        TD_QT_PRJ="${TD_QT_PRJ%.*}"
        #echo "TX_NAME=$TD_QT_PRJ"
        ;;
      esac
    
      if test -f $PRJDEST/$TD_QT_PRJ$TD_PRO_SUFFIX/Makefile ; then
        echo "cd ${PRJDEST}/${TD_QT_PRJ}${TD_PRO_SUFFIX}"
        cd $PRJDEST/$TD_QT_PRJ$TD_PRO_SUFFIX
    
	      echo "  Build ${TD_QT_PRJ}${TD_PRO_SUFFIX} makefile"
	      #echo "  >make -f Makefile  -configuration $TD_PRO_SUFFIX >$PRJDEST/$TD_QT_PRJ_$TD_PRO_SUFFIX.log"
	      make >"${PRJDEST}/${TD_QT_PRJ}_${TD_PRO_SUFFIX}.log" 2>&1
  
	      cd $SV_PWD
      fi
    done
	
  fi
done
