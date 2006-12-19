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

begin_ifndef
ifndef|#
directive|ifndef
name|FLEX_ARRAY
end_ifndef

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
operator|<
literal|3
operator|)
end_if

begin_define
DECL|macro|FLEX_ARRAY
define|#
directive|define
name|FLEX_ARRAY
value|0
end_define

begin_else
else|#
directive|else
end_else

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

begin_define
DECL|macro|_GNU_SOURCE
define|#
directive|define
name|_GNU_SOURCE
end_define

begin_define
DECL|macro|_BSD_SOURCE
define|#
directive|define
name|_BSD_SOURCE
end_define

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
file|<sys/wait.h>
end_include

begin_include
include|#
directive|include
file|<fnmatch.h>
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
file|<grp.h>
end_include

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

begin_comment
comment|/* General helper functions */
end_comment

begin_decl_stmt
specifier|extern
name|void
name|usage
argument_list|(
specifier|const
name|char
operator|*
name|err
argument_list|)
name|NORETURN
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|void
name|die
argument_list|(
specifier|const
name|char
operator|*
name|err
argument_list|,
operator|...
argument_list|)
name|NORETURN
name|__attribute__
argument_list|(
operator|(
name|format
argument_list|(
name|printf
argument_list|,
literal|1
argument_list|,
literal|2
argument_list|)
operator|)
argument_list|)
decl_stmt|;
end_decl_stmt

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

begin_decl_stmt
specifier|extern
name|void
name|set_usage_routine
argument_list|(
name|void
argument_list|(
argument|*routine
argument_list|)
operator|(
specifier|const
name|char
operator|*
name|err
operator|)
name|NORETURN
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|void
name|set_die_routine
argument_list|(
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
name|NORETURN
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

begin_ifdef
ifdef|#
directive|ifdef
name|NO_MMAP
end_ifdef

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

begin_define
DECL|macro|MAP_FAILED
define|#
directive|define
name|MAP_FAILED
value|((void*)-1)
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
value|gitfakemmap
end_define

begin_define
DECL|macro|munmap
define|#
directive|define
name|munmap
value|gitfakemunmap
end_define

begin_function_decl
specifier|extern
name|void
modifier|*
name|gitfakemmap
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
name|gitfakemunmap
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
comment|/* NO_MMAP */
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
comment|/* NO_MMAP */
end_comment

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

begin_function
DECL|function|xstrdup
specifier|static
specifier|inline
name|char
modifier|*
name|xstrdup
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|)
block|{
name|char
modifier|*
name|ret
init|=
name|strdup
argument_list|(
name|str
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
name|die
argument_list|(
literal|"Out of memory, strdup failed"
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|xmalloc
specifier|static
specifier|inline
name|void
modifier|*
name|xmalloc
parameter_list|(
name|size_t
name|size
parameter_list|)
block|{
name|void
modifier|*
name|ret
init|=
name|malloc
argument_list|(
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
operator|!
name|size
condition|)
name|ret
operator|=
name|malloc
argument_list|(
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
name|die
argument_list|(
literal|"Out of memory, malloc failed"
argument_list|)
expr_stmt|;
ifdef|#
directive|ifdef
name|XMALLOC_POISON
name|memset
argument_list|(
name|ret
argument_list|,
literal|0xA5
argument_list|,
name|size
argument_list|)
expr_stmt|;
endif|#
directive|endif
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|xrealloc
specifier|static
specifier|inline
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
block|{
name|void
modifier|*
name|ret
init|=
name|realloc
argument_list|(
name|ptr
argument_list|,
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
operator|!
name|size
condition|)
name|ret
operator|=
name|realloc
argument_list|(
name|ptr
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
name|die
argument_list|(
literal|"Out of memory, realloc failed"
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|xcalloc
specifier|static
specifier|inline
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
block|{
name|void
modifier|*
name|ret
init|=
name|calloc
argument_list|(
name|nmemb
argument_list|,
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
operator|(
operator|!
name|nmemb
operator|||
operator|!
name|size
operator|)
condition|)
name|ret
operator|=
name|calloc
argument_list|(
literal|1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
name|die
argument_list|(
literal|"Out of memory, calloc failed"
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|xread
specifier|static
specifier|inline
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
block|{
name|ssize_t
name|nr
decl_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|nr
operator|=
name|read
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|nr
operator|<
literal|0
operator|)
operator|&&
operator|(
name|errno
operator|==
name|EAGAIN
operator|||
name|errno
operator|==
name|EINTR
operator|)
condition|)
continue|continue;
return|return
name|nr
return|;
block|}
block|}
end_function

begin_function
DECL|function|xwrite
specifier|static
specifier|inline
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
block|{
name|ssize_t
name|nr
decl_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|nr
operator|=
name|write
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|nr
operator|<
literal|0
operator|)
operator|&&
operator|(
name|errno
operator|==
name|EAGAIN
operator|||
name|errno
operator|==
name|EINTR
operator|)
condition|)
continue|continue;
return|return
name|nr
return|;
block|}
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

begin_endif
endif|#
directive|endif
end_endif

end_unit

