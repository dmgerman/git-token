begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|_UTIME_H_
end_ifndef

begin_define
DECL|macro|_UTIME_H_
define|#
directive|define
name|_UTIME_H_
end_define

begin_comment
comment|/*  * UTIME.H  * This file has no copyright assigned and is placed in the Public Domain.  * This file is a part of the mingw-runtime package.  *  * The mingw-runtime package and its code is distributed in the hope that it  * will be useful but WITHOUT ANY WARRANTY.  ALL WARRANTIES, EXPRESSED OR  * IMPLIED ARE HEREBY DISCLAIMED.  This includes but is not limited to  * warranties of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *  * You are free to use this package and its code without limitation.  */
end_comment

begin_comment
comment|/*  * Structure used by _utime function.  */
end_comment

begin_struct
DECL|struct|_utimbuf
struct|struct
name|_utimbuf
block|{
DECL|member|actime
name|time_t
name|actime
decl_stmt|;
comment|/* Access time */
DECL|member|modtime
name|time_t
name|modtime
decl_stmt|;
comment|/* Modification time */
block|}
struct|;
end_struct

begin_ifndef
ifndef|#
directive|ifndef
name|_NO_OLDNAMES
end_ifndef

begin_comment
comment|/* NOTE: Must be the same as _utimbuf above. */
end_comment

begin_struct
DECL|struct|utimbuf
struct|struct
name|utimbuf
block|{
DECL|member|actime
name|time_t
name|actime
decl_stmt|;
DECL|member|modtime
name|time_t
name|modtime
decl_stmt|;
block|}
struct|;
end_struct

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* Not _NO_OLDNAMES */
end_comment

begin_endif
endif|#
directive|endif
end_endif

end_unit

