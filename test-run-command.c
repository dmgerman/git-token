begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * test-run-command.c: test run command API.  *  * (C) 2009 Ilari Liusvaara<ilari.liusvaara@elisanet.fi>  *  * This code is free software; you can redistribute it and/or modify  * it under the terms of the GNU General Public License version 2 as  * published by the Free Software Foundation.  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_include
include|#
directive|include
file|<errno.h>
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
name|struct
name|child_process
name|proc
decl_stmt|;
name|memset
argument_list|(
operator|&
name|proc
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|proc
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|<
literal|3
condition|)
return|return
literal|1
return|;
name|proc
operator|.
name|argv
operator|=
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
name|argv
operator|+
literal|2
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"start-command-ENOENT"
argument_list|)
condition|)
block|{
if|if
condition|(
name|start_command
argument_list|(
operator|&
name|proc
argument_list|)
operator|<
literal|0
operator|&&
name|errno
operator|==
name|ENOENT
condition|)
return|return
literal|0
return|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"FAIL %s\n"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"check usage\n"
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

end_unit

