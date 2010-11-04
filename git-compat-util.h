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

begin_ifdef
ifdef|#
directive|ifdef
name|WIN32
end_ifdef

begin_comment
comment|/* Both MinGW and MSVC */
end_comment

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

begin_include
include|#
directive|include
file|<sys/param.h>
end_include

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

begin_include
include|#
directive|include
file|<fnmatch.h>
end_include

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
name|__MINGW32__
end_ifndef

begin_include
include|#
directive|include
file|<sys/wait.h>
end_include

begin_include
include|#
directive|include
file|<sys/poll.h>
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
file|<inttypes.h>
end_include

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

begin_include
include|#
directive|include
file|"compat/cygwin.h"
end_include

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

begin_else
else|#
directive|else
end_else

begin_comment
comment|/* __MINGW32__ */
end_comment

begin_comment
comment|/* pull in Windows compatibility stuff */
end_comment

begin_include
include|#
directive|include
file|"compat/mingw.h"
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* __MINGW32__ */
end_comment

begin_ifdef
ifdef|#
directive|ifdef
name|_MSC_VER
end_ifdef

begin_include
include|#
directive|include
file|"compat/msvc.h"
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|NO_LIBGEN_H
end_ifndef

begin_include
include|#
directive|include
file|<libgen.h>
end_include

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|basename
define|#
directive|define
name|basename
value|gitbasename
end_define

begin_function_decl
specifier|extern
name|char
modifier|*
name|gitbasename
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

begin_ifdef
ifdef|#
directive|ifdef
name|__GNUC__
end_ifdef

begin_define
DECL|macro|NORETURN
define|#
directive|define
name|NORETURN
value|__attribute__((__noreturn__))
end_define

begin_define
DECL|macro|NORETURN_PTR
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
DECL|macro|NORETURN
define|#
directive|define
name|NORETURN
value|__declspec(noreturn)
end_define

begin_define
DECL|macro|NORETURN_PTR
define|#
directive|define
name|NORETURN_PTR
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|NORETURN
define|#
directive|define
name|NORETURN
end_define

begin_define
DECL|macro|NORETURN_PTR
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
DECL|macro|__attribute__
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

begin_include
include|#
directive|include
file|"compat/bswap.h"
end_include

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

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_STRTOK_R
end_ifdef

begin_define
DECL|macro|strtok_r
define|#
directive|define
name|strtok_r
value|gitstrtok_r
end_define

begin_function_decl
specifier|extern
name|char
modifier|*
name|gitstrtok_r
parameter_list|(
name|char
modifier|*
name|s
parameter_list|,
specifier|const
name|char
modifier|*
name|delim
parameter_list|,
name|char
modifier|*
modifier|*
name|save_ptr
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

begin_decl_stmt
specifier|extern
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

begin_endif
endif|#
directive|endif
end_endif

end_unit

