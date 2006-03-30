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

begin_include
include|#
directive|include
file|"tree-walk.h"
end_include

begin_struct_decl
struct_decl|struct
name|diff_options
struct_decl|;
end_struct_decl

begin_typedef
DECL|typedef|change_fn_t
typedef|typedef
name|void
function_decl|(
modifier|*
name|change_fn_t
function_decl|)
parameter_list|(
name|struct
name|diff_options
modifier|*
name|options
parameter_list|,
name|unsigned
name|old_mode
parameter_list|,
name|unsigned
name|new_mode
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|add_remove_fn_t
typedef|typedef
name|void
function_decl|(
modifier|*
name|add_remove_fn_t
function_decl|)
parameter_list|(
name|struct
name|diff_options
modifier|*
name|options
parameter_list|,
name|int
name|addremove
parameter_list|,
name|unsigned
name|mode
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|diff_options
struct|struct
name|diff_options
block|{
DECL|member|paths
specifier|const
name|char
modifier|*
modifier|*
name|paths
decl_stmt|;
DECL|member|filter
specifier|const
name|char
modifier|*
name|filter
decl_stmt|;
DECL|member|orderfile
specifier|const
name|char
modifier|*
name|orderfile
decl_stmt|;
DECL|member|pickaxe
specifier|const
name|char
modifier|*
name|pickaxe
decl_stmt|;
DECL|member|recursive
name|unsigned
name|recursive
range|:
literal|1
decl_stmt|,
DECL|member|tree_in_recursive
name|tree_in_recursive
range|:
literal|1
decl_stmt|,
DECL|member|full_index
name|full_index
range|:
literal|1
decl_stmt|;
DECL|member|break_opt
name|int
name|break_opt
decl_stmt|;
DECL|member|detect_rename
name|int
name|detect_rename
decl_stmt|;
DECL|member|find_copies_harder
name|int
name|find_copies_harder
decl_stmt|;
DECL|member|line_termination
name|int
name|line_termination
decl_stmt|;
DECL|member|output_format
name|int
name|output_format
decl_stmt|;
DECL|member|pickaxe_opts
name|int
name|pickaxe_opts
decl_stmt|;
DECL|member|rename_score
name|int
name|rename_score
decl_stmt|;
DECL|member|reverse_diff
name|int
name|reverse_diff
decl_stmt|;
DECL|member|rename_limit
name|int
name|rename_limit
decl_stmt|;
DECL|member|setup
name|int
name|setup
decl_stmt|;
DECL|member|abbrev
name|int
name|abbrev
decl_stmt|;
DECL|member|change
name|change_fn_t
name|change
decl_stmt|;
DECL|member|add_remove
name|add_remove_fn_t
name|add_remove
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|void
name|diff_tree_setup_paths
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|paths
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|diff_tree
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|t1
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t2
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|diff_tree_sha1
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|old
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|combine_diff_path
struct|struct
name|combine_diff_path
block|{
DECL|member|next
name|struct
name|combine_diff_path
modifier|*
name|next
decl_stmt|;
DECL|member|len
name|int
name|len
decl_stmt|;
DECL|member|path
name|char
modifier|*
name|path
decl_stmt|;
DECL|member|mode
name|unsigned
name|int
name|mode
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|struct|combine_diff_parent
struct|struct
name|combine_diff_parent
block|{
DECL|member|status
name|char
name|status
decl_stmt|;
DECL|member|mode
name|unsigned
name|int
name|mode
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|parent
block|}
name|parent
index|[
name|FLEX_ARRAY
index|]
struct|;
block|}
struct|;
end_struct

begin_define
DECL|macro|combine_diff_path_size
define|#
directive|define
name|combine_diff_path_size
parameter_list|(
name|n
parameter_list|,
name|l
parameter_list|)
define|\
value|(sizeof(struct combine_diff_path) + \ 	 sizeof(struct combine_diff_parent) * (n) + (l) + 1)
end_define

begin_function_decl
specifier|extern
name|int
name|show_combined_diff
parameter_list|(
name|struct
name|combine_diff_path
modifier|*
name|elem
parameter_list|,
name|int
name|num_parent
parameter_list|,
name|int
name|dense
parameter_list|,
specifier|const
name|char
modifier|*
name|header
parameter_list|,
name|struct
name|diff_options
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|diff_tree_combined_merge
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_addremove
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|,
name|int
name|addremove
parameter_list|,
name|unsigned
name|mode
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_change
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|,
name|unsigned
name|mode1
parameter_list|,
name|unsigned
name|mode2
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha2
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_unmerge
parameter_list|(
name|struct
name|diff_options
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
specifier|extern
name|int
name|diff_scoreopt_parse
parameter_list|(
specifier|const
name|char
modifier|*
name|opt
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|DIFF_SETUP_REVERSE
define|#
directive|define
name|DIFF_SETUP_REVERSE
value|1
end_define

begin_define
DECL|macro|DIFF_SETUP_USE_CACHE
define|#
directive|define
name|DIFF_SETUP_USE_CACHE
value|2
end_define

begin_define
DECL|macro|DIFF_SETUP_USE_SIZE_CACHE
define|#
directive|define
name|DIFF_SETUP_USE_SIZE_CACHE
value|4
end_define

begin_function_decl
specifier|extern
name|int
name|git_diff_config
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
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_setup
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|diff_opt_parse
parameter_list|(
name|struct
name|diff_options
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
specifier|extern
name|int
name|diff_setup_done
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|DIFF_DETECT_RENAME
define|#
directive|define
name|DIFF_DETECT_RENAME
value|1
end_define

begin_define
DECL|macro|DIFF_DETECT_COPY
define|#
directive|define
name|DIFF_DETECT_COPY
value|2
end_define

begin_define
DECL|macro|DIFF_PICKAXE_ALL
define|#
directive|define
name|DIFF_PICKAXE_ALL
value|1
end_define

begin_function_decl
specifier|extern
name|void
name|diffcore_std
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diffcore_std_no_resolve
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|COMMON_DIFF_OPTIONS_HELP
define|#
directive|define
name|COMMON_DIFF_OPTIONS_HELP
define|\
value|"\ncommon diff options:\n" \ "  -z            output diff-raw with lines terminated with NUL.\n" \ "  -p            output patch format.\n" \ "  -u            synonym for -p.\n" \ "  --name-only   show only names of changed files.\n" \ "  --name-status show names and status of changed files.\n" \ "  --full-index  show full object name on index lines.\n" \ "  --abbrev=<n>  abbreviate object names in diff-tree header and diff-raw.\n" \ "  -R            swap input file pairs.\n" \ "  -B            detect complete rewrites.\n" \ "  -M            detect renames.\n" \ "  -C            detect copies.\n" \ "  --find-copies-harder\n" \ "                try unchanged files as candidate for copy detection.\n" \ "  -l<n>         limit rename attempts up to<n> paths.\n" \ "  -O<file>      reorder diffs according to the<file>.\n" \ "  -S<string>    find filepair whose only one side contains the string.\n" \ "  --pickaxe-all\n" \ "                show all files diff when -S is used and hit is found.\n"
end_define

begin_function_decl
specifier|extern
name|int
name|diff_queue_is_empty
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|DIFF_FORMAT_RAW
define|#
directive|define
name|DIFF_FORMAT_RAW
value|1
end_define

begin_define
DECL|macro|DIFF_FORMAT_PATCH
define|#
directive|define
name|DIFF_FORMAT_PATCH
value|2
end_define

begin_define
DECL|macro|DIFF_FORMAT_NO_OUTPUT
define|#
directive|define
name|DIFF_FORMAT_NO_OUTPUT
value|3
end_define

begin_define
DECL|macro|DIFF_FORMAT_NAME
define|#
directive|define
name|DIFF_FORMAT_NAME
value|4
end_define

begin_define
DECL|macro|DIFF_FORMAT_NAME_STATUS
define|#
directive|define
name|DIFF_FORMAT_NAME_STATUS
value|5
end_define

begin_function_decl
specifier|extern
name|void
name|diff_flush
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* diff-raw status letters */
end_comment

begin_define
DECL|macro|DIFF_STATUS_ADDED
define|#
directive|define
name|DIFF_STATUS_ADDED
value|'A'
end_define

begin_define
DECL|macro|DIFF_STATUS_COPIED
define|#
directive|define
name|DIFF_STATUS_COPIED
value|'C'
end_define

begin_define
DECL|macro|DIFF_STATUS_DELETED
define|#
directive|define
name|DIFF_STATUS_DELETED
value|'D'
end_define

begin_define
DECL|macro|DIFF_STATUS_MODIFIED
define|#
directive|define
name|DIFF_STATUS_MODIFIED
value|'M'
end_define

begin_define
DECL|macro|DIFF_STATUS_RENAMED
define|#
directive|define
name|DIFF_STATUS_RENAMED
value|'R'
end_define

begin_define
DECL|macro|DIFF_STATUS_TYPE_CHANGED
define|#
directive|define
name|DIFF_STATUS_TYPE_CHANGED
value|'T'
end_define

begin_define
DECL|macro|DIFF_STATUS_UNKNOWN
define|#
directive|define
name|DIFF_STATUS_UNKNOWN
value|'X'
end_define

begin_define
DECL|macro|DIFF_STATUS_UNMERGED
define|#
directive|define
name|DIFF_STATUS_UNMERGED
value|'U'
end_define

begin_comment
comment|/* these are not diff-raw status letters proper, but used by  * diffcore-filter insn to specify additional restrictions.  */
end_comment

begin_define
DECL|macro|DIFF_STATUS_FILTER_AON
define|#
directive|define
name|DIFF_STATUS_FILTER_AON
value|'*'
end_define

begin_define
DECL|macro|DIFF_STATUS_FILTER_BROKEN
define|#
directive|define
name|DIFF_STATUS_FILTER_BROKEN
value|'B'
end_define

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|diff_unique_abbrev
parameter_list|(
specifier|const
name|unsigned
name|char
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

begin_comment
comment|/* DIFF_H */
end_comment

end_unit

