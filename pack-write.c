begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"pack.h"
end_include

begin_include
include|#
directive|include
file|"csum-file.h"
end_include

begin_function
DECL|function|reset_pack_idx_option
name|void
name|reset_pack_idx_option
parameter_list|(
name|struct
name|pack_idx_option
modifier|*
name|opts
parameter_list|)
block|{
name|memset
argument_list|(
name|opts
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|opts
argument_list|)
argument_list|)
expr_stmt|;
name|opts
operator|->
name|version
operator|=
literal|2
expr_stmt|;
name|opts
operator|->
name|off32_limit
operator|=
literal|0x7fffffff
expr_stmt|;
block|}
end_function

begin_function
DECL|function|sha1_compare
specifier|static
name|int
name|sha1_compare
parameter_list|(
specifier|const
name|void
modifier|*
name|_a
parameter_list|,
specifier|const
name|void
modifier|*
name|_b
parameter_list|)
block|{
name|struct
name|pack_idx_entry
modifier|*
name|a
init|=
operator|*
operator|(
expr|struct
name|pack_idx_entry
operator|*
operator|*
operator|)
name|_a
decl_stmt|;
name|struct
name|pack_idx_entry
modifier|*
name|b
init|=
operator|*
operator|(
expr|struct
name|pack_idx_entry
operator|*
operator|*
operator|)
name|_b
decl_stmt|;
return|return
name|hashcmp
argument_list|(
name|a
operator|->
name|sha1
argument_list|,
name|b
operator|->
name|sha1
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|cmp_uint32
specifier|static
name|int
name|cmp_uint32
parameter_list|(
specifier|const
name|void
modifier|*
name|a_
parameter_list|,
specifier|const
name|void
modifier|*
name|b_
parameter_list|)
block|{
name|uint32_t
name|a
init|=
operator|*
operator|(
operator|(
name|uint32_t
operator|*
operator|)
name|a_
operator|)
decl_stmt|;
name|uint32_t
name|b
init|=
operator|*
operator|(
operator|(
name|uint32_t
operator|*
operator|)
name|b_
operator|)
decl_stmt|;
return|return
operator|(
name|a
operator|<
name|b
operator|)
condition|?
operator|-
literal|1
else|:
operator|(
name|a
operator|!=
name|b
operator|)
return|;
block|}
end_function

begin_function
DECL|function|need_large_offset
specifier|static
name|int
name|need_large_offset
parameter_list|(
name|off_t
name|offset
parameter_list|,
specifier|const
name|struct
name|pack_idx_option
modifier|*
name|opts
parameter_list|)
block|{
name|uint32_t
name|ofsval
decl_stmt|;
if|if
condition|(
operator|(
name|offset
operator|>>
literal|31
operator|)
operator|||
operator|(
name|opts
operator|->
name|off32_limit
operator|<
name|offset
operator|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
operator|!
name|opts
operator|->
name|anomaly_nr
condition|)
return|return
literal|0
return|;
name|ofsval
operator|=
name|offset
expr_stmt|;
return|return
operator|!
operator|!
name|bsearch
argument_list|(
operator|&
name|ofsval
argument_list|,
name|opts
operator|->
name|anomaly
argument_list|,
name|opts
operator|->
name|anomaly_nr
argument_list|,
sizeof|sizeof
argument_list|(
name|ofsval
argument_list|)
argument_list|,
name|cmp_uint32
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * On entry *sha1 contains the pack content SHA1 hash, on exit it is  * the SHA1 hash of sorted object names. The objects array passed in  * will be sorted by SHA1 on exit.  */
end_comment

begin_function
DECL|function|write_idx_file
specifier|const
name|char
modifier|*
name|write_idx_file
parameter_list|(
specifier|const
name|char
modifier|*
name|index_name
parameter_list|,
name|struct
name|pack_idx_entry
modifier|*
modifier|*
name|objects
parameter_list|,
name|int
name|nr_objects
parameter_list|,
specifier|const
name|struct
name|pack_idx_option
modifier|*
name|opts
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|sha1file
modifier|*
name|f
decl_stmt|;
name|struct
name|pack_idx_entry
modifier|*
modifier|*
name|sorted_by_sha
decl_stmt|,
modifier|*
modifier|*
name|list
decl_stmt|,
modifier|*
modifier|*
name|last
decl_stmt|;
name|off_t
name|last_obj_offset
init|=
literal|0
decl_stmt|;
name|uint32_t
name|array
index|[
literal|256
index|]
decl_stmt|;
name|int
name|i
decl_stmt|,
name|fd
decl_stmt|;
name|git_SHA_CTX
name|ctx
decl_stmt|;
name|uint32_t
name|index_version
decl_stmt|;
if|if
condition|(
name|nr_objects
condition|)
block|{
name|sorted_by_sha
operator|=
name|objects
expr_stmt|;
name|list
operator|=
name|sorted_by_sha
expr_stmt|;
name|last
operator|=
name|sorted_by_sha
operator|+
name|nr_objects
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_objects
condition|;
operator|++
name|i
control|)
block|{
if|if
condition|(
name|objects
index|[
name|i
index|]
operator|->
name|offset
operator|>
name|last_obj_offset
condition|)
name|last_obj_offset
operator|=
name|objects
index|[
name|i
index|]
operator|->
name|offset
expr_stmt|;
block|}
name|qsort
argument_list|(
name|sorted_by_sha
argument_list|,
name|nr_objects
argument_list|,
sizeof|sizeof
argument_list|(
name|sorted_by_sha
index|[
literal|0
index|]
argument_list|)
argument_list|,
name|sha1_compare
argument_list|)
expr_stmt|;
block|}
else|else
name|sorted_by_sha
operator|=
name|list
operator|=
name|last
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|opts
operator|->
name|flags
operator|&
name|WRITE_IDX_VERIFY
condition|)
block|{
name|assert
argument_list|(
name|index_name
argument_list|)
expr_stmt|;
name|f
operator|=
name|sha1fd_check
argument_list|(
name|index_name
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
operator|!
name|index_name
condition|)
block|{
specifier|static
name|char
name|tmpfile
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|fd
operator|=
name|odb_mkstemp
argument_list|(
name|tmpfile
argument_list|,
sizeof|sizeof
argument_list|(
name|tmpfile
argument_list|)
argument_list|,
literal|"pack/tmp_idx_XXXXXX"
argument_list|)
expr_stmt|;
name|index_name
operator|=
name|xstrdup
argument_list|(
name|tmpfile
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|unlink
argument_list|(
name|index_name
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|index_name
argument_list|,
name|O_CREAT
operator||
name|O_EXCL
operator||
name|O_WRONLY
argument_list|,
literal|0600
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"unable to create '%s'"
argument_list|,
name|index_name
argument_list|)
expr_stmt|;
name|f
operator|=
name|sha1fd
argument_list|(
name|fd
argument_list|,
name|index_name
argument_list|)
expr_stmt|;
block|}
comment|/* if last object's offset is>= 2^31 we should use index V2 */
name|index_version
operator|=
name|need_large_offset
argument_list|(
name|last_obj_offset
argument_list|,
name|opts
argument_list|)
condition|?
literal|2
else|:
name|opts
operator|->
name|version
expr_stmt|;
comment|/* index versions 2 and above need a header */
if|if
condition|(
name|index_version
operator|>=
literal|2
condition|)
block|{
name|struct
name|pack_idx_header
name|hdr
decl_stmt|;
name|hdr
operator|.
name|idx_signature
operator|=
name|htonl
argument_list|(
name|PACK_IDX_SIGNATURE
argument_list|)
expr_stmt|;
name|hdr
operator|.
name|idx_version
operator|=
name|htonl
argument_list|(
name|index_version
argument_list|)
expr_stmt|;
name|sha1write
argument_list|(
name|f
argument_list|,
operator|&
name|hdr
argument_list|,
sizeof|sizeof
argument_list|(
name|hdr
argument_list|)
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * Write the first-level table (the list is sorted, 	 * but we use a 256-entry lookup to be able to avoid 	 * having to do eight extra binary search iterations). 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|256
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|pack_idx_entry
modifier|*
modifier|*
name|next
init|=
name|list
decl_stmt|;
while|while
condition|(
name|next
operator|<
name|last
condition|)
block|{
name|struct
name|pack_idx_entry
modifier|*
name|obj
init|=
operator|*
name|next
decl_stmt|;
if|if
condition|(
name|obj
operator|->
name|sha1
index|[
literal|0
index|]
operator|!=
name|i
condition|)
break|break;
name|next
operator|++
expr_stmt|;
block|}
name|array
index|[
name|i
index|]
operator|=
name|htonl
argument_list|(
name|next
operator|-
name|sorted_by_sha
argument_list|)
expr_stmt|;
name|list
operator|=
name|next
expr_stmt|;
block|}
name|sha1write
argument_list|(
name|f
argument_list|,
name|array
argument_list|,
literal|256
operator|*
literal|4
argument_list|)
expr_stmt|;
comment|/* compute the SHA1 hash of sorted object names. */
name|git_SHA1_Init
argument_list|(
operator|&
name|ctx
argument_list|)
expr_stmt|;
comment|/* 	 * Write the actual SHA1 entries.. 	 */
name|list
operator|=
name|sorted_by_sha
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_objects
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|pack_idx_entry
modifier|*
name|obj
init|=
operator|*
name|list
operator|++
decl_stmt|;
if|if
condition|(
name|index_version
operator|<
literal|2
condition|)
block|{
name|uint32_t
name|offset
init|=
name|htonl
argument_list|(
name|obj
operator|->
name|offset
argument_list|)
decl_stmt|;
name|sha1write
argument_list|(
name|f
argument_list|,
operator|&
name|offset
argument_list|,
literal|4
argument_list|)
expr_stmt|;
block|}
name|sha1write
argument_list|(
name|f
argument_list|,
name|obj
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|git_SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
name|obj
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|opts
operator|->
name|flags
operator|&
name|WRITE_IDX_STRICT
operator|)
operator|&&
operator|(
name|i
operator|&&
operator|!
name|hashcmp
argument_list|(
name|list
index|[
operator|-
literal|2
index|]
operator|->
name|sha1
argument_list|,
name|obj
operator|->
name|sha1
argument_list|)
operator|)
condition|)
name|die
argument_list|(
literal|"The same object %s appears twice in the pack"
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|index_version
operator|>=
literal|2
condition|)
block|{
name|unsigned
name|int
name|nr_large_offset
init|=
literal|0
decl_stmt|;
comment|/* write the crc32 table */
name|list
operator|=
name|sorted_by_sha
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_objects
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|pack_idx_entry
modifier|*
name|obj
init|=
operator|*
name|list
operator|++
decl_stmt|;
name|uint32_t
name|crc32_val
init|=
name|htonl
argument_list|(
name|obj
operator|->
name|crc32
argument_list|)
decl_stmt|;
name|sha1write
argument_list|(
name|f
argument_list|,
operator|&
name|crc32_val
argument_list|,
literal|4
argument_list|)
expr_stmt|;
block|}
comment|/* write the 32-bit offset table */
name|list
operator|=
name|sorted_by_sha
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_objects
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|pack_idx_entry
modifier|*
name|obj
init|=
operator|*
name|list
operator|++
decl_stmt|;
name|uint32_t
name|offset
decl_stmt|;
name|offset
operator|=
operator|(
name|need_large_offset
argument_list|(
name|obj
operator|->
name|offset
argument_list|,
name|opts
argument_list|)
condition|?
operator|(
literal|0x80000000
operator||
name|nr_large_offset
operator|++
operator|)
else|:
name|obj
operator|->
name|offset
operator|)
expr_stmt|;
name|offset
operator|=
name|htonl
argument_list|(
name|offset
argument_list|)
expr_stmt|;
name|sha1write
argument_list|(
name|f
argument_list|,
operator|&
name|offset
argument_list|,
literal|4
argument_list|)
expr_stmt|;
block|}
comment|/* write the large offset table */
name|list
operator|=
name|sorted_by_sha
expr_stmt|;
while|while
condition|(
name|nr_large_offset
condition|)
block|{
name|struct
name|pack_idx_entry
modifier|*
name|obj
init|=
operator|*
name|list
operator|++
decl_stmt|;
name|uint64_t
name|offset
init|=
name|obj
operator|->
name|offset
decl_stmt|;
name|uint32_t
name|split
index|[
literal|2
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|need_large_offset
argument_list|(
name|offset
argument_list|,
name|opts
argument_list|)
condition|)
continue|continue;
name|split
index|[
literal|0
index|]
operator|=
name|htonl
argument_list|(
name|offset
operator|>>
literal|32
argument_list|)
expr_stmt|;
name|split
index|[
literal|1
index|]
operator|=
name|htonl
argument_list|(
name|offset
operator|&
literal|0xffffffff
argument_list|)
expr_stmt|;
name|sha1write
argument_list|(
name|f
argument_list|,
name|split
argument_list|,
literal|8
argument_list|)
expr_stmt|;
name|nr_large_offset
operator|--
expr_stmt|;
block|}
block|}
name|sha1write
argument_list|(
name|f
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|sha1close
argument_list|(
name|f
argument_list|,
name|NULL
argument_list|,
operator|(
operator|(
name|opts
operator|->
name|flags
operator|&
name|WRITE_IDX_VERIFY
operator|)
condition|?
name|CSUM_CLOSE
else|:
name|CSUM_FSYNC
operator|)
argument_list|)
expr_stmt|;
name|git_SHA1_Final
argument_list|(
name|sha1
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
return|return
name|index_name
return|;
block|}
end_function

begin_comment
comment|/*  * Update pack header with object_count and compute new SHA1 for pack data  * associated to pack_fd, and write that SHA1 at the end.  That new SHA1  * is also returned in new_pack_sha1.  *  * If partial_pack_sha1 is non null, then the SHA1 of the existing pack  * (without the header update) is computed and validated against the  * one provided in partial_pack_sha1.  The validation is performed at  * partial_pack_offset bytes in the pack file.  The SHA1 of the remaining  * data (i.e. from partial_pack_offset to the end) is then computed and  * returned in partial_pack_sha1.  *  * Note that new_pack_sha1 is updated last, so both new_pack_sha1 and  * partial_pack_sha1 can refer to the same buffer if the caller is not  * interested in the resulting SHA1 of pack data above partial_pack_offset.  */
end_comment

begin_function
DECL|function|fixup_pack_header_footer
name|void
name|fixup_pack_header_footer
parameter_list|(
name|int
name|pack_fd
parameter_list|,
name|unsigned
name|char
modifier|*
name|new_pack_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|pack_name
parameter_list|,
name|uint32_t
name|object_count
parameter_list|,
name|unsigned
name|char
modifier|*
name|partial_pack_sha1
parameter_list|,
name|off_t
name|partial_pack_offset
parameter_list|)
block|{
name|int
name|aligned_sz
decl_stmt|,
name|buf_sz
init|=
literal|8
operator|*
literal|1024
decl_stmt|;
name|git_SHA_CTX
name|old_sha1_ctx
decl_stmt|,
name|new_sha1_ctx
decl_stmt|;
name|struct
name|pack_header
name|hdr
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|;
name|git_SHA1_Init
argument_list|(
operator|&
name|old_sha1_ctx
argument_list|)
expr_stmt|;
name|git_SHA1_Init
argument_list|(
operator|&
name|new_sha1_ctx
argument_list|)
expr_stmt|;
if|if
condition|(
name|lseek
argument_list|(
name|pack_fd
argument_list|,
literal|0
argument_list|,
name|SEEK_SET
argument_list|)
operator|!=
literal|0
condition|)
name|die_errno
argument_list|(
literal|"Failed seeking to start of '%s'"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_in_full
argument_list|(
name|pack_fd
argument_list|,
operator|&
name|hdr
argument_list|,
sizeof|sizeof
argument_list|(
name|hdr
argument_list|)
argument_list|)
operator|!=
sizeof|sizeof
argument_list|(
name|hdr
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"Unable to reread header of '%s'"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|lseek
argument_list|(
name|pack_fd
argument_list|,
literal|0
argument_list|,
name|SEEK_SET
argument_list|)
operator|!=
literal|0
condition|)
name|die_errno
argument_list|(
literal|"Failed seeking to start of '%s'"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
name|git_SHA1_Update
argument_list|(
operator|&
name|old_sha1_ctx
argument_list|,
operator|&
name|hdr
argument_list|,
sizeof|sizeof
argument_list|(
name|hdr
argument_list|)
argument_list|)
expr_stmt|;
name|hdr
operator|.
name|hdr_entries
operator|=
name|htonl
argument_list|(
name|object_count
argument_list|)
expr_stmt|;
name|git_SHA1_Update
argument_list|(
operator|&
name|new_sha1_ctx
argument_list|,
operator|&
name|hdr
argument_list|,
sizeof|sizeof
argument_list|(
name|hdr
argument_list|)
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
name|pack_fd
argument_list|,
operator|&
name|hdr
argument_list|,
sizeof|sizeof
argument_list|(
name|hdr
argument_list|)
argument_list|)
expr_stmt|;
name|partial_pack_offset
operator|-=
sizeof|sizeof
argument_list|(
name|hdr
argument_list|)
expr_stmt|;
name|buf
operator|=
name|xmalloc
argument_list|(
name|buf_sz
argument_list|)
expr_stmt|;
name|aligned_sz
operator|=
name|buf_sz
operator|-
sizeof|sizeof
argument_list|(
name|hdr
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|ssize_t
name|m
decl_stmt|,
name|n
decl_stmt|;
name|m
operator|=
operator|(
name|partial_pack_sha1
operator|&&
name|partial_pack_offset
operator|<
name|aligned_sz
operator|)
condition|?
name|partial_pack_offset
else|:
name|aligned_sz
expr_stmt|;
name|n
operator|=
name|xread
argument_list|(
name|pack_fd
argument_list|,
name|buf
argument_list|,
name|m
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|n
condition|)
break|break;
if|if
condition|(
name|n
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"Failed to checksum '%s'"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
name|git_SHA1_Update
argument_list|(
operator|&
name|new_sha1_ctx
argument_list|,
name|buf
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|aligned_sz
operator|-=
name|n
expr_stmt|;
if|if
condition|(
operator|!
name|aligned_sz
condition|)
name|aligned_sz
operator|=
name|buf_sz
expr_stmt|;
if|if
condition|(
operator|!
name|partial_pack_sha1
condition|)
continue|continue;
name|git_SHA1_Update
argument_list|(
operator|&
name|old_sha1_ctx
argument_list|,
name|buf
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|partial_pack_offset
operator|-=
name|n
expr_stmt|;
if|if
condition|(
name|partial_pack_offset
operator|==
literal|0
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|git_SHA1_Final
argument_list|(
name|sha1
argument_list|,
operator|&
name|old_sha1_ctx
argument_list|)
expr_stmt|;
if|if
condition|(
name|hashcmp
argument_list|(
name|sha1
argument_list|,
name|partial_pack_sha1
argument_list|)
operator|!=
literal|0
condition|)
name|die
argument_list|(
literal|"Unexpected checksum for %s "
literal|"(disk corruption?)"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
comment|/* 			 * Now let's compute the SHA1 of the remainder of the 			 * pack, which also means making partial_pack_offset 			 * big enough not to matter anymore. 			 */
name|git_SHA1_Init
argument_list|(
operator|&
name|old_sha1_ctx
argument_list|)
expr_stmt|;
name|partial_pack_offset
operator|=
operator|~
name|partial_pack_offset
expr_stmt|;
name|partial_pack_offset
operator|-=
name|MSB
argument_list|(
name|partial_pack_offset
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|partial_pack_sha1
condition|)
name|git_SHA1_Final
argument_list|(
name|partial_pack_sha1
argument_list|,
operator|&
name|old_sha1_ctx
argument_list|)
expr_stmt|;
name|git_SHA1_Final
argument_list|(
name|new_pack_sha1
argument_list|,
operator|&
name|new_sha1_ctx
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
name|pack_fd
argument_list|,
name|new_pack_sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|fsync_or_die
argument_list|(
name|pack_fd
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|index_pack_lockfile
name|char
modifier|*
name|index_pack_lockfile
parameter_list|(
name|int
name|ip_out
parameter_list|)
block|{
name|char
name|packname
index|[
literal|46
index|]
decl_stmt|;
comment|/* 	 * The first thing we expect from index-pack's output 	 * is "pack\t%40s\n" or "keep\t%40s\n" (46 bytes) where 	 * %40s is the newly created pack SHA1 name.  In the "keep" 	 * case, we need it to remove the corresponding .keep file 	 * later on.  If we don't get that then tough luck with it. 	 */
if|if
condition|(
name|read_in_full
argument_list|(
name|ip_out
argument_list|,
name|packname
argument_list|,
literal|46
argument_list|)
operator|==
literal|46
operator|&&
name|packname
index|[
literal|45
index|]
operator|==
literal|'\n'
operator|&&
name|memcmp
argument_list|(
name|packname
argument_list|,
literal|"keep\t"
argument_list|,
literal|5
argument_list|)
operator|==
literal|0
condition|)
block|{
name|char
name|path
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|packname
index|[
literal|45
index|]
operator|=
literal|0
expr_stmt|;
name|snprintf
argument_list|(
name|path
argument_list|,
sizeof|sizeof
argument_list|(
name|path
argument_list|)
argument_list|,
literal|"%s/pack/pack-%s.keep"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|,
name|packname
operator|+
literal|5
argument_list|)
expr_stmt|;
return|return
name|xstrdup
argument_list|(
name|path
argument_list|)
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_comment
comment|/*  * The per-object header is a pretty dense thing, which is  *  - first byte: low four bits are "size", then three bits of "type",  *    and the high bit is "size continues".  *  - each byte afterwards: low seven bits are size continuation,  *    with the high bit being "size continues"  */
end_comment

begin_function
DECL|function|encode_in_pack_object_header
name|int
name|encode_in_pack_object_header
parameter_list|(
name|enum
name|object_type
name|type
parameter_list|,
name|uintmax_t
name|size
parameter_list|,
name|unsigned
name|char
modifier|*
name|hdr
parameter_list|)
block|{
name|int
name|n
init|=
literal|1
decl_stmt|;
name|unsigned
name|char
name|c
decl_stmt|;
if|if
condition|(
name|type
operator|<
name|OBJ_COMMIT
operator|||
name|type
operator|>
name|OBJ_REF_DELTA
condition|)
name|die
argument_list|(
literal|"bad type %d"
argument_list|,
name|type
argument_list|)
expr_stmt|;
name|c
operator|=
operator|(
name|type
operator|<<
literal|4
operator|)
operator||
operator|(
name|size
operator|&
literal|15
operator|)
expr_stmt|;
name|size
operator|>>=
literal|4
expr_stmt|;
while|while
condition|(
name|size
condition|)
block|{
operator|*
name|hdr
operator|++
operator|=
name|c
operator||
literal|0x80
expr_stmt|;
name|c
operator|=
name|size
operator|&
literal|0x7f
expr_stmt|;
name|size
operator|>>=
literal|7
expr_stmt|;
name|n
operator|++
expr_stmt|;
block|}
operator|*
name|hdr
operator|=
name|c
expr_stmt|;
return|return
name|n
return|;
block|}
end_function

end_unit

