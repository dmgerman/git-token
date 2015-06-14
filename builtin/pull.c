begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git pull"  *  * Based on git-pull.sh by Junio C Hamano  *  * Fetch one or more remote refs and merge it/them into the current HEAD.  */
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
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_decl_stmt
DECL|variable|pull_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|pull_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git pull [options] [<repository> [<refspec>...]]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pull_options
specifier|static
name|struct
name|option
name|pull_options
index|[]
init|=
block|{
name|OPT_END
argument_list|()
block|}
decl_stmt|;
end_decl_stmt

begin_comment
comment|/**  * Parses argv into [<repo> [<refspecs>...]], returning their values in `repo`  * as a string and `refspecs` as a null-terminated array of strings. If `repo`  * is not provided in argv, it is set to NULL.  */
end_comment

begin_function
DECL|function|parse_repo_refspecs
specifier|static
name|void
name|parse_repo_refspecs
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
modifier|*
name|repo
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
modifier|*
name|refspecs
parameter_list|)
block|{
if|if
condition|(
name|argc
operator|>
literal|0
condition|)
block|{
operator|*
name|repo
operator|=
operator|*
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
block|}
else|else
operator|*
name|repo
operator|=
name|NULL
expr_stmt|;
operator|*
name|refspecs
operator|=
name|argv
expr_stmt|;
block|}
end_function

begin_comment
comment|/**  * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the  * repository and refspecs to fetch, or NULL if they are not provided.  */
end_comment

begin_function
DECL|function|run_fetch
specifier|static
name|int
name|run_fetch
parameter_list|(
specifier|const
name|char
modifier|*
name|repo
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspecs
parameter_list|)
block|{
name|struct
name|argv_array
name|args
init|=
name|ARGV_ARRAY_INIT
decl_stmt|;
name|int
name|ret
decl_stmt|;
name|argv_array_pushl
argument_list|(
operator|&
name|args
argument_list|,
literal|"fetch"
argument_list|,
literal|"--update-head-ok"
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|repo
condition|)
block|{
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|repo
argument_list|)
expr_stmt|;
name|argv_array_pushv
argument_list|(
operator|&
name|args
argument_list|,
name|refspecs
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|*
name|refspecs
condition|)
name|die
argument_list|(
literal|"BUG: refspecs without repo?"
argument_list|)
expr_stmt|;
name|ret
operator|=
name|run_command_v_opt
argument_list|(
name|args
operator|.
name|argv
argument_list|,
name|RUN_GIT_CMD
argument_list|)
expr_stmt|;
name|argv_array_clear
argument_list|(
operator|&
name|args
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_comment
comment|/**  * Runs git-merge, returning its exit status.  */
end_comment

begin_function
DECL|function|run_merge
specifier|static
name|int
name|run_merge
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|ret
decl_stmt|;
name|struct
name|argv_array
name|args
init|=
name|ARGV_ARRAY_INIT
decl_stmt|;
name|argv_array_pushl
argument_list|(
operator|&
name|args
argument_list|,
literal|"merge"
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
literal|"FETCH_HEAD"
argument_list|)
expr_stmt|;
name|ret
operator|=
name|run_command_v_opt
argument_list|(
name|args
operator|.
name|argv
argument_list|,
name|RUN_GIT_CMD
argument_list|)
expr_stmt|;
name|argv_array_clear
argument_list|(
operator|&
name|args
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|cmd_pull
name|int
name|cmd_pull
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
specifier|const
name|char
modifier|*
name|repo
decl_stmt|,
modifier|*
modifier|*
name|refspecs
decl_stmt|;
if|if
condition|(
operator|!
name|getenv
argument_list|(
literal|"_GIT_USE_BUILTIN_PULL"
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
literal|"%s/git-pull"
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
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
name|pull_options
argument_list|,
name|pull_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|parse_repo_refspecs
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
operator|&
name|repo
argument_list|,
operator|&
name|refspecs
argument_list|)
expr_stmt|;
if|if
condition|(
name|run_fetch
argument_list|(
name|repo
argument_list|,
name|refspecs
argument_list|)
condition|)
return|return
literal|1
return|;
return|return
name|run_merge
argument_list|()
return|;
block|}
end_function

end_unit

