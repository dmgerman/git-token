begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_decl_stmt
DECL|variable|stage
specifier|static
name|int
name|stage
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|read_one_entry
specifier|static
name|int
name|read_one_entry
parameter_list|(
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
parameter_list|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|pathname
argument_list|)
decl_stmt|;
name|unsigned
name|int
name|size
init|=
name|cache_entry_size
argument_list|(
name|baselen
operator|+
name|len
argument_list|)
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|malloc
argument_list|(
name|size
argument_list|)
decl_stmt|;
name|memset
argument_list|(
name|ce
argument_list|,
literal|0
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
name|memcpy
argument_list|(
name|ce
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
return|return
name|add_cache_entry
argument_list|(
name|ce
argument_list|,
literal|1
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|read_tree
specifier|static
name|int
name|read_tree
parameter_list|(
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
parameter_list|)
block|{
name|void
modifier|*
name|buffer
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|buffer
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
name|buffer
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tree"
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
while|while
condition|(
name|size
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|buffer
argument_list|)
operator|+
literal|1
decl_stmt|;
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|buffer
operator|+
name|len
decl_stmt|;
name|char
modifier|*
name|path
init|=
name|strchr
argument_list|(
name|buffer
argument_list|,
literal|' '
argument_list|)
operator|+
literal|1
decl_stmt|;
name|unsigned
name|int
name|mode
decl_stmt|;
if|if
condition|(
name|size
operator|<
name|len
operator|+
literal|20
operator|||
name|sscanf
argument_list|(
name|buffer
argument_list|,
literal|"%o"
argument_list|,
operator|&
name|mode
argument_list|)
operator|!=
literal|1
condition|)
return|return
operator|-
literal|1
return|;
name|buffer
operator|=
name|sha1
operator|+
literal|20
expr_stmt|;
name|size
operator|-=
name|len
operator|+
literal|20
expr_stmt|;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
block|{
name|int
name|retval
decl_stmt|;
name|int
name|pathlen
init|=
name|strlen
argument_list|(
name|path
argument_list|)
decl_stmt|;
name|char
modifier|*
name|newbase
init|=
name|malloc
argument_list|(
name|baselen
operator|+
literal|1
operator|+
name|pathlen
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
name|read_tree
argument_list|(
name|sha1
argument_list|,
name|newbase
argument_list|,
name|baselen
operator|+
name|pathlen
operator|+
literal|1
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
if|if
condition|(
name|read_one_entry
argument_list|(
name|sha1
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|,
name|path
argument_list|,
name|mode
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|remove_lock
specifier|static
name|int
name|remove_lock
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|remove_lock_file
specifier|static
name|void
name|remove_lock_file
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
name|remove_lock
condition|)
name|unlink
argument_list|(
literal|".git/index.lock"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|same
specifier|static
name|int
name|same
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|a
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|b
parameter_list|)
block|{
return|return
name|a
operator|->
name|ce_mode
operator|==
name|b
operator|->
name|ce_mode
operator|&&
operator|!
name|memcmp
argument_list|(
name|a
operator|->
name|sha1
argument_list|,
name|b
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * This removes all trivial merges that don't change the tree  * and collapses them to state 0.  *  * _Any_ other merge is left to user policy.  That includes "both  * created the same file", and "both removed the same file" - which are  * trivial, but the user might still want to _note_ it.   */
end_comment

begin_function
DECL|function|merge_entries
specifier|static
name|struct
name|cache_entry
modifier|*
name|merge_entries
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|a
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|b
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|c
parameter_list|)
block|{
name|int
name|len
init|=
name|ce_namelen
argument_list|(
name|a
argument_list|)
decl_stmt|;
comment|/* 	 * Are they all the same filename? We won't do 	 * any name merging 	 */
if|if
condition|(
name|ce_namelen
argument_list|(
name|b
argument_list|)
operator|!=
name|len
operator|||
name|ce_namelen
argument_list|(
name|c
argument_list|)
operator|!=
name|len
operator|||
name|memcmp
argument_list|(
name|a
operator|->
name|name
argument_list|,
name|b
operator|->
name|name
argument_list|,
name|len
argument_list|)
operator|||
name|memcmp
argument_list|(
name|a
operator|->
name|name
argument_list|,
name|c
operator|->
name|name
argument_list|,
name|len
argument_list|)
condition|)
return|return
name|NULL
return|;
comment|/* 	 * Ok, all three entries describe the same 	 * filename, but maybe the contents or file 	 * mode have changed? 	 * 	 * The trivial cases end up being the ones where two 	 * out of three files are the same: 	 *  - both destinations the same, trivially take either 	 *  - one of the destination versions hasn't changed, 	 *    take the other. 	 * 	 * The "all entries exactly the same" case falls out as 	 * a special case of any of the "two same" cases. 	 * 	 * Here "a" is "original", and "b" and "c" are the two 	 * trees we are merging. 	 */
if|if
condition|(
name|same
argument_list|(
name|b
argument_list|,
name|c
argument_list|)
condition|)
return|return
name|c
return|;
if|if
condition|(
name|same
argument_list|(
name|a
argument_list|,
name|b
argument_list|)
condition|)
return|return
name|c
return|;
if|if
condition|(
name|same
argument_list|(
name|a
argument_list|,
name|c
argument_list|)
condition|)
return|return
name|b
return|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|trivially_merge_cache
specifier|static
name|void
name|trivially_merge_cache
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|src
parameter_list|,
name|int
name|nr
parameter_list|)
block|{
name|struct
name|cache_entry
modifier|*
modifier|*
name|dst
init|=
name|src
decl_stmt|;
while|while
condition|(
name|nr
condition|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|,
modifier|*
name|result
decl_stmt|;
name|ce
operator|=
name|src
index|[
literal|0
index|]
expr_stmt|;
if|if
condition|(
name|nr
operator|>
literal|2
operator|&&
operator|(
name|result
operator|=
name|merge_entries
argument_list|(
name|ce
argument_list|,
name|src
index|[
literal|1
index|]
argument_list|,
name|src
index|[
literal|2
index|]
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|ce
operator|=
name|result
expr_stmt|;
name|ce
operator|->
name|ce_flags
operator|&=
operator|~
name|htons
argument_list|(
name|CE_STAGEMASK
argument_list|)
expr_stmt|;
name|src
operator|+=
literal|2
expr_stmt|;
name|nr
operator|-=
literal|2
expr_stmt|;
name|active_nr
operator|-=
literal|2
expr_stmt|;
block|}
operator|*
name|dst
operator|=
name|ce
expr_stmt|;
name|src
operator|++
expr_stmt|;
name|dst
operator|++
expr_stmt|;
name|nr
operator|--
expr_stmt|;
block|}
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
name|int
name|i
decl_stmt|,
name|newfd
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|newfd
operator|=
name|open
argument_list|(
literal|".git/index.lock"
argument_list|,
name|O_RDWR
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0600
argument_list|)
expr_stmt|;
if|if
condition|(
name|newfd
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to create new cachefile"
argument_list|)
expr_stmt|;
name|atexit
argument_list|(
name|remove_lock_file
argument_list|)
expr_stmt|;
name|remove_lock
operator|=
literal|1
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
comment|/* "-m" stands for "merge", meaning we start in stage 1 */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-m"
argument_list|)
condition|)
block|{
name|stage
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|arg
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
name|usage
argument_list|(
literal|"read-tree [-m]<sha1>"
argument_list|)
expr_stmt|;
if|if
condition|(
name|stage
operator|>
literal|3
condition|)
name|usage
argument_list|(
literal|"can't merge more than two trees"
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_tree
argument_list|(
name|sha1
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"failed to unpack tree object %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|stage
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|stage
operator|==
literal|4
condition|)
name|trivially_merge_cache
argument_list|(
name|active_cache
argument_list|,
name|active_nr
argument_list|)
expr_stmt|;
if|if
condition|(
name|write_cache
argument_list|(
name|newfd
argument_list|,
name|active_cache
argument_list|,
name|active_nr
argument_list|)
operator|||
name|rename
argument_list|(
literal|".git/index.lock"
argument_list|,
literal|".git/index"
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to write new index file"
argument_list|)
expr_stmt|;
name|remove_lock
operator|=
literal|0
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

