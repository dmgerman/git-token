begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Various trivial helper wrappers around standard functions  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_function
DECL|function|xstrdup
name|char
modifier|*
name|xstrdup
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|)
block|{
name|char
modifier|*
name|ret
init|=
name|strdup
argument_list|(
name|str
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
block|{
name|release_pack_memory
argument_list|(
name|strlen
argument_list|(
name|str
argument_list|)
operator|+
literal|1
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
name|ret
operator|=
name|strdup
argument_list|(
name|str
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
name|die
argument_list|(
literal|"Out of memory, strdup failed"
argument_list|)
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|xmalloc
name|void
modifier|*
name|xmalloc
parameter_list|(
name|size_t
name|size
parameter_list|)
block|{
name|void
modifier|*
name|ret
init|=
name|malloc
argument_list|(
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
operator|!
name|size
condition|)
name|ret
operator|=
name|malloc
argument_list|(
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
block|{
name|release_pack_memory
argument_list|(
name|size
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
name|ret
operator|=
name|malloc
argument_list|(
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
operator|!
name|size
condition|)
name|ret
operator|=
name|malloc
argument_list|(
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
name|die
argument_list|(
literal|"Out of memory, malloc failed"
argument_list|)
expr_stmt|;
block|}
ifdef|#
directive|ifdef
name|XMALLOC_POISON
name|memset
argument_list|(
name|ret
argument_list|,
literal|0xA5
argument_list|,
name|size
argument_list|)
expr_stmt|;
endif|#
directive|endif
return|return
name|ret
return|;
block|}
end_function

begin_comment
comment|/*  * xmemdupz() allocates (len + 1) bytes of memory, duplicates "len" bytes of  * "data" to the allocated memory, zero terminates the allocated memory,  * and returns a pointer to the allocated memory. If the allocation fails,  * the program dies.  */
end_comment

begin_function
DECL|function|xmemdupz
name|void
modifier|*
name|xmemdupz
parameter_list|(
specifier|const
name|void
modifier|*
name|data
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|char
modifier|*
name|p
init|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|1
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|p
argument_list|,
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|p
index|[
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
return|return
name|p
return|;
block|}
end_function

begin_function
DECL|function|xstrndup
name|char
modifier|*
name|xstrndup
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|char
modifier|*
name|p
init|=
name|memchr
argument_list|(
name|str
argument_list|,
literal|'\0'
argument_list|,
name|len
argument_list|)
decl_stmt|;
return|return
name|xmemdupz
argument_list|(
name|str
argument_list|,
name|p
condition|?
name|p
operator|-
name|str
else|:
name|len
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|xrealloc
name|void
modifier|*
name|xrealloc
parameter_list|(
name|void
modifier|*
name|ptr
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|void
modifier|*
name|ret
init|=
name|realloc
argument_list|(
name|ptr
argument_list|,
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
operator|!
name|size
condition|)
name|ret
operator|=
name|realloc
argument_list|(
name|ptr
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
block|{
name|release_pack_memory
argument_list|(
name|size
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
name|ret
operator|=
name|realloc
argument_list|(
name|ptr
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
operator|!
name|size
condition|)
name|ret
operator|=
name|realloc
argument_list|(
name|ptr
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
name|die
argument_list|(
literal|"Out of memory, realloc failed"
argument_list|)
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|xcalloc
name|void
modifier|*
name|xcalloc
parameter_list|(
name|size_t
name|nmemb
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|void
modifier|*
name|ret
init|=
name|calloc
argument_list|(
name|nmemb
argument_list|,
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
operator|(
operator|!
name|nmemb
operator|||
operator|!
name|size
operator|)
condition|)
name|ret
operator|=
name|calloc
argument_list|(
literal|1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
block|{
name|release_pack_memory
argument_list|(
name|nmemb
operator|*
name|size
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
name|ret
operator|=
name|calloc
argument_list|(
name|nmemb
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
operator|(
operator|!
name|nmemb
operator|||
operator|!
name|size
operator|)
condition|)
name|ret
operator|=
name|calloc
argument_list|(
literal|1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
name|die
argument_list|(
literal|"Out of memory, calloc failed"
argument_list|)
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|xmmap
name|void
modifier|*
name|xmmap
parameter_list|(
name|void
modifier|*
name|start
parameter_list|,
name|size_t
name|length
parameter_list|,
name|int
name|prot
parameter_list|,
name|int
name|flags
parameter_list|,
name|int
name|fd
parameter_list|,
name|off_t
name|offset
parameter_list|)
block|{
name|void
modifier|*
name|ret
init|=
name|mmap
argument_list|(
name|start
argument_list|,
name|length
argument_list|,
name|prot
argument_list|,
name|flags
argument_list|,
name|fd
argument_list|,
name|offset
argument_list|)
decl_stmt|;
if|if
condition|(
name|ret
operator|==
name|MAP_FAILED
condition|)
block|{
if|if
condition|(
operator|!
name|length
condition|)
return|return
name|NULL
return|;
name|release_pack_memory
argument_list|(
name|length
argument_list|,
name|fd
argument_list|)
expr_stmt|;
name|ret
operator|=
name|mmap
argument_list|(
name|start
argument_list|,
name|length
argument_list|,
name|prot
argument_list|,
name|flags
argument_list|,
name|fd
argument_list|,
name|offset
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|==
name|MAP_FAILED
condition|)
name|die_errno
argument_list|(
literal|"Out of memory? mmap failed"
argument_list|)
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

begin_comment
comment|/*  * xread() is the same a read(), but it automatically restarts read()  * operations with a recoverable error (EAGAIN and EINTR). xread()  * DOES NOT GUARANTEE that "len" bytes is read even if the data is available.  */
end_comment

begin_function
DECL|function|xread
name|ssize_t
name|xread
parameter_list|(
name|int
name|fd
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|ssize_t
name|nr
decl_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|nr
operator|=
name|read
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|nr
operator|<
literal|0
operator|)
operator|&&
operator|(
name|errno
operator|==
name|EAGAIN
operator|||
name|errno
operator|==
name|EINTR
operator|)
condition|)
continue|continue;
return|return
name|nr
return|;
block|}
block|}
end_function

begin_comment
comment|/*  * xwrite() is the same a write(), but it automatically restarts write()  * operations with a recoverable error (EAGAIN and EINTR). xwrite() DOES NOT  * GUARANTEE that "len" bytes is written even if the operation is successful.  */
end_comment

begin_function
DECL|function|xwrite
name|ssize_t
name|xwrite
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|ssize_t
name|nr
decl_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|nr
operator|=
name|write
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|nr
operator|<
literal|0
operator|)
operator|&&
operator|(
name|errno
operator|==
name|EAGAIN
operator|||
name|errno
operator|==
name|EINTR
operator|)
condition|)
continue|continue;
return|return
name|nr
return|;
block|}
block|}
end_function

begin_function
DECL|function|read_in_full
name|ssize_t
name|read_in_full
parameter_list|(
name|int
name|fd
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|count
parameter_list|)
block|{
name|char
modifier|*
name|p
init|=
name|buf
decl_stmt|;
name|ssize_t
name|total
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|count
operator|>
literal|0
condition|)
block|{
name|ssize_t
name|loaded
init|=
name|xread
argument_list|(
name|fd
argument_list|,
name|p
argument_list|,
name|count
argument_list|)
decl_stmt|;
if|if
condition|(
name|loaded
operator|<=
literal|0
condition|)
return|return
name|total
condition|?
name|total
else|:
name|loaded
return|;
name|count
operator|-=
name|loaded
expr_stmt|;
name|p
operator|+=
name|loaded
expr_stmt|;
name|total
operator|+=
name|loaded
expr_stmt|;
block|}
return|return
name|total
return|;
block|}
end_function

begin_function
DECL|function|write_in_full
name|ssize_t
name|write_in_full
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|count
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|p
init|=
name|buf
decl_stmt|;
name|ssize_t
name|total
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|count
operator|>
literal|0
condition|)
block|{
name|ssize_t
name|written
init|=
name|xwrite
argument_list|(
name|fd
argument_list|,
name|p
argument_list|,
name|count
argument_list|)
decl_stmt|;
if|if
condition|(
name|written
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|written
condition|)
block|{
name|errno
operator|=
name|ENOSPC
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|count
operator|-=
name|written
expr_stmt|;
name|p
operator|+=
name|written
expr_stmt|;
name|total
operator|+=
name|written
expr_stmt|;
block|}
return|return
name|total
return|;
block|}
end_function

begin_function
DECL|function|xdup
name|int
name|xdup
parameter_list|(
name|int
name|fd
parameter_list|)
block|{
name|int
name|ret
init|=
name|dup
argument_list|(
name|fd
argument_list|)
decl_stmt|;
if|if
condition|(
name|ret
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"dup failed"
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|xfdopen
name|FILE
modifier|*
name|xfdopen
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|char
modifier|*
name|mode
parameter_list|)
block|{
name|FILE
modifier|*
name|stream
init|=
name|fdopen
argument_list|(
name|fd
argument_list|,
name|mode
argument_list|)
decl_stmt|;
if|if
condition|(
name|stream
operator|==
name|NULL
condition|)
name|die_errno
argument_list|(
literal|"Out of memory? fdopen failed"
argument_list|)
expr_stmt|;
return|return
name|stream
return|;
block|}
end_function

begin_function
DECL|function|xmkstemp
name|int
name|xmkstemp
parameter_list|(
name|char
modifier|*
name|template
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|fd
operator|=
name|mkstemp
argument_list|(
name|template
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"Unable to create temporary file"
argument_list|)
expr_stmt|;
return|return
name|fd
return|;
block|}
end_function

begin_comment
comment|/*  * zlib wrappers to make sure we don't silently miss errors  * at init time.  */
end_comment

begin_function
DECL|function|git_inflate_init
name|void
name|git_inflate_init
parameter_list|(
name|z_streamp
name|strm
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|err
decl_stmt|;
switch|switch
condition|(
name|inflateInit
argument_list|(
name|strm
argument_list|)
condition|)
block|{
case|case
name|Z_OK
case|:
return|return;
case|case
name|Z_MEM_ERROR
case|:
name|err
operator|=
literal|"out of memory"
expr_stmt|;
break|break;
case|case
name|Z_VERSION_ERROR
case|:
name|err
operator|=
literal|"wrong version"
expr_stmt|;
break|break;
default|default:
name|err
operator|=
literal|"error"
expr_stmt|;
block|}
name|die
argument_list|(
literal|"inflateInit: %s (%s)"
argument_list|,
name|err
argument_list|,
name|strm
operator|->
name|msg
condition|?
name|strm
operator|->
name|msg
else|:
literal|"no message"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_inflate_end
name|void
name|git_inflate_end
parameter_list|(
name|z_streamp
name|strm
parameter_list|)
block|{
if|if
condition|(
name|inflateEnd
argument_list|(
name|strm
argument_list|)
operator|!=
name|Z_OK
condition|)
name|error
argument_list|(
literal|"inflateEnd: %s"
argument_list|,
name|strm
operator|->
name|msg
condition|?
name|strm
operator|->
name|msg
else|:
literal|"failed"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_inflate
name|int
name|git_inflate
parameter_list|(
name|z_streamp
name|strm
parameter_list|,
name|int
name|flush
parameter_list|)
block|{
name|int
name|ret
init|=
name|inflate
argument_list|(
name|strm
argument_list|,
name|flush
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|err
decl_stmt|;
switch|switch
condition|(
name|ret
condition|)
block|{
comment|/* Out of memory is fatal. */
case|case
name|Z_MEM_ERROR
case|:
name|die
argument_list|(
literal|"inflate: out of memory"
argument_list|)
expr_stmt|;
comment|/* Data corruption errors: we may want to recover from them (fsck) */
case|case
name|Z_NEED_DICT
case|:
name|err
operator|=
literal|"needs dictionary"
expr_stmt|;
break|break;
case|case
name|Z_DATA_ERROR
case|:
name|err
operator|=
literal|"data stream error"
expr_stmt|;
break|break;
case|case
name|Z_STREAM_ERROR
case|:
name|err
operator|=
literal|"stream consistency error"
expr_stmt|;
break|break;
default|default:
name|err
operator|=
literal|"unknown error"
expr_stmt|;
break|break;
comment|/* Z_BUF_ERROR: normal, needs more space in the output buffer */
case|case
name|Z_BUF_ERROR
case|:
case|case
name|Z_OK
case|:
case|case
name|Z_STREAM_END
case|:
return|return
name|ret
return|;
block|}
name|error
argument_list|(
literal|"inflate: %s (%s)"
argument_list|,
name|err
argument_list|,
name|strm
operator|->
name|msg
condition|?
name|strm
operator|->
name|msg
else|:
literal|"no message"
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|odb_mkstemp
name|int
name|odb_mkstemp
parameter_list|(
name|char
modifier|*
name|template
parameter_list|,
name|size_t
name|limit
parameter_list|,
specifier|const
name|char
modifier|*
name|pattern
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|snprintf
argument_list|(
name|template
argument_list|,
name|limit
argument_list|,
literal|"%s/%s"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|,
name|pattern
argument_list|)
expr_stmt|;
name|fd
operator|=
name|mkstemp
argument_list|(
name|template
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|fd
condition|)
return|return
name|fd
return|;
comment|/* slow path */
comment|/* some mkstemp implementations erase template on failure */
name|snprintf
argument_list|(
name|template
argument_list|,
name|limit
argument_list|,
literal|"%s/%s"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|,
name|pattern
argument_list|)
expr_stmt|;
name|safe_create_leading_directories
argument_list|(
name|template
argument_list|)
expr_stmt|;
return|return
name|xmkstemp
argument_list|(
name|template
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|odb_pack_keep
name|int
name|odb_pack_keep
parameter_list|(
name|char
modifier|*
name|name
parameter_list|,
name|size_t
name|namesz
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|snprintf
argument_list|(
name|name
argument_list|,
name|namesz
argument_list|,
literal|"%s/pack/pack-%s.keep"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|name
argument_list|,
name|O_RDWR
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0600
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|fd
condition|)
return|return
name|fd
return|;
comment|/* slow path */
name|safe_create_leading_directories
argument_list|(
name|name
argument_list|)
expr_stmt|;
return|return
name|open
argument_list|(
name|name
argument_list|,
name|O_RDWR
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0600
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|unlink_or_warn
name|int
name|unlink_or_warn
parameter_list|(
specifier|const
name|char
modifier|*
name|file
parameter_list|)
block|{
name|int
name|rc
init|=
name|unlink
argument_list|(
name|file
argument_list|)
decl_stmt|;
if|if
condition|(
name|rc
operator|<
literal|0
condition|)
block|{
name|int
name|err
init|=
name|errno
decl_stmt|;
if|if
condition|(
name|ENOENT
operator|!=
name|err
condition|)
block|{
name|warning
argument_list|(
literal|"unable to unlink %s: %s"
argument_list|,
name|file
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|errno
operator|=
name|err
expr_stmt|;
block|}
block|}
return|return
name|rc
return|;
block|}
end_function

end_unit

