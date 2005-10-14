begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  */
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
file|<stdio.h>
end_include

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|ac
parameter_list|,
name|char
modifier|*
modifier|*
name|av
parameter_list|)
block|{
if|if
condition|(
name|ac
operator|!=
literal|2
condition|)
name|usage
argument_list|(
literal|"git-check-ref-format refname"
argument_list|)
expr_stmt|;
if|if
condition|(
name|check_ref_format
argument_list|(
name|av
index|[
literal|1
index|]
argument_list|)
condition|)
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

