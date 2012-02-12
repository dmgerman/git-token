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

begin_ifndef
ifndef|#
directive|ifndef
name|WIN32
end_ifndef

begin_function
DECL|function|pager_preexec
specifier|static
name|void
name|pager_preexec
parameter_list|(
name|void
parameter_list|)
block|{
comment|/* 	 * Work around bug in "less" by not starting it until we 	 * have real input 	 */
name|fd_set
name|in
decl_stmt|;
name|FD_ZERO
argument_list|(
operator|&
name|in
argument_list|)
expr_stmt|;
name|FD_SET
argument_list|(
literal|0
argument_list|,
operator|&
name|in
argument_list|)
expr_stmt|;
name|select
argument_list|(
literal|1
argument_list|,
operator|&
name|in
argument_list|,
name|NULL
argument_list|,
operator|&
name|in
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

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
elseif|else
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
block|{
specifier|static
specifier|const
name|char
modifier|*
name|env
index|[]
init|=
block|{
literal|"LESS=FRSX"
block|,
name|NULL
block|}
decl_stmt|;
name|pager_process
operator|.
name|env
operator|=
name|env
expr_stmt|;
block|}
ifndef|#
directive|ifndef
name|WIN32
name|pager_process
operator|.
name|preexec_cb
operator|=
name|pager_preexec
expr_stmt|;
endif|#
directive|endif
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
comment|/*  * How many columns do we need to show this number in decimal?  */
end_comment

begin_function
DECL|function|decimal_width
name|int
name|decimal_width
parameter_list|(
name|int
name|number
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|width
decl_stmt|;
for|for
control|(
name|width
operator|=
literal|1
operator|,
name|i
operator|=
literal|10
init|;
name|i
operator|<=
name|number
condition|;
name|width
operator|++
control|)
name|i
operator|*=
literal|10
expr_stmt|;
return|return
name|width
return|;
block|}
end_function

end_unit

