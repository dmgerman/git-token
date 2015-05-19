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
file|"userdiff.h"
end_include

begin_include
include|#
directive|include
file|"streaming.h"
end_include

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
parameter_list|,
name|int
name|unknown_type
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
name|struct
name|object_info
name|oi
init|=
block|{
name|NULL
block|}
decl_stmt|;
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|unsigned
name|flags
init|=
name|LOOKUP_REPLACE_OBJECT
decl_stmt|;
if|if
condition|(
name|unknown_type
condition|)
name|flags
operator||=
name|LOOKUP_UNKNOWN_OBJECT
expr_stmt|;
if|if
condition|(
name|get_sha1_with_context
argument_list|(
name|obj_name
argument_list|,
literal|0
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
name|oi
operator|.
name|typename
operator|=
operator|&
name|sb
expr_stmt|;
if|if
condition|(
name|sha1_object_info_extended
argument_list|(
name|sha1
argument_list|,
operator|&
name|oi
argument_list|,
name|flags
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"git cat-file: could not get object info"
argument_list|)
expr_stmt|;
if|if
condition|(
name|sb
operator|.
name|len
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sb
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|sb
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
name|oi
operator|.
name|sizep
operator|=
operator|&
name|size
expr_stmt|;
if|if
condition|(
name|sha1_object_info_extended
argument_list|(
name|sha1
argument_list|,
operator|&
name|oi
argument_list|,
name|flags
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"git cat-file: could not get object info"
argument_list|)
expr_stmt|;
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
break|break;
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
comment|/* otherwise just spit out the data */
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
specifier|const
name|char
modifier|*
name|target
decl_stmt|;
if|if
condition|(
operator|!
name|skip_prefix
argument_list|(
name|buffer
argument_list|,
literal|"object "
argument_list|,
operator|&
name|target
argument_list|)
operator|||
name|get_sha1_hex
argument_list|(
name|target
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

begin_struct
DECL|struct|expand_data
struct|struct
name|expand_data
block|{
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|type
name|enum
name|object_type
name|type
decl_stmt|;
DECL|member|size
name|unsigned
name|long
name|size
decl_stmt|;
DECL|member|disk_size
name|unsigned
name|long
name|disk_size
decl_stmt|;
DECL|member|rest
specifier|const
name|char
modifier|*
name|rest
decl_stmt|;
DECL|member|delta_base_sha1
name|unsigned
name|char
name|delta_base_sha1
index|[
literal|20
index|]
decl_stmt|;
comment|/* 	 * If mark_query is true, we do not expand anything, but rather 	 * just mark the object_info with items we wish to query. 	 */
DECL|member|mark_query
name|int
name|mark_query
decl_stmt|;
comment|/* 	 * Whether to split the input on whitespace before feeding it to 	 * get_sha1; this is decided during the mark_query phase based on 	 * whether we have a %(rest) token in our format. 	 */
DECL|member|split_on_whitespace
name|int
name|split_on_whitespace
decl_stmt|;
comment|/* 	 * After a mark_query run, this object_info is set up to be 	 * passed to sha1_object_info_extended. It will point to the data 	 * elements above, so you can retrieve the response from there. 	 */
DECL|member|info
name|struct
name|object_info
name|info
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|is_atom
specifier|static
name|int
name|is_atom
parameter_list|(
specifier|const
name|char
modifier|*
name|atom
parameter_list|,
specifier|const
name|char
modifier|*
name|s
parameter_list|,
name|int
name|slen
parameter_list|)
block|{
name|int
name|alen
init|=
name|strlen
argument_list|(
name|atom
argument_list|)
decl_stmt|;
return|return
name|alen
operator|==
name|slen
operator|&&
operator|!
name|memcmp
argument_list|(
name|atom
argument_list|,
name|s
argument_list|,
name|alen
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|expand_atom
specifier|static
name|void
name|expand_atom
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|atom
parameter_list|,
name|int
name|len
parameter_list|,
name|void
modifier|*
name|vdata
parameter_list|)
block|{
name|struct
name|expand_data
modifier|*
name|data
init|=
name|vdata
decl_stmt|;
if|if
condition|(
name|is_atom
argument_list|(
literal|"objectname"
argument_list|,
name|atom
argument_list|,
name|len
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|data
operator|->
name|mark_query
condition|)
name|strbuf_addstr
argument_list|(
name|sb
argument_list|,
name|sha1_to_hex
argument_list|(
name|data
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|is_atom
argument_list|(
literal|"objecttype"
argument_list|,
name|atom
argument_list|,
name|len
argument_list|)
condition|)
block|{
if|if
condition|(
name|data
operator|->
name|mark_query
condition|)
name|data
operator|->
name|info
operator|.
name|typep
operator|=
operator|&
name|data
operator|->
name|type
expr_stmt|;
else|else
name|strbuf_addstr
argument_list|(
name|sb
argument_list|,
name|typename
argument_list|(
name|data
operator|->
name|type
argument_list|)
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|is_atom
argument_list|(
literal|"objectsize"
argument_list|,
name|atom
argument_list|,
name|len
argument_list|)
condition|)
block|{
if|if
condition|(
name|data
operator|->
name|mark_query
condition|)
name|data
operator|->
name|info
operator|.
name|sizep
operator|=
operator|&
name|data
operator|->
name|size
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
name|sb
argument_list|,
literal|"%lu"
argument_list|,
name|data
operator|->
name|size
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|is_atom
argument_list|(
literal|"objectsize:disk"
argument_list|,
name|atom
argument_list|,
name|len
argument_list|)
condition|)
block|{
if|if
condition|(
name|data
operator|->
name|mark_query
condition|)
name|data
operator|->
name|info
operator|.
name|disk_sizep
operator|=
operator|&
name|data
operator|->
name|disk_size
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
name|sb
argument_list|,
literal|"%lu"
argument_list|,
name|data
operator|->
name|disk_size
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|is_atom
argument_list|(
literal|"rest"
argument_list|,
name|atom
argument_list|,
name|len
argument_list|)
condition|)
block|{
if|if
condition|(
name|data
operator|->
name|mark_query
condition|)
name|data
operator|->
name|split_on_whitespace
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
name|data
operator|->
name|rest
condition|)
name|strbuf_addstr
argument_list|(
name|sb
argument_list|,
name|data
operator|->
name|rest
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|is_atom
argument_list|(
literal|"deltabase"
argument_list|,
name|atom
argument_list|,
name|len
argument_list|)
condition|)
block|{
if|if
condition|(
name|data
operator|->
name|mark_query
condition|)
name|data
operator|->
name|info
operator|.
name|delta_base_sha1
operator|=
name|data
operator|->
name|delta_base_sha1
expr_stmt|;
else|else
name|strbuf_addstr
argument_list|(
name|sb
argument_list|,
name|sha1_to_hex
argument_list|(
name|data
operator|->
name|delta_base_sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
else|else
name|die
argument_list|(
literal|"unknown format element: %.*s"
argument_list|,
name|len
argument_list|,
name|atom
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|expand_format
specifier|static
name|size_t
name|expand_format
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|start
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|end
decl_stmt|;
if|if
condition|(
operator|*
name|start
operator|!=
literal|'('
condition|)
return|return
literal|0
return|;
name|end
operator|=
name|strchr
argument_list|(
name|start
operator|+
literal|1
argument_list|,
literal|')'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|end
condition|)
name|die
argument_list|(
literal|"format element '%s' does not end in ')'"
argument_list|,
name|start
argument_list|)
expr_stmt|;
name|expand_atom
argument_list|(
name|sb
argument_list|,
name|start
operator|+
literal|1
argument_list|,
name|end
operator|-
name|start
operator|-
literal|1
argument_list|,
name|data
argument_list|)
expr_stmt|;
return|return
name|end
operator|-
name|start
operator|+
literal|1
return|;
block|}
end_function

begin_function
DECL|function|print_object_or_die
specifier|static
name|void
name|print_object_or_die
parameter_list|(
name|int
name|fd
parameter_list|,
name|struct
name|expand_data
modifier|*
name|data
parameter_list|)
block|{
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|data
operator|->
name|sha1
decl_stmt|;
name|assert
argument_list|(
name|data
operator|->
name|info
operator|.
name|typep
argument_list|)
expr_stmt|;
if|if
condition|(
name|data
operator|->
name|type
operator|==
name|OBJ_BLOB
condition|)
block|{
if|if
condition|(
name|stream_blob_to_fd
argument_list|(
name|fd
argument_list|,
name|sha1
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to stream %s to stdout"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|enum
name|object_type
name|type
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|void
modifier|*
name|contents
decl_stmt|;
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
if|if
condition|(
operator|!
name|contents
condition|)
name|die
argument_list|(
literal|"object %s disappeared"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|!=
name|data
operator|->
name|type
condition|)
name|die
argument_list|(
literal|"object %s changed type!?"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|data
operator|->
name|info
operator|.
name|sizep
operator|&&
name|size
operator|!=
name|data
operator|->
name|size
condition|)
name|die
argument_list|(
literal|"object %s changed size!?"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
name|fd
argument_list|,
name|contents
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|contents
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_struct
DECL|struct|batch_options
struct|struct
name|batch_options
block|{
DECL|member|enabled
name|int
name|enabled
decl_stmt|;
DECL|member|print_contents
name|int
name|print_contents
decl_stmt|;
DECL|member|format
specifier|const
name|char
modifier|*
name|format
decl_stmt|;
block|}
struct|;
end_struct

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
name|struct
name|batch_options
modifier|*
name|opt
parameter_list|,
name|struct
name|expand_data
modifier|*
name|data
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
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
name|data
operator|->
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
name|sha1_object_info_extended
argument_list|(
name|data
operator|->
name|sha1
argument_list|,
operator|&
name|data
operator|->
name|info
argument_list|,
name|LOOKUP_REPLACE_OBJECT
argument_list|)
operator|<
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
return|return
literal|0
return|;
block|}
name|strbuf_expand
argument_list|(
operator|&
name|buf
argument_list|,
name|opt
operator|->
name|format
argument_list|,
name|expand_format
argument_list|,
name|data
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
operator|&
name|buf
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|buf
operator|.
name|buf
argument_list|,
name|buf
operator|.
name|len
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt
operator|->
name|print_contents
condition|)
block|{
name|print_object_or_die
argument_list|(
literal|1
argument_list|,
name|data
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
literal|"\n"
argument_list|,
literal|1
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
name|struct
name|batch_options
modifier|*
name|opt
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|expand_data
name|data
decl_stmt|;
name|int
name|save_warning
decl_stmt|;
name|int
name|retval
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|opt
operator|->
name|format
condition|)
name|opt
operator|->
name|format
operator|=
literal|"%(objectname) %(objecttype) %(objectsize)"
expr_stmt|;
comment|/* 	 * Expand once with our special mark_query flag, which will prime the 	 * object_info to be handed to sha1_object_info_extended for each 	 * object. 	 */
name|memset
argument_list|(
operator|&
name|data
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|data
argument_list|)
argument_list|)
expr_stmt|;
name|data
operator|.
name|mark_query
operator|=
literal|1
expr_stmt|;
name|strbuf_expand
argument_list|(
operator|&
name|buf
argument_list|,
name|opt
operator|->
name|format
argument_list|,
name|expand_format
argument_list|,
operator|&
name|data
argument_list|)
expr_stmt|;
name|data
operator|.
name|mark_query
operator|=
literal|0
expr_stmt|;
comment|/* 	 * If we are printing out the object, then always fill in the type, 	 * since we will want to decide whether or not to stream. 	 */
if|if
condition|(
name|opt
operator|->
name|print_contents
condition|)
name|data
operator|.
name|info
operator|.
name|typep
operator|=
operator|&
name|data
operator|.
name|type
expr_stmt|;
comment|/* 	 * We are going to call get_sha1 on a potentially very large number of 	 * objects. In most large cases, these will be actual object sha1s. The 	 * cost to double-check that each one is not also a ref (just so we can 	 * warn) ends up dwarfing the actual cost of the object lookups 	 * themselves. We can work around it by just turning off the warning. 	 */
name|save_warning
operator|=
name|warn_on_object_refname_ambiguity
expr_stmt|;
name|warn_on_object_refname_ambiguity
operator|=
literal|0
expr_stmt|;
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
if|if
condition|(
name|data
operator|.
name|split_on_whitespace
condition|)
block|{
comment|/* 			 * Split at first whitespace, tying off the beginning 			 * of the string and saving the remainder (or NULL) in 			 * data.rest. 			 */
name|char
modifier|*
name|p
init|=
name|strpbrk
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|" \t"
argument_list|)
decl_stmt|;
if|if
condition|(
name|p
condition|)
block|{
while|while
condition|(
operator|*
name|p
operator|&&
name|strchr
argument_list|(
literal|" \t"
argument_list|,
operator|*
name|p
argument_list|)
condition|)
operator|*
name|p
operator|++
operator|=
literal|'\0'
expr_stmt|;
block|}
name|data
operator|.
name|rest
operator|=
name|p
expr_stmt|;
block|}
name|retval
operator|=
name|batch_one_object
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
name|opt
argument_list|,
operator|&
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|retval
condition|)
break|break;
block|}
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
name|warn_on_object_refname_ambiguity
operator|=
name|save_warning
expr_stmt|;
return|return
name|retval
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
name|N_
argument_list|(
literal|"git cat-file (-t [--allow-unknown-type]|-s [--allow-unknown-type]|-e|-p|<type>|--textconv)<object>"
argument_list|)
block|,
name|N_
argument_list|(
literal|"git cat-file (--batch | --batch-check)<<list-of-objects>"
argument_list|)
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
DECL|function|batch_option_callback
specifier|static
name|int
name|batch_option_callback
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|unset
parameter_list|)
block|{
name|struct
name|batch_options
modifier|*
name|bo
init|=
name|opt
operator|->
name|value
decl_stmt|;
if|if
condition|(
name|unset
condition|)
block|{
name|memset
argument_list|(
name|bo
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|bo
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|bo
operator|->
name|enabled
operator|=
literal|1
expr_stmt|;
name|bo
operator|->
name|print_contents
operator|=
operator|!
name|strcmp
argument_list|(
name|opt
operator|->
name|long_name
argument_list|,
literal|"batch"
argument_list|)
expr_stmt|;
name|bo
operator|->
name|format
operator|=
name|arg
expr_stmt|;
return|return
literal|0
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
name|struct
name|batch_options
name|batch
init|=
block|{
literal|0
block|}
decl_stmt|;
name|int
name|unknown_type
init|=
literal|0
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
name|N_
argument_list|(
literal|"<type> can be one of: blob, tree, commit, tag"
argument_list|)
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|'t'
argument_list|,
name|NULL
argument_list|,
operator|&
name|opt
argument_list|,
name|N_
argument_list|(
literal|"show object type"
argument_list|)
argument_list|,
literal|'t'
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|'s'
argument_list|,
name|NULL
argument_list|,
operator|&
name|opt
argument_list|,
name|N_
argument_list|(
literal|"show object size"
argument_list|)
argument_list|,
literal|'s'
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|'e'
argument_list|,
name|NULL
argument_list|,
operator|&
name|opt
argument_list|,
name|N_
argument_list|(
literal|"exit with zero when there's no error"
argument_list|)
argument_list|,
literal|'e'
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|'p'
argument_list|,
name|NULL
argument_list|,
operator|&
name|opt
argument_list|,
name|N_
argument_list|(
literal|"pretty-print object's content"
argument_list|)
argument_list|,
literal|'p'
argument_list|)
block|,
name|OPT_CMDMODE
argument_list|(
literal|0
argument_list|,
literal|"textconv"
argument_list|,
operator|&
name|opt
argument_list|,
name|N_
argument_list|(
literal|"for blob objects, run textconv on object's content"
argument_list|)
argument_list|,
literal|'c'
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"allow-unknown-type"
argument_list|,
operator|&
name|unknown_type
argument_list|,
name|N_
argument_list|(
literal|"allow -s and -t to work with broken/corrupt objects"
argument_list|)
argument_list|)
block|,
block|{
name|OPTION_CALLBACK
block|,
literal|0
block|,
literal|"batch"
block|,
operator|&
name|batch
block|,
literal|"format"
block|,
name|N_
argument_list|(
literal|"show info and content of objects fed from the standard input"
argument_list|)
block|,
name|PARSE_OPT_OPTARG
block|,
name|batch_option_callback
block|}
block|,
block|{
name|OPTION_CALLBACK
block|,
literal|0
block|,
literal|"batch-check"
block|,
operator|&
name|batch
block|,
literal|"format"
block|,
name|N_
argument_list|(
literal|"show info about objects fed from the standard input"
argument_list|)
block|,
name|PARSE_OPT_OPTARG
block|,
name|batch_option_callback
block|}
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
operator|.
name|enabled
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
operator|.
name|enabled
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
operator|.
name|enabled
condition|)
return|return
name|batch_objects
argument_list|(
operator|&
name|batch
argument_list|)
return|;
if|if
condition|(
name|unknown_type
operator|&&
name|opt
operator|!=
literal|'t'
operator|&&
name|opt
operator|!=
literal|'s'
condition|)
name|die
argument_list|(
literal|"git cat-file --allow-unknown-type: use with -s or -t"
argument_list|)
expr_stmt|;
return|return
name|cat_one_file
argument_list|(
name|opt
argument_list|,
name|exp_type
argument_list|,
name|obj_name
argument_list|,
name|unknown_type
argument_list|)
return|;
block|}
end_function

end_unit

