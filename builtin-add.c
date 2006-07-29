begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * "git add" builtin command  *  * Copyright (C) 2006 Linus Torvalds  */
end_comment

begin_include
include|#
directive|include
file|<fnmatch.h>
end_include

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
DECL|variable|builtin_add_usage
specifier|static
specifier|const
name|char
name|builtin_add_usage
index|[]
init|=
literal|"git-add [-n] [-v]<filepattern>..."
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|prune_directory
specifier|static
name|void
name|prune_directory
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|,
name|int
name|prefix
parameter_list|)
block|{
name|char
modifier|*
name|seen
decl_stmt|;
name|int
name|i
decl_stmt|,
name|specs
decl_stmt|;
name|struct
name|dir_entry
modifier|*
modifier|*
name|src
decl_stmt|,
modifier|*
modifier|*
name|dst
decl_stmt|;
for|for
control|(
name|specs
operator|=
literal|0
init|;
name|pathspec
index|[
name|specs
index|]
condition|;
name|specs
operator|++
control|)
comment|/* nothing */
empty_stmt|;
name|seen
operator|=
name|xcalloc
argument_list|(
name|specs
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|src
operator|=
name|dst
operator|=
name|dir
operator|->
name|entries
expr_stmt|;
name|i
operator|=
name|dir
operator|->
name|nr
expr_stmt|;
while|while
condition|(
operator|--
name|i
operator|>=
literal|0
condition|)
block|{
name|struct
name|dir_entry
modifier|*
name|entry
init|=
operator|*
name|src
operator|++
decl_stmt|;
if|if
condition|(
operator|!
name|match_pathspec
argument_list|(
name|pathspec
argument_list|,
name|entry
operator|->
name|name
argument_list|,
name|entry
operator|->
name|len
argument_list|,
name|prefix
argument_list|,
name|seen
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|entry
argument_list|)
expr_stmt|;
continue|continue;
block|}
operator|*
name|dst
operator|++
operator|=
name|entry
expr_stmt|;
block|}
name|dir
operator|->
name|nr
operator|=
name|dst
operator|-
name|dir
operator|->
name|entries
expr_stmt|;
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
block|{
name|struct
name|stat
name|st
decl_stmt|;
specifier|const
name|char
modifier|*
name|match
decl_stmt|;
if|if
condition|(
name|seen
index|[
name|i
index|]
condition|)
continue|continue;
comment|/* Existing file? We must have ignored it */
name|match
operator|=
name|pathspec
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|match
index|[
literal|0
index|]
operator|||
operator|!
name|lstat
argument_list|(
name|match
argument_list|,
operator|&
name|st
argument_list|)
condition|)
continue|continue;
name|die
argument_list|(
literal|"pathspec '%s' did not match any files"
argument_list|,
name|match
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|fill_directory
specifier|static
name|void
name|fill_directory
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|path
decl_stmt|,
modifier|*
name|base
decl_stmt|;
name|int
name|baselen
decl_stmt|;
comment|/* Set up the default git porcelain excludes */
name|memset
argument_list|(
name|dir
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|dir
argument_list|)
argument_list|)
expr_stmt|;
name|dir
operator|->
name|exclude_per_dir
operator|=
literal|".gitignore"
expr_stmt|;
name|path
operator|=
name|git_path
argument_list|(
literal|"info/exclude"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|access
argument_list|(
name|path
argument_list|,
name|R_OK
argument_list|)
condition|)
name|add_excludes_from_file
argument_list|(
name|dir
argument_list|,
name|path
argument_list|)
expr_stmt|;
comment|/* 	 * Calculate common prefix for the pathspec, and 	 * use that to optimize the directory walk 	 */
name|baselen
operator|=
name|common_prefix
argument_list|(
name|pathspec
argument_list|)
expr_stmt|;
name|path
operator|=
literal|"."
expr_stmt|;
name|base
operator|=
literal|""
expr_stmt|;
if|if
condition|(
name|baselen
condition|)
block|{
name|char
modifier|*
name|common
init|=
name|xmalloc
argument_list|(
name|baselen
operator|+
literal|1
argument_list|)
decl_stmt|;
name|common
operator|=
name|xmalloc
argument_list|(
name|baselen
operator|+
literal|1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|common
argument_list|,
operator|*
name|pathspec
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
name|common
index|[
name|baselen
index|]
operator|=
literal|0
expr_stmt|;
name|path
operator|=
name|base
operator|=
name|common
expr_stmt|;
block|}
comment|/* Read the directory and prune it */
name|read_directory
argument_list|(
name|dir
argument_list|,
name|path
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
if|if
condition|(
name|pathspec
condition|)
name|prune_directory
argument_list|(
name|dir
argument_list|,
name|pathspec
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_file_to_index
specifier|static
name|int
name|add_file_to_index
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|verbose
parameter_list|)
block|{
name|int
name|size
decl_stmt|,
name|namelen
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|path
argument_list|,
operator|&
name|st
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s: unable to stat (%s)"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|S_ISREG
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
operator|&&
operator|!
name|S_ISLNK
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s: can only add regular files or symbolic links"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|namelen
operator|=
name|strlen
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|size
operator|=
name|cache_entry_size
argument_list|(
name|namelen
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
name|memcpy
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|path
argument_list|,
name|namelen
argument_list|)
expr_stmt|;
name|ce
operator|->
name|ce_flags
operator|=
name|htons
argument_list|(
name|namelen
argument_list|)
expr_stmt|;
name|fill_stat_cache_info
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
name|ce
operator|->
name|ce_mode
operator|=
name|create_ce_mode
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|trust_executable_bit
condition|)
block|{
comment|/* If there is an existing entry, pick the mode bits 		 * from it. 		 */
name|int
name|pos
init|=
name|cache_name_pos
argument_list|(
name|path
argument_list|,
name|namelen
argument_list|)
decl_stmt|;
if|if
condition|(
name|pos
operator|>=
literal|0
condition|)
name|ce
operator|->
name|ce_mode
operator|=
name|active_cache
index|[
name|pos
index|]
operator|->
name|ce_mode
expr_stmt|;
block|}
if|if
condition|(
name|index_path
argument_list|(
name|ce
operator|->
name|sha1
argument_list|,
name|path
argument_list|,
operator|&
name|st
argument_list|,
literal|1
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to index file %s"
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|add_cache_entry
argument_list|(
name|ce
argument_list|,
name|ADD_CACHE_OK_TO_ADD
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to add %s to index"
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"add '%s'\n"
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
return|return
literal|0
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
DECL|function|cmd_add
name|int
name|cmd_add
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
specifier|const
name|char
modifier|*
name|prefix
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
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
decl_stmt|;
name|struct
name|dir_struct
name|dir
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
name|arg
index|[
literal|0
index|]
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
name|die
argument_list|(
name|builtin_add_usage
argument_list|)
expr_stmt|;
block|}
name|git_config
argument_list|(
name|git_default_config
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
name|fill_directory
argument_list|(
operator|&
name|dir
argument_list|,
name|pathspec
argument_list|)
expr_stmt|;
if|if
condition|(
name|show_only
condition|)
block|{
specifier|const
name|char
modifier|*
name|sep
init|=
literal|""
decl_stmt|,
modifier|*
name|eof
init|=
literal|""
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|dir
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|printf
argument_list|(
literal|"%s%s"
argument_list|,
name|sep
argument_list|,
name|dir
operator|.
name|entries
index|[
name|i
index|]
operator|->
name|name
argument_list|)
expr_stmt|;
name|sep
operator|=
literal|" "
expr_stmt|;
name|eof
operator|=
literal|"\n"
expr_stmt|;
block|}
name|fputs
argument_list|(
name|eof
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|dir
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|add_file_to_index
argument_list|(
name|dir
operator|.
name|entries
index|[
name|i
index|]
operator|->
name|name
argument_list|,
name|verbose
argument_list|)
expr_stmt|;
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

