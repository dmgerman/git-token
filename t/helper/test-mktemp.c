begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * test-mktemp.c: code to exercise the creation of temporary files  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
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
name|argv
index|[]
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
literal|"Expected 1 parameter defining the temporary file template"
argument_list|)
expr_stmt|;
name|xmkstemp
argument_list|(
name|xstrdup
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

