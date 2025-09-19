#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/pbkdf2_sha3.h>
#include <libf/al/sha3.h>


int main(void) {
	uint8 buf[20];

	FSYMBOL(pbkdf2_sha3)((uint8 *)"password", 8,
		(uint8 *)"salt", 4,
		buf, 20,
		1, SHA3_512_TYPE);

	for (int32 i = 0; i < 20; i++)
		printf("%02x", buf[i]);
	printf("\n");

	FSYMBOL(pbkdf2_sha3)((uint8 *)"password", 8,
		(uint8 *)"salt", 4,
		buf, 20,
		1, SHA3_384_TYPE);

	for (int32 i = 0; i < 20; i++)
		printf("%02x", buf[i]);
	printf("\n");

	FSYMBOL(pbkdf2_sha3)((uint8 *)"password", 8,
		(uint8 *)"salt", 4,
		buf, 20,
		1, SHA3_256_TYPE);

	for (int32 i = 0; i < 20; i++)
		printf("%02x", buf[i]);
	printf("\n");

	FSYMBOL(pbkdf2_sha3)((uint8 *)"password", 8,
		(uint8 *)"salt", 4,
		buf, 20,
		1, SHA3_224_TYPE);

	for (int32 i = 0; i < 20; i++)
		printf("%02x", buf[i]);
	printf("\n");

	return 0;
}
