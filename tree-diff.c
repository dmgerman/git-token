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

begin_include
include|#
directive|include
file|"diffcore.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_function_decl
specifier|static
name|void
name|show_path
parameter_list|(
name|struct
name|strbuf
modifier|*
name|base
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t1
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t2
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
name|tree_entry_len
argument_list|(
operator|&
name|t1
operator|->
name|entry
argument_list|)
expr_stmt|;
name|pathlen2
operator|=
name|tree_entry_len
argument_list|(
operator|&
name|t2
operator|->
name|entry
argument_list|)
expr_stmt|;
comment|/* 	 * NOTE files and directories *always* compare differently, 	 * even when having the same name. 	 */
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
return|return
name|cmp
return|;
block|}
end_function

begin_comment
comment|/* convert path, t1/t2 -> opt->diff_*() callbacks */
end_comment

begin_function
DECL|function|emit_diff
specifier|static
name|void
name|emit_diff
parameter_list|(
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|,
name|struct
name|strbuf
modifier|*
name|path
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t1
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t2
parameter_list|)
block|{
name|unsigned
name|int
name|mode1
init|=
name|t1
condition|?
name|t1
operator|->
name|entry
operator|.
name|mode
else|:
literal|0
decl_stmt|;
name|unsigned
name|int
name|mode2
init|=
name|t2
condition|?
name|t2
operator|->
name|entry
operator|.
name|mode
else|:
literal|0
decl_stmt|;
if|if
condition|(
name|mode1
operator|&&
name|mode2
condition|)
block|{
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
name|t1
operator|->
name|entry
operator|.
name|sha1
argument_list|,
name|t2
operator|->
name|entry
operator|.
name|sha1
argument_list|,
literal|1
argument_list|,
literal|1
argument_list|,
name|path
operator|->
name|buf
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
else|else
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
name|int
name|addremove
decl_stmt|;
if|if
condition|(
name|mode2
condition|)
block|{
name|addremove
operator|=
literal|'+'
expr_stmt|;
name|sha1
operator|=
name|t2
operator|->
name|entry
operator|.
name|sha1
expr_stmt|;
name|mode
operator|=
name|mode2
expr_stmt|;
block|}
else|else
block|{
name|addremove
operator|=
literal|'-'
expr_stmt|;
name|sha1
operator|=
name|t1
operator|->
name|entry
operator|.
name|sha1
expr_stmt|;
name|mode
operator|=
name|mode1
expr_stmt|;
block|}
name|opt
operator|->
name|add_remove
argument_list|(
name|opt
argument_list|,
name|addremove
argument_list|,
name|mode
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|,
name|path
operator|->
name|buf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/* new path should be added to diff  *  * 3 cases on how/when it should be called and behaves:  *  *	!t1,  t2	-> path added, parent lacks it  *	 t1, !t2	-> path removed from parent  *	 t1,  t2	-> path modified  */
end_comment

begin_function
DECL|function|show_path
specifier|static
name|void
name|show_path
parameter_list|(
name|struct
name|strbuf
modifier|*
name|base
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t1
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t2
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
name|int
name|pathlen
decl_stmt|;
name|int
name|old_baselen
init|=
name|base
operator|->
name|len
decl_stmt|;
name|int
name|isdir
decl_stmt|,
name|recurse
init|=
literal|0
decl_stmt|,
name|emitthis
init|=
literal|1
decl_stmt|;
comment|/* at least something has to be valid */
name|assert
argument_list|(
name|t1
operator|||
name|t2
argument_list|)
expr_stmt|;
if|if
condition|(
name|t2
condition|)
block|{
comment|/* path present in resulting tree */
name|tree_entry_extract
argument_list|(
name|t2
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
name|tree_entry_len
argument_list|(
operator|&
name|t2
operator|->
name|entry
argument_list|)
expr_stmt|;
name|isdir
operator|=
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/* 		 * a path was removed - take path from parent. Also take 		 * mode from parent, to decide on recursion. 		 */
name|tree_entry_extract
argument_list|(
name|t1
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
name|tree_entry_len
argument_list|(
operator|&
name|t1
operator|->
name|entry
argument_list|)
expr_stmt|;
name|isdir
operator|=
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
expr_stmt|;
name|mode
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|DIFF_OPT_TST
argument_list|(
name|opt
argument_list|,
name|RECURSIVE
argument_list|)
operator|&&
name|isdir
condition|)
block|{
name|recurse
operator|=
literal|1
expr_stmt|;
name|emitthis
operator|=
name|DIFF_OPT_TST
argument_list|(
name|opt
argument_list|,
name|TREE_IN_RECURSIVE
argument_list|)
expr_stmt|;
block|}
name|strbuf_add
argument_list|(
name|base
argument_list|,
name|path
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
if|if
condition|(
name|emitthis
condition|)
name|emit_diff
argument_list|(
name|opt
argument_list|,
name|base
argument_list|,
name|t1
argument_list|,
name|t2
argument_list|)
expr_stmt|;
if|if
condition|(
name|recurse
condition|)
block|{
name|strbuf_addch
argument_list|(
name|base
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
name|diff_tree_sha1
argument_list|(
name|t1
condition|?
name|t1
operator|->
name|entry
operator|.
name|sha1
else|:
name|NULL
argument_list|,
name|t2
condition|?
name|t2
operator|->
name|entry
operator|.
name|sha1
else|:
name|NULL
argument_list|,
name|base
operator|->
name|buf
argument_list|,
name|opt
argument_list|)
expr_stmt|;
block|}
name|strbuf_setlen
argument_list|(
name|base
argument_list|,
name|old_baselen
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|skip_uninteresting
specifier|static
name|void
name|skip_uninteresting
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|t
parameter_list|,
name|struct
name|strbuf
modifier|*
name|base
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|)
block|{
name|enum
name|interesting
name|match
decl_stmt|;
while|while
condition|(
name|t
operator|->
name|size
condition|)
block|{
name|match
operator|=
name|tree_entry_interesting
argument_list|(
operator|&
name|t
operator|->
name|entry
argument_list|,
name|base
argument_list|,
literal|0
argument_list|,
operator|&
name|opt
operator|->
name|pathspec
argument_list|)
expr_stmt|;
if|if
condition|(
name|match
condition|)
block|{
if|if
condition|(
name|match
operator|==
name|all_entries_not_interesting
condition|)
name|t
operator|->
name|size
operator|=
literal|0
expr_stmt|;
break|break;
block|}
name|update_tree_entry
argument_list|(
name|t
argument_list|)
expr_stmt|;
block|}
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
name|base_str
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|)
block|{
name|struct
name|strbuf
name|base
decl_stmt|;
name|int
name|baselen
init|=
name|strlen
argument_list|(
name|base_str
argument_list|)
decl_stmt|;
comment|/* Enable recursion indefinitely */
name|opt
operator|->
name|pathspec
operator|.
name|recursive
operator|=
name|DIFF_OPT_TST
argument_list|(
name|opt
argument_list|,
name|RECURSIVE
argument_list|)
expr_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|base
argument_list|,
name|PATH_MAX
argument_list|)
expr_stmt|;
name|strbuf_add
argument_list|(
operator|&
name|base
argument_list|,
name|base_str
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|cmp
decl_stmt|;
if|if
condition|(
name|diff_can_quit_early
argument_list|(
name|opt
argument_list|)
condition|)
break|break;
if|if
condition|(
name|opt
operator|->
name|pathspec
operator|.
name|nr
condition|)
block|{
name|skip_uninteresting
argument_list|(
name|t1
argument_list|,
operator|&
name|base
argument_list|,
name|opt
argument_list|)
expr_stmt|;
name|skip_uninteresting
argument_list|(
name|t2
argument_list|,
operator|&
name|base
argument_list|,
name|opt
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|t1
operator|->
name|size
condition|)
block|{
if|if
condition|(
operator|!
name|t2
operator|->
name|size
condition|)
break|break;
name|show_path
argument_list|(
operator|&
name|base
argument_list|,
name|opt
argument_list|,
comment|/*t1=*/
name|NULL
argument_list|,
name|t2
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
name|show_path
argument_list|(
operator|&
name|base
argument_list|,
name|opt
argument_list|,
name|t1
argument_list|,
comment|/*t2=*/
name|NULL
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
name|t1
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|cmp
operator|=
name|compare_tree_entry
argument_list|(
name|t1
argument_list|,
name|t2
argument_list|)
expr_stmt|;
comment|/* t1 = t2 */
if|if
condition|(
name|cmp
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|DIFF_OPT_TST
argument_list|(
name|opt
argument_list|,
name|FIND_COPIES_HARDER
argument_list|)
operator|||
name|hashcmp
argument_list|(
name|t1
operator|->
name|entry
operator|.
name|sha1
argument_list|,
name|t2
operator|->
name|entry
operator|.
name|sha1
argument_list|)
operator|||
operator|(
name|t1
operator|->
name|entry
operator|.
name|mode
operator|!=
name|t2
operator|->
name|entry
operator|.
name|mode
operator|)
condition|)
name|show_path
argument_list|(
operator|&
name|base
argument_list|,
name|opt
argument_list|,
name|t1
argument_list|,
name|t2
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
name|t1
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
name|t2
argument_list|)
expr_stmt|;
block|}
comment|/* t1< t2 */
elseif|else
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
block|{
name|show_path
argument_list|(
operator|&
name|base
argument_list|,
name|opt
argument_list|,
name|t1
argument_list|,
comment|/*t2=*/
name|NULL
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
name|t1
argument_list|)
expr_stmt|;
block|}
comment|/* t1> t2 */
else|else
block|{
name|show_path
argument_list|(
operator|&
name|base
argument_list|,
name|opt
argument_list|,
comment|/*t1=*/
name|NULL
argument_list|,
name|t2
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
name|t2
argument_list|)
expr_stmt|;
block|}
block|}
name|strbuf_release
argument_list|(
operator|&
name|base
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * Does it look like the resulting diff might be due to a rename?  *  - single entry  *  - not a valid previous file  */
end_comment

begin_function
DECL|function|diff_might_be_rename
specifier|static
specifier|inline
name|int
name|diff_might_be_rename
parameter_list|(
name|void
parameter_list|)
block|{
return|return
name|diff_queued_diff
operator|.
name|nr
operator|==
literal|1
operator|&&
operator|!
name|DIFF_FILE_VALID
argument_list|(
name|diff_queued_diff
operator|.
name|queue
index|[
literal|0
index|]
operator|->
name|one
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|try_to_follow_renames
specifier|static
name|void
name|try_to_follow_renames
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
name|struct
name|diff_options
name|diff_opts
decl_stmt|;
name|struct
name|diff_queue_struct
modifier|*
name|q
init|=
operator|&
name|diff_queued_diff
decl_stmt|;
name|struct
name|diff_filepair
modifier|*
name|choice
decl_stmt|;
name|int
name|i
decl_stmt|;
comment|/* 	 * follow-rename code is very specific, we need exactly one 	 * path. Magic that matches more than one path is not 	 * supported. 	 */
name|GUARD_PATHSPEC
argument_list|(
operator|&
name|opt
operator|->
name|pathspec
argument_list|,
name|PATHSPEC_FROMTOP
operator||
name|PATHSPEC_LITERAL
argument_list|)
expr_stmt|;
if|#
directive|if
literal|0
comment|/* 	 * We should reject wildcards as well. Unfortunately we 	 * haven't got a reliable way to detect that 'foo\*bar' in 	 * fact has no wildcards. nowildcard_len is merely a hint for 	 * optimization. Let it slip for now until wildmatch is taught 	 * about dry-run mode and returns wildcard info. 	 */
block|if (opt->pathspec.has_wildcard) 		die("BUG:%s:%d: wildcards are not supported", 		    __FILE__, __LINE__);
endif|#
directive|endif
comment|/* Remove the file creation entry from the diff queue, and remember it */
name|choice
operator|=
name|q
operator|->
name|queue
index|[
literal|0
index|]
expr_stmt|;
name|q
operator|->
name|nr
operator|=
literal|0
expr_stmt|;
name|diff_setup
argument_list|(
operator|&
name|diff_opts
argument_list|)
expr_stmt|;
name|DIFF_OPT_SET
argument_list|(
operator|&
name|diff_opts
argument_list|,
name|RECURSIVE
argument_list|)
expr_stmt|;
name|DIFF_OPT_SET
argument_list|(
operator|&
name|diff_opts
argument_list|,
name|FIND_COPIES_HARDER
argument_list|)
expr_stmt|;
name|diff_opts
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_NO_OUTPUT
expr_stmt|;
name|diff_opts
operator|.
name|single_follow
operator|=
name|opt
operator|->
name|pathspec
operator|.
name|items
index|[
literal|0
index|]
operator|.
name|match
expr_stmt|;
name|diff_opts
operator|.
name|break_opt
operator|=
name|opt
operator|->
name|break_opt
expr_stmt|;
name|diff_opts
operator|.
name|rename_score
operator|=
name|opt
operator|->
name|rename_score
expr_stmt|;
name|diff_setup_done
argument_list|(
operator|&
name|diff_opts
argument_list|)
expr_stmt|;
name|diff_tree
argument_list|(
name|t1
argument_list|,
name|t2
argument_list|,
name|base
argument_list|,
operator|&
name|diff_opts
argument_list|)
expr_stmt|;
name|diffcore_std
argument_list|(
operator|&
name|diff_opts
argument_list|)
expr_stmt|;
name|free_pathspec
argument_list|(
operator|&
name|diff_opts
operator|.
name|pathspec
argument_list|)
expr_stmt|;
comment|/* Go through the new set of filepairing, and see if we find a more interesting one */
name|opt
operator|->
name|found_follow
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
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|diff_filepair
modifier|*
name|p
init|=
name|q
operator|->
name|queue
index|[
name|i
index|]
decl_stmt|;
comment|/* 		 * Found a source? Not only do we use that for the new 		 * diff_queued_diff, we will also use that as the path in 		 * the future! 		 */
if|if
condition|(
operator|(
name|p
operator|->
name|status
operator|==
literal|'R'
operator|||
name|p
operator|->
name|status
operator|==
literal|'C'
operator|)
operator|&&
operator|!
name|strcmp
argument_list|(
name|p
operator|->
name|two
operator|->
name|path
argument_list|,
name|opt
operator|->
name|pathspec
operator|.
name|items
index|[
literal|0
index|]
operator|.
name|match
argument_list|)
condition|)
block|{
specifier|const
name|char
modifier|*
name|path
index|[
literal|2
index|]
decl_stmt|;
comment|/* Switch the file-pairs around */
name|q
operator|->
name|queue
index|[
name|i
index|]
operator|=
name|choice
expr_stmt|;
name|choice
operator|=
name|p
expr_stmt|;
comment|/* Update the path we use from now on.. */
name|path
index|[
literal|0
index|]
operator|=
name|p
operator|->
name|one
operator|->
name|path
expr_stmt|;
name|path
index|[
literal|1
index|]
operator|=
name|NULL
expr_stmt|;
name|free_pathspec
argument_list|(
operator|&
name|opt
operator|->
name|pathspec
argument_list|)
expr_stmt|;
name|parse_pathspec
argument_list|(
operator|&
name|opt
operator|->
name|pathspec
argument_list|,
name|PATHSPEC_ALL_MAGIC
operator|&
operator|~
name|PATHSPEC_LITERAL
argument_list|,
name|PATHSPEC_LITERAL_PATH
argument_list|,
literal|""
argument_list|,
name|path
argument_list|)
expr_stmt|;
comment|/* 			 * The caller expects us to return a set of vanilla 			 * filepairs to let a later call to diffcore_std() 			 * it makes to sort the renames out (among other 			 * things), but we already have found renames 			 * ourselves; signal diffcore_std() not to muck with 			 * rename information. 			 */
name|opt
operator|->
name|found_follow
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
comment|/* 	 * Then, discard all the non-relevant file pairs... 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|diff_filepair
modifier|*
name|p
init|=
name|q
operator|->
name|queue
index|[
name|i
index|]
decl_stmt|;
name|diff_free_filepair
argument_list|(
name|p
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * .. and re-instate the one we want (which might be either the 	 * original one, or the rename/copy we found) 	 */
name|q
operator|->
name|queue
index|[
literal|0
index|]
operator|=
name|choice
expr_stmt|;
name|q
operator|->
name|nr
operator|=
literal|1
expr_stmt|;
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
name|unsigned
name|long
name|size1
decl_stmt|,
name|size2
decl_stmt|;
name|int
name|retval
decl_stmt|;
name|tree1
operator|=
name|fill_tree_descriptor
argument_list|(
operator|&
name|t1
argument_list|,
name|old
argument_list|)
expr_stmt|;
name|tree2
operator|=
name|fill_tree_descriptor
argument_list|(
operator|&
name|t2
argument_list|,
name|new
argument_list|)
expr_stmt|;
name|size1
operator|=
name|t1
operator|.
name|size
expr_stmt|;
name|size2
operator|=
name|t2
operator|.
name|size
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
if|if
condition|(
operator|!
operator|*
name|base
operator|&&
name|DIFF_OPT_TST
argument_list|(
name|opt
argument_list|,
name|FOLLOW_RENAMES
argument_list|)
operator|&&
name|diff_might_be_rename
argument_list|()
condition|)
block|{
name|init_tree_desc
argument_list|(
operator|&
name|t1
argument_list|,
name|tree1
argument_list|,
name|size1
argument_list|)
expr_stmt|;
name|init_tree_desc
argument_list|(
operator|&
name|t2
argument_list|,
name|tree2
argument_list|,
name|size2
argument_list|)
expr_stmt|;
name|try_to_follow_renames
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
block|}
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
DECL|function|diff_root_tree_sha1
name|int
name|diff_root_tree_sha1
parameter_list|(
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
return|return
name|diff_tree_sha1
argument_list|(
name|NULL
argument_list|,
name|new
argument_list|,
name|base
argument_list|,
name|opt
argument_list|)
return|;
block|}
end_function

end_unit

