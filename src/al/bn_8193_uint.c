/* bn_8193_uint.c - big number calculation (8193-bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_8193.h>


/* @func: bn_u8193_cmp - big number compare
* @param1: const bn_8193_t # number a
* @param2: const bn_8193_t # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_u8193_cmp)(const bn_8193_t a, const bn_8193_t b) {
	for (int32 i = BN_8193_SIZE - 1; i >= 0; i--) {
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}

	return 0;
} /* end */

/* @func: bn_u8193_cmp1 - big number compare
* @param1: const bn_8193_t # number a
* @param2: uint32          # number b
* @return: int32           # 0: a==b, 1: a>b, -1: a<b
*/
int32 FSYMBOL(bn_u8193_cmp1)(const bn_8193_t a, uint32 b) {
	for (int32 i = BN_8193_SIZE - 1; i >= 0; i--) {
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

/* @func: bn_u8193_add - big number addition
* @param1: bn_8193_t       # sum
* @param2: const bn_8193_t # addend
* @param3: const bn_8193_t # addend
* @return: void
*/
void FSYMBOL(bn_u8193_add)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b) {
	bn_8193_t rr;

	if (!FSYMBOL(bn_8193_bits)(a)) {
		FSYMBOL(bn_8193_move)(r, b);
		return;
	}
	if (!FSYMBOL(bn_8193_bits)(b)) {
		FSYMBOL(bn_8193_move)(r, a);
		return;
	}

	FSYMBOL(bn_8193_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_8193_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] + b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_8193_move)(r, rr);
} /* end */

/* @func: bn_u8193_sub - big number subtraction
* @param1: bn_8193_t       # difference
* @param2: const bn_8193_t # minuend
* @param3: const bn_8193_t # subtract
* @return: void
*/
void FSYMBOL(bn_u8193_sub)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b) {
	bn_8193_t rr;

	if (!FSYMBOL(bn_8193_bits)(a)) {
		FSYMBOL(bn_8193_move)(r, b);
		return;
	}
	if (!FSYMBOL(bn_8193_bits)(b)) {
		FSYMBOL(bn_8193_move)(r, a);
		return;
	}

	FSYMBOL(bn_8193_init)(rr);

	int32 borrow = 0;
	for (int32 i = 0; i < BN_8193_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] - b[i] - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
	}

	FSYMBOL(bn_8193_move)(r, rr);
} /* end */

/* @func: bn_u8193_mul - big number multiplication
* @param1: bn_8193_t       # product
* @param2: const bn_8193_t # multiplicand
* @param3: const bn_8193_t # multiplier
* @return: void
*/
void FSYMBOL(bn_u8193_mul)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b) {
	bn_8193_t rr;
	int32 asize, bsize;

	asize = FSYMBOL(bn_8193_bits)(a);
	bsize = FSYMBOL(bn_8193_bits)(b);
	if (!asize || !bsize) {
		FSYMBOL(bn_8193_init)(r);
		return;
	}

	FSYMBOL(bn_8193_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_8193_SIZE; i++) {
		carry = 0;
		for (int32 j = 0; j < BN_8193_SIZE; j++) {
			if ((i + j) >= BN_8193_SIZE)
				break;
			uint64L tmp = (uint64L)a[i] * b[j] + rr[i + j]
				+ carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
	}

	FSYMBOL(bn_8193_move)(r, rr);
} /* end */

/* @func: bn_u8193_div - big number division
* @param1: bn_8193_t       # quotient
* @param2: bn_8193_t       # remainder
* @param3: const bn_8193_t # dividend
* @param4: const bn_8193_t # divisor
* @return: void
*/
void FSYMBOL(bn_u8193_div)(bn_8193_t quo, bn_8193_t rem,
		const bn_8193_t a, const bn_8193_t b) {
	bn_8193_t rquo, rrem, sh_b;
	FSYMBOL(bn_8193_move)(rrem, a);
	FSYMBOL(bn_8193_move)(sh_b, b);
	FSYMBOL(bn_8193_move)(rem, rrem);
	FSYMBOL(bn_8193_init)(quo);

	if (FSYMBOL(bn_u8193_cmp)(rrem, sh_b) < 0)
		return;
	if (!(FSYMBOL(bn_8193_bits)(rrem)
			&& FSYMBOL(bn_8193_bits)(sh_b))) {
		FSYMBOL(bn_8193_init)(rem);
		return;
	}

	uint32 sh = FSYMBOL(bn_8193_bits)(rrem)
		- FSYMBOL(bn_8193_bits)(sh_b);

	FSYMBOL(bn_8193_lsh)(sh_b, sh);
	while (FSYMBOL(bn_u8193_cmp)(rrem, sh_b) >= 0) {
		FSYMBOL(bn_8193_lsh)(sh_b, 1);
		sh++;
	}

	FSYMBOL(bn_8193_init)(rquo);
	for (; sh > 0; sh--) {
		FSYMBOL(bn_8193_rsh)(sh_b, 1);
		FSYMBOL(bn_8193_lsh)(rquo, 1);
		if (FSYMBOL(bn_u8193_cmp)(rrem, sh_b) >= 0) {
			FSYMBOL(bn_u8193_sub)(rrem, rrem, sh_b);
			rquo[0] |= 1;
		}
	}

	FSYMBOL(bn_8193_move)(quo, rquo);
	FSYMBOL(bn_8193_move)(rem, rrem);
} /* end */

/* @func: bn_u8193_add1 - big number addition
* @param1: bn_8193_t       # sum
* @param2: const bn_8193_t # addend
* @param3: uint32          # addend
* @return: void
*/
void FSYMBOL(bn_u8193_add1)(bn_8193_t r,
		const bn_8193_t a, uint32 b) {
	bn_8193_t rr;

	if (!b) {
		FSYMBOL(bn_8193_move)(r, a);
		return;
	}
	if (!FSYMBOL(bn_8193_bits)(a)) {
		FSYMBOL(bn_8193_set1)(r, b);
		return;
	}

	FSYMBOL(bn_8193_init)(rr);

	uint32 carry = b;
	for (int32 i = 0; i < BN_8193_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_8193_move)(r, rr);
} /* end */

/* @func: bn_u8193_sub1 - big number subtraction
* @param1: bn_8193_t       # difference
* @param2: const bn_8193_t # minuend
* @param3: uint32          # subtract
* @return: void
*/
void FSYMBOL(bn_u8193_sub1)(bn_8193_t r,
		const bn_8193_t a, uint32 b) {
	bn_8193_t rr;

	if (!b) {
		FSYMBOL(bn_8193_move)(r, a);
		return;
	}
	if (!FSYMBOL(bn_8193_bits)(a)) {
		FSYMBOL(bn_8193_set1)(r, b);
		return;
	}

	FSYMBOL(bn_8193_init)(rr);

	int32 borrow = 0;
	for (int32 i = 0; i < BN_8193_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] - b - borrow;
		rr[i] = tmp & 0xffffffff;
		borrow = (tmp >> 32) ? 1 : 0;
		b = 0;
	}

	FSYMBOL(bn_8193_move)(r, rr);
} /* end */

/* @func: bn_u8193_mul1 - big number multiplication
* @param1: bn_8193_t       # product
* @param2: const bn_8193_t # multiplicand
* @param3: uint32          # multiplier
* @return: void
*/
void FSYMBOL(bn_u8193_mul1)(bn_8193_t r,
		const bn_8193_t a, uint32 b) {
	bn_8193_t rr;

	if (!b || !FSYMBOL(bn_8193_bits)(a)) {
		FSYMBOL(bn_8193_init)(r);
		return;
	}

	FSYMBOL(bn_8193_init)(rr);

	uint32 carry = 0;
	for (int32 i = 0; i < BN_8193_SIZE; i++) {
		uint64L tmp = (uint64L)a[i] * b + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	FSYMBOL(bn_8193_move)(r, rr);
} /* end */

/* @func: bn_u8193_div1 - big number division
* @param1: bn_8193_t       # quotient
* @param2: bn_8193_t       # remainder
* @param3: const bn_8193_t # dividend
* @param4: uint32          # divisor
* @return: void
*/
void FSYMBOL(bn_u8193_div1)(bn_8193_t quo, bn_8193_t rem,
		const bn_8193_t a, uint32 b) {
	bn_8193_t tmp;
	FSYMBOL(bn_8193_set1)(tmp, b);

	FSYMBOL(bn_u8193_div)(quo, rem, a, tmp);
} /* end */
