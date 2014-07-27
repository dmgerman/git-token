begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"split-index.h"
end_include

begin_include
include|#
directive|include
file|"ewah/ewok.h"
end_include

begin_function
DECL|function|init_split_index
name|struct
name|split_index
modifier|*
name|init_split_index
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
name|split_index
condition|)
block|{
name|istate
operator|->
name|split_index
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|istate
operator|->
name|split_index
argument_list|)
argument_list|)
expr_stmt|;
name|istate
operator|->
name|split_index
operator|->
name|refcount
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|istate
operator|->
name|split_index
return|;
block|}
end_function

begin_function
DECL|function|read_link_extension
name|int
name|read_link_extension
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|,
specifier|const
name|void
modifier|*
name|data_
parameter_list|,
name|unsigned
name|long
name|sz
parameter_list|)
block|{
specifier|const
name|unsigned
name|char
modifier|*
name|data
init|=
name|data_
decl_stmt|;
name|struct
name|split_index
modifier|*
name|si
decl_stmt|;
name|int
name|ret
decl_stmt|;
if|if
condition|(
name|sz
operator|<
literal|20
condition|)
return|return
name|error
argument_list|(
literal|"corrupt link extension (too short)"
argument_list|)
return|;
name|si
operator|=
name|init_split_index
argument_list|(
name|istate
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|si
operator|->
name|base_sha1
argument_list|,
name|data
argument_list|)
expr_stmt|;
name|data
operator|+=
literal|20
expr_stmt|;
name|sz
operator|-=
literal|20
expr_stmt|;
if|if
condition|(
operator|!
name|sz
condition|)
return|return
literal|0
return|;
name|si
operator|->
name|delete_bitmap
operator|=
name|ewah_new
argument_list|()
expr_stmt|;
name|ret
operator|=
name|ewah_read_mmap
argument_list|(
name|si
operator|->
name|delete_bitmap
argument_list|,
name|data
argument_list|,
name|sz
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"corrupt delete bitmap in link extension"
argument_list|)
return|;
name|data
operator|+=
name|ret
expr_stmt|;
name|sz
operator|-=
name|ret
expr_stmt|;
name|si
operator|->
name|replace_bitmap
operator|=
name|ewah_new
argument_list|()
expr_stmt|;
name|ret
operator|=
name|ewah_read_mmap
argument_list|(
name|si
operator|->
name|replace_bitmap
argument_list|,
name|data
argument_list|,
name|sz
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"corrupt replace bitmap in link extension"
argument_list|)
return|;
if|if
condition|(
name|ret
operator|!=
name|sz
condition|)
return|return
name|error
argument_list|(
literal|"garbage at the end of link extension"
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|write_strbuf
specifier|static
name|int
name|write_strbuf
parameter_list|(
name|void
modifier|*
name|user_data
parameter_list|,
specifier|const
name|void
modifier|*
name|data
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|struct
name|strbuf
modifier|*
name|sb
init|=
name|user_data
decl_stmt|;
name|strbuf_add
argument_list|(
name|sb
argument_list|,
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
return|return
name|len
return|;
block|}
end_function

begin_function
DECL|function|write_link_extension
name|int
name|write_link_extension
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|struct
name|index_state
modifier|*
name|istate
parameter_list|)
block|{
name|struct
name|split_index
modifier|*
name|si
init|=
name|istate
operator|->
name|split_index
decl_stmt|;
name|strbuf_add
argument_list|(
name|sb
argument_list|,
name|si
operator|->
name|base_sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|si
operator|->
name|delete_bitmap
operator|&&
operator|!
name|si
operator|->
name|replace_bitmap
condition|)
return|return
literal|0
return|;
name|ewah_serialize_to
argument_list|(
name|si
operator|->
name|delete_bitmap
argument_list|,
name|write_strbuf
argument_list|,
name|sb
argument_list|)
expr_stmt|;
name|ewah_serialize_to
argument_list|(
name|si
operator|->
name|replace_bitmap
argument_list|,
name|write_strbuf
argument_list|,
name|sb
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|mark_base_index_entries
specifier|static
name|void
name|mark_base_index_entries
parameter_list|(
name|struct
name|index_state
modifier|*
name|base
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
comment|/* 	 * To keep track of the shared entries between 	 * istate->base->cache[] and istate->cache[], base entry 	 * position is stored in each base entry. All positions start 	 * from 1 instead of 0, which is resrved to say "this is a new 	 * entry". 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|base
operator|->
name|cache_nr
condition|;
name|i
operator|++
control|)
name|base
operator|->
name|cache
index|[
name|i
index|]
operator|->
name|index
operator|=
name|i
operator|+
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|move_cache_to_base_index
name|void
name|move_cache_to_base_index
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|)
block|{
name|struct
name|split_index
modifier|*
name|si
init|=
name|istate
operator|->
name|split_index
decl_stmt|;
name|int
name|i
decl_stmt|;
comment|/* 	 * do not delete old si->base, its index entries may be shared 	 * with istate->cache[]. Accept a bit of leaking here because 	 * this code is only used by short-lived update-index. 	 */
name|si
operator|->
name|base
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|si
operator|->
name|base
argument_list|)
argument_list|)
expr_stmt|;
name|si
operator|->
name|base
operator|->
name|version
operator|=
name|istate
operator|->
name|version
expr_stmt|;
comment|/* zero timestamp disables racy test in ce_write_index() */
name|si
operator|->
name|base
operator|->
name|timestamp
operator|=
name|istate
operator|->
name|timestamp
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|si
operator|->
name|base
operator|->
name|cache
argument_list|,
name|istate
operator|->
name|cache_nr
argument_list|,
name|si
operator|->
name|base
operator|->
name|cache_alloc
argument_list|)
expr_stmt|;
name|si
operator|->
name|base
operator|->
name|cache_nr
operator|=
name|istate
operator|->
name|cache_nr
expr_stmt|;
name|memcpy
argument_list|(
name|si
operator|->
name|base
operator|->
name|cache
argument_list|,
name|istate
operator|->
name|cache
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|istate
operator|->
name|cache
argument_list|)
operator|*
name|istate
operator|->
name|cache_nr
argument_list|)
expr_stmt|;
name|mark_base_index_entries
argument_list|(
name|si
operator|->
name|base
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
name|si
operator|->
name|base
operator|->
name|cache_nr
condition|;
name|i
operator|++
control|)
name|si
operator|->
name|base
operator|->
name|cache
index|[
name|i
index|]
operator|->
name|ce_flags
operator|&=
operator|~
name|CE_UPDATE_IN_BASE
expr_stmt|;
block|}
end_function

begin_function
DECL|function|mark_entry_for_delete
specifier|static
name|void
name|mark_entry_for_delete
parameter_list|(
name|size_t
name|pos
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|struct
name|index_state
modifier|*
name|istate
init|=
name|data
decl_stmt|;
if|if
condition|(
name|pos
operator|>=
name|istate
operator|->
name|cache_nr
condition|)
name|die
argument_list|(
literal|"position for delete %d exceeds base index size %d"
argument_list|,
operator|(
name|int
operator|)
name|pos
argument_list|,
name|istate
operator|->
name|cache_nr
argument_list|)
expr_stmt|;
name|istate
operator|->
name|cache
index|[
name|pos
index|]
operator|->
name|ce_flags
operator||=
name|CE_REMOVE
expr_stmt|;
name|istate
operator|->
name|split_index
operator|->
name|nr_deletions
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|replace_entry
specifier|static
name|void
name|replace_entry
parameter_list|(
name|size_t
name|pos
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|struct
name|index_state
modifier|*
name|istate
init|=
name|data
decl_stmt|;
name|struct
name|split_index
modifier|*
name|si
init|=
name|istate
operator|->
name|split_index
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|dst
decl_stmt|,
modifier|*
name|src
decl_stmt|;
if|if
condition|(
name|pos
operator|>=
name|istate
operator|->
name|cache_nr
condition|)
name|die
argument_list|(
literal|"position for replacement %d exceeds base index size %d"
argument_list|,
operator|(
name|int
operator|)
name|pos
argument_list|,
name|istate
operator|->
name|cache_nr
argument_list|)
expr_stmt|;
if|if
condition|(
name|si
operator|->
name|nr_replacements
operator|>=
name|si
operator|->
name|saved_cache_nr
condition|)
name|die
argument_list|(
literal|"too many replacements (%d vs %d)"
argument_list|,
name|si
operator|->
name|nr_replacements
argument_list|,
name|si
operator|->
name|saved_cache_nr
argument_list|)
expr_stmt|;
name|dst
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
name|dst
operator|->
name|ce_flags
operator|&
name|CE_REMOVE
condition|)
name|die
argument_list|(
literal|"entry %d is marked as both replaced and deleted"
argument_list|,
operator|(
name|int
operator|)
name|pos
argument_list|)
expr_stmt|;
name|src
operator|=
name|si
operator|->
name|saved_cache
index|[
name|si
operator|->
name|nr_replacements
index|]
expr_stmt|;
if|if
condition|(
name|ce_namelen
argument_list|(
name|src
argument_list|)
condition|)
name|die
argument_list|(
literal|"corrupt link extension, entry %d should have "
literal|"zero length name"
argument_list|,
operator|(
name|int
operator|)
name|pos
argument_list|)
expr_stmt|;
name|src
operator|->
name|index
operator|=
name|pos
operator|+
literal|1
expr_stmt|;
name|src
operator|->
name|ce_flags
operator||=
name|CE_UPDATE_IN_BASE
expr_stmt|;
name|src
operator|->
name|ce_namelen
operator|=
name|dst
operator|->
name|ce_namelen
expr_stmt|;
name|copy_cache_entry
argument_list|(
name|dst
argument_list|,
name|src
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|src
argument_list|)
expr_stmt|;
name|si
operator|->
name|nr_replacements
operator|++
expr_stmt|;
block|}
end_function

begin_function
DECL|function|merge_base_index
name|void
name|merge_base_index
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|)
block|{
name|struct
name|split_index
modifier|*
name|si
init|=
name|istate
operator|->
name|split_index
decl_stmt|;
name|unsigned
name|int
name|i
decl_stmt|;
name|mark_base_index_entries
argument_list|(
name|si
operator|->
name|base
argument_list|)
expr_stmt|;
name|si
operator|->
name|saved_cache
operator|=
name|istate
operator|->
name|cache
expr_stmt|;
name|si
operator|->
name|saved_cache_nr
operator|=
name|istate
operator|->
name|cache_nr
expr_stmt|;
name|istate
operator|->
name|cache_nr
operator|=
name|si
operator|->
name|base
operator|->
name|cache_nr
expr_stmt|;
name|istate
operator|->
name|cache
operator|=
name|NULL
expr_stmt|;
name|istate
operator|->
name|cache_alloc
operator|=
literal|0
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|istate
operator|->
name|cache
argument_list|,
name|istate
operator|->
name|cache_nr
argument_list|,
name|istate
operator|->
name|cache_alloc
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|istate
operator|->
name|cache
argument_list|,
name|si
operator|->
name|base
operator|->
name|cache
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|istate
operator|->
name|cache
argument_list|)
operator|*
name|istate
operator|->
name|cache_nr
argument_list|)
expr_stmt|;
name|si
operator|->
name|nr_deletions
operator|=
literal|0
expr_stmt|;
name|si
operator|->
name|nr_replacements
operator|=
literal|0
expr_stmt|;
name|ewah_each_bit
argument_list|(
name|si
operator|->
name|replace_bitmap
argument_list|,
name|replace_entry
argument_list|,
name|istate
argument_list|)
expr_stmt|;
name|ewah_each_bit
argument_list|(
name|si
operator|->
name|delete_bitmap
argument_list|,
name|mark_entry_for_delete
argument_list|,
name|istate
argument_list|)
expr_stmt|;
if|if
condition|(
name|si
operator|->
name|nr_deletions
condition|)
name|remove_marked_cache_entries
argument_list|(
name|istate
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
name|si
operator|->
name|nr_replacements
init|;
name|i
operator|<
name|si
operator|->
name|saved_cache_nr
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|ce_namelen
argument_list|(
name|si
operator|->
name|saved_cache
index|[
name|i
index|]
argument_list|)
condition|)
name|die
argument_list|(
literal|"corrupt link extension, entry %d should "
literal|"have non-zero length name"
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|add_index_entry
argument_list|(
name|istate
argument_list|,
name|si
operator|->
name|saved_cache
index|[
name|i
index|]
argument_list|,
name|ADD_CACHE_OK_TO_ADD
operator||
name|ADD_CACHE_KEEP_CACHE_TREE
operator||
comment|/* 				 * we may have to replay what 				 * merge-recursive.c:update_stages() 				 * does, which has this flag on 				 */
name|ADD_CACHE_SKIP_DFCHECK
argument_list|)
expr_stmt|;
name|si
operator|->
name|saved_cache
index|[
name|i
index|]
operator|=
name|NULL
expr_stmt|;
block|}
name|ewah_free
argument_list|(
name|si
operator|->
name|delete_bitmap
argument_list|)
expr_stmt|;
name|ewah_free
argument_list|(
name|si
operator|->
name|replace_bitmap
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|si
operator|->
name|saved_cache
argument_list|)
expr_stmt|;
name|si
operator|->
name|delete_bitmap
operator|=
name|NULL
expr_stmt|;
name|si
operator|->
name|replace_bitmap
operator|=
name|NULL
expr_stmt|;
name|si
operator|->
name|saved_cache
operator|=
name|NULL
expr_stmt|;
name|si
operator|->
name|saved_cache_nr
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|prepare_to_write_split_index
name|void
name|prepare_to_write_split_index
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|)
block|{
name|struct
name|split_index
modifier|*
name|si
init|=
name|init_split_index
argument_list|(
name|istate
argument_list|)
decl_stmt|;
name|struct
name|cache_entry
modifier|*
modifier|*
name|entries
init|=
name|NULL
decl_stmt|,
modifier|*
name|ce
decl_stmt|;
name|int
name|i
decl_stmt|,
name|nr_entries
init|=
literal|0
decl_stmt|,
name|nr_alloc
init|=
literal|0
decl_stmt|;
name|si
operator|->
name|delete_bitmap
operator|=
name|ewah_new
argument_list|()
expr_stmt|;
name|si
operator|->
name|replace_bitmap
operator|=
name|ewah_new
argument_list|()
expr_stmt|;
if|if
condition|(
name|si
operator|->
name|base
condition|)
block|{
comment|/* Go through istate->cache[] and mark CE_MATCHED to 		 * entry with positive index. We'll go through 		 * base->cache[] later to delete all entries in base 		 * that are not marked eith either CE_MATCHED or 		 * CE_UPDATE_IN_BASE. If istate->cache[i] is a 		 * duplicate, deduplicate it. 		 */
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
name|base
decl_stmt|;
comment|/* namelen is checked separately */
specifier|const
name|unsigned
name|int
name|ondisk_flags
init|=
name|CE_STAGEMASK
operator||
name|CE_VALID
operator||
name|CE_EXTENDED_FLAGS
decl_stmt|;
name|unsigned
name|int
name|ce_flags
decl_stmt|,
name|base_flags
decl_stmt|,
name|ret
decl_stmt|;
name|ce
operator|=
name|istate
operator|->
name|cache
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|ce
operator|->
name|index
condition|)
continue|continue;
if|if
condition|(
name|ce
operator|->
name|index
operator|>
name|si
operator|->
name|base
operator|->
name|cache_nr
condition|)
block|{
name|ce
operator|->
name|index
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
name|ce
operator|->
name|ce_flags
operator||=
name|CE_MATCHED
expr_stmt|;
comment|/* or "shared" */
name|base
operator|=
name|si
operator|->
name|base
operator|->
name|cache
index|[
name|ce
operator|->
name|index
operator|-
literal|1
index|]
expr_stmt|;
if|if
condition|(
name|ce
operator|==
name|base
condition|)
continue|continue;
if|if
condition|(
name|ce
operator|->
name|ce_namelen
operator|!=
name|base
operator|->
name|ce_namelen
operator|||
name|strcmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|base
operator|->
name|name
argument_list|)
condition|)
block|{
name|ce
operator|->
name|index
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
name|ce_flags
operator|=
name|ce
operator|->
name|ce_flags
expr_stmt|;
name|base_flags
operator|=
name|base
operator|->
name|ce_flags
expr_stmt|;
comment|/* only on-disk flags matter */
name|ce
operator|->
name|ce_flags
operator|&=
name|ondisk_flags
expr_stmt|;
name|base
operator|->
name|ce_flags
operator|&=
name|ondisk_flags
expr_stmt|;
name|ret
operator|=
name|memcmp
argument_list|(
operator|&
name|ce
operator|->
name|ce_stat_data
argument_list|,
operator|&
name|base
operator|->
name|ce_stat_data
argument_list|,
name|offsetof
argument_list|(
expr|struct
name|cache_entry
argument_list|,
name|name
argument_list|)
operator|-
name|offsetof
argument_list|(
expr|struct
name|cache_entry
argument_list|,
name|ce_stat_data
argument_list|)
argument_list|)
expr_stmt|;
name|ce
operator|->
name|ce_flags
operator|=
name|ce_flags
expr_stmt|;
name|base
operator|->
name|ce_flags
operator|=
name|base_flags
expr_stmt|;
if|if
condition|(
name|ret
condition|)
name|ce
operator|->
name|ce_flags
operator||=
name|CE_UPDATE_IN_BASE
expr_stmt|;
name|free
argument_list|(
name|base
argument_list|)
expr_stmt|;
name|si
operator|->
name|base
operator|->
name|cache
index|[
name|ce
operator|->
name|index
operator|-
literal|1
index|]
operator|=
name|ce
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
name|si
operator|->
name|base
operator|->
name|cache_nr
condition|;
name|i
operator|++
control|)
block|{
name|ce
operator|=
name|si
operator|->
name|base
operator|->
name|cache
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
operator|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_REMOVE
operator|)
operator|||
operator|!
operator|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_MATCHED
operator|)
condition|)
name|ewah_set
argument_list|(
name|si
operator|->
name|delete_bitmap
argument_list|,
name|i
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_UPDATE_IN_BASE
condition|)
block|{
name|ewah_set
argument_list|(
name|si
operator|->
name|replace_bitmap
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|ce
operator|->
name|ce_flags
operator||=
name|CE_STRIP_NAME
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|entries
argument_list|,
name|nr_entries
operator|+
literal|1
argument_list|,
name|nr_alloc
argument_list|)
expr_stmt|;
name|entries
index|[
name|nr_entries
operator|++
index|]
operator|=
name|ce
expr_stmt|;
block|}
block|}
block|}
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
name|ce
operator|=
name|istate
operator|->
name|cache
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
operator|(
operator|!
name|si
operator|->
name|base
operator|||
operator|!
name|ce
operator|->
name|index
operator|)
operator|&&
operator|!
operator|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_REMOVE
operator|)
condition|)
block|{
name|assert
argument_list|(
operator|!
operator|(
name|ce
operator|->
name|ce_flags
operator|&
name|CE_STRIP_NAME
operator|)
argument_list|)
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|entries
argument_list|,
name|nr_entries
operator|+
literal|1
argument_list|,
name|nr_alloc
argument_list|)
expr_stmt|;
name|entries
index|[
name|nr_entries
operator|++
index|]
operator|=
name|ce
expr_stmt|;
block|}
name|ce
operator|->
name|ce_flags
operator|&=
operator|~
name|CE_MATCHED
expr_stmt|;
block|}
comment|/* 	 * take cache[] out temporarily, put entries[] in its place 	 * for writing 	 */
name|si
operator|->
name|saved_cache
operator|=
name|istate
operator|->
name|cache
expr_stmt|;
name|si
operator|->
name|saved_cache_nr
operator|=
name|istate
operator|->
name|cache_nr
expr_stmt|;
name|istate
operator|->
name|cache
operator|=
name|entries
expr_stmt|;
name|istate
operator|->
name|cache_nr
operator|=
name|nr_entries
expr_stmt|;
block|}
end_function

begin_function
DECL|function|finish_writing_split_index
name|void
name|finish_writing_split_index
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|)
block|{
name|struct
name|split_index
modifier|*
name|si
init|=
name|init_split_index
argument_list|(
name|istate
argument_list|)
decl_stmt|;
name|ewah_free
argument_list|(
name|si
operator|->
name|delete_bitmap
argument_list|)
expr_stmt|;
name|ewah_free
argument_list|(
name|si
operator|->
name|replace_bitmap
argument_list|)
expr_stmt|;
name|si
operator|->
name|delete_bitmap
operator|=
name|NULL
expr_stmt|;
name|si
operator|->
name|replace_bitmap
operator|=
name|NULL
expr_stmt|;
name|free
argument_list|(
name|istate
operator|->
name|cache
argument_list|)
expr_stmt|;
name|istate
operator|->
name|cache
operator|=
name|si
operator|->
name|saved_cache
expr_stmt|;
name|istate
operator|->
name|cache_nr
operator|=
name|si
operator|->
name|saved_cache_nr
expr_stmt|;
block|}
end_function

begin_function
DECL|function|discard_split_index
name|void
name|discard_split_index
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|)
block|{
name|struct
name|split_index
modifier|*
name|si
init|=
name|istate
operator|->
name|split_index
decl_stmt|;
if|if
condition|(
operator|!
name|si
condition|)
return|return;
name|istate
operator|->
name|split_index
operator|=
name|NULL
expr_stmt|;
name|si
operator|->
name|refcount
operator|--
expr_stmt|;
if|if
condition|(
name|si
operator|->
name|refcount
condition|)
return|return;
if|if
condition|(
name|si
operator|->
name|base
condition|)
block|{
name|discard_index
argument_list|(
name|si
operator|->
name|base
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|si
operator|->
name|base
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|si
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|save_or_free_index_entry
name|void
name|save_or_free_index_entry
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
if|if
condition|(
name|ce
operator|->
name|index
operator|&&
name|istate
operator|->
name|split_index
operator|&&
name|istate
operator|->
name|split_index
operator|->
name|base
operator|&&
name|ce
operator|->
name|index
operator|<=
name|istate
operator|->
name|split_index
operator|->
name|base
operator|->
name|cache_nr
operator|&&
name|ce
operator|==
name|istate
operator|->
name|split_index
operator|->
name|base
operator|->
name|cache
index|[
name|ce
operator|->
name|index
operator|-
literal|1
index|]
condition|)
name|ce
operator|->
name|ce_flags
operator||=
name|CE_REMOVE
expr_stmt|;
else|else
name|free
argument_list|(
name|ce
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|replace_index_entry_in_base
name|void
name|replace_index_entry_in_base
parameter_list|(
name|struct
name|index_state
modifier|*
name|istate
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|old
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|new
parameter_list|)
block|{
if|if
condition|(
name|old
operator|->
name|index
operator|&&
name|istate
operator|->
name|split_index
operator|&&
name|istate
operator|->
name|split_index
operator|->
name|base
operator|&&
name|old
operator|->
name|index
operator|<=
name|istate
operator|->
name|split_index
operator|->
name|base
operator|->
name|cache_nr
condition|)
block|{
name|new
operator|->
name|index
operator|=
name|old
operator|->
name|index
expr_stmt|;
if|if
condition|(
name|old
operator|!=
name|istate
operator|->
name|split_index
operator|->
name|base
operator|->
name|cache
index|[
name|new
operator|->
name|index
operator|-
literal|1
index|]
condition|)
name|free
argument_list|(
name|istate
operator|->
name|split_index
operator|->
name|base
operator|->
name|cache
index|[
name|new
operator|->
name|index
operator|-
literal|1
index|]
argument_list|)
expr_stmt|;
name|istate
operator|->
name|split_index
operator|->
name|base
operator|->
name|cache
index|[
name|new
operator|->
name|index
operator|-
literal|1
index|]
operator|=
name|new
expr_stmt|;
block|}
block|}
end_function

end_unit
