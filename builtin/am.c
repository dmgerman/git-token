begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git am"  *  * Based on git-am.sh by Junio C Hamano.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_function
DECL|function|cmd_am
name|int
name|cmd_am
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
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
comment|/* 	 * NEEDSWORK: Once all the features of git-am.sh have been 	 * re-implemented in builtin/am.c, this preamble can be removed. 	 */
if|if
condition|(
operator|!
name|getenv
argument_list|(
literal|"_GIT_USE_BUILTIN_AM"
argument_list|)
condition|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|mkpath
argument_list|(
literal|"%s/git-am"
argument_list|,
name|git_exec_path
argument_list|()
argument_list|)
decl_stmt|;
if|if
condition|(
name|sane_execvp
argument_list|(
name|path
argument_list|,
operator|(
name|char
operator|*
operator|*
operator|)
name|argv
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"could not exec %s"
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|prefix
operator|=
name|setup_git_directory
argument_list|()
expr_stmt|;
name|trace_repo_setup
argument_list|(
name|prefix
argument_list|)
expr_stmt|;
name|setup_work_tree
argument_list|()
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

