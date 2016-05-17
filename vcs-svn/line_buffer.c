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
file|"line_buffer.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_define
DECL|macro|COPY_BUFFER_LEN
define|#
directive|define
name|COPY_BUFFER_LEN
value|4096
end_define

begin_function
DECL|function|buffer_init
name|int
name|buffer_init
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|filename
parameter_list|)
block|{
name|buf
operator|->
name|infile
operator|=
name|filename
condition|?
name|fopen
argument_list|(
name|filename
argument_list|,
literal|"r"
argument_list|)
else|:
name|stdin
expr_stmt|;
if|if
condition|(
operator|!
name|buf
operator|->
name|infile
condition|)
return|return
operator|-
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|buffer_fdinit
name|int
name|buffer_fdinit
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|,
name|int
name|fd
parameter_list|)
block|{
name|buf
operator|->
name|infile
operator|=
name|fdopen
argument_list|(
name|fd
argument_list|,
literal|"r"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf
operator|->
name|infile
condition|)
return|return
operator|-
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|buffer_tmpfile_init
name|int
name|buffer_tmpfile_init
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
block|{
name|buf
operator|->
name|infile
operator|=
name|tmpfile
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|buf
operator|->
name|infile
condition|)
return|return
operator|-
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|buffer_deinit
name|int
name|buffer_deinit
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
block|{
name|int
name|err
decl_stmt|;
if|if
condition|(
name|buf
operator|->
name|infile
operator|==
name|stdin
condition|)
return|return
name|ferror
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
return|;
name|err
operator|=
name|ferror
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
expr_stmt|;
name|err
operator||=
name|fclose
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
expr_stmt|;
return|return
name|err
return|;
block|}
end_function

begin_function
DECL|function|buffer_tmpfile_rewind
name|FILE
modifier|*
name|buffer_tmpfile_rewind
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
block|{
name|rewind
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
expr_stmt|;
return|return
name|buf
operator|->
name|infile
return|;
block|}
end_function

begin_function
DECL|function|buffer_tmpfile_prepare_to_read
name|long
name|buffer_tmpfile_prepare_to_read
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
block|{
name|long
name|pos
init|=
name|ftell
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
decl_stmt|;
if|if
condition|(
name|pos
operator|<
literal|0
condition|)
return|return
name|error_errno
argument_list|(
literal|"ftell error"
argument_list|)
return|;
if|if
condition|(
name|fseek
argument_list|(
name|buf
operator|->
name|infile
argument_list|,
literal|0
argument_list|,
name|SEEK_SET
argument_list|)
condition|)
return|return
name|error_errno
argument_list|(
literal|"seek error"
argument_list|)
return|;
return|return
name|pos
return|;
block|}
end_function

begin_function
DECL|function|buffer_ferror
name|int
name|buffer_ferror
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
block|{
return|return
name|ferror
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|buffer_read_char
name|int
name|buffer_read_char
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
block|{
return|return
name|fgetc
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* Read a line without trailing newline. */
end_comment

begin_function
DECL|function|buffer_read_line
name|char
modifier|*
name|buffer_read_line
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
block|{
name|char
modifier|*
name|end
decl_stmt|;
if|if
condition|(
operator|!
name|fgets
argument_list|(
name|buf
operator|->
name|line_buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
operator|->
name|line_buffer
argument_list|)
argument_list|,
name|buf
operator|->
name|infile
argument_list|)
condition|)
comment|/* Error or data exhausted. */
return|return
name|NULL
return|;
name|end
operator|=
name|buf
operator|->
name|line_buffer
operator|+
name|strlen
argument_list|(
name|buf
operator|->
name|line_buffer
argument_list|)
expr_stmt|;
if|if
condition|(
name|end
index|[
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|end
index|[
operator|-
literal|1
index|]
operator|=
literal|'\0'
expr_stmt|;
elseif|else
if|if
condition|(
name|feof
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
condition|)
empty_stmt|;
comment|/* No newline at end of file.  That's fine. */
else|else
comment|/* 		 * Line was too long. 		 * There is probably a saner way to deal with this, 		 * but for now let's return an error. 		 */
return|return
name|NULL
return|;
return|return
name|buf
operator|->
name|line_buffer
return|;
block|}
end_function

begin_function
DECL|function|buffer_read_binary
name|size_t
name|buffer_read_binary
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
return|return
name|strbuf_fread
argument_list|(
name|sb
argument_list|,
name|size
argument_list|,
name|buf
operator|->
name|infile
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|buffer_copy_bytes
name|off_t
name|buffer_copy_bytes
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|,
name|off_t
name|nbytes
parameter_list|)
block|{
name|char
name|byte_buffer
index|[
name|COPY_BUFFER_LEN
index|]
decl_stmt|;
name|off_t
name|done
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|done
operator|<
name|nbytes
operator|&&
operator|!
name|feof
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
operator|&&
operator|!
name|ferror
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
condition|)
block|{
name|off_t
name|len
init|=
name|nbytes
operator|-
name|done
decl_stmt|;
name|size_t
name|in
init|=
name|len
operator|<
name|COPY_BUFFER_LEN
condition|?
name|len
else|:
name|COPY_BUFFER_LEN
decl_stmt|;
name|in
operator|=
name|fread
argument_list|(
name|byte_buffer
argument_list|,
literal|1
argument_list|,
name|in
argument_list|,
name|buf
operator|->
name|infile
argument_list|)
expr_stmt|;
name|done
operator|+=
name|in
expr_stmt|;
name|fwrite
argument_list|(
name|byte_buffer
argument_list|,
literal|1
argument_list|,
name|in
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
name|ferror
argument_list|(
name|stdout
argument_list|)
condition|)
return|return
name|done
operator|+
name|buffer_skip_bytes
argument_list|(
name|buf
argument_list|,
name|nbytes
operator|-
name|done
argument_list|)
return|;
block|}
return|return
name|done
return|;
block|}
end_function

begin_function
DECL|function|buffer_skip_bytes
name|off_t
name|buffer_skip_bytes
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|,
name|off_t
name|nbytes
parameter_list|)
block|{
name|char
name|byte_buffer
index|[
name|COPY_BUFFER_LEN
index|]
decl_stmt|;
name|off_t
name|done
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|done
operator|<
name|nbytes
operator|&&
operator|!
name|feof
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
operator|&&
operator|!
name|ferror
argument_list|(
name|buf
operator|->
name|infile
argument_list|)
condition|)
block|{
name|off_t
name|len
init|=
name|nbytes
operator|-
name|done
decl_stmt|;
name|size_t
name|in
init|=
name|len
operator|<
name|COPY_BUFFER_LEN
condition|?
name|len
else|:
name|COPY_BUFFER_LEN
decl_stmt|;
name|done
operator|+=
name|fread
argument_list|(
name|byte_buffer
argument_list|,
literal|1
argument_list|,
name|in
argument_list|,
name|buf
operator|->
name|infile
argument_list|)
expr_stmt|;
block|}
return|return
name|done
return|;
block|}
end_function

end_unit

