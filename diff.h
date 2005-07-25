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

begin_define
DECL|macro|DIFF_FILE_CANON_MODE
define|#
directive|define
name|DIFF_FILE_CANON_MODE
parameter_list|(
name|mode
parameter_list|)
define|\
value|(S_ISREG(mode) ? (S_IFREG | ce_permissions(mode)) : \ 	S_ISLNK(mode) ? S_IFLNK : S_IFDIR)
end_define

begin_function_decl
specifier|extern
name|void
name|diff_addremove
parameter_list|(
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
name|diff_helper_input
parameter_list|(
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
name|path1
parameter_list|,
name|int
name|status
parameter_list|,
name|int
name|score
parameter_list|,
specifier|const
name|char
modifier|*
name|path2
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_unmerge
parameter_list|(
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
name|void
name|diff_setup
parameter_list|(
name|int
name|flags
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
specifier|const
name|char
modifier|*
modifier|*
name|paths
parameter_list|,
name|int
name|detect_rename
parameter_list|,
name|int
name|rename_score
parameter_list|,
specifier|const
name|char
modifier|*
name|pickaxe
parameter_list|,
name|int
name|pickaxe_opts
parameter_list|,
name|int
name|break_opt
parameter_list|,
specifier|const
name|char
modifier|*
name|orderfile
parameter_list|,
specifier|const
name|char
modifier|*
name|filter
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diffcore_std_no_resolve
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|paths
parameter_list|,
specifier|const
name|char
modifier|*
name|pickaxe
parameter_list|,
name|int
name|pickaxe_opts
parameter_list|,
specifier|const
name|char
modifier|*
name|orderfile
parameter_list|,
specifier|const
name|char
modifier|*
name|filter
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|COMMON_DIFF_OPTIONS_HELP
define|#
directive|define
name|COMMON_DIFF_OPTIONS_HELP
define|\
value|"\ncommon diff options:\n" \ "  -r		diff recursively (only meaningful in diff-tree)\n" \ "  -z		output diff-raw with lines terminated with NUL.\n" \ "  -p		output patch format.\n" \ "  -u		synonym for -p.\n" \ "  --name-only	show only names of changed files.\n" \ "  --name-only-z\n" \ "		same as --name-only but terminate lines with NUL.\n" \ "  -R		swap input file pairs.\n" \ "  -B		detect complete rewrites.\n" \ "  -M		detect renames.\n" \ "  -C		detect copies.\n" \ "  --find-copies-harder\n" \ "		try unchanged files as candidate for copy detection.\n" \ "  -O<file>	reorder diffs according to the<file>.\n" \ "  -S<string>	find filepair whose only one side contains the string.\n" \ "  --pickaxe-all\n" \ "		show all files diff when -S is used and hit is found.\n"
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

begin_function_decl
specifier|extern
name|void
name|diff_flush
parameter_list|(
name|int
name|output_style
parameter_list|,
name|int
name|line_terminator
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
value|'N'
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
value|'A'
end_define

begin_define
DECL|macro|DIFF_STATUS_FILTER_BROKEN
define|#
directive|define
name|DIFF_STATUS_FILTER_BROKEN
value|'B'
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* DIFF_H */
end_comment

end_unit

