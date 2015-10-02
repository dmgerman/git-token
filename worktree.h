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

