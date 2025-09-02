/* bn_4097_uint.c - big number calculation (4097-bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_4097.h>


/* @func: bn_u4097_cmp - big number compare
* @param1: const bn_4097_t # number a
* @param2: const bn_4097_t # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_u4097_cmp)(const bn_4097_t a, const bn_4097_t b) {
	for (int32 i = BN_4097_SIZE - 1; i >= 0; i--) {
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}

	return 0;
} /* end */

/* @func: bn_u4097_cmp1 - big number compare
* @param1: const bn_4097_t # number a
* @param2: uint32          # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_u4097_cmp1)(const bn_4097_t a, uint32 b) {
	for (int32 i = BN_4097_SIZE - 1; i >= 0; i--) {
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

/* @func: bn_u4097_add - big number addition
* @param1: bn_4097_t       # sum
* @param2: const bn_4097_t # addend
* @param3: const bn_4097_t # addend
* @return: void
*/
void FSYMBOL(bn_u4097_add)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b) {
	bn_4097_t rr;

	if (!FSYMBOL(bn_4097_bits)(a)) {
		FSYMBOL(bn_4097_move)(r, b);
		return;
	}
	if (!FSYMBOL(bn_4097_bits)(b)) {
		FSYMBOL(bn_4097_move)(r, a);
		return;
	}

	FSYMBOL(bn_4097_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_4097_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] + b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_4097_move)(r, rr);
} /* end */

/* @func: bn_u4097_sub - big number subtraction
* @param1: bn_4097_t       # difference
* @param2: const bn_4097_t # minuend
* @param3: const bn_4097_t # subtract
* @return: void
*/
void FSYMBOL(bn_u4097_sub)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b) {
	bn_4097_t rr;

	if (!FSYMBOL(bn_4097_bits)(a)) {
		FSYMBOL(bn_4097_move)(r, b);
		return;
	}
	if (!FSYMBOL(bn_4097_bits)(b)) {
		FSYMBOL(bn_4097_move)(r, a);
		return;
	}

	FSYMBOL(bn_4097_init)(rr);

	int32 borrow = 0;
	for (int32 i = 0; i < BN_4097_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] - b[i] - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
	}

	FSYMBOL(bn_4097_move)(r, rr);
} /* end */

/* @func: bn_u4097_mul - big number multiplication
* @param1: bn_4097_t       # product
* @param2: const bn_4097_t # multiplicand
* @param3: const bn_4097_t # multiplier
* @return: void
*/
void FSYMBOL(bn_u4097_mul)(bn_4097_t r,
		const bn_4097_t a, const bn_4097_t b) {
	bn_4097_t rr;
	int32 asize, bsize;

	asize = FSYMBOL(bn_4097_bits)(a);
	bsize = FSYMBOL(bn_4097_bits)(b);
	if (!asize || !bsize) {
		FSYMBOL(bn_4097_init)(r);
		return;
	}

	FSYMBOL(bn_4097_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_4097_SIZE; i++) {
		carry = 0;
		for (int32 j = 0; j < BN_4097_SIZE; j++) {
			if ((i + j) >= BN_4097_SIZE)
				break;
			uint64L tmp = (uint64L)a[i] * b[j] + rr[i + j]
				+ carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
	}

	FSYMBOL(bn_4097_move)(r, rr);
} /* end */

/* @func: bn_u4097_div - big number division
* @param1: bn_4097_t       # quotient
* @param2: bn_4097_t       # remainder
* @param3: const bn_4097_t # dividend
* @param4: const bn_4097_t # divisor
* @return: void
*/
void FSYMBOL(bn_u4097_div)(bn_4097_t quo, bn_4097_t rem,
		const bn_4097_t a, const bn_4097_t b) {
	bn_4097_t rquo, rrem, sh_b;
	FSYMBOL(bn_4097_move)(rrem, a);
	FSYMBOL(bn_4097_move)(sh_b, b);
	FSYMBOL(bn_4097_move)(rem, rrem);
	FSYMBOL(bn_4097_init)(quo);

	if (FSYMBOL(bn_u4097_cmp)(rrem, sh_b) < 0)
		return;
	if (!(FSYMBOL(bn_4097_bits)(rrem)
			&& FSYMBOL(bn_4097_bits)(sh_b))) {
		FSYMBOL(bn_4097_init)(rem);
		return;
	}

	uint32 sh = FSYMBOL(bn_4097_bits)(rrem)
		- FSYMBOL(bn_4097_bits)(sh_b);

	FSYMBOL(bn_4097_lsh)(sh_b, sh);
	while (FSYMBOL(bn_u4097_cmp)(rrem, sh_b) >= 0) {
		FSYMBOL(bn_4097_lsh)(sh_b, 1);
		sh++;
	}

	FSYMBOL(bn_4097_init)(rquo);
	for (; sh > 0; sh--) {
		FSYMBOL(bn_4097_rsh)(sh_b, 1);
		FSYMBOL(bn_4097_lsh)(rquo, 1);
		if (FSYMBOL(bn_u4097_cmp)(rrem, sh_b) >= 0) {
			FSYMBOL(bn_u4097_sub)(rrem, rrem, sh_b);
			rquo[0] |= 1;
		}
	}

	FSYMBOL(bn_4097_move)(quo, rquo);
	FSYMBOL(bn_4097_move)(rem, rrem);
} /* end */

/* @func: bn_u4097_add1 - big number addition
* @param1: bn_4097_t       # sum
* @param2: const bn_4097_t # addend
* @param3: uint32          # addend
* @return: void
*/
void FSYMBOL(bn_u4097_add1)(bn_4097_t r,
		const bn_4097_t a, uint32 b) {
	bn_4097_t rr;

	if (!b) {
		FSYMBOL(bn_4097_move)(r, a);
		return;
	}
	if (!FSYMBOL(bn_4097_bits)(a)) {
		FSYMBOL(bn_4097_set1)(r, b);
		return;
	}

	FSYMBOL(bn_4097_init)(rr);

	uint32 carry = b;
	for (int32 i = 0; i < BN_4097_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_4097_move)(r, rr);
} /* end */

/* @func: bn_u4097_sub1 - big number subtraction
* @param1: bn_4097_t       # difference
* @param2: const bn_4097_t # minuend
* @param3: uint32          # subtract
* @return: void
*/
void FSYMBOL(bn_u4097_sub1)(bn_4097_t r,
		const bn_4097_t a, uint32 b) {
	bn_4097_t rr;

	if (!b) {
		FSYMBOL(bn_4097_move)(r, a);
		return;
	}
	if (!FSYMBOL(bn_4097_bits)(a)) {
		FSYMBOL(bn_4097_set1)(r, b);
		return;
	}

	FSYMBOL(bn_4097_init)(rr);

	int32 borrow = 0;
	for (int32 i = 0; i < BN_4097_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] - b - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
		b = 0;
	}

	FSYMBOL(bn_4097_move)(r, rr);
} /* end */

/* @func: bn_u4097_mul1 - big number multiplication
* @param1: bn_4097_t       # product
* @param2: const bn_4097_t # multiplicand
* @param3: uint32          # multiplier
* @return: void
*/
void FSYMBOL(bn_u4097_mul1)(bn_4097_t r,
		const bn_4097_t a, uint32 b) {
	bn_4097_t rr;

	if (!b || !FSYMBOL(bn_4097_bits)(a)) {
		FSYMBOL(bn_4097_init)(r);
		return;
	}

	FSYMBOL(bn_4097_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_4097_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] * b + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_4097_move)(r, rr);
} /* end */

/* @func: bn_u4097_div1 - big number division
* @param1: bn_4097_t       # quotient
* @param2: bn_4097_t       # remainder
* @param3: const bn_4097_t # dividend
* @param4: uint32          # divisor
* @return: void
*/
void FSYMBOL(bn_u4097_div1)(bn_4097_t quo, bn_4097_t rem,
		const bn_4097_t a, uint32 b) {
	bn_4097_t tmp;
	FSYMBOL(bn_4097_set1)(tmp, b);

	FSYMBOL(bn_u4097_div)(quo, rem, a, tmp);
} /* end */
