begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * "git push"  */
end_comment

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
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"remote.h"
end_include

begin_decl_stmt
DECL|variable|push_usage
specifier|static
specifier|const
name|char
name|push_usage
index|[]
init|=
literal|"git-push [--all] [--tags] [--receive-pack=<git-receive-pack>] [--repo=all] [-f | --force] [-v] [<repository><refspec>...]"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|all
DECL|variable|tags
DECL|variable|force
DECL|variable|thin
DECL|variable|verbose
specifier|static
name|int
name|all
decl_stmt|,
name|tags
decl_stmt|,
name|force
decl_stmt|,
name|thin
init|=
literal|1
decl_stmt|,
name|verbose
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|receivepack
specifier|static
specifier|const
name|char
modifier|*
name|receivepack
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|refspec
specifier|static
specifier|const
name|char
modifier|*
modifier|*
name|refspec
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|refspec_nr
specifier|static
name|int
name|refspec_nr
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|add_refspec
specifier|static
name|void
name|add_refspec
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|)
block|{
name|int
name|nr
init|=
name|refspec_nr
operator|+
literal|1
decl_stmt|;
name|refspec
operator|=
name|xrealloc
argument_list|(
name|refspec
argument_list|,
name|nr
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|refspec
index|[
name|nr
operator|-
literal|1
index|]
operator|=
name|ref
expr_stmt|;
name|refspec_nr
operator|=
name|nr
expr_stmt|;
block|}
end_function

begin_function
DECL|function|expand_one_ref
specifier|static
name|int
name|expand_one_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
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
comment|/* Ignore the "refs/" at the beginning of the refname */
name|ref
operator|+=
literal|5
expr_stmt|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|ref
argument_list|,
literal|"tags/"
argument_list|)
condition|)
name|add_refspec
argument_list|(
name|xstrdup
argument_list|(
name|ref
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|expand_refspecs
specifier|static
name|void
name|expand_refspecs
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
name|all
condition|)
block|{
if|if
condition|(
name|refspec_nr
condition|)
name|die
argument_list|(
literal|"cannot mix '--all' and a refspec"
argument_list|)
expr_stmt|;
comment|/* 		 * No need to expand "--all" - we'll just use 		 * the "--all" flag to send-pack 		 */
return|return;
block|}
if|if
condition|(
operator|!
name|tags
condition|)
return|return;
name|for_each_ref
argument_list|(
name|expand_one_ref
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
end_function

begin_struct
DECL|struct|wildcard_cb
struct|struct
name|wildcard_cb
block|{
DECL|member|from_prefix
specifier|const
name|char
modifier|*
name|from_prefix
decl_stmt|;
DECL|member|from_prefix_len
name|int
name|from_prefix_len
decl_stmt|;
DECL|member|to_prefix
specifier|const
name|char
modifier|*
name|to_prefix
decl_stmt|;
DECL|member|to_prefix_len
name|int
name|to_prefix_len
decl_stmt|;
DECL|member|force
name|int
name|force
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|expand_wildcard_ref
specifier|static
name|int
name|expand_wildcard_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
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
name|struct
name|wildcard_cb
modifier|*
name|cb
init|=
name|cb_data
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|ref
argument_list|)
decl_stmt|;
name|char
modifier|*
name|expanded
decl_stmt|,
modifier|*
name|newref
decl_stmt|;
if|if
condition|(
name|len
operator|<
name|cb
operator|->
name|from_prefix_len
operator|||
name|memcmp
argument_list|(
name|cb
operator|->
name|from_prefix
argument_list|,
name|ref
argument_list|,
name|cb
operator|->
name|from_prefix_len
argument_list|)
condition|)
return|return
literal|0
return|;
name|expanded
operator|=
name|xmalloc
argument_list|(
name|len
operator|*
literal|2
operator|+
name|cb
operator|->
name|force
operator|+
operator|(
name|cb
operator|->
name|to_prefix_len
operator|-
name|cb
operator|->
name|from_prefix_len
operator|)
operator|+
literal|2
argument_list|)
expr_stmt|;
name|newref
operator|=
name|expanded
operator|+
name|cb
operator|->
name|force
expr_stmt|;
if|if
condition|(
name|cb
operator|->
name|force
condition|)
name|expanded
index|[
literal|0
index|]
operator|=
literal|'+'
expr_stmt|;
name|memcpy
argument_list|(
name|newref
argument_list|,
name|ref
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|newref
index|[
name|len
index|]
operator|=
literal|':'
expr_stmt|;
name|memcpy
argument_list|(
name|newref
operator|+
name|len
operator|+
literal|1
argument_list|,
name|cb
operator|->
name|to_prefix
argument_list|,
name|cb
operator|->
name|to_prefix_len
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|newref
operator|+
name|len
operator|+
literal|1
operator|+
name|cb
operator|->
name|to_prefix_len
argument_list|,
name|ref
operator|+
name|cb
operator|->
name|from_prefix_len
argument_list|)
expr_stmt|;
name|add_refspec
argument_list|(
name|expanded
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|wildcard_ref
specifier|static
name|int
name|wildcard_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|)
block|{
name|int
name|len
decl_stmt|;
specifier|const
name|char
modifier|*
name|colon
decl_stmt|;
name|struct
name|wildcard_cb
name|cb
decl_stmt|;
name|memset
argument_list|(
operator|&
name|cb
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|cb
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ref
index|[
literal|0
index|]
operator|==
literal|'+'
condition|)
block|{
name|cb
operator|.
name|force
operator|=
literal|1
expr_stmt|;
name|ref
operator|++
expr_stmt|;
block|}
name|len
operator|=
name|strlen
argument_list|(
name|ref
argument_list|)
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
if|if
condition|(
operator|!
operator|(
name|colon
operator|&&
name|ref
operator|<
name|colon
operator|&&
name|colon
index|[
operator|-
literal|2
index|]
operator|==
literal|'/'
operator|&&
name|colon
index|[
operator|-
literal|1
index|]
operator|==
literal|'*'
operator|&&
comment|/* "<mine>/<asterisk>:<yours>/<asterisk>" is at least 7 bytes */
literal|7
operator|<=
name|len
operator|&&
name|ref
index|[
name|len
operator|-
literal|2
index|]
operator|==
literal|'/'
operator|&&
name|ref
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'*'
operator|)
condition|)
return|return
literal|0
return|;
name|cb
operator|.
name|from_prefix
operator|=
name|ref
expr_stmt|;
name|cb
operator|.
name|from_prefix_len
operator|=
name|colon
operator|-
name|ref
operator|-
literal|1
expr_stmt|;
name|cb
operator|.
name|to_prefix
operator|=
name|colon
operator|+
literal|1
expr_stmt|;
name|cb
operator|.
name|to_prefix_len
operator|=
name|len
operator|-
operator|(
name|colon
operator|-
name|ref
operator|)
operator|-
literal|2
expr_stmt|;
name|for_each_ref
argument_list|(
name|expand_wildcard_ref
argument_list|,
operator|&
name|cb
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|set_refspecs
specifier|static
name|void
name|set_refspecs
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|refs
parameter_list|,
name|int
name|nr
parameter_list|)
block|{
if|if
condition|(
name|nr
condition|)
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
name|nr
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"tag"
argument_list|,
name|ref
argument_list|)
condition|)
block|{
name|char
modifier|*
name|tag
decl_stmt|;
name|int
name|len
decl_stmt|;
if|if
condition|(
name|nr
operator|<=
operator|++
name|i
condition|)
name|die
argument_list|(
literal|"tag shorthand without<tag>"
argument_list|)
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|refs
index|[
name|i
index|]
argument_list|)
operator|+
literal|11
expr_stmt|;
name|tag
operator|=
name|xmalloc
argument_list|(
name|len
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|tag
argument_list|,
literal|"refs/tags/"
argument_list|)
expr_stmt|;
name|strcat
argument_list|(
name|tag
argument_list|,
name|refs
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|ref
operator|=
name|tag
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|wildcard_ref
argument_list|(
name|ref
argument_list|)
condition|)
continue|continue;
name|add_refspec
argument_list|(
name|ref
argument_list|)
expr_stmt|;
block|}
block|}
name|expand_refspecs
argument_list|()
expr_stmt|;
block|}
end_function

begin_function
DECL|function|do_push
specifier|static
name|int
name|do_push
parameter_list|(
specifier|const
name|char
modifier|*
name|repo
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|errs
decl_stmt|;
name|int
name|common_argc
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
name|int
name|argc
decl_stmt|;
name|struct
name|remote
modifier|*
name|remote
init|=
name|remote_get
argument_list|(
name|repo
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|remote
condition|)
name|die
argument_list|(
literal|"bad repository '%s'"
argument_list|,
name|repo
argument_list|)
expr_stmt|;
if|if
condition|(
name|remote
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
name|remote
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
name|remote
operator|->
name|receivepack
argument_list|)
expr_stmt|;
name|receivepack
operator|=
name|rp
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|refspec
operator|&&
operator|!
name|all
operator|&&
operator|!
name|tags
operator|&&
name|remote
operator|->
name|push_refspec_nr
condition|)
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|remote
operator|->
name|push_refspec_nr
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|wildcard_ref
argument_list|(
name|remote
operator|->
name|push_refspec
index|[
name|i
index|]
argument_list|)
condition|)
name|add_refspec
argument_list|(
name|remote
operator|->
name|push_refspec
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
block|}
name|argv
operator|=
name|xmalloc
argument_list|(
operator|(
name|refspec_nr
operator|+
literal|10
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
literal|"dummy-send-pack"
expr_stmt|;
name|argc
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|all
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
name|force
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
name|receivepack
condition|)
name|argv
index|[
name|argc
operator|++
index|]
operator|=
name|receivepack
expr_stmt|;
name|common_argc
operator|=
name|argc
expr_stmt|;
name|errs
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
name|remote
operator|->
name|uri_nr
condition|;
name|i
operator|++
control|)
block|{
name|int
name|err
decl_stmt|;
name|int
name|dest_argc
init|=
name|common_argc
decl_stmt|;
name|int
name|dest_refspec_nr
init|=
name|refspec_nr
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|dest_refspec
init|=
name|refspec
decl_stmt|;
specifier|const
name|char
modifier|*
name|dest
init|=
name|remote
operator|->
name|uri
index|[
name|i
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|sender
init|=
literal|"send-pack"
decl_stmt|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|dest
argument_list|,
literal|"http://"
argument_list|)
operator|||
operator|!
name|prefixcmp
argument_list|(
name|dest
argument_list|,
literal|"https://"
argument_list|)
condition|)
name|sender
operator|=
literal|"http-push"
expr_stmt|;
else|else
block|{
name|char
modifier|*
name|rem
init|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|remote
operator|->
name|name
argument_list|)
operator|+
literal|10
argument_list|)
decl_stmt|;
name|sprintf
argument_list|(
name|rem
argument_list|,
literal|"--remote=%s"
argument_list|,
name|remote
operator|->
name|name
argument_list|)
expr_stmt|;
name|argv
index|[
name|dest_argc
operator|++
index|]
operator|=
name|rem
expr_stmt|;
if|if
condition|(
name|thin
condition|)
name|argv
index|[
name|dest_argc
operator|++
index|]
operator|=
literal|"--thin"
expr_stmt|;
block|}
name|argv
index|[
literal|0
index|]
operator|=
name|sender
expr_stmt|;
name|argv
index|[
name|dest_argc
operator|++
index|]
operator|=
name|dest
expr_stmt|;
while|while
condition|(
name|dest_refspec_nr
operator|--
condition|)
name|argv
index|[
name|dest_argc
operator|++
index|]
operator|=
operator|*
name|dest_refspec
operator|++
expr_stmt|;
name|argv
index|[
name|dest_argc
index|]
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Pushing to %s\n"
argument_list|,
name|dest
argument_list|)
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
if|if
condition|(
operator|!
name|err
condition|)
continue|continue;
name|error
argument_list|(
literal|"failed to push to '%s'"
argument_list|,
name|remote
operator|->
name|uri
index|[
name|i
index|]
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
name|sender
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
name|sender
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
name|sender
argument_list|)
expr_stmt|;
block|}
name|errs
operator|++
expr_stmt|;
block|}
return|return
operator|!
operator|!
name|errs
return|;
block|}
end_function

begin_function
DECL|function|cmd_push
name|int
name|cmd_push
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
decl_stmt|;
specifier|const
name|char
modifier|*
name|repo
init|=
name|NULL
decl_stmt|;
comment|/* default repository */
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
block|{
name|repo
operator|=
name|arg
expr_stmt|;
name|i
operator|++
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-v"
argument_list|)
condition|)
block|{
name|verbose
operator|=
literal|1
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
literal|"--repo="
argument_list|)
condition|)
block|{
name|repo
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
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--all"
argument_list|)
condition|)
block|{
name|all
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
literal|"--tags"
argument_list|)
condition|)
block|{
name|tags
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
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-f"
argument_list|)
condition|)
block|{
name|force
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
literal|"--thin"
argument_list|)
condition|)
block|{
name|thin
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
literal|"--no-thin"
argument_list|)
condition|)
block|{
name|thin
operator|=
literal|0
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
literal|"--receive-pack="
argument_list|)
condition|)
block|{
name|receivepack
operator|=
name|arg
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
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|push_usage
argument_list|)
expr_stmt|;
block|}
name|set_refspecs
argument_list|(
name|argv
operator|+
name|i
argument_list|,
name|argc
operator|-
name|i
argument_list|)
expr_stmt|;
return|return
name|do_push
argument_list|(
name|repo
argument_list|)
return|;
block|}
end_function

end_unit

