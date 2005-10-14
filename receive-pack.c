begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
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
file|<sys/wait.h>
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
DECL|variable|unpacker
specifier|static
specifier|const
name|char
name|unpacker
index|[]
init|=
literal|"git-unpack-objects"
decl_stmt|;
end_decl_stmt

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
parameter_list|)
block|{
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
DECL|member|updated
name|unsigned
name|char
name|updated
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
literal|0
index|]
decl_stmt|;
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
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|is_all_zeroes
specifier|static
name|int
name|is_all_zeroes
parameter_list|(
specifier|const
name|char
modifier|*
name|hex
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
literal|40
condition|;
name|i
operator|++
control|)
if|if
condition|(
operator|*
name|hex
operator|++
operator|!=
literal|'0'
condition|)
return|return
literal|0
return|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|verify_old_ref
specifier|static
name|int
name|verify_old_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|char
modifier|*
name|hex_contents
parameter_list|)
block|{
name|int
name|fd
decl_stmt|,
name|ret
decl_stmt|;
name|char
name|buffer
index|[
literal|60
index|]
decl_stmt|;
if|if
condition|(
name|is_all_zeroes
argument_list|(
name|hex_contents
argument_list|)
condition|)
return|return
literal|0
return|;
name|fd
operator|=
name|open
argument_list|(
name|name
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
name|ret
operator|=
name|read
argument_list|(
name|fd
argument_list|,
name|buffer
argument_list|,
literal|40
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|!=
literal|40
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|memcmp
argument_list|(
name|buffer
argument_list|,
name|hex_contents
argument_list|,
literal|40
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

begin_decl_stmt
DECL|variable|update_hook
specifier|static
name|char
name|update_hook
index|[]
init|=
literal|"hooks/update"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|run_update_hook
specifier|static
name|int
name|run_update_hook
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|char
modifier|*
name|old_hex
parameter_list|,
name|char
modifier|*
name|new_hex
parameter_list|)
block|{
name|int
name|code
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
name|code
operator|=
name|run_command
argument_list|(
name|update_hook
argument_list|,
name|refname
argument_list|,
name|old_hex
argument_list|,
name|new_hex
argument_list|,
name|NULL
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
literal|0
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_FORK
case|:
name|die
argument_list|(
literal|"hook fork failed"
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_EXEC
case|:
name|die
argument_list|(
literal|"hook execute failed"
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID
case|:
name|die
argument_list|(
literal|"waitpid failed"
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_WRONG_PID
case|:
name|die
argument_list|(
literal|"waitpid is confused"
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_SIGNAL
case|:
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s died of signal"
argument_list|,
name|update_hook
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_NOEXIT
case|:
name|die
argument_list|(
literal|"%s died strangely"
argument_list|,
name|update_hook
argument_list|)
expr_stmt|;
default|default:
name|error
argument_list|(
literal|"%s exited with error code %d"
argument_list|,
name|update_hook
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
DECL|function|update
specifier|static
name|int
name|update
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|)
block|{
name|char
name|new_hex
index|[
literal|60
index|]
decl_stmt|,
modifier|*
name|old_hex
decl_stmt|,
modifier|*
name|lock_name
decl_stmt|;
name|int
name|newfd
decl_stmt|,
name|namelen
decl_stmt|,
name|written
decl_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|name
argument_list|,
literal|"refs/"
argument_list|,
literal|5
argument_list|)
operator|&&
name|check_ref_format
argument_list|(
name|name
operator|+
literal|5
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"refusing to create funny ref '%s' locally"
argument_list|,
name|name
argument_list|)
return|;
name|namelen
operator|=
name|strlen
argument_list|(
name|name
argument_list|)
expr_stmt|;
name|lock_name
operator|=
name|xmalloc
argument_list|(
name|namelen
operator|+
literal|10
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|lock_name
argument_list|,
name|name
argument_list|,
name|namelen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|lock_name
operator|+
name|namelen
argument_list|,
literal|".lock"
argument_list|,
literal|6
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|new_hex
argument_list|,
name|sha1_to_hex
argument_list|(
name|new_sha1
argument_list|)
argument_list|)
expr_stmt|;
name|old_hex
operator|=
name|sha1_to_hex
argument_list|(
name|old_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|has_sha1_file
argument_list|(
name|new_sha1
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"unpack should have generated %s, "
literal|"but I can't find it!"
argument_list|,
name|new_hex
argument_list|)
return|;
name|safe_create_leading_directories
argument_list|(
name|lock_name
argument_list|)
expr_stmt|;
name|newfd
operator|=
name|open
argument_list|(
name|lock_name
argument_list|,
name|O_CREAT
operator||
name|O_EXCL
operator||
name|O_WRONLY
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
if|if
condition|(
name|newfd
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"unable to create %s (%s)"
argument_list|,
name|lock_name
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
comment|/* Write the ref with an ending '\n' */
name|new_hex
index|[
literal|40
index|]
operator|=
literal|'\n'
expr_stmt|;
name|new_hex
index|[
literal|41
index|]
operator|=
literal|0
expr_stmt|;
name|written
operator|=
name|write
argument_list|(
name|newfd
argument_list|,
name|new_hex
argument_list|,
literal|41
argument_list|)
expr_stmt|;
comment|/* Remove the '\n' again */
name|new_hex
index|[
literal|40
index|]
operator|=
literal|0
expr_stmt|;
name|close
argument_list|(
name|newfd
argument_list|)
expr_stmt|;
if|if
condition|(
name|written
operator|!=
literal|41
condition|)
block|{
name|unlink
argument_list|(
name|lock_name
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"unable to write %s"
argument_list|,
name|lock_name
argument_list|)
return|;
block|}
if|if
condition|(
name|verify_old_ref
argument_list|(
name|name
argument_list|,
name|old_hex
argument_list|)
operator|<
literal|0
condition|)
block|{
name|unlink
argument_list|(
name|lock_name
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"%s changed during push"
argument_list|,
name|name
argument_list|)
return|;
block|}
if|if
condition|(
name|run_update_hook
argument_list|(
name|name
argument_list|,
name|old_hex
argument_list|,
name|new_hex
argument_list|)
condition|)
block|{
name|unlink
argument_list|(
name|lock_name
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"hook declined to update %s\n"
argument_list|,
name|name
argument_list|)
return|;
block|}
elseif|else
if|if
condition|(
name|rename
argument_list|(
name|lock_name
argument_list|,
name|name
argument_list|)
operator|<
literal|0
condition|)
block|{
name|unlink
argument_list|(
name|lock_name
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"unable to replace %s"
argument_list|,
name|name
argument_list|)
return|;
block|}
else|else
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s: %s -> %s\n"
argument_list|,
name|name
argument_list|,
name|old_hex
argument_list|,
name|new_hex
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
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
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
if|if
condition|(
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
if|if
condition|(
operator|!
name|cmd_p
operator|->
name|updated
condition|)
continue|continue;
name|argc
operator|++
expr_stmt|;
block|}
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
literal|1
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
if|if
condition|(
operator|!
name|cmd_p
operator|->
name|updated
condition|)
continue|continue;
name|argv
index|[
name|argc
index|]
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
name|argv
index|[
name|argc
index|]
argument_list|,
name|cmd_p
operator|->
name|ref_name
argument_list|)
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
name|run_command_v
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * This gets called after(if) we've successfully  * unpacked the data payload.  */
end_comment

begin_function
DECL|function|execute_commands
specifier|static
name|void
name|execute_commands
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|command
modifier|*
name|cmd
init|=
name|commands
decl_stmt|;
while|while
condition|(
name|cmd
condition|)
block|{
name|cmd
operator|->
name|updated
operator|=
operator|!
name|update
argument_list|(
name|cmd
operator|->
name|ref_name
argument_list|,
name|cmd
operator|->
name|old_sha1
argument_list|,
name|cmd
operator|->
name|new_sha1
argument_list|)
expr_stmt|;
name|cmd
operator|=
name|cmd
operator|->
name|next
expr_stmt|;
block|}
name|run_update_post_hook
argument_list|(
name|commands
argument_list|)
expr_stmt|;
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
name|int
name|len
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
name|memcpy
argument_list|(
name|cmd
operator|->
name|old_sha1
argument_list|,
name|old_sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|cmd
operator|->
name|new_sha1
argument_list|,
name|new_sha1
argument_list|,
literal|20
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
DECL|function|unpack
specifier|static
name|void
name|unpack
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|code
init|=
name|run_command
argument_list|(
name|unpacker
argument_list|,
name|NULL
argument_list|)
decl_stmt|;
switch|switch
condition|(
name|code
condition|)
block|{
case|case
literal|0
case|:
return|return;
case|case
operator|-
name|ERR_RUN_COMMAND_FORK
case|:
name|die
argument_list|(
literal|"unpack fork failed"
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_EXEC
case|:
name|die
argument_list|(
literal|"unpack execute failed"
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID
case|:
name|die
argument_list|(
literal|"waitpid failed"
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_WRONG_PID
case|:
name|die
argument_list|(
literal|"waitpid is confused"
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_SIGNAL
case|:
name|die
argument_list|(
literal|"%s died of signal"
argument_list|,
name|unpacker
argument_list|)
expr_stmt|;
case|case
operator|-
name|ERR_RUN_COMMAND_WAITPID_NOEXIT
case|:
name|die
argument_list|(
literal|"%s died strangely"
argument_list|,
name|unpacker
argument_list|)
expr_stmt|;
default|default:
name|die
argument_list|(
literal|"%s exited with error code %d"
argument_list|,
name|unpacker
argument_list|,
operator|-
name|code
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
decl_stmt|;
specifier|const
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
specifier|const
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
comment|/* chdir to the directory. If that fails, try appending ".git" */
if|if
condition|(
name|chdir
argument_list|(
name|dir
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|chdir
argument_list|(
name|mkpath
argument_list|(
literal|"%s.git"
argument_list|,
name|dir
argument_list|)
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to cd to %s"
argument_list|,
name|dir
argument_list|)
expr_stmt|;
block|}
comment|/* If we have a ".git" directory, chdir to it */
name|chdir
argument_list|(
literal|".git"
argument_list|)
expr_stmt|;
name|putenv
argument_list|(
literal|"GIT_DIR=."
argument_list|)
expr_stmt|;
if|if
condition|(
name|access
argument_list|(
literal|"objects"
argument_list|,
name|X_OK
argument_list|)
operator|<
literal|0
operator|||
name|access
argument_list|(
literal|"refs/heads"
argument_list|,
name|X_OK
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"%s doesn't appear to be a git directory"
argument_list|,
name|dir
argument_list|)
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
name|unpack
argument_list|()
expr_stmt|;
name|execute_commands
argument_list|()
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

