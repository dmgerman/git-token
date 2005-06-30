begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
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
DECL|variable|receive_pack_usage
specifier|static
specifier|const
name|char
name|receive_pack_usage
index|[]
init|=
literal|"git-receive-pack [--unpack=executable]<git-dir> [heads]"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|unpacker
specifier|static
specifier|const
name|char
modifier|*
name|unpacker
init|=
literal|"git-unpack-objects"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|path_match
specifier|static
name|int
name|path_match
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|nr
parameter_list|,
name|char
modifier|*
modifier|*
name|match
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|pathlen
init|=
name|strlen
argument_list|(
name|path
argument_list|)
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|char
modifier|*
name|s
init|=
name|match
index|[
name|i
index|]
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|s
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|len
operator|||
name|len
operator|>
name|pathlen
condition|)
continue|continue;
if|if
condition|(
name|memcmp
argument_list|(
name|path
operator|+
name|pathlen
operator|-
name|len
argument_list|,
name|s
argument_list|,
name|len
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|pathlen
operator|>
name|len
operator|&&
name|path
index|[
name|pathlen
operator|-
name|len
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
continue|continue;
operator|*
name|s
operator|=
literal|0
expr_stmt|;
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|show_ref
specifier|static
name|void
name|show_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"%s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|read_ref
specifier|static
name|int
name|read_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|ret
init|=
operator|-
literal|1
decl_stmt|;
name|int
name|fd
init|=
name|open
argument_list|(
name|path
argument_list|,
name|O_RDONLY
argument_list|)
decl_stmt|;
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
DECL|function|write_head_info
specifier|static
name|void
name|write_head_info
parameter_list|(
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|nr
parameter_list|,
name|char
modifier|*
modifier|*
name|match
parameter_list|)
block|{
name|DIR
modifier|*
name|dir
init|=
name|opendir
argument_list|(
name|base
argument_list|)
decl_stmt|;
if|if
condition|(
name|dir
condition|)
block|{
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
name|int
name|baselen
init|=
name|strlen
argument_list|(
name|base
argument_list|)
decl_stmt|;
name|char
modifier|*
name|path
init|=
name|xmalloc
argument_list|(
name|baselen
operator|+
literal|257
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|path
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|de
operator|=
name|readdir
argument_list|(
name|dir
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|int
name|namelen
decl_stmt|;
if|if
condition|(
name|de
operator|->
name|d_name
index|[
literal|0
index|]
operator|==
literal|'.'
condition|)
continue|continue;
name|namelen
operator|=
name|strlen
argument_list|(
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|namelen
operator|>
literal|255
condition|)
continue|continue;
name|memcpy
argument_list|(
name|path
operator|+
name|baselen
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
name|namelen
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|path
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
continue|continue;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
name|path
index|[
name|baselen
operator|+
name|namelen
index|]
operator|=
literal|'/'
expr_stmt|;
name|path
index|[
name|baselen
operator|+
name|namelen
operator|+
literal|1
index|]
operator|=
literal|0
expr_stmt|;
name|write_head_info
argument_list|(
name|path
argument_list|,
name|nr
argument_list|,
name|match
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|read_ref
argument_list|(
name|path
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
continue|continue;
if|if
condition|(
name|nr
operator|&&
operator|!
name|path_match
argument_list|(
name|path
argument_list|,
name|nr
argument_list|,
name|match
argument_list|)
condition|)
continue|continue;
name|show_ref
argument_list|(
name|path
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_struct
DECL|struct|line
struct|struct
name|line
block|{
DECL|member|next
name|struct
name|line
modifier|*
name|next
decl_stmt|;
DECL|member|data
name|char
name|data
index|[
literal|0
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|commands
name|struct
name|line
modifier|*
name|commands
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * This gets called after(if) we've successfully  * unpacked the data payload.  */
end_comment

begin_function
DECL|function|execute_commands
specifier|static
name|void
name|execute_commands
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|line
modifier|*
name|line
init|=
name|commands
decl_stmt|;
while|while
condition|(
name|line
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s"
argument_list|,
name|line
operator|->
name|data
argument_list|)
expr_stmt|;
name|line
operator|=
name|line
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|read_head_info
specifier|static
name|void
name|read_head_info
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|line
modifier|*
modifier|*
name|p
init|=
operator|&
name|commands
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
name|int
name|len
init|=
name|packet_read_line
argument_list|(
literal|0
argument_list|,
name|line
argument_list|,
sizeof|sizeof
argument_list|(
name|line
argument_list|)
argument_list|)
decl_stmt|;
name|struct
name|line
modifier|*
name|n
decl_stmt|;
if|if
condition|(
operator|!
name|len
condition|)
break|break;
name|n
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|line
argument_list|)
operator|+
name|len
argument_list|)
expr_stmt|;
name|n
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|memcpy
argument_list|(
name|n
operator|->
name|data
argument_list|,
name|line
operator|+
literal|4
argument_list|,
name|len
operator|-
literal|3
argument_list|)
expr_stmt|;
operator|*
name|p
operator|=
name|n
expr_stmt|;
name|p
operator|=
operator|&
name|n
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|unpack
specifier|static
name|void
name|unpack
parameter_list|(
name|void
parameter_list|)
block|{
name|pid_t
name|pid
init|=
name|fork
argument_list|()
decl_stmt|;
if|if
condition|(
name|pid
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unpack fork failed"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pid
condition|)
block|{
name|setenv
argument_list|(
literal|"GIT_DIR"
argument_list|,
literal|"."
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|execlp
argument_list|(
name|unpacker
argument_list|,
name|unpacker
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"unpack execute failed"
argument_list|)
expr_stmt|;
block|}
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|status
decl_stmt|,
name|code
decl_stmt|;
name|int
name|retval
init|=
name|waitpid
argument_list|(
name|pid
argument_list|,
operator|&
name|status
argument_list|,
literal|0
argument_list|)
decl_stmt|;
if|if
condition|(
name|retval
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|==
name|EINTR
condition|)
continue|continue;
name|die
argument_list|(
literal|"waitpid failed (%s)"
argument_list|,
name|strerror
argument_list|(
name|retval
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|retval
operator|!=
name|pid
condition|)
name|die
argument_list|(
literal|"waitpid is confused"
argument_list|)
expr_stmt|;
if|if
condition|(
name|WIFSIGNALED
argument_list|(
name|status
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s died of signal %d"
argument_list|,
name|unpacker
argument_list|,
name|WTERMSIG
argument_list|(
name|status
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|WIFEXITED
argument_list|(
name|status
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s died out of really strange complications"
argument_list|,
name|unpacker
argument_list|)
expr_stmt|;
name|code
operator|=
name|WEXITSTATUS
argument_list|(
name|status
argument_list|)
expr_stmt|;
if|if
condition|(
name|code
condition|)
name|die
argument_list|(
literal|"%s exited with error code %d"
argument_list|,
name|unpacker
argument_list|,
name|code
argument_list|)
expr_stmt|;
return|return;
block|}
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
name|nr_heads
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|dir
init|=
name|NULL
decl_stmt|;
name|char
modifier|*
modifier|*
name|heads
init|=
name|NULL
decl_stmt|;
name|argv
operator|++
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
specifier|const
name|char
modifier|*
name|arg
init|=
operator|*
name|argv
operator|++
decl_stmt|;
if|if
condition|(
operator|*
name|arg
operator|==
literal|'-'
condition|)
block|{
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--unpack="
argument_list|,
literal|9
argument_list|)
condition|)
block|{
name|unpacker
operator|=
name|arg
operator|+
literal|9
expr_stmt|;
continue|continue;
block|}
comment|/* Do flag handling here */
name|usage
argument_list|(
name|receive_pack_usage
argument_list|)
expr_stmt|;
block|}
name|dir
operator|=
name|arg
expr_stmt|;
name|heads
operator|=
name|argv
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
name|dir
condition|)
name|usage
argument_list|(
name|receive_pack_usage
argument_list|)
expr_stmt|;
comment|/* chdir to the directory. If that fails, try appending ".git" */
if|if
condition|(
name|chdir
argument_list|(
name|dir
argument_list|)
operator|<
literal|0
condition|)
block|{
specifier|static
name|char
name|path
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|snprintf
argument_list|(
name|path
argument_list|,
sizeof|sizeof
argument_list|(
name|path
argument_list|)
argument_list|,
literal|"%s.git"
argument_list|,
name|dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|chdir
argument_list|(
name|path
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to cd to %s"
argument_list|,
name|dir
argument_list|)
expr_stmt|;
block|}
comment|/* If we have a ".git" directory, chdir to it */
name|chdir
argument_list|(
literal|".git"
argument_list|)
expr_stmt|;
if|if
condition|(
name|access
argument_list|(
literal|"objects"
argument_list|,
name|X_OK
argument_list|)
operator|<
literal|0
operator|||
name|access
argument_list|(
literal|"refs/heads"
argument_list|,
name|X_OK
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"%s doesn't appear to be a git directory"
argument_list|,
name|dir
argument_list|)
expr_stmt|;
name|write_head_info
argument_list|(
literal|"refs/"
argument_list|,
name|nr_heads
argument_list|,
name|heads
argument_list|)
expr_stmt|;
comment|/* EOF */
name|packet_flush
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|read_head_info
argument_list|()
expr_stmt|;
name|unpack
argument_list|()
expr_stmt|;
name|execute_commands
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

