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
file|"commit.h"
end_include

begin_function
DECL|function|show_new
specifier|static
name|void
name|show_new
parameter_list|(
name|char
modifier|*
name|type
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1_new
parameter_list|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"  %s: %s\n"
argument_list|,
name|type
argument_list|,
name|find_unique_abbrev
argument_list|(
name|sha1_new
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|update_ref
specifier|static
name|int
name|update_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|action
parameter_list|,
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
name|char
modifier|*
name|oldval
parameter_list|)
block|{
name|int
name|len
decl_stmt|;
name|char
name|msg
index|[
literal|1024
index|]
decl_stmt|;
name|char
modifier|*
name|rla
init|=
name|getenv
argument_list|(
literal|"GIT_REFLOG_ACTION"
argument_list|)
decl_stmt|;
specifier|static
name|struct
name|ref_lock
modifier|*
name|lock
decl_stmt|;
if|if
condition|(
operator|!
name|rla
condition|)
name|rla
operator|=
literal|"(reflog update)"
expr_stmt|;
name|len
operator|=
name|snprintf
argument_list|(
name|msg
argument_list|,
sizeof|sizeof
argument_list|(
name|msg
argument_list|)
argument_list|,
literal|"%s: %s"
argument_list|,
name|rla
argument_list|,
name|action
argument_list|)
expr_stmt|;
if|if
condition|(
sizeof|sizeof
argument_list|(
name|msg
argument_list|)
operator|<=
name|len
condition|)
name|die
argument_list|(
literal|"insanely long action"
argument_list|)
expr_stmt|;
name|lock
operator|=
name|lock_any_ref_for_update
argument_list|(
name|refname
argument_list|,
name|oldval
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lock
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|write_ref_sha1
argument_list|(
name|lock
argument_list|,
name|sha1
argument_list|,
name|msg
argument_list|)
operator|<
literal|0
condition|)
return|return
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|update_local_ref
specifier|static
name|int
name|update_local_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|new_head
parameter_list|,
specifier|const
name|char
modifier|*
name|note
parameter_list|,
name|int
name|verbose
parameter_list|,
name|int
name|force
parameter_list|)
block|{
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|char
name|sha1_old
index|[
literal|20
index|]
decl_stmt|,
name|sha1_new
index|[
literal|20
index|]
decl_stmt|;
name|char
name|oldh
index|[
literal|41
index|]
decl_stmt|,
name|newh
index|[
literal|41
index|]
decl_stmt|;
name|struct
name|commit
modifier|*
name|current
decl_stmt|,
modifier|*
name|updated
decl_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|new_head
argument_list|,
name|sha1_new
argument_list|)
condition|)
name|die
argument_list|(
literal|"malformed object name %s"
argument_list|,
name|new_head
argument_list|)
expr_stmt|;
if|if
condition|(
name|sha1_object_info
argument_list|(
name|sha1_new
argument_list|,
name|type
argument_list|,
name|NULL
argument_list|)
condition|)
name|die
argument_list|(
literal|"object %s not found"
argument_list|,
name|new_head
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|*
name|name
condition|)
block|{
comment|/* Not storing */
if|if
condition|(
name|verbose
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"* fetched %s\n"
argument_list|,
name|note
argument_list|)
expr_stmt|;
name|show_new
argument_list|(
name|type
argument_list|,
name|sha1_new
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
if|if
condition|(
name|get_sha1
argument_list|(
name|name
argument_list|,
name|sha1_old
argument_list|)
condition|)
block|{
name|char
modifier|*
name|msg
decl_stmt|;
name|just_store
label|:
comment|/* new ref */
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|name
argument_list|,
literal|"refs/tags/"
argument_list|,
literal|10
argument_list|)
condition|)
name|msg
operator|=
literal|"storing tag"
expr_stmt|;
else|else
name|msg
operator|=
literal|"storing head"
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"* %s: storing %s\n"
argument_list|,
name|name
argument_list|,
name|note
argument_list|)
expr_stmt|;
name|show_new
argument_list|(
name|type
argument_list|,
name|sha1_new
argument_list|)
expr_stmt|;
return|return
name|update_ref
argument_list|(
name|msg
argument_list|,
name|name
argument_list|,
name|sha1_new
argument_list|,
name|NULL
argument_list|)
return|;
block|}
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|sha1_old
argument_list|,
name|sha1_new
argument_list|)
condition|)
block|{
if|if
condition|(
name|verbose
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"* %s: same as %s\n"
argument_list|,
name|name
argument_list|,
name|note
argument_list|)
expr_stmt|;
name|show_new
argument_list|(
name|type
argument_list|,
name|sha1_new
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|name
argument_list|,
literal|"refs/tags/"
argument_list|,
literal|10
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"* %s: updating with %s\n"
argument_list|,
name|name
argument_list|,
name|note
argument_list|)
expr_stmt|;
name|show_new
argument_list|(
name|type
argument_list|,
name|sha1_new
argument_list|)
expr_stmt|;
return|return
name|update_ref
argument_list|(
literal|"updating tag"
argument_list|,
name|name
argument_list|,
name|sha1_new
argument_list|,
name|NULL
argument_list|)
return|;
block|}
name|current
operator|=
name|lookup_commit_reference
argument_list|(
name|sha1_old
argument_list|)
expr_stmt|;
name|updated
operator|=
name|lookup_commit_reference
argument_list|(
name|sha1_new
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|current
operator|||
operator|!
name|updated
condition|)
goto|goto
name|just_store
goto|;
name|strcpy
argument_list|(
name|oldh
argument_list|,
name|find_unique_abbrev
argument_list|(
name|current
operator|->
name|object
operator|.
name|sha1
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|newh
argument_list|,
name|find_unique_abbrev
argument_list|(
name|sha1_new
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|in_merge_bases
argument_list|(
name|current
argument_list|,
operator|&
name|updated
argument_list|,
literal|1
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"* %s: fast forward to %s\n"
argument_list|,
name|name
argument_list|,
name|note
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"  old..new: %s..%s\n"
argument_list|,
name|oldh
argument_list|,
name|newh
argument_list|)
expr_stmt|;
return|return
name|update_ref
argument_list|(
literal|"fast forward"
argument_list|,
name|name
argument_list|,
name|sha1_new
argument_list|,
name|sha1_old
argument_list|)
return|;
block|}
if|if
condition|(
operator|!
name|force
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"* %s: not updating to non-fast forward %s\n"
argument_list|,
name|name
argument_list|,
name|note
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"  old...new: %s...%s\n"
argument_list|,
name|oldh
argument_list|,
name|newh
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"* %s: forcing update to non-fast forward %s\n"
argument_list|,
name|name
argument_list|,
name|note
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"  old...new: %s...%s\n"
argument_list|,
name|oldh
argument_list|,
name|newh
argument_list|)
expr_stmt|;
return|return
name|update_ref
argument_list|(
literal|"forced-update"
argument_list|,
name|name
argument_list|,
name|sha1_new
argument_list|,
name|sha1_old
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|append_fetch_head
specifier|static
name|int
name|append_fetch_head
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|head
parameter_list|,
specifier|const
name|char
modifier|*
name|remote
parameter_list|,
specifier|const
name|char
modifier|*
name|remote_name
parameter_list|,
specifier|const
name|char
modifier|*
name|remote_nick
parameter_list|,
specifier|const
name|char
modifier|*
name|local_name
parameter_list|,
name|int
name|not_for_merge
parameter_list|,
name|int
name|verbose
parameter_list|,
name|int
name|force
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|int
name|remote_len
decl_stmt|,
name|i
decl_stmt|,
name|note_len
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
name|note
index|[
literal|1024
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|what
decl_stmt|,
modifier|*
name|kind
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|head
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"Not a valid object name: %s"
argument_list|,
name|head
argument_list|)
return|;
name|commit
operator|=
name|lookup_commit_reference
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
condition|)
name|not_for_merge
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|remote_name
argument_list|,
literal|"HEAD"
argument_list|)
condition|)
block|{
name|kind
operator|=
literal|""
expr_stmt|;
name|what
operator|=
literal|""
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|remote_name
argument_list|,
literal|"refs/heads/"
argument_list|,
literal|11
argument_list|)
condition|)
block|{
name|kind
operator|=
literal|"branch"
expr_stmt|;
name|what
operator|=
name|remote_name
operator|+
literal|11
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|remote_name
argument_list|,
literal|"refs/tags/"
argument_list|,
literal|10
argument_list|)
condition|)
block|{
name|kind
operator|=
literal|"tag"
expr_stmt|;
name|what
operator|=
name|remote_name
operator|+
literal|10
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|remote_name
argument_list|,
literal|"refs/remotes/"
argument_list|,
literal|13
argument_list|)
condition|)
block|{
name|kind
operator|=
literal|"remote branch"
expr_stmt|;
name|what
operator|=
name|remote_name
operator|+
literal|13
expr_stmt|;
block|}
else|else
block|{
name|kind
operator|=
literal|""
expr_stmt|;
name|what
operator|=
name|remote_name
expr_stmt|;
block|}
name|remote_len
operator|=
name|strlen
argument_list|(
name|remote
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
name|remote_len
operator|-
literal|1
init|;
name|remote
index|[
name|i
index|]
operator|==
literal|'/'
operator|&&
literal|0
operator|<=
name|i
condition|;
name|i
operator|--
control|)
empty_stmt|;
name|remote_len
operator|=
name|i
operator|+
literal|1
expr_stmt|;
if|if
condition|(
literal|4
operator|<
name|i
operator|&&
operator|!
name|strncmp
argument_list|(
literal|".git"
argument_list|,
name|remote
operator|+
name|i
operator|-
literal|3
argument_list|,
literal|4
argument_list|)
condition|)
name|remote_len
operator|=
name|i
operator|-
literal|3
expr_stmt|;
name|note_len
operator|=
name|sprintf
argument_list|(
name|note
argument_list|,
literal|"%s\t%s\t"
argument_list|,
name|sha1_to_hex
argument_list|(
name|commit
condition|?
name|commit
operator|->
name|object
operator|.
name|sha1
else|:
name|sha1
argument_list|)
argument_list|,
name|not_for_merge
condition|?
literal|"not-for-merge"
else|:
literal|""
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|what
condition|)
block|{
if|if
condition|(
operator|*
name|kind
condition|)
name|note_len
operator|+=
name|sprintf
argument_list|(
name|note
operator|+
name|note_len
argument_list|,
literal|"%s "
argument_list|,
name|kind
argument_list|)
expr_stmt|;
name|note_len
operator|+=
name|sprintf
argument_list|(
name|note
operator|+
name|note_len
argument_list|,
literal|"'%s' of "
argument_list|,
name|what
argument_list|)
expr_stmt|;
block|}
name|note_len
operator|+=
name|sprintf
argument_list|(
name|note
operator|+
name|note_len
argument_list|,
literal|"%.*s"
argument_list|,
name|remote_len
argument_list|,
name|remote
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|fp
argument_list|,
literal|"%s\n"
argument_list|,
name|note
argument_list|)
expr_stmt|;
return|return
name|update_local_ref
argument_list|(
name|local_name
argument_list|,
name|head
argument_list|,
name|note
argument_list|,
name|verbose
argument_list|,
name|force
argument_list|)
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|keep
specifier|static
name|char
modifier|*
name|keep
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|remove_keep
specifier|static
name|void
name|remove_keep
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
name|keep
operator|&&
operator|*
name|keep
condition|)
name|unlink
argument_list|(
name|keep
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|remove_keep_on_signal
specifier|static
name|void
name|remove_keep_on_signal
parameter_list|(
name|int
name|signo
parameter_list|)
block|{
name|remove_keep
argument_list|()
expr_stmt|;
name|signal
argument_list|(
name|SIGINT
argument_list|,
name|SIG_DFL
argument_list|)
expr_stmt|;
name|raise
argument_list|(
name|signo
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|find_local_name
specifier|static
name|char
modifier|*
name|find_local_name
parameter_list|(
specifier|const
name|char
modifier|*
name|remote_name
parameter_list|,
specifier|const
name|char
modifier|*
name|refs
parameter_list|,
name|int
modifier|*
name|force_p
parameter_list|,
name|int
modifier|*
name|not_for_merge_p
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|ref
init|=
name|refs
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|remote_name
argument_list|)
decl_stmt|;
while|while
condition|(
name|ref
condition|)
block|{
specifier|const
name|char
modifier|*
name|next
decl_stmt|;
name|int
name|single_force
decl_stmt|,
name|not_for_merge
decl_stmt|;
while|while
condition|(
operator|*
name|ref
operator|==
literal|'\n'
condition|)
name|ref
operator|++
expr_stmt|;
if|if
condition|(
operator|!
operator|*
name|ref
condition|)
break|break;
name|next
operator|=
name|strchr
argument_list|(
name|ref
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
name|single_force
operator|=
name|not_for_merge
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|*
name|ref
operator|==
literal|'+'
condition|)
block|{
name|single_force
operator|=
literal|1
expr_stmt|;
name|ref
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|ref
operator|==
literal|'.'
condition|)
block|{
name|not_for_merge
operator|=
literal|1
expr_stmt|;
name|ref
operator|++
expr_stmt|;
if|if
condition|(
operator|*
name|ref
operator|==
literal|'+'
condition|)
block|{
name|single_force
operator|=
literal|1
expr_stmt|;
name|ref
operator|++
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|remote_name
argument_list|,
name|ref
argument_list|,
name|len
argument_list|)
operator|&&
name|ref
index|[
name|len
index|]
operator|==
literal|':'
condition|)
block|{
specifier|const
name|char
modifier|*
name|local_part
init|=
name|ref
operator|+
name|len
operator|+
literal|1
decl_stmt|;
name|char
modifier|*
name|ret
decl_stmt|;
name|int
name|retlen
decl_stmt|;
if|if
condition|(
operator|!
name|next
condition|)
name|retlen
operator|=
name|strlen
argument_list|(
name|local_part
argument_list|)
expr_stmt|;
else|else
name|retlen
operator|=
name|next
operator|-
name|local_part
expr_stmt|;
name|ret
operator|=
name|xmalloc
argument_list|(
name|retlen
operator|+
literal|1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|ret
argument_list|,
name|local_part
argument_list|,
name|retlen
argument_list|)
expr_stmt|;
name|ret
index|[
name|retlen
index|]
operator|=
literal|0
expr_stmt|;
operator|*
name|force_p
operator|=
name|single_force
expr_stmt|;
operator|*
name|not_for_merge_p
operator|=
name|not_for_merge
expr_stmt|;
return|return
name|ret
return|;
block|}
name|ref
operator|=
name|next
expr_stmt|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|fetch_native_store
specifier|static
name|int
name|fetch_native_store
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|remote
parameter_list|,
specifier|const
name|char
modifier|*
name|remote_nick
parameter_list|,
specifier|const
name|char
modifier|*
name|refs
parameter_list|,
name|int
name|verbose
parameter_list|,
name|int
name|force
parameter_list|)
block|{
name|char
name|buffer
index|[
literal|1024
index|]
decl_stmt|;
name|int
name|err
init|=
literal|0
decl_stmt|;
name|signal
argument_list|(
name|SIGINT
argument_list|,
name|remove_keep_on_signal
argument_list|)
expr_stmt|;
name|atexit
argument_list|(
name|remove_keep
argument_list|)
expr_stmt|;
while|while
condition|(
name|fgets
argument_list|(
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
argument_list|,
name|stdin
argument_list|)
condition|)
block|{
name|int
name|len
decl_stmt|;
name|char
modifier|*
name|cp
decl_stmt|;
name|char
modifier|*
name|local_name
decl_stmt|;
name|int
name|single_force
decl_stmt|,
name|not_for_merge
decl_stmt|;
for|for
control|(
name|cp
operator|=
name|buffer
init|;
operator|*
name|cp
operator|&&
operator|!
name|isspace
argument_list|(
operator|*
name|cp
argument_list|)
condition|;
name|cp
operator|++
control|)
empty_stmt|;
if|if
condition|(
operator|*
name|cp
condition|)
operator|*
name|cp
operator|++
operator|=
literal|0
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|cp
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|&&
name|cp
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|cp
index|[
operator|--
name|len
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|buffer
argument_list|,
literal|"failed"
argument_list|)
condition|)
name|die
argument_list|(
literal|"Fetch failure: %s"
argument_list|,
name|remote
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|buffer
argument_list|,
literal|"pack"
argument_list|)
condition|)
continue|continue;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|buffer
argument_list|,
literal|"keep"
argument_list|)
condition|)
block|{
name|char
modifier|*
name|od
init|=
name|get_object_directory
argument_list|()
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|od
argument_list|)
operator|+
name|strlen
argument_list|(
name|cp
argument_list|)
operator|+
literal|50
decl_stmt|;
name|keep
operator|=
name|xmalloc
argument_list|(
name|len
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|keep
argument_list|,
literal|"%s/pack/pack-%s.keep"
argument_list|,
name|od
argument_list|,
name|cp
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|local_name
operator|=
name|find_local_name
argument_list|(
name|cp
argument_list|,
name|refs
argument_list|,
operator|&
name|single_force
argument_list|,
operator|&
name|not_for_merge
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|local_name
condition|)
continue|continue;
name|err
operator||=
name|append_fetch_head
argument_list|(
name|fp
argument_list|,
name|buffer
argument_list|,
name|remote
argument_list|,
name|cp
argument_list|,
name|remote_nick
argument_list|,
name|local_name
argument_list|,
name|not_for_merge
argument_list|,
name|verbose
argument_list|,
name|force
operator|||
name|single_force
argument_list|)
expr_stmt|;
block|}
return|return
name|err
return|;
block|}
end_function

begin_function
DECL|function|parse_reflist
specifier|static
name|int
name|parse_reflist
parameter_list|(
specifier|const
name|char
modifier|*
name|reflist
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|ref
decl_stmt|;
name|printf
argument_list|(
literal|"refs='"
argument_list|)
expr_stmt|;
for|for
control|(
name|ref
operator|=
name|reflist
init|;
name|ref
condition|;
control|)
block|{
specifier|const
name|char
modifier|*
name|next
decl_stmt|;
while|while
condition|(
operator|*
name|ref
operator|&&
name|isspace
argument_list|(
operator|*
name|ref
argument_list|)
condition|)
name|ref
operator|++
expr_stmt|;
if|if
condition|(
operator|!
operator|*
name|ref
condition|)
break|break;
for|for
control|(
name|next
operator|=
name|ref
init|;
operator|*
name|next
operator|&&
operator|!
name|isspace
argument_list|(
operator|*
name|next
argument_list|)
condition|;
name|next
operator|++
control|)
empty_stmt|;
name|printf
argument_list|(
literal|"\n%.*s"
argument_list|,
call|(
name|int
call|)
argument_list|(
name|next
operator|-
name|ref
argument_list|)
argument_list|,
name|ref
argument_list|)
expr_stmt|;
name|ref
operator|=
name|next
expr_stmt|;
block|}
name|printf
argument_list|(
literal|"'\n"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"rref='"
argument_list|)
expr_stmt|;
for|for
control|(
name|ref
operator|=
name|reflist
init|;
name|ref
condition|;
control|)
block|{
specifier|const
name|char
modifier|*
name|next
decl_stmt|,
modifier|*
name|colon
decl_stmt|;
while|while
condition|(
operator|*
name|ref
operator|&&
name|isspace
argument_list|(
operator|*
name|ref
argument_list|)
condition|)
name|ref
operator|++
expr_stmt|;
if|if
condition|(
operator|!
operator|*
name|ref
condition|)
break|break;
for|for
control|(
name|next
operator|=
name|ref
init|;
operator|*
name|next
operator|&&
operator|!
name|isspace
argument_list|(
operator|*
name|next
argument_list|)
condition|;
name|next
operator|++
control|)
empty_stmt|;
if|if
condition|(
operator|*
name|ref
operator|==
literal|'.'
condition|)
name|ref
operator|++
expr_stmt|;
if|if
condition|(
operator|*
name|ref
operator|==
literal|'+'
condition|)
name|ref
operator|++
expr_stmt|;
name|colon
operator|=
name|strchr
argument_list|(
name|ref
argument_list|,
literal|':'
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%.*s"
argument_list|,
call|(
name|int
call|)
argument_list|(
operator|(
name|colon
condition|?
name|colon
else|:
name|next
operator|)
operator|-
name|ref
argument_list|)
argument_list|,
name|ref
argument_list|)
expr_stmt|;
name|ref
operator|=
name|next
expr_stmt|;
block|}
name|printf
argument_list|(
literal|"'\n"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|expand_refs_wildcard
specifier|static
name|int
name|expand_refs_wildcard
parameter_list|(
specifier|const
name|char
modifier|*
name|ls_remote_result
parameter_list|,
name|int
name|numrefs
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refs
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|matchlen
decl_stmt|,
name|replacelen
decl_stmt|;
name|int
name|found_one
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|remote
init|=
operator|*
name|refs
operator|++
decl_stmt|;
name|numrefs
operator|--
expr_stmt|;
if|if
condition|(
name|numrefs
operator|==
literal|0
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Nothing specified for fetching with remote.%s.fetch\n"
argument_list|,
name|remote
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"empty\n"
argument_list|)
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|numrefs
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|ref
init|=
name|refs
index|[
name|i
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|lref
init|=
name|ref
decl_stmt|;
specifier|const
name|char
modifier|*
name|colon
decl_stmt|;
specifier|const
name|char
modifier|*
name|tail
decl_stmt|;
specifier|const
name|char
modifier|*
name|ls
decl_stmt|;
specifier|const
name|char
modifier|*
name|next
decl_stmt|;
if|if
condition|(
operator|*
name|lref
operator|==
literal|'+'
condition|)
name|lref
operator|++
expr_stmt|;
name|colon
operator|=
name|strchr
argument_list|(
name|lref
argument_list|,
literal|':'
argument_list|)
expr_stmt|;
name|tail
operator|=
name|lref
operator|+
name|strlen
argument_list|(
name|lref
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|colon
operator|&&
literal|2
operator|<
name|colon
operator|-
name|lref
operator|&&
name|colon
index|[
operator|-
literal|1
index|]
operator|==
literal|'*'
operator|&&
name|colon
index|[
operator|-
literal|2
index|]
operator|==
literal|'/'
operator|&&
literal|2
operator|<
name|tail
operator|-
operator|(
name|colon
operator|+
literal|1
operator|)
operator|&&
name|tail
index|[
operator|-
literal|1
index|]
operator|==
literal|'*'
operator|&&
name|tail
index|[
operator|-
literal|2
index|]
operator|==
literal|'/'
operator|)
condition|)
block|{
comment|/* not a glob */
if|if
condition|(
operator|!
name|found_one
operator|++
condition|)
name|printf
argument_list|(
literal|"explicit\n"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
continue|continue;
block|}
comment|/* glob */
if|if
condition|(
operator|!
name|found_one
operator|++
condition|)
name|printf
argument_list|(
literal|"glob\n"
argument_list|)
expr_stmt|;
comment|/* lref to colon-2 is remote hierarchy name; 		 * colon+1 to tail-2 is local. 		 */
name|matchlen
operator|=
operator|(
name|colon
operator|-
literal|1
operator|)
operator|-
name|lref
expr_stmt|;
name|replacelen
operator|=
operator|(
name|tail
operator|-
literal|1
operator|)
operator|-
operator|(
name|colon
operator|+
literal|1
operator|)
expr_stmt|;
for|for
control|(
name|ls
operator|=
name|ls_remote_result
init|;
name|ls
condition|;
name|ls
operator|=
name|next
control|)
block|{
specifier|const
name|char
modifier|*
name|eol
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|namelen
decl_stmt|;
while|while
condition|(
operator|*
name|ls
operator|&&
name|isspace
argument_list|(
operator|*
name|ls
argument_list|)
condition|)
name|ls
operator|++
expr_stmt|;
name|next
operator|=
name|strchr
argument_list|(
name|ls
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
name|eol
operator|=
operator|!
name|next
condition|?
operator|(
name|ls
operator|+
name|strlen
argument_list|(
name|ls
argument_list|)
operator|)
else|:
name|next
expr_stmt|;
if|if
condition|(
operator|!
name|memcmp
argument_list|(
literal|"^{}"
argument_list|,
name|eol
operator|-
literal|3
argument_list|,
literal|3
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|ls
argument_list|,
name|sha1
argument_list|)
condition|)
continue|continue;
name|ls
operator|+=
literal|40
expr_stmt|;
while|while
condition|(
name|ls
operator|<
name|eol
operator|&&
name|isspace
argument_list|(
operator|*
name|ls
argument_list|)
condition|)
name|ls
operator|++
expr_stmt|;
comment|/* ls to next (or eol) is the name. 			 * is it identical to lref to colon-2? 			 */
if|if
condition|(
operator|(
name|eol
operator|-
name|ls
operator|)
operator|<=
name|matchlen
operator|||
name|strncmp
argument_list|(
name|ls
argument_list|,
name|lref
argument_list|,
name|matchlen
argument_list|)
condition|)
continue|continue;
comment|/* Yes, it is a match */
name|namelen
operator|=
name|eol
operator|-
name|ls
expr_stmt|;
if|if
condition|(
name|lref
operator|!=
name|ref
condition|)
name|putchar
argument_list|(
literal|'+'
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%.*s:%.*s%.*s\n"
argument_list|,
name|namelen
argument_list|,
name|ls
argument_list|,
name|replacelen
argument_list|,
name|colon
operator|+
literal|1
argument_list|,
name|namelen
operator|-
name|matchlen
argument_list|,
name|ls
operator|+
name|matchlen
argument_list|)
expr_stmt|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_fetch__tool
name|int
name|cmd_fetch__tool
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
name|verbose
init|=
literal|0
decl_stmt|;
name|int
name|force
init|=
literal|0
decl_stmt|;
while|while
condition|(
literal|1
operator|<
name|argc
condition|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
literal|1
index|]
decl_stmt|;
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
name|verbose
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"-f"
argument_list|,
name|arg
argument_list|)
condition|)
name|force
operator|=
literal|1
expr_stmt|;
else|else
break|break;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|<=
literal|1
condition|)
return|return
name|error
argument_list|(
literal|"Missing subcommand"
argument_list|)
return|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"append-fetch-head"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
name|int
name|result
decl_stmt|;
name|FILE
modifier|*
name|fp
decl_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|8
condition|)
return|return
name|error
argument_list|(
literal|"append-fetch-head takes 6 args"
argument_list|)
return|;
name|fp
operator|=
name|fopen
argument_list|(
name|git_path
argument_list|(
literal|"FETCH_HEAD"
argument_list|)
argument_list|,
literal|"a"
argument_list|)
expr_stmt|;
name|result
operator|=
name|append_fetch_head
argument_list|(
name|fp
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|,
name|argv
index|[
literal|4
index|]
argument_list|,
name|argv
index|[
literal|5
index|]
argument_list|,
name|argv
index|[
literal|6
index|]
argument_list|,
operator|!
operator|!
name|argv
index|[
literal|7
index|]
index|[
literal|0
index|]
argument_list|,
name|verbose
argument_list|,
name|force
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|fp
argument_list|)
expr_stmt|;
return|return
name|result
return|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"update-local-ref"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
if|if
condition|(
name|argc
operator|!=
literal|5
condition|)
return|return
name|error
argument_list|(
literal|"update-local-ref takes 3 args"
argument_list|)
return|;
return|return
name|update_local_ref
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|,
name|argv
index|[
literal|4
index|]
argument_list|,
name|verbose
argument_list|,
name|force
argument_list|)
return|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"native-store"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
name|int
name|result
decl_stmt|;
name|FILE
modifier|*
name|fp
decl_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|5
condition|)
return|return
name|error
argument_list|(
literal|"fetch-native-store takes 3 args"
argument_list|)
return|;
name|fp
operator|=
name|fopen
argument_list|(
name|git_path
argument_list|(
literal|"FETCH_HEAD"
argument_list|)
argument_list|,
literal|"a"
argument_list|)
expr_stmt|;
name|result
operator|=
name|fetch_native_store
argument_list|(
name|fp
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|,
name|argv
index|[
literal|4
index|]
argument_list|,
name|verbose
argument_list|,
name|force
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|fp
argument_list|)
expr_stmt|;
return|return
name|result
return|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"parse-reflist"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
if|if
condition|(
name|argc
operator|!=
literal|3
condition|)
return|return
name|error
argument_list|(
literal|"parse-reflist takes 1 arg"
argument_list|)
return|;
return|return
name|parse_reflist
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|)
return|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"expand-refs-wildcard"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
if|if
condition|(
name|argc
operator|<
literal|4
condition|)
return|return
name|error
argument_list|(
literal|"expand-refs-wildcard takes at least 2 args"
argument_list|)
return|;
return|return
name|expand_refs_wildcard
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|argc
operator|-
literal|3
argument_list|,
name|argv
operator|+
literal|3
argument_list|)
return|;
block|}
return|return
name|error
argument_list|(
literal|"Unknown subcommand: %s"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
return|;
block|}
end_function

end_unit

