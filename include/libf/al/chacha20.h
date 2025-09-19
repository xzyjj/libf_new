/* chacha20.h - chacha20 encryption definitions */

#ifndef LIBF_AL_CHACHA20_H
#define LIBF_AL_CHACHA20_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: chacha20 */
#undef CHACHA20_KEYLEN
#define CHACHA20_KEYLEN 32
#undef CHACHA20_RANLEN
#define CHACHA20_RANLEN 12
#undef CHACHA20_BLOCKSIZE
#define CHACHA20_BLOCKSIZE 64

#undef CHACHA20_CINDEX
#define CHACHA20_CINDEX 12
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* chacha20.c */
extern void FSYMBOL(chacha20_init_state)(uint32 state[16], const uint8 key[64],
		const uint8 ran[12], uint32 count);
extern void FSYMBOL(chacha20_block)(const uint32 state[16], uint32 out[16]);
extern uint32 FSYMBOL(chacha20_crypto)(const uint8 *key, const uint8 *ran,
		uint32 count, uint8 *buf, uint64 len);

#ifdef __cplusplus
}
#endif


#endif
