/* ed25519.c - edwards-curve digital signature algorithm (eddsa) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_512.h>
#include <libf/al/ed25519.h>


/* @func: _ed25519_extended_gcd (static) - extended euclidean algorithm
* @param1: const bn_int512_t # input number
* @param2: const bn_int512_t # input number
* @param3: bn_int512_t       # bezout coefficient
* @param4: bn_int512_t       # bezout coefficient
* @param5: bn_int512_t       # return greatest common divisor
* @return: void
*/
static void _ed25519_extended_gcd(const bn_int512_t a, const bn_int512_t b,
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
	_ed25519_extended_gcd(_b, rem, _x, _y, r);

	FSYMBOL(bn_int512_move)(x, _y);

	/* y = _x - ((a / b) * _y) */
	FSYMBOL(bn_int512_divmod)(quo, rem, a, b);
	FSYMBOL(bn_int512_mul)(quo, quo, _y);
	FSYMBOL(bn_int512_sub)(y, _x, quo);
} /* end */

/* @func: ed25519_mod_inverse - modular inverse based on extended euclidean
* @param1: const bn_int512_t # number
* @param2: const bn_int512_t # modulus
* @param3: bn_int512_t       # inverse modulus
* @return: int32             # 0: success, -1: fail
*/
int32 FSYMBOL(ed25519_mod_inverse)(const bn_int512_t a, const bn_int512_t m,
		bn_int512_t r) {
	bn_int512_t x, y, quo, rem, rr;
	_ed25519_extended_gcd(a, m, x, y, rr);

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

/* @func: ed25519_mod_pow - fast modular exponentiation
* @param1: const bn_int512_t # number
* @param2: const bn_int512_t # exponential
* @param3: const bn_int512_t # modulus
* @param4: bn_int512_t       # result
* @return: void
*/
void FSYMBOL(ed25519_mod_pow)(const bn_int512_t a, const bn_int512_t b,
		const bn_int512_t m, bn_int512_t r) {
	bn_int512_t rr, quo, a1, b1;
	FSYMBOL(bn_int512_zero)(rr);
	FSYMBOL(bn_int512_move)(b1, b);
	rr[0] = 1; /* rr = 1 */

	/* a1 = a1 % m */
	FSYMBOL(bn_int512_divmod)(quo, a1, a, m);
	while (FSYMBOL(bn_int512_cmp_1)(b1, 0) > 0) {
		if (b1[0] & 1) { /* b1 & 1 */
			/* rr = (rr * a1) % m */
			FSYMBOL(bn_int512_mul)(rr, rr, a1);
			FSYMBOL(bn_int512_divmod)(quo, rr, rr, m);
		}

		/* a1 = (a1 * a1) % m */
		FSYMBOL(bn_int512_mul)(a1, a1, a1);
		FSYMBOL(bn_int512_divmod)(quo, a1, a1, m);
		/* b1 >>= 1 */
		FSYMBOL(bn_int512_rsh)(b1);
	}

	FSYMBOL(bn_int512_move)(r, rr);
} /* end */

/* @func: ed25519_point_add - edwards curve point addition
* @param1: const bn_int512_t            # prime modulus
* @param2: const bn_int512_t            # curve constant
* @param3: const struct ed25519_point * # curve point x1, y1, z1, t1
* @param4: const struct ed25519_point * # curve point x2, y2, z2, t2
* @param5: struct ed25519_point *       # curve point x3, y3, z3, t3
* @return: void
*/
void FSYMBOL(ed25519_point_add)(const bn_int512_t p, const bn_int512_t d,
		const struct ed25519_point *xyz1,
		const struct ed25519_point *xyz2,
		struct ed25519_point *r_xyz3) {
	bn_int512_t _a, _b, _t, _a1, _b1, _c1, _d1, _e1, _f1, _g1, _h1;
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

	/* _a = (xyz1->y - xyz1->x) % p */
	FSYMBOL(bn_int512_sub)(_a, xyz1->y, xyz1->x);
	FSYMBOL(bn_int512_divmod)(_t, _a, _a, p);
	/* _b = (xyz2->y - xyz2->x) % p */
	FSYMBOL(bn_int512_sub)(_b, xyz2->y, xyz2->x);
	FSYMBOL(bn_int512_divmod)(_t, _b, _b, p);
	/* _a1 = (_a * _b) % p */
	FSYMBOL(bn_int512_mul)(_a1, _a, _b);
	FSYMBOL(bn_int512_divmod)(_t, _a1, _a1, p);

	/* _a = (xyz1->y + xyz1->x) % p */
	FSYMBOL(bn_int512_add)(_a, xyz1->y, xyz1->x);
	/* _b = (xyz2->y + xyz2->x) % p */
	FSYMBOL(bn_int512_add)(_b, xyz2->y, xyz2->x);
	/* _b1 = (_a * _b) % p */
	FSYMBOL(bn_int512_mul)(_b1, _a, _b);
	FSYMBOL(bn_int512_divmod)(_t, _b1, _b1, p);

	/* _c1 = (xyz1->t * xyz2->t) % p */
	FSYMBOL(bn_int512_mul)(_c1, xyz1->t, xyz2->t);
	FSYMBOL(bn_int512_divmod)(_t, _c1, _c1, p);
	/* _c1 = ((_c1 * d) % p) << 1 */
	FSYMBOL(bn_int512_mul)(_c1, _c1, d);
	FSYMBOL(bn_int512_divmod)(_t, _c1, _c1, p);
	FSYMBOL(bn_int512_lsh)(_c1);

	/* _d1 = ((xyz1->z * xyz2->z) % p) << 1 */
	FSYMBOL(bn_int512_mul)(_d1, xyz1->z, xyz2->z);
	FSYMBOL(bn_int512_divmod)(_t, _d1, _d1, p);
	FSYMBOL(bn_int512_lsh)(_d1);

	/* _e1 = (_b1 - _a1) % p */
	FSYMBOL(bn_int512_sub)(_e1, _b1, _a1);
	FSYMBOL(bn_int512_divmod)(_t, _e1, _e1, p);

	/* _f1 = (_d1 - _c1) % p */
	FSYMBOL(bn_int512_sub)(_f1, _d1, _c1);
	FSYMBOL(bn_int512_divmod)(_t, _f1, _f1, p);

	/* _g1 = (_d1 + _c1) % p */
	FSYMBOL(bn_int512_add)(_g1, _d1, _c1);
	FSYMBOL(bn_int512_divmod)(_t, _g1, _g1, p);

	/* _h1 = (_b1 + _a1) % p */
	FSYMBOL(bn_int512_add)(_h1, _b1, _a1);
	FSYMBOL(bn_int512_divmod)(_t, _h1, _h1, p);

	/* xyz3->x = (_e1 * _f1) % p */
	FSYMBOL(bn_int512_mul)(_t, _e1, _f1);
	FSYMBOL(bn_int512_divmod)(_t, r_xyz3->x, _t, p);

	/* xyz3->y = (_g1 * _h1) % p */
	FSYMBOL(bn_int512_mul)(_t, _g1, _h1);
	FSYMBOL(bn_int512_divmod)(_t, r_xyz3->y, _t, p);

	/* xyz3->t = (_e1 * _h1) % p */
	FSYMBOL(bn_int512_mul)(_t, _e1, _h1);
	FSYMBOL(bn_int512_divmod)(_t, r_xyz3->t, _t, p);

	/* xyz3->z = (_f1 * _g1) % p */
	FSYMBOL(bn_int512_mul)(_t, _f1, _g1);
	FSYMBOL(bn_int512_divmod)(_t, r_xyz3->z, _t, p);
} /* end */

/* @func: ed25519_point_double - edwards curve point doubling
* @param1: const bn_int512_t            # prime modulus
* @param2: const bn_int512_t            # curve constant
* @param3: const struct ed25519_point * # curve point x1, y1, z1, t1
* @param5: struct ed25519_point *       # curve point x3, y3, z3, t3
* @return: void
*/
void FSYMBOL(ed25519_point_double)(const bn_int512_t p, const bn_int512_t d,
		const struct ed25519_point *xyz1,
		struct ed25519_point *r_xyz3) {
	FSYMBOL(ed25519_point_add)(p, d, xyz1, xyz1, r_xyz3);
} /* end */

/* @func: ed25519_scalar_mul - montgomery ladder scalar multiplication
* @param1: const bn_int512_t            # prime modulus
* @param2: const bn_int512_t            # curve constant
* @param3: const bn_int512_t            # private key
* @param4: const struct ed25519_point * # curve point x1, y1, z1, t1
* @param5: struct ed25519_point *       # curve point x2, y2, z2, t2
* @return: void
*/
void FSYMBOL(ed25519_scalar_mul)(const bn_int512_t p, const bn_int512_t d,
		const bn_int512_t k, const struct ed25519_point *xyz1,
		struct ed25519_point *r_xyz2) {
	struct ed25519_point _xyz1, _xyz2, _xyz3;
	FSYMBOL(bn_int512_zero)(_xyz1.x);
	FSYMBOL(bn_int512_zero)(_xyz1.y);
	FSYMBOL(bn_int512_zero)(_xyz1.z);
	FSYMBOL(bn_int512_zero)(_xyz1.t);
	_xyz1.y[0] = 1;
	_xyz1.z[0] = 1; /* xyz1 = (0, 1, 1, 8) */

	FSYMBOL(bn_int512_move)(_xyz2.x, xyz1->x);
	FSYMBOL(bn_int512_move)(_xyz2.y, xyz1->y);
	FSYMBOL(bn_int512_move)(_xyz2.z, xyz1->z);
	FSYMBOL(bn_int512_move)(_xyz2.t, xyz1->t);

	for (int32 i = 0; i < 256; i++) {
		FSYMBOL(ed25519_point_add)(p, d, &_xyz1, &_xyz2, &_xyz3);

		int32 k_t = (k[i / 32] >> (i % 32)) & 1;
		if (k_t) {
			FSYMBOL(bn_int512_move)(_xyz1.x, _xyz3.x);
			FSYMBOL(bn_int512_move)(_xyz1.y, _xyz3.y);
			FSYMBOL(bn_int512_move)(_xyz1.z, _xyz3.z);
			FSYMBOL(bn_int512_move)(_xyz1.t, _xyz3.t);

		}

		FSYMBOL(ed25519_point_double)(p, d, &_xyz2, &_xyz2);
	}

	FSYMBOL(bn_int512_move)(r_xyz2->x, _xyz1.x);
	FSYMBOL(bn_int512_move)(r_xyz2->y, _xyz1.y);
	FSYMBOL(bn_int512_move)(r_xyz2->z, _xyz1.z);
	FSYMBOL(bn_int512_move)(r_xyz2->t, _xyz1.t);
} /* end */

/* @func: ed25519_point_equal - edwards curve point comparison is equal
* @param1: const bn_int512_t            # prime modulus
* @param2: const struct ed25519_point * # curve point x1, y1, z1, t1
* @param3: const struct ed25519_point * # curve point x2, y2, z2, t2
* @return: int32                        # 0: p1 == p2, 1: p1 != p2
*/
int32 FSYMBOL(ed25519_point_equal)(const bn_int512_t p,
		const struct ed25519_point *xyz1,
		const struct ed25519_point *xyz2) {
	bn_int512_t _a, _b, _t;
	/*
	* (((x1 * z2) - (x2 * z1)) % p) != 0
	* (((y1 * z2) - (y2 * z1)) % p) != 0
	*/

	FSYMBOL(bn_int512_mul)(_a, xyz1->x, xyz2->z);
	FSYMBOL(bn_int512_mul)(_b, xyz2->x, xyz1->z);
	FSYMBOL(bn_int512_sub)(_a, _a, _b);
	FSYMBOL(bn_int512_divmod)(_t, _b, _a, p);
	if (FSYMBOL(bn_int512_cmp_1)(_b, 0))
		return 1;

	FSYMBOL(bn_int512_mul)(_a, xyz1->y, xyz2->z);
	FSYMBOL(bn_int512_mul)(_b, xyz2->y, xyz1->z);
	FSYMBOL(bn_int512_sub)(_a, _a, _b);
	FSYMBOL(bn_int512_divmod)(_t, _b, _a, p);
	if (FSYMBOL(bn_int512_cmp_1)(_b, 0))
		return 1;

	return 0;
} /* end */

/* @func: ed25519_point_recover_x - calculate the corresponding curve point x
* @param1: const bn_int512_t # prime modulus
* @param2: const bn_int512_t # curve constant
* @param3: const bn_int512_t # curve point y
* @param4: uint32            # sign (compressd x high-bit: k[7] >> 31)
* @param5: bn_int512_t       # curve point x
* @return: void
*/
void FSYMBOL(ed25519_point_recover_x)(const bn_int512_t p, const bn_int512_t d,
		const bn_int512_t y, uint32 sign, bn_int512_t r) {
	bn_int512_t _x1, _x2, _y2, _x, _t1, _t2;
	/*
	* y2 = (y ** 2) % p
	* x1 = y2 - 1
	* x2 = ((d * y2) % p) + 1
	* x2 = (x1 * inv(x2, p)) % p
	* x = modpow(x2, (p + 3) / 8, p)
	* if ((x ** 2) % p)
	*   x = (x * modpow(2, (p - 1) / 4, p)) % p
	* if ((x & 1) != sign)
	*   x = p - x
	*/

	FSYMBOL(bn_int512_zero)(_t1);
	_t1[0] = 1;

	/* _y2 = (y ** 2) % p */
	FSYMBOL(bn_int512_mul)(_y2, y, y);
	FSYMBOL(bn_int512_divmod)(_t2, _y2, _y2, p);

	/* _x1 = _y2 - 1 */
	FSYMBOL(bn_int512_sub)(_x1, _y2, _t1);

	/* _x2 = ((d * _y2) % p) + 1 */
	FSYMBOL(bn_int512_mul)(_x2, d, _y2);
	FSYMBOL(bn_int512_divmod)(_t2, _x2, _x2, p);
	FSYMBOL(bn_int512_add)(_x2, _x2, _t1);

	/* _x2 = (_x1 * inv(_x2, p)) % p */
	FSYMBOL(ed25519_mod_inverse)(_x2, p, _x2);
	FSYMBOL(bn_int512_mul)(_x2, _x1, _x2);
	FSYMBOL(bn_int512_divmod)(_t2, _x2, _x2, p);

	/* _t2 = (p + 3) / 8 */
	_t1[0] = 3;
	FSYMBOL(bn_int512_add)(_t2, p, _t1);
	_t1[0] = 8;
	FSYMBOL(bn_int512_divmod)(_t2, _t1, _t2, _t1);

	/* _x = modpow(_x2, _t2, p) */
	FSYMBOL(ed25519_mod_pow)(_x2, _t2, p, _x);

	/* _t2 = _x ** 2 */
	FSYMBOL(bn_int512_mul)(_t2, _x, _x);

	/* ((_t2 - _x2) % p) != 0 */
	FSYMBOL(bn_int512_sub)(_t2, _t2, _x2);
	FSYMBOL(bn_int512_divmod)(_t1, _t2, _t2, p);
	if (FSYMBOL(bn_int512_cmp_1)(_t2, 0)) {
		/* _t2 = (p - 1) / 4 */
		FSYMBOL(bn_int512_zero)(_t1);
		_t1[0] = 1;
		FSYMBOL(bn_int512_sub)(_t2, p, _t1);
		_t1[0] = 4;
		FSYMBOL(bn_int512_divmod)(_t2, _t1, _t2, _t1);

		/* _t2 = modpow(2, t2, p) */
		FSYMBOL(bn_int512_zero)(_t1);
		_t1[0] = 2;
		FSYMBOL(ed25519_mod_pow)(_t1, _t2, p, _t2);

		/* _x = (_x * _t2) % p  */
		FSYMBOL(bn_int512_mul)(_x, _x, _t2);
		FSYMBOL(bn_int512_divmod)(_t1, _x, _x, p);
	}

	/* (x & 1) != sign */
	if ((_x[0] & 1) != sign)
		FSYMBOL(bn_int512_sub)(_x, p, _x);

	FSYMBOL(bn_int512_move)(r, _x);
} /* end */

/* @func: ed25519_point_compress - curve point compress
* @param1: const bn_int512_t            # prime modulus
* @param2: const struct ed25519_point * # curve point x1, y1, z1, t1
* @param3: bn_int512_t                  # public key
* @return: void
*/
void FSYMBOL(ed25519_point_compress)(const bn_int512_t p,
		const struct ed25519_point *xyz1, bn_int512_t r) {
	bn_int512_t _x, _y, _z, _t;
	/*
	* x = (x1 * inv(z, p)) % p
	* y = (y1 * inv(z, p)) % p
	* r = y | ((x & 1) << 255)
	*/

	/* _z = inv(z, p) */
	FSYMBOL(ed25519_mod_inverse)(xyz1->z, p, _z);

	/* _x = (xyz1->x * _z) % p */
	FSYMBOL(bn_int512_mul)(_x, xyz1->x, _z);
	FSYMBOL(bn_int512_divmod)(_t, _x, _x, p);

	/* _y = (xyz1->y * _z) % p */
	FSYMBOL(bn_int512_mul)(_y, xyz1->y, _z);
	FSYMBOL(bn_int512_divmod)(_t, _y, _y, p);

	/* r = _y | ((_x & 1) << 255) */
	_y[7] |= (_x[0] & 1) << 31;
	FSYMBOL(bn_int512_move)(r, _y);
} /* end */

/* @func: ed25519_point_decompress - curve point decompress
* @param1: const bn_int512_t      # prime modulus
* @param2: const bn_int512_t      # curve constant
* @param3: const bn_int512_t      # public key
* @param4: struct ed25519_point * # curve point x1, y1, z1, t1
* @return: void
*/
void FSYMBOL(ed25519_point_decompress)(const bn_int512_t p, const bn_int512_t d,
		const bn_int512_t k, struct ed25519_point *r_xyz1) {
	bn_int512_t _y, _x, _t;
	FSYMBOL(bn_int512_move)(_y, k);
	/*
	* y = y1 & ((1 << 255) - 1)
	* x1 = rec_x(p, d, y, k >> 255)
	* y1 = y1
	* z1 = 1
	* t1 = (x1 * y) % p
	*/

	/* _y = _y & ((1 << 255) - 1) */
	_y[7] &= (1U << 31) - 1;

	/* _x = rec_x(p, d, _y) */
	FSYMBOL(ed25519_point_recover_x)(p, d, _y, k[7] >> 31, _x);

	/* x, y, z, t = (_x, _y, 1, (_x * _y) % p) */
	FSYMBOL(bn_int512_move)(r_xyz1->x, _x);
	FSYMBOL(bn_int512_move)(r_xyz1->y, _y);
	FSYMBOL(bn_int512_zero)(r_xyz1->z);
	r_xyz1->z[0] = 1;
	FSYMBOL(bn_int512_mul)(_t, _x, _y);
	FSYMBOL(bn_int512_divmod)(_t, r_xyz1->t, _t, p);
} /* end */

/* @func: ed25519_clamp_key - private key clamping
* @param1: bn_int512_t # private key
*/
void FSYMBOL(ed25519_clamp_key)(bn_int512_t k) {
	k[0] &= ~0x07;
	k[7] |= 0x40000000;
	k[7] &= 0x7fffffff;
} /* end */
