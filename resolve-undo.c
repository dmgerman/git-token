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
file|"resolve-undo.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_comment
comment|/* The only error case is to run out of memory in string-list */
end_comment

begin_function
DECL|function|record_resolve_undo
name|void
name|record_resolve_undo
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|)
block|{
name|struct
name|string_list_item
modifier|*
name|lost
decl_stmt|;
name|struct
name|resolve_undo_info
modifier|*
name|ui
decl_stmt|;
name|struct
name|string_list
modifier|*
name|resolve_undo
decl_stmt|;
name|int
name|stage
init|=
name|ce_stage
argument_list|(
name|ce
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|stage
condition|)
return|return;
if|if
condition|(
operator|!
name|istate
operator|->
name|resolve_undo
condition|)
block|{
name|resolve_undo
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|resolve_undo
argument_list|)
argument_list|)
expr_stmt|;
name|resolve_undo
operator|->
name|strdup_strings
operator|=
literal|1
expr_stmt|;
name|istate
operator|->
name|resolve_undo
operator|=
name|resolve_undo
expr_stmt|;
block|}
name|resolve_undo
operator|=
name|istate
operator|->
name|resolve_undo
expr_stmt|;
name|lost
operator|=
name|string_list_insert
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|resolve_undo
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lost
operator|->
name|util
condition|)
name|lost
operator|->
name|util
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|ui
argument_list|)
argument_list|)
expr_stmt|;
name|ui
operator|=
name|lost
operator|->
name|util
expr_stmt|;
name|hashcpy
argument_list|(
name|ui
operator|->
name|sha1
index|[
name|stage
operator|-
literal|1
index|]
argument_list|,
name|ce
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|ui
operator|->
name|mode
index|[
name|stage
operator|-
literal|1
index|]
operator|=
name|ce
operator|->
name|ce_mode
expr_stmt|;
block|}
end_function

begin_function
DECL|function|write_one
specifier|static
name|int
name|write_one
parameter_list|(
name|struct
name|string_list_item
modifier|*
name|item
parameter_list|,
name|void
modifier|*
name|cbdata
parameter_list|)
block|{
name|struct
name|strbuf
modifier|*
name|sb
init|=
name|cbdata
decl_stmt|;
name|struct
name|resolve_undo_info
modifier|*
name|ui
init|=
name|item
operator|->
name|util
decl_stmt|;
name|int
name|i
decl_stmt|;
if|if
condition|(
operator|!
name|ui
condition|)
return|return
literal|0
return|;
name|strbuf_addstr
argument_list|(
name|sb
argument_list|,
name|item
operator|->
name|string
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
name|sb
argument_list|,
literal|0
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
literal|3
condition|;
name|i
operator|++
control|)
name|strbuf_addf
argument_list|(
name|sb
argument_list|,
literal|"%o%c"
argument_list|,
name|ui
operator|->
name|mode
index|[
name|i
index|]
argument_list|,
literal|0
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
literal|3
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|ui
operator|->
name|mode
index|[
name|i
index|]
condition|)
continue|continue;
name|strbuf_add
argument_list|(
name|sb
argument_list|,
name|ui
operator|->
name|sha1
index|[
name|i
index|]
argument_list|,
literal|20
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|resolve_undo_write
name|void
name|resolve_undo_write
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|struct
name|string_list
modifier|*
name|resolve_undo
parameter_list|)
block|{
name|for_each_string_list
argument_list|(
name|resolve_undo
argument_list|,
name|write_one
argument_list|,
name|sb
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|resolve_undo_read
name|struct
name|string_list
modifier|*
name|resolve_undo_read
parameter_list|(
specifier|const
name|char
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|struct
name|string_list
modifier|*
name|resolve_undo
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|char
modifier|*
name|endptr
decl_stmt|;
name|int
name|i
decl_stmt|;
name|resolve_undo
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|resolve_undo
argument_list|)
argument_list|)
expr_stmt|;
name|resolve_undo
operator|->
name|strdup_strings
operator|=
literal|1
expr_stmt|;
while|while
condition|(
name|size
condition|)
block|{
name|struct
name|string_list_item
modifier|*
name|lost
decl_stmt|;
name|struct
name|resolve_undo_info
modifier|*
name|ui
decl_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|data
argument_list|)
operator|+
literal|1
expr_stmt|;
if|if
condition|(
name|size
operator|<=
name|len
condition|)
goto|goto
name|error
goto|;
name|lost
operator|=
name|string_list_insert
argument_list|(
name|data
argument_list|,
name|resolve_undo
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lost
operator|->
name|util
condition|)
name|lost
operator|->
name|util
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|ui
argument_list|)
argument_list|)
expr_stmt|;
name|ui
operator|=
name|lost
operator|->
name|util
expr_stmt|;
name|size
operator|-=
name|len
expr_stmt|;
name|data
operator|+=
name|len
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|3
condition|;
name|i
operator|++
control|)
block|{
name|ui
operator|->
name|mode
index|[
name|i
index|]
operator|=
name|strtoul
argument_list|(
name|data
argument_list|,
operator|&
name|endptr
argument_list|,
literal|8
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|endptr
operator|||
name|endptr
operator|==
name|data
operator|||
operator|*
name|endptr
condition|)
goto|goto
name|error
goto|;
name|len
operator|=
operator|(
name|endptr
operator|+
literal|1
operator|)
operator|-
operator|(
name|char
operator|*
operator|)
name|data
expr_stmt|;
if|if
condition|(
name|size
operator|<=
name|len
condition|)
goto|goto
name|error
goto|;
name|size
operator|-=
name|len
expr_stmt|;
name|data
operator|+=
name|len
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
literal|3
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|ui
operator|->
name|mode
index|[
name|i
index|]
condition|)
continue|continue;
if|if
condition|(
name|size
operator|<
literal|20
condition|)
goto|goto
name|error
goto|;
name|hashcpy
argument_list|(
name|ui
operator|->
name|sha1
index|[
name|i
index|]
argument_list|,
operator|(
specifier|const
name|unsigned
name|char
operator|*
operator|)
name|data
argument_list|)
expr_stmt|;
name|size
operator|-=
literal|20
expr_stmt|;
name|data
operator|+=
literal|20
expr_stmt|;
block|}
block|}
return|return
name|resolve_undo
return|;
name|error
label|:
name|string_list_clear
argument_list|(
name|resolve_undo
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|error
argument_list|(
literal|"Index records invalid resolve-undo information"
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|resolve_undo_clear_index
name|void
name|resolve_undo_clear_index
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|)
block|{
name|struct
name|string_list
modifier|*
name|resolve_undo
init|=
name|istate
operator|->
name|resolve_undo
decl_stmt|;
if|if
condition|(
operator|!
name|resolve_undo
condition|)
return|return;
name|string_list_clear
argument_list|(
name|resolve_undo
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|resolve_undo
argument_list|)
expr_stmt|;
name|istate
operator|->
name|resolve_undo
operator|=
name|NULL
expr_stmt|;
name|istate
operator|->
name|cache_changed
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|unmerge_index_entry_at
name|int
name|unmerge_index_entry_at
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|,
name|int
name|pos
parameter_list|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|;
name|struct
name|string_list_item
modifier|*
name|item
decl_stmt|;
name|struct
name|resolve_undo_info
modifier|*
name|ru
decl_stmt|;
name|int
name|i
decl_stmt|,
name|err
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|istate
operator|->
name|resolve_undo
condition|)
return|return
name|pos
return|;
name|ce
operator|=
name|istate
operator|->
name|cache
index|[
name|pos
index|]
expr_stmt|;
if|if
condition|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|)
block|{
comment|/* already unmerged */
while|while
condition|(
operator|(
name|pos
operator|<
name|istate
operator|->
name|cache_nr
operator|)
operator|&&
operator|!
name|strcmp
argument_list|(
name|istate
operator|->
name|cache
index|[
name|pos
index|]
operator|->
name|name
argument_list|,
name|ce
operator|->
name|name
argument_list|)
condition|)
name|pos
operator|++
expr_stmt|;
return|return
name|pos
operator|-
literal|1
return|;
comment|/* return the last entry processed */
block|}
name|item
operator|=
name|string_list_lookup
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|istate
operator|->
name|resolve_undo
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|item
condition|)
return|return
name|pos
return|;
name|ru
operator|=
name|item
operator|->
name|util
expr_stmt|;
if|if
condition|(
operator|!
name|ru
condition|)
return|return
name|pos
return|;
name|remove_index_entry_at
argument_list|(
name|istate
argument_list|,
name|pos
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
literal|3
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|nce
decl_stmt|;
if|if
condition|(
operator|!
name|ru
operator|->
name|mode
index|[
name|i
index|]
condition|)
continue|continue;
name|nce
operator|=
name|make_cache_entry
argument_list|(
name|ru
operator|->
name|mode
index|[
name|i
index|]
argument_list|,
name|ru
operator|->
name|sha1
index|[
name|i
index|]
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|i
operator|+
literal|1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|add_index_entry
argument_list|(
name|istate
argument_list|,
name|nce
argument_list|,
name|ADD_CACHE_OK_TO_ADD
argument_list|)
condition|)
block|{
name|err
operator|=
literal|1
expr_stmt|;
name|error
argument_list|(
literal|"cannot unmerge '%s'"
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|err
condition|)
return|return
name|pos
return|;
name|free
argument_list|(
name|ru
argument_list|)
expr_stmt|;
name|item
operator|->
name|util
operator|=
name|NULL
expr_stmt|;
return|return
name|unmerge_index_entry_at
argument_list|(
name|istate
argument_list|,
name|pos
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|unmerge_index
name|void
name|unmerge_index
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
operator|!
name|istate
operator|->
name|resolve_undo
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
name|istate
operator|->
name|cache_nr
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
name|istate
operator|->
name|cache
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
name|NULL
argument_list|)
condition|)
continue|continue;
name|i
operator|=
name|unmerge_index_entry_at
argument_list|(
name|istate
argument_list|,
name|i
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit

