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
DECL|variable|diff_cmd
specifier|static
name|char
modifier|*
name|diff_cmd
init|=
literal|"diff -L '%s' -u -N  - '%s'"
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Help to copy the thing properly quoted for the shell safety.  * any single quote is replaced with '\'', and the caller is  * expected to enclose the result within a single quote pair.  *  * E.g.  *  original     sq_expand     result  *  name     ==> name      ==> 'name'  *  a b      ==> a b       ==> 'a b'  *  a'b      ==> a'\''b    ==> 'a'\''b'  *  * NOTE! The returned memory belongs to this function so  * do not free it.  */
end_comment

begin_function
DECL|function|sq_expand
specifier|static
name|char
modifier|*
name|sq_expand
parameter_list|(
name|char
modifier|*
name|src
parameter_list|)
block|{
specifier|static
name|char
modifier|*
name|buf
init|=
name|NULL
decl_stmt|;
specifier|static
name|int
name|buf_size
init|=
operator|-
literal|1
decl_stmt|;
name|int
name|cnt
decl_stmt|,
name|c
decl_stmt|;
name|char
modifier|*
name|cp
decl_stmt|;
comment|/* count bytes needed to store the quoted string. */
for|for
control|(
name|cnt
operator|=
literal|1
operator|,
name|cp
operator|=
name|src
init|;
operator|*
name|cp
condition|;
name|cnt
operator|++
operator|,
name|cp
operator|++
control|)
if|if
condition|(
operator|*
name|cp
operator|==
literal|'\''
condition|)
name|cnt
operator|+=
literal|3
expr_stmt|;
if|if
condition|(
name|buf_size
operator|<
name|cnt
condition|)
block|{
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
name|buf_size
operator|=
name|cnt
expr_stmt|;
name|buf
operator|=
name|malloc
argument_list|(
name|cnt
argument_list|)
expr_stmt|;
block|}
name|cp
operator|=
name|buf
expr_stmt|;
while|while
condition|(
operator|(
name|c
operator|=
operator|*
name|src
operator|++
operator|)
condition|)
block|{
if|if
condition|(
name|c
operator|!=
literal|'\''
condition|)
operator|*
name|cp
operator|++
operator|=
name|c
expr_stmt|;
else|else
block|{
name|cp
operator|=
name|strcpy
argument_list|(
name|cp
argument_list|,
literal|"'\\''"
argument_list|)
expr_stmt|;
name|cp
operator|+=
literal|4
expr_stmt|;
block|}
block|}
operator|*
name|cp
operator|=
literal|0
expr_stmt|;
return|return
name|buf
return|;
block|}
end_function

begin_function
DECL|function|show_differences
specifier|static
name|void
name|show_differences
parameter_list|(
name|char
modifier|*
name|name
parameter_list|,
name|void
modifier|*
name|old_contents
parameter_list|,
name|unsigned
name|long
name|long
name|old_size
parameter_list|)
block|{
name|FILE
modifier|*
name|f
decl_stmt|;
specifier|static
name|char
modifier|*
name|cmd
init|=
name|NULL
decl_stmt|;
specifier|static
name|int
name|cmd_size
init|=
operator|-
literal|1
decl_stmt|;
name|char
modifier|*
name|name_sq
init|=
name|sq_expand
argument_list|(
name|name
argument_list|)
decl_stmt|;
name|int
name|cmd_required_length
init|=
name|strlen
argument_list|(
name|name_sq
argument_list|)
operator|*
literal|2
operator|+
name|strlen
argument_list|(
name|diff_cmd
argument_list|)
decl_stmt|;
if|if
condition|(
name|cmd_size
operator|<
name|cmd_required_length
condition|)
block|{
name|free
argument_list|(
name|cmd
argument_list|)
expr_stmt|;
name|cmd_size
operator|=
name|cmd_required_length
expr_stmt|;
name|cmd
operator|=
name|malloc
argument_list|(
name|cmd_required_length
argument_list|)
expr_stmt|;
block|}
name|snprintf
argument_list|(
name|cmd
argument_list|,
name|cmd_size
argument_list|,
name|diff_cmd
argument_list|,
name|name_sq
argument_list|,
name|name_sq
argument_list|)
expr_stmt|;
name|f
operator|=
name|popen
argument_list|(
name|cmd
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
if|if
condition|(
name|old_size
condition|)
name|fwrite
argument_list|(
name|old_contents
argument_list|,
name|old_size
argument_list|,
literal|1
argument_list|,
name|f
argument_list|)
expr_stmt|;
name|pclose
argument_list|(
name|f
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_diff_empty
specifier|static
name|void
name|show_diff_empty
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|)
block|{
name|char
modifier|*
name|old
decl_stmt|;
name|unsigned
name|long
name|int
name|size
decl_stmt|;
name|int
name|lines
init|=
literal|0
decl_stmt|;
name|unsigned
name|char
name|type
index|[
literal|20
index|]
decl_stmt|,
modifier|*
name|p
decl_stmt|,
modifier|*
name|end
decl_stmt|;
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
name|size
operator|>
literal|0
condition|)
block|{
name|int
name|startline
init|=
literal|1
decl_stmt|;
name|int
name|c
init|=
literal|0
decl_stmt|;
name|printf
argument_list|(
literal|"--- %s\n"
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"+++ /dev/null\n"
argument_list|)
expr_stmt|;
name|p
operator|=
name|old
expr_stmt|;
name|end
operator|=
name|old
operator|+
name|size
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|end
condition|)
if|if
condition|(
operator|*
name|p
operator|++
operator|==
literal|'\n'
condition|)
name|lines
operator|++
expr_stmt|;
name|printf
argument_list|(
literal|"@@ -1,%d +0,0 @@\n"
argument_list|,
name|lines
argument_list|)
expr_stmt|;
name|p
operator|=
name|old
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|end
condition|)
block|{
name|c
operator|=
operator|*
name|p
operator|++
expr_stmt|;
if|if
condition|(
name|startline
condition|)
block|{
name|putchar
argument_list|(
literal|'-'
argument_list|)
expr_stmt|;
name|startline
operator|=
literal|0
expr_stmt|;
block|}
name|putchar
argument_list|(
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|==
literal|'\n'
condition|)
name|startline
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|c
operator|!=
literal|'\n'
condition|)
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
block|}
block|}
end_function

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
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
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
name|show_differences
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|old
argument_list|,
name|size
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

