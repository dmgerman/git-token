begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"../git-compat-util.h"
end_include

begin_function
DECL|function|gitstrdup
name|char
modifier|*
name|gitstrdup
parameter_list|(
specifier|const
name|char
modifier|*
name|s1
parameter_list|)
block|{
name|size_t
name|len
init|=
name|strlen
argument_list|(
name|s1
argument_list|)
operator|+
literal|1
decl_stmt|;
name|char
modifier|*
name|s2
init|=
name|malloc
argument_list|(
name|len
argument_list|)
decl_stmt|;
if|if
condition|(
name|s2
condition|)
name|memcpy
argument_list|(
name|s2
argument_list|,
name|s1
argument_list|,
name|len
argument_list|)
expr_stmt|;
return|return
name|s2
return|;
block|}
end_function

end_unit

