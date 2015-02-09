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

begin_include
include|#
directive|include
file|"sha1-array.h"
end_include

begin_include
include|#
directive|include
file|"gpg-interface.h"
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
name|sha1_array
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
block|,
name|NULL
block|, 	}
decl_stmt|;
name|struct
name|child_process
name|po
init|=
name|CHILD_PROCESS_INIT
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
if|if
condition|(
name|is_repository_shallow
argument_list|()
condition|)
name|argv
index|[
name|i
operator|++
index|]
operator|=
literal|"--shallow"
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
name|sha1
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
operator|!
name|starts_with
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
operator|!
name|starts_with
argument_list|(
name|line
argument_list|,
literal|"ok "
argument_list|)
operator|&&
operator|!
name|starts_with
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
DECL|function|advertise_shallow_grafts_cb
specifier|static
name|int
name|advertise_shallow_grafts_cb
parameter_list|(
specifier|const
name|struct
name|commit_graft
modifier|*
name|graft
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
block|{
name|struct
name|strbuf
modifier|*
name|sb
init|=
name|cb
decl_stmt|;
if|if
condition|(
name|graft
operator|->
name|nr_parent
operator|==
operator|-
literal|1
condition|)
name|packet_buf_write
argument_list|(
name|sb
argument_list|,
literal|"shallow %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|graft
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|advertise_shallow_grafts_buf
specifier|static
name|void
name|advertise_shallow_grafts_buf
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
block|{
if|if
condition|(
operator|!
name|is_repository_shallow
argument_list|()
condition|)
return|return;
name|for_each_commit_graft
argument_list|(
name|advertise_shallow_grafts_cb
argument_list|,
name|sb
argument_list|)
expr_stmt|;
block|}
end_function

begin_define
DECL|macro|CHECK_REF_NO_PUSH
define|#
directive|define
name|CHECK_REF_NO_PUSH
value|-1
end_define

begin_define
DECL|macro|CHECK_REF_STATUS_REJECTED
define|#
directive|define
name|CHECK_REF_STATUS_REJECTED
value|-2
end_define

begin_define
DECL|macro|CHECK_REF_UPTODATE
define|#
directive|define
name|CHECK_REF_UPTODATE
value|-3
end_define

begin_function
DECL|function|check_to_send_update
specifier|static
name|int
name|check_to_send_update
parameter_list|(
specifier|const
name|struct
name|ref
modifier|*
name|ref
parameter_list|,
specifier|const
name|struct
name|send_pack_args
modifier|*
name|args
parameter_list|)
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
return|return
name|CHECK_REF_NO_PUSH
return|;
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
name|REF_STATUS_REJECT_NODELETE
case|:
return|return
name|CHECK_REF_STATUS_REJECTED
return|;
case|case
name|REF_STATUS_UPTODATE
case|:
return|return
name|CHECK_REF_UPTODATE
return|;
default|default:
return|return
literal|0
return|;
block|}
block|}
end_function

begin_comment
comment|/*  * the beginning of the next line, or the end of buffer.  *  * NEEDSWORK: perhaps move this to git-compat-util.h or somewhere and  * convert many similar uses found by "git grep -A4 memchr".  */
end_comment

begin_function
DECL|function|next_line
specifier|static
specifier|const
name|char
modifier|*
name|next_line
parameter_list|(
specifier|const
name|char
modifier|*
name|line
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|nl
init|=
name|memchr
argument_list|(
name|line
argument_list|,
literal|'\n'
argument_list|,
name|len
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|nl
condition|)
return|return
name|line
operator|+
name|len
return|;
comment|/* incomplete line */
return|return
name|nl
operator|+
literal|1
return|;
block|}
end_function

begin_function
DECL|function|generate_push_cert
specifier|static
name|int
name|generate_push_cert
parameter_list|(
name|struct
name|strbuf
modifier|*
name|req_buf
parameter_list|,
specifier|const
name|struct
name|ref
modifier|*
name|remote_refs
parameter_list|,
name|struct
name|send_pack_args
modifier|*
name|args
parameter_list|,
specifier|const
name|char
modifier|*
name|cap_string
parameter_list|,
specifier|const
name|char
modifier|*
name|push_cert_nonce
parameter_list|)
block|{
specifier|const
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
name|char
modifier|*
name|signing_key
init|=
name|xstrdup
argument_list|(
name|get_signing_key
argument_list|()
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|cp
decl_stmt|,
modifier|*
name|np
decl_stmt|;
name|struct
name|strbuf
name|cert
init|=
name|STRBUF_INIT
decl_stmt|;
name|int
name|update_seen
init|=
literal|0
decl_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|cert
argument_list|,
literal|"certificate version 0.1\n"
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|cert
argument_list|,
literal|"pusher %s "
argument_list|,
name|signing_key
argument_list|)
expr_stmt|;
name|datestamp
argument_list|(
operator|&
name|cert
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
operator|&
name|cert
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|url
operator|&&
operator|*
name|args
operator|->
name|url
condition|)
block|{
name|char
modifier|*
name|anon_url
init|=
name|transport_anonymize_url
argument_list|(
name|args
operator|->
name|url
argument_list|)
decl_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|cert
argument_list|,
literal|"pushee %s\n"
argument_list|,
name|anon_url
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|anon_url
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|push_cert_nonce
index|[
literal|0
index|]
condition|)
name|strbuf_addf
argument_list|(
operator|&
name|cert
argument_list|,
literal|"nonce %s\n"
argument_list|,
name|push_cert_nonce
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|cert
argument_list|,
literal|"\n"
argument_list|)
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
name|check_to_send_update
argument_list|(
name|ref
argument_list|,
name|args
argument_list|)
operator|<
literal|0
condition|)
continue|continue;
name|update_seen
operator|=
literal|1
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|cert
argument_list|,
literal|"%s %s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|new_sha1
argument_list|)
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|update_seen
condition|)
goto|goto
name|free_return
goto|;
if|if
condition|(
name|sign_buffer
argument_list|(
operator|&
name|cert
argument_list|,
operator|&
name|cert
argument_list|,
name|signing_key
argument_list|)
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"failed to sign the push certificate"
argument_list|)
argument_list|)
expr_stmt|;
name|packet_buf_write
argument_list|(
name|req_buf
argument_list|,
literal|"push-cert%c%s"
argument_list|,
literal|0
argument_list|,
name|cap_string
argument_list|)
expr_stmt|;
for|for
control|(
name|cp
operator|=
name|cert
operator|.
name|buf
init|;
name|cp
operator|<
name|cert
operator|.
name|buf
operator|+
name|cert
operator|.
name|len
condition|;
name|cp
operator|=
name|np
control|)
block|{
name|np
operator|=
name|next_line
argument_list|(
name|cp
argument_list|,
name|cert
operator|.
name|buf
operator|+
name|cert
operator|.
name|len
operator|-
name|cp
argument_list|)
expr_stmt|;
name|packet_buf_write
argument_list|(
name|req_buf
argument_list|,
literal|"%.*s"
argument_list|,
call|(
name|int
call|)
argument_list|(
name|np
operator|-
name|cp
argument_list|)
argument_list|,
name|cp
argument_list|)
expr_stmt|;
block|}
name|packet_buf_write
argument_list|(
name|req_buf
argument_list|,
literal|"push-cert-end\n"
argument_list|)
expr_stmt|;
name|free_return
label|:
name|free
argument_list|(
name|signing_key
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|cert
argument_list|)
expr_stmt|;
return|return
name|update_seen
return|;
block|}
end_function

begin_function
DECL|function|atomic_push_failure
specifier|static
name|int
name|atomic_push_failure
parameter_list|(
name|struct
name|send_pack_args
modifier|*
name|args
parameter_list|,
name|struct
name|ref
modifier|*
name|remote_refs
parameter_list|,
name|struct
name|ref
modifier|*
name|failing_ref
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
comment|/* Mark other refs as failed */
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
switch|switch
condition|(
name|ref
operator|->
name|status
condition|)
block|{
case|case
name|REF_STATUS_EXPECTING_REPORT
case|:
name|ref
operator|->
name|status
operator|=
name|REF_STATUS_ATOMIC_PUSH_FAILED
expr_stmt|;
continue|continue;
default|default:
break|break;
comment|/* do nothing */
block|}
block|}
return|return
name|error
argument_list|(
literal|"atomic push failed for ref %s. status: %d\n"
argument_list|,
name|failing_ref
operator|->
name|name
argument_list|,
name|failing_ref
operator|->
name|status
argument_list|)
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
name|sha1_array
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
name|strbuf
name|cap_buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
name|int
name|need_pack_data
init|=
literal|0
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
name|int
name|use_atomic
init|=
literal|0
decl_stmt|;
name|int
name|atomic_supported
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
specifier|const
name|char
modifier|*
name|push_cert_nonce
init|=
name|NULL
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
name|server_supports
argument_list|(
literal|"no-thin"
argument_list|)
condition|)
name|args
operator|->
name|use_thin_pack
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|server_supports
argument_list|(
literal|"atomic"
argument_list|)
condition|)
name|atomic_supported
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|push_cert
condition|)
block|{
name|int
name|len
decl_stmt|;
name|push_cert_nonce
operator|=
name|server_feature_value
argument_list|(
literal|"push-cert"
argument_list|,
operator|&
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|push_cert_nonce
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"the receiving end does not support --signed push"
argument_list|)
argument_list|)
expr_stmt|;
name|push_cert_nonce
operator|=
name|xmemdupz
argument_list|(
name|push_cert_nonce
argument_list|,
name|len
argument_list|)
expr_stmt|;
block|}
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
if|if
condition|(
name|args
operator|->
name|atomic
operator|&&
operator|!
name|atomic_supported
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"server does not support --atomic push"
argument_list|)
argument_list|)
expr_stmt|;
name|use_atomic
operator|=
name|atomic_supported
operator|&&
name|args
operator|->
name|atomic
expr_stmt|;
if|if
condition|(
name|status_report
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|cap_buf
argument_list|,
literal|" report-status"
argument_list|)
expr_stmt|;
if|if
condition|(
name|use_sideband
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|cap_buf
argument_list|,
literal|" side-band-64k"
argument_list|)
expr_stmt|;
if|if
condition|(
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
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|cap_buf
argument_list|,
literal|" quiet"
argument_list|)
expr_stmt|;
if|if
condition|(
name|use_atomic
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|cap_buf
argument_list|,
literal|" atomic"
argument_list|)
expr_stmt|;
if|if
condition|(
name|agent_supported
condition|)
name|strbuf_addf
argument_list|(
operator|&
name|cap_buf
argument_list|,
literal|" agent=%s"
argument_list|,
name|git_user_agent_sanitized
argument_list|()
argument_list|)
expr_stmt|;
comment|/* 	 * NEEDSWORK: why does delete-refs have to be so specific to 	 * send-pack machinery that set_ref_status_for_push() cannot 	 * set this bit for us??? 	 */
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
if|if
condition|(
name|ref
operator|->
name|deletion
operator|&&
operator|!
name|allow_deleting_refs
condition|)
name|ref
operator|->
name|status
operator|=
name|REF_STATUS_REJECT_NODELETE
expr_stmt|;
if|if
condition|(
operator|!
name|args
operator|->
name|dry_run
condition|)
name|advertise_shallow_grafts_buf
argument_list|(
operator|&
name|req_buf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|args
operator|->
name|dry_run
operator|&&
name|args
operator|->
name|push_cert
condition|)
name|cmds_sent
operator|=
name|generate_push_cert
argument_list|(
operator|&
name|req_buf
argument_list|,
name|remote_refs
argument_list|,
name|args
argument_list|,
name|cap_buf
operator|.
name|buf
argument_list|,
name|push_cert_nonce
argument_list|)
expr_stmt|;
comment|/* 	 * Clear the status for each ref and see if we need to send 	 * the pack data. 	 */
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
name|check_to_send_update
argument_list|(
name|ref
argument_list|,
name|args
argument_list|)
condition|)
block|{
case|case
literal|0
case|:
comment|/* no error */
break|break;
case|case
name|CHECK_REF_STATUS_REJECTED
case|:
comment|/* 			 * When we know the server would reject a ref update if 			 * we were to send it and we're trying to send the refs 			 * atomically, abort the whole operation. 			 */
if|if
condition|(
name|use_atomic
condition|)
return|return
name|atomic_push_failure
argument_list|(
name|args
argument_list|,
name|remote_refs
argument_list|,
name|ref
argument_list|)
return|;
comment|/* Fallthrough for non atomic case. */
default|default:
continue|continue;
block|}
if|if
condition|(
operator|!
name|ref
operator|->
name|deletion
condition|)
name|need_pack_data
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|dry_run
operator|||
operator|!
name|status_report
condition|)
name|ref
operator|->
name|status
operator|=
name|REF_STATUS_OK
expr_stmt|;
else|else
name|ref
operator|->
name|status
operator|=
name|REF_STATUS_EXPECTING_REPORT
expr_stmt|;
block|}
comment|/* 	 * Finally, tell the other end! 	 */
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
modifier|*
name|old_hex
decl_stmt|,
modifier|*
name|new_hex
decl_stmt|;
if|if
condition|(
name|args
operator|->
name|dry_run
operator|||
name|args
operator|->
name|push_cert
condition|)
continue|continue;
if|if
condition|(
name|check_to_send_update
argument_list|(
name|ref
argument_list|,
name|args
argument_list|)
operator|<
literal|0
condition|)
continue|continue;
name|old_hex
operator|=
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|old_sha1
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
operator|!
name|cmds_sent
condition|)
block|{
name|packet_buf_write
argument_list|(
operator|&
name|req_buf
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
name|cap_buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|cmds_sent
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
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
operator|(
name|cmds_sent
operator|||
name|is_repository_shallow
argument_list|()
operator|)
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
name|strbuf_release
argument_list|(
operator|&
name|cap_buf
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
name|need_pack_data
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

