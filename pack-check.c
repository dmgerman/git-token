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
file|"pack-revindex.h"
end_include

begin_struct
DECL|struct|idx_entry
struct|struct
name|idx_entry
block|{
DECL|member|sha1
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
DECL|member|offset
name|off_t
name|offset
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|compare_entries
specifier|static
name|int
name|compare_entries
parameter_list|(
specifier|const
name|void
modifier|*
name|e1
parameter_list|,
specifier|const
name|void
modifier|*
name|e2
parameter_list|)
block|{
specifier|const
name|struct
name|idx_entry
modifier|*
name|entry1
init|=
name|e1
decl_stmt|;
specifier|const
name|struct
name|idx_entry
modifier|*
name|entry2
init|=
name|e2
decl_stmt|;
if|if
condition|(
name|entry1
operator|->
name|offset
operator|<
name|entry2
operator|->
name|offset
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|entry1
operator|->
name|offset
operator|>
name|entry2
operator|->
name|offset
condition|)
return|return
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|verify_packfile
specifier|static
name|int
name|verify_packfile
parameter_list|(
name|struct
name|packed_git
modifier|*
name|p
parameter_list|,
name|struct
name|pack_window
modifier|*
modifier|*
name|w_curs
parameter_list|)
block|{
name|off_t
name|index_size
init|=
name|p
operator|->
name|index_size
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|index_base
init|=
name|p
operator|->
name|index_data
decl_stmt|;
name|SHA_CTX
name|ctx
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|,
modifier|*
name|pack_sig
decl_stmt|;
name|off_t
name|offset
init|=
literal|0
decl_stmt|,
name|pack_sig_ofs
init|=
name|p
operator|->
name|pack_size
operator|-
literal|20
decl_stmt|;
name|uint32_t
name|nr_objects
decl_stmt|,
name|i
decl_stmt|;
name|int
name|err
init|=
literal|0
decl_stmt|;
name|struct
name|idx_entry
modifier|*
name|entries
decl_stmt|;
comment|/* Note that the pack header checks are actually performed by 	 * use_pack when it first opens the pack file.  If anything 	 * goes wrong during those checks then the call will die out 	 * immediately. 	 */
name|SHA1_Init
argument_list|(
operator|&
name|ctx
argument_list|)
expr_stmt|;
while|while
condition|(
name|offset
operator|<
name|pack_sig_ofs
condition|)
block|{
name|unsigned
name|int
name|remaining
decl_stmt|;
name|unsigned
name|char
modifier|*
name|in
init|=
name|use_pack
argument_list|(
name|p
argument_list|,
name|w_curs
argument_list|,
name|offset
argument_list|,
operator|&
name|remaining
argument_list|)
decl_stmt|;
name|offset
operator|+=
name|remaining
expr_stmt|;
if|if
condition|(
name|offset
operator|>
name|pack_sig_ofs
condition|)
name|remaining
operator|-=
call|(
name|unsigned
name|int
call|)
argument_list|(
name|offset
operator|-
name|pack_sig_ofs
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
name|in
argument_list|,
name|remaining
argument_list|)
expr_stmt|;
block|}
name|SHA1_Final
argument_list|(
name|sha1
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
name|pack_sig
operator|=
name|use_pack
argument_list|(
name|p
argument_list|,
name|w_curs
argument_list|,
name|pack_sig_ofs
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|hashcmp
argument_list|(
name|sha1
argument_list|,
name|pack_sig
argument_list|)
condition|)
name|err
operator|=
name|error
argument_list|(
literal|"%s SHA1 checksum mismatch"
argument_list|,
name|p
operator|->
name|pack_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|hashcmp
argument_list|(
name|index_base
operator|+
name|index_size
operator|-
literal|40
argument_list|,
name|pack_sig
argument_list|)
condition|)
name|err
operator|=
name|error
argument_list|(
literal|"%s SHA1 does not match its inddex"
argument_list|,
name|p
operator|->
name|pack_name
argument_list|)
expr_stmt|;
name|unuse_pack
argument_list|(
name|w_curs
argument_list|)
expr_stmt|;
comment|/* Make sure everything reachable from idx is valid.  Since we 	 * have verified that nr_objects matches between idx and pack, 	 * we do not do scan-streaming check on the pack file. 	 */
name|nr_objects
operator|=
name|p
operator|->
name|num_objects
expr_stmt|;
name|entries
operator|=
name|xmalloc
argument_list|(
name|nr_objects
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|entries
argument_list|)
argument_list|)
expr_stmt|;
comment|/* first sort entries by pack offset, since unpacking them is more efficient that way */
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
name|entries
index|[
name|i
index|]
operator|.
name|sha1
operator|=
name|nth_packed_object_sha1
argument_list|(
name|p
argument_list|,
name|i
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|entries
index|[
name|i
index|]
operator|.
name|sha1
condition|)
name|die
argument_list|(
literal|"internal error pack-check nth-packed-object"
argument_list|)
expr_stmt|;
name|entries
index|[
name|i
index|]
operator|.
name|offset
operator|=
name|find_pack_entry_one
argument_list|(
name|entries
index|[
name|i
index|]
operator|.
name|sha1
argument_list|,
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|entries
index|[
name|i
index|]
operator|.
name|offset
condition|)
name|die
argument_list|(
literal|"internal error pack-check find-pack-entry-one"
argument_list|)
expr_stmt|;
block|}
name|qsort
argument_list|(
name|entries
argument_list|,
name|nr_objects
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|entries
argument_list|)
argument_list|,
name|compare_entries
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
name|nr_objects
condition|;
name|i
operator|++
control|)
block|{
name|void
modifier|*
name|data
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|data
operator|=
name|unpack_entry
argument_list|(
name|p
argument_list|,
name|entries
index|[
name|i
index|]
operator|.
name|offset
argument_list|,
operator|&
name|type
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|data
condition|)
block|{
name|err
operator|=
name|error
argument_list|(
literal|"cannot unpack %s from %s at offset %"
name|PRIuMAX
literal|""
argument_list|,
name|sha1_to_hex
argument_list|(
name|entries
index|[
name|i
index|]
operator|.
name|sha1
argument_list|)
argument_list|,
name|p
operator|->
name|pack_name
argument_list|,
operator|(
name|uintmax_t
operator|)
name|entries
index|[
name|i
index|]
operator|.
name|offset
argument_list|)
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|check_sha1_signature
argument_list|(
name|entries
index|[
name|i
index|]
operator|.
name|sha1
argument_list|,
name|data
argument_list|,
name|size
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
argument_list|)
condition|)
block|{
name|err
operator|=
name|error
argument_list|(
literal|"packed %s from %s is corrupt"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entries
index|[
name|i
index|]
operator|.
name|sha1
argument_list|)
argument_list|,
name|p
operator|->
name|pack_name
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
break|break;
block|}
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|entries
argument_list|)
expr_stmt|;
return|return
name|err
return|;
block|}
end_function

begin_define
DECL|macro|MAX_CHAIN
define|#
directive|define
name|MAX_CHAIN
value|50
end_define

begin_function
DECL|function|show_pack_info
specifier|static
name|void
name|show_pack_info
parameter_list|(
name|struct
name|packed_git
modifier|*
name|p
parameter_list|)
block|{
name|uint32_t
name|nr_objects
decl_stmt|,
name|i
decl_stmt|,
name|chain_histogram
index|[
name|MAX_CHAIN
operator|+
literal|1
index|]
decl_stmt|;
name|nr_objects
operator|=
name|p
operator|->
name|num_objects
expr_stmt|;
name|memset
argument_list|(
name|chain_histogram
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|chain_histogram
argument_list|)
argument_list|)
expr_stmt|;
name|init_pack_revindex
argument_list|()
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
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|unsigned
name|char
name|base_sha1
index|[
literal|20
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|type
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|unsigned
name|long
name|store_size
decl_stmt|;
name|off_t
name|offset
decl_stmt|;
name|unsigned
name|int
name|delta_chain_length
decl_stmt|;
name|sha1
operator|=
name|nth_packed_object_sha1
argument_list|(
name|p
argument_list|,
name|i
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|sha1
condition|)
name|die
argument_list|(
literal|"internal error pack-check nth-packed-object"
argument_list|)
expr_stmt|;
name|offset
operator|=
name|find_pack_entry_one
argument_list|(
name|sha1
argument_list|,
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|offset
condition|)
name|die
argument_list|(
literal|"internal error pack-check find-pack-entry-one"
argument_list|)
expr_stmt|;
name|type
operator|=
name|packed_object_info_detail
argument_list|(
name|p
argument_list|,
name|offset
argument_list|,
operator|&
name|size
argument_list|,
operator|&
name|store_size
argument_list|,
operator|&
name|delta_chain_length
argument_list|,
name|base_sha1
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s "
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|delta_chain_length
condition|)
name|printf
argument_list|(
literal|"%-6s %lu %lu %"
name|PRIuMAX
literal|"\n"
argument_list|,
name|type
argument_list|,
name|size
argument_list|,
name|store_size
argument_list|,
operator|(
name|uintmax_t
operator|)
name|offset
argument_list|)
expr_stmt|;
else|else
block|{
name|printf
argument_list|(
literal|"%-6s %lu %lu %"
name|PRIuMAX
literal|" %u %s\n"
argument_list|,
name|type
argument_list|,
name|size
argument_list|,
name|store_size
argument_list|,
operator|(
name|uintmax_t
operator|)
name|offset
argument_list|,
name|delta_chain_length
argument_list|,
name|sha1_to_hex
argument_list|(
name|base_sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|delta_chain_length
operator|<=
name|MAX_CHAIN
condition|)
name|chain_histogram
index|[
name|delta_chain_length
index|]
operator|++
expr_stmt|;
else|else
name|chain_histogram
index|[
literal|0
index|]
operator|++
expr_stmt|;
block|}
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<=
name|MAX_CHAIN
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|chain_histogram
index|[
name|i
index|]
condition|)
continue|continue;
name|printf
argument_list|(
literal|"chain length = %d: %d object%s\n"
argument_list|,
name|i
argument_list|,
name|chain_histogram
index|[
name|i
index|]
argument_list|,
name|chain_histogram
index|[
name|i
index|]
operator|>
literal|1
condition|?
literal|"s"
else|:
literal|""
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|chain_histogram
index|[
literal|0
index|]
condition|)
name|printf
argument_list|(
literal|"chain length> %d: %d object%s\n"
argument_list|,
name|MAX_CHAIN
argument_list|,
name|chain_histogram
index|[
literal|0
index|]
argument_list|,
name|chain_histogram
index|[
literal|0
index|]
operator|>
literal|1
condition|?
literal|"s"
else|:
literal|""
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|verify_pack
name|int
name|verify_pack
parameter_list|(
name|struct
name|packed_git
modifier|*
name|p
parameter_list|,
name|int
name|verbose
parameter_list|)
block|{
name|off_t
name|index_size
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|index_base
decl_stmt|;
name|SHA_CTX
name|ctx
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|err
init|=
literal|0
decl_stmt|;
name|struct
name|pack_window
modifier|*
name|w_curs
init|=
name|NULL
decl_stmt|;
if|if
condition|(
name|open_pack_index
argument_list|(
name|p
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"packfile %s index not opened"
argument_list|,
name|p
operator|->
name|pack_name
argument_list|)
return|;
name|index_size
operator|=
name|p
operator|->
name|index_size
expr_stmt|;
name|index_base
operator|=
name|p
operator|->
name|index_data
expr_stmt|;
comment|/* Verify SHA1 sum of the index file */
name|SHA1_Init
argument_list|(
operator|&
name|ctx
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
name|index_base
argument_list|,
call|(
name|unsigned
name|int
call|)
argument_list|(
name|index_size
operator|-
literal|20
argument_list|)
argument_list|)
expr_stmt|;
name|SHA1_Final
argument_list|(
name|sha1
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
if|if
condition|(
name|hashcmp
argument_list|(
name|sha1
argument_list|,
name|index_base
operator|+
name|index_size
operator|-
literal|20
argument_list|)
condition|)
name|err
operator|=
name|error
argument_list|(
literal|"Packfile index for %s SHA1 mismatch"
argument_list|,
name|p
operator|->
name|pack_name
argument_list|)
expr_stmt|;
comment|/* Verify pack file */
name|err
operator||=
name|verify_packfile
argument_list|(
name|p
argument_list|,
operator|&
name|w_curs
argument_list|)
expr_stmt|;
name|unuse_pack
argument_list|(
operator|&
name|w_curs
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
block|{
if|if
condition|(
name|err
condition|)
name|printf
argument_list|(
literal|"%s: bad\n"
argument_list|,
name|p
operator|->
name|pack_name
argument_list|)
expr_stmt|;
else|else
block|{
name|show_pack_info
argument_list|(
name|p
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s: ok\n"
argument_list|,
name|p
operator|->
name|pack_name
argument_list|)
expr_stmt|;
block|}
block|}
return|return
name|err
return|;
block|}
end_function

end_unit

