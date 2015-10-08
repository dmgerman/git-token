begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|WORKTREE_H
end_ifndef

begin_define
DECL|macro|WORKTREE_H
define|#
directive|define
name|WORKTREE_H
end_define

begin_struct
DECL|struct|worktree
struct|struct
name|worktree
block|{
DECL|member|path
name|char
modifier|*
name|path
decl_stmt|;
DECL|member|git_dir
name|char
modifier|*
name|git_dir
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* Functions for acting on the information about worktrees. */
end_comment

begin_comment
comment|/*  * Get the worktrees.  The primary worktree will always be the first returned,  * and linked worktrees will be pointed to by 'next' in each subsequent  * worktree.  No specific ordering is done on the linked worktrees.  *  * The caller is responsible for freeing the memory from the returned  * worktree(s).  */
end_comment

begin_function_decl
specifier|extern
name|struct
name|worktree
modifier|*
modifier|*
name|get_worktrees
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Free up the memory for worktree(s)  */
end_comment

begin_function_decl
specifier|extern
name|void
name|free_worktrees
parameter_list|(
name|struct
name|worktree
modifier|*
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Check if a per-worktree symref points to a ref in the main worktree  * or any linked worktree, and return the path to the exising worktree  * if it is.  Returns NULL if there is no existing ref.  The caller is  * responsible for freeing the returned path.  */
end_comment

begin_function_decl
specifier|extern
name|char
modifier|*
name|find_shared_symref
parameter_list|(
specifier|const
name|char
modifier|*
name|symref
parameter_list|,
specifier|const
name|char
modifier|*
name|target
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

