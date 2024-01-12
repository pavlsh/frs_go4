/* Copyright (c) 2017, Haakan T. Johansson */
/* Copyright (c) 2020, Lukas Rahmn */
/* Copyright (c) 2020, Anton Fredriksson */
/* All rights reserved. */

/* Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the authors nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DPTC_IMPL_PACK

/* Include this file multiple times to implement the various unpacking
 * functions.
 */

#define DPTC_IMPL_PACK    dptc_compress16
#define DPTC_INPUT_TYPE   uint16_t
#include "dptc_pack.c"
#undef DPTC_IMPL_PACK
#undef DPTC_INPUT_TYPE

#define DPTC_IMPL_PACK    dptc_compress32
#define DPTC_INPUT_TYPE   uint32_t
#include "dptc_pack.c"
#undef DPTC_IMPL_PACK
#undef DPTC_INPUT_TYPE

#define DPTC_IMPL_PACK    dptc_ads_compress32
#define DPTC_INPUT_TYPE   uint32_t
#define DPTC_IMPL_ADAPTIVE_DOWNSAMPLING 1
#include "dptc_pack.c"
#undef DPTC_IMPL_PACK
#undef DPTC_INPUT_TYPE
#undef DPTC_IMPL_ADAPTIVE_DOWNSAMPLING

#else /* DPTC_IMPL_PACK */

#include "dptc.h"

#if DPTC_DEBUG_PACK
#include <stdio.h>
#define DPTC_INT_DEBUG(...)  printf(__VA_ARGS__)
#else
#define DPTC_INT_DEBUG(...)  do { } while (0)
#endif

#if DPTC_DEBUG_PACK || DPTC_DEBUG_UNPACK
void dptc_printbits(uint64_t x, int n);
#endif

/* Compress an array using DPTC.
 * Reference implementation.
 */

size_t DPTC_IMPL_PACK(DPTC_INPUT_TYPE *data, size_t ndata,
		      uint32_t *compr, size_t ncompr,
		      int bits
#if DPTC_IMPL_ADAPTIVE_DOWNSAMPLING
		      , int max_level
#endif
#if DPTC_GET_TOTALBITS
		      , int *totalbits
#endif
		      )
{
  (void) ncompr;
  
  /* *** Constants based on the number of bits. *** */

  /* Mask for the data bits. */
  uint32_t mask = ((((uint32_t) 1) << bits)-1);
  /* Signbit for the data. */
  uint32_t signbit   = mask ^ (mask >> 1);

  /* Number of bits required to store the number of bits used in a chunk. */
  /* Only allowing 'bits' values up to 19, we can do log2 with a lookup. */
  uint32_t storebits = (bits-3-1 < 16) ?
    ((0x4444444433332211ll >> ((bits-3-1)*4)) & 0xf) : 5;
  /* Mask for the same. */
  uint32_t storebitsmask = ((((uint32_t) 1) << storebits)-1);

#if DPTC_IMPL_ADAPTIVE_DOWNSAMPLING
  /* Number of bits required to store the inital level. */
  /* TODO: One should consider to just define his to be 3. */
  uint32_t level_bits = (max_level < 16) ?
    ((0x4444444433332211ll >> (max_level*4)) & 0xf) : 5;
  /* Mask for the same. */
  /* uint32_t level_mask = (1 << level_bits) - 1; */
#endif

  uint32_t prev;
  size_t i, j;
  uint64_t buffer = 0;
  int avail = 0;

  uint32_t *start = compr;

#if DPTC_ENABLE_PREDICTOR
  uint32_t d1 = 0, d2 = 0, d3 = 0;
#endif

  int flipsign;

#if DPTC_IMPL_ADAPTIVE_DOWNSAMPLING
  uint32_t prev_level = 0;
#endif

  /* As opposed to the unpacking routine, we have no problem to handle
   * the case with more bits than the input type allows.
   */

  DPTC_INT_DEBUG("-----------------------------------------------------\n");

  /* Dump the first data value directly. */

  if (!ndata)
    {
#if DPTC_GET_TOTALBITS
      *totalbits = 0;
#endif
      /* No data compresses to no data. */
      return 0;
    }

#if DPTC_IMPL_ADAPTIVE_DOWNSAMPLING
  prev_level = data[0] >> 28;
  buffer |= prev_level << avail;
  avail += level_bits;
  DPTC_INT_DEBUG("init level: %d\n", prev_level);
#endif

  buffer |= ((data[0] ^ signbit) & mask) << avail;
  avail += bits;

  prev = data[0] & mask;
#if DPTC_ENABLE_PREDICTOR
  d1 = prev;
  d2 = d3 = 0;
#endif
  flipsign = (prev & signbit);

  int prevnumbits = bits;

  for (i = 1; i < ndata; )
    {
      uint32_t tmp[20];
#if DPTC_IMPL_ADAPTIVE_DOWNSAMPLING
      uint32_t steps[20];
#endif

      /* Very limited testing suggests that 4 or 5 values are the
       * optimal number of values to store with the same number of bits.
       * Both 3 and 6 yielded worse results.
       */

      size_t numstore = DPTC_CHUNK_SIZE;
      int numbits;
      uint32_t storebase;
      int numbitsdiff;
      uint32_t storehighbitall = 0;

      if (numstore > ndata - i)
	numstore = ndata - i;

      for (j = 0; j < numstore; j++)
	{
	  /* The value to store is the difference to the previous value. */

	  uint32_t value;
#if DPTC_IMPL_ADAPTIVE_DOWNSAMPLING
	  uint32_t level;
	  uint32_t step;
#endif
	  int diff;
	  int storediff;
	  int storeflipped;
	  uint32_t store;
	  uint32_t storesignmask;
	  uint32_t storehighbit;

	  value = data[i+j];

#if DPTC_IMPL_ADAPTIVE_DOWNSAMPLING
	  level = value >> 28;

	  step = level - prev_level; /* -1, 0 or 1 */
	  steps[j] = step;

	  uint32_t d_shift = (step >> 1) & 1; /* (step == -1) */
	  uint32_t u_shift = !(step - 1); /* (step == 1) */

	  diff = ((int) (value << d_shift) - (int) (prev << u_shift)) & mask;

	  DPTC_INT_DEBUG("%2d: lvl %d - %d = %d [u:%d d:%d]\n",
	                 i+j,
			 level, prev_level, step,
	                 u_shift, d_shift);

	  prev_level = level;
#else
	  diff = ((int) value - (int) prev) & mask;
#endif

	  /* Simple predictor. */

#if DPTC_ENABLE_PREDICTOR
	  storediff = diff;

	  if (((d1 & d2 & d3) | ((-d1) & (-d2) & (-d3))) & signbit)
	    {
	      DPTC_INT_DEBUG("%d: q %d %d %d\n", i, d1, d2, d3);

	      storediff = (storediff - d1) & mask;
	    }

	  d3 = d2;
	  d2 = d1;
	  d1 = diff;
#else
	  storediff = diff;
#endif

	  /* Since we need to do look-ahead in the data to determine the
	   * code size, we store to a temporary array.
	   */

#if DPTC_ENABLE_FLIPSIGN
	  storeflipped = flipsign ? -storediff : storediff;
#else
	  storeflipped = storediff;
#endif
	
	  tmp[j] = store = storeflipped & mask;

#if DPTC_ENABLE_FLIPSIGN
	  if (storediff)
	    flipsign = (storediff & signbit);
#endif

	  DPTC_INT_DEBUG("%2d: %04x - %04x = %04x - storediff %04x - "
			 "storeflipped %04x [flip %d]\n",
			 i+j,
	                 value & mask, prev & mask,
			 diff & mask, storediff & mask,
			 storeflipped & mask, flipsign);

	  prev = data[i+j];

	  storesignmask = ((int32_t) (store << (32-bits))) >> 31;
	  storehighbit = (store ^ storesignmask) & mask;

	  storehighbitall |= storehighbit;
	    
	  DPTC_INT_DEBUG("numb2: %04x %04x %04x %04x\n",
			 store, storesignmask, storehighbit, storehighbitall);
	}

      /* Number of bits needed. */

      if (storehighbitall)
	numbits = sizeof (int) * 8 - __builtin_clz(storehighbitall) + 1;
      else
	numbits = 1;

      DPTC_INT_DEBUG("numb2: %04x : %d : p%d\n",
		     storehighbitall, numbits, prevnumbits);

      numbitsdiff = (numbits - prevnumbits + 2);
      if (numbitsdiff < 0)
	numbitsdiff += bits;
      if (numbitsdiff > bits)
	numbitsdiff -= bits;
      
      DPTC_INT_DEBUG("--> %d\n", avail);

      if (numbitsdiff > 0 &&
	  numbitsdiff <= 3)
	{
	  DPTC_INT_DEBUG("short: %d-2 (%d-p%d) - *",
			 numbitsdiff, numbits, prevnumbits);

	  buffer |= ((uint64_t) (numbitsdiff)) << avail;
	  avail += 2;

#if DPTC_DEBUG_PACK
	  dptc_printbits (numbitsdiff, 2);
	  DPTC_INT_DEBUG("\n");
#endif
	}
      else
	{
	  DPTC_INT_DEBUG("long: %d+2 (%d-p%d) %d [%d] - *",
			 numbitsdiff - 4, numbits, prevnumbits,
			 ((numbitsdiff - 4) & storebitsmask),
			 storebits);
	  (void) storebitsmask; /* TODO: really unused? */

	  buffer |= ((uint64_t) 0) << avail;
	  avail += 2;
	  numbitsdiff -= 4;
	  if (numbitsdiff < 0)
	    numbitsdiff += bits;
	  buffer |= ((uint64_t) ((numbitsdiff)/* & storebitsmask*/)) << avail;
	  avail += storebits;

#if DPTC_DEBUG_PACK
	  dptc_printbits (numbitsdiff << 2, storebits + 2);
	  DPTC_INT_DEBUG("\n");
#endif
	}
      
      /* Eject full words to the output buffer. */

      if (avail > 32)
	{
	  *compr = (uint32_t) buffer;

	  DPTC_INT_DEBUG("--> %08x %d\n", *compr, avail - 32);

	  compr++;
	  buffer >>= 32;
	  avail -= 32;
	}

      storebase = (-((1 << numbits) >> 1)) & mask;

      DPTC_INT_DEBUG(" -> %d bits (base 0x%04x)\n", numbits, storebase);

      for (j = 0; j < numstore; j++)
	{
	  uint32_t store;

#if DPTC_IMPL_ADAPTIVE_DOWNSAMPLING
	  uint32_t step;
	  uint32_t stepcode;
	  uint32_t stepbits;

	  step = steps[j];

	  /* step = -1, 0 or 1 gives shift amount 0, 2 or 4 */
	  /* We pick from values 01 (4), 00 (2), 11 (0) */
	  /* 0001 0011 = 0x13 */
	  stepcode = (0x13 >> (step + step + 2)) & 3;
	  stepbits = 2 - !step; /* 1 for step == 0, 2 otherwise */

#if DPTC_DEBUG_PACK
	  DPTC_INT_DEBUG("step %d - %x (%d) - *",
			 step, stepcode, stepbits);
	  dptc_printbits (stepcode, stepbits);
	  DPTC_INT_DEBUG("\n");
#endif

	  buffer |= ((uint64_t) stepcode) << avail;
	  avail += stepbits;
#endif

	  store = tmp[j];

#if DPTC_DEBUG_PACK
	  DPTC_INT_DEBUG("%04x(%5d) - %04x(%5d) - *",
			 store, store,
			 (store - storebase) & mask,
			 (store - storebase) & mask);
	  dptc_printbits ((store - storebase) & mask, numbits);
	  DPTC_INT_DEBUG("\n");
#endif

	  store = (store - storebase) & mask;

	  buffer |= ((uint64_t) store) << avail;
	  avail += numbits;

	  if (avail > 32)
	    {
	      *compr = (uint32_t) buffer;
#if DPTC_DEBUG_PACK
	      DPTC_INT_DEBUG("--> %08x %d   ", *compr, avail - 32);
	      dptc_printbits (*compr, 32);
	      DPTC_INT_DEBUG("\n");
#endif
	      compr++;
	      buffer >>= 32;
	      avail -= 32;
	    }
	}

      i += numstore;

      prevnumbits = numbits;
    }

  /* There are always bits remaining.  Eject to output buffer. */

  *compr = (uint32_t) buffer;
#if DEBUGPACK
  printf ("--> %08x %d\n", *compr, avail - 32);
#endif
  compr++;

#if DPTC_GET_TOTALBITS
  avail -= 32;
  *totalbits = (compr - start)*32 + avail;
#endif

  DPTC_INT_DEBUG("-----------------------------------------------------\n");
  
  return compr - start;
}

#undef DPTC_INT_DEBUG

#endif/*DPTC_IMPL_PACK*/

#ifndef DPTC_IMPL_PACK

#if DPTC_DEBUG_PACK || DPTC_DEBUG_UNPACK
void dptc_printbits(uint64_t x, int n)
{
  int i;
  char buf[65];
  if (n < 64 && x >= (((uint64_t) 1) << n))
    putchar('?');
  for (i = 0; i < n; i++)
    {
      buf[63-i] = '0' + (x & 1);
      x >>= 1;
    }
  buf[64] = 0;
  printf("%s", buf+64-n);
}
#endif

#endif/*!DPTC_IMPL_PACK*/
