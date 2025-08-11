#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/al/crc64.h>


int main(void) {
	uint64 read_size = 0;
	char buf[1 << 16];

	uint64L c = 0;
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		c = FSYMBOL(crc64)((uint8 *)buf, read_size,
			CRC64_DEFAULT_MSB_TYPE);
		printf("crc64msb: %llu (0x%016llx)\n", c, c);

		c = FSYMBOL(crc64)((uint8 *)buf, read_size,
			CRC64_DEFAULT_LSB_TYPE);
		printf("crc64lsb: %llu (0x%016llx)\n", c, c);

		c = FSYMBOL(crc64)((uint8 *)buf, read_size,
			CRC64_ECMA_MSB_TYPE);
		printf("crc64msb-e: %llu (0x%016llx)\n", c, c);

		c = FSYMBOL(crc64)((uint8 *)buf, read_size,
			CRC64_ECMA_LSB_TYPE);
		printf("crc64lsb-e: %llu (0x%016llx)\n", c, c);

		break;
	}

	return 0;
}
