/* bn_6145_gcd.c - big number calculation (6145-bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_6145.h>


/* @func: bn_i6145_gcd - extended euclidean algorithm (non-recursive)
* @param1: bn_6145_t       # greatest common divisor
* @param2: bn_6145_t       # bezout coefficient
* @param3: bn_6145_t       # bezout coefficient
* @param4: const bn_6145_t # number
* @param5: const bn_6145_t # number
* @return: void
*/
void FSYMBOL(bn_i6145_gcd)(bn_6145_t r, bn_6145_t x, bn_6145_t y,
		const bn_6145_t a, const bn_6145_t b) {
	bn_6145_t a1, b1, s1, s2, t1, t2, t3, t4;
	FSYMBOL(bn_6145_move)(a1, a);
	FSYMBOL(bn_6145_move)(b1, b);
	FSYMBOL(bn_6145_set1)(s1, 1);
	FSYMBOL(bn_6145_set1)(s2, 0);
	FSYMBOL(bn_6145_set1)(t1, 0);
	FSYMBOL(bn_6145_set1)(t2, 1);

	while (FSYMBOL(bn_6145_bits)(b1)) {
		/* t3 = a1 % b1 */
		FSYMBOL(bn_i6145_divmod)(t3, r, a1, b1);

		/* (a1, b1) = (b1, a1 - (b1 * t3)) */
		FSYMBOL(bn_i6145_mul)(t4, b1, t3);
		FSYMBOL(bn_i6145_sub)(t4, a1, t4);
		FSYMBOL(bn_6145_move)(a1, b1);
		FSYMBOL(bn_6145_move)(b1, t4);

		/* (s1, s2) = (s2, s1 - (s2 * t3)) */
		FSYMBOL(bn_i6145_mul)(t4, s2, t3);
		FSYMBOL(bn_i6145_sub)(t4, s1, t4);
		FSYMBOL(bn_6145_move)(s1, s2);
		FSYMBOL(bn_6145_move)(s2, t4);

		/* (t1, t2) = (t2, t1 - (t2 * t3)) */
		FSYMBOL(bn_i6145_mul)(t4, t2, t3);
		FSYMBOL(bn_i6145_sub)(t4, t1, t4);
		FSYMBOL(bn_6145_move)(t1, t2);
		FSYMBOL(bn_6145_move)(t2, t4);
	}

	FSYMBOL(bn_6145_move)(r, a1);
	FSYMBOL(bn_6145_move)(x, s1);
	FSYMBOL(bn_6145_move)(y, t1);
} /* end */

/* @func: bn_i6145_gcd_ext - extended euclidean algorithm
* @param1: bn_6145_t       # greatest common divisor
* @param2: bn_6145_t       # bezout coefficient
* @param3: bn_6145_t       # bezout coefficient
* @param4: const bn_6145_t # number
* @param5: const bn_6145_t # number
* @return: void
*/
void FSYMBOL(bn_i6145_gcd_ext)(bn_6145_t r, bn_6145_t x, bn_6145_t y,
		const bn_6145_t a, const bn_6145_t b) {
	bn_6145_t _b, _x, _y, quo, rem;

	if (!FSYMBOL(bn_6145_bits)(b)) {
		FSYMBOL(bn_6145_set1)(x, 1);
		FSYMBOL(bn_6145_set1)(y, 0);
		FSYMBOL(bn_6145_move)(r, a);
		return;
	}

	/* gcd_ext(b, a % b) */
	FSYMBOL(bn_6145_move)(_b, b);
	FSYMBOL(bn_i6145_divmod)(quo, rem, a, b);
	FSYMBOL(bn_i6145_gcd_ext)(r, _x, _y, _b, rem);

	/* x = _y */
	FSYMBOL(bn_6145_move)(x, _y);

	/* y = _x - ((a / b) * _y) */
	FSYMBOL(bn_i6145_divmod)(quo, rem, a, b);
	FSYMBOL(bn_i6145_mul)(quo, quo, _y);
	FSYMBOL(bn_i6145_sub)(y, _x, quo);
} /* end */

/* @func: bn_i6145_inv - modular inverse
* @param1: bn_6145_t       # inverse modulus
* @param2: const bn_6145_t # number
* @param3: const bn_6145_t # modulus
* @return: int32           # 0: success, -1: fail
*/
int32 FSYMBOL(bn_i6145_inv)(bn_6145_t r,
		const bn_6145_t a, const bn_6145_t b) {
	bn_6145_t g, x, y;

	FSYMBOL(bn_i6145_gcd)(g, x, y, a, b);
	if (FSYMBOL(bn_i6145_cmp1)(g, 1)) {
		FSYMBOL(bn_6145_init)(r);
		return -1;
	}

	/* r = x % b */
	/* FSYMBOL(bn_i6145_divmod)(y, r, x, b); */

	/* r = ((x % b) + b) % b */
	FSYMBOL(bn_i6145_divmod)(y, x, x, b);
	FSYMBOL(bn_i6145_add)(x, x, b);
	FSYMBOL(bn_i6145_divmod)(y, r, x, b);

	return 0;
} /* end */
