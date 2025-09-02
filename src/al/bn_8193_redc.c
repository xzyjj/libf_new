/* bn_8193_redc.c - big number calculation (8193-bit) implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_8193.h>


/* @func: bn_i8193_redc_init - redc montgomery reduction init
* @param1: bn_i8193_redc_t # redc context
* @param2: const bn_8193_t # modules
* @return: int32           # 0: success, -1: fail
*/
int32 FSYMBOL(bn_i8193_redc_init)(bn_i8193_redc_t *ctx,
		const bn_8193_t n) {
	bn_8193_t quo;
	ctx->k = FSYMBOL(bn_8193_bits)(n);

	/* R = 1 << k */
	FSYMBOL(bn_8193_set1)(ctx->R, 1);
	FSYMBOL(bn_8193_lsh)(ctx->R, ctx->k);

	/* n_prime = -inv(n, R) */
	if (FSYMBOL(bn_i8193_inv)(ctx->np, n, ctx->R))
		return -1;
	ctx->np[BN_8193_SIG] = 1;

	/* n_prime = n_prime % R */
	FSYMBOL(bn_i8193_divmod)(quo, ctx->np, ctx->np, ctx->R);

	/* Rs = (R * R) % n */
	FSYMBOL(bn_i8193_mul)(ctx->Rs, ctx->R, ctx->R);
	FSYMBOL(bn_i8193_divmod)(quo, ctx->Rs, ctx->Rs, n);

	/* mask = R - 1 */
	FSYMBOL(bn_i8193_sub1)(ctx->mask, ctx->R, 1);

	return 0;
} /* end */

/* @func: bn_i8193_redc_mod - redc montgomery reduction (norm)
* @param1: bn_i8193_redc_t # redc context
* @param2: bn_8193_t       # result
* @param3: const bn_8193_t # number
* @param4: const bn_8193_t # modules
* @return: void
*/
void FSYMBOL(bn_i8193_redc_mod)(bn_i8193_redc_t *ctx, bn_8193_t r,
		const bn_8193_t a, const bn_8193_t n) {
	bn_8193_t rr;

	/* r = ((a & mask) * n_prime) & mask */
	FSYMBOL(bn_8193_and)(rr, a, ctx->mask);
	FSYMBOL(bn_i8193_mul)(rr, rr, ctx->np);
	FSYMBOL(bn_8193_and)(rr, rr, ctx->mask);

	/* r = (a + (r * n)) >> k */
	FSYMBOL(bn_i8193_mul)(rr, rr, n);
	FSYMBOL(bn_i8193_add)(rr, rr, a);
	FSYMBOL(bn_8193_rsh)(rr, ctx->k);

	/* r = if r >= n : adjust */
	if (FSYMBOL(bn_i8193_cmp)(rr, n) >= 0)
		FSYMBOL(bn_i8193_sub)(rr, rr, n);

	FSYMBOL(bn_8193_move)(r, rr);
} /* end */

/* @func: bn_i8193_redc_moto - redc montgomery domain
* @param1: bn_i8193_redc_t # redc context
* @param2: bn_8193_t       # result
* @param3: const bn_8193_t # number
* @param4: const bn_8193_t # modules
* @return: void
*/
void FSYMBOL(bn_i8193_redc_moto)(bn_i8193_redc_t *ctx, bn_8193_t r,
		const bn_8193_t a, const bn_8193_t n) {
	bn_8193_t rr;

	/* r = redc(a * Rs, n) */
	FSYMBOL(bn_i8193_mul)(rr, a, ctx->Rs);
	FSYMBOL(bn_i8193_redc_mod)(ctx, r, rr, n);
} /* end */

/* @func: bn_i8193_redc_pow - redc montgomery modular exponentiation
* @param1: bn_8193_t       # result
* @param2: const bn_8193_t # number
* @param3: const bn_8193_t # exponential
* @param4: const bn_8193_t # modules
* @return: int32           # 0: success, -1: fail
*/
int32 FSYMBOL(bn_i8193_redc_pow)(bn_8193_t r,
		const bn_8193_t a, const bn_8193_t b,
		const bn_8193_t m) {
	bn_i8193_redc_t ctx;
	bn_8193_t rr, a1, b1;

	if (FSYMBOL(bn_i8193_redc_init)(&ctx, m))
		return -1;

	FSYMBOL(bn_8193_set1)(rr, 1);
	FSYMBOL(bn_i8193_redc_moto)(&ctx, rr, rr, m);
	FSYMBOL(bn_i8193_redc_moto)(&ctx, a1, a, m);
	FSYMBOL(bn_8193_move)(b1, b);

	while (FSYMBOL(bn_8193_bits)(b1)) {
		if (b1[0] & 1) {
			/* rr = (rr * a1) % m */
			FSYMBOL(bn_i8193_mul)(rr, rr, a1);
			FSYMBOL(bn_i8193_redc_mod)(&ctx, rr, rr, m);
		}

		/* a1 = (a1 * a1) % m */
		FSYMBOL(bn_i8193_mul)(a1, a1, a1);
		FSYMBOL(bn_i8193_redc_mod)(&ctx, a1, a1, m);

		/* b1 >>= 1 */
		FSYMBOL(bn_8193_rsh)(b1, 1);
	}

	/* r = rr % m */
	FSYMBOL(bn_i8193_redc_mod)(&ctx, r, rr, m);

	return 0;
} /* end */
