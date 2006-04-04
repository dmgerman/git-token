begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"delta.h"
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

begin_include
include|#
directive|include
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_decl_stmt
DECL|variable|index_pack_usage
specifier|static
specifier|const
name|char
name|index_pack_usage
index|[]
init|=
literal|"git-index-pack [-o index-file] pack-file"
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|object_entry
struct|struct
name|object_entry
block|{
DECL|member|offset
name|unsigned
name|long
name|offset
decl_stmt|;
DECL|member|type
name|enum
name|object_type
name|type
decl_stmt|;
DECL|member|real_type
name|enum
name|object_type
name|real_type
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|delta_entry
struct|struct
name|delta_entry
block|{
DECL|member|obj
name|struct
name|object_entry
modifier|*
name|obj
decl_stmt|;
DECL|member|base_sha1
name|unsigned
name|char
name|base_sha1
index|[
literal|20
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|pack_name
specifier|static
specifier|const
name|char
modifier|*
name|pack_name
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pack_base
specifier|static
name|unsigned
name|char
modifier|*
name|pack_base
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pack_size
specifier|static
name|unsigned
name|long
name|pack_size
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|objects
specifier|static
name|struct
name|object_entry
modifier|*
name|objects
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|deltas
specifier|static
name|struct
name|delta_entry
modifier|*
name|deltas
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nr_objects
specifier|static
name|int
name|nr_objects
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nr_deltas
specifier|static
name|int
name|nr_deltas
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|open_pack_file
specifier|static
name|void
name|open_pack_file
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|pack_name
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"cannot open packfile '%s': %s"
argument_list|,
name|pack_name
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|fstat
argument_list|(
name|fd
argument_list|,
operator|&
name|st
argument_list|)
condition|)
block|{
name|int
name|err
init|=
name|errno
decl_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"cannot fstat packfile '%s': %s"
argument_list|,
name|pack_name
argument_list|,
name|strerror
argument_list|(
name|err
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|pack_size
operator|=
name|st
operator|.
name|st_size
expr_stmt|;
name|pack_base
operator|=
name|mmap
argument_list|(
name|NULL
argument_list|,
name|pack_size
argument_list|,
name|PROT_READ
argument_list|,
name|MAP_PRIVATE
argument_list|,
name|fd
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|pack_base
operator|==
name|MAP_FAILED
condition|)
block|{
name|int
name|err
init|=
name|errno
decl_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"cannot mmap packfile '%s': %s"
argument_list|,
name|pack_name
argument_list|,
name|strerror
argument_list|(
name|err
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|parse_pack_header
specifier|static
name|void
name|parse_pack_header
parameter_list|(
name|void
parameter_list|)
block|{
specifier|const
name|struct
name|pack_header
modifier|*
name|hdr
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|SHA_CTX
name|ctx
decl_stmt|;
comment|/* Ensure there are enough bytes for the header and final SHA1 */
if|if
condition|(
name|pack_size
operator|<
sizeof|sizeof
argument_list|(
expr|struct
name|pack_header
argument_list|)
operator|+
literal|20
condition|)
name|die
argument_list|(
literal|"packfile '%s' is too small"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
comment|/* Header consistency check */
name|hdr
operator|=
operator|(
name|void
operator|*
operator|)
name|pack_base
expr_stmt|;
if|if
condition|(
name|hdr
operator|->
name|hdr_signature
operator|!=
name|htonl
argument_list|(
name|PACK_SIGNATURE
argument_list|)
condition|)
name|die
argument_list|(
literal|"packfile '%s' signature mismatch"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pack_version_ok
argument_list|(
name|hdr
operator|->
name|hdr_version
argument_list|)
condition|)
name|die
argument_list|(
literal|"packfile '%s' version %d unsupported"
argument_list|,
name|pack_name
argument_list|,
name|ntohl
argument_list|(
name|hdr
operator|->
name|hdr_version
argument_list|)
argument_list|)
expr_stmt|;
name|nr_objects
operator|=
name|ntohl
argument_list|(
name|hdr
operator|->
name|hdr_entries
argument_list|)
expr_stmt|;
comment|/* Check packfile integrity */
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
name|pack_base
argument_list|,
name|pack_size
operator|-
literal|20
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
name|memcmp
argument_list|(
name|sha1
argument_list|,
name|pack_base
operator|+
name|pack_size
operator|-
literal|20
argument_list|,
literal|20
argument_list|)
condition|)
name|die
argument_list|(
literal|"packfile '%s' SHA1 mismatch"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
specifier|static
name|void
name|bad_object
argument_list|(
name|unsigned
name|long
name|offset
argument_list|,
specifier|const
name|char
operator|*
name|format
argument_list|,
operator|...
argument_list|)
name|NORETURN
name|__attribute__
argument_list|(
operator|(
name|format
argument_list|(
name|printf
argument_list|,
literal|2
argument_list|,
literal|3
argument_list|)
operator|)
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|bad_object
specifier|static
name|void
name|bad_object
parameter_list|(
name|unsigned
name|long
name|offset
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|params
decl_stmt|;
name|char
name|buf
index|[
literal|1024
index|]
decl_stmt|;
name|va_start
argument_list|(
name|params
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|vsnprintf
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
name|format
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"packfile '%s': bad object at offset %lu: %s"
argument_list|,
name|pack_name
argument_list|,
name|offset
argument_list|,
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|unpack_entry_data
specifier|static
name|void
modifier|*
name|unpack_entry_data
parameter_list|(
name|unsigned
name|long
name|offset
parameter_list|,
name|unsigned
name|long
modifier|*
name|current_pos
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|unsigned
name|long
name|pack_limit
init|=
name|pack_size
operator|-
literal|20
decl_stmt|;
name|unsigned
name|long
name|pos
init|=
operator|*
name|current_pos
decl_stmt|;
name|z_stream
name|stream
decl_stmt|;
name|void
modifier|*
name|buf
init|=
name|xmalloc
argument_list|(
name|size
argument_list|)
decl_stmt|;
name|memset
argument_list|(
operator|&
name|stream
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|stream
argument_list|)
argument_list|)
expr_stmt|;
name|stream
operator|.
name|next_out
operator|=
name|buf
expr_stmt|;
name|stream
operator|.
name|avail_out
operator|=
name|size
expr_stmt|;
name|stream
operator|.
name|next_in
operator|=
name|pack_base
operator|+
name|pos
expr_stmt|;
name|stream
operator|.
name|avail_in
operator|=
name|pack_limit
operator|-
name|pos
expr_stmt|;
name|inflateInit
argument_list|(
operator|&
name|stream
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|ret
init|=
name|inflate
argument_list|(
operator|&
name|stream
argument_list|,
literal|0
argument_list|)
decl_stmt|;
if|if
condition|(
name|ret
operator|==
name|Z_STREAM_END
condition|)
break|break;
if|if
condition|(
name|ret
operator|!=
name|Z_OK
condition|)
name|bad_object
argument_list|(
name|offset
argument_list|,
literal|"inflate returned %d"
argument_list|,
name|ret
argument_list|)
expr_stmt|;
block|}
name|inflateEnd
argument_list|(
operator|&
name|stream
argument_list|)
expr_stmt|;
if|if
condition|(
name|stream
operator|.
name|total_out
operator|!=
name|size
condition|)
name|bad_object
argument_list|(
name|offset
argument_list|,
literal|"size mismatch (expected %lu, got %lu)"
argument_list|,
name|size
argument_list|,
name|stream
operator|.
name|total_out
argument_list|)
expr_stmt|;
operator|*
name|current_pos
operator|=
name|pack_limit
operator|-
name|stream
operator|.
name|avail_in
expr_stmt|;
return|return
name|buf
return|;
block|}
end_function

begin_function
DECL|function|unpack_raw_entry
specifier|static
name|void
modifier|*
name|unpack_raw_entry
parameter_list|(
name|unsigned
name|long
name|offset
parameter_list|,
name|enum
name|object_type
modifier|*
name|obj_type
parameter_list|,
name|unsigned
name|long
modifier|*
name|obj_size
parameter_list|,
name|unsigned
name|char
modifier|*
name|delta_base
parameter_list|,
name|unsigned
name|long
modifier|*
name|next_obj_offset
parameter_list|)
block|{
name|unsigned
name|long
name|pack_limit
init|=
name|pack_size
operator|-
literal|20
decl_stmt|;
name|unsigned
name|long
name|pos
init|=
name|offset
decl_stmt|;
name|unsigned
name|char
name|c
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|unsigned
name|shift
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
name|void
modifier|*
name|data
decl_stmt|;
name|c
operator|=
name|pack_base
index|[
name|pos
operator|++
index|]
expr_stmt|;
name|type
operator|=
operator|(
name|c
operator|>>
literal|4
operator|)
operator|&
literal|7
expr_stmt|;
name|size
operator|=
operator|(
name|c
operator|&
literal|15
operator|)
expr_stmt|;
name|shift
operator|=
literal|4
expr_stmt|;
while|while
condition|(
name|c
operator|&
literal|0x80
condition|)
block|{
if|if
condition|(
name|pos
operator|>=
name|pack_limit
condition|)
name|bad_object
argument_list|(
name|offset
argument_list|,
literal|"object extends past end of pack"
argument_list|)
expr_stmt|;
name|c
operator|=
name|pack_base
index|[
name|pos
operator|++
index|]
expr_stmt|;
name|size
operator|+=
operator|(
name|c
operator|&
literal|0x7fUL
operator|)
operator|<<
name|shift
expr_stmt|;
name|shift
operator|+=
literal|7
expr_stmt|;
block|}
switch|switch
condition|(
name|type
condition|)
block|{
case|case
name|OBJ_DELTA
case|:
if|if
condition|(
name|pos
operator|+
literal|20
operator|>=
name|pack_limit
condition|)
name|bad_object
argument_list|(
name|offset
argument_list|,
literal|"object extends past end of pack"
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|delta_base
argument_list|,
name|pack_base
operator|+
name|pos
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|pos
operator|+=
literal|20
expr_stmt|;
comment|/* fallthru */
case|case
name|OBJ_COMMIT
case|:
case|case
name|OBJ_TREE
case|:
case|case
name|OBJ_BLOB
case|:
case|case
name|OBJ_TAG
case|:
name|data
operator|=
name|unpack_entry_data
argument_list|(
name|offset
argument_list|,
operator|&
name|pos
argument_list|,
name|size
argument_list|)
expr_stmt|;
break|break;
default|default:
name|bad_object
argument_list|(
name|offset
argument_list|,
literal|"bad object type %d"
argument_list|,
name|type
argument_list|)
expr_stmt|;
block|}
operator|*
name|obj_type
operator|=
name|type
expr_stmt|;
operator|*
name|obj_size
operator|=
name|size
expr_stmt|;
operator|*
name|next_obj_offset
operator|=
name|pos
expr_stmt|;
return|return
name|data
return|;
block|}
end_function

begin_function
DECL|function|find_delta
specifier|static
name|int
name|find_delta
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|base_sha1
parameter_list|)
block|{
name|int
name|first
init|=
literal|0
decl_stmt|,
name|last
init|=
name|nr_deltas
decl_stmt|;
while|while
condition|(
name|first
operator|<
name|last
condition|)
block|{
name|int
name|next
init|=
operator|(
name|first
operator|+
name|last
operator|)
operator|/
literal|2
decl_stmt|;
name|struct
name|delta_entry
modifier|*
name|delta
init|=
operator|&
name|deltas
index|[
name|next
index|]
decl_stmt|;
name|int
name|cmp
decl_stmt|;
name|cmp
operator|=
name|memcmp
argument_list|(
name|base_sha1
argument_list|,
name|delta
operator|->
name|base_sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cmp
condition|)
return|return
name|next
return|;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
block|{
name|last
operator|=
name|next
expr_stmt|;
continue|continue;
block|}
name|first
operator|=
name|next
operator|+
literal|1
expr_stmt|;
block|}
return|return
operator|-
name|first
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|find_deltas_based_on_sha1
specifier|static
name|int
name|find_deltas_based_on_sha1
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|base_sha1
parameter_list|,
name|int
modifier|*
name|first_index
parameter_list|,
name|int
modifier|*
name|last_index
parameter_list|)
block|{
name|int
name|first
init|=
name|find_delta
argument_list|(
name|base_sha1
argument_list|)
decl_stmt|;
name|int
name|last
init|=
name|first
decl_stmt|;
name|int
name|end
init|=
name|nr_deltas
operator|-
literal|1
decl_stmt|;
if|if
condition|(
name|first
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
while|while
condition|(
name|first
operator|>
literal|0
operator|&&
operator|!
name|memcmp
argument_list|(
name|deltas
index|[
name|first
operator|-
literal|1
index|]
operator|.
name|base_sha1
argument_list|,
name|base_sha1
argument_list|,
literal|20
argument_list|)
condition|)
operator|--
name|first
expr_stmt|;
while|while
condition|(
name|last
operator|<
name|end
operator|&&
operator|!
name|memcmp
argument_list|(
name|deltas
index|[
name|last
operator|+
literal|1
index|]
operator|.
name|base_sha1
argument_list|,
name|base_sha1
argument_list|,
literal|20
argument_list|)
condition|)
operator|++
name|last
expr_stmt|;
operator|*
name|first_index
operator|=
name|first
expr_stmt|;
operator|*
name|last_index
operator|=
name|last
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|sha1_object
specifier|static
name|void
name|sha1_object
parameter_list|(
specifier|const
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|enum
name|object_type
name|type
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|SHA_CTX
name|ctx
decl_stmt|;
name|char
name|header
index|[
literal|50
index|]
decl_stmt|;
name|int
name|header_size
decl_stmt|;
specifier|const
name|char
modifier|*
name|type_str
decl_stmt|;
switch|switch
condition|(
name|type
condition|)
block|{
case|case
name|OBJ_COMMIT
case|:
name|type_str
operator|=
name|commit_type
expr_stmt|;
break|break;
case|case
name|OBJ_TREE
case|:
name|type_str
operator|=
name|tree_type
expr_stmt|;
break|break;
case|case
name|OBJ_BLOB
case|:
name|type_str
operator|=
name|blob_type
expr_stmt|;
break|break;
case|case
name|OBJ_TAG
case|:
name|type_str
operator|=
name|tag_type
expr_stmt|;
break|break;
default|default:
name|die
argument_list|(
literal|"bad type %d"
argument_list|,
name|type
argument_list|)
expr_stmt|;
block|}
name|header_size
operator|=
name|sprintf
argument_list|(
name|header
argument_list|,
literal|"%s %lu"
argument_list|,
name|type_str
argument_list|,
name|size
argument_list|)
operator|+
literal|1
expr_stmt|;
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
name|header
argument_list|,
name|header_size
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
name|data
argument_list|,
name|size
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
block|}
end_function

begin_function
DECL|function|resolve_delta
specifier|static
name|void
name|resolve_delta
parameter_list|(
name|struct
name|delta_entry
modifier|*
name|delta
parameter_list|,
name|void
modifier|*
name|base_data
parameter_list|,
name|unsigned
name|long
name|base_size
parameter_list|,
name|enum
name|object_type
name|type
parameter_list|)
block|{
name|struct
name|object_entry
modifier|*
name|obj
init|=
name|delta
operator|->
name|obj
decl_stmt|;
name|void
modifier|*
name|delta_data
decl_stmt|;
name|unsigned
name|long
name|delta_size
decl_stmt|;
name|void
modifier|*
name|result
decl_stmt|;
name|unsigned
name|long
name|result_size
decl_stmt|;
name|enum
name|object_type
name|delta_type
decl_stmt|;
name|unsigned
name|char
name|base_sha1
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|long
name|next_obj_offset
decl_stmt|;
name|int
name|j
decl_stmt|,
name|first
decl_stmt|,
name|last
decl_stmt|;
name|obj
operator|->
name|real_type
operator|=
name|type
expr_stmt|;
name|delta_data
operator|=
name|unpack_raw_entry
argument_list|(
name|obj
operator|->
name|offset
argument_list|,
operator|&
name|delta_type
argument_list|,
operator|&
name|delta_size
argument_list|,
name|base_sha1
argument_list|,
operator|&
name|next_obj_offset
argument_list|)
expr_stmt|;
name|result
operator|=
name|patch_delta
argument_list|(
name|base_data
argument_list|,
name|base_size
argument_list|,
name|delta_data
argument_list|,
name|delta_size
argument_list|,
operator|&
name|result_size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|delta_data
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|result
condition|)
name|bad_object
argument_list|(
name|obj
operator|->
name|offset
argument_list|,
literal|"failed to apply delta"
argument_list|)
expr_stmt|;
name|sha1_object
argument_list|(
name|result
argument_list|,
name|result_size
argument_list|,
name|type
argument_list|,
name|obj
operator|->
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|find_deltas_based_on_sha1
argument_list|(
name|obj
operator|->
name|sha1
argument_list|,
operator|&
name|first
argument_list|,
operator|&
name|last
argument_list|)
condition|)
block|{
for|for
control|(
name|j
operator|=
name|first
init|;
name|j
operator|<=
name|last
condition|;
name|j
operator|++
control|)
name|resolve_delta
argument_list|(
operator|&
name|deltas
index|[
name|j
index|]
argument_list|,
name|result
argument_list|,
name|result_size
argument_list|,
name|type
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|result
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|compare_delta_entry
specifier|static
name|int
name|compare_delta_entry
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
name|delta_entry
modifier|*
name|delta_a
init|=
name|a
decl_stmt|;
specifier|const
name|struct
name|delta_entry
modifier|*
name|delta_b
init|=
name|b
decl_stmt|;
return|return
name|memcmp
argument_list|(
name|delta_a
operator|->
name|base_sha1
argument_list|,
name|delta_b
operator|->
name|base_sha1
argument_list|,
literal|20
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|parse_pack_objects
specifier|static
name|void
name|parse_pack_objects
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|unsigned
name|long
name|offset
init|=
sizeof|sizeof
argument_list|(
expr|struct
name|pack_header
argument_list|)
decl_stmt|;
name|unsigned
name|char
name|base_sha1
index|[
literal|20
index|]
decl_stmt|;
name|void
modifier|*
name|data
decl_stmt|;
name|unsigned
name|long
name|data_size
decl_stmt|;
comment|/* 	 * First pass: 	 * - find locations of all objects; 	 * - calculate SHA1 of all non-delta objects; 	 * - remember base SHA1 for all deltas. 	 */
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
name|object_entry
modifier|*
name|obj
init|=
operator|&
name|objects
index|[
name|i
index|]
decl_stmt|;
name|obj
operator|->
name|offset
operator|=
name|offset
expr_stmt|;
name|data
operator|=
name|unpack_raw_entry
argument_list|(
name|offset
argument_list|,
operator|&
name|obj
operator|->
name|type
argument_list|,
operator|&
name|data_size
argument_list|,
name|base_sha1
argument_list|,
operator|&
name|offset
argument_list|)
expr_stmt|;
name|obj
operator|->
name|real_type
operator|=
name|obj
operator|->
name|type
expr_stmt|;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_DELTA
condition|)
block|{
name|struct
name|delta_entry
modifier|*
name|delta
init|=
operator|&
name|deltas
index|[
name|nr_deltas
operator|++
index|]
decl_stmt|;
name|delta
operator|->
name|obj
operator|=
name|obj
expr_stmt|;
name|memcpy
argument_list|(
name|delta
operator|->
name|base_sha1
argument_list|,
name|base_sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
block|}
else|else
name|sha1_object
argument_list|(
name|data
argument_list|,
name|data_size
argument_list|,
name|obj
operator|->
name|type
argument_list|,
name|obj
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|offset
operator|!=
name|pack_size
operator|-
literal|20
condition|)
name|die
argument_list|(
literal|"packfile '%s' has junk at the end"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
comment|/* Sort deltas by base SHA1 for fast searching */
name|qsort
argument_list|(
name|deltas
argument_list|,
name|nr_deltas
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|delta_entry
argument_list|)
argument_list|,
name|compare_delta_entry
argument_list|)
expr_stmt|;
comment|/* 	 * Second pass: 	 * - for all non-delta objects, look if it is used as a base for 	 *   deltas; 	 * - if used as a base, uncompress the object and apply all deltas, 	 *   recursively checking if the resulting object is used as a base 	 *   for some more deltas. 	 */
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
name|object_entry
modifier|*
name|obj
init|=
operator|&
name|objects
index|[
name|i
index|]
decl_stmt|;
name|int
name|j
decl_stmt|,
name|first
decl_stmt|,
name|last
decl_stmt|;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_DELTA
condition|)
continue|continue;
if|if
condition|(
name|find_deltas_based_on_sha1
argument_list|(
name|obj
operator|->
name|sha1
argument_list|,
operator|&
name|first
argument_list|,
operator|&
name|last
argument_list|)
condition|)
continue|continue;
name|data
operator|=
name|unpack_raw_entry
argument_list|(
name|obj
operator|->
name|offset
argument_list|,
operator|&
name|obj
operator|->
name|type
argument_list|,
operator|&
name|data_size
argument_list|,
name|base_sha1
argument_list|,
operator|&
name|offset
argument_list|)
expr_stmt|;
for|for
control|(
name|j
operator|=
name|first
init|;
name|j
operator|<=
name|last
condition|;
name|j
operator|++
control|)
name|resolve_delta
argument_list|(
operator|&
name|deltas
index|[
name|j
index|]
argument_list|,
name|data
argument_list|,
name|data_size
argument_list|,
name|obj
operator|->
name|type
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
argument_list|)
expr_stmt|;
block|}
comment|/* Check for unresolved deltas */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_deltas
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|deltas
index|[
name|i
index|]
operator|.
name|obj
operator|->
name|real_type
operator|==
name|OBJ_DELTA
condition|)
name|die
argument_list|(
literal|"packfile '%s' has unresolved deltas"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
block|}
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
name|object_entry
modifier|*
name|a
init|=
operator|*
operator|(
expr|struct
name|object_entry
operator|*
operator|*
operator|)
name|_a
decl_stmt|;
name|struct
name|object_entry
modifier|*
name|b
init|=
operator|*
operator|(
expr|struct
name|object_entry
operator|*
operator|*
operator|)
name|_b
decl_stmt|;
return|return
name|memcmp
argument_list|(
name|a
operator|->
name|sha1
argument_list|,
name|b
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|write_index_file
specifier|static
name|void
name|write_index_file
parameter_list|(
specifier|const
name|char
modifier|*
name|index_name
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
name|object_entry
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
name|unsigned
name|int
name|array
index|[
literal|256
index|]
decl_stmt|;
name|int
name|i
decl_stmt|;
name|SHA_CTX
name|ctx
decl_stmt|;
if|if
condition|(
name|nr_objects
condition|)
block|{
name|sorted_by_sha
operator|=
name|xcalloc
argument_list|(
name|nr_objects
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|object_entry
operator|*
argument_list|)
argument_list|)
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
name|sorted_by_sha
index|[
name|i
index|]
operator|=
operator|&
name|objects
index|[
name|i
index|]
expr_stmt|;
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
name|unlink
argument_list|(
name|index_name
argument_list|)
expr_stmt|;
name|f
operator|=
name|sha1create
argument_list|(
literal|"%s"
argument_list|,
name|index_name
argument_list|)
expr_stmt|;
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
name|object_entry
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
name|object_entry
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
sizeof|sizeof
argument_list|(
name|int
argument_list|)
argument_list|)
expr_stmt|;
comment|/* recompute the SHA1 hash of sorted object names. 	 * currently pack-objects does not do this, but that 	 * can be fixed. 	 */
name|SHA1_Init
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
name|object_entry
modifier|*
name|obj
init|=
operator|*
name|list
operator|++
decl_stmt|;
name|unsigned
name|int
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
name|SHA1_Update
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
block|}
name|sha1write
argument_list|(
name|f
argument_list|,
name|pack_base
operator|+
name|pack_size
operator|-
literal|20
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
literal|1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|sorted_by_sha
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
name|int
name|i
decl_stmt|;
name|char
modifier|*
name|index_name
init|=
name|NULL
decl_stmt|;
name|char
modifier|*
name|index_name_buf
init|=
name|NULL
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|*
name|arg
operator|==
literal|'-'
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-o"
argument_list|)
condition|)
block|{
if|if
condition|(
name|index_name
operator|||
operator|(
name|i
operator|+
literal|1
operator|)
operator|>=
name|argc
condition|)
name|usage
argument_list|(
name|index_pack_usage
argument_list|)
expr_stmt|;
name|index_name
operator|=
name|argv
index|[
operator|++
name|i
index|]
expr_stmt|;
block|}
else|else
name|usage
argument_list|(
name|index_pack_usage
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|pack_name
condition|)
name|usage
argument_list|(
name|index_pack_usage
argument_list|)
expr_stmt|;
name|pack_name
operator|=
name|arg
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|pack_name
condition|)
name|usage
argument_list|(
name|index_pack_usage
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|index_name
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|pack_name
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
operator|<
literal|5
operator|||
name|strcmp
argument_list|(
name|pack_name
operator|+
name|len
operator|-
literal|5
argument_list|,
literal|".pack"
argument_list|)
condition|)
name|die
argument_list|(
literal|"packfile name '%s' does not end with '.pack'"
argument_list|,
name|pack_name
argument_list|)
expr_stmt|;
name|index_name_buf
operator|=
name|xmalloc
argument_list|(
name|len
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|index_name_buf
argument_list|,
name|pack_name
argument_list|,
name|len
operator|-
literal|5
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|index_name_buf
operator|+
name|len
operator|-
literal|5
argument_list|,
literal|".idx"
argument_list|)
expr_stmt|;
name|index_name
operator|=
name|index_name_buf
expr_stmt|;
block|}
name|open_pack_file
argument_list|()
expr_stmt|;
name|parse_pack_header
argument_list|()
expr_stmt|;
name|objects
operator|=
name|xcalloc
argument_list|(
name|nr_objects
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|object_entry
argument_list|)
argument_list|)
expr_stmt|;
name|deltas
operator|=
name|xcalloc
argument_list|(
name|nr_objects
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|delta_entry
argument_list|)
argument_list|)
expr_stmt|;
name|parse_pack_objects
argument_list|()
expr_stmt|;
name|free
argument_list|(
name|deltas
argument_list|)
expr_stmt|;
name|write_index_file
argument_list|(
name|index_name
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|objects
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|index_name_buf
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

