begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"urlmatch.h"
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
specifier|const
name|char
name|usage
index|[]
init|=
literal|"test-urlmatch-normalization [-p | -l]<url1> |<url1><url2>"
decl_stmt|;
name|char
modifier|*
name|url1
decl_stmt|,
modifier|*
name|url2
decl_stmt|;
name|int
name|opt_p
init|=
literal|0
decl_stmt|,
name|opt_l
init|=
literal|0
decl_stmt|;
comment|/* 	 * For one url, succeed if url_normalize succeeds on it, fail otherwise. 	 * For two urls, succeed only if url_normalize succeeds on both and 	 * the results compare equal with strcmp.  If -p is given (one url only) 	 * and url_normalize succeeds, print the result followed by "\n".  If 	 * -l is given (one url only) and url_normalize succeeds, print the 	 * returned length in decimal followed by "\n". 	 */
if|if
condition|(
name|argc
operator|>
literal|1
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-p"
argument_list|)
condition|)
block|{
name|opt_p
operator|=
literal|1
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argc
operator|>
literal|1
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-l"
argument_list|)
condition|)
block|{
name|opt_l
operator|=
literal|1
expr_stmt|;
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
operator|<
literal|2
operator|||
name|argc
operator|>
literal|3
condition|)
name|die
argument_list|(
literal|"%s"
argument_list|,
name|usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|==
literal|2
condition|)
block|{
name|struct
name|url_info
name|info
decl_stmt|;
name|url1
operator|=
name|url_normalize
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
operator|&
name|info
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|url1
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|opt_p
condition|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|url1
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_l
condition|)
name|printf
argument_list|(
literal|"%u\n"
argument_list|,
operator|(
name|unsigned
operator|)
name|info
operator|.
name|url_len
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|opt_p
operator|||
name|opt_l
condition|)
name|die
argument_list|(
literal|"%s"
argument_list|,
name|usage
argument_list|)
expr_stmt|;
name|url1
operator|=
name|url_normalize
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|url2
operator|=
name|url_normalize
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
return|return
operator|(
name|url1
operator|&&
name|url2
operator|&&
operator|!
name|strcmp
argument_list|(
name|url1
argument_list|,
name|url2
argument_list|)
operator|)
condition|?
literal|0
else|:
literal|1
return|;
block|}
end_function

end_unit

