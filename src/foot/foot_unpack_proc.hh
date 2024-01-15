/* This file is to get included in the main UnpackProc class header file 
 * which expands the `TGo4EventProcessor` class.
 * DECL macros are to be expanded in the header file, and IMPL macros in implementation (cxx, cpp, cc) files.
 * This code is written this way to keep the FOOT structure as reminiscent of R3B unpacking, while keeping
 * the direct changes to main UnpackProc/UnpackEvent classes minimal.
 * -- Martin Bajzek [M.Bajzek@gsi.de], 11.01.2024 */

#ifndef __GO4_FOOT_UNPACK_PROC_HH__
#define __GO4_FOOT_UNPACK_PROC_HH__

#include "foot_common.hh"
#include "dptc/dptc.h"
#include "endian.h"

typedef uint32_t u32;
typedef uint16_t u16; 
typedef uint8_t u8; 

#define _countof(x) ( sizeof x / sizeof *x )
/* H4cKz */
#define __EMPTY()
#define __DEREF(x) x __EMPTY()
#define __EVAL(...) __VA_ARGS__

/* DECL directives are to be expanded inside UNPACK_PROC_CLASS declaration file 
 * IMPL directives are to be expanded inside UNPACK_PROC_CLASS implementation file 
 * Directives starting with 2 underscores are used only as helpers here, shouldn't be exported.
 */

#define FOOT_PROC_ID 2
#define N_CHANNELS 320

/* 11 header words, 5 trailer words */
#define N_WORDS (FOOT_RAW_DATA_WORDS + 16)

union foot_ch_data {
	struct {
#if __BYTE_ORDER == __BIG_ENDIAN
		u32 adc2_2 : 8;
		u32 adc2_1 : 8;
		u32 adc1_2 : 8;
		u32 adc1_1 : 8;
#else	
		u32 adc1_1 : 8;
		u32 adc1_2 : 8;
		u32 adc2_1 : 8;
		u32 adc2_2 : 8;
#endif
	};
	u32 word;
};

#ifdef UNPACK_FOOT_WR_SUBEV 
#define __UNPACK_FOOT_WR_IMPL__(UNPACK_EVENT_CLASS, UNPACK_PROC_CLASS) \
	void UNPACK_PROC_CLASS::UnpackFootWrSubev(UNPACK_EVENT_CLASS* fOut, TGo4MbsSubEvent* psubevt)  \
	{ \
		int ctrl_id = psubevt->GetControl(); \
		int id = ctrl_id; \
		u32* pdata = (u32*)psubevt->GetDataField(); \
		int lenMax = (psubevt->GetDlen()-2)/2; \
		if(lenMax != 6 && lenMax != 5) { \
			fprintf(stderr, "Matched FOOT WR %d, but the subevent size is %d. FOOT WR should be 5 or 6 words long.\n", id, lenMax); \
			return; \
		} \
	 \
	 \
		Bool_t* ts_bad = fOut->foot[id].ts_bad; \
		if(*pdata & 0x10000) *ts_bad = 1; \
	 \
		/* Timestamp payload is unpacked from the data itself. \
		 * Same as the sync check values. Here just check if the structure is correct + bad ts bitflag. */ \
	 \
		if(++pdata; (*pdata & 0xffff0000) != 0x03e10000) { \
			fprintf(stderr, "FOOT WR: %d, missed the 0x03e1 tag. Word is: 0x%08x\n", id, *pdata); \
			return; \
		} \
		if(++pdata; (*pdata & 0xffff0000) != 0x04e10000) { \
			fprintf(stderr, "FOOT WR: %d, missed the 0x04e1 tag. Word is: 0x%08x\n", id, *pdata); \
			return; \
		} \
		if(++pdata; (*pdata & 0xffff0000) != 0x05e10000) { \
			fprintf(stderr, "FOOT WR: %d, missed the 0x05e1 tag. Word is: 0x%08x\n", id, *pdata); \
			return; \
		} \
		if(++pdata; (*pdata & 0xffff0000) != 0x06e10000) { \
			fprintf(stderr, "FOOT WR: %d, missed the 0x06e1 tag. Word is: 0x%08x\n", id, *pdata); \
			return; \
		} \
		if(++pdata; lenMax == 6 && ((*pdata & 0xfff00000) != 0xf1a00000)) { \
			fprintf(stderr, "FOOT WR: %d, sync check missed the 0xf1a tag. Word is 0x%0x. FOOT Event flagged as bad.\n", id, *pdata); \
			*ts_bad = 1; \
			return; \
		} \
	}
#else
#define __UNPACK_FOOT_WR_IMPL__(UNPACK_EVENT_CLASS, UNPACK_PROC_CLASS) \
	inline void UNPACK_PROC_CLASS::UnpackFootWrSubev(UNPACK_EVENT_CLASS* fOut, TGo4MbsSubEvent* psubevt) {}
#endif
/* __UNPACK_FOOT_WR_IMPL__ ends */

#define __UNPACK_FOOT_DATA_IMPL__(UNPACK_EVENT_CLASS, UNPACK_PROC_CLASS) \
	void UNPACK_PROC_CLASS::UnpackFootData(UNPACK_EVENT_CLASS* fOut, TGo4MbsSubEvent* psubevt) { \
		int ctrl_id = psubevt->GetControl(); \
		int id = ctrl_id; \
	 \
		u32* pdata = (u32*)psubevt->GetDataField(); \
		int lenMax = (psubevt->GetDlen()-2)/2;	 \
	 \
		/* First words: \
		 * Raw [0]>0: counter \
		 * Compressed: [0] : counter, [1]>0 wordcount \
		 */ \
		u32 ev = (u32)(*pdata++); \
		u32 count = (u32)(*pdata++); \
		bool is_comp = 0x80000000 & count; \
		count &= 0x7fffffff; \
	 \
		u32 raw[N_WORDS]; \
		memset(raw, 0xaa, sizeof raw); \
		u32* rp; \
	 \
		/* Go over the payload */\
		if(is_comp) {  \
			/* Compressed */ \
			 \
			dptc_unpack16(pdata, count, (u16*)raw, _countof(raw) * 2, 16); \
		} \
		else {  \
			/* Raw */ \
			 \
			rp = raw; \
			*rp++ = count; \
			for(u32 i=0; i<count-1; ++i, ++rp, ++pdata) \
				*rp = *pdata; \
		} \
	 \
		/* Check header. */\
		rp = raw; \
		u32 header; \
	 \
		header = *rp++; \
		if(16 + N_CHANNELS != header) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid event size: %x.\n", id, header); \
			return; \
		} \
		header = *rp++; \
		if(0xeadebaba != header) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid header2: 0x%08x.\n", id, header); \
			return; \
		} \
		header = *rp++; \
		if(0xd451d451 != header) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid header3: 0x%08x.\n", id, header); \
			return; \
		} \
	 \
		/* Event counter. */ \
		header = *rp++; \
		 \
		/* TS 32 LSB. */ \
		u32 ts_lo = *rp++; \
	 \
		/* TS 32 MSB. */ \
		u32 ts_hi = *rp++; \
	 \
		header = *rp++; \
		if(N_CHANNELS + 8 != header) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid header7: 0x%08x", id, header); \
			return; \
		} \
		header = *rp++; \
		if(0x4ead0501 != header) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid header8: 0x%08x", id, header); \
			return; \
		} \
		header = *rp++; \
		if(0x4ead1502 != header) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid header9: 0x%08x", id, header); \
			return; \
		} \
		header = *rp++; \
		if(0x4ead2503 != header) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid header10: 0x%08x", id, header); \
			return; \
		} \
		header = *rp++; \
		if (0x4ead3504 != header) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid header11: 0x%08x", id, header); \
			return; \
		} \
	 \
		/* Local data container */ \
		u32 outp[FOOT_CHN]; \
	\
		/* Go over raw data */ \
		for(u32 i=0; i < N_CHANNELS; ++i) { \
			foot_ch_data ch_data; \
			ch_data.word = *rp++; \
	 \
			/* Compressed data is unswizzled, but raw not. */ \
	 \
			u32 j = 2 * i; \
			u32 k = j; \
			if(!is_comp) k = 64 * (j % 10) + j / 10; \
			outp[k]	  = ((ch_data.adc1_2 << 8) |  ch_data.adc1_1) / 4; \
	 \
			k = ++j; \
			if(!is_comp) k = 64 * (j % 10) + j / 10; \
			outp[k]	  = ((ch_data.adc2_2 << 8) |  ch_data.adc2_1) / 4; \
		} \
	 \
		u32 footer; \
		 \
		footer = *rp++; \
		if(0xf007e501 != footer) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid footer1: 0x%08x", id, footer); \
			return; \
		} \
		footer = *rp++; \
		if(0xf007e502 != footer) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid footer2: 0x%08x", id, footer); \
			return; \
		} \
		footer = *rp++; \
		if(0xf007e503 != footer) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid footer3: 0x%08x", id, footer); \
			return; \
		} \
		footer = *rp++; \
		if(0x0000fafe != (0x0000ffff & footer)) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid footer4: 0x%08x", id, footer); \
			return; \
		} \
	 \
		u16 ts_status = 0xfff & static_cast<u16>(footer >> 24); \
		u8 sync_value = 0xf & static_cast<u8>(footer >> 16); \
	 \
		footer = *rp++; \
		if(0xbacca000 != footer) { \
			fprintf(stderr, "FOOT %d while unpacking data. Invalid footer5: 0x%08x", id, footer); \
			return; \
		} \
		 \
		/* Fill histograms */ \
		auto h = h2_foot_raw[id]; \
		for(u32 ch = 0; ch < FOOT_CHN; ++ch) \
			h->Fill(ch, outp[ch]); \
	 \
		/* After all the checks, cpy the data into UnpackEvent fields. */ \
		auto& foot_out = fOut->foot[id]; \
		*foot_out.ts_lo = ts_lo; \
		*foot_out.ts_hi = ts_hi; \
		*foot_out.sync_value = sync_value; \
		*foot_out.data_avail = FOOT_CHN; \
		memcpy(foot_out.foot_i, fDummy640, sizeof fDummy640); \
		memcpy(foot_out.foot_e, outp, sizeof outp); \
	 \
	} \

/* __UNPACK_FOOT_DATA_IMPL__ ends */

#define __FOOT_RAW_HIST_DECL__(NAME) \
	TH2I* NAME[MAX_FOOT_I] = {0};

// ---------------------------------- //
// ------ MACROS TO BRING HOME ------ //
// ---------------------------------- //

/* Next 4 directives are ones for export. */
/* Follow the next 5 steps */

/* (1) Expand this in the UnpackProc class definition */
#define FOOT_UNPACK_PROC_DECL(UNPACK_EVENT_CLASS, HIST_NAME) \
	void BuildFootEvent(UNPACK_EVENT_CLASS*, TGo4MbsEvent*); \
	void UnpackFootWrSubev(UNPACK_EVENT_CLASS*, TGo4MbsSubEvent*); \
	void UnpackFootData(UNPACK_EVENT_CLASS*, TGo4MbsSubEvent*); \
	\
	/* Keep dummy on stack to keep memcpy'ing 1,2,...640 into FOOTI <(~_~)> */ \
	u32 fDummy640[FOOT_CHN]; \
	__FOOT_RAW_HIST_DECL__(HIST_NAME)

/* (2) Expand this in the implementation file. */
#define UNPACK_FOOT_PROC_IMPL(UNPACK_EVENT_CLASS, UNPACK_PROC_CLASS) \
	\
	__EVAL (__UNPACK_FOOT_WR_IMPL__) ( __DEREF(UNPACK_EVENT_CLASS), __DEREF(UNPACK_PROC_CLASS) ) \
	__EVAL (__UNPACK_FOOT_DATA_IMPL__) ( __DEREF(UNPACK_EVENT_CLASS), __DEREF(UNPACK_PROC_CLASS) ) \
	\
	void UNPACK_PROC_CLASS::BuildFootEvent(UNPACK_EVENT_CLASS* fOut, TGo4MbsEvent* fInput) { \
		fInput->ResetIterator(); \
		TGo4MbsSubEvent* psubevt(nullptr); \
		while((psubevt = fInput->NextSubEvent()) != nullptr) { \
			if(psubevt->GetProcid() != FOOT_PROC_ID) continue; \
			 \
			if((psubevt->GetType() == 83) && (psubevt->GetSubtype() == 8300)) UnpackFootData(fOut, psubevt); \
			else if((psubevt->GetType() == 10) && (psubevt->GetSubtype() == 1)) UnpackFootWrSubev(fOut, psubevt); \
		} \
	};

/* (3) Expand this in the UnpackProc ctor impl */
#define FOOT_RAW_HIST_IMPL(HIST_NAME) \
	for(int i=0; i<MAX_FOOT_I; ++i) \
		HIST_NAME[i] = MakeH2I("Unpack/FOOT", Form("Raw%d", i), 640,1,641,4096,0,4096, "channel", "ADC val", 1); \
	 \
	for(int i=0; i<FOOT_CHN; ++i) \
		fDummy640[i] = i+1;

/* (4) Call the `BuildFootEvent(...)` method in the main `BuildEvent(...)` impl. */

/* (5) Expand this in the general subev loop after. */
#define SKIP_FOOT_SUBEV(SEV_PTR) \
	if(SEV_PTR->GetType() == 83 && SEV_PTR->GetSubtype() == 8300) continue; \
	if(SEV_PTR->GetProcid() == FOOT_PROC_ID && SEV_PTR->GetType() == 10 && SEV_PTR->GetSubtype() == 1) continue; \

#endif /* __GO4_FOOT_UNPACK_PROC_HH__ */
