begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"sigchain.h"
end_include

begin_include
include|#
directive|include
file|"connected.h"
end_include

begin_include
include|#
directive|include
file|"transport.h"
end_include

begin_function
DECL|function|check_everything_connected
name|int
name|check_everything_connected
parameter_list|(
name|sha1_iterate_fn
name|fn
parameter_list|,
name|int
name|quiet
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
return|return
name|check_everything_connected_with_transport
argument_list|(
name|fn
argument_list|,
name|quiet
argument_list|,
name|cb_data
argument_list|,
name|NULL
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * If we feed all the commits we want to verify to this command  *  *  $ git rev-list --objects --stdin --not --all  *  * and if it does not error out, that means everything reachable from  * these commits locally exists and is connected to our existing refs.  * Note that this does _not_ validate the individual objects.  *  * Returns 0 if everything is connected, non-zero otherwise.  */
end_comment

begin_function
DECL|function|check_everything_connected_real
specifier|static
name|int
name|check_everything_connected_real
parameter_list|(
name|sha1_iterate_fn
name|fn
parameter_list|,
name|int
name|quiet
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|,
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
specifier|const
name|char
modifier|*
name|shallow_file
parameter_list|)
block|{
name|struct
name|child_process
name|rev_list
init|=
name|CHILD_PROCESS_INIT
decl_stmt|;
name|char
name|commit
index|[
literal|41
index|]
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|err
init|=
literal|0
decl_stmt|;
name|struct
name|packed_git
modifier|*
name|new_pack
init|=
name|NULL
decl_stmt|;
name|size_t
name|base_len
decl_stmt|;
if|if
condition|(
name|fn
argument_list|(
name|cb_data
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
name|err
return|;
if|if
condition|(
name|transport
operator|&&
name|transport
operator|->
name|smart_options
operator|&&
name|transport
operator|->
name|smart_options
operator|->
name|self_contained_and_connected
operator|&&
name|transport
operator|->
name|pack_lockfile
operator|&&
name|strip_suffix
argument_list|(
name|transport
operator|->
name|pack_lockfile
argument_list|,
literal|".keep"
argument_list|,
operator|&
name|base_len
argument_list|)
condition|)
block|{
name|struct
name|strbuf
name|idx_file
init|=
name|STRBUF_INIT
decl_stmt|;
name|strbuf_add
argument_list|(
operator|&
name|idx_file
argument_list|,
name|transport
operator|->
name|pack_lockfile
argument_list|,
name|base_len
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|idx_file
argument_list|,
literal|".idx"
argument_list|)
expr_stmt|;
name|new_pack
operator|=
name|add_packed_git
argument_list|(
name|idx_file
operator|.
name|buf
argument_list|,
name|idx_file
operator|.
name|len
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|idx_file
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|shallow_file
condition|)
block|{
name|argv_array_push
argument_list|(
operator|&
name|rev_list
operator|.
name|args
argument_list|,
literal|"--shallow-file"
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|rev_list
operator|.
name|args
argument_list|,
name|shallow_file
argument_list|)
expr_stmt|;
block|}
name|argv_array_push
argument_list|(
operator|&
name|rev_list
operator|.
name|args
argument_list|,
literal|"rev-list"
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|rev_list
operator|.
name|args
argument_list|,
literal|"--objects"
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|rev_list
operator|.
name|args
argument_list|,
literal|"--stdin"
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|rev_list
operator|.
name|args
argument_list|,
literal|"--not"
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|rev_list
operator|.
name|args
argument_list|,
literal|"--all"
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|rev_list
operator|.
name|args
argument_list|,
literal|"--quiet"
argument_list|)
expr_stmt|;
name|rev_list
operator|.
name|git_cmd
operator|=
literal|1
expr_stmt|;
name|rev_list
operator|.
name|in
operator|=
operator|-
literal|1
expr_stmt|;
name|rev_list
operator|.
name|no_stdout
operator|=
literal|1
expr_stmt|;
name|rev_list
operator|.
name|no_stderr
operator|=
name|quiet
expr_stmt|;
if|if
condition|(
name|start_command
argument_list|(
operator|&
name|rev_list
argument_list|)
condition|)
return|return
name|error
argument_list|(
name|_
argument_list|(
literal|"Could not run 'git rev-list'"
argument_list|)
argument_list|)
return|;
name|sigchain_push
argument_list|(
name|SIGPIPE
argument_list|,
name|SIG_IGN
argument_list|)
expr_stmt|;
name|commit
index|[
literal|40
index|]
operator|=
literal|'\n'
expr_stmt|;
do|do
block|{
comment|/* 		 * If index-pack already checked that: 		 * - there are no dangling pointers in the new pack 		 * - the pack is self contained 		 * Then if the updated ref is in the new pack, then we 		 * are sure the ref is good and not sending it to 		 * rev-list for verification. 		 */
if|if
condition|(
name|new_pack
operator|&&
name|find_pack_entry_one
argument_list|(
name|sha1
argument_list|,
name|new_pack
argument_list|)
condition|)
continue|continue;
name|memcpy
argument_list|(
name|commit
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
name|write_in_full
argument_list|(
name|rev_list
operator|.
name|in
argument_list|,
name|commit
argument_list|,
literal|41
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|!=
name|EPIPE
operator|&&
name|errno
operator|!=
name|EINVAL
condition|)
name|error_errno
argument_list|(
name|_
argument_list|(
literal|"failed write to rev-list"
argument_list|)
argument_list|)
expr_stmt|;
name|err
operator|=
operator|-
literal|1
expr_stmt|;
break|break;
block|}
block|}
do|while
condition|(
operator|!
name|fn
argument_list|(
name|cb_data
argument_list|,
name|sha1
argument_list|)
condition|)
do|;
if|if
condition|(
name|close
argument_list|(
name|rev_list
operator|.
name|in
argument_list|)
condition|)
name|err
operator|=
name|error_errno
argument_list|(
name|_
argument_list|(
literal|"failed to close rev-list's stdin"
argument_list|)
argument_list|)
expr_stmt|;
name|sigchain_pop
argument_list|(
name|SIGPIPE
argument_list|)
expr_stmt|;
return|return
name|finish_command
argument_list|(
operator|&
name|rev_list
argument_list|)
operator|||
name|err
return|;
block|}
end_function

begin_function
DECL|function|check_everything_connected_with_transport
name|int
name|check_everything_connected_with_transport
parameter_list|(
name|sha1_iterate_fn
name|fn
parameter_list|,
name|int
name|quiet
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|,
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
block|{
return|return
name|check_everything_connected_real
argument_list|(
name|fn
argument_list|,
name|quiet
argument_list|,
name|cb_data
argument_list|,
name|transport
argument_list|,
name|NULL
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|check_shallow_connected
name|int
name|check_shallow_connected
parameter_list|(
name|sha1_iterate_fn
name|fn
parameter_list|,
name|int
name|quiet
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|,
specifier|const
name|char
modifier|*
name|shallow_file
parameter_list|)
block|{
return|return
name|check_everything_connected_real
argument_list|(
name|fn
argument_list|,
name|quiet
argument_list|,
name|cb_data
argument_list|,
name|NULL
argument_list|,
name|shallow_file
argument_list|)
return|;
block|}
end_function

end_unit

