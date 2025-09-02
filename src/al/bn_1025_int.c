/* bn_1025_int.c - big number calculation (1025-bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_1025.h>


/* @func: bn_i1025_cmp - big number compare
* @param1: const bn_1025_t # number a
* @param2: const bn_1025_t # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_i1025_cmp)(const bn_1025_t a, const bn_1025_t b) {
	if (a[BN_1025_SIG] < b[BN_1025_SIG])
		return 1;
	if (a[BN_1025_SIG] > b[BN_1025_SIG])
		return -1;

	for (int32 i = BN_1025_SIZE - 1; i >= 0; i--) {
		if (a[i] < b[i])
			return a[BN_1025_SIG] ? 1 : -1;
		if (a[i] > b[i])
			return a[BN_1025_SIG] ? -1 : 1;
	}

	return 0;
} /* end */

/* @func: bn_u1025_cmp1 - big number compare
* @param1: const bn_1025_t # number a
* @param2: uint32          # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_i1025_cmp1)(const bn_1025_t a, uint32 b) {
	if (a[BN_1025_SIG])
		return -1;

	for (int32 i = BN_1025_SIZE - 1; i >= 0; i--) {
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

/* @func: bn_i1025_add - big number addition
* @param1: bn_1025_t       # sum
* @param2: const bn_1025_t # addend
* @param3: const bn_1025_t # addend
* @return: void
*/
void FSYMBOL(bn_i1025_add)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b) {
	uint32 neg = a[BN_1025_SIG];

	if (neg == b[BN_1025_SIG]) {
		FSYMBOL(bn_u1025_add)(r, a, b);
		r[BN_1025_SIG] = neg;
		return;
	}

	int32 k = FSYMBOL(bn_u1025_cmp)(a, b);
	if (k > 0) {
		FSYMBOL(bn_u1025_sub)(r, a, b);
		r[BN_1025_SIG] = neg;
	} else if (k < 0) {
		neg = b[BN_1025_SIG];
		FSYMBOL(bn_u1025_sub)(r, b, a);
		r[BN_1025_SIG] = neg;
	} else {
		FSYMBOL(bn_1025_init)(r);
	}
} /* end */

/* @func: bn_i1025_sub - big number subtraction
* @param1: bn_1025_t       # difference
* @param2: const bn_1025_t # minuend
* @param3: const bn_1025_t # subtract
* @return: void
*/
void FSYMBOL(bn_i1025_sub)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b) {
	bn_1025_t neg;
	FSYMBOL(bn_1025_move)(neg, b);
	neg[BN_1025_SIG] = !neg[BN_1025_SIG];

	FSYMBOL(bn_i1025_add)(r, a, neg);
} /* end */

/* @func: bn_i1025_mul - big number multiplication
* @param1: bn_1025_t       # product
* @param2: const bn_1025_t # multiplicand
* @param3: const bn_1025_t # multiplier
* @return: void
*/
void FSYMBOL(bn_i1025_mul)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b) {
	uint32 neg = a[BN_1025_SIG] ^ b[BN_1025_SIG];

	FSYMBOL(bn_u1025_mul)(r, a, b);
	r[BN_1025_SIG] = neg;

	if (!FSYMBOL(bn_1025_bits)(r))
		r[BN_1025_SIG] = 0;
} /* end */

/* @func: bn_i1025_div - big number division
* @param1: bn_1025_t       # quotient
* @param2: bn_1025_t       # remainder
* @param3: const bn_1025_t # dividend
* @param4: const bn_1025_t # divisor
* @return: void
*/
void FSYMBOL(bn_i1025_div)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, const bn_1025_t b) {
	uint32 neg1 = a[BN_1025_SIG], neg2 = b[BN_1025_SIG];

	FSYMBOL(bn_u1025_div)(quo, rem, a, b);
	quo[BN_1025_SIG] = neg1 ^ neg2;
	rem[BN_1025_SIG] = neg1;

	if (!FSYMBOL(bn_1025_bits)(quo))
		quo[BN_1025_SIG] = 0;
	if (!FSYMBOL(bn_1025_bits)(rem))
		rem[BN_1025_SIG] = 0;
} /* end */

/* @func: bn_i1025_divmod - big number division (mathematical definition)
* @param1: bn_1025_t       # quotient
* @param2: bn_1025_t       # remainder
* @param3: const bn_1025_t # dividend
* @param4: const bn_1025_t # divisor
* @return: void
*/
void FSYMBOL(bn_i1025_divmod)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, const bn_1025_t b) {
	bn_1025_t neg;
	FSYMBOL(bn_1025_move)(neg, b);

	FSYMBOL(bn_i1025_div)(quo, rem, a, b);

	if (rem[BN_1025_SIG]) {
		FSYMBOL(bn_i1025_add)(rem, rem, neg);
		rem[BN_1025_SIG] = 0;
	}
} /* end */

/* @func: bn_i1025_mul1 - big number multiplication
* @param1: bn_1025_t       # product
* @param2: const bn_1025_t # multiplicand
* @param3: uint32          # multiplier
* @return: void
*/
void FSYMBOL(bn_i1025_mul1)(bn_1025_t r,
		const bn_1025_t a, uint32 b) {
	uint32 neg = a[BN_1025_SIG];

	FSYMBOL(bn_u1025_mul1)(r, a, b);
	r[BN_1025_SIG] = neg;

	if (!FSYMBOL(bn_1025_bits)(r))
		r[BN_1025_SIG] = 0;
} /* end */

/* @func: bn_i1025_add1 - big number addition
* @param1: bn_1025_t       # sum
* @param2: const bn_1025_t # addend
* @param3: uint32          # addend
* @return: void
*/
void FSYMBOL(bn_i1025_add1)(bn_1025_t r,
		const bn_1025_t a, uint32 b) {
	uint32 neg = a[BN_1025_SIG];

	if (!neg) {
		FSYMBOL(bn_u1025_add1)(r, a, b);
		r[BN_1025_SIG] = neg;
		return;
	}

	int32 k = FSYMBOL(bn_u1025_cmp1)(a, b);
	if (k > 0) {
		FSYMBOL(bn_u1025_sub1)(r, a, b);
		r[BN_1025_SIG] = neg;
	} else if (k < 0) {
		FSYMBOL(bn_1025_set1)(r, b - a[0]);
	} else {
		FSYMBOL(bn_1025_init)(r);
	}
} /* end */

/* @func: bn_i1025_sub1 - big number subtraction
* @param1: bn_1025_t       # difference
* @param2: const bn_1025_t # minuend
* @param3: uint32          # subtract
* @return: void
*/
void FSYMBOL(bn_i1025_sub1)(bn_1025_t r,
		const bn_1025_t a, uint32 b) {
	bn_1025_t neg;
	FSYMBOL(bn_1025_move)(neg, a);
	neg[BN_1025_SIG] = !neg[BN_1025_SIG];

	FSYMBOL(bn_i1025_add1)(r, neg, b);
	r[BN_1025_SIG] = a[BN_1025_SIG];
} /* end */

/* @func: bn_i1025_div1 - big number division
* @param1: bn_1025_t       # quotient
* @param2: bn_1025_t       # remainder
* @param3: const bn_1025_t # dividend
* @param4: uint32          # divisor
* @return: void
*/
void FSYMBOL(bn_i1025_div1)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, uint32 b) {
	bn_1025_t tmp;
	FSYMBOL(bn_1025_set1)(tmp, b);

	FSYMBOL(bn_i1025_div)(quo, rem, a, tmp);
} /* end */

/* @func: bn_i1025_divmod1 - big number division (mathematical definition)
* @param1: bn_1025_t       # quotient
* @param2: bn_1025_t       # remainder
* @param3: const bn_1025_t # dividend
* @param4: uint32          # divisor
* @return: void
*/
void FSYMBOL(bn_i1025_divmod1)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, uint32 b) {
	bn_1025_t tmp;
	FSYMBOL(bn_1025_set1)(tmp, b);

	FSYMBOL(bn_i1025_divmod)(quo, rem, a, tmp);
} /* end */

/* @func: bn_i1025_mod_pow - fast modular exponentiation
* @param1: bn_1025_t       # result
* @param2: const bn_1025_t # number
* @param3: const bn_1025_t # exponential
* @param4: const bn_1025_t # modules
* @return: int32           # 0: success, -1: fail
*/
void FSYMBOL(bn_i1025_mod_pow)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b,
		const bn_1025_t m) {
	bn_1025_t rr, quo, a1, b1;
	FSYMBOL(bn_1025_set1)(rr, 1);
	FSYMBOL(bn_1025_move)(b1, b);

	/* a1 = a % m */
	FSYMBOL(bn_i1025_divmod)(quo, a1, a, m);
	while (FSYMBOL(bn_1025_bits)(b1)) {
		if (b1[0] & 1) {
			/* rr = (rr * a1) % m */
			FSYMBOL(bn_i1025_mul)(rr, rr, a1);
			FSYMBOL(bn_i1025_divmod)(quo, rr, rr, m);
		}

		/* a1 = (a1 * a1) % m */
		FSYMBOL(bn_i1025_mul)(a1, a1, a1);
		FSYMBOL(bn_i1025_divmod)(quo, a1, a1, m);

		/* b1 >>= 1 */
		FSYMBOL(bn_1025_rsh)(b1, 1);
	}

	FSYMBOL(bn_1025_move)(r, rr);
} /* end */
