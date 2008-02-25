begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"pack.h"
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
file|"exec_cmd.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_decl_stmt
DECL|variable|receive_pack_usage
specifier|static
specifier|const
name|char
name|receive_pack_usage
index|[]
init|=
literal|"git-receive-pack<git-dir>"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|deny_non_fast_forwards
specifier|static
name|int
name|deny_non_fast_forwards
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|receive_fsck_objects
specifier|static
name|int
name|receive_fsck_objects
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|receive_unpack_limit
specifier|static
name|int
name|receive_unpack_limit
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|transfer_unpack_limit
specifier|static
name|int
name|transfer_unpack_limit
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|unpack_limit
specifier|static
name|int
name|unpack_limit
init|=
literal|100
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|report_status
specifier|static
name|int
name|report_status
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|capabilities
specifier|static
name|char
name|capabilities
index|[]
init|=
literal|" report-status delete-refs "
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|capabilities_sent
specifier|static
name|int
name|capabilities_sent
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|receive_pack_config
specifier|static
name|int
name|receive_pack_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
if|if
condition|(
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"receive.denynonfastforwards"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|deny_non_fast_forwards
operator|=
name|git_config_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"receive.unpacklimit"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|receive_unpack_limit
operator|=
name|git_config_int
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"transfer.unpacklimit"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|transfer_unpack_limit
operator|=
name|git_config_int
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"receive.fsckobjects"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|receive_fsck_objects
operator|=
name|git_config_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
return|return
name|git_default_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|show_ref
specifier|static
name|int
name|show_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|path
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
if|if
condition|(
name|capabilities_sent
condition|)
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"%s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|path
argument_list|)
expr_stmt|;
else|else
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"%s %s%c%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|path
argument_list|,
literal|0
argument_list|,
name|capabilities
argument_list|)
expr_stmt|;
name|capabilities_sent
operator|=
literal|1
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|write_head_info
specifier|static
name|void
name|write_head_info
parameter_list|(
name|void
parameter_list|)
block|{
name|for_each_ref
argument_list|(
name|show_ref
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|capabilities_sent
condition|)
name|show_ref
argument_list|(
literal|"capabilities^{}"
argument_list|,
name|null_sha1
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
end_function

begin_struct
DECL|struct|command
struct|struct
name|command
block|{
DECL|member|next
name|struct
name|command
modifier|*
name|next
decl_stmt|;
DECL|member|error_string
specifier|const
name|char
modifier|*
name|error_string
decl_stmt|;
DECL|member|old_sha1
name|unsigned
name|char
name|old_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|new_sha1
name|unsigned
name|char
name|new_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|ref_name
name|char
name|ref_name
index|[
name|FLEX_ARRAY
index|]
decl_stmt|;
comment|/* more */
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|commands
specifier|static
name|struct
name|command
modifier|*
name|commands
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pre_receive_hook
specifier|static
specifier|const
name|char
name|pre_receive_hook
index|[]
init|=
literal|"hooks/pre-receive"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|post_receive_hook
specifier|static
specifier|const
name|char
name|post_receive_hook
index|[]
init|=
literal|"hooks/post-receive"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|hook_status
specifier|static
name|int
name|hook_status
parameter_list|(
name|int
name|code
parameter_list|,
specifier|const
name|char
modifier|*
name|hook_name
parameter_list|)
block|{
switch|switch
condition|(
name|code
condition|)
block|{
case|case
literal|0
case|:
return|return
literal|0
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_FORK
case|:
return|return
name|error
argument_list|(
literal|"hook fork failed"
argument_list|)
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_EXEC
case|:
return|return
name|error
argument_list|(
literal|"hook execute failed"
argument_list|)
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_PIPE
case|:
return|return
name|error
argument_list|(
literal|"hook pipe failed"
argument_list|)
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID
case|:
return|return
name|error
argument_list|(
literal|"waitpid failed"
argument_list|)
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_WRONG_PID
case|:
return|return
name|error
argument_list|(
literal|"waitpid is confused"
argument_list|)
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_SIGNAL
case|:
return|return
name|error
argument_list|(
literal|"%s died of signal"
argument_list|,
name|hook_name
argument_list|)
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_NOEXIT
case|:
return|return
name|error
argument_list|(
literal|"%s died strangely"
argument_list|,
name|hook_name
argument_list|)
return|;
default|default:
name|error
argument_list|(
literal|"%s exited with error code %d"
argument_list|,
name|hook_name
argument_list|,
operator|-
name|code
argument_list|)
expr_stmt|;
return|return
operator|-
name|code
return|;
block|}
block|}
end_function

begin_function
DECL|function|run_hook
specifier|static
name|int
name|run_hook
parameter_list|(
specifier|const
name|char
modifier|*
name|hook_name
parameter_list|)
block|{
specifier|static
name|char
name|buf
index|[
sizeof|sizeof
argument_list|(
name|commands
operator|->
name|old_sha1
argument_list|)
operator|*
literal|2
operator|+
name|PATH_MAX
operator|+
literal|4
index|]
decl_stmt|;
name|struct
name|command
modifier|*
name|cmd
decl_stmt|;
name|struct
name|child_process
name|proc
decl_stmt|;
specifier|const
name|char
modifier|*
name|argv
index|[
literal|2
index|]
decl_stmt|;
name|int
name|have_input
init|=
literal|0
decl_stmt|,
name|code
decl_stmt|;
for|for
control|(
name|cmd
operator|=
name|commands
init|;
operator|!
name|have_input
operator|&&
name|cmd
condition|;
name|cmd
operator|=
name|cmd
operator|->
name|next
control|)
block|{
if|if
condition|(
operator|!
name|cmd
operator|->
name|error_string
condition|)
name|have_input
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|have_input
operator|||
name|access
argument_list|(
name|hook_name
argument_list|,
name|X_OK
argument_list|)
operator|<
literal|0
condition|)
return|return
literal|0
return|;
name|argv
index|[
literal|0
index|]
operator|=
name|hook_name
expr_stmt|;
name|argv
index|[
literal|1
index|]
operator|=
name|NULL
expr_stmt|;
name|memset
argument_list|(
operator|&
name|proc
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|proc
argument_list|)
argument_list|)
expr_stmt|;
name|proc
operator|.
name|argv
operator|=
name|argv
expr_stmt|;
name|proc
operator|.
name|in
operator|=
operator|-
literal|1
expr_stmt|;
name|proc
operator|.
name|stdout_to_stderr
operator|=
literal|1
expr_stmt|;
name|code
operator|=
name|start_command
argument_list|(
operator|&
name|proc
argument_list|)
expr_stmt|;
if|if
condition|(
name|code
condition|)
return|return
name|hook_status
argument_list|(
name|code
argument_list|,
name|hook_name
argument_list|)
return|;
for|for
control|(
name|cmd
operator|=
name|commands
init|;
name|cmd
condition|;
name|cmd
operator|=
name|cmd
operator|->
name|next
control|)
block|{
if|if
condition|(
operator|!
name|cmd
operator|->
name|error_string
condition|)
block|{
name|size_t
name|n
init|=
name|snprintf
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
literal|"%s %s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|cmd
operator|->
name|old_sha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|cmd
operator|->
name|new_sha1
argument_list|)
argument_list|,
name|cmd
operator|->
name|ref_name
argument_list|)
decl_stmt|;
if|if
condition|(
name|write_in_full
argument_list|(
name|proc
operator|.
name|in
argument_list|,
name|buf
argument_list|,
name|n
argument_list|)
operator|!=
name|n
condition|)
break|break;
block|}
block|}
return|return
name|hook_status
argument_list|(
name|finish_command
argument_list|(
operator|&
name|proc
argument_list|)
argument_list|,
name|hook_name
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|run_update_hook
specifier|static
name|int
name|run_update_hook
parameter_list|(
name|struct
name|command
modifier|*
name|cmd
parameter_list|)
block|{
specifier|static
specifier|const
name|char
name|update_hook
index|[]
init|=
literal|"hooks/update"
decl_stmt|;
name|struct
name|child_process
name|proc
decl_stmt|;
specifier|const
name|char
modifier|*
name|argv
index|[
literal|5
index|]
decl_stmt|;
if|if
condition|(
name|access
argument_list|(
name|update_hook
argument_list|,
name|X_OK
argument_list|)
operator|<
literal|0
condition|)
return|return
literal|0
return|;
name|argv
index|[
literal|0
index|]
operator|=
name|update_hook
expr_stmt|;
name|argv
index|[
literal|1
index|]
operator|=
name|cmd
operator|->
name|ref_name
expr_stmt|;
name|argv
index|[
literal|2
index|]
operator|=
name|sha1_to_hex
argument_list|(
name|cmd
operator|->
name|old_sha1
argument_list|)
expr_stmt|;
name|argv
index|[
literal|3
index|]
operator|=
name|sha1_to_hex
argument_list|(
name|cmd
operator|->
name|new_sha1
argument_list|)
expr_stmt|;
name|argv
index|[
literal|4
index|]
operator|=
name|NULL
expr_stmt|;
name|memset
argument_list|(
operator|&
name|proc
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|proc
argument_list|)
argument_list|)
expr_stmt|;
name|proc
operator|.
name|argv
operator|=
name|argv
expr_stmt|;
name|proc
operator|.
name|no_stdin
operator|=
literal|1
expr_stmt|;
name|proc
operator|.
name|stdout_to_stderr
operator|=
literal|1
expr_stmt|;
return|return
name|hook_status
argument_list|(
name|run_command
argument_list|(
operator|&
name|proc
argument_list|)
argument_list|,
name|update_hook
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|update
specifier|static
specifier|const
name|char
modifier|*
name|update
parameter_list|(
name|struct
name|command
modifier|*
name|cmd
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|name
init|=
name|cmd
operator|->
name|ref_name
decl_stmt|;
name|unsigned
name|char
modifier|*
name|old_sha1
init|=
name|cmd
operator|->
name|old_sha1
decl_stmt|;
name|unsigned
name|char
modifier|*
name|new_sha1
init|=
name|cmd
operator|->
name|new_sha1
decl_stmt|;
name|struct
name|ref_lock
modifier|*
name|lock
decl_stmt|;
comment|/* only refs/... are allowed */
if|if
condition|(
name|prefixcmp
argument_list|(
name|name
argument_list|,
literal|"refs/"
argument_list|)
operator|||
name|check_ref_format
argument_list|(
name|name
operator|+
literal|5
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"refusing to create funny ref '%s' remotely"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
literal|"funny refname"
return|;
block|}
if|if
condition|(
operator|!
name|is_null_sha1
argument_list|(
name|new_sha1
argument_list|)
operator|&&
operator|!
name|has_sha1_file
argument_list|(
name|new_sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"unpack should have generated %s, "
literal|"but I can't find it!"
argument_list|,
name|sha1_to_hex
argument_list|(
name|new_sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|"bad pack"
return|;
block|}
if|if
condition|(
name|deny_non_fast_forwards
operator|&&
operator|!
name|is_null_sha1
argument_list|(
name|new_sha1
argument_list|)
operator|&&
operator|!
name|is_null_sha1
argument_list|(
name|old_sha1
argument_list|)
operator|&&
operator|!
name|prefixcmp
argument_list|(
name|name
argument_list|,
literal|"refs/heads/"
argument_list|)
condition|)
block|{
name|struct
name|object
modifier|*
name|old_object
decl_stmt|,
modifier|*
name|new_object
decl_stmt|;
name|struct
name|commit
modifier|*
name|old_commit
decl_stmt|,
modifier|*
name|new_commit
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|bases
decl_stmt|,
modifier|*
name|ent
decl_stmt|;
name|old_object
operator|=
name|parse_object
argument_list|(
name|old_sha1
argument_list|)
expr_stmt|;
name|new_object
operator|=
name|parse_object
argument_list|(
name|new_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|old_object
operator|||
operator|!
name|new_object
operator|||
name|old_object
operator|->
name|type
operator|!=
name|OBJ_COMMIT
operator|||
name|new_object
operator|->
name|type
operator|!=
name|OBJ_COMMIT
condition|)
block|{
name|error
argument_list|(
literal|"bad sha1 objects for %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
literal|"bad ref"
return|;
block|}
name|old_commit
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|old_object
expr_stmt|;
name|new_commit
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|new_object
expr_stmt|;
name|bases
operator|=
name|get_merge_bases
argument_list|(
name|old_commit
argument_list|,
name|new_commit
argument_list|,
literal|1
argument_list|)
expr_stmt|;
for|for
control|(
name|ent
operator|=
name|bases
init|;
name|ent
condition|;
name|ent
operator|=
name|ent
operator|->
name|next
control|)
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|old_sha1
argument_list|,
name|ent
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
break|break;
name|free_commit_list
argument_list|(
name|bases
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ent
condition|)
block|{
name|error
argument_list|(
literal|"denying non-fast forward %s"
literal|" (you should pull first)"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
literal|"non-fast forward"
return|;
block|}
block|}
if|if
condition|(
name|run_update_hook
argument_list|(
name|cmd
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"hook declined to update %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
literal|"hook declined"
return|;
block|}
if|if
condition|(
name|is_null_sha1
argument_list|(
name|new_sha1
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|parse_object
argument_list|(
name|old_sha1
argument_list|)
condition|)
block|{
name|warning
argument_list|(
literal|"Allowing deletion of corrupt ref."
argument_list|)
expr_stmt|;
name|old_sha1
operator|=
name|NULL
expr_stmt|;
block|}
if|if
condition|(
name|delete_ref
argument_list|(
name|name
argument_list|,
name|old_sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"failed to delete %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
literal|"failed to delete"
return|;
block|}
return|return
name|NULL
return|;
comment|/* good */
block|}
else|else
block|{
name|lock
operator|=
name|lock_any_ref_for_update
argument_list|(
name|name
argument_list|,
name|old_sha1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lock
condition|)
block|{
name|error
argument_list|(
literal|"failed to lock %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
literal|"failed to lock"
return|;
block|}
if|if
condition|(
name|write_ref_sha1
argument_list|(
name|lock
argument_list|,
name|new_sha1
argument_list|,
literal|"push"
argument_list|)
condition|)
block|{
return|return
literal|"failed to write"
return|;
comment|/* error() already called */
block|}
return|return
name|NULL
return|;
comment|/* good */
block|}
block|}
end_function

begin_decl_stmt
DECL|variable|update_post_hook
specifier|static
name|char
name|update_post_hook
index|[]
init|=
literal|"hooks/post-update"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|run_update_post_hook
specifier|static
name|void
name|run_update_post_hook
parameter_list|(
name|struct
name|command
modifier|*
name|cmd
parameter_list|)
block|{
name|struct
name|command
modifier|*
name|cmd_p
decl_stmt|;
name|int
name|argc
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
for|for
control|(
name|argc
operator|=
literal|0
operator|,
name|cmd_p
operator|=
name|cmd
init|;
name|cmd_p
condition|;
name|cmd_p
operator|=
name|cmd_p
operator|->
name|next
control|)
block|{
if|if
condition|(
name|cmd_p
operator|->
name|error_string
condition|)
continue|continue;
name|argc
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|argc
operator|||
name|access
argument_list|(
name|update_post_hook
argument_list|,
name|X_OK
argument_list|)
operator|<
literal|0
condition|)
return|return;
name|argv
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|argv
argument_list|)
operator|*
operator|(
literal|2
operator|+
name|argc
operator|)
argument_list|)
expr_stmt|;
name|argv
index|[
literal|0
index|]
operator|=
name|update_post_hook
expr_stmt|;
for|for
control|(
name|argc
operator|=
literal|1
operator|,
name|cmd_p
operator|=
name|cmd
init|;
name|cmd_p
condition|;
name|cmd_p
operator|=
name|cmd_p
operator|->
name|next
control|)
block|{
name|char
modifier|*
name|p
decl_stmt|;
if|if
condition|(
name|cmd_p
operator|->
name|error_string
condition|)
continue|continue;
name|p
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|cmd_p
operator|->
name|ref_name
argument_list|)
operator|+
literal|1
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|p
argument_list|,
name|cmd_p
operator|->
name|ref_name
argument_list|)
expr_stmt|;
name|argv
index|[
name|argc
index|]
operator|=
name|p
expr_stmt|;
name|argc
operator|++
expr_stmt|;
block|}
name|argv
index|[
name|argc
index|]
operator|=
name|NULL
expr_stmt|;
name|run_command_v_opt
argument_list|(
name|argv
argument_list|,
name|RUN_COMMAND_NO_STDIN
operator||
name|RUN_COMMAND_STDOUT_TO_STDERR
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|execute_commands
specifier|static
name|void
name|execute_commands
parameter_list|(
specifier|const
name|char
modifier|*
name|unpacker_error
parameter_list|)
block|{
name|struct
name|command
modifier|*
name|cmd
init|=
name|commands
decl_stmt|;
if|if
condition|(
name|unpacker_error
condition|)
block|{
while|while
condition|(
name|cmd
condition|)
block|{
name|cmd
operator|->
name|error_string
operator|=
literal|"n/a (unpacker error)"
expr_stmt|;
name|cmd
operator|=
name|cmd
operator|->
name|next
expr_stmt|;
block|}
return|return;
block|}
if|if
condition|(
name|run_hook
argument_list|(
name|pre_receive_hook
argument_list|)
condition|)
block|{
while|while
condition|(
name|cmd
condition|)
block|{
name|cmd
operator|->
name|error_string
operator|=
literal|"pre-receive hook declined"
expr_stmt|;
name|cmd
operator|=
name|cmd
operator|->
name|next
expr_stmt|;
block|}
return|return;
block|}
while|while
condition|(
name|cmd
condition|)
block|{
name|cmd
operator|->
name|error_string
operator|=
name|update
argument_list|(
name|cmd
argument_list|)
expr_stmt|;
name|cmd
operator|=
name|cmd
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|read_head_info
specifier|static
name|void
name|read_head_info
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|command
modifier|*
modifier|*
name|p
init|=
operator|&
name|commands
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
specifier|static
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
name|unsigned
name|char
name|old_sha1
index|[
literal|20
index|]
decl_stmt|,
name|new_sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|command
modifier|*
name|cmd
decl_stmt|;
name|char
modifier|*
name|refname
decl_stmt|;
name|int
name|len
decl_stmt|,
name|reflen
decl_stmt|;
name|len
operator|=
name|packet_read_line
argument_list|(
literal|0
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
name|line
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|line
index|[
operator|--
name|len
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|len
operator|<
literal|83
operator|||
name|line
index|[
literal|40
index|]
operator|!=
literal|' '
operator|||
name|line
index|[
literal|81
index|]
operator|!=
literal|' '
operator|||
name|get_sha1_hex
argument_list|(
name|line
argument_list|,
name|old_sha1
argument_list|)
operator|||
name|get_sha1_hex
argument_list|(
name|line
operator|+
literal|41
argument_list|,
name|new_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"protocol error: expected old/new/ref, got '%s'"
argument_list|,
name|line
argument_list|)
expr_stmt|;
name|refname
operator|=
name|line
operator|+
literal|82
expr_stmt|;
name|reflen
operator|=
name|strlen
argument_list|(
name|refname
argument_list|)
expr_stmt|;
if|if
condition|(
name|reflen
operator|+
literal|82
operator|<
name|len
condition|)
block|{
if|if
condition|(
name|strstr
argument_list|(
name|refname
operator|+
name|reflen
operator|+
literal|1
argument_list|,
literal|"report-status"
argument_list|)
condition|)
name|report_status
operator|=
literal|1
expr_stmt|;
block|}
name|cmd
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|command
argument_list|)
operator|+
name|len
operator|-
literal|80
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|cmd
operator|->
name|old_sha1
argument_list|,
name|old_sha1
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|cmd
operator|->
name|new_sha1
argument_list|,
name|new_sha1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|cmd
operator|->
name|ref_name
argument_list|,
name|line
operator|+
literal|82
argument_list|,
name|len
operator|-
literal|81
argument_list|)
expr_stmt|;
name|cmd
operator|->
name|error_string
operator|=
name|NULL
expr_stmt|;
name|cmd
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
operator|*
name|p
operator|=
name|cmd
expr_stmt|;
name|p
operator|=
operator|&
name|cmd
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|parse_pack_header
specifier|static
specifier|const
name|char
modifier|*
name|parse_pack_header
parameter_list|(
name|struct
name|pack_header
modifier|*
name|hdr
parameter_list|)
block|{
switch|switch
condition|(
name|read_pack_header
argument_list|(
literal|0
argument_list|,
name|hdr
argument_list|)
condition|)
block|{
case|case
name|PH_ERROR_EOF
case|:
return|return
literal|"eof before pack header was fully read"
return|;
case|case
name|PH_ERROR_PACK_SIGNATURE
case|:
return|return
literal|"protocol error (pack signature mismatch detected)"
return|;
case|case
name|PH_ERROR_PROTOCOL
case|:
return|return
literal|"protocol error (pack version unsupported)"
return|;
default|default:
return|return
literal|"unknown error in parse_pack_header"
return|;
case|case
literal|0
case|:
return|return
name|NULL
return|;
block|}
block|}
end_function

begin_decl_stmt
DECL|variable|pack_lockfile
specifier|static
specifier|const
name|char
modifier|*
name|pack_lockfile
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|unpack
specifier|static
specifier|const
name|char
modifier|*
name|unpack
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|pack_header
name|hdr
decl_stmt|;
specifier|const
name|char
modifier|*
name|hdr_err
decl_stmt|;
name|char
name|hdr_arg
index|[
literal|38
index|]
decl_stmt|;
name|hdr_err
operator|=
name|parse_pack_header
argument_list|(
operator|&
name|hdr
argument_list|)
expr_stmt|;
if|if
condition|(
name|hdr_err
condition|)
return|return
name|hdr_err
return|;
name|snprintf
argument_list|(
name|hdr_arg
argument_list|,
sizeof|sizeof
argument_list|(
name|hdr_arg
argument_list|)
argument_list|,
literal|"--pack_header=%u,%u"
argument_list|,
name|ntohl
argument_list|(
name|hdr
operator|.
name|hdr_version
argument_list|)
argument_list|,
name|ntohl
argument_list|(
name|hdr
operator|.
name|hdr_entries
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ntohl
argument_list|(
name|hdr
operator|.
name|hdr_entries
argument_list|)
operator|<
name|unpack_limit
condition|)
block|{
name|int
name|code
decl_stmt|,
name|i
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|unpacker
index|[
literal|4
index|]
decl_stmt|;
name|unpacker
index|[
name|i
operator|++
index|]
operator|=
literal|"unpack-objects"
expr_stmt|;
if|if
condition|(
name|receive_fsck_objects
condition|)
name|unpacker
index|[
name|i
operator|++
index|]
operator|=
literal|"--strict"
expr_stmt|;
name|unpacker
index|[
name|i
operator|++
index|]
operator|=
name|hdr_arg
expr_stmt|;
name|unpacker
index|[
name|i
operator|++
index|]
operator|=
name|NULL
expr_stmt|;
name|code
operator|=
name|run_command_v_opt
argument_list|(
name|unpacker
argument_list|,
name|RUN_GIT_CMD
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|code
condition|)
block|{
case|case
literal|0
case|:
return|return
name|NULL
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_FORK
case|:
return|return
literal|"unpack fork failed"
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_EXEC
case|:
return|return
literal|"unpack execute failed"
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID
case|:
return|return
literal|"waitpid failed"
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_WRONG_PID
case|:
return|return
literal|"waitpid is confused"
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_SIGNAL
case|:
return|return
literal|"unpacker died of signal"
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_NOEXIT
case|:
return|return
literal|"unpacker died strangely"
return|;
default|default:
return|return
literal|"unpacker exited with error code"
return|;
block|}
block|}
else|else
block|{
specifier|const
name|char
modifier|*
name|keeper
index|[
literal|7
index|]
decl_stmt|;
name|int
name|s
decl_stmt|,
name|status
decl_stmt|,
name|i
init|=
literal|0
decl_stmt|;
name|char
name|keep_arg
index|[
literal|256
index|]
decl_stmt|;
name|struct
name|child_process
name|ip
decl_stmt|;
name|s
operator|=
name|sprintf
argument_list|(
name|keep_arg
argument_list|,
literal|"--keep=receive-pack %i on "
argument_list|,
name|getpid
argument_list|()
argument_list|)
expr_stmt|;
if|if
condition|(
name|gethostname
argument_list|(
name|keep_arg
operator|+
name|s
argument_list|,
sizeof|sizeof
argument_list|(
name|keep_arg
argument_list|)
operator|-
name|s
argument_list|)
condition|)
name|strcpy
argument_list|(
name|keep_arg
operator|+
name|s
argument_list|,
literal|"localhost"
argument_list|)
expr_stmt|;
name|keeper
index|[
name|i
operator|++
index|]
operator|=
literal|"index-pack"
expr_stmt|;
name|keeper
index|[
name|i
operator|++
index|]
operator|=
literal|"--stdin"
expr_stmt|;
if|if
condition|(
name|receive_fsck_objects
condition|)
name|keeper
index|[
name|i
operator|++
index|]
operator|=
literal|"--strict"
expr_stmt|;
name|keeper
index|[
name|i
operator|++
index|]
operator|=
literal|"--fix-thin"
expr_stmt|;
name|keeper
index|[
name|i
operator|++
index|]
operator|=
name|hdr_arg
expr_stmt|;
name|keeper
index|[
name|i
operator|++
index|]
operator|=
name|keep_arg
expr_stmt|;
name|keeper
index|[
name|i
operator|++
index|]
operator|=
name|NULL
expr_stmt|;
name|memset
argument_list|(
operator|&
name|ip
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|ip
argument_list|)
argument_list|)
expr_stmt|;
name|ip
operator|.
name|argv
operator|=
name|keeper
expr_stmt|;
name|ip
operator|.
name|out
operator|=
operator|-
literal|1
expr_stmt|;
name|ip
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
name|ip
argument_list|)
condition|)
return|return
literal|"index-pack fork failed"
return|;
name|pack_lockfile
operator|=
name|index_pack_lockfile
argument_list|(
name|ip
operator|.
name|out
argument_list|)
expr_stmt|;
name|status
operator|=
name|finish_command
argument_list|(
operator|&
name|ip
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|status
condition|)
block|{
name|reprepare_packed_git
argument_list|()
expr_stmt|;
return|return
name|NULL
return|;
block|}
return|return
literal|"index-pack abnormal exit"
return|;
block|}
block|}
end_function

begin_function
DECL|function|report
specifier|static
name|void
name|report
parameter_list|(
specifier|const
name|char
modifier|*
name|unpack_status
parameter_list|)
block|{
name|struct
name|command
modifier|*
name|cmd
decl_stmt|;
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"unpack %s\n"
argument_list|,
name|unpack_status
condition|?
name|unpack_status
else|:
literal|"ok"
argument_list|)
expr_stmt|;
for|for
control|(
name|cmd
operator|=
name|commands
init|;
name|cmd
condition|;
name|cmd
operator|=
name|cmd
operator|->
name|next
control|)
block|{
if|if
condition|(
operator|!
name|cmd
operator|->
name|error_string
condition|)
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"ok %s\n"
argument_list|,
name|cmd
operator|->
name|ref_name
argument_list|)
expr_stmt|;
else|else
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"ng %s %s\n"
argument_list|,
name|cmd
operator|->
name|ref_name
argument_list|,
name|cmd
operator|->
name|error_string
argument_list|)
expr_stmt|;
block|}
name|packet_flush
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|delete_only
specifier|static
name|int
name|delete_only
parameter_list|(
name|struct
name|command
modifier|*
name|cmd
parameter_list|)
block|{
while|while
condition|(
name|cmd
condition|)
block|{
if|if
condition|(
operator|!
name|is_null_sha1
argument_list|(
name|cmd
operator|->
name|new_sha1
argument_list|)
condition|)
return|return
literal|0
return|;
name|cmd
operator|=
name|cmd
operator|->
name|next
expr_stmt|;
block|}
return|return
literal|1
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
decl_stmt|;
name|char
modifier|*
name|dir
init|=
name|NULL
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
control|)
block|{
name|char
modifier|*
name|arg
init|=
operator|*
name|argv
operator|++
decl_stmt|;
if|if
condition|(
operator|*
name|arg
operator|==
literal|'-'
condition|)
block|{
comment|/* Do flag handling here */
name|usage
argument_list|(
name|receive_pack_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|dir
condition|)
name|usage
argument_list|(
name|receive_pack_usage
argument_list|)
expr_stmt|;
name|dir
operator|=
name|arg
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|dir
condition|)
name|usage
argument_list|(
name|receive_pack_usage
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|enter_repo
argument_list|(
name|dir
argument_list|,
literal|0
argument_list|)
condition|)
name|die
argument_list|(
literal|"'%s': unable to chdir or not a git archive"
argument_list|,
name|dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|is_repository_shallow
argument_list|()
condition|)
name|die
argument_list|(
literal|"attempt to push into a shallow repository"
argument_list|)
expr_stmt|;
name|git_config
argument_list|(
name|receive_pack_config
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|transfer_unpack_limit
condition|)
name|unpack_limit
operator|=
name|transfer_unpack_limit
expr_stmt|;
elseif|else
if|if
condition|(
literal|0
operator|<=
name|receive_unpack_limit
condition|)
name|unpack_limit
operator|=
name|receive_unpack_limit
expr_stmt|;
name|write_head_info
argument_list|()
expr_stmt|;
comment|/* EOF */
name|packet_flush
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|read_head_info
argument_list|()
expr_stmt|;
if|if
condition|(
name|commands
condition|)
block|{
specifier|const
name|char
modifier|*
name|unpack_status
init|=
name|NULL
decl_stmt|;
if|if
condition|(
operator|!
name|delete_only
argument_list|(
name|commands
argument_list|)
condition|)
name|unpack_status
operator|=
name|unpack
argument_list|()
expr_stmt|;
name|execute_commands
argument_list|(
name|unpack_status
argument_list|)
expr_stmt|;
if|if
condition|(
name|pack_lockfile
condition|)
name|unlink
argument_list|(
name|pack_lockfile
argument_list|)
expr_stmt|;
if|if
condition|(
name|report_status
condition|)
name|report
argument_list|(
name|unpack_status
argument_list|)
expr_stmt|;
name|run_hook
argument_list|(
name|post_receive_hook
argument_list|)
expr_stmt|;
name|run_update_post_hook
argument_list|(
name|commands
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

