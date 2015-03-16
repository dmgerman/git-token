begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2005 Junio C Hamano  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"diffcore.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"cache-tree.h"
end_include

begin_include
include|#
directive|include
file|"unpack-trees.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"submodule.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_comment
comment|/*  * diff-files  */
end_comment

begin_comment
comment|/*  * Has the work tree entity been removed?  *  * Return 1 if it was removed from the work tree, 0 if an entity to be  * compared with the cache entry ce still exists (the latter includes  * the case where a directory that is not a submodule repository  * exists for ce that is a submodule -- it is a submodule that is not  * checked out).  Return negative for an error.  */
end_comment

begin_function
DECL|function|check_removed
specifier|static
name|int
name|check_removed
parameter_list|(
specifier|const
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|struct
name|stat
modifier|*
name|st
parameter_list|)
block|{
if|if
condition|(
name|lstat
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|st
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|!=
name|ENOENT
operator|&&
name|errno
operator|!=
name|ENOTDIR
condition|)
return|return
operator|-
literal|1
return|;
return|return
literal|1
return|;
block|}
if|if
condition|(
name|has_symlink_leading_path
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|ce_namelen
argument_list|(
name|ce
argument_list|)
argument_list|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|st
operator|->
name|st_mode
argument_list|)
condition|)
block|{
name|unsigned
name|char
name|sub
index|[
literal|20
index|]
decl_stmt|;
comment|/* 		 * If ce is already a gitlink, we can have a plain 		 * directory (i.e. the submodule is not checked out), 		 * or a checked out submodule.  Either case this is not 		 * a case where something was removed from the work tree, 		 * so we will return 0. 		 * 		 * Otherwise, if the directory is not a submodule 		 * repository, that means ce which was a blob turned into 		 * a directory --- the blob was removed! 		 */
if|if
condition|(
operator|!
name|S_ISGITLINK
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
operator|&&
name|resolve_gitlink_ref
argument_list|(
name|ce
operator|->
name|name
argument_list|,
literal|"HEAD"
argument_list|,
name|sub
argument_list|)
condition|)
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * Has a file changed or has a submodule new commits or a dirty work tree?  *  * Return 1 when changes are detected, 0 otherwise. If the DIRTY_SUBMODULES  * option is set, the caller does not only want to know if a submodule is  * modified at all but wants to know all the conditions that are met (new  * commits, untracked content and/or modified content).  */
end_comment

begin_function
DECL|function|match_stat_with_submodule
specifier|static
name|int
name|match_stat_with_submodule
parameter_list|(
name|struct
name|diff_options
modifier|*
name|diffopt
parameter_list|,
specifier|const
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|struct
name|stat
modifier|*
name|st
parameter_list|,
name|unsigned
name|ce_option
parameter_list|,
name|unsigned
modifier|*
name|dirty_submodule
parameter_list|)
block|{
name|int
name|changed
init|=
name|ce_match_stat
argument_list|(
name|ce
argument_list|,
name|st
argument_list|,
name|ce_option
argument_list|)
decl_stmt|;
if|if
condition|(
name|S_ISGITLINK
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
condition|)
block|{
name|unsigned
name|orig_flags
init|=
name|diffopt
operator|->
name|flags
decl_stmt|;
if|if
condition|(
operator|!
name|DIFF_OPT_TST
argument_list|(
name|diffopt
argument_list|,
name|OVERRIDE_SUBMODULE_CONFIG
argument_list|)
condition|)
name|set_diffopt_flags_from_submodule_config
argument_list|(
name|diffopt
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|DIFF_OPT_TST
argument_list|(
name|diffopt
argument_list|,
name|IGNORE_SUBMODULES
argument_list|)
condition|)
name|changed
operator|=
literal|0
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|DIFF_OPT_TST
argument_list|(
name|diffopt
argument_list|,
name|IGNORE_DIRTY_SUBMODULES
argument_list|)
operator|&&
operator|(
operator|!
name|changed
operator|||
name|DIFF_OPT_TST
argument_list|(
name|diffopt
argument_list|,
name|DIRTY_SUBMODULES
argument_list|)
operator|)
condition|)
operator|*
name|dirty_submodule
operator|=
name|is_submodule_modified
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|DIFF_OPT_TST
argument_list|(
name|diffopt
argument_list|,
name|IGNORE_UNTRACKED_IN_SUBMODULES
argument_list|)
argument_list|)
expr_stmt|;
name|diffopt
operator|->
name|flags
operator|=
name|orig_flags
expr_stmt|;
block|}
return|return
name|changed
return|;
block|}
end_function

begin_function
DECL|function|run_diff_files
name|int
name|run_diff_files
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|unsigned
name|int
name|option
parameter_list|)
block|{
name|int
name|entries
decl_stmt|,
name|i
decl_stmt|;
name|int
name|diff_unmerged_stage
init|=
name|revs
operator|->
name|max_count
decl_stmt|;
name|unsigned
name|ce_option
init|=
operator|(
operator|(
name|option
operator|&
name|DIFF_RACY_IS_MODIFIED
operator|)
condition|?
name|CE_MATCH_RACY_IS_DIRTY
else|:
literal|0
operator|)
decl_stmt|;
name|diff_set_mnemonic_prefix
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
literal|"i/"
argument_list|,
literal|"w/"
argument_list|)
expr_stmt|;
if|if
condition|(
name|diff_unmerged_stage
operator|<
literal|0
condition|)
name|diff_unmerged_stage
operator|=
literal|2
expr_stmt|;
name|entries
operator|=
name|active_nr
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|entries
condition|;
name|i
operator|++
control|)
block|{
name|unsigned
name|int
name|oldmode
decl_stmt|,
name|newmode
decl_stmt|;
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
name|int
name|changed
decl_stmt|;
name|unsigned
name|dirty_submodule
init|=
literal|0
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
decl_stmt|,
modifier|*
name|new_sha1
decl_stmt|;
if|if
condition|(
name|diff_can_quit_early
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|)
condition|)
break|break;
if|if
condition|(
operator|!
name|ce_path_match
argument_list|(
name|ce
argument_list|,
operator|&
name|revs
operator|->
name|prune_data
argument_list|,
name|NULL
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|)
block|{
name|struct
name|combine_diff_path
modifier|*
name|dpath
decl_stmt|;
name|struct
name|diff_filepair
modifier|*
name|pair
decl_stmt|;
name|unsigned
name|int
name|wt_mode
init|=
literal|0
decl_stmt|;
name|int
name|num_compare_stages
init|=
literal|0
decl_stmt|;
name|size_t
name|path_len
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|path_len
operator|=
name|ce_namelen
argument_list|(
name|ce
argument_list|)
expr_stmt|;
name|dpath
operator|=
name|xmalloc
argument_list|(
name|combine_diff_path_size
argument_list|(
literal|5
argument_list|,
name|path_len
argument_list|)
argument_list|)
expr_stmt|;
name|dpath
operator|->
name|path
operator|=
operator|(
name|char
operator|*
operator|)
operator|&
operator|(
name|dpath
operator|->
name|parent
index|[
literal|5
index|]
operator|)
expr_stmt|;
name|dpath
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|memcpy
argument_list|(
name|dpath
operator|->
name|path
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|path_len
argument_list|)
expr_stmt|;
name|dpath
operator|->
name|path
index|[
name|path_len
index|]
operator|=
literal|'\0'
expr_stmt|;
name|hashclr
argument_list|(
name|dpath
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
operator|(
name|dpath
operator|->
name|parent
index|[
literal|0
index|]
operator|)
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|combine_diff_parent
argument_list|)
operator|*
literal|5
argument_list|)
expr_stmt|;
name|changed
operator|=
name|check_removed
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|changed
condition|)
name|wt_mode
operator|=
name|ce_mode_from_stat
argument_list|(
name|ce
argument_list|,
name|st
operator|.
name|st_mode
argument_list|)
expr_stmt|;
else|else
block|{
if|if
condition|(
name|changed
operator|<
literal|0
condition|)
block|{
name|perror
argument_list|(
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|wt_mode
operator|=
literal|0
expr_stmt|;
block|}
name|dpath
operator|->
name|mode
operator|=
name|wt_mode
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|entries
condition|)
block|{
name|struct
name|cache_entry
modifier|*
name|nce
init|=
name|active_cache
index|[
name|i
index|]
decl_stmt|;
name|int
name|stage
decl_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|nce
operator|->
name|name
argument_list|)
condition|)
break|break;
comment|/* Stage #2 (ours) is the first parent, 				 * stage #3 (theirs) is the second. 				 */
name|stage
operator|=
name|ce_stage
argument_list|(
name|nce
argument_list|)
expr_stmt|;
if|if
condition|(
literal|2
operator|<=
name|stage
condition|)
block|{
name|int
name|mode
init|=
name|nce
operator|->
name|ce_mode
decl_stmt|;
name|num_compare_stages
operator|++
expr_stmt|;
name|hashcpy
argument_list|(
name|dpath
operator|->
name|parent
index|[
name|stage
operator|-
literal|2
index|]
operator|.
name|sha1
argument_list|,
name|nce
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|dpath
operator|->
name|parent
index|[
name|stage
operator|-
literal|2
index|]
operator|.
name|mode
operator|=
name|ce_mode_from_stat
argument_list|(
name|nce
argument_list|,
name|mode
argument_list|)
expr_stmt|;
name|dpath
operator|->
name|parent
index|[
name|stage
operator|-
literal|2
index|]
operator|.
name|status
operator|=
name|DIFF_STATUS_MODIFIED
expr_stmt|;
block|}
comment|/* diff against the proper unmerged stage */
if|if
condition|(
name|stage
operator|==
name|diff_unmerged_stage
condition|)
name|ce
operator|=
name|nce
expr_stmt|;
name|i
operator|++
expr_stmt|;
block|}
comment|/* 			 * Compensate for loop update 			 */
name|i
operator|--
expr_stmt|;
if|if
condition|(
name|revs
operator|->
name|combine_merges
operator|&&
name|num_compare_stages
operator|==
literal|2
condition|)
block|{
name|show_combined_diff
argument_list|(
name|dpath
argument_list|,
literal|2
argument_list|,
name|revs
operator|->
name|dense_combined_merges
argument_list|,
name|revs
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|dpath
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|free
argument_list|(
name|dpath
argument_list|)
expr_stmt|;
name|dpath
operator|=
name|NULL
expr_stmt|;
comment|/* 			 * Show the diff for the 'ce' if we found the one 			 * from the desired stage. 			 */
name|pair
operator|=
name|diff_unmerge
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|wt_mode
condition|)
name|pair
operator|->
name|two
operator|->
name|mode
operator|=
name|wt_mode
expr_stmt|;
if|if
condition|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
operator|!=
name|diff_unmerged_stage
condition|)
continue|continue;
block|}
if|if
condition|(
name|ce_uptodate
argument_list|(
name|ce
argument_list|)
operator|||
name|ce_skip_worktree
argument_list|(
name|ce
argument_list|)
condition|)
continue|continue;
comment|/* If CE_VALID is set, don't look at workdir for file removal */
if|if
condition|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_VALID
condition|)
block|{
name|changed
operator|=
literal|0
expr_stmt|;
name|newmode
operator|=
name|ce
operator|->
name|ce_mode
expr_stmt|;
block|}
else|else
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|changed
operator|=
name|check_removed
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
if|if
condition|(
name|changed
condition|)
block|{
if|if
condition|(
name|changed
operator|<
literal|0
condition|)
block|{
name|perror
argument_list|(
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|diff_addremove
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
literal|'-'
argument_list|,
name|ce
operator|->
name|ce_mode
argument_list|,
name|ce
operator|->
name|sha1
argument_list|,
operator|!
name|is_null_sha1
argument_list|(
name|ce
operator|->
name|sha1
argument_list|)
argument_list|,
name|ce
operator|->
name|name
argument_list|,
literal|0
argument_list|)
expr_stmt|;
continue|continue;
block|}
elseif|else
if|if
condition|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_INTENT_TO_ADD
condition|)
block|{
name|diff_addremove
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
literal|'+'
argument_list|,
name|ce
operator|->
name|ce_mode
argument_list|,
name|EMPTY_BLOB_SHA1_BIN
argument_list|,
literal|0
argument_list|,
name|ce
operator|->
name|name
argument_list|,
literal|0
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|changed
operator|=
name|match_stat_with_submodule
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
name|ce
argument_list|,
operator|&
name|st
argument_list|,
name|ce_option
argument_list|,
operator|&
name|dirty_submodule
argument_list|)
expr_stmt|;
name|newmode
operator|=
name|ce_mode_from_stat
argument_list|(
name|ce
argument_list|,
name|st
operator|.
name|st_mode
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|changed
operator|&&
operator|!
name|dirty_submodule
condition|)
block|{
name|ce_mark_uptodate
argument_list|(
name|ce
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|DIFF_OPT_TST
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
name|FIND_COPIES_HARDER
argument_list|)
condition|)
continue|continue;
block|}
name|oldmode
operator|=
name|ce
operator|->
name|ce_mode
expr_stmt|;
name|old_sha1
operator|=
name|ce
operator|->
name|sha1
expr_stmt|;
name|new_sha1
operator|=
name|changed
condition|?
name|null_sha1
else|:
name|ce
operator|->
name|sha1
expr_stmt|;
name|diff_change
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
name|oldmode
argument_list|,
name|newmode
argument_list|,
name|old_sha1
argument_list|,
name|new_sha1
argument_list|,
operator|!
name|is_null_sha1
argument_list|(
name|old_sha1
argument_list|)
argument_list|,
operator|!
name|is_null_sha1
argument_list|(
name|new_sha1
argument_list|)
argument_list|,
name|ce
operator|->
name|name
argument_list|,
literal|0
argument_list|,
name|dirty_submodule
argument_list|)
expr_stmt|;
block|}
name|diffcore_std
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|)
expr_stmt|;
name|diff_flush
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * diff-index  */
end_comment

begin_comment
comment|/* A file entry went away or appeared */
end_comment

begin_function
DECL|function|diff_index_show_file
specifier|static
name|void
name|diff_index_show_file
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|sha1_valid
parameter_list|,
name|unsigned
name|int
name|mode
parameter_list|,
name|unsigned
name|dirty_submodule
parameter_list|)
block|{
name|diff_addremove
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
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
name|sha1_valid
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|dirty_submodule
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_stat_data
specifier|static
name|int
name|get_stat_data
parameter_list|(
specifier|const
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
modifier|*
name|sha1p
parameter_list|,
name|unsigned
name|int
modifier|*
name|modep
parameter_list|,
name|int
name|cached
parameter_list|,
name|int
name|match_missing
parameter_list|,
name|unsigned
modifier|*
name|dirty_submodule
parameter_list|,
name|struct
name|diff_options
modifier|*
name|diffopt
parameter_list|)
block|{
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|ce
operator|->
name|sha1
decl_stmt|;
name|unsigned
name|int
name|mode
init|=
name|ce
operator|->
name|ce_mode
decl_stmt|;
if|if
condition|(
operator|!
name|cached
operator|&&
operator|!
name|ce_uptodate
argument_list|(
name|ce
argument_list|)
condition|)
block|{
name|int
name|changed
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|changed
operator|=
name|check_removed
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
if|if
condition|(
name|changed
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
elseif|else
if|if
condition|(
name|changed
condition|)
block|{
if|if
condition|(
name|match_missing
condition|)
block|{
operator|*
name|sha1p
operator|=
name|sha1
expr_stmt|;
operator|*
name|modep
operator|=
name|mode
expr_stmt|;
return|return
literal|0
return|;
block|}
return|return
operator|-
literal|1
return|;
block|}
name|changed
operator|=
name|match_stat_with_submodule
argument_list|(
name|diffopt
argument_list|,
name|ce
argument_list|,
operator|&
name|st
argument_list|,
literal|0
argument_list|,
name|dirty_submodule
argument_list|)
expr_stmt|;
if|if
condition|(
name|changed
condition|)
block|{
name|mode
operator|=
name|ce_mode_from_stat
argument_list|(
name|ce
argument_list|,
name|st
operator|.
name|st_mode
argument_list|)
expr_stmt|;
name|sha1
operator|=
name|null_sha1
expr_stmt|;
block|}
block|}
operator|*
name|sha1p
operator|=
name|sha1
expr_stmt|;
operator|*
name|modep
operator|=
name|mode
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|show_new_file
specifier|static
name|void
name|show_new_file
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
name|struct
name|cache_entry
modifier|*
name|new
parameter_list|,
name|int
name|cached
parameter_list|,
name|int
name|match_missing
parameter_list|)
block|{
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|unsigned
name|int
name|mode
decl_stmt|;
name|unsigned
name|dirty_submodule
init|=
literal|0
decl_stmt|;
comment|/* 	 * New file in the index: it might actually be different in 	 * the working tree. 	 */
if|if
condition|(
name|get_stat_data
argument_list|(
name|new
argument_list|,
operator|&
name|sha1
argument_list|,
operator|&
name|mode
argument_list|,
name|cached
argument_list|,
name|match_missing
argument_list|,
operator|&
name|dirty_submodule
argument_list|,
operator|&
name|revs
operator|->
name|diffopt
argument_list|)
operator|<
literal|0
condition|)
return|return;
name|diff_index_show_file
argument_list|(
name|revs
argument_list|,
literal|"+"
argument_list|,
name|new
argument_list|,
name|sha1
argument_list|,
operator|!
name|is_null_sha1
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|mode
argument_list|,
name|dirty_submodule
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_modified
specifier|static
name|int
name|show_modified
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
name|struct
name|cache_entry
modifier|*
name|old
parameter_list|,
specifier|const
name|struct
name|cache_entry
modifier|*
name|new
parameter_list|,
name|int
name|report_missing
parameter_list|,
name|int
name|cached
parameter_list|,
name|int
name|match_missing
parameter_list|)
block|{
name|unsigned
name|int
name|mode
decl_stmt|,
name|oldmode
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|unsigned
name|dirty_submodule
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|get_stat_data
argument_list|(
name|new
argument_list|,
operator|&
name|sha1
argument_list|,
operator|&
name|mode
argument_list|,
name|cached
argument_list|,
name|match_missing
argument_list|,
operator|&
name|dirty_submodule
argument_list|,
operator|&
name|revs
operator|->
name|diffopt
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|report_missing
condition|)
name|diff_index_show_file
argument_list|(
name|revs
argument_list|,
literal|"-"
argument_list|,
name|old
argument_list|,
name|old
operator|->
name|sha1
argument_list|,
literal|1
argument_list|,
name|old
operator|->
name|ce_mode
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|revs
operator|->
name|combine_merges
operator|&&
operator|!
name|cached
operator|&&
operator|(
name|hashcmp
argument_list|(
name|sha1
argument_list|,
name|old
operator|->
name|sha1
argument_list|)
operator|||
name|hashcmp
argument_list|(
name|old
operator|->
name|sha1
argument_list|,
name|new
operator|->
name|sha1
argument_list|)
operator|)
condition|)
block|{
name|struct
name|combine_diff_path
modifier|*
name|p
decl_stmt|;
name|int
name|pathlen
init|=
name|ce_namelen
argument_list|(
name|new
argument_list|)
decl_stmt|;
name|p
operator|=
name|xmalloc
argument_list|(
name|combine_diff_path_size
argument_list|(
literal|2
argument_list|,
name|pathlen
argument_list|)
argument_list|)
expr_stmt|;
name|p
operator|->
name|path
operator|=
operator|(
name|char
operator|*
operator|)
operator|&
name|p
operator|->
name|parent
index|[
literal|2
index|]
expr_stmt|;
name|p
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|memcpy
argument_list|(
name|p
operator|->
name|path
argument_list|,
name|new
operator|->
name|name
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
name|p
operator|->
name|path
index|[
name|pathlen
index|]
operator|=
literal|0
expr_stmt|;
name|p
operator|->
name|mode
operator|=
name|mode
expr_stmt|;
name|hashclr
argument_list|(
name|p
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|p
operator|->
name|parent
argument_list|,
literal|0
argument_list|,
literal|2
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|combine_diff_parent
argument_list|)
argument_list|)
expr_stmt|;
name|p
operator|->
name|parent
index|[
literal|0
index|]
operator|.
name|status
operator|=
name|DIFF_STATUS_MODIFIED
expr_stmt|;
name|p
operator|->
name|parent
index|[
literal|0
index|]
operator|.
name|mode
operator|=
name|new
operator|->
name|ce_mode
expr_stmt|;
name|hashcpy
argument_list|(
name|p
operator|->
name|parent
index|[
literal|0
index|]
operator|.
name|sha1
argument_list|,
name|new
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|p
operator|->
name|parent
index|[
literal|1
index|]
operator|.
name|status
operator|=
name|DIFF_STATUS_MODIFIED
expr_stmt|;
name|p
operator|->
name|parent
index|[
literal|1
index|]
operator|.
name|mode
operator|=
name|old
operator|->
name|ce_mode
expr_stmt|;
name|hashcpy
argument_list|(
name|p
operator|->
name|parent
index|[
literal|1
index|]
operator|.
name|sha1
argument_list|,
name|old
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|show_combined_diff
argument_list|(
name|p
argument_list|,
literal|2
argument_list|,
name|revs
operator|->
name|dense_combined_merges
argument_list|,
name|revs
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|p
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|oldmode
operator|=
name|old
operator|->
name|ce_mode
expr_stmt|;
if|if
condition|(
name|mode
operator|==
name|oldmode
operator|&&
operator|!
name|hashcmp
argument_list|(
name|sha1
argument_list|,
name|old
operator|->
name|sha1
argument_list|)
operator|&&
operator|!
name|dirty_submodule
operator|&&
operator|!
name|DIFF_OPT_TST
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
name|FIND_COPIES_HARDER
argument_list|)
condition|)
return|return
literal|0
return|;
name|diff_change
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
name|oldmode
argument_list|,
name|mode
argument_list|,
name|old
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|,
operator|!
name|is_null_sha1
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|old
operator|->
name|name
argument_list|,
literal|0
argument_list|,
name|dirty_submodule
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * This gets a mix of an existing index and a tree, one pathname entry  * at a time. The index entry may be a single stage-0 one, but it could  * also be multiple unmerged entries (in which case idx_pos/idx_nr will  * give you the position and number of entries in the index).  */
end_comment

begin_function
DECL|function|do_oneway_diff
specifier|static
name|void
name|do_oneway_diff
parameter_list|(
name|struct
name|unpack_trees_options
modifier|*
name|o
parameter_list|,
specifier|const
name|struct
name|cache_entry
modifier|*
name|idx
parameter_list|,
specifier|const
name|struct
name|cache_entry
modifier|*
name|tree
parameter_list|)
block|{
name|struct
name|rev_info
modifier|*
name|revs
init|=
name|o
operator|->
name|unpack_data
decl_stmt|;
name|int
name|match_missing
decl_stmt|,
name|cached
decl_stmt|;
comment|/* i-t-a entries do not actually exist in the index */
if|if
condition|(
name|idx
operator|&&
operator|(
name|idx
operator|->
name|ce_flags
operator|&
name|CE_INTENT_TO_ADD
operator|)
condition|)
block|{
name|idx
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
operator|!
name|tree
condition|)
return|return;
comment|/* nothing to diff.. */
block|}
comment|/* if the entry is not checked out, don't examine work tree */
name|cached
operator|=
name|o
operator|->
name|index_only
operator|||
operator|(
name|idx
operator|&&
operator|(
operator|(
name|idx
operator|->
name|ce_flags
operator|&
name|CE_VALID
operator|)
operator|||
name|ce_skip_worktree
argument_list|(
name|idx
argument_list|)
operator|)
operator|)
expr_stmt|;
comment|/* 	 * Backward compatibility wart - "diff-index -m" does 	 * not mean "do not ignore merges", but "match_missing". 	 * 	 * But with the revision flag parsing, that's found in 	 * "!revs->ignore_merges". 	 */
name|match_missing
operator|=
operator|!
name|revs
operator|->
name|ignore_merges
expr_stmt|;
if|if
condition|(
name|cached
operator|&&
name|idx
operator|&&
name|ce_stage
argument_list|(
name|idx
argument_list|)
condition|)
block|{
name|struct
name|diff_filepair
modifier|*
name|pair
decl_stmt|;
name|pair
operator|=
name|diff_unmerge
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
name|idx
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|tree
condition|)
name|fill_filespec
argument_list|(
name|pair
operator|->
name|one
argument_list|,
name|tree
operator|->
name|sha1
argument_list|,
literal|1
argument_list|,
name|tree
operator|->
name|ce_mode
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* 	 * Something added to the tree? 	 */
if|if
condition|(
operator|!
name|tree
condition|)
block|{
name|show_new_file
argument_list|(
name|revs
argument_list|,
name|idx
argument_list|,
name|cached
argument_list|,
name|match_missing
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* 	 * Something removed from the tree? 	 */
if|if
condition|(
operator|!
name|idx
condition|)
block|{
name|diff_index_show_file
argument_list|(
name|revs
argument_list|,
literal|"-"
argument_list|,
name|tree
argument_list|,
name|tree
operator|->
name|sha1
argument_list|,
literal|1
argument_list|,
name|tree
operator|->
name|ce_mode
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* Show difference between old and new */
name|show_modified
argument_list|(
name|revs
argument_list|,
name|tree
argument_list|,
name|idx
argument_list|,
literal|1
argument_list|,
name|cached
argument_list|,
name|match_missing
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * The unpack_trees() interface is designed for merging, so  * the different source entries are designed primarily for  * the source trees, with the old index being really mainly  * used for being replaced by the result.  *  * For diffing, the index is more important, and we only have a  * single tree.  *  * We're supposed to advance o->pos to skip what we have already processed.  *  * This wrapper makes it all more readable, and takes care of all  * the fairly complex unpack_trees() semantic requirements, including  * the skipping, the path matching, the type conflict cases etc.  */
end_comment

begin_function
DECL|function|oneway_diff
specifier|static
name|int
name|oneway_diff
parameter_list|(
specifier|const
name|struct
name|cache_entry
modifier|*
specifier|const
modifier|*
name|src
parameter_list|,
name|struct
name|unpack_trees_options
modifier|*
name|o
parameter_list|)
block|{
specifier|const
name|struct
name|cache_entry
modifier|*
name|idx
init|=
name|src
index|[
literal|0
index|]
decl_stmt|;
specifier|const
name|struct
name|cache_entry
modifier|*
name|tree
init|=
name|src
index|[
literal|1
index|]
decl_stmt|;
name|struct
name|rev_info
modifier|*
name|revs
init|=
name|o
operator|->
name|unpack_data
decl_stmt|;
comment|/* 	 * Unpack-trees generates a DF/conflict entry if 	 * there was a directory in the index and a tree 	 * in the tree. From a diff standpoint, that's a 	 * delete of the tree and a create of the file. 	 */
if|if
condition|(
name|tree
operator|==
name|o
operator|->
name|df_conflict_entry
condition|)
name|tree
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|ce_path_match
argument_list|(
name|idx
condition|?
name|idx
else|:
name|tree
argument_list|,
operator|&
name|revs
operator|->
name|prune_data
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
name|do_oneway_diff
argument_list|(
name|o
argument_list|,
name|idx
argument_list|,
name|tree
argument_list|)
expr_stmt|;
if|if
condition|(
name|diff_can_quit_early
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|)
condition|)
block|{
name|o
operator|->
name|exiting_early
operator|=
literal|1
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|diff_cache
specifier|static
name|int
name|diff_cache
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|tree_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|tree_name
parameter_list|,
name|int
name|cached
parameter_list|)
block|{
name|struct
name|tree
modifier|*
name|tree
decl_stmt|;
name|struct
name|tree_desc
name|t
decl_stmt|;
name|struct
name|unpack_trees_options
name|opts
decl_stmt|;
name|tree
operator|=
name|parse_tree_indirect
argument_list|(
name|tree_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree
condition|)
return|return
name|error
argument_list|(
literal|"bad tree object %s"
argument_list|,
name|tree_name
condition|?
name|tree_name
else|:
name|sha1_to_hex
argument_list|(
name|tree_sha1
argument_list|)
argument_list|)
return|;
name|memset
argument_list|(
operator|&
name|opts
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|opts
argument_list|)
argument_list|)
expr_stmt|;
name|opts
operator|.
name|head_idx
operator|=
literal|1
expr_stmt|;
name|opts
operator|.
name|index_only
operator|=
name|cached
expr_stmt|;
name|opts
operator|.
name|diff_index_cached
operator|=
operator|(
name|cached
operator|&&
operator|!
name|DIFF_OPT_TST
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
name|FIND_COPIES_HARDER
argument_list|)
operator|)
expr_stmt|;
name|opts
operator|.
name|merge
operator|=
literal|1
expr_stmt|;
name|opts
operator|.
name|fn
operator|=
name|oneway_diff
expr_stmt|;
name|opts
operator|.
name|unpack_data
operator|=
name|revs
expr_stmt|;
name|opts
operator|.
name|src_index
operator|=
operator|&
name|the_index
expr_stmt|;
name|opts
operator|.
name|dst_index
operator|=
name|NULL
expr_stmt|;
name|opts
operator|.
name|pathspec
operator|=
operator|&
name|revs
operator|->
name|diffopt
operator|.
name|pathspec
expr_stmt|;
name|opts
operator|.
name|pathspec
operator|->
name|recursive
operator|=
literal|1
expr_stmt|;
name|init_tree_desc
argument_list|(
operator|&
name|t
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
return|return
name|unpack_trees
argument_list|(
literal|1
argument_list|,
operator|&
name|t
argument_list|,
operator|&
name|opts
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|run_diff_index
name|int
name|run_diff_index
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|int
name|cached
parameter_list|)
block|{
name|struct
name|object_array_entry
modifier|*
name|ent
decl_stmt|;
name|ent
operator|=
name|revs
operator|->
name|pending
operator|.
name|objects
expr_stmt|;
if|if
condition|(
name|diff_cache
argument_list|(
name|revs
argument_list|,
name|ent
operator|->
name|item
operator|->
name|sha1
argument_list|,
name|ent
operator|->
name|name
argument_list|,
name|cached
argument_list|)
condition|)
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
name|diff_set_mnemonic_prefix
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|,
literal|"c/"
argument_list|,
name|cached
condition|?
literal|"i/"
else|:
literal|"w/"
argument_list|)
expr_stmt|;
name|diffcore_fix_diff_index
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|)
expr_stmt|;
name|diffcore_std
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|)
expr_stmt|;
name|diff_flush
argument_list|(
operator|&
name|revs
operator|->
name|diffopt
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|do_diff_cache
name|int
name|do_diff_cache
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|tree_sha1
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|)
block|{
name|struct
name|rev_info
name|revs
decl_stmt|;
name|init_revisions
argument_list|(
operator|&
name|revs
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|copy_pathspec
argument_list|(
operator|&
name|revs
operator|.
name|prune_data
argument_list|,
operator|&
name|opt
operator|->
name|pathspec
argument_list|)
expr_stmt|;
name|revs
operator|.
name|diffopt
operator|=
operator|*
name|opt
expr_stmt|;
if|if
condition|(
name|diff_cache
argument_list|(
operator|&
name|revs
argument_list|,
name|tree_sha1
argument_list|,
name|NULL
argument_list|,
literal|1
argument_list|)
condition|)
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|index_differs_from
name|int
name|index_differs_from
parameter_list|(
specifier|const
name|char
modifier|*
name|def
parameter_list|,
name|int
name|diff_flags
parameter_list|)
block|{
name|struct
name|rev_info
name|rev
decl_stmt|;
name|struct
name|setup_revision_opt
name|opt
decl_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|opt
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|opt
argument_list|)
argument_list|)
expr_stmt|;
name|opt
operator|.
name|def
operator|=
name|def
expr_stmt|;
name|setup_revisions
argument_list|(
literal|0
argument_list|,
name|NULL
argument_list|,
operator|&
name|rev
argument_list|,
operator|&
name|opt
argument_list|)
expr_stmt|;
name|DIFF_OPT_SET
argument_list|(
operator|&
name|rev
operator|.
name|diffopt
argument_list|,
name|QUICK
argument_list|)
expr_stmt|;
name|DIFF_OPT_SET
argument_list|(
operator|&
name|rev
operator|.
name|diffopt
argument_list|,
name|EXIT_WITH_STATUS
argument_list|)
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|flags
operator||=
name|diff_flags
expr_stmt|;
name|run_diff_index
argument_list|(
operator|&
name|rev
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|rev
operator|.
name|pending
operator|.
name|alloc
condition|)
name|free
argument_list|(
name|rev
operator|.
name|pending
operator|.
name|objects
argument_list|)
expr_stmt|;
return|return
operator|(
name|DIFF_OPT_TST
argument_list|(
operator|&
name|rev
operator|.
name|diffopt
argument_list|,
name|HAS_CHANGES
argument_list|)
operator|!=
literal|0
operator|)
return|;
block|}
end_function

end_unit

