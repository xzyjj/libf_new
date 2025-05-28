/* x25519.c - curve25519 elliptic-curve crypto (ecc) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_512.h>
#include <libf/al/x25519.h>


/* @func: _x25519_extended_gcd (static) - extended euclidean algorithm
* @param1: const bn_int512_t # input number
* @param2: const bn_int512_t # input number
* @param3: bn_int512_t       # bezout coefficient
* @param4: bn_int512_t       # bezout coefficient
* @param5: bn_int512_t       # return greatest common divisor
* @return: void
*/
void _x25519_extended_gcd(const bn_int512_t a, const bn_int512_t b,
		bn_int512_t x, bn_int512_t y, bn_int512_t r) {
	bn_int512_t _b, _x, _y, _quo, _rem;

	if (!FSYMBOL(bn_int512_cmp_1)(b, 0)) {
		FSYMBOL(bn_int512_zero)(x);
		FSYMBOL(bn_int512_zero)(y);
		x[0] = 1;
		y[0] = 0;
		FSYMBOL(bn_int512_move)(r, a);
		return;
	}

	FSYMBOL(bn_int512_move)(_b, b);
	FSYMBOL(bn_int512_divmod)(_quo, _rem, a, b);
	_x25519_extended_gcd(_b, _rem, _x, _y, r);

	FSYMBOL(bn_int512_move)(x, _y);

	FSYMBOL(bn_int512_divmod)(_quo, _rem, a, b);
	FSYMBOL(bn_int512_mul)(_quo, _quo, _y);
	FSYMBOL(bn_int512_sub)(y, _x, _quo);
} /* end */

/* @func: x25519_mod_inverse - modular inverse based on extended euclidean
* @param1: const bn_int512_t # number
* @param2: const bn_int512_t # modulus
* @param3: bn_int512_t       # inverse modulus
* @return: int32              # -1: fail, 0: success
*/
int32 FSYMBOL(x25519_mod_inverse)(const bn_int512_t a, const bn_int512_t m,
		bn_int512_t r) {
	bn_int512_t x, y, quo, rem, rr;
	_x25519_extended_gcd(a, m, x, y, rr);

	if (FSYMBOL(bn_int512_cmp_1)(rr, 1)) {
		FSYMBOL(bn_int512_zero)(r);
		return -1;
	} else {
		FSYMBOL(bn_int512_divmod)(quo, rem, x, m);
		FSYMBOL(bn_int512_add)(rem, rem, m);
		FSYMBOL(bn_int512_divmod)(quo, rr, rem, m);
		FSYMBOL(bn_int512_move)(r, rr);
	}

	return 0;
} /* end */

/* @func: x25519_point_add - montgomery curve point addition
* @param1: const bn_int512_t # prime modulus
* @param2: const bn_int512_t # curve point x1
* @param3: const bn_int512_t # curve point z1
* @param4: const bn_int512_t # curve point x2
* @param5: const bn_int512_t # curve point z2
* @param6: const bn_int512_t # x difference
* @param7: const bn_int512_t # z difference
* @param8: bn_int512_t       # curve point x3
* @param9: bn_int512_t       # curve point z3
* @return: void
*/
void FSYMBOL(x25519_point_add)(const bn_int512_t p,
		const bn_int512_t x1, const bn_int512_t z1,
		const bn_int512_t x2, const bn_int512_t z2,
		const bn_int512_t x_d, const bn_int512_t z_d,
		bn_int512_t r_x, bn_int512_t r_z) {
	bn_int512_t _a, _b, _t, _x1, _z1, _x2, _z2;

	/* _x1 = z_d << 2 */
	FSYMBOL(bn_int512_move)(_x1, z_d);
	FSYMBOL(bn_int512_lsh)(_x1);
	FSYMBOL(bn_int512_lsh)(_x1);

	/* _z1 = x_d << 2 */
	FSYMBOL(bn_int512_move)(_z1, x_d);
	FSYMBOL(bn_int512_lsh)(_z1);
	FSYMBOL(bn_int512_lsh)(_z1);

	/* _a = (x2 * x1) % p */
	FSYMBOL(bn_int512_mul)(_a, x2, x1);
	FSYMBOL(bn_int512_divmod)(_t, _a, _a, p);
	/* _b = (z2 * z1) % p */
	FSYMBOL(bn_int512_mul)(_b, z2, z1);
	FSYMBOL(bn_int512_divmod)(_t, _b, _b, p);

	/* _x2 = (_a - _b) % p */
	FSYMBOL(bn_int512_sub)(_x2, _a, _b);
	FSYMBOL(bn_int512_divmod)(_t, _x2, _x2, p);
	/* _x2 = (_x2 * _x2) % p */
	FSYMBOL(bn_int512_mul)(_x2, _x2, _x2);
	FSYMBOL(bn_int512_divmod)(_t, _x2, _x2, p);

	/* _a = (x2 * z1) % p */
	FSYMBOL(bn_int512_mul)(_a, x2, z1);
	FSYMBOL(bn_int512_divmod)(_t, _a, _a, p);
	/* _b = (z2 * x1) % p */
	FSYMBOL(bn_int512_mul)(_b, z2, x1);
	FSYMBOL(bn_int512_divmod)(_t, _b, _b, p);

	/* _z2 = (_a - _b) % p */
	FSYMBOL(bn_int512_sub)(_z2, _a, _b);
	FSYMBOL(bn_int512_divmod)(_t, _z2, _z2, p);
	/* _z2 = (_z2 * _z2) % p */
	FSYMBOL(bn_int512_mul)(_z2, _z2, _z2);
	FSYMBOL(bn_int512_divmod)(_t, _z2, _z2, p);

	/* _x2 = _x2 * _x1 */
	FSYMBOL(bn_int512_mul)(_x2, _x2, _x1);
	/* _z2 = _z2 * _z1 */
	FSYMBOL(bn_int512_mul)(_z2, _z2, _z1);

	/* r_x = _x2 % p */
	FSYMBOL(bn_int512_divmod)(_t, r_x, _x2, p);
	/* r_z = _z2 % p */
	FSYMBOL(bn_int512_divmod)(_t, r_z, _z2, p);
} /* end */

/* @func: x25519_point_double - montgomery curve point doubling
* @param1: const bn_int512_t # prime modulus
* @param2: const bn_int512_t # curve parameter
* @param3: const bn_int512_t # curve point x1
* @param4: const bn_int512_t # curve point z1
* @param5: bn_int512_t       # curve point x3
* @param6: bn_int512_t       # curve point z3 
* @return: void
*/
void FSYMBOL(x25519_point_double)(const bn_int512_t p, const bn_int512_t a,
		const bn_int512_t x1, const bn_int512_t z1,
		bn_int512_t r_x, bn_int512_t r_z) {
	bn_int512_t _x1, _z1, _x2, _z2, _xz, _t;

	/* _x2 = (x1 * x1) % p */
	FSYMBOL(bn_int512_mul)(_x2, x1, x1);
	FSYMBOL(bn_int512_divmod)(_t, _x2, _x2, p);
	/* _z2 = (z1 * z1) % p */
	FSYMBOL(bn_int512_mul)(_z2, z1, z1);
	FSYMBOL(bn_int512_divmod)(_t, _z2, _z2, p);
	/* _xz = (x1 * z1) % p */
	FSYMBOL(bn_int512_mul)(_xz, x1, z1);
	FSYMBOL(bn_int512_divmod)(_t, _xz, _xz, p);

	/* _x1 = (_x2 - _z2) % p */
	FSYMBOL(bn_int512_sub)(_x1, _x2, _z2);
	FSYMBOL(bn_int512_divmod)(_t, _x1, _x1, p);
	/* _x1 = x1 * x1 */
	FSYMBOL(bn_int512_mul)(_x1, _x1, _x1);

	/* _z1 = (a * _xz) % p */
	FSYMBOL(bn_int512_mul)(_z1, a, _xz);
	FSYMBOL(bn_int512_divmod)(_t, _z1, _z1, p);
	/* _z1 = _z1 + _x2 */
	FSYMBOL(bn_int512_add)(_z1, _z1, _x2);
	/* _z1 = _z1 + _z2 */
	FSYMBOL(bn_int512_add)(_z1, _z1, _z2);
	/* _z1 = (_z1 * _xz) % p */
	FSYMBOL(bn_int512_mul)(_z1, _z1, _xz);
	FSYMBOL(bn_int512_divmod)(_t, _z1, _z1, p);

	/* _z1 = _z1 << 2 */
	FSYMBOL(bn_int512_lsh)(_z1);
	FSYMBOL(bn_int512_lsh)(_z1);

	/* r_x = _x1 % p */
	FSYMBOL(bn_int512_divmod)(_t, r_x, _x1, p);
	/* r_z = _z1 % p */
	FSYMBOL(bn_int512_divmod)(_t, r_z, _z1, p);
} /* end */

/* @func: x25519_scalar_mul - montgomery ladder scalar multiplication
* @param1: const bn_int512_t # private key
* @param2: const bn_int512_t # base point
* @param3: const bn_int512_t # prime modulus
* @param4: const bn_int512_t # curve parameter
* @param5: bn_int512_t       # scalar
* @return: void
*/
void FSYMBOL(x25519_scalar_mul)(const bn_int512_t k, const bn_int512_t b,
		const bn_int512_t p, const bn_int512_t a, bn_int512_t r) {
	bn_int512_t _x1, _z1, _x2, _z2, _z3, _t;
	FSYMBOL(bn_int512_zero)(_x1);
	FSYMBOL(bn_int512_zero)(_z1);
	FSYMBOL(bn_int512_zero)(_z2);
	FSYMBOL(bn_int512_zero)(_z3);
	FSYMBOL(bn_int512_move)(_x2, b);
	_x1[0] = 1;
	_z2[0] = 1;
	_z3[0] = 1;

	for (int32 i = 255; i >= 0; i--) {
		int32 k_t = (k[i / 32] >> (i % 32)) & 1;
		if (k_t) {
			FSYMBOL(bn_int512_move)(_t, _x1);
			FSYMBOL(bn_int512_move)(_x1, _x2);
			FSYMBOL(bn_int512_move)(_x2, _t);

			FSYMBOL(bn_int512_move)(_t, _z1);
			FSYMBOL(bn_int512_move)(_z1, _z2);
			FSYMBOL(bn_int512_move)(_z2, _t);
		}

		FSYMBOL(x25519_point_add)(p, _x1, _z1, _x2, _z2, b, _z3,
			_x2, _z2);
		FSYMBOL(x25519_point_double)(p, a, _x1, _z1, _x1, _z1);

		if (k_t) {
			FSYMBOL(bn_int512_move)(_t, _x1);	
			FSYMBOL(bn_int512_move)(_x1, _x2);
			FSYMBOL(bn_int512_move)(_x2, _t);

			FSYMBOL(bn_int512_move)(_t, _z1);
			FSYMBOL(bn_int512_move)(_z1, _z2);
			FSYMBOL(bn_int512_move)(_z2, _t);
		}
	}

	FSYMBOL(x25519_mod_inverse)(_z1, p, _z1);
	FSYMBOL(bn_int512_mul)(_x1, _x1, _z1);
	FSYMBOL(bn_int512_divmod)(_t, r, _x1, p);
} /* end */

/* @func: x25519_shared_key - create a shared secret
* @param1: const bn_int512_t # private key
* @param2: const bn_int512_t # public key (other side)
* @param3: const bn_int512_t # prime modulus
* @param4: const bn_int512_t # curve parameters
* @param5: bn_int512_t       # shared secret
* @return: void
*/
void FSYMBOL(x25519_shared_key)(const bn_int512_t pri, const bn_int512_t pub,
		const bn_int512_t p, const bn_int512_t a, bn_int512_t key) {
	FSYMBOL(x25519_scalar_mul)(pri, pub, p, a, key);
} /* end */

/* @func: x25519_clamp_key - private key clamping
* @param1: bn_int512_t # private key
*/
void FSYMBOL(x25519_clamp_key)(bn_int512_t k) {
	k[0] &= ~0x07;
	k[7] |= 0x40000000;
	k[7] &= 0x7fffffff;
} /* end */

/* @func: x25519_base_mask - mask the most significant bit of 'u' (rfc7748-5.0)
* @param1: bn_int512_t # private key
*/
void FSYMBOL(x25519_base_mask)(bn_int512_t b) {
	b[7] &= 0x7fffffff;
} /* end */
