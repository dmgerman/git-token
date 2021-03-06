begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"credential.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"unix-socket.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_define
DECL|macro|FLAG_SPAWN
define|#
directive|define
name|FLAG_SPAWN
value|0x1
end_define

begin_define
DECL|macro|FLAG_RELAY
define|#
directive|define
name|FLAG_RELAY
value|0x2
end_define

begin_function
DECL|function|send_request
specifier|static
name|int
name|send_request
parameter_list|(
specifier|const
name|char
modifier|*
name|socket
parameter_list|,
specifier|const
name|struct
name|strbuf
modifier|*
name|out
parameter_list|)
block|{
name|int
name|got_data
init|=
literal|0
decl_stmt|;
name|int
name|fd
init|=
name|unix_stream_connect
argument_list|(
name|socket
argument_list|)
decl_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|write_in_full
argument_list|(
name|fd
argument_list|,
name|out
operator|->
name|buf
argument_list|,
name|out
operator|->
name|len
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"unable to write to cache daemon"
argument_list|)
expr_stmt|;
name|shutdown
argument_list|(
name|fd
argument_list|,
name|SHUT_WR
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|char
name|in
index|[
literal|1024
index|]
decl_stmt|;
name|int
name|r
decl_stmt|;
name|r
operator|=
name|read_in_full
argument_list|(
name|fd
argument_list|,
name|in
argument_list|,
sizeof|sizeof
argument_list|(
name|in
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|==
literal|0
condition|)
break|break;
if|if
condition|(
name|r
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"read error from cache daemon"
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|in
argument_list|,
name|r
argument_list|)
expr_stmt|;
name|got_data
operator|=
literal|1
expr_stmt|;
block|}
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
return|return
name|got_data
return|;
block|}
end_function

begin_function
DECL|function|spawn_daemon
specifier|static
name|void
name|spawn_daemon
parameter_list|(
specifier|const
name|char
modifier|*
name|socket
parameter_list|)
block|{
name|struct
name|child_process
name|daemon
init|=
name|CHILD_PROCESS_INIT
decl_stmt|;
specifier|const
name|char
modifier|*
name|argv
index|[]
init|=
block|{
name|NULL
block|,
name|NULL
block|,
name|NULL
block|}
decl_stmt|;
name|char
name|buf
index|[
literal|128
index|]
decl_stmt|;
name|int
name|r
decl_stmt|;
name|argv
index|[
literal|0
index|]
operator|=
literal|"git-credential-cache--daemon"
expr_stmt|;
name|argv
index|[
literal|1
index|]
operator|=
name|socket
expr_stmt|;
name|daemon
operator|.
name|argv
operator|=
name|argv
expr_stmt|;
name|daemon
operator|.
name|no_stdin
operator|=
literal|1
expr_stmt|;
name|daemon
operator|.
name|out
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|start_command
argument_list|(
operator|&
name|daemon
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"unable to start cache daemon"
argument_list|)
expr_stmt|;
name|r
operator|=
name|read_in_full
argument_list|(
name|daemon
operator|.
name|out
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
name|r
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"unable to read result code from cache daemon"
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|!=
literal|3
operator|||
name|memcmp
argument_list|(
name|buf
argument_list|,
literal|"ok\n"
argument_list|,
literal|3
argument_list|)
condition|)
name|die
argument_list|(
literal|"cache daemon did not start: %.*s"
argument_list|,
name|r
argument_list|,
name|buf
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|daemon
operator|.
name|out
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|do_cache
specifier|static
name|void
name|do_cache
parameter_list|(
specifier|const
name|char
modifier|*
name|socket
parameter_list|,
specifier|const
name|char
modifier|*
name|action
parameter_list|,
name|int
name|timeout
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"action=%s\n"
argument_list|,
name|action
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"timeout=%d\n"
argument_list|,
name|timeout
argument_list|)
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|FLAG_RELAY
condition|)
block|{
if|if
condition|(
name|strbuf_read
argument_list|(
operator|&
name|buf
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"unable to relay credential"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|send_request
argument_list|(
name|socket
argument_list|,
operator|&
name|buf
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|!=
name|ENOENT
operator|&&
name|errno
operator|!=
name|ECONNREFUSED
condition|)
name|die_errno
argument_list|(
literal|"unable to connect to cache daemon"
argument_list|)
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|FLAG_SPAWN
condition|)
block|{
name|spawn_daemon
argument_list|(
name|socket
argument_list|)
expr_stmt|;
if|if
condition|(
name|send_request
argument_list|(
name|socket
argument_list|,
operator|&
name|buf
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"unable to connect to cache daemon"
argument_list|)
expr_stmt|;
block|}
block|}
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_main
name|int
name|cmd_main
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
name|char
modifier|*
name|socket_path
init|=
name|NULL
decl_stmt|;
name|int
name|timeout
init|=
literal|900
decl_stmt|;
specifier|const
name|char
modifier|*
name|op
decl_stmt|;
specifier|const
name|char
modifier|*
specifier|const
name|usage
index|[]
init|=
block|{
literal|"git credential-cache [<options>]<action>"
block|,
name|NULL
block|}
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_INTEGER
argument_list|(
literal|0
argument_list|,
literal|"timeout"
argument_list|,
operator|&
name|timeout
argument_list|,
literal|"number of seconds to cache credentials"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"socket"
argument_list|,
operator|&
name|socket_path
argument_list|,
literal|"path"
argument_list|,
literal|"path of cache-daemon socket"
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|NULL
argument_list|,
name|options
argument_list|,
name|usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|argc
condition|)
name|usage_with_options
argument_list|(
name|usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
name|op
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|socket_path
condition|)
name|socket_path
operator|=
name|expand_user_path
argument_list|(
literal|"~/.git-credential-cache/socket"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|socket_path
condition|)
name|die
argument_list|(
literal|"unable to find a suitable socket path; use --socket"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|op
argument_list|,
literal|"exit"
argument_list|)
condition|)
name|do_cache
argument_list|(
name|socket_path
argument_list|,
name|op
argument_list|,
name|timeout
argument_list|,
literal|0
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|op
argument_list|,
literal|"get"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|op
argument_list|,
literal|"erase"
argument_list|)
condition|)
name|do_cache
argument_list|(
name|socket_path
argument_list|,
name|op
argument_list|,
name|timeout
argument_list|,
name|FLAG_RELAY
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|op
argument_list|,
literal|"store"
argument_list|)
condition|)
name|do_cache
argument_list|(
name|socket_path
argument_list|,
name|op
argument_list|,
name|timeout
argument_list|,
name|FLAG_RELAY
operator||
name|FLAG_SPAWN
argument_list|)
expr_stmt|;
else|else
empty_stmt|;
comment|/* ignore unknown operation */
return|return
literal|0
return|;
block|}
end_function

end_unit

