begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"blob.h"
end_include

begin_decl_stmt
DECL|variable|tag_type
specifier|const
name|char
modifier|*
name|tag_type
init|=
literal|"tag"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|deref_tag
name|struct
name|object
modifier|*
name|deref_tag
parameter_list|(
name|struct
name|object
modifier|*
name|o
parameter_list|,
specifier|const
name|char
modifier|*
name|warn
parameter_list|,
name|int
name|warnlen
parameter_list|)
block|{
while|while
condition|(
name|o
operator|&&
name|o
operator|->
name|type
operator|==
name|OBJ_TAG
condition|)
if|if
condition|(
operator|(
operator|(
expr|struct
name|tag
operator|*
operator|)
name|o
operator|)
operator|->
name|tagged
condition|)
name|o
operator|=
name|parse_object
argument_list|(
operator|(
operator|(
expr|struct
name|tag
operator|*
operator|)
name|o
operator|)
operator|->
name|tagged
operator|->
name|sha1
argument_list|)
expr_stmt|;
else|else
name|o
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
operator|!
name|o
operator|&&
name|warn
condition|)
block|{
if|if
condition|(
operator|!
name|warnlen
condition|)
name|warnlen
operator|=
name|strlen
argument_list|(
name|warn
argument_list|)
expr_stmt|;
name|error
argument_list|(
literal|"missing object referenced by '%.*s'"
argument_list|,
name|warnlen
argument_list|,
name|warn
argument_list|)
expr_stmt|;
block|}
return|return
name|o
return|;
block|}
end_function

begin_function
DECL|function|lookup_tag
name|struct
name|tag
modifier|*
name|lookup_tag
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
init|=
name|lookup_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|obj
condition|)
return|return
name|create_object
argument_list|(
name|sha1
argument_list|,
name|OBJ_TAG
argument_list|,
name|alloc_tag_node
argument_list|()
argument_list|)
return|;
if|if
condition|(
operator|!
name|obj
operator|->
name|type
condition|)
name|obj
operator|->
name|type
operator|=
name|OBJ_TAG
expr_stmt|;
if|if
condition|(
name|obj
operator|->
name|type
operator|!=
name|OBJ_TAG
condition|)
block|{
name|error
argument_list|(
literal|"Object %s is a %s, not a tag"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|typename
argument_list|(
name|obj
operator|->
name|type
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
return|return
operator|(
expr|struct
name|tag
operator|*
operator|)
name|obj
return|;
block|}
end_function

begin_function
DECL|function|parse_tag_buffer
name|int
name|parse_tag_buffer
parameter_list|(
name|struct
name|tag
modifier|*
name|item
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
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
specifier|const
name|char
modifier|*
name|bufptr
init|=
name|data
decl_stmt|;
specifier|const
name|char
modifier|*
name|tail
init|=
name|bufptr
operator|+
name|size
decl_stmt|;
specifier|const
name|char
modifier|*
name|nl
decl_stmt|;
if|if
condition|(
name|item
operator|->
name|object
operator|.
name|parsed
condition|)
return|return
literal|0
return|;
name|item
operator|->
name|object
operator|.
name|parsed
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|size
operator|<
literal|64
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|memcmp
argument_list|(
literal|"object "
argument_list|,
name|bufptr
argument_list|,
literal|7
argument_list|)
operator|||
name|get_sha1_hex
argument_list|(
name|bufptr
operator|+
literal|7
argument_list|,
name|sha1
argument_list|)
operator|||
name|bufptr
index|[
literal|47
index|]
operator|!=
literal|'\n'
condition|)
return|return
operator|-
literal|1
return|;
name|bufptr
operator|+=
literal|48
expr_stmt|;
comment|/* "object " + sha1 + "\n" */
if|if
condition|(
name|prefixcmp
argument_list|(
name|bufptr
argument_list|,
literal|"type "
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|bufptr
operator|+=
literal|5
expr_stmt|;
name|nl
operator|=
name|memchr
argument_list|(
name|bufptr
argument_list|,
literal|'\n'
argument_list|,
name|tail
operator|-
name|bufptr
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|nl
operator|||
sizeof|sizeof
argument_list|(
name|type
argument_list|)
operator|<=
operator|(
name|nl
operator|-
name|bufptr
operator|)
condition|)
return|return
operator|-
literal|1
return|;
name|strncpy
argument_list|(
name|type
argument_list|,
name|bufptr
argument_list|,
name|nl
operator|-
name|bufptr
argument_list|)
expr_stmt|;
name|type
index|[
name|nl
operator|-
name|bufptr
index|]
operator|=
literal|'\0'
expr_stmt|;
name|bufptr
operator|=
name|nl
operator|+
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
name|blob_type
argument_list|)
condition|)
block|{
name|item
operator|->
name|tagged
operator|=
operator|&
name|lookup_blob
argument_list|(
name|sha1
argument_list|)
operator|->
name|object
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
name|tree_type
argument_list|)
condition|)
block|{
name|item
operator|->
name|tagged
operator|=
operator|&
name|lookup_tree
argument_list|(
name|sha1
argument_list|)
operator|->
name|object
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
name|commit_type
argument_list|)
condition|)
block|{
name|item
operator|->
name|tagged
operator|=
operator|&
name|lookup_commit
argument_list|(
name|sha1
argument_list|)
operator|->
name|object
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
name|tag_type
argument_list|)
condition|)
block|{
name|item
operator|->
name|tagged
operator|=
operator|&
name|lookup_tag
argument_list|(
name|sha1
argument_list|)
operator|->
name|object
expr_stmt|;
block|}
else|else
block|{
name|error
argument_list|(
literal|"Unknown type %s"
argument_list|,
name|type
argument_list|)
expr_stmt|;
name|item
operator|->
name|tagged
operator|=
name|NULL
expr_stmt|;
block|}
if|if
condition|(
name|prefixcmp
argument_list|(
name|bufptr
argument_list|,
literal|"tag "
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|bufptr
operator|+=
literal|4
expr_stmt|;
name|nl
operator|=
name|memchr
argument_list|(
name|bufptr
argument_list|,
literal|'\n'
argument_list|,
name|tail
operator|-
name|bufptr
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|nl
condition|)
return|return
operator|-
literal|1
return|;
name|item
operator|->
name|tag
operator|=
name|xmemdupz
argument_list|(
name|bufptr
argument_list|,
name|nl
operator|-
name|bufptr
argument_list|)
expr_stmt|;
name|bufptr
operator|=
name|nl
operator|+
literal|1
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|parse_tag
name|int
name|parse_tag
parameter_list|(
name|struct
name|tag
modifier|*
name|item
parameter_list|)
block|{
name|enum
name|object_type
name|type
decl_stmt|;
name|void
modifier|*
name|data
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|int
name|ret
decl_stmt|;
if|if
condition|(
name|item
operator|->
name|object
operator|.
name|parsed
condition|)
return|return
literal|0
return|;
name|data
operator|=
name|read_sha1_file
argument_list|(
name|item
operator|->
name|object
operator|.
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
name|data
condition|)
return|return
name|error
argument_list|(
literal|"Could not read %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
return|;
if|if
condition|(
name|type
operator|!=
name|OBJ_TAG
condition|)
block|{
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"Object %s not a tag"
argument_list|,
name|sha1_to_hex
argument_list|(
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
return|;
block|}
name|ret
operator|=
name|parse_tag_buffer
argument_list|(
name|item
argument_list|,
name|data
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

end_unit

