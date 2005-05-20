begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Deltafication of a GIT database.  *  * (C) 2005 Nicolas Pitre<nico@cam.org>  *  * This code is free software; you can redistribute it and/or modify  * it under the terms of the GNU General Public License version 2 as  * published by the Free Software Foundation.  */
end_comment

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

begin_function
DECL|function|replace_object
specifier|static
name|int
name|replace_object
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|char
name|tmpfile
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|int
name|fd
decl_stmt|;
name|snprintf
argument_list|(
name|tmpfile
argument_list|,
sizeof|sizeof
argument_list|(
name|tmpfile
argument_list|)
argument_list|,
literal|"%s/obj_XXXXXX"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|)
expr_stmt|;
name|fd
operator|=
name|mkstemp
argument_list|(
name|tmpfile
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"%s: %s\n"
argument_list|,
name|tmpfile
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
if|if
condition|(
name|write
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
operator|!=
name|size
condition|)
block|{
name|perror
argument_list|(
literal|"unable to write file"
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|tmpfile
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|fchmod
argument_list|(
name|fd
argument_list|,
literal|0444
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|rename
argument_list|(
name|tmpfile
argument_list|,
name|sha1_file_name
argument_list|(
name|sha1
argument_list|)
argument_list|)
condition|)
block|{
name|perror
argument_list|(
literal|"unable to replace original object"
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|tmpfile
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|create_object
specifier|static
name|void
modifier|*
name|create_object
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|len
parameter_list|,
name|char
modifier|*
name|hdr
parameter_list|,
name|int
name|hdrlen
parameter_list|,
name|unsigned
name|long
modifier|*
name|retsize
parameter_list|)
block|{
name|char
modifier|*
name|compressed
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|z_stream
name|stream
decl_stmt|;
comment|/* Set it up */
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
name|deflateInit
argument_list|(
operator|&
name|stream
argument_list|,
name|Z_BEST_COMPRESSION
argument_list|)
expr_stmt|;
name|size
operator|=
name|deflateBound
argument_list|(
operator|&
name|stream
argument_list|,
name|len
operator|+
name|hdrlen
argument_list|)
expr_stmt|;
name|compressed
operator|=
name|xmalloc
argument_list|(
name|size
argument_list|)
expr_stmt|;
comment|/* Compress it */
name|stream
operator|.
name|next_out
operator|=
name|compressed
expr_stmt|;
name|stream
operator|.
name|avail_out
operator|=
name|size
expr_stmt|;
comment|/* First header.. */
name|stream
operator|.
name|next_in
operator|=
name|hdr
expr_stmt|;
name|stream
operator|.
name|avail_in
operator|=
name|hdrlen
expr_stmt|;
while|while
condition|(
name|deflate
argument_list|(
operator|&
name|stream
argument_list|,
literal|0
argument_list|)
operator|==
name|Z_OK
condition|)
comment|/* nothing */
empty_stmt|;
comment|/* Then the data itself.. */
name|stream
operator|.
name|next_in
operator|=
name|buf
expr_stmt|;
name|stream
operator|.
name|avail_in
operator|=
name|len
expr_stmt|;
while|while
condition|(
name|deflate
argument_list|(
operator|&
name|stream
argument_list|,
name|Z_FINISH
argument_list|)
operator|==
name|Z_OK
condition|)
comment|/* nothing */
empty_stmt|;
name|deflateEnd
argument_list|(
operator|&
name|stream
argument_list|)
expr_stmt|;
operator|*
name|retsize
operator|=
name|stream
operator|.
name|total_out
expr_stmt|;
return|return
name|compressed
return|;
block|}
end_function

begin_function
DECL|function|restore_original_object
specifier|static
name|int
name|restore_original_object
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|len
parameter_list|,
name|char
modifier|*
name|type
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|char
name|hdr
index|[
literal|50
index|]
decl_stmt|;
name|int
name|hdrlen
decl_stmt|,
name|ret
decl_stmt|;
name|void
modifier|*
name|compressed
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|hdrlen
operator|=
name|sprintf
argument_list|(
name|hdr
argument_list|,
literal|"%s %lu"
argument_list|,
name|type
argument_list|,
name|len
argument_list|)
operator|+
literal|1
expr_stmt|;
name|compressed
operator|=
name|create_object
argument_list|(
name|buf
argument_list|,
name|len
argument_list|,
name|hdr
argument_list|,
name|hdrlen
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
name|ret
operator|=
name|replace_object
argument_list|(
name|compressed
argument_list|,
name|size
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|compressed
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|create_delta_object
specifier|static
name|void
modifier|*
name|create_delta_object
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|len
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1_ref
parameter_list|,
name|unsigned
name|long
modifier|*
name|size
parameter_list|)
block|{
name|char
name|hdr
index|[
literal|50
index|]
decl_stmt|;
name|int
name|hdrlen
decl_stmt|;
comment|/* Generate the header + sha1 of reference for delta */
name|hdrlen
operator|=
name|sprintf
argument_list|(
name|hdr
argument_list|,
literal|"delta %lu"
argument_list|,
name|len
operator|+
literal|20
argument_list|)
operator|+
literal|1
expr_stmt|;
name|memcpy
argument_list|(
name|hdr
operator|+
name|hdrlen
argument_list|,
name|sha1_ref
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|hdrlen
operator|+=
literal|20
expr_stmt|;
return|return
name|create_object
argument_list|(
name|buf
argument_list|,
name|len
argument_list|,
name|hdr
argument_list|,
name|hdrlen
argument_list|,
name|size
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|get_object_size
specifier|static
name|unsigned
name|long
name|get_object_size
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|sha1_file_name
argument_list|(
name|sha1
argument_list|)
argument_list|,
operator|&
name|st
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s: %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|st
operator|.
name|st_size
return|;
block|}
end_function

begin_function
DECL|function|get_buffer
specifier|static
name|void
modifier|*
name|get_buffer
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|char
modifier|*
name|type
parameter_list|,
name|unsigned
name|long
modifier|*
name|size
parameter_list|)
block|{
name|unsigned
name|long
name|mapsize
decl_stmt|;
name|void
modifier|*
name|map
init|=
name|map_sha1_file
argument_list|(
name|sha1
argument_list|,
operator|&
name|mapsize
argument_list|)
decl_stmt|;
if|if
condition|(
name|map
condition|)
block|{
name|void
modifier|*
name|buffer
init|=
name|unpack_sha1_file
argument_list|(
name|map
argument_list|,
name|mapsize
argument_list|,
name|type
argument_list|,
name|size
argument_list|)
decl_stmt|;
name|munmap
argument_list|(
name|map
argument_list|,
name|mapsize
argument_list|)
expr_stmt|;
if|if
condition|(
name|buffer
condition|)
return|return
name|buffer
return|;
block|}
name|error
argument_list|(
literal|"unable to get object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|expand_delta
specifier|static
name|void
modifier|*
name|expand_delta
parameter_list|(
name|void
modifier|*
name|delta
parameter_list|,
name|unsigned
name|long
name|delta_size
parameter_list|,
name|char
modifier|*
name|type
parameter_list|,
name|unsigned
name|long
modifier|*
name|size
parameter_list|,
name|unsigned
name|int
modifier|*
name|depth
parameter_list|,
name|char
modifier|*
name|head
parameter_list|)
block|{
name|void
modifier|*
name|buf
init|=
name|NULL
decl_stmt|;
operator|*
name|depth
operator|++
expr_stmt|;
if|if
condition|(
name|delta_size
operator|<
literal|20
condition|)
block|{
name|error
argument_list|(
literal|"delta object is bad"
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|delta
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|unsigned
name|long
name|ref_size
decl_stmt|;
name|void
modifier|*
name|ref
init|=
name|get_buffer
argument_list|(
name|delta
argument_list|,
name|type
argument_list|,
operator|&
name|ref_size
argument_list|)
decl_stmt|;
if|if
condition|(
name|ref
operator|&&
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"delta"
argument_list|)
condition|)
name|ref
operator|=
name|expand_delta
argument_list|(
name|ref
argument_list|,
name|ref_size
argument_list|,
name|type
argument_list|,
operator|&
name|ref_size
argument_list|,
name|depth
argument_list|,
name|head
argument_list|)
expr_stmt|;
else|else
name|memcpy
argument_list|(
name|head
argument_list|,
name|delta
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
name|ref
condition|)
name|buf
operator|=
name|patch_delta
argument_list|(
name|ref
argument_list|,
name|ref_size
argument_list|,
name|delta
operator|+
literal|20
argument_list|,
name|delta_size
operator|-
literal|20
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|ref
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|delta
argument_list|)
expr_stmt|;
block|}
return|return
name|buf
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|mkdelta_usage
specifier|static
name|char
modifier|*
name|mkdelta_usage
init|=
literal|"mkdelta [ --max-depth=N ]<reference_sha1><target_sha1> [<next_sha1> ... ]"
decl_stmt|;
end_decl_stmt

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
name|unsigned
name|char
name|sha1_ref
index|[
literal|20
index|]
decl_stmt|,
name|sha1_trg
index|[
literal|20
index|]
decl_stmt|,
name|head_ref
index|[
literal|20
index|]
decl_stmt|,
name|head_trg
index|[
literal|20
index|]
decl_stmt|;
name|char
name|type_ref
index|[
literal|20
index|]
decl_stmt|,
name|type_trg
index|[
literal|20
index|]
decl_stmt|;
name|void
modifier|*
name|buf_ref
decl_stmt|,
modifier|*
name|buf_trg
decl_stmt|,
modifier|*
name|buf_delta
decl_stmt|;
name|unsigned
name|long
name|size_ref
decl_stmt|,
name|size_trg
decl_stmt|,
name|size_orig
decl_stmt|,
name|size_delta
decl_stmt|;
name|unsigned
name|int
name|depth_ref
decl_stmt|,
name|depth_trg
decl_stmt|,
name|depth_max
init|=
operator|-
literal|1
decl_stmt|;
name|int
name|i
decl_stmt|,
name|verbose
init|=
literal|0
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"-v"
argument_list|)
condition|)
block|{
name|verbose
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"-d"
argument_list|)
operator|&&
name|i
operator|+
literal|1
operator|<
name|argc
condition|)
block|{
name|depth_max
operator|=
name|atoi
argument_list|(
name|argv
index|[
operator|++
name|i
index|]
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--max-depth="
argument_list|,
literal|12
argument_list|)
condition|)
block|{
name|depth_max
operator|=
name|atoi
argument_list|(
name|argv
index|[
name|i
index|]
operator|+
literal|12
argument_list|)
expr_stmt|;
block|}
else|else
break|break;
block|}
if|if
condition|(
name|i
operator|+
operator|(
name|depth_max
operator|!=
literal|0
operator|)
operator|>=
name|argc
condition|)
name|usage
argument_list|(
name|mkdelta_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
name|sha1_ref
argument_list|)
condition|)
name|die
argument_list|(
literal|"bad sha1 %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|depth_ref
operator|=
literal|0
expr_stmt|;
name|buf_ref
operator|=
name|get_buffer
argument_list|(
name|sha1_ref
argument_list|,
name|type_ref
argument_list|,
operator|&
name|size_ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|buf_ref
operator|&&
operator|!
name|strcmp
argument_list|(
name|type_ref
argument_list|,
literal|"delta"
argument_list|)
condition|)
name|buf_ref
operator|=
name|expand_delta
argument_list|(
name|buf_ref
argument_list|,
name|size_ref
argument_list|,
name|type_ref
argument_list|,
operator|&
name|size_ref
argument_list|,
operator|&
name|depth_ref
argument_list|,
name|head_ref
argument_list|)
expr_stmt|;
else|else
name|memcpy
argument_list|(
name|head_ref
argument_list|,
name|sha1_ref
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf_ref
condition|)
name|die
argument_list|(
literal|"unable to obtain initial object %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|depth_ref
operator|>
name|depth_max
condition|)
block|{
if|if
condition|(
name|restore_original_object
argument_list|(
name|buf_ref
argument_list|,
name|size_ref
argument_list|,
name|type_ref
argument_list|,
name|sha1_ref
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to restore %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"undelta %s (depth was %d)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
name|depth_ref
argument_list|)
expr_stmt|;
name|depth_ref
operator|=
literal|0
expr_stmt|;
block|}
comment|/* 	 * TODO: deltafication should be tried against any early object 	 * in the object list and not only the previous object. 	 */
while|while
condition|(
operator|++
name|i
operator|<
name|argc
condition|)
block|{
if|if
condition|(
name|get_sha1
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
name|sha1_trg
argument_list|)
condition|)
name|die
argument_list|(
literal|"bad sha1 %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|depth_trg
operator|=
literal|0
expr_stmt|;
name|buf_trg
operator|=
name|get_buffer
argument_list|(
name|sha1_trg
argument_list|,
name|type_trg
argument_list|,
operator|&
name|size_trg
argument_list|)
expr_stmt|;
if|if
condition|(
name|buf_trg
operator|&&
operator|!
name|size_trg
condition|)
block|{
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"skip    %s (object is empty)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|size_orig
operator|=
name|size_trg
expr_stmt|;
if|if
condition|(
name|buf_trg
operator|&&
operator|!
name|strcmp
argument_list|(
name|type_trg
argument_list|,
literal|"delta"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|buf_trg
argument_list|,
name|sha1_ref
argument_list|,
literal|20
argument_list|)
condition|)
block|{
comment|/* delta already in place */
name|depth_ref
operator|++
expr_stmt|;
name|memcpy
argument_list|(
name|sha1_ref
argument_list|,
name|sha1_trg
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|buf_ref
operator|=
name|patch_delta
argument_list|(
name|buf_ref
argument_list|,
name|size_ref
argument_list|,
name|buf_trg
operator|+
literal|20
argument_list|,
name|size_trg
operator|-
literal|20
argument_list|,
operator|&
name|size_ref
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf_ref
condition|)
name|die
argument_list|(
literal|"unable to apply delta %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|depth_ref
operator|>
name|depth_max
condition|)
block|{
if|if
condition|(
name|restore_original_object
argument_list|(
name|buf_ref
argument_list|,
name|size_ref
argument_list|,
name|type_ref
argument_list|,
name|sha1_ref
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to restore %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"undelta %s (depth was %d)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
name|depth_ref
argument_list|)
expr_stmt|;
name|depth_ref
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"skip    %s (delta already in place)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|buf_trg
operator|=
name|expand_delta
argument_list|(
name|buf_trg
argument_list|,
name|size_trg
argument_list|,
name|type_trg
argument_list|,
operator|&
name|size_trg
argument_list|,
operator|&
name|depth_trg
argument_list|,
name|head_trg
argument_list|)
expr_stmt|;
block|}
else|else
name|memcpy
argument_list|(
name|head_trg
argument_list|,
name|sha1_trg
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf_trg
condition|)
name|die
argument_list|(
literal|"unable to read target object %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|depth_trg
operator|>
name|depth_max
condition|)
block|{
if|if
condition|(
name|restore_original_object
argument_list|(
name|buf_trg
argument_list|,
name|size_trg
argument_list|,
name|type_trg
argument_list|,
name|sha1_trg
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to restore %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"undelta %s (depth was %d)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
name|depth_trg
argument_list|)
expr_stmt|;
name|depth_trg
operator|=
literal|0
expr_stmt|;
name|size_orig
operator|=
name|size_trg
expr_stmt|;
block|}
if|if
condition|(
name|depth_max
operator|==
literal|0
condition|)
goto|goto
name|skip
goto|;
if|if
condition|(
name|strcmp
argument_list|(
name|type_ref
argument_list|,
name|type_trg
argument_list|)
condition|)
name|die
argument_list|(
literal|"type mismatch for object %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|size_ref
condition|)
block|{
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"skip    %s (initial object is empty)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
goto|goto
name|skip
goto|;
block|}
if|if
condition|(
name|depth_ref
operator|+
literal|1
operator|>
name|depth_max
condition|)
block|{
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"skip    %s (exceeding max link depth)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
goto|goto
name|skip
goto|;
block|}
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|head_ref
argument_list|,
name|sha1_trg
argument_list|,
literal|20
argument_list|)
condition|)
block|{
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"skip    %s (would create a loop)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
goto|goto
name|skip
goto|;
block|}
name|buf_delta
operator|=
name|diff_delta
argument_list|(
name|buf_ref
argument_list|,
name|size_ref
argument_list|,
name|buf_trg
argument_list|,
name|size_trg
argument_list|,
operator|&
name|size_delta
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf_delta
condition|)
name|die
argument_list|(
literal|"out of memory"
argument_list|)
expr_stmt|;
comment|/* no need to even try to compress if original 		   uncompressed is already smaller */
if|if
condition|(
name|size_delta
operator|+
literal|20
operator|<
name|size_orig
condition|)
block|{
name|void
modifier|*
name|buf_obj
decl_stmt|;
name|unsigned
name|long
name|size_obj
decl_stmt|;
name|buf_obj
operator|=
name|create_delta_object
argument_list|(
name|buf_delta
argument_list|,
name|size_delta
argument_list|,
name|sha1_ref
argument_list|,
operator|&
name|size_obj
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf_delta
argument_list|)
expr_stmt|;
name|size_orig
operator|=
name|get_object_size
argument_list|(
name|sha1_trg
argument_list|)
expr_stmt|;
if|if
condition|(
name|size_obj
operator|>=
name|size_orig
condition|)
block|{
name|free
argument_list|(
name|buf_obj
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"skip    %s (original is smaller)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
goto|goto
name|skip
goto|;
block|}
if|if
condition|(
name|replace_object
argument_list|(
name|buf_obj
argument_list|,
name|size_obj
argument_list|,
name|sha1_trg
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to write delta for %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf_obj
argument_list|)
expr_stmt|;
name|depth_ref
operator|++
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"delta   %s (size=%ld.%02ld%%, depth=%d)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
name|size_obj
operator|*
literal|100
operator|/
name|size_orig
argument_list|,
operator|(
name|size_obj
operator|*
literal|10000
operator|/
name|size_orig
operator|)
operator|%
literal|100
argument_list|,
name|depth_ref
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|free
argument_list|(
name|buf_delta
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"skip    %s (original is smaller)\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|skip
label|:
name|depth_ref
operator|=
name|depth_trg
expr_stmt|;
name|memcpy
argument_list|(
name|head_ref
argument_list|,
name|head_trg
argument_list|,
literal|20
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|buf_ref
argument_list|)
expr_stmt|;
name|buf_ref
operator|=
name|buf_trg
expr_stmt|;
name|size_ref
operator|=
name|size_trg
expr_stmt|;
name|memcpy
argument_list|(
name|sha1_ref
argument_list|,
name|sha1_trg
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

end_unit

