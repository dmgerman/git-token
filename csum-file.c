begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * csum-file.c  *  * Copyright (C) 2005 Linus Torvalds  *  * Simple file write infrastructure for writing SHA1-summed  * files. Useful when you write a file that you want to be  * able to verify hasn't been messed with afterwards.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"progress.h"
end_include

begin_include
include|#
directive|include
file|"csum-file.h"
end_include

begin_function
DECL|function|flush
specifier|static
name|void
name|flush
parameter_list|(
name|struct
name|sha1file
modifier|*
name|f
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|unsigned
name|int
name|count
parameter_list|)
block|{
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|ret
init|=
name|xwrite
argument_list|(
name|f
operator|->
name|fd
argument_list|,
name|buf
argument_list|,
name|count
argument_list|)
decl_stmt|;
if|if
condition|(
name|ret
operator|>
literal|0
condition|)
block|{
name|f
operator|->
name|total
operator|+=
name|ret
expr_stmt|;
name|display_throughput
argument_list|(
name|f
operator|->
name|tp
argument_list|,
name|f
operator|->
name|total
argument_list|)
expr_stmt|;
name|buf
operator|=
operator|(
name|char
operator|*
operator|)
name|buf
operator|+
name|ret
expr_stmt|;
name|count
operator|-=
name|ret
expr_stmt|;
if|if
condition|(
name|count
condition|)
continue|continue;
return|return;
block|}
if|if
condition|(
operator|!
name|ret
condition|)
name|die
argument_list|(
literal|"sha1 file '%s' write error. Out of diskspace"
argument_list|,
name|f
operator|->
name|name
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"sha1 file '%s' write error (%s)"
argument_list|,
name|f
operator|->
name|name
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|sha1flush
name|void
name|sha1flush
parameter_list|(
name|struct
name|sha1file
modifier|*
name|f
parameter_list|)
block|{
name|unsigned
name|offset
init|=
name|f
operator|->
name|offset
decl_stmt|;
if|if
condition|(
name|offset
condition|)
block|{
name|git_SHA1_Update
argument_list|(
operator|&
name|f
operator|->
name|ctx
argument_list|,
name|f
operator|->
name|buffer
argument_list|,
name|offset
argument_list|)
expr_stmt|;
name|flush
argument_list|(
name|f
argument_list|,
name|f
operator|->
name|buffer
argument_list|,
name|offset
argument_list|)
expr_stmt|;
name|f
operator|->
name|offset
operator|=
literal|0
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|sha1close
name|int
name|sha1close
parameter_list|(
name|struct
name|sha1file
modifier|*
name|f
parameter_list|,
name|unsigned
name|char
modifier|*
name|result
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|sha1flush
argument_list|(
name|f
argument_list|)
expr_stmt|;
name|git_SHA1_Final
argument_list|(
name|f
operator|->
name|buffer
argument_list|,
operator|&
name|f
operator|->
name|ctx
argument_list|)
expr_stmt|;
if|if
condition|(
name|result
condition|)
name|hashcpy
argument_list|(
name|result
argument_list|,
name|f
operator|->
name|buffer
argument_list|)
expr_stmt|;
if|if
condition|(
name|flags
operator|&
operator|(
name|CSUM_CLOSE
operator||
name|CSUM_FSYNC
operator|)
condition|)
block|{
comment|/* write checksum and close fd */
name|flush
argument_list|(
name|f
argument_list|,
name|f
operator|->
name|buffer
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|CSUM_FSYNC
condition|)
name|fsync_or_die
argument_list|(
name|f
operator|->
name|fd
argument_list|,
name|f
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|close
argument_list|(
name|f
operator|->
name|fd
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s: sha1 file error on close (%s)"
argument_list|,
name|f
operator|->
name|name
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|fd
operator|=
literal|0
expr_stmt|;
block|}
else|else
name|fd
operator|=
name|f
operator|->
name|fd
expr_stmt|;
name|free
argument_list|(
name|f
argument_list|)
expr_stmt|;
return|return
name|fd
return|;
block|}
end_function

begin_function
DECL|function|sha1write
name|int
name|sha1write
parameter_list|(
name|struct
name|sha1file
modifier|*
name|f
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|unsigned
name|int
name|count
parameter_list|)
block|{
while|while
condition|(
name|count
condition|)
block|{
name|unsigned
name|offset
init|=
name|f
operator|->
name|offset
decl_stmt|;
name|unsigned
name|left
init|=
sizeof|sizeof
argument_list|(
name|f
operator|->
name|buffer
argument_list|)
operator|-
name|offset
decl_stmt|;
name|unsigned
name|nr
init|=
name|count
operator|>
name|left
condition|?
name|left
else|:
name|count
decl_stmt|;
name|void
modifier|*
name|data
decl_stmt|;
if|if
condition|(
name|f
operator|->
name|do_crc
condition|)
name|f
operator|->
name|crc32
operator|=
name|crc32
argument_list|(
name|f
operator|->
name|crc32
argument_list|,
name|buf
argument_list|,
name|nr
argument_list|)
expr_stmt|;
if|if
condition|(
name|nr
operator|==
sizeof|sizeof
argument_list|(
name|f
operator|->
name|buffer
argument_list|)
condition|)
block|{
comment|/* process full buffer directly without copy */
name|data
operator|=
name|buf
expr_stmt|;
block|}
else|else
block|{
name|memcpy
argument_list|(
name|f
operator|->
name|buffer
operator|+
name|offset
argument_list|,
name|buf
argument_list|,
name|nr
argument_list|)
expr_stmt|;
name|data
operator|=
name|f
operator|->
name|buffer
expr_stmt|;
block|}
name|count
operator|-=
name|nr
expr_stmt|;
name|offset
operator|+=
name|nr
expr_stmt|;
name|buf
operator|=
operator|(
name|char
operator|*
operator|)
name|buf
operator|+
name|nr
expr_stmt|;
name|left
operator|-=
name|nr
expr_stmt|;
if|if
condition|(
operator|!
name|left
condition|)
block|{
name|git_SHA1_Update
argument_list|(
operator|&
name|f
operator|->
name|ctx
argument_list|,
name|data
argument_list|,
name|offset
argument_list|)
expr_stmt|;
name|flush
argument_list|(
name|f
argument_list|,
name|data
argument_list|,
name|offset
argument_list|)
expr_stmt|;
name|offset
operator|=
literal|0
expr_stmt|;
block|}
name|f
operator|->
name|offset
operator|=
name|offset
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|sha1fd
name|struct
name|sha1file
modifier|*
name|sha1fd
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
return|return
name|sha1fd_throughput
argument_list|(
name|fd
argument_list|,
name|name
argument_list|,
name|NULL
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|sha1fd_throughput
name|struct
name|sha1file
modifier|*
name|sha1fd_throughput
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|struct
name|progress
modifier|*
name|tp
parameter_list|)
block|{
name|struct
name|sha1file
modifier|*
name|f
init|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|f
argument_list|)
argument_list|)
decl_stmt|;
name|f
operator|->
name|fd
operator|=
name|fd
expr_stmt|;
name|f
operator|->
name|offset
operator|=
literal|0
expr_stmt|;
name|f
operator|->
name|total
operator|=
literal|0
expr_stmt|;
name|f
operator|->
name|tp
operator|=
name|tp
expr_stmt|;
name|f
operator|->
name|name
operator|=
name|name
expr_stmt|;
name|f
operator|->
name|do_crc
operator|=
literal|0
expr_stmt|;
name|git_SHA1_Init
argument_list|(
operator|&
name|f
operator|->
name|ctx
argument_list|)
expr_stmt|;
return|return
name|f
return|;
block|}
end_function

begin_function
DECL|function|crc32_begin
name|void
name|crc32_begin
parameter_list|(
name|struct
name|sha1file
modifier|*
name|f
parameter_list|)
block|{
name|f
operator|->
name|crc32
operator|=
name|crc32
argument_list|(
literal|0
argument_list|,
name|Z_NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|f
operator|->
name|do_crc
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|crc32_end
name|uint32_t
name|crc32_end
parameter_list|(
name|struct
name|sha1file
modifier|*
name|f
parameter_list|)
block|{
name|f
operator|->
name|do_crc
operator|=
literal|0
expr_stmt|;
return|return
name|f
operator|->
name|crc32
return|;
block|}
end_function

end_unit

