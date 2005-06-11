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

begin_decl_stmt
DECL|variable|update
specifier|static
name|int
name|update
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
comment|/*  * This removes all trivial merges that don't change the tree  * and collapses them to state 0.  */
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
comment|/* 	 * Ok, all three entries describe the same 	 * filename, but maybe the contents or file 	 * mode have changed? 	 * 	 * The trivial cases end up being the ones where two 	 * out of three files are the same: 	 *  - both destinations the same, trivially take either 	 *  - one of the destination versions hasn't changed, 	 *    take the other. 	 * 	 * The "all entries exactly the same" case falls out as 	 * a special case of any of the "two same" cases. 	 * 	 * Here "a" is "original", and "b" and "c" are the two 	 * trees we are merging. 	 */
if|if
condition|(
name|a
operator|&&
name|b
operator|&&
name|c
condition|)
block|{
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
block|}
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
DECL|function|reject_merge
specifier|static
name|void
name|reject_merge
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|)
block|{
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
DECL|function|merged_entry_internal
specifier|static
name|int
name|merged_entry_internal
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|merge
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|old
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|dst
parameter_list|,
name|int
name|allow_dirty
parameter_list|)
block|{
name|merge
operator|->
name|ce_flags
operator||=
name|htons
argument_list|(
name|CE_UPDATE
argument_list|)
expr_stmt|;
if|if
condition|(
name|old
condition|)
block|{
comment|/* 		 * See if we can re-use the old CE directly? 		 * That way we get the uptodate stat info. 		 * 		 * This also removes the UPDATE flag on 		 * a match. 		 */
if|if
condition|(
name|same
argument_list|(
name|old
argument_list|,
name|merge
argument_list|)
condition|)
block|{
operator|*
name|merge
operator|=
operator|*
name|old
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|allow_dirty
condition|)
block|{
name|verify_uptodate
argument_list|(
name|old
argument_list|)
expr_stmt|;
block|}
block|}
name|merge
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
name|merge
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|merged_entry_allow_dirty
specifier|static
name|int
name|merged_entry_allow_dirty
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|merge
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|old
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|dst
parameter_list|)
block|{
return|return
name|merged_entry_internal
argument_list|(
name|merge
argument_list|,
name|old
argument_list|,
name|dst
argument_list|,
literal|1
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|merged_entry
specifier|static
name|int
name|merged_entry
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|merge
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|old
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|dst
parameter_list|)
block|{
return|return
name|merged_entry_internal
argument_list|(
name|merge
argument_list|,
name|old
argument_list|,
name|dst
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|deleted_entry
specifier|static
name|int
name|deleted_entry
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|old
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|dst
parameter_list|)
block|{
if|if
condition|(
name|old
condition|)
name|verify_uptodate
argument_list|(
name|old
argument_list|)
expr_stmt|;
name|ce
operator|->
name|ce_mode
operator|=
literal|0
expr_stmt|;
operator|*
name|dst
operator|++
operator|=
name|ce
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|threeway_merge
specifier|static
name|int
name|threeway_merge
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|stages
index|[
literal|4
index|]
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|dst
parameter_list|)
block|{
name|struct
name|cache_entry
modifier|*
name|old
init|=
name|stages
index|[
literal|0
index|]
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|a
init|=
name|stages
index|[
literal|1
index|]
decl_stmt|,
modifier|*
name|b
init|=
name|stages
index|[
literal|2
index|]
decl_stmt|,
modifier|*
name|c
init|=
name|stages
index|[
literal|3
index|]
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|merge
decl_stmt|;
name|int
name|count
decl_stmt|;
comment|/* #5ALT */
if|if
condition|(
operator|!
name|a
operator|&&
name|b
operator|&&
name|c
operator|&&
name|same
argument_list|(
name|b
argument_list|,
name|c
argument_list|)
condition|)
block|{
if|if
condition|(
name|old
operator|&&
operator|!
name|same
argument_list|(
name|b
argument_list|,
name|old
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
return|return
name|merged_entry_allow_dirty
argument_list|(
name|b
argument_list|,
name|old
argument_list|,
name|dst
argument_list|)
return|;
block|}
comment|/* 	 * If we have an entry in the index cache ("old"), then we want 	 * to make sure that it matches any entries in stage 2 ("first 	 * branch", aka "b"). 	 */
if|if
condition|(
name|old
condition|)
block|{
if|if
condition|(
operator|!
name|b
operator|||
operator|!
name|same
argument_list|(
name|old
argument_list|,
name|b
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
block|}
name|merge
operator|=
name|merge_entries
argument_list|(
name|a
argument_list|,
name|b
argument_list|,
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|merge
condition|)
return|return
name|merged_entry
argument_list|(
name|merge
argument_list|,
name|old
argument_list|,
name|dst
argument_list|)
return|;
if|if
condition|(
name|old
condition|)
name|verify_uptodate
argument_list|(
name|old
argument_list|)
expr_stmt|;
name|count
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|a
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
name|a
expr_stmt|;
name|count
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|b
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
name|b
expr_stmt|;
name|count
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|c
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
name|c
expr_stmt|;
name|count
operator|++
expr_stmt|;
block|}
return|return
name|count
return|;
block|}
end_function

begin_comment
comment|/*  * Two-way merge.  *  * The rule is to "carry forward" what is in the index without losing  * information across a "fast forward", favoring a successful merge  * over a merge failure when it makes sense.  For details of the  * "carry forward" rule, please see<Documentation/git-read-tree.txt>.  *  */
end_comment

begin_function
DECL|function|twoway_merge
specifier|static
name|int
name|twoway_merge
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|src
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|dst
parameter_list|)
block|{
name|struct
name|cache_entry
modifier|*
name|current
init|=
name|src
index|[
literal|0
index|]
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|oldtree
init|=
name|src
index|[
literal|1
index|]
decl_stmt|,
modifier|*
name|newtree
init|=
name|src
index|[
literal|2
index|]
decl_stmt|;
if|if
condition|(
name|src
index|[
literal|3
index|]
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|current
condition|)
block|{
if|if
condition|(
operator|(
operator|!
name|oldtree
operator|&&
operator|!
name|newtree
operator|)
operator|||
comment|/* 4 and 5 */
operator|(
operator|!
name|oldtree
operator|&&
name|newtree
operator|&&
name|same
argument_list|(
name|current
argument_list|,
name|newtree
argument_list|)
operator|)
operator|||
comment|/* 6 and 7 */
operator|(
name|oldtree
operator|&&
name|newtree
operator|&&
name|same
argument_list|(
name|oldtree
argument_list|,
name|newtree
argument_list|)
operator|)
operator|||
comment|/* 14 and 15 */
operator|(
name|oldtree
operator|&&
name|newtree
operator|&&
operator|!
name|same
argument_list|(
name|oldtree
argument_list|,
name|newtree
argument_list|)
operator|&&
comment|/* 18 and 19*/
name|same
argument_list|(
name|current
argument_list|,
name|newtree
argument_list|)
operator|)
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
name|current
expr_stmt|;
return|return
literal|1
return|;
block|}
elseif|else
if|if
condition|(
name|oldtree
operator|&&
operator|!
name|newtree
operator|&&
name|same
argument_list|(
name|current
argument_list|,
name|oldtree
argument_list|)
condition|)
block|{
comment|/* 10 or 11 */
return|return
name|deleted_entry
argument_list|(
name|oldtree
argument_list|,
name|current
argument_list|,
name|dst
argument_list|)
return|;
block|}
elseif|else
if|if
condition|(
name|oldtree
operator|&&
name|newtree
operator|&&
name|same
argument_list|(
name|current
argument_list|,
name|oldtree
argument_list|)
operator|&&
operator|!
name|same
argument_list|(
name|current
argument_list|,
name|newtree
argument_list|)
condition|)
block|{
comment|/* 20 or 21 */
return|return
name|merged_entry
argument_list|(
name|newtree
argument_list|,
name|current
argument_list|,
name|dst
argument_list|)
return|;
block|}
else|else
comment|/* all other failures */
return|return
operator|-
literal|1
return|;
block|}
elseif|else
if|if
condition|(
name|newtree
condition|)
return|return
name|merged_entry
argument_list|(
name|newtree
argument_list|,
name|current
argument_list|,
name|dst
argument_list|)
return|;
else|else
return|return
name|deleted_entry
argument_list|(
name|oldtree
argument_list|,
name|current
argument_list|,
name|dst
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * Two-way merge emulated with three-way merge.  *  * This treats "read-tree -m H M" by transforming it internally  * into "read-tree -m H I+H M", where I+H is a tree that would  * contain the contents of the current index file, overlayed on  * top of H.  Unlike the traditional two-way merge, this leaves  * the stages in the resulting index file and lets the user resolve  * the merge conflicts using standard tools for three-way merge.  *  * This function is just to set-up such an arrangement, and the  * actual merge uses threeway_merge() function.  */
end_comment

begin_function
DECL|function|setup_emu23
specifier|static
name|void
name|setup_emu23
parameter_list|(
name|void
parameter_list|)
block|{
comment|/* stage0 contains I, stage1 H, stage2 M. 	 * move stage2 to stage3, and create stage2 entries 	 * by scanning stage0 and stage1 entries. 	 */
name|int
name|i
decl_stmt|,
name|namelen
decl_stmt|,
name|size
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|,
modifier|*
name|stage2
decl_stmt|;
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
name|ce
operator|=
name|active_cache
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
operator|!=
literal|2
condition|)
continue|continue;
comment|/* hoist them up to stage 3 */
name|namelen
operator|=
name|ce_namelen
argument_list|(
name|ce
argument_list|)
expr_stmt|;
name|ce
operator|->
name|ce_flags
operator|=
name|create_ce_flags
argument_list|(
name|namelen
argument_list|,
literal|3
argument_list|)
expr_stmt|;
block|}
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
name|ce
operator|=
name|active_cache
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
operator|>
literal|1
condition|)
continue|continue;
name|namelen
operator|=
name|ce_namelen
argument_list|(
name|ce
argument_list|)
expr_stmt|;
name|size
operator|=
name|cache_entry_size
argument_list|(
name|namelen
argument_list|)
expr_stmt|;
name|stage2
operator|=
name|xmalloc
argument_list|(
name|size
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|stage2
argument_list|,
name|ce
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|stage2
operator|->
name|ce_flags
operator|=
name|create_ce_flags
argument_list|(
name|namelen
argument_list|,
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
name|add_cache_entry
argument_list|(
name|stage2
argument_list|,
name|ADD_CACHE_OK_TO_ADD
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"cannot merge index and our head tree"
argument_list|)
expr_stmt|;
comment|/* We are done with this name, so skip to next name */
while|while
condition|(
name|i
operator|<
name|active_nr
operator|&&
name|ce_namelen
argument_list|(
name|active_cache
index|[
name|i
index|]
argument_list|)
operator|==
name|namelen
operator|&&
operator|!
name|memcmp
argument_list|(
name|active_cache
index|[
name|i
index|]
operator|->
name|name
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|namelen
argument_list|)
condition|)
name|i
operator|++
expr_stmt|;
name|i
operator|--
expr_stmt|;
comment|/* compensate for the loop control */
block|}
block|}
end_function

begin_comment
comment|/*  * One-way merge.  *  * The rule is:  * - take the stat information from stage0, take the data from stage1  */
end_comment

begin_function
DECL|function|oneway_merge
specifier|static
name|int
name|oneway_merge
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|src
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|dst
parameter_list|)
block|{
name|struct
name|cache_entry
modifier|*
name|old
init|=
name|src
index|[
literal|0
index|]
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|a
init|=
name|src
index|[
literal|1
index|]
decl_stmt|;
if|if
condition|(
name|src
index|[
literal|2
index|]
operator|||
name|src
index|[
literal|3
index|]
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|a
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|old
operator|&&
name|same
argument_list|(
name|old
argument_list|,
name|a
argument_list|)
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
name|old
expr_stmt|;
return|return
literal|1
return|;
block|}
return|return
name|merged_entry
argument_list|(
name|a
argument_list|,
name|NULL
argument_list|,
name|dst
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|check_updates
specifier|static
name|void
name|check_updates
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
name|checkout
name|state
init|=
block|{
operator|.
name|base_dir
operator|=
literal|""
block|,
operator|.
name|force
operator|=
literal|1
block|,
operator|.
name|quiet
operator|=
literal|1
block|,
operator|.
name|refresh_cache
operator|=
literal|1
block|, 	}
decl_stmt|;
name|unsigned
name|short
name|mask
init|=
name|htons
argument_list|(
name|CE_UPDATE
argument_list|)
decl_stmt|;
while|while
condition|(
name|nr
operator|--
condition|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
operator|*
name|src
operator|++
decl_stmt|;
if|if
condition|(
operator|!
name|ce
operator|->
name|ce_mode
condition|)
block|{
if|if
condition|(
name|update
condition|)
name|unlink
argument_list|(
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|ce
operator|->
name|ce_flags
operator|&
name|mask
condition|)
block|{
name|ce
operator|->
name|ce_flags
operator|&=
operator|~
name|mask
expr_stmt|;
if|if
condition|(
name|update
condition|)
name|checkout_entry
argument_list|(
name|ce
argument_list|,
operator|&
name|state
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_typedef
DECL|typedef|merge_fn_t
typedef|typedef
name|int
function_decl|(
modifier|*
name|merge_fn_t
function_decl|)
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_function
DECL|function|merge_cache
specifier|static
name|void
name|merge_cache
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|src
parameter_list|,
name|int
name|nr
parameter_list|,
name|merge_fn_t
name|fn
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
name|int
name|entries
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|name
decl_stmt|,
modifier|*
name|ce
decl_stmt|,
modifier|*
name|stages
index|[
literal|4
index|]
init|=
block|{
name|NULL
block|, }
decl_stmt|;
name|name
operator|=
name|ce
operator|=
operator|*
name|src
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|stage
init|=
name|ce_stage
argument_list|(
name|ce
argument_list|)
decl_stmt|;
name|stages
index|[
name|stage
index|]
operator|=
name|ce
expr_stmt|;
name|ce
operator|=
operator|*
operator|++
name|src
expr_stmt|;
name|active_nr
operator|--
expr_stmt|;
if|if
condition|(
operator|!
operator|--
name|nr
condition|)
break|break;
if|if
condition|(
operator|!
name|path_matches
argument_list|(
name|ce
argument_list|,
name|name
argument_list|)
condition|)
break|break;
block|}
name|entries
operator|=
name|fn
argument_list|(
name|stages
argument_list|,
name|dst
argument_list|)
expr_stmt|;
if|if
condition|(
name|entries
operator|<
literal|0
condition|)
name|reject_merge
argument_list|(
name|name
argument_list|)
expr_stmt|;
name|dst
operator|+=
name|entries
expr_stmt|;
name|active_nr
operator|+=
name|entries
expr_stmt|;
block|}
name|check_updates
argument_list|(
name|active_cache
argument_list|,
name|active_nr
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|read_cache_unmerged
specifier|static
name|int
name|read_cache_unmerged
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|deleted
decl_stmt|;
name|struct
name|cache_entry
modifier|*
modifier|*
name|dst
decl_stmt|;
name|read_cache
argument_list|()
expr_stmt|;
name|dst
operator|=
name|active_cache
expr_stmt|;
name|deleted
operator|=
literal|0
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
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|active_cache
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|)
block|{
name|deleted
operator|++
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|deleted
condition|)
operator|*
name|dst
operator|=
name|ce
expr_stmt|;
name|dst
operator|++
expr_stmt|;
block|}
name|active_nr
operator|-=
name|deleted
expr_stmt|;
return|return
name|deleted
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|read_tree_usage
specifier|static
name|char
modifier|*
name|read_tree_usage
init|=
literal|"git-read-tree (<sha> | -m [-u]<sha1> [<sha2> [<sha3>]])"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cache_file
specifier|static
name|struct
name|cache_file
name|cache_file
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
decl_stmt|,
name|reset
decl_stmt|,
name|emu23
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
name|hold_index_file_for_update
argument_list|(
operator|&
name|cache_file
argument_list|,
name|get_index_file
argument_list|()
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
name|merge
operator|=
literal|0
expr_stmt|;
name|reset
operator|=
literal|0
expr_stmt|;
name|emu23
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
comment|/* "-u" means "update", meaning that a merge will update the working directory */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-u"
argument_list|)
condition|)
block|{
name|update
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
comment|/* This differs from "-m" in that we'll silently ignore unmerged entries */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--reset"
argument_list|)
condition|)
block|{
if|if
condition|(
name|stage
operator|||
name|merge
operator|||
name|emu23
condition|)
name|usage
argument_list|(
name|read_tree_usage
argument_list|)
expr_stmt|;
name|reset
operator|=
literal|1
expr_stmt|;
name|merge
operator|=
literal|1
expr_stmt|;
name|stage
operator|=
literal|1
expr_stmt|;
name|read_cache_unmerged
argument_list|()
expr_stmt|;
block|}
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
if|if
condition|(
name|stage
operator|||
name|merge
operator|||
name|emu23
condition|)
name|usage
argument_list|(
name|read_tree_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_cache_unmerged
argument_list|()
condition|)
name|die
argument_list|(
literal|"you need to resolve your current index first"
argument_list|)
expr_stmt|;
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
comment|/* "-emu23" uses 3-way merge logic to perform fast-forward */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--emu23"
argument_list|)
condition|)
block|{
if|if
condition|(
name|stage
operator|||
name|merge
operator|||
name|emu23
condition|)
name|usage
argument_list|(
name|read_tree_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_cache_unmerged
argument_list|()
condition|)
name|die
argument_list|(
literal|"you need to resolve your current index first"
argument_list|)
expr_stmt|;
name|merge
operator|=
name|emu23
operator|=
name|stage
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
name|update
operator|&&
operator|!
name|merge
condition|)
name|usage
argument_list|(
name|read_tree_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|merge
condition|)
block|{
specifier|static
specifier|const
name|merge_fn_t
name|merge_function
index|[]
init|=
block|{
index|[
literal|1
index|]
operator|=
name|oneway_merge
block|,
index|[
literal|2
index|]
operator|=
name|twoway_merge
block|,
index|[
literal|3
index|]
operator|=
name|threeway_merge
block|, 		}
decl_stmt|;
name|merge_fn_t
name|fn
decl_stmt|;
if|if
condition|(
name|stage
operator|<
literal|2
operator|||
name|stage
operator|>
literal|4
condition|)
name|die
argument_list|(
literal|"just how do you expect me to merge %d trees?"
argument_list|,
name|stage
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|emu23
operator|&&
name|stage
operator|!=
literal|3
condition|)
name|die
argument_list|(
literal|"--emu23 takes only two trees"
argument_list|)
expr_stmt|;
name|fn
operator|=
name|merge_function
index|[
name|stage
operator|-
literal|1
index|]
expr_stmt|;
if|if
condition|(
name|stage
operator|==
literal|3
operator|&&
name|emu23
condition|)
block|{
name|setup_emu23
argument_list|()
expr_stmt|;
name|fn
operator|=
name|merge_function
index|[
literal|3
index|]
expr_stmt|;
block|}
name|merge_cache
argument_list|(
name|active_cache
argument_list|,
name|active_nr
argument_list|,
name|fn
argument_list|)
expr_stmt|;
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
name|commit_index_file
argument_list|(
operator|&
name|cache_file
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to write new index file"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

