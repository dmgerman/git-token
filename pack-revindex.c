begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"pack-revindex.h"
end_include

begin_comment
comment|/*  * Pack index for existing packs give us easy access to the offsets into  * corresponding pack file where each object's data starts, but the entries  * do not store the size of the compressed representation (uncompressed  * size is easily available by examining the pack entry header).  It is  * also rather expensive to find the sha1 for an object given its offset.  *  * We build a hashtable of existing packs (pack_revindex), and keep reverse  * index here -- pack index file is sorted by object name mapping to offset;  * this pack_revindex[].revindex array is a list of offset/index_nr pairs  * ordered by offset, so if you know the offset of an object, next offset  * is where its packed representation ends and the index_nr can be used to  * get the object sha1 from the main index.  */
end_comment

begin_struct
DECL|struct|pack_revindex
struct|struct
name|pack_revindex
block|{
DECL|member|p
name|struct
name|packed_git
modifier|*
name|p
decl_stmt|;
DECL|member|revindex
name|struct
name|revindex_entry
modifier|*
name|revindex
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|pack_revindex
specifier|static
name|struct
name|pack_revindex
modifier|*
name|pack_revindex
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pack_revindex_hashsz
specifier|static
name|int
name|pack_revindex_hashsz
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|pack_revindex_ix
specifier|static
name|int
name|pack_revindex_ix
parameter_list|(
name|struct
name|packed_git
modifier|*
name|p
parameter_list|)
block|{
name|unsigned
name|long
name|ui
init|=
operator|(
name|unsigned
name|long
operator|)
name|p
decl_stmt|;
name|int
name|i
decl_stmt|;
name|ui
operator|=
name|ui
operator|^
operator|(
name|ui
operator|>>
literal|16
operator|)
expr_stmt|;
comment|/* defeat structure alignment */
name|i
operator|=
call|(
name|int
call|)
argument_list|(
name|ui
operator|%
name|pack_revindex_hashsz
argument_list|)
expr_stmt|;
while|while
condition|(
name|pack_revindex
index|[
name|i
index|]
operator|.
name|p
condition|)
block|{
if|if
condition|(
name|pack_revindex
index|[
name|i
index|]
operator|.
name|p
operator|==
name|p
condition|)
return|return
name|i
return|;
if|if
condition|(
operator|++
name|i
operator|==
name|pack_revindex_hashsz
condition|)
name|i
operator|=
literal|0
expr_stmt|;
block|}
return|return
operator|-
literal|1
operator|-
name|i
return|;
block|}
end_function

begin_function
DECL|function|init_pack_revindex
specifier|static
name|void
name|init_pack_revindex
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|num
decl_stmt|;
name|struct
name|packed_git
modifier|*
name|p
decl_stmt|;
for|for
control|(
name|num
operator|=
literal|0
operator|,
name|p
operator|=
name|packed_git
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
name|num
operator|++
expr_stmt|;
if|if
condition|(
operator|!
name|num
condition|)
return|return;
name|pack_revindex_hashsz
operator|=
name|num
operator|*
literal|11
expr_stmt|;
name|pack_revindex
operator|=
name|xcalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|pack_revindex
argument_list|)
argument_list|,
name|pack_revindex_hashsz
argument_list|)
expr_stmt|;
for|for
control|(
name|p
operator|=
name|packed_git
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
name|num
operator|=
name|pack_revindex_ix
argument_list|(
name|p
argument_list|)
expr_stmt|;
name|num
operator|=
operator|-
literal|1
operator|-
name|num
expr_stmt|;
name|pack_revindex
index|[
name|num
index|]
operator|.
name|p
operator|=
name|p
expr_stmt|;
block|}
comment|/* revindex elements are lazily initialized */
block|}
end_function

begin_comment
comment|/*  * This is a least-significant-digit radix sort.  *  * It sorts each of the "n" items in "entries" by its offset field. The "max"  * parameter must be at least as large as the largest offset in the array,  * and lets us quit the sort early.  */
end_comment

begin_function
DECL|function|sort_revindex
specifier|static
name|void
name|sort_revindex
parameter_list|(
name|struct
name|revindex_entry
modifier|*
name|entries
parameter_list|,
name|unsigned
name|n
parameter_list|,
name|off_t
name|max
parameter_list|)
block|{
comment|/* 	 * We use a "digit" size of 16 bits. That keeps our memory 	 * usage reasonable, and we can generally (for a 4G or smaller 	 * packfile) quit after two rounds of radix-sorting. 	 */
DECL|macro|DIGIT_SIZE
define|#
directive|define
name|DIGIT_SIZE
value|(16)
DECL|macro|BUCKETS
define|#
directive|define
name|BUCKETS
value|(1<< DIGIT_SIZE)
comment|/* 	 * We want to know the bucket that a[i] will go into when we are using 	 * the digit that is N bits from the (least significant) end. 	 */
DECL|macro|BUCKET_FOR
define|#
directive|define
name|BUCKET_FOR
parameter_list|(
name|a
parameter_list|,
name|i
parameter_list|,
name|bits
parameter_list|)
value|(((a)[(i)].offset>> (bits))& (BUCKETS-1))
comment|/* 	 * We need O(n) temporary storage. Rather than do an extra copy of the 	 * partial results into "entries", we sort back and forth between the 	 * real array and temporary storage. In each iteration of the loop, we 	 * keep track of them with alias pointers, always sorting from "from" 	 * to "to". 	 */
name|struct
name|revindex_entry
modifier|*
name|tmp
init|=
name|xmalloc
argument_list|(
name|n
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|tmp
argument_list|)
argument_list|)
decl_stmt|;
name|struct
name|revindex_entry
modifier|*
name|from
init|=
name|entries
decl_stmt|,
modifier|*
name|to
init|=
name|tmp
decl_stmt|;
name|int
name|bits
decl_stmt|;
name|unsigned
modifier|*
name|pos
init|=
name|xmalloc
argument_list|(
name|BUCKETS
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|pos
argument_list|)
argument_list|)
decl_stmt|;
comment|/* 	 * If (max>> bits) is zero, then we know that the radix digit we are 	 * on (and any higher) will be zero for all entries, and our loop will 	 * be a no-op, as everybody lands in the same zero-th bucket. 	 */
for|for
control|(
name|bits
operator|=
literal|0
init|;
name|max
operator|>>
name|bits
condition|;
name|bits
operator|+=
name|DIGIT_SIZE
control|)
block|{
name|struct
name|revindex_entry
modifier|*
name|swap
decl_stmt|;
name|unsigned
name|i
decl_stmt|;
name|memset
argument_list|(
name|pos
argument_list|,
literal|0
argument_list|,
name|BUCKETS
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|pos
argument_list|)
argument_list|)
expr_stmt|;
comment|/* 		 * We want pos[i] to store the index of the last element that 		 * will go in bucket "i" (actually one past the last element). 		 * To do this, we first count the items that will go in each 		 * bucket, which gives us a relative offset from the last 		 * bucket. We can then cumulatively add the index from the 		 * previous bucket to get the true index. 		 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|n
condition|;
name|i
operator|++
control|)
name|pos
index|[
name|BUCKET_FOR
argument_list|(
name|from
argument_list|,
name|i
argument_list|,
name|bits
argument_list|)
index|]
operator|++
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|BUCKETS
condition|;
name|i
operator|++
control|)
name|pos
index|[
name|i
index|]
operator|+=
name|pos
index|[
name|i
operator|-
literal|1
index|]
expr_stmt|;
comment|/* 		 * Now we can drop the elements into their correct buckets (in 		 * our temporary array).  We iterate the pos counter backwards 		 * to avoid using an extra index to count up. And since we are 		 * going backwards there, we must also go backwards through the 		 * array itself, to keep the sort stable. 		 * 		 * Note that we use an unsigned iterator to make sure we can 		 * handle 2^32-1 objects, even on a 32-bit system. But this 		 * means we cannot use the more obvious "i>= 0" loop condition 		 * for counting backwards, and must instead check for 		 * wrap-around with UINT_MAX. 		 */
for|for
control|(
name|i
operator|=
name|n
operator|-
literal|1
init|;
name|i
operator|!=
name|UINT_MAX
condition|;
name|i
operator|--
control|)
name|to
index|[
operator|--
name|pos
index|[
name|BUCKET_FOR
argument_list|(
name|from
argument_list|,
name|i
argument_list|,
name|bits
argument_list|)
index|]
index|]
operator|=
name|from
index|[
name|i
index|]
expr_stmt|;
comment|/* 		 * Now "to" contains the most sorted list, so we swap "from" and 		 * "to" for the next iteration. 		 */
name|swap
operator|=
name|from
expr_stmt|;
name|from
operator|=
name|to
expr_stmt|;
name|to
operator|=
name|swap
expr_stmt|;
block|}
comment|/* 	 * If we ended with our data in the original array, great. If not, 	 * we have to move it back from the temporary storage. 	 */
if|if
condition|(
name|from
operator|!=
name|entries
condition|)
name|memcpy
argument_list|(
name|entries
argument_list|,
name|tmp
argument_list|,
name|n
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|entries
argument_list|)
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tmp
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|pos
argument_list|)
expr_stmt|;
DECL|macro|BUCKET_FOR
undef|#
directive|undef
name|BUCKET_FOR
DECL|macro|BUCKETS
undef|#
directive|undef
name|BUCKETS
DECL|macro|DIGIT_SIZE
undef|#
directive|undef
name|DIGIT_SIZE
block|}
end_function

begin_comment
comment|/*  * Ordered list of offsets of objects in the pack.  */
end_comment

begin_function
DECL|function|create_pack_revindex
specifier|static
name|void
name|create_pack_revindex
parameter_list|(
name|struct
name|pack_revindex
modifier|*
name|rix
parameter_list|)
block|{
name|struct
name|packed_git
modifier|*
name|p
init|=
name|rix
operator|->
name|p
decl_stmt|;
name|unsigned
name|num_ent
init|=
name|p
operator|->
name|num_objects
decl_stmt|;
name|unsigned
name|i
decl_stmt|;
specifier|const
name|char
modifier|*
name|index
init|=
name|p
operator|->
name|index_data
decl_stmt|;
name|rix
operator|->
name|revindex
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|rix
operator|->
name|revindex
argument_list|)
operator|*
operator|(
name|num_ent
operator|+
literal|1
operator|)
argument_list|)
expr_stmt|;
name|index
operator|+=
literal|4
operator|*
literal|256
expr_stmt|;
if|if
condition|(
name|p
operator|->
name|index_version
operator|>
literal|1
condition|)
block|{
specifier|const
name|uint32_t
modifier|*
name|off_32
init|=
operator|(
name|uint32_t
operator|*
operator|)
operator|(
name|index
operator|+
literal|8
operator|+
name|p
operator|->
name|num_objects
operator|*
operator|(
literal|20
operator|+
literal|4
operator|)
operator|)
decl_stmt|;
specifier|const
name|uint32_t
modifier|*
name|off_64
init|=
name|off_32
operator|+
name|p
operator|->
name|num_objects
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|num_ent
condition|;
name|i
operator|++
control|)
block|{
name|uint32_t
name|off
init|=
name|ntohl
argument_list|(
operator|*
name|off_32
operator|++
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|off
operator|&
literal|0x80000000
operator|)
condition|)
block|{
name|rix
operator|->
name|revindex
index|[
name|i
index|]
operator|.
name|offset
operator|=
name|off
expr_stmt|;
block|}
else|else
block|{
name|rix
operator|->
name|revindex
index|[
name|i
index|]
operator|.
name|offset
operator|=
operator|(
operator|(
name|uint64_t
operator|)
name|ntohl
argument_list|(
operator|*
name|off_64
operator|++
argument_list|)
operator|)
operator|<<
literal|32
expr_stmt|;
name|rix
operator|->
name|revindex
index|[
name|i
index|]
operator|.
name|offset
operator||=
name|ntohl
argument_list|(
operator|*
name|off_64
operator|++
argument_list|)
expr_stmt|;
block|}
name|rix
operator|->
name|revindex
index|[
name|i
index|]
operator|.
name|nr
operator|=
name|i
expr_stmt|;
block|}
block|}
else|else
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|num_ent
condition|;
name|i
operator|++
control|)
block|{
name|uint32_t
name|hl
init|=
operator|*
operator|(
operator|(
name|uint32_t
operator|*
operator|)
operator|(
name|index
operator|+
literal|24
operator|*
name|i
operator|)
operator|)
decl_stmt|;
name|rix
operator|->
name|revindex
index|[
name|i
index|]
operator|.
name|offset
operator|=
name|ntohl
argument_list|(
name|hl
argument_list|)
expr_stmt|;
name|rix
operator|->
name|revindex
index|[
name|i
index|]
operator|.
name|nr
operator|=
name|i
expr_stmt|;
block|}
block|}
comment|/* This knows the pack format -- the 20-byte trailer 	 * follows immediately after the last object data. 	 */
name|rix
operator|->
name|revindex
index|[
name|num_ent
index|]
operator|.
name|offset
operator|=
name|p
operator|->
name|pack_size
operator|-
literal|20
expr_stmt|;
name|rix
operator|->
name|revindex
index|[
name|num_ent
index|]
operator|.
name|nr
operator|=
operator|-
literal|1
expr_stmt|;
name|sort_revindex
argument_list|(
name|rix
operator|->
name|revindex
argument_list|,
name|num_ent
argument_list|,
name|p
operator|->
name|pack_size
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|find_pack_revindex
name|struct
name|revindex_entry
modifier|*
name|find_pack_revindex
parameter_list|(
name|struct
name|packed_git
modifier|*
name|p
parameter_list|,
name|off_t
name|ofs
parameter_list|)
block|{
name|int
name|num
decl_stmt|;
name|unsigned
name|lo
decl_stmt|,
name|hi
decl_stmt|;
name|struct
name|pack_revindex
modifier|*
name|rix
decl_stmt|;
name|struct
name|revindex_entry
modifier|*
name|revindex
decl_stmt|;
if|if
condition|(
operator|!
name|pack_revindex_hashsz
condition|)
name|init_pack_revindex
argument_list|()
expr_stmt|;
name|num
operator|=
name|pack_revindex_ix
argument_list|(
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
name|num
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"internal error: pack revindex fubar"
argument_list|)
expr_stmt|;
name|rix
operator|=
operator|&
name|pack_revindex
index|[
name|num
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|rix
operator|->
name|revindex
condition|)
name|create_pack_revindex
argument_list|(
name|rix
argument_list|)
expr_stmt|;
name|revindex
operator|=
name|rix
operator|->
name|revindex
expr_stmt|;
name|lo
operator|=
literal|0
expr_stmt|;
name|hi
operator|=
name|p
operator|->
name|num_objects
operator|+
literal|1
expr_stmt|;
do|do
block|{
name|unsigned
name|mi
init|=
name|lo
operator|+
operator|(
name|hi
operator|-
name|lo
operator|)
operator|/
literal|2
decl_stmt|;
if|if
condition|(
name|revindex
index|[
name|mi
index|]
operator|.
name|offset
operator|==
name|ofs
condition|)
block|{
return|return
name|revindex
operator|+
name|mi
return|;
block|}
elseif|else
if|if
condition|(
name|ofs
operator|<
name|revindex
index|[
name|mi
index|]
operator|.
name|offset
condition|)
name|hi
operator|=
name|mi
expr_stmt|;
else|else
name|lo
operator|=
name|mi
operator|+
literal|1
expr_stmt|;
block|}
do|while
condition|(
name|lo
operator|<
name|hi
condition|)
do|;
name|error
argument_list|(
literal|"bad offset for revindex"
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|discard_revindex
name|void
name|discard_revindex
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
name|pack_revindex_hashsz
condition|)
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
name|pack_revindex_hashsz
condition|;
name|i
operator|++
control|)
name|free
argument_list|(
name|pack_revindex
index|[
name|i
index|]
operator|.
name|revindex
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|pack_revindex
argument_list|)
expr_stmt|;
name|pack_revindex_hashsz
operator|=
literal|0
expr_stmt|;
block|}
block|}
end_function

end_unit

