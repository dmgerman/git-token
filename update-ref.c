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

begin_decl_stmt
DECL|variable|git_update_ref_usage
specifier|static
specifier|const
name|char
name|git_update_ref_usage
index|[]
init|=
literal|"git-update-ref<refname><value> [<oldval>]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|re_verify
specifier|static
name|int
name|re_verify
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|char
modifier|*
name|oldsha1
parameter_list|,
name|unsigned
name|char
modifier|*
name|currsha1
parameter_list|)
block|{
name|char
name|buf
index|[
literal|40
index|]
decl_stmt|;
name|int
name|fd
init|=
name|open
argument_list|(
name|path
argument_list|,
name|O_RDONLY
argument_list|)
decl_stmt|,
name|nr
decl_stmt|;
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
name|nr
operator|=
name|read
argument_list|(
name|fd
argument_list|,
name|buf
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
name|nr
operator|!=
literal|40
operator|||
name|get_sha1_hex
argument_list|(
name|buf
argument_list|,
name|currsha1
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
return|return
name|memcmp
argument_list|(
name|oldsha1
argument_list|,
name|currsha1
argument_list|,
literal|20
argument_list|)
condition|?
operator|-
literal|1
else|:
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
name|hex
decl_stmt|;
specifier|const
name|char
modifier|*
name|refname
decl_stmt|,
modifier|*
name|value
decl_stmt|,
modifier|*
name|oldval
decl_stmt|,
modifier|*
name|path
decl_stmt|;
name|char
modifier|*
name|lockpath
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
decl_stmt|,
name|currsha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|fd
decl_stmt|,
name|written
decl_stmt|;
name|setup_git_directory
argument_list|()
expr_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|<
literal|3
operator|||
name|argc
operator|>
literal|4
condition|)
name|usage
argument_list|(
name|git_update_ref_usage
argument_list|)
expr_stmt|;
name|refname
operator|=
name|argv
index|[
literal|1
index|]
expr_stmt|;
name|value
operator|=
name|argv
index|[
literal|2
index|]
expr_stmt|;
name|oldval
operator|=
name|argv
index|[
literal|3
index|]
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
name|memset
argument_list|(
name|oldsha1
argument_list|,
literal|0
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
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
name|path
operator|=
name|resolve_ref
argument_list|(
name|git_path
argument_list|(
literal|"%s"
argument_list|,
name|refname
argument_list|)
argument_list|,
name|currsha1
argument_list|,
operator|!
operator|!
name|oldval
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|path
condition|)
name|die
argument_list|(
literal|"No such ref: %s"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
if|if
condition|(
name|oldval
condition|)
block|{
if|if
condition|(
name|memcmp
argument_list|(
name|currsha1
argument_list|,
name|oldsha1
argument_list|,
literal|20
argument_list|)
condition|)
name|die
argument_list|(
literal|"Ref %s is at %s but expected %s"
argument_list|,
name|refname
argument_list|,
name|sha1_to_hex
argument_list|(
name|currsha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|oldsha1
argument_list|)
argument_list|)
expr_stmt|;
comment|/* Nothing to do? */
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|oldsha1
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
condition|)
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
name|path
operator|=
name|strdup
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|lockpath
operator|=
name|mkpath
argument_list|(
literal|"%s.lock"
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|safe_create_leading_directories
argument_list|(
name|lockpath
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Unable to create all of %s"
argument_list|,
name|lockpath
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|lockpath
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
name|fd
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Unable to create %s"
argument_list|,
name|lockpath
argument_list|)
expr_stmt|;
name|hex
operator|=
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|hex
index|[
literal|40
index|]
operator|=
literal|'\n'
expr_stmt|;
name|written
operator|=
name|write
argument_list|(
name|fd
argument_list|,
name|hex
argument_list|,
literal|41
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
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
name|lockpath
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"Unable to write to %s"
argument_list|,
name|lockpath
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * Re-read the ref after getting the lock to verify 	 */
if|if
condition|(
name|oldval
operator|&&
name|re_verify
argument_list|(
name|path
argument_list|,
name|oldsha1
argument_list|,
name|currsha1
argument_list|)
operator|<
literal|0
condition|)
block|{
name|unlink
argument_list|(
name|lockpath
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"Ref lock failed"
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * Finally, replace the old ref with the new one 	 */
if|if
condition|(
name|rename
argument_list|(
name|lockpath
argument_list|,
name|path
argument_list|)
operator|<
literal|0
condition|)
block|{
name|unlink
argument_list|(
name|lockpath
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"Unable to create %s"
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

