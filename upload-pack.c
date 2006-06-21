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
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_include
include|#
directive|include
file|<signal.h>
end_include

begin_include
include|#
directive|include
file|<sys/poll.h>
end_include

begin_include
include|#
directive|include
file|<sys/wait.h>
end_include

begin_decl_stmt
DECL|variable|upload_pack_usage
specifier|static
specifier|const
name|char
name|upload_pack_usage
index|[]
init|=
literal|"git-upload-pack [--strict] [--timeout=nn]<dir>"
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|THEY_HAVE
define|#
directive|define
name|THEY_HAVE
value|(1U<< 0)
end_define

begin_define
DECL|macro|OUR_REF
define|#
directive|define
name|OUR_REF
value|(1U<< 1)
end_define

begin_define
DECL|macro|WANTED
define|#
directive|define
name|WANTED
value|(1U<< 2)
end_define

begin_define
DECL|macro|MAX_HAS
define|#
directive|define
name|MAX_HAS
value|256
end_define

begin_define
DECL|macro|MAX_NEEDS
define|#
directive|define
name|MAX_NEEDS
value|256
end_define

begin_decl_stmt
DECL|variable|nr_has
DECL|variable|nr_needs
DECL|variable|multi_ack
DECL|variable|nr_our_refs
specifier|static
name|int
name|nr_has
init|=
literal|0
decl_stmt|,
name|nr_needs
init|=
literal|0
decl_stmt|,
name|multi_ack
init|=
literal|0
decl_stmt|,
name|nr_our_refs
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|use_thin_pack
specifier|static
name|int
name|use_thin_pack
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|has_sha1
specifier|static
name|unsigned
name|char
name|has_sha1
index|[
name|MAX_HAS
index|]
index|[
literal|20
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|needs_sha1
specifier|static
name|unsigned
name|char
name|needs_sha1
index|[
name|MAX_NEEDS
index|]
index|[
literal|20
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|timeout
specifier|static
name|unsigned
name|int
name|timeout
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|reset_timeout
specifier|static
name|void
name|reset_timeout
parameter_list|(
name|void
parameter_list|)
block|{
name|alarm
argument_list|(
name|timeout
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strip
specifier|static
name|int
name|strip
parameter_list|(
name|char
modifier|*
name|line
parameter_list|,
name|int
name|len
parameter_list|)
block|{
if|if
condition|(
name|len
operator|&&
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
return|return
name|len
return|;
block|}
end_function

begin_function
DECL|function|create_pack_file
specifier|static
name|void
name|create_pack_file
parameter_list|(
name|void
parameter_list|)
block|{
comment|/* Pipes between rev-list to pack-objects and pack-objects to us. */
name|int
name|lp_pipe
index|[
literal|2
index|]
decl_stmt|,
name|pu_pipe
index|[
literal|2
index|]
decl_stmt|;
name|pid_t
name|pid_rev_list
decl_stmt|,
name|pid_pack_objects
decl_stmt|;
name|int
name|create_full_pack
init|=
operator|(
name|nr_our_refs
operator|==
name|nr_needs
operator|&&
operator|!
name|nr_has
operator|)
decl_stmt|;
name|char
name|data
index|[
literal|8193
index|]
decl_stmt|;
name|int
name|buffered
init|=
operator|-
literal|1
decl_stmt|;
if|if
condition|(
name|pipe
argument_list|(
name|lp_pipe
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"git-upload-pack: unable to create pipe"
argument_list|)
expr_stmt|;
name|pid_rev_list
operator|=
name|fork
argument_list|()
expr_stmt|;
if|if
condition|(
name|pid_rev_list
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"git-upload-pack: unable to fork git-rev-list"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pid_rev_list
condition|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|args
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|;
name|char
modifier|*
modifier|*
name|p
decl_stmt|;
if|if
condition|(
name|create_full_pack
condition|)
block|{
name|args
operator|=
literal|10
expr_stmt|;
name|use_thin_pack
operator|=
literal|0
expr_stmt|;
comment|/* no point doing it */
block|}
else|else
name|args
operator|=
name|nr_has
operator|+
name|nr_needs
operator|+
literal|5
expr_stmt|;
name|p
operator|=
name|xmalloc
argument_list|(
name|args
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|argv
operator|=
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
name|p
expr_stmt|;
name|buf
operator|=
name|xmalloc
argument_list|(
name|args
operator|*
literal|45
argument_list|)
expr_stmt|;
name|dup2
argument_list|(
name|lp_pipe
index|[
literal|1
index|]
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|close
argument_list|(
literal|0
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|lp_pipe
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|lp_pipe
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
operator|*
name|p
operator|++
operator|=
literal|"rev-list"
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|use_thin_pack
condition|?
literal|"--objects-edge"
else|:
literal|"--objects"
expr_stmt|;
if|if
condition|(
name|create_full_pack
operator|||
name|MAX_NEEDS
operator|<=
name|nr_needs
condition|)
operator|*
name|p
operator|++
operator|=
literal|"--all"
expr_stmt|;
else|else
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_needs
condition|;
name|i
operator|++
control|)
block|{
operator|*
name|p
operator|++
operator|=
name|buf
expr_stmt|;
name|memcpy
argument_list|(
name|buf
argument_list|,
name|sha1_to_hex
argument_list|(
name|needs_sha1
index|[
name|i
index|]
argument_list|)
argument_list|,
literal|41
argument_list|)
expr_stmt|;
name|buf
operator|+=
literal|41
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|create_full_pack
condition|)
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_has
condition|;
name|i
operator|++
control|)
block|{
operator|*
name|p
operator|++
operator|=
name|buf
expr_stmt|;
operator|*
name|buf
operator|++
operator|=
literal|'^'
expr_stmt|;
name|memcpy
argument_list|(
name|buf
argument_list|,
name|sha1_to_hex
argument_list|(
name|has_sha1
index|[
name|i
index|]
argument_list|)
argument_list|,
literal|41
argument_list|)
expr_stmt|;
name|buf
operator|+=
literal|41
expr_stmt|;
block|}
operator|*
name|p
operator|++
operator|=
name|NULL
expr_stmt|;
name|execv_git_cmd
argument_list|(
name|argv
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"git-upload-pack: unable to exec git-rev-list"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|pipe
argument_list|(
name|pu_pipe
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"git-upload-pack: unable to create pipe"
argument_list|)
expr_stmt|;
name|pid_pack_objects
operator|=
name|fork
argument_list|()
expr_stmt|;
if|if
condition|(
name|pid_pack_objects
operator|<
literal|0
condition|)
block|{
comment|/* daemon sets things up to ignore TERM */
name|kill
argument_list|(
name|pid_rev_list
argument_list|,
name|SIGKILL
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"git-upload-pack: unable to fork git-pack-objects"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|pid_pack_objects
condition|)
block|{
name|dup2
argument_list|(
name|lp_pipe
index|[
literal|0
index|]
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|dup2
argument_list|(
name|pu_pipe
index|[
literal|1
index|]
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|lp_pipe
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|lp_pipe
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|pu_pipe
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|pu_pipe
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|execl_git_cmd
argument_list|(
literal|"pack-objects"
argument_list|,
literal|"--stdout"
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|kill
argument_list|(
name|pid_rev_list
argument_list|,
name|SIGKILL
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"git-upload-pack: unable to exec git-pack-objects"
argument_list|)
expr_stmt|;
block|}
name|close
argument_list|(
name|lp_pipe
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|lp_pipe
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
comment|/* We read from pu_pipe[0] to capture the pack data. 	 */
name|close
argument_list|(
name|pu_pipe
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
specifier|const
name|char
modifier|*
name|who
decl_stmt|;
name|struct
name|pollfd
name|pfd
index|[
literal|2
index|]
decl_stmt|;
name|pid_t
name|pid
decl_stmt|;
name|int
name|status
decl_stmt|;
name|ssize_t
name|sz
decl_stmt|;
name|int
name|pu
decl_stmt|,
name|pollsize
decl_stmt|;
name|pollsize
operator|=
literal|0
expr_stmt|;
name|pu
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|pu_pipe
index|[
literal|0
index|]
condition|)
block|{
name|pfd
index|[
name|pollsize
index|]
operator|.
name|fd
operator|=
name|pu_pipe
index|[
literal|0
index|]
expr_stmt|;
name|pfd
index|[
name|pollsize
index|]
operator|.
name|events
operator|=
name|POLLIN
expr_stmt|;
name|pu
operator|=
name|pollsize
expr_stmt|;
name|pollsize
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|pollsize
condition|)
block|{
if|if
condition|(
name|poll
argument_list|(
name|pfd
argument_list|,
name|pollsize
argument_list|,
operator|-
literal|1
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|!=
name|EINTR
condition|)
block|{
name|error
argument_list|(
literal|"poll failed, resuming: %s"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|sleep
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
continue|continue;
block|}
if|if
condition|(
literal|0
operator|<=
name|pu
operator|&&
operator|(
name|pfd
index|[
name|pu
index|]
operator|.
name|revents
operator|&
operator|(
name|POLLIN
operator||
name|POLLHUP
operator|)
operator|)
condition|)
block|{
comment|/* Data ready; we keep the last byte 				 * to ourselves in case we detect 				 * broken rev-list, so that we can 				 * leave the stream corrupted.  This 				 * is unfortunate -- unpack-objects 				 * would happily accept a valid pack 				 * data with trailing garbage, so 				 * appending garbage after we pass all 				 * the pack data is not good enough to 				 * signal breakage to downstream. 				 */
name|char
modifier|*
name|cp
init|=
name|data
decl_stmt|;
name|ssize_t
name|outsz
init|=
literal|0
decl_stmt|;
if|if
condition|(
literal|0
operator|<=
name|buffered
condition|)
block|{
operator|*
name|cp
operator|++
operator|=
name|buffered
expr_stmt|;
name|outsz
operator|++
expr_stmt|;
block|}
name|sz
operator|=
name|read
argument_list|(
name|pu_pipe
index|[
literal|0
index|]
argument_list|,
name|cp
argument_list|,
sizeof|sizeof
argument_list|(
name|data
argument_list|)
operator|-
name|outsz
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<
name|sz
condition|)
empty_stmt|;
elseif|else
if|if
condition|(
name|sz
operator|==
literal|0
condition|)
block|{
name|close
argument_list|(
name|pu_pipe
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|pu_pipe
index|[
literal|0
index|]
operator|=
operator|-
literal|1
expr_stmt|;
block|}
else|else
goto|goto
name|fail
goto|;
name|sz
operator|+=
name|outsz
expr_stmt|;
if|if
condition|(
literal|1
operator|<
name|sz
condition|)
block|{
name|buffered
operator|=
name|data
index|[
name|sz
operator|-
literal|1
index|]
operator|&
literal|0xFF
expr_stmt|;
name|sz
operator|--
expr_stmt|;
block|}
else|else
name|buffered
operator|=
operator|-
literal|1
expr_stmt|;
name|sz
operator|=
name|xwrite
argument_list|(
literal|1
argument_list|,
name|data
argument_list|,
name|sz
argument_list|)
expr_stmt|;
if|if
condition|(
name|sz
operator|<
literal|0
condition|)
goto|goto
name|fail
goto|;
block|}
block|}
comment|/* See if the children are still there */
if|if
condition|(
name|pid_rev_list
operator|||
name|pid_pack_objects
condition|)
block|{
name|pid
operator|=
name|waitpid
argument_list|(
operator|-
literal|1
argument_list|,
operator|&
name|status
argument_list|,
name|WNOHANG
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pid
condition|)
continue|continue;
name|who
operator|=
operator|(
operator|(
name|pid
operator|==
name|pid_rev_list
operator|)
condition|?
literal|"git-rev-list"
else|:
operator|(
name|pid
operator|==
name|pid_pack_objects
operator|)
condition|?
literal|"git-pack-objects"
else|:
name|NULL
operator|)
expr_stmt|;
if|if
condition|(
operator|!
name|who
condition|)
block|{
if|if
condition|(
name|pid
operator|<
literal|0
condition|)
block|{
name|error
argument_list|(
literal|"git-upload-pack: %s"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
goto|goto
name|fail
goto|;
block|}
name|error
argument_list|(
literal|"git-upload-pack: we weren't "
literal|"waiting for %d"
argument_list|,
name|pid
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|WIFEXITED
argument_list|(
name|status
argument_list|)
operator|||
name|WEXITSTATUS
argument_list|(
name|status
argument_list|)
operator|>
literal|0
condition|)
block|{
name|error
argument_list|(
literal|"git-upload-pack: %s died with error."
argument_list|,
name|who
argument_list|)
expr_stmt|;
goto|goto
name|fail
goto|;
block|}
if|if
condition|(
name|pid
operator|==
name|pid_rev_list
condition|)
name|pid_rev_list
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|pid
operator|==
name|pid_pack_objects
condition|)
name|pid_pack_objects
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|pid_rev_list
operator|||
name|pid_pack_objects
condition|)
continue|continue;
block|}
comment|/* both died happily */
if|if
condition|(
name|pollsize
condition|)
continue|continue;
comment|/* flush the data */
if|if
condition|(
literal|0
operator|<=
name|buffered
condition|)
block|{
name|data
index|[
literal|0
index|]
operator|=
name|buffered
expr_stmt|;
name|sz
operator|=
name|xwrite
argument_list|(
literal|1
argument_list|,
name|data
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|sz
operator|<
literal|0
condition|)
goto|goto
name|fail
goto|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"flushed.\n"
argument_list|)
expr_stmt|;
block|}
return|return;
block|}
name|fail
label|:
if|if
condition|(
name|pid_pack_objects
condition|)
name|kill
argument_list|(
name|pid_pack_objects
argument_list|,
name|SIGKILL
argument_list|)
expr_stmt|;
if|if
condition|(
name|pid_rev_list
condition|)
name|kill
argument_list|(
name|pid_rev_list
argument_list|,
name|SIGKILL
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"git-upload-pack: aborting due to possible repository corruption on the remote side."
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|got_sha1
specifier|static
name|int
name|got_sha1
parameter_list|(
name|char
modifier|*
name|hex
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|hex
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"git-upload-pack: expected SHA1 object, got '%s'"
argument_list|,
name|hex
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|has_sha1_file
argument_list|(
name|sha1
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|nr_has
operator|<
name|MAX_HAS
condition|)
block|{
name|struct
name|object
modifier|*
name|o
init|=
name|lookup_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|o
operator|&&
name|o
operator|->
name|parsed
operator|)
condition|)
name|o
operator|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|o
condition|)
name|die
argument_list|(
literal|"oops (%s)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|o
operator|->
name|type
operator|==
name|TYPE_COMMIT
condition|)
block|{
name|struct
name|commit_list
modifier|*
name|parents
decl_stmt|;
if|if
condition|(
name|o
operator|->
name|flags
operator|&
name|THEY_HAVE
condition|)
return|return
literal|0
return|;
name|o
operator|->
name|flags
operator||=
name|THEY_HAVE
expr_stmt|;
for|for
control|(
name|parents
operator|=
operator|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o
operator|)
operator|->
name|parents
init|;
name|parents
condition|;
name|parents
operator|=
name|parents
operator|->
name|next
control|)
name|parents
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator||=
name|THEY_HAVE
expr_stmt|;
block|}
name|memcpy
argument_list|(
name|has_sha1
index|[
name|nr_has
operator|++
index|]
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
block|}
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|get_common_commits
specifier|static
name|int
name|get_common_commits
parameter_list|(
name|void
parameter_list|)
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
name|sha1
index|[
literal|20
index|]
decl_stmt|,
name|last_sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|len
decl_stmt|;
name|track_object_refs
operator|=
literal|0
expr_stmt|;
name|save_commit_buffer
operator|=
literal|0
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
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
name|reset_timeout
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|len
condition|)
block|{
if|if
condition|(
name|nr_has
operator|==
literal|0
operator|||
name|multi_ack
condition|)
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"NAK\n"
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|len
operator|=
name|strip
argument_list|(
name|line
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|line
argument_list|,
literal|"have "
argument_list|,
literal|5
argument_list|)
condition|)
block|{
if|if
condition|(
name|got_sha1
argument_list|(
name|line
operator|+
literal|5
argument_list|,
name|sha1
argument_list|)
operator|&&
operator|(
name|multi_ack
operator|||
name|nr_has
operator|==
literal|1
operator|)
condition|)
block|{
if|if
condition|(
name|nr_has
operator|>=
name|MAX_HAS
condition|)
name|multi_ack
operator|=
literal|0
expr_stmt|;
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"ACK %s%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|multi_ack
condition|?
literal|" continue"
else|:
literal|""
argument_list|)
expr_stmt|;
if|if
condition|(
name|multi_ack
condition|)
name|memcpy
argument_list|(
name|last_sha1
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
block|}
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|line
argument_list|,
literal|"done"
argument_list|)
condition|)
block|{
if|if
condition|(
name|nr_has
operator|>
literal|0
condition|)
block|{
if|if
condition|(
name|multi_ack
condition|)
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"ACK %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|last_sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"NAK\n"
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|die
argument_list|(
literal|"git-upload-pack: expected SHA1 list, got '%s'"
argument_list|,
name|line
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|receive_needs
specifier|static
name|int
name|receive_needs
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
name|int
name|len
decl_stmt|,
name|needs
decl_stmt|;
name|needs
operator|=
literal|0
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|struct
name|object
modifier|*
name|o
decl_stmt|;
name|unsigned
name|char
name|dummy
index|[
literal|20
index|]
decl_stmt|,
modifier|*
name|sha1_buf
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
name|reset_timeout
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|len
condition|)
return|return
name|needs
return|;
name|sha1_buf
operator|=
name|dummy
expr_stmt|;
if|if
condition|(
name|needs
operator|==
name|MAX_NEEDS
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"warning: supporting only a max of %d requests. "
literal|"sending everything instead.\n"
argument_list|,
name|MAX_NEEDS
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|needs
operator|<
name|MAX_NEEDS
condition|)
name|sha1_buf
operator|=
name|needs_sha1
index|[
name|needs
index|]
expr_stmt|;
if|if
condition|(
name|strncmp
argument_list|(
literal|"want "
argument_list|,
name|line
argument_list|,
literal|5
argument_list|)
operator|||
name|get_sha1_hex
argument_list|(
name|line
operator|+
literal|5
argument_list|,
name|sha1_buf
argument_list|)
condition|)
name|die
argument_list|(
literal|"git-upload-pack: protocol error, "
literal|"expected to get sha, not '%s'"
argument_list|,
name|line
argument_list|)
expr_stmt|;
if|if
condition|(
name|strstr
argument_list|(
name|line
operator|+
literal|45
argument_list|,
literal|"multi_ack"
argument_list|)
condition|)
name|multi_ack
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|strstr
argument_list|(
name|line
operator|+
literal|45
argument_list|,
literal|"thin-pack"
argument_list|)
condition|)
name|use_thin_pack
operator|=
literal|1
expr_stmt|;
comment|/* We have sent all our refs already, and the other end 		 * should have chosen out of them; otherwise they are 		 * asking for nonsense. 		 * 		 * Hmph.  We may later want to allow "want" line that 		 * asks for something like "master~10" (symbolic)... 		 * would it make sense?  I don't know. 		 */
name|o
operator|=
name|lookup_object
argument_list|(
name|sha1_buf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|o
operator|||
operator|!
operator|(
name|o
operator|->
name|flags
operator|&
name|OUR_REF
operator|)
condition|)
name|die
argument_list|(
literal|"git-upload-pack: not our ref %s"
argument_list|,
name|line
operator|+
literal|5
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|o
operator|->
name|flags
operator|&
name|WANTED
operator|)
condition|)
block|{
name|o
operator|->
name|flags
operator||=
name|WANTED
expr_stmt|;
name|needs
operator|++
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|send_ref
specifier|static
name|int
name|send_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
specifier|static
name|char
modifier|*
name|capabilities
init|=
literal|"multi_ack thin-pack"
decl_stmt|;
name|struct
name|object
modifier|*
name|o
init|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|o
condition|)
name|die
argument_list|(
literal|"git-upload-pack: cannot find object %s:"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|capabilities
condition|)
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
name|refname
argument_list|,
literal|0
argument_list|,
name|capabilities
argument_list|)
expr_stmt|;
else|else
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
name|refname
argument_list|)
expr_stmt|;
name|capabilities
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|o
operator|->
name|flags
operator|&
name|OUR_REF
operator|)
condition|)
block|{
name|o
operator|->
name|flags
operator||=
name|OUR_REF
expr_stmt|;
name|nr_our_refs
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|o
operator|->
name|type
operator|==
name|TYPE_TAG
condition|)
block|{
name|o
operator|=
name|deref_tag
argument_list|(
name|o
argument_list|,
name|refname
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"%s %s^{}\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|o
operator|->
name|sha1
argument_list|)
argument_list|,
name|refname
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|upload_pack
specifier|static
name|int
name|upload_pack
parameter_list|(
name|void
parameter_list|)
block|{
name|reset_timeout
argument_list|()
expr_stmt|;
name|head_ref
argument_list|(
name|send_ref
argument_list|)
expr_stmt|;
name|for_each_ref
argument_list|(
name|send_ref
argument_list|)
expr_stmt|;
name|packet_flush
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|nr_needs
operator|=
name|receive_needs
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|nr_needs
condition|)
return|return
literal|0
return|;
name|get_common_commits
argument_list|()
expr_stmt|;
name|create_pack_file
argument_list|()
expr_stmt|;
return|return
literal|0
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
name|char
modifier|*
name|dir
decl_stmt|;
name|int
name|i
decl_stmt|;
name|int
name|strict
init|=
literal|0
decl_stmt|;
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
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|arg
index|[
literal|0
index|]
operator|!=
literal|'-'
condition|)
break|break;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--strict"
argument_list|)
condition|)
block|{
name|strict
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--timeout="
argument_list|,
literal|10
argument_list|)
condition|)
block|{
name|timeout
operator|=
name|atoi
argument_list|(
name|arg
operator|+
literal|10
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
literal|"--"
argument_list|)
condition|)
block|{
name|i
operator|++
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
name|i
operator|!=
name|argc
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|upload_pack_usage
argument_list|)
expr_stmt|;
name|dir
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|enter_repo
argument_list|(
name|dir
argument_list|,
name|strict
argument_list|)
condition|)
name|die
argument_list|(
literal|"'%s': unable to chdir or not a git archive"
argument_list|,
name|dir
argument_list|)
expr_stmt|;
name|upload_pack
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

