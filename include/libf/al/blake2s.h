/* blake2s.h - blake2 cryptographic hash definitions */

#ifndef LIBF_AL_BLAKE2S_H
#define LIBF_AL_BLAKE2S_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: blake2s */
#undef BLAKE2S_128_LEN
#define BLAKE2S_128_LEN 16
#undef BLAKE2S_196_LEN
#define BLAKE2S_196_LEN 24
#undef BLAKE2S_256_LEN
#define BLAKE2S_256_LEN 32

#undef BLAKE2S_BLOCKSIZE
#define BLAKE2S_BLOCKSIZE 64

#undef blake2s_param
struct blake2s_param {
	uint8 digest_length; /* 1 */
	uint8 key_length;    /* 2 */
	uint8 fanout;        /* 3 */
	uint8 depth;         /* 4 */
	uint32 leaf_length;  /* 8 */
	uint32 node_offset;  /* 12 */
	uint16 xof_length;   /* 14 */
	uint8 node_depth;    /* 15 */
	uint8 inner_length;  /* 16 */
	uint8 salt[8];       /* 24 */
	uint8 personal[8];   /* 32 */
};

#undef blake2s_ctx
struct blake2s_ctx {
	uint32 state[8];
	uint32 tsize[2];
	uint32 flags[2];
	union {
		struct blake2s_param param;
		uint32 state[8];
	} u;
	uint8 buf[BLAKE2S_BLOCKSIZE];
	uint32 count;
};

#undef BLAKE2S_NEW
#define BLAKE2S_NEW(x) struct blake2s_ctx x

#undef BLAKE2S_STATE
#define BLAKE2S_STATE(x, n) (((uint8 *)(x)->state)[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* blake2s.c */
extern int32 FSYMBOL(blake2s_init)(struct blake2s_ctx *ctx, uint32 dsize);
extern void FSYMBOL(blake2s_process)(struct blake2s_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(blake2s_finish)(struct blake2s_ctx *ctx);
extern void FSYMBOL(blake2s)(struct blake2s_ctx *ctx, const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
