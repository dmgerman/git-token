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

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"grep.h"
end_include

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
DECL|macro|TREESAME
define|#
directive|define
name|TREESAME
value|(1u<<2)
end_define

begin_define
DECL|macro|SHOWN
define|#
directive|define
name|SHOWN
value|(1u<<3)
end_define

begin_define
DECL|macro|TMP_MARK
define|#
directive|define
name|TMP_MARK
value|(1u<<4)
end_define

begin_comment
DECL|macro|TMP_MARK
comment|/* for isolated cases; clean after use */
end_comment

begin_define
DECL|macro|BOUNDARY
define|#
directive|define
name|BOUNDARY
value|(1u<<5)
end_define

begin_define
DECL|macro|CHILD_SHOWN
define|#
directive|define
name|CHILD_SHOWN
value|(1u<<6)
end_define

begin_define
DECL|macro|ADDED
define|#
directive|define
name|ADDED
value|(1u<<7)
end_define

begin_comment
DECL|macro|ADDED
comment|/* Parents already parsed and added? */
end_comment

begin_define
DECL|macro|SYMMETRIC_LEFT
define|#
directive|define
name|SYMMETRIC_LEFT
value|(1u<<8)
end_define

begin_define
DECL|macro|ALL_REV_FLAGS
define|#
directive|define
name|ALL_REV_FLAGS
value|((1u<<9)-1)
end_define

begin_struct_decl
struct_decl|struct
name|rev_info
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|log_info
struct_decl|;
end_struct_decl

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
DECL|member|pending
name|struct
name|object_array
name|pending
decl_stmt|;
comment|/* Parents of shown commits */
DECL|member|boundary_commits
name|struct
name|object_array
name|boundary_commits
decl_stmt|;
comment|/* Basic information */
DECL|member|prefix
specifier|const
name|char
modifier|*
name|prefix
decl_stmt|;
DECL|member|def
specifier|const
name|char
modifier|*
name|def
decl_stmt|;
DECL|member|prune_data
name|void
modifier|*
name|prune_data
decl_stmt|;
DECL|member|early_output
name|unsigned
name|int
name|early_output
decl_stmt|;
comment|/* Traversal flags */
DECL|member|dense
name|unsigned
name|int
name|dense
range|:
literal|1
decl_stmt|,
DECL|member|prune
name|prune
range|:
literal|1
decl_stmt|,
DECL|member|no_merges
name|no_merges
range|:
literal|1
decl_stmt|,
DECL|member|no_walk
name|no_walk
range|:
literal|1
decl_stmt|,
DECL|member|show_all
name|show_all
range|:
literal|1
decl_stmt|,
DECL|member|remove_empty_trees
name|remove_empty_trees
range|:
literal|1
decl_stmt|,
DECL|member|simplify_history
name|simplify_history
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
DECL|member|simplify_merges
name|simplify_merges
range|:
literal|1
decl_stmt|,
DECL|member|simplify_by_decoration
name|simplify_by_decoration
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
decl_stmt|,
DECL|member|kept_pack_only
name|kept_pack_only
range|:
literal|1
decl_stmt|,
DECL|member|boundary
name|boundary
range|:
literal|2
decl_stmt|,
DECL|member|left_right
name|left_right
range|:
literal|1
decl_stmt|,
DECL|member|rewrite_parents
name|rewrite_parents
range|:
literal|1
decl_stmt|,
DECL|member|print_parents
name|print_parents
range|:
literal|1
decl_stmt|,
DECL|member|show_source
name|show_source
range|:
literal|1
decl_stmt|,
DECL|member|show_decorations
name|show_decorations
range|:
literal|1
decl_stmt|,
DECL|member|reverse
name|reverse
range|:
literal|1
decl_stmt|,
DECL|member|reverse_output_stage
name|reverse_output_stage
range|:
literal|1
decl_stmt|,
DECL|member|cherry_pick
name|cherry_pick
range|:
literal|1
decl_stmt|,
DECL|member|first_parent_only
name|first_parent_only
range|:
literal|1
decl_stmt|;
comment|/* Diff flags */
DECL|member|diff
name|unsigned
name|int
name|diff
range|:
literal|1
decl_stmt|,
DECL|member|full_diff
name|full_diff
range|:
literal|1
decl_stmt|,
DECL|member|show_root_diff
name|show_root_diff
range|:
literal|1
decl_stmt|,
DECL|member|no_commit_id
name|no_commit_id
range|:
literal|1
decl_stmt|,
DECL|member|verbose_header
name|verbose_header
range|:
literal|1
decl_stmt|,
DECL|member|ignore_merges
name|ignore_merges
range|:
literal|1
decl_stmt|,
DECL|member|combine_merges
name|combine_merges
range|:
literal|1
decl_stmt|,
DECL|member|dense_combined_merges
name|dense_combined_merges
range|:
literal|1
decl_stmt|,
DECL|member|always_show_header
name|always_show_header
range|:
literal|1
decl_stmt|;
comment|/* Format info */
DECL|member|shown_one
name|unsigned
name|int
name|shown_one
range|:
literal|1
decl_stmt|,
DECL|member|show_merge
name|show_merge
range|:
literal|1
decl_stmt|,
DECL|member|abbrev_commit
name|abbrev_commit
range|:
literal|1
decl_stmt|,
DECL|member|use_terminator
name|use_terminator
range|:
literal|1
decl_stmt|,
DECL|member|missing_newline
name|missing_newline
range|:
literal|1
decl_stmt|;
DECL|member|date_mode
name|enum
name|date_mode
name|date_mode
decl_stmt|;
DECL|member|abbrev
name|unsigned
name|int
name|abbrev
decl_stmt|;
DECL|member|commit_format
name|enum
name|cmit_fmt
name|commit_format
decl_stmt|;
DECL|member|loginfo
name|struct
name|log_info
modifier|*
name|loginfo
decl_stmt|;
DECL|member|nr
DECL|member|total
name|int
name|nr
decl_stmt|,
name|total
decl_stmt|;
DECL|member|mime_boundary
specifier|const
name|char
modifier|*
name|mime_boundary
decl_stmt|;
DECL|member|patch_suffix
specifier|const
name|char
modifier|*
name|patch_suffix
decl_stmt|;
DECL|member|numbered_files
name|int
name|numbered_files
decl_stmt|;
DECL|member|message_id
name|char
modifier|*
name|message_id
decl_stmt|;
DECL|member|ref_message_ids
name|struct
name|string_list
modifier|*
name|ref_message_ids
decl_stmt|;
DECL|member|add_signoff
specifier|const
name|char
modifier|*
name|add_signoff
decl_stmt|;
DECL|member|extra_headers
specifier|const
name|char
modifier|*
name|extra_headers
decl_stmt|;
DECL|member|log_reencode
specifier|const
name|char
modifier|*
name|log_reencode
decl_stmt|;
DECL|member|subject_prefix
specifier|const
name|char
modifier|*
name|subject_prefix
decl_stmt|;
DECL|member|no_inline
name|int
name|no_inline
decl_stmt|;
DECL|member|show_log_size
name|int
name|show_log_size
decl_stmt|;
comment|/* Filter by commit log message */
DECL|member|grep_filter
name|struct
name|grep_opt
name|grep_filter
decl_stmt|;
comment|/* Display history graph */
DECL|member|graph
name|struct
name|git_graph
modifier|*
name|graph
decl_stmt|;
comment|/* special limits */
DECL|member|skip_count
name|int
name|skip_count
decl_stmt|;
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
comment|/* diff info for patches and for paths limiting */
DECL|member|diffopt
name|struct
name|diff_options
name|diffopt
decl_stmt|;
DECL|member|pruning
name|struct
name|diff_options
name|pruning
decl_stmt|;
DECL|member|reflog_info
name|struct
name|reflog_walk_info
modifier|*
name|reflog_info
decl_stmt|;
DECL|member|children
name|struct
name|decoration
name|children
decl_stmt|;
DECL|member|merge_simplification
name|struct
name|decoration
name|merge_simplification
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|REV_TREE_SAME
define|#
directive|define
name|REV_TREE_SAME
value|0
end_define

begin_define
DECL|macro|REV_TREE_NEW
define|#
directive|define
name|REV_TREE_NEW
value|1
end_define

begin_define
DECL|macro|REV_TREE_DIFFERENT
define|#
directive|define
name|REV_TREE_DIFFERENT
value|2
end_define

begin_comment
comment|/* revision.c */
end_comment

begin_function_decl
name|void
name|read_revisions_from_stdin
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
function_decl|;
end_function_decl

begin_typedef
DECL|typedef|show_early_output_fn_t
typedef|typedef
name|void
function_decl|(
modifier|*
name|show_early_output_fn_t
function_decl|)
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|,
name|struct
name|commit_list
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_decl_stmt
specifier|extern
specifier|volatile
name|show_early_output_fn_t
name|show_early_output
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|extern
name|void
name|init_revisions
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
function_decl|;
end_function_decl

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
name|parse_revision_opt
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|struct
name|parse_opt_ctx_t
modifier|*
name|ctx
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|options
parameter_list|,
specifier|const
name|char
modifier|*
specifier|const
name|usagestr
index|[]
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|handle_revision_arg
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|int
name|flags
parameter_list|,
name|int
name|cant_be_filename
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
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
name|void
name|add_object
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|struct
name|object_array
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

begin_function_decl
specifier|extern
name|void
name|add_pending_object
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|struct
name|object
modifier|*
name|obj
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|add_head_to_pending
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_enum
DECL|enum|commit_action
enum|enum
name|commit_action
block|{
DECL|enumerator|commit_ignore
name|commit_ignore
block|,
DECL|enumerator|commit_show
name|commit_show
block|,
DECL|enumerator|commit_error
name|commit_error
block|}
enum|;
end_enum

begin_function_decl
specifier|extern
name|enum
name|commit_action
name|simplify_commit
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

