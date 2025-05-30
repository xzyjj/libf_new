/* aes.c - advanced encryption standard implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/aes.h>


/* @def: aes
* aes substitution box
*/
static const uint8 aes_sbox[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
	0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
	0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26,
	0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
	0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
	0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed,
	0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
	0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
	0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec,
	0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
	0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
	0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d,
	0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
	0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
	0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
	0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
	0xb0, 0x54, 0xbb, 0x16
	};

/* aes reverse substitution box */
static const uint8 aes_rsbox[256] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e,
	0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
	0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32,
	0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
	0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
	0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50,
	0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
	0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
	0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41,
	0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
	0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
	0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b,
	0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
	0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
	0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d,
	0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63,
	0x55, 0x21, 0x0c, 0x7d
	};

/* round constant */
static const uint8 aes_rcon[11] = {
	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
	};
/* end */

/* @func: _aes_rotword (static) - shift 4-byte in a word to the left
* @param1: uint8 * # word array (4-byte)
* @return: void
*/
static void _aes_rotword(uint8 *word) {
	uint8 tmp = word[0];
	word[0] = word[1];
	word[1] = word[2];
	word[2] = word[3];
	word[3] = tmp;
} /* end */

/* @func: _aes_subword (static) - s-box mapping replaces the a word 4-byte
* @param1: uint8 * # word array (4-byte)
* @return: void
*/
static void _aes_subword(uint8 *word) {
	word[0] = aes_sbox[word[0]];
	word[1] = aes_sbox[word[1]];
	word[2] = aes_sbox[word[2]];
	word[3] = aes_sbox[word[3]];
} /* end */

/* @func: _aes_rcon (static) - a word 1-byte add round constant
* @param1: uint8 * # word array (4-byte)
* @param2: int32   # round constant index
* @return: void
*/
static void _aes_rcon(uint8 *word, int32 i) {
	word[0] ^= aes_rcon[i];
} /* end */

/* @func: _aes_addroundkey (static) - add roundkey (keyexp) to the state buffer
* @param1: uint8 * # state buffer (length: AES_BLOCKSIZE)
* @param2: uint8 * # roundkey (length: AES_BLOCKSIZE)
* @return: void
*/
static void _aes_addroundkey(uint8 *state, uint8 *roundkey) {
	for (int32 i = 0; i < AES_BLOCKSIZE; i++)
		state[i] ^= roundkey[i];
} /* end */

/* @func: _aes_subbytes (static) - s-box mapping replaces the state buffer
* @param1: uint8 * # state buffer (length: AES_BLOCKSIZE)
* @return: void
*/
static void _aes_subbytes(uint8 *state) {
	for (int32 i = 0; i < AES_BLOCKSIZE; i++)
		state[i] = aes_sbox[state[i]];
} /* end */

/* @func: _aes_invsubbytes (static) - rs-box mapping replaces the state buffer
* @param1: uint8 * # state buffer (length: AES_BLOCKSIZE)
* @return: void
*/
static void _aes_invsubbytes(uint8 *state) {
	for (int32 i = 0; i < AES_BLOCKSIZE; i++)
		state[i] = aes_rsbox[state[i]];
} /* end */

/* @func: _aes_shiftrows (static) - the row in the state buffer shifted left
* @param1: uint8 * # state buffer (length: AES_BLOCKSIZE)
* @return: void
*/
static void _aes_shiftrows(uint8 *state) {
	uint8 tmp = state[1];
	state[1] = state[5];
	state[5] = state[9];
	state[9] = state[13];
	state[13] = tmp;

	tmp = state[2];
	state[2] = state[10];
	state[10] = tmp;

	tmp = state[6];
	state[6] = state[14];
	state[14] = tmp;

	tmp = state[3];
	state[3] = state[15];
	state[15] = state[11];
	state[11] = state[7];
	state[7] = tmp;
} /* end */

/* @func: _aes_invshiftrows (static) - the row in the state buffer shifted right
* @param1: uint8 * # state buffer (length: AES_BLOCKSIZE)
* @return: void
*/
static void _aes_invshiftrows(uint8 *state) {
	uint8 tmp = state[13];
	state[13] = state[9];
	state[9] = state[5];
	state[5] = state[1];
	state[1] = tmp;

	tmp = state[2];
	state[2] = state[10];
	state[10] = tmp;

	tmp = state[6];
	state[6] = state[14];
	state[14] = tmp;

	tmp = state[3];
	state[3] = state[7];
	state[7] = state[11];
	state[11] = state[15];
	state[15] = tmp;
} /* end */

/* @func: _aes_xtime (static) - compute finite field gf(2^8) multiply by 2
* @param1: uint8 # number
* @return: void
*/
static uint8 _aes_xtime(uint8 n) {
	return ((n << 1) ^ (((n >> 7) & 1) * 0x1b));
} /* end */

/* @func: _aes_mixcolumns (static) - mix column matrix transformat
* @param1: uint8 * # state buffer (length: AES_BLOCKSIZE)
* @return: void
*/
static void _aes_mixcolumns(uint8 *state) {
	uint8 a, b, c, *col;
	for (int32 i = 0; i < AES_BLOCKSIZE; i += 4) {
		col = &state[i];
		a = col[0];
		b = col[0] ^ col[1] ^ col[2] ^ col[3];

		c = col[0] ^ col[1];
		c = _aes_xtime(c) ^ b;
		col[0] ^= c;

		c = col[1] ^ col[2];
		c = _aes_xtime(c) ^ b;
		col[1] ^= c;

		c = col[2] ^ col[3];
		c = _aes_xtime(c) ^ b;
		col[2] ^= c;

		c = col[3] ^ a;
		c = _aes_xtime(c) ^ b;
		col[3] ^= c;
	}
} /* end */

/* @func: _aes_invmultiply (static) - compute finite field gf(2^8) multiply
* @param1: uint8 # number
* @param2: uint8 # number
* @return: uint8 # number
*/
static uint8 _aes_invmultiply(uint8 x, uint8 y) {
	uint8 tmp, a, b, c, d;
	a = _aes_xtime(x);
	b = _aes_xtime(a);
	c = _aes_xtime(b);
	d = _aes_xtime(c);

	tmp = (y & 1) * x;
	tmp ^= ((y >> 1) & 1) * a;
	tmp ^= ((y >> 2) & 1) * b;
	tmp ^= ((y >> 3) & 1) * c;
	tmp ^= ((y >> 4) & 1) * d;

	return tmp;
} /* end */

/* @func: _aes_invmixcolumns (static) - inverse mix column matrix transformat
* @param1: uint8 * # state buffer (length: AES_BLOCKSIZE)
* @return: void
*/
static void _aes_invmixcolumns(uint8 *state) {
	uint8 a, b, c, d, e, *col;
	for (int32 i = 0; i < AES_BLOCKSIZE; i += 4) {
		col = &state[i];
		a = col[0];
		b = col[1];
		c = col[2];
		d = col[3];

		e = _aes_invmultiply(a, 0x0e);
		e ^= _aes_invmultiply(b, 0x0b);
		e ^= _aes_invmultiply(c, 0x0d);
		e ^= _aes_invmultiply(d, 0x09);
		col[0] = e;

		e = _aes_invmultiply(a, 0x09);
		e ^= _aes_invmultiply(b, 0x0e);
		e ^= _aes_invmultiply(c, 0x0b);
		e ^= _aes_invmultiply(d, 0x0d);
		col[1] = e;

		e = _aes_invmultiply(a, 0x0d);
		e ^= _aes_invmultiply(b, 0x09);
		e ^= _aes_invmultiply(c, 0x0e);
		e ^= _aes_invmultiply(d, 0x0b);
		col[2] = e;

		e = _aes_invmultiply(a, 0x0b);
		e ^= _aes_invmultiply(b, 0x0d);
		e ^= _aes_invmultiply(c, 0x09);
		e ^= _aes_invmultiply(d, 0x0e);
		col[3] = e;
	}
} /* end */

/* @func: _aes_keyexp (static) - aes key expansion
* @param1: struct aes_ctx * # aes struct context
* @param2: uint8 *          # aes key (length: AES_*_KEYLEN)
* @return: void
*/
static void _aes_keyexp(struct aes_ctx *ctx, const uint8 *key) {
	XSYMBOL(memset)(ctx->keyexp, 0, AES_KEYEXPLEN);
	XSYMBOL(memcpy)(ctx->keyexp, key, ctx->keylen);

	uint8 tmp[4];
	for (int32 i = ctx->keylen;
			i < (AES_BLOCKSIZE * (ctx->rounds + 1));
			i += 4) {
		int32 k = i - 4;
		tmp[0] = ctx->keyexp[k];
		tmp[1] = ctx->keyexp[k + 1];
		tmp[2] = ctx->keyexp[k + 2];
		tmp[3] = ctx->keyexp[k + 3];

		if (!(i % ctx->keylen)) {
			_aes_rotword(tmp);
			_aes_subword(tmp);
			_aes_rcon(tmp, i / ctx->keylen);
		}

		k = i - ctx->keylen;
		ctx->keyexp[i] = ctx->keyexp[k] ^ tmp[0];
		ctx->keyexp[i + 1] = ctx->keyexp[k + 1] ^ tmp[1];
		ctx->keyexp[i + 2] = ctx->keyexp[k + 2] ^ tmp[2];
		ctx->keyexp[i + 3] = ctx->keyexp[k + 3] ^ tmp[3];
	}
} /* end */

/* @func: aes_init - aes struct context init
* @param1: struct aes_ctx * # aes struct context
* @param2: const uint8 *    # aes key (length: AES_*_KEYLEN)
* @param3: int32            # aes type
* @return: int32            # 0: no error, -1: type error
*/
int32 FSYMBOL(aes_init)(struct aes_ctx *ctx, const uint8 *key, int32 type) {
	if (type == 0) { /* aes128 */
		ctx->type = AES_128_TYPE;
		ctx->keylen = AES_128_KEYLEN;
		ctx->rounds = AES_128_ROUNDS;
	} else if (type == 1) { /* aes192 */
		ctx->type = AES_192_TYPE;
		ctx->keylen = AES_192_KEYLEN;
		ctx->rounds = AES_192_ROUNDS;
	} else if (type == 2) { /* aes256 */
		ctx->type = AES_256_TYPE;
		ctx->keylen = AES_256_KEYLEN;
		ctx->rounds = AES_256_ROUNDS;
	} else {
		return -1;
	}

	_aes_keyexp(ctx, key);
	ctx->count = 0;

	return 0;
} /* end */

/* @func: aes_encrypt - aes encrypt
* @param1: struct aes_ctx * # aes struct context
* @param2: uint8 *          # state buffer (length: AES_BLOCKSIZE)
* @return: void
*/
void FSYMBOL(aes_encrypt)(struct aes_ctx *ctx, uint8 *state) {
	_aes_addroundkey(state, ctx->keyexp);

	for (int32 i = 1; i < ctx->rounds; i++) {
		_aes_subbytes(state);
		_aes_shiftrows(state);
		_aes_mixcolumns(state);
		_aes_addroundkey(state, &ctx->keyexp[i * AES_BLOCKSIZE]);
	}

	_aes_subbytes(state);
	_aes_shiftrows(state);
	_aes_addroundkey(state, &ctx->keyexp[ctx->rounds * AES_BLOCKSIZE]);
} /* end */

/* @func: aes_decrypt - aes decrypt
* @param1: struct aes_ctx * # aes struct context
* @param2: uint8 *          # state buffer (length: AES_BLOCKSIZE)
* @return: void
*/
void FSYMBOL(aes_decrypt)(struct aes_ctx *ctx, uint8 *state) {
	_aes_addroundkey(state, &ctx->keyexp[ctx->rounds * AES_BLOCKSIZE]);

	for (int32 i = ctx->rounds - 1; i > 0; i--) {
		_aes_invshiftrows(state);
		_aes_invsubbytes(state);
		_aes_addroundkey(state, &ctx->keyexp[i * AES_BLOCKSIZE]);
		_aes_invmixcolumns(state);
	}

	_aes_invshiftrows(state);
	_aes_invsubbytes(state);
	_aes_addroundkey(state, ctx->keyexp);
} /* end */

/* @func: aes_encrypt_process - aes encrypt processing buffer
* @param1: struct aes_ctx * # aes struct context
* @param2: int32 (*)(const uint8 *, void *) # output callback
* @param3: void *           # callback arg
* @param4: const uint8 *    # input buffer
* @param5: uint64           # input length
* @return: int32            # 0: no error, -1: callback error
*/
int32 FSYMBOL(aes_encrypt_process)(struct aes_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg,
		const uint8 *s, uint64 len) {
	int32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == AES_BLOCKSIZE) {
			FSYMBOL(aes_encrypt)(ctx, ctx->buf);
			if (call(ctx->buf, arg))
				return -1;
			n = 0;
		}
	}
	ctx->count = n;

	return 0;
} /* end */

/* @func: aes_encrypt_finish - aes encrypt process the remaining bytes in \
*                              the buffer and end
* @param1: struct aes_ctx * # aes struct context
* @param2: int32 (*)(const uint8 *, void *) # output callback
* @param3: void *           # callback arg
* @return: int32            # 0: no error, -1: callback error
*/
int32 FSYMBOL(aes_encrypt_finish)(struct aes_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg) {
	if (!ctx->count)
		return 0;

	XSYMBOL(memset)(&ctx->buf[ctx->count], 0, AES_BLOCKSIZE - ctx->count);
	FSYMBOL(aes_encrypt)(ctx, ctx->buf);
	if (call(ctx->buf, arg))
		return -1;
	ctx->count = 0;

	return 0;
} /* end */

/* @func: aes_decrypt_process - aes decrypt processing buffer
* @param1: struct aes_ctx * # aes struct context
* @param2: int32 (*)(const uint8 *, void *) # output callback
* @param3: void *           # callback arg
* @param4: const uint8 *    # input buffer
* @param5: uint64           # input length
* @return: int32            # 0: no error, -1: callback error
*/
int32 FSYMBOL(aes_decrypt_process)(struct aes_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg,
		const uint8 *s, uint64 len) {
	int32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == AES_BLOCKSIZE) {
			FSYMBOL(aes_decrypt)(ctx, ctx->buf);
			if (call(ctx->buf, arg))
				return -1;
			n = 0;
		}
	}
	ctx->count = n;

	return 0;
} /* end */
