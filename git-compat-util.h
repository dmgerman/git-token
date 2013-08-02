begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|GIT_COMPAT_UTIL_H
end_ifndef

begin_define
DECL|macro|GIT_COMPAT_UTIL_H
define|#
directive|define
name|GIT_COMPAT_UTIL_H
end_define

begin_define
DECL|macro|_FILE_OFFSET_BITS
define|#
directive|define
name|_FILE_OFFSET_BITS
value|64
end_define

begin_ifndef
ifndef|#
directive|ifndef
name|FLEX_ARRAY
end_ifndef

begin_comment
comment|/*  * See if our compiler is known to support flexible array members.  */
end_comment

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__STDC_VERSION__
argument_list|)
operator|&&
operator|(
name|__STDC_VERSION__
operator|>=
literal|199901L
operator|)
operator|&&
operator|(
operator|!
name|defined
argument_list|(
name|__SUNPRO_C
argument_list|)
operator|||
operator|(
name|__SUNPRO_C
operator|>
literal|0x580
operator|)
operator|)
end_if

begin_define
DECL|macro|FLEX_ARRAY
define|#
directive|define
name|FLEX_ARRAY
end_define

begin_comment
DECL|macro|FLEX_ARRAY
comment|/* empty */
end_comment

begin_elif
elif|#
directive|elif
name|defined
argument_list|(
name|__GNUC__
argument_list|)
end_elif

begin_if
if|#
directive|if
operator|(
name|__GNUC__
operator|>=
literal|3
operator|)
end_if

begin_define
DECL|macro|FLEX_ARRAY
define|#
directive|define
name|FLEX_ARRAY
end_define

begin_comment
DECL|macro|FLEX_ARRAY
comment|/* empty */
end_comment

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|FLEX_ARRAY
define|#
directive|define
name|FLEX_ARRAY
value|0
end_define

begin_comment
DECL|macro|FLEX_ARRAY
comment|/* older GNU extension */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  * Otherwise, default to safer but a bit wasteful traditional style  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|FLEX_ARRAY
end_ifndef

begin_define
DECL|macro|FLEX_ARRAY
define|#
directive|define
name|FLEX_ARRAY
value|1
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|ARRAY_SIZE
define|#
directive|define
name|ARRAY_SIZE
parameter_list|(
name|x
parameter_list|)
value|(sizeof(x)/sizeof(x[0]))
end_define

begin_define
DECL|macro|bitsizeof
define|#
directive|define
name|bitsizeof
parameter_list|(
name|x
parameter_list|)
value|(CHAR_BIT * sizeof(x))
end_define

begin_define
DECL|macro|maximum_signed_value_of_type
define|#
directive|define
name|maximum_signed_value_of_type
parameter_list|(
name|a
parameter_list|)
define|\
value|(INTMAX_MAX>> (bitsizeof(intmax_t) - bitsizeof(a)))
end_define

begin_define
DECL|macro|maximum_unsigned_value_of_type
define|#
directive|define
name|maximum_unsigned_value_of_type
parameter_list|(
name|a
parameter_list|)
define|\
value|(UINTMAX_MAX>> (bitsizeof(uintmax_t) - bitsizeof(a)))
end_define

begin_comment
comment|/*  * Signed integer overflow is undefined in C, so here's a helper macro  * to detect if the sum of two integers will overflow.  *  * Requires: a>= 0, typeof(a) equals typeof(b)  */
end_comment

begin_define
DECL|macro|signed_add_overflows
define|#
directive|define
name|signed_add_overflows
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
define|\
value|((b)> maximum_signed_value_of_type(a) - (a))
end_define

begin_define
DECL|macro|unsigned_add_overflows
define|#
directive|define
name|unsigned_add_overflows
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
define|\
value|((b)> maximum_unsigned_value_of_type(a) - (a))
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|__GNUC__
end_ifdef

begin_define
DECL|macro|TYPEOF
define|#
directive|define
name|TYPEOF
parameter_list|(
name|x
parameter_list|)
value|(__typeof__(x))
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|TYPEOF
define|#
directive|define
name|TYPEOF
parameter_list|(
name|x
parameter_list|)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|MSB
define|#
directive|define
name|MSB
parameter_list|(
name|x
parameter_list|,
name|bits
parameter_list|)
value|((x)& TYPEOF(x)(~0ULL<< (bitsizeof(x) - (bits))))
end_define

begin_define
DECL|macro|HAS_MULTI_BITS
define|#
directive|define
name|HAS_MULTI_BITS
parameter_list|(
name|i
parameter_list|)
value|((i)& ((i) - 1))
end_define

begin_comment
DECL|macro|HAS_MULTI_BITS
comment|/* checks if an integer has more than 1 bit set */
end_comment

begin_define
DECL|macro|DIV_ROUND_UP
define|#
directive|define
name|DIV_ROUND_UP
parameter_list|(
name|n
parameter_list|,
name|d
parameter_list|)
value|(((n) + (d) - 1) / (d))
end_define

begin_comment
comment|/* Approximation of the length of the decimal representation of this type. */
end_comment

begin_define
DECL|macro|decimal_length
define|#
directive|define
name|decimal_length
parameter_list|(
name|x
parameter_list|)
value|((int)(sizeof(x) * 2.56 + 0.5) + 1)
end_define

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__sun__
argument_list|)
end_if

begin_comment
comment|/*   * On Solaris, when _XOPEN_EXTENDED is set, its header file   * forces the programs to be XPG4v2, defeating any _XOPEN_SOURCE   * setting to say we are XPG5 or XPG6.  Also on Solaris,   * XPG6 programs must be compiled with a c99 compiler, while   * non XPG6 programs must be compiled with a pre-c99 compiler.   */
end_comment

begin_if
if|#
directive|if
name|__STDC_VERSION__
operator|-
literal|0
operator|>=
literal|199901L
end_if

begin_define
DECL|macro|_XOPEN_SOURCE
define|#
directive|define
name|_XOPEN_SOURCE
value|600
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|_XOPEN_SOURCE
define|#
directive|define
name|_XOPEN_SOURCE
value|500
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_elif
elif|#
directive|elif
operator|!
name|defined
argument_list|(
name|__APPLE__
argument_list|)
operator|&&
operator|!
name|defined
argument_list|(
name|__FreeBSD__
argument_list|)
operator|&&
operator|!
name|defined
argument_list|(
name|__USLC__
argument_list|)
operator|&&
expr|\
operator|!
name|defined
argument_list|(
name|_M_UNIX
argument_list|)
operator|&&
operator|!
name|defined
argument_list|(
name|__sgi
argument_list|)
operator|&&
operator|!
name|defined
argument_list|(
name|__DragonFly__
argument_list|)
operator|&&
expr|\
operator|!
name|defined
argument_list|(
name|__TANDEM
argument_list|)
operator|&&
operator|!
name|defined
argument_list|(
name|__QNX__
argument_list|)
end_elif

begin_define
DECL|macro|_XOPEN_SOURCE
define|#
directive|define
name|_XOPEN_SOURCE
value|600
end_define

begin_comment
DECL|macro|_XOPEN_SOURCE
comment|/* glibc2 and AIX 5.3L need 500, OpenBSD needs 600 for S_ISLNK() */
end_comment

begin_define
DECL|macro|_XOPEN_SOURCE_EXTENDED
define|#
directive|define
name|_XOPEN_SOURCE_EXTENDED
value|1
end_define

begin_comment
DECL|macro|_XOPEN_SOURCE_EXTENDED
comment|/* AIX 5.3L needs this */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|_ALL_SOURCE
define|#
directive|define
name|_ALL_SOURCE
value|1
end_define

begin_define
DECL|macro|_GNU_SOURCE
define|#
directive|define
name|_GNU_SOURCE
value|1
end_define

begin_define
DECL|macro|_BSD_SOURCE
define|#
directive|define
name|_BSD_SOURCE
value|1
end_define

begin_define
DECL|macro|_NETBSD_SOURCE
define|#
directive|define
name|_NETBSD_SOURCE
value|1
end_define

begin_define
DECL|macro|_SGI_SOURCE
define|#
directive|define
name|_SGI_SOURCE
value|1
end_define

begin_if
if|#
directive|if
name|defined
argument_list|(
name|WIN32
argument_list|)
operator|&&
operator|!
name|defined
argument_list|(
name|__CYGWIN__
argument_list|)
end_if

begin_comment
comment|/* Both MinGW and MSVC */
end_comment

begin_if
if|#
directive|if
name|defined
argument_list|(
name|_MSC_VER
argument_list|)
end_if

begin_define
DECL|macro|_WIN32_WINNT
define|#
directive|define
name|_WIN32_WINNT
value|0x0502
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|WIN32_LEAN_AND_MEAN
define|#
directive|define
name|WIN32_LEAN_AND_MEAN
end_define

begin_comment
DECL|macro|WIN32_LEAN_AND_MEAN
comment|/* stops windows.h including winsock.h */
end_comment

begin_include
include|#
directive|include
file|<winsock2.h>
end_include

begin_include
include|#
directive|include
file|<windows.h>
end_include

begin_define
DECL|macro|GIT_WINDOWS_NATIVE
define|#
directive|define
name|GIT_WINDOWS_NATIVE
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_include
include|#
directive|include
file|<unistd.h>
end_include

begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<sys/stat.h>
end_include

begin_include
include|#
directive|include
file|<fcntl.h>
end_include

begin_include
include|#
directive|include
file|<stddef.h>
end_include

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_include
include|#
directive|include
file|<stdarg.h>
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_ifdef
ifdef|#
directive|ifdef
name|HAVE_STRINGS_H
end_ifdef

begin_include
include|#
directive|include
file|<strings.h>
end_include

begin_comment
comment|/* for strcasecmp() */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_include
include|#
directive|include
file|<errno.h>
end_include

begin_include
include|#
directive|include
file|<limits.h>
end_include

begin_ifdef
ifdef|#
directive|ifdef
name|NEEDS_SYS_PARAM_H
end_ifdef

begin_include
include|#
directive|include
file|<sys/param.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_include
include|#
directive|include
file|<sys/types.h>
end_include

begin_include
include|#
directive|include
file|<dirent.h>
end_include

begin_include
include|#
directive|include
file|<sys/time.h>
end_include

begin_include
include|#
directive|include
file|<time.h>
end_include

begin_include
include|#
directive|include
file|<signal.h>
end_include

begin_ifndef
ifndef|#
directive|ifndef
name|USE_WILDMATCH
end_ifndef

begin_include
include|#
directive|include
file|<fnmatch.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_include
include|#
directive|include
file|<assert.h>
end_include

begin_include
include|#
directive|include
file|<regex.h>
end_include

begin_include
include|#
directive|include
file|<utime.h>
end_include

begin_include
include|#
directive|include
file|<syslog.h>
end_include

begin_ifndef
ifndef|#
directive|ifndef
name|NO_SYS_POLL_H
end_ifndef

begin_include
include|#
directive|include
file|<sys/poll.h>
end_include

begin_else
else|#
directive|else
end_else

begin_include
include|#
directive|include
file|<poll.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__MINGW32__
argument_list|)
end_if

begin_comment
comment|/* pull in Windows compatibility stuff */
end_comment

begin_include
include|#
directive|include
file|"compat/mingw.h"
end_include

begin_elif
elif|#
directive|elif
name|defined
argument_list|(
name|_MSC_VER
argument_list|)
end_elif

begin_include
include|#
directive|include
file|"compat/msvc.h"
end_include

begin_else
else|#
directive|else
end_else

begin_include
include|#
directive|include
file|<sys/wait.h>
end_include

begin_include
include|#
directive|include
file|<sys/resource.h>
end_include

begin_include
include|#
directive|include
file|<sys/socket.h>
end_include

begin_include
include|#
directive|include
file|<sys/ioctl.h>
end_include

begin_include
include|#
directive|include
file|<termios.h>
end_include

begin_ifndef
ifndef|#
directive|ifndef
name|NO_SYS_SELECT_H
end_ifndef

begin_include
include|#
directive|include
file|<sys/select.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_include
include|#
directive|include
file|<netinet/in.h>
end_include

begin_include
include|#
directive|include
file|<netinet/tcp.h>
end_include

begin_include
include|#
directive|include
file|<arpa/inet.h>
end_include

begin_include
include|#
directive|include
file|<netdb.h>
end_include

begin_include
include|#
directive|include
file|<pwd.h>
end_include

begin_include
include|#
directive|include
file|<sys/un.h>
end_include

begin_ifndef
ifndef|#
directive|ifndef
name|NO_INTTYPES_H
end_ifndef

begin_include
include|#
directive|include
file|<inttypes.h>
end_include

begin_else
else|#
directive|else
end_else

begin_include
include|#
directive|include
file|<stdint.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_INTPTR_T
end_ifdef

begin_comment
comment|/*  * On I16LP32, ILP32 and LP64 "long" is the save bet, however  * on LLP86, IL33LLP64 and P64 it needs to be "long long",  * while on IP16 and IP16L32 it is "int" (resp. "short")  * Size needs to match (or exceed) 'sizeof(void *)'.  * We can't take "long long" here as not everybody has it.  */
end_comment

begin_typedef
DECL|typedef|intptr_t
typedef|typedef
name|long
name|intptr_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|uintptr_t
typedef|typedef
name|unsigned
name|long
name|uintptr_t
typedef|;
end_typedef

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__CYGWIN__
argument_list|)
end_if

begin_undef
DECL|macro|_XOPEN_SOURCE
undef|#
directive|undef
name|_XOPEN_SOURCE
end_undef

begin_include
include|#
directive|include
file|<grp.h>
end_include

begin_define
DECL|macro|_XOPEN_SOURCE
define|#
directive|define
name|_XOPEN_SOURCE
value|600
end_define

begin_else
else|#
directive|else
end_else

begin_undef
DECL|macro|_ALL_SOURCE
undef|#
directive|undef
name|_ALL_SOURCE
end_undef

begin_comment
DECL|macro|_ALL_SOURCE
comment|/* AIX 5.3L defines a struct list with _ALL_SOURCE. */
end_comment

begin_include
include|#
directive|include
file|<grp.h>
end_include

begin_define
DECL|macro|_ALL_SOURCE
define|#
directive|define
name|_ALL_SOURCE
value|1
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* used on Mac OS X */
end_comment

begin_ifdef
ifdef|#
directive|ifdef
name|PRECOMPOSE_UNICODE
end_ifdef

begin_include
include|#
directive|include
file|"compat/precompose_utf8.h"
end_include

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|precompose_str
define|#
directive|define
name|precompose_str
parameter_list|(
name|in
parameter_list|,
name|i_nfd2nfc
parameter_list|)
end_define

begin_define
DECL|macro|precompose_argv
define|#
directive|define
name|precompose_argv
parameter_list|(
name|c
parameter_list|,
name|v
parameter_list|)
end_define

begin_define
DECL|macro|probe_utf8_pathname_composition
define|#
directive|define
name|probe_utf8_pathname_composition
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NEEDS_CLIPPED_WRITE
end_ifdef

begin_function_decl
name|ssize_t
name|clipped_write
parameter_list|(
name|int
name|fildes
parameter_list|,
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|nbyte
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|write
define|#
directive|define
name|write
parameter_list|(
name|x
parameter_list|,
name|y
parameter_list|,
name|z
parameter_list|)
value|clipped_write((x),(y),(z))
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|MKDIR_WO_TRAILING_SLASH
end_ifdef

begin_define
DECL|macro|mkdir
define|#
directive|define
name|mkdir
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
value|compat_mkdir_wo_trailing_slash((a),(b))
end_define

begin_function_decl
specifier|extern
name|int
name|compat_mkdir_wo_trailing_slash
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|mode_t
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_STRUCT_ITIMERVAL
end_ifdef

begin_struct
DECL|struct|itimerval
struct|struct
name|itimerval
block|{
DECL|member|it_interval
name|struct
name|timeval
name|it_interval
decl_stmt|;
DECL|member|it_value
name|struct
name|timeval
name|it_value
decl_stmt|;
block|}
endif|#
directive|endif
ifdef|#
directive|ifdef
name|NO_SETITIMER
DECL|macro|setitimer
define|#
directive|define
name|setitimer
parameter_list|(
name|which
parameter_list|,
name|value
parameter_list|,
name|ovalue
parameter_list|)
endif|#
directive|endif
ifndef|#
directive|ifndef
name|NO_LIBGEN_H
include|#
directive|include
file|<libgen.h>
else|#
directive|else
define|#
directive|define
name|basename
value|gitbasename
decl|extern
name|char
modifier|*
name|gitbasename
argument_list|(
name|char
operator|*
argument_list|)
struct|;
end_struct

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|NO_ICONV
end_ifndef

begin_include
include|#
directive|include
file|<iconv.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|NO_OPENSSL
end_ifndef

begin_include
include|#
directive|include
file|<openssl/ssl.h>
end_include

begin_include
include|#
directive|include
file|<openssl/err.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* On most systems<netdb.h> would have given us this, but  * not on some systems (e.g. z/OS).  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|NI_MAXHOST
end_ifndef

begin_define
DECL|macro|NI_MAXHOST
define|#
directive|define
name|NI_MAXHOST
value|1025
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|NI_MAXSERV
end_ifndef

begin_define
DECL|macro|NI_MAXSERV
define|#
directive|define
name|NI_MAXSERV
value|32
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* On most systems<limits.h> would have given us this, but  * not on some systems (e.g. GNU/Hurd).  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|PATH_MAX
end_ifndef

begin_define
DECL|macro|PATH_MAX
define|#
directive|define
name|PATH_MAX
value|4096
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|PRIuMAX
end_ifndef

begin_define
DECL|macro|PRIuMAX
define|#
directive|define
name|PRIuMAX
value|"llu"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|PRIu32
end_ifndef

begin_define
DECL|macro|PRIu32
define|#
directive|define
name|PRIu32
value|"u"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|PRIx32
end_ifndef

begin_define
DECL|macro|PRIx32
define|#
directive|define
name|PRIx32
value|"x"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|PRIo32
end_ifndef

begin_define
DECL|macro|PRIo32
define|#
directive|define
name|PRIo32
value|"o"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|PATH_SEP
end_ifndef

begin_define
DECL|macro|PATH_SEP
define|#
directive|define
name|PATH_SEP
value|':'
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|HAVE_PATHS_H
end_ifdef

begin_include
include|#
directive|include
file|<paths.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|_PATH_DEFPATH
end_ifndef

begin_define
DECL|macro|_PATH_DEFPATH
define|#
directive|define
name|_PATH_DEFPATH
value|"/usr/local/bin:/usr/bin:/bin"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|STRIP_EXTENSION
end_ifndef

begin_define
DECL|macro|STRIP_EXTENSION
define|#
directive|define
name|STRIP_EXTENSION
value|""
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|has_dos_drive_prefix
end_ifndef

begin_define
DECL|macro|has_dos_drive_prefix
define|#
directive|define
name|has_dos_drive_prefix
parameter_list|(
name|path
parameter_list|)
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|is_dir_sep
end_ifndef

begin_define
DECL|macro|is_dir_sep
define|#
directive|define
name|is_dir_sep
parameter_list|(
name|c
parameter_list|)
value|((c) == '/')
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|find_last_dir_sep
end_ifndef

begin_define
DECL|macro|find_last_dir_sep
define|#
directive|define
name|find_last_dir_sep
parameter_list|(
name|path
parameter_list|)
value|strrchr(path, '/')
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__HP_cc
argument_list|)
operator|&&
operator|(
name|__HP_cc
operator|>=
literal|61000
operator|)
end_if

begin_define
DECL|macro|NORETURN
define|#
directive|define
name|NORETURN
value|__attribute__((noreturn))
end_define

begin_define
DECL|macro|NORETURN_PTR
define|#
directive|define
name|NORETURN_PTR
end_define

begin_elif
elif|#
directive|elif
name|defined
argument_list|(
name|__GNUC__
argument_list|)
operator|&&
operator|!
name|defined
argument_list|(
name|NO_NORETURN
argument_list|)
end_elif

begin_define
define|#
directive|define
name|NORETURN
value|__attribute__((__noreturn__))
end_define

begin_define
define|#
directive|define
name|NORETURN_PTR
value|__attribute__((__noreturn__))
end_define

begin_elif
elif|#
directive|elif
name|defined
argument_list|(
name|_MSC_VER
argument_list|)
end_elif

begin_define
define|#
directive|define
name|NORETURN
value|__declspec(noreturn)
end_define

begin_define
define|#
directive|define
name|NORETURN_PTR
end_define

begin_else
else|#
directive|else
end_else

begin_define
define|#
directive|define
name|NORETURN
end_define

begin_define
define|#
directive|define
name|NORETURN_PTR
end_define

begin_ifndef
ifndef|#
directive|ifndef
name|__attribute__
end_ifndef

begin_define
define|#
directive|define
name|__attribute__
parameter_list|(
name|x
parameter_list|)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* The sentinel attribute is valid from gcc version 4.0 */
end_comment

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__GNUC__
argument_list|)
operator|&&
operator|(
name|__GNUC__
operator|>=
literal|4
operator|)
end_if

begin_define
DECL|macro|LAST_ARG_MUST_BE_NULL
define|#
directive|define
name|LAST_ARG_MUST_BE_NULL
value|__attribute__((sentinel))
end_define

begin_else
else|#
directive|else
end_else

begin_define
define|#
directive|define
name|LAST_ARG_MUST_BE_NULL
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_include
include|#
directive|include
file|"compat/bswap.h"
end_include

begin_ifdef
ifdef|#
directive|ifdef
name|USE_WILDMATCH
end_ifdef

begin_include
include|#
directive|include
file|"wildmatch.h"
end_include

begin_define
DECL|macro|FNM_PATHNAME
define|#
directive|define
name|FNM_PATHNAME
value|WM_PATHNAME
end_define

begin_define
DECL|macro|FNM_CASEFOLD
define|#
directive|define
name|FNM_CASEFOLD
value|WM_CASEFOLD
end_define

begin_define
DECL|macro|FNM_NOMATCH
define|#
directive|define
name|FNM_NOMATCH
value|WM_NOMATCH
end_define

begin_function
DECL|function|fnmatch
specifier|static
specifier|inline
name|int
name|fnmatch
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
return|return
name|wildmatch
argument_list|(
name|pattern
argument_list|,
name|string
argument_list|,
name|flags
argument_list|,
name|NULL
argument_list|)
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* General helper functions */
end_comment

begin_function_decl
specifier|extern
name|void
name|vreportf
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|vwritef
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|NORETURN
name|void
name|usage
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|NORETURN
name|void
name|usagef
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
function_decl|__attribute__
parameter_list|(
function_decl|(format
parameter_list|(
name|printf
parameter_list|,
function_decl|1
operator|,
function_decl|2
end_function_decl

begin_empty_stmt
unit|)))
empty_stmt|;
end_empty_stmt

begin_function_decl
specifier|extern
name|NORETURN
name|void
name|die
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
function_decl|__attribute__
parameter_list|(
function_decl|(format
parameter_list|(
name|printf
parameter_list|,
function_decl|1
operator|,
function_decl|2
end_function_decl

begin_empty_stmt
unit|)))
empty_stmt|;
end_empty_stmt

begin_function_decl
specifier|extern
name|NORETURN
name|void
name|die_errno
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
function_decl|__attribute__
parameter_list|(
function_decl|(format
parameter_list|(
name|printf
parameter_list|,
function_decl|1
operator|,
function_decl|2
end_function_decl

begin_empty_stmt
unit|)))
empty_stmt|;
end_empty_stmt

begin_function_decl
specifier|extern
name|int
name|error
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
function_decl|__attribute__
parameter_list|(
function_decl|(format
parameter_list|(
name|printf
parameter_list|,
function_decl|1
operator|,
function_decl|2
end_function_decl

begin_empty_stmt
unit|)))
empty_stmt|;
end_empty_stmt

begin_function_decl
specifier|extern
name|void
name|warning
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
function_decl|__attribute__
parameter_list|(
function_decl|(format
parameter_list|(
name|printf
parameter_list|,
function_decl|1
operator|,
function_decl|2
end_function_decl

begin_empty_stmt
unit|)))
empty_stmt|;
end_empty_stmt

begin_comment
comment|/*  * Let callers be aware of the constant return value; this can help  * gcc with -Wuninitialized analysis. We restrict this trick to gcc, though,  * because some compilers may not support variadic macros. Since we're only  * trying to help gcc, anyway, it's OK; other compilers will fall back to  * using the function as usual.  */
end_comment

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__GNUC__
argument_list|)
operator|&&
operator|!
name|defined
argument_list|(
name|__clang__
argument_list|)
end_if

begin_define
DECL|macro|error
define|#
directive|define
name|error
parameter_list|(
modifier|...
parameter_list|)
value|(error(__VA_ARGS__), -1)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
specifier|extern
name|void
name|set_die_routine
argument_list|(
name|NORETURN_PTR
name|void
argument_list|(
argument|*routine
argument_list|)
operator|(
specifier|const
name|char
operator|*
name|err
operator|,
name|va_list
name|params
operator|)
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|extern
name|void
name|set_error_routine
parameter_list|(
name|void
function_decl|(
modifier|*
name|routine
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|set_die_is_recursing_routine
parameter_list|(
name|int
function_decl|(
modifier|*
name|routine
function_decl|)
parameter_list|(
name|void
parameter_list|)
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|prefixcmp
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|suffixcmp
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
specifier|const
name|char
modifier|*
name|suffix
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|skip_prefix
specifier|static
specifier|inline
specifier|const
name|char
modifier|*
name|skip_prefix
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|size_t
name|len
init|=
name|strlen
argument_list|(
name|prefix
argument_list|)
decl_stmt|;
return|return
name|strncmp
argument_list|(
name|str
argument_list|,
name|prefix
argument_list|,
name|len
argument_list|)
condition|?
name|NULL
else|:
name|str
operator|+
name|len
return|;
block|}
end_function

begin_if
if|#
directive|if
name|defined
argument_list|(
name|NO_MMAP
argument_list|)
operator|||
name|defined
argument_list|(
name|USE_WIN32_MMAP
argument_list|)
end_if

begin_ifndef
ifndef|#
directive|ifndef
name|PROT_READ
end_ifndef

begin_define
DECL|macro|PROT_READ
define|#
directive|define
name|PROT_READ
value|1
end_define

begin_define
DECL|macro|PROT_WRITE
define|#
directive|define
name|PROT_WRITE
value|2
end_define

begin_define
DECL|macro|MAP_PRIVATE
define|#
directive|define
name|MAP_PRIVATE
value|1
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|mmap
define|#
directive|define
name|mmap
value|git_mmap
end_define

begin_define
DECL|macro|munmap
define|#
directive|define
name|munmap
value|git_munmap
end_define

begin_function_decl
specifier|extern
name|void
modifier|*
name|git_mmap
parameter_list|(
name|void
modifier|*
name|start
parameter_list|,
name|size_t
name|length
parameter_list|,
name|int
name|prot
parameter_list|,
name|int
name|flags
parameter_list|,
name|int
name|fd
parameter_list|,
name|off_t
name|offset
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|git_munmap
parameter_list|(
name|void
modifier|*
name|start
parameter_list|,
name|size_t
name|length
parameter_list|)
function_decl|;
end_function_decl

begin_else
else|#
directive|else
end_else

begin_comment
comment|/* NO_MMAP || USE_WIN32_MMAP */
end_comment

begin_include
include|#
directive|include
file|<sys/mman.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* NO_MMAP || USE_WIN32_MMAP */
end_comment

begin_ifdef
ifdef|#
directive|ifdef
name|NO_MMAP
end_ifdef

begin_comment
comment|/* This value must be multiple of (pagesize * 2) */
end_comment

begin_define
DECL|macro|DEFAULT_PACKED_GIT_WINDOW_SIZE
define|#
directive|define
name|DEFAULT_PACKED_GIT_WINDOW_SIZE
value|(1 * 1024 * 1024)
end_define

begin_else
else|#
directive|else
end_else

begin_comment
comment|/* NO_MMAP */
end_comment

begin_comment
comment|/* This value must be multiple of (pagesize * 2) */
end_comment

begin_define
DECL|macro|DEFAULT_PACKED_GIT_WINDOW_SIZE
define|#
directive|define
name|DEFAULT_PACKED_GIT_WINDOW_SIZE
define|\
value|(sizeof(void*)>= 8 \ 		?  1 * 1024 * 1024 * 1024 \ 		: 32 * 1024 * 1024)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* NO_MMAP */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|MAP_FAILED
end_ifndef

begin_define
DECL|macro|MAP_FAILED
define|#
directive|define
name|MAP_FAILED
value|((void *)-1)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_ST_BLOCKS_IN_STRUCT_STAT
end_ifdef

begin_define
DECL|macro|on_disk_bytes
define|#
directive|define
name|on_disk_bytes
parameter_list|(
name|st
parameter_list|)
value|((st).st_size)
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|on_disk_bytes
define|#
directive|define
name|on_disk_bytes
parameter_list|(
name|st
parameter_list|)
value|((st).st_blocks * 512)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|DEFAULT_PACKED_GIT_LIMIT
define|#
directive|define
name|DEFAULT_PACKED_GIT_LIMIT
define|\
value|((1024L * 1024L) * (sizeof(void*)>= 8 ? 8192 : 256))
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|NO_PREAD
end_ifdef

begin_define
DECL|macro|pread
define|#
directive|define
name|pread
value|git_pread
end_define

begin_function_decl
specifier|extern
name|ssize_t
name|git_pread
parameter_list|(
name|int
name|fd
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|count
parameter_list|,
name|off_t
name|offset
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  * Forward decl that will remind us if its twin in cache.h changes.  * This function is used in compat/pread.c.  But we can't include  * cache.h there.  */
end_comment

begin_function_decl
specifier|extern
name|ssize_t
name|read_in_full
parameter_list|(
name|int
name|fd
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|count
parameter_list|)
function_decl|;
end_function_decl

begin_ifdef
ifdef|#
directive|ifdef
name|NO_SETENV
end_ifdef

begin_define
DECL|macro|setenv
define|#
directive|define
name|setenv
value|gitsetenv
end_define

begin_function_decl
specifier|extern
name|int
name|gitsetenv
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_MKDTEMP
end_ifdef

begin_define
DECL|macro|mkdtemp
define|#
directive|define
name|mkdtemp
value|gitmkdtemp
end_define

begin_function_decl
specifier|extern
name|char
modifier|*
name|gitmkdtemp
parameter_list|(
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_MKSTEMPS
end_ifdef

begin_define
DECL|macro|mkstemps
define|#
directive|define
name|mkstemps
value|gitmkstemps
end_define

begin_function_decl
specifier|extern
name|int
name|gitmkstemps
parameter_list|(
name|char
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_UNSETENV
end_ifdef

begin_define
DECL|macro|unsetenv
define|#
directive|define
name|unsetenv
value|gitunsetenv
end_define

begin_function_decl
specifier|extern
name|void
name|gitunsetenv
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_STRCASESTR
end_ifdef

begin_define
DECL|macro|strcasestr
define|#
directive|define
name|strcasestr
value|gitstrcasestr
end_define

begin_function_decl
specifier|extern
name|char
modifier|*
name|gitstrcasestr
parameter_list|(
specifier|const
name|char
modifier|*
name|haystack
parameter_list|,
specifier|const
name|char
modifier|*
name|needle
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_STRLCPY
end_ifdef

begin_define
DECL|macro|strlcpy
define|#
directive|define
name|strlcpy
value|gitstrlcpy
end_define

begin_function_decl
specifier|extern
name|size_t
name|gitstrlcpy
parameter_list|(
name|char
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|size_t
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_STRTOUMAX
end_ifdef

begin_define
DECL|macro|strtoumax
define|#
directive|define
name|strtoumax
value|gitstrtoumax
end_define

begin_function_decl
specifier|extern
name|uintmax_t
name|gitstrtoumax
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|char
modifier|*
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|strtoimax
define|#
directive|define
name|strtoimax
value|gitstrtoimax
end_define

begin_function_decl
specifier|extern
name|intmax_t
name|gitstrtoimax
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|char
modifier|*
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_HSTRERROR
end_ifdef

begin_define
DECL|macro|hstrerror
define|#
directive|define
name|hstrerror
value|githstrerror
end_define

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|githstrerror
parameter_list|(
name|int
name|herror
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_MEMMEM
end_ifdef

begin_define
DECL|macro|memmem
define|#
directive|define
name|memmem
value|gitmemmem
end_define

begin_function_decl
name|void
modifier|*
name|gitmemmem
parameter_list|(
specifier|const
name|void
modifier|*
name|haystack
parameter_list|,
name|size_t
name|haystacklen
parameter_list|,
specifier|const
name|void
modifier|*
name|needle
parameter_list|,
name|size_t
name|needlelen
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_GETPAGESIZE
end_ifdef

begin_define
DECL|macro|getpagesize
define|#
directive|define
name|getpagesize
parameter_list|()
value|sysconf(_SC_PAGESIZE)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|FREAD_READS_DIRECTORIES
end_ifdef

begin_ifdef
ifdef|#
directive|ifdef
name|fopen
end_ifdef

begin_undef
DECL|macro|fopen
undef|#
directive|undef
name|fopen
end_undef

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|fopen
define|#
directive|define
name|fopen
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
value|git_fopen(a,b)
end_define

begin_function_decl
specifier|extern
name|FILE
modifier|*
name|git_fopen
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|SNPRINTF_RETURNS_BOGUS
end_ifdef

begin_define
DECL|macro|snprintf
define|#
directive|define
name|snprintf
value|git_snprintf
end_define

begin_function_decl
specifier|extern
name|int
name|git_snprintf
parameter_list|(
name|char
modifier|*
name|str
parameter_list|,
name|size_t
name|maxsize
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|vsnprintf
define|#
directive|define
name|vsnprintf
value|git_vsnprintf
end_define

begin_function_decl
specifier|extern
name|int
name|git_vsnprintf
parameter_list|(
name|char
modifier|*
name|str
parameter_list|,
name|size_t
name|maxsize
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
name|va_list
name|ap
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|__GLIBC_PREREQ
end_ifdef

begin_if
if|#
directive|if
name|__GLIBC_PREREQ
argument_list|(
literal|2
operator|,
literal|1
argument_list|)
end_if

begin_define
DECL|macro|HAVE_STRCHRNUL
define|#
directive|define
name|HAVE_STRCHRNUL
end_define

begin_define
DECL|macro|HAVE_MEMPCPY
define|#
directive|define
name|HAVE_MEMPCPY
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|HAVE_STRCHRNUL
end_ifndef

begin_define
DECL|macro|strchrnul
define|#
directive|define
name|strchrnul
value|gitstrchrnul
end_define

begin_function
DECL|function|gitstrchrnul
specifier|static
specifier|inline
name|char
modifier|*
name|gitstrchrnul
parameter_list|(
specifier|const
name|char
modifier|*
name|s
parameter_list|,
name|int
name|c
parameter_list|)
block|{
while|while
condition|(
operator|*
name|s
operator|&&
operator|*
name|s
operator|!=
name|c
condition|)
name|s
operator|++
expr_stmt|;
return|return
operator|(
name|char
operator|*
operator|)
name|s
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|HAVE_MEMPCPY
end_ifndef

begin_define
DECL|macro|mempcpy
define|#
directive|define
name|mempcpy
value|gitmempcpy
end_define

begin_function
DECL|function|gitmempcpy
specifier|static
specifier|inline
name|void
modifier|*
name|gitmempcpy
parameter_list|(
name|void
modifier|*
name|dest
parameter_list|,
specifier|const
name|void
modifier|*
name|src
parameter_list|,
name|size_t
name|n
parameter_list|)
block|{
return|return
operator|(
name|char
operator|*
operator|)
name|memcpy
argument_list|(
name|dest
argument_list|,
name|src
argument_list|,
name|n
argument_list|)
operator|+
name|n
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_INET_PTON
end_ifdef

begin_function_decl
name|int
name|inet_pton
parameter_list|(
name|int
name|af
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|,
name|void
modifier|*
name|dst
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_INET_NTOP
end_ifdef

begin_function_decl
specifier|const
name|char
modifier|*
name|inet_ntop
parameter_list|(
name|int
name|af
parameter_list|,
specifier|const
name|void
modifier|*
name|src
parameter_list|,
name|char
modifier|*
name|dst
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
specifier|extern
name|void
name|release_pack_memory
parameter_list|(
name|size_t
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_typedef
DECL|typedef|try_to_free_t
typedef|typedef
name|void
function_decl|(
modifier|*
name|try_to_free_t
function_decl|)
parameter_list|(
name|size_t
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
specifier|extern
name|try_to_free_t
name|set_try_to_free_routine
parameter_list|(
name|try_to_free_t
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|xstrdup
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|xmalloc
parameter_list|(
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|xmallocz
parameter_list|(
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|xmemdupz
parameter_list|(
specifier|const
name|void
modifier|*
name|data
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|xstrndup
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|xrealloc
parameter_list|(
name|void
modifier|*
name|ptr
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|xcalloc
parameter_list|(
name|size_t
name|nmemb
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|xmmap
parameter_list|(
name|void
modifier|*
name|start
parameter_list|,
name|size_t
name|length
parameter_list|,
name|int
name|prot
parameter_list|,
name|int
name|flags
parameter_list|,
name|int
name|fd
parameter_list|,
name|off_t
name|offset
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|ssize_t
name|xread
parameter_list|(
name|int
name|fd
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|ssize_t
name|xwrite
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|xdup
parameter_list|(
name|int
name|fd
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|FILE
modifier|*
name|xfdopen
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|char
modifier|*
name|mode
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|xmkstemp
parameter_list|(
name|char
modifier|*
name|template
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|xmkstemp_mode
parameter_list|(
name|char
modifier|*
name|template
parameter_list|,
name|int
name|mode
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|odb_mkstemp
parameter_list|(
name|char
modifier|*
name|template
parameter_list|,
name|size_t
name|limit
parameter_list|,
specifier|const
name|char
modifier|*
name|pattern
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|odb_pack_keep
parameter_list|(
name|char
modifier|*
name|name
parameter_list|,
name|size_t
name|namesz
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|xsize_t
specifier|static
specifier|inline
name|size_t
name|xsize_t
parameter_list|(
name|off_t
name|len
parameter_list|)
block|{
if|if
condition|(
name|len
operator|>
operator|(
name|size_t
operator|)
name|len
condition|)
name|die
argument_list|(
literal|"Cannot handle files this big"
argument_list|)
expr_stmt|;
return|return
operator|(
name|size_t
operator|)
name|len
return|;
block|}
end_function

begin_function
DECL|function|has_extension
specifier|static
specifier|inline
name|int
name|has_extension
parameter_list|(
specifier|const
name|char
modifier|*
name|filename
parameter_list|,
specifier|const
name|char
modifier|*
name|ext
parameter_list|)
block|{
name|size_t
name|len
init|=
name|strlen
argument_list|(
name|filename
argument_list|)
decl_stmt|;
name|size_t
name|extlen
init|=
name|strlen
argument_list|(
name|ext
argument_list|)
decl_stmt|;
return|return
name|len
operator|>
name|extlen
operator|&&
operator|!
name|memcmp
argument_list|(
name|filename
operator|+
name|len
operator|-
name|extlen
argument_list|,
name|ext
argument_list|,
name|extlen
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* in ctype.c, for kwset users */
end_comment

begin_decl_stmt
specifier|extern
specifier|const
name|char
name|tolower_trans_tbl
index|[
literal|256
index|]
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Sane ctype - no locale, and works with signed chars */
end_comment

begin_undef
DECL|macro|isascii
undef|#
directive|undef
name|isascii
end_undef

begin_undef
DECL|macro|isspace
undef|#
directive|undef
name|isspace
end_undef

begin_undef
DECL|macro|isdigit
undef|#
directive|undef
name|isdigit
end_undef

begin_undef
DECL|macro|isalpha
undef|#
directive|undef
name|isalpha
end_undef

begin_undef
DECL|macro|isalnum
undef|#
directive|undef
name|isalnum
end_undef

begin_undef
DECL|macro|isprint
undef|#
directive|undef
name|isprint
end_undef

begin_undef
DECL|macro|islower
undef|#
directive|undef
name|islower
end_undef

begin_undef
DECL|macro|isupper
undef|#
directive|undef
name|isupper
end_undef

begin_undef
DECL|macro|tolower
undef|#
directive|undef
name|tolower
end_undef

begin_undef
DECL|macro|toupper
undef|#
directive|undef
name|toupper
end_undef

begin_undef
DECL|macro|iscntrl
undef|#
directive|undef
name|iscntrl
end_undef

begin_undef
DECL|macro|ispunct
undef|#
directive|undef
name|ispunct
end_undef

begin_undef
DECL|macro|isxdigit
undef|#
directive|undef
name|isxdigit
end_undef

begin_decl_stmt
specifier|extern
specifier|const
name|unsigned
name|char
name|sane_ctype
index|[
literal|256
index|]
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|GIT_SPACE
define|#
directive|define
name|GIT_SPACE
value|0x01
end_define

begin_define
DECL|macro|GIT_DIGIT
define|#
directive|define
name|GIT_DIGIT
value|0x02
end_define

begin_define
DECL|macro|GIT_ALPHA
define|#
directive|define
name|GIT_ALPHA
value|0x04
end_define

begin_define
DECL|macro|GIT_GLOB_SPECIAL
define|#
directive|define
name|GIT_GLOB_SPECIAL
value|0x08
end_define

begin_define
DECL|macro|GIT_REGEX_SPECIAL
define|#
directive|define
name|GIT_REGEX_SPECIAL
value|0x10
end_define

begin_define
DECL|macro|GIT_PATHSPEC_MAGIC
define|#
directive|define
name|GIT_PATHSPEC_MAGIC
value|0x20
end_define

begin_define
DECL|macro|GIT_CNTRL
define|#
directive|define
name|GIT_CNTRL
value|0x40
end_define

begin_define
DECL|macro|GIT_PUNCT
define|#
directive|define
name|GIT_PUNCT
value|0x80
end_define

begin_define
DECL|macro|sane_istest
define|#
directive|define
name|sane_istest
parameter_list|(
name|x
parameter_list|,
name|mask
parameter_list|)
value|((sane_ctype[(unsigned char)(x)]& (mask)) != 0)
end_define

begin_define
DECL|macro|isascii
define|#
directive|define
name|isascii
parameter_list|(
name|x
parameter_list|)
value|(((x)& ~0x7f) == 0)
end_define

begin_define
DECL|macro|isspace
define|#
directive|define
name|isspace
parameter_list|(
name|x
parameter_list|)
value|sane_istest(x,GIT_SPACE)
end_define

begin_define
DECL|macro|isdigit
define|#
directive|define
name|isdigit
parameter_list|(
name|x
parameter_list|)
value|sane_istest(x,GIT_DIGIT)
end_define

begin_define
DECL|macro|isalpha
define|#
directive|define
name|isalpha
parameter_list|(
name|x
parameter_list|)
value|sane_istest(x,GIT_ALPHA)
end_define

begin_define
DECL|macro|isalnum
define|#
directive|define
name|isalnum
parameter_list|(
name|x
parameter_list|)
value|sane_istest(x,GIT_ALPHA | GIT_DIGIT)
end_define

begin_define
DECL|macro|isprint
define|#
directive|define
name|isprint
parameter_list|(
name|x
parameter_list|)
value|((x)>= 0x20&& (x)<= 0x7e)
end_define

begin_define
DECL|macro|islower
define|#
directive|define
name|islower
parameter_list|(
name|x
parameter_list|)
value|sane_iscase(x, 1)
end_define

begin_define
DECL|macro|isupper
define|#
directive|define
name|isupper
parameter_list|(
name|x
parameter_list|)
value|sane_iscase(x, 0)
end_define

begin_define
DECL|macro|is_glob_special
define|#
directive|define
name|is_glob_special
parameter_list|(
name|x
parameter_list|)
value|sane_istest(x,GIT_GLOB_SPECIAL)
end_define

begin_define
DECL|macro|is_regex_special
define|#
directive|define
name|is_regex_special
parameter_list|(
name|x
parameter_list|)
value|sane_istest(x,GIT_GLOB_SPECIAL | GIT_REGEX_SPECIAL)
end_define

begin_define
DECL|macro|iscntrl
define|#
directive|define
name|iscntrl
parameter_list|(
name|x
parameter_list|)
value|(sane_istest(x,GIT_CNTRL))
end_define

begin_define
DECL|macro|ispunct
define|#
directive|define
name|ispunct
parameter_list|(
name|x
parameter_list|)
value|sane_istest(x, GIT_PUNCT | GIT_REGEX_SPECIAL | \ 		GIT_GLOB_SPECIAL | GIT_PATHSPEC_MAGIC)
end_define

begin_define
DECL|macro|isxdigit
define|#
directive|define
name|isxdigit
parameter_list|(
name|x
parameter_list|)
value|(hexval_table[x] != -1)
end_define

begin_define
DECL|macro|tolower
define|#
directive|define
name|tolower
parameter_list|(
name|x
parameter_list|)
value|sane_case((unsigned char)(x), 0x20)
end_define

begin_define
DECL|macro|toupper
define|#
directive|define
name|toupper
parameter_list|(
name|x
parameter_list|)
value|sane_case((unsigned char)(x), 0)
end_define

begin_define
DECL|macro|is_pathspec_magic
define|#
directive|define
name|is_pathspec_magic
parameter_list|(
name|x
parameter_list|)
value|sane_istest(x,GIT_PATHSPEC_MAGIC)
end_define

begin_function
DECL|function|sane_case
specifier|static
specifier|inline
name|int
name|sane_case
parameter_list|(
name|int
name|x
parameter_list|,
name|int
name|high
parameter_list|)
block|{
if|if
condition|(
name|sane_istest
argument_list|(
name|x
argument_list|,
name|GIT_ALPHA
argument_list|)
condition|)
name|x
operator|=
operator|(
name|x
operator|&
operator|~
literal|0x20
operator|)
operator||
name|high
expr_stmt|;
return|return
name|x
return|;
block|}
end_function

begin_function
DECL|function|sane_iscase
specifier|static
specifier|inline
name|int
name|sane_iscase
parameter_list|(
name|int
name|x
parameter_list|,
name|int
name|is_lower
parameter_list|)
block|{
if|if
condition|(
operator|!
name|sane_istest
argument_list|(
name|x
argument_list|,
name|GIT_ALPHA
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|is_lower
condition|)
return|return
operator|(
name|x
operator|&
literal|0x20
operator|)
operator|!=
literal|0
return|;
else|else
return|return
operator|(
name|x
operator|&
literal|0x20
operator|)
operator|==
literal|0
return|;
block|}
end_function

begin_function
DECL|function|strtoul_ui
specifier|static
specifier|inline
name|int
name|strtoul_ui
parameter_list|(
name|char
specifier|const
modifier|*
name|s
parameter_list|,
name|int
name|base
parameter_list|,
name|unsigned
name|int
modifier|*
name|result
parameter_list|)
block|{
name|unsigned
name|long
name|ul
decl_stmt|;
name|char
modifier|*
name|p
decl_stmt|;
name|errno
operator|=
literal|0
expr_stmt|;
name|ul
operator|=
name|strtoul
argument_list|(
name|s
argument_list|,
operator|&
name|p
argument_list|,
name|base
argument_list|)
expr_stmt|;
if|if
condition|(
name|errno
operator|||
operator|*
name|p
operator|||
name|p
operator|==
name|s
operator|||
operator|(
name|unsigned
name|int
operator|)
name|ul
operator|!=
name|ul
condition|)
return|return
operator|-
literal|1
return|;
operator|*
name|result
operator|=
name|ul
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|strtol_i
specifier|static
specifier|inline
name|int
name|strtol_i
parameter_list|(
name|char
specifier|const
modifier|*
name|s
parameter_list|,
name|int
name|base
parameter_list|,
name|int
modifier|*
name|result
parameter_list|)
block|{
name|long
name|ul
decl_stmt|;
name|char
modifier|*
name|p
decl_stmt|;
name|errno
operator|=
literal|0
expr_stmt|;
name|ul
operator|=
name|strtol
argument_list|(
name|s
argument_list|,
operator|&
name|p
argument_list|,
name|base
argument_list|)
expr_stmt|;
if|if
condition|(
name|errno
operator|||
operator|*
name|p
operator|||
name|p
operator|==
name|s
operator|||
operator|(
name|int
operator|)
name|ul
operator|!=
name|ul
condition|)
return|return
operator|-
literal|1
return|;
operator|*
name|result
operator|=
name|ul
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_ifdef
ifdef|#
directive|ifdef
name|INTERNAL_QSORT
end_ifdef

begin_function_decl
name|void
name|git_qsort
parameter_list|(
name|void
modifier|*
name|base
parameter_list|,
name|size_t
name|nmemb
parameter_list|,
name|size_t
name|size
parameter_list|,
name|int
function_decl|(
modifier|*
name|compar
function_decl|)
parameter_list|(
specifier|const
name|void
modifier|*
parameter_list|,
specifier|const
name|void
modifier|*
parameter_list|)
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|qsort
define|#
directive|define
name|qsort
value|git_qsort
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|DIR_HAS_BSD_GROUP_SEMANTICS
end_ifndef

begin_define
DECL|macro|FORCE_DIR_SET_GID
define|#
directive|define
name|FORCE_DIR_SET_GID
value|S_ISGID
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|FORCE_DIR_SET_GID
define|#
directive|define
name|FORCE_DIR_SET_GID
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_NSEC
end_ifdef

begin_undef
DECL|macro|USE_NSEC
undef|#
directive|undef
name|USE_NSEC
end_undef

begin_define
DECL|macro|ST_CTIME_NSEC
define|#
directive|define
name|ST_CTIME_NSEC
parameter_list|(
name|st
parameter_list|)
value|0
end_define

begin_define
DECL|macro|ST_MTIME_NSEC
define|#
directive|define
name|ST_MTIME_NSEC
parameter_list|(
name|st
parameter_list|)
value|0
end_define

begin_else
else|#
directive|else
end_else

begin_ifdef
ifdef|#
directive|ifdef
name|USE_ST_TIMESPEC
end_ifdef

begin_define
DECL|macro|ST_CTIME_NSEC
define|#
directive|define
name|ST_CTIME_NSEC
parameter_list|(
name|st
parameter_list|)
value|((unsigned int)((st).st_ctimespec.tv_nsec))
end_define

begin_define
DECL|macro|ST_MTIME_NSEC
define|#
directive|define
name|ST_MTIME_NSEC
parameter_list|(
name|st
parameter_list|)
value|((unsigned int)((st).st_mtimespec.tv_nsec))
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|ST_CTIME_NSEC
define|#
directive|define
name|ST_CTIME_NSEC
parameter_list|(
name|st
parameter_list|)
value|((unsigned int)((st).st_ctim.tv_nsec))
end_define

begin_define
DECL|macro|ST_MTIME_NSEC
define|#
directive|define
name|ST_MTIME_NSEC
parameter_list|(
name|st
parameter_list|)
value|((unsigned int)((st).st_mtim.tv_nsec))
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|UNRELIABLE_FSTAT
end_ifdef

begin_define
DECL|macro|fstat_is_reliable
define|#
directive|define
name|fstat_is_reliable
parameter_list|()
value|0
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|fstat_is_reliable
define|#
directive|define
name|fstat_is_reliable
parameter_list|()
value|1
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|va_copy
end_ifndef

begin_comment
comment|/*  * Since an obvious implementation of va_list would be to make it a  * pointer into the stack frame, a simple assignment will work on  * many systems.  But let's try to be more portable.  */
end_comment

begin_ifdef
ifdef|#
directive|ifdef
name|__va_copy
end_ifdef

begin_define
DECL|macro|va_copy
define|#
directive|define
name|va_copy
parameter_list|(
name|dst
parameter_list|,
name|src
parameter_list|)
value|__va_copy(dst, src)
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|va_copy
define|#
directive|define
name|va_copy
parameter_list|(
name|dst
parameter_list|,
name|src
parameter_list|)
value|((dst) = (src))
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  * Preserves errno, prints a message, but gives no warning for ENOENT.  * Always returns the return value of unlink(2).  */
end_comment

begin_function_decl
name|int
name|unlink_or_warn
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Likewise for rmdir(2).  */
end_comment

begin_function_decl
name|int
name|rmdir_or_warn
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Calls the correct function out of {unlink,rmdir}_or_warn based on  * the supplied file mode.  */
end_comment

begin_function_decl
name|int
name|remove_or_warn
parameter_list|(
name|unsigned
name|int
name|mode
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Call access(2), but warn for any error except "missing file"  * (ENOENT or ENOTDIR).  */
end_comment

begin_define
DECL|macro|ACCESS_EACCES_OK
define|#
directive|define
name|ACCESS_EACCES_OK
value|(1U<< 0)
end_define

begin_function_decl
name|int
name|access_or_warn
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|mode
parameter_list|,
name|unsigned
name|flag
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|access_or_die
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|mode
parameter_list|,
name|unsigned
name|flag
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Warn on an inaccessible file that ought to be accessible */
end_comment

begin_function_decl
name|void
name|warn_on_inaccessible
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Get the passwd entry for the UID of the current process. */
end_comment

begin_function_decl
name|struct
name|passwd
modifier|*
name|xgetpwuid_self
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

