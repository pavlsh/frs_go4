#general make file for any Go4 module
## I have other part for go4 Makefile parts on my home PC
-include $(GO4SYS)/share/Makefile.config

# for compile example under cygwin, 
# relative path to GO4SYS directory should be specified
ifdef GO4_WIN32
  GO4SYS = ../go4
endif   

# comment to switch on optimization
DOOPTIMIZATION = false

.PHONY: all build submakes clean

all: build submakes

submakes :
	$(MAKE) -C foot

-include Module.mk

build: all-$(MODULE_NAME)

clean-obj: clean-obj-$(MODULE_NAME)
	$(info Clean objects in $(MODULE_NAME))

clean: clean-$(MODULE_NAME)
	$(info Clean everything in $(MODULE_NAME))
	$(RM) ../exec/createlml.pl
	rm -f ../exec/runGo4.sh
	rm -f ../exec/createGo4Trees.sh
	rm -f ../exec/inc
	rm -f ../exec/loadGo4Trees.C
	rm -f ../exec/loadUserTrees.C
	rm -f ../exec/setup.C
	rm -f ../exec/libGo4UserAnalysis*
	rm -f ../exec/MainUserAnalysis
	rm -f libGo4UserAnalysis*
	rm -f MainUserAnalysis
	$(MAKE) -C foot clean

ifdef DOMAP
ifeq ($(wildcard $(GO4MAP)),)
map:
	@echo "Map for Go4 classes not generated. Call make map in $(GO4SYS) directory"
else
map: map-$(MODULE_NAME)
	@echo "Map for $(MODULE_NAME) done"
endif
else
map:
	@echo "Map for $(MODULE_NAME) can not be generated with ROOT $(shell root-config --version)"
endif

ifdef DOPACKAGE
package:
	@tar chf $(MODULE_NAME).tar Module.mk
	@tar rhf $(MODULE_NAME).tar $(subst $(CURDIR),.,$(DISTRFILES))
	@tar rhf $(MODULE_NAME).tar Makefile --ignore-failed-read
	@gzip -f $(MODULE_NAME).tar
	@echo "$(MODULE_NAME).tar.gz done"
endif

install: all 
	rm -f ../exec/createlml.pl
	rm -f ../exec/runGo4.sh
	rm -f ../exec/createGo4Trees.sh
	rm -f ../exec/inc
	rm -f ../exec/loadGo4Trees.C
	rm -f ../exec/loadUserTrees.C
	rm -f ../exec/setup.C
	rm -f ../exec/libGo4UserAnalysis*
#	rm -f ../exec/MainUserAnalysis
	cp createlml.pl ../exec
	cp runGo4.sh ../exec
	cp createGo4Trees.sh ../exec
	cp loadGo4Trees.C ../exec
	ln -s ../source/inc ../exec
	cp loadUserTrees.C ../exec
	#cp setup.C ../exec
	ln -s ../source/setup.C ../exec
	cp libGo4UserAnalysis* ../exec
#	cp MainUserAnalysis ../exec
## I have other part for go4 Makefile parts on my home PC
include $(GO4SYS)/share/Makefile.rules
##include foot/foot.mk
