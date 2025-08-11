/* crc32.h - crc (cyclic redundancy check) definitions */

#ifndef LIBF_AL_CRC32_H
#define LIBF_AL_CRC32_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: crc32 */
#undef CRC32_DEFAULT_MSB_TYPE
#define CRC32_DEFAULT_MSB_TYPE 0
#undef CRC32_DEFAULT_LSB_TYPE
#define CRC32_DEFAULT_LSB_TYPE 1
#undef CRC32_CASTAGNOLI_MSB_TYPE
#define CRC32_CASTAGNOLI_MSB_TYPE 2
#undef CRC32_CASTAGNOLI_LSB_TYPE
#define CRC32_CASTAGNOLI_LSB_TYPE 3
#undef CRC32_KOOPMAN_MSB_TYPE
#define CRC32_KOOPMAN_MSB_TYPE 4
#undef CRC32_KOOPMAN_LSB_TYPE
#define CRC32_KOOPMAN_LSB_TYPE 5
#undef CRC32_CKSUM_MSB_TYPE
#define CRC32_CKSUM_MSB_TYPE 6

#undef CRC32_TABLE_SIZE
#define CRC32_TABLE_SIZE 256
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* crc32.c */
extern const uint32 *FSYMBOL(crc32_table)(int32 type);
extern uint32 FSYMBOL(crc32_msb)(const uint32 *t, uint32 c, const uint8 *s,
		uint32 len);
extern uint32 FSYMBOL(crc32_lsb)(const uint32 *t, uint32 c, const uint8 *s,
		uint32 len);
extern uint32 FSYMBOL(crc32_cksum_size_msb)(const uint32 *t, uint32 c,
		uint32 len);
extern uint32 FSYMBOL(crc32)(const uint8 *s, uint32 len, int32 type);

#ifdef __cplusplus
}
#endif


#endif
