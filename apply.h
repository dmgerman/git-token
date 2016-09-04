begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|APPLY_H
end_ifndef

begin_define
DECL|macro|APPLY_H
define|#
directive|define
name|APPLY_H
end_define

begin_enum
DECL|enum|apply_ws_error_action
enum|enum
name|apply_ws_error_action
block|{
DECL|enumerator|nowarn_ws_error
name|nowarn_ws_error
block|,
DECL|enumerator|warn_on_ws_error
name|warn_on_ws_error
block|,
DECL|enumerator|die_on_ws_error
name|die_on_ws_error
block|,
DECL|enumerator|correct_ws_error
name|correct_ws_error
block|}
enum|;
end_enum

begin_enum
DECL|enum|apply_ws_ignore
enum|enum
name|apply_ws_ignore
block|{
DECL|enumerator|ignore_ws_none
name|ignore_ws_none
block|,
DECL|enumerator|ignore_ws_change
name|ignore_ws_change
block|}
enum|;
end_enum

begin_comment
comment|/*  * We need to keep track of how symlinks in the preimage are  * manipulated by the patches.  A patch to add a/b/c where a/b  * is a symlink should not be allowed to affect the directory  * the symlink points at, but if the same patch removes a/b,  * it is perfectly fine, as the patch removes a/b to make room  * to create a directory a/b so that a/b/c can be created.  *  * See also "struct string_list symlink_changes" in "struct  * apply_state".  */
end_comment

begin_define
DECL|macro|APPLY_SYMLINK_GOES_AWAY
define|#
directive|define
name|APPLY_SYMLINK_GOES_AWAY
value|01
end_define

begin_define
DECL|macro|APPLY_SYMLINK_IN_RESULT
define|#
directive|define
name|APPLY_SYMLINK_IN_RESULT
value|02
end_define

begin_struct
DECL|struct|apply_state
struct|struct
name|apply_state
block|{
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
comment|/* These are lock_file related */
DECL|member|lock_file
name|struct
name|lock_file
modifier|*
name|lock_file
decl_stmt|;
DECL|member|newfd
name|int
name|newfd
decl_stmt|;
comment|/* These control what gets looked at and modified */
DECL|member|apply
name|int
name|apply
decl_stmt|;
comment|/* this is not a dry-run */
DECL|member|cached
name|int
name|cached
decl_stmt|;
comment|/* apply to the index only */
DECL|member|check
name|int
name|check
decl_stmt|;
comment|/* preimage must match working tree, don't actually apply */
DECL|member|check_index
name|int
name|check_index
decl_stmt|;
comment|/* preimage must match the indexed version */
DECL|member|update_index
name|int
name|update_index
decl_stmt|;
comment|/* check_index&& apply */
comment|/* These control cosmetic aspect of the output */
DECL|member|diffstat
name|int
name|diffstat
decl_stmt|;
comment|/* just show a diffstat, and don't actually apply */
DECL|member|numstat
name|int
name|numstat
decl_stmt|;
comment|/* just show a numeric diffstat, and don't actually apply */
DECL|member|summary
name|int
name|summary
decl_stmt|;
comment|/* just report creation, deletion, etc, and don't actually apply */
comment|/* These boolean parameters control how the apply is done */
DECL|member|allow_overlap
name|int
name|allow_overlap
decl_stmt|;
DECL|member|apply_in_reverse
name|int
name|apply_in_reverse
decl_stmt|;
DECL|member|apply_with_reject
name|int
name|apply_with_reject
decl_stmt|;
DECL|member|apply_verbosely
name|int
name|apply_verbosely
decl_stmt|;
DECL|member|no_add
name|int
name|no_add
decl_stmt|;
DECL|member|threeway
name|int
name|threeway
decl_stmt|;
DECL|member|unidiff_zero
name|int
name|unidiff_zero
decl_stmt|;
DECL|member|unsafe_paths
name|int
name|unsafe_paths
decl_stmt|;
comment|/* Other non boolean parameters */
DECL|member|fake_ancestor
specifier|const
name|char
modifier|*
name|fake_ancestor
decl_stmt|;
DECL|member|patch_input_file
specifier|const
name|char
modifier|*
name|patch_input_file
decl_stmt|;
DECL|member|line_termination
name|int
name|line_termination
decl_stmt|;
DECL|member|root
name|struct
name|strbuf
name|root
decl_stmt|;
DECL|member|p_value
name|int
name|p_value
decl_stmt|;
DECL|member|p_value_known
name|int
name|p_value_known
decl_stmt|;
DECL|member|p_context
name|unsigned
name|int
name|p_context
decl_stmt|;
comment|/* Exclude and include path parameters */
DECL|member|limit_by_name
name|struct
name|string_list
name|limit_by_name
decl_stmt|;
DECL|member|has_include
name|int
name|has_include
decl_stmt|;
comment|/* Various "current state" */
DECL|member|linenr
name|int
name|linenr
decl_stmt|;
comment|/* current line number */
DECL|member|symlink_changes
name|struct
name|string_list
name|symlink_changes
decl_stmt|;
comment|/* we have to track symlinks */
comment|/* 	 * For "diff-stat" like behaviour, we keep track of the biggest change 	 * we've seen, and the longest filename. That allows us to do simple 	 * scaling. 	 */
DECL|member|max_change
name|int
name|max_change
decl_stmt|;
DECL|member|max_len
name|int
name|max_len
decl_stmt|;
comment|/* 	 * Records filenames that have been touched, in order to handle 	 * the case where more than one patches touch the same file. 	 */
DECL|member|fn_table
name|struct
name|string_list
name|fn_table
decl_stmt|;
comment|/* These control whitespace errors */
DECL|member|ws_error_action
name|enum
name|apply_ws_error_action
name|ws_error_action
decl_stmt|;
DECL|member|ws_ignore_action
name|enum
name|apply_ws_ignore
name|ws_ignore_action
decl_stmt|;
DECL|member|whitespace_option
specifier|const
name|char
modifier|*
name|whitespace_option
decl_stmt|;
DECL|member|whitespace_error
name|int
name|whitespace_error
decl_stmt|;
DECL|member|squelch_whitespace_errors
name|int
name|squelch_whitespace_errors
decl_stmt|;
DECL|member|applied_after_fixing_ws
name|int
name|applied_after_fixing_ws
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|int
name|parse_whitespace_option
parameter_list|(
name|struct
name|apply_state
modifier|*
name|state
parameter_list|,
specifier|const
name|char
modifier|*
name|option
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|parse_ignorewhitespace_option
parameter_list|(
name|struct
name|apply_state
modifier|*
name|state
parameter_list|,
specifier|const
name|char
modifier|*
name|option
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|init_apply_state
parameter_list|(
name|struct
name|apply_state
modifier|*
name|state
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|struct
name|lock_file
modifier|*
name|lock_file
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|clear_apply_state
parameter_list|(
name|struct
name|apply_state
modifier|*
name|state
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|check_apply_state
parameter_list|(
name|struct
name|apply_state
modifier|*
name|state
parameter_list|,
name|int
name|force_apply
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Some aspects of the apply behavior are controlled by the following  * bits in the "options" parameter passed to apply_all_patches().  */
end_comment

begin_define
DECL|macro|APPLY_OPT_INACCURATE_EOF
define|#
directive|define
name|APPLY_OPT_INACCURATE_EOF
value|(1<<0)
end_define

begin_comment
DECL|macro|APPLY_OPT_INACCURATE_EOF
comment|/* accept inaccurate eof */
end_comment

begin_define
DECL|macro|APPLY_OPT_RECOUNT
define|#
directive|define
name|APPLY_OPT_RECOUNT
value|(1<<1)
end_define

begin_comment
DECL|macro|APPLY_OPT_RECOUNT
comment|/* accept inaccurate line count */
end_comment

begin_endif
endif|#
directive|endif
end_endif

end_unit

