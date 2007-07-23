begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_comment
comment|/*  * Returns the length of a line, without trailing spaces.  *  * If the line ends with newline, it will be removed too.  */
end_comment

begin_function
DECL|function|cleanup
specifier|static
name|size_t
name|cleanup
parameter_list|(
name|char
modifier|*
name|line
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
if|if
condition|(
name|len
condition|)
block|{
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
name|len
operator|--
expr_stmt|;
while|while
condition|(
name|len
condition|)
block|{
name|unsigned
name|char
name|c
init|=
name|line
index|[
name|len
operator|-
literal|1
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|isspace
argument_list|(
name|c
argument_list|)
condition|)
break|break;
name|len
operator|--
expr_stmt|;
block|}
block|}
return|return
name|len
return|;
block|}
end_function

begin_comment
comment|/*  * Remove empty lines from the beginning and end  * and also trailing spaces from every line.  *  * Note that the buffer will not be NUL-terminated.  *  * Turn multiple consecutive empty lines between paragraphs  * into just one empty line.  *  * If the input has only empty lines and spaces,  * no output will be produced.  *  * If last line has a newline at the end, it will be removed.  *  * Enable skip_comments to skip every line starting with "#".  */
end_comment

begin_function
DECL|function|stripspace
name|size_t
name|stripspace
parameter_list|(
name|char
modifier|*
name|buffer
parameter_list|,
name|size_t
name|length
parameter_list|,
name|int
name|skip_comments
parameter_list|)
block|{
name|int
name|empties
init|=
operator|-
literal|1
decl_stmt|;
name|size_t
name|i
decl_stmt|,
name|j
decl_stmt|,
name|len
decl_stmt|,
name|newlen
decl_stmt|;
name|char
modifier|*
name|eol
decl_stmt|;
for|for
control|(
name|i
operator|=
name|j
operator|=
literal|0
init|;
name|i
operator|<
name|length
condition|;
name|i
operator|+=
name|len
operator|,
name|j
operator|+=
name|newlen
control|)
block|{
name|eol
operator|=
name|memchr
argument_list|(
name|buffer
operator|+
name|i
argument_list|,
literal|'\n'
argument_list|,
name|length
operator|-
name|i
argument_list|)
expr_stmt|;
name|len
operator|=
name|eol
condition|?
name|eol
operator|-
operator|(
name|buffer
operator|+
name|i
operator|)
operator|+
literal|1
else|:
name|length
operator|-
name|i
expr_stmt|;
if|if
condition|(
name|skip_comments
operator|&&
name|len
operator|&&
name|buffer
index|[
name|i
index|]
operator|==
literal|'#'
condition|)
block|{
name|newlen
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
name|newlen
operator|=
name|cleanup
argument_list|(
name|buffer
operator|+
name|i
argument_list|,
name|len
argument_list|)
expr_stmt|;
comment|/* Not just an empty line? */
if|if
condition|(
name|newlen
condition|)
block|{
if|if
condition|(
name|empties
operator|!=
operator|-
literal|1
condition|)
name|buffer
index|[
name|j
operator|++
index|]
operator|=
literal|'\n'
expr_stmt|;
if|if
condition|(
name|empties
operator|>
literal|0
condition|)
name|buffer
index|[
name|j
operator|++
index|]
operator|=
literal|'\n'
expr_stmt|;
name|empties
operator|=
literal|0
expr_stmt|;
name|memmove
argument_list|(
name|buffer
operator|+
name|j
argument_list|,
name|buffer
operator|+
name|i
argument_list|,
name|newlen
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|empties
operator|<
literal|0
condition|)
continue|continue;
name|empties
operator|++
expr_stmt|;
block|}
return|return
name|j
return|;
block|}
end_function

begin_function
DECL|function|cmd_stripspace
name|int
name|cmd_stripspace
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
name|char
modifier|*
name|buffer
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|int
name|strip_comments
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|argc
operator|>
literal|1
operator|&&
operator|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-s"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--strip-comments"
argument_list|)
operator|)
condition|)
name|strip_comments
operator|=
literal|1
expr_stmt|;
name|size
operator|=
literal|1024
expr_stmt|;
name|buffer
operator|=
name|xmalloc
argument_list|(
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_fd
argument_list|(
literal|0
argument_list|,
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"could not read the input"
argument_list|)
expr_stmt|;
block|}
name|size
operator|=
name|stripspace
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|,
name|strip_comments
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|size
condition|)
name|putc
argument_list|(
literal|'\n'
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

