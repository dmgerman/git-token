begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2006 Franck Bui-Huu  */
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

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"argv-array.h"
end_include

begin_decl_stmt
DECL|variable|upload_archive_usage
specifier|static
specifier|const
name|char
name|upload_archive_usage
index|[]
init|=
literal|"git upload-archive<repo>"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|deadchild
specifier|static
specifier|const
name|char
name|deadchild
index|[]
init|=
literal|"git upload-archive: archiver died with error"
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|MAX_ARGS
define|#
directive|define
name|MAX_ARGS
value|(64)
end_define

begin_function
DECL|function|cmd_upload_archive_writer
name|int
name|cmd_upload_archive_writer
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
name|struct
name|argv_array
name|sent_argv
init|=
name|ARGV_ARRAY_INIT
decl_stmt|;
specifier|const
name|char
modifier|*
name|arg_cmd
init|=
literal|"argument "
decl_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|2
condition|)
name|usage
argument_list|(
name|upload_archive_usage
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|enter_repo
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|0
argument_list|)
condition|)
name|die
argument_list|(
literal|"'%s' does not appear to be a git repository"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
comment|/* put received options in sent_argv[] */
name|argv_array_push
argument_list|(
operator|&
name|sent_argv
argument_list|,
literal|"git-upload-archive"
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|char
modifier|*
name|buf
init|=
name|packet_read_line
argument_list|(
literal|0
argument_list|,
name|NULL
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
break|break;
comment|/* got a flush */
if|if
condition|(
name|sent_argv
operator|.
name|argc
operator|>
name|MAX_ARGS
condition|)
name|die
argument_list|(
literal|"Too many options (>%d)"
argument_list|,
name|MAX_ARGS
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|starts_with
argument_list|(
name|buf
argument_list|,
name|arg_cmd
argument_list|)
condition|)
name|die
argument_list|(
literal|"'argument' token or flush expected"
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|sent_argv
argument_list|,
name|buf
operator|+
name|strlen
argument_list|(
name|arg_cmd
argument_list|)
argument_list|)
expr_stmt|;
block|}
comment|/* parse all options sent by the client */
return|return
name|write_archive
argument_list|(
name|sent_argv
operator|.
name|argc
argument_list|,
name|sent_argv
operator|.
name|argv
argument_list|,
name|prefix
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
literal|1
argument_list|)
return|;
block|}
end_function

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|1
argument|,
literal|2
argument|))
argument_list|)
end_macro

begin_function
DECL|function|error_clnt
specifier|static
name|void
name|error_clnt
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|va_list
name|params
decl_stmt|;
name|va_start
argument_list|(
name|params
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|strbuf_vaddf
argument_list|(
operator|&
name|buf
argument_list|,
name|fmt
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
name|send_sideband
argument_list|(
literal|1
argument_list|,
literal|3
argument_list|,
name|buf
operator|.
name|buf
argument_list|,
name|buf
operator|.
name|len
argument_list|,
name|LARGE_PACKET_MAX
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"sent error to the client: %s"
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|process_input
specifier|static
name|ssize_t
name|process_input
parameter_list|(
name|int
name|child_fd
parameter_list|,
name|int
name|band
parameter_list|)
block|{
name|char
name|buf
index|[
literal|16384
index|]
decl_stmt|;
name|ssize_t
name|sz
init|=
name|read
argument_list|(
name|child_fd
argument_list|,
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|)
decl_stmt|;
if|if
condition|(
name|sz
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|!=
name|EAGAIN
operator|&&
name|errno
operator|!=
name|EINTR
condition|)
name|error_clnt
argument_list|(
literal|"read error: %s\n"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|sz
return|;
block|}
name|send_sideband
argument_list|(
literal|1
argument_list|,
name|band
argument_list|,
name|buf
argument_list|,
name|sz
argument_list|,
name|LARGE_PACKET_MAX
argument_list|)
expr_stmt|;
return|return
name|sz
return|;
block|}
end_function

begin_function
DECL|function|cmd_upload_archive
name|int
name|cmd_upload_archive
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
name|struct
name|child_process
name|writer
init|=
block|{
name|argv
block|}
decl_stmt|;
comment|/* 	 * Set up sideband subprocess. 	 * 	 * We (parent) monitor and read from child, sending its fd#1 and fd#2 	 * multiplexed out to our fd#1.  If the child dies, we tell the other 	 * end over channel #3. 	 */
name|argv
index|[
literal|0
index|]
operator|=
literal|"upload-archive--writer"
expr_stmt|;
name|writer
operator|.
name|out
operator|=
name|writer
operator|.
name|err
operator|=
operator|-
literal|1
expr_stmt|;
name|writer
operator|.
name|git_cmd
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|start_command
argument_list|(
operator|&
name|writer
argument_list|)
condition|)
block|{
name|int
name|err
init|=
name|errno
decl_stmt|;
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"NACK unable to spawn subprocess\n"
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"upload-archive: %s"
argument_list|,
name|strerror
argument_list|(
name|err
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"ACK\n"
argument_list|)
expr_stmt|;
name|packet_flush
argument_list|(
literal|1
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|struct
name|pollfd
name|pfd
index|[
literal|2
index|]
decl_stmt|;
name|pfd
index|[
literal|0
index|]
operator|.
name|fd
operator|=
name|writer
operator|.
name|out
expr_stmt|;
name|pfd
index|[
literal|0
index|]
operator|.
name|events
operator|=
name|POLLIN
expr_stmt|;
name|pfd
index|[
literal|1
index|]
operator|.
name|fd
operator|=
name|writer
operator|.
name|err
expr_stmt|;
name|pfd
index|[
literal|1
index|]
operator|.
name|events
operator|=
name|POLLIN
expr_stmt|;
if|if
condition|(
name|poll
argument_list|(
name|pfd
argument_list|,
literal|2
argument_list|,
operator|-
literal|1
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|!=
name|EINTR
condition|)
block|{
name|error_errno
argument_list|(
literal|"poll failed resuming"
argument_list|)
expr_stmt|;
name|sleep
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
continue|continue;
block|}
if|if
condition|(
name|pfd
index|[
literal|1
index|]
operator|.
name|revents
operator|&
name|POLLIN
condition|)
comment|/* Status stream ready */
if|if
condition|(
name|process_input
argument_list|(
name|pfd
index|[
literal|1
index|]
operator|.
name|fd
argument_list|,
literal|2
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|pfd
index|[
literal|0
index|]
operator|.
name|revents
operator|&
name|POLLIN
condition|)
comment|/* Data stream ready */
if|if
condition|(
name|process_input
argument_list|(
name|pfd
index|[
literal|0
index|]
operator|.
name|fd
argument_list|,
literal|1
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|finish_command
argument_list|(
operator|&
name|writer
argument_list|)
condition|)
name|error_clnt
argument_list|(
literal|"%s"
argument_list|,
name|deadchild
argument_list|)
expr_stmt|;
name|packet_flush
argument_list|(
literal|1
argument_list|)
expr_stmt|;
break|break;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

