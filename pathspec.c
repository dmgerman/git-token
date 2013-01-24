begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"pathspec.h"
end_include

begin_comment
comment|/*  * Finds which of the given pathspecs match items in the index.  *  * For each pathspec, sets the corresponding entry in the seen[] array  * (which should be specs items long, i.e. the same size as pathspec)  * to the nature of the "closest" (i.e. most specific) match found for  * that pathspec in the index, if it was a closer type of match than  * the existing entry.  As an optimization, matching is skipped  * altogether if seen[] already only contains non-zero entries.  *  * If seen[] has not already been written to, it may make sense  * to use find_pathspecs_matching_against_index() instead.  */
end_comment

begin_function
DECL|function|add_pathspec_matches_against_index
name|void
name|add_pathspec_matches_against_index
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|,
name|char
modifier|*
name|seen
parameter_list|,
name|int
name|specs
parameter_list|)
block|{
name|int
name|num_unmatched
init|=
literal|0
decl_stmt|,
name|i
decl_stmt|;
comment|/* 	 * Since we are walking the index as if we were walking the directory, 	 * we have to mark the matched pathspec as seen; otherwise we will 	 * mistakenly think that the user gave a pathspec that did not match 	 * anything. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|specs
condition|;
name|i
operator|++
control|)
if|if
condition|(
operator|!
name|seen
index|[
name|i
index|]
condition|)
name|num_unmatched
operator|++
expr_stmt|;
if|if
condition|(
operator|!
name|num_unmatched
condition|)
return|return;
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
name|match_pathspec
argument_list|(
name|pathspec
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|ce_namelen
argument_list|(
name|ce
argument_list|)
argument_list|,
literal|0
argument_list|,
name|seen
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/*  * Finds which of the given pathspecs match items in the index.  *  * This is a one-shot wrapper around add_pathspec_matches_against_index()  * which allocates, populates, and returns a seen[] array indicating the  * nature of the "closest" (i.e. most specific) matches which each of the  * given pathspecs achieves against all items in the index.  */
end_comment

begin_function
DECL|function|find_pathspecs_matching_against_index
name|char
modifier|*
name|find_pathspecs_matching_against_index
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
block|{
name|char
modifier|*
name|seen
decl_stmt|;
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|pathspec
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
empty_stmt|;
comment|/* just counting */
name|seen
operator|=
name|xcalloc
argument_list|(
name|i
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|add_pathspec_matches_against_index
argument_list|(
name|pathspec
argument_list|,
name|seen
argument_list|,
name|i
argument_list|)
expr_stmt|;
return|return
name|seen
return|;
block|}
end_function

begin_comment
comment|/*  * Check the index to see whether path refers to a submodule, or  * something inside a submodule.  If the former, returns the path with  * any trailing slash stripped.  If the latter, dies with an error  * message.  */
end_comment

begin_function
DECL|function|check_path_for_gitlink
specifier|const
name|char
modifier|*
name|check_path_for_gitlink
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|path_len
init|=
name|strlen
argument_list|(
name|path
argument_list|)
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
name|S_ISGITLINK
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
condition|)
block|{
name|int
name|ce_len
init|=
name|ce_namelen
argument_list|(
name|ce
argument_list|)
decl_stmt|;
if|if
condition|(
name|path_len
operator|<=
name|ce_len
operator|||
name|path
index|[
name|ce_len
index|]
operator|!=
literal|'/'
operator|||
name|memcmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|path
argument_list|,
name|ce_len
argument_list|)
condition|)
comment|/* path does not refer to this 				 * submodule or anything inside it */
continue|continue;
if|if
condition|(
name|path_len
operator|==
name|ce_len
operator|+
literal|1
condition|)
block|{
comment|/* path refers to submodule; 				 * strip trailing slash */
return|return
name|xstrndup
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|ce_len
argument_list|)
return|;
block|}
else|else
block|{
name|die
argument_list|(
name|_
argument_list|(
literal|"Path '%s' is in submodule '%.*s'"
argument_list|)
argument_list|,
name|path
argument_list|,
name|ce_len
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
block|}
block|}
return|return
name|path
return|;
block|}
end_function

begin_comment
comment|/*  * Dies if the given path refers to a file inside a symlinked  * directory in the index.  */
end_comment

begin_function
DECL|function|die_if_path_beyond_symlink
name|void
name|die_if_path_beyond_symlink
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
if|if
condition|(
name|has_symlink_leading_path
argument_list|(
name|path
argument_list|,
name|strlen
argument_list|(
name|path
argument_list|)
argument_list|)
condition|)
block|{
name|int
name|len
init|=
name|prefix
condition|?
name|strlen
argument_list|(
name|prefix
argument_list|)
else|:
literal|0
decl_stmt|;
name|die
argument_list|(
name|_
argument_list|(
literal|"'%s' is beyond a symbolic link"
argument_list|)
argument_list|,
name|path
operator|+
name|len
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit

