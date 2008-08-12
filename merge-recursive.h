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

begin_function_decl
name|int
name|merge_recursive
parameter_list|(
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
specifier|const
name|char
modifier|*
name|branch1
parameter_list|,
specifier|const
name|char
modifier|*
name|branch2
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

begin_function_decl
name|int
name|merge_trees
parameter_list|(
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
specifier|const
name|char
modifier|*
name|branch1
parameter_list|,
specifier|const
name|char
modifier|*
name|branch2
parameter_list|,
name|struct
name|tree
modifier|*
modifier|*
name|result
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|merge_recursive_generic
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|base_list
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|head_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|head_name
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|next_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|next_name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|merge_recursive_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|merge_recursive_setup
parameter_list|(
name|int
name|is_subtree_merge
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|tree
modifier|*
name|write_tree_from_memory
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
name|int
name|merge_recursive_verbosity
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

end_unit

