begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"transport.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_struct
DECL|struct|helper_data
struct|struct
name|helper_data
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|helper
name|struct
name|child_process
modifier|*
name|helper
decl_stmt|;
DECL|member|out
name|FILE
modifier|*
name|out
decl_stmt|;
DECL|member|fetch
name|unsigned
name|fetch
range|:
literal|1
decl_stmt|,
DECL|member|option
name|option
range|:
literal|1
decl_stmt|,
DECL|member|push
name|push
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|get_helper
specifier|static
name|struct
name|child_process
modifier|*
name|get_helper
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
name|struct
name|helper_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|child_process
modifier|*
name|helper
decl_stmt|;
if|if
condition|(
name|data
operator|->
name|helper
condition|)
return|return
name|data
operator|->
name|helper
return|;
name|helper
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|helper
argument_list|)
argument_list|)
expr_stmt|;
name|helper
operator|->
name|in
operator|=
operator|-
literal|1
expr_stmt|;
name|helper
operator|->
name|out
operator|=
operator|-
literal|1
expr_stmt|;
name|helper
operator|->
name|err
operator|=
literal|0
expr_stmt|;
name|helper
operator|->
name|argv
operator|=
name|xcalloc
argument_list|(
literal|4
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|helper
operator|->
name|argv
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"remote-%s"
argument_list|,
name|data
operator|->
name|name
argument_list|)
expr_stmt|;
name|helper
operator|->
name|argv
index|[
literal|0
index|]
operator|=
name|strbuf_detach
argument_list|(
operator|&
name|buf
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|helper
operator|->
name|argv
index|[
literal|1
index|]
operator|=
name|transport
operator|->
name|remote
operator|->
name|name
expr_stmt|;
name|helper
operator|->
name|argv
index|[
literal|2
index|]
operator|=
name|transport
operator|->
name|url
expr_stmt|;
name|helper
operator|->
name|git_cmd
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|start_command
argument_list|(
name|helper
argument_list|)
condition|)
name|die
argument_list|(
literal|"Unable to run helper: git %s"
argument_list|,
name|helper
operator|->
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|data
operator|->
name|helper
operator|=
name|helper
expr_stmt|;
name|write_str_in_full
argument_list|(
name|helper
operator|->
name|in
argument_list|,
literal|"capabilities\n"
argument_list|)
expr_stmt|;
name|data
operator|->
name|out
operator|=
name|xfdopen
argument_list|(
name|helper
operator|->
name|out
argument_list|,
literal|"r"
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
if|if
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|buf
argument_list|,
name|data
operator|->
name|out
argument_list|,
literal|'\n'
argument_list|)
operator|==
name|EOF
condition|)
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
comment|/* child died, message supplied already */
if|if
condition|(
operator|!
operator|*
name|buf
operator|.
name|buf
condition|)
break|break;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"fetch"
argument_list|)
condition|)
name|data
operator|->
name|fetch
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"option"
argument_list|)
condition|)
name|data
operator|->
name|option
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"push"
argument_list|)
condition|)
name|data
operator|->
name|push
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|data
operator|->
name|helper
return|;
block|}
end_function

begin_function
DECL|function|disconnect_helper
specifier|static
name|int
name|disconnect_helper
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
name|struct
name|helper_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
if|if
condition|(
name|data
operator|->
name|helper
condition|)
block|{
name|write_str_in_full
argument_list|(
name|data
operator|->
name|helper
operator|->
name|in
argument_list|,
literal|"\n"
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|data
operator|->
name|helper
operator|->
name|in
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|data
operator|->
name|out
argument_list|)
expr_stmt|;
name|finish_command
argument_list|(
name|data
operator|->
name|helper
argument_list|)
expr_stmt|;
name|free
argument_list|(
operator|(
name|char
operator|*
operator|)
name|data
operator|->
name|helper
operator|->
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
operator|->
name|helper
operator|->
name|argv
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
operator|->
name|helper
argument_list|)
expr_stmt|;
name|data
operator|->
name|helper
operator|=
name|NULL
expr_stmt|;
block|}
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|unsupported_options
specifier|static
specifier|const
name|char
modifier|*
name|unsupported_options
index|[]
init|=
block|{
name|TRANS_OPT_UPLOADPACK
block|,
name|TRANS_OPT_RECEIVEPACK
block|,
name|TRANS_OPT_THIN
block|,
name|TRANS_OPT_KEEP
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|boolean_options
specifier|static
specifier|const
name|char
modifier|*
name|boolean_options
index|[]
init|=
block|{
name|TRANS_OPT_THIN
block|,
name|TRANS_OPT_KEEP
block|,
name|TRANS_OPT_FOLLOWTAGS
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|set_helper_option
specifier|static
name|int
name|set_helper_option
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
name|struct
name|helper_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
name|struct
name|child_process
modifier|*
name|helper
init|=
name|get_helper
argument_list|(
name|transport
argument_list|)
decl_stmt|;
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|int
name|i
decl_stmt|,
name|ret
decl_stmt|,
name|is_bool
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|data
operator|->
name|option
condition|)
return|return
literal|1
return|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ARRAY_SIZE
argument_list|(
name|unsupported_options
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|name
argument_list|,
name|unsupported_options
index|[
name|i
index|]
argument_list|)
condition|)
return|return
literal|1
return|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ARRAY_SIZE
argument_list|(
name|boolean_options
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|name
argument_list|,
name|boolean_options
index|[
name|i
index|]
argument_list|)
condition|)
block|{
name|is_bool
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"option %s "
argument_list|,
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|is_bool
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|buf
argument_list|,
name|value
condition|?
literal|"true"
else|:
literal|"false"
argument_list|)
expr_stmt|;
else|else
name|quote_c_style
argument_list|(
name|value
argument_list|,
operator|&
name|buf
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
operator|&
name|buf
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
if|if
condition|(
name|write_in_full
argument_list|(
name|helper
operator|->
name|in
argument_list|,
name|buf
operator|.
name|buf
argument_list|,
name|buf
operator|.
name|len
argument_list|)
operator|!=
name|buf
operator|.
name|len
condition|)
name|die_errno
argument_list|(
literal|"cannot send option to %s"
argument_list|,
name|data
operator|->
name|name
argument_list|)
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|buf
argument_list|,
name|data
operator|->
name|out
argument_list|,
literal|'\n'
argument_list|)
operator|==
name|EOF
condition|)
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
comment|/* child died, message supplied already */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"ok"
argument_list|)
condition|)
name|ret
operator|=
literal|0
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"error"
argument_list|)
condition|)
block|{
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"unsupported"
argument_list|)
condition|)
name|ret
operator|=
literal|1
expr_stmt|;
else|else
block|{
name|warning
argument_list|(
literal|"%s unexpectedly said: '%s'"
argument_list|,
name|data
operator|->
name|name
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|standard_options
specifier|static
name|void
name|standard_options
parameter_list|(
name|struct
name|transport
modifier|*
name|t
parameter_list|)
block|{
name|char
name|buf
index|[
literal|16
index|]
decl_stmt|;
name|int
name|n
decl_stmt|;
name|int
name|v
init|=
name|t
operator|->
name|verbose
decl_stmt|;
name|int
name|no_progress
init|=
name|v
operator|<
literal|0
operator|||
operator|(
operator|!
name|t
operator|->
name|progress
operator|&&
operator|!
name|isatty
argument_list|(
literal|1
argument_list|)
operator|)
decl_stmt|;
name|set_helper_option
argument_list|(
name|t
argument_list|,
literal|"progress"
argument_list|,
operator|!
name|no_progress
condition|?
literal|"true"
else|:
literal|"false"
argument_list|)
expr_stmt|;
name|n
operator|=
name|snprintf
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
literal|"%d"
argument_list|,
name|v
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|>=
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
condition|)
name|die
argument_list|(
literal|"impossibly large verbosity value"
argument_list|)
expr_stmt|;
name|set_helper_option
argument_list|(
name|t
argument_list|,
literal|"verbosity"
argument_list|,
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|fetch_with_fetch
specifier|static
name|int
name|fetch_with_fetch
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|nr_heads
parameter_list|,
specifier|const
name|struct
name|ref
modifier|*
modifier|*
name|to_fetch
parameter_list|)
block|{
name|struct
name|helper_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
name|int
name|i
decl_stmt|;
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|standard_options
argument_list|(
name|transport
argument_list|)
expr_stmt|;
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
name|struct
name|ref
modifier|*
name|posn
init|=
name|to_fetch
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|posn
operator|->
name|status
operator|&
name|REF_STATUS_UPTODATE
condition|)
continue|continue;
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"fetch %s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|posn
operator|->
name|old_sha1
argument_list|)
argument_list|,
name|posn
operator|->
name|name
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
if|if
condition|(
name|write_in_full
argument_list|(
name|data
operator|->
name|helper
operator|->
name|in
argument_list|,
name|buf
operator|.
name|buf
argument_list|,
name|buf
operator|.
name|len
argument_list|)
operator|!=
name|buf
operator|.
name|len
condition|)
name|die_errno
argument_list|(
literal|"cannot send fetch to %s"
argument_list|,
name|data
operator|->
name|name
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|strbuf_reset
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|buf
argument_list|,
name|data
operator|->
name|out
argument_list|,
literal|'\n'
argument_list|)
operator|==
name|EOF
condition|)
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
comment|/* child died, message supplied already */
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"lock "
argument_list|)
condition|)
block|{
specifier|const
name|char
modifier|*
name|name
init|=
name|buf
operator|.
name|buf
operator|+
literal|5
decl_stmt|;
if|if
condition|(
name|transport
operator|->
name|pack_lockfile
condition|)
name|warning
argument_list|(
literal|"%s also locked %s"
argument_list|,
name|data
operator|->
name|name
argument_list|,
name|name
argument_list|)
expr_stmt|;
else|else
name|transport
operator|->
name|pack_lockfile
operator|=
name|xstrdup
argument_list|(
name|name
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|buf
operator|.
name|len
condition|)
break|break;
else|else
name|warning
argument_list|(
literal|"%s unexpectedly said: '%s'"
argument_list|,
name|data
operator|->
name|name
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fetch
specifier|static
name|int
name|fetch
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|nr_heads
parameter_list|,
specifier|const
name|struct
name|ref
modifier|*
modifier|*
name|to_fetch
parameter_list|)
block|{
name|struct
name|helper_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
name|int
name|i
decl_stmt|,
name|count
decl_stmt|;
name|count
operator|=
literal|0
expr_stmt|;
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
if|if
condition|(
operator|!
operator|(
name|to_fetch
index|[
name|i
index|]
operator|->
name|status
operator|&
name|REF_STATUS_UPTODATE
operator|)
condition|)
name|count
operator|++
expr_stmt|;
if|if
condition|(
operator|!
name|count
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|data
operator|->
name|fetch
condition|)
return|return
name|fetch_with_fetch
argument_list|(
name|transport
argument_list|,
name|nr_heads
argument_list|,
name|to_fetch
argument_list|)
return|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|push_refs
specifier|static
name|int
name|push_refs
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|struct
name|ref
modifier|*
name|remote_refs
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|int
name|force_all
init|=
name|flags
operator|&
name|TRANSPORT_PUSH_FORCE
decl_stmt|;
name|int
name|mirror
init|=
name|flags
operator|&
name|TRANSPORT_PUSH_MIRROR
decl_stmt|;
name|struct
name|helper_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|child_process
modifier|*
name|helper
decl_stmt|;
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
if|if
condition|(
operator|!
name|remote_refs
condition|)
return|return
literal|0
return|;
name|helper
operator|=
name|get_helper
argument_list|(
name|transport
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|data
operator|->
name|push
condition|)
return|return
literal|1
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
if|if
condition|(
name|ref
operator|->
name|peer_ref
condition|)
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
elseif|else
if|if
condition|(
operator|!
name|mirror
condition|)
continue|continue;
name|ref
operator|->
name|deletion
operator|=
name|is_null_sha1
argument_list|(
name|ref
operator|->
name|new_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ref
operator|->
name|deletion
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
name|new_sha1
argument_list|)
condition|)
block|{
name|ref
operator|->
name|status
operator|=
name|REF_STATUS_UPTODATE
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|force_all
condition|)
name|ref
operator|->
name|force
operator|=
literal|1
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|buf
argument_list|,
literal|"push "
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ref
operator|->
name|deletion
condition|)
block|{
if|if
condition|(
name|ref
operator|->
name|force
condition|)
name|strbuf_addch
argument_list|(
operator|&
name|buf
argument_list|,
literal|'+'
argument_list|)
expr_stmt|;
if|if
condition|(
name|ref
operator|->
name|peer_ref
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|buf
argument_list|,
name|ref
operator|->
name|peer_ref
operator|->
name|name
argument_list|)
expr_stmt|;
else|else
name|strbuf_addstr
argument_list|(
operator|&
name|buf
argument_list|,
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|new_sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|strbuf_addch
argument_list|(
operator|&
name|buf
argument_list|,
literal|':'
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|buf
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
operator|&
name|buf
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|buf
operator|.
name|len
operator|==
literal|0
condition|)
return|return
literal|0
return|;
name|transport
operator|->
name|verbose
operator|=
name|flags
operator|&
name|TRANSPORT_PUSH_VERBOSE
condition|?
literal|1
else|:
literal|0
expr_stmt|;
name|standard_options
argument_list|(
name|transport
argument_list|)
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|TRANSPORT_PUSH_DRY_RUN
condition|)
block|{
if|if
condition|(
name|set_helper_option
argument_list|(
name|transport
argument_list|,
literal|"dry-run"
argument_list|,
literal|"true"
argument_list|)
operator|!=
literal|0
condition|)
name|die
argument_list|(
literal|"helper %s does not support dry-run"
argument_list|,
name|data
operator|->
name|name
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
if|if
condition|(
name|write_in_full
argument_list|(
name|helper
operator|->
name|in
argument_list|,
name|buf
operator|.
name|buf
argument_list|,
name|buf
operator|.
name|len
argument_list|)
operator|!=
name|buf
operator|.
name|len
condition|)
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
name|ref
operator|=
name|remote_refs
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|char
modifier|*
name|refname
decl_stmt|,
modifier|*
name|msg
decl_stmt|;
name|int
name|status
decl_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|buf
argument_list|,
name|data
operator|->
name|out
argument_list|,
literal|'\n'
argument_list|)
operator|==
name|EOF
condition|)
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
comment|/* child died, message supplied already */
if|if
condition|(
operator|!
name|buf
operator|.
name|len
condition|)
break|break;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"ok "
argument_list|)
condition|)
block|{
name|status
operator|=
name|REF_STATUS_OK
expr_stmt|;
name|refname
operator|=
name|buf
operator|.
name|buf
operator|+
literal|3
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"error "
argument_list|)
condition|)
block|{
name|status
operator|=
name|REF_STATUS_REMOTE_REJECT
expr_stmt|;
name|refname
operator|=
name|buf
operator|.
name|buf
operator|+
literal|6
expr_stmt|;
block|}
else|else
name|die
argument_list|(
literal|"expected ok/error, helper said '%s'\n"
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
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
block|{
name|struct
name|strbuf
name|msg_buf
init|=
name|STRBUF_INIT
decl_stmt|;
specifier|const
name|char
modifier|*
name|end
decl_stmt|;
operator|*
name|msg
operator|++
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
operator|!
name|unquote_c_style
argument_list|(
operator|&
name|msg_buf
argument_list|,
name|msg
argument_list|,
operator|&
name|end
argument_list|)
condition|)
name|msg
operator|=
name|strbuf_detach
argument_list|(
operator|&
name|msg_buf
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
else|else
name|msg
operator|=
name|xstrdup
argument_list|(
name|msg
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|msg_buf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|msg
argument_list|,
literal|"no match"
argument_list|)
condition|)
block|{
name|status
operator|=
name|REF_STATUS_NONE
expr_stmt|;
name|free
argument_list|(
name|msg
argument_list|)
expr_stmt|;
name|msg
operator|=
name|NULL
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|msg
argument_list|,
literal|"up to date"
argument_list|)
condition|)
block|{
name|status
operator|=
name|REF_STATUS_UPTODATE
expr_stmt|;
name|free
argument_list|(
name|msg
argument_list|)
expr_stmt|;
name|msg
operator|=
name|NULL
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|msg
argument_list|,
literal|"non-fast forward"
argument_list|)
condition|)
block|{
name|status
operator|=
name|REF_STATUS_REJECT_NONFASTFORWARD
expr_stmt|;
name|free
argument_list|(
name|msg
argument_list|)
expr_stmt|;
name|msg
operator|=
name|NULL
expr_stmt|;
block|}
block|}
if|if
condition|(
name|ref
condition|)
name|ref
operator|=
name|find_ref_by_name
argument_list|(
name|ref
argument_list|,
name|refname
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ref
condition|)
name|ref
operator|=
name|find_ref_by_name
argument_list|(
name|remote_refs
argument_list|,
name|refname
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ref
condition|)
block|{
name|warning
argument_list|(
literal|"helper reported unexpected status of %s"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|ref
operator|->
name|status
operator|=
name|status
expr_stmt|;
name|ref
operator|->
name|remote_status
operator|=
name|msg
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|get_refs_list
specifier|static
name|struct
name|ref
modifier|*
name|get_refs_list
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|for_push
parameter_list|)
block|{
name|struct
name|helper_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
name|struct
name|child_process
modifier|*
name|helper
decl_stmt|;
name|struct
name|ref
modifier|*
name|ret
init|=
name|NULL
decl_stmt|;
name|struct
name|ref
modifier|*
modifier|*
name|tail
init|=
operator|&
name|ret
decl_stmt|;
name|struct
name|ref
modifier|*
name|posn
decl_stmt|;
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|helper
operator|=
name|get_helper
argument_list|(
name|transport
argument_list|)
expr_stmt|;
if|if
condition|(
name|data
operator|->
name|push
operator|&&
name|for_push
condition|)
name|write_str_in_full
argument_list|(
name|helper
operator|->
name|in
argument_list|,
literal|"list for-push\n"
argument_list|)
expr_stmt|;
else|else
name|write_str_in_full
argument_list|(
name|helper
operator|->
name|in
argument_list|,
literal|"list\n"
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|char
modifier|*
name|eov
decl_stmt|,
modifier|*
name|eon
decl_stmt|;
if|if
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|buf
argument_list|,
name|data
operator|->
name|out
argument_list|,
literal|'\n'
argument_list|)
operator|==
name|EOF
condition|)
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
comment|/* child died, message supplied already */
if|if
condition|(
operator|!
operator|*
name|buf
operator|.
name|buf
condition|)
break|break;
name|eov
operator|=
name|strchr
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|' '
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|eov
condition|)
name|die
argument_list|(
literal|"Malformed response in ref list: %s"
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|eon
operator|=
name|strchr
argument_list|(
name|eov
operator|+
literal|1
argument_list|,
literal|' '
argument_list|)
expr_stmt|;
operator|*
name|eov
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
name|eon
condition|)
operator|*
name|eon
operator|=
literal|'\0'
expr_stmt|;
operator|*
name|tail
operator|=
name|alloc_ref
argument_list|(
name|eov
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|buf
operator|.
name|buf
index|[
literal|0
index|]
operator|==
literal|'@'
condition|)
operator|(
operator|*
name|tail
operator|)
operator|->
name|symref
operator|=
name|xstrdup
argument_list|(
name|buf
operator|.
name|buf
operator|+
literal|1
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|buf
operator|.
name|buf
index|[
literal|0
index|]
operator|!=
literal|'?'
condition|)
name|get_sha1_hex
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
operator|(
operator|*
name|tail
operator|)
operator|->
name|old_sha1
argument_list|)
expr_stmt|;
name|tail
operator|=
operator|&
operator|(
operator|(
operator|*
name|tail
operator|)
operator|->
name|next
operator|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
for|for
control|(
name|posn
operator|=
name|ret
init|;
name|posn
condition|;
name|posn
operator|=
name|posn
operator|->
name|next
control|)
name|resolve_remote_symref
argument_list|(
name|posn
argument_list|,
name|ret
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|transport_helper_init
name|int
name|transport_helper_init
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|struct
name|helper_data
modifier|*
name|data
init|=
name|xcalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|data
argument_list|)
argument_list|,
literal|1
argument_list|)
decl_stmt|;
name|data
operator|->
name|name
operator|=
name|name
expr_stmt|;
name|transport
operator|->
name|data
operator|=
name|data
expr_stmt|;
name|transport
operator|->
name|set_option
operator|=
name|set_helper_option
expr_stmt|;
name|transport
operator|->
name|get_refs_list
operator|=
name|get_refs_list
expr_stmt|;
name|transport
operator|->
name|fetch
operator|=
name|fetch
expr_stmt|;
name|transport
operator|->
name|push_refs
operator|=
name|push_refs
expr_stmt|;
name|transport
operator|->
name|disconnect
operator|=
name|disconnect_helper
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

