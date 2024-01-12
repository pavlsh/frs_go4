/* This file is to get included in the main UnpackEvent class header file 
 * which expands the `TGo4EventEvent` class.
 * DECL macros are to be expanded in the header file, and IMPL macros in implementation (cxx, cpp, cc) files.
 * This code is extremely hacked, as to keep the FOOT structure as reminiscent of R3B unpacking, while keeping
 * the direct changes to main UnpackProc/UnpackEvent classes minimal.
 * -- Martin Bajzek [M.Bajzek@gsi.de], 11.01.2024 */

#ifndef __GO4_FOOT_UNPACK_EVENT_HH__ 
#define __GO4_FOOT_UNPACK_EVENT_HH__

#include "foot_common.hh"

struct TFootPtr {
	Bool_t* ts_bad;
	UInt_t* ts_lo;
	UInt_t* ts_hi;
	UInt_t* sync_value;
	UInt_t* data_avail;
	UInt_t* foot_i;
	UInt_t* foot_e;
	TFootPtr() = default;
};

/* Directives starting with 2 underscores are used only as helpers here, shouldn't be exported. */

#define __FOOT_RAW__(x) \
	Bool_t FOOT##x##TSBAD ; \
	UInt_t FOOT##x##TLO ; \
	UInt_t FOOT##x##THI ; \
	UInt_t FOOT##x##SY ; \
	UInt_t FOOT##x ; \
	UInt_t FOOT##x##I[FOOT_CHN] ; \
	UInt_t FOOT##x##E[FOOT_CHN] ; \

#define __ASSIGN_FOOT_PTR__(x) \
	foot[x].ts_bad     = &FOOT##x##TSBAD ; \
	foot[x].ts_lo      = &FOOT##x##TLO ; \
	foot[x].ts_hi      = &FOOT##x##THI ; \
	foot[x].sync_value = &FOOT##x##SY ; \
	foot[x].data_avail = &FOOT##x ; \
	foot[x].foot_i     =  FOOT##x##I ; \
	foot[x].foot_e     =  FOOT##x##E ; 

#define __CLEAR_FOOT__(x) \
	FOOT##x##TSBAD = 0; \
	FOOT##x##TLO = 0; \
	FOOT##x##THI = 0; \
	FOOT##x##SY = 0; \
	FOOT##x = 0; \
	memset(FOOT##x##I, 0, sizeof FOOT##x##I); \
	memset(FOOT##x##E, 0, sizeof FOOT##x##E); \

/* Follow next 3 steps. */
/* (1) Insert generated `FOOT_UNPACK_STRUCTURES_DECL` directive inside UnpackEvent's field declaration. */
#include "__foot_defs.hh"

/* (2) Insert generated `FOOT_PTR_ASSIGN` directive inside UnpackEvent's ctor. */
#include "__foot_ptr_assign.hh"

/* (3) Insert  generated `CLEAR_FOOT` directive inside UnpackEvent's Clear function. */
#include "__foot_clear_all.hh"



#endif /* __GO4_FOOT_UNPACK_EVENT_HH__ */
