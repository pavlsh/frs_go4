ifndef GO4SYS
$(error 'GO4SYS' have to be defined)
endif

$(info CPPFLAGS  $(CPPFLAGS))

FOOT_GEN=gen_hh.sh
FOOT_GEN_DEP=foot_common.hh
FOOT_GEN_RES=$(addprefix __foot_, defs.hh ptr_assign.hh clear_all.hh)

FOOT_LIB=../../libFOOT.so
CLASSLIST= TFOOTParameter TFOOTSortEvent TFOOTSortProc

FOOT_O=$(addsuffix .o,$(CLASSLIST))
FOOT_H=$(addsuffix .h,$(CLASSLIST))

CPPFLAGS := $(addprefix -I,\
$(shell root-config --incdir) $(GO4SYS)/include ../) -fPIC
CXXFLAGS := -Wall -MMD -MP -O2
LDFLAGS := $(addprefix -L,\
$(shell root-config --libdir) $(GO4SYS)/lib)
LDLIBS := $(addprefix -l, Go4Base)


RDFLAGS :=\
$(addprefix -I=,  $(GO4SYS)/include ../)

.PHONY : all clean


all: $(FOOT_GEN_RES) $(FOOT_LIB)

$(FOOT_GEN_RES) &: $(FOOT_GEN_DEP)
	./$(FOOT_GEN)

clean:
	$(RM) $(FOOT_GEN_RES)
	$(RM) $(FOOT_O)
	$(RM) FOOTDict.cxx


FOOTDict.cxx: $(FOOT_H) LinkDef.h
	rootcling  $(RDFLAGS) -f $@ -c  $(CXXFLAGS) -p $^


$(FOOT_LIB): FOOTDict.o $(FOOT_O)
	$(CXX) $(LDFLAGS)  -shared -o $@ $^ $(LDLIBS)

%.o: %.cxx
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $< -o $@
