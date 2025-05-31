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
static void _x25519_extended_gcd(const bn_int512_t a, const bn_int512_t b,
		bn_int512_t x, bn_int512_t y, bn_int512_t r) {
	bn_int512_t _b, _x, _y, quo, rem;

	if (!FSYMBOL(bn_int512_cmp_1)(b, 0)) {
		FSYMBOL(bn_int512_zero)(x);
		FSYMBOL(bn_int512_zero)(y);
		x[0] = 1;
		y[0] = 0;
		FSYMBOL(bn_int512_move)(r, a);
		return; /* x, y, r = (1, 0, a) */
	}

	FSYMBOL(bn_int512_move)(_b, b);
	/* rem = a % b */
	FSYMBOL(bn_int512_divmod)(quo, rem, a, b);
	_x25519_extended_gcd(_b, rem, _x, _y, r);

	FSYMBOL(bn_int512_move)(x, _y);

	/* y = _x - ((a / b) * _y) */
	FSYMBOL(bn_int512_divmod)(quo, rem, a, b);
	FSYMBOL(bn_int512_mul)(quo, quo, _y);
	FSYMBOL(bn_int512_sub)(y, _x, quo);
} /* end */

/* @func: x25519_mod_inverse - modular inverse based on extended euclidean
* @param1: const bn_int512_t # number
* @param2: const bn_int512_t # modulus
* @param3: bn_int512_t       # inverse modulus
* @return: int32             # 0: success, -1: fail
*/
int32 FSYMBOL(x25519_mod_inverse)(const bn_int512_t a, const bn_int512_t m,
		bn_int512_t r) {
	bn_int512_t x, y, quo, rem, rr;
	_x25519_extended_gcd(a, m, x, y, rr);

	if (FSYMBOL(bn_int512_cmp_1)(rr, 1)) { /* rr != 1 */
		FSYMBOL(bn_int512_zero)(r);
		return -1;
	} else {
		/* r = ((x % m) + m) % m */
		FSYMBOL(bn_int512_divmod)(quo, rem, x, m);
		FSYMBOL(bn_int512_add)(rem, rem, m);
		FSYMBOL(bn_int512_divmod)(quo, rr, rem, m);
		FSYMBOL(bn_int512_move)(r, rr);
	}

	return 0;
} /* end */

/* @func: x25519_point_add - montgomery curve point addition
* @param1: const bn_int512_t           # prime modulus
* @param2: const struct x25519_point * # curve point x1, z1
* @param3: const struct x25519_point * # curve point x2, z2
* @param4: const struct x25519_point * # x, z difference
* @param5: struct x25519_point *       # curve point x3, z3
* @return: void
*/
void FSYMBOL(x25519_point_add)(const bn_int512_t p,
		const struct x25519_point *xz1,
		const struct x25519_point *xz2,
		const struct x25519_point *xzd,
		struct x25519_point *r_xz3) {
	bn_int512_t _a, _b, _t, _x1, _z1, _x2, _z2;
	/*
	* a1 = (x2 * x1) % p
	* b1 = (z2 * z1) % p
	* x3 = (((a1 - b1) % p) ** 2) % p
	* x3 = ((z_d << 2) * x3) % p
	* a2 = (x2 * z1) % p
	* b2 = (z2 * x1) % p
	* z3 = (((a2 - b2) % p) ** 2) % p
	* z3 = ((x_d << 2) * z3) % p
	*/

	/* _x1 = xzd->z << 2 */
	FSYMBOL(bn_int512_move)(_x1, xzd->z);
	FSYMBOL(bn_int512_lsh)(_x1);
	FSYMBOL(bn_int512_lsh)(_x1);

	/* _z1 = xzd->x << 2 */
	FSYMBOL(bn_int512_move)(_z1, xzd->x);
	FSYMBOL(bn_int512_lsh)(_z1);
	FSYMBOL(bn_int512_lsh)(_z1);

	/* _a = (xz2->x * xz1->x) % p */
	FSYMBOL(bn_int512_mul)(_a, xz2->x, xz1->x);
	FSYMBOL(bn_int512_divmod)(_t, _a, _a, p);
	/* _b = (xz2->z * xz1->z) % p */
	FSYMBOL(bn_int512_mul)(_b, xz2->z, xz1->z);
	FSYMBOL(bn_int512_divmod)(_t, _b, _b, p);

	/* _x2 = (_a - _b) % p */
	FSYMBOL(bn_int512_sub)(_x2, _a, _b);
	FSYMBOL(bn_int512_divmod)(_t, _x2, _x2, p);
	/* _x2 = (_x2 * _x2) % p */
	FSYMBOL(bn_int512_mul)(_x2, _x2, _x2);
	FSYMBOL(bn_int512_divmod)(_t, _x2, _x2, p);

	/* _a = (xz2->x * xz1->z) % p */
	FSYMBOL(bn_int512_mul)(_a, xz2->x, xz1->z);
	FSYMBOL(bn_int512_divmod)(_t, _a, _a, p);
	/* _b = (xz2->z * xz1->x) % p */
	FSYMBOL(bn_int512_mul)(_b, xz2->z, xz1->x);
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

	/* r_xz3->x = _x2 % p */
	FSYMBOL(bn_int512_divmod)(_t, r_xz3->x, _x2, p);
	/* r_xz3->z = _z2 % p */
	FSYMBOL(bn_int512_divmod)(_t, r_xz3->z, _z2, p);
} /* end */

/* @func: x25519_point_double - montgomery curve point doubling
* @param1: const bn_int512_t           # prime modulus
* @param2: const bn_int512_t           # curve parameter
* @param2: const struct x25519_point * # curve point x1, z1
* @param5: struct x25519_point *       # curve point x3, z3
* @return: void
*/
void FSYMBOL(x25519_point_double)(const bn_int512_t p, const bn_int512_t a,
		const struct x25519_point *xz1,
		struct x25519_point *r_xz3) {
	bn_int512_t _x1, _z1, _x2, _z2, _xz, _t;
	/*
	* t1 = (x1 ** 2) % p
	* t2 = (z1 ** 2) % p
	* t3 = (x1 * z1) % p
	* x3 = (((t1 - t2) % p) ** 2) % p
	* z3 = ((((a * t3) % p) + t1 + t2) * t3) % p
	* z3 = (z3 << 2) % p
	*/

	/* _x2 = (xz1->x * xz1->x) % p */
	FSYMBOL(bn_int512_mul)(_x2, xz1->x, xz1->x);
	FSYMBOL(bn_int512_divmod)(_t, _x2, _x2, p);
	/* _z2 = (xz1->z * xz1->z) % p */
	FSYMBOL(bn_int512_mul)(_z2, xz1->z, xz1->z);
	FSYMBOL(bn_int512_divmod)(_t, _z2, _z2, p);
	/* _xz = (xz1->x * xz1->z) % p */
	FSYMBOL(bn_int512_mul)(_xz, xz1->x, xz1->z);
	FSYMBOL(bn_int512_divmod)(_t, _xz, _xz, p);

	/* _x1 = (_x2 - _z2) % p */
	FSYMBOL(bn_int512_sub)(_x1, _x2, _z2);
	FSYMBOL(bn_int512_divmod)(_t, _x1, _x1, p);
	/* _x1 = _x1 * _x1 */
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

	/* r_xz3->x = _x1 % p */
	FSYMBOL(bn_int512_divmod)(_t, r_xz3->x, _x1, p);
	/* r_xz3->z = _z1 % p */
	FSYMBOL(bn_int512_divmod)(_t, r_xz3->z, _z1, p);
} /* end */

/* @func: x25519_scalar_mul - montgomery ladder scalar multiplication
* @param1: const bn_int512_t # prime modulus
* @param2: const bn_int512_t # curve parameter
* @param3: const bn_int512_t # private key
* @param4: const bn_int512_t # base point
* @param5: bn_int512_t       # scalar
* @return: void
*/
void FSYMBOL(x25519_scalar_mul)(const bn_int512_t p, const bn_int512_t a,
		const bn_int512_t k, const bn_int512_t b, bn_int512_t r) {
	struct x25519_point _xz1, _xz2, _xz3;
	bn_int512_t _t;

	FSYMBOL(bn_int512_zero)(_xz1.x);
	FSYMBOL(bn_int512_zero)(_xz1.z);
	_xz1.x[0] = 1; /* x1, z1 = (1, 0) */

	FSYMBOL(bn_int512_move)(_xz2.x, b);
	FSYMBOL(bn_int512_zero)(_xz2.z);
	_xz2.z[0] = 1; /* x2, z2 = (b, 1) */

	FSYMBOL(bn_int512_move)(_xz3.x, b);
	FSYMBOL(bn_int512_zero)(_xz3.z);
	_xz3.z[0] = 1; /* x3, z3 = (b, 1) */

	for (int32 i = 255; i >= 0; i--) {
		int32 k_t = (k[i / 32] >> (i % 32)) & 1;
		if (k_t) {
			FSYMBOL(bn_int512_move)(_t, _xz1.x);
			FSYMBOL(bn_int512_move)(_xz1.x, _xz2.x);
			FSYMBOL(bn_int512_move)(_xz2.x, _t);
			FSYMBOL(bn_int512_move)(_t, _xz1.z);
			FSYMBOL(bn_int512_move)(_xz1.z, _xz2.z);
			FSYMBOL(bn_int512_move)(_xz2.z, _t);
		}

		FSYMBOL(x25519_point_add)(p, &_xz1, &_xz2, &_xz3, &_xz2);
		FSYMBOL(x25519_point_double)(p, a, &_xz1, &_xz1);

		if (k_t) {
			FSYMBOL(bn_int512_move)(_t, _xz1.x);
			FSYMBOL(bn_int512_move)(_xz1.x, _xz2.x);
			FSYMBOL(bn_int512_move)(_xz2.x, _t);
			FSYMBOL(bn_int512_move)(_t, _xz1.z);
			FSYMBOL(bn_int512_move)(_xz1.z, _xz2.z);
			FSYMBOL(bn_int512_move)(_xz2.z, _t);
		}
	}

	/* r = (inv(z1, p) * x1) % p */
	FSYMBOL(x25519_mod_inverse)(_xz1.z, p, _t);
	FSYMBOL(bn_int512_mul)(_t, _t, _xz1.x);
	FSYMBOL(bn_int512_divmod)(_t, r, _t, p);
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
