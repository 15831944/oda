
CONVERTEDBY = $$(QMAKESPEC)
isEqual(CONVERTEDBY, "") {

  #####################################################
  #
  # uses for alternative direct building with QtCreator
  #
  # attention: this way is additonal and is not recommended.
  #            you should redefine variables ODADLLSUFFIX.
  #            TD_CONF_NAME, ODAROOT & ODAFLD below.
  #            result is mix of mingw & msvc on Windows:
  #              support for such thing is minimal,
  #              stability is unpredictable.
  #
  # much better is use standard building via CMake - see
  # https://jira.opendesign.com/browse/SUP-66
  #

  #ODADLLSUFFIX=_20.11src_10
  #TD_CONF_NAME=vc10dll

  ODADLLSUFFIX=_20.11src_14
  TD_CONF_NAME=vc14_amd64dll


  ODAROOT=C:\_Oda
  ODAFLD=trunk_git
  
  #DEFINES += TEIGHA_TRIAL  # for trial version only
  #ODAVERSION=20.11 # unused now

  #####################################################

  ODADIR=$${ODAROOT}/$${ODAFLD}

  CONFIG(debug, debug|release): TD_CONF_NAME = $${TD_CONF_NAME}dbg 
}
else {

  # normalizing to use $${...} instead $$(...) with qmake project conversion

  TMPVAR=$$(ODADIR)
  ODADIR=$$TMPVAR
  TMPVAR=$$(ODAROOT)
  ODAROOT=$$TMPVAR
  TMPVAR=$$(ODAFLD)
  ODAFLD=$$TMPVAR
  TMPVAR=$$(TD_CONF_NAME)
  TD_CONF_NAME=$$TMPVAR
  TMPVAR=$$(ODADLLSUFFIX)
  ODADLLSUFFIX=$$TMPVAR
  TMPVAR=$$(ODAVERSION)
  ODAVERSION=$$TMPVAR
}
