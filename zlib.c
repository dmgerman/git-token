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

end_unit

