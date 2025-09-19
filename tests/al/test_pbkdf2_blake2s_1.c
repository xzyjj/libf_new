#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/pbkdf2_blake2s.h>


int main(void) {
	uint8 buf[20];

	FSYMBOL(pbkdf2_blake2s)((uint8 *)"password", 8,
		(uint8 *)"salt", 4,
		buf, 20,
		1);

	for (int32 i = 0; i < 20; i++)
		printf("%02x", buf[i]);
	printf("\n");

	return 0;
}
