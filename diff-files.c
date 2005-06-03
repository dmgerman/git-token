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
modifier|*
name|diff_files_usage
init|=
literal|"git-diff-files [-p] [-q] [-r] [-z] [-M] [-C] [-R] [-S<string>] [-O<orderfile>] [paths...]"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_output_format
specifier|static
name|int
name|diff_output_format
init|=
name|DIFF_FORMAT_HUMAN
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|detect_rename
specifier|static
name|int
name|detect_rename
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_setup_opt
specifier|static
name|int
name|diff_setup_opt
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_score_opt
specifier|static
name|int
name|diff_score_opt
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pickaxe
specifier|static
specifier|const
name|char
modifier|*
name|pickaxe
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pickaxe_opts
specifier|static
name|int
name|pickaxe_opts
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_break_opt
specifier|static
name|int
name|diff_break_opt
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|orderfile
specifier|static
specifier|const
name|char
modifier|*
name|orderfile
init|=
name|NULL
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
specifier|static
specifier|const
name|unsigned
name|char
name|null_sha1
index|[
literal|20
index|]
init|=
block|{
literal|0
block|, }
decl_stmt|;
name|int
name|entries
init|=
name|read_cache
argument_list|()
decl_stmt|;
name|int
name|i
decl_stmt|;
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
literal|"-p"
argument_list|)
condition|)
name|diff_output_format
operator|=
name|DIFF_FORMAT_PATCH
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
literal|"-z"
argument_list|)
condition|)
name|diff_output_format
operator|=
name|DIFF_FORMAT_MACHINE
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
literal|"-R"
argument_list|)
condition|)
name|diff_setup_opt
operator||=
name|DIFF_SETUP_REVERSE
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-S"
argument_list|,
literal|2
argument_list|)
condition|)
name|pickaxe
operator|=
name|argv
index|[
literal|1
index|]
operator|+
literal|2
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-O"
argument_list|,
literal|2
argument_list|)
condition|)
name|orderfile
operator|=
name|argv
index|[
literal|1
index|]
operator|+
literal|2
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
literal|"--pickaxe-all"
argument_list|)
condition|)
name|pickaxe_opts
operator|=
name|DIFF_PICKAXE_ALL
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-B"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
if|if
condition|(
operator|(
name|diff_break_opt
operator|=
name|diff_scoreopt_parse
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
operator|)
operator|==
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|diff_files_usage
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-M"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
if|if
condition|(
operator|(
name|diff_score_opt
operator|=
name|diff_scoreopt_parse
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
operator|)
operator|==
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|diff_files_usage
argument_list|)
expr_stmt|;
name|detect_rename
operator|=
name|DIFF_DETECT_RENAME
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-C"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
if|if
condition|(
operator|(
name|diff_score_opt
operator|=
name|diff_scoreopt_parse
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
operator|)
operator|==
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|diff_files_usage
argument_list|)
expr_stmt|;
name|detect_rename
operator|=
name|DIFF_DETECT_COPY
expr_stmt|;
block|}
else|else
name|usage
argument_list|(
name|diff_files_usage
argument_list|)
expr_stmt|;
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
block|}
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
name|diff_setup
argument_list|(
name|diff_setup_opt
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
name|detect_rename
operator|<
name|DIFF_DETECT_COPY
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
name|show_modified
argument_list|(
name|oldmode
argument_list|,
name|DIFF_FILE_CANON_MODE
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
argument_list|,
name|ce
operator|->
name|sha1
argument_list|,
name|null_sha1
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
name|diffcore_std
argument_list|(
operator|(
literal|1
operator|<
name|argc
operator|)
condition|?
name|argv
operator|+
literal|1
else|:
name|NULL
argument_list|,
name|detect_rename
argument_list|,
name|diff_score_opt
argument_list|,
name|pickaxe
argument_list|,
name|pickaxe_opts
argument_list|,
name|diff_break_opt
argument_list|,
name|orderfile
argument_list|)
expr_stmt|;
name|diff_flush
argument_list|(
name|diff_output_format
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

