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

begin_function
DECL|function|git_inflate_init
name|void
name|git_inflate_init
parameter_list|(
name|z_streamp
name|strm
parameter_list|)
block|{
name|int
name|status
init|=
name|inflateInit
argument_list|(
name|strm
argument_list|)
decl_stmt|;
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
DECL|function|git_inflate_init_gzip_only
name|void
name|git_inflate_init_gzip_only
parameter_list|(
name|z_streamp
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
init|=
name|inflateInit2
argument_list|(
name|strm
argument_list|,
name|windowBits
argument_list|)
decl_stmt|;
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
name|int
name|status
init|=
name|inflateEnd
argument_list|(
name|strm
argument_list|)
decl_stmt|;
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
name|status
init|=
name|inflate
argument_list|(
name|strm
argument_list|,
name|flush
argument_list|)
decl_stmt|;
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
case|case
name|Z_MEM_ERROR
case|:
name|die
argument_list|(
literal|"inflate: out of memory"
argument_list|)
expr_stmt|;
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
name|status
return|;
block|}
end_function

begin_function
DECL|function|git_deflate_init
name|void
name|git_deflate_init
parameter_list|(
name|z_streamp
name|strm
parameter_list|,
name|int
name|level
parameter_list|)
block|{
name|int
name|status
init|=
name|deflateInit
argument_list|(
name|strm
argument_list|,
name|level
argument_list|)
decl_stmt|;
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
DECL|function|git_deflate_init_gzip
name|void
name|git_deflate_init_gzip
parameter_list|(
name|z_streamp
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
init|=
name|deflateInit2
argument_list|(
name|strm
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
decl_stmt|;
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
DECL|function|git_deflate_end
name|void
name|git_deflate_end
parameter_list|(
name|z_streamp
name|strm
parameter_list|)
block|{
name|int
name|status
init|=
name|deflateEnd
argument_list|(
name|strm
argument_list|)
decl_stmt|;
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
DECL|function|git_deflate_end_gently
name|int
name|git_deflate_end_gently
parameter_list|(
name|z_streamp
name|strm
parameter_list|)
block|{
return|return
name|deflateEnd
argument_list|(
name|strm
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|git_deflate
name|int
name|git_deflate
parameter_list|(
name|z_streamp
name|strm
parameter_list|,
name|int
name|flush
parameter_list|)
block|{
name|int
name|status
init|=
name|deflate
argument_list|(
name|strm
argument_list|,
name|flush
argument_list|)
decl_stmt|;
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
case|case
name|Z_MEM_ERROR
case|:
name|die
argument_list|(
literal|"deflate: out of memory"
argument_list|)
expr_stmt|;
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
name|status
return|;
block|}
end_function

end_unit

