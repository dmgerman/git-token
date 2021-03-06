begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
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
file|"sha1-array.h"
end_include

begin_decl_stmt
DECL|variable|fetch_pack_usage
specifier|static
specifier|const
name|char
name|fetch_pack_usage
index|[]
init|=
literal|"git fetch-pack [--all] [--stdin] [--quiet | -q] [--keep | -k] [--thin] "
literal|"[--include-tag] [--upload-pack=<git-upload-pack>] [--depth=<n>] "
literal|"[--no-progress] [--diag-url] [-v] [<host>:]<directory> [<refs>...]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|add_sought_entry
specifier|static
name|void
name|add_sought_entry
parameter_list|(
name|struct
name|ref
modifier|*
modifier|*
modifier|*
name|sought
parameter_list|,
name|int
modifier|*
name|nr
parameter_list|,
name|int
modifier|*
name|alloc
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
name|struct
name|object_id
name|oid
decl_stmt|;
if|if
condition|(
operator|!
name|get_oid_hex
argument_list|(
name|name
argument_list|,
operator|&
name|oid
argument_list|)
condition|)
block|{
if|if
condition|(
name|name
index|[
name|GIT_SHA1_HEXSZ
index|]
operator|==
literal|' '
condition|)
block|{
comment|/*<sha1><ref>, find refname */
name|name
operator|+=
name|GIT_SHA1_HEXSZ
operator|+
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|name
index|[
name|GIT_SHA1_HEXSZ
index|]
operator|==
literal|'\0'
condition|)
block|{
empty_stmt|;
comment|/*<sha1>, leave sha1 as name */
block|}
else|else
block|{
comment|/*<ref>, clear cruft from oid */
name|oidclr
argument_list|(
operator|&
name|oid
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
comment|/*<ref>, clear cruft from get_oid_hex */
name|oidclr
argument_list|(
operator|&
name|oid
argument_list|)
expr_stmt|;
block|}
name|ref
operator|=
name|alloc_ref
argument_list|(
name|name
argument_list|)
expr_stmt|;
name|oidcpy
argument_list|(
operator|&
name|ref
operator|->
name|old_oid
argument_list|,
operator|&
name|oid
argument_list|)
expr_stmt|;
operator|(
operator|*
name|nr
operator|)
operator|++
expr_stmt|;
name|ALLOC_GROW
argument_list|(
operator|*
name|sought
argument_list|,
operator|*
name|nr
argument_list|,
operator|*
name|alloc
argument_list|)
expr_stmt|;
operator|(
operator|*
name|sought
operator|)
index|[
operator|*
name|nr
operator|-
literal|1
index|]
operator|=
name|ref
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_fetch_pack
name|int
name|cmd_fetch_pack
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
name|ret
decl_stmt|;
name|struct
name|ref
modifier|*
name|ref
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
name|struct
name|ref
modifier|*
modifier|*
name|sought
init|=
name|NULL
decl_stmt|;
name|int
name|nr_sought
init|=
literal|0
decl_stmt|,
name|alloc_sought
init|=
literal|0
decl_stmt|;
name|int
name|fd
index|[
literal|2
index|]
decl_stmt|;
name|char
modifier|*
name|pack_lockfile
init|=
name|NULL
decl_stmt|;
name|char
modifier|*
modifier|*
name|pack_lockfile_ptr
init|=
name|NULL
decl_stmt|;
name|struct
name|child_process
modifier|*
name|conn
decl_stmt|;
name|struct
name|fetch_pack_args
name|args
decl_stmt|;
name|struct
name|sha1_array
name|shallow
init|=
name|SHA1_ARRAY_INIT
decl_stmt|;
name|struct
name|string_list
name|deepen_not
init|=
name|STRING_LIST_INIT_DUP
decl_stmt|;
name|packet_trace_identity
argument_list|(
literal|"fetch-pack"
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|args
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|args
argument_list|)
argument_list|)
expr_stmt|;
name|args
operator|.
name|uploadpack
operator|=
literal|"git-upload-pack"
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
operator|&&
operator|*
name|argv
index|[
name|i
index|]
operator|==
literal|'-'
condition|;
name|i
operator|++
control|)
block|{
specifier|const
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
name|skip_prefix
argument_list|(
name|arg
argument_list|,
literal|"--upload-pack="
argument_list|,
operator|&
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|uploadpack
operator|=
name|arg
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|skip_prefix
argument_list|(
name|arg
argument_list|,
literal|"--exec="
argument_list|,
operator|&
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|uploadpack
operator|=
name|arg
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"--quiet"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"-q"
argument_list|,
name|arg
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
literal|"--keep"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"-k"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|lock_pack
operator|=
name|args
operator|.
name|keep_pack
expr_stmt|;
name|args
operator|.
name|keep_pack
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
literal|"--thin"
argument_list|,
name|arg
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
literal|"--include-tag"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|include_tag
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
literal|"--all"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|fetch_all
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
literal|"--stdin"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|stdin_refs
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
literal|"--diag-url"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|diag_url
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
literal|"-v"
argument_list|,
name|arg
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
name|skip_prefix
argument_list|(
name|arg
argument_list|,
literal|"--depth="
argument_list|,
operator|&
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|depth
operator|=
name|strtol
argument_list|(
name|arg
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|skip_prefix
argument_list|(
name|arg
argument_list|,
literal|"--shallow-since="
argument_list|,
operator|&
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|deepen_since
operator|=
name|xstrdup
argument_list|(
name|arg
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|skip_prefix
argument_list|(
name|arg
argument_list|,
literal|"--shallow-exclude="
argument_list|,
operator|&
name|arg
argument_list|)
condition|)
block|{
name|string_list_append
argument_list|(
operator|&
name|deepen_not
argument_list|,
name|arg
argument_list|)
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
literal|"--deepen-relative"
argument_list|)
condition|)
block|{
name|args
operator|.
name|deepen_relative
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
literal|"--no-progress"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|no_progress
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
literal|"--stateless-rpc"
argument_list|,
name|arg
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
literal|"--lock-pack"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|lock_pack
operator|=
literal|1
expr_stmt|;
name|pack_lockfile_ptr
operator|=
operator|&
name|pack_lockfile
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"--check-self-contained-and-connected"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|check_self_contained_and_connected
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
literal|"--cloning"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|cloning
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
literal|"--update-shallow"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|args
operator|.
name|update_shallow
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|fetch_pack_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|deepen_not
operator|.
name|nr
condition|)
name|args
operator|.
name|deepen_not
operator|=
operator|&
name|deepen_not
expr_stmt|;
if|if
condition|(
name|i
operator|<
name|argc
condition|)
name|dest
operator|=
name|argv
index|[
name|i
operator|++
index|]
expr_stmt|;
else|else
name|usage
argument_list|(
name|fetch_pack_usage
argument_list|)
expr_stmt|;
comment|/* 	 * Copy refs from cmdline to growable list, then append any 	 * refs from the standard input: 	 */
for|for
control|(
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
name|add_sought_entry
argument_list|(
operator|&
name|sought
argument_list|,
operator|&
name|nr_sought
argument_list|,
operator|&
name|alloc_sought
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|args
operator|.
name|stdin_refs
condition|)
block|{
if|if
condition|(
name|args
operator|.
name|stateless_rpc
condition|)
block|{
comment|/* in stateless RPC mode we use pkt-line to read 			 * from stdin, until we get a flush packet 			 */
for|for
control|(
init|;
condition|;
control|)
block|{
name|char
modifier|*
name|line
init|=
name|packet_read_line
argument_list|(
literal|0
argument_list|,
name|NULL
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|line
condition|)
break|break;
name|add_sought_entry
argument_list|(
operator|&
name|sought
argument_list|,
operator|&
name|nr_sought
argument_list|,
operator|&
name|alloc_sought
argument_list|,
name|line
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
comment|/* read from stdin one ref per line, until EOF */
name|struct
name|strbuf
name|line
init|=
name|STRBUF_INIT
decl_stmt|;
while|while
condition|(
name|strbuf_getline_lf
argument_list|(
operator|&
name|line
argument_list|,
name|stdin
argument_list|)
operator|!=
name|EOF
condition|)
name|add_sought_entry
argument_list|(
operator|&
name|sought
argument_list|,
operator|&
name|nr_sought
argument_list|,
operator|&
name|alloc_sought
argument_list|,
name|line
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|line
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
name|int
name|flags
init|=
name|args
operator|.
name|verbose
condition|?
name|CONNECT_VERBOSE
else|:
literal|0
decl_stmt|;
if|if
condition|(
name|args
operator|.
name|diag_url
condition|)
name|flags
operator||=
name|CONNECT_DIAG_URL
expr_stmt|;
name|conn
operator|=
name|git_connect
argument_list|(
name|fd
argument_list|,
name|dest
argument_list|,
name|args
operator|.
name|uploadpack
argument_list|,
name|flags
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|conn
condition|)
return|return
name|args
operator|.
name|diag_url
condition|?
literal|0
else|:
literal|1
return|;
block|}
name|get_remote_heads
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
operator|&
name|ref
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
operator|&
name|shallow
argument_list|)
expr_stmt|;
name|ref
operator|=
name|fetch_pack
argument_list|(
operator|&
name|args
argument_list|,
name|fd
argument_list|,
name|conn
argument_list|,
name|ref
argument_list|,
name|dest
argument_list|,
name|sought
argument_list|,
name|nr_sought
argument_list|,
operator|&
name|shallow
argument_list|,
name|pack_lockfile_ptr
argument_list|)
expr_stmt|;
if|if
condition|(
name|pack_lockfile
condition|)
block|{
name|printf
argument_list|(
literal|"lock %s\n"
argument_list|,
name|pack_lockfile
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|args
operator|.
name|check_self_contained_and_connected
operator|&&
name|args
operator|.
name|self_contained_and_connected
condition|)
block|{
name|printf
argument_list|(
literal|"connectivity-ok\n"
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
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
if|if
condition|(
name|finish_connect
argument_list|(
name|conn
argument_list|)
condition|)
return|return
literal|1
return|;
name|ret
operator|=
operator|!
name|ref
expr_stmt|;
comment|/* 	 * If the heads to pull were given, we should have consumed 	 * all of them by matching the remote.  Otherwise, 'git fetch 	 * remote no-such-ref' would silently succeed without issuing 	 * an error. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_sought
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|sought
index|[
name|i
index|]
operator|||
name|sought
index|[
name|i
index|]
operator|->
name|matched
condition|)
continue|continue;
name|error
argument_list|(
literal|"no such remote ref %s"
argument_list|,
name|sought
index|[
name|i
index|]
operator|->
name|name
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
block|}
while|while
condition|(
name|ref
condition|)
block|{
name|printf
argument_list|(
literal|"%s %s\n"
argument_list|,
name|oid_to_hex
argument_list|(
operator|&
name|ref
operator|->
name|old_oid
argument_list|)
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|ref
operator|=
name|ref
operator|->
name|next
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

end_unit

