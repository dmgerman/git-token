begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<time.h>
end_include

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
name|char
name|result
index|[
literal|100
index|]
decl_stmt|;
name|time_t
name|t
decl_stmt|;
name|memcpy
argument_list|(
name|result
argument_list|,
literal|"bad"
argument_list|,
literal|4
argument_list|)
expr_stmt|;
name|parse_date
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
name|result
argument_list|,
sizeof|sizeof
argument_list|(
name|result
argument_list|)
argument_list|)
expr_stmt|;
name|t
operator|=
name|strtoul
argument_list|(
name|result
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s -> %s -> %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
name|result
argument_list|,
name|ctime
argument_list|(
operator|&
name|t
argument_list|)
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

