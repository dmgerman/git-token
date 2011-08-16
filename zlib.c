begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * zlib wrappers to make sure we don't silently miss errors  * at init time.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_function
DECL|function|zerr_to_string
specifier|static
specifier|const
name|char
modifier|*
name|zerr_to_string
parameter_list|(
name|int
name|status
parameter_list|)
block|{
switch|switch
condition|(
name|status
condition|)
block|{
case|case
name|Z_MEM_ERROR
case|:
return|return
literal|"out of memory"
return|;
case|case
name|Z_VERSION_ERROR
case|:
return|return
literal|"wrong version"
return|;
case|case
name|Z_NEED_DICT
case|:
return|return
literal|"needs dictionary"
return|;
case|case
name|Z_DATA_ERROR
case|:
return|return
literal|"data stream error"
return|;
case|case
name|Z_STREAM_ERROR
case|:
return|return
literal|"stream consistency error"
return|;
default|default:
return|return
literal|"unknown error"
return|;
block|}
block|}
end_function

begin_comment
comment|/*  * avail_in and avail_out in zlib are counted in uInt, which typically  * limits the size of the buffer we can use to 4GB when interacting  * with zlib in a single call to inflate/deflate.  */
end_comment

begin_comment
comment|/* #define ZLIB_BUF_MAX ((uInt)-1) */
end_comment

begin_define
DECL|macro|ZLIB_BUF_MAX
define|#
directive|define
name|ZLIB_BUF_MAX
value|((uInt) 1024 * 1024 * 1024)
end_define

begin_comment
DECL|macro|ZLIB_BUF_MAX
comment|/* 1GB */
end_comment

begin_function
DECL|function|zlib_buf_cap
specifier|static
specifier|inline
name|uInt
name|zlib_buf_cap
parameter_list|(
name|unsigned
name|long
name|len
parameter_list|)
block|{
return|return
operator|(
name|ZLIB_BUF_MAX
operator|<
name|len
operator|)
condition|?
name|ZLIB_BUF_MAX
else|:
name|len
return|;
block|}
end_function

begin_function
DECL|function|zlib_pre_call
specifier|static
name|void
name|zlib_pre_call
parameter_list|(
name|git_zstream
modifier|*
name|s
parameter_list|)
block|{
name|s
operator|->
name|z
operator|.
name|next_in
operator|=
name|s
operator|->
name|next_in
expr_stmt|;
name|s
operator|->
name|z
operator|.
name|next_out
operator|=
name|s
operator|->
name|next_out
expr_stmt|;
name|s
operator|->
name|z
operator|.
name|total_in
operator|=
name|s
operator|->
name|total_in
expr_stmt|;
name|s
operator|->
name|z
operator|.
name|total_out
operator|=
name|s
operator|->
name|total_out
expr_stmt|;
name|s
operator|->
name|z
operator|.
name|avail_in
operator|=
name|zlib_buf_cap
argument_list|(
name|s
operator|->
name|avail_in
argument_list|)
expr_stmt|;
name|s
operator|->
name|z
operator|.
name|avail_out
operator|=
name|zlib_buf_cap
argument_list|(
name|s
operator|->
name|avail_out
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|zlib_post_call
specifier|static
name|void
name|zlib_post_call
parameter_list|(
name|git_zstream
modifier|*
name|s
parameter_list|)
block|{
name|unsigned
name|long
name|bytes_consumed
decl_stmt|;
name|unsigned
name|long
name|bytes_produced
decl_stmt|;
name|bytes_consumed
operator|=
name|s
operator|->
name|z
operator|.
name|next_in
operator|-
name|s
operator|->
name|next_in
expr_stmt|;
name|bytes_produced
operator|=
name|s
operator|->
name|z
operator|.
name|next_out
operator|-
name|s
operator|->
name|next_out
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|z
operator|.
name|total_out
operator|!=
name|s
operator|->
name|total_out
operator|+
name|bytes_produced
condition|)
name|die
argument_list|(
literal|"BUG: total_out mismatch"
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|z
operator|.
name|total_in
operator|!=
name|s
operator|->
name|total_in
operator|+
name|bytes_consumed
condition|)
name|die
argument_list|(
literal|"BUG: total_in mismatch"
argument_list|)
expr_stmt|;
name|s
operator|->
name|total_out
operator|=
name|s
operator|->
name|z
operator|.
name|total_out
expr_stmt|;
name|s
operator|->
name|total_in
operator|=
name|s
operator|->
name|z
operator|.
name|total_in
expr_stmt|;
name|s
operator|->
name|next_in
operator|=
name|s
operator|->
name|z
operator|.
name|next_in
expr_stmt|;
name|s
operator|->
name|next_out
operator|=
name|s
operator|->
name|z
operator|.
name|next_out
expr_stmt|;
name|s
operator|->
name|avail_in
operator|-=
name|bytes_consumed
expr_stmt|;
name|s
operator|->
name|avail_out
operator|-=
name|bytes_produced
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_inflate_init
name|void
name|git_inflate_init
parameter_list|(
name|git_zstream
modifier|*
name|strm
parameter_list|)
block|{
name|int
name|status
decl_stmt|;
name|zlib_pre_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
name|status
operator|=
name|inflateInit
argument_list|(
operator|&
name|strm
operator|->
name|z
argument_list|)
expr_stmt|;
name|zlib_post_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
operator|==
name|Z_OK
condition|)
return|return;
name|die
argument_list|(
literal|"inflateInit: %s (%s)"
argument_list|,
name|zerr_to_string
argument_list|(
name|status
argument_list|)
argument_list|,
name|strm
operator|->
name|z
operator|.
name|msg
condition|?
name|strm
operator|->
name|z
operator|.
name|msg
else|:
literal|"no message"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_inflate_init_gzip_only
name|void
name|git_inflate_init_gzip_only
parameter_list|(
name|git_zstream
modifier|*
name|strm
parameter_list|)
block|{
comment|/* 	 * Use default 15 bits, +16 is to accept only gzip and to 	 * yield Z_DATA_ERROR when fed zlib format. 	 */
specifier|const
name|int
name|windowBits
init|=
literal|15
operator|+
literal|16
decl_stmt|;
name|int
name|status
decl_stmt|;
name|zlib_pre_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
name|status
operator|=
name|inflateInit2
argument_list|(
operator|&
name|strm
operator|->
name|z
argument_list|,
name|windowBits
argument_list|)
expr_stmt|;
name|zlib_post_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
operator|==
name|Z_OK
condition|)
return|return;
name|die
argument_list|(
literal|"inflateInit2: %s (%s)"
argument_list|,
name|zerr_to_string
argument_list|(
name|status
argument_list|)
argument_list|,
name|strm
operator|->
name|z
operator|.
name|msg
condition|?
name|strm
operator|->
name|z
operator|.
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
name|git_zstream
modifier|*
name|strm
parameter_list|)
block|{
name|int
name|status
decl_stmt|;
name|zlib_pre_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
name|status
operator|=
name|inflateEnd
argument_list|(
operator|&
name|strm
operator|->
name|z
argument_list|)
expr_stmt|;
name|zlib_post_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
operator|==
name|Z_OK
condition|)
return|return;
name|error
argument_list|(
literal|"inflateEnd: %s (%s)"
argument_list|,
name|zerr_to_string
argument_list|(
name|status
argument_list|)
argument_list|,
name|strm
operator|->
name|z
operator|.
name|msg
condition|?
name|strm
operator|->
name|z
operator|.
name|msg
else|:
literal|"no message"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_inflate
name|int
name|git_inflate
parameter_list|(
name|git_zstream
modifier|*
name|strm
parameter_list|,
name|int
name|flush
parameter_list|)
block|{
name|int
name|status
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|zlib_pre_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
comment|/* Never say Z_FINISH unless we are feeding everything */
name|status
operator|=
name|inflate
argument_list|(
operator|&
name|strm
operator|->
name|z
argument_list|,
operator|(
name|strm
operator|->
name|z
operator|.
name|avail_in
operator|!=
name|strm
operator|->
name|avail_in
operator|)
condition|?
literal|0
else|:
name|flush
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
operator|==
name|Z_MEM_ERROR
condition|)
name|die
argument_list|(
literal|"inflate: out of memory"
argument_list|)
expr_stmt|;
name|zlib_post_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
comment|/* 		 * Let zlib work another round, while we can still 		 * make progress. 		 */
if|if
condition|(
operator|(
name|strm
operator|->
name|avail_out
operator|&&
operator|!
name|strm
operator|->
name|z
operator|.
name|avail_out
operator|)
operator|&&
operator|(
name|status
operator|==
name|Z_OK
operator|||
name|status
operator|==
name|Z_BUF_ERROR
operator|)
condition|)
continue|continue;
break|break;
block|}
switch|switch
condition|(
name|status
condition|)
block|{
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
name|status
return|;
default|default:
break|break;
block|}
name|error
argument_list|(
literal|"inflate: %s (%s)"
argument_list|,
name|zerr_to_string
argument_list|(
name|status
argument_list|)
argument_list|,
name|strm
operator|->
name|z
operator|.
name|msg
condition|?
name|strm
operator|->
name|z
operator|.
name|msg
else|:
literal|"no message"
argument_list|)
expr_stmt|;
return|return
name|status
return|;
block|}
end_function

begin_if
if|#
directive|if
name|defined
argument_list|(
name|NO_DEFLATE_BOUND
argument_list|)
operator|||
name|ZLIB_VERNUM
operator|<
literal|0x1200
end_if

begin_define
DECL|macro|deflateBound
define|#
directive|define
name|deflateBound
parameter_list|(
name|c
parameter_list|,
name|s
parameter_list|)
value|((s) + (((s) + 7)>> 3) + (((s) + 63)>> 6) + 11)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|git_deflate_bound
name|unsigned
name|long
name|git_deflate_bound
parameter_list|(
name|git_zstream
modifier|*
name|strm
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
return|return
name|deflateBound
argument_list|(
operator|&
name|strm
operator|->
name|z
argument_list|,
name|size
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|git_deflate_init
name|void
name|git_deflate_init
parameter_list|(
name|git_zstream
modifier|*
name|strm
parameter_list|,
name|int
name|level
parameter_list|)
block|{
name|int
name|status
decl_stmt|;
name|zlib_pre_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
name|status
operator|=
name|deflateInit
argument_list|(
operator|&
name|strm
operator|->
name|z
argument_list|,
name|level
argument_list|)
expr_stmt|;
name|zlib_post_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
operator|==
name|Z_OK
condition|)
return|return;
name|die
argument_list|(
literal|"deflateInit: %s (%s)"
argument_list|,
name|zerr_to_string
argument_list|(
name|status
argument_list|)
argument_list|,
name|strm
operator|->
name|z
operator|.
name|msg
condition|?
name|strm
operator|->
name|z
operator|.
name|msg
else|:
literal|"no message"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_deflate_init_gzip
name|void
name|git_deflate_init_gzip
parameter_list|(
name|git_zstream
modifier|*
name|strm
parameter_list|,
name|int
name|level
parameter_list|)
block|{
comment|/* 	 * Use default 15 bits, +16 is to generate gzip header/trailer 	 * instead of the zlib wrapper. 	 */
specifier|const
name|int
name|windowBits
init|=
literal|15
operator|+
literal|16
decl_stmt|;
name|int
name|status
decl_stmt|;
name|zlib_pre_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
name|status
operator|=
name|deflateInit2
argument_list|(
operator|&
name|strm
operator|->
name|z
argument_list|,
name|level
argument_list|,
name|Z_DEFLATED
argument_list|,
name|windowBits
argument_list|,
literal|8
argument_list|,
name|Z_DEFAULT_STRATEGY
argument_list|)
expr_stmt|;
name|zlib_post_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
operator|==
name|Z_OK
condition|)
return|return;
name|die
argument_list|(
literal|"deflateInit2: %s (%s)"
argument_list|,
name|zerr_to_string
argument_list|(
name|status
argument_list|)
argument_list|,
name|strm
operator|->
name|z
operator|.
name|msg
condition|?
name|strm
operator|->
name|z
operator|.
name|msg
else|:
literal|"no message"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_deflate_end
name|void
name|git_deflate_end
parameter_list|(
name|git_zstream
modifier|*
name|strm
parameter_list|)
block|{
name|int
name|status
decl_stmt|;
name|zlib_pre_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
name|status
operator|=
name|deflateEnd
argument_list|(
operator|&
name|strm
operator|->
name|z
argument_list|)
expr_stmt|;
name|zlib_post_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
operator|==
name|Z_OK
condition|)
return|return;
name|error
argument_list|(
literal|"deflateEnd: %s (%s)"
argument_list|,
name|zerr_to_string
argument_list|(
name|status
argument_list|)
argument_list|,
name|strm
operator|->
name|z
operator|.
name|msg
condition|?
name|strm
operator|->
name|z
operator|.
name|msg
else|:
literal|"no message"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_deflate_end_gently
name|int
name|git_deflate_end_gently
parameter_list|(
name|git_zstream
modifier|*
name|strm
parameter_list|)
block|{
name|int
name|status
decl_stmt|;
name|zlib_pre_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
name|status
operator|=
name|deflateEnd
argument_list|(
operator|&
name|strm
operator|->
name|z
argument_list|)
expr_stmt|;
name|zlib_post_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
return|return
name|status
return|;
block|}
end_function

begin_function
DECL|function|git_deflate
name|int
name|git_deflate
parameter_list|(
name|git_zstream
modifier|*
name|strm
parameter_list|,
name|int
name|flush
parameter_list|)
block|{
name|int
name|status
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|zlib_pre_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
comment|/* Never say Z_FINISH unless we are feeding everything */
name|status
operator|=
name|deflate
argument_list|(
operator|&
name|strm
operator|->
name|z
argument_list|,
operator|(
name|strm
operator|->
name|z
operator|.
name|avail_in
operator|!=
name|strm
operator|->
name|avail_in
operator|)
condition|?
literal|0
else|:
name|flush
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
operator|==
name|Z_MEM_ERROR
condition|)
name|die
argument_list|(
literal|"deflate: out of memory"
argument_list|)
expr_stmt|;
name|zlib_post_call
argument_list|(
name|strm
argument_list|)
expr_stmt|;
comment|/* 		 * Let zlib work another round, while we can still 		 * make progress. 		 */
if|if
condition|(
operator|(
name|strm
operator|->
name|avail_out
operator|&&
operator|!
name|strm
operator|->
name|z
operator|.
name|avail_out
operator|)
operator|&&
operator|(
name|status
operator|==
name|Z_OK
operator|||
name|status
operator|==
name|Z_BUF_ERROR
operator|)
condition|)
continue|continue;
break|break;
block|}
switch|switch
condition|(
name|status
condition|)
block|{
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
name|status
return|;
default|default:
break|break;
block|}
name|error
argument_list|(
literal|"deflate: %s (%s)"
argument_list|,
name|zerr_to_string
argument_list|(
name|status
argument_list|)
argument_list|,
name|strm
operator|->
name|z
operator|.
name|msg
condition|?
name|strm
operator|->
name|z
operator|.
name|msg
else|:
literal|"no message"
argument_list|)
expr_stmt|;
return|return
name|status
return|;
block|}
end_function

end_unit

