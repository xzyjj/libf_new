/* x25519_fast.c - curve25519 elliptic-curve crypto (ecc) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/x25519_fast.h>


/* @func: _x25519_swap (static) - numerical swap
* @param1: uint32 [8] # number a
* @param2: uint32 [8] # number b
* @param3: uint32     # low bit (0: keep, 1: swap)
* @return: void
*/
static void _x25519_swap(uint32 a[8], uint32 b[8], uint32 bit) {
	bit = (~bit & 1) - 1;
	for (int32 i = 0; i < 8; i++) {
		uint32 t = bit & (a[i] ^ b[i]);
		a[i] ^= t;
		b[i] ^= t;
	}
} /* end */

/* @func: _x25519_add (static) - curve25519 addition (modular reduction)
* @param1: uint32 [8]       # sum
* @param2: const uint32 [8] # addend
* @param3: const uint32 [8] # addend
* @return: void
*/
static void _x25519_add(uint32 r[8],
		const uint32 a[8], const uint32 b[8]) {
	uint32 carry = 0;
	uint64L tmp = 0;

	/* r = a + b */
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)a[i] + b[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* NOTE: (2**256) % (2**255-19) = 38 */

	/* r = r % p modular reduction */
	carry *= 38;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry *= 38;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: _x25519_sub (static) - curve25519 subtraction (modular reduction)
* @param1: uint32 [8]       # difference
* @param2: const uint32 [8] # minuend
* @param3: const uint32 [8] # subtract
* @return: void
*/
static void _x25519_sub(uint32 r[8],
		const uint32 a[8], const uint32 b[8]) {
	uint32 carry = 0;
	uint64L tmp = 0;

	/* r = a - b */
	for (int32 i = 0; i < 8; i++) {
		tmp = (int64L)a[i] - b[i] + (int32)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* NOTE: (2**256) % (2**255-19) = 38 */

	/* r = r % p modular reduction */
	tmp = (uint64L)r[0] - (carry & 38);
	r[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32 i = 1; i < 8; i++) {
		tmp = (uint64L)r[i] + (int32)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64L)r[0] - (carry & 38);
	r[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32 i = 1; i < 8; i++) {
		tmp = (uint64L)r[i] + (int32)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: _x25519_mul (static) - curve25519 multiplication (modular reduction)
* @param1: uint32 [8]       # product
* @param2: const uint32 [8] # multiplicand
* @param3: const uint32 [8] # multiplier
* @return: void
*/
static void _x25519_mul(uint32 r[8],
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

	/* NOTE: (2**256) % (2**255-19) = 38 */

	/* r = rr % p modular reduction */
	carry = 0;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)rr[i + 8] * 38 + rr[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry *= 38;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry *= 38;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: _x25519_mul121665 (static) - curve25519 '(486662 - 2) / 4 = 121665' \
*                                      multiplication (modular reduction)
* @param1: uint32 [8]       # product
* @param2: const uint32 [8] # multiplicand
* @param2: const uint32 [8] # addend
* @return: void
*/
static void _x25519_mul121665(uint32 r[8],
		const uint32 a[8], const uint32 b[8]) {
	uint32 carry = 0;
	uint64L tmp = 0;

	/* r = a * 121665 + b, (486662 - 2) / 4 = 121665 */
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)a[i] * 121665 + b[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* NOTE: (2**256) % (2**255-19) = 38 */

	/* r = r % p modular reduction */
	carry *= 38;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry *= 38;
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: _x25519_inv (static) - modular inversion based on fermat's \
*                                little theorem
* @param1: uint32 [8]       # inverse modulus
* @param2: const uint32 [8] # number
* @return: void
*/
static void _x25519_inv(uint32 r[8], const uint32 z[8]) {
	uint32 z2[8], z9[8], z11[8];
	uint32 z2_5_0[8], z2_10_0[8], z2_20_0[8], z2_50_0[8], z2_100_0[8];
	uint32 t0[8], t1[8];

	/* 2 */
	_x25519_mul(z2, z, z);
	/* 4 */
	_x25519_mul(t1, z2, z2);
	/* 8 */
	_x25519_mul(t0, t1, t1);
	/* 9 */
	_x25519_mul(z9, t0, z);
	/* 11 */
	_x25519_mul(z11, z9, z2);
	/* 22 */
	_x25519_mul(t0, z11, z11);
	/* (2**5) - (2**0) */
	_x25519_mul(z2_5_0, t0, z9);

	/* (2**6) - (2**1) */
	_x25519_mul(t0, z2_5_0, z2_5_0);
	/* (2**7) - (2**2) */
	_x25519_mul(t1, t0, t0);
	/* (2**8) - (2**3) */
	_x25519_mul(t0, t1, t1);
	/* (2**9) - (2**4) */
	_x25519_mul(t1, t0, t0);
	/* (2**10) - (2**5) */
	_x25519_mul(t0, t1, t1);
	/* (2**10) - (2**0) */
	_x25519_mul(z2_10_0, t0, z2_5_0);

	/* (2**11) - (2**1) */
	_x25519_mul(t0, z2_10_0, z2_10_0);
	/* (2**12) - (2**2) */
	_x25519_mul(t1, t0, t0);
	/* (2**20) - (2**10) */
	for (int32 i = 2; i < 10; i += 2) {
		_x25519_mul(t0, t1, t1);
		_x25519_mul(t1, t0, t0);
	}
	/* (2**20) - (2**0) */
	_x25519_mul(z2_20_0, t1, z2_10_0);

	/* (2**21) - (2**1) */
	_x25519_mul(t0, z2_20_0, z2_20_0);
	/* (2**22) - (2**2) */
	_x25519_mul(t1, t0, t0);
	/* (2**40) - (2**20) */
	for (int32 i = 2; i < 20; i += 2) {
		_x25519_mul(t0, t1, t1);
		_x25519_mul(t1, t0, t0);
	}
	/* (2**40) - (2**0) */
	_x25519_mul(t0,t1,z2_20_0);

	/* (2**41) - (2**1) */
	_x25519_mul(t1, t0, t0);
	/* (2**42) - (2**2) */
	_x25519_mul(t0, t1, t1);
	/* (2**50) - (2**10) */
	for (int32 i = 2; i < 10; i += 2) {
		_x25519_mul(t1, t0, t0);
		_x25519_mul(t0, t1, t1);
	}
	/* (2**50) - (2**0) */
	_x25519_mul(z2_50_0, t0, z2_10_0);

	/* (2**51) - (2**1) */
	_x25519_mul(t0, z2_50_0, z2_50_0);
	/* (2**52) - (2**2) */
	_x25519_mul(t1, t0, t0);
	/* (2**100) - (2**50) */
	for (int32 i = 2; i < 50; i += 2) {
		_x25519_mul(t0, t1, t1);
		_x25519_mul(t1, t0, t0);
	}
	/* (2**100) - (2**0) */
	_x25519_mul(z2_100_0, t1, z2_50_0);

	/* (2**101) - (2**1) */
	_x25519_mul(t1, z2_100_0, z2_100_0);
	/* (2**102) - (2**2) */
	_x25519_mul(t0, t1, t1);
	/* (2**200) - (2**100) */
	for (int32 i = 2; i < 100; i += 2) {
		_x25519_mul(t1, t0, t0);
		_x25519_mul(t0, t1, t1);
	}
	/* (2**200) - (2**0) */
	_x25519_mul(t1, t0, z2_100_0);

	/* (2**201) - (2**1) */
	_x25519_mul(t0, t1, t1);
	/* (2**202) - (2**2) */
	_x25519_mul(t1, t0, t0);
	/* (2**250) - (2**50) */
	for (int32 i = 2; i < 50; i += 2) {
		_x25519_mul(t0, t1, t1);
		_x25519_mul(t1, t0, t0);
	}
	/* (2**250) - (2**0) */
	_x25519_mul(t0, t1, z2_50_0);

	/* (2**251) - (2**1) */
	_x25519_mul(t1, t0, t0);
	/* (2**252) - (2**2) */
	_x25519_mul(t0, t1, t1);
	/* (2**253) - (2**3) */
	_x25519_mul(t1, t0, t0);
	/* (2**254) - (2**4) */
	_x25519_mul(t0, t1, t1);
	/* (2**255) - (2**5) */
	_x25519_mul(t1, t0, t0);
	/* (2**255) - 21 */
	_x25519_mul(r, t1, z11);
} /* end */

/* @func: _x25519_mod (static) - curve25519 modular reduction (norm)
* @param1: uint32 [8] # number
* @return: void
*/
static void _x25519_mod(uint32 r[8]) {
	const uint32 *p = X25519_FAST_P;
	uint32 rr[8], t[8];
	uint32 carry = 0;
	uint64L tmp = 0;

	/* rr = r - p */
	for (int32 i = 0; i < 8; i++) {
		tmp = (int64L)r[i] - p[i] + (int32)carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* t = if r < p : adjust t */
	t[0] = carry & 0xffffffed;
	for (int32 i = 1; i < 7; i++)
		t[i] = carry;
	t[7] = carry >> 1;

	/* rr = rr + t */
	for (int32 i = 0; i < 8; i++) {
		tmp = (uint64L)rr[i] + t[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* swap */
	_x25519_swap(r, rr, r[7] >> 31);
} /* end */

/* @func: x25519_fast_scalar_mul - montgomery ladder scalar multiplication
* @param1: const uint32 [8] # private key
* @param2: const uint32 [8] # base point
* @param3: uint32 [8]       # scalar
* @return: void
*/
void FSYMBOL(x25519_fast_scalar_mul)(const uint32 k[8], const uint32 b[8],
		uint32 r[8]) {
	uint32 x1[8], z1[8], x2[8], z2[8];
	for (int32 i = 0; i < 8; i++) {
		x1[i] = 0;
		z1[i] = 0;
		x2[i] = b[i];
		z2[i] = 0;
	}
	x1[0] = 1;
	z2[0] = 1;
	/* x1, z1 = (1, 0) */
	/* x2, z2 = (b, 1) */

	uint32 A[8], AA[8], B[8], BB[8], E[8], C[8], D[8], DA[8], CB[8];
	for (int32 i = 255; i >= 0; i--) {
		uint32 k_t = k[i / 32] >> (i % 32);
		_x25519_swap(x1, x2, k_t);
		_x25519_swap(z1, z2, k_t);

		/* A = x1 + z1 */
		_x25519_add(A, x1, z1);
		/* AA = A ** 2 */
		_x25519_mul(AA, A, A);

		/* B = x1 - z1 */
		_x25519_sub(B, x1, z1);
		/* BB = B ** 2 */
		_x25519_mul(BB, B, B);

		/* E = AA - BB */
		_x25519_sub(E, AA, BB);
		/* C = x2 + z2 */
		_x25519_add(C, x2, z2);
		/* D = x2 - z2 */
		_x25519_sub(D, x2, z2);

		/* DA = D * A */
		_x25519_mul(DA, D, A);
		/* CB = C * B */
		_x25519_mul(CB, C, B);

		/* x2 = (DA * CB) ** 2 */
		_x25519_add(x2, DA, CB);
		_x25519_mul(x2, x2, x2);

		/* z2 = b * ((DA - CB) ** 2) */
		_x25519_sub(z2, DA, CB);
		_x25519_mul(z2, z2, z2);
		_x25519_mul(z2, z2, b);

		/* x1 = AA * BB */
		_x25519_mul(x1, AA, BB);

		/* z1 = E * (AA + 121665 * E) */
		_x25519_mul121665(z1, E, AA);
		/* _x25519_add(z1, z1, AA); */
		_x25519_mul(z1, z1, E);

		_x25519_swap(x1, x2, k_t);
		_x25519_swap(z1, z2, k_t);
	}

	/* r = (inv(z1) * x1) % p */
	_x25519_inv(z1, z1);
	_x25519_mul(r, z1, x1);
	_x25519_mod(r);
} /* end */

/* @func: x25519_fast_clamp_key - private key clamping
* @param1: uint32 [8] # private key
* @return: void
*/
void FSYMBOL(x25519_fast_clamp_key)(uint32 k[8]) {
	k[0] &= ~0x07;
	k[7] |= 0x40000000;
	k[7] &= 0x7fffffff;
} /* end */

/* @func: x25519_fast_base_mask - mask the most significant bit of 'u'
* @param1: uint32 [8] # base point
* @return: void
*/
void FSYMBOL(x25519_fast_base_mask)(uint32 b[8]) {
	b[7] &= 0x7fffffff;
} /* end */

/* @func: x25519_fast_private_key - x25519 private key operation function
* @param1: uint8 * # private key (length: X25519_LEN)
* @return: void
*/
void FSYMBOL(x25519_fast_private_key)(uint8 *pri) {
	uint32 _pri[8];
	XSYMBOL(memcpy)(_pri, pri, X25519_LEN);

	FSYMBOL(x25519_fast_clamp_key)(_pri);

	XSYMBOL(memcpy)(pri, _pri, X25519_LEN);
} /* end */

/* @func: x25519_fast_public_key - x25519 public key create function
* @param1: const uint8 * # private key (length: X25519_LEN)
* @param2: uint8 *       # public key (length: X25519_LEN)
* @return: void
*/
void FSYMBOL(x25519_fast_public_key)(const uint8 *pri, uint8 *pub) {
	uint32 _pri[8], _pub[8];
	XSYMBOL(memcpy)(_pri, pri, X25519_LEN);
	XSYMBOL(memcpy)(_pub, pub, X25519_LEN);

	FSYMBOL(x25519_fast_scalar_mul)(_pri, X25519_FAST_B, _pub);

	XSYMBOL(memcpy)(pub, _pub, X25519_LEN);
} /* end */

/* @func: x25519_fast_shared_key - x25519 shared key create function
* @param1: const uint8 * # private key (length: X25519_LEN)
* @param2: const uint8 * # public key (length: X25519_LEN)
* @param3: uint8 *       # shared key (length: X25519_LEN)
* @return: void
*/
void FSYMBOL(x25519_fast_shared_key)(const uint8 *pri, const uint8 *pub,
		uint8 *key) {
	uint32 _pri[8], _pub[8], _key[8];
	XSYMBOL(memcpy)(_pri, pri, X25519_LEN);
	XSYMBOL(memcpy)(_pub, pub, X25519_LEN);
	XSYMBOL(memcpy)(_key, key, X25519_LEN);

	FSYMBOL(x25519_fast_base_mask)(_pub);
	FSYMBOL(x25519_fast_scalar_mul)(_pri, _pub, _key);

	XSYMBOL(memcpy)(key, _key, X25519_LEN);
} /* end */
