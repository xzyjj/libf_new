/* x448_ref.c - curve448 elliptic-curve crypto (ecc) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_1024.h>
#include <libf/al/x448_ref.h>


/* @func: _x448_extended_gcd (static) - extended euclidean algorithm
* @param1: const bn_int1024_t # input number
* @param2: const bn_int1024_t # input number
* @param3: bn_int1024_t       # bezout coefficient
* @param4: bn_int1024_t       # bezout coefficient
* @param5: bn_int1024_t       # return greatest common divisor
* @return: void
*/
static void _x448_extended_gcd(const bn_int1024_t a, const bn_int1024_t b,
		bn_int1024_t x, bn_int1024_t y, bn_int1024_t r) {
	bn_int1024_t _b, _x, _y, quo, rem;

	if (!FSYMBOL(bn_int1024_cmp_1)(b, 0)) {
		FSYMBOL(bn_int1024_zero)(x);
		FSYMBOL(bn_int1024_zero)(y);
		x[0] = 1;
		y[0] = 0;
		FSYMBOL(bn_int1024_move)(r, a);
		return; /* x, y, r = (1, 0, a) */
	}

	FSYMBOL(bn_int1024_move)(_b, b);
	/* rem = a % b */
	FSYMBOL(bn_int1024_divmod)(quo, rem, a, b);
	_x448_extended_gcd(_b, rem, _x, _y, r);

	FSYMBOL(bn_int1024_move)(x, _y);

	/* y = _x - ((a / b) * _y) */
	FSYMBOL(bn_int1024_divmod)(quo, rem, a, b);
	FSYMBOL(bn_int1024_mul)(quo, quo, _y);
	FSYMBOL(bn_int1024_sub)(y, _x, quo);
} /* end */

/* @func: x448_mod_inverse - modular inverse based on extended euclidean
* @param1: const bn_int1024_t # number
* @param2: const bn_int1024_t # modulus
* @param3: bn_int1024_t       # inverse modulus
* @return: int32              # 0: success, -1: fail
*/
int32 FSYMBOL(x448_mod_inverse)(const bn_int1024_t a, const bn_int1024_t m,
		bn_int1024_t r) {
	bn_int1024_t x, y, quo, rem, rr;
	_x448_extended_gcd(a, m, x, y, rr);

	if (FSYMBOL(bn_int1024_cmp_1)(rr, 1)) { /* rr != 1 */
		FSYMBOL(bn_int1024_zero)(r);
		return -1;
	} else {
		/* r = ((x % m) + m) % m */
		FSYMBOL(bn_int1024_divmod)(quo, rem, x, m);
		FSYMBOL(bn_int1024_add)(rem, rem, m);
		FSYMBOL(bn_int1024_divmod)(quo, rr, rem, m);
		FSYMBOL(bn_int1024_move)(r, rr);
	}

	return 0;
} /* end */

/* @func: x448_point_add - montgomery curve point addition
* @param1: const bn_int1024_t        # prime modulus
* @param2: const struct x448_point * # curve point x1, z1
* @param3: const struct x448_point * # curve point x2, z2
* @param4: const struct x448_point * # x, z difference
* @param5: struct x448_point *       # curve point x3, z3
* @return: void
*/
void FSYMBOL(x448_point_add)(const bn_int1024_t p,
		const struct x448_point *xz1,
		const struct x448_point *xz2,
		const struct x448_point *xzd,
		struct x448_point *r_xz3) {
	bn_int1024_t _a, _b, _t, _x1, _z1, _x2, _z2;
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
	FSYMBOL(bn_int1024_move)(_x1, xzd->z);
	FSYMBOL(bn_int1024_lsh)(_x1);
	FSYMBOL(bn_int1024_lsh)(_x1);

	/* _z1 = xzd->x << 2 */
	FSYMBOL(bn_int1024_move)(_z1, xzd->x);
	FSYMBOL(bn_int1024_lsh)(_z1);
	FSYMBOL(bn_int1024_lsh)(_z1);

	/* _a = (xz2->x * xz1->x) % p */
	FSYMBOL(bn_int1024_mul)(_a, xz2->x, xz1->x);
	FSYMBOL(bn_int1024_divmod)(_t, _a, _a, p);
	/* _b = (xz2->z * xz1->z) % p */
	FSYMBOL(bn_int1024_mul)(_b, xz2->z, xz1->z);
	FSYMBOL(bn_int1024_divmod)(_t, _b, _b, p);

	/* _x2 = (_a - _b) % p */
	FSYMBOL(bn_int1024_sub)(_x2, _a, _b);
	FSYMBOL(bn_int1024_divmod)(_t, _x2, _x2, p);
	/* _x2 = (_x2 * _x2) % p */
	FSYMBOL(bn_int1024_mul)(_x2, _x2, _x2);
	FSYMBOL(bn_int1024_divmod)(_t, _x2, _x2, p);

	/* _a = (xz2->x * xz1->z) % p */
	FSYMBOL(bn_int1024_mul)(_a, xz2->x, xz1->z);
	FSYMBOL(bn_int1024_divmod)(_t, _a, _a, p);
	/* _b = (xz2->z * xz1->x) % p */
	FSYMBOL(bn_int1024_mul)(_b, xz2->z, xz1->x);
	FSYMBOL(bn_int1024_divmod)(_t, _b, _b, p);

	/* _z2 = (_a - _b) % p */
	FSYMBOL(bn_int1024_sub)(_z2, _a, _b);
	FSYMBOL(bn_int1024_divmod)(_t, _z2, _z2, p);
	/* _z2 = (_z2 * _z2) % p */
	FSYMBOL(bn_int1024_mul)(_z2, _z2, _z2);
	FSYMBOL(bn_int1024_divmod)(_t, _z2, _z2, p);

	/* _x2 = _x2 * _x1 */
	FSYMBOL(bn_int1024_mul)(_x2, _x2, _x1);
	/* _z2 = _z2 * _z1 */
	FSYMBOL(bn_int1024_mul)(_z2, _z2, _z1);

	/* r_xz3->x = _x2 % p */
	FSYMBOL(bn_int1024_divmod)(_t, r_xz3->x, _x2, p);
	/* r_xz3->z = _z2 % p */
	FSYMBOL(bn_int1024_divmod)(_t, r_xz3->z, _z2, p);
} /* end */

/* @func: x448_point_double - montgomery curve point doubling
* @param1: const bn_int1024_t        # prime modulus
* @param2: const bn_int1024_t        # curve parameter
* @param2: const struct x448_point * # curve point x1, z1
* @param5: struct x448_point *       # curve point x3, z3
* @return: void
*/
void FSYMBOL(x448_point_double)(const bn_int1024_t p, const bn_int1024_t a,
		const struct x448_point *xz1,
		struct x448_point *r_xz3) {
	bn_int1024_t _x1, _z1, _x2, _z2, _xz, _t;
	/*
	* t1 = (x1 ** 2) % p
	* t2 = (z1 ** 2) % p
	* t3 = (x1 * z1) % p
	* x3 = (((t1 - t2) % p) ** 2) % p
	* z3 = ((((a * t3) % p) + t1 + t2) * t3) % p
	* z3 = (z3 << 2) % p
	*/

	/* _x2 = (xz1->x * xz1->x) % p */
	FSYMBOL(bn_int1024_mul)(_x2, xz1->x, xz1->x);
	FSYMBOL(bn_int1024_divmod)(_t, _x2, _x2, p);
	/* _z2 = (xz1->z * xz1->z) % p */
	FSYMBOL(bn_int1024_mul)(_z2, xz1->z, xz1->z);
	FSYMBOL(bn_int1024_divmod)(_t, _z2, _z2, p);
	/* _xz = (xz1->x * xz1->z) % p */
	FSYMBOL(bn_int1024_mul)(_xz, xz1->x, xz1->z);
	FSYMBOL(bn_int1024_divmod)(_t, _xz, _xz, p);

	/* _x1 = (_x2 - _z2) % p */
	FSYMBOL(bn_int1024_sub)(_x1, _x2, _z2);
	FSYMBOL(bn_int1024_divmod)(_t, _x1, _x1, p);
	/* _x1 = _x1 * _x1 */
	FSYMBOL(bn_int1024_mul)(_x1, _x1, _x1);

	/* _z1 = (a * _xz) % p */
	FSYMBOL(bn_int1024_mul)(_z1, a, _xz);
	FSYMBOL(bn_int1024_divmod)(_t, _z1, _z1, p);
	/* _z1 = _z1 + _x2 */
	FSYMBOL(bn_int1024_add)(_z1, _z1, _x2);
	/* _z1 = _z1 + _z2 */
	FSYMBOL(bn_int1024_add)(_z1, _z1, _z2);
	/* _z1 = (_z1 * _xz) % p */
	FSYMBOL(bn_int1024_mul)(_z1, _z1, _xz);
	FSYMBOL(bn_int1024_divmod)(_t, _z1, _z1, p);

	/* _z1 = _z1 << 2 */
	FSYMBOL(bn_int1024_lsh)(_z1);
	FSYMBOL(bn_int1024_lsh)(_z1);

	/* r_xz3->x = _x1 % p */
	FSYMBOL(bn_int1024_divmod)(_t, r_xz3->x, _x1, p);
	/* r_xz3->z = _z1 % p */
	FSYMBOL(bn_int1024_divmod)(_t, r_xz3->z, _z1, p);
} /* end */

/* @func: x448_scalar_mul - montgomery ladder scalar multiplication
* @param1: const bn_int1024_t # prime modulus
* @param2: const bn_int1024_t # curve parameter
* @param3: const bn_int1024_t # private key
* @param4: const bn_int1024_t # base point
* @param5: bn_int1024_t       # scalar
* @return: void
*/
void FSYMBOL(x448_scalar_mul)(const bn_int1024_t p, const bn_int1024_t a,
		const bn_int1024_t k, const bn_int1024_t b, bn_int1024_t r) {
	struct x448_point _xz1, _xz2, _xz3;
	bn_int1024_t _t;

	FSYMBOL(bn_int1024_zero)(_xz1.x);
	FSYMBOL(bn_int1024_zero)(_xz1.z);
	_xz1.x[0] = 1; /* x1, z1 = (1, 0) */

	FSYMBOL(bn_int1024_move)(_xz2.x, b);
	FSYMBOL(bn_int1024_zero)(_xz2.z);
	_xz2.z[0] = 1; /* x2, z2 = (b, 1) */

	FSYMBOL(bn_int1024_move)(_xz3.x, b);
	FSYMBOL(bn_int1024_zero)(_xz3.z);
	_xz3.z[0] = 1; /* x3, z3 = (b, 1) */

	for (int32 i = 448; i >= 0; i--) {
		int32 k_t = (k[i / 32] >> (i % 32)) & 1;
		if (k_t) {
			FSYMBOL(bn_int1024_move)(_t, _xz1.x);
			FSYMBOL(bn_int1024_move)(_xz1.x, _xz2.x);
			FSYMBOL(bn_int1024_move)(_xz2.x, _t);
			FSYMBOL(bn_int1024_move)(_t, _xz1.z);
			FSYMBOL(bn_int1024_move)(_xz1.z, _xz2.z);
			FSYMBOL(bn_int1024_move)(_xz2.z, _t);
		}

		FSYMBOL(x448_point_add)(p, &_xz1, &_xz2, &_xz3, &_xz2);
		FSYMBOL(x448_point_double)(p, a, &_xz1, &_xz1);

		if (k_t) {
			FSYMBOL(bn_int1024_move)(_t, _xz1.x);
			FSYMBOL(bn_int1024_move)(_xz1.x, _xz2.x);
			FSYMBOL(bn_int1024_move)(_xz2.x, _t);
			FSYMBOL(bn_int1024_move)(_t, _xz1.z);
			FSYMBOL(bn_int1024_move)(_xz1.z, _xz2.z);
			FSYMBOL(bn_int1024_move)(_xz2.z, _t);
		}
	}

	/* r = (inv(z1, p) * x1) % p */
	FSYMBOL(x448_mod_inverse)(_xz1.z, p, _t);
	FSYMBOL(bn_int1024_mul)(_t, _t, _xz1.x);
	FSYMBOL(bn_int1024_divmod)(_t, r, _t, p);
} /* end */

/* @func: x448_clamp_key - private key clamping
* @param1: bn_int1024_t # private key
* @return: void
*/
void FSYMBOL(x448_clamp_key)(bn_int1024_t k) {
	k[0] &= ~0x03;
	k[13] |= 0x80000000;
} /* end */

/* @func: x448_private_key - x448 private key operation function
* @param1: uint8 * # private key (length: X448_LEN)
* @return: void
*/
void FSYMBOL(x448_private_key)(uint8 *pri) {
	FSYMBOL(x448_clamp_key)((uint32 *)pri);
} /* end */

/* @func: x448_public_key - x448 public key create function
* @param1: const uint8 * # private key (length: X448_LEN)
* @param2: uint8 *       # public key (length: X448_LEN)
* @return: void
*/
void FSYMBOL(x448_public_key)(const uint8 *pri, uint8 *pub) {
	bn_int1024_t p, a, b, _pri, _pub;
	FSYMBOL(bn_int1024_strtonum)(p, X448_P, NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(a, X448_A, NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, X448_B, NULL, 10);
	FSYMBOL(bn_int1024_zero)(_pri);
	XSYMBOL(memcpy)(_pri, pri, X448_LEN);

	FSYMBOL(x448_scalar_mul)(p, a, _pri, b, _pub);

	XSYMBOL(memcpy)(pub, _pub, X448_LEN);
} /* end */

/* @func: x448_shared_key - x448 shared key create function
* @param1: const uint8 * # private key (length: X448_LEN)
* @param2: const uint8 * # public key (length: X448_LEN)
* @param3: uint8 *       # shared key (length: X448_LEN)
* @return: void
*/
void FSYMBOL(x448_shared_key)(const uint8 *pri, const uint8 *pub,
		uint8 *key) {
	bn_int1024_t p, a, b, _pri, _pub, _key;
	FSYMBOL(bn_int1024_strtonum)(p, X448_P, NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(a, X448_A, NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, X448_B, NULL, 10);
	FSYMBOL(bn_int1024_zero)(_pri);
	FSYMBOL(bn_int1024_zero)(_pub);
	XSYMBOL(memcpy)(_pri, pri, X448_LEN);
	XSYMBOL(memcpy)(_pub, pub, X448_LEN);

	FSYMBOL(x448_scalar_mul)(p, a, _pri, _pub, _key);

	XSYMBOL(memcpy)(key, _key, X448_LEN);
} /* end */
