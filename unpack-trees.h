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

begin_struct
DECL|struct|unpack_trees_error_msgs
struct|struct
name|unpack_trees_error_msgs
block|{
DECL|member|would_overwrite
specifier|const
name|char
modifier|*
name|would_overwrite
decl_stmt|;
DECL|member|not_uptodate_file
specifier|const
name|char
modifier|*
name|not_uptodate_file
decl_stmt|;
DECL|member|not_uptodate_dir
specifier|const
name|char
modifier|*
name|not_uptodate_dir
decl_stmt|;
DECL|member|would_lose_untracked
specifier|const
name|char
modifier|*
name|would_lose_untracked
decl_stmt|;
DECL|member|bind_overlap
specifier|const
name|char
modifier|*
name|bind_overlap
decl_stmt|;
block|}
struct|;
end_struct

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
DECL|member|gently
name|gently
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
DECL|member|fn
name|merge_fn_t
name|fn
decl_stmt|;
DECL|member|msgs
name|struct
name|unpack_trees_error_msgs
name|msgs
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

