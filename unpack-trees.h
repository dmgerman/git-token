begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|UNPACK_TREES_H
end_ifndef

begin_define
DECL|macro|UNPACK_TREES_H
define|#
directive|define
name|UNPACK_TREES_H
end_define

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_define
DECL|macro|MAX_UNPACK_TREES
define|#
directive|define
name|MAX_UNPACK_TREES
value|8
end_define

begin_struct_decl
struct_decl|struct
name|unpack_trees_options
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|exclude_list
struct_decl|;
end_struct_decl

begin_typedef
DECL|typedef|merge_fn_t
typedef|typedef
name|int
function_decl|(
modifier|*
name|merge_fn_t
function_decl|)
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|src
parameter_list|,
name|struct
name|unpack_trees_options
modifier|*
name|options
parameter_list|)
function_decl|;
end_typedef

begin_enum
DECL|enum|unpack_trees_error_types
enum|enum
name|unpack_trees_error_types
block|{
DECL|enumerator|ERROR_WOULD_OVERWRITE
name|ERROR_WOULD_OVERWRITE
init|=
literal|0
block|,
DECL|enumerator|ERROR_NOT_UPTODATE_FILE
name|ERROR_NOT_UPTODATE_FILE
block|,
DECL|enumerator|ERROR_NOT_UPTODATE_DIR
name|ERROR_NOT_UPTODATE_DIR
block|,
DECL|enumerator|ERROR_WOULD_LOSE_UNTRACKED_OVERWRITTEN
name|ERROR_WOULD_LOSE_UNTRACKED_OVERWRITTEN
block|,
DECL|enumerator|ERROR_WOULD_LOSE_UNTRACKED_REMOVED
name|ERROR_WOULD_LOSE_UNTRACKED_REMOVED
block|,
DECL|enumerator|ERROR_BIND_OVERLAP
name|ERROR_BIND_OVERLAP
block|,
DECL|enumerator|ERROR_SPARSE_NOT_UPTODATE_FILE
name|ERROR_SPARSE_NOT_UPTODATE_FILE
block|,
DECL|enumerator|ERROR_WOULD_LOSE_ORPHANED_OVERWRITTEN
name|ERROR_WOULD_LOSE_ORPHANED_OVERWRITTEN
block|,
DECL|enumerator|ERROR_WOULD_LOSE_ORPHANED_REMOVED
name|ERROR_WOULD_LOSE_ORPHANED_REMOVED
block|,
DECL|enumerator|NB_UNPACK_TREES_ERROR_TYPES
name|NB_UNPACK_TREES_ERROR_TYPES
block|}
enum|;
end_enum

begin_comment
comment|/*  * Sets the list of user-friendly error messages to be used by the  * command "cmd" (either merge or checkout), and show_all_errors to 1.  */
end_comment

begin_function_decl
name|void
name|setup_unpack_trees_porcelain
parameter_list|(
name|struct
name|unpack_trees_options
modifier|*
name|opts
parameter_list|,
specifier|const
name|char
modifier|*
name|cmd
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|unpack_trees_options
struct|struct
name|unpack_trees_options
block|{
DECL|member|reset
name|unsigned
name|int
name|reset
decl_stmt|,
DECL|member|merge
name|merge
decl_stmt|,
DECL|member|update
name|update
decl_stmt|,
DECL|member|index_only
name|index_only
decl_stmt|,
DECL|member|nontrivial_merge
name|nontrivial_merge
decl_stmt|,
DECL|member|trivial_merges_only
name|trivial_merges_only
decl_stmt|,
DECL|member|verbose_update
name|verbose_update
decl_stmt|,
DECL|member|aggressive
name|aggressive
decl_stmt|,
DECL|member|skip_unmerged
name|skip_unmerged
decl_stmt|,
DECL|member|initial_checkout
name|initial_checkout
decl_stmt|,
DECL|member|diff_index_cached
name|diff_index_cached
decl_stmt|,
DECL|member|debug_unpack
name|debug_unpack
decl_stmt|,
DECL|member|skip_sparse_checkout
name|skip_sparse_checkout
decl_stmt|,
DECL|member|gently
name|gently
decl_stmt|,
DECL|member|show_all_errors
name|show_all_errors
decl_stmt|,
DECL|member|dry_run
name|dry_run
decl_stmt|;
DECL|member|prefix
specifier|const
name|char
modifier|*
name|prefix
decl_stmt|;
DECL|member|cache_bottom
name|int
name|cache_bottom
decl_stmt|;
DECL|member|dir
name|struct
name|dir_struct
modifier|*
name|dir
decl_stmt|;
DECL|member|pathspec
name|struct
name|pathspec
modifier|*
name|pathspec
decl_stmt|;
DECL|member|fn
name|merge_fn_t
name|fn
decl_stmt|;
DECL|member|msgs
specifier|const
name|char
modifier|*
name|msgs
index|[
name|NB_UNPACK_TREES_ERROR_TYPES
index|]
decl_stmt|;
comment|/* 	 * Store error messages in an array, each case 	 * corresponding to a error message type 	 */
DECL|member|unpack_rejects
name|struct
name|string_list
name|unpack_rejects
index|[
name|NB_UNPACK_TREES_ERROR_TYPES
index|]
decl_stmt|;
DECL|member|head_idx
name|int
name|head_idx
decl_stmt|;
DECL|member|merge_size
name|int
name|merge_size
decl_stmt|;
DECL|member|df_conflict_entry
name|struct
name|cache_entry
modifier|*
name|df_conflict_entry
decl_stmt|;
DECL|member|unpack_data
name|void
modifier|*
name|unpack_data
decl_stmt|;
DECL|member|dst_index
name|struct
name|index_state
modifier|*
name|dst_index
decl_stmt|;
DECL|member|src_index
name|struct
name|index_state
modifier|*
name|src_index
decl_stmt|;
DECL|member|result
name|struct
name|index_state
name|result
decl_stmt|;
DECL|member|el
name|struct
name|exclude_list
modifier|*
name|el
decl_stmt|;
comment|/* for internal use */
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|int
name|unpack_trees
parameter_list|(
name|unsigned
name|n
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t
parameter_list|,
name|struct
name|unpack_trees_options
modifier|*
name|options
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|threeway_merge
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|stages
parameter_list|,
name|struct
name|unpack_trees_options
modifier|*
name|o
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|twoway_merge
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|src
parameter_list|,
name|struct
name|unpack_trees_options
modifier|*
name|o
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|bind_merge
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|src
parameter_list|,
name|struct
name|unpack_trees_options
modifier|*
name|o
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|oneway_merge
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|src
parameter_list|,
name|struct
name|unpack_trees_options
modifier|*
name|o
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

