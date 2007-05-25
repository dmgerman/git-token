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

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"remote.h"
end_include

begin_decl_stmt
DECL|variable|send_pack_usage
specifier|static
specifier|const
name|char
name|send_pack_usage
index|[]
init|=
literal|"git-send-pack [--all] [--force] [--receive-pack=<git-receive-pack>] [--verbose] [--thin] [<host>:]<directory> [<ref>...]\n"
literal|"  --all and explicit<ref> specification are mutually exclusive."
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|receivepack
specifier|static
specifier|const
name|char
modifier|*
name|receivepack
init|=
literal|"git-receive-pack"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|verbose
specifier|static
name|int
name|verbose
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|send_all
specifier|static
name|int
name|send_all
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|force_update
specifier|static
name|int
name|force_update
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|use_thin_pack
specifier|static
name|int
name|use_thin_pack
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Make a pack stream and spit it out into file descriptor fd  */
end_comment

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
comment|/* 	 * The child becomes pack-objects --revs; we feed 	 * the revision parameters to it via its stdin and 	 * let its stdout go back to the other end. 	 */
specifier|const
name|char
modifier|*
name|args
index|[]
init|=
block|{
literal|"pack-objects"
block|,
literal|"--all-progress"
block|,
literal|"--revs"
block|,
literal|"--stdout"
block|,
name|NULL
block|,
name|NULL
block|, 	}
decl_stmt|;
name|struct
name|child_process
name|po
decl_stmt|;
if|if
condition|(
name|use_thin_pack
condition|)
name|args
index|[
literal|4
index|]
operator|=
literal|"--thin"
expr_stmt|;
name|memset
argument_list|(
operator|&
name|po
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|po
argument_list|)
argument_list|)
expr_stmt|;
name|po
operator|.
name|argv
operator|=
name|args
expr_stmt|;
name|po
operator|.
name|in
operator|=
operator|-
literal|1
expr_stmt|;
name|po
operator|.
name|out
operator|=
name|fd
expr_stmt|;
name|po
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
name|po
argument_list|)
condition|)
name|die
argument_list|(
literal|"git-pack-objects failed (%s)"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
comment|/* 	 * We feed the pack-objects we just spawned with revision 	 * parameters by writing to the pipe. 	 */
while|while
condition|(
name|refs
condition|)
block|{
name|char
name|buf
index|[
literal|42
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|is_null_sha1
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
name|memcpy
argument_list|(
name|buf
operator|+
literal|1
argument_list|,
name|sha1_to_hex
argument_list|(
name|refs
operator|->
name|old_sha1
argument_list|)
argument_list|,
literal|40
argument_list|)
expr_stmt|;
name|buf
index|[
literal|0
index|]
operator|=
literal|'^'
expr_stmt|;
name|buf
index|[
literal|41
index|]
operator|=
literal|'\n'
expr_stmt|;
if|if
condition|(
operator|!
name|write_or_whine
argument_list|(
name|po
operator|.
name|in
argument_list|,
name|buf
argument_list|,
literal|42
argument_list|,
literal|"send-pack: send refs"
argument_list|)
condition|)
break|break;
block|}
if|if
condition|(
operator|!
name|is_null_sha1
argument_list|(
name|refs
operator|->
name|new_sha1
argument_list|)
condition|)
block|{
name|memcpy
argument_list|(
name|buf
argument_list|,
name|sha1_to_hex
argument_list|(
name|refs
operator|->
name|new_sha1
argument_list|)
argument_list|,
literal|40
argument_list|)
expr_stmt|;
name|buf
index|[
literal|40
index|]
operator|=
literal|'\n'
expr_stmt|;
if|if
condition|(
operator|!
name|write_or_whine
argument_list|(
name|po
operator|.
name|in
argument_list|,
name|buf
argument_list|,
literal|41
argument_list|,
literal|"send-pack: send refs"
argument_list|)
condition|)
break|break;
block|}
name|refs
operator|=
name|refs
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
name|finish_command
argument_list|(
operator|&
name|po
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"pack-objects died with strange error"
argument_list|)
return|;
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
name|OBJ_COMMIT
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
name|OBJ_COMMIT
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
parameter_list|,
name|int
name|flag
parameter_list|,
name|void
modifier|*
name|cb_data
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
name|hashcpy
argument_list|(
name|ref
operator|->
name|new_sha1
argument_list|,
name|sha1
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
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|receive_status
specifier|static
name|int
name|receive_status
parameter_list|(
name|int
name|in
parameter_list|)
block|{
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
name|int
name|ret
init|=
literal|0
decl_stmt|;
name|int
name|len
init|=
name|packet_read_line
argument_list|(
name|in
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
name|len
operator|<
literal|10
operator|||
name|memcmp
argument_list|(
name|line
argument_list|,
literal|"unpack "
argument_list|,
literal|7
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"did not receive status back\n"
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|memcmp
argument_list|(
name|line
argument_list|,
literal|"unpack ok\n"
argument_list|,
literal|10
argument_list|)
condition|)
block|{
name|fputs
argument_list|(
name|line
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
block|}
while|while
condition|(
literal|1
condition|)
block|{
name|len
operator|=
name|packet_read_line
argument_list|(
name|in
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
name|len
operator|<
literal|3
operator|||
operator|(
name|memcmp
argument_list|(
name|line
argument_list|,
literal|"ok"
argument_list|,
literal|2
argument_list|)
operator|&&
name|memcmp
argument_list|(
name|line
argument_list|,
literal|"ng"
argument_list|,
literal|2
argument_list|)
operator|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"protocol error: %s\n"
argument_list|,
name|line
argument_list|)
expr_stmt|;
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|line
argument_list|,
literal|"ok"
argument_list|,
literal|2
argument_list|)
condition|)
continue|continue;
name|fputs
argument_list|(
name|line
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
block|}
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
name|struct
name|remote
modifier|*
name|remote
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
name|int
name|ask_for_status_report
init|=
literal|0
decl_stmt|;
name|int
name|allow_deleting_refs
init|=
literal|0
decl_stmt|;
name|int
name|expect_status_report
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
name|REF_NORMAL
argument_list|)
expr_stmt|;
name|get_local_heads
argument_list|()
expr_stmt|;
comment|/* Does the other end support the reporting? */
if|if
condition|(
name|server_supports
argument_list|(
literal|"report-status"
argument_list|)
condition|)
name|ask_for_status_report
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|server_supports
argument_list|(
literal|"delete-refs"
argument_list|)
condition|)
name|allow_deleting_refs
operator|=
literal|1
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
name|int
name|will_delete_ref
decl_stmt|;
if|if
condition|(
operator|!
name|ref
operator|->
name|peer_ref
condition|)
continue|continue;
name|will_delete_ref
operator|=
name|is_null_sha1
argument_list|(
name|ref
operator|->
name|peer_ref
operator|->
name|new_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|will_delete_ref
operator|&&
operator|!
name|allow_deleting_refs
condition|)
block|{
name|error
argument_list|(
literal|"remote does not support deleting refs"
argument_list|)
expr_stmt|;
name|ret
operator|=
operator|-
literal|2
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|will_delete_ref
operator|&&
operator|!
name|hashcmp
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
argument_list|)
condition|)
block|{
if|if
condition|(
name|verbose
condition|)
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
comment|/* This part determines what can overwrite what. 		 * The rules are: 		 * 		 * (0) you can always use --force or +A:B notation to 		 *     selectively force individual ref pairs. 		 * 		 * (1) if the old thing does not exist, it is OK. 		 * 		 * (2) if you do not have the old thing, you are not allowed 		 *     to overwrite it; you would not know what you are losing 		 *     otherwise. 		 * 		 * (3) if both new and old are commit-ish, and new is a 		 *     descendant of old, it is OK. 		 * 		 * (4) regardless of all of the above, removing :B is 		 *     always allowed. 		 */
if|if
condition|(
operator|!
name|force_update
operator|&&
operator|!
name|will_delete_ref
operator|&&
operator|!
name|is_null_sha1
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
operator|||
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
comment|/* We do not have the remote ref, or 				 * we know that the remote ref is not 				 * an ancestor of what we are trying to 				 * push.  Either way this can be losing 				 * commits at the remote end and likely 				 * we were not up to date to begin with. 				 */
name|error
argument_list|(
literal|"remote '%s' is not a strict "
literal|"subset of local ref '%s'. "
literal|"maybe you are not up-to-date and "
literal|"need to pull first?"
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
name|hashcpy
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
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|will_delete_ref
condition|)
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
if|if
condition|(
name|ask_for_status_report
condition|)
block|{
name|packet_write
argument_list|(
name|out
argument_list|,
literal|"%s %s %s%c%s"
argument_list|,
name|old_hex
argument_list|,
name|new_hex
argument_list|,
name|ref
operator|->
name|name
argument_list|,
literal|0
argument_list|,
literal|"report-status"
argument_list|)
expr_stmt|;
name|ask_for_status_report
operator|=
literal|0
expr_stmt|;
name|expect_status_report
operator|=
literal|1
expr_stmt|;
block|}
else|else
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
if|if
condition|(
name|will_delete_ref
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"deleting '%s'\n"
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
else|else
block|{
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
if|if
condition|(
name|remote
condition|)
block|{
name|struct
name|refspec
name|rs
decl_stmt|;
name|rs
operator|.
name|src
operator|=
name|ref
operator|->
name|name
expr_stmt|;
name|remote_find_tracking
argument_list|(
name|remote
argument_list|,
operator|&
name|rs
argument_list|)
expr_stmt|;
if|if
condition|(
name|rs
operator|.
name|dst
condition|)
block|{
name|struct
name|ref_lock
modifier|*
name|lock
decl_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|" Also local %s\n"
argument_list|,
name|rs
operator|.
name|dst
argument_list|)
expr_stmt|;
if|if
condition|(
name|will_delete_ref
condition|)
block|{
if|if
condition|(
name|delete_ref
argument_list|(
name|rs
operator|.
name|dst
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"Failed to delete"
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
name|lock
operator|=
name|lock_any_ref_for_update
argument_list|(
name|rs
operator|.
name|dst
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lock
condition|)
name|error
argument_list|(
literal|"Failed to lock"
argument_list|)
expr_stmt|;
else|else
name|write_ref_sha1
argument_list|(
name|lock
argument_list|,
name|ref
operator|->
name|new_sha1
argument_list|,
literal|"update by push"
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|rs
operator|.
name|dst
argument_list|)
expr_stmt|;
block|}
block|}
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
name|ret
operator|=
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
if|if
condition|(
name|expect_status_report
condition|)
block|{
if|if
condition|(
name|receive_status
argument_list|(
name|in
argument_list|)
condition|)
name|ret
operator|=
operator|-
literal|4
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|new_refs
operator|&&
name|ret
operator|==
literal|0
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Everything up-to-date\n"
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|verify_remote_names
specifier|static
name|void
name|verify_remote_names
parameter_list|(
name|int
name|nr_heads
parameter_list|,
name|char
modifier|*
modifier|*
name|heads
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
name|nr_heads
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|remote
init|=
name|strchr
argument_list|(
name|heads
index|[
name|i
index|]
argument_list|,
literal|':'
argument_list|)
decl_stmt|;
name|remote
operator|=
name|remote
condition|?
operator|(
name|remote
operator|+
literal|1
operator|)
else|:
name|heads
index|[
name|i
index|]
expr_stmt|;
switch|switch
condition|(
name|check_ref_format
argument_list|(
name|remote
argument_list|)
condition|)
block|{
case|case
literal|0
case|:
comment|/* ok */
case|case
operator|-
literal|2
case|:
comment|/* ok but a single level -- that is fine for 			  * a match pattern. 			  */
case|case
operator|-
literal|3
case|:
comment|/* ok but ends with a pattern-match character */
continue|continue;
block|}
name|die
argument_list|(
literal|"remote part of refspec is not a valid name in %s"
argument_list|,
name|heads
index|[
name|i
index|]
argument_list|)
expr_stmt|;
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
name|char
modifier|*
name|remote_name
init|=
name|NULL
decl_stmt|;
name|struct
name|remote
modifier|*
name|remote
init|=
name|NULL
decl_stmt|;
name|setup_git_directory
argument_list|()
expr_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
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
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--receive-pack="
argument_list|)
condition|)
block|{
name|receivepack
operator|=
name|arg
operator|+
literal|15
expr_stmt|;
continue|continue;
block|}
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
name|receivepack
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
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--remote="
argument_list|)
condition|)
block|{
name|remote_name
operator|=
name|arg
operator|+
literal|9
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--verbose"
argument_list|)
condition|)
block|{
name|verbose
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
literal|"--thin"
argument_list|)
condition|)
block|{
name|use_thin_pack
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
name|verify_remote_names
argument_list|(
name|nr_heads
argument_list|,
name|heads
argument_list|)
expr_stmt|;
if|if
condition|(
name|remote_name
condition|)
block|{
name|remote
operator|=
name|remote_get
argument_list|(
name|remote_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|remote_has_uri
argument_list|(
name|remote
argument_list|,
name|dest
argument_list|)
condition|)
block|{
name|die
argument_list|(
literal|"Destination %s is not a uri for %s"
argument_list|,
name|dest
argument_list|,
name|remote_name
argument_list|)
expr_stmt|;
block|}
block|}
name|pid
operator|=
name|git_connect
argument_list|(
name|fd
argument_list|,
name|dest
argument_list|,
name|receivepack
argument_list|,
name|verbose
condition|?
name|CONNECT_VERBOSE
else|:
literal|0
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
name|remote
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
name|ret
operator||=
name|finish_connect
argument_list|(
name|pid
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|ret
return|;
block|}
end_function

end_unit

