FOOT_DIR=foot
FOOT_GEN=$(FOOT_DIR)/gen_hh.sh
FOOT_GEN_RESULTS=$(addprefix $(FOOT_DIR)/__foot_, defs.hh ptr_assign.hh clear_all.hh)
#DPTC_PATH=$(FOOT_DIR)/dptc
#DPTC_FILE_PREFIX=dptc_unpack
#FRS_EXEO += $(DPTC_PATH)/$(DPTC_FILE_PREFIX).o

## We make group target because FOOT_GEN require one file and generate 3.
$(FOOT_GEN_RESULTS) &: $(FOOT_DIR)/foot_common.hh
	./$(FOOT_GEN)
#	gcc -c -MMD $(DPTC_PATH)/$(DPTC_FILE_PREFIX).c -o $(DPTC_PATH)/$(DPTC_FILE_PREFIX).o
#	ln -sf $(DPTC_PATH)/$(DPTC_FILE_PREFIX).o $(DPTC_FILE_PREFIX).o
#
-include foot_hh
