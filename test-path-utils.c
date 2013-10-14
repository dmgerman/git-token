begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_comment
comment|/*  * A "string_list_each_func_t" function that normalizes an entry from  * GIT_CEILING_DIRECTORIES.  If the path is unusable for some reason,  * die with an explanation.  */
end_comment

begin_function
DECL|function|normalize_ceiling_entry
specifier|static
name|int
name|normalize_ceiling_entry
parameter_list|(
name|struct
name|string_list_item
modifier|*
name|item
parameter_list|,
name|void
modifier|*
name|unused
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|ceil
init|=
name|item
operator|->
name|string
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|ceil
argument_list|)
decl_stmt|;
name|char
name|buf
index|[
name|PATH_MAX
operator|+
literal|1
index|]
decl_stmt|;
if|if
condition|(
name|len
operator|==
literal|0
condition|)
name|die
argument_list|(
literal|"Empty path is not supported"
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>
name|PATH_MAX
condition|)
name|die
argument_list|(
literal|"Path \"%s\" is too long"
argument_list|,
name|ceil
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_absolute_path
argument_list|(
name|ceil
argument_list|)
condition|)
name|die
argument_list|(
literal|"Path \"%s\" is not absolute"
argument_list|,
name|ceil
argument_list|)
expr_stmt|;
if|if
condition|(
name|normalize_path_copy
argument_list|(
name|buf
argument_list|,
name|ceil
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Path \"%s\" could not be normalized"
argument_list|,
name|ceil
argument_list|)
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>
literal|1
operator|&&
name|buf
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
name|die
argument_list|(
literal|"Normalized path \"%s\" ended with slash"
argument_list|,
name|buf
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|item
operator|->
name|string
argument_list|)
expr_stmt|;
name|item
operator|->
name|string
operator|=
name|xstrdup
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|normalize_argv_string
specifier|static
name|void
name|normalize_argv_string
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|input
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|input
argument_list|,
literal|"<null>"
argument_list|)
condition|)
operator|*
name|var
operator|=
name|NULL
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|input
argument_list|,
literal|"<empty>"
argument_list|)
condition|)
operator|*
name|var
operator|=
literal|""
expr_stmt|;
else|else
operator|*
name|var
operator|=
name|input
expr_stmt|;
if|if
condition|(
operator|*
name|var
operator|&&
operator|(
operator|*
operator|*
name|var
operator|==
literal|'<'
operator|||
operator|*
operator|*
name|var
operator|==
literal|'('
operator|)
condition|)
name|die
argument_list|(
literal|"Bad value: %s\n"
argument_list|,
name|input
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
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
if|if
condition|(
name|argc
operator|==
literal|3
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"normalize_path_copy"
argument_list|)
condition|)
block|{
name|char
modifier|*
name|buf
init|=
name|xmalloc
argument_list|(
name|PATH_MAX
operator|+
literal|1
argument_list|)
decl_stmt|;
name|int
name|rv
init|=
name|normalize_path_copy
argument_list|(
name|buf
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
name|rv
condition|)
name|buf
operator|=
literal|"++failed++"
expr_stmt|;
name|puts
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|argc
operator|>=
literal|2
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"real_path"
argument_list|)
condition|)
block|{
while|while
condition|(
name|argc
operator|>
literal|2
condition|)
block|{
name|puts
argument_list|(
name|real_path
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|)
argument_list|)
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
if|if
condition|(
name|argc
operator|>=
literal|2
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"absolute_path"
argument_list|)
condition|)
block|{
while|while
condition|(
name|argc
operator|>
literal|2
condition|)
block|{
name|puts
argument_list|(
name|absolute_path
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|)
argument_list|)
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
if|if
condition|(
name|argc
operator|==
literal|4
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"longest_ancestor_length"
argument_list|)
condition|)
block|{
name|int
name|len
decl_stmt|;
name|struct
name|string_list
name|ceiling_dirs
init|=
name|STRING_LIST_INIT_DUP
decl_stmt|;
name|char
modifier|*
name|path
init|=
name|xstrdup
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|)
decl_stmt|;
comment|/* 		 * We have to normalize the arguments because under 		 * Windows, bash mangles arguments that look like 		 * absolute POSIX paths or colon-separate lists of 		 * absolute POSIX paths into DOS paths (e.g., 		 * "/foo:/foo/bar" might be converted to 		 * "D:\Src\msysgit\foo;D:\Src\msysgit\foo\bar"), 		 * whereas longest_ancestor_length() requires paths 		 * that use forward slashes. 		 */
if|if
condition|(
name|normalize_path_copy
argument_list|(
name|path
argument_list|,
name|path
argument_list|)
condition|)
name|die
argument_list|(
literal|"Path \"%s\" could not be normalized"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
name|string_list_split
argument_list|(
operator|&
name|ceiling_dirs
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|,
name|PATH_SEP
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
name|filter_string_list
argument_list|(
operator|&
name|ceiling_dirs
argument_list|,
literal|0
argument_list|,
name|normalize_ceiling_entry
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|len
operator|=
name|longest_ancestor_length
argument_list|(
name|path
argument_list|,
operator|&
name|ceiling_dirs
argument_list|)
expr_stmt|;
name|string_list_clear
argument_list|(
operator|&
name|ceiling_dirs
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%d\n"
argument_list|,
name|len
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|argc
operator|>=
literal|4
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"prefix_path"
argument_list|)
condition|)
block|{
name|char
modifier|*
name|prefix
init|=
name|argv
index|[
literal|2
index|]
decl_stmt|;
name|int
name|prefix_len
init|=
name|strlen
argument_list|(
name|prefix
argument_list|)
decl_stmt|;
name|int
name|nongit_ok
decl_stmt|;
name|setup_git_directory_gently
argument_list|(
operator|&
name|nongit_ok
argument_list|)
expr_stmt|;
while|while
condition|(
name|argc
operator|>
literal|3
condition|)
block|{
name|puts
argument_list|(
name|prefix_path
argument_list|(
name|prefix
argument_list|,
name|prefix_len
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|)
argument_list|)
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
if|if
condition|(
name|argc
operator|==
literal|4
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"strip_path_suffix"
argument_list|)
condition|)
block|{
name|char
modifier|*
name|prefix
init|=
name|strip_path_suffix
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|)
decl_stmt|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|prefix
condition|?
name|prefix
else|:
literal|"(null)"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|argc
operator|==
literal|3
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"print_path"
argument_list|)
condition|)
block|{
name|puts
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|argc
operator|==
literal|4
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"relative_path"
argument_list|)
condition|)
block|{
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
specifier|const
name|char
modifier|*
name|in
decl_stmt|,
modifier|*
name|prefix
decl_stmt|,
modifier|*
name|rel
decl_stmt|;
name|normalize_argv_string
argument_list|(
operator|&
name|in
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
name|normalize_argv_string
argument_list|(
operator|&
name|prefix
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|)
expr_stmt|;
name|rel
operator|=
name|relative_path
argument_list|(
name|in
argument_list|,
name|prefix
argument_list|,
operator|&
name|sb
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|rel
condition|)
name|puts
argument_list|(
literal|"(null)"
argument_list|)
expr_stmt|;
else|else
name|puts
argument_list|(
name|strlen
argument_list|(
name|rel
argument_list|)
operator|>
literal|0
condition|?
name|rel
else|:
literal|"(empty)"
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s: unknown function name: %s\n"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|,
name|argv
index|[
literal|1
index|]
condition|?
name|argv
index|[
literal|1
index|]
else|:
literal|"(there was none)"
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

end_unit

