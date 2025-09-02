/* bn_1025_uint.c - big number calculation (1025-bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_1025.h>


/* @func: bn_u1025_cmp - big number compare
* @param1: const bn_1025_t # number a
* @param2: const bn_1025_t # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_u1025_cmp)(const bn_1025_t a, const bn_1025_t b) {
	for (int32 i = BN_1025_SIZE - 1; i >= 0; i--) {
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}

	return 0;
} /* end */

/* @func: bn_u1025_cmp1 - big number compare
* @param1: const bn_1025_t # number a
* @param2: uint32          # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_u1025_cmp1)(const bn_1025_t a, uint32 b) {
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

/* @func: bn_u1025_add - big number addition
* @param1: bn_1025_t       # sum
* @param2: const bn_1025_t # addend
* @param3: const bn_1025_t # addend
* @return: void
*/
void FSYMBOL(bn_u1025_add)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b) {
	bn_1025_t rr;

	if (!FSYMBOL(bn_1025_bits)(a)) {
		FSYMBOL(bn_1025_move)(r, b);
		return;
	}
	if (!FSYMBOL(bn_1025_bits)(b)) {
		FSYMBOL(bn_1025_move)(r, a);
		return;
	}

	FSYMBOL(bn_1025_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_1025_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] + b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_1025_move)(r, rr);
} /* end */

/* @func: bn_u1025_sub - big number subtraction
* @param1: bn_1025_t       # difference
* @param2: const bn_1025_t # minuend
* @param3: const bn_1025_t # subtract
* @return: void
*/
void FSYMBOL(bn_u1025_sub)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b) {
	bn_1025_t rr;

	if (!FSYMBOL(bn_1025_bits)(a)) {
		FSYMBOL(bn_1025_move)(r, b);
		return;
	}
	if (!FSYMBOL(bn_1025_bits)(b)) {
		FSYMBOL(bn_1025_move)(r, a);
		return;
	}

	FSYMBOL(bn_1025_init)(rr);

	int32 borrow = 0;
	for (int32 i = 0; i < BN_1025_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] - b[i] - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
	}

	FSYMBOL(bn_1025_move)(r, rr);
} /* end */

/* @func: bn_u1025_mul - big number multiplication
* @param1: bn_1025_t       # product
* @param2: const bn_1025_t # multiplicand
* @param3: const bn_1025_t # multiplier
* @return: void
*/
void FSYMBOL(bn_u1025_mul)(bn_1025_t r,
		const bn_1025_t a, const bn_1025_t b) {
	bn_1025_t rr;
	int32 asize, bsize;

	asize = FSYMBOL(bn_1025_bits)(a);
	bsize = FSYMBOL(bn_1025_bits)(b);
	if (!asize || !bsize) {
		FSYMBOL(bn_1025_init)(r);
		return;
	}

	FSYMBOL(bn_1025_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_1025_SIZE; i++) {
		carry = 0;
		for (int32 j = 0; j < BN_1025_SIZE; j++) {
			if ((i + j) >= BN_1025_SIZE)
				break;
			uint64L tmp = (uint64L)a[i] * b[j] + rr[i + j]
				+ carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
	}

	FSYMBOL(bn_1025_move)(r, rr);
} /* end */

/* @func: bn_u1025_div - big number division
* @param1: bn_1025_t       # quotient
* @param2: bn_1025_t       # remainder
* @param3: const bn_1025_t # dividend
* @param4: const bn_1025_t # divisor
* @return: void
*/
void FSYMBOL(bn_u1025_div)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, const bn_1025_t b) {
	bn_1025_t rquo, rrem, sh_b;
	FSYMBOL(bn_1025_move)(rrem, a);
	FSYMBOL(bn_1025_move)(sh_b, b);
	FSYMBOL(bn_1025_move)(rem, rrem);
	FSYMBOL(bn_1025_init)(quo);

	if (FSYMBOL(bn_u1025_cmp)(rrem, sh_b) < 0)
		return;
	if (!(FSYMBOL(bn_1025_bits)(rrem)
			&& FSYMBOL(bn_1025_bits)(sh_b))) {
		FSYMBOL(bn_1025_init)(rem);
		return;
	}

	uint32 sh = FSYMBOL(bn_1025_bits)(rrem)
		- FSYMBOL(bn_1025_bits)(sh_b);

	FSYMBOL(bn_1025_lsh)(sh_b, sh);
	while (FSYMBOL(bn_u1025_cmp)(rrem, sh_b) >= 0) {
		FSYMBOL(bn_1025_lsh)(sh_b, 1);
		sh++;
	}

	FSYMBOL(bn_1025_init)(rquo);
	for (; sh > 0; sh--) {
		FSYMBOL(bn_1025_rsh)(sh_b, 1);
		FSYMBOL(bn_1025_lsh)(rquo, 1);
		if (FSYMBOL(bn_u1025_cmp)(rrem, sh_b) >= 0) {
			FSYMBOL(bn_u1025_sub)(rrem, rrem, sh_b);
			rquo[0] |= 1;
		}
	}

	FSYMBOL(bn_1025_move)(quo, rquo);
	FSYMBOL(bn_1025_move)(rem, rrem);
} /* end */

/* @func: bn_u1025_add1 - big number addition
* @param1: bn_1025_t       # sum
* @param2: const bn_1025_t # addend
* @param3: uint32          # addend
* @return: void
*/
void FSYMBOL(bn_u1025_add1)(bn_1025_t r,
		const bn_1025_t a, uint32 b) {
	bn_1025_t rr;

	if (!b) {
		FSYMBOL(bn_1025_move)(r, a);
		return;
	}
	if (!FSYMBOL(bn_1025_bits)(a)) {
		FSYMBOL(bn_1025_set1)(r, b);
		return;
	}

	FSYMBOL(bn_1025_init)(rr);

	uint32 carry = b;
	for (int32 i = 0; i < BN_1025_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_1025_move)(r, rr);
} /* end */

/* @func: bn_u1025_sub1 - big number subtraction
* @param1: bn_1025_t       # difference
* @param2: const bn_1025_t # minuend
* @param3: uint32          # subtract
* @return: void
*/
void FSYMBOL(bn_u1025_sub1)(bn_1025_t r,
		const bn_1025_t a, uint32 b) {
	bn_1025_t rr;

	if (!b) {
		FSYMBOL(bn_1025_move)(r, a);
		return;
	}
	if (!FSYMBOL(bn_1025_bits)(a)) {
		FSYMBOL(bn_1025_set1)(r, b);
		return;
	}

	FSYMBOL(bn_1025_init)(rr);

	int32 borrow = 0;
	for (int32 i = 0; i < BN_1025_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] - b - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
		b = 0;
	}

	FSYMBOL(bn_1025_move)(r, rr);
} /* end */

/* @func: bn_u1025_mul1 - big number multiplication
* @param1: bn_1025_t       # product
* @param2: const bn_1025_t # multiplicand
* @param3: uint32          # multiplier
* @return: void
*/
void FSYMBOL(bn_u1025_mul1)(bn_1025_t r,
		const bn_1025_t a, uint32 b) {
	bn_1025_t rr;

	if (!b || !FSYMBOL(bn_1025_bits)(a)) {
		FSYMBOL(bn_1025_init)(r);
		return;
	}

	FSYMBOL(bn_1025_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_1025_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] * b + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_1025_move)(r, rr);
} /* end */

/* @func: bn_u1025_div1 - big number division
* @param1: bn_1025_t       # quotient
* @param2: bn_1025_t       # remainder
* @param3: const bn_1025_t # dividend
* @param4: uint32          # divisor
* @return: void
*/
void FSYMBOL(bn_u1025_div1)(bn_1025_t quo, bn_1025_t rem,
		const bn_1025_t a, uint32 b) {
	bn_1025_t tmp;
	FSYMBOL(bn_1025_set1)(tmp, b);

	FSYMBOL(bn_u1025_div)(quo, rem, a, tmp);
} /* end */
