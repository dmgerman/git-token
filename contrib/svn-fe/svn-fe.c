begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * This file is in the public domain.  * You may freely use, modify, distribute, and relicense it.  */
end_comment

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_include
include|#
directive|include
file|"svndump.h"
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
name|svndump_init
argument_list|(
name|NULL
argument_list|)
condition|)
return|return
literal|1
return|;
name|svndump_read
argument_list|(
operator|(
name|argc
operator|>
literal|1
operator|)
condition|?
name|argv
index|[
literal|1
index|]
else|:
name|NULL
argument_list|,
literal|"refs/heads/master"
argument_list|)
expr_stmt|;
name|svndump_deinit
argument_list|()
expr_stmt|;
name|svndump_reset
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

