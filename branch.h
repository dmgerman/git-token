begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|BRANCH_H
end_ifndef

begin_define
DECL|macro|BRANCH_H
define|#
directive|define
name|BRANCH_H
end_define

begin_comment
comment|/* Functions for acting on the information about branches. */
end_comment

begin_comment
comment|/*  * Creates a new branch, where head is the branch currently checked  * out, name is the new branch name, start_name is the name of the  * existing branch that the new branch should start from, force  * enables overwriting an existing (non-head) branch, reflog creates a  * reflog for the branch, and track causes the new branch to be  * configured to merge the remote branch that start_name is a tracking  * branch for (if any).  */
end_comment

begin_function_decl
name|void
name|create_branch
parameter_list|(
specifier|const
name|char
modifier|*
name|head
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|start_name
parameter_list|,
name|int
name|force
parameter_list|,
name|int
name|reflog
parameter_list|,
name|int
name|clobber_head
parameter_list|,
name|int
name|quiet
parameter_list|,
name|enum
name|branch_track
name|track
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Validates that the requested branch may be created, returning the  * interpreted ref in ref, force indicates whether (non-head) branches  * may be overwritten. A non-zero return value indicates that the force  * parameter was non-zero and the branch already exists.  *  * Contrary to all of the above, when attr_only is 1, the caller is  * not interested in verifying if it is Ok to update the named  * branch to point at a potentially different commit. It is merely  * asking if it is OK to change some attribute for the named branch  * (e.g. tracking upstream).  *  * NEEDSWORK: This needs to be split into two separate functions in the  * longer run for sanity.  *  */
end_comment

begin_function_decl
name|int
name|validate_new_branchname
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|struct
name|strbuf
modifier|*
name|ref
parameter_list|,
name|int
name|force
parameter_list|,
name|int
name|attr_only
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Remove information about the state of working on the current  * branch. (E.g., MERGE_HEAD)  */
end_comment

begin_function_decl
name|void
name|remove_branch_state
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Configure local branch "local" as downstream to branch "remote"  * from remote "origin".  Used by git branch --set-upstream.  * Returns 0 on success.  */
end_comment

begin_define
DECL|macro|BRANCH_CONFIG_VERBOSE
define|#
directive|define
name|BRANCH_CONFIG_VERBOSE
value|01
end_define

begin_function_decl
specifier|extern
name|int
name|install_branch_config
parameter_list|(
name|int
name|flag
parameter_list|,
specifier|const
name|char
modifier|*
name|local
parameter_list|,
specifier|const
name|char
modifier|*
name|origin
parameter_list|,
specifier|const
name|char
modifier|*
name|remote
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Read branch description  */
end_comment

begin_function_decl
specifier|extern
name|int
name|read_branch_desc
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|branch_name
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Check if a branch is checked out in the main worktree or any linked  * worktree and die (with a message describing its checkout location) if  * it is.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|die_if_checked_out
parameter_list|(
specifier|const
name|char
modifier|*
name|branch
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Update all per-worktree HEADs pointing at the old ref to point the new ref.  * This will be used when renaming a branch. Returns 0 if successful, non-zero  * otherwise.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|replace_each_worktree_head_symref
parameter_list|(
specifier|const
name|char
modifier|*
name|oldref
parameter_list|,
specifier|const
name|char
modifier|*
name|newref
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

