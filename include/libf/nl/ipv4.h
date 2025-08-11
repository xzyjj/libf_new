/* ipv4.h - definitions */

#ifndef LIBF_NL_IPV4_H
#define LIBF_NL_IPV4_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: ipv4
*  0               1               2               3
*  0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |Version|  IHL  |Type of Service|          Total Length         |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |         Identification        |Flags|      Fragment Offset    |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |  Time to Live |    Protocol   |         Header Checksum       |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |                       Source Address                          |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |                    Destination Address                        |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* \                    Options 0-40     /     Padding             \
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*
*
*   0   1   2   3   4   5   6   7
* +---+---+---+---+---+---+---+---+
* |Precedence |     TOS       |MBZ|
* +---+---+---+---+---+---+---+---+
*
*   0   1   2   3   4   5   6   7
* +---+---+---+---+---+---+---+---+
* |          DSCP         |  ECN  |
* +---+---+---+---+---+---+---+---+
*
*/

/* packet header */
#undef ipv4_packet_hdr
struct ipv4_packet_hdr {
	uint8 version:4;   /* ipv4: 4 */
	uint8 ihl:4;       /* header length (4-byte per unit) */
	uint8 tos;         /* type of service */
	uint16 length;     /* total length: header + data */
	uint16 identif;    /* identification */
	uint16 flags:3;    /* control flags */
	uint16 frag_of:13; /* fragment offset (8-byte per unit) */
	uint8 ttl;         /* time to live */
	uint8 proto;       /* upper layer protocol type */
};

/* packet checksum */
#undef ipv4_packet_sum
struct ipv4_packet_sum { /* recalculate each route */
	uint16 check; /* header checksum */
};

/* packet address */
#undef ipv4_packet_addr
struct ipv4_packet_addr {
	uint32 saddr; /* source address */
	uint32 daddr; /* destination address */
};

/* packet options */
#undef ipv4_packet_opt
struct ipv4_packet_opt {
	uint8 opts[40]; /* optional fields */
};

/* header size: 20-60byte (60: h:20 + o:40) */
#undef ipv4_packet
struct ipv4_packet { /* header */
	struct ipv4_packet_hdr hdr;
	struct ipv4_packet_sum sum;
	struct ipv4_packet_addr addr;
	struct ipv4_packet_opt opt;
};

/* version */
#undef IPV4_VERSION
#define IPV4_VERSION 4

/* tos */
#undef IPV4_TOS_GET_PREC
#define IPV4_TOS_GET_PREC(x) ((x) & 0x07)
#undef IPV4_TOS_SET_PREC
#define IPV4_TOS_SET_PREC(x) ((x) & 0x07)
#undef IPV4_TOS_GET_TYPE
#define IPV4_TOS_GET_TYPE(x) (((x) >> 3) & 0x0f)
#undef IPV4_TOS_SET_TYPE
#define IPV4_TOS_SET_TYPE(x) (((x) & 0x0f) << 3)

/* Class Selector */
#undef IPV4_DSCP_CS0
#define IPV4_DSCP_CS0 0x00
#undef IPV4_DSCP_CS1
#define IPV4_DSCP_CS1 0x08
#undef IPV4_DSCP_CS2
#define IPV4_DSCP_CS2 0x10
#undef IPV4_DSCP_CS3
#define IPV4_DSCP_CS3 0x18
#undef IPV4_DSCP_CS4
#define IPV4_DSCP_CS4 0x20
#undef IPV4_DSCP_CS5
#define IPV4_DSCP_CS5 0x28
#undef IPV4_DSCP_CS6
#define IPV4_DSCP_CS6 0x30
#undef IPV4_DSCP_CS7
#define IPV4_DSCP_CS7 0x38
/* expedited forwarding */
#undef IPV4_DSCP_EF
#define IPV4_DSCP_EF 0x2e

/* flags */
#undef IPV4_FLAGS_DF
#define IPV4_FLAGS_DF 0x01
#undef IPV4_FLAGS_MF
#define IPV4_FLAGS_MF 0x02

#undef IPV4_OPT_ALIGN
#define IPV4_OPT_ALIGN(x) (4 * (((x) + 3) / 4))

/* end */


#endif
