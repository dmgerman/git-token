begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PRECOMPOSE_UNICODE_H
end_ifndef

begin_include
include|#
directive|include
file|<sys/stat.h>
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
file|<iconv.h>
end_include

begin_typedef
DECL|struct|dirent_prec_psx
typedef|typedef
struct|struct
name|dirent_prec_psx
block|{
DECL|member|d_ino
name|ino_t
name|d_ino
decl_stmt|;
comment|/* Posix */
DECL|member|max_name_len
name|size_t
name|max_name_len
decl_stmt|;
comment|/* See below */
DECL|member|d_type
name|unsigned
name|char
name|d_type
decl_stmt|;
comment|/* available on all systems git runs on */
comment|/* 	 * See http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/dirent.h.html 	 * NAME_MAX + 1 should be enough, but some systems have 	 * NAME_MAX=255 and strlen(d_name) may return 508 or 510 	 * Solution: allocate more when needed, see precompose_utf8_readdir() 	 */
DECL|member|d_name
name|char
name|d_name
index|[
name|NAME_MAX
operator|+
literal|1
index|]
decl_stmt|;
DECL|typedef|dirent_prec_psx
block|}
name|dirent_prec_psx
typedef|;
end_typedef

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|ic_precompose
name|iconv_t
name|ic_precompose
decl_stmt|;
DECL|member|dirp
name|DIR
modifier|*
name|dirp
decl_stmt|;
DECL|member|dirent_nfc
name|struct
name|dirent_prec_psx
modifier|*
name|dirent_nfc
decl_stmt|;
DECL|typedef|PREC_DIR
block|}
name|PREC_DIR
typedef|;
end_typedef

begin_function_decl
name|void
name|precompose_argv
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|probe_utf8_pathname_composition
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|PREC_DIR
modifier|*
name|precompose_utf8_opendir
parameter_list|(
specifier|const
name|char
modifier|*
name|dirname
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|dirent_prec_psx
modifier|*
name|precompose_utf8_readdir
parameter_list|(
name|PREC_DIR
modifier|*
name|dirp
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|precompose_utf8_closedir
parameter_list|(
name|PREC_DIR
modifier|*
name|dirp
parameter_list|)
function_decl|;
end_function_decl

begin_ifndef
ifndef|#
directive|ifndef
name|PRECOMPOSE_UNICODE_C
end_ifndef

begin_define
DECL|macro|dirent
define|#
directive|define
name|dirent
value|dirent_prec_psx
end_define

begin_define
DECL|macro|opendir
define|#
directive|define
name|opendir
parameter_list|(
name|n
parameter_list|)
value|precompose_utf8_opendir(n)
end_define

begin_define
DECL|macro|readdir
define|#
directive|define
name|readdir
parameter_list|(
name|d
parameter_list|)
value|precompose_utf8_readdir(d)
end_define

begin_define
DECL|macro|closedir
define|#
directive|define
name|closedir
parameter_list|(
name|d
parameter_list|)
value|precompose_utf8_closedir(d)
end_define

begin_define
DECL|macro|DIR
define|#
directive|define
name|DIR
value|PREC_DIR
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PRECOMPOSE_UNICODE_C */
end_comment

begin_define
DECL|macro|PRECOMPOSE_UNICODE_H
define|#
directive|define
name|PRECOMPOSE_UNICODE_H
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PRECOMPOSE_UNICODE_H */
end_comment

end_unit

