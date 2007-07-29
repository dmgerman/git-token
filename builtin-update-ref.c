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
file|"builtin.h"
end_include

begin_decl_stmt
DECL|variable|git_update_ref_usage
specifier|static
specifier|const
name|char
name|git_update_ref_usage
index|[]
init|=
literal|"git-update-ref [-m<reason>] (-d<refname><value> | [--no-deref]<refname><value> [<oldval>])"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_update_ref
name|int
name|cmd_update_ref
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
specifier|const
name|char
modifier|*
name|refname
init|=
name|NULL
decl_stmt|,
modifier|*
name|value
init|=
name|NULL
decl_stmt|,
modifier|*
name|oldval
init|=
name|NULL
decl_stmt|,
modifier|*
name|msg
init|=
name|NULL
decl_stmt|;
name|struct
name|ref_lock
modifier|*
name|lock
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|,
name|oldsha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|i
decl_stmt|,
name|delete
decl_stmt|,
name|ref_flags
decl_stmt|;
name|delete
operator|=
literal|0
expr_stmt|;
name|ref_flags
operator|=
literal|0
expr_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"-m"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
condition|)
block|{
if|if
condition|(
name|i
operator|+
literal|1
operator|>=
name|argc
condition|)
name|usage
argument_list|(
name|git_update_ref_usage
argument_list|)
expr_stmt|;
name|msg
operator|=
name|argv
index|[
operator|++
name|i
index|]
expr_stmt|;
if|if
condition|(
operator|!
operator|*
name|msg
condition|)
name|die
argument_list|(
literal|"Refusing to perform update with empty message."
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"-d"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
condition|)
block|{
name|delete
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
literal|"--no-deref"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
condition|)
block|{
name|ref_flags
operator||=
name|REF_NODEREF
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|refname
condition|)
block|{
name|refname
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|value
condition|)
block|{
name|value
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|oldval
condition|)
block|{
name|oldval
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
continue|continue;
block|}
block|}
if|if
condition|(
operator|!
name|refname
operator|||
operator|!
name|value
condition|)
name|usage
argument_list|(
name|git_update_ref_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|value
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s: not a valid SHA1"
argument_list|,
name|value
argument_list|)
expr_stmt|;
if|if
condition|(
name|delete
condition|)
block|{
if|if
condition|(
name|oldval
condition|)
name|usage
argument_list|(
name|git_update_ref_usage
argument_list|)
expr_stmt|;
return|return
name|delete_ref
argument_list|(
name|refname
argument_list|,
name|sha1
argument_list|)
return|;
block|}
name|hashclr
argument_list|(
name|oldsha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|oldval
operator|&&
operator|*
name|oldval
operator|&&
name|get_sha1
argument_list|(
name|oldval
argument_list|,
name|oldsha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s: not a valid old SHA1"
argument_list|,
name|oldval
argument_list|)
expr_stmt|;
name|lock
operator|=
name|lock_any_ref_for_update
argument_list|(
name|refname
argument_list|,
name|oldval
condition|?
name|oldsha1
else|:
name|NULL
argument_list|,
name|ref_flags
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lock
condition|)
name|die
argument_list|(
literal|"%s: cannot lock the ref"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
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
name|die
argument_list|(
literal|"%s: cannot update the ref"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

