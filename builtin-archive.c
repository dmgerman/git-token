begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2006 Franck Bui-Huu  * Copyright (c) 2006 Rene Scharfe  */
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
file|"archive.h"
end_include

begin_include
include|#
directive|include
file|"pkt-line.h"
end_include

begin_include
include|#
directive|include
file|"sideband.h"
end_include

begin_function
DECL|function|run_remote_archiver
specifier|static
name|int
name|run_remote_archiver
parameter_list|(
specifier|const
name|char
modifier|*
name|remote
parameter_list|,
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
name|char
modifier|*
name|url
decl_stmt|,
name|buf
index|[
name|LARGE_PACKET_MAX
index|]
decl_stmt|;
name|int
name|fd
index|[
literal|2
index|]
decl_stmt|,
name|i
decl_stmt|,
name|len
decl_stmt|,
name|rv
decl_stmt|;
name|struct
name|child_process
modifier|*
name|conn
decl_stmt|;
specifier|const
name|char
modifier|*
name|exec
init|=
literal|"git-upload-archive"
decl_stmt|;
name|int
name|exec_at
init|=
literal|0
decl_stmt|,
name|exec_value_at
init|=
literal|0
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--exec="
argument_list|)
condition|)
block|{
if|if
condition|(
name|exec_at
condition|)
name|die
argument_list|(
literal|"multiple --exec specified"
argument_list|)
expr_stmt|;
name|exec
operator|=
name|arg
operator|+
literal|7
expr_stmt|;
name|exec_at
operator|=
name|i
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--exec"
argument_list|)
condition|)
block|{
if|if
condition|(
name|exec_at
condition|)
name|die
argument_list|(
literal|"multiple --exec specified"
argument_list|)
expr_stmt|;
if|if
condition|(
name|i
operator|+
literal|1
operator|>=
name|argc
condition|)
name|die
argument_list|(
literal|"option --exec requires a value"
argument_list|)
expr_stmt|;
name|exec
operator|=
name|argv
index|[
name|i
operator|+
literal|1
index|]
expr_stmt|;
name|exec_at
operator|=
name|i
expr_stmt|;
name|exec_value_at
operator|=
operator|++
name|i
expr_stmt|;
block|}
block|}
name|url
operator|=
name|xstrdup
argument_list|(
name|remote
argument_list|)
expr_stmt|;
name|conn
operator|=
name|git_connect
argument_list|(
name|fd
argument_list|,
name|url
argument_list|,
name|exec
argument_list|,
literal|0
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|i
operator|==
name|exec_at
operator|||
name|i
operator|==
name|exec_value_at
condition|)
continue|continue;
name|packet_write
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|,
literal|"argument %s\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
name|packet_flush
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|len
operator|=
name|packet_read_line
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|len
condition|)
name|die
argument_list|(
literal|"git archive: expected ACK/NAK, got EOF"
argument_list|)
expr_stmt|;
if|if
condition|(
name|buf
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|buf
index|[
operator|--
name|len
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|buf
argument_list|,
literal|"ACK"
argument_list|)
condition|)
block|{
if|if
condition|(
name|len
operator|>
literal|5
operator|&&
operator|!
name|prefixcmp
argument_list|(
name|buf
argument_list|,
literal|"NACK "
argument_list|)
condition|)
name|die
argument_list|(
literal|"git archive: NACK %s"
argument_list|,
name|buf
operator|+
literal|5
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"git archive: protocol error"
argument_list|)
expr_stmt|;
block|}
name|len
operator|=
name|packet_read_line
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
condition|)
name|die
argument_list|(
literal|"git archive: expected a flush"
argument_list|)
expr_stmt|;
comment|/* Now, start reading from fd[0] and spit it out to stdout */
name|rv
operator|=
name|recv_sideband
argument_list|(
literal|"archive"
argument_list|,
name|fd
index|[
literal|0
index|]
argument_list|,
literal|1
argument_list|,
literal|2
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|rv
operator||=
name|finish_connect
argument_list|(
name|conn
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|rv
return|;
block|}
end_function

begin_function
DECL|function|extract_remote_arg
specifier|static
specifier|const
name|char
modifier|*
name|extract_remote_arg
parameter_list|(
name|int
modifier|*
name|ac
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|av
parameter_list|)
block|{
name|int
name|ix
decl_stmt|,
name|iy
decl_stmt|,
name|cnt
init|=
operator|*
name|ac
decl_stmt|;
name|int
name|no_more_options
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|remote
init|=
name|NULL
decl_stmt|;
for|for
control|(
name|ix
operator|=
name|iy
operator|=
literal|1
init|;
name|ix
operator|<
name|cnt
condition|;
name|ix
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|av
index|[
name|ix
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--"
argument_list|)
condition|)
name|no_more_options
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|no_more_options
condition|)
block|{
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--remote="
argument_list|)
condition|)
block|{
if|if
condition|(
name|remote
condition|)
name|die
argument_list|(
literal|"Multiple --remote specified"
argument_list|)
expr_stmt|;
name|remote
operator|=
name|arg
operator|+
literal|9
expr_stmt|;
continue|continue;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--remote"
argument_list|)
condition|)
block|{
if|if
condition|(
name|remote
condition|)
name|die
argument_list|(
literal|"Multiple --remote specified"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|++
name|ix
operator|>=
name|cnt
condition|)
name|die
argument_list|(
literal|"option --remote requires a value"
argument_list|)
expr_stmt|;
name|remote
operator|=
name|av
index|[
name|ix
index|]
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|arg
index|[
literal|0
index|]
operator|!=
literal|'-'
condition|)
name|no_more_options
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|ix
operator|!=
name|iy
condition|)
name|av
index|[
name|iy
index|]
operator|=
name|arg
expr_stmt|;
name|iy
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|remote
condition|)
block|{
name|av
index|[
operator|--
name|cnt
index|]
operator|=
name|NULL
expr_stmt|;
operator|*
name|ac
operator|=
name|cnt
expr_stmt|;
block|}
return|return
name|remote
return|;
block|}
end_function

begin_function
DECL|function|cmd_archive
name|int
name|cmd_archive
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
name|remote
init|=
name|NULL
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|remote
operator|=
name|extract_remote_arg
argument_list|(
operator|&
name|argc
argument_list|,
name|argv
argument_list|)
expr_stmt|;
if|if
condition|(
name|remote
condition|)
return|return
name|run_remote_archiver
argument_list|(
name|remote
argument_list|,
name|argc
argument_list|,
name|argv
argument_list|)
return|;
name|setvbuf
argument_list|(
name|stderr
argument_list|,
name|NULL
argument_list|,
name|_IOLBF
argument_list|,
name|BUFSIZ
argument_list|)
expr_stmt|;
return|return
name|write_archive
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
literal|1
argument_list|)
return|;
block|}
end_function

end_unit

