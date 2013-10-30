begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
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
file|"remote.h"
end_include

begin_include
include|#
directive|include
file|"connect.h"
end_include

begin_include
include|#
directive|include
file|"send-pack.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_include
include|#
directive|include
file|"transport.h"
end_include

begin_include
include|#
directive|include
file|"version.h"
end_include

begin_function
DECL|function|feed_object
specifier|static
name|int
name|feed_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|fd
parameter_list|,
name|int
name|negative
parameter_list|)
block|{
name|char
name|buf
index|[
literal|42
index|]
decl_stmt|;
if|if
condition|(
name|negative
operator|&&
operator|!
name|has_sha1_file
argument_list|(
name|sha1
argument_list|)
condition|)
return|return
literal|1
return|;
name|memcpy
argument_list|(
name|buf
operator|+
name|negative
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
literal|40
argument_list|)
expr_stmt|;
if|if
condition|(
name|negative
condition|)
name|buf
index|[
literal|0
index|]
operator|=
literal|'^'
expr_stmt|;
name|buf
index|[
literal|40
operator|+
name|negative
index|]
operator|=
literal|'\n'
expr_stmt|;
return|return
name|write_or_whine
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
literal|41
operator|+
name|negative
argument_list|,
literal|"send-pack: send refs"
argument_list|)
return|;
block|}
end_function

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
parameter_list|,
name|struct
name|extra_have_objects
modifier|*
name|extra
parameter_list|,
name|struct
name|send_pack_args
modifier|*
name|args
parameter_list|)
block|{
comment|/* 	 * The child becomes pack-objects --revs; we feed 	 * the revision parameters to it via its stdin and 	 * let its stdout go back to the other end. 	 */
specifier|const
name|char
modifier|*
name|argv
index|[]
init|=
block|{
literal|"pack-objects"
block|,
literal|"--all-progress-implied"
block|,
literal|"--revs"
block|,
literal|"--stdout"
block|,
name|NULL
block|,
name|NULL
block|,
name|NULL
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
name|int
name|i
decl_stmt|;
name|i
operator|=
literal|4
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|use_thin_pack
condition|)
name|argv
index|[
name|i
operator|++
index|]
operator|=
literal|"--thin"
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|use_ofs_delta
condition|)
name|argv
index|[
name|i
operator|++
index|]
operator|=
literal|"--delta-base-offset"
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|quiet
operator|||
operator|!
name|args
operator|->
name|progress
condition|)
name|argv
index|[
name|i
operator|++
index|]
operator|=
literal|"-q"
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|progress
condition|)
name|argv
index|[
name|i
operator|++
index|]
operator|=
literal|"--progress"
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
name|argv
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
name|args
operator|->
name|stateless_rpc
condition|?
operator|-
literal|1
else|:
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
name|die_errno
argument_list|(
literal|"git pack-objects failed"
argument_list|)
expr_stmt|;
comment|/* 	 * We feed the pack-objects we just spawned with revision 	 * parameters by writing to the pipe. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|extra
operator|->
name|nr
condition|;
name|i
operator|++
control|)
if|if
condition|(
operator|!
name|feed_object
argument_list|(
name|extra
operator|->
name|array
index|[
name|i
index|]
argument_list|,
name|po
operator|.
name|in
argument_list|,
literal|1
argument_list|)
condition|)
break|break;
while|while
condition|(
name|refs
condition|)
block|{
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
operator|!
name|feed_object
argument_list|(
name|refs
operator|->
name|old_sha1
argument_list|,
name|po
operator|.
name|in
argument_list|,
literal|1
argument_list|)
condition|)
break|break;
if|if
condition|(
operator|!
name|is_null_sha1
argument_list|(
name|refs
operator|->
name|new_sha1
argument_list|)
operator|&&
operator|!
name|feed_object
argument_list|(
name|refs
operator|->
name|new_sha1
argument_list|,
name|po
operator|.
name|in
argument_list|,
literal|0
argument_list|)
condition|)
break|break;
name|refs
operator|=
name|refs
operator|->
name|next
expr_stmt|;
block|}
name|close
argument_list|(
name|po
operator|.
name|in
argument_list|)
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|stateless_rpc
condition|)
block|{
name|char
modifier|*
name|buf
init|=
name|xmalloc
argument_list|(
name|LARGE_PACKET_MAX
argument_list|)
decl_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|ssize_t
name|n
init|=
name|xread
argument_list|(
name|po
operator|.
name|out
argument_list|,
name|buf
argument_list|,
name|LARGE_PACKET_MAX
argument_list|)
decl_stmt|;
if|if
condition|(
name|n
operator|<=
literal|0
condition|)
break|break;
name|send_sideband
argument_list|(
name|fd
argument_list|,
operator|-
literal|1
argument_list|,
name|buf
argument_list|,
name|n
argument_list|,
name|LARGE_PACKET_MAX
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|po
operator|.
name|out
argument_list|)
expr_stmt|;
name|po
operator|.
name|out
operator|=
operator|-
literal|1
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
operator|-
literal|1
return|;
return|return
literal|0
return|;
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
parameter_list|,
name|struct
name|ref
modifier|*
name|refs
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|hint
decl_stmt|;
name|int
name|ret
init|=
literal|0
decl_stmt|;
name|char
modifier|*
name|line
init|=
name|packet_read_line
argument_list|(
name|in
argument_list|,
name|NULL
argument_list|)
decl_stmt|;
if|if
condition|(
name|prefixcmp
argument_list|(
name|line
argument_list|,
literal|"unpack "
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"did not receive remote status"
argument_list|)
return|;
if|if
condition|(
name|strcmp
argument_list|(
name|line
argument_list|,
literal|"unpack ok"
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"unpack failed: %s"
argument_list|,
name|line
operator|+
literal|7
argument_list|)
expr_stmt|;
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
block|}
name|hint
operator|=
name|NULL
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|char
modifier|*
name|refname
decl_stmt|;
name|char
modifier|*
name|msg
decl_stmt|;
name|line
operator|=
name|packet_read_line
argument_list|(
name|in
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|line
condition|)
break|break;
if|if
condition|(
name|prefixcmp
argument_list|(
name|line
argument_list|,
literal|"ok "
argument_list|)
operator|&&
name|prefixcmp
argument_list|(
name|line
argument_list|,
literal|"ng "
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"invalid ref status from remote: %s"
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
name|refname
operator|=
name|line
operator|+
literal|3
expr_stmt|;
name|msg
operator|=
name|strchr
argument_list|(
name|refname
argument_list|,
literal|' '
argument_list|)
expr_stmt|;
if|if
condition|(
name|msg
condition|)
operator|*
name|msg
operator|++
operator|=
literal|'\0'
expr_stmt|;
comment|/* first try searching at our hint, falling back to all refs */
if|if
condition|(
name|hint
condition|)
name|hint
operator|=
name|find_ref_by_name
argument_list|(
name|hint
argument_list|,
name|refname
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|hint
condition|)
name|hint
operator|=
name|find_ref_by_name
argument_list|(
name|refs
argument_list|,
name|refname
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|hint
condition|)
block|{
name|warning
argument_list|(
literal|"remote reported status on unknown ref: %s"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|hint
operator|->
name|status
operator|!=
name|REF_STATUS_EXPECTING_REPORT
condition|)
block|{
name|warning
argument_list|(
literal|"remote reported status on unexpected ref: %s"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|line
index|[
literal|0
index|]
operator|==
literal|'o'
operator|&&
name|line
index|[
literal|1
index|]
operator|==
literal|'k'
condition|)
name|hint
operator|->
name|status
operator|=
name|REF_STATUS_OK
expr_stmt|;
else|else
block|{
name|hint
operator|->
name|status
operator|=
name|REF_STATUS_REMOTE_REJECT
expr_stmt|;
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|msg
condition|)
name|hint
operator|->
name|remote_status
operator|=
name|xstrdup
argument_list|(
name|msg
argument_list|)
expr_stmt|;
comment|/* start our next search from the next ref */
name|hint
operator|=
name|hint
operator|->
name|next
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|sideband_demux
specifier|static
name|int
name|sideband_demux
parameter_list|(
name|int
name|in
parameter_list|,
name|int
name|out
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|int
modifier|*
name|fd
init|=
name|data
decl_stmt|,
name|ret
decl_stmt|;
ifdef|#
directive|ifdef
name|NO_PTHREADS
name|close
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|ret
operator|=
name|recv_sideband
argument_list|(
literal|"send-pack"
argument_list|,
name|fd
index|[
literal|0
index|]
argument_list|,
name|out
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
DECL|function|send_pack
name|int
name|send_pack
parameter_list|(
name|struct
name|send_pack_args
modifier|*
name|args
parameter_list|,
name|int
name|fd
index|[]
parameter_list|,
name|struct
name|child_process
modifier|*
name|conn
parameter_list|,
name|struct
name|ref
modifier|*
name|remote_refs
parameter_list|,
name|struct
name|extra_have_objects
modifier|*
name|extra_have
parameter_list|)
block|{
name|int
name|in
init|=
name|fd
index|[
literal|0
index|]
decl_stmt|;
name|int
name|out
init|=
name|fd
index|[
literal|1
index|]
decl_stmt|;
name|struct
name|strbuf
name|req_buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
name|int
name|new_refs
decl_stmt|;
name|int
name|allow_deleting_refs
init|=
literal|0
decl_stmt|;
name|int
name|status_report
init|=
literal|0
decl_stmt|;
name|int
name|use_sideband
init|=
literal|0
decl_stmt|;
name|int
name|quiet_supported
init|=
literal|0
decl_stmt|;
name|int
name|agent_supported
init|=
literal|0
decl_stmt|;
name|unsigned
name|cmds_sent
init|=
literal|0
decl_stmt|;
name|int
name|ret
decl_stmt|;
name|struct
name|async
name|demux
decl_stmt|;
comment|/* Does the other end support the reporting? */
if|if
condition|(
name|server_supports
argument_list|(
literal|"report-status"
argument_list|)
condition|)
name|status_report
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
if|if
condition|(
name|server_supports
argument_list|(
literal|"ofs-delta"
argument_list|)
condition|)
name|args
operator|->
name|use_ofs_delta
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|server_supports
argument_list|(
literal|"side-band-64k"
argument_list|)
condition|)
name|use_sideband
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|server_supports
argument_list|(
literal|"quiet"
argument_list|)
condition|)
name|quiet_supported
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|server_supports
argument_list|(
literal|"agent"
argument_list|)
condition|)
name|agent_supported
operator|=
literal|1
expr_stmt|;
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
literal|"Perhaps you should specify a branch such as 'master'.\n"
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
if|if
condition|(
operator|!
name|ref
operator|->
name|peer_ref
operator|&&
operator|!
name|args
operator|->
name|send_mirror
condition|)
continue|continue;
comment|/* Check for statuses set by set_ref_status_for_push() */
switch|switch
condition|(
name|ref
operator|->
name|status
condition|)
block|{
case|case
name|REF_STATUS_REJECT_NONFASTFORWARD
case|:
case|case
name|REF_STATUS_REJECT_ALREADY_EXISTS
case|:
case|case
name|REF_STATUS_REJECT_FETCH_FIRST
case|:
case|case
name|REF_STATUS_REJECT_NEEDS_FORCE
case|:
case|case
name|REF_STATUS_REJECT_STALE
case|:
case|case
name|REF_STATUS_UPTODATE
case|:
continue|continue;
default|default:
empty_stmt|;
comment|/* do nothing */
block|}
if|if
condition|(
name|ref
operator|->
name|deletion
operator|&&
operator|!
name|allow_deleting_refs
condition|)
block|{
name|ref
operator|->
name|status
operator|=
name|REF_STATUS_REJECT_NODELETE
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|ref
operator|->
name|deletion
condition|)
name|new_refs
operator|++
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|dry_run
condition|)
block|{
name|ref
operator|->
name|status
operator|=
name|REF_STATUS_OK
expr_stmt|;
block|}
else|else
block|{
name|char
modifier|*
name|old_hex
init|=
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|)
decl_stmt|;
name|char
modifier|*
name|new_hex
init|=
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|new_sha1
argument_list|)
decl_stmt|;
name|int
name|quiet
init|=
name|quiet_supported
operator|&&
operator|(
name|args
operator|->
name|quiet
operator|||
operator|!
name|args
operator|->
name|progress
operator|)
decl_stmt|;
if|if
condition|(
operator|!
name|cmds_sent
operator|&&
operator|(
name|status_report
operator|||
name|use_sideband
operator|||
name|quiet
operator|||
name|agent_supported
operator|)
condition|)
block|{
name|packet_buf_write
argument_list|(
operator|&
name|req_buf
argument_list|,
literal|"%s %s %s%c%s%s%s%s%s"
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
name|status_report
condition|?
literal|" report-status"
else|:
literal|""
argument_list|,
name|use_sideband
condition|?
literal|" side-band-64k"
else|:
literal|""
argument_list|,
name|quiet
condition|?
literal|" quiet"
else|:
literal|""
argument_list|,
name|agent_supported
condition|?
literal|" agent="
else|:
literal|""
argument_list|,
name|agent_supported
condition|?
name|git_user_agent_sanitized
argument_list|()
else|:
literal|""
argument_list|)
expr_stmt|;
block|}
else|else
name|packet_buf_write
argument_list|(
operator|&
name|req_buf
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
name|ref
operator|->
name|status
operator|=
name|status_report
condition|?
name|REF_STATUS_EXPECTING_REPORT
else|:
name|REF_STATUS_OK
expr_stmt|;
name|cmds_sent
operator|++
expr_stmt|;
block|}
block|}
if|if
condition|(
name|args
operator|->
name|stateless_rpc
condition|)
block|{
if|if
condition|(
operator|!
name|args
operator|->
name|dry_run
operator|&&
name|cmds_sent
condition|)
block|{
name|packet_buf_flush
argument_list|(
operator|&
name|req_buf
argument_list|)
expr_stmt|;
name|send_sideband
argument_list|(
name|out
argument_list|,
operator|-
literal|1
argument_list|,
name|req_buf
operator|.
name|buf
argument_list|,
name|req_buf
operator|.
name|len
argument_list|,
name|LARGE_PACKET_MAX
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
name|write_or_die
argument_list|(
name|out
argument_list|,
name|req_buf
operator|.
name|buf
argument_list|,
name|req_buf
operator|.
name|len
argument_list|)
expr_stmt|;
name|packet_flush
argument_list|(
name|out
argument_list|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|req_buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|use_sideband
operator|&&
name|cmds_sent
condition|)
block|{
name|memset
argument_list|(
operator|&
name|demux
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|demux
argument_list|)
argument_list|)
expr_stmt|;
name|demux
operator|.
name|proc
operator|=
name|sideband_demux
expr_stmt|;
name|demux
operator|.
name|data
operator|=
name|fd
expr_stmt|;
name|demux
operator|.
name|out
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|start_async
argument_list|(
operator|&
name|demux
argument_list|)
condition|)
name|die
argument_list|(
literal|"send-pack: unable to fork off sideband demultiplexer"
argument_list|)
expr_stmt|;
name|in
operator|=
name|demux
operator|.
name|out
expr_stmt|;
block|}
if|if
condition|(
name|new_refs
operator|&&
name|cmds_sent
condition|)
block|{
if|if
condition|(
name|pack_objects
argument_list|(
name|out
argument_list|,
name|remote_refs
argument_list|,
name|extra_have
argument_list|,
name|args
argument_list|)
operator|<
literal|0
condition|)
block|{
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
name|ref
operator|->
name|status
operator|=
name|REF_STATUS_NONE
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|stateless_rpc
condition|)
name|close
argument_list|(
name|out
argument_list|)
expr_stmt|;
if|if
condition|(
name|git_connection_is_socket
argument_list|(
name|conn
argument_list|)
condition|)
name|shutdown
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|SHUT_WR
argument_list|)
expr_stmt|;
if|if
condition|(
name|use_sideband
condition|)
name|finish_async
argument_list|(
operator|&
name|demux
argument_list|)
expr_stmt|;
name|fd
index|[
literal|1
index|]
operator|=
operator|-
literal|1
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
operator|!
name|args
operator|->
name|stateless_rpc
condition|)
comment|/* Closed by pack_objects() via start_command() */
name|fd
index|[
literal|1
index|]
operator|=
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|args
operator|->
name|stateless_rpc
operator|&&
name|cmds_sent
condition|)
name|packet_flush
argument_list|(
name|out
argument_list|)
expr_stmt|;
if|if
condition|(
name|status_report
operator|&&
name|cmds_sent
condition|)
name|ret
operator|=
name|receive_status
argument_list|(
name|in
argument_list|,
name|remote_refs
argument_list|)
expr_stmt|;
else|else
name|ret
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|stateless_rpc
condition|)
name|packet_flush
argument_list|(
name|out
argument_list|)
expr_stmt|;
if|if
condition|(
name|use_sideband
operator|&&
name|cmds_sent
condition|)
block|{
if|if
condition|(
name|finish_async
argument_list|(
operator|&
name|demux
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"error in sideband demultiplexer"
argument_list|)
expr_stmt|;
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
block|}
name|close
argument_list|(
name|demux
operator|.
name|out
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|ret
operator|<
literal|0
condition|)
return|return
name|ret
return|;
if|if
condition|(
name|args
operator|->
name|porcelain
condition|)
return|return
literal|0
return|;
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
switch|switch
condition|(
name|ref
operator|->
name|status
condition|)
block|{
case|case
name|REF_STATUS_NONE
case|:
case|case
name|REF_STATUS_UPTODATE
case|:
case|case
name|REF_STATUS_OK
case|:
break|break;
default|default:
return|return
operator|-
literal|1
return|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

