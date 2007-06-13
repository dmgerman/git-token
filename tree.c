begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
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

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"tree-walk.h"
end_include

begin_decl_stmt
DECL|variable|tree_type
specifier|const
name|char
modifier|*
name|tree_type
init|=
literal|"tree"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|read_one_entry
specifier|static
name|int
name|read_one_entry
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
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
specifier|const
name|char
modifier|*
name|pathname
parameter_list|,
name|unsigned
name|mode
parameter_list|,
name|int
name|stage
parameter_list|)
block|{
name|int
name|len
decl_stmt|;
name|unsigned
name|int
name|size
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
return|return
name|READ_TREE_RECURSIVE
return|;
name|len
operator|=
name|strlen
argument_list|(
name|pathname
argument_list|)
expr_stmt|;
name|size
operator|=
name|cache_entry_size
argument_list|(
name|baselen
operator|+
name|len
argument_list|)
expr_stmt|;
name|ce
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|ce
operator|->
name|ce_mode
operator|=
name|create_ce_mode
argument_list|(
name|mode
argument_list|)
expr_stmt|;
name|ce
operator|->
name|ce_flags
operator|=
name|create_ce_flags
argument_list|(
name|baselen
operator|+
name|len
argument_list|,
name|stage
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|ce
operator|->
name|name
operator|+
name|baselen
argument_list|,
name|pathname
argument_list|,
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|ce
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
return|return
name|add_cache_entry
argument_list|(
name|ce
argument_list|,
name|ADD_CACHE_OK_TO_ADD
operator||
name|ADD_CACHE_SKIP_DFCHECK
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|match_tree_entry
specifier|static
name|int
name|match_tree_entry
parameter_list|(
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|int
name|mode
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|paths
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|match
decl_stmt|;
name|int
name|pathlen
decl_stmt|;
if|if
condition|(
operator|!
name|paths
condition|)
return|return
literal|1
return|;
name|pathlen
operator|=
name|strlen
argument_list|(
name|path
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|match
operator|=
operator|*
name|paths
operator|++
operator|)
operator|!=
name|NULL
condition|)
block|{
name|int
name|matchlen
init|=
name|strlen
argument_list|(
name|match
argument_list|)
decl_stmt|;
if|if
condition|(
name|baselen
operator|>=
name|matchlen
condition|)
block|{
comment|/* If it doesn't match, move along... */
if|if
condition|(
name|strncmp
argument_list|(
name|base
argument_list|,
name|match
argument_list|,
name|matchlen
argument_list|)
condition|)
continue|continue;
comment|/* The base is a subdirectory of a path which was specified. */
return|return
literal|1
return|;
block|}
comment|/* Does the base match? */
if|if
condition|(
name|strncmp
argument_list|(
name|base
argument_list|,
name|match
argument_list|,
name|baselen
argument_list|)
condition|)
continue|continue;
name|match
operator|+=
name|baselen
expr_stmt|;
name|matchlen
operator|-=
name|baselen
expr_stmt|;
if|if
condition|(
name|pathlen
operator|>
name|matchlen
condition|)
continue|continue;
if|if
condition|(
name|matchlen
operator|>
name|pathlen
condition|)
block|{
if|if
condition|(
name|match
index|[
name|pathlen
index|]
operator|!=
literal|'/'
condition|)
continue|continue;
if|if
condition|(
operator|!
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
continue|continue;
block|}
if|if
condition|(
name|strncmp
argument_list|(
name|path
argument_list|,
name|match
argument_list|,
name|pathlen
argument_list|)
condition|)
continue|continue;
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|read_tree_recursive
name|int
name|read_tree_recursive
parameter_list|(
name|struct
name|tree
modifier|*
name|tree
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
name|int
name|stage
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|match
parameter_list|,
name|read_tree_fn_t
name|fn
parameter_list|)
block|{
name|struct
name|tree_desc
name|desc
decl_stmt|;
name|struct
name|name_entry
name|entry
decl_stmt|;
if|if
condition|(
name|parse_tree
argument_list|(
name|tree
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|init_tree_desc
argument_list|(
operator|&
name|desc
argument_list|,
name|tree
operator|->
name|buffer
argument_list|,
name|tree
operator|->
name|size
argument_list|)
expr_stmt|;
while|while
condition|(
name|tree_entry
argument_list|(
operator|&
name|desc
argument_list|,
operator|&
name|entry
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|match_tree_entry
argument_list|(
name|base
argument_list|,
name|baselen
argument_list|,
name|entry
operator|.
name|path
argument_list|,
name|entry
operator|.
name|mode
argument_list|,
name|match
argument_list|)
condition|)
continue|continue;
switch|switch
condition|(
name|fn
argument_list|(
name|entry
operator|.
name|sha1
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|,
name|entry
operator|.
name|path
argument_list|,
name|entry
operator|.
name|mode
argument_list|,
name|stage
argument_list|)
condition|)
block|{
case|case
literal|0
case|:
continue|continue;
case|case
name|READ_TREE_RECURSIVE
case|:
break|break;
empty_stmt|;
default|default:
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|S_ISDIR
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
condition|)
block|{
name|int
name|retval
decl_stmt|;
name|char
modifier|*
name|newbase
decl_stmt|;
name|unsigned
name|int
name|pathlen
init|=
name|tree_entry_len
argument_list|(
name|entry
operator|.
name|path
argument_list|,
name|entry
operator|.
name|sha1
argument_list|)
decl_stmt|;
name|newbase
operator|=
name|xmalloc
argument_list|(
name|baselen
operator|+
literal|1
operator|+
name|pathlen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|newbase
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|newbase
operator|+
name|baselen
argument_list|,
name|entry
operator|.
name|path
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
name|newbase
index|[
name|baselen
operator|+
name|pathlen
index|]
operator|=
literal|'/'
expr_stmt|;
name|retval
operator|=
name|read_tree_recursive
argument_list|(
name|lookup_tree
argument_list|(
name|entry
operator|.
name|sha1
argument_list|)
argument_list|,
name|newbase
argument_list|,
name|baselen
operator|+
name|pathlen
operator|+
literal|1
argument_list|,
name|stage
argument_list|,
name|match
argument_list|,
name|fn
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|newbase
argument_list|)
expr_stmt|;
if|if
condition|(
name|retval
condition|)
return|return
operator|-
literal|1
return|;
continue|continue;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|read_tree
name|int
name|read_tree
parameter_list|(
name|struct
name|tree
modifier|*
name|tree
parameter_list|,
name|int
name|stage
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|match
parameter_list|)
block|{
return|return
name|read_tree_recursive
argument_list|(
name|tree
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|,
name|stage
argument_list|,
name|match
argument_list|,
name|read_one_entry
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|lookup_tree
name|struct
name|tree
modifier|*
name|lookup_tree
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
name|OBJ_TREE
argument_list|,
name|alloc_tree_node
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
name|OBJ_TREE
expr_stmt|;
if|if
condition|(
name|obj
operator|->
name|type
operator|!=
name|OBJ_TREE
condition|)
block|{
name|error
argument_list|(
literal|"Object %s is a %s, not a tree"
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
name|tree
operator|*
operator|)
name|obj
return|;
block|}
end_function

begin_comment
comment|/*  * NOTE! Tree refs to external git repositories  * (ie gitlinks) do not count as real references.  *  * You don't have to have those repositories  * available at all, much less have the objects  * accessible from the current repository.  */
end_comment

begin_function
DECL|function|track_tree_refs
specifier|static
name|void
name|track_tree_refs
parameter_list|(
name|struct
name|tree
modifier|*
name|item
parameter_list|)
block|{
name|int
name|n_refs
init|=
literal|0
decl_stmt|,
name|i
decl_stmt|;
name|struct
name|object_refs
modifier|*
name|refs
decl_stmt|;
name|struct
name|tree_desc
name|desc
decl_stmt|;
name|struct
name|name_entry
name|entry
decl_stmt|;
comment|/* Count how many entries there are.. */
name|init_tree_desc
argument_list|(
operator|&
name|desc
argument_list|,
name|item
operator|->
name|buffer
argument_list|,
name|item
operator|->
name|size
argument_list|)
expr_stmt|;
while|while
condition|(
name|tree_entry
argument_list|(
operator|&
name|desc
argument_list|,
operator|&
name|entry
argument_list|)
condition|)
block|{
if|if
condition|(
name|S_ISDIRLNK
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
condition|)
continue|continue;
name|n_refs
operator|++
expr_stmt|;
block|}
comment|/* Allocate object refs and walk it again.. */
name|i
operator|=
literal|0
expr_stmt|;
name|refs
operator|=
name|alloc_object_refs
argument_list|(
name|n_refs
argument_list|)
expr_stmt|;
name|init_tree_desc
argument_list|(
operator|&
name|desc
argument_list|,
name|item
operator|->
name|buffer
argument_list|,
name|item
operator|->
name|size
argument_list|)
expr_stmt|;
while|while
condition|(
name|tree_entry
argument_list|(
operator|&
name|desc
argument_list|,
operator|&
name|entry
argument_list|)
condition|)
block|{
name|struct
name|object
modifier|*
name|obj
decl_stmt|;
if|if
condition|(
name|S_ISDIRLNK
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
condition|)
name|obj
operator|=
operator|&
name|lookup_tree
argument_list|(
name|entry
operator|.
name|sha1
argument_list|)
operator|->
name|object
expr_stmt|;
elseif|else
if|if
condition|(
name|S_ISREG
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
operator|||
name|S_ISLNK
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
condition|)
name|obj
operator|=
operator|&
name|lookup_blob
argument_list|(
name|entry
operator|.
name|sha1
argument_list|)
operator|->
name|object
expr_stmt|;
else|else
block|{
name|warning
argument_list|(
literal|"in tree %s: entry %s has bad mode %.6o\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|,
name|entry
operator|.
name|path
argument_list|,
name|entry
operator|.
name|mode
argument_list|)
expr_stmt|;
name|obj
operator|=
name|lookup_unknown_object
argument_list|(
name|entry
operator|.
name|sha1
argument_list|)
expr_stmt|;
block|}
name|refs
operator|->
name|ref
index|[
name|i
operator|++
index|]
operator|=
name|obj
expr_stmt|;
block|}
name|set_object_refs
argument_list|(
operator|&
name|item
operator|->
name|object
argument_list|,
name|refs
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|parse_tree_buffer
name|int
name|parse_tree_buffer
parameter_list|(
name|struct
name|tree
modifier|*
name|item
parameter_list|,
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
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
name|item
operator|->
name|buffer
operator|=
name|buffer
expr_stmt|;
name|item
operator|->
name|size
operator|=
name|size
expr_stmt|;
if|if
condition|(
name|track_object_refs
condition|)
name|track_tree_refs
argument_list|(
name|item
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|parse_tree
name|int
name|parse_tree
parameter_list|(
name|struct
name|tree
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
name|buffer
decl_stmt|;
name|unsigned
name|long
name|size
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
name|buffer
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
name|buffer
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
name|OBJ_TREE
condition|)
block|{
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"Object %s not a tree"
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
return|return
name|parse_tree_buffer
argument_list|(
name|item
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|parse_tree_indirect
name|struct
name|tree
modifier|*
name|parse_tree_indirect
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
name|parse_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
do|do
block|{
if|if
condition|(
operator|!
name|obj
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_TREE
condition|)
return|return
operator|(
expr|struct
name|tree
operator|*
operator|)
name|obj
return|;
elseif|else
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_COMMIT
condition|)
name|obj
operator|=
operator|&
operator|(
operator|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|obj
operator|)
operator|->
name|tree
operator|->
name|object
operator|)
expr_stmt|;
elseif|else
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_TAG
condition|)
name|obj
operator|=
operator|(
operator|(
expr|struct
name|tag
operator|*
operator|)
name|obj
operator|)
operator|->
name|tagged
expr_stmt|;
else|else
return|return
name|NULL
return|;
if|if
condition|(
operator|!
name|obj
operator|->
name|parsed
condition|)
name|parse_object
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
expr_stmt|;
block|}
do|while
condition|(
literal|1
condition|)
do|;
block|}
end_function

end_unit

