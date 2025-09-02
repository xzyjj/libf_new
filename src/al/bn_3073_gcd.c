/* bn_3073_gcd.c - big number calculation (3073-bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_3073.h>


/* @func: bn_i3073_gcd - extended euclidean algorithm (non-recursive)
* @param1: bn_3073_t       # greatest common divisor
* @param2: bn_3073_t       # bezout coefficient
* @param3: bn_3073_t       # bezout coefficient
* @param4: const bn_3073_t # number
* @param5: const bn_3073_t # number
* @return: void
*/
void FSYMBOL(bn_i3073_gcd)(bn_3073_t r, bn_3073_t x, bn_3073_t y,
		const bn_3073_t a, const bn_3073_t b) {
	bn_3073_t a1, b1, s1, s2, t1, t2, t3, t4;
	FSYMBOL(bn_3073_move)(a1, a);
	FSYMBOL(bn_3073_move)(b1, b);
	FSYMBOL(bn_3073_set1)(s1, 1);
	FSYMBOL(bn_3073_set1)(s2, 0);
	FSYMBOL(bn_3073_set1)(t1, 0);
	FSYMBOL(bn_3073_set1)(t2, 1);

	while (FSYMBOL(bn_3073_bits)(b1)) {
		/* t3 = a1 % b1 */
		FSYMBOL(bn_i3073_divmod)(t3, r, a1, b1);

		/* (a1, b1) = (b1, a1 - (b1 * t3)) */
		FSYMBOL(bn_i3073_mul)(t4, b1, t3);
		FSYMBOL(bn_i3073_sub)(t4, a1, t4);
		FSYMBOL(bn_3073_move)(a1, b1);
		FSYMBOL(bn_3073_move)(b1, t4);

		/* (s1, s2) = (s2, s1 - (s2 * t3)) */
		FSYMBOL(bn_i3073_mul)(t4, s2, t3);
		FSYMBOL(bn_i3073_sub)(t4, s1, t4);
		FSYMBOL(bn_3073_move)(s1, s2);
		FSYMBOL(bn_3073_move)(s2, t4);

		/* (t1, t2) = (t2, t1 - (t2 * t3)) */
		FSYMBOL(bn_i3073_mul)(t4, t2, t3);
		FSYMBOL(bn_i3073_sub)(t4, t1, t4);
		FSYMBOL(bn_3073_move)(t1, t2);
		FSYMBOL(bn_3073_move)(t2, t4);
	}

	FSYMBOL(bn_3073_move)(r, a1);
	FSYMBOL(bn_3073_move)(x, s1);
	FSYMBOL(bn_3073_move)(y, t1);
} /* end */

/* @func: bn_i3073_gcd_ext - extended euclidean algorithm
* @param1: bn_3073_t       # greatest common divisor
* @param2: bn_3073_t       # bezout coefficient
* @param3: bn_3073_t       # bezout coefficient
* @param4: const bn_3073_t # number
* @param5: const bn_3073_t # number
* @return: void
*/
void FSYMBOL(bn_i3073_gcd_ext)(bn_3073_t r, bn_3073_t x, bn_3073_t y,
		const bn_3073_t a, const bn_3073_t b) {
	bn_3073_t _b, _x, _y, quo, rem;

	if (!FSYMBOL(bn_3073_bits)(b)) {
		FSYMBOL(bn_3073_set1)(x, 1);
		FSYMBOL(bn_3073_set1)(y, 0);
		FSYMBOL(bn_3073_move)(r, a);
		return;
	}

	/* gcd_ext(b, a % b) */
	FSYMBOL(bn_3073_move)(_b, b);
	FSYMBOL(bn_i3073_divmod)(quo, rem, a, b);
	FSYMBOL(bn_i3073_gcd_ext)(r, _x, _y, _b, rem);

	/* x = _y */
	FSYMBOL(bn_3073_move)(x, _y);

	/* y = _x - ((a / b) * _y) */
	FSYMBOL(bn_i3073_divmod)(quo, rem, a, b);
	FSYMBOL(bn_i3073_mul)(quo, quo, _y);
	FSYMBOL(bn_i3073_sub)(y, _x, quo);
} /* end */

/* @func: bn_i3073_inv - modular inverse
* @param1: bn_3073_t       # inverse modulus
* @param2: const bn_3073_t # number
* @param3: const bn_3073_t # modulus
* @return: int32           # 0: success, -1: fail
*/
int32 FSYMBOL(bn_i3073_inv)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b) {
	bn_3073_t g, x, y;

	FSYMBOL(bn_i3073_gcd)(g, x, y, a, b);
	if (FSYMBOL(bn_i3073_cmp1)(g, 1)) {
		FSYMBOL(bn_3073_init)(r);
		return -1;
	}

	/* r = x % b */
	/* FSYMBOL(bn_i3073_divmod)(y, r, x, b); */

	/* r = ((x % b) + b) % b */
	FSYMBOL(bn_i3073_divmod)(y, x, x, b);
	FSYMBOL(bn_i3073_add)(x, x, b);
	FSYMBOL(bn_i3073_divmod)(y, r, x, b);

	return 0;
} /* end */
