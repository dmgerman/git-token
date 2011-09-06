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

begin_decl_stmt
DECL|variable|send_pack_usage
specifier|static
specifier|const
name|char
name|send_pack_usage
index|[]
init|=
literal|"git send-pack [--all | --mirror] [--dry-run] [--force] [--receive-pack=<git-receive-pack>] [--verbose] [--thin] [<host>:]<directory> [<ref>...]\n"
literal|"  --all and explicit<ref> specification are mutually exclusive."
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|args
specifier|static
name|struct
name|send_pack_args
name|args
decl_stmt|;
end_decl_stmt

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
return|return
name|error
argument_list|(
literal|"did not receive remote status"
argument_list|)
return|;
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
name|char
modifier|*
name|p
init|=
name|line
operator|+
name|strlen
argument_list|(
name|line
argument_list|)
operator|-
literal|1
decl_stmt|;
if|if
condition|(
operator|*
name|p
operator|==
literal|'\n'
condition|)
operator|*
name|p
operator|=
literal|'\0'
expr_stmt|;
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
literal|"ok "
argument_list|,
literal|3
argument_list|)
operator|&&
name|memcmp
argument_list|(
name|line
argument_list|,
literal|"ng "
argument_list|,
literal|3
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
name|line
index|[
name|strlen
argument_list|(
name|line
argument_list|)
operator|-
literal|1
index|]
operator|=
literal|'\0'
expr_stmt|;
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
DECL|function|print_helper_status
specifier|static
name|void
name|print_helper_status
parameter_list|(
name|struct
name|ref
modifier|*
name|ref
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
for|for
control|(
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
specifier|const
name|char
modifier|*
name|msg
init|=
name|NULL
decl_stmt|;
specifier|const
name|char
modifier|*
name|res
decl_stmt|;
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
name|res
operator|=
literal|"error"
expr_stmt|;
name|msg
operator|=
literal|"no match"
expr_stmt|;
break|break;
case|case
name|REF_STATUS_OK
case|:
name|res
operator|=
literal|"ok"
expr_stmt|;
break|break;
case|case
name|REF_STATUS_UPTODATE
case|:
name|res
operator|=
literal|"ok"
expr_stmt|;
name|msg
operator|=
literal|"up to date"
expr_stmt|;
break|break;
case|case
name|REF_STATUS_REJECT_NONFASTFORWARD
case|:
name|res
operator|=
literal|"error"
expr_stmt|;
name|msg
operator|=
literal|"non-fast forward"
expr_stmt|;
break|break;
case|case
name|REF_STATUS_REJECT_NODELETE
case|:
case|case
name|REF_STATUS_REMOTE_REJECT
case|:
name|res
operator|=
literal|"error"
expr_stmt|;
break|break;
case|case
name|REF_STATUS_EXPECTING_REPORT
case|:
default|default:
continue|continue;
block|}
name|strbuf_reset
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"%s %s"
argument_list|,
name|res
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|ref
operator|->
name|remote_status
condition|)
name|msg
operator|=
name|ref
operator|->
name|remote_status
expr_stmt|;
if|if
condition|(
name|msg
condition|)
block|{
name|strbuf_addch
argument_list|(
operator|&
name|buf
argument_list|,
literal|' '
argument_list|)
expr_stmt|;
name|quote_two_c_style
argument_list|(
operator|&
name|buf
argument_list|,
literal|""
argument_list|,
name|msg
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
name|strbuf_addch
argument_list|(
operator|&
name|buf
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
name|safe_write
argument_list|(
literal|1
argument_list|,
name|buf
operator|.
name|buf
argument_list|,
name|buf
operator|.
name|len
argument_list|)
expr_stmt|;
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
if|if
condition|(
operator|!
name|cmds_sent
operator|&&
operator|(
name|status_report
operator|||
name|use_sideband
operator|)
condition|)
block|{
name|packet_buf_write
argument_list|(
operator|&
name|req_buf
argument_list|,
literal|"%s %s %s%c%s%s"
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
name|safe_write
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
literal|"receive-pack: unable to fork off sideband demultiplexer"
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
return|return
operator|-
literal|1
return|;
block|}
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

begin_function
DECL|function|cmd_send_pack
name|int
name|cmd_send_pack
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
name|int
name|i
decl_stmt|,
name|nr_refspecs
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|refspecs
init|=
name|NULL
decl_stmt|;
specifier|const
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
specifier|const
name|char
modifier|*
name|dest
init|=
name|NULL
decl_stmt|;
name|int
name|fd
index|[
literal|2
index|]
decl_stmt|;
name|struct
name|child_process
modifier|*
name|conn
decl_stmt|;
name|struct
name|extra_have_objects
name|extra_have
decl_stmt|;
name|struct
name|ref
modifier|*
name|remote_refs
decl_stmt|,
modifier|*
name|local_refs
decl_stmt|;
name|int
name|ret
decl_stmt|;
name|int
name|helper_status
init|=
literal|0
decl_stmt|;
name|int
name|send_all
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|receivepack
init|=
literal|"git-receive-pack"
decl_stmt|;
name|int
name|flags
decl_stmt|;
name|int
name|nonfastforward
init|=
literal|0
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
operator|,
name|argv
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
literal|"--dry-run"
argument_list|)
condition|)
block|{
name|args
operator|.
name|dry_run
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
literal|"--mirror"
argument_list|)
condition|)
block|{
name|args
operator|.
name|send_mirror
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
name|args
operator|.
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
name|args
operator|.
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
name|args
operator|.
name|use_thin_pack
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
literal|"--stateless-rpc"
argument_list|)
condition|)
block|{
name|args
operator|.
name|stateless_rpc
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
literal|"--helper-status"
argument_list|)
condition|)
block|{
name|helper_status
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
name|refspecs
operator|=
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
name|argv
expr_stmt|;
name|nr_refspecs
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
comment|/* 	 * --all and --mirror are incompatible; neither makes sense 	 * with any refspecs. 	 */
if|if
condition|(
operator|(
name|refspecs
operator|&&
operator|(
name|send_all
operator|||
name|args
operator|.
name|send_mirror
operator|)
operator|)
operator|||
operator|(
name|send_all
operator|&&
name|args
operator|.
name|send_mirror
operator|)
condition|)
name|usage
argument_list|(
name|send_pack_usage
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
name|remote_has_url
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
if|if
condition|(
name|args
operator|.
name|stateless_rpc
condition|)
block|{
name|conn
operator|=
name|NULL
expr_stmt|;
name|fd
index|[
literal|0
index|]
operator|=
literal|0
expr_stmt|;
name|fd
index|[
literal|1
index|]
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|conn
operator|=
name|git_connect
argument_list|(
name|fd
argument_list|,
name|dest
argument_list|,
name|receivepack
argument_list|,
name|args
operator|.
name|verbose
condition|?
name|CONNECT_VERBOSE
else|:
literal|0
argument_list|)
expr_stmt|;
block|}
name|memset
argument_list|(
operator|&
name|extra_have
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|extra_have
argument_list|)
argument_list|)
expr_stmt|;
name|get_remote_heads
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
operator|&
name|remote_refs
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
name|REF_NORMAL
argument_list|,
operator|&
name|extra_have
argument_list|)
expr_stmt|;
name|transport_verify_remote_names
argument_list|(
name|nr_refspecs
argument_list|,
name|refspecs
argument_list|)
expr_stmt|;
name|local_refs
operator|=
name|get_local_heads
argument_list|()
expr_stmt|;
name|flags
operator|=
name|MATCH_REFS_NONE
expr_stmt|;
if|if
condition|(
name|send_all
condition|)
name|flags
operator||=
name|MATCH_REFS_ALL
expr_stmt|;
if|if
condition|(
name|args
operator|.
name|send_mirror
condition|)
name|flags
operator||=
name|MATCH_REFS_MIRROR
expr_stmt|;
comment|/* match them up */
if|if
condition|(
name|match_refs
argument_list|(
name|local_refs
argument_list|,
operator|&
name|remote_refs
argument_list|,
name|nr_refspecs
argument_list|,
name|refspecs
argument_list|,
name|flags
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|set_ref_status_for_push
argument_list|(
name|remote_refs
argument_list|,
name|args
operator|.
name|send_mirror
argument_list|,
name|args
operator|.
name|force_update
argument_list|)
expr_stmt|;
name|ret
operator|=
name|send_pack
argument_list|(
operator|&
name|args
argument_list|,
name|fd
argument_list|,
name|conn
argument_list|,
name|remote_refs
argument_list|,
operator|&
name|extra_have
argument_list|)
expr_stmt|;
if|if
condition|(
name|helper_status
condition|)
name|print_helper_status
argument_list|(
name|remote_refs
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
name|close
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|ret
operator||=
name|finish_connect
argument_list|(
name|conn
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|helper_status
condition|)
name|transport_print_push_status
argument_list|(
name|dest
argument_list|,
name|remote_refs
argument_list|,
name|args
operator|.
name|verbose
argument_list|,
literal|0
argument_list|,
operator|&
name|nonfastforward
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|args
operator|.
name|dry_run
operator|&&
name|remote
condition|)
block|{
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
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
name|transport_update_tracking_ref
argument_list|(
name|remote
argument_list|,
name|ref
argument_list|,
name|args
operator|.
name|verbose
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|ret
operator|&&
operator|!
name|transport_refs_pushed
argument_list|(
name|remote_refs
argument_list|)
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

end_unit

