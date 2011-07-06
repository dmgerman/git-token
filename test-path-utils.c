begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

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
if|if
condition|(
name|argc
operator|==
literal|3
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"normalize_absolute_path"
argument_list|)
condition|)
block|{
name|char
modifier|*
name|buf
init|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|)
operator|+
literal|1
argument_list|)
decl_stmt|;
name|int
name|rv
init|=
name|normalize_absolute_path
argument_list|(
name|buf
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
decl_stmt|;
name|assert
argument_list|(
name|strlen
argument_list|(
name|buf
argument_list|)
operator|==
name|rv
argument_list|)
expr_stmt|;
name|puts
argument_list|(
name|buf
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|>=
literal|2
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"make_absolute_path"
argument_list|)
condition|)
block|{
while|while
condition|(
name|argc
operator|>
literal|2
condition|)
block|{
name|puts
argument_list|(
name|make_absolute_path
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|)
argument_list|)
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
block|}
if|if
condition|(
name|argc
operator|==
literal|4
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"longest_ancestor_length"
argument_list|)
condition|)
block|{
name|int
name|len
init|=
name|longest_ancestor_length
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
argument_list|)
decl_stmt|;
name|printf
argument_list|(
literal|"%d\n"
argument_list|,
name|len
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

