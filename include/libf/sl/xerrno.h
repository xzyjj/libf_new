/* xerrno.h - error value message definitions */

#ifndef LIBF_SL_XERRNO_H
#define LIBF_SL_XERRNO_H

#include <libf/config.h>
#include <libf/sl/xstdint.h>


/* @func: errno
* undefined */
#undef XEUNDEF_STR
#define XEUNDEF_STR "<Undefined>"
#undef XEUNDEF
#define XEUNDEF 0

/* argument list too long */
#undef XE2BIG_STR
#define XE2BIG_STR "Argument list too long"
#undef XE2BIG
#define XE2BIG 1

/* permission denied */
#undef XEACCES_STR
#define XEACCES_STR "Permission denied"
#undef XEACCES
#define XEACCES 2

/* address in use */
#undef XEADDRINUSE_STR
#define XEADDRINUSE_STR "Address in use"
#undef XEADDRINUSE
#define XEADDRINUSE 3

/* address not available */
#undef XEADDRNOTAVAIL_STR
#define XEADDRNOTAVAIL_STR "Address not available"
#undef XEADDRNOTAVAIL
#define XEADDRNOTAVAIL 4

/* address family not supported */
#undef XEAFNOSUPPORT_STR
#define XEAFNOSUPPORT_STR "Address family not supported"
#undef XEAFNOSUPPORT
#define XEAFNOSUPPORT 5

/* resource unavailable, try again */
#undef XEAGAIN_STR
#define XEAGAIN_STR "Resource unavailable, try again"
#undef XEAGAIN
#define XEAGAIN 6

/* connection already in progress */
#undef XEALREADY_STR
#define XEALREADY_STR "Connection already in progress"
#undef XEALREADY
#define XEALREADY 7

/* bad file descriptor */
#undef XEBADF_STR
#define XEBADF_STR "Bad file descriptor"
#undef XEBADF
#define XEBADF 8

/* bad message */
#undef XEBADMSG_STR
#define XEBADMSG_STR "Bad message"
#undef XEBADMSG
#define XEBADMSG 9

/* device or resource busy */
#undef XEBUSY_STR
#define XEBUSY_STR "Device or resource busy"
#undef XEBUSY
#define XEBUSY 10

/* operation canceled */
#undef XECANCELED_STR
#define XECANCELED_STR "Operation canceled"
#undef XECANCELED
#define XECANCELED 11

/* no child processes */
#undef XECHILD_STR
#define XECHILD_STR "No child processes"
#undef XECHILD
#define XECHILD 12

/* connection aborted */
#undef XECONNABORTED_STR
#define XECONNABORTED_STR "Connection aborted"
#undef XECONNABORTED
#define XECONNABORTED 13

/* connection aborted */
#undef XECONNREFUSED_STR
#define XECONNREFUSED_STR "connection refused"
#undef XECONNREFUSED
#define XECONNREFUSED 14

/* connection reset */
#undef XECONNRESET_STR
#define XECONNRESET_STR "Connection reset"
#undef XECONNRESET
#define XECONNRESET 15

/* resource deadlock would occur */
#undef XEDEADLK_STR
#define XEDEADLK_STR "Resource deadlock would occur"
#undef XEDEADLK
#define XEDEADLK 16

/* destination address required */
#undef XEDESTADDRREQ_STR
#define XEDESTADDRREQ_STR "Destination address required"
#undef XEDESTADDRREQ
#define XEDESTADDRREQ 17

/* mathematics argument out of domain of function */
#undef XEDOM_STR
#define XEDOM_STR "Mathematics argument out of domain of function"
#undef XEDOM
#define XEDOM 18

/* reserved */
#undef XEDQUOT_STR
#define XEDQUOT_STR "<EDQUOT Reserved>"
#undef XEDQUOT
#define XEDQUOT 19

/* file exists */
#undef XEEXIST_STR
#define XEEXIST_STR "File exists"
#undef XEEXIST
#define XEEXIST 20

/* bad address */
#undef XEFAULT_STR
#define XEFAULT_STR "Bad address"
#undef XEFAULT
#define XEFAULT 21

/* file too large */
#undef XEFBIG_STR
#define XEFBIG_STR "File too large"
#undef XEFBIG
#define XEFBIG 22

/* host is unreachable */
#undef XEHOSTUNREACH_STR
#define XEHOSTUNREACH_STR "Host is unreachable"
#undef XEHOSTUNREACH
#define XEHOSTUNREACH 23

/* identifier removed */
#undef XEIDRM_STR
#define XEIDRM_STR "Identifier removed"
#undef XEIDRM
#define XEIDRM 24

/* illegal byte sequence */
#undef XEILSEQ_STR
#define XEILSEQ_STR "Illegal byte sequence"
#undef XEILSEQ
#define XEILSEQ 25

/* operation in progress */
#undef XEINPROGRESS_STR
#define XEINPROGRESS_STR "Operation in progress"
#undef XEINPROGRESS
#define XEINPROGRESS 26

/* interrupted function */
#undef XEINTR_STR
#define XEINTR_STR "Interrupted function"
#undef XEINTR
#define XEINTR 27

/* invalid argument */
#undef XEINVAL_STR
#define XEINVAL_STR "Invalid argument"
#undef XEINVAL
#define XEINVAL 28

/* i/o error */
#undef XEIO_STR
#define XEIO_STR "I/O error"
#undef XEIO
#define XEIO 29

/* socket is connected */
#undef XEISCONN_STR
#define XEISCONN_STR "Socket is connected"
#undef XEISCONN
#define XEISCONN 30

/* is a directory */
#undef XEISDIR_STR
#define XEISDIR_STR "Is a directory"
#undef XEISDIR
#define XEISDIR 31

/* too many levels of symbolic links */
#undef XELOOP_STR
#define XELOOP_STR "Too many levels of symbolic links"
#undef XELOOP
#define XELOOP 32

/* file descriptor value too large */
#undef XEMFILE_STR
#define XEMFILE_STR "File descriptor value too large"
#undef XEMFILE
#define XEMFILE 33

/* too many links */
#undef XEMLINK_STR
#define XEMLINK_STR "Too many links"
#undef XEMLINK
#define XEMLINK 34

/* message too large */
#undef XEMSGSIZE_STR
#define XEMSGSIZE_STR "Message too large"
#undef XEMSGSIZE
#define XEMSGSIZE 35

/* reserved */
#undef XEMULTIHOP_STR
#define XEMULTIHOP_STR "<EMULTIHOP Reserved>"
#undef XEMULTIHOP
#define XEMULTIHOP 36

/* filename too long */
#undef XENAMETOOLONG_STR
#define XENAMETOOLONG_STR "Filename too long"
#undef XENAMETOOLONG
#define XENAMETOOLONG 37

/* network is down */
#undef XENETDOWN_STR
#define XENETDOWN_STR "Network is down"
#undef XENETDOWN
#define XENETDOWN 38

/* connection aborted by network */
#undef XENETRESET_STR
#define XENETRESET_STR "Connection aborted by network"
#undef XENETRESET
#define XENETRESET 39

/* network unreachable */
#undef XENETUNREACH_STR
#define XENETUNREACH_STR "Network unreachable"
#undef XENETUNREACH
#define XENETUNREACH 40

/* too many files open in system */
#undef XENFILE_STR
#define XENFILE_STR "Too many files open in system"
#undef XENFILE
#define XENFILE 41

/* no buffer space available */
#undef XENOBUFS_STR
#define XENOBUFS_STR "No buffer space available"
#undef XENOBUFS
#define XENOBUFS 42

/* no message is available on the stream head read queue */
#undef XENODATA_STR
#define XENODATA_STR "No message is available on the STREAM head read queue"
#undef XENODATA
#define XENODATA 43

/* no such device */
#undef XENODEV_STR
#define XENODEV_STR "No such device"
#undef XENODEV
#define XENODEV 44

/* no such file or directory */
#undef XENOENT_STR
#define XENOENT_STR "No such file or directory"
#undef XENOENT
#define XENOENT 45

/* executable file format error */
#undef XENOEXEC_STR
#define XENOEXEC_STR "Executable file format error"
#undef XENOEXEC
#define XENOEXEC 46

/* no locks available */
#undef XENOLCK_STR
#define XENOLCK_STR "No locks available"
#undef XENOLCK
#define XENOLCK 47

/* reserved */
#undef XENOLINK_STR
#define XENOLINK_STR "<ENOLINK Reserved>"
#undef XENOLINK
#define XENOLINK 48

/* not enough space */
#undef XENOMEM_STR
#define XENOMEM_STR "Not enough space"
#undef XENOMEM
#define XENOMEM 49

/* no message of the desired type */
#undef XENOMSG_STR
#define XENOMSG_STR "No message of the desired type"
#undef XENOMSG
#define XENOMSG 50

/* protocol not available */
#undef XENOPROTOOPT_STR
#define XENOPROTOOPT_STR "Protocol not available"
#undef XENOPROTOOPT
#define XENOPROTOOPT 51

/* no space left on device */
#undef XENOSPC_STR
#define XENOSPC_STR "No space left on device"
#undef XENOSPC
#define XENOSPC 52

/* no stream resources */
#undef XENOSR_STR
#define XENOSR_STR "No STREAM resources"
#undef XENOSR
#define XENOSR 53

/* not a stream */
#undef XENOSTR_STR
#define XENOSTR_STR "Not a STREAM"
#undef XENOSTR
#define XENOSTR 54

/* functionality not supported */
#undef XENOSYS_STR
#define XENOSYS_STR "Functionality not supported"
#undef XENOSYS
#define XENOSYS 55

/* the socket is not connected */
#undef XENOTCONN_STR
#define XENOTCONN_STR "The socket is not connected"
#undef XENOTCONN
#define XENOTCONN 56

/* not a directory or a symbolic link to a directory */
#undef XENOTDIR_STR
#define XENOTDIR_STR "Not a directory or a symbolic link to a directory"
#undef XENOTDIR
#define XENOTDIR 57

/* directory not empty */
#undef XENOTEMPTY_STR
#define XENOTEMPTY_STR "Directory not empty"
#undef XENOTEMPTY
#define XENOTEMPTY 58

/* state not recoverable */
#undef XENOTRECOVERABLE_STR
#define XENOTRECOVERABLE_STR "State not recoverable"
#undef XENOTRECOVERABLE
#define XENOTRECOVERABLE 59

/* not a socket */
#undef XENOTSOCK_STR
#define XENOTSOCK_STR "Not a socket"
#undef XENOTSOCK
#define XENOTSOCK 60

/* not supported */
#undef XENOTSUP_STR
#define XENOTSUP_STR "Not supported"
#undef XENOTSUP
#define XENOTSUP 61

/* inappropriate i/o control operation */
#undef XENOTTY_STR
#define XENOTTY_STR "Inappropriate I/O control operation"
#undef XENOTTY
#define XENOTTY 62

/* no such device or address */
#undef XENXIO_STR
#define XENXIO_STR "No such device or address"
#undef XENXIO
#define XENXIO 63

/* operation not supported on socket */
#undef XEOPNOTSUPP_STR
#define XEOPNOTSUPP_STR "Operation not supported on socket"
#undef XEOPNOTSUPP
#define XEOPNOTSUPP 64

/* value too large to be stored in data type */
#undef XEOVERFLOW_STR
#define XEOVERFLOW_STR "Value too large to be stored in data type"
#undef XEOVERFLOW
#define XEOVERFLOW 65

/* previous owner died */
#undef XEOWNERDEAD_STR
#define XEOWNERDEAD_STR "Previous owner died"
#undef XEOWNERDEAD
#define XEOWNERDEAD 66

/* operation not permitted */
#undef XEPERM_STR
#define XEPERM_STR "Operation not permitted"
#undef XEPERM
#define XEPERM 67

/* broken pipe */
#undef XEPIPE_STR
#define XEPIPE_STR "Broken pipe"
#undef XEPIPE
#define XEPIPE 68

/* protocol error */
#undef XEPROTO_STR
#define XEPROTO_STR "Protocol error"
#undef XEPROTO
#define XEPROTO 69

/* protocol not supported */
#undef XEPROTONOSUPPORT_STR
#define XEPROTONOSUPPORT_STR "Protocol not supported"
#undef XEPROTONOSUPPORT
#define XEPROTONOSUPPORT 70

/* protocol wrong type for socket */
#undef XEPROTOTYPE_STR
#define XEPROTOTYPE_STR "Protocol wrong type for socket"
#undef XEPROTOTYPE
#define XEPROTOTYPE 71

/* result too large */
#undef XERANGE_STR
#define XERANGE_STR "Result too large"
#undef XERANGE
#define XERANGE 72

/* read-only file system */
#undef XEROFS_STR
#define XEROFS_STR "Read-only file system"
#undef XEROFS
#define XEROFS 73

/* invalid seek */
#undef XESPIPE_STR
#define XESPIPE_STR "Invalid seek"
#undef XESPIPE
#define XESPIPE 74

/* no such process */
#undef XESRCH_STR
#define XESRCH_STR "No such process"
#undef XESRCH
#define XESRCH 75

/* reserved */
#undef XESTALE_STR
#define XESTALE_STR "<ESTALE Reserved>"
#undef XESTALE
#define XESTALE 76

/* stream ioctl() timeout */
#undef XETIME_STR
#define XETIME_STR "Stream ioctl() timeout"
#undef XETIME
#define XETIME 77

/* connection timed out */
#undef XETIMEDOUT_STR
#define XETIMEDOUT_STR "Connection timed out"
#undef XETIMEDOUT
#define XETIMEDOUT 78

/* text file busy */
#undef XETXTBSY_STR
#define XETXTBSY_STR "Text file busy"
#undef XETXTBSY
#define XETXTBSY 79

/* operation would block */
#undef XEWOULDBLOCK_STR
#define XEWOULDBLOCK_STR "Operation would block"
#undef XEWOULDBLOCK
#define XEWOULDBLOCK 80

/* cross-device link */
#undef XEXDEV_STR
#define XEXDEV_STR "Cross-device link"
#undef XEXDEV
#define XEXDEV 81

/* max (EOF) */
#undef XEMAX_STR
#define XEMAX_STR "<Unknown>"
#undef XEMAX
#define XEMAX 82
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
