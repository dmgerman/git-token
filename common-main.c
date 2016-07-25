begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_comment
comment|/*  * Many parts of Git have subprograms communicate via pipe, expect the  * upstream of a pipe to die with SIGPIPE when the downstream of a  * pipe does not need to read all that is written.  Some third-party  * programs that ignore or block SIGPIPE for their own reason forget  * to restore SIGPIPE handling to the default before spawning Git and  * break this carefully orchestrated machinery.  *  * Restore the way SIGPIPE is handled to default, which is what we  * expect.  */
end_comment

begin_function
DECL|function|restore_sigpipe_to_default
specifier|static
name|void
name|restore_sigpipe_to_default
parameter_list|(
name|void
parameter_list|)
block|{
name|sigset_t
name|unblock
decl_stmt|;
name|sigemptyset
argument_list|(
operator|&
name|unblock
argument_list|)
expr_stmt|;
name|sigaddset
argument_list|(
operator|&
name|unblock
argument_list|,
name|SIGPIPE
argument_list|)
expr_stmt|;
name|sigprocmask
argument_list|(
name|SIG_UNBLOCK
argument_list|,
operator|&
name|unblock
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|signal
argument_list|(
name|SIGPIPE
argument_list|,
name|SIG_DFL
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
comment|/* 	 * Always open file descriptors 0/1/2 to avoid clobbering files 	 * in die().  It also avoids messing up when the pipes are dup'ed 	 * onto stdin/stdout/stderr in the child processes we spawn. 	 */
name|sanitize_stdfds
argument_list|()
expr_stmt|;
name|git_setup_gettext
argument_list|()
expr_stmt|;
name|argv
index|[
literal|0
index|]
operator|=
name|git_extract_argv0_path
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|restore_sigpipe_to_default
argument_list|()
expr_stmt|;
return|return
name|cmd_main
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|)
return|;
block|}
end_function

end_unit
