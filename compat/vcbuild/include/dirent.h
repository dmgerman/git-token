begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * DIRENT.H (formerly DIRLIB.H)  * This file has no copyright assigned and is placed in the Public Domain.  * This file is a part of the mingw-runtime package.  *  * The mingw-runtime package and its code is distributed in the hope that it  * will be useful but WITHOUT ANY WARRANTY.  ALL WARRANTIES, EXPRESSED OR  * IMPLIED ARE HEREBY DISCLAIMED.  This includes but is not limited to  * warranties of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *  * You are free to use this package and its code without limitation.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_DIRENT_H_
end_ifndef

begin_define
DECL|macro|_DIRENT_H_
define|#
directive|define
name|_DIRENT_H_
end_define

begin_include
include|#
directive|include
file|<io.h>
end_include

begin_define
DECL|macro|PATH_MAX
define|#
directive|define
name|PATH_MAX
value|512
end_define

begin_define
DECL|macro|__MINGW_NOTHROW
define|#
directive|define
name|__MINGW_NOTHROW
end_define

begin_ifndef
ifndef|#
directive|ifndef
name|RC_INVOKED
end_ifndef

begin_ifdef
ifdef|#
directive|ifdef
name|__cplusplus
end_ifdef

begin_extern
extern|extern
literal|"C"
block|{
endif|#
directive|endif
DECL|struct|dirent
struct|struct
name|dirent
block|{
DECL|member|d_ino
name|long
name|d_ino
decl_stmt|;
comment|/* Always zero. */
DECL|member|d_reclen
name|unsigned
name|short
name|d_reclen
decl_stmt|;
comment|/* Always zero. */
DECL|member|d_namlen
name|unsigned
name|short
name|d_namlen
decl_stmt|;
comment|/* Length of name in d_name. */
DECL|member|d_name
name|char
name|d_name
index|[
name|FILENAME_MAX
index|]
decl_stmt|;
comment|/* File name. */
block|}
struct|;
comment|/*  * This is an internal data structure. Good programmers will not use it  * except as an argument to one of the functions below.  * dd_stat field is now int (was short in older versions).  */
typedef|typedef
struct|struct
block|{
comment|/* disk transfer area for this dir */
DECL|member|dd_dta
name|struct
name|_finddata_t
name|dd_dta
decl_stmt|;
comment|/* dirent struct to return from dir (NOTE: this makes this thread 	 * safe as long as only one thread uses a particular DIR struct at 	 * a time) */
DECL|member|dd_dir
name|struct
name|dirent
name|dd_dir
decl_stmt|;
comment|/* _findnext handle */
DECL|member|dd_handle
name|long
name|dd_handle
decl_stmt|;
comment|/* 	 * Status of search: 	 *   0 = not started yet (next entry to read is first entry) 	 *  -1 = off the end 	 *   positive = 0 based index of next entry 	 */
DECL|member|dd_stat
name|int
name|dd_stat
decl_stmt|;
comment|/* given path for dir with search pattern (struct is extended) */
DECL|member|dd_name
name|char
name|dd_name
index|[
name|PATH_MAX
operator|+
literal|3
index|]
decl_stmt|;
DECL|typedef|DIR
block|}
name|DIR
typedef|;
name|DIR
modifier|*
name|__cdecl
name|__MINGW_NOTHROW
name|opendir
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
name|struct
name|dirent
modifier|*
name|__cdecl
name|__MINGW_NOTHROW
name|readdir
parameter_list|(
name|DIR
modifier|*
parameter_list|)
function_decl|;
name|int
name|__cdecl
name|__MINGW_NOTHROW
name|closedir
parameter_list|(
name|DIR
modifier|*
parameter_list|)
function_decl|;
name|void
name|__cdecl
name|__MINGW_NOTHROW
name|rewinddir
parameter_list|(
name|DIR
modifier|*
parameter_list|)
function_decl|;
name|long
name|__cdecl
name|__MINGW_NOTHROW
name|telldir
parameter_list|(
name|DIR
modifier|*
parameter_list|)
function_decl|;
name|void
name|__cdecl
name|__MINGW_NOTHROW
name|seekdir
parameter_list|(
name|DIR
modifier|*
parameter_list|,
name|long
parameter_list|)
function_decl|;
comment|/* wide char versions */
DECL|struct|_wdirent
struct|struct
name|_wdirent
block|{
DECL|member|d_ino
name|long
name|d_ino
decl_stmt|;
comment|/* Always zero. */
DECL|member|d_reclen
name|unsigned
name|short
name|d_reclen
decl_stmt|;
comment|/* Always zero. */
DECL|member|d_namlen
name|unsigned
name|short
name|d_namlen
decl_stmt|;
comment|/* Length of name in d_name. */
DECL|member|d_name
name|wchar_t
name|d_name
index|[
name|FILENAME_MAX
index|]
decl_stmt|;
comment|/* File name. */
block|}
struct|;
comment|/*  * This is an internal data structure. Good programmers will not use it  * except as an argument to one of the functions below.  */
typedef|typedef
struct|struct
block|{
comment|/* disk transfer area for this dir */
comment|//struct _wfinddata_t	dd_dta;
comment|/* dirent struct to return from dir (NOTE: this makes this thread 	 * safe as long as only one thread uses a particular DIR struct at 	 * a time) */
DECL|member|dd_dir
name|struct
name|_wdirent
name|dd_dir
decl_stmt|;
comment|/* _findnext handle */
DECL|member|dd_handle
name|long
name|dd_handle
decl_stmt|;
comment|/* 	 * Status of search: 	 *   0 = not started yet (next entry to read is first entry) 	 *  -1 = off the end 	 *   positive = 0 based index of next entry 	 */
DECL|member|dd_stat
name|int
name|dd_stat
decl_stmt|;
comment|/* given path for dir with search pattern (struct is extended) */
DECL|member|dd_name
name|wchar_t
name|dd_name
index|[
literal|1
index|]
decl_stmt|;
DECL|typedef|_WDIR
block|}
name|_WDIR
typedef|;
name|_WDIR
modifier|*
name|__cdecl
name|__MINGW_NOTHROW
name|_wopendir
parameter_list|(
specifier|const
name|wchar_t
modifier|*
parameter_list|)
function_decl|;
name|struct
name|_wdirent
modifier|*
name|__cdecl
name|__MINGW_NOTHROW
name|_wreaddir
parameter_list|(
name|_WDIR
modifier|*
parameter_list|)
function_decl|;
name|int
name|__cdecl
name|__MINGW_NOTHROW
name|_wclosedir
parameter_list|(
name|_WDIR
modifier|*
parameter_list|)
function_decl|;
name|void
name|__cdecl
name|__MINGW_NOTHROW
name|_wrewinddir
parameter_list|(
name|_WDIR
modifier|*
parameter_list|)
function_decl|;
name|long
name|__cdecl
name|__MINGW_NOTHROW
name|_wtelldir
parameter_list|(
name|_WDIR
modifier|*
parameter_list|)
function_decl|;
name|void
name|__cdecl
name|__MINGW_NOTHROW
name|_wseekdir
parameter_list|(
name|_WDIR
modifier|*
parameter_list|,
name|long
parameter_list|)
function_decl|;
ifdef|#
directive|ifdef
name|__cplusplus
block|}
end_extern

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* Not RC_INVOKED */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* Not _DIRENT_H_ */
end_comment

end_unit

