begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"sigchain.h"
end_include

begin_ifndef
ifndef|#
directive|ifndef
name|DEFAULT_PAGER
end_ifndef

begin_define
DECL|macro|DEFAULT_PAGER
define|#
directive|define
name|DEFAULT_PAGER
value|"less"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|pager_process
specifier|static
name|struct
name|child_process
name|pager_process
init|=
name|CHILD_PROCESS_INIT
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pager_program
specifier|static
specifier|const
name|char
modifier|*
name|pager_program
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|wait_for_pager
specifier|static
name|void
name|wait_for_pager
parameter_list|(
name|int
name|in_signal
parameter_list|)
block|{
if|if
condition|(
operator|!
name|in_signal
condition|)
block|{
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stderr
argument_list|)
expr_stmt|;
block|}
comment|/* signal EOF to pager */
name|close
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|close
argument_list|(
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
name|in_signal
condition|)
name|finish_command_in_signal
argument_list|(
operator|&
name|pager_process
argument_list|)
expr_stmt|;
else|else
name|finish_command
argument_list|(
operator|&
name|pager_process
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wait_for_pager_atexit
specifier|static
name|void
name|wait_for_pager_atexit
parameter_list|(
name|void
parameter_list|)
block|{
name|wait_for_pager
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wait_for_pager_signal
specifier|static
name|void
name|wait_for_pager_signal
parameter_list|(
name|int
name|signo
parameter_list|)
block|{
name|wait_for_pager
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|sigchain_pop
argument_list|(
name|signo
argument_list|)
expr_stmt|;
name|raise
argument_list|(
name|signo
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|core_pager_config
specifier|static
name|int
name|core_pager_config
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
name|data
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"core.pager"
argument_list|)
condition|)
return|return
name|git_config_string
argument_list|(
operator|&
name|pager_program
argument_list|,
name|var
argument_list|,
name|value
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|read_early_config
specifier|static
name|void
name|read_early_config
parameter_list|(
name|config_fn_t
name|cb
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|git_config_with_options
argument_list|(
name|cb
argument_list|,
name|data
argument_list|,
name|NULL
argument_list|,
literal|1
argument_list|)
expr_stmt|;
comment|/* 	 * Note that this is a really dirty hack that does the wrong thing in 	 * many cases. The crux of the problem is that we cannot run 	 * setup_git_directory() early on in git's setup, so we have no idea if 	 * we are in a repository or not, and therefore are not sure whether 	 * and how to read repository-local config. 	 * 	 * So if we _aren't_ in a repository (or we are but we would reject its 	 * core.repositoryformatversion), we'll read whatever is in .git/config 	 * blindly. Similarly, if we _are_ in a repository, but not at the 	 * root, we'll fail to find .git/config (because it's really 	 * ../.git/config, etc). See t7006 for a complete set of failures. 	 * 	 * However, we have historically provided this hack because it does 	 * work some of the time (namely when you are at the top-level of a 	 * valid repository), and would rarely make things worse (i.e., you do 	 * not generally have a .git/config file sitting around). 	 */
if|if
condition|(
operator|!
name|startup_info
operator|->
name|have_repository
condition|)
block|{
name|struct
name|git_config_source
name|repo_config
decl_stmt|;
name|memset
argument_list|(
operator|&
name|repo_config
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|repo_config
argument_list|)
argument_list|)
expr_stmt|;
name|repo_config
operator|.
name|file
operator|=
literal|".git/config"
expr_stmt|;
name|git_config_with_options
argument_list|(
name|cb
argument_list|,
name|data
argument_list|,
operator|&
name|repo_config
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|git_pager
specifier|const
name|char
modifier|*
name|git_pager
parameter_list|(
name|int
name|stdout_is_tty
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|pager
decl_stmt|;
if|if
condition|(
operator|!
name|stdout_is_tty
condition|)
return|return
name|NULL
return|;
name|pager
operator|=
name|getenv
argument_list|(
literal|"GIT_PAGER"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pager
condition|)
block|{
if|if
condition|(
operator|!
name|pager_program
condition|)
name|read_early_config
argument_list|(
name|core_pager_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|pager
operator|=
name|pager_program
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|pager
condition|)
name|pager
operator|=
name|getenv
argument_list|(
literal|"PAGER"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pager
condition|)
name|pager
operator|=
name|DEFAULT_PAGER
expr_stmt|;
if|if
condition|(
operator|!
operator|*
name|pager
operator|||
operator|!
name|strcmp
argument_list|(
name|pager
argument_list|,
literal|"cat"
argument_list|)
condition|)
name|pager
operator|=
name|NULL
expr_stmt|;
return|return
name|pager
return|;
block|}
end_function

begin_function
DECL|function|setup_pager_env
specifier|static
name|void
name|setup_pager_env
parameter_list|(
name|struct
name|argv_array
modifier|*
name|env
parameter_list|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
name|int
name|i
decl_stmt|;
name|char
modifier|*
name|pager_env
init|=
name|xstrdup
argument_list|(
name|PAGER_ENV
argument_list|)
decl_stmt|;
name|int
name|n
init|=
name|split_cmdline
argument_list|(
name|pager_env
argument_list|,
operator|&
name|argv
argument_list|)
decl_stmt|;
if|if
condition|(
name|n
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"malformed build-time PAGER_ENV: %s"
argument_list|,
name|split_cmdline_strerror
argument_list|(
name|n
argument_list|)
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|n
condition|;
name|i
operator|++
control|)
block|{
name|char
modifier|*
name|cp
init|=
name|strchr
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|'='
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|cp
condition|)
name|die
argument_list|(
literal|"malformed build-time PAGER_ENV"
argument_list|)
expr_stmt|;
operator|*
name|cp
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
operator|!
name|getenv
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|)
condition|)
block|{
operator|*
name|cp
operator|=
literal|'='
expr_stmt|;
name|argv_array_push
argument_list|(
name|env
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
block|}
name|free
argument_list|(
name|pager_env
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|argv
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|prepare_pager_args
name|void
name|prepare_pager_args
parameter_list|(
name|struct
name|child_process
modifier|*
name|pager_process
parameter_list|,
specifier|const
name|char
modifier|*
name|pager
parameter_list|)
block|{
name|argv_array_push
argument_list|(
operator|&
name|pager_process
operator|->
name|args
argument_list|,
name|pager
argument_list|)
expr_stmt|;
name|pager_process
operator|->
name|use_shell
operator|=
literal|1
expr_stmt|;
name|setup_pager_env
argument_list|(
operator|&
name|pager_process
operator|->
name|env_array
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|setup_pager
name|void
name|setup_pager
parameter_list|(
name|void
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|pager
init|=
name|git_pager
argument_list|(
name|isatty
argument_list|(
literal|1
argument_list|)
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|pager
condition|)
return|return;
comment|/* 	 * force computing the width of the terminal before we redirect 	 * the standard output to the pager. 	 */
operator|(
name|void
operator|)
name|term_columns
argument_list|()
expr_stmt|;
name|setenv
argument_list|(
literal|"GIT_PAGER_IN_USE"
argument_list|,
literal|"true"
argument_list|,
literal|1
argument_list|)
expr_stmt|;
comment|/* spawn the pager */
name|prepare_pager_args
argument_list|(
operator|&
name|pager_process
argument_list|,
name|pager
argument_list|)
expr_stmt|;
name|pager_process
operator|.
name|in
operator|=
operator|-
literal|1
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|pager_process
operator|.
name|env_array
argument_list|,
literal|"GIT_PAGER_IN_USE"
argument_list|)
expr_stmt|;
if|if
condition|(
name|start_command
argument_list|(
operator|&
name|pager_process
argument_list|)
condition|)
return|return;
comment|/* original process continues, but writes to the pipe */
name|dup2
argument_list|(
name|pager_process
operator|.
name|in
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|isatty
argument_list|(
literal|2
argument_list|)
condition|)
name|dup2
argument_list|(
name|pager_process
operator|.
name|in
argument_list|,
literal|2
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|pager_process
operator|.
name|in
argument_list|)
expr_stmt|;
comment|/* this makes sure that the parent terminates after the pager */
name|sigchain_push_common
argument_list|(
name|wait_for_pager_signal
argument_list|)
expr_stmt|;
name|atexit
argument_list|(
name|wait_for_pager_atexit
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|pager_in_use
name|int
name|pager_in_use
parameter_list|(
name|void
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|env
decl_stmt|;
name|env
operator|=
name|getenv
argument_list|(
literal|"GIT_PAGER_IN_USE"
argument_list|)
expr_stmt|;
return|return
name|env
condition|?
name|git_config_bool
argument_list|(
literal|"GIT_PAGER_IN_USE"
argument_list|,
name|env
argument_list|)
else|:
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * Return cached value (if set) or $COLUMNS environment variable (if  * set and positive) or ioctl(1, TIOCGWINSZ).ws_col (if positive),  * and default to 80 if all else fails.  */
end_comment

begin_function
DECL|function|term_columns
name|int
name|term_columns
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|int
name|term_columns_at_startup
decl_stmt|;
name|char
modifier|*
name|col_string
decl_stmt|;
name|int
name|n_cols
decl_stmt|;
if|if
condition|(
name|term_columns_at_startup
condition|)
return|return
name|term_columns_at_startup
return|;
name|term_columns_at_startup
operator|=
literal|80
expr_stmt|;
name|col_string
operator|=
name|getenv
argument_list|(
literal|"COLUMNS"
argument_list|)
expr_stmt|;
if|if
condition|(
name|col_string
operator|&&
operator|(
name|n_cols
operator|=
name|atoi
argument_list|(
name|col_string
argument_list|)
operator|)
operator|>
literal|0
condition|)
name|term_columns_at_startup
operator|=
name|n_cols
expr_stmt|;
ifdef|#
directive|ifdef
name|TIOCGWINSZ
else|else
block|{
name|struct
name|winsize
name|ws
decl_stmt|;
if|if
condition|(
operator|!
name|ioctl
argument_list|(
literal|1
argument_list|,
name|TIOCGWINSZ
argument_list|,
operator|&
name|ws
argument_list|)
operator|&&
name|ws
operator|.
name|ws_col
condition|)
name|term_columns_at_startup
operator|=
name|ws
operator|.
name|ws_col
expr_stmt|;
block|}
endif|#
directive|endif
return|return
name|term_columns_at_startup
return|;
block|}
end_function

begin_comment
comment|/*  * How many columns do we need to show this number in decimal?  */
end_comment

begin_function
DECL|function|decimal_width
name|int
name|decimal_width
parameter_list|(
name|uintmax_t
name|number
parameter_list|)
block|{
name|int
name|width
decl_stmt|;
for|for
control|(
name|width
operator|=
literal|1
init|;
name|number
operator|>=
literal|10
condition|;
name|width
operator|++
control|)
name|number
operator|/=
literal|10
expr_stmt|;
return|return
name|width
return|;
block|}
end_function

begin_struct
DECL|struct|pager_command_config_data
struct|struct
name|pager_command_config_data
block|{
DECL|member|cmd
specifier|const
name|char
modifier|*
name|cmd
decl_stmt|;
DECL|member|want
name|int
name|want
decl_stmt|;
DECL|member|value
name|char
modifier|*
name|value
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|pager_command_config
specifier|static
name|int
name|pager_command_config
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
name|vdata
parameter_list|)
block|{
name|struct
name|pager_command_config_data
modifier|*
name|data
init|=
name|vdata
decl_stmt|;
specifier|const
name|char
modifier|*
name|cmd
decl_stmt|;
if|if
condition|(
name|skip_prefix
argument_list|(
name|var
argument_list|,
literal|"pager."
argument_list|,
operator|&
name|cmd
argument_list|)
operator|&&
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
name|data
operator|->
name|cmd
argument_list|)
condition|)
block|{
name|int
name|b
init|=
name|git_config_maybe_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
decl_stmt|;
if|if
condition|(
name|b
operator|>=
literal|0
condition|)
name|data
operator|->
name|want
operator|=
name|b
expr_stmt|;
else|else
block|{
name|data
operator|->
name|want
operator|=
literal|1
expr_stmt|;
name|data
operator|->
name|value
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/* returns 0 for "no pager", 1 for "use pager", and -1 for "not specified" */
end_comment

begin_function
DECL|function|check_pager_config
name|int
name|check_pager_config
parameter_list|(
specifier|const
name|char
modifier|*
name|cmd
parameter_list|)
block|{
name|struct
name|pager_command_config_data
name|data
decl_stmt|;
name|data
operator|.
name|cmd
operator|=
name|cmd
expr_stmt|;
name|data
operator|.
name|want
operator|=
operator|-
literal|1
expr_stmt|;
name|data
operator|.
name|value
operator|=
name|NULL
expr_stmt|;
name|read_early_config
argument_list|(
name|pager_command_config
argument_list|,
operator|&
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|data
operator|.
name|value
condition|)
name|pager_program
operator|=
name|data
operator|.
name|value
expr_stmt|;
return|return
name|data
operator|.
name|want
return|;
block|}
end_function

end_unit

