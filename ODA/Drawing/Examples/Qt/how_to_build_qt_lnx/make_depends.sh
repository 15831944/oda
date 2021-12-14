#!/bin/sh

SRC_DIR="${PWD}"
#@echo on

if [ "$1" != "" ] ; then SRC_DIR="$1" ; fi
if test ! -d $SRC_DIR ; then exit 0 ; fi
cd $SRC_DIR

TMPFILE=$(mktemp -t -p .)
rm -f "$TMPFILE"
TMPFILE="${TMPFILE}.tmp"
#TMPFILE="tmp_file.tmp"
#echo "TMPFILE = $TMPFILE"

#echo "PATH = $SRC_DIR/*"
#
#for FL in $SRC_DIR/*
FILE_LST=$(ls -b | cut -d: -f1) # | fgrep executable) | cut -d: -f1)
for FL in $FILE_LST
#
do
  #echo "test1-$FL"
  case "$FL" in
  *.dat) continue ;;
  *.sh) continue ;;
  *.tmp) continue ;;
  esac
  if [ "$FL" = "" ] ; then continue ; fi
  #echo "test2-\"$FL\""
  
  DEPS_LST=$(ldd $FL | awk '/=>/ {print $1}')
  #if [ "$DEPS_LST" = "" ] || [ -L $FL ]; then continue  ; fi
  
  for DEP in $DEPS_LST
  do
    if [ -L $FL ]; then continue  ; fi
    for FL_ in $FILE_LST
    do
      eval "case \"${DEP}\" in $FL_*) DEP=\"\" ;; esac"
      if [ "$DEP" = "" ] ; then DEP="" ; break ; fi
      #echo "test3=\"$DEP_\""
  
    done
    if [ "$DEP" = "" ] ; then continue  ; fi

    #echo "\"$DEP\"" #test
    #echo -n " $DEP"
    echo "$DEP"
  done
  
  ##READELF=$(type readelf 2> /dev/null)
  #SONAME=$(objdump -p $FL | awk '/SONAME/ {print $2}')
  #if [ "$SONAME" = "" ]; then 
  #  #echo "${FL##*/}"
  #  continue
  #fi
  #if [ ! -L $FL ]; then echo "$SONAME" ; fi
  
done | sort -u >"$TMPFILE"

SONAMES_LST=$(cat -s $TMPFILE)
rm -f "$TMPFILE"

for SONAME in $SONAMES_LST
do
  #echo "SONAME=\"$SONAME\":"
  
  cd "/var/lib/dpkg/info"
  FILE_LST=$(ls -b *.symbols)
  for FL in $FILE_LST
  do
    INFO_LST=$(grep -e $SONAME $FL)
    if [ "$INFO_LST" = "" ] ; then continue ; fi
    #echo "  INFO_LST=\"$INFO_LST\":"
    
    SONAME_=""
    PACKAGENAME=""
    for INFO in $INFO_LST 
    do
      if [ "$SONAME_" = "" ] ; then SONAME_="$INFO" ; continue ; fi
      PACKAGENAME="$INFO"
      break 
    done
    if [ "$PACKAGENAME" = "" ] ; then 
      # package of $SONAME_ was not found
      break 
    fi
 
    #echo "(FL=\"$FL\")"
    FLVER=$(echo $FL | sed s/.symbols/.shlibs/)
    #echo "(FLVER=\"$FLVER\")"
    VER_LST=$(grep -e $PACKAGENAME $FLVER)
    #echo "(VER_LST=\"$VER_LST\")"

    PACKAGEGLOBALNAME=""
    VERMAJOR=""
    PACKAGENAME_=""
    VERLEFT=""
    VERRIGHT=""
    for VER in $VER_LST 
    do
      if [ "$PACKAGEGLOBALNAME" = "" ] ; then PACKAGEGLOBALNAME="$VER" ; continue ; fi
      if [ "$VERMAJOR" = "" ] ; then VERMAJOR="$VER" ; continue ; fi
      if [ "$PACKAGENAME_" = "" ] ; then PACKAGENAME_="$VER" ; continue ; fi
      if [ "$PACKAGENAME" != "$PACKAGENAME_" ] ; then break ; fi 
      if [ "$VER" = "udeb:" ] ; then break ; fi 
      if [ "$VERLEFT" = "" ] ; then VERLEFT="$VER" ; continue ; fi
      if [ "$VERLEFT" != "(>=" ] ; then break ; fi
      
      VERRIGHT="$VER" 
      break
    done
    if [ "$VERRIGHT" = "" ] ; then 
      echo "${PACKAGENAME}"
    else
      echo "${PACKAGENAME}#(>=#${VERRIGHT}"
    fi
    break;
  done
done | sort -u >"$TMPFILE"

DEPENDS_LST=$(cat -s $TMPFILE)
rm -f "$TMPFILE"

DEPENDS=""
for DEP in $DEPENDS_LST
do
  if [ "$DEPENDS" != "" ] ; then DEPENDS="${DEPENDS}, " ; fi
  DEP=$(echo $DEP | sed s/#/\ /g)
  DEPENDS="${DEPENDS}${DEP}"
done

echo "$DEPENDS"
