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
DECL|variable|show_diff_usage
specifier|static
specifier|const
name|char
modifier|*
name|show_diff_usage
init|=
literal|"show-diff [-q] [-s] [-z] [paths...]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|matches_pathspec
specifier|static
name|int
name|matches_pathspec
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|char
modifier|*
modifier|*
name|spec
parameter_list|,
name|int
name|cnt
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|namelen
init|=
name|ce_namelen
argument_list|(
name|ce
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
name|cnt
condition|;
name|i
operator|++
control|)
block|{
name|int
name|speclen
init|=
name|strlen
argument_list|(
name|spec
index|[
name|i
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|spec
index|[
name|i
index|]
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|speclen
argument_list|)
operator|&&
name|speclen
operator|<=
name|namelen
operator|&&
operator|(
name|ce
operator|->
name|name
index|[
name|speclen
index|]
operator|==
literal|0
operator|||
name|ce
operator|->
name|name
index|[
name|speclen
index|]
operator|==
literal|'/'
operator|)
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
name|silent
init|=
literal|0
decl_stmt|;
name|int
name|silent_on_nonexisting_files
init|=
literal|0
decl_stmt|;
name|int
name|machine_readable
init|=
literal|0
decl_stmt|;
name|int
name|reverse
init|=
literal|0
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
literal|"-R"
argument_list|)
condition|)
name|reverse
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
literal|"-s"
argument_list|)
condition|)
name|silent_on_nonexisting_files
operator|=
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
literal|"-q"
argument_list|)
condition|)
name|silent_on_nonexisting_files
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
literal|"-z"
argument_list|)
condition|)
name|machine_readable
operator|=
literal|1
expr_stmt|;
else|else
name|usage
argument_list|(
name|show_diff_usage
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
name|prepare_diff_cmd
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
name|long
name|size
decl_stmt|;
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|void
modifier|*
name|old
decl_stmt|;
if|if
condition|(
literal|1
operator|<
name|argc
operator|&&
operator|!
name|matches_pathspec
argument_list|(
name|ce
argument_list|,
name|argv
operator|+
literal|1
argument_list|,
name|argc
operator|-
literal|1
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
if|if
condition|(
name|machine_readable
condition|)
name|printf
argument_list|(
literal|"U %s%c"
argument_list|,
name|ce
operator|->
name|name
argument_list|,
literal|0
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s: Unmerged\n"
argument_list|,
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
name|stat
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
operator|==
name|ENOENT
operator|&&
name|silent_on_nonexisting_files
condition|)
continue|continue;
if|if
condition|(
name|machine_readable
condition|)
name|printf
argument_list|(
literal|"X %s%c"
argument_list|,
name|ce
operator|->
name|name
argument_list|,
literal|0
argument_list|)
expr_stmt|;
else|else
block|{
name|printf
argument_list|(
literal|"%s: %s\n"
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|errno
operator|==
name|ENOENT
condition|)
name|show_diff_empty
argument_list|(
name|ce
operator|->
name|sha1
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|reverse
argument_list|)
expr_stmt|;
block|}
continue|continue;
block|}
name|changed
operator|=
name|cache_match_stat
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
continue|continue;
if|if
condition|(
operator|!
name|machine_readable
condition|)
name|printf
argument_list|(
literal|"%s: %s\n"
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|sha1_to_hex
argument_list|(
name|ce
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
else|else
block|{
name|printf
argument_list|(
literal|"%s %s%c"
argument_list|,
name|sha1_to_hex
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
if|if
condition|(
name|silent
condition|)
continue|continue;
name|old
operator|=
name|read_sha1_file
argument_list|(
name|ce
operator|->
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
name|old
condition|)
name|error
argument_list|(
literal|"unable to read blob object for %s (%s)"
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|sha1_to_hex
argument_list|(
name|ce
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
else|else
name|show_differences
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|old
argument_list|,
name|size
argument_list|,
name|reverse
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|old
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

