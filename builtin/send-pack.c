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

begin_include
include|#
directive|include
file|"version.h"
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
name|REF_STATUS_REJECT_ALREADY_EXISTS
case|:
name|res
operator|=
literal|"error"
expr_stmt|;
name|msg
operator|=
literal|"already exists"
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
name|unsigned
name|int
name|reject_reasons
decl_stmt|;
name|int
name|progress
init|=
operator|-
literal|1
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
literal|"--quiet"
argument_list|)
condition|)
block|{
name|args
operator|.
name|quiet
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
literal|"--progress"
argument_list|)
condition|)
block|{
name|progress
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
literal|"--no-progress"
argument_list|)
condition|)
block|{
name|progress
operator|=
literal|0
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
name|progress
operator|==
operator|-
literal|1
condition|)
name|progress
operator|=
operator|!
name|args
operator|.
name|quiet
operator|&&
name|isatty
argument_list|(
literal|2
argument_list|)
expr_stmt|;
name|args
operator|.
name|progress
operator|=
name|progress
expr_stmt|;
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
name|match_push_refs
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
name|reject_reasons
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

