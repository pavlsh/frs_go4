#ifndef _TFRS_DEBUG_FCNS_CPP
#define _TFRS_DEBUG_FCNS_CPP

#include <assert.h>
#include "colours.hh"

constexpr unsigned BARRIER[] = {0xf5200000, 0xf5000000, 0xbabababa};
constexpr unsigned BARRIER_MASK[] = {0xffff0000, 0xffff0000, 0xffffffff};
#define ARR_SIZE(x) sizeof(x) / sizeof(*x)

static_assert(ARR_SIZE(BARRIER) == ARR_SIZE(BARRIER_MASK), "In debug_fncs.cpp error in barrier sizes.\n");
constexpr int B_ARRAY_SIZE = ARR_SIZE(BARRIER);

bool is_barrier(int*);

void print_curr_module(int* curr_word, int curr_len, int max_len) {
	int* prev_word = curr_word;
	int backtrace_len = curr_len;
	while((--backtrace_len) >= 0 && !is_barrier(--prev_word)) {}
	// prev_word should point to a barrier.
	
	int line_counter=1;
	printf("%s%08x%s ", BI_BLUE, *prev_word++, I_BLUE); // print this barrier first
	backtrace_len++;
	while((backtrace_len++) < curr_len) {
		printf("%08x ", *prev_word++);
		if((++line_counter) % 8 == 0) printf("\n");
	}
	
	printf("%s%08x%s ", BI_RED, *curr_word, COLOR_OFF);

	while(++curr_len < max_len && !is_barrier(++curr_word)) {
		printf("%08x ", *curr_word);
		if((++line_counter) % 8 == 0) printf("\n");
	}
	printf("%s [[.. NEXT MODULE]]%s", L_ORANGE, COLOR_OFF);
}

bool is_barrier(int* word) {
	for(int _i=0; _i<B_ARRAY_SIZE; ++_i) {
		if((*word & BARRIER_MASK[_i]) == BARRIER[_i]) return true; 
	}
	return false;
}

#endif
