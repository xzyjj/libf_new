/* xxhash64.c - extremely fast hash algorithm (64bit) definitions */

#ifndef LIBF_AL_XXHASH64_H
#define LIBF_AL_XXHASH64_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: xxhash64 */
#undef XXHASH64_BLOCKSIZE
#define XXHASH64_BLOCKSIZE 32

#undef xxhash64_ctx
struct xxhash64_ctx {
	uint64L state[4];
	uint8 buf[XXHASH64_BLOCKSIZE];
	uint64L seed;
	uint32 count;
};

#undef XXHASH64_NEW
#define XXHASH64_NEW(name) struct xxhash64_ctx name
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xxhash64.c */
extern void FSYMBOL(xxhash64_init)(struct xxhash64_ctx *ctx, uint64L seed);
extern void FSYMBOL(xxhash64_process)(struct xxhash64_ctx *ctx, const uint8 *s,
		uint64 len);
extern uint64L FSYMBOL(xxhash64_finish)(struct xxhash64_ctx *ctx, uint64L len);
extern void FSYMBOL(xxhash64)(struct xxhash64_ctx *ctx, const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
