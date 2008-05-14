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

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_function
DECL|function|pprint_tag
specifier|static
name|void
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
name|write_or_die
argument_list|(
literal|1
argument_list|,
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
name|write_or_die
argument_list|(
literal|1
argument_list|,
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
name|write_or_die
argument_list|(
literal|1
argument_list|,
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
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
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
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|cp
argument_list|,
name|endp
operator|-
name|cp
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_cat_file
name|int
name|cmd_cat_file
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
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|enum
name|object_type
name|type
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
specifier|const
name|char
modifier|*
name|exp_type
decl_stmt|,
modifier|*
name|obj_name
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|3
condition|)
name|usage
argument_list|(
literal|"git-cat-file [-t|-s|-e|-p|<type>]<sha1>"
argument_list|)
expr_stmt|;
name|exp_type
operator|=
name|argv
index|[
literal|1
index|]
expr_stmt|;
name|obj_name
operator|=
name|argv
index|[
literal|2
index|]
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|obj_name
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Not a valid object name %s"
argument_list|,
name|obj_name
argument_list|)
expr_stmt|;
name|opt
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|exp_type
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
name|opt
operator|=
name|exp_type
index|[
literal|1
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|opt
operator|||
name|exp_type
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
name|type
operator|=
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|>
literal|0
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
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
name|type
operator|=
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|>
literal|0
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
name|type
operator|=
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Not a valid object name %s"
argument_list|,
name|obj_name
argument_list|)
expr_stmt|;
comment|/* custom pretty-print here */
if|if
condition|(
name|type
operator|==
name|OBJ_TREE
condition|)
block|{
specifier|const
name|char
modifier|*
name|ls_args
index|[
literal|3
index|]
init|=
block|{
literal|"ls-tree"
block|,
name|obj_name
block|,
name|NULL
block|}
decl_stmt|;
return|return
name|cmd_ls_tree
argument_list|(
literal|2
argument_list|,
name|ls_args
argument_list|,
name|NULL
argument_list|)
return|;
block|}
name|buf
operator|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
operator|&
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
name|obj_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|==
name|OBJ_TAG
condition|)
block|{
name|pprint_tag
argument_list|(
name|sha1
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
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
name|exp_type
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
name|exp_type
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
name|obj_name
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
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

