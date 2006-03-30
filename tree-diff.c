begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Helper functions for tree diff generation  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_comment
comment|// What paths are we interested in?
end_comment

begin_decl_stmt
DECL|variable|nr_paths
specifier|static
name|int
name|nr_paths
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|paths
specifier|static
specifier|const
name|char
modifier|*
modifier|*
name|paths
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pathlens
specifier|static
name|int
modifier|*
name|pathlens
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|malloc_base
specifier|static
name|char
modifier|*
name|malloc_base
parameter_list|(
specifier|const
name|char
modifier|*
name|base
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|pathlen
parameter_list|)
block|{
name|int
name|baselen
init|=
name|strlen
argument_list|(
name|base
argument_list|)
decl_stmt|;
name|char
modifier|*
name|newbase
init|=
name|xmalloc
argument_list|(
name|baselen
operator|+
name|pathlen
operator|+
literal|2
argument_list|)
decl_stmt|;
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
name|path
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|newbase
operator|+
name|baselen
operator|+
name|pathlen
argument_list|,
literal|"/"
argument_list|,
literal|2
argument_list|)
expr_stmt|;
return|return
name|newbase
return|;
block|}
end_function

begin_function_decl
specifier|static
name|int
name|show_entry
parameter_list|(
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|compare_tree_entry
specifier|static
name|int
name|compare_tree_entry
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|t1
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t2
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|)
block|{
name|unsigned
name|mode1
decl_stmt|,
name|mode2
decl_stmt|;
specifier|const
name|char
modifier|*
name|path1
decl_stmt|,
modifier|*
name|path2
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|,
modifier|*
name|sha2
decl_stmt|;
name|int
name|cmp
decl_stmt|,
name|pathlen1
decl_stmt|,
name|pathlen2
decl_stmt|;
name|sha1
operator|=
name|tree_entry_extract
argument_list|(
name|t1
argument_list|,
operator|&
name|path1
argument_list|,
operator|&
name|mode1
argument_list|)
expr_stmt|;
name|sha2
operator|=
name|tree_entry_extract
argument_list|(
name|t2
argument_list|,
operator|&
name|path2
argument_list|,
operator|&
name|mode2
argument_list|)
expr_stmt|;
name|pathlen1
operator|=
name|strlen
argument_list|(
name|path1
argument_list|)
expr_stmt|;
name|pathlen2
operator|=
name|strlen
argument_list|(
name|path2
argument_list|)
expr_stmt|;
name|cmp
operator|=
name|base_name_compare
argument_list|(
name|path1
argument_list|,
name|pathlen1
argument_list|,
name|mode1
argument_list|,
name|path2
argument_list|,
name|pathlen2
argument_list|,
name|mode2
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
block|{
name|show_entry
argument_list|(
name|opt
argument_list|,
literal|"-"
argument_list|,
name|t1
argument_list|,
name|base
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|cmp
operator|>
literal|0
condition|)
block|{
name|show_entry
argument_list|(
name|opt
argument_list|,
literal|"+"
argument_list|,
name|t2
argument_list|,
name|base
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
if|if
condition|(
operator|!
name|opt
operator|->
name|find_copies_harder
operator|&&
operator|!
name|memcmp
argument_list|(
name|sha1
argument_list|,
name|sha2
argument_list|,
literal|20
argument_list|)
operator|&&
name|mode1
operator|==
name|mode2
condition|)
return|return
literal|0
return|;
comment|/* 	 * If the filemode has changed to/from a directory from/to a regular 	 * file, we need to consider it a remove and an add. 	 */
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode1
argument_list|)
operator|!=
name|S_ISDIR
argument_list|(
name|mode2
argument_list|)
condition|)
block|{
name|show_entry
argument_list|(
name|opt
argument_list|,
literal|"-"
argument_list|,
name|t1
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|show_entry
argument_list|(
name|opt
argument_list|,
literal|"+"
argument_list|,
name|t2
argument_list|,
name|base
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|opt
operator|->
name|recursive
operator|&&
name|S_ISDIR
argument_list|(
name|mode1
argument_list|)
condition|)
block|{
name|int
name|retval
decl_stmt|;
name|char
modifier|*
name|newbase
init|=
name|malloc_base
argument_list|(
name|base
argument_list|,
name|path1
argument_list|,
name|pathlen1
argument_list|)
decl_stmt|;
if|if
condition|(
name|opt
operator|->
name|tree_in_recursive
condition|)
name|opt
operator|->
name|change
argument_list|(
name|opt
argument_list|,
name|mode1
argument_list|,
name|mode2
argument_list|,
name|sha1
argument_list|,
name|sha2
argument_list|,
name|base
argument_list|,
name|path1
argument_list|)
expr_stmt|;
name|retval
operator|=
name|diff_tree_sha1
argument_list|(
name|sha1
argument_list|,
name|sha2
argument_list|,
name|newbase
argument_list|,
name|opt
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|newbase
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
name|opt
operator|->
name|change
argument_list|(
name|opt
argument_list|,
name|mode1
argument_list|,
name|mode2
argument_list|,
name|sha1
argument_list|,
name|sha2
argument_list|,
name|base
argument_list|,
name|path1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|interesting
specifier|static
name|int
name|interesting
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
name|unsigned
name|mode
decl_stmt|;
name|int
name|i
decl_stmt|;
name|int
name|baselen
decl_stmt|,
name|pathlen
decl_stmt|;
if|if
condition|(
operator|!
name|nr_paths
condition|)
return|return
literal|1
return|;
operator|(
name|void
operator|)
name|tree_entry_extract
argument_list|(
name|desc
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|mode
argument_list|)
expr_stmt|;
name|pathlen
operator|=
name|strlen
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|baselen
operator|=
name|strlen
argument_list|(
name|base
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_paths
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|match
init|=
name|paths
index|[
name|i
index|]
decl_stmt|;
name|int
name|matchlen
init|=
name|pathlens
index|[
name|i
index|]
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
comment|/* No matches */
block|}
end_function

begin_comment
comment|/* A whole sub-tree went away or appeared */
end_comment

begin_function
DECL|function|show_tree
specifier|static
name|void
name|show_tree
parameter_list|(
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
while|while
condition|(
name|desc
operator|->
name|size
condition|)
block|{
if|if
condition|(
name|interesting
argument_list|(
name|desc
argument_list|,
name|base
argument_list|)
condition|)
name|show_entry
argument_list|(
name|opt
argument_list|,
name|prefix
argument_list|,
name|desc
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
name|desc
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/* A file entry went away or appeared */
end_comment

begin_function
DECL|function|show_entry
specifier|static
name|int
name|show_entry
parameter_list|(
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
name|unsigned
name|mode
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|tree_entry_extract
argument_list|(
name|desc
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|mode
argument_list|)
decl_stmt|;
if|if
condition|(
name|opt
operator|->
name|recursive
operator|&&
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
block|{
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|newbase
init|=
name|malloc_base
argument_list|(
name|base
argument_list|,
name|path
argument_list|,
name|strlen
argument_list|(
name|path
argument_list|)
argument_list|)
decl_stmt|;
name|struct
name|tree_desc
name|inner
decl_stmt|;
name|void
modifier|*
name|tree
decl_stmt|;
name|tree
operator|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|inner
operator|.
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree
operator|||
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tree"
argument_list|)
condition|)
name|die
argument_list|(
literal|"corrupt tree sha %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|inner
operator|.
name|buf
operator|=
name|tree
expr_stmt|;
name|show_tree
argument_list|(
name|opt
argument_list|,
name|prefix
argument_list|,
operator|&
name|inner
argument_list|,
name|newbase
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|newbase
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|opt
operator|->
name|add_remove
argument_list|(
name|opt
argument_list|,
name|prefix
index|[
literal|0
index|]
argument_list|,
name|mode
argument_list|,
name|sha1
argument_list|,
name|base
argument_list|,
name|path
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|diff_tree
name|int
name|diff_tree
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|t1
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t2
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|)
block|{
while|while
condition|(
name|t1
operator|->
name|size
operator||
name|t2
operator|->
name|size
condition|)
block|{
if|if
condition|(
name|nr_paths
operator|&&
name|t1
operator|->
name|size
operator|&&
operator|!
name|interesting
argument_list|(
name|t1
argument_list|,
name|base
argument_list|)
condition|)
block|{
name|update_tree_entry
argument_list|(
name|t1
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|nr_paths
operator|&&
name|t2
operator|->
name|size
operator|&&
operator|!
name|interesting
argument_list|(
name|t2
argument_list|,
name|base
argument_list|)
condition|)
block|{
name|update_tree_entry
argument_list|(
name|t2
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|t1
operator|->
name|size
condition|)
block|{
name|show_entry
argument_list|(
name|opt
argument_list|,
literal|"+"
argument_list|,
name|t2
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
name|t2
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|t2
operator|->
name|size
condition|)
block|{
name|show_entry
argument_list|(
name|opt
argument_list|,
literal|"-"
argument_list|,
name|t1
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
name|t1
argument_list|)
expr_stmt|;
continue|continue;
block|}
switch|switch
condition|(
name|compare_tree_entry
argument_list|(
name|t1
argument_list|,
name|t2
argument_list|,
name|base
argument_list|,
name|opt
argument_list|)
condition|)
block|{
case|case
operator|-
literal|1
case|:
name|update_tree_entry
argument_list|(
name|t1
argument_list|)
expr_stmt|;
continue|continue;
case|case
literal|0
case|:
name|update_tree_entry
argument_list|(
name|t1
argument_list|)
expr_stmt|;
comment|/* Fallthrough */
case|case
literal|1
case|:
name|update_tree_entry
argument_list|(
name|t2
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|die
argument_list|(
literal|"git-diff-tree: internal error"
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|diff_tree_sha1
name|int
name|diff_tree_sha1
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|old
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|)
block|{
name|void
modifier|*
name|tree1
decl_stmt|,
modifier|*
name|tree2
decl_stmt|;
name|struct
name|tree_desc
name|t1
decl_stmt|,
name|t2
decl_stmt|;
name|int
name|retval
decl_stmt|;
name|tree1
operator|=
name|read_object_with_reference
argument_list|(
name|old
argument_list|,
literal|"tree"
argument_list|,
operator|&
name|t1
operator|.
name|size
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree1
condition|)
name|die
argument_list|(
literal|"unable to read source tree (%s)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|old
argument_list|)
argument_list|)
expr_stmt|;
name|tree2
operator|=
name|read_object_with_reference
argument_list|(
name|new
argument_list|,
literal|"tree"
argument_list|,
operator|&
name|t2
operator|.
name|size
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree2
condition|)
name|die
argument_list|(
literal|"unable to read destination tree (%s)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|new
argument_list|)
argument_list|)
expr_stmt|;
name|t1
operator|.
name|buf
operator|=
name|tree1
expr_stmt|;
name|t2
operator|.
name|buf
operator|=
name|tree2
expr_stmt|;
name|retval
operator|=
name|diff_tree
argument_list|(
operator|&
name|t1
argument_list|,
operator|&
name|t2
argument_list|,
name|base
argument_list|,
name|opt
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree2
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|count_paths
specifier|static
name|int
name|count_paths
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|paths
parameter_list|)
block|{
name|int
name|i
init|=
literal|0
decl_stmt|;
while|while
condition|(
operator|*
name|paths
operator|++
condition|)
name|i
operator|++
expr_stmt|;
return|return
name|i
return|;
block|}
end_function

begin_function
DECL|function|diff_tree_setup_paths
name|void
name|diff_tree_setup_paths
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|p
parameter_list|)
block|{
if|if
condition|(
name|p
condition|)
block|{
name|int
name|i
decl_stmt|;
name|paths
operator|=
name|p
expr_stmt|;
name|nr_paths
operator|=
name|count_paths
argument_list|(
name|paths
argument_list|)
expr_stmt|;
if|if
condition|(
name|nr_paths
operator|==
literal|0
condition|)
block|{
name|pathlens
operator|=
name|NULL
expr_stmt|;
return|return;
block|}
name|pathlens
operator|=
name|xmalloc
argument_list|(
name|nr_paths
operator|*
sizeof|sizeof
argument_list|(
name|int
argument_list|)
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_paths
condition|;
name|i
operator|++
control|)
name|pathlens
index|[
name|i
index|]
operator|=
name|strlen
argument_list|(
name|paths
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit

