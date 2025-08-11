/* crc64.h - crc (cyclic redundancy check) definitions */

#ifndef LIBF_AL_CRC64_H
#define LIBF_AL_CRC64_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @def: crc64 */
#undef CRC64_DEFAULT_MSB_TYPE
#define CRC64_DEFAULT_MSB_TYPE 0
#undef CRC64_DEFAULT_LSB_TYPE
#define CRC64_DEFAULT_LSB_TYPE 1
#undef CRC64_ECMA_MSB_TYPE
#define CRC64_ECMA_MSB_TYPE 2
#undef CRC64_ECMA_LSB_TYPE
#define CRC64_ECMA_LSB_TYPE 3

#undef CRC64_TABLE_SIZE
#define CRC64_TABLE_SIZE 256
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* crc64.c */
extern const uint64L *FSYMBOL(crc64_table)(int32 type);
extern uint64L FSYMBOL(crc64_msb)(const uint64L *t, uint64L c, const uint8 *s,
		uint32 len);
extern uint64L FSYMBOL(crc64_lsb)(const uint64L *t, uint64L c, const uint8 *s,
		uint32 len);
extern uint64L FSYMBOL(crc64)(const uint8 *s, uint32 len, int32 type);

#ifdef __cplusplus
}
#endif


#endif
