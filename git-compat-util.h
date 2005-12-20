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
file|<netinet/in.h>
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
condition|)
name|die
argument_list|(
literal|"Out of memory, malloc failed"
argument_list|)
expr_stmt|;
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

begin_ifndef
ifndef|#
directive|ifndef
name|MAXPATHLEN
end_ifndef

begin_define
DECL|macro|MAXPATHLEN
define|#
directive|define
name|MAXPATHLEN
value|256
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

end_unit

