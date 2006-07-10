begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * "git rm" builtin command  *  * Copyright (C) Linus Torvalds 2006  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"cache-tree.h"
end_include

begin_decl_stmt
DECL|variable|builtin_rm_usage
specifier|static
specifier|const
name|char
name|builtin_rm_usage
index|[]
init|=
literal|"git-rm [-n] [-v] [-f]<filepattern>..."
decl_stmt|;
end_decl_stmt

begin_struct
specifier|static
struct|struct
block|{
DECL|member|nr
DECL|member|alloc
name|int
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
DECL|member|name
specifier|const
name|char
modifier|*
modifier|*
name|name
decl_stmt|;
DECL|variable|list
block|}
name|list
struct|;
end_struct

begin_function
DECL|function|add_list
specifier|static
name|void
name|add_list
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
if|if
condition|(
name|list
operator|.
name|nr
operator|>=
name|list
operator|.
name|alloc
condition|)
block|{
name|list
operator|.
name|alloc
operator|=
name|alloc_nr
argument_list|(
name|list
operator|.
name|alloc
argument_list|)
expr_stmt|;
name|list
operator|.
name|name
operator|=
name|xrealloc
argument_list|(
name|list
operator|.
name|name
argument_list|,
name|list
operator|.
name|alloc
operator|*
sizeof|sizeof
argument_list|(
specifier|const
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|list
operator|.
name|name
index|[
name|list
operator|.
name|nr
operator|++
index|]
operator|=
name|name
expr_stmt|;
block|}
end_function

begin_function
DECL|function|remove_file
specifier|static
name|int
name|remove_file
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|int
name|ret
decl_stmt|;
name|char
modifier|*
name|slash
decl_stmt|;
name|ret
operator|=
name|unlink
argument_list|(
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
operator|(
name|slash
operator|=
name|strrchr
argument_list|(
name|name
argument_list|,
literal|'/'
argument_list|)
operator|)
condition|)
block|{
name|char
modifier|*
name|n
init|=
name|strdup
argument_list|(
name|name
argument_list|)
decl_stmt|;
do|do
block|{
name|n
index|[
name|slash
operator|-
name|name
index|]
operator|=
literal|0
expr_stmt|;
name|name
operator|=
name|n
expr_stmt|;
block|}
do|while
condition|(
operator|!
name|rmdir
argument_list|(
name|name
argument_list|)
operator|&&
operator|(
name|slash
operator|=
name|strrchr
argument_list|(
name|name
argument_list|,
literal|'/'
argument_list|)
operator|)
condition|)
do|;
block|}
return|return
name|ret
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|lock_file
specifier|static
name|struct
name|lock_file
name|lock_file
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_rm
name|int
name|cmd_rm
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|char
modifier|*
modifier|*
name|envp
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|newfd
decl_stmt|;
name|int
name|verbose
init|=
literal|0
decl_stmt|,
name|show_only
init|=
literal|0
decl_stmt|,
name|force
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|prefix
init|=
name|setup_git_directory
argument_list|()
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
decl_stmt|;
name|char
modifier|*
name|seen
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
name|newfd
operator|=
name|hold_lock_file_for_update
argument_list|(
operator|&
name|lock_file
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
literal|"unable to create new index file"
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"index file corrupt"
argument_list|)
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
if|if
condition|(
operator|*
name|arg
operator|!=
literal|'-'
condition|)
break|break;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--"
argument_list|)
condition|)
block|{
name|i
operator|++
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-n"
argument_list|)
condition|)
block|{
name|show_only
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-v"
argument_list|)
condition|)
block|{
name|verbose
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-f"
argument_list|)
condition|)
block|{
name|force
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|die
argument_list|(
name|builtin_rm_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|<=
name|i
condition|)
name|usage
argument_list|(
name|builtin_rm_usage
argument_list|)
expr_stmt|;
name|pathspec
operator|=
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
operator|+
name|i
argument_list|)
expr_stmt|;
name|seen
operator|=
name|NULL
expr_stmt|;
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
comment|/* nothing */
empty_stmt|;
name|seen
operator|=
name|xmalloc
argument_list|(
name|i
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|seen
argument_list|,
literal|0
argument_list|,
name|i
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
operator|!
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
condition|)
continue|continue;
name|add_list
argument_list|(
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|pathspec
condition|)
block|{
specifier|const
name|char
modifier|*
name|match
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
operator|(
name|match
operator|=
name|pathspec
index|[
name|i
index|]
operator|)
operator|!=
name|NULL
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|match
operator|&&
operator|!
name|seen
index|[
name|i
index|]
condition|)
name|die
argument_list|(
literal|"pathspec '%s' did not match any files"
argument_list|,
name|match
argument_list|)
expr_stmt|;
block|}
block|}
comment|/* 	 * First remove the names from the index: we won't commit 	 * the index unless all of them succeed 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|list
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|list
operator|.
name|name
index|[
name|i
index|]
decl_stmt|;
name|printf
argument_list|(
literal|"rm '%s'\n"
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|remove_file_from_cache
argument_list|(
name|path
argument_list|)
condition|)
name|die
argument_list|(
literal|"git rm: unable to remove %s"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|cache_tree_invalidate_path
argument_list|(
name|active_cache_tree
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|show_only
condition|)
return|return
literal|0
return|;
comment|/* 	 * Then, if we used "-f", remove the filenames from the 	 * workspace. If we fail to remove the first one, we 	 * abort the "git rm" (but once we've successfully removed 	 * any file at all, we'll go ahead and commit to it all: 	 * by then we've already committed ourselves and can't fail 	 * in the middle) 	 */
if|if
condition|(
name|force
condition|)
block|{
name|int
name|removed
init|=
literal|0
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|list
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|list
operator|.
name|name
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|remove_file
argument_list|(
name|path
argument_list|)
condition|)
block|{
name|removed
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|removed
condition|)
name|die
argument_list|(
literal|"git rm: %s: %s"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|active_cache_changed
condition|)
block|{
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
name|close
argument_list|(
name|newfd
argument_list|)
operator|||
name|commit_lock_file
argument_list|(
operator|&
name|lock_file
argument_list|)
condition|)
name|die
argument_list|(
literal|"Unable to write new index file"
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

