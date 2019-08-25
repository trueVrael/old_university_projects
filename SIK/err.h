#ifndef _ERR_
#define _ERR_
#include <inttypes.h>
struct pair{
	uint64_t begin;
	uint64_t end;
};
/* wypisuje informacje o blednym zakonczeniu funkcji systemowej 
i konczy dzialanie */
extern void syserr(const char *fmt, ...);

/* wypisuje informacje o bledzie i konczy dzialanie */
extern void fatal(const char *fmt, ...);

#endif
