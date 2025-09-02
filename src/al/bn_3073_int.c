/* bn_3073_int.c - big number calculation (3073-bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_3073.h>


/* @func: bn_i3073_cmp - big number compare
* @param1: const bn_3073_t # number a
* @param2: const bn_3073_t # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_i3073_cmp)(const bn_3073_t a, const bn_3073_t b) {
	if (a[BN_3073_SIG] < b[BN_3073_SIG])
		return 1;
	if (a[BN_3073_SIG] > b[BN_3073_SIG])
		return -1;

	for (int32 i = BN_3073_SIZE - 1; i >= 0; i--) {
		if (a[i] < b[i])
			return a[BN_3073_SIG] ? 1 : -1;
		if (a[i] > b[i])
			return a[BN_3073_SIG] ? -1 : 1;
	}

	return 0;
} /* end */

/* @func: bn_u3073_cmp1 - big number compare
* @param1: const bn_3073_t # number a
* @param2: uint32          # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_i3073_cmp1)(const bn_3073_t a, uint32 b) {
	if (a[BN_3073_SIG])
		return -1;

	for (int32 i = BN_3073_SIZE - 1; i >= 0; i--) {
		if (i) {
			if (a[i])
				return 1;
		} else {
			if (a[i] > b)
				return 1;
			if (a[i] < b)
				return -1;
		}
	}

	return 0;
} /* end */

/* @func: bn_i3073_add - big number addition
* @param1: bn_3073_t       # sum
* @param2: const bn_3073_t # addend
* @param3: const bn_3073_t # addend
* @return: void
*/
void FSYMBOL(bn_i3073_add)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b) {
	uint32 neg = a[BN_3073_SIG];

	if (neg == b[BN_3073_SIG]) {
		FSYMBOL(bn_u3073_add)(r, a, b);
		r[BN_3073_SIG] = neg;
		return;
	}

	int32 k = FSYMBOL(bn_u3073_cmp)(a, b);
	if (k > 0) {
		FSYMBOL(bn_u3073_sub)(r, a, b);
		r[BN_3073_SIG] = neg;
	} else if (k < 0) {
		neg = b[BN_3073_SIG];
		FSYMBOL(bn_u3073_sub)(r, b, a);
		r[BN_3073_SIG] = neg;
	} else {
		FSYMBOL(bn_3073_init)(r);
	}
} /* end */

/* @func: bn_i3073_sub - big number subtraction
* @param1: bn_3073_t       # difference
* @param2: const bn_3073_t # minuend
* @param3: const bn_3073_t # subtract
* @return: void
*/
void FSYMBOL(bn_i3073_sub)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b) {
	bn_3073_t neg;
	FSYMBOL(bn_3073_move)(neg, b);
	neg[BN_3073_SIG] = !neg[BN_3073_SIG];

	FSYMBOL(bn_i3073_add)(r, a, neg);
} /* end */

/* @func: bn_i3073_mul - big number multiplication
* @param1: bn_3073_t       # product
* @param2: const bn_3073_t # multiplicand
* @param3: const bn_3073_t # multiplier
* @return: void
*/
void FSYMBOL(bn_i3073_mul)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b) {
	uint32 neg = a[BN_3073_SIG] ^ b[BN_3073_SIG];

	FSYMBOL(bn_u3073_mul)(r, a, b);
	r[BN_3073_SIG] = neg;

	if (!FSYMBOL(bn_3073_bits)(r))
		r[BN_3073_SIG] = 0;
} /* end */

/* @func: bn_i3073_div - big number division
* @param1: bn_3073_t       # quotient
* @param2: bn_3073_t       # remainder
* @param3: const bn_3073_t # dividend
* @param4: const bn_3073_t # divisor
* @return: void
*/
void FSYMBOL(bn_i3073_div)(bn_3073_t quo, bn_3073_t rem,
		const bn_3073_t a, const bn_3073_t b) {
	uint32 neg1 = a[BN_3073_SIG], neg2 = b[BN_3073_SIG];

	FSYMBOL(bn_u3073_div)(quo, rem, a, b);
	quo[BN_3073_SIG] = neg1 ^ neg2;
	rem[BN_3073_SIG] = neg1;

	if (!FSYMBOL(bn_3073_bits)(quo))
		quo[BN_3073_SIG] = 0;
	if (!FSYMBOL(bn_3073_bits)(rem))
		rem[BN_3073_SIG] = 0;
} /* end */

/* @func: bn_i3073_divmod - big number division (mathematical definition)
* @param1: bn_3073_t       # quotient
* @param2: bn_3073_t       # remainder
* @param3: const bn_3073_t # dividend
* @param4: const bn_3073_t # divisor
* @return: void
*/
void FSYMBOL(bn_i3073_divmod)(bn_3073_t quo, bn_3073_t rem,
		const bn_3073_t a, const bn_3073_t b) {
	bn_3073_t neg;
	FSYMBOL(bn_3073_move)(neg, b);

	FSYMBOL(bn_i3073_div)(quo, rem, a, b);

	if (rem[BN_3073_SIG]) {
		FSYMBOL(bn_i3073_add)(rem, rem, neg);
		rem[BN_3073_SIG] = 0;
	}
} /* end */

/* @func: bn_i3073_mul1 - big number multiplication
* @param1: bn_3073_t       # product
* @param2: const bn_3073_t # multiplicand
* @param3: uint32          # multiplier
* @return: void
*/
void FSYMBOL(bn_i3073_mul1)(bn_3073_t r,
		const bn_3073_t a, uint32 b) {
	uint32 neg = a[BN_3073_SIG];

	FSYMBOL(bn_u3073_mul1)(r, a, b);
	r[BN_3073_SIG] = neg;

	if (!FSYMBOL(bn_3073_bits)(r))
		r[BN_3073_SIG] = 0;
} /* end */

/* @func: bn_i3073_add1 - big number addition
* @param1: bn_3073_t       # sum
* @param2: const bn_3073_t # addend
* @param3: uint32          # addend
* @return: void
*/
void FSYMBOL(bn_i3073_add1)(bn_3073_t r,
		const bn_3073_t a, uint32 b) {
	uint32 neg = a[BN_3073_SIG];

	if (!neg) {
		FSYMBOL(bn_u3073_add1)(r, a, b);
		r[BN_3073_SIG] = neg;
		return;
	}

	int32 k = FSYMBOL(bn_u3073_cmp1)(a, b);
	if (k > 0) {
		FSYMBOL(bn_u3073_sub1)(r, a, b);
		r[BN_3073_SIG] = neg;
	} else if (k < 0) {
		FSYMBOL(bn_3073_set1)(r, b - a[0]);
	} else {
		FSYMBOL(bn_3073_init)(r);
	}
} /* end */

/* @func: bn_i3073_sub1 - big number subtraction
* @param1: bn_3073_t       # difference
* @param2: const bn_3073_t # minuend
* @param3: uint32          # subtract
* @return: void
*/
void FSYMBOL(bn_i3073_sub1)(bn_3073_t r,
		const bn_3073_t a, uint32 b) {
	bn_3073_t neg;
	FSYMBOL(bn_3073_move)(neg, a);
	neg[BN_3073_SIG] = !neg[BN_3073_SIG];

	FSYMBOL(bn_i3073_add1)(r, neg, b);
	r[BN_3073_SIG] = a[BN_3073_SIG];
} /* end */

/* @func: bn_i3073_div1 - big number division
* @param1: bn_3073_t       # quotient
* @param2: bn_3073_t       # remainder
* @param3: const bn_3073_t # dividend
* @param4: uint32          # divisor
* @return: void
*/
void FSYMBOL(bn_i3073_div1)(bn_3073_t quo, bn_3073_t rem,
		const bn_3073_t a, uint32 b) {
	bn_3073_t tmp;
	FSYMBOL(bn_3073_set1)(tmp, b);

	FSYMBOL(bn_i3073_div)(quo, rem, a, tmp);
} /* end */

/* @func: bn_i3073_divmod1 - big number division (mathematical definition)
* @param1: bn_3073_t       # quotient
* @param2: bn_3073_t       # remainder
* @param3: const bn_3073_t # dividend
* @param4: uint32          # divisor
* @return: void
*/
void FSYMBOL(bn_i3073_divmod1)(bn_3073_t quo, bn_3073_t rem,
		const bn_3073_t a, uint32 b) {
	bn_3073_t tmp;
	FSYMBOL(bn_3073_set1)(tmp, b);

	FSYMBOL(bn_i3073_divmod)(quo, rem, a, tmp);
} /* end */

/* @func: bn_i3073_mod_pow - fast modular exponentiation
* @param1: bn_3073_t       # result
* @param2: const bn_3073_t # number
* @param3: const bn_3073_t # exponential
* @param4: const bn_3073_t # modules
* @return: int32           # 0: success, -1: fail
*/
void FSYMBOL(bn_i3073_mod_pow)(bn_3073_t r,
		const bn_3073_t a, const bn_3073_t b,
		const bn_3073_t m) {
	bn_3073_t rr, quo, a1, b1;
	FSYMBOL(bn_3073_set1)(rr, 1);
	FSYMBOL(bn_3073_move)(b1, b);

	/* a1 = a % m */
	FSYMBOL(bn_i3073_divmod)(quo, a1, a, m);
	while (FSYMBOL(bn_3073_bits)(b1)) {
		if (b1[0] & 1) {
			/* rr = (rr * a1) % m */
			FSYMBOL(bn_i3073_mul)(rr, rr, a1);
			FSYMBOL(bn_i3073_divmod)(quo, rr, rr, m);
		}

		/* a1 = (a1 * a1) % m */
		FSYMBOL(bn_i3073_mul)(a1, a1, a1);
		FSYMBOL(bn_i3073_divmod)(quo, a1, a1, m);

		/* b1 >>= 1 */
		FSYMBOL(bn_3073_rsh)(b1, 1);
	}

	FSYMBOL(bn_3073_move)(r, rr);
} /* end */
