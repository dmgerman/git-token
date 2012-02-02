begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Licensed under a two-clause BSD-style license.  * See LICENSE for details.  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"sliding_window.h"
end_include

begin_include
include|#
directive|include
file|"line_buffer.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_function
DECL|function|input_error
specifier|static
name|int
name|input_error
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|file
parameter_list|)
block|{
if|if
condition|(
operator|!
name|buffer_ferror
argument_list|(
name|file
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"delta preimage ends early"
argument_list|)
return|;
return|return
name|error
argument_list|(
literal|"cannot read delta preimage: %s"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|skip_or_whine
specifier|static
name|int
name|skip_or_whine
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|file
parameter_list|,
name|off_t
name|gap
parameter_list|)
block|{
if|if
condition|(
name|buffer_skip_bytes
argument_list|(
name|file
argument_list|,
name|gap
argument_list|)
operator|!=
name|gap
condition|)
return|return
name|input_error
argument_list|(
name|file
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|read_to_fill_or_whine
specifier|static
name|int
name|read_to_fill_or_whine
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|file
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|size_t
name|width
parameter_list|)
block|{
name|buffer_read_binary
argument_list|(
name|file
argument_list|,
name|buf
argument_list|,
name|width
operator|-
name|buf
operator|->
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|buf
operator|->
name|len
operator|!=
name|width
condition|)
return|return
name|input_error
argument_list|(
name|file
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|check_offset_overflow
specifier|static
name|int
name|check_offset_overflow
parameter_list|(
name|off_t
name|offset
parameter_list|,
name|uintmax_t
name|len
parameter_list|)
block|{
if|if
condition|(
name|len
operator|>
name|maximum_signed_value_of_type
argument_list|(
name|off_t
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"unrepresentable length in delta: "
literal|"%"
name|PRIuMAX
literal|"> OFF_MAX"
argument_list|,
name|len
argument_list|)
return|;
if|if
condition|(
name|signed_add_overflows
argument_list|(
name|offset
argument_list|,
operator|(
name|off_t
operator|)
name|len
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"unrepresentable offset in delta: "
literal|"%"
name|PRIuMAX
literal|" + %"
name|PRIuMAX
literal|"> OFF_MAX"
argument_list|,
operator|(
name|uintmax_t
operator|)
name|offset
argument_list|,
name|len
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|move_window
name|int
name|move_window
parameter_list|(
name|struct
name|sliding_view
modifier|*
name|view
parameter_list|,
name|off_t
name|off
parameter_list|,
name|size_t
name|width
parameter_list|)
block|{
name|off_t
name|file_offset
decl_stmt|;
name|assert
argument_list|(
name|view
argument_list|)
expr_stmt|;
name|assert
argument_list|(
name|view
operator|->
name|width
operator|<=
name|view
operator|->
name|buf
operator|.
name|len
argument_list|)
expr_stmt|;
name|assert
argument_list|(
operator|!
name|check_offset_overflow
argument_list|(
name|view
operator|->
name|off
argument_list|,
name|view
operator|->
name|buf
operator|.
name|len
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|check_offset_overflow
argument_list|(
name|off
argument_list|,
name|width
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|off
operator|<
name|view
operator|->
name|off
operator|||
name|off
operator|+
name|width
operator|<
name|view
operator|->
name|off
operator|+
name|view
operator|->
name|width
condition|)
return|return
name|error
argument_list|(
literal|"invalid delta: window slides left"
argument_list|)
return|;
if|if
condition|(
name|view
operator|->
name|max_off
operator|>=
literal|0
operator|&&
name|view
operator|->
name|max_off
operator|<
name|off
operator|+
name|width
condition|)
return|return
name|error
argument_list|(
literal|"delta preimage ends early"
argument_list|)
return|;
name|file_offset
operator|=
name|view
operator|->
name|off
operator|+
name|view
operator|->
name|buf
operator|.
name|len
expr_stmt|;
if|if
condition|(
name|off
operator|<
name|file_offset
condition|)
block|{
comment|/* Move the overlapping region into place. */
name|strbuf_remove
argument_list|(
operator|&
name|view
operator|->
name|buf
argument_list|,
literal|0
argument_list|,
name|off
operator|-
name|view
operator|->
name|off
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/* Seek ahead to skip the gap. */
if|if
condition|(
name|skip_or_whine
argument_list|(
name|view
operator|->
name|file
argument_list|,
name|off
operator|-
name|file_offset
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|strbuf_setlen
argument_list|(
operator|&
name|view
operator|->
name|buf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|view
operator|->
name|buf
operator|.
name|len
operator|>
name|width
condition|)
empty_stmt|;
comment|/* Already read. */
elseif|else
if|if
condition|(
name|read_to_fill_or_whine
argument_list|(
name|view
operator|->
name|file
argument_list|,
operator|&
name|view
operator|->
name|buf
argument_list|,
name|width
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|view
operator|->
name|off
operator|=
name|off
expr_stmt|;
name|view
operator|->
name|width
operator|=
name|width
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

