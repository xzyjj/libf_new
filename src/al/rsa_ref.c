/* rsa_ref.c - rsa (rivest–shamir–adleman) cryptosystem implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_8192.h>
#include <libf/al/rsa_ref.h>


/* @func: _rsa4096_extended_gcd (static) - extended euclidean algorithm
* @param1: const bn_int8192_t # input number
* @param2: const bn_int8192_t # input number
* @param3: bn_int8192_t       # bezout coefficient
* @param4: bn_int8192_t       # bezout coefficient
* @param5: bn_int8192_t       # return greatest common divisor
* @return: void
*/
static void _rsa4096_extended_gcd(const bn_int8192_t a, const bn_int8192_t b,
		bn_int8192_t x, bn_int8192_t y, bn_int8192_t r) {
	bn_int8192_t _b, _x, _y, quo, rem;

	if (!FSYMBOL(bn_int8192_cmp_1)(b, 0)) {
		FSYMBOL(bn_int8192_zero)(x);
		FSYMBOL(bn_int8192_zero)(y);
		x[0] = 1;
		y[0] = 0;
		FSYMBOL(bn_int8192_move)(r, a);
		return; /* x, y, r = (1, 0, a) */
	}

	FSYMBOL(bn_int8192_move)(_b, b);
	/* rem = a % b */
	FSYMBOL(bn_int8192_divmod)(quo, rem, a, b);
	_rsa4096_extended_gcd(_b, rem, _x, _y, r);

	FSYMBOL(bn_int8192_move)(x, _y);

	/* y = _x - ((a / b) * _y) */
	FSYMBOL(bn_int8192_divmod)(quo, rem, a, b);
	FSYMBOL(bn_int8192_mul)(quo, quo, _y);
	FSYMBOL(bn_int8192_sub)(y, _x, quo);
} /* end */

/* @func: rsa4096_mod_inverse - modular inverse based on extended euclidean
* @param1: const bn_int8192_t # number
* @param2: const bn_int8192_t # modulus
* @param3: bn_int8192_t       # inverse modulus
* @return: int32              # 0: success, -1: fail
*/
int32 FSYMBOL(rsa4096_mod_inverse)(const bn_int8192_t a, const bn_int8192_t m,
		bn_int8192_t r) {
	bn_int8192_t x, y, quo, rem, rr;
	_rsa4096_extended_gcd(a, m, x, y, rr);

	if (FSYMBOL(bn_int8192_cmp_1)(rr, 1)) { /* rr != 1 */
		FSYMBOL(bn_int8192_zero)(r);
		return -1;
	} else {
		/* r = ((x % m) + m) % m */
		FSYMBOL(bn_int8192_divmod)(quo, rem, x, m);
		FSYMBOL(bn_int8192_add)(rem, rem, m);
		FSYMBOL(bn_int8192_divmod)(quo, rr, rem, m);
		FSYMBOL(bn_int8192_move)(r, rr);
	}

	return 0;
} /* end */

/* @func: rsa4096_mod_pow - fast modular exponentiation
* @param1: const bn_int8192_t # number
* @param2: const bn_int8192_t # exponential
* @param3: const bn_int8192_t # modulus
* @param4: bn_int8192_t       # result
* @return: void
*/
void FSYMBOL(rsa4096_mod_pow)(const bn_int8192_t a, const bn_int8192_t b,
		const bn_int8192_t m, bn_int8192_t r) {
	bn_int8192_t rr, quo, a1, b1;
	FSYMBOL(bn_int8192_zero)(rr);
	FSYMBOL(bn_int8192_move)(b1, b);
	rr[0] = 1; /* rr = 1 */

	/* a1 = a1 % m */
	FSYMBOL(bn_int8192_divmod)(quo, a1, a, m);
	while (FSYMBOL(bn_int8192_cmp_1)(b1, 0) > 0) {
		if (b1[0] & 1) { /* b1 & 1 */
			/* rr = (rr * a1) % m */
			FSYMBOL(bn_int8192_mul)(rr, rr, a1);
			FSYMBOL(bn_int8192_divmod)(quo, rr, rr, m);
		}

		/* a1 = (a1 * a1) % m */
		FSYMBOL(bn_int8192_mul)(a1, a1, a1);
		FSYMBOL(bn_int8192_divmod)(quo, a1, a1, m);
		/* b1 >>= 1 */
		FSYMBOL(bn_int8192_rsh)(b1);
	}

	FSYMBOL(bn_int8192_move)(r, rr);
} /* end */

/* @func: rsa4096_genkey - calculate private and public keys
* @param1: const bn_int8192_t # prime P
* @param2: const bn_int8192_t # prime Q
* @param3: bn_int8192_t       # public key
* @param4: bn_int8192_t       # private key
* @param5: bn_int8192_t       # modulus
* @return: int32              # 0: no error, -1: p == q, or inv() fail
*/
int32 FSYMBOL(rsa4096_genkey)(const bn_int8192_t p, const bn_int8192_t q,
		bn_int8192_t e, bn_int8192_t d, bn_int8192_t n) {
	bn_int8192_t _a, _b, _t;
	FSYMBOL(bn_int8192_zero)(_t);
	_t[0] = 1;

	/* p == q */
	if (!FSYMBOL(bn_int8192_cmp)(p, q)) {
		FSYMBOL(bn_int8192_zero)(e);
		FSYMBOL(bn_int8192_zero)(d);
		FSYMBOL(bn_int8192_zero)(n);
		return -1;
	}

	/* _a = (p - 1) * (q - 1) */
	FSYMBOL(bn_int8192_sub)(_a, p, _t);
	FSYMBOL(bn_int8192_sub)(_b, q, _t);
	FSYMBOL(bn_int8192_mul)(_a, _a, _b);

	/* n = p * q */
	FSYMBOL(bn_int8192_mul)(n, p, q);

	/* e = 65537 */
	FSYMBOL(bn_int8192_zero)(e);
	e[0] = 65537;

	/* d = inv(e, _a) */
	return FSYMBOL(rsa4096_mod_inverse)(e, _a, d);
} /* end */

/* @func: rsa4096_encrypt - rsa encrypto function
* @param1: const bn_int8192_t # public key
* @param2: const bn_int8192_t # modulus
* @param3: bn_int8192_t       # input/output message
* @return: int32              # 0: no error, -1: m > n
*/
int32 FSYMBOL(rsa4096_encrypt)(const bn_int8192_t e, const bn_int8192_t n,
		bn_int8192_t m) {
	if (FSYMBOL(bn_int8192_cmp)(m, n) > 0)
		return -1;

	FSYMBOL(rsa4096_mod_pow)(m, e, n, m);

	return 0;
} /* end */

/* @func: rsa4096_decrypt - rsa decrypto function
* @param1: const bn_int8192_t # private key
* @param2: const bn_int8192_t # modulus
* @param3: bn_int8192_t       # input/output message
* @return: int32              # 0: no error, -1: m > n
*/
int32 FSYMBOL(rsa4096_decrypt)(const bn_int8192_t d, const bn_int8192_t n,
		bn_int8192_t m) {
	if (FSYMBOL(bn_int8192_cmp)(m, n) > 0)
		return -1;

	FSYMBOL(rsa4096_mod_pow)(m, d, n, m);

	return 0;
} /* end */
