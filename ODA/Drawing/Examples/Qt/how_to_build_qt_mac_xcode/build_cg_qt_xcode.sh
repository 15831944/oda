#! /bin/sh

if test ! -n "${ODADIR}" ; then export ODADIR="/_Oda/trunk" ; fi
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

TD_QT_PRJ_LIST=""
TD_QT_PRJ_LIST="${TD_QT_PRJ_LIST} OdaQtOpenRL.txv"

#------

for TD_CONF_NAME in $TD_CONF_LIST
do
  TD_CONF_PATH="${ODABIN}/${TD_CONF_NAME}"
  if test ! -d "${TD_CONF_PATH}" ; then continue ; fi

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
    
    if test -d $PRJDEST/$TD_QT_PRJ$TD_PRO_SUFFIX.xcodeproj ; then
      #echo "cd ${DEST}/${TD_QT_PRJ}"
      cd $PRJDEST
  
      echo "Build ${TD_QT_PRJ} $TD_PRO_SUFFIX xcodeproj"
      
      if test -f "${ODADIR}/Drawing/Examples/Qt/${TD_QT_PRJ}/Info.plist" ; then 
        cp -f "${ODADIR}/Drawing/Examples/Qt/${TD_QT_PRJ}/Info.plist" "${PRJDEST}/Info.plist" 
        #echo "  >xcodebuild -project $TD_QT_PRJ$TD_PRO_SUFFIX.xcodeproj -configuration $TD_PRO_SUFFIX >$PRJDEST/$TD_QT_PRJ_$TD_PRO_SUFFIX.log"
        
        xcodebuild -project $TD_QT_PRJ$TD_PRO_SUFFIX.xcodeproj -configuration $TD_PRO_SUFFIX >"${PRJDEST}/${TD_QT_PRJ}_${TD_PRO_SUFFIX}.log"
        # to prevent using it by other app
        rm -f "${PRJDEST}/Info.plist"
      else
        #echo "  >xcodebuild -project $TD_QT_PRJ$TD_PRO_SUFFIX.xcodeproj -configuration $TD_PRO_SUFFIX >$PRJDEST/$TD_QT_PRJ_$TD_PRO_SUFFIX.log"
        
        XCODE_SETTINNGS=
        if test "$TX_EXT" != "" ; then 
          XCODE_SETTINNGS="EXECUTABLE_EXTENSION=$TX_EXT PRODUCT_NAME=$TD_QT_PRJ EXECUTABLE_SUFFIX=.$TX_EXT"
          #echo "XCODEBUILD -project $TD_QT_PRJ$TD_PRO_SUFFIX.xcodeproj -configuration $TD_PRO_SUFFIX $XCODE_SETTINNGS"
        fi
        xcodebuild -project $TD_QT_PRJ$TD_PRO_SUFFIX.xcodeproj -configuration $TD_PRO_SUFFIX $XCODE_SETTINNGS >"${PRJDEST}/${TD_QT_PRJ}_${TD_PRO_SUFFIX}.log"

      fi        

      cd $SV_PWD
    fi
  done
	
done
