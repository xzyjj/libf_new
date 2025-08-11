//* ed25519_fast_sign.c - edwards-curve digital signature algorithm (eddsa) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/sha512.h>
#include <libf/al/ed25519_fast.h>


/* @def: ed25519_fast
* Base Point Order
*   P1..P15 = q * (1..15) */
static const uint32 _w_BPO[16][8] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0x5cf5d3ed, 0x5812631a, 0xa2f79cd6, 0x14def9de, 0, 0, 0, 0x10000000 },
	{ 0xb9eba7da, 0xb024c634, 0x45ef39ac, 0x29bdf3bd, 0, 0, 0, 0x20000000 },
	{ 0x16e17bc7, 0x0837294f, 0xe8e6d683, 0x3e9ced9b, 0, 0, 0, 0x30000000 },
	{ 0x73d74fb4, 0x60498c69, 0x8bde7359, 0x537be77a, 0, 0, 0, 0x40000000 },
	{ 0xd0cd23a1, 0xb85bef83, 0x2ed6102f, 0x685ae159, 0, 0, 0, 0x50000000 },
	{ 0x2dc2f78e, 0x106e529e, 0xd1cdad06, 0x7d39db37, 0, 0, 0, 0x60000000 },
	{ 0x8ab8cb7b, 0x6880b5b8, 0x74c549dc, 0x9218d516, 0, 0, 0, 0x70000000 },
	{ 0xe7ae9f68, 0xc09318d2, 0x17bce6b2, 0xa6f7cef5, 0, 0, 0, 0x80000000 },
	{ 0x44a47355, 0x18a57bed, 0xbab48389, 0xbbd6c8d3, 0, 0, 0, 0x90000000 },
	{ 0xa19a4742, 0x70b7df07, 0x5dac205f, 0xd0b5c2b2, 0, 0, 0, 0xa0000000 },
	{ 0xfe901b2f, 0xc8ca4221, 0x00a3bd35, 0xe594bc91, 0, 0, 0, 0xb0000000 },
	{ 0x5b85ef1c, 0x20dca53c, 0xa39b5a0c, 0xfa73b66f, 0, 0, 0, 0xc0000000 },
	{ 0xb87bc309, 0x78ef0856, 0x4692f6e2, 0x0f52b04e, 1, 0, 0, 0xd0000000 },
	{ 0x157196f6, 0xd1016b71, 0xe98a93b8, 0x2431aa2c, 1, 0, 0, 0xe0000000 },
	{ 0x72676ae3, 0x2913ce8b, 0x8c82308f, 0x3910a40b, 1, 0, 0, 0xf0000000 }
	};

/* -R = (q - (2**256)) % q
*       443877084437957656573631004654138375888 */
static const uint32 _w_minusR[8] = {
	0xcf5d3ed0, 0x812631a5, 0x2f79cd65, 0x4def9dea, 1, 0, 0, 0
	};
/* end */

/* @func: _ed25519_modw (static) - curve25519 BPO (base point order) \
*                                  modular reduction
* @param1: uint32 [8]       # result
* @param2: const uint32 [8] # number
* @param3: uint32           # high-bit
* @return: void
*/
static void _ed25519_modw(uint32 r[8],
		const uint32 a[8], uint32 b) {
	const uint32 *p;
	uint32 rr[8];
	uint32 carry = 0;
	uint64L tmp = 0;

	/* NOTE: (a + (b * R)) % q === (a - (b * (-R))) % q */

	/* rr = r * minusR */
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)b * _w_minusR[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = a - rr */
	carry = 0;
	for (int32 i = 0; i < 8; i++) {
		tmp = (int64L)a[i] - rr[i] + (int32)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r + p */
	p = _w_BPO[carry & 1];
	carry = 0;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)r[i] + p[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: _ed25519_mod (static) - curve25519 modular reduction
* @param1: uint32 [8] # number
* @return: void
*/
static void _ed25519_mod(uint32 r[8]) {
	const uint32 *p = _w_BPO[r[7] >> 28]; /* 256 - 252 = 4 */
	uint32 carry = 0;
	uint64L tmp = 0;

	/* r = r - p */
	for (int32 i = 0; i < 8; i++) {
		tmp = (int64L)r[i] - p[i] + (int32)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r + p */
	p = _w_BPO[carry & 1];
	carry = 0;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)r[i] + p[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: _ed25519_mul (static) - curve25519 multiplication (modular reduction)
* @param1: uint32 [8]       # product
* @param2: const uint32 [8] # multiplicand
* @param3: const uint32 [8] # multiplier
* @return: void
*/
static void _ed25519_mul(uint32 r[8],
		const uint32 a[8], const uint32 b[8]) {
	uint32 rr[16];
	uint32 carry = 0;
	uint64L tmp = 0;

	for (int32 i = 0; i < 16; i++)
		rr[i] = 0;

	/* rr = a * b */
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)a[0] * b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
	rr[8] = carry;

	for (int32 i = 1; i < 8; i++) {
		carry = 0;
		for (int32 j = 0; j < 8; j++) {
			tmp = (uint64L)a[i] * b[j] + rr[i + j] + carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
		rr[i + 8] = carry;
	}

	/* r = rr % q */
	_ed25519_modw(rr + 7, rr + 7, rr[15]);
	_ed25519_modw(rr + 6, rr + 6, rr[14]);
	_ed25519_modw(rr + 5, rr + 5, rr[13]);
	_ed25519_modw(rr + 4, rr + 4, rr[12]);
	_ed25519_modw(rr + 3, rr + 3, rr[11]);
	_ed25519_modw(rr + 2, rr + 2, rr[10]);
	_ed25519_modw(rr + 1, rr + 1, rr[9]);
	_ed25519_modw(r, rr, rr[8]);
} /* end */

/* @func: _ed25519_add (static) - curve25519 addition (modular reduction)
* @param1: uint32 [8]       # sum
* @param2: const uint32 [8] # addend
* @param3: const uint32 [8] # addend
* @return: void
*/
static void _ed25519_add(uint32 r[8],
		const uint32 a[8], const uint32 b[8]) {
	uint32 carry = 0;
	uint64L tmp = 0;

	/* r = a + b */
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)a[i] + b[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r % q */
	_ed25519_modw(r, r, carry);
} /* end */

/* @func: _ed25519_digest (static) - sha512 digest modular reduction
* @param1: const uint8 [64] # sha512 digest
* @param2: uint32 [8]       # result
* @return: void
*/
static void _ed25519_digest(uint8 dig[64], uint32 r[8]) {
	uint32 rr[16];
	XSYMBOL(memcpy)(rr, dig, ED25519_SIGN_LEN),

	/* r = rr % q */
	_ed25519_modw(rr + 7, rr + 7, rr[15]);
	_ed25519_modw(rr + 6, rr + 6, rr[14]);
	_ed25519_modw(rr + 5, rr + 5, rr[13]);
	_ed25519_modw(rr + 4, rr + 4, rr[12]);
	_ed25519_modw(rr + 3, rr + 3, rr[11]);
	_ed25519_modw(rr + 2, rr + 2, rr[10]);
	_ed25519_modw(rr + 1, rr + 1, rr[9]);
	_ed25519_modw(r, rr, rr[8]);
} /* end */

/* @func: ed25519_fast_private_key - ed25519 private key operation function
* @param1: const uint8 * # input private key (length: ED25519_LEN)
* @param2: uint32 [8]    # private key
* @param3: uint32 [8]    # nonce
* @return: void
*/
void FSYMBOL(ed25519_fast_private_key)(const uint8 *key, uint32 pri[8],
		uint32 ran[8]) {
	SHA512_NEW(sha_ctx);
	FSYMBOL(sha512_init)(&sha_ctx);
	FSYMBOL(sha512)(&sha_ctx, key, ED25519_LEN);

	/* copy(pri, sha, ED25519_LEN) */
	XSYMBOL(memcpy)(pri, &(SHA512_STATE(&sha_ctx, 0)),
		ED25519_LEN);
	/* copy(ran, sha + ED25519_LEN, ED25519_LEN) */
	XSYMBOL(memcpy)(ran, &(SHA512_STATE(&sha_ctx, ED25519_LEN)),
		ED25519_LEN);

	FSYMBOL(ed25519_fast_clamp_key)(pri);
} /* end */

/* @func: ed25519_fast_public_key - ed25519 public key create
* @param1: const uint8 * # private key (length: ED25519_LEN)
* @param2: uint8 *       # public key (length: ED25519_LEN)
* @return: void
*/
void FSYMBOL(ed25519_fast_public_key)(const uint8 *pri, uint8 *pub) {
	uint32 _pri[8], _ran[8];
	struct ed25519_point xyz2;

	FSYMBOL(ed25519_fast_private_key)(pri, _pri, _ran);

	/* pub = compress(scalar(pri, ED25519_FAST_BASE)) */
	FSYMBOL(ed25519_fast_scalar_mul)(_pri, &(ED25519_FAST_BASE), &xyz2);
	FSYMBOL(ed25519_fast_point_compress)(&xyz2, (uint32 *)pub);
} /* end */

/* @func: ed25519_fast_sign - ed25519 signature function
* @param1: const uint8 * # private key (length: ED25519_LEN)
* @param2: const uint8 * # input message
* @param3: uint64        # message length
* @param4: uint8 *       # signature (length: ED25519_SIGN_LEN)
* @return: void
*/
void FSYMBOL(ed25519_fast_sign)(const uint8 *pri, const uint8 *mesg,
		uint64 len, uint8 *sign) {
	uint32 _pri[8], _ran[8], _pub[8], r[8], Rs[8], h[8], s[8];
	struct ed25519_point xyz2;
	SHA512_NEW(sha_ctx);

	FSYMBOL(ed25519_fast_private_key)(pri, _pri, _ran);
	FSYMBOL(ed25519_fast_public_key)(pri, (uint8 *)_pub);

	/* sh = sha(_ran + mesg) */
	FSYMBOL(sha512_init)(&sha_ctx);
	FSYMBOL(sha512_process)(&sha_ctx, (uint8 *)_ran, ED25519_LEN);
	FSYMBOL(sha512_process)(&sha_ctx, mesg, len);
	FSYMBOL(sha512_finish)(&sha_ctx, len + ED25519_LEN);

	/* r = digest(sh) % q */
	_ed25519_digest(&(SHA512_STATE(&sha_ctx, 0)), r);
	_ed25519_mod(r);

	/* Rs = compress(scalar(r, ED25519_FAST_BASE)) */
	FSYMBOL(ed25519_fast_scalar_mul)(r, &(ED25519_FAST_BASE), &xyz2);
	FSYMBOL(ed25519_fast_point_compress)(&xyz2, Rs);

	/* sh = sha(Rs + _pub + mesg) */
	FSYMBOL(sha512_init)(&sha_ctx);
	FSYMBOL(sha512_process)(&sha_ctx, (uint8 *)Rs, ED25519_LEN);
	FSYMBOL(sha512_process)(&sha_ctx, (uint8 *)_pub, ED25519_LEN);
	FSYMBOL(sha512_process)(&sha_ctx, mesg, len);
	FSYMBOL(sha512_finish)(&sha_ctx, len + (ED25519_LEN * 2));

	/* h = digest(sh) */
	_ed25519_digest(&(SHA512_STATE(&sha_ctx, 0)), h);

	/* s = ((h * _pri) + r) % q */
	_ed25519_mul(s, h, _pri);
	_ed25519_add(s, s, r);
	_ed25519_mod(s);

	XSYMBOL(memcpy)(sign, Rs, ED25519_LEN);
	XSYMBOL(memcpy)(sign + ED25519_LEN, s, ED25519_LEN);
} /* end */

/* @func: ed25519_fast_verify - ed25519 signature verification function
* @param1: const uint8 * # public key (length: ED25519_LEN)
* @param2: const uint8 * # signature (length: ED25519_SIGN_LEN)
* @param3: const uint8 * # input message
* @param4: uint64        # message length
* @return: int32         # 0: success, 1: fail
*/
int32 FSYMBOL(ed25519_fast_verify)(const uint8 *pub, const uint8 *sign,
		const uint8 *mesg, uint64 len) {
	uint32 Rs[8], s[8], h[8];
	struct ed25519_point A, R, hA, sB;
	SHA512_NEW(sha_ctx);

	XSYMBOL(memcpy)(Rs, sign, ED25519_LEN);
	XSYMBOL(memcpy)(s, sign + ED25519_LEN, ED25519_LEN);

	/* A = decompress(pub) */
	FSYMBOL(ed25519_fast_point_decompress)((uint32 *)pub, &A);
	/* R = decompress(Rs) */
	FSYMBOL(ed25519_fast_point_decompress)(Rs, &R);

	/* sh = sha(Rs + pub + mesg) */
	FSYMBOL(sha512_init)(&sha_ctx);
	FSYMBOL(sha512_process)(&sha_ctx, (uint8 *)Rs, ED25519_LEN);
	FSYMBOL(sha512_process)(&sha_ctx, (uint8 *)pub, ED25519_LEN);
	FSYMBOL(sha512_process)(&sha_ctx, mesg, len);
	FSYMBOL(sha512_finish)(&sha_ctx, len + (ED25519_LEN * 2));

	/* h = digest(sh) % q */
	_ed25519_digest(&(SHA512_STATE(&sha_ctx, 0)), h);
	_ed25519_mod(h);

	/* hA = scalar(h, A) */
	FSYMBOL(ed25519_fast_scalar_mul)(h, &A, &hA);
	/* hA = add(R, hA) */
	FSYMBOL(ed25519_fast_point_add)(&R, &hA, &hA);

	/* sB = scalar(s, ED25519_FAST_BASE) */
	FSYMBOL(ed25519_fast_scalar_mul)(s, &(ED25519_FAST_BASE), &sB);

	return FSYMBOL(ed25519_fast_point_equal)(&hA, &sB);
} /* end */
