begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_function
DECL|function|flush_buffer
specifier|static
name|void
name|flush_buffer
parameter_list|(
specifier|const
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
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
name|xwrite
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
block|}
end_function

begin_function
DECL|function|pprint_tag
specifier|static
name|int
name|pprint_tag
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
comment|/* the parser in tag.c is useless here. */
specifier|const
name|char
modifier|*
name|endp
init|=
name|buf
operator|+
name|size
decl_stmt|;
specifier|const
name|char
modifier|*
name|cp
init|=
name|buf
decl_stmt|;
while|while
condition|(
name|cp
operator|<
name|endp
condition|)
block|{
name|char
name|c
init|=
operator|*
name|cp
operator|++
decl_stmt|;
if|if
condition|(
name|c
operator|!=
literal|'\n'
condition|)
continue|continue;
if|if
condition|(
literal|7
operator|<=
name|endp
operator|-
name|cp
operator|&&
operator|!
name|memcmp
argument_list|(
literal|"tagger "
argument_list|,
name|cp
argument_list|,
literal|7
argument_list|)
condition|)
block|{
specifier|const
name|char
modifier|*
name|tagger
init|=
name|cp
decl_stmt|;
comment|/* Found the tagger line.  Copy out the contents 			 * of the buffer so far. 			 */
name|flush_buffer
argument_list|(
name|buf
argument_list|,
name|cp
operator|-
name|buf
argument_list|)
expr_stmt|;
comment|/* 			 * Do something intelligent, like pretty-printing 			 * the date. 			 */
while|while
condition|(
name|cp
operator|<
name|endp
condition|)
block|{
if|if
condition|(
operator|*
name|cp
operator|++
operator|==
literal|'\n'
condition|)
block|{
comment|/* tagger to cp is a line 					 * that has ident and time. 					 */
specifier|const
name|char
modifier|*
name|sp
init|=
name|tagger
decl_stmt|;
name|char
modifier|*
name|ep
decl_stmt|;
name|unsigned
name|long
name|date
decl_stmt|;
name|long
name|tz
decl_stmt|;
while|while
condition|(
name|sp
operator|<
name|cp
operator|&&
operator|*
name|sp
operator|!=
literal|'>'
condition|)
name|sp
operator|++
expr_stmt|;
if|if
condition|(
name|sp
operator|==
name|cp
condition|)
block|{
comment|/* give up */
name|flush_buffer
argument_list|(
name|tagger
argument_list|,
name|cp
operator|-
name|tagger
argument_list|)
expr_stmt|;
break|break;
block|}
while|while
condition|(
name|sp
operator|<
name|cp
operator|&&
operator|!
operator|(
literal|'0'
operator|<=
operator|*
name|sp
operator|&&
operator|*
name|sp
operator|<=
literal|'9'
operator|)
condition|)
name|sp
operator|++
expr_stmt|;
name|flush_buffer
argument_list|(
name|tagger
argument_list|,
name|sp
operator|-
name|tagger
argument_list|)
expr_stmt|;
name|date
operator|=
name|strtoul
argument_list|(
name|sp
argument_list|,
operator|&
name|ep
argument_list|,
literal|10
argument_list|)
expr_stmt|;
name|tz
operator|=
name|strtol
argument_list|(
name|ep
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
name|sp
operator|=
name|show_date
argument_list|(
name|date
argument_list|,
name|tz
argument_list|)
expr_stmt|;
name|flush_buffer
argument_list|(
name|sp
argument_list|,
name|strlen
argument_list|(
name|sp
argument_list|)
argument_list|)
expr_stmt|;
name|xwrite
argument_list|(
literal|1
argument_list|,
literal|"\n"
argument_list|,
literal|1
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
break|break;
block|}
if|if
condition|(
name|cp
operator|<
name|endp
operator|&&
operator|*
name|cp
operator|==
literal|'\n'
condition|)
comment|/* end of header */
break|break;
block|}
comment|/* At this point, we have copied out the header up to the end of 	 * the tagger line and cp points at one past \n.  It could be the 	 * next header line after the tagger line, or it could be another 	 * \n that marks the end of the headers.  We need to copy out the 	 * remainder as is. 	 */
if|if
condition|(
name|cp
operator|<
name|endp
condition|)
name|flush_buffer
argument_list|(
name|cp
argument_list|,
name|endp
operator|-
name|cp
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

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
name|git_config
argument_list|(
name|git_default_config
argument_list|)
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
literal|"git-cat-file [-t|-s|-e|-p|<type>]<sha1>"
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
literal|'p'
case|:
if|if
condition|(
name|get_sha1
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|sha1
argument_list|)
operator|||
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
name|NULL
argument_list|)
condition|)
name|die
argument_list|(
literal|"Not a valid object name %s"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
comment|/* custom pretty-print here */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tree"
argument_list|)
condition|)
return|return
name|execl_git_cmd
argument_list|(
literal|"ls-tree"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|,
name|NULL
argument_list|)
return|;
name|buf
operator|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
name|die
argument_list|(
literal|"Cannot read object %s"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tag"
argument_list|)
condition|)
return|return
name|pprint_tag
argument_list|(
name|sha1
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
return|;
comment|/* otherwise just spit out the data */
break|break;
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
name|flush_buffer
argument_list|(
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

