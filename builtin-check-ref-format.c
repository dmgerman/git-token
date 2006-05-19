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
file|"builtin.h"
end_include

begin_function
DECL|function|cmd_check_ref_format
name|int
name|cmd_check_ref_format
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
name|char
modifier|*
modifier|*
name|envp
parameter_list|)
block|{
if|if
condition|(
name|argc
operator|!=
literal|2
condition|)
name|usage
argument_list|(
literal|"git check-ref-format refname"
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|check_ref_format
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
return|;
block|}
end_function

end_unit

