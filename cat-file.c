begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

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
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|void
modifier|*
name|buf
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|int
name|opt
decl_stmt|;
name|setup_git_directory
argument_list|()
expr_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|3
operator|||
name|get_sha1
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|sha1
argument_list|)
condition|)
name|usage
argument_list|(
literal|"git-cat-file [-t|-s|-e|<type>]<sha1>"
argument_list|)
expr_stmt|;
name|opt
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|argv
index|[
literal|1
index|]
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
name|opt
operator|=
name|argv
index|[
literal|1
index|]
index|[
literal|1
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|opt
operator|||
name|argv
index|[
literal|1
index|]
index|[
literal|2
index|]
condition|)
name|opt
operator|=
operator|-
literal|1
expr_stmt|;
comment|/* Not a single character option */
block|}
name|buf
operator|=
name|NULL
expr_stmt|;
switch|switch
condition|(
name|opt
condition|)
block|{
case|case
literal|'t'
case|:
if|if
condition|(
operator|!
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|type
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
break|break;
case|case
literal|'s'
case|:
if|if
condition|(
operator|!
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|size
argument_list|)
condition|)
block|{
name|printf
argument_list|(
literal|"%lu\n"
argument_list|,
name|size
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
break|break;
case|case
literal|'e'
case|:
return|return
operator|!
name|has_sha1_file
argument_list|(
name|sha1
argument_list|)
return|;
case|case
literal|0
case|:
name|buf
operator|=
name|read_object_with_reference
argument_list|(
name|sha1
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|,
operator|&
name|size
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
break|break;
default|default:
name|die
argument_list|(
literal|"git-cat-file: unknown option: %s\n"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|buf
condition|)
name|die
argument_list|(
literal|"git-cat-file %s: bad file"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
while|while
condition|(
name|size
operator|>
literal|0
condition|)
block|{
name|long
name|ret
init|=
name|write
argument_list|(
literal|1
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
name|ret
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|==
name|EAGAIN
condition|)
continue|continue;
comment|/* Ignore epipe */
if|if
condition|(
name|errno
operator|==
name|EPIPE
condition|)
break|break;
name|die
argument_list|(
literal|"git-cat-file: %s"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|ret
condition|)
block|{
name|die
argument_list|(
literal|"git-cat-file: disk full?"
argument_list|)
expr_stmt|;
block|}
name|size
operator|-=
name|ret
expr_stmt|;
name|buf
operator|+=
name|ret
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

