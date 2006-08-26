begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"tree-walk.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_function
DECL|function|fill_tree_descriptor
name|void
modifier|*
name|fill_tree_descriptor
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|unsigned
name|long
name|size
init|=
literal|0
decl_stmt|;
name|void
modifier|*
name|buf
init|=
name|NULL
decl_stmt|;
if|if
condition|(
name|sha1
condition|)
block|{
name|buf
operator|=
name|read_object_with_reference
argument_list|(
name|sha1
argument_list|,
name|tree_type
argument_list|,
operator|&
name|size
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
name|die
argument_list|(
literal|"unable to read tree %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|desc
operator|->
name|size
operator|=
name|size
expr_stmt|;
name|desc
operator|->
name|buf
operator|=
name|buf
expr_stmt|;
return|return
name|buf
return|;
block|}
end_function

begin_function
DECL|function|entry_compare
specifier|static
name|int
name|entry_compare
parameter_list|(
name|struct
name|name_entry
modifier|*
name|a
parameter_list|,
name|struct
name|name_entry
modifier|*
name|b
parameter_list|)
block|{
return|return
name|base_name_compare
argument_list|(
name|a
operator|->
name|path
argument_list|,
name|a
operator|->
name|pathlen
argument_list|,
name|a
operator|->
name|mode
argument_list|,
name|b
operator|->
name|path
argument_list|,
name|b
operator|->
name|pathlen
argument_list|,
name|b
operator|->
name|mode
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|entry_clear
specifier|static
name|void
name|entry_clear
parameter_list|(
name|struct
name|name_entry
modifier|*
name|a
parameter_list|)
block|{
name|memset
argument_list|(
name|a
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|a
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|entry_extract
specifier|static
name|void
name|entry_extract
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|t
parameter_list|,
name|struct
name|name_entry
modifier|*
name|a
parameter_list|)
block|{
name|a
operator|->
name|sha1
operator|=
name|tree_entry_extract
argument_list|(
name|t
argument_list|,
operator|&
name|a
operator|->
name|path
argument_list|,
operator|&
name|a
operator|->
name|mode
argument_list|)
expr_stmt|;
name|a
operator|->
name|pathlen
operator|=
name|strlen
argument_list|(
name|a
operator|->
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|update_tree_entry
name|void
name|update_tree_entry
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|)
block|{
specifier|const
name|void
modifier|*
name|buf
init|=
name|desc
operator|->
name|buf
decl_stmt|;
name|unsigned
name|long
name|size
init|=
name|desc
operator|->
name|size
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|buf
argument_list|)
operator|+
literal|1
operator|+
literal|20
decl_stmt|;
if|if
condition|(
name|size
operator|<
name|len
condition|)
name|die
argument_list|(
literal|"corrupt tree file"
argument_list|)
expr_stmt|;
name|desc
operator|->
name|buf
operator|=
operator|(
name|char
operator|*
operator|)
name|buf
operator|+
name|len
expr_stmt|;
name|desc
operator|->
name|size
operator|=
name|size
operator|-
name|len
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_mode
specifier|static
specifier|const
name|char
modifier|*
name|get_mode
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
name|unsigned
name|int
modifier|*
name|modep
parameter_list|)
block|{
name|unsigned
name|char
name|c
decl_stmt|;
name|unsigned
name|int
name|mode
init|=
literal|0
decl_stmt|;
while|while
condition|(
operator|(
name|c
operator|=
operator|*
name|str
operator|++
operator|)
operator|!=
literal|' '
condition|)
block|{
if|if
condition|(
name|c
operator|<
literal|'0'
operator|||
name|c
operator|>
literal|'7'
condition|)
return|return
name|NULL
return|;
name|mode
operator|=
operator|(
name|mode
operator|<<
literal|3
operator|)
operator|+
operator|(
name|c
operator|-
literal|'0'
operator|)
expr_stmt|;
block|}
operator|*
name|modep
operator|=
name|mode
expr_stmt|;
return|return
name|str
return|;
block|}
end_function

begin_function
DECL|function|tree_entry_extract
specifier|const
name|unsigned
name|char
modifier|*
name|tree_entry_extract
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathp
parameter_list|,
name|unsigned
name|int
modifier|*
name|modep
parameter_list|)
block|{
specifier|const
name|void
modifier|*
name|tree
init|=
name|desc
operator|->
name|buf
decl_stmt|;
name|unsigned
name|long
name|size
init|=
name|desc
operator|->
name|size
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|tree
argument_list|)
operator|+
literal|1
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
init|=
operator|(
name|unsigned
name|char
operator|*
operator|)
name|tree
operator|+
name|len
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
name|unsigned
name|int
name|mode
decl_stmt|;
name|path
operator|=
name|get_mode
argument_list|(
name|tree
argument_list|,
operator|&
name|mode
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|path
operator|||
name|size
operator|<
name|len
operator|+
literal|20
condition|)
name|die
argument_list|(
literal|"corrupt tree file"
argument_list|)
expr_stmt|;
operator|*
name|pathp
operator|=
name|path
expr_stmt|;
operator|*
name|modep
operator|=
name|canon_mode
argument_list|(
name|mode
argument_list|)
expr_stmt|;
return|return
name|sha1
return|;
block|}
end_function

begin_function
DECL|function|tree_entry
name|int
name|tree_entry
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|,
name|struct
name|name_entry
modifier|*
name|entry
parameter_list|)
block|{
specifier|const
name|void
modifier|*
name|tree
init|=
name|desc
operator|->
name|buf
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
name|unsigned
name|long
name|len
decl_stmt|,
name|size
init|=
name|desc
operator|->
name|size
decl_stmt|;
if|if
condition|(
operator|!
name|size
condition|)
return|return
literal|0
return|;
name|path
operator|=
name|get_mode
argument_list|(
name|tree
argument_list|,
operator|&
name|entry
operator|->
name|mode
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|path
condition|)
name|die
argument_list|(
literal|"corrupt tree file"
argument_list|)
expr_stmt|;
name|entry
operator|->
name|path
operator|=
name|path
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|entry
operator|->
name|pathlen
operator|=
name|len
expr_stmt|;
name|path
operator|+=
name|len
operator|+
literal|1
expr_stmt|;
name|entry
operator|->
name|sha1
operator|=
operator|(
specifier|const
name|unsigned
name|char
operator|*
operator|)
name|path
expr_stmt|;
name|path
operator|+=
literal|20
expr_stmt|;
name|len
operator|=
name|path
operator|-
operator|(
name|char
operator|*
operator|)
name|tree
expr_stmt|;
if|if
condition|(
name|len
operator|>
name|size
condition|)
name|die
argument_list|(
literal|"corrupt tree file"
argument_list|)
expr_stmt|;
name|desc
operator|->
name|buf
operator|=
name|path
expr_stmt|;
name|desc
operator|->
name|size
operator|=
name|size
operator|-
name|len
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|traverse_trees
name|void
name|traverse_trees
parameter_list|(
name|int
name|n
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|traverse_callback_t
name|callback
parameter_list|)
block|{
name|struct
name|name_entry
modifier|*
name|entry
init|=
name|xmalloc
argument_list|(
name|n
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|entry
argument_list|)
argument_list|)
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|struct
name|name_entry
name|entry
index|[
literal|3
index|]
decl_stmt|;
name|unsigned
name|long
name|mask
init|=
literal|0
decl_stmt|;
name|int
name|i
decl_stmt|,
name|last
decl_stmt|;
name|last
operator|=
operator|-
literal|1
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|n
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|t
index|[
name|i
index|]
operator|.
name|size
condition|)
continue|continue;
name|entry_extract
argument_list|(
name|t
operator|+
name|i
argument_list|,
name|entry
operator|+
name|i
argument_list|)
expr_stmt|;
if|if
condition|(
name|last
operator|>=
literal|0
condition|)
block|{
name|int
name|cmp
init|=
name|entry_compare
argument_list|(
name|entry
operator|+
name|i
argument_list|,
name|entry
operator|+
name|last
argument_list|)
decl_stmt|;
comment|/* 				 * Is the new name bigger than the old one? 				 * Ignore it 				 */
if|if
condition|(
name|cmp
operator|>
literal|0
condition|)
continue|continue;
comment|/* 				 * Is the new name smaller than the old one? 				 * Ignore all old ones 				 */
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
name|mask
operator|=
literal|0
expr_stmt|;
block|}
name|mask
operator||=
literal|1ul
operator|<<
name|i
expr_stmt|;
name|last
operator|=
name|i
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|mask
condition|)
break|break;
comment|/* 		 * Update the tree entries we've walked, and clear 		 * all the unused name-entries. 		 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|n
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|mask
operator|&
operator|(
literal|1ul
operator|<<
name|i
operator|)
condition|)
block|{
name|update_tree_entry
argument_list|(
name|t
operator|+
name|i
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|entry_clear
argument_list|(
name|entry
operator|+
name|i
argument_list|)
expr_stmt|;
block|}
name|callback
argument_list|(
name|n
argument_list|,
name|mask
argument_list|,
name|entry
argument_list|,
name|base
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|entry
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|find_tree_entry
specifier|static
name|int
name|find_tree_entry
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|t
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|char
modifier|*
name|result
parameter_list|,
name|unsigned
modifier|*
name|mode
parameter_list|)
block|{
name|int
name|namelen
init|=
name|strlen
argument_list|(
name|name
argument_list|)
decl_stmt|;
while|while
condition|(
name|t
operator|->
name|size
condition|)
block|{
specifier|const
name|char
modifier|*
name|entry
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|int
name|entrylen
decl_stmt|,
name|cmp
decl_stmt|;
name|sha1
operator|=
name|tree_entry_extract
argument_list|(
name|t
argument_list|,
operator|&
name|entry
argument_list|,
name|mode
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
name|t
argument_list|)
expr_stmt|;
name|entrylen
operator|=
name|strlen
argument_list|(
name|entry
argument_list|)
expr_stmt|;
if|if
condition|(
name|entrylen
operator|>
name|namelen
condition|)
continue|continue;
name|cmp
operator|=
name|memcmp
argument_list|(
name|name
argument_list|,
name|entry
argument_list|,
name|entrylen
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmp
operator|>
literal|0
condition|)
continue|continue;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
break|break;
if|if
condition|(
name|entrylen
operator|==
name|namelen
condition|)
block|{
name|hashcpy
argument_list|(
name|result
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|name
index|[
name|entrylen
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
operator|*
name|mode
argument_list|)
condition|)
break|break;
if|if
condition|(
operator|++
name|entrylen
operator|==
name|namelen
condition|)
block|{
name|hashcpy
argument_list|(
name|result
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
return|return
name|get_tree_entry
argument_list|(
name|sha1
argument_list|,
name|name
operator|+
name|entrylen
argument_list|,
name|result
argument_list|,
name|mode
argument_list|)
return|;
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|get_tree_entry
name|int
name|get_tree_entry
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|tree_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
modifier|*
name|mode
parameter_list|)
block|{
name|int
name|retval
decl_stmt|;
name|void
modifier|*
name|tree
decl_stmt|;
name|struct
name|tree_desc
name|t
decl_stmt|;
name|tree
operator|=
name|read_object_with_reference
argument_list|(
name|tree_sha1
argument_list|,
name|tree_type
argument_list|,
operator|&
name|t
operator|.
name|size
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree
condition|)
return|return
operator|-
literal|1
return|;
name|t
operator|.
name|buf
operator|=
name|tree
expr_stmt|;
name|retval
operator|=
name|find_tree_entry
argument_list|(
operator|&
name|t
argument_list|,
name|name
argument_list|,
name|sha1
argument_list|,
name|mode
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

end_unit

