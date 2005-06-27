begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<ctype.h>
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"delta.h"
end_include

begin_include
include|#
directive|include
file|"csum-file.h"
end_include

begin_decl_stmt
DECL|variable|pack_usage
specifier|static
specifier|const
name|char
name|pack_usage
index|[]
init|=
literal|"git-pack-objects [--window=N] [--depth=N] base-name< object-list"
decl_stmt|;
end_decl_stmt

begin_enum
DECL|enum|object_type
enum|enum
name|object_type
block|{
DECL|enumerator|OBJ_NONE
name|OBJ_NONE
block|,
DECL|enumerator|OBJ_COMMIT
name|OBJ_COMMIT
block|,
DECL|enumerator|OBJ_TREE
name|OBJ_TREE
block|,
DECL|enumerator|OBJ_BLOB
name|OBJ_BLOB
block|,
DECL|enumerator|OBJ_DELTA
name|OBJ_DELTA
block|}
enum|;
end_enum

begin_struct
DECL|struct|object_entry
struct|struct
name|object_entry
block|{
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|size
name|unsigned
name|long
name|size
decl_stmt|;
DECL|member|offset
name|unsigned
name|long
name|offset
decl_stmt|;
DECL|member|depth
name|unsigned
name|int
name|depth
decl_stmt|;
DECL|member|hash
name|unsigned
name|int
name|hash
decl_stmt|;
DECL|member|type
name|enum
name|object_type
name|type
decl_stmt|;
DECL|member|delta_size
name|unsigned
name|long
name|delta_size
decl_stmt|;
DECL|member|delta
name|struct
name|object_entry
modifier|*
name|delta
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|sorted_by_sha
DECL|variable|sorted_by_type
specifier|static
name|struct
name|object_entry
modifier|*
modifier|*
name|sorted_by_sha
decl_stmt|,
modifier|*
modifier|*
name|sorted_by_type
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|objects
specifier|static
name|struct
name|object_entry
modifier|*
name|objects
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nr_objects
DECL|variable|nr_alloc
specifier|static
name|int
name|nr_objects
init|=
literal|0
decl_stmt|,
name|nr_alloc
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|base_name
specifier|static
specifier|const
name|char
modifier|*
name|base_name
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pack_file_sha1
specifier|static
name|unsigned
name|char
name|pack_file_sha1
index|[
literal|20
index|]
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|delta_against
specifier|static
name|void
modifier|*
name|delta_against
parameter_list|(
name|void
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|struct
name|object_entry
modifier|*
name|entry
parameter_list|)
block|{
name|unsigned
name|long
name|othersize
decl_stmt|,
name|delta_size
decl_stmt|;
name|char
name|type
index|[
literal|10
index|]
decl_stmt|;
name|void
modifier|*
name|otherbuf
init|=
name|read_sha1_file
argument_list|(
name|entry
operator|->
name|delta
operator|->
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|othersize
argument_list|)
decl_stmt|;
name|void
modifier|*
name|delta_buf
decl_stmt|;
if|if
condition|(
operator|!
name|otherbuf
condition|)
name|die
argument_list|(
literal|"unable to read %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|delta
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|delta_buf
operator|=
name|diff_delta
argument_list|(
name|otherbuf
argument_list|,
name|othersize
argument_list|,
name|buf
argument_list|,
name|size
argument_list|,
operator|&
name|delta_size
argument_list|,
operator|~
literal|0UL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|delta_buf
operator|||
name|delta_size
operator|!=
name|entry
operator|->
name|delta_size
condition|)
name|die
argument_list|(
literal|"delta size changed"
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|otherbuf
argument_list|)
expr_stmt|;
return|return
name|delta_buf
return|;
block|}
end_function

begin_function
DECL|function|write_object
specifier|static
name|unsigned
name|long
name|write_object
parameter_list|(
name|struct
name|sha1file
modifier|*
name|f
parameter_list|,
name|struct
name|object_entry
modifier|*
name|entry
parameter_list|)
block|{
name|unsigned
name|long
name|size
decl_stmt|;
name|char
name|type
index|[
literal|10
index|]
decl_stmt|;
name|void
modifier|*
name|buf
init|=
name|read_sha1_file
argument_list|(
name|entry
operator|->
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|size
argument_list|)
decl_stmt|;
name|char
name|header
index|[
literal|25
index|]
decl_stmt|;
name|unsigned
name|hdrlen
decl_stmt|,
name|datalen
decl_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
name|die
argument_list|(
literal|"unable to read %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|size
operator|!=
name|entry
operator|->
name|size
condition|)
name|die
argument_list|(
literal|"object %s size inconsistency (%lu vs %lu)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|sha1
argument_list|)
argument_list|,
name|size
argument_list|,
name|entry
operator|->
name|size
argument_list|)
expr_stmt|;
comment|/* 	 * The object header is a byte of 'type' followed by four bytes of 	 * length, except for deltas that has the 20 bytes of delta sha 	 * instead. 	 */
name|header
index|[
literal|0
index|]
operator|=
literal|".CTB"
index|[
name|entry
operator|->
name|type
index|]
expr_stmt|;
name|hdrlen
operator|=
literal|5
expr_stmt|;
if|if
condition|(
name|entry
operator|->
name|delta
condition|)
block|{
name|header
index|[
literal|0
index|]
operator|=
literal|'D'
expr_stmt|;
name|memcpy
argument_list|(
name|header
operator|+
literal|5
argument_list|,
name|entry
operator|->
name|delta
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|buf
operator|=
name|delta_against
argument_list|(
name|buf
argument_list|,
name|size
argument_list|,
name|entry
argument_list|)
expr_stmt|;
name|size
operator|=
name|entry
operator|->
name|delta_size
expr_stmt|;
name|hdrlen
operator|=
literal|25
expr_stmt|;
block|}
name|datalen
operator|=
name|htonl
argument_list|(
name|size
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|header
operator|+
literal|1
argument_list|,
operator|&
name|datalen
argument_list|,
literal|4
argument_list|)
expr_stmt|;
name|sha1write
argument_list|(
name|f
argument_list|,
name|header
argument_list|,
name|hdrlen
argument_list|)
expr_stmt|;
name|datalen
operator|=
name|sha1write_compressed
argument_list|(
name|f
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return
name|hdrlen
operator|+
name|datalen
return|;
block|}
end_function

begin_function
DECL|function|write_pack_file
specifier|static
name|void
name|write_pack_file
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|struct
name|sha1file
modifier|*
name|f
init|=
name|sha1create
argument_list|(
literal|"%s.%s"
argument_list|,
name|base_name
argument_list|,
literal|"pack"
argument_list|)
decl_stmt|;
name|unsigned
name|long
name|offset
init|=
literal|0
decl_stmt|;
name|unsigned
name|long
name|mb
decl_stmt|;
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
name|entry
init|=
name|objects
operator|+
name|i
decl_stmt|;
name|entry
operator|->
name|offset
operator|=
name|offset
expr_stmt|;
name|offset
operator|+=
name|write_object
argument_list|(
name|f
argument_list|,
name|entry
argument_list|)
expr_stmt|;
block|}
name|sha1close
argument_list|(
name|f
argument_list|,
name|pack_file_sha1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|mb
operator|=
name|offset
operator|>>
literal|20
expr_stmt|;
name|offset
operator|&=
literal|0xfffff
expr_stmt|;
block|}
end_function

begin_function
DECL|function|write_index_file
specifier|static
name|void
name|write_index_file
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|struct
name|sha1file
modifier|*
name|f
init|=
name|sha1create
argument_list|(
literal|"%s.%s"
argument_list|,
name|base_name
argument_list|,
literal|"idx"
argument_list|)
decl_stmt|;
name|struct
name|object_entry
modifier|*
modifier|*
name|list
init|=
name|sorted_by_sha
decl_stmt|;
name|struct
name|object_entry
modifier|*
modifier|*
name|last
init|=
name|list
operator|+
name|nr_objects
decl_stmt|;
name|unsigned
name|int
name|array
index|[
literal|256
index|]
decl_stmt|;
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
name|entry
init|=
operator|*
name|next
decl_stmt|;
if|if
condition|(
name|entry
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
name|entry
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
name|entry
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
name|entry
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
name|pack_file_sha1
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
block|}
end_function

begin_function
DECL|function|add_object_entry
specifier|static
name|void
name|add_object_entry
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
name|int
name|hash
parameter_list|)
block|{
name|unsigned
name|int
name|idx
init|=
name|nr_objects
decl_stmt|;
name|struct
name|object_entry
modifier|*
name|entry
decl_stmt|;
if|if
condition|(
name|idx
operator|>=
name|nr_alloc
condition|)
block|{
name|unsigned
name|int
name|needed
init|=
operator|(
name|idx
operator|+
literal|1024
operator|)
operator|*
literal|3
operator|/
literal|2
decl_stmt|;
name|objects
operator|=
name|xrealloc
argument_list|(
name|objects
argument_list|,
name|needed
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|entry
argument_list|)
argument_list|)
expr_stmt|;
name|nr_alloc
operator|=
name|needed
expr_stmt|;
block|}
name|entry
operator|=
name|objects
operator|+
name|idx
expr_stmt|;
name|memset
argument_list|(
name|entry
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|entry
argument_list|)
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|entry
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|entry
operator|->
name|hash
operator|=
name|hash
expr_stmt|;
name|nr_objects
operator|=
name|idx
operator|+
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|check_object
specifier|static
name|void
name|check_object
parameter_list|(
name|struct
name|object_entry
modifier|*
name|entry
parameter_list|)
block|{
name|char
name|buffer
index|[
literal|128
index|]
decl_stmt|;
name|char
name|type
index|[
literal|10
index|]
decl_stmt|;
name|unsigned
name|long
name|mapsize
decl_stmt|;
name|z_stream
name|stream
decl_stmt|;
name|void
modifier|*
name|map
decl_stmt|;
name|map
operator|=
name|map_sha1_file
argument_list|(
name|entry
operator|->
name|sha1
argument_list|,
operator|&
name|mapsize
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|map
condition|)
name|die
argument_list|(
literal|"unable to map %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|unpack_sha1_header
argument_list|(
operator|&
name|stream
argument_list|,
name|map
argument_list|,
name|mapsize
argument_list|,
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to unpack %s header"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|munmap
argument_list|(
name|map
argument_list|,
name|mapsize
argument_list|)
expr_stmt|;
if|if
condition|(
name|parse_sha1_header
argument_list|(
name|buffer
argument_list|,
name|type
argument_list|,
operator|&
name|entry
operator|->
name|size
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to parse %s header"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"commit"
argument_list|)
condition|)
block|{
name|entry
operator|->
name|type
operator|=
name|OBJ_COMMIT
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tree"
argument_list|)
condition|)
block|{
name|entry
operator|->
name|type
operator|=
name|OBJ_TREE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"blob"
argument_list|)
condition|)
block|{
name|entry
operator|->
name|type
operator|=
name|OBJ_BLOB
expr_stmt|;
block|}
else|else
name|die
argument_list|(
literal|"unable to pack object %s of type %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|sha1
argument_list|)
argument_list|,
name|type
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_object_details
specifier|static
name|void
name|get_object_details
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|struct
name|object_entry
modifier|*
name|entry
init|=
name|objects
decl_stmt|;
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
name|check_object
argument_list|(
name|entry
operator|++
argument_list|)
expr_stmt|;
block|}
end_function

begin_typedef
DECL|typedef|entry_sort_t
typedef|typedef
name|int
function_decl|(
modifier|*
name|entry_sort_t
function_decl|)
parameter_list|(
specifier|const
name|struct
name|object_entry
modifier|*
parameter_list|,
specifier|const
name|struct
name|object_entry
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_decl_stmt
DECL|variable|current_sort
specifier|static
name|entry_sort_t
name|current_sort
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|sort_comparator
specifier|static
name|int
name|sort_comparator
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
name|current_sort
argument_list|(
name|a
argument_list|,
name|b
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|create_sorted_list
specifier|static
name|struct
name|object_entry
modifier|*
modifier|*
name|create_sorted_list
parameter_list|(
name|entry_sort_t
name|sort
parameter_list|)
block|{
name|struct
name|object_entry
modifier|*
modifier|*
name|list
init|=
name|xmalloc
argument_list|(
name|nr_objects
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|object_entry
operator|*
argument_list|)
argument_list|)
decl_stmt|;
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
name|nr_objects
condition|;
name|i
operator|++
control|)
name|list
index|[
name|i
index|]
operator|=
name|objects
operator|+
name|i
expr_stmt|;
name|current_sort
operator|=
name|sort
expr_stmt|;
name|qsort
argument_list|(
name|list
argument_list|,
name|nr_objects
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|object_entry
operator|*
argument_list|)
argument_list|,
name|sort_comparator
argument_list|)
expr_stmt|;
return|return
name|list
return|;
block|}
end_function

begin_function
DECL|function|sha1_sort
specifier|static
name|int
name|sha1_sort
parameter_list|(
specifier|const
name|struct
name|object_entry
modifier|*
name|a
parameter_list|,
specifier|const
name|struct
name|object_entry
modifier|*
name|b
parameter_list|)
block|{
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
DECL|function|type_size_sort
specifier|static
name|int
name|type_size_sort
parameter_list|(
specifier|const
name|struct
name|object_entry
modifier|*
name|a
parameter_list|,
specifier|const
name|struct
name|object_entry
modifier|*
name|b
parameter_list|)
block|{
if|if
condition|(
name|a
operator|->
name|type
operator|<
name|b
operator|->
name|type
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|a
operator|->
name|type
operator|>
name|b
operator|->
name|type
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|a
operator|->
name|hash
operator|<
name|b
operator|->
name|hash
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|a
operator|->
name|hash
operator|>
name|b
operator|->
name|hash
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|a
operator|->
name|size
operator|<
name|b
operator|->
name|size
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|a
operator|->
name|size
operator|>
name|b
operator|->
name|size
condition|)
return|return
literal|1
return|;
return|return
name|a
operator|<
name|b
condition|?
operator|-
literal|1
else|:
operator|(
name|a
operator|>
name|b
operator|)
return|;
block|}
end_function

begin_struct
DECL|struct|unpacked
struct|struct
name|unpacked
block|{
DECL|member|entry
name|struct
name|object_entry
modifier|*
name|entry
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * We search for deltas _backwards_ in a list sorted by type and  * by size, so that we see progressively smaller and smaller files.  * That's because we prefer deltas to be from the bigger file  * to the smaller - deletes are potentially cheaper, but perhaps  * more importantly, the bigger file is likely the more recent  * one.  */
end_comment

begin_function
DECL|function|try_delta
specifier|static
name|int
name|try_delta
parameter_list|(
name|struct
name|unpacked
modifier|*
name|cur
parameter_list|,
name|struct
name|unpacked
modifier|*
name|old
parameter_list|,
name|unsigned
name|max_depth
parameter_list|)
block|{
name|struct
name|object_entry
modifier|*
name|cur_entry
init|=
name|cur
operator|->
name|entry
decl_stmt|;
name|struct
name|object_entry
modifier|*
name|old_entry
init|=
name|old
operator|->
name|entry
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|,
name|oldsize
decl_stmt|,
name|delta_size
decl_stmt|,
name|sizediff
decl_stmt|;
name|long
name|max_size
decl_stmt|;
name|void
modifier|*
name|delta_buf
decl_stmt|;
comment|/* Don't bother doing diffs between different types */
if|if
condition|(
name|cur_entry
operator|->
name|type
operator|!=
name|old_entry
operator|->
name|type
condition|)
return|return
operator|-
literal|1
return|;
name|size
operator|=
name|cur_entry
operator|->
name|size
expr_stmt|;
if|if
condition|(
name|size
operator|<
literal|50
condition|)
return|return
operator|-
literal|1
return|;
name|oldsize
operator|=
name|old_entry
operator|->
name|size
expr_stmt|;
name|sizediff
operator|=
name|oldsize
operator|>
name|size
condition|?
name|oldsize
operator|-
name|size
else|:
name|size
operator|-
name|oldsize
expr_stmt|;
if|if
condition|(
name|sizediff
operator|>
name|size
operator|/
literal|8
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|old_entry
operator|->
name|depth
operator|>=
name|max_depth
condition|)
return|return
literal|0
return|;
comment|/* 	 * NOTE! 	 * 	 * We always delta from the bigger to the smaller, since that's 	 * more space-efficient (deletes don't have to say _what_ they 	 * delete). 	 */
name|max_size
operator|=
name|size
operator|/
literal|2
operator|-
literal|20
expr_stmt|;
if|if
condition|(
name|cur_entry
operator|->
name|delta
condition|)
name|max_size
operator|=
name|cur_entry
operator|->
name|delta_size
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|sizediff
operator|>=
name|max_size
condition|)
return|return
operator|-
literal|1
return|;
name|delta_buf
operator|=
name|diff_delta
argument_list|(
name|old
operator|->
name|data
argument_list|,
name|oldsize
argument_list|,
name|cur
operator|->
name|data
argument_list|,
name|size
argument_list|,
operator|&
name|delta_size
argument_list|,
name|max_size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|delta_buf
condition|)
return|return
literal|0
return|;
name|cur_entry
operator|->
name|delta
operator|=
name|old_entry
expr_stmt|;
name|cur_entry
operator|->
name|delta_size
operator|=
name|delta_size
expr_stmt|;
name|cur_entry
operator|->
name|depth
operator|=
name|old_entry
operator|->
name|depth
operator|+
literal|1
expr_stmt|;
name|free
argument_list|(
name|delta_buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|find_deltas
specifier|static
name|void
name|find_deltas
parameter_list|(
name|struct
name|object_entry
modifier|*
modifier|*
name|list
parameter_list|,
name|int
name|window
parameter_list|,
name|int
name|depth
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|idx
decl_stmt|;
name|unsigned
name|int
name|array_size
init|=
name|window
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|unpacked
argument_list|)
decl_stmt|;
name|struct
name|unpacked
modifier|*
name|array
init|=
name|xmalloc
argument_list|(
name|array_size
argument_list|)
decl_stmt|;
name|memset
argument_list|(
name|array
argument_list|,
literal|0
argument_list|,
name|array_size
argument_list|)
expr_stmt|;
name|i
operator|=
name|nr_objects
expr_stmt|;
name|idx
operator|=
literal|0
expr_stmt|;
while|while
condition|(
operator|--
name|i
operator|>=
literal|0
condition|)
block|{
name|struct
name|object_entry
modifier|*
name|entry
init|=
name|list
index|[
name|i
index|]
decl_stmt|;
name|struct
name|unpacked
modifier|*
name|n
init|=
name|array
operator|+
name|idx
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|char
name|type
index|[
literal|10
index|]
decl_stmt|;
name|int
name|j
decl_stmt|;
name|free
argument_list|(
name|n
operator|->
name|data
argument_list|)
expr_stmt|;
name|n
operator|->
name|entry
operator|=
name|entry
expr_stmt|;
name|n
operator|->
name|data
operator|=
name|read_sha1_file
argument_list|(
name|entry
operator|->
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|size
operator|!=
name|entry
operator|->
name|size
condition|)
name|die
argument_list|(
literal|"object %s inconsistent object length (%lu vs %lu)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|entry
operator|->
name|sha1
argument_list|)
argument_list|,
name|size
argument_list|,
name|entry
operator|->
name|size
argument_list|)
expr_stmt|;
name|j
operator|=
name|window
expr_stmt|;
while|while
condition|(
operator|--
name|j
operator|>
literal|0
condition|)
block|{
name|unsigned
name|int
name|other_idx
init|=
name|idx
operator|+
name|j
decl_stmt|;
name|struct
name|unpacked
modifier|*
name|m
decl_stmt|;
if|if
condition|(
name|other_idx
operator|>=
name|window
condition|)
name|other_idx
operator|-=
name|window
expr_stmt|;
name|m
operator|=
name|array
operator|+
name|other_idx
expr_stmt|;
if|if
condition|(
operator|!
name|m
operator|->
name|entry
condition|)
break|break;
if|if
condition|(
name|try_delta
argument_list|(
name|n
argument_list|,
name|m
argument_list|,
name|depth
argument_list|)
operator|<
literal|0
condition|)
break|break;
block|}
name|idx
operator|++
expr_stmt|;
if|if
condition|(
name|idx
operator|>=
name|window
condition|)
name|idx
operator|=
literal|0
expr_stmt|;
block|}
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
name|char
name|line
index|[
name|PATH_MAX
operator|+
literal|20
index|]
decl_stmt|;
name|int
name|window
init|=
literal|10
decl_stmt|,
name|depth
init|=
literal|10
decl_stmt|;
name|int
name|i
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
name|strncmp
argument_list|(
literal|"--window="
argument_list|,
name|arg
argument_list|,
literal|9
argument_list|)
condition|)
block|{
name|char
modifier|*
name|end
decl_stmt|;
name|window
operator|=
name|strtoul
argument_list|(
name|arg
operator|+
literal|9
argument_list|,
operator|&
name|end
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|arg
index|[
literal|9
index|]
operator|||
operator|*
name|end
condition|)
name|usage
argument_list|(
name|pack_usage
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
literal|"--depth="
argument_list|,
name|arg
argument_list|,
literal|8
argument_list|)
condition|)
block|{
name|char
modifier|*
name|end
decl_stmt|;
name|depth
operator|=
name|strtoul
argument_list|(
name|arg
operator|+
literal|8
argument_list|,
operator|&
name|end
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|arg
index|[
literal|8
index|]
operator|||
operator|*
name|end
condition|)
name|usage
argument_list|(
name|pack_usage
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|pack_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|base_name
condition|)
name|usage
argument_list|(
name|pack_usage
argument_list|)
expr_stmt|;
name|base_name
operator|=
name|arg
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|base_name
condition|)
name|usage
argument_list|(
name|pack_usage
argument_list|)
expr_stmt|;
while|while
condition|(
name|fgets
argument_list|(
name|line
argument_list|,
sizeof|sizeof
argument_list|(
name|line
argument_list|)
argument_list|,
name|stdin
argument_list|)
operator|!=
name|NULL
condition|)
block|{
name|unsigned
name|int
name|hash
decl_stmt|;
name|char
modifier|*
name|p
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|line
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"expected sha1, got garbage"
argument_list|)
expr_stmt|;
name|hash
operator|=
literal|0
expr_stmt|;
name|p
operator|=
name|line
operator|+
literal|40
expr_stmt|;
while|while
condition|(
operator|*
name|p
condition|)
block|{
name|unsigned
name|char
name|c
init|=
operator|*
name|p
operator|++
decl_stmt|;
if|if
condition|(
name|isspace
argument_list|(
name|c
argument_list|)
condition|)
continue|continue;
name|hash
operator|=
name|hash
operator|*
literal|11
operator|+
name|c
expr_stmt|;
block|}
name|add_object_entry
argument_list|(
name|sha1
argument_list|,
name|hash
argument_list|)
expr_stmt|;
block|}
name|get_object_details
argument_list|()
expr_stmt|;
name|printf
argument_list|(
literal|"Packing %d objects\n"
argument_list|,
name|nr_objects
argument_list|)
expr_stmt|;
name|sorted_by_sha
operator|=
name|create_sorted_list
argument_list|(
name|sha1_sort
argument_list|)
expr_stmt|;
name|sorted_by_type
operator|=
name|create_sorted_list
argument_list|(
name|type_size_sort
argument_list|)
expr_stmt|;
if|if
condition|(
name|window
operator|&&
name|depth
condition|)
name|find_deltas
argument_list|(
name|sorted_by_type
argument_list|,
name|window
operator|+
literal|1
argument_list|,
name|depth
argument_list|)
expr_stmt|;
name|write_pack_file
argument_list|()
expr_stmt|;
name|write_index_file
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

