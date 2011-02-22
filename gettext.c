begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2010 Ãvar ArnfjÃ¶rÃ° Bjarmason  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"gettext.h"
end_include

begin_function
DECL|function|use_gettext_poison
name|int
name|use_gettext_poison
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|int
name|poison_requested
init|=
operator|-
literal|1
decl_stmt|;
if|if
condition|(
name|poison_requested
operator|==
operator|-
literal|1
condition|)
name|poison_requested
operator|=
name|getenv
argument_list|(
literal|"GIT_GETTEXT_POISON"
argument_list|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
return|return
name|poison_requested
return|;
block|}
end_function

end_unit

