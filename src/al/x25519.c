/* x25519.c - curve25519 elliptic-curve crypto implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_512.h>
#include <libf/al/x25519.h>


/* @func: _x25519_extended_gcd (static) - extended euclidean algorithm
* @param1: const bn_int1024_t # input number
* @param2: const bn_int1024_t # input number
* @param3: bn_int1024_t       # bezout coefficient
* @param4: bn_int1024_t       # bezout coefficient
* @param5: bn_int1024_t       # return greatest common divisor
* @return: void
*/
void _x25519_extended_gcd(const bn_int1024_t a, const bn_int1024_t b,
		bn_int1024_t x, bn_int1024_t y, bn_int1024_t r) {
	bn_int1024_t _b, _x, _y, _quo, _rem;

	if (!FSYMBOL(bn_int1024_cmp_1)(b, 0)) {
		FSYMBOL(bn_int1024_zero)(x);
		FSYMBOL(bn_int1024_zero)(y);
		x[0] = 1;
		y[0] = 0;
		FSYMBOL(bn_int1024_move)(r, a);
		return;
	}

	FSYMBOL(bn_int1024_move)(_b, b);
	FSYMBOL(bn_int1024_divmod)(_quo, _rem, a, b);
	_x25519_extended_gcd(_b, _rem, _x, _y, r);

	FSYMBOL(bn_int1024_move)(x, _y);

	FSYMBOL(bn_int1024_divmod)(_quo, _rem, a, b);
	FSYMBOL(bn_int1024_mul)(_quo, _quo, _y);
	FSYMBOL(bn_int1024_sub)(y, _x, _quo);
} /* end */

/* @func: x25519_mod_inverse - modular inverse based on extended euclidean
* @param1: const bn_int1024_t # number
* @param2: const bn_int1024_t # modulus
* @param3: bn_int1024_t       # inverse modulus
* @return: int32              # -1: fail, 0: success
*/
int32 FSYMBOL(x25519_mod_inverse)(const bn_int1024_t a, const bn_int1024_t m,
		bn_int1024_t r) {
	bn_int1024_t x, y, quo, rem, rr;
	_x25519_extended_gcd(a, m, x, y, rr);

	if (FSYMBOL(bn_int1024_cmp_1)(rr, 1)) {
		FSYMBOL(bn_int1024_zero)(r);
		return -1;
	} else {
		FSYMBOL(bn_int1024_divmod)(quo, rem, x, m);
		FSYMBOL(bn_int1024_add)(rem, rem, m);
		FSYMBOL(bn_int1024_divmod)(quo, rr, rem, m);
		FSYMBOL(bn_int1024_move)(r, rr);
	}

	return 0;
} /* end */

/* @func: x25519_scalar_mul - montgomery ladder scalar multiplication
* @param1: const bn_int1024_t # private key
* @param2: const bn_int1024_t # base point
* @param3: const bn_int1024_t # prime modulus
* @param4: const bn_int1024_t # curve parameters
* @param5: bn_int1024_t       # scalar
* @return: void
*/
void FSYMBOL(x25519_scalar_mul)(const bn_int1024_t k, const bn_int1024_t b,
		const bn_int1024_t p, const bn_int1024_t a, bn_int1024_t r) {
	bn_int1024_t x1, x2, z2, x3, z3;
	FSYMBOL(bn_int1024_move)(x1, b);
	FSYMBOL(bn_int1024_zero)(x2);
	FSYMBOL(bn_int1024_zero)(z2);
	FSYMBOL(bn_int1024_move)(x3, b);
	FSYMBOL(bn_int1024_zero)(z3);
	x2[0] = 1;
	z3[0] = 1;

	bn_int1024_t t, A, AA, B, BB, E, C, D, DA, CB;
	for (int32 i = 254; i >= 0; i--) {
		int32 k_t = (k[i / 32] >> (i % 32)) & 1;
		if (k_t) {
			FSYMBOL(bn_int1024_move)(t, x2);	
			FSYMBOL(bn_int1024_move)(x2, x3);
			FSYMBOL(bn_int1024_move)(x3, t);

			FSYMBOL(bn_int1024_move)(t, z2);	
			FSYMBOL(bn_int1024_move)(z2, z3);
			FSYMBOL(bn_int1024_move)(z3, t);
		}

		/* A = (x2 + z2) % p */
		FSYMBOL(bn_int1024_add)(A, x2, z2);
		FSYMBOL(bn_int1024_divmod)(t, A, A, p);

		/* AA = (A * A) % p */
		FSYMBOL(bn_int1024_mul)(AA, A, A);
		FSYMBOL(bn_int1024_divmod)(t, AA, AA, p);

		/* B = (x2 - z2) % p */
		FSYMBOL(bn_int1024_sub)(B, x2, z2);
		FSYMBOL(bn_int1024_divmod)(t, B, B, p);

		/* BB = (B * B) % p */
		FSYMBOL(bn_int1024_mul)(BB, B, B);
		FSYMBOL(bn_int1024_divmod)(t, BB, BB, p);

		/* E = (AA - BB) % p */
		FSYMBOL(bn_int1024_sub)(E, AA, BB);
		FSYMBOL(bn_int1024_divmod)(t, E, E, p);

		/* C = (x3 + z3) % p */
		FSYMBOL(bn_int1024_add)(C, x3, z3);
		FSYMBOL(bn_int1024_divmod)(t, C, C, p);

		/* D = (x3 - z3) % p */
		FSYMBOL(bn_int1024_sub)(D, x3, z3);
		FSYMBOL(bn_int1024_divmod)(t, D, D, p);

		/* DA = (D * A) % p */
		FSYMBOL(bn_int1024_mul)(DA, D, A);
		FSYMBOL(bn_int1024_divmod)(t, DA, DA, p);

		/* CB = (C * B) % p */
		FSYMBOL(bn_int1024_mul)(CB, C, B);
		FSYMBOL(bn_int1024_divmod)(t, CB, CB, p);

		/* x3 = ((DA + CA) ** 2) % p */
		FSYMBOL(bn_int1024_add)(x3, DA, CB);
		FSYMBOL(bn_int1024_mul)(x3, x3, x3);
		FSYMBOL(bn_int1024_divmod)(t, x3, x3, p);

		/* x3 = (x1 * ((DA - CB) ** 2)) % p */
		FSYMBOL(bn_int1024_sub)(z3, DA, CB);
		FSYMBOL(bn_int1024_mul)(z3, z3, z3);
		FSYMBOL(bn_int1024_mul)(z3, x1, z3);
		FSYMBOL(bn_int1024_divmod)(t, z3, z3, p);

		/* x2 = (AA * BB) % p */
		FSYMBOL(bn_int1024_mul)(x2, AA, BB);
		FSYMBOL(bn_int1024_divmod)(t, x2, x2, p);

		/* x2 = (E * (AA + ((a * E) % p))) % p */
		FSYMBOL(bn_int1024_mul)(z2, a, E);
		FSYMBOL(bn_int1024_divmod)(t, z2, z2, p);
		FSYMBOL(bn_int1024_add)(z2, z2, AA);
		FSYMBOL(bn_int1024_mul)(z2, z2, E);
		FSYMBOL(bn_int1024_divmod)(t, z2, z2, p);

		if (k_t) {
			FSYMBOL(bn_int1024_move)(t, x2);	
			FSYMBOL(bn_int1024_move)(x2, x3);
			FSYMBOL(bn_int1024_move)(x3, t);

			FSYMBOL(bn_int1024_move)(t, z2);	
			FSYMBOL(bn_int1024_move)(z2, z3);
			FSYMBOL(bn_int1024_move)(z3, t);
		}
	}

	FSYMBOL(x25519_mod_inverse)(z2, p, z2);
	FSYMBOL(bn_int1024_mul)(x2, x2, z2);
	FSYMBOL(bn_int1024_divmod)(t, r, x2, p);
} /* end */

/* @func: x25519_shared_key - create a shared secret
* @param1: const bn_int1024_t # private key
* @param2: const bn_int1024_t # public key (other side)
* @param3: const bn_int1024_t # prime modulus
* @param4: const bn_int1024_t # curve parameters
* @param5: bn_int1024_t       # shared secret
* @return: void
*/
void FSYMBOL(x25519_shared_key)(const bn_int1024_t pri, const bn_int1024_t pub,
		const bn_int1024_t p, const bn_int1024_t a, bn_int1024_t key) {
	FSYMBOL(x25519_scalar_mul)(pri, pub, p, a, key);
} /* end */

/* @func: x25519_clamp_key - private key clamping
* @param1: bn_int1024_t # private key
*/
void FSYMBOL(x25519_clamp_key)(bn_int1024_t k) {
	k[0] &= ~0x07;
	k[7] |= 0x40000000;
	k[7] &= 0x7fffffff;
} /* end */
