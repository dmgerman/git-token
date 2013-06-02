begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"../git-compat-util.h"
end_include

begin_undef
DECL|macro|write
undef|#
directive|undef
name|write
end_undef

begin_comment
comment|/*  * Version of write that will write at most INT_MAX bytes.  * Workaround a xnu bug on Mac OS X  */
end_comment

begin_function
DECL|function|clipped_write
name|ssize_t
name|clipped_write
parameter_list|(
name|int
name|fildes
parameter_list|,
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|nbyte
parameter_list|)
block|{
if|if
condition|(
name|nbyte
operator|>
name|INT_MAX
condition|)
name|nbyte
operator|=
name|INT_MAX
expr_stmt|;
return|return
name|write
argument_list|(
name|fildes
argument_list|,
name|buf
argument_list|,
name|nbyte
argument_list|)
return|;
block|}
end_function

end_unit

