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

begin_undef
DECL|macro|stat
undef|#
directive|undef
name|stat
end_undef

begin_undef
DECL|macro|_stati64
undef|#
directive|undef
name|_stati64
end_undef

begin_include
include|#
directive|include
file|"compat/mingw.h"
end_include

begin_undef
DECL|macro|stat
undef|#
directive|undef
name|stat
end_undef

begin_define
DECL|macro|stat
define|#
directive|define
name|stat
value|_stati64
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

begin_comment
comment|/*    Even though _stati64 is normally just defined at _stat64    on Windows, we specify it here as a proper struct to avoid    compiler warnings about macro redefinition due to magic in    mingw.h. Struct taken from ReactOS (GNU GPL license). */
end_comment

begin_struct
DECL|struct|_stati64
struct|struct
name|_stati64
block|{
DECL|member|st_dev
name|_dev_t
name|st_dev
decl_stmt|;
DECL|member|st_ino
name|_ino_t
name|st_ino
decl_stmt|;
DECL|member|st_mode
name|unsigned
name|short
name|st_mode
decl_stmt|;
DECL|member|st_nlink
name|short
name|st_nlink
decl_stmt|;
DECL|member|st_uid
name|short
name|st_uid
decl_stmt|;
DECL|member|st_gid
name|short
name|st_gid
decl_stmt|;
DECL|member|st_rdev
name|_dev_t
name|st_rdev
decl_stmt|;
DECL|member|st_size
name|__int64
name|st_size
decl_stmt|;
DECL|member|st_atime
name|time_t
name|st_atime
decl_stmt|;
DECL|member|st_mtime
name|time_t
name|st_mtime
decl_stmt|;
DECL|member|st_ctime
name|time_t
name|st_ctime
decl_stmt|;
block|}
struct|;
end_struct

begin_endif
endif|#
directive|endif
end_endif

end_unit

