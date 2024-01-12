#ifndef _GO4_DEFINES_
#define _GO4_DEFINES_

#define TRAV_MUS_WR // comment out this line if travmus has no WR
#define QDC_IS_BAD

#define SKIP_REST(pdata, len, lenMax) \
	while(len < lenMax) { \
		pdata++; \
		len++;\
	} \

#endif
