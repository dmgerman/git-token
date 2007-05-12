begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_function
DECL|function|has_symlink_leading_path
name|int
name|has_symlink_leading_path
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|char
modifier|*
name|last_symlink
parameter_list|)
block|{
name|char
name|path
index|[
name|PATH_MAX
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|sp
decl_stmt|,
modifier|*
name|ep
decl_stmt|;
name|char
modifier|*
name|dp
decl_stmt|;
name|sp
operator|=
name|name
expr_stmt|;
name|dp
operator|=
name|path
expr_stmt|;
if|if
condition|(
name|last_symlink
operator|&&
operator|*
name|last_symlink
condition|)
block|{
name|size_t
name|last_len
init|=
name|strlen
argument_list|(
name|last_symlink
argument_list|)
decl_stmt|;
name|size_t
name|len
init|=
name|strlen
argument_list|(
name|name
argument_list|)
decl_stmt|;
if|if
condition|(
name|last_len
operator|<
name|len
operator|&&
operator|!
name|strncmp
argument_list|(
name|name
argument_list|,
name|last_symlink
argument_list|,
name|last_len
argument_list|)
operator|&&
name|name
index|[
name|last_len
index|]
operator|==
literal|'/'
condition|)
return|return
literal|1
return|;
operator|*
name|last_symlink
operator|=
literal|'\0'
expr_stmt|;
block|}
while|while
condition|(
literal|1
condition|)
block|{
name|size_t
name|len
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|ep
operator|=
name|strchr
argument_list|(
name|sp
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ep
condition|)
break|break;
name|len
operator|=
name|ep
operator|-
name|sp
expr_stmt|;
if|if
condition|(
name|PATH_MAX
operator|<=
name|dp
operator|+
name|len
operator|-
name|path
operator|+
literal|2
condition|)
return|return
literal|0
return|;
comment|/* new name is longer than that??? */
name|memcpy
argument_list|(
name|dp
argument_list|,
name|sp
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|dp
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|path
argument_list|,
operator|&
name|st
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|S_ISLNK
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
if|if
condition|(
name|last_symlink
condition|)
name|strcpy
argument_list|(
name|last_symlink
argument_list|,
name|path
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
name|dp
index|[
name|len
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
name|dp
operator|=
name|dp
operator|+
name|len
expr_stmt|;
name|sp
operator|=
name|ep
operator|+
literal|1
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

