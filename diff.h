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

begin_include
include|#
directive|include
file|"pathspec.h"
end_include

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_struct_decl
struct_decl|struct
name|rev_info
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|diff_options
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|diff_queue_struct
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|strbuf
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|diff_filespec
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|userdiff_driver
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|sha1_array
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|commit
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|combine_diff_path
struct_decl|;
end_struct_decl

begin_typedef
DECL|typedef|pathchange_fn_t
typedef|typedef
name|int
function_decl|(
modifier|*
name|pathchange_fn_t
function_decl|)
parameter_list|(
name|struct
name|diff_options
modifier|*
name|options
parameter_list|,
name|struct
name|combine_diff_path
modifier|*
name|path
parameter_list|)
function_decl|;
end_typedef

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
name|int
name|old_sha1_valid
parameter_list|,
name|int
name|new_sha1_valid
parameter_list|,
specifier|const
name|char
modifier|*
name|fullpath
parameter_list|,
name|unsigned
name|old_dirty_submodule
parameter_list|,
name|unsigned
name|new_dirty_submodule
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
name|int
name|sha1_valid
parameter_list|,
specifier|const
name|char
modifier|*
name|fullpath
parameter_list|,
name|unsigned
name|dirty_submodule
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|diff_format_fn_t
typedef|typedef
name|void
function_decl|(
modifier|*
name|diff_format_fn_t
function_decl|)
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
name|q
parameter_list|,
name|struct
name|diff_options
modifier|*
name|options
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|diff_prefix_fn_t
typedef|typedef
name|struct
name|strbuf
modifier|*
function_decl|(
modifier|*
name|diff_prefix_fn_t
function_decl|)
parameter_list|(
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_typedef

begin_define
DECL|macro|DIFF_FORMAT_RAW
define|#
directive|define
name|DIFF_FORMAT_RAW
value|0x0001
end_define

begin_define
DECL|macro|DIFF_FORMAT_DIFFSTAT
define|#
directive|define
name|DIFF_FORMAT_DIFFSTAT
value|0x0002
end_define

begin_define
DECL|macro|DIFF_FORMAT_NUMSTAT
define|#
directive|define
name|DIFF_FORMAT_NUMSTAT
value|0x0004
end_define

begin_define
DECL|macro|DIFF_FORMAT_SUMMARY
define|#
directive|define
name|DIFF_FORMAT_SUMMARY
value|0x0008
end_define

begin_define
DECL|macro|DIFF_FORMAT_PATCH
define|#
directive|define
name|DIFF_FORMAT_PATCH
value|0x0010
end_define

begin_define
DECL|macro|DIFF_FORMAT_SHORTSTAT
define|#
directive|define
name|DIFF_FORMAT_SHORTSTAT
value|0x0020
end_define

begin_define
DECL|macro|DIFF_FORMAT_DIRSTAT
define|#
directive|define
name|DIFF_FORMAT_DIRSTAT
value|0x0040
end_define

begin_comment
comment|/* These override all above */
end_comment

begin_define
DECL|macro|DIFF_FORMAT_NAME
define|#
directive|define
name|DIFF_FORMAT_NAME
value|0x0100
end_define

begin_define
DECL|macro|DIFF_FORMAT_NAME_STATUS
define|#
directive|define
name|DIFF_FORMAT_NAME_STATUS
value|0x0200
end_define

begin_define
DECL|macro|DIFF_FORMAT_CHECKDIFF
define|#
directive|define
name|DIFF_FORMAT_CHECKDIFF
value|0x0400
end_define

begin_comment
comment|/* Same as output_format = 0 but we know that -s flag was given  * and we should not give default value to output_format.  */
end_comment

begin_define
DECL|macro|DIFF_FORMAT_NO_OUTPUT
define|#
directive|define
name|DIFF_FORMAT_NO_OUTPUT
value|0x0800
end_define

begin_define
DECL|macro|DIFF_FORMAT_CALLBACK
define|#
directive|define
name|DIFF_FORMAT_CALLBACK
value|0x1000
end_define

begin_define
DECL|macro|DIFF_OPT_RECURSIVE
define|#
directive|define
name|DIFF_OPT_RECURSIVE
value|(1<<  0)
end_define

begin_define
DECL|macro|DIFF_OPT_TREE_IN_RECURSIVE
define|#
directive|define
name|DIFF_OPT_TREE_IN_RECURSIVE
value|(1<<  1)
end_define

begin_define
DECL|macro|DIFF_OPT_BINARY
define|#
directive|define
name|DIFF_OPT_BINARY
value|(1<<  2)
end_define

begin_define
DECL|macro|DIFF_OPT_TEXT
define|#
directive|define
name|DIFF_OPT_TEXT
value|(1<<  3)
end_define

begin_define
DECL|macro|DIFF_OPT_FULL_INDEX
define|#
directive|define
name|DIFF_OPT_FULL_INDEX
value|(1<<  4)
end_define

begin_define
DECL|macro|DIFF_OPT_SILENT_ON_REMOVE
define|#
directive|define
name|DIFF_OPT_SILENT_ON_REMOVE
value|(1<<  5)
end_define

begin_define
DECL|macro|DIFF_OPT_FIND_COPIES_HARDER
define|#
directive|define
name|DIFF_OPT_FIND_COPIES_HARDER
value|(1<<  6)
end_define

begin_define
DECL|macro|DIFF_OPT_FOLLOW_RENAMES
define|#
directive|define
name|DIFF_OPT_FOLLOW_RENAMES
value|(1<<  7)
end_define

begin_define
DECL|macro|DIFF_OPT_RENAME_EMPTY
define|#
directive|define
name|DIFF_OPT_RENAME_EMPTY
value|(1<<  8)
end_define

begin_comment
comment|/* (1<<  9) unused */
end_comment

begin_define
DECL|macro|DIFF_OPT_HAS_CHANGES
define|#
directive|define
name|DIFF_OPT_HAS_CHANGES
value|(1<< 10)
end_define

begin_define
DECL|macro|DIFF_OPT_QUICK
define|#
directive|define
name|DIFF_OPT_QUICK
value|(1<< 11)
end_define

begin_define
DECL|macro|DIFF_OPT_NO_INDEX
define|#
directive|define
name|DIFF_OPT_NO_INDEX
value|(1<< 12)
end_define

begin_define
DECL|macro|DIFF_OPT_ALLOW_EXTERNAL
define|#
directive|define
name|DIFF_OPT_ALLOW_EXTERNAL
value|(1<< 13)
end_define

begin_define
DECL|macro|DIFF_OPT_EXIT_WITH_STATUS
define|#
directive|define
name|DIFF_OPT_EXIT_WITH_STATUS
value|(1<< 14)
end_define

begin_define
DECL|macro|DIFF_OPT_REVERSE_DIFF
define|#
directive|define
name|DIFF_OPT_REVERSE_DIFF
value|(1<< 15)
end_define

begin_define
DECL|macro|DIFF_OPT_CHECK_FAILED
define|#
directive|define
name|DIFF_OPT_CHECK_FAILED
value|(1<< 16)
end_define

begin_define
DECL|macro|DIFF_OPT_RELATIVE_NAME
define|#
directive|define
name|DIFF_OPT_RELATIVE_NAME
value|(1<< 17)
end_define

begin_define
DECL|macro|DIFF_OPT_IGNORE_SUBMODULES
define|#
directive|define
name|DIFF_OPT_IGNORE_SUBMODULES
value|(1<< 18)
end_define

begin_define
DECL|macro|DIFF_OPT_DIRSTAT_CUMULATIVE
define|#
directive|define
name|DIFF_OPT_DIRSTAT_CUMULATIVE
value|(1<< 19)
end_define

begin_define
DECL|macro|DIFF_OPT_DIRSTAT_BY_FILE
define|#
directive|define
name|DIFF_OPT_DIRSTAT_BY_FILE
value|(1<< 20)
end_define

begin_define
DECL|macro|DIFF_OPT_ALLOW_TEXTCONV
define|#
directive|define
name|DIFF_OPT_ALLOW_TEXTCONV
value|(1<< 21)
end_define

begin_define
DECL|macro|DIFF_OPT_DIFF_FROM_CONTENTS
define|#
directive|define
name|DIFF_OPT_DIFF_FROM_CONTENTS
value|(1<< 22)
end_define

begin_define
DECL|macro|DIFF_OPT_DIRTY_SUBMODULES
define|#
directive|define
name|DIFF_OPT_DIRTY_SUBMODULES
value|(1<< 24)
end_define

begin_define
DECL|macro|DIFF_OPT_IGNORE_UNTRACKED_IN_SUBMODULES
define|#
directive|define
name|DIFF_OPT_IGNORE_UNTRACKED_IN_SUBMODULES
value|(1<< 25)
end_define

begin_define
DECL|macro|DIFF_OPT_IGNORE_DIRTY_SUBMODULES
define|#
directive|define
name|DIFF_OPT_IGNORE_DIRTY_SUBMODULES
value|(1<< 26)
end_define

begin_define
DECL|macro|DIFF_OPT_OVERRIDE_SUBMODULE_CONFIG
define|#
directive|define
name|DIFF_OPT_OVERRIDE_SUBMODULE_CONFIG
value|(1<< 27)
end_define

begin_define
DECL|macro|DIFF_OPT_DIRSTAT_BY_LINE
define|#
directive|define
name|DIFF_OPT_DIRSTAT_BY_LINE
value|(1<< 28)
end_define

begin_define
DECL|macro|DIFF_OPT_FUNCCONTEXT
define|#
directive|define
name|DIFF_OPT_FUNCCONTEXT
value|(1<< 29)
end_define

begin_define
DECL|macro|DIFF_OPT_PICKAXE_IGNORE_CASE
define|#
directive|define
name|DIFF_OPT_PICKAXE_IGNORE_CASE
value|(1<< 30)
end_define

begin_define
DECL|macro|DIFF_OPT_DEFAULT_FOLLOW_RENAMES
define|#
directive|define
name|DIFF_OPT_DEFAULT_FOLLOW_RENAMES
value|(1U<< 31)
end_define

begin_define
DECL|macro|DIFF_OPT_TST
define|#
directive|define
name|DIFF_OPT_TST
parameter_list|(
name|opts
parameter_list|,
name|flag
parameter_list|)
value|((opts)->flags& DIFF_OPT_##flag)
end_define

begin_define
DECL|macro|DIFF_OPT_TOUCHED
define|#
directive|define
name|DIFF_OPT_TOUCHED
parameter_list|(
name|opts
parameter_list|,
name|flag
parameter_list|)
value|((opts)->touched_flags& DIFF_OPT_##flag)
end_define

begin_define
DECL|macro|DIFF_OPT_SET
define|#
directive|define
name|DIFF_OPT_SET
parameter_list|(
name|opts
parameter_list|,
name|flag
parameter_list|)
value|(((opts)->flags |= DIFF_OPT_##flag),((opts)->touched_flags |= DIFF_OPT_##flag))
end_define

begin_define
DECL|macro|DIFF_OPT_CLR
define|#
directive|define
name|DIFF_OPT_CLR
parameter_list|(
name|opts
parameter_list|,
name|flag
parameter_list|)
value|(((opts)->flags&= ~DIFF_OPT_##flag),((opts)->touched_flags |= DIFF_OPT_##flag))
end_define

begin_define
DECL|macro|DIFF_XDL_TST
define|#
directive|define
name|DIFF_XDL_TST
parameter_list|(
name|opts
parameter_list|,
name|flag
parameter_list|)
value|((opts)->xdl_opts& XDF_##flag)
end_define

begin_define
DECL|macro|DIFF_XDL_SET
define|#
directive|define
name|DIFF_XDL_SET
parameter_list|(
name|opts
parameter_list|,
name|flag
parameter_list|)
value|((opts)->xdl_opts |= XDF_##flag)
end_define

begin_define
DECL|macro|DIFF_XDL_CLR
define|#
directive|define
name|DIFF_XDL_CLR
parameter_list|(
name|opts
parameter_list|,
name|flag
parameter_list|)
value|((opts)->xdl_opts&= ~XDF_##flag)
end_define

begin_define
DECL|macro|DIFF_WITH_ALG
define|#
directive|define
name|DIFF_WITH_ALG
parameter_list|(
name|opts
parameter_list|,
name|flag
parameter_list|)
value|(((opts)->xdl_opts& ~XDF_DIFF_ALGORITHM_MASK) | XDF_##flag)
end_define

begin_enum
DECL|enum|diff_words_type
enum|enum
name|diff_words_type
block|{
DECL|enumerator|DIFF_WORDS_NONE
name|DIFF_WORDS_NONE
init|=
literal|0
block|,
DECL|enumerator|DIFF_WORDS_PORCELAIN
name|DIFF_WORDS_PORCELAIN
block|,
DECL|enumerator|DIFF_WORDS_PLAIN
name|DIFF_WORDS_PLAIN
block|,
DECL|enumerator|DIFF_WORDS_COLOR
name|DIFF_WORDS_COLOR
block|}
enum|;
end_enum

begin_enum
DECL|enum|diff_submodule_format
enum|enum
name|diff_submodule_format
block|{
DECL|enumerator|DIFF_SUBMODULE_SHORT
name|DIFF_SUBMODULE_SHORT
init|=
literal|0
block|,
DECL|enumerator|DIFF_SUBMODULE_LOG
name|DIFF_SUBMODULE_LOG
block|,
DECL|enumerator|DIFF_SUBMODULE_INLINE_DIFF
name|DIFF_SUBMODULE_INLINE_DIFF
block|}
enum|;
end_enum

begin_struct
DECL|struct|diff_options
struct|struct
name|diff_options
block|{
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
DECL|member|single_follow
specifier|const
name|char
modifier|*
name|single_follow
decl_stmt|;
DECL|member|a_prefix
DECL|member|b_prefix
specifier|const
name|char
modifier|*
name|a_prefix
decl_stmt|,
modifier|*
name|b_prefix
decl_stmt|;
DECL|member|line_prefix
specifier|const
name|char
modifier|*
name|line_prefix
decl_stmt|;
DECL|member|line_prefix_length
name|size_t
name|line_prefix_length
decl_stmt|;
DECL|member|flags
name|unsigned
name|flags
decl_stmt|;
DECL|member|touched_flags
name|unsigned
name|touched_flags
decl_stmt|;
comment|/* diff-filter bits */
DECL|member|filter
name|unsigned
name|int
name|filter
decl_stmt|;
DECL|member|use_color
name|int
name|use_color
decl_stmt|;
DECL|member|context
name|int
name|context
decl_stmt|;
DECL|member|interhunkcontext
name|int
name|interhunkcontext
decl_stmt|;
DECL|member|break_opt
name|int
name|break_opt
decl_stmt|;
DECL|member|detect_rename
name|int
name|detect_rename
decl_stmt|;
DECL|member|irreversible_delete
name|int
name|irreversible_delete
decl_stmt|;
DECL|member|skip_stat_unmatch
name|int
name|skip_stat_unmatch
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
DECL|member|rename_limit
name|int
name|rename_limit
decl_stmt|;
DECL|member|needed_rename_limit
name|int
name|needed_rename_limit
decl_stmt|;
DECL|member|degraded_cc_to_c
name|int
name|degraded_cc_to_c
decl_stmt|;
DECL|member|show_rename_progress
name|int
name|show_rename_progress
decl_stmt|;
DECL|member|dirstat_permille
name|int
name|dirstat_permille
decl_stmt|;
DECL|member|setup
name|int
name|setup
decl_stmt|;
DECL|member|abbrev
name|int
name|abbrev
decl_stmt|;
comment|/* white-space error highlighting */
DECL|macro|WSEH_NEW
define|#
directive|define
name|WSEH_NEW
value|1
DECL|macro|WSEH_CONTEXT
define|#
directive|define
name|WSEH_CONTEXT
value|2
DECL|macro|WSEH_OLD
define|#
directive|define
name|WSEH_OLD
value|4
DECL|member|ws_error_highlight
name|unsigned
name|ws_error_highlight
decl_stmt|;
DECL|member|prefix
specifier|const
name|char
modifier|*
name|prefix
decl_stmt|;
DECL|member|prefix_length
name|int
name|prefix_length
decl_stmt|;
DECL|member|stat_sep
specifier|const
name|char
modifier|*
name|stat_sep
decl_stmt|;
DECL|member|xdl_opts
name|long
name|xdl_opts
decl_stmt|;
DECL|member|stat_width
name|int
name|stat_width
decl_stmt|;
DECL|member|stat_name_width
name|int
name|stat_name_width
decl_stmt|;
DECL|member|stat_graph_width
name|int
name|stat_graph_width
decl_stmt|;
DECL|member|stat_count
name|int
name|stat_count
decl_stmt|;
DECL|member|word_regex
specifier|const
name|char
modifier|*
name|word_regex
decl_stmt|;
DECL|member|word_diff
name|enum
name|diff_words_type
name|word_diff
decl_stmt|;
DECL|member|submodule_format
name|enum
name|diff_submodule_format
name|submodule_format
decl_stmt|;
comment|/* this is set by diffcore for DIFF_FORMAT_PATCH */
DECL|member|found_changes
name|int
name|found_changes
decl_stmt|;
comment|/* to support internal diff recursion by --follow hack*/
DECL|member|found_follow
name|int
name|found_follow
decl_stmt|;
DECL|member|set_default
name|void
function_decl|(
modifier|*
name|set_default
function_decl|)
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|)
function_decl|;
DECL|member|file
name|FILE
modifier|*
name|file
decl_stmt|;
DECL|member|close_file
name|int
name|close_file
decl_stmt|;
DECL|member|pathspec
name|struct
name|pathspec
name|pathspec
decl_stmt|;
DECL|member|pathchange
name|pathchange_fn_t
name|pathchange
decl_stmt|;
DECL|member|change
name|change_fn_t
name|change
decl_stmt|;
DECL|member|add_remove
name|add_remove_fn_t
name|add_remove
decl_stmt|;
DECL|member|format_callback
name|diff_format_fn_t
name|format_callback
decl_stmt|;
DECL|member|format_callback_data
name|void
modifier|*
name|format_callback_data
decl_stmt|;
DECL|member|output_prefix
name|diff_prefix_fn_t
name|output_prefix
decl_stmt|;
DECL|member|output_prefix_data
name|void
modifier|*
name|output_prefix_data
decl_stmt|;
DECL|member|diff_path_counter
name|int
name|diff_path_counter
decl_stmt|;
block|}
struct|;
end_struct

begin_enum
DECL|enum|color_diff
enum|enum
name|color_diff
block|{
DECL|enumerator|DIFF_RESET
name|DIFF_RESET
init|=
literal|0
block|,
DECL|enumerator|DIFF_CONTEXT
name|DIFF_CONTEXT
init|=
literal|1
block|,
DECL|enumerator|DIFF_METAINFO
name|DIFF_METAINFO
init|=
literal|2
block|,
DECL|enumerator|DIFF_FRAGINFO
name|DIFF_FRAGINFO
init|=
literal|3
block|,
DECL|enumerator|DIFF_FILE_OLD
name|DIFF_FILE_OLD
init|=
literal|4
block|,
DECL|enumerator|DIFF_FILE_NEW
name|DIFF_FILE_NEW
init|=
literal|5
block|,
DECL|enumerator|DIFF_COMMIT
name|DIFF_COMMIT
init|=
literal|6
block|,
DECL|enumerator|DIFF_WHITESPACE
name|DIFF_WHITESPACE
init|=
literal|7
block|,
DECL|enumerator|DIFF_FUNCINFO
name|DIFF_FUNCINFO
init|=
literal|8
block|}
enum|;
end_enum

begin_function_decl
specifier|const
name|char
modifier|*
name|diff_get_color
parameter_list|(
name|int
name|diff_use_color
parameter_list|,
name|enum
name|color_diff
name|ix
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|diff_get_color_opt
define|#
directive|define
name|diff_get_color_opt
parameter_list|(
name|o
parameter_list|,
name|ix
parameter_list|)
define|\
value|diff_get_color((o)->use_color, ix)
end_define

begin_function_decl
specifier|const
name|char
modifier|*
name|diff_line_prefix
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
specifier|const
name|char
name|mime_boundary_leader
index|[]
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|extern
name|struct
name|combine_diff_path
modifier|*
name|diff_tree_paths
parameter_list|(
name|struct
name|combine_diff_path
modifier|*
name|p
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
modifier|*
name|parent_sha1
parameter_list|,
name|int
name|nparent
parameter_list|,
name|struct
name|strbuf
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

begin_function_decl
specifier|extern
name|int
name|diff_root_tree_sha1
parameter_list|(
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
DECL|member|oid
name|struct
name|object_id
name|oid
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
DECL|member|oid
name|struct
name|object_id
name|oid
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
value|st_add4(sizeof(struct combine_diff_path), (l), 1, \ 		st_mult(sizeof(struct combine_diff_parent), (n)))
end_define

begin_function_decl
specifier|extern
name|void
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
name|struct
name|rev_info
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_tree_combined
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|struct
name|sha1_array
modifier|*
name|parents
parameter_list|,
name|int
name|dense
parameter_list|,
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_tree_combined_merge
parameter_list|(
specifier|const
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|int
name|dense
parameter_list|,
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|diff_set_mnemonic_prefix
parameter_list|(
name|struct
name|diff_options
modifier|*
name|options
parameter_list|,
specifier|const
name|char
modifier|*
name|a
parameter_list|,
specifier|const
name|char
modifier|*
name|b
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|diff_can_quit_early
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
name|int
name|sha1_valid
parameter_list|,
specifier|const
name|char
modifier|*
name|fullpath
parameter_list|,
name|unsigned
name|dirty_submodule
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
name|int
name|sha1_valid
parameter_list|,
name|int
name|sha2_valid
parameter_list|,
specifier|const
name|char
modifier|*
name|fullpath
parameter_list|,
name|unsigned
name|dirty_submodule1
parameter_list|,
name|unsigned
name|dirty_submodule2
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|struct
name|diff_filepair
modifier|*
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

begin_comment
comment|/*  * Poor man's alternative to parse-option, to allow both stuck form  * (--option=value) and separate form (--option value).  */
end_comment

begin_function_decl
specifier|extern
name|int
name|parse_long_opt
parameter_list|(
specifier|const
name|char
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|optarg
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|git_diff_basic_config
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
specifier|extern
name|int
name|git_diff_heuristic_config
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
specifier|extern
name|void
name|init_diff_ui_defaults
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|git_diff_ui_config
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

begin_define
DECL|macro|DIFF_PICKAXE_REGEX
define|#
directive|define
name|DIFF_PICKAXE_REGEX
value|2
end_define

begin_define
DECL|macro|DIFF_PICKAXE_KIND_S
define|#
directive|define
name|DIFF_PICKAXE_KIND_S
value|4
end_define

begin_comment
DECL|macro|DIFF_PICKAXE_KIND_S
comment|/* traditional plumbing counter */
end_comment

begin_define
DECL|macro|DIFF_PICKAXE_KIND_G
define|#
directive|define
name|DIFF_PICKAXE_KIND_G
value|8
end_define

begin_comment
DECL|macro|DIFF_PICKAXE_KIND_G
comment|/* grep in the patch */
end_comment

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
name|diffcore_fix_diff_index
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
value|"\ncommon diff options:\n" \ "  -z            output diff-raw with lines terminated with NUL.\n" \ "  -p            output patch format.\n" \ "  -u            synonym for -p.\n" \ "  --patch-with-raw\n" \ "                output both a patch and the diff-raw format.\n" \ "  --stat        show diffstat instead of patch.\n" \ "  --numstat     show numeric diffstat instead of patch.\n" \ "  --patch-with-stat\n" \ "                output a patch and prepend its diffstat.\n" \ "  --name-only   show only names of changed files.\n" \ "  --name-status show names and status of changed files.\n" \ "  --full-index  show full object name on index lines.\n" \ "  --abbrev=<n>  abbreviate object names in diff-tree header and diff-raw.\n" \ "  -R            swap input file pairs.\n" \ "  -B            detect complete rewrites.\n" \ "  -M            detect renames.\n" \ "  -C            detect copies.\n" \ "  --find-copies-harder\n" \ "                try unchanged files as candidate for copy detection.\n" \ "  -l<n>         limit rename attempts up to<n> paths.\n" \ "  -O<file>      reorder diffs according to the<file>.\n" \ "  -S<string>    find filepair whose only one side contains the string.\n" \ "  --pickaxe-all\n" \ "                show all files diff when -S is used and hit is found.\n" \ "  -a  --text    treat all files as text.\n"
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

begin_function_decl
specifier|extern
name|void
name|diff_warn_rename_limit
parameter_list|(
specifier|const
name|char
modifier|*
name|varname
parameter_list|,
name|int
name|needed
parameter_list|,
name|int
name|degraded_cc
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

begin_comment
comment|/* do not report anything on removed paths */
end_comment

begin_define
DECL|macro|DIFF_SILENT_ON_REMOVED
define|#
directive|define
name|DIFF_SILENT_ON_REMOVED
value|01
end_define

begin_comment
comment|/* report racily-clean paths as modified */
end_comment

begin_define
DECL|macro|DIFF_RACY_IS_MODIFIED
define|#
directive|define
name|DIFF_RACY_IS_MODIFIED
value|02
end_define

begin_function_decl
specifier|extern
name|int
name|run_diff_files
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|unsigned
name|int
name|option
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|run_diff_index
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|int
name|cached
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|do_diff_cache
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
parameter_list|,
name|struct
name|diff_options
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|diff_flush_patch_id
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|,
name|unsigned
name|char
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|diff_result_code
parameter_list|(
name|struct
name|diff_options
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_no_index
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|,
name|int
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|index_differs_from
parameter_list|(
specifier|const
name|char
modifier|*
name|def
parameter_list|,
name|int
name|diff_flags
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Fill the contents of the filespec "df", respecting any textconv defined by  * its userdiff driver.  The "driver" parameter must come from a  * previous call to get_textconv(), and therefore should either be NULL or have  * textconv enabled.  *  * Note that the memory ownership of the resulting buffer depends on whether  * the driver field is NULL. If it is, then the memory belongs to the filespec  * struct. If it is non-NULL, then "outbuf" points to a newly allocated buffer  * that should be freed by the caller.  */
end_comment

begin_function_decl
specifier|extern
name|size_t
name|fill_textconv
parameter_list|(
name|struct
name|userdiff_driver
modifier|*
name|driver
parameter_list|,
name|struct
name|diff_filespec
modifier|*
name|df
parameter_list|,
name|char
modifier|*
modifier|*
name|outbuf
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Look up the userdiff driver for the given filespec, and return it if  * and only if it has textconv enabled (otherwise return NULL). The result  * can be passed to fill_textconv().  */
end_comment

begin_function_decl
specifier|extern
name|struct
name|userdiff_driver
modifier|*
name|get_textconv
parameter_list|(
name|struct
name|diff_filespec
modifier|*
name|one
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|parse_rename_score
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|cp_p
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|long
name|parse_algorithm_value
parameter_list|(
specifier|const
name|char
modifier|*
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|print_stat_summary
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
name|int
name|files
parameter_list|,
name|int
name|insertions
parameter_list|,
name|int
name|deletions
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|setup_diff_pager
parameter_list|(
name|struct
name|diff_options
modifier|*
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

