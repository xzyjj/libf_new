/* xerrno.h - error value message definitions */

#ifndef LIBF_SL_XERRNO_H
#define LIBF_SL_XERRNO_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @func: errno
* Undefined error */
#undef XEUNDEF_STR
#define XEUNDEF_STR "Undefined error"
#undef XEUNDEF
#define XEUNDEF 0

/* Operation not permitted */
#undef XEPERM
#define XEPERM 1
#undef XEPERM_STR
#define XEPERM_STR "Operation not permitted"

/* No such file or directory */
#undef XENOENT
#define XENOENT 2
#undef XENOENT_STR
#define XENOENT_STR "No such file or directory"

/* No such process */
#undef XESRCH
#define XESRCH 3
#undef XESRCH_STR
#define XESRCH_STR "No such process"

/* Interrupted system call */
#undef XEINTR
#define XEINTR 4
#undef XEINTR_STR
#define XEINTR_STR "Interrupted system call"

/* I/O error */
#undef XEIO
#define XEIO 5
#undef XEIO_STR
#define XEIO_STR "I/O error"

/* No such device or address */
#undef XENXIO
#define XENXIO 6
#undef XENXIO_STR
#define XENXIO_STR "No such device or address"

/* Argument list too long */
#undef XE2BIG
#define XE2BIG 7
#undef XE2BIG_STR
#define XE2BIG_STR "Argument list too long"

/* Exec format error */
#undef XENOEXEC
#define XENOEXEC 8
#undef XENOEXEC_STR
#define XENOEXEC_STR "Exec format error"

/* Bad file descriptor */
#undef XEBADF
#define XEBADF 9
#undef XEBADF_STR
#define XEBADF_STR "Bad file descriptor"

/* No child processes */
#undef XECHILD
#define XECHILD 10
#undef XECHILD_STR
#define XECHILD_STR "No child processes"

/* Try again */
#undef XEAGAIN
#define XEAGAIN 11
#undef XEAGAIN_STR
#define XEAGAIN_STR "Try again"

/* Out of memory */
#undef XENOMEM
#define XENOMEM 12
#undef XENOMEM_STR
#define XENOMEM_STR "Out of memory"

/* Permission denied */
#undef XEACCES
#define XEACCES 13
#undef XEACCES_STR
#define XEACCES_STR "Permission denied"

/* Bad address */
#undef XEFAULT
#define XEFAULT 14
#undef XEFAULT_STR
#define XEFAULT_STR "Bad address"

/* Block device required */
#undef XENOTBLK
#define XENOTBLK 15
#undef XENOTBLK_STR
#define XENOTBLK_STR "Block device required"

/* Device or resource busy */
#undef XEBUSY
#define XEBUSY 16
#undef XEBUSY_STR
#define XEBUSY_STR "Device or resource busy"

/* File exists */
#undef XEEXIST
#define XEEXIST 17
#undef XEEXIST_STR
#define XEEXIST_STR "File exists"

/* Cross-device link */
#undef XEXDEV
#define XEXDEV 18
#undef XEXDEV_STR
#define XEXDEV_STR "Cross-device link"

/* No such device */
#undef XENODEV
#define XENODEV 19
#undef XENODEV_STR
#define XENODEV_STR "No such device"

/* Not a directory */
#undef XENOTDIR
#define XENOTDIR 20
#undef XENOTDIR_STR
#define XENOTDIR_STR "Not a directory"

/* Is a directory */
#undef XEISDIR
#define XEISDIR 21
#undef XEISDIR_STR
#define XEISDIR_STR "Is a directory"

/* Invalid argument */
#undef XEINVAL
#define XEINVAL 22
#undef XEINVAL_STR
#define XEINVAL_STR "Invalid argument"

/* File table overflow */
#undef XENFILE
#define XENFILE 23
#undef XENFILE_STR
#define XENFILE_STR "File table overflow"

/* Too many open files */
#undef XEMFILE
#define XEMFILE 24
#undef XEMFILE_STR
#define XEMFILE_STR "Too many open files"

/* Inappropriate ioctl for device */
#undef XENOTTY
#define XENOTTY 25
#undef XENOTTY_STR
#define XENOTTY_STR "Inappropriate ioctl for device"

/* Text file busy */
#undef XETXTBSY
#define XETXTBSY 26
#undef XETXTBSY_STR
#define XETXTBSY_STR "Text file busy"

/* File too large */
#undef XEFBIG
#define XEFBIG 27
#undef XEFBIG_STR
#define XEFBIG_STR "File too large"

/* No space left on device */
#undef XENOSPC
#define XENOSPC 28
#undef XENOSPC_STR
#define XENOSPC_STR "No space left on device"

/* Illegal seek */
#undef XESPIPE
#define XESPIPE 29
#undef XESPIPE_STR
#define XESPIPE_STR "Illegal seek"

/* Read-only file system */
#undef XEROFS
#define XEROFS 30
#undef XEROFS_STR
#define XEROFS_STR "Read-only file system"

/* Too many links */
#undef XEMLINK
#define XEMLINK 31
#undef XEMLINK_STR
#define XEMLINK_STR "Too many links"

/* Broken pipe */
#undef XEPIPE
#define XEPIPE 32
#undef XEPIPE_STR
#define XEPIPE_STR "Broken pipe"

/* Math argument out of domain of func */
#undef XEDOM
#define XEDOM 33
#undef XEDOM_STR
#define XEDOM_STR "Math argument out of domain of func"

/* Math result not representable */
#undef XERANGE
#define XERANGE 34
#undef XERANGE_STR
#define XERANGE_STR "Math result not representable"

/* Resource deadlock would occur */
#undef XEDEADLK
#define XEDEADLK 35
#undef XEDEADLK_STR
#define XEDEADLK_STR "Resource deadlock would occur"

/* File name too long */
#undef XENAMETOOLONG
#define XENAMETOOLONG 36
#undef XENAMETOOLONG_STR
#define XENAMETOOLONG_STR "File name too long"

/* No record locks available */
#undef XENOLCK
#define XENOLCK 37
#undef XENOLCK_STR
#define XENOLCK_STR "No record locks available"

/* Function not implemented */
#undef XENOSYS
#define XENOSYS 38
#undef XENOSYS_STR
#define XENOSYS_STR "Function not implemented"

/* Directory not empty */
#undef XENOTEMPTY
#define XENOTEMPTY 39
#undef XENOTEMPTY_STR
#define XENOTEMPTY_STR "Directory not empty"

/* Too many symbolic links encountered */
#undef XELOOP
#define XELOOP 40
#undef XELOOP_STR
#define XELOOP_STR "Too many symbolic links encountered"

/* No message of desired type */
#undef XENOMSG
#define XENOMSG 42
#undef XENOMSG_STR
#define XENOMSG_STR "No message of desired type"

/* Identifier removed */
#undef XEIDRM
#define XEIDRM 43
#undef XEIDRM_STR
#define XEIDRM_STR "Identifier removed"

/* Channel number out of range */
#undef XECHRNG
#define XECHRNG 44
#undef XECHRNG_STR
#define XECHRNG_STR "Channel number out of range"

/* Level 2 not synchronized */
#undef XEL2NSYNC
#define XEL2NSYNC 45
#undef XEL2NSYNC_STR
#define XEL2NSYNC_STR "Level 2 not synchronized"

/* Level 3 halted */
#undef XEL3HLT
#define XEL3HLT 46
#undef XEL3HLT_STR
#define XEL3HLT_STR "Level 3 halted"

/* Level 3 reset */
#undef XEL3RST
#define XEL3RST 47
#undef XEL3RST_STR
#define XEL3RST_STR "Level 3 reset"

/* Link number out of range */
#undef XELNRNG
#define XELNRNG 48
#undef XELNRNG_STR
#define XELNRNG_STR "Link number out of range"

/* Protocol driver not attached */
#undef XEUNATCH
#define XEUNATCH 49
#undef XEUNATCH_STR
#define XEUNATCH_STR "Protocol driver not attached"

/* No CSI structure available */
#undef XENOCSI
#define XENOCSI 50
#undef XENOCSI_STR
#define XENOCSI_STR "No CSI structure available"

/* Level 2 halted */
#undef XEL2HLT
#define XEL2HLT 51
#undef XEL2HLT_STR
#define XEL2HLT_STR "Level 2 halted"

/* Invalid exchange */
#undef XEBADE
#define XEBADE 52
#undef XEBADE_STR
#define XEBADE_STR "Invalid exchange"

/* Invalid request descriptor */
#undef XEBADR
#define XEBADR 53
#undef XEBADR_STR
#define XEBADR_STR "Invalid request descriptor"

/* Exchange full */
#undef XEXFULL
#define XEXFULL 54
#undef XEXFULL_STR
#define XEXFULL_STR "Exchange full"

/* No anode */
#undef XENOANO
#define XENOANO 55
#undef XENOANO_STR
#define XENOANO_STR "No anode"

/* Invalid request code */
#undef XEBADRQC
#define XEBADRQC 56
#undef XEBADRQC_STR
#define XEBADRQC_STR "Invalid request code"

/* Invalid slot */
#undef XEBADSLT
#define XEBADSLT 57
#undef XEBADSLT_STR
#define XEBADSLT_STR "Invalid slot"

/* Bad font file format */
#undef XEBFONT
#define XEBFONT 59
#undef XEBFONT_STR
#define XEBFONT_STR "Bad font file format"

/* Device not a stream */
#undef XENOSTR
#define XENOSTR 60
#undef XENOSTR_STR
#define XENOSTR_STR "Device not a stream"

/* No data available */
#undef XENODATA
#define XENODATA 61
#undef XENODATA_STR
#define XENODATA_STR "No data available"

/* Timer expired */
#undef XETIME
#define XETIME 62
#undef XETIME_STR
#define XETIME_STR "Timer expired"

/* Out of streams resources */
#undef XENOSR
#define XENOSR 63
#undef XENOSR_STR
#define XENOSR_STR "Out of streams resources"

/* Machine is not on the network */
#undef XENONET
#define XENONET 64
#undef XENONET_STR
#define XENONET_STR "Machine is not on the network"

/* Package not installed */
#undef XENOPKG
#define XENOPKG 65
#undef XENOPKG_STR
#define XENOPKG_STR "Package not installed"

/* Object is remote */
#undef XEREMOTE
#define XEREMOTE 66
#undef XEREMOTE_STR
#define XEREMOTE_STR "Object is remote"

/* Link has been severed */
#undef XENOLINK
#define XENOLINK 67
#undef XENOLINK_STR
#define XENOLINK_STR "Link has been severed"

/* Advertise error */
#undef XEADV
#define XEADV 68
#undef XEADV_STR
#define XEADV_STR "Advertise error"

/* Srmount error */
#undef XESRMNT
#define XESRMNT 69
#undef XESRMNT_STR
#define XESRMNT_STR "Srmount error"

/* Communication error on send */
#undef XECOMM
#define XECOMM 70
#undef XECOMM_STR
#define XECOMM_STR "Communication error on send"

/* Protocol error */
#undef XEPROTO
#define XEPROTO 71
#undef XEPROTO_STR
#define XEPROTO_STR "Protocol error"

/* Multihop attempted */
#undef XEMULTIHOP
#define XEMULTIHOP 72
#undef XEMULTIHOP_STR
#define XEMULTIHOP_STR "Multihop attempted"

/* RFS specific error */
#undef XEDOTDOT
#define XEDOTDOT 73
#undef XEDOTDOT_STR
#define XEDOTDOT_STR "RFS specific error"

/* Not a data message */
#undef XEBADMSG
#define XEBADMSG 74
#undef XEBADMSG_STR
#define XEBADMSG_STR "Not a data message"

/* Value too large for defined data type */
#undef XEOVERFLOW
#define XEOVERFLOW 75
#undef XEOVERFLOW_STR
#define XEOVERFLOW_STR "Value too large for defined data type"

/* Name not unique on network */
#undef XENOTUNIQ
#define XENOTUNIQ 76
#undef XENOTUNIQ_STR
#define XENOTUNIQ_STR "Name not unique on network"

/* File descriptor in bad state */
#undef XEBADFD
#define XEBADFD 77
#undef XEBADFD_STR
#define XEBADFD_STR "File descriptor in bad state"

/* Remote address changed */
#undef XEREMCHG
#define XEREMCHG 78
#undef XEREMCHG_STR
#define XEREMCHG_STR "Remote address changed"

/* Can not access a needed shared library */
#undef XELIBACC
#define XELIBACC 79
#undef XELIBACC_STR
#define XELIBACC_STR "Can not access a needed shared library"

/* Accessing a corrupted shared library */
#undef XELIBBAD
#define XELIBBAD 80
#undef XELIBBAD_STR
#define XELIBBAD_STR "Accessing a corrupted shared library"

/* .lib section in a.out corrupted */
#undef XELIBSCN
#define XELIBSCN 81
#undef XELIBSCN_STR
#define XELIBSCN_STR ".lib section in a.out corrupted"

/* Attempting to link in too many shared libraries */
#undef XELIBMAX
#define XELIBMAX 82
#undef XELIBMAX_STR
#define XELIBMAX_STR "Attempting to link in too many shared libraries"

/* Cannot exec a shared library directly */
#undef XELIBEXEC
#define XELIBEXEC 83
#undef XELIBEXEC_STR
#define XELIBEXEC_STR "Cannot exec a shared library directly"

/* Illegal byte sequence */
#undef XEILSEQ
#define XEILSEQ 84
#undef XEILSEQ_STR
#define XEILSEQ_STR "Illegal byte sequence"

/* Interrupted system call should be restarted */
#undef XERESTART
#define XERESTART 85
#undef XERESTART_STR
#define XERESTART_STR "Interrupted system call should be restarted"

/* Streams pipe error */
#undef XESTRPIPE
#define XESTRPIPE 86
#undef XESTRPIPE_STR
#define XESTRPIPE_STR "Streams pipe error"

/* Too many users */
#undef XEUSERS
#define XEUSERS 87
#undef XEUSERS_STR
#define XEUSERS_STR "Too many users"

/* Socket operation on non-socket */
#undef XENOTSOCK
#define XENOTSOCK 88
#undef XENOTSOCK_STR
#define XENOTSOCK_STR "Socket operation on non-socket"

/* Destination address required */
#undef XEDESTADDRREQ
#define XEDESTADDRREQ 89
#undef XEDESTADDRREQ_STR
#define XEDESTADDRREQ_STR "Destination address required"

/* Message too long */
#undef XEMSGSIZE
#define XEMSGSIZE 90
#undef XEMSGSIZE_STR
#define XEMSGSIZE_STR "Message too long"

/* Protocol wrong type for socket */
#undef XEPROTOTYPE
#define XEPROTOTYPE 91
#undef XEPROTOTYPE_STR
#define XEPROTOTYPE_STR "Protocol wrong type for socket"

/* Protocol not available */
#undef XENOPROTOOPT
#define XENOPROTOOPT 92
#undef XENOPROTOOPT_STR
#define XENOPROTOOPT_STR "Protocol not available"

/* Protocol not supported */
#undef XEPROTONOSUPPORT
#define XEPROTONOSUPPORT 93
#undef XEPROTONOSUPPORT_STR
#define XEPROTONOSUPPORT_STR "Protocol not supported"

/* Socket type not supported */
#undef XESOCKTNOSUPPORT
#define XESOCKTNOSUPPORT 94
#undef XESOCKTNOSUPPORT_STR
#define XESOCKTNOSUPPORT_STR "Socket type not supported"

/* Operation not supported on transport endpoint */
#undef XEOPNOTSUPP
#define XEOPNOTSUPP 95
#undef XEOPNOTSUPP_STR
#define XEOPNOTSUPP_STR "Operation not supported on transport endpoint"

/* Protocol family not supported */
#undef XEPFNOSUPPORT
#define XEPFNOSUPPORT 96
#undef XEPFNOSUPPORT_STR
#define XEPFNOSUPPORT_STR "Protocol family not supported"

/* Address family not supported by protocol */
#undef XEAFNOSUPPORT
#define XEAFNOSUPPORT 97
#undef XEAFNOSUPPORT_STR
#define XEAFNOSUPPORT_STR "Address family not supported by protocol"

/* Address already in use */
#undef XEADDRINUSE
#define XEADDRINUSE 98
#undef XEADDRINUSE_STR
#define XEADDRINUSE_STR "Address already in use"

/* Cannot assign requested address */
#undef XEADDRNOTAVAIL
#define XEADDRNOTAVAIL 99
#undef XEADDRNOTAVAIL_STR
#define XEADDRNOTAVAIL_STR "Cannot assign requested address"

/* Network is down */
#undef XENETDOWN
#define XENETDOWN 100
#undef XENETDOWN_STR
#define XENETDOWN_STR "Network is down"

/* Network is unreachable */
#undef XENETUNREACH
#define XENETUNREACH 101
#undef XENETUNREACH_STR
#define XENETUNREACH_STR "Network is unreachable"

/* Network dropped connection because of reset */
#undef XENETRESET
#define XENETRESET 102
#undef XENETRESET_STR
#define XENETRESET_STR "Network dropped connection because of reset"

/* Software caused connection abort */
#undef XECONNABORTED
#define XECONNABORTED 103
#undef XECONNABORTED_STR
#define XECONNABORTED_STR "Software caused connection abort"

/* Connection reset by peer */
#undef XECONNRESET
#define XECONNRESET 104
#undef XECONNRESET_STR
#define XECONNRESET_STR "Connection reset by peer"

/* No buffer space available */
#undef XENOBUFS
#define XENOBUFS 105
#undef XENOBUFS_STR
#define XENOBUFS_STR "No buffer space available"

/* Transport endpoint is already connected */
#undef XEISCONN
#define XEISCONN 106
#undef XEISCONN_STR
#define XEISCONN_STR "Transport endpoint is already connected"

/* Transport endpoint is not connected */
#undef XENOTCONN
#define XENOTCONN 107
#undef XENOTCONN_STR
#define XENOTCONN_STR "Transport endpoint is not connected"

/* Cannot send after transport endpoint shutdown */
#undef XESHUTDOWN
#define XESHUTDOWN 108
#undef XESHUTDOWN_STR
#define XESHUTDOWN_STR "Cannot send after transport endpoint shutdown"

/* Too many references: cannot splice */
#undef XETOOMANYREFS
#define XETOOMANYREFS 109
#undef XETOOMANYREFS_STR
#define XETOOMANYREFS_STR "Too many references: cannot splice"

/* Connection timed out */
#undef XETIMEDOUT
#define XETIMEDOUT 110
#undef XETIMEDOUT_STR
#define XETIMEDOUT_STR "Connection timed out"

/* Connection refused */
#undef XECONNREFUSED
#define XECONNREFUSED 111
#undef XECONNREFUSED_STR
#define XECONNREFUSED_STR "Connection refused"

/* Host is down */
#undef XEHOSTDOWN
#define XEHOSTDOWN 112
#undef XEHOSTDOWN_STR
#define XEHOSTDOWN_STR "Host is down"

/* No route to host */
#undef XEHOSTUNREACH
#define XEHOSTUNREACH 113
#undef XEHOSTUNREACH_STR
#define XEHOSTUNREACH_STR "No route to host"

/* Operation already in progress */
#undef XEALREADY
#define XEALREADY 114
#undef XEALREADY_STR
#define XEALREADY_STR "Operation already in progress"

/* Operation now in progress */
#undef XEINPROGRESS
#define XEINPROGRESS 115
#undef XEINPROGRESS_STR
#define XEINPROGRESS_STR "Operation now in progress"

/* Stale NFS file handle */
#undef XESTALE
#define XESTALE 116
#undef XESTALE_STR
#define XESTALE_STR "Stale NFS file handle"

/* Structure needs cleaning */
#undef XEUCLEAN
#define XEUCLEAN 117
#undef XEUCLEAN_STR
#define XEUCLEAN_STR "Structure needs cleaning"

/* Not a XENIX named type file */
#undef XENOTNAM
#define XENOTNAM 118
#undef XENOTNAM_STR
#define XENOTNAM_STR "Not a XENIX named type file"

/* No XENIX semaphores available */
#undef XENAVAIL
#define XENAVAIL 119
#undef XENAVAIL_STR
#define XENAVAIL_STR "No XENIX semaphores available"

/* Is a named type file */
#undef XEISNAM
#define XEISNAM 120
#undef XEISNAM_STR
#define XEISNAM_STR "Is a named type file"

/* Remote I/O error */
#undef XEREMOTEIO
#define XEREMOTEIO 121
#undef XEREMOTEIO_STR
#define XEREMOTEIO_STR "Remote I/O error"

/* Quota exceeded */
#undef XEDQUOT
#define XEDQUOT 122
#undef XEDQUOT_STR
#define XEDQUOT_STR "Quota exceeded"

/* No medium found */
#undef XENOMEDIUM
#define XENOMEDIUM 123
#undef XENOMEDIUM_STR
#define XENOMEDIUM_STR "No medium found"

/* Wrong medium type */
#undef XEMEDIUMTYPE
#define XEMEDIUMTYPE 124
#undef XEMEDIUMTYPE_STR
#define XEMEDIUMTYPE_STR "Wrong medium type"

/* Operation Canceled */
#undef XECANCELED
#define XECANCELED 125
#undef XECANCELED_STR
#define XECANCELED_STR "Operation Canceled"

/* Required key not available */
#undef XENOKEY
#define XENOKEY 126
#undef XENOKEY_STR
#define XENOKEY_STR "Required key not available"

/* Key has expired */
#undef XEKEYEXPIRED
#define XEKEYEXPIRED 127
#undef XEKEYEXPIRED_STR
#define XEKEYEXPIRED_STR "Key has expired"

/* Key has been revoked */
#undef XEKEYREVOKED
#define XEKEYREVOKED 128
#undef XEKEYREVOKED_STR
#define XEKEYREVOKED_STR "Key has been revoked"

/* Key was rejected by service */
#undef XEKEYREJECTED
#define XEKEYREJECTED 129
#undef XEKEYREJECTED_STR
#define XEKEYREJECTED_STR "Key was rejected by service"

/* Owner died */
#undef XEOWNERDEAD
#define XEOWNERDEAD 130
#undef XEOWNERDEAD_STR
#define XEOWNERDEAD_STR "Owner died"

/* State not recoverable */
#undef XENOTRECOVERABLE
#define XENOTRECOVERABLE 131
#undef XENOTRECOVERABLE_STR
#define XENOTRECOVERABLE_STR "State not recoverable"

/* Operation not possible due to RF-kill */
#undef XERFKILL
#define XERFKILL 132
#undef XERFKILL_STR
#define XERFKILL_STR "Operation not possible due to RF-kill"

/* Memory page has hardware error */
#undef XEHWPOISON
#define XEHWPOISON 133
#undef XEHWPOISON_STR
#define XEHWPOISON_STR "Memory page has hardware error"

/* Max */
#undef XEMAX
#define XEMAX 134
#undef XEMAX_STR
#define XEMAX_STR "Unknown error"
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xerrno_strerror.c */
extern char *XSYMBOL(strerror)(int32 n);

#ifdef __cplusplus
}
#endif


#endif
