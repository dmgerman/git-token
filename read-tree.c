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
DECL|function|unpack_tree
specifier|static
name|int
name|unpack_tree
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
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
name|int
name|ret
decl_stmt|;
name|buffer
operator|=
name|read_object_with_reference
argument_list|(
name|sha1
argument_list|,
literal|"tree"
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
name|buffer
condition|)
return|return
operator|-
literal|1
return|;
name|ret
operator|=
name|read_tree
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|,
name|stage
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|lockfile_name
specifier|static
name|char
modifier|*
name|lockfile_name
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
name|lockfile_name
condition|)
name|unlink
argument_list|(
name|lockfile_name
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|path_matches
specifier|static
name|int
name|path_matches
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
name|int
name|len
init|=
name|ce_namelen
argument_list|(
name|a
argument_list|)
decl_stmt|;
return|return
name|ce_namelen
argument_list|(
name|b
argument_list|)
operator|==
name|len
operator|&&
operator|!
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
return|;
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

begin_comment
comment|/*  * When a CE gets turned into an unmerged entry, we  * want it to be up-to-date  */
end_comment

begin_function
DECL|function|verify_uptodate
specifier|static
name|void
name|verify_uptodate
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
if|if
condition|(
operator|!
name|lstat
argument_list|(
name|ce
operator|->
name|name
argument_list|,
operator|&
name|st
argument_list|)
condition|)
block|{
name|unsigned
name|changed
init|=
name|ce_match_stat
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|changed
condition|)
return|return;
name|errno
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|errno
operator|==
name|ENOENT
condition|)
return|return;
name|die
argument_list|(
literal|"Entry '%s' not uptodate. Cannot merge."
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * If the old tree contained a CE that isn't even in the  * result, that's always a problem, regardless of whether  * it's up-to-date or not (ie it can be a file that we  * have updated but not committed yet).  */
end_comment

begin_function
DECL|function|verify_cleared
specifier|static
name|void
name|verify_cleared
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|)
block|{
if|if
condition|(
name|ce
condition|)
name|die
argument_list|(
literal|"Entry '%s' would be overwritten by merge. Cannot merge."
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|old_match
specifier|static
name|int
name|old_match
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|old
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|a
parameter_list|)
block|{
return|return
name|old
operator|&&
name|path_matches
argument_list|(
name|old
argument_list|,
name|a
argument_list|)
operator|&&
name|same
argument_list|(
name|old
argument_list|,
name|a
argument_list|)
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
name|struct
name|cache_entry
modifier|*
name|old
init|=
name|NULL
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
comment|/* We throw away original cache entries except for the stat information */
if|if
condition|(
operator|!
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|)
block|{
name|verify_cleared
argument_list|(
name|old
argument_list|)
expr_stmt|;
name|old
operator|=
name|ce
expr_stmt|;
name|src
operator|++
expr_stmt|;
name|nr
operator|--
expr_stmt|;
name|active_nr
operator|--
expr_stmt|;
continue|continue;
block|}
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
comment|/* 			 * See if we can re-use the old CE directly? 			 * That way we get the uptodate stat info. 			 */
if|if
condition|(
name|old_match
argument_list|(
name|old
argument_list|,
name|result
argument_list|)
condition|)
block|{
operator|*
name|result
operator|=
operator|*
name|old
expr_stmt|;
name|old
operator|=
name|NULL
expr_stmt|;
block|}
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
comment|/* 		 * If we had an old entry that we now effectively 		 * overwrite, make sure it wasn't dirty. 		 */
if|if
condition|(
name|old_match
argument_list|(
name|old
argument_list|,
name|ce
argument_list|)
condition|)
block|{
name|verify_uptodate
argument_list|(
name|old
argument_list|)
expr_stmt|;
name|old
operator|=
name|NULL
expr_stmt|;
block|}
operator|*
name|dst
operator|++
operator|=
name|ce
expr_stmt|;
name|src
operator|++
expr_stmt|;
name|nr
operator|--
expr_stmt|;
block|}
name|verify_cleared
argument_list|(
name|old
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|merge_stat_info
specifier|static
name|void
name|merge_stat_info
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
specifier|static
name|struct
name|cache_entry
name|null_entry
decl_stmt|;
name|struct
name|cache_entry
modifier|*
modifier|*
name|dst
init|=
name|src
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|old
init|=
operator|&
name|null_entry
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
decl_stmt|;
name|ce
operator|=
name|src
index|[
literal|0
index|]
expr_stmt|;
comment|/* We throw away original cache entries except for the stat information */
if|if
condition|(
operator|!
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|)
block|{
name|old
operator|=
name|ce
expr_stmt|;
name|src
operator|++
expr_stmt|;
name|nr
operator|--
expr_stmt|;
name|active_nr
operator|--
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|path_matches
argument_list|(
name|ce
argument_list|,
name|old
argument_list|)
operator|&&
name|same
argument_list|(
name|ce
argument_list|,
name|old
argument_list|)
condition|)
operator|*
name|ce
operator|=
operator|*
name|old
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
operator|*
name|dst
operator|++
operator|=
name|ce
expr_stmt|;
name|src
operator|++
expr_stmt|;
name|nr
operator|--
expr_stmt|;
block|}
block|}
end_function

begin_decl_stmt
DECL|variable|read_tree_usage
specifier|static
name|char
modifier|*
name|read_tree_usage
init|=
literal|"git-read-tree (<sha> | -m<sha1> [<sha2><sha3>])"
decl_stmt|;
end_decl_stmt

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
decl_stmt|,
name|merge
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
specifier|static
name|char
name|lockfile
index|[
name|MAXPATHLEN
operator|+
literal|1
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|indexfile
init|=
name|get_index_file
argument_list|()
decl_stmt|;
name|snprintf
argument_list|(
name|lockfile
argument_list|,
sizeof|sizeof
argument_list|(
name|lockfile
argument_list|)
argument_list|,
literal|"%s.lock"
argument_list|,
name|indexfile
argument_list|)
expr_stmt|;
name|newfd
operator|=
name|open
argument_list|(
name|lockfile
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
name|lockfile_name
operator|=
name|lockfile
expr_stmt|;
name|merge
operator|=
literal|0
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
name|int
name|i
decl_stmt|;
if|if
condition|(
name|stage
condition|)
name|die
argument_list|(
literal|"-m needs to come first"
argument_list|)
expr_stmt|;
name|read_cache
argument_list|()
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|active_nr
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ce_stage
argument_list|(
name|active_cache
index|[
name|i
index|]
argument_list|)
condition|)
name|die
argument_list|(
literal|"you need to resolve your current index first"
argument_list|)
expr_stmt|;
block|}
name|stage
operator|=
literal|1
expr_stmt|;
name|merge
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|get_sha1
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
name|read_tree_usage
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
name|read_tree_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|unpack_tree
argument_list|(
name|sha1
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
name|merge
condition|)
block|{
switch|switch
condition|(
name|stage
condition|)
block|{
case|case
literal|4
case|:
comment|/* Three-way merge */
name|trivially_merge_cache
argument_list|(
name|active_cache
argument_list|,
name|active_nr
argument_list|)
expr_stmt|;
break|break;
case|case
literal|2
case|:
comment|/* Just read a tree, merge with old cache contents */
name|merge_stat_info
argument_list|(
name|active_cache
argument_list|,
name|active_nr
argument_list|)
expr_stmt|;
break|break;
default|default:
name|die
argument_list|(
literal|"just how do you expect me to merge %d trees?"
argument_list|,
name|stage
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
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
name|lockfile
argument_list|,
name|indexfile
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to write new index file"
argument_list|)
expr_stmt|;
name|lockfile_name
operator|=
name|NULL
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

