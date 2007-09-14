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
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"http.h"
end_include

begin_include
include|#
directive|include
file|"pkt-line.h"
end_include

begin_include
include|#
directive|include
file|"fetch-pack.h"
end_include

begin_include
include|#
directive|include
file|"walker.h"
end_include

begin_include
include|#
directive|include
file|"bundle.h"
end_include

begin_comment
comment|/* Generic functions for using commit walkers */
end_comment

begin_function
DECL|function|fetch_objs_via_walker
specifier|static
name|int
name|fetch_objs_via_walker
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|nr_objs
parameter_list|,
name|struct
name|ref
modifier|*
modifier|*
name|to_fetch
parameter_list|)
block|{
name|char
modifier|*
name|dest
init|=
name|xstrdup
argument_list|(
name|transport
operator|->
name|url
argument_list|)
decl_stmt|;
name|struct
name|walker
modifier|*
name|walker
init|=
name|transport
operator|->
name|data
decl_stmt|;
name|char
modifier|*
modifier|*
name|objs
init|=
name|xmalloc
argument_list|(
name|nr_objs
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|objs
argument_list|)
argument_list|)
decl_stmt|;
name|int
name|i
decl_stmt|;
name|walker
operator|->
name|get_all
operator|=
literal|1
expr_stmt|;
name|walker
operator|->
name|get_tree
operator|=
literal|1
expr_stmt|;
name|walker
operator|->
name|get_history
operator|=
literal|1
expr_stmt|;
name|walker
operator|->
name|get_verbosely
operator|=
name|transport
operator|->
name|verbose
expr_stmt|;
name|walker
operator|->
name|get_recover
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
name|nr_objs
condition|;
name|i
operator|++
control|)
name|objs
index|[
name|i
index|]
operator|=
name|xstrdup
argument_list|(
name|sha1_to_hex
argument_list|(
name|to_fetch
index|[
name|i
index|]
operator|->
name|old_sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|walker_fetch
argument_list|(
name|walker
argument_list|,
name|nr_objs
argument_list|,
name|objs
argument_list|,
name|NULL
argument_list|,
name|dest
argument_list|)
condition|)
name|die
argument_list|(
literal|"Fetch failed."
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
name|nr_objs
condition|;
name|i
operator|++
control|)
name|free
argument_list|(
name|objs
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|objs
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|dest
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|disconnect_walker
specifier|static
name|int
name|disconnect_walker
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
name|struct
name|walker
modifier|*
name|walker
init|=
name|transport
operator|->
name|data
decl_stmt|;
if|if
condition|(
name|walker
condition|)
name|walker_free
argument_list|(
name|walker
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|rsync_transport
specifier|static
specifier|const
name|struct
name|transport_ops
name|rsync_transport
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|curl_transport_push
specifier|static
name|int
name|curl_transport_push
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|refspec_nr
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspec
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
name|int
name|argc
decl_stmt|;
name|int
name|err
decl_stmt|;
name|argv
operator|=
name|xmalloc
argument_list|(
operator|(
name|refspec_nr
operator|+
literal|11
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|argv
index|[
literal|0
index|]
operator|=
literal|"http-push"
expr_stmt|;
name|argc
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|TRANSPORT_PUSH_ALL
condition|)
name|argv
index|[
name|argc
operator|++
index|]
operator|=
literal|"--all"
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|TRANSPORT_PUSH_FORCE
condition|)
name|argv
index|[
name|argc
operator|++
index|]
operator|=
literal|"--force"
expr_stmt|;
name|argv
index|[
name|argc
operator|++
index|]
operator|=
name|transport
operator|->
name|url
expr_stmt|;
while|while
condition|(
name|refspec_nr
operator|--
condition|)
name|argv
index|[
name|argc
operator|++
index|]
operator|=
operator|*
name|refspec
operator|++
expr_stmt|;
name|argv
index|[
name|argc
index|]
operator|=
name|NULL
expr_stmt|;
name|err
operator|=
name|run_command_v_opt
argument_list|(
name|argv
argument_list|,
name|RUN_GIT_CMD
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|err
condition|)
block|{
case|case
operator|-
name|ERR_RUN_COMMAND_FORK
case|:
name|error
argument_list|(
literal|"unable to fork for %s"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_EXEC
case|:
name|error
argument_list|(
literal|"unable to exec %s"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
break|break;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID
case|:
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_WRONG_PID
case|:
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_SIGNAL
case|:
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_NOEXIT
case|:
name|error
argument_list|(
literal|"%s died with strange error"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
block|}
return|return
operator|!
operator|!
name|err
return|;
block|}
end_function

begin_ifndef
ifndef|#
directive|ifndef
name|NO_CURL
end_ifndef

begin_function
DECL|function|missing__target
specifier|static
name|int
name|missing__target
parameter_list|(
name|int
name|code
parameter_list|,
name|int
name|result
parameter_list|)
block|{
return|return
comment|/* file:// URL -- do we ever use one??? */
operator|(
name|result
operator|==
name|CURLE_FILE_COULDNT_READ_FILE
operator|)
operator|||
comment|/* http:// and https:// URL */
operator|(
name|code
operator|==
literal|404
operator|&&
name|result
operator|==
name|CURLE_HTTP_RETURNED_ERROR
operator|)
operator|||
comment|/* ftp:// URL */
operator|(
name|code
operator|==
literal|550
operator|&&
name|result
operator|==
name|CURLE_FTP_COULDNT_RETR_FILE
operator|)
return|;
block|}
end_function

begin_define
DECL|macro|missing_target
define|#
directive|define
name|missing_target
parameter_list|(
name|a
parameter_list|)
value|missing__target((a)->http_code, (a)->curl_result)
end_define

begin_function
DECL|function|get_refs_via_curl
specifier|static
name|struct
name|ref
modifier|*
name|get_refs_via_curl
parameter_list|(
specifier|const
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
name|struct
name|buffer
name|buffer
decl_stmt|;
name|char
modifier|*
name|data
decl_stmt|,
modifier|*
name|start
decl_stmt|,
modifier|*
name|mid
decl_stmt|;
name|char
modifier|*
name|ref_name
decl_stmt|;
name|char
modifier|*
name|refs_url
decl_stmt|;
name|int
name|i
init|=
literal|0
decl_stmt|;
name|struct
name|active_request_slot
modifier|*
name|slot
decl_stmt|;
name|struct
name|slot_results
name|results
decl_stmt|;
name|struct
name|ref
modifier|*
name|refs
init|=
name|NULL
decl_stmt|;
name|struct
name|ref
modifier|*
name|ref
init|=
name|NULL
decl_stmt|;
name|struct
name|ref
modifier|*
name|last_ref
init|=
name|NULL
decl_stmt|;
name|data
operator|=
name|xmalloc
argument_list|(
literal|4096
argument_list|)
expr_stmt|;
name|buffer
operator|.
name|size
operator|=
literal|4096
expr_stmt|;
name|buffer
operator|.
name|posn
operator|=
literal|0
expr_stmt|;
name|buffer
operator|.
name|buffer
operator|=
name|data
expr_stmt|;
name|refs_url
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|transport
operator|->
name|url
argument_list|)
operator|+
literal|11
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|refs_url
argument_list|,
literal|"%s/info/refs"
argument_list|,
name|transport
operator|->
name|url
argument_list|)
expr_stmt|;
name|http_init
argument_list|()
expr_stmt|;
name|slot
operator|=
name|get_active_slot
argument_list|()
expr_stmt|;
name|slot
operator|->
name|results
operator|=
operator|&
name|results
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|slot
operator|->
name|curl
argument_list|,
name|CURLOPT_FILE
argument_list|,
operator|&
name|buffer
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|slot
operator|->
name|curl
argument_list|,
name|CURLOPT_WRITEFUNCTION
argument_list|,
name|fwrite_buffer
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|slot
operator|->
name|curl
argument_list|,
name|CURLOPT_URL
argument_list|,
name|refs_url
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|slot
operator|->
name|curl
argument_list|,
name|CURLOPT_HTTPHEADER
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|start_active_slot
argument_list|(
name|slot
argument_list|)
condition|)
block|{
name|run_active_slot
argument_list|(
name|slot
argument_list|)
expr_stmt|;
if|if
condition|(
name|results
operator|.
name|curl_result
operator|!=
name|CURLE_OK
condition|)
block|{
if|if
condition|(
name|missing_target
argument_list|(
operator|&
name|results
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|buffer
operator|.
name|buffer
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
else|else
block|{
name|free
argument_list|(
name|buffer
operator|.
name|buffer
argument_list|)
expr_stmt|;
name|error
argument_list|(
literal|"%s"
argument_list|,
name|curl_errorstr
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
block|}
block|}
else|else
block|{
name|free
argument_list|(
name|buffer
operator|.
name|buffer
argument_list|)
expr_stmt|;
name|error
argument_list|(
literal|"Unable to start request"
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|http_cleanup
argument_list|()
expr_stmt|;
name|data
operator|=
name|buffer
operator|.
name|buffer
expr_stmt|;
name|start
operator|=
name|NULL
expr_stmt|;
name|mid
operator|=
name|data
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|buffer
operator|.
name|posn
condition|)
block|{
if|if
condition|(
operator|!
name|start
condition|)
name|start
operator|=
operator|&
name|data
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|data
index|[
name|i
index|]
operator|==
literal|'\t'
condition|)
name|mid
operator|=
operator|&
name|data
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|data
index|[
name|i
index|]
operator|==
literal|'\n'
condition|)
block|{
name|data
index|[
name|i
index|]
operator|=
literal|0
expr_stmt|;
name|ref_name
operator|=
name|mid
operator|+
literal|1
expr_stmt|;
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
name|strlen
argument_list|(
name|ref_name
argument_list|)
operator|+
literal|1
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|ref
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|ref
argument_list|)
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|ref
operator|->
name|name
argument_list|,
name|ref_name
argument_list|)
expr_stmt|;
name|get_sha1_hex
argument_list|(
name|start
argument_list|,
name|ref
operator|->
name|old_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|refs
condition|)
name|refs
operator|=
name|ref
expr_stmt|;
if|if
condition|(
name|last_ref
condition|)
name|last_ref
operator|->
name|next
operator|=
name|ref
expr_stmt|;
name|last_ref
operator|=
name|ref
expr_stmt|;
name|start
operator|=
name|NULL
expr_stmt|;
block|}
name|i
operator|++
expr_stmt|;
block|}
name|free
argument_list|(
name|buffer
operator|.
name|buffer
argument_list|)
expr_stmt|;
return|return
name|refs
return|;
block|}
end_function

begin_else
else|#
directive|else
end_else

begin_function
DECL|function|get_refs_via_curl
specifier|static
name|struct
name|ref
modifier|*
name|get_refs_via_curl
parameter_list|(
specifier|const
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
name|die
argument_list|(
literal|"Cannot fetch from '%s' without curl ..."
argument_list|,
name|transport
operator|->
name|url
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|curl_transport
specifier|static
specifier|const
name|struct
name|transport_ops
name|curl_transport
init|=
block|{
comment|/* set_option */
name|NULL
block|,
comment|/* get_refs_list */
name|get_refs_via_curl
block|,
comment|/* fetch */
name|fetch_objs_via_walker
block|,
comment|/* push */
name|curl_transport_push
block|,
comment|/* disconnect */
name|disconnect_walker
block|}
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|bundle_transport_data
struct|struct
name|bundle_transport_data
block|{
DECL|member|fd
name|int
name|fd
decl_stmt|;
DECL|member|header
name|struct
name|bundle_header
name|header
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|get_refs_from_bundle
specifier|static
name|struct
name|ref
modifier|*
name|get_refs_from_bundle
parameter_list|(
specifier|const
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
name|struct
name|bundle_transport_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
name|struct
name|ref
modifier|*
name|result
init|=
name|NULL
decl_stmt|;
name|int
name|i
decl_stmt|;
if|if
condition|(
name|data
operator|->
name|fd
operator|>
literal|0
condition|)
name|close
argument_list|(
name|data
operator|->
name|fd
argument_list|)
expr_stmt|;
name|data
operator|->
name|fd
operator|=
name|read_bundle_header
argument_list|(
name|transport
operator|->
name|url
argument_list|,
operator|&
name|data
operator|->
name|header
argument_list|)
expr_stmt|;
if|if
condition|(
name|data
operator|->
name|fd
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Could not read bundle '%s'."
argument_list|,
name|transport
operator|->
name|url
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
name|data
operator|->
name|header
operator|.
name|references
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|ref_list_entry
modifier|*
name|e
init|=
name|data
operator|->
name|header
operator|.
name|references
operator|.
name|list
operator|+
name|i
decl_stmt|;
name|struct
name|ref
modifier|*
name|ref
init|=
name|alloc_ref
argument_list|(
name|strlen
argument_list|(
name|e
operator|->
name|name
argument_list|)
argument_list|)
decl_stmt|;
name|hashcpy
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|,
name|e
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|ref
operator|->
name|name
argument_list|,
name|e
operator|->
name|name
argument_list|)
expr_stmt|;
name|ref
operator|->
name|next
operator|=
name|result
expr_stmt|;
name|result
operator|=
name|ref
expr_stmt|;
block|}
return|return
name|result
return|;
block|}
end_function

begin_function
DECL|function|fetch_refs_from_bundle
specifier|static
name|int
name|fetch_refs_from_bundle
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|nr_heads
parameter_list|,
name|struct
name|ref
modifier|*
modifier|*
name|to_fetch
parameter_list|)
block|{
name|struct
name|bundle_transport_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
return|return
name|unbundle
argument_list|(
operator|&
name|data
operator|->
name|header
argument_list|,
name|data
operator|->
name|fd
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|close_bundle
specifier|static
name|int
name|close_bundle
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
name|struct
name|bundle_transport_data
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
name|fd
operator|>
literal|0
condition|)
name|close
argument_list|(
name|data
operator|->
name|fd
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|bundle_transport
specifier|static
specifier|const
name|struct
name|transport_ops
name|bundle_transport
init|=
block|{
comment|/* set_option */
name|NULL
block|,
comment|/* get_refs_list */
name|get_refs_from_bundle
block|,
comment|/* fetch */
name|fetch_refs_from_bundle
block|,
comment|/* push */
name|NULL
block|,
comment|/* disconnect */
name|close_bundle
block|}
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|git_transport_data
struct|struct
name|git_transport_data
block|{
DECL|member|thin
name|unsigned
name|thin
range|:
literal|1
decl_stmt|;
DECL|member|keep
name|unsigned
name|keep
range|:
literal|1
decl_stmt|;
DECL|member|unpacklimit
name|int
name|unpacklimit
decl_stmt|;
DECL|member|depth
name|int
name|depth
decl_stmt|;
DECL|member|uploadpack
specifier|const
name|char
modifier|*
name|uploadpack
decl_stmt|;
DECL|member|receivepack
specifier|const
name|char
modifier|*
name|receivepack
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|set_git_option
specifier|static
name|int
name|set_git_option
parameter_list|(
name|struct
name|transport
modifier|*
name|connection
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
name|git_transport_data
modifier|*
name|data
init|=
name|connection
operator|->
name|data
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|name
argument_list|,
name|TRANS_OPT_UPLOADPACK
argument_list|)
condition|)
block|{
name|data
operator|->
name|uploadpack
operator|=
name|value
expr_stmt|;
return|return
literal|0
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|name
argument_list|,
name|TRANS_OPT_RECEIVEPACK
argument_list|)
condition|)
block|{
name|data
operator|->
name|receivepack
operator|=
name|value
expr_stmt|;
return|return
literal|0
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|name
argument_list|,
name|TRANS_OPT_THIN
argument_list|)
condition|)
block|{
name|data
operator|->
name|thin
operator|=
operator|!
operator|!
name|value
expr_stmt|;
return|return
literal|0
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|name
argument_list|,
name|TRANS_OPT_KEEP
argument_list|)
condition|)
block|{
name|data
operator|->
name|keep
operator|=
operator|!
operator|!
name|value
expr_stmt|;
return|return
literal|0
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|name
argument_list|,
name|TRANS_OPT_UNPACKLIMIT
argument_list|)
condition|)
block|{
name|data
operator|->
name|unpacklimit
operator|=
name|atoi
argument_list|(
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|name
argument_list|,
name|TRANS_OPT_DEPTH
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|value
condition|)
name|data
operator|->
name|depth
operator|=
literal|0
expr_stmt|;
else|else
name|data
operator|->
name|depth
operator|=
name|atoi
argument_list|(
name|value
argument_list|)
expr_stmt|;
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
DECL|function|get_refs_via_connect
specifier|static
name|struct
name|ref
modifier|*
name|get_refs_via_connect
parameter_list|(
specifier|const
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
name|struct
name|git_transport_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
name|struct
name|ref
modifier|*
name|refs
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
name|char
modifier|*
name|dest
init|=
name|xstrdup
argument_list|(
name|transport
operator|->
name|url
argument_list|)
decl_stmt|;
name|pid
operator|=
name|git_connect
argument_list|(
name|fd
argument_list|,
name|dest
argument_list|,
name|data
operator|->
name|uploadpack
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|pid
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Failed to connect to \"%s\""
argument_list|,
name|transport
operator|->
name|url
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
name|refs
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|packet_flush
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
name|free
argument_list|(
name|dest
argument_list|)
expr_stmt|;
return|return
name|refs
return|;
block|}
end_function

begin_function
DECL|function|fetch_refs_via_pack
specifier|static
name|int
name|fetch_refs_via_pack
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|nr_heads
parameter_list|,
name|struct
name|ref
modifier|*
modifier|*
name|to_fetch
parameter_list|)
block|{
name|struct
name|git_transport_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
name|char
modifier|*
modifier|*
name|heads
init|=
name|xmalloc
argument_list|(
name|nr_heads
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|heads
argument_list|)
argument_list|)
decl_stmt|;
name|struct
name|ref
modifier|*
name|refs
decl_stmt|;
name|char
modifier|*
name|dest
init|=
name|xstrdup
argument_list|(
name|transport
operator|->
name|url
argument_list|)
decl_stmt|;
name|struct
name|fetch_pack_args
name|args
decl_stmt|;
name|int
name|i
decl_stmt|;
name|args
operator|.
name|uploadpack
operator|=
name|data
operator|->
name|uploadpack
expr_stmt|;
name|args
operator|.
name|quiet
operator|=
literal|0
expr_stmt|;
name|args
operator|.
name|keep_pack
operator|=
name|data
operator|->
name|keep
expr_stmt|;
name|args
operator|.
name|unpacklimit
operator|=
name|data
operator|->
name|unpacklimit
expr_stmt|;
name|args
operator|.
name|use_thin_pack
operator|=
name|data
operator|->
name|thin
expr_stmt|;
name|args
operator|.
name|fetch_all
operator|=
literal|0
expr_stmt|;
name|args
operator|.
name|verbose
operator|=
name|transport
operator|->
name|verbose
expr_stmt|;
name|args
operator|.
name|depth
operator|=
name|data
operator|->
name|depth
expr_stmt|;
name|args
operator|.
name|no_progress
operator|=
literal|0
expr_stmt|;
name|setup_fetch_pack
argument_list|(
operator|&
name|args
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
name|heads
index|[
name|i
index|]
operator|=
name|xstrdup
argument_list|(
name|to_fetch
index|[
name|i
index|]
operator|->
name|name
argument_list|)
expr_stmt|;
name|refs
operator|=
name|fetch_pack
argument_list|(
name|dest
argument_list|,
name|nr_heads
argument_list|,
name|heads
argument_list|,
operator|&
name|transport
operator|->
name|pack_lockfile
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
name|free
argument_list|(
name|heads
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|free_refs
argument_list|(
name|refs
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|dest
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|git_transport_push
specifier|static
name|int
name|git_transport_push
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|refspec_nr
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspec
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|struct
name|git_transport_data
modifier|*
name|data
init|=
name|transport
operator|->
name|data
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
name|char
modifier|*
name|rem
decl_stmt|;
name|int
name|argc
decl_stmt|;
name|int
name|err
decl_stmt|;
name|argv
operator|=
name|xmalloc
argument_list|(
operator|(
name|refspec_nr
operator|+
literal|11
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|argv
index|[
literal|0
index|]
operator|=
literal|"send-pack"
expr_stmt|;
name|argc
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|TRANSPORT_PUSH_ALL
condition|)
name|argv
index|[
name|argc
operator|++
index|]
operator|=
literal|"--all"
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|TRANSPORT_PUSH_FORCE
condition|)
name|argv
index|[
name|argc
operator|++
index|]
operator|=
literal|"--force"
expr_stmt|;
if|if
condition|(
name|data
operator|->
name|receivepack
condition|)
block|{
name|char
modifier|*
name|rp
init|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|data
operator|->
name|receivepack
argument_list|)
operator|+
literal|16
argument_list|)
decl_stmt|;
name|sprintf
argument_list|(
name|rp
argument_list|,
literal|"--receive-pack=%s"
argument_list|,
name|data
operator|->
name|receivepack
argument_list|)
expr_stmt|;
name|argv
index|[
name|argc
operator|++
index|]
operator|=
name|rp
expr_stmt|;
block|}
if|if
condition|(
name|data
operator|->
name|thin
condition|)
name|argv
index|[
name|argc
operator|++
index|]
operator|=
literal|"--thin"
expr_stmt|;
name|rem
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|transport
operator|->
name|remote
operator|->
name|name
argument_list|)
operator|+
literal|10
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|rem
argument_list|,
literal|"--remote=%s"
argument_list|,
name|transport
operator|->
name|remote
operator|->
name|name
argument_list|)
expr_stmt|;
name|argv
index|[
name|argc
operator|++
index|]
operator|=
name|rem
expr_stmt|;
name|argv
index|[
name|argc
operator|++
index|]
operator|=
name|transport
operator|->
name|url
expr_stmt|;
while|while
condition|(
name|refspec_nr
operator|--
condition|)
name|argv
index|[
name|argc
operator|++
index|]
operator|=
operator|*
name|refspec
operator|++
expr_stmt|;
name|argv
index|[
name|argc
index|]
operator|=
name|NULL
expr_stmt|;
name|err
operator|=
name|run_command_v_opt
argument_list|(
name|argv
argument_list|,
name|RUN_GIT_CMD
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|err
condition|)
block|{
case|case
operator|-
name|ERR_RUN_COMMAND_FORK
case|:
name|error
argument_list|(
literal|"unable to fork for %s"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_EXEC
case|:
name|error
argument_list|(
literal|"unable to exec %s"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
break|break;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID
case|:
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_WRONG_PID
case|:
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_SIGNAL
case|:
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_NOEXIT
case|:
name|error
argument_list|(
literal|"%s died with strange error"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
block|}
return|return
operator|!
operator|!
name|err
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|git_transport
specifier|static
specifier|const
name|struct
name|transport_ops
name|git_transport
init|=
block|{
comment|/* set_option */
name|set_git_option
block|,
comment|/* get_refs_list */
name|get_refs_via_connect
block|,
comment|/* fetch */
name|fetch_refs_via_pack
block|,
comment|/* push */
name|git_transport_push
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|is_local
specifier|static
name|int
name|is_local
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|colon
init|=
name|strchr
argument_list|(
name|url
argument_list|,
literal|':'
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|slash
init|=
name|strchr
argument_list|(
name|url
argument_list|,
literal|'/'
argument_list|)
decl_stmt|;
return|return
operator|!
name|colon
operator|||
operator|(
name|slash
operator|&&
name|slash
operator|<
name|colon
operator|)
return|;
block|}
end_function

begin_function
DECL|function|is_file
specifier|static
name|int
name|is_file
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|)
block|{
name|struct
name|stat
name|buf
decl_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|url
argument_list|,
operator|&
name|buf
argument_list|)
condition|)
return|return
literal|0
return|;
return|return
name|S_ISREG
argument_list|(
name|buf
operator|.
name|st_mode
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|transport_get
name|struct
name|transport
modifier|*
name|transport_get
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
specifier|const
name|char
modifier|*
name|url
parameter_list|,
name|int
name|fetch
parameter_list|)
block|{
name|struct
name|transport
modifier|*
name|ret
init|=
name|NULL
decl_stmt|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|url
argument_list|,
literal|"rsync://"
argument_list|)
condition|)
block|{
name|ret
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|ret
argument_list|)
argument_list|)
expr_stmt|;
name|ret
operator|->
name|data
operator|=
name|NULL
expr_stmt|;
name|ret
operator|->
name|ops
operator|=
operator|&
name|rsync_transport
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|url
argument_list|,
literal|"http://"
argument_list|)
operator|||
operator|!
name|prefixcmp
argument_list|(
name|url
argument_list|,
literal|"https://"
argument_list|)
operator|||
operator|!
name|prefixcmp
argument_list|(
name|url
argument_list|,
literal|"ftp://"
argument_list|)
condition|)
block|{
name|ret
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|ret
argument_list|)
argument_list|)
expr_stmt|;
name|ret
operator|->
name|ops
operator|=
operator|&
name|curl_transport
expr_stmt|;
if|if
condition|(
name|fetch
condition|)
name|ret
operator|->
name|data
operator|=
name|get_http_walker
argument_list|(
name|url
argument_list|)
expr_stmt|;
else|else
name|ret
operator|->
name|data
operator|=
name|NULL
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|is_local
argument_list|(
name|url
argument_list|)
operator|&&
name|is_file
argument_list|(
name|url
argument_list|)
condition|)
block|{
name|struct
name|bundle_transport_data
modifier|*
name|data
init|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|data
argument_list|)
argument_list|)
decl_stmt|;
name|ret
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|ret
argument_list|)
argument_list|)
expr_stmt|;
name|ret
operator|->
name|data
operator|=
name|data
expr_stmt|;
name|ret
operator|->
name|ops
operator|=
operator|&
name|bundle_transport
expr_stmt|;
block|}
else|else
block|{
name|struct
name|git_transport_data
modifier|*
name|data
init|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|data
argument_list|)
argument_list|)
decl_stmt|;
name|ret
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|ret
argument_list|)
argument_list|)
expr_stmt|;
name|ret
operator|->
name|data
operator|=
name|data
expr_stmt|;
name|data
operator|->
name|thin
operator|=
literal|1
expr_stmt|;
name|data
operator|->
name|uploadpack
operator|=
literal|"git-upload-pack"
expr_stmt|;
if|if
condition|(
name|remote
operator|&&
name|remote
operator|->
name|uploadpack
condition|)
name|data
operator|->
name|uploadpack
operator|=
name|remote
operator|->
name|uploadpack
expr_stmt|;
name|data
operator|->
name|receivepack
operator|=
literal|"git-receive-pack"
expr_stmt|;
if|if
condition|(
name|remote
operator|&&
name|remote
operator|->
name|receivepack
condition|)
name|data
operator|->
name|receivepack
operator|=
name|remote
operator|->
name|receivepack
expr_stmt|;
name|data
operator|->
name|unpacklimit
operator|=
operator|-
literal|1
expr_stmt|;
name|ret
operator|->
name|ops
operator|=
operator|&
name|git_transport
expr_stmt|;
block|}
if|if
condition|(
name|ret
condition|)
block|{
name|ret
operator|->
name|remote
operator|=
name|remote
expr_stmt|;
name|ret
operator|->
name|url
operator|=
name|url
expr_stmt|;
name|ret
operator|->
name|remote_refs
operator|=
name|NULL
expr_stmt|;
name|ret
operator|->
name|fetch
operator|=
operator|!
operator|!
name|fetch
expr_stmt|;
name|ret
operator|->
name|pack_lockfile
operator|=
name|NULL
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|transport_set_option
name|int
name|transport_set_option
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
name|int
name|ret
init|=
literal|1
decl_stmt|;
if|if
condition|(
name|transport
operator|->
name|ops
operator|->
name|set_option
condition|)
name|ret
operator|=
name|transport
operator|->
name|ops
operator|->
name|set_option
argument_list|(
name|transport
argument_list|,
name|name
argument_list|,
name|value
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|<
literal|0
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"For '%s' option %s cannot be set to '%s'\n"
argument_list|,
name|transport
operator|->
name|url
argument_list|,
name|name
argument_list|,
name|value
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|>
literal|0
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"For '%s' option %s is ignored\n"
argument_list|,
name|transport
operator|->
name|url
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|transport_push
name|int
name|transport_push
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|refspec_nr
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspec
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
if|if
condition|(
operator|!
name|transport
operator|->
name|ops
operator|->
name|push
condition|)
return|return
literal|1
return|;
return|return
name|transport
operator|->
name|ops
operator|->
name|push
argument_list|(
name|transport
argument_list|,
name|refspec_nr
argument_list|,
name|refspec
argument_list|,
name|flags
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|transport_get_remote_refs
name|struct
name|ref
modifier|*
name|transport_get_remote_refs
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
if|if
condition|(
operator|!
name|transport
operator|->
name|remote_refs
condition|)
name|transport
operator|->
name|remote_refs
operator|=
name|transport
operator|->
name|ops
operator|->
name|get_refs_list
argument_list|(
name|transport
argument_list|)
expr_stmt|;
return|return
name|transport
operator|->
name|remote_refs
return|;
block|}
end_function

begin_function
DECL|function|transport_fetch_refs
name|int
name|transport_fetch_refs
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|struct
name|ref
modifier|*
name|refs
parameter_list|)
block|{
name|int
name|rc
decl_stmt|;
name|int
name|nr_heads
init|=
literal|0
decl_stmt|,
name|nr_alloc
init|=
literal|0
decl_stmt|;
name|struct
name|ref
modifier|*
modifier|*
name|heads
init|=
name|NULL
decl_stmt|;
name|struct
name|ref
modifier|*
name|rm
decl_stmt|;
for|for
control|(
name|rm
operator|=
name|refs
init|;
name|rm
condition|;
name|rm
operator|=
name|rm
operator|->
name|next
control|)
block|{
if|if
condition|(
name|rm
operator|->
name|peer_ref
operator|&&
operator|!
name|hashcmp
argument_list|(
name|rm
operator|->
name|peer_ref
operator|->
name|old_sha1
argument_list|,
name|rm
operator|->
name|old_sha1
argument_list|)
condition|)
continue|continue;
name|ALLOC_GROW
argument_list|(
name|heads
argument_list|,
name|nr_heads
operator|+
literal|1
argument_list|,
name|nr_alloc
argument_list|)
expr_stmt|;
name|heads
index|[
name|nr_heads
operator|++
index|]
operator|=
name|rm
expr_stmt|;
block|}
name|rc
operator|=
name|transport
operator|->
name|ops
operator|->
name|fetch
argument_list|(
name|transport
argument_list|,
name|nr_heads
argument_list|,
name|heads
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|heads
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
end_function

begin_function
DECL|function|transport_unlock_pack
name|void
name|transport_unlock_pack
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
if|if
condition|(
name|transport
operator|->
name|pack_lockfile
condition|)
block|{
name|unlink
argument_list|(
name|transport
operator|->
name|pack_lockfile
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|transport
operator|->
name|pack_lockfile
argument_list|)
expr_stmt|;
name|transport
operator|->
name|pack_lockfile
operator|=
name|NULL
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|transport_disconnect
name|int
name|transport_disconnect
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
name|int
name|ret
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|transport
operator|->
name|ops
operator|->
name|disconnect
condition|)
name|ret
operator|=
name|transport
operator|->
name|ops
operator|->
name|disconnect
argument_list|(
name|transport
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|transport
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

end_unit

