begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|MERGE_RECURSIVE_H
end_ifndef

begin_define
DECL|macro|MERGE_RECURSIVE_H
define|#
directive|define
name|MERGE_RECURSIVE_H
end_define

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_struct
DECL|struct|merge_options
struct|struct
name|merge_options
block|{
DECL|member|ancestor
specifier|const
name|char
modifier|*
name|ancestor
decl_stmt|;
DECL|member|branch1
specifier|const
name|char
modifier|*
name|branch1
decl_stmt|;
DECL|member|branch2
specifier|const
name|char
modifier|*
name|branch2
decl_stmt|;
enum|enum
block|{
DECL|enumerator|MERGE_RECURSIVE_NORMAL
name|MERGE_RECURSIVE_NORMAL
init|=
literal|0
block|,
DECL|enumerator|MERGE_RECURSIVE_OURS
name|MERGE_RECURSIVE_OURS
block|,
DECL|enumerator|MERGE_RECURSIVE_THEIRS
name|MERGE_RECURSIVE_THEIRS
DECL|member|recursive_variant
block|}
name|recursive_variant
enum|;
DECL|member|subtree_shift
specifier|const
name|char
modifier|*
name|subtree_shift
decl_stmt|;
DECL|member|buffer_output
name|unsigned
name|buffer_output
decl_stmt|;
comment|/* 1: output at end, 2: keep buffered */
DECL|member|renormalize
name|unsigned
name|renormalize
range|:
literal|1
decl_stmt|;
DECL|member|xdl_opts
name|long
name|xdl_opts
decl_stmt|;
DECL|member|verbosity
name|int
name|verbosity
decl_stmt|;
DECL|member|detect_rename
name|int
name|detect_rename
decl_stmt|;
DECL|member|diff_rename_limit
name|int
name|diff_rename_limit
decl_stmt|;
DECL|member|merge_rename_limit
name|int
name|merge_rename_limit
decl_stmt|;
DECL|member|rename_score
name|int
name|rename_score
decl_stmt|;
DECL|member|needed_rename_limit
name|int
name|needed_rename_limit
decl_stmt|;
DECL|member|show_rename_progress
name|int
name|show_rename_progress
decl_stmt|;
DECL|member|call_depth
name|int
name|call_depth
decl_stmt|;
DECL|member|obuf
name|struct
name|strbuf
name|obuf
decl_stmt|;
DECL|member|current_file_set
name|struct
name|string_list
name|current_file_set
decl_stmt|;
DECL|member|current_directory_set
name|struct
name|string_list
name|current_directory_set
decl_stmt|;
DECL|member|df_conflict_file_set
name|struct
name|string_list
name|df_conflict_file_set
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* merge_trees() but with recursive ancestor consolidation */
end_comment

begin_function_decl
name|int
name|merge_recursive
parameter_list|(
name|struct
name|merge_options
modifier|*
name|o
parameter_list|,
name|struct
name|commit
modifier|*
name|h1
parameter_list|,
name|struct
name|commit
modifier|*
name|h2
parameter_list|,
name|struct
name|commit_list
modifier|*
name|ancestors
parameter_list|,
name|struct
name|commit
modifier|*
modifier|*
name|result
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* rename-detecting three-way merge, no recursion */
end_comment

begin_function_decl
name|int
name|merge_trees
parameter_list|(
name|struct
name|merge_options
modifier|*
name|o
parameter_list|,
name|struct
name|tree
modifier|*
name|head
parameter_list|,
name|struct
name|tree
modifier|*
name|merge
parameter_list|,
name|struct
name|tree
modifier|*
name|common
parameter_list|,
name|struct
name|tree
modifier|*
modifier|*
name|result
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * "git-merge-recursive" can be fed trees; wrap them into  * virtual commits and call merge_recursive() proper.  */
end_comment

begin_function_decl
name|int
name|merge_recursive_generic
parameter_list|(
name|struct
name|merge_options
modifier|*
name|o
parameter_list|,
specifier|const
name|struct
name|object_id
modifier|*
name|head
parameter_list|,
specifier|const
name|struct
name|object_id
modifier|*
name|merge
parameter_list|,
name|int
name|num_ca
parameter_list|,
specifier|const
name|struct
name|object_id
modifier|*
modifier|*
name|ca
parameter_list|,
name|struct
name|commit
modifier|*
modifier|*
name|result
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|init_merge_options
parameter_list|(
name|struct
name|merge_options
modifier|*
name|o
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|tree
modifier|*
name|write_tree_from_memory
parameter_list|(
name|struct
name|merge_options
modifier|*
name|o
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_merge_opt
parameter_list|(
name|struct
name|merge_options
modifier|*
name|out
parameter_list|,
specifier|const
name|char
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

