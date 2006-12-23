begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_function
DECL|function|read_or_die
name|void
name|read_or_die
parameter_list|(
name|int
name|fd
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|count
parameter_list|)
block|{
name|char
modifier|*
name|p
init|=
name|buf
decl_stmt|;
name|ssize_t
name|loaded
decl_stmt|;
while|while
condition|(
name|count
operator|>
literal|0
condition|)
block|{
name|loaded
operator|=
name|xread
argument_list|(
name|fd
argument_list|,
name|p
argument_list|,
name|count
argument_list|)
expr_stmt|;
if|if
condition|(
name|loaded
operator|==
literal|0
condition|)
name|die
argument_list|(
literal|"unexpected end of file"
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|loaded
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"read error (%s)"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|count
operator|-=
name|loaded
expr_stmt|;
name|p
operator|+=
name|loaded
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|write_or_die
name|void
name|write_or_die
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|count
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|p
init|=
name|buf
decl_stmt|;
name|ssize_t
name|written
decl_stmt|;
while|while
condition|(
name|count
operator|>
literal|0
condition|)
block|{
name|written
operator|=
name|xwrite
argument_list|(
name|fd
argument_list|,
name|p
argument_list|,
name|count
argument_list|)
expr_stmt|;
if|if
condition|(
name|written
operator|==
literal|0
condition|)
name|die
argument_list|(
literal|"disk full?"
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|written
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|==
name|EPIPE
condition|)
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"write error (%s)"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|count
operator|-=
name|written
expr_stmt|;
name|p
operator|+=
name|written
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|write_or_whine
name|int
name|write_or_whine
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|count
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|p
init|=
name|buf
decl_stmt|;
name|ssize_t
name|written
decl_stmt|;
while|while
condition|(
name|count
operator|>
literal|0
condition|)
block|{
name|written
operator|=
name|xwrite
argument_list|(
name|fd
argument_list|,
name|p
argument_list|,
name|count
argument_list|)
expr_stmt|;
if|if
condition|(
name|written
operator|==
literal|0
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s: disk full?\n"
argument_list|,
name|msg
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
elseif|else
if|if
condition|(
name|written
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|==
name|EPIPE
condition|)
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s: write error (%s)\n"
argument_list|,
name|msg
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|count
operator|-=
name|written
expr_stmt|;
name|p
operator|+=
name|written
expr_stmt|;
block|}
return|return
literal|1
return|;
block|}
end_function

end_unit

