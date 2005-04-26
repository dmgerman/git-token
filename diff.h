begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2005 Junio C Hamano  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|DIFF_H
end_ifndef

begin_define
DECL|macro|DIFF_H
define|#
directive|define
name|DIFF_H
end_define

begin_comment
comment|/* These two are for backward compatibility with show-diff;  * new users should not use them.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|show_differences
parameter_list|(
specifier|const
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|int
name|reverse
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|show_diff_empty
parameter_list|(
specifier|const
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|int
name|reverse
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|diff_spec
struct|struct
name|diff_spec
block|{
union|union
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
comment|/* path on the filesystem */
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
comment|/* blob object ID */
DECL|member|u
block|}
name|u
union|;
DECL|member|mode
name|unsigned
name|short
name|mode
decl_stmt|;
comment|/* file mode */
DECL|member|sha1_valid
name|unsigned
name|sha1_valid
range|:
literal|1
decl_stmt|;
comment|/* if true, use u.sha1 and trust mode. 				  * (however with a NULL SHA1, read them 				  * from the file!). 				  * if false, use u.name and read mode from 				  * the filesystem. 				  */
DECL|member|file_valid
name|unsigned
name|file_valid
range|:
literal|1
decl_stmt|;
comment|/* if false the file does not even exist */
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|void
name|run_external_diff
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|struct
name|diff_spec
modifier|*
parameter_list|,
name|struct
name|diff_spec
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* DIFF_H */
end_comment

end_unit

