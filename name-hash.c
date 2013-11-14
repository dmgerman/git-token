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

begin_struct
DECL|struct|dir_entry
struct|struct
name|dir_entry
block|{
DECL|member|ent
name|struct
name|hashmap_entry
name|ent
decl_stmt|;
DECL|member|parent
name|struct
name|dir_entry
modifier|*
name|parent
decl_stmt|;
DECL|member|ce
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|;
DECL|member|nr
name|int
name|nr
decl_stmt|;
DECL|member|namelen
name|unsigned
name|int
name|namelen
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|dir_entry_cmp
specifier|static
name|int
name|dir_entry_cmp
parameter_list|(
specifier|const
name|struct
name|dir_entry
modifier|*
name|e1
parameter_list|,
specifier|const
name|struct
name|dir_entry
modifier|*
name|e2
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
return|return
name|e1
operator|->
name|namelen
operator|!=
name|e2
operator|->
name|namelen
operator|||
name|strncasecmp
argument_list|(
name|e1
operator|->
name|ce
operator|->
name|name
argument_list|,
name|name
condition|?
name|name
else|:
name|e2
operator|->
name|ce
operator|->
name|name
argument_list|,
name|e1
operator|->
name|namelen
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|find_dir_entry
specifier|static
name|struct
name|dir_entry
modifier|*
name|find_dir_entry
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
name|unsigned
name|int
name|namelen
parameter_list|)
block|{
name|struct
name|dir_entry
name|key
decl_stmt|;
name|hashmap_entry_init
argument_list|(
operator|&
name|key
argument_list|,
name|memihash
argument_list|(
name|name
argument_list|,
name|namelen
argument_list|)
argument_list|)
expr_stmt|;
name|key
operator|.
name|namelen
operator|=
name|namelen
expr_stmt|;
return|return
name|hashmap_get
argument_list|(
operator|&
name|istate
operator|->
name|dir_hash
argument_list|,
operator|&
name|key
argument_list|,
name|name
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|hash_dir_entry
specifier|static
name|struct
name|dir_entry
modifier|*
name|hash_dir_entry
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
parameter_list|,
name|int
name|namelen
parameter_list|)
block|{
comment|/* 	 * Throw each directory component in the hash for quick lookup 	 * during a git status. Directory components are stored without their 	 * closing slash.  Despite submodules being a directory, they never 	 * reach this point, because they are stored 	 * in index_state.name_hash (as ordinary cache_entries). 	 * 	 * Note that the cache_entry stored with the dir_entry merely 	 * supplies the name of the directory (up to dir_entry.namelen). We 	 * track the number of 'active' files in a directory in dir_entry.nr, 	 * so we can tell if the directory is still relevant, e.g. for git 	 * status. However, if cache_entries are removed, we cannot pinpoint 	 * an exact cache_entry that's still active. It is very possible that 	 * multiple dir_entries point to the same cache_entry. 	 */
name|struct
name|dir_entry
modifier|*
name|dir
decl_stmt|;
comment|/* get length of parent directory */
while|while
condition|(
name|namelen
operator|>
literal|0
operator|&&
operator|!
name|is_dir_sep
argument_list|(
name|ce
operator|->
name|name
index|[
name|namelen
operator|-
literal|1
index|]
argument_list|)
condition|)
name|namelen
operator|--
expr_stmt|;
if|if
condition|(
name|namelen
operator|<=
literal|0
condition|)
return|return
name|NULL
return|;
name|namelen
operator|--
expr_stmt|;
comment|/* lookup existing entry for that directory */
name|dir
operator|=
name|find_dir_entry
argument_list|(
name|istate
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|namelen
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dir
condition|)
block|{
comment|/* not found, create it and add to hash table */
name|dir
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|dir_entry
argument_list|)
argument_list|)
expr_stmt|;
name|hashmap_entry_init
argument_list|(
name|dir
argument_list|,
name|memihash
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|namelen
argument_list|)
argument_list|)
expr_stmt|;
name|dir
operator|->
name|namelen
operator|=
name|namelen
expr_stmt|;
name|dir
operator|->
name|ce
operator|=
name|ce
expr_stmt|;
name|hashmap_add
argument_list|(
operator|&
name|istate
operator|->
name|dir_hash
argument_list|,
name|dir
argument_list|)
expr_stmt|;
comment|/* recursively add missing parent directories */
name|dir
operator|->
name|parent
operator|=
name|hash_dir_entry
argument_list|(
name|istate
argument_list|,
name|ce
argument_list|,
name|namelen
argument_list|)
expr_stmt|;
block|}
return|return
name|dir
return|;
block|}
end_function

begin_function
DECL|function|add_dir_entry
specifier|static
name|void
name|add_dir_entry
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
comment|/* Add reference to the directory entry (and parents if 0). */
name|struct
name|dir_entry
modifier|*
name|dir
init|=
name|hash_dir_entry
argument_list|(
name|istate
argument_list|,
name|ce
argument_list|,
name|ce_namelen
argument_list|(
name|ce
argument_list|)
argument_list|)
decl_stmt|;
while|while
condition|(
name|dir
operator|&&
operator|!
operator|(
name|dir
operator|->
name|nr
operator|++
operator|)
condition|)
name|dir
operator|=
name|dir
operator|->
name|parent
expr_stmt|;
block|}
end_function

begin_function
DECL|function|remove_dir_entry
specifier|static
name|void
name|remove_dir_entry
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
comment|/* 	 * Release reference to the directory entry (and parents if 0). 	 * 	 * Note: we do not remove / free the entry because there's no 	 * hash.[ch]::remove_hash and dir->next may point to other entries 	 * that are still valid, so we must not free the memory. 	 */
name|struct
name|dir_entry
modifier|*
name|dir
init|=
name|hash_dir_entry
argument_list|(
name|istate
argument_list|,
name|ce
argument_list|,
name|ce_namelen
argument_list|(
name|ce
argument_list|)
argument_list|)
decl_stmt|;
while|while
condition|(
name|dir
operator|&&
name|dir
operator|->
name|nr
operator|&&
operator|!
operator|(
operator|--
name|dir
operator|->
name|nr
operator|)
condition|)
name|dir
operator|=
name|dir
operator|->
name|parent
expr_stmt|;
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
name|memihash
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
if|if
condition|(
name|ignore_case
operator|&&
operator|!
operator|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_UNHASHED
operator|)
condition|)
name|add_dir_entry
argument_list|(
name|istate
argument_list|,
name|ce
argument_list|)
expr_stmt|;
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
if|if
condition|(
name|istate
operator|->
name|cache_nr
condition|)
name|preallocate_hash
argument_list|(
operator|&
name|istate
operator|->
name|name_hash
argument_list|,
name|istate
operator|->
name|cache_nr
argument_list|)
expr_stmt|;
name|hashmap_init
argument_list|(
operator|&
name|istate
operator|->
name|dir_hash
argument_list|,
operator|(
name|hashmap_cmp_fn
operator|)
name|dir_entry_cmp
argument_list|,
literal|0
argument_list|)
expr_stmt|;
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
comment|/* if already hashed, add reference to directory entries */
if|if
condition|(
name|ignore_case
operator|&&
operator|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_STATE_MASK
operator|)
operator|==
name|CE_STATE_MASK
condition|)
name|add_dir_entry
argument_list|(
name|istate
argument_list|,
name|ce
argument_list|)
expr_stmt|;
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

begin_comment
comment|/*  * We don't actually *remove* it, we can just mark it invalid so that  * we won't find it in lookups.  *  * Not only would we have to search the lists (simple enough), but  * we'd also have to rehash other hash buckets in case this makes the  * hash bucket empty (common). So it's much better to just mark  * it.  */
end_comment

begin_function
DECL|function|remove_name_hash
name|void
name|remove_name_hash
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
comment|/* if already hashed, release reference to directory entries */
if|if
condition|(
name|ignore_case
operator|&&
operator|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_STATE_MASK
operator|)
operator|==
name|CE_HASHED
condition|)
name|remove_dir_entry
argument_list|(
name|istate
argument_list|,
name|ce
argument_list|)
expr_stmt|;
name|ce
operator|->
name|ce_flags
operator||=
name|CE_UNHASHED
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
if|if
condition|(
operator|!
name|icase
condition|)
return|return
literal|0
return|;
return|return
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
DECL|function|index_dir_exists
name|struct
name|cache_entry
modifier|*
name|index_dir_exists
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
parameter_list|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|;
name|struct
name|dir_entry
modifier|*
name|dir
decl_stmt|;
name|lazy_init_name_hash
argument_list|(
name|istate
argument_list|)
expr_stmt|;
name|dir
operator|=
name|find_dir_entry
argument_list|(
name|istate
argument_list|,
name|name
argument_list|,
name|namelen
argument_list|)
expr_stmt|;
if|if
condition|(
name|dir
operator|&&
name|dir
operator|->
name|nr
condition|)
return|return
name|dir
operator|->
name|ce
return|;
comment|/* 	 * It might be a submodule. Unlike plain directories, which are stored 	 * in the dir-hash, submodules are stored in the name-hash, so check 	 * there, as well. 	 */
name|ce
operator|=
name|index_file_exists
argument_list|(
name|istate
argument_list|,
name|name
argument_list|,
name|namelen
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|ce
operator|&&
name|S_ISGITLINK
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
condition|)
return|return
name|ce
return|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|index_file_exists
name|struct
name|cache_entry
modifier|*
name|index_file_exists
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
name|memihash
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
if|if
condition|(
name|namelen
operator|>
literal|0
operator|&&
name|name
index|[
name|namelen
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
return|return
name|index_dir_exists
argument_list|(
name|istate
argument_list|,
name|name
argument_list|,
name|namelen
operator|-
literal|1
argument_list|)
return|;
return|return
name|index_file_exists
argument_list|(
name|istate
argument_list|,
name|name
argument_list|,
name|namelen
argument_list|,
name|icase
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|free_name_hash
name|void
name|free_name_hash
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|)
block|{
if|if
condition|(
operator|!
name|istate
operator|->
name|name_hash_initialized
condition|)
return|return;
name|istate
operator|->
name|name_hash_initialized
operator|=
literal|0
expr_stmt|;
name|free_hash
argument_list|(
operator|&
name|istate
operator|->
name|name_hash
argument_list|)
expr_stmt|;
name|hashmap_free
argument_list|(
operator|&
name|istate
operator|->
name|dir_hash
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

