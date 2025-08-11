/* x448_fast.c - curve448 elliptic-curve crypto (ecc) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/x448_fast.h>


/* @func: _x448_swap (static) - numerical swap
* @param1: uint32 [14] # number a
* @param2: uint32 [14] # number b
* @param3: uint32      # low bit (0: keep, 1: swap)
* @return: void
*/
static void _x448_swap(uint32 a[14], uint32 b[14], uint32 bit) {
	bit = (~bit & 1) - 1;
	for (int32 i = 0; i < 14; i++) {
		uint32 t = bit & (a[i] ^ b[i]);
		a[i] ^= t;
		b[i] ^= t;
	}
} /* end */

/* @func: _x448_add (static) - curve448 addition (modular reduction)
* @param1: uint32 [14]       # sum
* @param2: const uint32 [14] # addend
* @param3: const uint32 [14] # addend
* @return: void
*/
static void _x448_add(uint32 r[14],
		const uint32 a[14], const uint32 b[14]) {
	uint32 carry = 0, carry2 = 0;
	uint64L tmp = 0;

	/* r = a + b */
	for (int32 i = 0; i < 14; i++) {
		tmp = (uint64L)a[i] + b[i] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* NOTE: (2**448) % (2**448-2**224-1) = 2**224+1 */

 	/* r = r % p modular reduction */
	carry += carry2;
	for (int32 i = 0; i < 7; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry += carry2;
	for (int32 i = 7; i < 14; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: _x448_sub (static) - curve448 subtraction (modular reduction)
* @param1: uint32 [14]       # difference
* @param2: const uint32 [14] # minuend
* @param3: const uint32 [14] # subtract
* @return: void
*/
static void _x448_sub(uint32 r[14],
		const uint32 a[14], const uint32 b[14]) {
	uint32 carry = 0, carry2 = 0;
	uint64L tmp = 0;

	/* r = a - b */
	for (int32 i = 0; i < 14; i++) {	
		tmp = (int64L)a[i] - b[i] + (int32)carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* NOTE: (2**448) % (2**448-2**224-1) = 2**224+1 */

 	/* r = r % p modular reduction */
	tmp = (uint64L)r[0] - (carry2 & 1);
	r[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32 i = 1; i < 7; i++) {
		tmp = (uint64L)r[i] + (int32)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64L)r[7] - (carry2 & 1);
	r[7] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32 i = 8; i < 14; i++) {
		tmp = (uint64L)r[i] + (int32)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: _x448_mul (static) - curve448 multiplication (modular reduction)
* @param1: uint32 [14]       # product
* @param2: const uint32 [14] # multiplicand
* @param3: const uint32 [14] # multiplier
* @return: void
*/
static void _x448_mul(uint32 r[14],
		const uint32 a[14], const uint32 b[14]) {
	uint32 rr[28];
	uint32 carry = 0, carry2 = 0;
	uint64L tmp = 0;

	/* rr = a * b */
	for (int32 i = 0; i < 28; i++)
		rr[i] = 0;

	for (int32 i = 0; i < 14; i++) {
		tmp = (uint64L)a[0] * b[i] + carry2;
		rr[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}
	rr[14] = carry2;

	for (int32 i = 1; i < 14; i++) {
		carry2 = 0;
		for (int32 j = 0; j < 14; j++) {
			tmp = (uint64L)a[i] * b[j] + rr[i + j] + carry2;
			rr[i + j] = tmp & 0xffffffff;
			carry2 = tmp >> 32;
		}
		rr[i + 14] = carry2;
	}

	/* NOTE: (2**448) % (2**448-2**224-1) = 2**224+1 */

 	/* r = rr % p modular reduction */
	carry2 = 0;
	for (int32 i = 0; i < 7; i++) {
		tmp = (uint64L)rr[i] + rr[i + 14] + rr[i + 21] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}
	for (int32 i = 7; i < 14; i++) {
		tmp = (uint64L)rr[i] + rr[i + 14] + rr[i + 7]
			+ rr[i + 14] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	carry += carry2;
	for (int32 i = 0; i < 7; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry += carry2;
	for (int32 i = 7; i < 14; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: _x448_mul39081 (static) - curve448 '(156326 - 2) / 4 = 39081' \
*                                   multiplication (modular reduction)
* @param1: uint32 [14]       # product
* @param2: const uint32 [14] # multiplicand
* @param2: const uint32 [14] # addend
* @return: void
*/
static void _x448_mul39081(uint32 r[14],
		const uint32 a[14], const uint32 b[14]) {
	uint32 carry = 0, carry2 = 0;
	uint64L tmp = 0;

	/* r = a * 39081 + b, (156326 - 2) / 4 = 39081 */
	for (int32 i = 0; i < 14; i++) {
		tmp = (uint64L)a[i] * 39081 + b[i] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* NOTE: (2**448) % (2**448-2**224-1) = 2**224+1 */

 	/* r = rr % p modular reduction */
	carry += carry2;
	for (int32 i = 0; i < 7; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry += carry2;
	for (int32 i = 7; i < 14; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: _x448_inv (static) - modular inversion based on fermat's \
*                              little theorem
* @param1: uint32 [14]       # inverse modulus
* @param2: const uint32 [14] # number
* @return: void
*/
static void _x448_inv(uint32 r[14], const uint32 z[14]) {
	uint32 t[14];

	_x448_mul(t, z, z);
	_x448_mul(t, t, z);
	for (int32 i = 0; i < 221; i++) {
		_x448_mul(t, t, t);
		_x448_mul(t, t, z);
	}

	_x448_mul(t, t, t);
	for (int32 i = 0; i < 222; i++) {
		_x448_mul(t, t, t);
		_x448_mul(t, t, z);
	}

	_x448_mul(t, t, t);
	_x448_mul(t, t, t);
	_x448_mul(r, t, z);
} /* end */

/* @func: _x448_mod (static) - curve448 modular reduction (norm)
* @param1: uint32 [14] # number
* @return: void
*/
static void _x448_mod(uint32 r[14]) {
	uint32 carry = 0, carry2 = 0;
	uint64L tmp = 0;

	/* NOTE: (2**448) % (2**448-2**224-1) = 2**224+1 */

	/* if r < p : adjust carry2 */
	tmp = (uint64L)r[0] + 1;
	carry2 = tmp >> 32;
	for (int32 i = 1; i < 7; i++) {
		tmp = (uint64L)r[i] + carry2;
		carry2 = tmp >> 32;
	}

	tmp = (uint64L)r[7] + 1;
	carry2 = tmp >> 32;
	for (int32 i = 8; i < 14; i++) {
		tmp = (uint64L)r[i] + carry2;
		carry2 = tmp >> 32;
	}

 	/* r = r % p modular reduction */
	carry += carry2;
	for (int32 i = 0; i < 7; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry += carry2;
	for (int32 i = 7; i < 14; i++) {
		tmp = (uint64L)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
} /* end */

/* @func: x448_fast_scalar_mul - montgomery ladder scalar multiplication
* @param1: const uint32 [14] # private key
* @param2: const uint32 [14] # base point
* @param3: uint32 [14]       # scalar
* @return: void
*/
void FSYMBOL(x448_fast_scalar_mul)(const uint32 k[14], const uint32 b[14],
		uint32 r[14]) {
	uint32 x1[14], z1[14], x2[14], z2[14];
	for (int32 i = 0; i < 14; i++) {
		x1[i] = 0;
		z1[i] = 0;
		x2[i] = b[i];
		z2[i] = 0;
	}
	x1[0] = 1;
	z2[0] = 1;
	/* x1, z1 = (1, 0) */
	/* x2, z2 = (b, 1) */

	uint32 A[14], AA[14], B[14], BB[14], E[14], C[14], D[14],
		DA[14], CB[14];
	for (int32 i = 447; i >= 0; i--) {
		uint32 k_t = k[i / 32] >> (i % 32);
		_x448_swap(x1, x2, k_t);
		_x448_swap(z1, z2, k_t);

		/* A = x1 + z1 */
		_x448_add(A, x1, z1);
		/* AA = A ** 2 */
		_x448_mul(AA, A, A);

		/* B = x1 - z1 */
		_x448_sub(B, x1, z1);
		/* BB = B ** 2 */
		_x448_mul(BB, B, B);

		/* E = AA - BB */
		_x448_sub(E, AA, BB);
		/* C = x2 + z2 */
		_x448_add(C, x2, z2);
		/* D = x2 - z2 */
		_x448_sub(D, x2, z2);

		/* DA = D * A */
		_x448_mul(DA, D, A);
		/* CB = C * B */
		_x448_mul(CB, C, B);

		/* x2 = (DA * CB) ** 2 */
		_x448_add(x2, DA, CB);
		_x448_mul(x2, x2, x2);

		/* z2 = b * ((DA - CB) ** 2) */
		_x448_sub(z2, DA, CB);
		_x448_mul(z2, z2, z2);
		_x448_mul(z2, z2, b);

		/* x1 = AA * BB */
		_x448_mul(x1, AA, BB);

		/* z1 = E * (AA + 39081 * E) */
		_x448_mul39081(z1, E, AA);
		/* _x448_add(z1, z1, AA); */
		_x448_mul(z1, z1, E);

		_x448_swap(x1, x2, k_t);
		_x448_swap(z1, z2, k_t);
	}

	/* r = (inv(z1) * x1) % p */
	_x448_inv(z1, z1);
	_x448_mul(r, z1, x1);
	_x448_mod(r);
} /* end */

/* @func: x448_fast_clamp_key - private key clamping
* @param1: uint32 [14] # private key
* @return: void
*/
void FSYMBOL(x448_fast_clamp_key)(uint32 k[14]) {
	k[0] &= ~0x03;
	k[13] |= 0x80000000;
} /* end */

/* @func: x448_fast_private_key - x448 private key operation function
* @param1: uint8 * # private key (length: X448_LEN)
* @return: void
*/
void FSYMBOL(x448_fast_private_key)(uint8 *pri) {
	uint32 _pri[14];
	XSYMBOL(memcpy)(_pri, pri, X448_LEN);

	FSYMBOL(x448_fast_clamp_key)(_pri);

	XSYMBOL(memcpy)(pri, _pri, X448_LEN);
} /* end */

/* @func: x448_fast_public_key - x448 public key create function
* @param1: const uint8 * # private key (length: X448_LEN)
* @param2: uint8 *       # public key (length: X448_LEN)
* @return: void
*/
void FSYMBOL(x448_fast_public_key)(const uint8 *pri, uint8 *pub) {
	uint32 _pri[14], _pub[14];
	XSYMBOL(memcpy)(_pri, pri, X448_LEN);
	XSYMBOL(memcpy)(_pub, pub, X448_LEN);

	FSYMBOL(x448_fast_scalar_mul)(_pri, X448_FAST_B, _pub);

	XSYMBOL(memcpy)(pub, _pub, X448_LEN);
} /* end */

/* @func: x448_fast_shared_key - x448 shared key create function
* @param1: const uint8 * # private key (length: X448_LEN)
* @param2: const uint8 * # public key (length: X448_LEN)
* @param3: uint8 *       # shared key (length: X448_LEN)
* @return: void
*/
void FSYMBOL(x448_fast_shared_key)(const uint8 *pri, const uint8 *pub,
		uint8 *key) {
	uint32 _pri[14], _pub[14], _key[14];
	XSYMBOL(memcpy)(_pri, pri, X448_LEN);
	XSYMBOL(memcpy)(_pub, pub, X448_LEN);
	XSYMBOL(memcpy)(_key, key, X448_LEN);

	FSYMBOL(x448_fast_scalar_mul)(_pri, _pub, _key);

	XSYMBOL(memcpy)(key, _key, X448_LEN);
} /* end */
