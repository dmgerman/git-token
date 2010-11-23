begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|DIRENT_H
end_ifndef

begin_define
DECL|macro|DIRENT_H
define|#
directive|define
name|DIRENT_H
end_define

begin_typedef
DECL|typedef|DIR
typedef|typedef
name|struct
name|DIR
name|DIR
typedef|;
end_typedef

begin_define
DECL|macro|DT_UNKNOWN
define|#
directive|define
name|DT_UNKNOWN
value|0
end_define

begin_define
DECL|macro|DT_DIR
define|#
directive|define
name|DT_DIR
value|1
end_define

begin_define
DECL|macro|DT_REG
define|#
directive|define
name|DT_REG
value|2
end_define

begin_define
DECL|macro|DT_LNK
define|#
directive|define
name|DT_LNK
value|3
end_define

begin_struct
DECL|struct|dirent
struct|struct
name|dirent
block|{
DECL|member|d_ino
name|long
name|d_ino
decl_stmt|;
comment|/* Always zero. */
DECL|member|d_name
name|char
name|d_name
index|[
name|FILENAME_MAX
index|]
decl_stmt|;
comment|/* File name. */
union|union
block|{
DECL|member|d_reclen
name|unsigned
name|short
name|d_reclen
decl_stmt|;
comment|/* Always zero. */
DECL|member|d_type
name|unsigned
name|char
name|d_type
decl_stmt|;
comment|/* Reimplementation adds this */
block|}
union|;
block|}
struct|;
end_struct

begin_function_decl
name|DIR
modifier|*
name|opendir
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
name|dirent
modifier|*
name|readdir
parameter_list|(
name|DIR
modifier|*
name|dir
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|closedir
parameter_list|(
name|DIR
modifier|*
name|dir
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* DIRENT_H */
end_comment

end_unit

