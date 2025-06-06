/* blake2b.h - blake2 cryptographic hash definitions */

#ifndef LIBF_AL_BLAKE2B_H
#define LIBF_AL_BLAKE2B_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: blake2b */
#undef BLAKE2B_224_LEN
#define BLAKE2B_224_LEN 28
#undef BLAKE2B_256_LEN
#define BLAKE2B_256_LEN 32
#undef BLAKE2B_384_LEN
#define BLAKE2B_384_LEN 48
#undef BLAKE2B_512_LEN
#define BLAKE2B_512_LEN 64

#undef BLAKE2B_BLOCKSIZE
#define BLAKE2B_BLOCKSIZE 128

#undef blake2b_param
struct blake2b_param {
	uint8 digest_length; /* 1 */
	uint8 key_length;    /* 2 */
	uint8 fanout;        /* 3 */
	uint8 depth;         /* 4 */
	uint32 leaf_length;  /* 8 */
	uint32 node_offset;  /* 12 */
	uint32 xof_length;   /* 16 */
	uint8 node_depth;    /* 17 */
	uint8 inner_length;  /* 18 */
	uint8 reserved[14];  /* 32 */
	uint8 salt[16];      /* 48 */
	uint8 personal[16];  /* 64 */
};

#undef blake2b_ctx
struct blake2b_ctx {
	uint64L state[8];
	uint64L tsize[2];
	uint64L flags[2];
	union {
		struct blake2b_param param;
		uint64L state[8];
	} u;
	uint8 buf[BLAKE2B_BLOCKSIZE];
	uint32 count;
};

#undef BLAKE2B_NEW
#define BLAKE2B_NEW(x) struct blake2b_ctx x

#undef BLAKE2B_STATE
#define BLAKE2B_STATE(x, n) (((uint8 *)(x)->state)[n])
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* blake2b.c */
extern int32 FSYMBOL(blake2b_init)(struct blake2b_ctx *ctx, uint32 dsize);
extern void FSYMBOL(blake2b_process)(struct blake2b_ctx *ctx, const uint8 *s,
		uint64 len);
extern void FSYMBOL(blake2b_finish)(struct blake2b_ctx *ctx);
extern void FSYMBOL(blake2b)(struct blake2b_ctx *ctx, const uint8 *s, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
