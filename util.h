/*--------------------------------------------------------------------
MIT License

Copyright (c) 2021 bbuhrow

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
--------------------------------------------------------------------*/

#ifndef _YTOOLS_UTIL_H_
#define _YTOOLS_UTIL_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* system-specific stuff ---------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* basic types  -------------------------------------------------------*/

	//moved to types.h

/* useful functions ---------------------------------------------------*/

#define INLINE __inline
#if defined(_MSC_VER)
	#define getpid _getpid
#endif

#if defined(__GNUC__) && __GNUC__ >= 3
	#define PREFETCH(addr) __builtin_prefetch(addr) 
#elif defined(_MSC_VER) && _MSC_VER >= 1400
	#define PREFETCH(addr) PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, addr)
#else
	#define PREFETCH(addr) /* nothing */
#endif

#define MIN(a,b) ((a) < (b)? (a) : (b))
#define MAX(a,b) ((a) > (b)? (a) : (b))

#define DEFAULT_L1_CACHE_SIZE (32 * 1024)
#define DEFAULT_L2_CACHE_SIZE (512 * 1024)


void get_random_seeds(uint32_t* seed1, uint32_t* seed2);

static INLINE uint32_t
get_rand(uint32_t *rand_seed, uint32_t *rand_carry) {
   
	/* A multiply-with-carry generator by George Marsaglia.
	   The period is about 2^63. */

	#define RAND_MULT 2131995753

	uint64_t temp;

	temp = (uint64_t)(*rand_seed) * 
		       (uint64_t)RAND_MULT + 
		       (uint64_t)(*rand_carry);
	*rand_seed = (uint32_t)temp;
	*rand_carry = (uint32_t)(temp >> 32);
	return (uint32_t)temp;
}



// ======================== randomness ==============================

    uint64_t spRand(uint64_t lower, uint64_t upper, uint64_t *lcg_state);


// ======================== searching  ==============================

    int bin_search_uint32(int idp, int idm, uint32_t q, uint32_t *input);

// ======================== sorting    ==============================

    int qcomp_int(const void *x, const void *y);
    int qcomp_uint16(const void *x, const void *y);
    int qcomp_uint32(const void *x, const void *y);
    int qcomp_uint64(const void *x, const void *y);
    int qcomp_double(const void *x, const void *y);

// ======================== timing     ==============================

//user dimis:
//http://cboard.cprogramming.com/cplusplus-programming/
//101085-how-measure-time-multi-core-machines-pthreads.html
//
    double y_difftime(struct timeval *, struct timeval *);

    //http://www.openasthra.com/c-tidbits/gettimeofday-function-for-windows/
#if defined (_MSC_VER)
    int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif

    uint64_t ytools_read_clock(void);
    uint64 measure_processor_speed(void);
    int lock_thread_to_core(void);
    int unlock_thread_from_core(void);

    char *gettimever(char *s);
    char * time_from_secs(char *str, unsigned long time);

// ======================== allocation ==============================

void * aligned_malloc(size_t len, uint32 align);
void aligned_free(void *newptr);

static INLINE void * xmalloc_align(size_t len)
{
#if defined (_MSC_VER) || defined(__MINGW32__)
    void *ptr = _aligned_malloc(len, 64);

#elif defined (__APPLE__)
    void *ptr = malloc(len);

#elif defined (__GNUC__)
    void *ptr = memalign(64, len);

#else
    void *ptr = malloc(len);

#endif

    return ptr;
}

static INLINE void * xmalloc(size_t len) {
    void *ptr = malloc(len);
    if (ptr == NULL) {
        printf("failed to allocate %u bytes\n", (uint32)len);
        exit(-1);
    }
    return ptr;
}

static INLINE void * xcalloc(size_t num, size_t len) {
    void *ptr = calloc(num, len);
    if (ptr == NULL) {
        printf("failed to calloc %u bytes\n", (uint32)(num * len));
        exit(-1);
    }
    return ptr;
}

static INLINE void * xrealloc(void *iptr, size_t len) {
    void *ptr = realloc(iptr, len);
    if (ptr == NULL) {
        printf("failed to reallocate %u bytes\n", (uint32)len);
        exit(-1);
    }
    return ptr;
}



void logprint(FILE *infile, char *args, ...);
void logprint_oc(const char *name, const char *method, char *args, ...);



// ======================== system id  ==============================


/* for turning on CPU-specific code */

enum cpu_type {
	cpu_generic,
	cpu_pentium,
	cpu_pentium2,
	cpu_pentium3,
	cpu_pentium4,
	cpu_pentium_m,
	cpu_core,
	cpu_athlon,
	cpu_athlon_xp,
	cpu_opteron,
};


void ytools_get_cache_sizes(uint32_t *level1_cache, uint32_t *level2_cache);
enum cpu_type ytools_get_cpu_type(void);

int extended_cpuid(char *idstr, int *cachelinesize, char *bSSE41Extensions, 
	char *AVX, char *AVX2, int do_print);

/* CPU-specific capabilities */

/* assume for all CPUs, even non-x86 CPUs. These guard
   assembly language that has other guards anyway, and
   the only CPU that doesn't have these instructions is
   the classic Pentium */

#define HAS_CMOV

// assme this is a modern cpu that has at least up through sse2.
#ifndef FORCE_GENERIC
#define HAS_MMX 1
#define HAS_SSE 1
#define HAS_SSE2 1
#define CACHE_LINE_64
#endif


#ifdef __cplusplus
}
#endif



#endif /* _YAFU_UTIL_H_ */
