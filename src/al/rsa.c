/* rsa.c - implementations */

#include <libf/config.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_8193.h>
#include <libf/al/rsa.h>


/* @func: rsa4096_genkey - generate rsa private and public key
* @param1: const bn_8193_t # prime P
* @param2: const bn_8193_t # prime Q
* @param3: bn_8193_t       # public key
* @param4: bn_8193_t       # private key
* @param5: bn_8193_t       # modulus
* @return: int32           # 0: no error, -1: p == q, or inv() fail
*/
int32 FSYMBOL(rsa4096_genkey)(const bn_8193_t p, const bn_8193_t q,
		bn_8193_t e, bn_8193_t d, bn_8193_t n) {
	bn_8193_t a, b;

	/* p == q */
	if (!FSYMBOL(bn_i8193_cmp)(p, q))
		return -1;

	/* a = (p - 1) * (q - 1) */
	FSYMBOL(bn_i8193_sub1)(a, p, 1);
	FSYMBOL(bn_i8193_sub1)(b, q, 1);
	FSYMBOL(bn_i8193_mul)(a, a, b);

	/* n = p * q */
	FSYMBOL(bn_i8193_mul)(n, p, q);

	/* e = 65537 */
	FSYMBOL(bn_8193_set1)(e, 65537);

	/* d = inv(e, a) */
	if (FSYMBOL(bn_i8193_inv)(d, e, a))
		return -1;

	return 0;
} /* end */

/* @func: rsa4096_encrypt - rsa public encryption
* @param1: const bn_8193_t # public key
* @param2: const bn_8193_t # modulus
* @param3: bn_8193_t       # pliantext
* @return: int32           # 0: no error, -1: m > n, or pow() fail
*/
int32 FSYMBOL(rsa4096_encrypt)(const bn_8193_t e, const bn_8193_t n,
		bn_8193_t m) {
	if (FSYMBOL(bn_i8193_cmp)(m, n) > 0) /* m > n */
		return -1;

	/* m = pow(m, e, n) */
	if (FSYMBOL(bn_i8193_redc_pow)(m, m, e, n))
		return -1;

	return 0;
} /* end */

/* @func: rsa4096_decrypt - rsa private decryption
* @param1: const bn_8193_t # private key
* @param2: const bn_8193_t # modulus
* @param3: bn_8193_t       # ciphertext
* @return: int32           # 0: no error, -1: m > n, or pow() fail
*/
int32 FSYMBOL(rsa4096_decrypt)(const bn_8193_t d, const bn_8193_t n,
		bn_8193_t m) {
	if (FSYMBOL(bn_i8193_cmp)(m, n) > 0) /* m > n */
		return -1;

	/* m = pow(m, d, n) */
	if (FSYMBOL(bn_i8193_redc_pow)(m, m, d, n))
		return -1;

	return 0;
} /* end */

/* @func: rsa4096_gencrt - generate rsa chinese remainder theorem parameter
* @param1: const bn_8193_t # prime P
* @param2: const bn_8193_t # prime Q
* @param3: const bn_8193_t # private key
* @param4: bn_8193_t       # crt p parameter
* @param5: bn_8193_t       # crt q parameter
* @param6: bn_8193_t       # Q inverse
* @return: int32           # 0: no error, -1: p == q, or inv() fail
*/
int32 FSYMBOL(rsa4096_gencrt)(const bn_8193_t p, const bn_8193_t q,
		const bn_8193_t d,
		bn_8193_t dp, bn_8193_t dq, bn_8193_t qinv) {
	bn_8193_t a, b, t;

	/* dp = d % (p - 1), dq = d % (q - 1) */
	FSYMBOL(bn_i8193_sub1)(a, p, 1);
	FSYMBOL(bn_i8193_sub1)(b, q, 1);
	FSYMBOL(bn_i8193_divmod)(t, dp, d, a);
	FSYMBOL(bn_i8193_divmod)(t, dq, d, b);

	/* qinv = inv(q, p) */
	if (FSYMBOL(bn_i8193_inv)(qinv, q, p))
		return -1;

	return 0;
} /* end */

/* @func: rsa4096_crt_decrypt - rsa chinese remainder theorem decryption
* @param1: const bn_8193_t # prime P
* @param2: const bn_8193_t # prime Q
* @param3: bn_8193_t       # crt p parameter
* @param4: bn_8193_t       # crt q parameter
* @param5: bn_8193_t       # Q inverse
* @param6: bn_8193_t       # ciphertext
* @return: int32           # 0: no error, -1: m > n, or pow() fail
*/
int32 FSYMBOL(rsa4096_crt_decrypt)(const bn_8193_t q, const bn_8193_t p,
		const bn_8193_t dp, const bn_8193_t dq, const bn_8193_t qinv,
		bn_8193_t m) {
	bn_8193_t mp, mq, r, t;

	/* mp = pow(m % p, dp, p) */
	FSYMBOL(bn_i8193_divmod)(t, mp, m, p);
	if (FSYMBOL(bn_i8193_redc_pow)(mp, mp, dp, p))
		return -1;

	/* mq = pow(m % q, dq, q) */
	FSYMBOL(bn_i8193_divmod)(t, mq, m, q);
	if (FSYMBOL(bn_i8193_redc_pow)(mq, mq, dq, q))
		return -1;

	/* t = ((mp - mq) * qinv) % p */
	FSYMBOL(bn_i8193_sub)(t, mp, mq);
	FSYMBOL(bn_i8193_mul)(t, t, qinv);
	FSYMBOL(bn_i8193_divmod)(r, t, t, p);

	/* m = (t * q) + mq */
	FSYMBOL(bn_i8193_mul)(t, t, q);
	FSYMBOL(bn_i8193_add)(m, t, mq);

	return 0;
} /* end */
