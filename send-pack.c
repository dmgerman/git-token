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

begin_decl_stmt
DECL|variable|send_pack_usage
specifier|static
specifier|const
name|char
name|send_pack_usage
index|[]
init|=
literal|"git-send-pack [--exec=other] destination [heads]*"
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
literal|"git-receive-pack"
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
DECL|member|old_sha1
name|unsigned
name|char
name|old_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|new_sha1
name|unsigned
name|char
name|new_sha1
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
DECL|function|exec_pack_objects
specifier|static
name|void
name|exec_pack_objects
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|char
modifier|*
name|args
index|[]
init|=
block|{
literal|"git-pack-objects"
block|,
literal|"--stdout"
block|,
name|NULL
block|}
decl_stmt|;
name|execvp
argument_list|(
literal|"git-pack-objects"
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"git-pack-objects exec failed (%s)"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|exec_rev_list
specifier|static
name|void
name|exec_rev_list
parameter_list|(
name|struct
name|ref
modifier|*
name|refs
parameter_list|)
block|{
specifier|static
name|char
modifier|*
name|args
index|[
literal|1000
index|]
decl_stmt|;
name|int
name|i
init|=
literal|0
decl_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
literal|"git-rev-list"
expr_stmt|;
comment|/* 0 */
name|args
index|[
name|i
operator|++
index|]
operator|=
literal|"--objects"
expr_stmt|;
comment|/* 1 */
while|while
condition|(
name|refs
condition|)
block|{
name|char
modifier|*
name|buf
init|=
name|malloc
argument_list|(
literal|100
argument_list|)
decl_stmt|;
if|if
condition|(
name|i
operator|>
literal|900
condition|)
name|die
argument_list|(
literal|"git-rev-list environment overflow"
argument_list|)
expr_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
name|buf
expr_stmt|;
name|snprintf
argument_list|(
name|buf
argument_list|,
literal|50
argument_list|,
literal|"^%s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|refs
operator|->
name|old_sha1
argument_list|)
argument_list|)
expr_stmt|;
name|buf
operator|+=
literal|50
expr_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
name|buf
expr_stmt|;
name|snprintf
argument_list|(
name|buf
argument_list|,
literal|50
argument_list|,
literal|"%s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|refs
operator|->
name|new_sha1
argument_list|)
argument_list|)
expr_stmt|;
name|refs
operator|=
name|refs
operator|->
name|next
expr_stmt|;
block|}
name|args
index|[
name|i
index|]
operator|=
name|NULL
expr_stmt|;
name|execvp
argument_list|(
literal|"git-rev-list"
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"git-rev-list exec failed (%s)"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|rev_list
specifier|static
name|void
name|rev_list
parameter_list|(
name|int
name|fd
parameter_list|,
name|struct
name|ref
modifier|*
name|refs
parameter_list|)
block|{
name|int
name|pipe_fd
index|[
literal|2
index|]
decl_stmt|;
name|pid_t
name|pack_objects_pid
decl_stmt|;
if|if
condition|(
name|pipe
argument_list|(
name|pipe_fd
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"rev-list setup: pipe failed"
argument_list|)
expr_stmt|;
name|pack_objects_pid
operator|=
name|fork
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|pack_objects_pid
condition|)
block|{
name|dup2
argument_list|(
name|pipe_fd
index|[
literal|0
index|]
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|dup2
argument_list|(
name|fd
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|pipe_fd
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|pipe_fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|exec_pack_objects
argument_list|()
expr_stmt|;
name|die
argument_list|(
literal|"pack-objects setup failed"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|pack_objects_pid
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"pack-objects fork failed"
argument_list|)
expr_stmt|;
name|dup2
argument_list|(
name|pipe_fd
index|[
literal|1
index|]
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|pipe_fd
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|pipe_fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|exec_rev_list
argument_list|(
name|refs
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|pack_objects
specifier|static
name|int
name|pack_objects
parameter_list|(
name|int
name|fd
parameter_list|,
name|struct
name|ref
modifier|*
name|refs
parameter_list|)
block|{
name|pid_t
name|rev_list_pid
decl_stmt|;
name|rev_list_pid
operator|=
name|fork
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|rev_list_pid
condition|)
block|{
name|rev_list
argument_list|(
name|fd
argument_list|,
name|refs
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"rev-list setup failed"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|rev_list_pid
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"rev-list fork failed"
argument_list|)
expr_stmt|;
comment|/* 	 * We don't wait for the rev-list pipeline in the parent: 	 * we end up waiting for the other end instead 	 */
return|return
literal|0
return|;
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
name|ref
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|fd
decl_stmt|,
name|ret
decl_stmt|;
specifier|static
name|char
name|pathname
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|char
name|buffer
index|[
literal|60
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|git_dir
init|=
name|gitenv
argument_list|(
name|GIT_DIR_ENVIRONMENT
argument_list|)
condition|?
else|:
name|DEFAULT_GIT_DIR_ENVIRONMENT
decl_stmt|;
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
name|ref
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
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
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
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|send_pack
specifier|static
name|int
name|send_pack
parameter_list|(
name|int
name|in
parameter_list|,
name|int
name|out
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
name|last_ref
init|=
operator|&
name|ref_list
decl_stmt|;
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|unsigned
name|char
name|old_sha1
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|char
name|new_sha1
index|[
literal|20
index|]
decl_stmt|;
specifier|static
name|char
name|buffer
index|[
literal|1000
index|]
decl_stmt|;
name|char
modifier|*
name|name
decl_stmt|;
name|int
name|len
decl_stmt|;
name|len
operator|=
name|packet_read_line
argument_list|(
name|in
argument_list|,
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
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
name|buffer
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|buffer
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
name|buffer
argument_list|,
name|old_sha1
argument_list|)
operator|||
name|buffer
index|[
literal|40
index|]
operator|!=
literal|' '
condition|)
name|die
argument_list|(
literal|"protocol error: expected sha/ref, got '%s'"
argument_list|,
name|buffer
argument_list|)
expr_stmt|;
name|name
operator|=
name|buffer
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
name|name
argument_list|,
name|nr_match
argument_list|,
name|match
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|read_ref
argument_list|(
name|name
argument_list|,
name|new_sha1
argument_list|)
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"no such local reference '%s'"
argument_list|,
name|name
argument_list|)
return|;
if|if
condition|(
operator|!
name|has_sha1_file
argument_list|(
name|old_sha1
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"remote '%s' points to object I don't have"
argument_list|,
name|name
argument_list|)
return|;
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|old_sha1
argument_list|,
name|new_sha1
argument_list|,
literal|20
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"'%s' unchanged\n"
argument_list|,
name|name
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|ref
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|ref
argument_list|)
operator|+
name|len
operator|-
literal|40
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|,
name|old_sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|ref
operator|->
name|new_sha1
argument_list|,
name|new_sha1
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
name|buffer
operator|+
literal|41
argument_list|,
name|len
operator|-
literal|40
argument_list|)
expr_stmt|;
name|ref
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
operator|*
name|last_ref
operator|=
name|ref
expr_stmt|;
name|last_ref
operator|=
operator|&
name|ref
operator|->
name|next
expr_stmt|;
block|}
for|for
control|(
name|ref
operator|=
name|ref_list
init|;
name|ref
condition|;
name|ref
operator|=
name|ref
operator|->
name|next
control|)
block|{
name|char
name|old_hex
index|[
literal|60
index|]
decl_stmt|,
modifier|*
name|new_hex
decl_stmt|;
name|strcpy
argument_list|(
name|old_hex
argument_list|,
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|)
argument_list|)
expr_stmt|;
name|new_hex
operator|=
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|new_sha1
argument_list|)
expr_stmt|;
name|packet_write
argument_list|(
name|out
argument_list|,
literal|"%s %s %s"
argument_list|,
name|old_hex
argument_list|,
name|new_hex
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"'%s': updating from %s to %s\n"
argument_list|,
name|ref
operator|->
name|name
argument_list|,
name|old_hex
argument_list|,
name|new_hex
argument_list|)
expr_stmt|;
block|}
name|packet_flush
argument_list|(
name|out
argument_list|)
expr_stmt|;
if|if
condition|(
name|ref_list
condition|)
name|pack_objects
argument_list|(
name|out
argument_list|,
name|ref_list
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|out
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
name|char
modifier|*
name|dest
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
name|int
name|fd
index|[
literal|2
index|]
decl_stmt|,
name|ret
decl_stmt|;
name|pid_t
name|pid
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
literal|"--exec="
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
name|send_pack_usage
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
name|send_pack_usage
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
name|send_pack
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|fd
index|[
literal|1
index|]
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

