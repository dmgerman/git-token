begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_decl_stmt
DECL|variable|sha1_file_directory
specifier|const
name|char
modifier|*
name|sha1_file_directory
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|active_cache
name|struct
name|cache_entry
modifier|*
modifier|*
name|active_cache
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|active_nr
DECL|variable|active_alloc
name|unsigned
name|int
name|active_nr
init|=
literal|0
decl_stmt|,
name|active_alloc
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|usage
name|void
name|usage
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|args
decl_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|err
argument_list|)
expr_stmt|;
name|vfprintf
argument_list|(
name|stderr
argument_list|,
name|err
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|hexval
specifier|static
name|unsigned
name|hexval
parameter_list|(
name|char
name|c
parameter_list|)
block|{
if|if
condition|(
name|c
operator|>=
literal|'0'
operator|&&
name|c
operator|<=
literal|'9'
condition|)
return|return
name|c
operator|-
literal|'0'
return|;
if|if
condition|(
name|c
operator|>=
literal|'a'
operator|&&
name|c
operator|<=
literal|'f'
condition|)
return|return
name|c
operator|-
literal|'a'
operator|+
literal|10
return|;
if|if
condition|(
name|c
operator|>=
literal|'A'
operator|&&
name|c
operator|<=
literal|'F'
condition|)
return|return
name|c
operator|-
literal|'A'
operator|+
literal|10
return|;
return|return
operator|~
literal|0
return|;
block|}
end_function

begin_function
DECL|function|get_sha1_hex
name|int
name|get_sha1_hex
parameter_list|(
specifier|const
name|char
modifier|*
name|hex
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
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
literal|20
condition|;
name|i
operator|++
control|)
block|{
name|unsigned
name|int
name|val
init|=
operator|(
name|hexval
argument_list|(
name|hex
index|[
literal|0
index|]
argument_list|)
operator|<<
literal|4
operator|)
operator||
name|hexval
argument_list|(
name|hex
index|[
literal|1
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
name|val
operator|&
operator|~
literal|0xff
condition|)
return|return
operator|-
literal|1
return|;
operator|*
name|sha1
operator|++
operator|=
name|val
expr_stmt|;
name|hex
operator|+=
literal|2
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|sha1_to_hex
name|char
modifier|*
name|sha1_to_hex
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
specifier|static
name|char
name|buffer
index|[
literal|50
index|]
decl_stmt|;
specifier|static
specifier|const
name|char
name|hex
index|[]
init|=
literal|"0123456789abcdef"
decl_stmt|;
name|char
modifier|*
name|buf
init|=
name|buffer
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
literal|20
condition|;
name|i
operator|++
control|)
block|{
name|unsigned
name|int
name|val
init|=
operator|*
name|sha1
operator|++
decl_stmt|;
operator|*
name|buf
operator|++
operator|=
name|hex
index|[
name|val
operator|>>
literal|4
index|]
expr_stmt|;
operator|*
name|buf
operator|++
operator|=
name|hex
index|[
name|val
operator|&
literal|0xf
index|]
expr_stmt|;
block|}
return|return
name|buffer
return|;
block|}
end_function

begin_comment
comment|/*  * NOTE! This returns a statically allocated buffer, so you have to be  * careful about using it. Do a "strdup()" if you need to save the  * filename.  */
end_comment

begin_function
DECL|function|sha1_file_name
name|char
modifier|*
name|sha1_file_name
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
specifier|static
name|char
modifier|*
name|name
decl_stmt|,
modifier|*
name|base
decl_stmt|;
if|if
condition|(
operator|!
name|base
condition|)
block|{
name|char
modifier|*
name|sha1_file_directory
init|=
name|getenv
argument_list|(
name|DB_ENVIRONMENT
argument_list|)
condition|?
else|:
name|DEFAULT_DB_ENVIRONMENT
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|sha1_file_directory
argument_list|)
decl_stmt|;
name|base
operator|=
name|malloc
argument_list|(
name|len
operator|+
literal|60
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|base
argument_list|,
name|sha1_file_directory
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|base
operator|+
name|len
argument_list|,
literal|0
argument_list|,
literal|60
argument_list|)
expr_stmt|;
name|base
index|[
name|len
index|]
operator|=
literal|'/'
expr_stmt|;
name|base
index|[
name|len
operator|+
literal|3
index|]
operator|=
literal|'/'
expr_stmt|;
name|name
operator|=
name|base
operator|+
name|len
operator|+
literal|1
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
literal|20
condition|;
name|i
operator|++
control|)
block|{
specifier|static
name|char
name|hex
index|[]
init|=
literal|"0123456789abcdef"
decl_stmt|;
name|unsigned
name|int
name|val
init|=
name|sha1
index|[
name|i
index|]
decl_stmt|;
name|char
modifier|*
name|pos
init|=
name|name
operator|+
name|i
operator|*
literal|2
operator|+
operator|(
name|i
operator|>
literal|0
operator|)
decl_stmt|;
operator|*
name|pos
operator|++
operator|=
name|hex
index|[
name|val
operator|>>
literal|4
index|]
expr_stmt|;
operator|*
name|pos
operator|=
name|hex
index|[
name|val
operator|&
literal|0xf
index|]
expr_stmt|;
block|}
return|return
name|base
return|;
block|}
end_function

begin_function
DECL|function|check_sha1_signature
name|int
name|check_sha1_signature
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|void
modifier|*
name|map
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|unsigned
name|char
name|real_sha1
index|[
literal|20
index|]
decl_stmt|;
name|SHA_CTX
name|c
decl_stmt|;
name|SHA1_Init
argument_list|(
operator|&
name|c
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|c
argument_list|,
name|map
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|SHA1_Final
argument_list|(
name|real_sha1
argument_list|,
operator|&
name|c
argument_list|)
expr_stmt|;
return|return
name|memcmp
argument_list|(
name|sha1
argument_list|,
name|real_sha1
argument_list|,
literal|20
argument_list|)
condition|?
operator|-
literal|1
else|:
literal|0
return|;
block|}
end_function

begin_function
DECL|function|map_sha1_file
name|void
modifier|*
name|map_sha1_file
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
name|long
modifier|*
name|size
parameter_list|)
block|{
name|char
modifier|*
name|filename
init|=
name|sha1_file_name
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|int
name|fd
init|=
name|open
argument_list|(
name|filename
argument_list|,
name|O_RDONLY
argument_list|)
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|void
modifier|*
name|map
decl_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
block|{
name|perror
argument_list|(
name|filename
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
if|if
condition|(
name|fstat
argument_list|(
name|fd
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
block|{
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|map
operator|=
name|mmap
argument_list|(
name|NULL
argument_list|,
name|st
operator|.
name|st_size
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
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
operator|-
literal|1
operator|==
operator|(
name|int
operator|)
operator|(
name|long
operator|)
name|map
condition|)
return|return
name|NULL
return|;
operator|*
name|size
operator|=
name|st
operator|.
name|st_size
expr_stmt|;
return|return
name|map
return|;
block|}
end_function

begin_function
DECL|function|unpack_sha1_file
name|void
modifier|*
name|unpack_sha1_file
parameter_list|(
name|void
modifier|*
name|map
parameter_list|,
name|unsigned
name|long
name|mapsize
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
name|int
name|ret
decl_stmt|,
name|bytes
decl_stmt|;
name|z_stream
name|stream
decl_stmt|;
name|char
name|buffer
index|[
literal|8192
index|]
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|;
comment|/* Get the data stream */
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
name|next_in
operator|=
name|map
expr_stmt|;
name|stream
operator|.
name|avail_in
operator|=
name|mapsize
expr_stmt|;
name|stream
operator|.
name|next_out
operator|=
name|buffer
expr_stmt|;
name|stream
operator|.
name|avail_out
operator|=
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
name|inflateInit
argument_list|(
operator|&
name|stream
argument_list|)
expr_stmt|;
name|ret
operator|=
name|inflate
argument_list|(
operator|&
name|stream
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|sscanf
argument_list|(
name|buffer
argument_list|,
literal|"%10s %lu"
argument_list|,
name|type
argument_list|,
name|size
argument_list|)
operator|!=
literal|2
condition|)
return|return
name|NULL
return|;
name|bytes
operator|=
name|strlen
argument_list|(
name|buffer
argument_list|)
operator|+
literal|1
expr_stmt|;
name|buf
operator|=
name|malloc
argument_list|(
operator|*
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
return|return
name|NULL
return|;
name|memcpy
argument_list|(
name|buf
argument_list|,
name|buffer
operator|+
name|bytes
argument_list|,
name|stream
operator|.
name|total_out
operator|-
name|bytes
argument_list|)
expr_stmt|;
name|bytes
operator|=
name|stream
operator|.
name|total_out
operator|-
name|bytes
expr_stmt|;
if|if
condition|(
name|bytes
operator|<
operator|*
name|size
operator|&&
name|ret
operator|==
name|Z_OK
condition|)
block|{
name|stream
operator|.
name|next_out
operator|=
name|buf
operator|+
name|bytes
expr_stmt|;
name|stream
operator|.
name|avail_out
operator|=
operator|*
name|size
operator|-
name|bytes
expr_stmt|;
while|while
condition|(
name|inflate
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
block|}
name|inflateEnd
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

begin_function
DECL|function|read_sha1_file
name|void
modifier|*
name|read_sha1_file
parameter_list|(
specifier|const
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
decl_stmt|,
modifier|*
name|buf
decl_stmt|;
name|map
operator|=
name|map_sha1_file
argument_list|(
name|sha1
argument_list|,
operator|&
name|mapsize
argument_list|)
expr_stmt|;
if|if
condition|(
name|map
condition|)
block|{
name|buf
operator|=
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
expr_stmt|;
name|munmap
argument_list|(
name|map
argument_list|,
name|mapsize
argument_list|)
expr_stmt|;
return|return
name|buf
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|write_sha1_file
name|int
name|write_sha1_file
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|len
parameter_list|,
name|unsigned
name|char
modifier|*
name|returnsha1
parameter_list|)
block|{
name|int
name|size
decl_stmt|;
name|char
modifier|*
name|compressed
decl_stmt|;
name|z_stream
name|stream
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|SHA_CTX
name|c
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
argument_list|)
expr_stmt|;
name|compressed
operator|=
name|malloc
argument_list|(
name|size
argument_list|)
expr_stmt|;
comment|/* Compress it */
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
name|size
operator|=
name|stream
operator|.
name|total_out
expr_stmt|;
comment|/* Sha1.. */
name|SHA1_Init
argument_list|(
operator|&
name|c
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|c
argument_list|,
name|compressed
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|SHA1_Final
argument_list|(
name|sha1
argument_list|,
operator|&
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|write_sha1_buffer
argument_list|(
name|sha1
argument_list|,
name|compressed
argument_list|,
name|size
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|returnsha1
condition|)
name|memcpy
argument_list|(
name|returnsha1
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|write_sha1_buffer
name|int
name|write_sha1_buffer
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|unsigned
name|int
name|size
parameter_list|)
block|{
name|char
modifier|*
name|filename
init|=
name|sha1_file_name
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|int
name|fd
decl_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|filename
argument_list|,
name|O_WRONLY
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
operator|(
name|errno
operator|==
name|EEXIST
operator|)
condition|?
literal|0
else|:
operator|-
literal|1
return|;
name|write
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|error
specifier|static
name|int
name|error
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"error: %s\n"
argument_list|,
name|string
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|cache_match_stat
name|int
name|cache_match_stat
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|struct
name|stat
modifier|*
name|st
parameter_list|)
block|{
name|unsigned
name|int
name|changed
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|ce
operator|->
name|mtime
operator|.
name|sec
operator|!=
operator|(
name|unsigned
name|int
operator|)
name|st
operator|->
name|st_mtim
operator|.
name|tv_sec
operator|||
name|ce
operator|->
name|mtime
operator|.
name|nsec
operator|!=
operator|(
name|unsigned
name|int
operator|)
name|st
operator|->
name|st_mtim
operator|.
name|tv_nsec
condition|)
name|changed
operator||=
name|MTIME_CHANGED
expr_stmt|;
if|if
condition|(
name|ce
operator|->
name|ctime
operator|.
name|sec
operator|!=
operator|(
name|unsigned
name|int
operator|)
name|st
operator|->
name|st_ctim
operator|.
name|tv_sec
operator|||
name|ce
operator|->
name|ctime
operator|.
name|nsec
operator|!=
operator|(
name|unsigned
name|int
operator|)
name|st
operator|->
name|st_ctim
operator|.
name|tv_nsec
condition|)
name|changed
operator||=
name|CTIME_CHANGED
expr_stmt|;
if|if
condition|(
name|ce
operator|->
name|st_uid
operator|!=
operator|(
name|unsigned
name|int
operator|)
name|st
operator|->
name|st_uid
operator|||
name|ce
operator|->
name|st_gid
operator|!=
operator|(
name|unsigned
name|int
operator|)
name|st
operator|->
name|st_gid
condition|)
name|changed
operator||=
name|OWNER_CHANGED
expr_stmt|;
if|if
condition|(
name|ce
operator|->
name|st_mode
operator|!=
operator|(
name|unsigned
name|int
operator|)
name|st
operator|->
name|st_mode
condition|)
name|changed
operator||=
name|MODE_CHANGED
expr_stmt|;
if|if
condition|(
name|ce
operator|->
name|st_dev
operator|!=
operator|(
name|unsigned
name|int
operator|)
name|st
operator|->
name|st_dev
operator|||
name|ce
operator|->
name|st_ino
operator|!=
operator|(
name|unsigned
name|int
operator|)
name|st
operator|->
name|st_ino
condition|)
name|changed
operator||=
name|INODE_CHANGED
expr_stmt|;
if|if
condition|(
name|ce
operator|->
name|st_size
operator|!=
operator|(
name|unsigned
name|int
operator|)
name|st
operator|->
name|st_size
condition|)
name|changed
operator||=
name|DATA_CHANGED
expr_stmt|;
return|return
name|changed
return|;
block|}
end_function

begin_function
DECL|function|cache_name_compare
name|int
name|cache_name_compare
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
name|int
name|len
init|=
name|len1
operator|<
name|len2
condition|?
name|len1
else|:
name|len2
decl_stmt|;
name|int
name|cmp
decl_stmt|;
name|cmp
operator|=
name|memcmp
argument_list|(
name|name1
argument_list|,
name|name2
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmp
condition|)
return|return
name|cmp
return|;
if|if
condition|(
name|len1
operator|<
name|len2
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|len1
operator|>
name|len2
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
DECL|function|cache_name_pos
name|int
name|cache_name_pos
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
name|int
name|first
decl_stmt|,
name|last
decl_stmt|;
name|first
operator|=
literal|0
expr_stmt|;
name|last
operator|=
name|active_nr
expr_stmt|;
while|while
condition|(
name|last
operator|>
name|first
condition|)
block|{
name|int
name|next
init|=
operator|(
name|last
operator|+
name|first
operator|)
operator|>>
literal|1
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|active_cache
index|[
name|next
index|]
decl_stmt|;
name|int
name|cmp
init|=
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
name|ce
operator|->
name|namelen
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|cmp
condition|)
return|return
operator|-
name|next
operator|-
literal|1
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
name|first
return|;
block|}
end_function

begin_function
DECL|function|remove_file_from_cache
name|int
name|remove_file_from_cache
parameter_list|(
name|char
modifier|*
name|path
parameter_list|)
block|{
name|int
name|pos
init|=
name|cache_name_pos
argument_list|(
name|path
argument_list|,
name|strlen
argument_list|(
name|path
argument_list|)
argument_list|)
decl_stmt|;
if|if
condition|(
name|pos
operator|<
literal|0
condition|)
block|{
name|pos
operator|=
operator|-
name|pos
operator|-
literal|1
expr_stmt|;
name|active_nr
operator|--
expr_stmt|;
if|if
condition|(
name|pos
operator|<
name|active_nr
condition|)
name|memmove
argument_list|(
name|active_cache
operator|+
name|pos
argument_list|,
name|active_cache
operator|+
name|pos
operator|+
literal|1
argument_list|,
operator|(
name|active_nr
operator|-
name|pos
operator|-
literal|1
operator|)
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|cache_entry
operator|*
argument_list|)
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|add_cache_entry
name|int
name|add_cache_entry
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|int
name|ok_to_add
parameter_list|)
block|{
name|int
name|pos
decl_stmt|;
name|pos
operator|=
name|cache_name_pos
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|ce
operator|->
name|namelen
argument_list|)
expr_stmt|;
comment|/* existing match? Just replace it */
if|if
condition|(
name|pos
operator|<
literal|0
condition|)
block|{
name|active_cache
index|[
operator|-
name|pos
operator|-
literal|1
index|]
operator|=
name|ce
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|ok_to_add
condition|)
return|return
operator|-
literal|1
return|;
comment|/* Make sure the array is big enough .. */
if|if
condition|(
name|active_nr
operator|==
name|active_alloc
condition|)
block|{
name|active_alloc
operator|=
name|alloc_nr
argument_list|(
name|active_alloc
argument_list|)
expr_stmt|;
name|active_cache
operator|=
name|realloc
argument_list|(
name|active_cache
argument_list|,
name|active_alloc
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|cache_entry
operator|*
argument_list|)
argument_list|)
expr_stmt|;
block|}
comment|/* Add it in.. */
name|active_nr
operator|++
expr_stmt|;
if|if
condition|(
name|active_nr
operator|>
name|pos
condition|)
name|memmove
argument_list|(
name|active_cache
operator|+
name|pos
operator|+
literal|1
argument_list|,
name|active_cache
operator|+
name|pos
argument_list|,
operator|(
name|active_nr
operator|-
name|pos
operator|-
literal|1
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|ce
argument_list|)
argument_list|)
expr_stmt|;
name|active_cache
index|[
name|pos
index|]
operator|=
name|ce
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|verify_hdr
specifier|static
name|int
name|verify_hdr
parameter_list|(
name|struct
name|cache_header
modifier|*
name|hdr
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|SHA_CTX
name|c
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
name|hdr
operator|->
name|signature
operator|!=
name|CACHE_SIGNATURE
condition|)
return|return
name|error
argument_list|(
literal|"bad signature"
argument_list|)
return|;
if|if
condition|(
name|hdr
operator|->
name|version
operator|!=
literal|1
condition|)
return|return
name|error
argument_list|(
literal|"bad version"
argument_list|)
return|;
name|SHA1_Init
argument_list|(
operator|&
name|c
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|c
argument_list|,
name|hdr
argument_list|,
name|offsetof
argument_list|(
expr|struct
name|cache_header
argument_list|,
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|c
argument_list|,
name|hdr
operator|+
literal|1
argument_list|,
name|size
operator|-
sizeof|sizeof
argument_list|(
operator|*
name|hdr
argument_list|)
argument_list|)
expr_stmt|;
name|SHA1_Final
argument_list|(
name|sha1
argument_list|,
operator|&
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|memcmp
argument_list|(
name|sha1
argument_list|,
name|hdr
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"bad header sha1"
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|read_cache
name|int
name|read_cache
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|fd
decl_stmt|,
name|i
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|,
name|offset
decl_stmt|;
name|void
modifier|*
name|map
decl_stmt|;
name|struct
name|cache_header
modifier|*
name|hdr
decl_stmt|;
name|errno
operator|=
name|EBUSY
expr_stmt|;
if|if
condition|(
name|active_cache
condition|)
return|return
name|error
argument_list|(
literal|"more than one cachefile"
argument_list|)
return|;
name|errno
operator|=
name|ENOENT
expr_stmt|;
name|sha1_file_directory
operator|=
name|getenv
argument_list|(
name|DB_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|sha1_file_directory
condition|)
name|sha1_file_directory
operator|=
name|DEFAULT_DB_ENVIRONMENT
expr_stmt|;
if|if
condition|(
name|access
argument_list|(
name|sha1_file_directory
argument_list|,
name|X_OK
argument_list|)
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"no access to SHA1 file directory"
argument_list|)
return|;
name|fd
operator|=
name|open
argument_list|(
literal|".dircache/index"
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
return|return
operator|(
name|errno
operator|==
name|ENOENT
operator|)
condition|?
literal|0
else|:
name|error
argument_list|(
literal|"open failed"
argument_list|)
return|;
name|size
operator|=
literal|0
expr_stmt|;
comment|// avoid gcc warning
name|map
operator|=
operator|(
name|void
operator|*
operator|)
operator|-
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|fstat
argument_list|(
name|fd
argument_list|,
operator|&
name|st
argument_list|)
condition|)
block|{
name|size
operator|=
name|st
operator|.
name|st_size
expr_stmt|;
name|errno
operator|=
name|EINVAL
expr_stmt|;
if|if
condition|(
name|size
operator|>=
sizeof|sizeof
argument_list|(
expr|struct
name|cache_header
argument_list|)
condition|)
name|map
operator|=
name|mmap
argument_list|(
name|NULL
argument_list|,
name|size
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
block|}
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
operator|-
literal|1
operator|==
operator|(
name|int
operator|)
operator|(
name|long
operator|)
name|map
condition|)
return|return
name|error
argument_list|(
literal|"mmap failed"
argument_list|)
return|;
name|hdr
operator|=
name|map
expr_stmt|;
if|if
condition|(
name|verify_hdr
argument_list|(
name|hdr
argument_list|,
name|size
argument_list|)
operator|<
literal|0
condition|)
goto|goto
name|unmap
goto|;
name|active_nr
operator|=
name|hdr
operator|->
name|entries
expr_stmt|;
name|active_alloc
operator|=
name|alloc_nr
argument_list|(
name|active_nr
argument_list|)
expr_stmt|;
name|active_cache
operator|=
name|calloc
argument_list|(
name|active_alloc
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|cache_entry
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|offset
operator|=
sizeof|sizeof
argument_list|(
operator|*
name|hdr
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
name|hdr
operator|->
name|entries
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|map
operator|+
name|offset
decl_stmt|;
name|offset
operator|=
name|offset
operator|+
name|ce_size
argument_list|(
name|ce
argument_list|)
expr_stmt|;
name|active_cache
index|[
name|i
index|]
operator|=
name|ce
expr_stmt|;
block|}
return|return
name|active_nr
return|;
name|unmap
label|:
name|munmap
argument_list|(
name|map
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|errno
operator|=
name|EINVAL
expr_stmt|;
return|return
name|error
argument_list|(
literal|"verify header failed"
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|write_cache
name|int
name|write_cache
parameter_list|(
name|int
name|newfd
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|cache
parameter_list|,
name|int
name|entries
parameter_list|)
block|{
name|SHA_CTX
name|c
decl_stmt|;
name|struct
name|cache_header
name|hdr
decl_stmt|;
name|int
name|i
decl_stmt|;
name|hdr
operator|.
name|signature
operator|=
name|CACHE_SIGNATURE
expr_stmt|;
name|hdr
operator|.
name|version
operator|=
literal|1
expr_stmt|;
name|hdr
operator|.
name|entries
operator|=
name|entries
expr_stmt|;
name|SHA1_Init
argument_list|(
operator|&
name|c
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|c
argument_list|,
operator|&
name|hdr
argument_list|,
name|offsetof
argument_list|(
expr|struct
name|cache_header
argument_list|,
name|sha1
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
name|entries
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|cache
index|[
name|i
index|]
decl_stmt|;
name|int
name|size
init|=
name|ce_size
argument_list|(
name|ce
argument_list|)
decl_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|c
argument_list|,
name|ce
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
name|SHA1_Final
argument_list|(
name|hdr
operator|.
name|sha1
argument_list|,
operator|&
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|write
argument_list|(
name|newfd
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
return|return
operator|-
literal|1
return|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|entries
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|cache
index|[
name|i
index|]
decl_stmt|;
name|int
name|size
init|=
name|ce_size
argument_list|(
name|ce
argument_list|)
decl_stmt|;
if|if
condition|(
name|write
argument_list|(
name|newfd
argument_list|,
name|ce
argument_list|,
name|size
argument_list|)
operator|!=
name|size
condition|)
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

end_unit

