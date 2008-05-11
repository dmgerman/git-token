begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * name-hash.c  *  * Hashing names in the index state  *  * Copyright (C) 2008 Linus Torvalds  */
end_comment

begin_define
DECL|macro|NO_THE_INDEX_COMPATIBILITY_MACROS
define|#
directive|define
name|NO_THE_INDEX_COMPATIBILITY_MACROS
end_define

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_comment
comment|/*  * This removes bit 5 if bit 6 is set.  *  * That will make US-ASCII characters hash to their upper-case  * equivalent. We could easily do this one whole word at a time,  * but that's for future worries.  */
end_comment

begin_function
DECL|function|icase_hash
specifier|static
specifier|inline
name|unsigned
name|char
name|icase_hash
parameter_list|(
name|unsigned
name|char
name|c
parameter_list|)
block|{
return|return
name|c
operator|&
operator|~
operator|(
operator|(
name|c
operator|&
literal|0x40
operator|)
operator|>>
literal|1
operator|)
return|;
block|}
end_function

begin_function
DECL|function|hash_name
specifier|static
name|unsigned
name|int
name|hash_name
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|namelen
parameter_list|)
block|{
name|unsigned
name|int
name|hash
init|=
literal|0x123
decl_stmt|;
do|do
block|{
name|unsigned
name|char
name|c
init|=
operator|*
name|name
operator|++
decl_stmt|;
name|c
operator|=
name|icase_hash
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|hash
operator|=
name|hash
operator|*
literal|101
operator|+
name|c
expr_stmt|;
block|}
do|while
condition|(
operator|--
name|namelen
condition|)
do|;
return|return
name|hash
return|;
block|}
end_function

begin_function
DECL|function|hash_index_entry
specifier|static
name|void
name|hash_index_entry
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
name|void
modifier|*
modifier|*
name|pos
decl_stmt|;
name|unsigned
name|int
name|hash
decl_stmt|;
if|if
condition|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_HASHED
condition|)
return|return;
name|ce
operator|->
name|ce_flags
operator||=
name|CE_HASHED
expr_stmt|;
name|ce
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|hash
operator|=
name|hash_name
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|ce_namelen
argument_list|(
name|ce
argument_list|)
argument_list|)
expr_stmt|;
name|pos
operator|=
name|insert_hash
argument_list|(
name|hash
argument_list|,
name|ce
argument_list|,
operator|&
name|istate
operator|->
name|name_hash
argument_list|)
expr_stmt|;
if|if
condition|(
name|pos
condition|)
block|{
name|ce
operator|->
name|next
operator|=
operator|*
name|pos
expr_stmt|;
operator|*
name|pos
operator|=
name|ce
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|lazy_init_name_hash
specifier|static
name|void
name|lazy_init_name_hash
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|)
block|{
name|int
name|nr
decl_stmt|;
if|if
condition|(
name|istate
operator|->
name|name_hash_initialized
condition|)
return|return;
for|for
control|(
name|nr
operator|=
literal|0
init|;
name|nr
operator|<
name|istate
operator|->
name|cache_nr
condition|;
name|nr
operator|++
control|)
name|hash_index_entry
argument_list|(
name|istate
argument_list|,
name|istate
operator|->
name|cache
index|[
name|nr
index|]
argument_list|)
expr_stmt|;
name|istate
operator|->
name|name_hash_initialized
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_name_hash
name|void
name|add_name_hash
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
name|ce
operator|->
name|ce_flags
operator|&=
operator|~
name|CE_UNHASHED
expr_stmt|;
if|if
condition|(
name|istate
operator|->
name|name_hash_initialized
condition|)
name|hash_index_entry
argument_list|(
name|istate
argument_list|,
name|ce
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|slow_same_name
specifier|static
name|int
name|slow_same_name
parameter_list|(
specifier|const
name|char
modifier|*
name|name1
parameter_list|,
name|int
name|len1
parameter_list|,
specifier|const
name|char
modifier|*
name|name2
parameter_list|,
name|int
name|len2
parameter_list|)
block|{
if|if
condition|(
name|len1
operator|!=
name|len2
condition|)
return|return
literal|0
return|;
while|while
condition|(
name|len1
condition|)
block|{
name|unsigned
name|char
name|c1
init|=
operator|*
name|name1
operator|++
decl_stmt|;
name|unsigned
name|char
name|c2
init|=
operator|*
name|name2
operator|++
decl_stmt|;
name|len1
operator|--
expr_stmt|;
if|if
condition|(
name|c1
operator|!=
name|c2
condition|)
block|{
name|c1
operator|=
name|toupper
argument_list|(
name|c1
argument_list|)
expr_stmt|;
name|c2
operator|=
name|toupper
argument_list|(
name|c2
argument_list|)
expr_stmt|;
if|if
condition|(
name|c1
operator|!=
name|c2
condition|)
return|return
literal|0
return|;
block|}
block|}
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|same_name
specifier|static
name|int
name|same_name
parameter_list|(
specifier|const
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|namelen
parameter_list|,
name|int
name|icase
parameter_list|)
block|{
name|int
name|len
init|=
name|ce_namelen
argument_list|(
name|ce
argument_list|)
decl_stmt|;
comment|/* 	 * Always do exact compare, even if we want a case-ignoring comparison; 	 * we do the quick exact one first, because it will be the common case. 	 */
if|if
condition|(
name|len
operator|==
name|namelen
operator|&&
operator|!
name|cache_name_compare
argument_list|(
name|name
argument_list|,
name|namelen
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|len
argument_list|)
condition|)
return|return
literal|1
return|;
return|return
name|icase
operator|&&
name|slow_same_name
argument_list|(
name|name
argument_list|,
name|namelen
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|len
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|index_name_exists
name|struct
name|cache_entry
modifier|*
name|index_name_exists
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|namelen
parameter_list|,
name|int
name|icase
parameter_list|)
block|{
name|unsigned
name|int
name|hash
init|=
name|hash_name
argument_list|(
name|name
argument_list|,
name|namelen
argument_list|)
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|;
name|lazy_init_name_hash
argument_list|(
name|istate
argument_list|)
expr_stmt|;
name|ce
operator|=
name|lookup_hash
argument_list|(
name|hash
argument_list|,
operator|&
name|istate
operator|->
name|name_hash
argument_list|)
expr_stmt|;
while|while
condition|(
name|ce
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_UNHASHED
operator|)
condition|)
block|{
if|if
condition|(
name|same_name
argument_list|(
name|ce
argument_list|,
name|name
argument_list|,
name|namelen
argument_list|,
name|icase
argument_list|)
condition|)
return|return
name|ce
return|;
block|}
name|ce
operator|=
name|ce
operator|->
name|next
expr_stmt|;
block|}
return|return
name|NULL
return|;
block|}
end_function

end_unit

