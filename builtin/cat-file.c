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

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"userdiff.h"
end_include

begin_include
include|#
directive|include
file|"streaming.h"
end_include

begin_define
DECL|macro|BATCH
define|#
directive|define
name|BATCH
value|1
end_define

begin_define
DECL|macro|BATCH_CHECK
define|#
directive|define
name|BATCH_CHECK
value|2
end_define

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
DECL|function|cat_one_file
specifier|static
name|int
name|cat_one_file
parameter_list|(
name|int
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|exp_type
parameter_list|,
specifier|const
name|char
modifier|*
name|obj_name
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
name|char
modifier|*
name|buf
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|struct
name|object_context
name|obj_context
decl_stmt|;
if|if
condition|(
name|get_sha1_with_context
argument_list|(
name|obj_name
argument_list|,
name|sha1
argument_list|,
operator|&
name|obj_context
argument_list|)
condition|)
name|die
argument_list|(
literal|"Not a valid object name %s"
argument_list|,
name|obj_name
argument_list|)
expr_stmt|;
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
name|NULL
block|}
decl_stmt|;
name|ls_args
index|[
literal|0
index|]
operator|=
literal|"ls-tree"
expr_stmt|;
name|ls_args
index|[
literal|1
index|]
operator|=
name|obj_name
expr_stmt|;
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
if|if
condition|(
name|type
operator|==
name|OBJ_BLOB
condition|)
return|return
name|stream_blob_to_fd
argument_list|(
literal|1
argument_list|,
name|sha1
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
return|;
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
literal|'c'
case|:
if|if
condition|(
operator|!
name|obj_context
operator|.
name|path
index|[
literal|0
index|]
condition|)
name|die
argument_list|(
literal|"git cat-file --textconv %s:<object> must be<sha1:path>"
argument_list|,
name|obj_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|textconv_object
argument_list|(
name|obj_context
operator|.
name|path
argument_list|,
name|obj_context
operator|.
name|mode
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|,
operator|&
name|buf
argument_list|,
operator|&
name|size
argument_list|)
condition|)
name|die
argument_list|(
literal|"git cat-file --textconv: unable to run textconv on %s"
argument_list|,
name|obj_name
argument_list|)
expr_stmt|;
break|break;
case|case
literal|0
case|:
if|if
condition|(
name|type_from_string
argument_list|(
name|exp_type
argument_list|)
operator|==
name|OBJ_BLOB
condition|)
block|{
name|unsigned
name|char
name|blob_sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|)
operator|==
name|OBJ_TAG
condition|)
block|{
name|enum
name|object_type
name|type
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|char
modifier|*
name|buffer
init|=
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
decl_stmt|;
if|if
condition|(
name|memcmp
argument_list|(
name|buffer
argument_list|,
literal|"object "
argument_list|,
literal|7
argument_list|)
operator|||
name|get_sha1_hex
argument_list|(
name|buffer
operator|+
literal|7
argument_list|,
name|blob_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s not a valid tag"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
block|}
else|else
name|hashcpy
argument_list|(
name|blob_sha1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|sha1_object_info
argument_list|(
name|blob_sha1
argument_list|,
name|NULL
argument_list|)
operator|==
name|OBJ_BLOB
condition|)
return|return
name|stream_blob_to_fd
argument_list|(
literal|1
argument_list|,
name|blob_sha1
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
return|;
comment|/* 			 * we attempted to dereference a tag to a blob 			 * and failed; there may be new dereference 			 * mechanisms this code is not aware of. 			 * fall-back to the usual case. 			 */
block|}
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
literal|"git cat-file: unknown option: %s"
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
literal|"git cat-file %s: bad file"
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

begin_function
DECL|function|batch_one_object
specifier|static
name|int
name|batch_one_object
parameter_list|(
specifier|const
name|char
modifier|*
name|obj_name
parameter_list|,
name|int
name|print_contents
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
init|=
literal|0
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|void
modifier|*
name|contents
init|=
name|contents
decl_stmt|;
if|if
condition|(
operator|!
name|obj_name
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|get_sha1
argument_list|(
name|obj_name
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|printf
argument_list|(
literal|"%s missing\n"
argument_list|,
name|obj_name
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|print_contents
operator|==
name|BATCH
condition|)
name|contents
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
else|else
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
operator|<=
literal|0
condition|)
block|{
name|printf
argument_list|(
literal|"%s missing\n"
argument_list|,
name|obj_name
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
name|print_contents
operator|==
name|BATCH
condition|)
name|free
argument_list|(
name|contents
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|printf
argument_list|(
literal|"%s %s %lu\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
name|print_contents
operator|==
name|BATCH
condition|)
block|{
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|contents
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|contents
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|batch_objects
specifier|static
name|int
name|batch_objects
parameter_list|(
name|int
name|print_contents
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
while|while
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|buf
argument_list|,
name|stdin
argument_list|,
literal|'\n'
argument_list|)
operator|!=
name|EOF
condition|)
block|{
name|int
name|error
init|=
name|batch_one_object
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
name|print_contents
argument_list|)
decl_stmt|;
if|if
condition|(
name|error
condition|)
return|return
name|error
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|cat_file_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|cat_file_usage
index|[]
init|=
block|{
literal|"git cat-file (-t|-s|-e|-p|<type>|--textconv)<object>"
block|,
literal|"git cat-file (--batch|--batch-check)<<list_of_objects>"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|git_cat_file_config
specifier|static
name|int
name|git_cat_file_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
block|{
if|if
condition|(
name|userdiff_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
return|return
name|git_default_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
name|cb
argument_list|)
return|;
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
name|int
name|opt
init|=
literal|0
decl_stmt|,
name|batch
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|exp_type
init|=
name|NULL
decl_stmt|,
modifier|*
name|obj_name
init|=
name|NULL
decl_stmt|;
specifier|const
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_GROUP
argument_list|(
literal|"<type> can be one of: blob, tree, commit, tag"
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|'t'
argument_list|,
name|NULL
argument_list|,
operator|&
name|opt
argument_list|,
literal|"show object type"
argument_list|,
literal|'t'
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|'s'
argument_list|,
name|NULL
argument_list|,
operator|&
name|opt
argument_list|,
literal|"show object size"
argument_list|,
literal|'s'
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|'e'
argument_list|,
name|NULL
argument_list|,
operator|&
name|opt
argument_list|,
literal|"exit with zero when there's no error"
argument_list|,
literal|'e'
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|'p'
argument_list|,
name|NULL
argument_list|,
operator|&
name|opt
argument_list|,
literal|"pretty-print object's content"
argument_list|,
literal|'p'
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"textconv"
argument_list|,
operator|&
name|opt
argument_list|,
literal|"for blob objects, run textconv on object's content"
argument_list|,
literal|'c'
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"batch"
argument_list|,
operator|&
name|batch
argument_list|,
literal|"show info and content of objects fed from the standard input"
argument_list|,
name|BATCH
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"batch-check"
argument_list|,
operator|&
name|batch
argument_list|,
literal|"show info about objects fed from the standard input"
argument_list|,
name|BATCH_CHECK
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|git_config
argument_list|(
name|git_cat_file_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|3
operator|&&
name|argc
operator|!=
literal|2
condition|)
name|usage_with_options
argument_list|(
name|cat_file_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
name|options
argument_list|,
name|cat_file_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt
condition|)
block|{
if|if
condition|(
name|argc
operator|==
literal|1
condition|)
name|obj_name
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
else|else
name|usage_with_options
argument_list|(
name|cat_file_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|opt
operator|&&
operator|!
name|batch
condition|)
block|{
if|if
condition|(
name|argc
operator|==
literal|2
condition|)
block|{
name|exp_type
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
name|obj_name
operator|=
name|argv
index|[
literal|1
index|]
expr_stmt|;
block|}
else|else
name|usage_with_options
argument_list|(
name|cat_file_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|batch
operator|&&
operator|(
name|opt
operator|||
name|argc
operator|)
condition|)
block|{
name|usage_with_options
argument_list|(
name|cat_file_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|batch
condition|)
return|return
name|batch_objects
argument_list|(
name|batch
argument_list|)
return|;
return|return
name|cat_one_file
argument_list|(
name|opt
argument_list|,
name|exp_type
argument_list|,
name|obj_name
argument_list|)
return|;
block|}
end_function

end_unit

