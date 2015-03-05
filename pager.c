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

begin_comment
comment|/*  * This is split up from the rest of git so that we can do  * something different on Windows.  */
end_comment

begin_decl_stmt
DECL|variable|pager_argv
specifier|static
specifier|const
name|char
modifier|*
name|pager_argv
index|[]
init|=
block|{
name|NULL
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

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

begin_function
DECL|function|wait_for_pager
specifier|static
name|void
name|wait_for_pager
parameter_list|(
name|void
parameter_list|)
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
name|finish_command
argument_list|(
operator|&
name|pager_process
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
argument_list|()
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
name|git_config
argument_list|(
name|git_default_config
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
name|pager_argv
index|[
literal|0
index|]
operator|=
name|pager
expr_stmt|;
name|pager_process
operator|.
name|use_shell
operator|=
literal|1
expr_stmt|;
name|pager_process
operator|.
name|argv
operator|=
name|pager_argv
expr_stmt|;
name|pager_process
operator|.
name|in
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|getenv
argument_list|(
literal|"LESS"
argument_list|)
condition|)
name|argv_array_push
argument_list|(
operator|&
name|pager_process
operator|.
name|env_array
argument_list|,
literal|"LESS=FRX"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|getenv
argument_list|(
literal|"LV"
argument_list|)
condition|)
name|argv_array_push
argument_list|(
operator|&
name|pager_process
operator|.
name|env_array
argument_list|,
literal|"LV=-c"
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
name|wait_for_pager
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
name|int
name|want
init|=
operator|-
literal|1
decl_stmt|;
name|struct
name|strbuf
name|key
init|=
name|STRBUF_INIT
decl_stmt|;
specifier|const
name|char
modifier|*
name|value
init|=
name|NULL
decl_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|key
argument_list|,
literal|"pager.%s"
argument_list|,
name|cmd
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|git_config_get_value
argument_list|(
name|key
operator|.
name|buf
argument_list|,
operator|&
name|value
argument_list|)
condition|)
block|{
name|int
name|b
init|=
name|git_config_maybe_bool
argument_list|(
name|key
operator|.
name|buf
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
name|want
operator|=
name|b
expr_stmt|;
else|else
block|{
name|want
operator|=
literal|1
expr_stmt|;
name|pager_program
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
block|}
name|strbuf_release
argument_list|(
operator|&
name|key
argument_list|)
expr_stmt|;
return|return
name|want
return|;
block|}
end_function

end_unit

