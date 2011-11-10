begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|__MSVC__HEAD
end_ifndef

begin_define
DECL|macro|__MSVC__HEAD
define|#
directive|define
name|__MSVC__HEAD
end_define

begin_include
include|#
directive|include
file|<direct.h>
end_include

begin_include
include|#
directive|include
file|<process.h>
end_include

begin_include
include|#
directive|include
file|<malloc.h>
end_include

begin_comment
comment|/* porting function */
end_comment

begin_define
DECL|macro|inline
define|#
directive|define
name|inline
value|__inline
end_define

begin_define
DECL|macro|__inline__
define|#
directive|define
name|__inline__
value|__inline
end_define

begin_define
DECL|macro|__attribute__
define|#
directive|define
name|__attribute__
parameter_list|(
name|x
parameter_list|)
end_define

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

begin_define
DECL|macro|strncasecmp
define|#
directive|define
name|strncasecmp
value|_strnicmp
end_define

begin_define
DECL|macro|ftruncate
define|#
directive|define
name|ftruncate
value|_chsize
end_define

begin_function
DECL|function|strcasecmp
specifier|static
name|__inline
name|int
name|strcasecmp
parameter_list|(
specifier|const
name|char
modifier|*
name|s1
parameter_list|,
specifier|const
name|char
modifier|*
name|s2
parameter_list|)
block|{
name|int
name|size1
init|=
name|strlen
argument_list|(
name|s1
argument_list|)
decl_stmt|;
name|int
name|sisz2
init|=
name|strlen
argument_list|(
name|s2
argument_list|)
decl_stmt|;
return|return
name|_strnicmp
argument_list|(
name|s1
argument_list|,
name|s2
argument_list|,
name|sisz2
operator|>
name|size1
condition|?
name|sisz2
else|:
name|size1
argument_list|)
return|;
block|}
end_function

begin_undef
DECL|macro|ERROR
undef|#
directive|undef
name|ERROR
end_undef

begin_comment
comment|/* Use mingw_lstat() instead of lstat()/stat() and mingw_fstat() instead  * of fstat(). We add the declaration of these functions here, suppressing  * the corresponding declarations in mingw.h, so that we can use the  * appropriate structure type (and function) names from the msvc headers.  */
end_comment

begin_define
DECL|macro|stat
define|#
directive|define
name|stat
value|_stat64
end_define

begin_function_decl
name|int
name|mingw_lstat
parameter_list|(
specifier|const
name|char
modifier|*
name|file_name
parameter_list|,
name|struct
name|stat
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|mingw_fstat
parameter_list|(
name|int
name|fd
parameter_list|,
name|struct
name|stat
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|fstat
define|#
directive|define
name|fstat
value|mingw_fstat
end_define

begin_define
DECL|macro|lstat
define|#
directive|define
name|lstat
value|mingw_lstat
end_define

begin_define
DECL|macro|_stat64
define|#
directive|define
name|_stat64
parameter_list|(
name|x
parameter_list|,
name|y
parameter_list|)
value|mingw_lstat(x,y)
end_define

begin_define
DECL|macro|ALREADY_DECLARED_STAT_FUNCS
define|#
directive|define
name|ALREADY_DECLARED_STAT_FUNCS
end_define

begin_include
include|#
directive|include
file|"compat/mingw.h"
end_include

begin_undef
DECL|macro|ALREADY_DECLARED_STAT_FUNCS
undef|#
directive|undef
name|ALREADY_DECLARED_STAT_FUNCS
end_undef

begin_endif
endif|#
directive|endif
end_endif

end_unit

