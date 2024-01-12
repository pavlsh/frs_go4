FRS_NAME        = FRSonline
MODULE_NAME         = $(FRS_NAME)

## normally should be like this for every module, but can be specific

FRS_DIR         = $(CURDIR)
#to compile under CYGWIN, specify 
#FRS_DIR        = .
BIN_DIR = $(CURDIR)/..

FRS_LINKDEF     = $(FRS_DIR)/Go4UserAnalysisLinkDef.$(HedSuf)
FRS_LIBNAME     = libGo4UserAnalysis
FRS_EXENAME     = MainUserAnalysis
#FRS_ONLYMAP     = $(FRS_DIR)/.localmap
#FRS_MAP         = $(FRS_DIR)/$(ROOTMAPNAME)

FRS_NOTLIBF     = 

## must be similar for every module

FRS_EXEO        = $(FRS_DIR)/$(FRS_EXENAME).$(ObjSuf)
FRS_EXES        = $(FRS_DIR)/$(FRS_EXENAME).$(SrcSuf)
FRS_EXE         = $(BIN_DIR)/$(FRS_EXENAME)$(ExeSuf)   

FRS_DICT        = $(FRS_DIR)/$(DICT_PREFIX)$(FRS_NAME)
FRS_DH          = $(FRS_DICT).$(HedSuf)
FRS_DS          = $(FRS_DICT).$(SrcSuf)
FRS_DO          = $(FRS_DICT).$(ObjSuf)

FRS_H           = $(filter-out $(FRS_EXEH) $(FRS_NOTLIBF) $(FRS_DH) $(FRS_LINKDEF), $(wildcard $(FRS_DIR)/*.$(HedSuf)))
FRS_S           = $(filter-out $(FRS_EXES) $(FRS_NOTLIBF) $(FRS_DS), $(wildcard $(FRS_DIR)/*.$(SrcSuf)))
FRS_O           = $(FRS_S:.$(SrcSuf)=.$(ObjSuf))

FRS_DEP         =  $(FRS_O:.$(ObjSuf)=.$(DepSuf))
FRS_DDEP        =  $(FRS_DO:.$(ObjSuf)=.$(DepSuf))
FRS_EDEP        =  $(FRS_EXEO:.o=.d)

FRS_SLIB        =  $(FRS_DIR)/$(FRS_LIBNAME).$(DllSuf)
FRS_LIBPCM      =  $(FRS_DIR)/$(FRS_LIBNAME)_rdict.pcm
FRS_LIB         =  $(FRS_DIR)/$(FRS_LIBNAME).$(DllSuf).$(VERSSUF)

# used in the main Makefile

EXAMPLELIBS        += $(FRS_LIB)
EXAMPLEEXECS       += $(FRS_EXE)
EXAMPDEPENDENCS    += $(FRS_DEP) $(FRS_DDEP) $(FRS_EDEP)

ifdef DOPACKAGE
DISTRFILES         += $(FRS_S) $(FRS_H) $(FRS_LINKDEF) $(FRS_EXEH) $(FRS_EXES)
DISTRFILES         += $(FRS_DIR)/AnalysisStart.sh $(FRS_DIR)/setup.C
endif

##### local rules #####

$(FRS_EXE):      $(BUILDGO4LIBS) $(FRS_EXEO) $(FRS_LIB)
	$(LD) $(LDFLAGS) $(FRS_EXEO) $(LIBS_FULLSET) $(FRS_LIB) $(OutPutOpt) $(FRS_EXE)
	@echo "$@  done"

$(FRS_LIB):   $(FRS_O) $(FRS_DO)
	@$(MakeLibrary) $(FRS_LIBNAME) "$(FRS_O) $(FRS_DO)" $(FRS_DIR)
	ln -sf $(FRS_SLIB) $(FRS_LIB) 
	ln -sf $(FRS_SLIB) $(BIN_DIR)
	ln -sf $(FRS_LIBPCM) $(BIN_DIR)

$(FRS_DS): $(FRS_H)  $(FRS_LINKDEF)
	@$(ROOTCINTGO4) $(FRS_H) $(FRS_LINKDEF)

# $(FRS_ONLYMAP): $(FRS_LINKDEF) $(FRS_LIB)
# 	@rm -f $(FRS_ONLYMAP)
# 	@$(MakeMap) $(FRS_ONLYMAP) $(FRS_SLIB) $(FRS_LINKDEF) "$(ANAL_LIB_DEP)"

all-$(FRS_NAME):     $(FRS_LIB) $(FRS_EXE) #map-$(FRS_NAME)

clean-$(FRS_NAME):
	@rm -f $(FRS_O) $(FRS_DO)
	@$(CleanLib) $(FRS_LIBNAME) $(FRS_DIR)
	@rm -f $(FRS_DEP) $(FRS_DDEP) $(FRS_DS) $(FRS_DH)
	@rm -f $(FRS_EDEP) $(FRS_EXEO) $(FRS_EXE)
	@rm -f $(FRS_ONLYMAP) $(FRS_MAP)

# ifdef DOMAP
# map-$(FRS_NAME): $(GO4MAP) $(FRS_ONLYMAP)
# 	@rm -f $(FRS_MAP)
# 	@cat $(GO4MAP) $(FRS_ONLYMAP) > $(FRS_MAP)
# else
# map-$(FRS_NAME):

# endif

