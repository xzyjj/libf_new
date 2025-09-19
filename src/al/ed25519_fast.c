/* ed25519_fast.c - edwards-curve digital signature algorithm (eddsa) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/ed25519_fast.h>


/* @func: _ed25519_swap (static) - numerical swap
* @param1: uint32 [8] # number a
* @param2: uint32 [8] # number b
* @param3: uint32     # low bit (0: keep, 1: swap)
* @return: void
*/
static void _ed25519_swap(uint32 a[8], uint32 b[8], uint32 bit) {
	bit = (~bit & 1) - 1;
	for (int32 i = 0; i < 8; i++) {
		uint32 t = bit & (a[i] ^ b[i]);
		a[i] ^= t;
		b[i] ^= t;
	}
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

/* @func: _ed25519_sub (static) - curve25519 subtraction (modular reduction)
* @param1: uint32 [8]       # difference
* @param2: const uint32 [8] # minuend
* @param3: const uint32 [8] # subtract
* @return: void
*/
static void _ed25519_sub(uint32 r[8],
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

/* @func: _ed25519_inv (static) - modular inversion based on fermat's \
*                                 little theorem
* @param1: uint32 [8]       # inverse modulus
* @param2: const uint32 [8] # number
* @return: void
*/
static void _ed25519_inv(uint32 r[8], const uint32 z[8]) {
	uint32 z2[8], z9[8], z11[8];
	uint32 z2_5_0[8], z2_10_0[8], z2_20_0[8], z2_50_0[8], z2_100_0[8];
	uint32 t0[8], t1[8];

	/* 2 */
	_ed25519_mul(z2, z, z);
	/* 4 */
	_ed25519_mul(t1, z2, z2);
	/* 8 */
	_ed25519_mul(t0, t1, t1);
	/* 9 */
	_ed25519_mul(z9, t0, z);
	/* 11 */
	_ed25519_mul(z11, z9, z2);
	/* 22 */
	_ed25519_mul(t0, z11, z11);
	/* (2**5) - (2**0) */
	_ed25519_mul(z2_5_0, t0, z9);

	/* (2**6) - (2**1) */
	_ed25519_mul(t0, z2_5_0, z2_5_0);
	/* (2**7) - (2**2) */
	_ed25519_mul(t1, t0, t0);
	/* (2**8) - (2**3) */
	_ed25519_mul(t0, t1, t1);
	/* (2**9) - (2**4) */
	_ed25519_mul(t1, t0, t0);
	/* (2**10) - (2**5) */
	_ed25519_mul(t0, t1, t1);
	/* (2**10) - (2**0) */
	_ed25519_mul(z2_10_0, t0, z2_5_0);

	/* (2**11) - (2**1) */
	_ed25519_mul(t0, z2_10_0, z2_10_0);
	/* (2**12) - (2**2) */
	_ed25519_mul(t1, t0, t0);
	/* (2**20) - (2**10) */
	for (int32 i = 2; i < 10; i += 2) {
		_ed25519_mul(t0, t1, t1);
		_ed25519_mul(t1, t0, t0);
	}
	/* (2**20) - (2**0) */
	_ed25519_mul(z2_20_0, t1, z2_10_0);

	/* (2**21) - (2**1) */
	_ed25519_mul(t0, z2_20_0, z2_20_0);
	/* (2**22) - (2**2) */
	_ed25519_mul(t1, t0, t0);
	/* (2**40) - (2**20) */
	for (int32 i = 2; i < 20; i += 2) {
		_ed25519_mul(t0, t1, t1);
		_ed25519_mul(t1, t0, t0);
	}
	/* (2**40) - (2**0) */
	_ed25519_mul(t0,t1,z2_20_0);

	/* (2**41) - (2**1) */
	_ed25519_mul(t1, t0, t0);
	/* (2**42) - (2**2) */
	_ed25519_mul(t0, t1, t1);
	/* (2**50) - (2**10) */
	for (int32 i = 2; i < 10; i += 2) {
		_ed25519_mul(t1, t0, t0);
		_ed25519_mul(t0, t1, t1);
	}
	/* (2**50) - (2**0) */
	_ed25519_mul(z2_50_0, t0, z2_10_0);

	/* (2**51) - (2**1) */
	_ed25519_mul(t0, z2_50_0, z2_50_0);
	/* (2**52) - (2**2) */
	_ed25519_mul(t1, t0, t0);
	/* (2**100) - (2**50) */
	for (int32 i = 2; i < 50; i += 2) {
		_ed25519_mul(t0, t1, t1);
		_ed25519_mul(t1, t0, t0);
	}
	/* (2**100) - (2**0) */
	_ed25519_mul(z2_100_0, t1, z2_50_0);

	/* (2**101) - (2**1) */
	_ed25519_mul(t1, z2_100_0, z2_100_0);
	/* (2**102) - (2**2) */
	_ed25519_mul(t0, t1, t1);
	/* (2**200) - (2**100) */
	for (int32 i = 2; i < 100; i += 2) {
		_ed25519_mul(t1, t0, t0);
		_ed25519_mul(t0, t1, t1);
	}
	/* (2**200) - (2**0) */
	_ed25519_mul(t1, t0, z2_100_0);

	/* (2**201) - (2**1) */
	_ed25519_mul(t0, t1, t1);
	/* (2**202) - (2**2) */
	_ed25519_mul(t1, t0, t0);
	/* (2**250) - (2**50) */
	for (int32 i = 2; i < 50; i += 2) {
		_ed25519_mul(t0, t1, t1);
		_ed25519_mul(t1, t0, t0);
	}
	/* (2**250) - (2**0) */
	_ed25519_mul(t0, t1, z2_50_0);

	/* (2**251) - (2**1) */
	_ed25519_mul(t1, t0, t0);
	/* (2**252) - (2**2) */
	_ed25519_mul(t0, t1, t1);
	/* (2**253) - (2**3) */
	_ed25519_mul(t1, t0, t0);
	/* (2**254) - (2**4) */
	_ed25519_mul(t0, t1, t1);
	/* (2**255) - (2**5) */
	_ed25519_mul(t1, t0, t0);
	/* (2**255) - 21 */
	_ed25519_mul(r, t1, z11);
} /* end */

/* @func: _ed25519_mod (static) - curve25519 modular reduction (norm)
* @param1: uint32 [8] # number
* @return: void
*/
static void _ed25519_mod(uint32 r[8]) {
	const uint32 *p = ED25519_FAST_P;
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
	_ed25519_swap(r, rr, r[7] >> 31);
} /* end */

/* @func: _ed25519_pow (static) - fast exponentiation (modular reduction)
* @param1: uint32 [8]       # result
* @param2: const uint32 [8] # number
* @param3: const uint32 [8] # exponential
* @return: void
*/
static void _ed25519_pow(uint32 r[8],
		const uint32 a[8], const uint32 b[8]) {
	const uint32 *p = ED25519_FAST_ONE;
	uint32 x[8], t[8], e[8];
	for (int32 i = 0; i < 8; i++) {
		x[i] = a[i];
		e[i] = b[i];
		r[i] = p[i];
	}

	for (int32 i = 0; i < 256; i++) {
		_ed25519_mul(t, r, x);
		_ed25519_swap(r, t, e[i / 32] >> (i % 32));
		_ed25519_mul(x, x, x);
	}
} /* end */

/* @func: _ed25519_iszero (static) - check if a number is zero
* @param1: const uint32 [8] # number
* @return: uint32           # 0: non-zero, 1: is zero
*/
static uint32 _ed25519_iszero(const uint32 a[8]) {
	uint32 r = 0;
	for (int32 i = 0; i < (8 * 2); i++)
		r |= ((uint16 *)a)[i];

	/*
	* 0x0000ffff - 1 == 0x0000fffe
	*   0x0000fffe >> 31 == 0x00000000
	* 0x00000000 - 1 == 0xffffffff
	*   0xffffffff >> 31 == 0x00000001
	*/
	return ((r - 1) >> 31) & 1;
} /* end */

/* @func: ed25519_fast_point_add - edwards curve point addition
* @param1: const struct ed25519_point * # addend curve point
* @param2: const struct ed25519_point * # addend curve point
* @param3: struct ed25519_point *       # result curve point
* @return: void
*/
void FSYMBOL(ed25519_fast_point_add)(const struct ed25519_point *xyz1,
		const struct ed25519_point *xyz2, struct ed25519_point *xyz3) {
	uint32 a[8], b[8], a1[8], b1[8], c1[8], d1[8],
		e1[8], f1[8], g1[8], h1[8];
	/*
	* a1 = (((y1 - x1) % p) * ((y2 - x2) % p)) % p
	* b1 = ((y1 + x1) * (y2 + x2)) % p
	* c1 = (t1 * t2) % p
	* c1 = ((c1 * d) % p) << 1
	* d1 = ((z1 * z2) % p) << 1
	* e1 = (b1 - a1) % p
	* f1 = (d1 - c1) % p
	* g1 = (d1 + c1) % p
	* h1 = (b1 + a1) % p
	* x3 = (e1 * f1) % p
	* y3 = (g1 * h1) % p
	* t3 = (e1 * h1) % p
	* z3 = (f1 * g1) % p
	*/

	/* b1 = ((y1 - x1) * (y2 - x2)) */
	_ed25519_sub(a, xyz1->y, xyz1->x);
	_ed25519_sub(b, xyz2->y, xyz2->x);
	_ed25519_mul(a1, a, b);

	/* b1 = ((y1 + x1) * (y2 + x2)) */
	_ed25519_add(a, xyz1->y, xyz1->x);
	_ed25519_add(b, xyz2->y, xyz2->x);
	_ed25519_mul(b1, a, b);

	/* c1 = ((t1 * t2) * d) * 2 */
	_ed25519_mul(c1, xyz1->t, xyz2->t);
	_ed25519_mul(c1, c1, ED25519_FAST_D);
	_ed25519_add(c1, c1, c1);

	/* d1 = (z1 * z2) * 2 */
	_ed25519_mul(d1, xyz1->z, xyz2->z);
	_ed25519_add(d1, d1, d1);

	/* e1 = b1 - a1 */
	_ed25519_sub(e1, b1, a1);
	/* f1 = d1 - c1 */
	_ed25519_sub(f1, d1, c1);
	/* g1 = d1 + c1 */
	_ed25519_add(g1, d1, c1);
	/* h1 = b1 + a1 */
	_ed25519_add(h1, b1, a1);

	/* x3 = e1 * f1 */
	_ed25519_mul(xyz3->x, e1, f1);
	/* y3 = g1 * h1 */
	_ed25519_mul(xyz3->y, g1, h1);
	/* t3 = e1 * h1 */
	_ed25519_mul(xyz3->t, e1, h1);
	/* z3 = f1 * g1 */
	_ed25519_mul(xyz3->z, f1, g1);
} /* end */

/* @func: ed25519_fast_point_double - edwards curve point doubling
* @param1: const struct ed25519_point * # addend curve point
* @param2: struct ed25519_point *       # result curve point
* @return: void
*/
void FSYMBOL(ed25519_fast_point_double)(const struct ed25519_point *xyz1,
		struct ed25519_point *xyz3) {
	uint32 a1[8], b1[8], c1[8], h1[8], e1[8], g1[8], f1[8];
	/*
	* a1 = (x1 ** 2) % p
	* b1 = (y1 ** 2) % p
	* c1 = ((z1 ** 2) % p) << 1
	* h1 = (a1 + b1) % p
	* e1 = ((x1 + y1) ** 2) % p
	* e1 = (h1 - e1) % p
	* g1 = (a1 - b1) % p
	* f1 = (c1 + g1) % p
	* x3 = (e1 * f1) % p
	* y3 = (g1 * h1) % p
	* t3 = (e1 * h1) % p
	* z3 = (f1 * g1) % p
	*/

	/* a1 = x1 ** 2 */
	_ed25519_mul(a1, xyz1->x, xyz1->x);
	/* b1 = y1 ** 2 */
	_ed25519_mul(b1, xyz1->y, xyz1->y);

	/* c1 = (z1 ** 2) * 2 */
	_ed25519_mul(c1, xyz1->z, xyz1->z);
	_ed25519_add(c1, c1, c1);

	/* h1 = a1 + b1 */
	_ed25519_add(h1, a1, b1);

	/* e1 = h1 - ((x1 + y1) ** 2) */
	_ed25519_add(e1, xyz1->x, xyz1->y);
	_ed25519_mul(e1, e1, e1);
	_ed25519_sub(e1, h1, e1);

	/* g1 = a1 - b1 */
	_ed25519_sub(g1, a1, b1);

	/* f1 = c1 + g1 */
	_ed25519_add(f1, c1, g1);

	/* x3 = e1 * f1 */
	_ed25519_mul(xyz3->x, e1, f1);
	/* y3 = g1 * h1 */
	_ed25519_mul(xyz3->y, g1, h1);
	/* t3 = e1 * h1 */
	_ed25519_mul(xyz3->t, e1, h1);
	/* z3 = f1 * g1 */
	_ed25519_mul(xyz3->z, f1, g1);
} /* end */

/* @func: ed25519_fast_scalar_mul - montgomery ladder scalar multiplication
* @param3: const uint32 [8]             # private key
* @param4: const struct ed25519_point * # base curve point
* @param5: struct ed25519_point *       # scalar curve point
* @return: void
*/
void FSYMBOL(ed25519_fast_scalar_mul)(const uint32 k[8],
		const struct ed25519_point *xyz1, struct ed25519_point *xyz2) {
	struct ed25519_point _xyz1, _xyz2, _xyz3;
	for (int32 i = 0; i < 8; i++) {
		_xyz1.x[i] = 0;
		_xyz1.y[i] = 0;
		_xyz1.z[i] = 0;
		_xyz1.t[i] = 0;
		_xyz2.x[i] = xyz1->x[i];
		_xyz2.y[i] = xyz1->y[i];
		_xyz2.z[i] = xyz1->z[i];
		_xyz2.t[i] = xyz1->t[i];
	}
	_xyz1.y[0] = 1;
	_xyz1.z[0] = 1;
	/*
	* _xyz1 = (0, 1, 1, 0)
	* _xyz2 = xyz1
	*/

	for (int32 i = 0; i < 256; i++) {
		FSYMBOL(ed25519_fast_point_add)(&_xyz1, &_xyz2, &_xyz3);

		uint32 k_t = k[i / 32] >> (i % 32);
		_ed25519_swap(_xyz1.x, _xyz3.x, k_t);
		_ed25519_swap(_xyz1.y, _xyz3.y, k_t);
		_ed25519_swap(_xyz1.z, _xyz3.z, k_t);
		_ed25519_swap(_xyz1.t, _xyz3.t, k_t);

		/* FSYMBOL(ed25519_fast_point_add)(&_xyz2, &_xyz2, &_xyz2); */
		FSYMBOL(ed25519_fast_point_double)(&_xyz2, &_xyz2);
	}

	for (int32 i = 0; i < 8; i++) {
		xyz2->x[i] = _xyz1.x[i];
		xyz2->y[i] = _xyz1.y[i];
		xyz2->z[i] = _xyz1.z[i];
		xyz2->t[i] = _xyz1.t[i];
	}
} /* end */

/* @func: ed25519_fast_point_equal - edwards curve point comparison is equal
* @param2: const struct ed25519_point * # curve point
* @param3: const struct ed25519_point * # curve point
* @return: int32                        # 0: p1 == p2, 1: p1 != p2
*/
int32 FSYMBOL(ed25519_fast_point_equal)(const struct ed25519_point *xyz1,
		const struct ed25519_point *xyz2) {
	uint32 a[8], b[8], r = 0;
	/*
	* if ((x1 * z2) - (x2 * z1))
	*   return 1;
	* if ((y1 * z2) - (y2 * z1))
	*   return 1;
	*/

	_ed25519_mul(a, xyz1->x, xyz2->z);
	_ed25519_mul(b, xyz2->x, xyz1->z);
	_ed25519_sub(a, a, b);
	r = _ed25519_iszero(a);

	_ed25519_mul(a, xyz1->y, xyz2->z);
	_ed25519_mul(b, xyz2->y, xyz1->z);
	_ed25519_sub(a, a, b);
	r &= _ed25519_iszero(a);

	return ~r & 1;
} /* end */

/* @func: ed25519_fast_point_recover_x - calculate the corresponding curve point x
* @param1: const uint32 [8] # curve point y
* @param2: uint32           # sign (compressd x high-bit: k[7] >> 31)
* @param3: uint32 [8]       # curve point x
* @return: void
*/
void FSYMBOL(ed25519_fast_point_recover_x)(const uint32 y[8], uint32 sign,
		uint32 r[8]) {
	uint32 x1[8], x2[8], y2[8], x[8], t1[8], iszero = 0;
	/*
	* y2 = (y ** 2) % p
	* x1 = y2 - 1
	* x2 = ((d * y2) % p) + 1
	* x2 = (x1 * inv(x2, p)) % p
	* x = modpow(x2, (p + 3) / 8), p)
	* if (((x ** 2) % p) - x2)
	*   x = (x * modpow(2, (p - 1) / 4, p)) % p
	* if ((x & 1) != sign)
	*   x = p - x
	*/

	/* y2 = y ** 2 */
	_ed25519_mul(y2, y, y);
	/* x1 = y2 - 1 */
	_ed25519_sub(x1, y2, ED25519_FAST_ONE);

	/* x2 = (d * y2) + 1 */
	_ed25519_mul(x2, y2, ED25519_FAST_D);
	_ed25519_add(x2, x2, ED25519_FAST_ONE);

	/* x2 = (x1 * inv(x2)) % p */
	_ed25519_inv(x2, x2);
	_ed25519_mul(x2, x2, x1);
	_ed25519_mod(x2);

	/* x = modpow(x2, (p + 3) / 8)) % p */
	_ed25519_pow(x, x2, ED25519_FAST_P38);
	_ed25519_mod(x);

	/* t1 = ((x ** 2) % p) - x2 */
	_ed25519_mul(t1, x, x);
	_ed25519_mod(t1);
	_ed25519_sub(t1, t1, x2);
	iszero = _ed25519_iszero(t1);

	/* x2 = (x * modpow(2, (p - 1) / 4, p)) % p */
	_ed25519_mul(x2, x, ED25519_FAST_P14);
	_ed25519_mod(x2);
	/* x = if t1 != 0 : swap x2 */
	_ed25519_swap(x, x2, ~iszero);

	/* x1 = p - x */
	_ed25519_sub(x1, ED25519_FAST_P, x);
	/* x = if x & 1 != sign : swap x1 */
	_ed25519_swap(x, x1, (x[0] & 1) ^ (sign & 1));

	/* r = x */
	for (int32 i = 0; i < 8; i++)
		r[i] = x[i];
} /* end */

/* @func: ed25519_fast_point_compress - curve point compress
* @param1: const struct ed25519_point * # curve point
* @param2: uint32 [8]                   # public key
* @return: void
*/
void FSYMBOL(ed25519_fast_point_compress)(const struct ed25519_point *xyz1,
		uint32 r[8]) {
	uint32 x[8], y[8], z[8];
	/*
	* x = (x1 * inv(z, p)) % p
	* y = (y1 * inv(z, p)) % p
	* r = y | ((x & 1) << 255)
	*/

	/* z = inv(z, p) */
	_ed25519_inv(z, xyz1->z);

	/* x = (xyz1->x * z) % p */
	_ed25519_mul(x, xyz1->x, z);
	_ed25519_mod(x);

	/* y = (xyz1->y * z) % p */
	_ed25519_mul(y, xyz1->y, z);
	_ed25519_mod(y);

	/* r = y | ((x & 1) << 255) */
	y[7] |= (x[0] & 1) << 31;

	for (int32 i = 0; i < 8; i++)
		r[i] = y[i];
} /* end */

/* @func: ed25519_fast_point_decompress - curve point decompress
* @param3: const uint32 [8]       # public key
* @param4: struct ed25519_point * # curve point
* @return: void
*/
void FSYMBOL(ed25519_fast_point_decompress)(const uint32 k[8],
		struct ed25519_point *xyz1) {
	uint32 y[8], x[8], t[8];
	/*
	* y = k & ((1 << 255) - 1)
	* x1 = rec_x(p, d, y, k >> 255)
	* y1 = y
	* z1 = 1
	* t1 = (x1 * y) % p
	*/

	/* y = k & ((1 << 255) - 1) */
	for (int32 i = 0; i < 8; i++)
		y[i] = k[i];
	y[7] &= (1U << 31) - 1;

	/* x = rec_x(y, k >> 255) */
	FSYMBOL(ed25519_fast_point_recover_x)(y, k[7] >> 31, x);

	/* t = x * y */
	_ed25519_mul(t, x, y);

	/* r = (x, y, 1, t) */
	for (int32 i = 0; i < 8; i++) {
		xyz1->x[i] = x[i];
		xyz1->y[i] = y[i];
		xyz1->z[i] = 0;
		xyz1->t[i] = t[i];
	}
	xyz1->z[0] = 1;
} /* end */

/* @func: ed25519_fast_clamp_key - private key clamping
* @param1: uint32 [8] # private key
* @return: void
*/
void FSYMBOL(ed25519_fast_clamp_key)(uint32 k[8]) {
	k[0] &= ~0x07;
	k[7] |= 0x40000000;
	k[7] &= 0x7fffffff;
} /* end */
