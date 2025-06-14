/* chacha20.c - chacha20 encryption implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/chacha20.h>


/* @def: chacha20
* key padding constant */
static const uint8 chacha20_constant[] = "expand 32-byte k";

#undef PACK4
#define PACK4(x) ((uint32)((x)[0]) | (uint32)((x)[1]) << 8 \
	| (uint32)((x)[2]) << 16 | (uint32)((x)[3]) << 24)

#undef ROTL32
#define ROTL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
/* end */

/* @func: _chacha20_qr (static) - chacha20 quarter round function
* @param1: uint32 [16] # state buffer
* @param2: uint32      # index a
* @param3: uint32      # index b
* @param4: uint32      # index c
* @param5: uint32      # index d
* @return: void
*/
static void _chacha20_qr(uint32 state[16], uint32 a, uint32 b,
		uint32 c, uint32 d) {
	state[a] += state[b];
	state[d] ^= state[a];
	state[d] = ROTL32(state[d], 16);

	state[c] += state[d];
	state[b] ^= state[c];
	state[b] = ROTL32(state[b], 12);

	state[a] += state[b];
	state[d] ^= state[a];
	state[d] = ROTL32(state[d], 8);

	state[c] += state[d];
	state[b] ^= state[c];
	state[b] = ROTL32(state[b], 7);
} /* end */

/* @func: chacha20_init_state - chacha20 init state
* @param1: uint32 [16]      # state buffer
* @param2: const uint8 [64] # key
* @param3: const uint8 [12] # nonce
* @param4: uint32           # block count
* @return: void
*/
void FSYMBOL(chacha20_init_state)(uint32 state[16], const uint8 key[64],
		const uint8 ran[12], uint32 count) {
	state[0] = PACK4(chacha20_constant);
	state[1] = PACK4(chacha20_constant + 4);
	state[2] = PACK4(chacha20_constant + 8);
	state[3] = PACK4(chacha20_constant + 12);

	state[4] = PACK4(key);
	state[5] = PACK4(key + 4);
	state[6] = PACK4(key + 8);
	state[7] = PACK4(key + 12);
	state[8] = PACK4(key + 16);
	state[9] = PACK4(key + 20);
	state[10] = PACK4(key + 24);
	state[11] = PACK4(key + 28);

	state[12] = count;

	state[13] = PACK4(ran);
	state[14] = PACK4(ran + 4);
	state[15] = PACK4(ran + 8);
} /* end */

/* @func: chacha20_block - chacha20 block function
* @param1: const uint32 [16] # state buffer
* @param2: uint32 [16]       # keystream
* @return: void
*/
void FSYMBOL(chacha20_block)(const uint32 state[16], uint32 out[16]) {
	for (int32 i = 0; i < 16; i++)
		out[i] = state[i];

	for (int32 i = 0; i < 10; i++) {
		/* column */
		_chacha20_qr(out, 0, 4, 8, 12);
		_chacha20_qr(out, 1, 5, 9, 13);
		_chacha20_qr(out, 2, 6, 10, 14);
		_chacha20_qr(out, 3, 7, 11, 15);

		/* diagonal */
		_chacha20_qr(out, 0, 5, 10, 15);
		_chacha20_qr(out, 1, 6, 11, 12);
		_chacha20_qr(out, 2, 7, 8, 13);
		_chacha20_qr(out, 3, 4, 9, 14);
	}

	for (int32 i = 0; i < 16; i++)
		out[i] += state[i];
} /* end */

/* @func: chacha20_crypto - chacha20 encrypt and decrypt (stream)
* @param1: const uint8 * # key (length: CHACHA20_KEYLEN)
* @param2: const uint8 * # nonce (length: CHACHA20_RANLEN)
* @param3: uint32        # count block (starting)
* @param4: uint8 *       # buffer
* @param5: uint64        # length
* @return: uint32        # count (+initial count)
*/
uint32 FSYMBOL(chacha20_crypto)(const uint8 *key, const uint8 *ran,
		uint32 count, uint8 *buf, uint64 len) {
	uint32 state[16];
	uint8 keystream[CHACHA20_BLOCKSIZE];
	FSYMBOL(chacha20_init_state)(state, key, ran, count);

	uint32 n = CHACHA20_BLOCKSIZE;
	while (len) {
		if (len < n)
			n = len;

		FSYMBOL(chacha20_block)(state, (uint32 *)keystream);
		for (uint32 i = 0; i < n; i++)
			buf[i] ^= keystream[i];

		buf += n;
		len -= n;
		state[CHACHA20_CINDEX]++;
	}

	return state[CHACHA20_CINDEX];
} /* end */
