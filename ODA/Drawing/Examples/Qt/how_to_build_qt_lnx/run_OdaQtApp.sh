#! /bin/sh

if test ! -n "${ODADIR}" ; then 

  export ODADIR="/_Oda/trunk"

  # http://www.softpanorama.org/Scripting/Shellorama/Control_structures/case_statement.shtml
  case "${PWD}" in 
  */Drawing/Examples/Qt/how_to_build_qt_lnx) 
    export ODADIR="${PWD%/Drawing/Examples/Qt/how_to_build_qt_lnx}"
    #echo "*** \"$ODADIR\"  ***"
    ;;
  esac
fi

# debug
if test -d "${ODADIR}/bin/lnxX86_4.1dlldbg" ; then APP_PATH=$ODADIR/bin/lnxX86_4.1dlldbg ; fi
if test -d "${ODADIR}/bin/lnxX86_4.2dlldbg" ; then APP_PATH=$ODADIR/bin/lnxX86_4.2dlldbg ; fi
if test -d "${ODADIR}/bin/lnxX86_4.4dlldbg" ; then APP_PATH=$ODADIR/bin/lnxX86_4.4dlldbg ; fi
if test -d "${ODADIR}/bin/lnxX86_4.7dlldbg" ; then APP_PATH=$ODADIR/bin/lnxX86_4.7dlldbg ; fi
if test -d "${ODADIR}/bin/lnxX86_4.8dlldbg" ; then APP_PATH=$ODADIR/bin/lnxX86_4.8dlldbg ; fi
if test -d "${ODADIR}/bin/lnxX86_4.9dlldbg" ; then APP_PATH=$ODADIR/bin/lnxX86_4.9dlldbg ; fi

# release
if test -d "${ODADIR}/bin/lnxX86_4.1dll" ; then APP_PATH=$ODADIR/bin/lnxX86_4.1dll ; fi
if test -d "${ODADIR}/bin/lnxX86_4.2dll" ; then APP_PATH=$ODADIR/bin/lnxX86_4.2dll ; fi
if test -d "${ODADIR}/bin/lnxX86_4.4dll" ; then APP_PATH=$ODADIR/bin/lnxX86_4.4dll ; fi
if test -d "${ODADIR}/bin/lnxX86_4.7dll" ; then APP_PATH=$ODADIR/bin/lnxX86_4.7dll ; fi
if test -d "${ODADIR}/bin/lnxX86_4.8dll" ; then APP_PATH=$ODADIR/bin/lnxX86_4.8dll ; fi
if test -d "${ODADIR}/bin/lnxX86_4.9dll" ; then APP_PATH=$ODADIR/bin/lnxX86_4.9dll ; fi

SV_LD_LIBRARY_PATH=$LD_LIBRARY_PATH
#export LD_LIBRARY_PATH=.:$SV_LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$APP_PATH
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"

$APP_PATH/OdaQtApp

#restore
export LD_LIBRARY_PATH=$SV_LD_LIBRARY_PATH
#echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
