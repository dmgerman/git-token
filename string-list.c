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
comment|/* if there is no exact match, point to the index where the entry could be  * inserted */
end_comment

begin_function
DECL|function|get_entry_index
specifier|static
name|int
name|get_entry_index
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|int
modifier|*
name|exact_match
parameter_list|)
block|{
name|int
name|left
init|=
operator|-
literal|1
decl_stmt|,
name|right
init|=
name|list
operator|->
name|nr
decl_stmt|;
while|while
condition|(
name|left
operator|+
literal|1
operator|<
name|right
condition|)
block|{
name|int
name|middle
init|=
operator|(
name|left
operator|+
name|right
operator|)
operator|/
literal|2
decl_stmt|;
name|int
name|compare
init|=
name|strcmp
argument_list|(
name|string
argument_list|,
name|list
operator|->
name|items
index|[
name|middle
index|]
operator|.
name|string
argument_list|)
decl_stmt|;
if|if
condition|(
name|compare
operator|<
literal|0
condition|)
name|right
operator|=
name|middle
expr_stmt|;
elseif|else
if|if
condition|(
name|compare
operator|>
literal|0
condition|)
name|left
operator|=
name|middle
expr_stmt|;
else|else
block|{
operator|*
name|exact_match
operator|=
literal|1
expr_stmt|;
return|return
name|middle
return|;
block|}
block|}
operator|*
name|exact_match
operator|=
literal|0
expr_stmt|;
return|return
name|right
return|;
block|}
end_function

begin_comment
comment|/* returns -1-index if already exists */
end_comment

begin_function
DECL|function|add_entry
specifier|static
name|int
name|add_entry
parameter_list|(
name|int
name|insert_at
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|)
block|{
name|int
name|exact_match
init|=
literal|0
decl_stmt|;
name|int
name|index
init|=
name|insert_at
operator|!=
operator|-
literal|1
condition|?
name|insert_at
else|:
name|get_entry_index
argument_list|(
name|list
argument_list|,
name|string
argument_list|,
operator|&
name|exact_match
argument_list|)
decl_stmt|;
if|if
condition|(
name|exact_match
condition|)
return|return
operator|-
literal|1
operator|-
name|index
return|;
if|if
condition|(
name|list
operator|->
name|nr
operator|+
literal|1
operator|>=
name|list
operator|->
name|alloc
condition|)
block|{
name|list
operator|->
name|alloc
operator|+=
literal|32
expr_stmt|;
name|list
operator|->
name|items
operator|=
name|xrealloc
argument_list|(
name|list
operator|->
name|items
argument_list|,
name|list
operator|->
name|alloc
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|string_list_item
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|index
operator|<
name|list
operator|->
name|nr
condition|)
name|memmove
argument_list|(
name|list
operator|->
name|items
operator|+
name|index
operator|+
literal|1
argument_list|,
name|list
operator|->
name|items
operator|+
name|index
argument_list|,
operator|(
name|list
operator|->
name|nr
operator|-
name|index
operator|)
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|string_list_item
argument_list|)
argument_list|)
expr_stmt|;
name|list
operator|->
name|items
index|[
name|index
index|]
operator|.
name|string
operator|=
name|list
operator|->
name|strdup_strings
condition|?
name|xstrdup
argument_list|(
name|string
argument_list|)
else|:
operator|(
name|char
operator|*
operator|)
name|string
expr_stmt|;
name|list
operator|->
name|items
index|[
name|index
index|]
operator|.
name|util
operator|=
name|NULL
expr_stmt|;
name|list
operator|->
name|nr
operator|++
expr_stmt|;
return|return
name|index
return|;
block|}
end_function

begin_function
DECL|function|string_list_insert
name|struct
name|string_list_item
modifier|*
name|string_list_insert
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
block|{
return|return
name|string_list_insert_at_index
argument_list|(
operator|-
literal|1
argument_list|,
name|string
argument_list|,
name|list
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|string_list_insert_at_index
name|struct
name|string_list_item
modifier|*
name|string_list_insert_at_index
parameter_list|(
name|int
name|insert_at
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
block|{
name|int
name|index
init|=
name|add_entry
argument_list|(
name|insert_at
argument_list|,
name|list
argument_list|,
name|string
argument_list|)
decl_stmt|;
if|if
condition|(
name|index
operator|<
literal|0
condition|)
name|index
operator|=
operator|-
literal|1
operator|-
name|index
expr_stmt|;
return|return
name|list
operator|->
name|items
operator|+
name|index
return|;
block|}
end_function

begin_function
DECL|function|string_list_has_string
name|int
name|string_list_has_string
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|)
block|{
name|int
name|exact_match
decl_stmt|;
name|get_entry_index
argument_list|(
name|list
argument_list|,
name|string
argument_list|,
operator|&
name|exact_match
argument_list|)
expr_stmt|;
return|return
name|exact_match
return|;
block|}
end_function

begin_function
DECL|function|string_list_find_insert_index
name|int
name|string_list_find_insert_index
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|int
name|negative_existing_index
parameter_list|)
block|{
name|int
name|exact_match
decl_stmt|;
name|int
name|index
init|=
name|get_entry_index
argument_list|(
name|list
argument_list|,
name|string
argument_list|,
operator|&
name|exact_match
argument_list|)
decl_stmt|;
if|if
condition|(
name|exact_match
condition|)
name|index
operator|=
operator|-
literal|1
operator|-
operator|(
name|negative_existing_index
condition|?
name|index
else|:
literal|0
operator|)
expr_stmt|;
return|return
name|index
return|;
block|}
end_function

begin_function
DECL|function|string_list_lookup
name|struct
name|string_list_item
modifier|*
name|string_list_lookup
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
block|{
name|int
name|exact_match
decl_stmt|,
name|i
init|=
name|get_entry_index
argument_list|(
name|list
argument_list|,
name|string
argument_list|,
operator|&
name|exact_match
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|exact_match
condition|)
return|return
name|NULL
return|;
return|return
name|list
operator|->
name|items
operator|+
name|i
return|;
block|}
end_function

begin_function
DECL|function|string_list_clear
name|void
name|string_list_clear
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|int
name|free_util
parameter_list|)
block|{
if|if
condition|(
name|list
operator|->
name|items
condition|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
name|list
operator|->
name|strdup_strings
condition|)
block|{
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
name|free
argument_list|(
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
if|if
condition|(
name|free_util
condition|)
block|{
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
name|free
argument_list|(
name|list
operator|->
name|items
index|[
name|i
index|]
operator|.
name|util
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|list
operator|->
name|items
argument_list|)
expr_stmt|;
block|}
name|list
operator|->
name|items
operator|=
name|NULL
expr_stmt|;
name|list
operator|->
name|nr
operator|=
name|list
operator|->
name|alloc
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|string_list_clear_func
name|void
name|string_list_clear_func
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|string_list_clear_func_t
name|clearfunc
parameter_list|)
block|{
if|if
condition|(
name|list
operator|->
name|items
condition|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
name|clearfunc
condition|)
block|{
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
name|clearfunc
argument_list|(
name|list
operator|->
name|items
index|[
name|i
index|]
operator|.
name|util
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
if|if
condition|(
name|list
operator|->
name|strdup_strings
condition|)
block|{
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
name|free
argument_list|(
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
name|free
argument_list|(
name|list
operator|->
name|items
argument_list|)
expr_stmt|;
block|}
name|list
operator|->
name|items
operator|=
name|NULL
expr_stmt|;
name|list
operator|->
name|nr
operator|=
name|list
operator|->
name|alloc
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|print_string_list
name|void
name|print_string_list
parameter_list|(
specifier|const
name|char
modifier|*
name|text
parameter_list|,
specifier|const
name|struct
name|string_list
modifier|*
name|p
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
name|text
condition|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|text
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
name|p
operator|->
name|nr
condition|;
name|i
operator|++
control|)
name|printf
argument_list|(
literal|"%s:%p\n"
argument_list|,
name|p
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|,
name|p
operator|->
name|items
index|[
name|i
index|]
operator|.
name|util
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|string_list_append
name|struct
name|string_list_item
modifier|*
name|string_list_append
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
block|{
name|ALLOC_GROW
argument_list|(
name|list
operator|->
name|items
argument_list|,
name|list
operator|->
name|nr
operator|+
literal|1
argument_list|,
name|list
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|list
operator|->
name|items
index|[
name|list
operator|->
name|nr
index|]
operator|.
name|string
operator|=
name|list
operator|->
name|strdup_strings
condition|?
name|xstrdup
argument_list|(
name|string
argument_list|)
else|:
operator|(
name|char
operator|*
operator|)
name|string
expr_stmt|;
return|return
name|list
operator|->
name|items
operator|+
name|list
operator|->
name|nr
operator|++
return|;
block|}
end_function

begin_function
DECL|function|cmp_items
specifier|static
name|int
name|cmp_items
parameter_list|(
specifier|const
name|void
modifier|*
name|a
parameter_list|,
specifier|const
name|void
modifier|*
name|b
parameter_list|)
block|{
specifier|const
name|struct
name|string_list_item
modifier|*
name|one
init|=
name|a
decl_stmt|;
specifier|const
name|struct
name|string_list_item
modifier|*
name|two
init|=
name|b
decl_stmt|;
return|return
name|strcmp
argument_list|(
name|one
operator|->
name|string
argument_list|,
name|two
operator|->
name|string
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|sort_string_list
name|void
name|sort_string_list
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
block|{
name|qsort
argument_list|(
name|list
operator|->
name|items
argument_list|,
name|list
operator|->
name|nr
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|list
operator|->
name|items
argument_list|)
argument_list|,
name|cmp_items
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|unsorted_string_list_has_string
name|int
name|unsorted_string_list_has_string
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|string
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|string
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
condition|)
return|return
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

end_unit

