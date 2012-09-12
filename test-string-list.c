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
comment|/*  * Parse an argument into a string list.  arg should either be a  * ':'-separated list of strings, or "-" to indicate an empty string  * list (as opposed to "", which indicates a string list containing a  * single empty string).  list->strdup_strings must be set.  */
end_comment

begin_function
DECL|function|parse_string_list
name|void
name|parse_string_list
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-"
argument_list|)
condition|)
return|return;
operator|(
name|void
operator|)
name|string_list_split
argument_list|(
name|list
argument_list|,
name|arg
argument_list|,
literal|':'
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|write_list
name|void
name|write_list
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
block|{
name|int
name|i
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
operator|->
name|nr
condition|;
name|i
operator|++
control|)
name|printf
argument_list|(
literal|"[%d]: \"%s\"\n"
argument_list|,
name|i
argument_list|,
name|list
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|write_list_compact
name|void
name|write_list_compact
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
operator|!
name|list
operator|->
name|nr
condition|)
name|printf
argument_list|(
literal|"-\n"
argument_list|)
expr_stmt|;
else|else
block|{
name|printf
argument_list|(
literal|"%s"
argument_list|,
name|list
operator|->
name|items
index|[
literal|0
index|]
operator|.
name|string
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
name|list
operator|->
name|nr
condition|;
name|i
operator|++
control|)
name|printf
argument_list|(
literal|":%s"
argument_list|,
name|list
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|prefix_cb
name|int
name|prefix_cb
parameter_list|(
name|struct
name|string_list_item
modifier|*
name|item
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|prefix
init|=
operator|(
specifier|const
name|char
operator|*
operator|)
name|cb_data
decl_stmt|;
return|return
operator|!
name|prefixcmp
argument_list|(
name|item
operator|->
name|string
argument_list|,
name|prefix
argument_list|)
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
if|if
condition|(
name|argc
operator|==
literal|5
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"split"
argument_list|)
condition|)
block|{
name|struct
name|string_list
name|list
init|=
name|STRING_LIST_INIT_DUP
decl_stmt|;
name|int
name|i
decl_stmt|;
specifier|const
name|char
modifier|*
name|s
init|=
name|argv
index|[
literal|2
index|]
decl_stmt|;
name|int
name|delim
init|=
operator|*
name|argv
index|[
literal|3
index|]
decl_stmt|;
name|int
name|maxsplit
init|=
name|atoi
argument_list|(
name|argv
index|[
literal|4
index|]
argument_list|)
decl_stmt|;
name|i
operator|=
name|string_list_split
argument_list|(
operator|&
name|list
argument_list|,
name|s
argument_list|,
name|delim
argument_list|,
name|maxsplit
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%d\n"
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|write_list
argument_list|(
operator|&
name|list
argument_list|)
expr_stmt|;
name|string_list_clear
argument_list|(
operator|&
name|list
argument_list|,
literal|0
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
literal|5
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"split_in_place"
argument_list|)
condition|)
block|{
name|struct
name|string_list
name|list
init|=
name|STRING_LIST_INIT_NODUP
decl_stmt|;
name|int
name|i
decl_stmt|;
name|char
modifier|*
name|s
init|=
name|xstrdup
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|)
decl_stmt|;
name|int
name|delim
init|=
operator|*
name|argv
index|[
literal|3
index|]
decl_stmt|;
name|int
name|maxsplit
init|=
name|atoi
argument_list|(
name|argv
index|[
literal|4
index|]
argument_list|)
decl_stmt|;
name|i
operator|=
name|string_list_split_in_place
argument_list|(
operator|&
name|list
argument_list|,
name|s
argument_list|,
name|delim
argument_list|,
name|maxsplit
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%d\n"
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|write_list
argument_list|(
operator|&
name|list
argument_list|)
expr_stmt|;
name|string_list_clear
argument_list|(
operator|&
name|list
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|s
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
literal|"filter"
argument_list|)
condition|)
block|{
comment|/* 		 * Retain only the items that have the specified prefix. 		 * Arguments: list|- prefix 		 */
name|struct
name|string_list
name|list
init|=
name|STRING_LIST_INIT_DUP
decl_stmt|;
specifier|const
name|char
modifier|*
name|prefix
init|=
name|argv
index|[
literal|3
index|]
decl_stmt|;
name|parse_string_list
argument_list|(
operator|&
name|list
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
name|filter_string_list
argument_list|(
operator|&
name|list
argument_list|,
literal|0
argument_list|,
name|prefix_cb
argument_list|,
operator|(
name|void
operator|*
operator|)
name|prefix
argument_list|)
expr_stmt|;
name|write_list_compact
argument_list|(
operator|&
name|list
argument_list|)
expr_stmt|;
name|string_list_clear
argument_list|(
operator|&
name|list
argument_list|,
literal|0
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
literal|"remove_duplicates"
argument_list|)
condition|)
block|{
name|struct
name|string_list
name|list
init|=
name|STRING_LIST_INIT_DUP
decl_stmt|;
name|parse_string_list
argument_list|(
operator|&
name|list
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
name|string_list_remove_duplicates
argument_list|(
operator|&
name|list
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|write_list_compact
argument_list|(
operator|&
name|list
argument_list|)
expr_stmt|;
name|string_list_clear
argument_list|(
operator|&
name|list
argument_list|,
literal|0
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

