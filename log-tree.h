begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|LOG_TREE_H
end_ifndef

begin_define
DECL|macro|LOG_TREE_H
define|#
directive|define
name|LOG_TREE_H
end_define

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_struct
DECL|struct|log_info
struct|struct
name|log_info
block|{
DECL|member|commit
DECL|member|parent
name|struct
name|commit
modifier|*
name|commit
decl_stmt|,
modifier|*
name|parent
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|void
name|init_log_tree_opt
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|log_tree_diff_flush
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|log_tree_commit
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|,
name|struct
name|commit
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|log_tree_opt_parse
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|show_log
parameter_list|(
name|struct
name|rev_info
modifier|*
name|opt
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|show_decorations
parameter_list|(
name|struct
name|rev_info
modifier|*
name|opt
parameter_list|,
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|log_write_email_headers
parameter_list|(
name|struct
name|rev_info
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|subject_p
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|extra_headers_p
parameter_list|,
name|int
modifier|*
name|need_8bit_cte_p
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|load_ref_decorations
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|FORMAT_PATCH_NAME_MAX
define|#
directive|define
name|FORMAT_PATCH_NAME_MAX
value|64
end_define

begin_function_decl
name|void
name|get_patch_filename
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|int
name|nr
parameter_list|,
specifier|const
name|char
modifier|*
name|suffix
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

