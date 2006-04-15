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

begin_endif
endif|#
directive|endif
end_endif

end_unit

