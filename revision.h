begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|REVISION_H
end_ifndef

begin_define
DECL|macro|REVISION_H
define|#
directive|define
name|REVISION_H
end_define

begin_define
DECL|macro|SEEN
define|#
directive|define
name|SEEN
value|(1u<<0)
end_define

begin_define
DECL|macro|UNINTERESTING
define|#
directive|define
name|UNINTERESTING
value|(1u<<1)
end_define

begin_define
DECL|macro|TREECHANGE
define|#
directive|define
name|TREECHANGE
value|(1u<<2)
end_define

begin_define
DECL|macro|SHOWN
define|#
directive|define
name|SHOWN
value|(1u<<3)
end_define

begin_struct
DECL|struct|rev_info
struct|struct
name|rev_info
block|{
comment|/* Starting list */
DECL|member|commits
name|struct
name|commit_list
modifier|*
name|commits
decl_stmt|;
DECL|member|pending_objects
name|struct
name|object_list
modifier|*
name|pending_objects
decl_stmt|;
comment|/* Basic information */
DECL|member|prefix
specifier|const
name|char
modifier|*
name|prefix
decl_stmt|;
DECL|member|paths
specifier|const
name|char
modifier|*
modifier|*
name|paths
decl_stmt|;
comment|/* Traversal flags */
DECL|member|dense
name|unsigned
name|int
name|dense
range|:
literal|1
decl_stmt|,
DECL|member|no_merges
name|no_merges
range|:
literal|1
decl_stmt|,
DECL|member|remove_empty_trees
name|remove_empty_trees
range|:
literal|1
decl_stmt|,
DECL|member|lifo
name|lifo
range|:
literal|1
decl_stmt|,
DECL|member|topo_order
name|topo_order
range|:
literal|1
decl_stmt|,
DECL|member|tag_objects
name|tag_objects
range|:
literal|1
decl_stmt|,
DECL|member|tree_objects
name|tree_objects
range|:
literal|1
decl_stmt|,
DECL|member|blob_objects
name|blob_objects
range|:
literal|1
decl_stmt|,
DECL|member|edge_hint
name|edge_hint
range|:
literal|1
decl_stmt|,
DECL|member|limited
name|limited
range|:
literal|1
decl_stmt|,
DECL|member|unpacked
name|unpacked
range|:
literal|1
decl_stmt|;
comment|/* special limits */
DECL|member|max_count
name|int
name|max_count
decl_stmt|;
DECL|member|max_age
name|unsigned
name|long
name|max_age
decl_stmt|;
DECL|member|min_age
name|unsigned
name|long
name|min_age
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* revision.c */
end_comment

begin_function_decl
specifier|extern
name|int
name|setup_revisions
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
name|char
modifier|*
name|def
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|prepare_revision_walk
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|struct
name|commit
modifier|*
name|get_revision
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|mark_parents_uninteresting
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|mark_tree_uninteresting
parameter_list|(
name|struct
name|tree
modifier|*
name|tree
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|name_path
struct|struct
name|name_path
block|{
DECL|member|up
name|struct
name|name_path
modifier|*
name|up
decl_stmt|;
DECL|member|elem_len
name|int
name|elem_len
decl_stmt|;
DECL|member|elem
specifier|const
name|char
modifier|*
name|elem
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|struct
name|object_list
modifier|*
modifier|*
name|add_object
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|struct
name|object_list
modifier|*
modifier|*
name|p
parameter_list|,
name|struct
name|name_path
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

