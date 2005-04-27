begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"rsh.h"
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_include
include|#
directive|include
file|<sys/types.h>
end_include

begin_include
include|#
directive|include
file|<sys/socket.h>
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_define
DECL|macro|COMMAND_SIZE
define|#
directive|define
name|COMMAND_SIZE
value|4096
end_define

begin_function
DECL|function|setup_connection
name|int
name|setup_connection
parameter_list|(
name|int
modifier|*
name|fd_in
parameter_list|,
name|int
modifier|*
name|fd_out
parameter_list|,
name|char
modifier|*
name|remote_prog
parameter_list|,
name|char
modifier|*
name|url
parameter_list|,
name|int
name|rmt_argc
parameter_list|,
name|char
modifier|*
modifier|*
name|rmt_argv
parameter_list|)
block|{
name|char
modifier|*
name|host
decl_stmt|;
name|char
modifier|*
name|path
decl_stmt|;
name|int
name|sv
index|[
literal|2
index|]
decl_stmt|;
name|char
name|command
index|[
name|COMMAND_SIZE
index|]
decl_stmt|;
name|char
modifier|*
name|posn
decl_stmt|;
name|int
name|i
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|url
argument_list|,
literal|"-"
argument_list|)
condition|)
block|{
operator|*
name|fd_in
operator|=
literal|0
expr_stmt|;
operator|*
name|fd_out
operator|=
literal|1
expr_stmt|;
return|return
literal|0
return|;
block|}
name|host
operator|=
name|strstr
argument_list|(
name|url
argument_list|,
literal|"//"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|host
condition|)
block|{
return|return
name|error
argument_list|(
literal|"Bad URL: %s"
argument_list|,
name|url
argument_list|)
return|;
block|}
name|host
operator|+=
literal|2
expr_stmt|;
name|path
operator|=
name|strchr
argument_list|(
name|host
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|path
condition|)
block|{
return|return
name|error
argument_list|(
literal|"Bad URL: %s"
argument_list|,
name|url
argument_list|)
return|;
block|}
operator|*
operator|(
name|path
operator|++
operator|)
operator|=
literal|'\0'
expr_stmt|;
comment|/* ssh<host> 'cd /<path>; stdio-pull<arg...><commit-id>' */
name|snprintf
argument_list|(
name|command
argument_list|,
name|COMMAND_SIZE
argument_list|,
literal|"cd /%s; SHA1_FILE_DIRECTORY=objects %s"
argument_list|,
name|path
argument_list|,
name|remote_prog
argument_list|)
expr_stmt|;
name|posn
operator|=
name|command
operator|+
name|strlen
argument_list|(
name|command
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
name|rmt_argc
condition|;
name|i
operator|++
control|)
block|{
operator|*
operator|(
name|posn
operator|++
operator|)
operator|=
literal|' '
expr_stmt|;
name|strncpy
argument_list|(
name|posn
argument_list|,
name|rmt_argv
index|[
name|i
index|]
argument_list|,
name|COMMAND_SIZE
operator|-
operator|(
name|posn
operator|-
name|command
operator|)
argument_list|)
expr_stmt|;
name|posn
operator|+=
name|strlen
argument_list|(
name|rmt_argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|posn
operator|-
name|command
operator|+
literal|4
operator|>=
name|COMMAND_SIZE
condition|)
block|{
return|return
name|error
argument_list|(
literal|"Command line too long"
argument_list|)
return|;
block|}
block|}
name|strcpy
argument_list|(
name|posn
argument_list|,
literal|" -"
argument_list|)
expr_stmt|;
if|if
condition|(
name|socketpair
argument_list|(
name|AF_LOCAL
argument_list|,
name|SOCK_STREAM
argument_list|,
literal|0
argument_list|,
name|sv
argument_list|)
condition|)
block|{
return|return
name|error
argument_list|(
literal|"Couldn't create socket"
argument_list|)
return|;
block|}
if|if
condition|(
operator|!
name|fork
argument_list|()
condition|)
block|{
name|close
argument_list|(
name|sv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|dup2
argument_list|(
name|sv
index|[
literal|0
index|]
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|dup2
argument_list|(
name|sv
index|[
literal|0
index|]
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|execlp
argument_list|(
literal|"ssh"
argument_list|,
literal|"ssh"
argument_list|,
name|host
argument_list|,
name|command
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
name|close
argument_list|(
name|sv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
operator|*
name|fd_in
operator|=
name|sv
index|[
literal|1
index|]
expr_stmt|;
operator|*
name|fd_out
operator|=
name|sv
index|[
literal|1
index|]
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

