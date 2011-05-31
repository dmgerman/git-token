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
name|enum
name|branch_track
name|track
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
comment|/*  * Configure local branch "local" as downstream to branch "remote"  * from remote "origin".  Used by git branch --set-upstream.  */
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
name|void
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

begin_endif
endif|#
directive|endif
end_endif

end_unit

