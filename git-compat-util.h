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

begin_comment
comment|/* Derived from Linux "Features Test Macro" header  * Convenience macros to test the versions of gcc (or  * a compatible compiler).  * Use them like this:  *  #if GIT_GNUC_PREREQ (2,8)  *   ... code requiring gcc 2.8 or later ...  *  #endif */
end_comment

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__GNUC__
argument_list|)
operator|&&
name|defined
argument_list|(
name|__GNUC_MINOR__
argument_list|)
end_if

begin_define
DECL|macro|GIT_GNUC_PREREQ
define|#
directive|define
name|GIT_GNUC_PREREQ
parameter_list|(
name|maj
parameter_list|,
name|min
parameter_list|)
define|\
value|((__GNUC__<< 16) + __GNUC_MINOR__>= ((maj)<< 16) + (min))
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|GIT_GNUC_PREREQ
define|#
directive|define
name|GIT_GNUC_PREREQ
parameter_list|(
name|maj
parameter_list|,
name|min
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

begin_comment
comment|/*  * BUILD_ASSERT_OR_ZERO - assert a build-time dependency, as an expression.  * @cond: the compile-time condition which must be true.  *  * Your compile will fail if the condition isn't true, or can't be evaluated  * by the compiler.  This can be used in an expression: its value is "0".  *  * Example:  *	#define foo_to_char(foo)					\  *		 ((char *)(foo)						\  *		  + BUILD_ASSERT_OR_ZERO(offsetof(struct foo, string) == 0))  */
end_comment

begin_define
DECL|macro|BUILD_ASSERT_OR_ZERO
define|#
directive|define
name|BUILD_ASSERT_OR_ZERO
parameter_list|(
name|cond
parameter_list|)
define|\
value|(sizeof(char [1 - 2*!(cond)]) - 1)
end_define

begin_if
if|#
directive|if
name|GIT_GNUC_PREREQ
argument_list|(
literal|3
operator|,
literal|1
argument_list|)
end_if

begin_comment
comment|/*&arr[0] degrades to a pointer: a different type from an array */
end_comment

begin_define
DECL|macro|BARF_UNLESS_AN_ARRAY
define|#
directive|define
name|BARF_UNLESS_AN_ARRAY
parameter_list|(
name|arr
parameter_list|)
define|\
value|BUILD_ASSERT_OR_ZERO(!__builtin_types_compatible_p(__typeof__(arr), \ 							   __typeof__(&(arr)[0])))
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|BARF_UNLESS_AN_ARRAY
define|#
directive|define
name|BARF_UNLESS_AN_ARRAY
parameter_list|(
name|arr
parameter_list|)
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  * ARRAY_SIZE - get the number of elements in a visible array  *<at> x: the array whose size you want.  *  * This does not work on pointers, or arrays declared as [], or  * function parameters.  With correct compiler support, such usage  * will cause a build error (see the build_assert_or_zero macro).  */
end_comment

begin_define
DECL|macro|ARRAY_SIZE
define|#
directive|define
name|ARRAY_SIZE
parameter_list|(
name|x
parameter_list|)
value|(sizeof(x) / sizeof((x)[0]) + BARF_UNLESS_AN_ARRAY(x))
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

begin_comment
comment|/*  * Returns true if the multiplication of "a" and "b" will  * overflow. The types of "a" and "b" must match and must be unsigned.  * Note that this macro evaluates "a" twice!  */
end_comment

begin_define
DECL|macro|unsigned_mult_overflows
define|#
directive|define
name|unsigned_mult_overflows
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
define|\
value|((a)&& (b)> maximum_unsigned_value_of_type(a) / (a))
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
operator|&&
operator|!
name|defined
argument_list|(
name|__MirBSD__
argument_list|)
operator|&&
expr|\
operator|!
name|defined
argument_list|(
name|__CYGWIN__
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
DECL|macro|_DEFAULT_SOURCE
define|#
directive|define
name|_DEFAULT_SOURCE
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
operator|&&
operator|!
name|defined
argument_list|(
name|_WIN32_WINNT
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

begin_ifdef
ifdef|#
directive|ifdef
name|HAVE_BSD_SYSCTL
end_ifdef

begin_include
include|#
directive|include
file|<sys/sysctl.h>
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
file|<sys/utsname.h>
end_include

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
parameter_list|()
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
struct|;
end_struct

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_SETITIMER
end_ifdef

begin_define
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
end_define

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

begin_define
DECL|macro|dirname
define|#
directive|define
name|dirname
value|gitdirname
end_define

begin_function_decl
specifier|extern
name|char
modifier|*
name|gitdirname
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

begin_ifdef
ifdef|#
directive|ifdef
name|__APPLE__
end_ifdef

begin_define
DECL|macro|__AVAILABILITY_MACROS_USES_AVAILABILITY
define|#
directive|define
name|__AVAILABILITY_MACROS_USES_AVAILABILITY
value|0
end_define

begin_include
include|#
directive|include
file|<AvailabilityMacros.h>
end_include

begin_undef
DECL|macro|DEPRECATED_ATTRIBUTE
undef|#
directive|undef
name|DEPRECATED_ATTRIBUTE
end_undef

begin_define
DECL|macro|DEPRECATED_ATTRIBUTE
define|#
directive|define
name|DEPRECATED_ATTRIBUTE
end_define

begin_undef
DECL|macro|__AVAILABILITY_MACROS_USES_AVAILABILITY
undef|#
directive|undef
name|__AVAILABILITY_MACROS_USES_AVAILABILITY
end_undef

begin_endif
endif|#
directive|endif
end_endif

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
name|SCNuMAX
end_ifndef

begin_define
DECL|macro|SCNuMAX
define|#
directive|define
name|SCNuMAX
value|PRIuMAX
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
name|has_dos_drive_prefix
end_ifndef

begin_function
DECL|function|git_has_dos_drive_prefix
specifier|static
specifier|inline
name|int
name|git_has_dos_drive_prefix
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
return|return
literal|0
return|;
block|}
end_function

begin_define
DECL|macro|has_dos_drive_prefix
define|#
directive|define
name|has_dos_drive_prefix
value|git_has_dos_drive_prefix
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|skip_dos_drive_prefix
end_ifndef

begin_function
DECL|function|git_skip_dos_drive_prefix
specifier|static
specifier|inline
name|int
name|git_skip_dos_drive_prefix
parameter_list|(
name|char
modifier|*
modifier|*
name|path
parameter_list|)
block|{
return|return
literal|0
return|;
block|}
end_function

begin_define
DECL|macro|skip_dos_drive_prefix
define|#
directive|define
name|skip_dos_drive_prefix
value|git_skip_dos_drive_prefix
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

begin_function
DECL|function|git_is_dir_sep
specifier|static
specifier|inline
name|int
name|git_is_dir_sep
parameter_list|(
name|int
name|c
parameter_list|)
block|{
return|return
name|c
operator|==
literal|'/'
return|;
block|}
end_function

begin_define
DECL|macro|is_dir_sep
define|#
directive|define
name|is_dir_sep
value|git_is_dir_sep
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|offset_1st_component
end_ifndef

begin_function
DECL|function|git_offset_1st_component
specifier|static
specifier|inline
name|int
name|git_offset_1st_component
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
return|return
name|is_dir_sep
argument_list|(
name|path
index|[
literal|0
index|]
argument_list|)
return|;
block|}
end_function

begin_define
DECL|macro|offset_1st_component
define|#
directive|define
name|offset_1st_component
value|git_offset_1st_component
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

begin_function
DECL|function|git_find_last_dir_sep
specifier|static
specifier|inline
name|char
modifier|*
name|git_find_last_dir_sep
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
return|return
name|strrchr
argument_list|(
name|path
argument_list|,
literal|'/'
argument_list|)
return|;
block|}
end_function

begin_define
DECL|macro|find_last_dir_sep
define|#
directive|define
name|find_last_dir_sep
value|git_find_last_dir_sep
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
name|__GNUC__
end_ifndef

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
DECL|macro|LAST_ARG_MUST_BE_NULL
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

begin_include
include|#
directive|include
file|"wildmatch.h"
end_include

begin_struct_decl
struct_decl|struct
name|strbuf
struct_decl|;
end_struct_decl

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
name|int
name|error_errno
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

begin_function_decl
specifier|extern
name|void
name|warning_errno
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

begin_ifndef
ifndef|#
directive|ifndef
name|NO_OPENSSL
end_ifndef

begin_ifdef
ifdef|#
directive|ifdef
name|APPLE_COMMON_CRYPTO
end_ifdef

begin_include
include|#
directive|include
file|"compat/apple-common-crypto.h"
end_include

begin_else
else|#
directive|else
end_else

begin_include
include|#
directive|include
file|<openssl/evp.h>
end_include

begin_include
include|#
directive|include
file|<openssl/hmac.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* APPLE_COMMON_CRYPTO */
end_comment

begin_include
include|#
directive|include
file|<openssl/x509v3.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* NO_OPENSSL */
end_comment

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
end_if

begin_function
DECL|function|const_error
specifier|static
specifier|inline
name|int
name|const_error
parameter_list|(
name|void
parameter_list|)
block|{
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_define
DECL|macro|error
define|#
directive|define
name|error
parameter_list|(
modifier|...
parameter_list|)
value|(error(__VA_ARGS__), const_error())
end_define

begin_define
DECL|macro|error_errno
define|#
directive|define
name|error_errno
parameter_list|(
modifier|...
parameter_list|)
value|(error_errno(__VA_ARGS__), const_error())
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

begin_extern
extern|extern void (*get_error_routine(void
end_extern

begin_expr_stmt
unit|))
operator|(
specifier|const
name|char
operator|*
name|err
operator|,
name|va_list
name|params
operator|)
expr_stmt|;
end_expr_stmt

begin_function_decl
specifier|extern
name|void
name|set_warn_routine
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
name|warn
parameter_list|,
name|va_list
name|params
parameter_list|)
parameter_list|)
function_decl|;
end_function_decl

begin_extern
extern|extern void (*get_warn_routine(void
end_extern

begin_expr_stmt
unit|))
operator|(
specifier|const
name|char
operator|*
name|warn
operator|,
name|va_list
name|params
operator|)
expr_stmt|;
end_expr_stmt

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
name|void
name|set_error_handle
parameter_list|(
name|FILE
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|starts_with
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

begin_comment
comment|/*  * If the string "str" begins with the string found in "prefix", return 1.  * The "out" parameter is set to "str + strlen(prefix)" (i.e., to the point in  * the string right after the prefix).  *  * Otherwise, return 0 and leave "out" untouched.  *  * Examples:  *  *   [extract branch name, fail if not a branch]  *   if (!skip_prefix(ref, "refs/heads/",&branch)  *	return -1;  *  *   [skip prefix if present, otherwise use whole string]  *   skip_prefix(name, "refs/heads/",&name);  */
end_comment

begin_function
DECL|function|skip_prefix
specifier|static
specifier|inline
name|int
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
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|out
parameter_list|)
block|{
do|do
block|{
if|if
condition|(
operator|!
operator|*
name|prefix
condition|)
block|{
operator|*
name|out
operator|=
name|str
expr_stmt|;
return|return
literal|1
return|;
block|}
block|}
do|while
condition|(
operator|*
name|str
operator|++
operator|==
operator|*
name|prefix
operator|++
condition|)
do|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * Like skip_prefix, but promises never to read past "len" bytes of the input  * buffer, and returns the remaining number of bytes in "out" via "outlen".  */
end_comment

begin_function
DECL|function|skip_prefix_mem
specifier|static
specifier|inline
name|int
name|skip_prefix_mem
parameter_list|(
specifier|const
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|out
parameter_list|,
name|size_t
modifier|*
name|outlen
parameter_list|)
block|{
name|size_t
name|prefix_len
init|=
name|strlen
argument_list|(
name|prefix
argument_list|)
decl_stmt|;
if|if
condition|(
name|prefix_len
operator|<=
name|len
operator|&&
operator|!
name|memcmp
argument_list|(
name|buf
argument_list|,
name|prefix
argument_list|,
name|prefix_len
argument_list|)
condition|)
block|{
operator|*
name|out
operator|=
name|buf
operator|+
name|prefix_len
expr_stmt|;
operator|*
name|outlen
operator|=
name|len
operator|-
name|prefix_len
expr_stmt|;
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * If buf ends with suffix, return 1 and subtract the length of the suffix  * from *len. Otherwise, return 0 and leave *len untouched.  */
end_comment

begin_function
DECL|function|strip_suffix_mem
specifier|static
specifier|inline
name|int
name|strip_suffix_mem
parameter_list|(
specifier|const
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
modifier|*
name|len
parameter_list|,
specifier|const
name|char
modifier|*
name|suffix
parameter_list|)
block|{
name|size_t
name|suflen
init|=
name|strlen
argument_list|(
name|suffix
argument_list|)
decl_stmt|;
if|if
condition|(
operator|*
name|len
operator|<
name|suflen
operator|||
name|memcmp
argument_list|(
name|buf
operator|+
operator|(
operator|*
name|len
operator|-
name|suflen
operator|)
argument_list|,
name|suffix
argument_list|,
name|suflen
argument_list|)
condition|)
return|return
literal|0
return|;
operator|*
name|len
operator|-=
name|suflen
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_comment
comment|/*  * If str ends with suffix, return 1 and set *len to the size of the string  * without the suffix. Otherwise, return 0 and set *len to the size of the  * string.  *  * Note that we do _not_ NUL-terminate str to the new length.  */
end_comment

begin_function
DECL|function|strip_suffix
specifier|static
specifier|inline
name|int
name|strip_suffix
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
parameter_list|,
name|size_t
modifier|*
name|len
parameter_list|)
block|{
operator|*
name|len
operator|=
name|strlen
argument_list|(
name|str
argument_list|)
expr_stmt|;
return|return
name|strip_suffix_mem
argument_list|(
name|str
argument_list|,
name|len
argument_list|,
name|suffix
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ends_with
specifier|static
specifier|inline
name|int
name|ends_with
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
block|{
name|size_t
name|len
decl_stmt|;
return|return
name|strip_suffix
argument_list|(
name|str
argument_list|,
name|suffix
argument_list|,
operator|&
name|len
argument_list|)
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

begin_ifdef
ifdef|#
directive|ifdef
name|NEEDS_MODE_TRANSLATION
end_ifdef

begin_undef
DECL|macro|S_IFMT
undef|#
directive|undef
name|S_IFMT
end_undef

begin_undef
DECL|macro|S_IFREG
undef|#
directive|undef
name|S_IFREG
end_undef

begin_undef
DECL|macro|S_IFDIR
undef|#
directive|undef
name|S_IFDIR
end_undef

begin_undef
DECL|macro|S_IFLNK
undef|#
directive|undef
name|S_IFLNK
end_undef

begin_undef
DECL|macro|S_IFBLK
undef|#
directive|undef
name|S_IFBLK
end_undef

begin_undef
DECL|macro|S_IFCHR
undef|#
directive|undef
name|S_IFCHR
end_undef

begin_undef
DECL|macro|S_IFIFO
undef|#
directive|undef
name|S_IFIFO
end_undef

begin_undef
DECL|macro|S_IFSOCK
undef|#
directive|undef
name|S_IFSOCK
end_undef

begin_define
DECL|macro|S_IFMT
define|#
directive|define
name|S_IFMT
value|0170000
end_define

begin_define
DECL|macro|S_IFREG
define|#
directive|define
name|S_IFREG
value|0100000
end_define

begin_define
DECL|macro|S_IFDIR
define|#
directive|define
name|S_IFDIR
value|0040000
end_define

begin_define
DECL|macro|S_IFLNK
define|#
directive|define
name|S_IFLNK
value|0120000
end_define

begin_define
DECL|macro|S_IFBLK
define|#
directive|define
name|S_IFBLK
value|0060000
end_define

begin_define
DECL|macro|S_IFCHR
define|#
directive|define
name|S_IFCHR
value|0020000
end_define

begin_define
DECL|macro|S_IFIFO
define|#
directive|define
name|S_IFIFO
value|0010000
end_define

begin_define
DECL|macro|S_IFSOCK
define|#
directive|define
name|S_IFSOCK
value|0140000
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|stat
end_ifdef

begin_undef
DECL|macro|stat
undef|#
directive|undef
name|stat
end_undef

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|stat
define|#
directive|define
name|stat
parameter_list|(
name|path
parameter_list|,
name|buf
parameter_list|)
value|git_stat(path, buf)
end_define

begin_function_decl
specifier|extern
name|int
name|git_stat
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|struct
name|stat
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_ifdef
ifdef|#
directive|ifdef
name|fstat
end_ifdef

begin_undef
DECL|macro|fstat
undef|#
directive|undef
name|fstat
end_undef

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|fstat
define|#
directive|define
name|fstat
parameter_list|(
name|fd
parameter_list|,
name|buf
parameter_list|)
value|git_fstat(fd, buf)
end_define

begin_function_decl
specifier|extern
name|int
name|git_fstat
parameter_list|(
name|int
parameter_list|,
name|struct
name|stat
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_ifdef
ifdef|#
directive|ifdef
name|lstat
end_ifdef

begin_undef
DECL|macro|lstat
undef|#
directive|undef
name|lstat
end_undef

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|lstat
define|#
directive|define
name|lstat
parameter_list|(
name|path
parameter_list|,
name|buf
parameter_list|)
value|git_lstat(path, buf)
end_define

begin_function_decl
specifier|extern
name|int
name|git_lstat
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|struct
name|stat
modifier|*
parameter_list|)
function_decl|;
end_function_decl

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
value|((1024L * 1024L) * (size_t)(sizeof(void*)>= 8 ? 8192 : 256))
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
name|OVERRIDE_STRDUP
end_ifdef

begin_ifdef
ifdef|#
directive|ifdef
name|strdup
end_ifdef

begin_undef
DECL|macro|strdup
undef|#
directive|undef
name|strdup
end_undef

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|strdup
define|#
directive|define
name|strdup
value|gitstrdup
end_define

begin_function_decl
name|char
modifier|*
name|gitstrdup
parameter_list|(
specifier|const
name|char
modifier|*
name|s
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

begin_ifndef
ifndef|#
directive|ifndef
name|O_CLOEXEC
end_ifndef

begin_define
DECL|macro|O_CLOEXEC
define|#
directive|define
name|O_CLOEXEC
value|0
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

begin_ifdef
ifdef|#
directive|ifdef
name|snprintf
end_ifdef

begin_undef
DECL|macro|snprintf
undef|#
directive|undef
name|snprintf
end_undef

begin_endif
endif|#
directive|endif
end_endif

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

begin_ifdef
ifdef|#
directive|ifdef
name|vsnprintf
end_ifdef

begin_undef
DECL|macro|vsnprintf
undef|#
directive|undef
name|vsnprintf
end_undef

begin_endif
endif|#
directive|endif
end_endif

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

begin_ifdef
ifdef|#
directive|ifdef
name|NO_PTHREADS
end_ifdef

begin_define
DECL|macro|atexit
define|#
directive|define
name|atexit
value|git_atexit
end_define

begin_function_decl
specifier|extern
name|int
name|git_atexit
parameter_list|(
name|void
function_decl|(
modifier|*
name|handler
function_decl|)
parameter_list|(
name|void
parameter_list|)
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

begin_function
DECL|function|st_add
specifier|static
specifier|inline
name|size_t
name|st_add
parameter_list|(
name|size_t
name|a
parameter_list|,
name|size_t
name|b
parameter_list|)
block|{
if|if
condition|(
name|unsigned_add_overflows
argument_list|(
name|a
argument_list|,
name|b
argument_list|)
condition|)
name|die
argument_list|(
literal|"size_t overflow: %"
name|PRIuMAX
literal|" + %"
name|PRIuMAX
argument_list|,
operator|(
name|uintmax_t
operator|)
name|a
argument_list|,
operator|(
name|uintmax_t
operator|)
name|b
argument_list|)
expr_stmt|;
return|return
name|a
operator|+
name|b
return|;
block|}
end_function

begin_define
DECL|macro|st_add3
define|#
directive|define
name|st_add3
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|,
name|c
parameter_list|)
value|st_add(st_add((a),(b)),(c))
end_define

begin_define
DECL|macro|st_add4
define|#
directive|define
name|st_add4
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|,
name|c
parameter_list|,
name|d
parameter_list|)
value|st_add(st_add3((a),(b),(c)),(d))
end_define

begin_function
DECL|function|st_mult
specifier|static
specifier|inline
name|size_t
name|st_mult
parameter_list|(
name|size_t
name|a
parameter_list|,
name|size_t
name|b
parameter_list|)
block|{
if|if
condition|(
name|unsigned_mult_overflows
argument_list|(
name|a
argument_list|,
name|b
argument_list|)
condition|)
name|die
argument_list|(
literal|"size_t overflow: %"
name|PRIuMAX
literal|" * %"
name|PRIuMAX
argument_list|,
operator|(
name|uintmax_t
operator|)
name|a
argument_list|,
operator|(
name|uintmax_t
operator|)
name|b
argument_list|)
expr_stmt|;
return|return
name|a
operator|*
name|b
return|;
block|}
end_function

begin_function
DECL|function|st_sub
specifier|static
specifier|inline
name|size_t
name|st_sub
parameter_list|(
name|size_t
name|a
parameter_list|,
name|size_t
name|b
parameter_list|)
block|{
if|if
condition|(
name|a
operator|<
name|b
condition|)
name|die
argument_list|(
literal|"size_t underflow: %"
name|PRIuMAX
literal|" - %"
name|PRIuMAX
argument_list|,
operator|(
name|uintmax_t
operator|)
name|a
argument_list|,
operator|(
name|uintmax_t
operator|)
name|b
argument_list|)
expr_stmt|;
return|return
name|a
operator|-
name|b
return|;
block|}
end_function

begin_ifdef
ifdef|#
directive|ifdef
name|HAVE_ALLOCA_H
end_ifdef

begin_include
include|#
directive|include
file|<alloca.h>
end_include

begin_define
DECL|macro|xalloca
define|#
directive|define
name|xalloca
parameter_list|(
name|size
parameter_list|)
value|(alloca(size))
end_define

begin_define
DECL|macro|xalloca_free
define|#
directive|define
name|xalloca_free
parameter_list|(
name|p
parameter_list|)
value|do {} while (0)
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|xalloca
define|#
directive|define
name|xalloca
parameter_list|(
name|size
parameter_list|)
value|(xmalloc(size))
end_define

begin_define
DECL|macro|xalloca_free
define|#
directive|define
name|xalloca_free
parameter_list|(
name|p
parameter_list|)
value|(free(p))
end_define

begin_endif
endif|#
directive|endif
end_endif

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
name|xmallocz_gently
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
name|void
modifier|*
name|xmmap_gently
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
name|xopen
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|flags
parameter_list|,
modifier|...
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
name|ssize_t
name|xpread
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
parameter_list|,
name|off_t
name|offset
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
name|xfopen
parameter_list|(
specifier|const
name|char
modifier|*
name|path
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
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|xgetcwd
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|FILE
modifier|*
name|fopen_for_writing
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|ALLOC_ARRAY
define|#
directive|define
name|ALLOC_ARRAY
parameter_list|(
name|x
parameter_list|,
name|alloc
parameter_list|)
value|(x) = xmalloc(st_mult(sizeof(*(x)), (alloc)))
end_define

begin_define
DECL|macro|REALLOC_ARRAY
define|#
directive|define
name|REALLOC_ARRAY
parameter_list|(
name|x
parameter_list|,
name|alloc
parameter_list|)
value|(x) = xrealloc((x), st_mult(sizeof(*(x)), (alloc)))
end_define

begin_define
DECL|macro|COPY_ARRAY
define|#
directive|define
name|COPY_ARRAY
parameter_list|(
name|dst
parameter_list|,
name|src
parameter_list|,
name|n
parameter_list|)
value|copy_array((dst), (src), (n), sizeof(*(dst)) + \ 	BUILD_ASSERT_OR_ZERO(sizeof(*(dst)) == sizeof(*(src))))
end_define

begin_function
DECL|function|copy_array
specifier|static
specifier|inline
name|void
name|copy_array
parameter_list|(
name|void
modifier|*
name|dst
parameter_list|,
specifier|const
name|void
modifier|*
name|src
parameter_list|,
name|size_t
name|n
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
if|if
condition|(
name|n
condition|)
name|memcpy
argument_list|(
name|dst
argument_list|,
name|src
argument_list|,
name|st_mult
argument_list|(
name|size
argument_list|,
name|n
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * These functions help you allocate structs with flex arrays, and copy  * the data directly into the array. For example, if you had:  *  *   struct foo {  *     int bar;  *     char name[FLEX_ARRAY];  *   };  *  * you can do:  *  *   struct foo *f;  *   FLEX_ALLOC_MEM(f, name, src, len);  *  * to allocate a "foo" with the contents of "src" in the "name" field.  * The resulting struct is automatically zero'd, and the flex-array field  * is NUL-terminated (whether the incoming src buffer was or not).  *  * The FLEXPTR_* variants operate on structs that don't use flex-arrays,  * but do want to store a pointer to some extra data in the same allocated  * block. For example, if you have:  *  *   struct foo {  *     char *name;  *     int bar;  *   };  *  * you can do:  *  *   struct foo *f;  *   FLEXPTR_ALLOC_STR(f, name, src);  *  * and "name" will point to a block of memory after the struct, which will be  * freed along with the struct (but the pointer can be repointed anywhere).  *  * The *_STR variants accept a string parameter rather than a ptr/len  * combination.  *  * Note that these macros will evaluate the first parameter multiple  * times, and it must be assignable as an lvalue.  */
end_comment

begin_define
DECL|macro|FLEX_ALLOC_MEM
define|#
directive|define
name|FLEX_ALLOC_MEM
parameter_list|(
name|x
parameter_list|,
name|flexname
parameter_list|,
name|buf
parameter_list|,
name|len
parameter_list|)
value|do { \ 	(x) = NULL;
comment|/* silence -Wuninitialized for offset calculation */
value|\ 	(x) = xalloc_flex(sizeof(*(x)), (char *)(&((x)->flexname)) - (char *)(x), (buf), (len)); \ } while (0)
end_define

begin_define
DECL|macro|FLEXPTR_ALLOC_MEM
define|#
directive|define
name|FLEXPTR_ALLOC_MEM
parameter_list|(
name|x
parameter_list|,
name|ptrname
parameter_list|,
name|buf
parameter_list|,
name|len
parameter_list|)
value|do { \ 	(x) = xalloc_flex(sizeof(*(x)), sizeof(*(x)), (buf), (len)); \ 	(x)->ptrname = (void *)((x)+1); \ } while(0)
end_define

begin_define
DECL|macro|FLEX_ALLOC_STR
define|#
directive|define
name|FLEX_ALLOC_STR
parameter_list|(
name|x
parameter_list|,
name|flexname
parameter_list|,
name|str
parameter_list|)
define|\
value|FLEX_ALLOC_MEM((x), flexname, (str), strlen(str))
end_define

begin_define
DECL|macro|FLEXPTR_ALLOC_STR
define|#
directive|define
name|FLEXPTR_ALLOC_STR
parameter_list|(
name|x
parameter_list|,
name|ptrname
parameter_list|,
name|str
parameter_list|)
define|\
value|FLEXPTR_ALLOC_MEM((x), ptrname, (str), strlen(str))
end_define

begin_function
DECL|function|xalloc_flex
specifier|static
specifier|inline
name|void
modifier|*
name|xalloc_flex
parameter_list|(
name|size_t
name|base_len
parameter_list|,
name|size_t
name|offset
parameter_list|,
specifier|const
name|void
modifier|*
name|src
parameter_list|,
name|size_t
name|src_len
parameter_list|)
block|{
name|unsigned
name|char
modifier|*
name|ret
init|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
name|st_add3
argument_list|(
name|base_len
argument_list|,
name|src_len
argument_list|,
literal|1
argument_list|)
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|ret
operator|+
name|offset
argument_list|,
name|src
argument_list|,
name|src_len
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|xstrdup_or_null
specifier|static
specifier|inline
name|char
modifier|*
name|xstrdup_or_null
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|)
block|{
return|return
name|str
condition|?
name|xstrdup
argument_list|(
name|str
argument_list|)
else|:
name|NULL
return|;
block|}
end_function

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

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|3
argument|,
literal|4
argument|))
argument_list|)
end_macro

begin_function_decl
specifier|extern
name|int
name|xsnprintf
parameter_list|(
name|char
modifier|*
name|dst
parameter_list|,
name|size_t
name|max
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* in ctype.c, for kwset users */
end_comment

begin_decl_stmt
specifier|extern
specifier|const
name|unsigned
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
value|(hexval_table[(unsigned char)(x)] != -1)
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
comment|/* negative values would be accepted by strtoul */
if|if
condition|(
name|strchr
argument_list|(
name|s
argument_list|,
literal|'-'
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
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

begin_define
DECL|macro|QSORT
define|#
directive|define
name|QSORT
parameter_list|(
name|base
parameter_list|,
name|n
parameter_list|,
name|compar
parameter_list|)
value|sane_qsort((base), (n), sizeof(*(base)), compar)
end_define

begin_function
DECL|function|sane_qsort
specifier|static
specifier|inline
name|void
name|sane_qsort
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
block|{
if|if
condition|(
name|nmemb
operator|>
literal|1
condition|)
name|qsort
argument_list|(
name|base
argument_list|,
name|nmemb
argument_list|,
name|size
argument_list|,
name|compar
argument_list|)
expr_stmt|;
block|}
end_function

begin_ifndef
ifndef|#
directive|ifndef
name|REG_STARTEND
end_ifndef

begin_error
error|#
directive|error
literal|"Git requires REG_STARTEND support. Compile with NO_REGEX=NeedsStartEnd"
end_error

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|regexec_buf
specifier|static
specifier|inline
name|int
name|regexec_buf
parameter_list|(
specifier|const
name|regex_t
modifier|*
name|preg
parameter_list|,
specifier|const
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|,
name|size_t
name|nmatch
parameter_list|,
name|regmatch_t
name|pmatch
index|[]
parameter_list|,
name|int
name|eflags
parameter_list|)
block|{
name|assert
argument_list|(
name|nmatch
operator|>
literal|0
operator|&&
name|pmatch
argument_list|)
expr_stmt|;
name|pmatch
index|[
literal|0
index|]
operator|.
name|rm_so
operator|=
literal|0
expr_stmt|;
name|pmatch
index|[
literal|0
index|]
operator|.
name|rm_eo
operator|=
name|size
expr_stmt|;
return|return
name|regexec
argument_list|(
name|preg
argument_list|,
name|buf
argument_list|,
name|nmatch
argument_list|,
name|pmatch
argument_list|,
name|eflags
operator||
name|REG_STARTEND
argument_list|)
return|;
block|}
end_function

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

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__GNUC__
argument_list|)
operator|||
operator|(
name|_MSC_VER
operator|>=
literal|1400
operator|)
operator|||
name|defined
argument_list|(
name|__C99_MACRO_WITH_VA_ARGS
argument_list|)
end_if

begin_define
DECL|macro|HAVE_VARIADIC_MACROS
define|#
directive|define
name|HAVE_VARIADIC_MACROS
value|1
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  * Preserves errno, prints a message, but gives no warning for ENOENT.  * Returns 0 on success, which includes trying to unlink an object that does  * not exist.  */
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
comment|/*   * Tries to unlink file.  Returns 0 if unlink succeeded   * or the file already didn't exist.  Returns -1 and   * appends a message to err suitable for   * 'error("%s", err->buf)' on error.   */
end_comment

begin_function_decl
name|int
name|unlink_or_msg
parameter_list|(
specifier|const
name|char
modifier|*
name|file
parameter_list|,
name|struct
name|strbuf
modifier|*
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Preserves errno, prints a message, but gives no warning for ENOENT.  * Returns 0 on success, which includes trying to remove a directory that does  * not exist.  */
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

begin_ifdef
ifdef|#
directive|ifdef
name|GMTIME_UNRELIABLE_ERRORS
end_ifdef

begin_function_decl
name|struct
name|tm
modifier|*
name|git_gmtime
parameter_list|(
specifier|const
name|time_t
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|tm
modifier|*
name|git_gmtime_r
parameter_list|(
specifier|const
name|time_t
modifier|*
parameter_list|,
name|struct
name|tm
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|gmtime
define|#
directive|define
name|gmtime
value|git_gmtime
end_define

begin_define
DECL|macro|gmtime_r
define|#
directive|define
name|gmtime_r
value|git_gmtime_r
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
operator|!
name|defined
argument_list|(
name|USE_PARENS_AROUND_GETTEXT_N
argument_list|)
operator|&&
name|defined
argument_list|(
name|__GNUC__
argument_list|)
end_if

begin_define
DECL|macro|USE_PARENS_AROUND_GETTEXT_N
define|#
directive|define
name|USE_PARENS_AROUND_GETTEXT_N
value|1
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|SHELL_PATH
end_ifndef

begin_define
DECL|macro|SHELL_PATH
define|#
directive|define
name|SHELL_PATH
value|"/bin/sh"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|_POSIX_THREAD_SAFE_FUNCTIONS
end_ifndef

begin_define
DECL|macro|flockfile
define|#
directive|define
name|flockfile
parameter_list|(
name|fh
parameter_list|)
end_define

begin_define
DECL|macro|funlockfile
define|#
directive|define
name|funlockfile
parameter_list|(
name|fh
parameter_list|)
end_define

begin_define
DECL|macro|getc_unlocked
define|#
directive|define
name|getc_unlocked
parameter_list|(
name|fh
parameter_list|)
value|getc(fh)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
specifier|extern
name|int
name|cmd_main
parameter_list|(
name|int
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
parameter_list|)
function_decl|;
end_function_decl

end_unit

