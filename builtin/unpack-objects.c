begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
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
file|"pack.h"
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

begin_include
include|#
directive|include
file|"tree-walk.h"
end_include

begin_include
include|#
directive|include
file|"progress.h"
end_include

begin_include
include|#
directive|include
file|"decorate.h"
end_include

begin_include
include|#
directive|include
file|"fsck.h"
end_include

begin_decl_stmt
DECL|variable|dry_run
DECL|variable|quiet
DECL|variable|recover
DECL|variable|has_errors
DECL|variable|strict
specifier|static
name|int
name|dry_run
decl_stmt|,
name|quiet
decl_stmt|,
name|recover
decl_stmt|,
name|has_errors
decl_stmt|,
name|strict
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|unpack_usage
specifier|static
specifier|const
name|char
name|unpack_usage
index|[]
init|=
literal|"git unpack-objects [-n] [-q] [-r] [--strict]< pack-file"
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* We always read in 4kB chunks. */
end_comment

begin_decl_stmt
DECL|variable|buffer
specifier|static
name|unsigned
name|char
name|buffer
index|[
literal|4096
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|offset
DECL|variable|len
specifier|static
name|unsigned
name|int
name|offset
decl_stmt|,
name|len
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|consumed_bytes
specifier|static
name|off_t
name|consumed_bytes
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ctx
specifier|static
name|git_SHA_CTX
name|ctx
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * When running under --strict mode, objects whose reachability are  * suspect are kept in core without getting written in the object  * store.  */
end_comment

begin_struct
DECL|struct|obj_buffer
struct|struct
name|obj_buffer
block|{
DECL|member|buffer
name|char
modifier|*
name|buffer
decl_stmt|;
DECL|member|size
name|unsigned
name|long
name|size
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|obj_decorate
specifier|static
name|struct
name|decoration
name|obj_decorate
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|lookup_object_buffer
specifier|static
name|struct
name|obj_buffer
modifier|*
name|lookup_object_buffer
parameter_list|(
name|struct
name|object
modifier|*
name|base
parameter_list|)
block|{
return|return
name|lookup_decoration
argument_list|(
operator|&
name|obj_decorate
argument_list|,
name|base
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|add_object_buffer
specifier|static
name|void
name|add_object_buffer
parameter_list|(
name|struct
name|object
modifier|*
name|object
parameter_list|,
name|char
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|struct
name|obj_buffer
modifier|*
name|obj
decl_stmt|;
name|obj
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|obj_buffer
argument_list|)
argument_list|)
expr_stmt|;
name|obj
operator|->
name|buffer
operator|=
name|buffer
expr_stmt|;
name|obj
operator|->
name|size
operator|=
name|size
expr_stmt|;
if|if
condition|(
name|add_decoration
argument_list|(
operator|&
name|obj_decorate
argument_list|,
name|object
argument_list|,
name|obj
argument_list|)
condition|)
name|die
argument_list|(
literal|"object %s tried to add buffer twice!"
argument_list|,
name|sha1_to_hex
argument_list|(
name|object
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * Make sure at least "min" bytes are available in the buffer, and  * return the pointer to the buffer.  */
end_comment

begin_function
DECL|function|fill
specifier|static
name|void
modifier|*
name|fill
parameter_list|(
name|int
name|min
parameter_list|)
block|{
if|if
condition|(
name|min
operator|<=
name|len
condition|)
return|return
name|buffer
operator|+
name|offset
return|;
if|if
condition|(
name|min
operator|>
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
condition|)
name|die
argument_list|(
literal|"cannot fill %d bytes"
argument_list|,
name|min
argument_list|)
expr_stmt|;
if|if
condition|(
name|offset
condition|)
block|{
name|git_SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
name|buffer
argument_list|,
name|offset
argument_list|)
expr_stmt|;
name|memmove
argument_list|(
name|buffer
argument_list|,
name|buffer
operator|+
name|offset
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|offset
operator|=
literal|0
expr_stmt|;
block|}
do|do
block|{
name|ssize_t
name|ret
init|=
name|xread
argument_list|(
literal|0
argument_list|,
name|buffer
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
operator|-
name|len
argument_list|)
decl_stmt|;
if|if
condition|(
name|ret
operator|<=
literal|0
condition|)
block|{
if|if
condition|(
operator|!
name|ret
condition|)
name|die
argument_list|(
literal|"early EOF"
argument_list|)
expr_stmt|;
name|die_errno
argument_list|(
literal|"read error on input"
argument_list|)
expr_stmt|;
block|}
name|len
operator|+=
name|ret
expr_stmt|;
block|}
do|while
condition|(
name|len
operator|<
name|min
condition|)
do|;
return|return
name|buffer
return|;
block|}
end_function

begin_function
DECL|function|use
specifier|static
name|void
name|use
parameter_list|(
name|int
name|bytes
parameter_list|)
block|{
if|if
condition|(
name|bytes
operator|>
name|len
condition|)
name|die
argument_list|(
literal|"used more bytes than were available"
argument_list|)
expr_stmt|;
name|len
operator|-=
name|bytes
expr_stmt|;
name|offset
operator|+=
name|bytes
expr_stmt|;
comment|/* make sure off_t is sufficiently large not to wrap */
if|if
condition|(
name|signed_add_overflows
argument_list|(
name|consumed_bytes
argument_list|,
name|bytes
argument_list|)
condition|)
name|die
argument_list|(
literal|"pack too large for current definition of off_t"
argument_list|)
expr_stmt|;
name|consumed_bytes
operator|+=
name|bytes
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_data
specifier|static
name|void
modifier|*
name|get_data
parameter_list|(
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|git_zstream
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
name|fill
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|stream
operator|.
name|avail_in
operator|=
name|len
expr_stmt|;
name|git_inflate_init
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
name|git_inflate
argument_list|(
operator|&
name|stream
argument_list|,
literal|0
argument_list|)
decl_stmt|;
name|use
argument_list|(
name|len
operator|-
name|stream
operator|.
name|avail_in
argument_list|)
expr_stmt|;
if|if
condition|(
name|stream
operator|.
name|total_out
operator|==
name|size
operator|&&
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
block|{
name|error
argument_list|(
literal|"inflate returned %d\n"
argument_list|,
name|ret
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
name|buf
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
operator|!
name|recover
condition|)
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|has_errors
operator|=
literal|1
expr_stmt|;
break|break;
block|}
name|stream
operator|.
name|next_in
operator|=
name|fill
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|stream
operator|.
name|avail_in
operator|=
name|len
expr_stmt|;
block|}
name|git_inflate_end
argument_list|(
operator|&
name|stream
argument_list|)
expr_stmt|;
return|return
name|buf
return|;
block|}
end_function

begin_struct
DECL|struct|delta_info
struct|struct
name|delta_info
block|{
DECL|member|base_sha1
name|unsigned
name|char
name|base_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|nr
name|unsigned
name|nr
decl_stmt|;
DECL|member|base_offset
name|off_t
name|base_offset
decl_stmt|;
DECL|member|size
name|unsigned
name|long
name|size
decl_stmt|;
DECL|member|delta
name|void
modifier|*
name|delta
decl_stmt|;
DECL|member|next
name|struct
name|delta_info
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|delta_list
specifier|static
name|struct
name|delta_info
modifier|*
name|delta_list
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|add_delta_to_list
specifier|static
name|void
name|add_delta_to_list
parameter_list|(
name|unsigned
name|nr
parameter_list|,
name|unsigned
specifier|const
name|char
modifier|*
name|base_sha1
parameter_list|,
name|off_t
name|base_offset
parameter_list|,
name|void
modifier|*
name|delta
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|struct
name|delta_info
modifier|*
name|info
init|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|info
argument_list|)
argument_list|)
decl_stmt|;
name|hashcpy
argument_list|(
name|info
operator|->
name|base_sha1
argument_list|,
name|base_sha1
argument_list|)
expr_stmt|;
name|info
operator|->
name|base_offset
operator|=
name|base_offset
expr_stmt|;
name|info
operator|->
name|size
operator|=
name|size
expr_stmt|;
name|info
operator|->
name|delta
operator|=
name|delta
expr_stmt|;
name|info
operator|->
name|nr
operator|=
name|nr
expr_stmt|;
name|info
operator|->
name|next
operator|=
name|delta_list
expr_stmt|;
name|delta_list
operator|=
name|info
expr_stmt|;
block|}
end_function

begin_struct
DECL|struct|obj_info
struct|struct
name|obj_info
block|{
DECL|member|offset
name|off_t
name|offset
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|obj
name|struct
name|object
modifier|*
name|obj
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|FLAG_OPEN
define|#
directive|define
name|FLAG_OPEN
value|(1u<<20)
end_define

begin_define
DECL|macro|FLAG_WRITTEN
define|#
directive|define
name|FLAG_WRITTEN
value|(1u<<21)
end_define

begin_decl_stmt
DECL|variable|obj_list
specifier|static
name|struct
name|obj_info
modifier|*
name|obj_list
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nr_objects
specifier|static
name|unsigned
name|nr_objects
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Called only from check_object() after it verified this object  * is Ok.  */
end_comment

begin_function
DECL|function|write_cached_object
specifier|static
name|void
name|write_cached_object
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|obj_buffer
modifier|*
name|obj_buf
init|=
name|lookup_object_buffer
argument_list|(
name|obj
argument_list|)
decl_stmt|;
if|if
condition|(
name|write_sha1_file
argument_list|(
name|obj_buf
operator|->
name|buffer
argument_list|,
name|obj_buf
operator|->
name|size
argument_list|,
name|typename
argument_list|(
name|obj
operator|->
name|type
argument_list|)
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"failed to write object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|obj
operator|->
name|flags
operator||=
name|FLAG_WRITTEN
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * At the very end of the processing, write_rest() scans the objects  * that have reachability requirements and calls this function.  * Verify its reachability and validity recursively and write it out.  */
end_comment

begin_function
DECL|function|check_object
specifier|static
name|int
name|check_object
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|int
name|type
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
if|if
condition|(
operator|!
name|obj
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|obj
operator|->
name|flags
operator|&
name|FLAG_WRITTEN
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|type
operator|!=
name|OBJ_ANY
operator|&&
name|obj
operator|->
name|type
operator|!=
name|type
condition|)
name|die
argument_list|(
literal|"object type mismatch"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|obj
operator|->
name|flags
operator|&
name|FLAG_OPEN
operator|)
condition|)
block|{
name|unsigned
name|long
name|size
decl_stmt|;
name|int
name|type
init|=
name|sha1_object_info
argument_list|(
name|obj
operator|->
name|sha1
argument_list|,
operator|&
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
name|type
operator|!=
name|obj
operator|->
name|type
operator|||
name|type
operator|<=
literal|0
condition|)
name|die
argument_list|(
literal|"object of unexpected type"
argument_list|)
expr_stmt|;
name|obj
operator|->
name|flags
operator||=
name|FLAG_WRITTEN
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|fsck_object
argument_list|(
name|obj
argument_list|,
literal|1
argument_list|,
name|fsck_error_function
argument_list|)
condition|)
name|die
argument_list|(
literal|"Error in object"
argument_list|)
expr_stmt|;
if|if
condition|(
name|fsck_walk
argument_list|(
name|obj
argument_list|,
name|check_object
argument_list|,
name|NULL
argument_list|)
condition|)
name|die
argument_list|(
literal|"Error on reachable objects of %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|write_cached_object
argument_list|(
name|obj
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|write_rest
specifier|static
name|void
name|write_rest
parameter_list|(
name|void
parameter_list|)
block|{
name|unsigned
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
block|{
if|if
condition|(
name|obj_list
index|[
name|i
index|]
operator|.
name|obj
condition|)
name|check_object
argument_list|(
name|obj_list
index|[
name|i
index|]
operator|.
name|obj
argument_list|,
name|OBJ_ANY
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function_decl
specifier|static
name|void
name|added_object
parameter_list|(
name|unsigned
name|nr
parameter_list|,
name|enum
name|object_type
name|type
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Write out nr-th object from the list, now we know the contents  * of it.  Under --strict, this buffers structured objects in-core,  * to be checked at the end.  */
end_comment

begin_function
DECL|function|write_object
specifier|static
name|void
name|write_object
parameter_list|(
name|unsigned
name|nr
parameter_list|,
name|enum
name|object_type
name|type
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strict
condition|)
block|{
if|if
condition|(
name|write_sha1_file
argument_list|(
name|buf
argument_list|,
name|size
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
argument_list|,
name|obj_list
index|[
name|nr
index|]
operator|.
name|sha1
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"failed to write object"
argument_list|)
expr_stmt|;
name|added_object
argument_list|(
name|nr
argument_list|,
name|type
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
name|obj_list
index|[
name|nr
index|]
operator|.
name|obj
operator|=
name|NULL
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|type
operator|==
name|OBJ_BLOB
condition|)
block|{
name|struct
name|blob
modifier|*
name|blob
decl_stmt|;
if|if
condition|(
name|write_sha1_file
argument_list|(
name|buf
argument_list|,
name|size
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
argument_list|,
name|obj_list
index|[
name|nr
index|]
operator|.
name|sha1
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"failed to write object"
argument_list|)
expr_stmt|;
name|added_object
argument_list|(
name|nr
argument_list|,
name|type
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
name|blob
operator|=
name|lookup_blob
argument_list|(
name|obj_list
index|[
name|nr
index|]
operator|.
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|blob
condition|)
name|blob
operator|->
name|object
operator|.
name|flags
operator||=
name|FLAG_WRITTEN
expr_stmt|;
else|else
name|die
argument_list|(
literal|"invalid blob object"
argument_list|)
expr_stmt|;
name|obj_list
index|[
name|nr
index|]
operator|.
name|obj
operator|=
name|NULL
expr_stmt|;
block|}
else|else
block|{
name|struct
name|object
modifier|*
name|obj
decl_stmt|;
name|int
name|eaten
decl_stmt|;
name|hash_sha1_file
argument_list|(
name|buf
argument_list|,
name|size
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
argument_list|,
name|obj_list
index|[
name|nr
index|]
operator|.
name|sha1
argument_list|)
expr_stmt|;
name|added_object
argument_list|(
name|nr
argument_list|,
name|type
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|obj
operator|=
name|parse_object_buffer
argument_list|(
name|obj_list
index|[
name|nr
index|]
operator|.
name|sha1
argument_list|,
name|type
argument_list|,
name|size
argument_list|,
name|buf
argument_list|,
operator|&
name|eaten
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|obj
condition|)
name|die
argument_list|(
literal|"invalid %s"
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
argument_list|)
expr_stmt|;
name|add_object_buffer
argument_list|(
name|obj
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|obj
operator|->
name|flags
operator||=
name|FLAG_OPEN
expr_stmt|;
name|obj_list
index|[
name|nr
index|]
operator|.
name|obj
operator|=
name|obj
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|resolve_delta
specifier|static
name|void
name|resolve_delta
parameter_list|(
name|unsigned
name|nr
parameter_list|,
name|enum
name|object_type
name|type
parameter_list|,
name|void
modifier|*
name|base
parameter_list|,
name|unsigned
name|long
name|base_size
parameter_list|,
name|void
modifier|*
name|delta
parameter_list|,
name|unsigned
name|long
name|delta_size
parameter_list|)
block|{
name|void
modifier|*
name|result
decl_stmt|;
name|unsigned
name|long
name|result_size
decl_stmt|;
name|result
operator|=
name|patch_delta
argument_list|(
name|base
argument_list|,
name|base_size
argument_list|,
name|delta
argument_list|,
name|delta_size
argument_list|,
operator|&
name|result_size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|result
condition|)
name|die
argument_list|(
literal|"failed to apply delta"
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|delta
argument_list|)
expr_stmt|;
name|write_object
argument_list|(
name|nr
argument_list|,
name|type
argument_list|,
name|result
argument_list|,
name|result_size
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * We now know the contents of an object (which is nr-th in the pack);  * resolve all the deltified objects that are based on it.  */
end_comment

begin_function
DECL|function|added_object
specifier|static
name|void
name|added_object
parameter_list|(
name|unsigned
name|nr
parameter_list|,
name|enum
name|object_type
name|type
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|struct
name|delta_info
modifier|*
modifier|*
name|p
init|=
operator|&
name|delta_list
decl_stmt|;
name|struct
name|delta_info
modifier|*
name|info
decl_stmt|;
while|while
condition|(
operator|(
name|info
operator|=
operator|*
name|p
operator|)
operator|!=
name|NULL
condition|)
block|{
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|info
operator|->
name|base_sha1
argument_list|,
name|obj_list
index|[
name|nr
index|]
operator|.
name|sha1
argument_list|)
operator|||
name|info
operator|->
name|base_offset
operator|==
name|obj_list
index|[
name|nr
index|]
operator|.
name|offset
condition|)
block|{
operator|*
name|p
operator|=
name|info
operator|->
name|next
expr_stmt|;
name|p
operator|=
operator|&
name|delta_list
expr_stmt|;
name|resolve_delta
argument_list|(
name|info
operator|->
name|nr
argument_list|,
name|type
argument_list|,
name|data
argument_list|,
name|size
argument_list|,
name|info
operator|->
name|delta
argument_list|,
name|info
operator|->
name|size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|info
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|p
operator|=
operator|&
name|info
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|unpack_non_delta_entry
specifier|static
name|void
name|unpack_non_delta_entry
parameter_list|(
name|enum
name|object_type
name|type
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|unsigned
name|nr
parameter_list|)
block|{
name|void
modifier|*
name|buf
init|=
name|get_data
argument_list|(
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|dry_run
operator|&&
name|buf
condition|)
name|write_object
argument_list|(
name|nr
argument_list|,
name|type
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
else|else
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|resolve_against_held
specifier|static
name|int
name|resolve_against_held
parameter_list|(
name|unsigned
name|nr
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|base
parameter_list|,
name|void
modifier|*
name|delta_data
parameter_list|,
name|unsigned
name|long
name|delta_size
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
decl_stmt|;
name|struct
name|obj_buffer
modifier|*
name|obj_buffer
decl_stmt|;
name|obj
operator|=
name|lookup_object
argument_list|(
name|base
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|obj
condition|)
return|return
literal|0
return|;
name|obj_buffer
operator|=
name|lookup_object_buffer
argument_list|(
name|obj
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|obj_buffer
condition|)
return|return
literal|0
return|;
name|resolve_delta
argument_list|(
name|nr
argument_list|,
name|obj
operator|->
name|type
argument_list|,
name|obj_buffer
operator|->
name|buffer
argument_list|,
name|obj_buffer
operator|->
name|size
argument_list|,
name|delta_data
argument_list|,
name|delta_size
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|unpack_delta_entry
specifier|static
name|void
name|unpack_delta_entry
parameter_list|(
name|enum
name|object_type
name|type
parameter_list|,
name|unsigned
name|long
name|delta_size
parameter_list|,
name|unsigned
name|nr
parameter_list|)
block|{
name|void
modifier|*
name|delta_data
decl_stmt|,
modifier|*
name|base
decl_stmt|;
name|unsigned
name|long
name|base_size
decl_stmt|;
name|unsigned
name|char
name|base_sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|type
operator|==
name|OBJ_REF_DELTA
condition|)
block|{
name|hashcpy
argument_list|(
name|base_sha1
argument_list|,
name|fill
argument_list|(
literal|20
argument_list|)
argument_list|)
expr_stmt|;
name|use
argument_list|(
literal|20
argument_list|)
expr_stmt|;
name|delta_data
operator|=
name|get_data
argument_list|(
name|delta_size
argument_list|)
expr_stmt|;
if|if
condition|(
name|dry_run
operator|||
operator|!
name|delta_data
condition|)
block|{
name|free
argument_list|(
name|delta_data
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|has_sha1_file
argument_list|(
name|base_sha1
argument_list|)
condition|)
empty_stmt|;
comment|/* Ok we have this one */
elseif|else
if|if
condition|(
name|resolve_against_held
argument_list|(
name|nr
argument_list|,
name|base_sha1
argument_list|,
name|delta_data
argument_list|,
name|delta_size
argument_list|)
condition|)
return|return;
comment|/* we are done */
else|else
block|{
comment|/* cannot resolve yet --- queue it */
name|hashcpy
argument_list|(
name|obj_list
index|[
name|nr
index|]
operator|.
name|sha1
argument_list|,
name|null_sha1
argument_list|)
expr_stmt|;
name|add_delta_to_list
argument_list|(
name|nr
argument_list|,
name|base_sha1
argument_list|,
literal|0
argument_list|,
name|delta_data
argument_list|,
name|delta_size
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
else|else
block|{
name|unsigned
name|base_found
init|=
literal|0
decl_stmt|;
name|unsigned
name|char
modifier|*
name|pack
decl_stmt|,
name|c
decl_stmt|;
name|off_t
name|base_offset
decl_stmt|;
name|unsigned
name|lo
decl_stmt|,
name|mid
decl_stmt|,
name|hi
decl_stmt|;
name|pack
operator|=
name|fill
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|c
operator|=
operator|*
name|pack
expr_stmt|;
name|use
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|base_offset
operator|=
name|c
operator|&
literal|127
expr_stmt|;
while|while
condition|(
name|c
operator|&
literal|128
condition|)
block|{
name|base_offset
operator|+=
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|base_offset
operator|||
name|MSB
argument_list|(
name|base_offset
argument_list|,
literal|7
argument_list|)
condition|)
name|die
argument_list|(
literal|"offset value overflow for delta base object"
argument_list|)
expr_stmt|;
name|pack
operator|=
name|fill
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|c
operator|=
operator|*
name|pack
expr_stmt|;
name|use
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|base_offset
operator|=
operator|(
name|base_offset
operator|<<
literal|7
operator|)
operator|+
operator|(
name|c
operator|&
literal|127
operator|)
expr_stmt|;
block|}
name|base_offset
operator|=
name|obj_list
index|[
name|nr
index|]
operator|.
name|offset
operator|-
name|base_offset
expr_stmt|;
if|if
condition|(
name|base_offset
operator|<=
literal|0
operator|||
name|base_offset
operator|>=
name|obj_list
index|[
name|nr
index|]
operator|.
name|offset
condition|)
name|die
argument_list|(
literal|"offset value out of bound for delta base object"
argument_list|)
expr_stmt|;
name|delta_data
operator|=
name|get_data
argument_list|(
name|delta_size
argument_list|)
expr_stmt|;
if|if
condition|(
name|dry_run
operator|||
operator|!
name|delta_data
condition|)
block|{
name|free
argument_list|(
name|delta_data
argument_list|)
expr_stmt|;
return|return;
block|}
name|lo
operator|=
literal|0
expr_stmt|;
name|hi
operator|=
name|nr
expr_stmt|;
while|while
condition|(
name|lo
operator|<
name|hi
condition|)
block|{
name|mid
operator|=
operator|(
name|lo
operator|+
name|hi
operator|)
operator|/
literal|2
expr_stmt|;
if|if
condition|(
name|base_offset
operator|<
name|obj_list
index|[
name|mid
index|]
operator|.
name|offset
condition|)
block|{
name|hi
operator|=
name|mid
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|base_offset
operator|>
name|obj_list
index|[
name|mid
index|]
operator|.
name|offset
condition|)
block|{
name|lo
operator|=
name|mid
operator|+
literal|1
expr_stmt|;
block|}
else|else
block|{
name|hashcpy
argument_list|(
name|base_sha1
argument_list|,
name|obj_list
index|[
name|mid
index|]
operator|.
name|sha1
argument_list|)
expr_stmt|;
name|base_found
operator|=
operator|!
name|is_null_sha1
argument_list|(
name|base_sha1
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
operator|!
name|base_found
condition|)
block|{
comment|/* 			 * The delta base object is itself a delta that 			 * has not been resolved yet. 			 */
name|hashcpy
argument_list|(
name|obj_list
index|[
name|nr
index|]
operator|.
name|sha1
argument_list|,
name|null_sha1
argument_list|)
expr_stmt|;
name|add_delta_to_list
argument_list|(
name|nr
argument_list|,
name|null_sha1
argument_list|,
name|base_offset
argument_list|,
name|delta_data
argument_list|,
name|delta_size
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
if|if
condition|(
name|resolve_against_held
argument_list|(
name|nr
argument_list|,
name|base_sha1
argument_list|,
name|delta_data
argument_list|,
name|delta_size
argument_list|)
condition|)
return|return;
name|base
operator|=
name|read_sha1_file
argument_list|(
name|base_sha1
argument_list|,
operator|&
name|type
argument_list|,
operator|&
name|base_size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|base
condition|)
block|{
name|error
argument_list|(
literal|"failed to read delta-pack base object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|base_sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|recover
condition|)
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|has_errors
operator|=
literal|1
expr_stmt|;
return|return;
block|}
name|resolve_delta
argument_list|(
name|nr
argument_list|,
name|type
argument_list|,
name|base
argument_list|,
name|base_size
argument_list|,
name|delta_data
argument_list|,
name|delta_size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|base
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|unpack_one
specifier|static
name|void
name|unpack_one
parameter_list|(
name|unsigned
name|nr
parameter_list|)
block|{
name|unsigned
name|shift
decl_stmt|;
name|unsigned
name|char
modifier|*
name|pack
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|,
name|c
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
name|obj_list
index|[
name|nr
index|]
operator|.
name|offset
operator|=
name|consumed_bytes
expr_stmt|;
name|pack
operator|=
name|fill
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|c
operator|=
operator|*
name|pack
expr_stmt|;
name|use
argument_list|(
literal|1
argument_list|)
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
name|pack
operator|=
name|fill
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|c
operator|=
operator|*
name|pack
expr_stmt|;
name|use
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|size
operator|+=
operator|(
name|c
operator|&
literal|0x7f
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
name|unpack_non_delta_entry
argument_list|(
name|type
argument_list|,
name|size
argument_list|,
name|nr
argument_list|)
expr_stmt|;
return|return;
case|case
name|OBJ_REF_DELTA
case|:
case|case
name|OBJ_OFS_DELTA
case|:
name|unpack_delta_entry
argument_list|(
name|type
argument_list|,
name|size
argument_list|,
name|nr
argument_list|)
expr_stmt|;
return|return;
default|default:
name|error
argument_list|(
literal|"bad object type %d"
argument_list|,
name|type
argument_list|)
expr_stmt|;
name|has_errors
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|recover
condition|)
return|return;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|unpack_all
specifier|static
name|void
name|unpack_all
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|struct
name|progress
modifier|*
name|progress
init|=
name|NULL
decl_stmt|;
name|struct
name|pack_header
modifier|*
name|hdr
init|=
name|fill
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|pack_header
argument_list|)
argument_list|)
decl_stmt|;
name|nr_objects
operator|=
name|ntohl
argument_list|(
name|hdr
operator|->
name|hdr_entries
argument_list|)
expr_stmt|;
if|if
condition|(
name|ntohl
argument_list|(
name|hdr
operator|->
name|hdr_signature
argument_list|)
operator|!=
name|PACK_SIGNATURE
condition|)
name|die
argument_list|(
literal|"bad pack file"
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
literal|"unknown pack file version %"
name|PRIu32
argument_list|,
name|ntohl
argument_list|(
name|hdr
operator|->
name|hdr_version
argument_list|)
argument_list|)
expr_stmt|;
name|use
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|pack_header
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|quiet
condition|)
name|progress
operator|=
name|start_progress
argument_list|(
literal|"Unpacking objects"
argument_list|,
name|nr_objects
argument_list|)
expr_stmt|;
name|obj_list
operator|=
name|xcalloc
argument_list|(
name|nr_objects
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|obj_list
argument_list|)
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
name|unpack_one
argument_list|(
name|i
argument_list|)
expr_stmt|;
name|display_progress
argument_list|(
name|progress
argument_list|,
name|i
operator|+
literal|1
argument_list|)
expr_stmt|;
block|}
name|stop_progress
argument_list|(
operator|&
name|progress
argument_list|)
expr_stmt|;
if|if
condition|(
name|delta_list
condition|)
name|die
argument_list|(
literal|"unresolved deltas left after unpacking"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_unpack_objects
name|int
name|cmd_unpack_objects
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|read_replace_refs
operator|=
literal|0
expr_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|quiet
operator|=
operator|!
name|isatty
argument_list|(
literal|2
argument_list|)
expr_stmt|;
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
literal|"-n"
argument_list|)
condition|)
block|{
name|dry_run
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-q"
argument_list|)
condition|)
block|{
name|quiet
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-r"
argument_list|)
condition|)
block|{
name|recover
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--strict"
argument_list|)
condition|)
block|{
name|strict
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--pack_header="
argument_list|)
condition|)
block|{
name|struct
name|pack_header
modifier|*
name|hdr
decl_stmt|;
name|char
modifier|*
name|c
decl_stmt|;
name|hdr
operator|=
operator|(
expr|struct
name|pack_header
operator|*
operator|)
name|buffer
expr_stmt|;
name|hdr
operator|->
name|hdr_signature
operator|=
name|htonl
argument_list|(
name|PACK_SIGNATURE
argument_list|)
expr_stmt|;
name|hdr
operator|->
name|hdr_version
operator|=
name|htonl
argument_list|(
name|strtoul
argument_list|(
name|arg
operator|+
literal|14
argument_list|,
operator|&
name|c
argument_list|,
literal|10
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|c
operator|!=
literal|','
condition|)
name|die
argument_list|(
literal|"bad %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|hdr
operator|->
name|hdr_entries
operator|=
name|htonl
argument_list|(
name|strtoul
argument_list|(
name|c
operator|+
literal|1
argument_list|,
operator|&
name|c
argument_list|,
literal|10
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|c
condition|)
name|die
argument_list|(
literal|"bad %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|len
operator|=
sizeof|sizeof
argument_list|(
operator|*
name|hdr
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|unpack_usage
argument_list|)
expr_stmt|;
block|}
comment|/* We don't take any non-flag arguments now.. Maybe some day */
name|usage
argument_list|(
name|unpack_usage
argument_list|)
expr_stmt|;
block|}
name|git_SHA1_Init
argument_list|(
operator|&
name|ctx
argument_list|)
expr_stmt|;
name|unpack_all
argument_list|()
expr_stmt|;
name|git_SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
name|buffer
argument_list|,
name|offset
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
if|if
condition|(
name|strict
condition|)
name|write_rest
argument_list|()
expr_stmt|;
if|if
condition|(
name|hashcmp
argument_list|(
name|fill
argument_list|(
literal|20
argument_list|)
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"final sha1 did not match"
argument_list|)
expr_stmt|;
name|use
argument_list|(
literal|20
argument_list|)
expr_stmt|;
comment|/* Write the last part of the buffer to stdout */
while|while
condition|(
name|len
condition|)
block|{
name|int
name|ret
init|=
name|xwrite
argument_list|(
literal|1
argument_list|,
name|buffer
operator|+
name|offset
argument_list|,
name|len
argument_list|)
decl_stmt|;
if|if
condition|(
name|ret
operator|<=
literal|0
condition|)
break|break;
name|len
operator|-=
name|ret
expr_stmt|;
name|offset
operator|+=
name|ret
expr_stmt|;
block|}
comment|/* All done */
return|return
name|has_errors
return|;
block|}
end_function

end_unit

