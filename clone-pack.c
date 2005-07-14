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
DECL|variable|quiet
specifier|static
name|int
name|quiet
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|clone_pack_usage
specifier|static
specifier|const
name|char
name|clone_pack_usage
index|[]
init|=
literal|"git-clone-pack [-q] [--exec=<git-upload-pack>] [<host>:]<directory> [<heads>]*"
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

begin_struct
DECL|struct|ref
struct|struct
name|ref
block|{
DECL|member|next
name|struct
name|ref
modifier|*
name|next
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|name
name|char
name|name
index|[
literal|0
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|get_remote_refs
specifier|static
name|struct
name|ref
modifier|*
name|get_remote_refs
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
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|ref_list
init|=
name|NULL
decl_stmt|,
modifier|*
modifier|*
name|next_ref
init|=
operator|&
name|ref_list
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
name|struct
name|ref
modifier|*
name|ref
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
literal|"git-clone-pack: protocol error - expected ref descriptor, got '%s'"
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
name|len
operator|=
name|len
operator|-
literal|40
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
name|ref
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|ref
argument_list|)
operator|+
name|len
argument_list|)
expr_stmt|;
name|ref
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|memcpy
argument_list|(
name|ref
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|ref
operator|->
name|name
argument_list|,
name|refname
argument_list|,
name|len
argument_list|)
expr_stmt|;
operator|*
name|next_ref
operator|=
name|ref
expr_stmt|;
name|next_ref
operator|=
operator|&
name|ref
operator|->
name|next
expr_stmt|;
block|}
return|return
name|ref_list
return|;
block|}
end_function

begin_function
DECL|function|clone_handshake
specifier|static
name|void
name|clone_handshake
parameter_list|(
name|int
name|fd
index|[
literal|2
index|]
parameter_list|,
name|struct
name|ref
modifier|*
name|ref
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
while|while
condition|(
name|ref
condition|)
block|{
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
name|ref
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|ref
operator|=
name|ref
operator|->
name|next
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
comment|/* We don't have nuttin' */
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
if|if
condition|(
name|get_ack
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|sha1
argument_list|)
condition|)
name|error
argument_list|(
literal|"Huh! git-clone-pack got positive ack for %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|is_master
specifier|static
name|int
name|is_master
parameter_list|(
name|struct
name|ref
modifier|*
name|ref
parameter_list|)
block|{
return|return
operator|!
name|strcmp
argument_list|(
name|ref
operator|->
name|name
argument_list|,
literal|"refs/heads/master"
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|write_one_ref
specifier|static
name|void
name|write_one_ref
parameter_list|(
name|struct
name|ref
modifier|*
name|ref
parameter_list|)
block|{
name|char
modifier|*
name|path
init|=
name|git_path
argument_list|(
name|ref
operator|->
name|name
argument_list|)
decl_stmt|;
name|int
name|fd
decl_stmt|;
name|char
modifier|*
name|hex
decl_stmt|;
if|if
condition|(
name|safe_create_leading_directories
argument_list|(
name|path
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to create leading directory for %s"
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|path
argument_list|,
name|O_CREAT
operator||
name|O_EXCL
operator||
name|O_WRONLY
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to create ref %s"
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|hex
operator|=
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|hex
index|[
literal|40
index|]
operator|=
literal|'\n'
expr_stmt|;
if|if
condition|(
name|write
argument_list|(
name|fd
argument_list|,
name|hex
argument_list|,
literal|41
argument_list|)
operator|!=
literal|41
condition|)
name|die
argument_list|(
literal|"unable to write ref %s"
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|write_refs
specifier|static
name|void
name|write_refs
parameter_list|(
name|struct
name|ref
modifier|*
name|ref
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|head
init|=
name|NULL
decl_stmt|,
modifier|*
name|head_ptr
decl_stmt|,
modifier|*
name|master_ref
decl_stmt|;
name|char
modifier|*
name|head_path
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|ref
operator|->
name|name
argument_list|,
literal|"HEAD"
argument_list|)
condition|)
block|{
name|head
operator|=
name|ref
expr_stmt|;
name|ref
operator|=
name|ref
operator|->
name|next
expr_stmt|;
block|}
name|head_ptr
operator|=
name|NULL
expr_stmt|;
name|master_ref
operator|=
name|NULL
expr_stmt|;
while|while
condition|(
name|ref
condition|)
block|{
if|if
condition|(
name|is_master
argument_list|(
name|ref
argument_list|)
condition|)
name|master_ref
operator|=
name|ref
expr_stmt|;
if|if
condition|(
name|head
operator|&&
operator|!
name|memcmp
argument_list|(
name|ref
operator|->
name|sha1
argument_list|,
name|head
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|head_ptr
operator|||
name|ref
operator|==
name|master_ref
condition|)
name|head_ptr
operator|=
name|ref
expr_stmt|;
block|}
name|write_one_ref
argument_list|(
name|ref
argument_list|)
expr_stmt|;
name|ref
operator|=
name|ref
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|head
condition|)
return|return;
name|head_path
operator|=
name|git_path
argument_list|(
literal|"HEAD"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|head_ptr
condition|)
block|{
comment|/* 		 * If we had a master ref, and it wasn't HEAD, we need to undo the 		 * symlink, and write a standalone HEAD. Give a warning, because that's 		 * really really wrong. 		 */
if|if
condition|(
name|master_ref
condition|)
block|{
name|error
argument_list|(
literal|"HEAD doesn't point to any refs! Making standalone HEAD"
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|head_path
argument_list|)
expr_stmt|;
block|}
name|write_one_ref
argument_list|(
name|head
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* We reset to the master branch if it's available */
if|if
condition|(
name|master_ref
condition|)
return|return;
comment|/* 	 * Uhhuh. Other end didn't have master. We start HEAD off with 	 * the first branch with the same value. 	 */
name|unlink
argument_list|(
name|head_path
argument_list|)
expr_stmt|;
if|if
condition|(
name|symlink
argument_list|(
name|head_ptr
operator|->
name|name
argument_list|,
name|head_path
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to link HEAD to %s"
argument_list|,
name|head_ptr
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|clone_pack
specifier|static
name|int
name|clone_pack
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
name|struct
name|ref
modifier|*
name|refs
decl_stmt|;
name|int
name|status
decl_stmt|;
name|pid_t
name|pid
decl_stmt|;
name|refs
operator|=
name|get_remote_refs
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|nr_match
argument_list|,
name|match
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|refs
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
literal|"no matching remote head"
argument_list|)
expr_stmt|;
block|}
name|clone_handshake
argument_list|(
name|fd
argument_list|,
name|refs
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
literal|"git-clone-pack: unable to fork off git-unpack-objects"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pid
condition|)
block|{
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
name|close
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|execlp
argument_list|(
literal|"git-unpack-objects"
argument_list|,
literal|"git-unpack-objects"
argument_list|,
name|quiet
condition|?
literal|"-q"
else|:
name|NULL
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
name|write_refs
argument_list|(
name|refs
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"-q"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|quiet
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
literal|"--exec="
argument_list|,
name|arg
argument_list|,
literal|7
argument_list|)
condition|)
block|{
name|exec
operator|=
name|arg
operator|+
literal|7
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|clone_pack_usage
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
name|clone_pack_usage
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
name|clone_pack
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

