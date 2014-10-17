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

begin_include
include|#
directive|include
file|"notes.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_comment
comment|/* Remember to update object flag allocation in object.h */
end_comment

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
DECL|macro|PATCHSAME
define|#
directive|define
name|PATCHSAME
value|(1u<<9)
end_define

begin_define
DECL|macro|BOTTOM
define|#
directive|define
name|BOTTOM
value|(1u<<10)
end_define

begin_define
DECL|macro|TRACK_LINEAR
define|#
directive|define
name|TRACK_LINEAR
value|(1u<<26)
end_define

begin_define
DECL|macro|ALL_REV_FLAGS
define|#
directive|define
name|ALL_REV_FLAGS
value|(((1u<<11)-1) | TRACK_LINEAR)
end_define

begin_define
DECL|macro|DECORATE_SHORT_REFS
define|#
directive|define
name|DECORATE_SHORT_REFS
value|1
end_define

begin_define
DECL|macro|DECORATE_FULL_REFS
define|#
directive|define
name|DECORATE_FULL_REFS
value|2
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

begin_struct_decl
struct_decl|struct
name|string_list
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|saved_parents
struct_decl|;
end_struct_decl

begin_struct
DECL|struct|rev_cmdline_info
struct|struct
name|rev_cmdline_info
block|{
DECL|member|nr
name|unsigned
name|int
name|nr
decl_stmt|;
DECL|member|alloc
name|unsigned
name|int
name|alloc
decl_stmt|;
DECL|struct|rev_cmdline_entry
struct|struct
name|rev_cmdline_entry
block|{
DECL|member|item
name|struct
name|object
modifier|*
name|item
decl_stmt|;
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
enum|enum
block|{
DECL|enumerator|REV_CMD_REF
name|REV_CMD_REF
block|,
DECL|enumerator|REV_CMD_PARENTS_ONLY
name|REV_CMD_PARENTS_ONLY
block|,
DECL|enumerator|REV_CMD_LEFT
name|REV_CMD_LEFT
block|,
DECL|enumerator|REV_CMD_RIGHT
name|REV_CMD_RIGHT
block|,
DECL|enumerator|REV_CMD_MERGE_BASE
name|REV_CMD_MERGE_BASE
block|,
DECL|enumerator|REV_CMD_REV
name|REV_CMD_REV
DECL|member|whence
block|}
name|whence
enum|;
DECL|member|flags
name|unsigned
name|flags
decl_stmt|;
DECL|member|rev
block|}
modifier|*
name|rev
struct|;
block|}
struct|;
end_struct

begin_define
DECL|macro|REVISION_WALK_WALK
define|#
directive|define
name|REVISION_WALK_WALK
value|0
end_define

begin_define
DECL|macro|REVISION_WALK_NO_WALK_SORTED
define|#
directive|define
name|REVISION_WALK_NO_WALK_SORTED
value|1
end_define

begin_define
DECL|macro|REVISION_WALK_NO_WALK_UNSORTED
define|#
directive|define
name|REVISION_WALK_NO_WALK_UNSORTED
value|2
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
comment|/* The end-points specified by the end user */
DECL|member|cmdline
name|struct
name|rev_cmdline_info
name|cmdline
decl_stmt|;
comment|/* excluding from --branches, --refs, etc. expansion */
DECL|member|ref_excludes
name|struct
name|string_list
modifier|*
name|ref_excludes
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
name|struct
name|pathspec
name|prune_data
decl_stmt|;
comment|/* topo-sort */
DECL|member|sort_order
name|enum
name|rev_sort_order
name|sort_order
decl_stmt|;
DECL|member|early_output
name|unsigned
name|int
name|early_output
range|:
literal|1
decl_stmt|,
DECL|member|ignore_missing
name|ignore_missing
range|:
literal|1
decl_stmt|,
DECL|member|ignore_missing_links
name|ignore_missing_links
range|:
literal|1
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
DECL|member|no_walk
name|no_walk
range|:
literal|2
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
DECL|member|verify_objects
name|verify_objects
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
DECL|member|boundary
name|boundary
range|:
literal|2
decl_stmt|,
DECL|member|count
name|count
range|:
literal|1
decl_stmt|,
DECL|member|left_right
name|left_right
range|:
literal|1
decl_stmt|,
DECL|member|left_only
name|left_only
range|:
literal|1
decl_stmt|,
DECL|member|right_only
name|right_only
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
DECL|member|cherry_mark
name|cherry_mark
range|:
literal|1
decl_stmt|,
DECL|member|bisect
name|bisect
range|:
literal|1
decl_stmt|,
DECL|member|ancestry_path
name|ancestry_path
range|:
literal|1
decl_stmt|,
DECL|member|first_parent_only
name|first_parent_only
range|:
literal|1
decl_stmt|,
DECL|member|line_level_traverse
name|line_level_traverse
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
DECL|member|shown_dashes
name|shown_dashes
range|:
literal|1
decl_stmt|,
DECL|member|show_merge
name|show_merge
range|:
literal|1
decl_stmt|,
DECL|member|show_notes
name|show_notes
range|:
literal|1
decl_stmt|,
DECL|member|show_notes_given
name|show_notes_given
range|:
literal|1
decl_stmt|,
DECL|member|show_signature
name|show_signature
range|:
literal|1
decl_stmt|,
DECL|member|pretty_given
name|pretty_given
range|:
literal|1
decl_stmt|,
DECL|member|abbrev_commit
name|abbrev_commit
range|:
literal|1
decl_stmt|,
DECL|member|abbrev_commit_given
name|abbrev_commit_given
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
decl_stmt|,
DECL|member|date_mode_explicit
name|date_mode_explicit
range|:
literal|1
decl_stmt|,
DECL|member|preserve_subject
name|preserve_subject
range|:
literal|1
decl_stmt|;
DECL|member|disable_stdin
name|unsigned
name|int
name|disable_stdin
range|:
literal|1
decl_stmt|;
DECL|member|leak_pending
name|unsigned
name|int
name|leak_pending
range|:
literal|1
decl_stmt|;
comment|/* --show-linear-break */
DECL|member|track_linear
name|unsigned
name|int
name|track_linear
range|:
literal|1
decl_stmt|,
DECL|member|track_first_time
name|track_first_time
range|:
literal|1
decl_stmt|,
DECL|member|linear
name|linear
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
DECL|member|reroll_count
name|int
name|reroll_count
decl_stmt|;
DECL|member|message_id
name|char
modifier|*
name|message_id
decl_stmt|;
DECL|member|from_ident
name|struct
name|ident_split
name|from_ident
decl_stmt|;
DECL|member|ref_message_ids
name|struct
name|string_list
modifier|*
name|ref_message_ids
decl_stmt|;
DECL|member|add_signoff
name|int
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
DECL|member|mailmap
name|struct
name|string_list
modifier|*
name|mailmap
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
DECL|member|min_parents
name|int
name|min_parents
decl_stmt|;
DECL|member|max_parents
name|int
name|max_parents
decl_stmt|;
DECL|member|include_check
name|int
function_decl|(
modifier|*
name|include_check
function_decl|)
parameter_list|(
name|struct
name|commit
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
DECL|member|include_check_data
name|void
modifier|*
name|include_check_data
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
DECL|member|treesame
name|struct
name|decoration
name|treesame
decl_stmt|;
comment|/* notes-specific options: which refs to show */
DECL|member|notes_opt
name|struct
name|display_notes_opt
name|notes_opt
decl_stmt|;
comment|/* commit counts */
DECL|member|count_left
name|int
name|count_left
decl_stmt|;
DECL|member|count_right
name|int
name|count_right
decl_stmt|;
DECL|member|count_same
name|int
name|count_same
decl_stmt|;
comment|/* line level range that we are chasing */
DECL|member|line_log_data
name|struct
name|decoration
name|line_log_data
decl_stmt|;
comment|/* copies of the parent lists, for --full-diff display */
DECL|member|saved_parents_slab
name|struct
name|saved_parents
modifier|*
name|saved_parents_slab
decl_stmt|;
DECL|member|previous_parents
name|struct
name|commit_list
modifier|*
name|previous_parents
decl_stmt|;
DECL|member|break_bar
specifier|const
name|char
modifier|*
name|break_bar
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|int
name|ref_excluded
parameter_list|(
name|struct
name|string_list
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|clear_ref_exclusion
parameter_list|(
name|struct
name|string_list
modifier|*
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|add_ref_exclusion
parameter_list|(
name|struct
name|string_list
modifier|*
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|exclude
parameter_list|)
function_decl|;
end_function_decl

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

begin_comment
DECL|macro|REV_TREE_NEW
comment|/* Only new files */
end_comment

begin_define
DECL|macro|REV_TREE_OLD
define|#
directive|define
name|REV_TREE_OLD
value|2
end_define

begin_comment
DECL|macro|REV_TREE_OLD
comment|/* Only files removed */
end_comment

begin_define
DECL|macro|REV_TREE_DIFFERENT
define|#
directive|define
name|REV_TREE_DIFFERENT
value|3
end_define

begin_comment
DECL|macro|REV_TREE_DIFFERENT
comment|/* Mixed changes */
end_comment

begin_comment
comment|/* revision.c */
end_comment

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

begin_struct
DECL|struct|setup_revision_opt
struct|struct
name|setup_revision_opt
block|{
DECL|member|def
specifier|const
name|char
modifier|*
name|def
decl_stmt|;
DECL|member|tweak
name|void
function_decl|(
modifier|*
name|tweak
function_decl|)
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|,
name|struct
name|setup_revision_opt
modifier|*
parameter_list|)
function_decl|;
DECL|member|submodule
specifier|const
name|char
modifier|*
name|submodule
decl_stmt|;
DECL|member|assume_dashdash
name|int
name|assume_dashdash
decl_stmt|;
DECL|member|revarg_opt
name|unsigned
name|revarg_opt
decl_stmt|;
block|}
struct|;
end_struct

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
name|struct
name|setup_revision_opt
modifier|*
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

begin_define
DECL|macro|REVARG_CANNOT_BE_FILENAME
define|#
directive|define
name|REVARG_CANNOT_BE_FILENAME
value|01
end_define

begin_define
DECL|macro|REVARG_COMMITTISH
define|#
directive|define
name|REVARG_COMMITTISH
value|02
end_define

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
name|unsigned
name|revarg_opt
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|reset_revision_walk
parameter_list|(
name|void
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
name|char
modifier|*
name|get_revision_mark
parameter_list|(
specifier|const
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
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
name|put_revision_mark
parameter_list|(
specifier|const
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
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
name|char
modifier|*
name|path_name
parameter_list|(
specifier|const
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
name|show_object_with_name
parameter_list|(
name|FILE
modifier|*
parameter_list|,
name|struct
name|object
modifier|*
parameter_list|,
specifier|const
name|struct
name|name_path
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

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
name|add_pending_sha1
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
name|int
name|flags
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

begin_function_decl
specifier|extern
name|void
name|add_reflogs_to_pending
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|add_index_objects_to_pending
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|,
name|unsigned
name|int
name|flags
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
name|get_commit_action
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

begin_enum
DECL|enum|rewrite_result
enum|enum
name|rewrite_result
block|{
DECL|enumerator|rewrite_one_ok
name|rewrite_one_ok
block|,
DECL|enumerator|rewrite_one_noparents
name|rewrite_one_noparents
block|,
DECL|enumerator|rewrite_one_error
name|rewrite_one_error
block|}
enum|;
end_enum

begin_typedef
DECL|typedef|rewrite_parent_fn_t
typedef|typedef
name|enum
name|rewrite_result
function_decl|(
modifier|*
name|rewrite_parent_fn_t
function_decl|)
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|struct
name|commit
modifier|*
modifier|*
name|pp
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
specifier|extern
name|int
name|rewrite_parents
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
parameter_list|,
name|rewrite_parent_fn_t
name|rewrite_parent
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Save a copy of the parent list, and return the saved copy.  This is  * used by the log machinery to retrieve the original parents when  * commit->parents has been modified by history simpification.  *  * You may only call save_parents() once per commit (this is checked  * for non-root commits).  *  * get_saved_parents() will transparently return commit->parents if  * history simplification is off.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|save_parents
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

begin_function_decl
specifier|extern
name|struct
name|commit_list
modifier|*
name|get_saved_parents
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
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
name|free_saved_parents
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

