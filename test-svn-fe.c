begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * test-svn-fe: Code to exercise the svn import lib  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"vcs-svn/svndump.h"
end_include

begin_include
include|#
directive|include
file|"vcs-svn/svndiff.h"
end_include

begin_include
include|#
directive|include
file|"vcs-svn/sliding_window.h"
end_include

begin_include
include|#
directive|include
file|"vcs-svn/line_buffer.h"
end_include

begin_decl_stmt
DECL|variable|test_svnfe_usage
specifier|static
specifier|const
name|char
name|test_svnfe_usage
index|[]
init|=
literal|"test-svn-fe (<dumpfile> | [-d]<preimage><delta><len>)"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|apply_delta
specifier|static
name|int
name|apply_delta
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
name|argv
index|[]
parameter_list|)
block|{
name|struct
name|line_buffer
name|preimage
init|=
name|LINE_BUFFER_INIT
decl_stmt|;
name|struct
name|line_buffer
name|delta
init|=
name|LINE_BUFFER_INIT
decl_stmt|;
name|struct
name|sliding_view
name|preimage_view
init|=
name|SLIDING_VIEW_INIT
argument_list|(
operator|&
name|preimage
argument_list|,
operator|-
literal|1
argument_list|)
decl_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|5
condition|)
name|usage
argument_list|(
name|test_svnfe_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|buffer_init
argument_list|(
operator|&
name|preimage
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"cannot open preimage"
argument_list|)
expr_stmt|;
if|if
condition|(
name|buffer_init
argument_list|(
operator|&
name|delta
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"cannot open delta"
argument_list|)
expr_stmt|;
if|if
condition|(
name|svndiff0_apply
argument_list|(
operator|&
name|delta
argument_list|,
operator|(
name|off_t
operator|)
name|strtoull
argument_list|(
name|argv
index|[
literal|4
index|]
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
argument_list|,
operator|&
name|preimage_view
argument_list|,
name|stdout
argument_list|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|buffer_deinit
argument_list|(
operator|&
name|preimage
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"cannot close preimage"
argument_list|)
expr_stmt|;
if|if
condition|(
name|buffer_deinit
argument_list|(
operator|&
name|delta
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"cannot close delta"
argument_list|)
expr_stmt|;
name|buffer_reset
argument_list|(
operator|&
name|preimage
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|preimage_view
operator|.
name|buf
argument_list|)
expr_stmt|;
name|buffer_reset
argument_list|(
operator|&
name|delta
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
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
name|char
modifier|*
name|argv
index|[]
parameter_list|)
block|{
if|if
condition|(
name|argc
operator|==
literal|2
condition|)
block|{
if|if
condition|(
name|svndump_init
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
condition|)
return|return
literal|1
return|;
name|svndump_read
argument_list|(
name|NULL
argument_list|)
expr_stmt|;
name|svndump_deinit
argument_list|()
expr_stmt|;
name|svndump_reset
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|argc
operator|>=
literal|2
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-d"
argument_list|)
condition|)
return|return
name|apply_delta
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|)
return|;
name|usage
argument_list|(
name|test_svnfe_usage
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

