begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_decl_stmt
DECL|variable|prune_packed_usage
specifier|static
specifier|const
name|char
name|prune_packed_usage
index|[]
init|=
literal|"git-prune-packed [-n]"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|dryrun
specifier|static
name|int
name|dryrun
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|prune_dir
specifier|static
name|void
name|prune_dir
parameter_list|(
name|int
name|i
parameter_list|,
name|DIR
modifier|*
name|dir
parameter_list|,
name|char
modifier|*
name|pathname
parameter_list|,
name|int
name|len
parameter_list|)
block|{
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
name|char
name|hex
index|[
literal|40
index|]
decl_stmt|;
name|sprintf
argument_list|(
name|hex
argument_list|,
literal|"%02x"
argument_list|,
name|i
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|de
operator|=
name|readdir
argument_list|(
name|dir
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|strlen
argument_list|(
name|de
operator|->
name|d_name
argument_list|)
operator|!=
literal|38
condition|)
continue|continue;
name|memcpy
argument_list|(
name|hex
operator|+
literal|2
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
literal|38
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|hex
argument_list|,
name|sha1
argument_list|)
condition|)
continue|continue;
if|if
condition|(
operator|!
name|has_sha1_pack
argument_list|(
name|sha1
argument_list|)
condition|)
continue|continue;
name|memcpy
argument_list|(
name|pathname
operator|+
name|len
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
literal|38
argument_list|)
expr_stmt|;
if|if
condition|(
name|dryrun
condition|)
name|printf
argument_list|(
literal|"rm -f %s\n"
argument_list|,
name|pathname
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|unlink
argument_list|(
name|pathname
argument_list|)
operator|<
literal|0
condition|)
name|error
argument_list|(
literal|"unable to unlink %s"
argument_list|,
name|pathname
argument_list|)
expr_stmt|;
block|}
name|pathname
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|rmdir
argument_list|(
name|pathname
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|prune_packed_objects
specifier|static
name|void
name|prune_packed_objects
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
specifier|static
name|char
name|pathname
index|[
name|PATH_MAX
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|dir
init|=
name|get_object_directory
argument_list|()
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|dir
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
operator|>
name|PATH_MAX
operator|-
literal|42
condition|)
name|die
argument_list|(
literal|"impossible object directory"
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|pathname
argument_list|,
name|dir
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|&&
name|pathname
index|[
name|len
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
name|pathname
index|[
name|len
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|256
condition|;
name|i
operator|++
control|)
block|{
name|DIR
modifier|*
name|d
decl_stmt|;
name|sprintf
argument_list|(
name|pathname
operator|+
name|len
argument_list|,
literal|"%02x/"
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|d
operator|=
name|opendir
argument_list|(
name|pathname
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|d
condition|)
continue|continue;
name|prune_dir
argument_list|(
name|i
argument_list|,
name|d
argument_list|,
name|pathname
argument_list|,
name|len
operator|+
literal|3
argument_list|)
expr_stmt|;
name|closedir
argument_list|(
name|d
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
operator|*
name|arg
operator|==
literal|'-'
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-n"
argument_list|)
condition|)
name|dryrun
operator|=
literal|1
expr_stmt|;
else|else
name|usage
argument_list|(
name|prune_packed_usage
argument_list|)
expr_stmt|;
continue|continue;
block|}
comment|/* Handle arguments here .. */
name|usage
argument_list|(
name|prune_packed_usage
argument_list|)
expr_stmt|;
block|}
name|prune_packed_objects
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

