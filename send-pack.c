begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
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

begin_decl_stmt
DECL|variable|send_pack_usage
specifier|static
specifier|const
name|char
name|send_pack_usage
index|[]
init|=
literal|"git-send-pack [--all] [--exec=git-receive-pack]<remote> [<head>...]\n"
literal|"  --all and explicit<head> specification are mutually exclusive."
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

begin_decl_stmt
DECL|variable|send_all
specifier|static
name|int
name|send_all
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|force_update
specifier|static
name|int
name|force_update
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|is_zero_sha1
specifier|static
name|int
name|is_zero_sha1
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|20
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|sha1
operator|++
condition|)
return|return
literal|0
return|;
block|}
return|return
literal|1
return|;
block|}
end_function

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
if|if
condition|(
operator|!
name|is_zero_sha1
argument_list|(
name|refs
operator|->
name|old_sha1
argument_list|)
operator|&&
name|has_sha1_file
argument_list|(
name|refs
operator|->
name|old_sha1
argument_list|)
condition|)
block|{
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
block|}
if|if
condition|(
operator|!
name|is_zero_sha1
argument_list|(
name|refs
operator|->
name|new_sha1
argument_list|)
condition|)
block|{
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
block|}
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
DECL|function|unmark_and_free
specifier|static
name|void
name|unmark_and_free
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|,
name|unsigned
name|int
name|mark
parameter_list|)
block|{
while|while
condition|(
name|list
condition|)
block|{
name|struct
name|commit_list
modifier|*
name|temp
init|=
name|list
decl_stmt|;
name|temp
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&=
operator|~
name|mark
expr_stmt|;
name|list
operator|=
name|temp
operator|->
name|next
expr_stmt|;
name|free
argument_list|(
name|temp
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ref_newer
specifier|static
name|int
name|ref_newer
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|o
decl_stmt|;
name|struct
name|commit
modifier|*
name|old
decl_stmt|,
modifier|*
name|new
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|list
decl_stmt|,
modifier|*
name|used
decl_stmt|;
name|int
name|found
init|=
literal|0
decl_stmt|;
comment|/* Both new and old must be commit-ish and new is descendant of 	 * old.  Otherwise we require --force. 	 */
name|o
operator|=
name|deref_tag
argument_list|(
name|parse_object
argument_list|(
name|old_sha1
argument_list|)
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|o
operator|||
name|o
operator|->
name|type
operator|!=
name|commit_type
condition|)
return|return
literal|0
return|;
name|old
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o
expr_stmt|;
name|o
operator|=
name|deref_tag
argument_list|(
name|parse_object
argument_list|(
name|new_sha1
argument_list|)
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|o
operator|||
name|o
operator|->
name|type
operator|!=
name|commit_type
condition|)
return|return
literal|0
return|;
name|new
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o
expr_stmt|;
if|if
condition|(
name|parse_commit
argument_list|(
name|new
argument_list|)
operator|<
literal|0
condition|)
return|return
literal|0
return|;
name|used
operator|=
name|list
operator|=
name|NULL
expr_stmt|;
name|commit_list_insert
argument_list|(
name|new
argument_list|,
operator|&
name|list
argument_list|)
expr_stmt|;
while|while
condition|(
name|list
condition|)
block|{
name|new
operator|=
name|pop_most_recent_commit
argument_list|(
operator|&
name|list
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|commit_list_insert
argument_list|(
name|new
argument_list|,
operator|&
name|used
argument_list|)
expr_stmt|;
if|if
condition|(
name|new
operator|==
name|old
condition|)
block|{
name|found
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
name|unmark_and_free
argument_list|(
name|list
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|unmark_and_free
argument_list|(
name|used
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
name|found
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|local_refs
DECL|variable|local_tail
specifier|static
name|struct
name|ref
modifier|*
name|local_refs
decl_stmt|,
modifier|*
modifier|*
name|local_tail
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|remote_refs
DECL|variable|remote_tail
specifier|static
name|struct
name|ref
modifier|*
name|remote_refs
decl_stmt|,
modifier|*
modifier|*
name|remote_tail
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|one_local_ref
specifier|static
name|int
name|one_local_ref
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
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|refname
argument_list|)
operator|+
literal|1
decl_stmt|;
name|ref
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|ref
argument_list|)
operator|+
name|len
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|ref
operator|->
name|new_sha1
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
name|local_tail
operator|=
name|ref
expr_stmt|;
name|local_tail
operator|=
operator|&
name|ref
operator|->
name|next
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|get_local_heads
specifier|static
name|void
name|get_local_heads
parameter_list|(
name|void
parameter_list|)
block|{
name|local_tail
operator|=
operator|&
name|local_refs
expr_stmt|;
name|for_each_ref
argument_list|(
name|one_local_ref
argument_list|)
expr_stmt|;
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
name|nr_refspec
parameter_list|,
name|char
modifier|*
modifier|*
name|refspec
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
name|int
name|new_refs
decl_stmt|;
name|int
name|ret
init|=
literal|0
decl_stmt|;
comment|/* No funny business with the matcher */
name|remote_tail
operator|=
name|get_remote_heads
argument_list|(
name|in
argument_list|,
operator|&
name|remote_refs
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|get_local_heads
argument_list|()
expr_stmt|;
comment|/* match them up */
if|if
condition|(
operator|!
name|remote_tail
condition|)
name|remote_tail
operator|=
operator|&
name|remote_refs
expr_stmt|;
if|if
condition|(
name|match_refs
argument_list|(
name|local_refs
argument_list|,
name|remote_refs
argument_list|,
operator|&
name|remote_tail
argument_list|,
name|nr_refspec
argument_list|,
name|refspec
argument_list|,
name|send_all
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|remote_refs
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"No refs in common and none specified; doing nothing.\n"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
comment|/* 	 * Finally, tell the other end! 	 */
name|new_refs
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|ref
operator|=
name|remote_refs
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
if|if
condition|(
operator|!
name|ref
operator|->
name|peer_ref
condition|)
continue|continue;
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|,
name|ref
operator|->
name|peer_ref
operator|->
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
literal|"'%s': up-to-date\n"
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
continue|continue;
block|}
comment|/* This part determines what can overwrite what. 		 * The rules are: 		 * 		 * (0) you can always use --force or +A:B notation to 		 *     selectively force individual ref pairs. 		 * 		 * (1) if the old thing does not exist, it is OK. 		 * 		 * (2) if you do not have the old thing, you are not allowed 		 *     to overwrite it; you would not know what you are losing 		 *     otherwise. 		 * 		 * (3) if both new and old are commit-ish, and new is a 		 *     descendant of old, it is OK. 		 */
if|if
condition|(
operator|!
name|force_update
operator|&&
operator|!
name|is_zero_sha1
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|)
operator|&&
operator|!
name|ref
operator|->
name|force
condition|)
block|{
if|if
condition|(
operator|!
name|has_sha1_file
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"remote '%s' object %s does not "
literal|"exist on local"
argument_list|,
name|ref
operator|->
name|name
argument_list|,
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|)
argument_list|)
expr_stmt|;
name|ret
operator|=
operator|-
literal|2
expr_stmt|;
continue|continue;
block|}
comment|/* We assume that local is fsck-clean.  Otherwise 			 * you _could_ have an old tag which points at 			 * something you do not have, which may or may not 			 * be a commit. 			 */
if|if
condition|(
operator|!
name|ref_newer
argument_list|(
name|ref
operator|->
name|peer_ref
operator|->
name|new_sha1
argument_list|,
name|ref
operator|->
name|old_sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"remote ref '%s' is not a strict "
literal|"subset of local ref '%s'."
argument_list|,
name|ref
operator|->
name|name
argument_list|,
name|ref
operator|->
name|peer_ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|ret
operator|=
operator|-
literal|2
expr_stmt|;
continue|continue;
block|}
block|}
name|memcpy
argument_list|(
name|ref
operator|->
name|new_sha1
argument_list|,
name|ref
operator|->
name|peer_ref
operator|->
name|new_sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
name|is_zero_sha1
argument_list|(
name|ref
operator|->
name|new_sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"cannot happen anymore"
argument_list|)
expr_stmt|;
name|ret
operator|=
operator|-
literal|3
expr_stmt|;
continue|continue;
block|}
name|new_refs
operator|++
expr_stmt|;
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
literal|"updating '%s'"
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|ref
operator|->
name|name
argument_list|,
name|ref
operator|->
name|peer_ref
operator|->
name|name
argument_list|)
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|" using '%s'"
argument_list|,
name|ref
operator|->
name|peer_ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"\n  from %s\n  to   %s\n"
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
name|new_refs
condition|)
name|pack_objects
argument_list|(
name|out
argument_list|,
name|remote_refs
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|out
argument_list|)
expr_stmt|;
return|return
name|ret
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
name|setup_git_directory
argument_list|()
expr_stmt|;
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
operator|,
name|argv
operator|++
control|)
block|{
name|char
modifier|*
name|arg
init|=
operator|*
name|argv
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--all"
argument_list|)
condition|)
block|{
name|send_all
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--force"
argument_list|)
condition|)
block|{
name|force_update
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|send_pack_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|dest
condition|)
block|{
name|dest
operator|=
name|arg
expr_stmt|;
continue|continue;
block|}
name|heads
operator|=
name|argv
expr_stmt|;
name|nr_heads
operator|=
name|argc
operator|-
name|i
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
if|if
condition|(
name|heads
operator|&&
name|send_all
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

