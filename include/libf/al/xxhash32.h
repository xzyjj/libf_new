/* xxhash32.c - extremely fast hash algorithm (32bit) definitions */

#ifndef LIBF_AL_XXHASH32_H
#define LIBF_AL_XXHASH32_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: xxhash32 */
#undef XXHASH32_BLOCKSIZE
#define XXHASH32_BLOCKSIZE 16

#undef xxhash32_ctx
struct xxhash32_ctx {
	uint32 state[4];
	uint8 buf[XXHASH32_BLOCKSIZE];
	uint32 seed;
	uint32 count;
};

#undef XXHASH32_NEW
#define XXHASH32_NEW(name) struct xxhash32_ctx name
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xxhash32.c */
extern void FSYMBOL(xxhash32_init)(struct xxhash32_ctx *ctx, uint32 seed);
extern void FSYMBOL(xxhash32_process)(struct xxhash32_ctx *ctx, const uint8 *s,
		uint64 len);
extern uint32 FSYMBOL(xxhash32_finish)(struct xxhash32_ctx *ctx, uint64 len);
extern void FSYMBOL(xxhash32)(struct xxhash32_ctx *ctx, const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
