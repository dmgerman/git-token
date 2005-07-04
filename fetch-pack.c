begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"pkt-line.h"
end_include

begin_include
include|#
directive|include
file|<sys/wait.h>
end_include

begin_decl_stmt
DECL|variable|fetch_pack_usage
specifier|static
specifier|const
name|char
name|fetch_pack_usage
index|[]
init|=
literal|"git-fetch-pack [host:]directory [heads]*< mycommitlist"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|exec
specifier|static
specifier|const
name|char
modifier|*
name|exec
init|=
literal|"git-upload-pack"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|get_ack
specifier|static
name|int
name|get_ack
parameter_list|(
name|int
name|fd
parameter_list|,
name|unsigned
name|char
modifier|*
name|result_sha1
parameter_list|)
block|{
specifier|static
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
name|int
name|len
init|=
name|packet_read_line
argument_list|(
name|fd
argument_list|,
name|line
argument_list|,
sizeof|sizeof
argument_list|(
name|line
argument_list|)
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|len
condition|)
name|die
argument_list|(
literal|"git-fetch-pack: expected ACK/NAK, got EOF"
argument_list|)
expr_stmt|;
if|if
condition|(
name|line
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|line
index|[
operator|--
name|len
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|line
argument_list|,
literal|"NAK"
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|line
argument_list|,
literal|"ACK "
argument_list|,
literal|3
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|get_sha1_hex
argument_list|(
name|line
operator|+
literal|4
argument_list|,
name|result_sha1
argument_list|)
condition|)
return|return
literal|1
return|;
block|}
name|die
argument_list|(
literal|"git-fetch_pack: expected ACK/NAK, got '%s'"
argument_list|,
name|line
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|find_common
specifier|static
name|int
name|find_common
parameter_list|(
name|int
name|fd
index|[
literal|2
index|]
parameter_list|,
name|unsigned
name|char
modifier|*
name|result_sha1
parameter_list|,
name|unsigned
name|char
modifier|*
name|remote
parameter_list|)
block|{
specifier|static
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
name|int
name|count
init|=
literal|0
decl_stmt|,
name|flushes
init|=
literal|0
decl_stmt|,
name|retval
decl_stmt|;
name|FILE
modifier|*
name|revs
decl_stmt|;
name|revs
operator|=
name|popen
argument_list|(
literal|"git-rev-list $(git-rev-parse --all)"
argument_list|,
literal|"r"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|revs
condition|)
name|die
argument_list|(
literal|"unable to run 'git-rev-list'"
argument_list|)
expr_stmt|;
name|packet_write
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|,
literal|"want %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|remote
argument_list|)
argument_list|)
expr_stmt|;
name|packet_flush
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|flushes
operator|=
literal|1
expr_stmt|;
name|retval
operator|=
operator|-
literal|1
expr_stmt|;
while|while
condition|(
name|fgets
argument_list|(
name|line
argument_list|,
sizeof|sizeof
argument_list|(
name|line
argument_list|)
argument_list|,
name|revs
argument_list|)
operator|!=
name|NULL
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|line
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"git-fetch-pack: expected object name, got crud"
argument_list|)
expr_stmt|;
name|packet_write
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|,
literal|"have %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
literal|31
operator|&
operator|++
name|count
operator|)
condition|)
block|{
name|packet_flush
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|flushes
operator|++
expr_stmt|;
comment|/* 			 * We keep one window "ahead" of the other side, and 			 * will wait for an ACK only on the next one 			 */
if|if
condition|(
name|count
operator|==
literal|32
condition|)
continue|continue;
if|if
condition|(
name|get_ack
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|result_sha1
argument_list|)
condition|)
block|{
name|flushes
operator|=
literal|0
expr_stmt|;
name|retval
operator|=
literal|0
expr_stmt|;
break|break;
block|}
name|flushes
operator|--
expr_stmt|;
block|}
block|}
name|pclose
argument_list|(
name|revs
argument_list|)
expr_stmt|;
name|packet_write
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|,
literal|"done\n"
argument_list|)
expr_stmt|;
while|while
condition|(
name|flushes
condition|)
block|{
name|flushes
operator|--
expr_stmt|;
if|if
condition|(
name|get_ack
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|result_sha1
argument_list|)
condition|)
return|return
literal|0
return|;
block|}
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|get_old_sha1
specifier|static
name|int
name|get_old_sha1
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
specifier|static
name|char
name|pathname
index|[
name|PATH_MAX
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|git_dir
decl_stmt|;
name|int
name|fd
decl_stmt|,
name|ret
decl_stmt|;
name|git_dir
operator|=
name|gitenv
argument_list|(
name|GIT_DIR_ENVIRONMENT
argument_list|)
condition|?
else|:
name|DEFAULT_GIT_DIR_ENVIRONMENT
expr_stmt|;
name|snprintf
argument_list|(
name|pathname
argument_list|,
sizeof|sizeof
argument_list|(
name|pathname
argument_list|)
argument_list|,
literal|"%s/%s"
argument_list|,
name|git_dir
argument_list|,
name|refname
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|pathname
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|fd
operator|>=
literal|0
condition|)
block|{
name|char
name|buffer
index|[
literal|60
index|]
decl_stmt|;
if|if
condition|(
name|read
argument_list|(
name|fd
argument_list|,
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
argument_list|)
operator|>=
literal|40
condition|)
name|ret
operator|=
name|get_sha1_hex
argument_list|(
name|buffer
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|check_ref
specifier|static
name|int
name|check_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|unsigned
name|char
name|mysha1
index|[
literal|20
index|]
decl_stmt|;
name|char
name|oldhex
index|[
literal|41
index|]
decl_stmt|;
if|if
condition|(
name|get_old_sha1
argument_list|(
name|refname
argument_list|,
name|mysha1
argument_list|)
operator|<
literal|0
condition|)
name|memset
argument_list|(
name|mysha1
argument_list|,
literal|0
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|sha1
argument_list|,
name|mysha1
argument_list|,
literal|20
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s: unchanged\n"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|memcpy
argument_list|(
name|oldhex
argument_list|,
name|sha1_to_hex
argument_list|(
name|mysha1
argument_list|)
argument_list|,
literal|41
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s: %s (%s)\n"
argument_list|,
name|refname
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|oldhex
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|get_remote_heads
specifier|static
name|int
name|get_remote_heads
parameter_list|(
name|int
name|fd
parameter_list|,
name|int
name|nr_match
parameter_list|,
name|char
modifier|*
modifier|*
name|match
parameter_list|,
name|unsigned
name|char
modifier|*
name|result
parameter_list|)
block|{
name|int
name|count
init|=
literal|0
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
specifier|static
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|refname
decl_stmt|;
name|int
name|len
decl_stmt|;
name|len
operator|=
name|packet_read_line
argument_list|(
name|fd
argument_list|,
name|line
argument_list|,
sizeof|sizeof
argument_list|(
name|line
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|len
condition|)
break|break;
if|if
condition|(
name|line
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|line
index|[
operator|--
name|len
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|len
operator|<
literal|42
operator|||
name|get_sha1_hex
argument_list|(
name|line
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"git-fetch-pack: protocol error - expected ref descriptor, got '%sÃ¤'"
argument_list|,
name|line
argument_list|)
expr_stmt|;
name|refname
operator|=
name|line
operator|+
literal|41
expr_stmt|;
if|if
condition|(
name|nr_match
operator|&&
operator|!
name|path_match
argument_list|(
name|refname
argument_list|,
name|nr_match
argument_list|,
name|match
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|check_ref
argument_list|(
name|refname
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|count
operator|++
expr_stmt|;
name|memcpy
argument_list|(
name|result
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
block|}
block|}
return|return
name|count
return|;
block|}
end_function

begin_function
DECL|function|fetch_pack
specifier|static
name|int
name|fetch_pack
parameter_list|(
name|int
name|fd
index|[
literal|2
index|]
parameter_list|,
name|int
name|nr_match
parameter_list|,
name|char
modifier|*
modifier|*
name|match
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|,
name|remote
index|[
literal|20
index|]
decl_stmt|;
name|int
name|heads
decl_stmt|,
name|status
decl_stmt|;
name|pid_t
name|pid
decl_stmt|;
name|heads
operator|=
name|get_remote_heads
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|nr_match
argument_list|,
name|match
argument_list|,
name|remote
argument_list|)
expr_stmt|;
if|if
condition|(
name|heads
operator|!=
literal|1
condition|)
block|{
name|packet_flush
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|die
argument_list|(
name|heads
condition|?
literal|"multiple remote heads"
else|:
literal|"no matching remote head"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|find_common
argument_list|(
name|fd
argument_list|,
name|sha1
argument_list|,
name|remote
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"git-fetch-pack: no common commits"
argument_list|)
expr_stmt|;
name|pid
operator|=
name|fork
argument_list|()
expr_stmt|;
if|if
condition|(
name|pid
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"git-fetch-pack: unable to fork off git-unpack-objects"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pid
condition|)
block|{
name|close
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|dup2
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
literal|0
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
name|execlp
argument_list|(
literal|"git-unpack-objects"
argument_list|,
literal|"git-unpack-objects"
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"git-unpack-objects exec failed"
argument_list|)
expr_stmt|;
block|}
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
while|while
condition|(
name|waitpid
argument_list|(
name|pid
argument_list|,
operator|&
name|status
argument_list|,
literal|0
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
name|die
argument_list|(
literal|"waiting for git-unpack-objects: %s"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|WIFEXITED
argument_list|(
name|status
argument_list|)
condition|)
block|{
name|int
name|code
init|=
name|WEXITSTATUS
argument_list|(
name|status
argument_list|)
decl_stmt|;
if|if
condition|(
name|code
condition|)
name|die
argument_list|(
literal|"git-unpack-objects died with error code %d"
argument_list|,
name|code
argument_list|)
expr_stmt|;
name|puts
argument_list|(
name|sha1_to_hex
argument_list|(
name|remote
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|WIFSIGNALED
argument_list|(
name|status
argument_list|)
condition|)
block|{
name|int
name|sig
init|=
name|WTERMSIG
argument_list|(
name|status
argument_list|)
decl_stmt|;
name|die
argument_list|(
literal|"git-unpack-objects died of signal %d"
argument_list|,
name|sig
argument_list|)
expr_stmt|;
block|}
name|die
argument_list|(
literal|"Sherlock Holmes! git-unpack-objects died of unnatural causes %d!"
argument_list|,
name|status
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
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|ret
decl_stmt|,
name|nr_heads
decl_stmt|;
name|char
modifier|*
name|dest
init|=
name|NULL
decl_stmt|,
modifier|*
modifier|*
name|heads
decl_stmt|;
name|int
name|fd
index|[
literal|2
index|]
decl_stmt|;
name|pid_t
name|pid
decl_stmt|;
name|nr_heads
operator|=
literal|0
expr_stmt|;
name|heads
operator|=
name|NULL
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
operator|*
name|arg
operator|==
literal|'-'
condition|)
block|{
comment|/* Arguments go here */
name|usage
argument_list|(
name|fetch_pack_usage
argument_list|)
expr_stmt|;
block|}
name|dest
operator|=
name|arg
expr_stmt|;
name|heads
operator|=
name|argv
operator|+
name|i
operator|+
literal|1
expr_stmt|;
name|nr_heads
operator|=
name|argc
operator|-
name|i
operator|-
literal|1
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|dest
condition|)
name|usage
argument_list|(
name|fetch_pack_usage
argument_list|)
expr_stmt|;
name|pid
operator|=
name|git_connect
argument_list|(
name|fd
argument_list|,
name|dest
argument_list|,
name|exec
argument_list|)
expr_stmt|;
if|if
condition|(
name|pid
operator|<
literal|0
condition|)
return|return
literal|1
return|;
name|ret
operator|=
name|fetch_pack
argument_list|(
name|fd
argument_list|,
name|nr_heads
argument_list|,
name|heads
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
name|finish_connect
argument_list|(
name|pid
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

end_unit

