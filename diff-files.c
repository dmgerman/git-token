begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
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

begin_decl_stmt
DECL|variable|diff_files_usage
specifier|static
specifier|const
name|char
name|diff_files_usage
index|[]
init|=
literal|"git-diff-files [-q] "
literal|"[<common diff options>] [<path>...]"
name|COMMON_DIFF_OPTIONS_HELP
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_options
specifier|static
name|struct
name|diff_options
name|diff_options
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|silent
specifier|static
name|int
name|silent
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|show_unmerge
specifier|static
name|void
name|show_unmerge
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|diff_unmerge
argument_list|(
operator|&
name|diff_options
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_file
specifier|static
name|void
name|show_file
parameter_list|(
name|int
name|pfx
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|)
block|{
name|diff_addremove
argument_list|(
operator|&
name|diff_options
argument_list|,
name|pfx
argument_list|,
name|ntohl
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
argument_list|,
name|ce
operator|->
name|sha1
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_modified
specifier|static
name|void
name|show_modified
parameter_list|(
name|int
name|oldmode
parameter_list|,
name|int
name|mode
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|char
modifier|*
name|path
parameter_list|)
block|{
name|diff_change
argument_list|(
operator|&
name|diff_options
argument_list|,
name|oldmode
argument_list|,
name|mode
argument_list|,
name|old_sha1
argument_list|,
name|sha1
argument_list|,
name|path
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
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
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
decl_stmt|;
specifier|const
name|char
modifier|*
name|prefix
init|=
name|setup_git_directory
argument_list|()
decl_stmt|;
name|int
name|entries
decl_stmt|,
name|i
decl_stmt|;
name|git_config
argument_list|(
name|git_diff_config
argument_list|)
expr_stmt|;
name|diff_setup
argument_list|(
operator|&
name|diff_options
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
operator|<
name|argc
operator|&&
name|argv
index|[
literal|1
index|]
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--"
argument_list|)
condition|)
block|{
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-q"
argument_list|)
condition|)
name|silent
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-r"
argument_list|)
condition|)
empty_stmt|;
comment|/* no-op */
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-s"
argument_list|)
condition|)
empty_stmt|;
comment|/* no-op */
else|else
block|{
name|int
name|diff_opt_cnt
decl_stmt|;
name|diff_opt_cnt
operator|=
name|diff_opt_parse
argument_list|(
operator|&
name|diff_options
argument_list|,
name|argv
operator|+
literal|1
argument_list|,
name|argc
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|diff_opt_cnt
operator|<
literal|0
condition|)
name|usage
argument_list|(
name|diff_files_usage
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|diff_opt_cnt
condition|)
block|{
name|argv
operator|+=
name|diff_opt_cnt
expr_stmt|;
name|argc
operator|-=
name|diff_opt_cnt
expr_stmt|;
continue|continue;
block|}
else|else
name|usage
argument_list|(
name|diff_files_usage
argument_list|)
expr_stmt|;
block|}
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
block|}
comment|/* Find the directory, and set up the pathspec */
name|pathspec
operator|=
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
operator|+
literal|1
argument_list|)
expr_stmt|;
name|entries
operator|=
name|read_cache
argument_list|()
expr_stmt|;
if|if
condition|(
name|diff_setup_done
argument_list|(
operator|&
name|diff_options
argument_list|)
operator|<
literal|0
condition|)
name|usage
argument_list|(
name|diff_files_usage
argument_list|)
expr_stmt|;
comment|/* At this point, if argc == 1, then we are doing everything. 	 * Otherwise argv[1] .. argv[argc-1] have the explicit paths. 	 */
if|if
condition|(
name|entries
operator|<
literal|0
condition|)
block|{
name|perror
argument_list|(
literal|"read_cache"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
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
name|entries
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
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
if|if
condition|(
operator|!
name|ce_path_match
argument_list|(
name|ce
argument_list|,
name|pathspec
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
name|show_unmerge
argument_list|(
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|entries
operator|&&
operator|!
name|strcmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|active_cache
index|[
name|i
index|]
operator|->
name|name
argument_list|)
condition|)
name|i
operator|++
expr_stmt|;
name|i
operator|--
expr_stmt|;
comment|/* compensate for loop control increments */
continue|continue;
block|}
if|if
condition|(
name|lstat
argument_list|(
name|ce
operator|->
name|name
argument_list|,
operator|&
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
if|if
condition|(
name|silent
condition|)
continue|continue;
name|show_file
argument_list|(
literal|'-'
argument_list|,
name|ce
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|changed
operator|=
name|ce_match_stat
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
operator|&&
operator|!
name|diff_options
operator|.
name|find_copies_harder
condition|)
continue|continue;
name|oldmode
operator|=
name|ntohl
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
expr_stmt|;
name|newmode
operator|=
name|DIFF_FILE_CANON_MODE
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
operator|&&
name|S_ISREG
argument_list|(
name|newmode
argument_list|)
operator|&&
name|S_ISREG
argument_list|(
name|oldmode
argument_list|)
operator|&&
operator|(
operator|(
name|newmode
operator|^
name|oldmode
operator|)
operator|==
literal|0111
operator|)
condition|)
name|newmode
operator|=
name|oldmode
expr_stmt|;
name|show_modified
argument_list|(
name|oldmode
argument_list|,
name|newmode
argument_list|,
name|ce
operator|->
name|sha1
argument_list|,
operator|(
name|changed
condition|?
name|null_sha1
else|:
name|ce
operator|->
name|sha1
operator|)
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
name|diffcore_std
argument_list|(
operator|&
name|diff_options
argument_list|)
expr_stmt|;
name|diff_flush
argument_list|(
operator|&
name|diff_options
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

