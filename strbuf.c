begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_function
DECL|function|strbuf_init
name|void
name|strbuf_init
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|size_t
name|hint
parameter_list|)
block|{
name|memset
argument_list|(
name|sb
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|sb
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|hint
condition|)
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
name|hint
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_release
name|void
name|strbuf_release
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
block|{
name|free
argument_list|(
name|sb
operator|->
name|buf
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|sb
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|sb
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_reset
name|void
name|strbuf_reset
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
block|{
if|if
condition|(
name|sb
operator|->
name|len
condition|)
name|strbuf_setlen
argument_list|(
name|sb
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|sb
operator|->
name|eof
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_detach
name|char
modifier|*
name|strbuf_detach
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
block|{
name|char
modifier|*
name|res
init|=
name|sb
operator|->
name|buf
decl_stmt|;
name|strbuf_init
argument_list|(
name|sb
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return
name|res
return|;
block|}
end_function

begin_function
DECL|function|strbuf_grow
name|void
name|strbuf_grow
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|size_t
name|extra
parameter_list|)
block|{
if|if
condition|(
name|sb
operator|->
name|len
operator|+
name|extra
operator|+
literal|1
operator|<=
name|sb
operator|->
name|len
condition|)
name|die
argument_list|(
literal|"you want to use way too much memory"
argument_list|)
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|sb
operator|->
name|buf
argument_list|,
name|sb
operator|->
name|len
operator|+
name|extra
operator|+
literal|1
argument_list|,
name|sb
operator|->
name|alloc
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_rtrim
name|void
name|strbuf_rtrim
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
block|{
while|while
condition|(
name|sb
operator|->
name|len
operator|>
literal|0
operator|&&
name|isspace
argument_list|(
operator|(
name|unsigned
name|char
operator|)
name|sb
operator|->
name|buf
index|[
name|sb
operator|->
name|len
operator|-
literal|1
index|]
argument_list|)
condition|)
name|sb
operator|->
name|len
operator|--
expr_stmt|;
name|sb
operator|->
name|buf
index|[
name|sb
operator|->
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_insert
name|void
name|strbuf_insert
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|size_t
name|pos
parameter_list|,
specifier|const
name|void
modifier|*
name|data
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|pos
operator|>=
name|sb
operator|->
name|len
condition|)
block|{
name|pos
operator|=
name|sb
operator|->
name|len
expr_stmt|;
block|}
else|else
block|{
name|memmove
argument_list|(
name|sb
operator|->
name|buf
operator|+
name|pos
operator|+
name|len
argument_list|,
name|sb
operator|->
name|buf
operator|+
name|pos
argument_list|,
name|sb
operator|->
name|len
operator|-
name|pos
argument_list|)
expr_stmt|;
block|}
name|memcpy
argument_list|(
name|sb
operator|->
name|buf
operator|+
name|pos
argument_list|,
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|strbuf_setlen
argument_list|(
name|sb
argument_list|,
name|sb
operator|->
name|len
operator|+
name|len
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_add
name|void
name|strbuf_add
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|void
modifier|*
name|data
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|sb
operator|->
name|buf
operator|+
name|sb
operator|->
name|len
argument_list|,
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|strbuf_setlen
argument_list|(
name|sb
argument_list|,
name|sb
operator|->
name|len
operator|+
name|len
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_addf
name|void
name|strbuf_addf
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|int
name|len
decl_stmt|;
name|va_list
name|ap
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|len
operator|=
name|vsnprintf
argument_list|(
name|sb
operator|->
name|buf
operator|+
name|sb
operator|->
name|len
argument_list|,
name|sb
operator|->
name|alloc
operator|-
name|sb
operator|->
name|len
argument_list|,
name|fmt
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|<
literal|0
condition|)
block|{
name|len
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|len
operator|>
name|strbuf_avail
argument_list|(
name|sb
argument_list|)
condition|)
block|{
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|len
operator|=
name|vsnprintf
argument_list|(
name|sb
operator|->
name|buf
operator|+
name|sb
operator|->
name|len
argument_list|,
name|sb
operator|->
name|alloc
operator|-
name|sb
operator|->
name|len
argument_list|,
name|fmt
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>
name|strbuf_avail
argument_list|(
name|sb
argument_list|)
condition|)
block|{
name|die
argument_list|(
literal|"this should not happen, your snprintf is broken"
argument_list|)
expr_stmt|;
block|}
block|}
name|strbuf_setlen
argument_list|(
name|sb
argument_list|,
name|sb
operator|->
name|len
operator|+
name|len
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_fread
name|size_t
name|strbuf_fread
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|size_t
name|size
parameter_list|,
name|FILE
modifier|*
name|f
parameter_list|)
block|{
name|size_t
name|res
decl_stmt|;
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|res
operator|=
name|fread
argument_list|(
name|sb
operator|->
name|buf
operator|+
name|sb
operator|->
name|len
argument_list|,
literal|1
argument_list|,
name|size
argument_list|,
name|f
argument_list|)
expr_stmt|;
if|if
condition|(
name|res
operator|>
literal|0
condition|)
block|{
name|strbuf_setlen
argument_list|(
name|sb
argument_list|,
name|sb
operator|->
name|len
operator|+
name|res
argument_list|)
expr_stmt|;
block|}
return|return
name|res
return|;
block|}
end_function

begin_function
DECL|function|strbuf_read
name|ssize_t
name|strbuf_read
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|int
name|fd
parameter_list|,
name|size_t
name|hint
parameter_list|)
block|{
name|size_t
name|oldlen
init|=
name|sb
operator|->
name|len
decl_stmt|;
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
name|hint
condition|?
name|hint
else|:
literal|8192
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|ssize_t
name|cnt
decl_stmt|;
name|cnt
operator|=
name|xread
argument_list|(
name|fd
argument_list|,
name|sb
operator|->
name|buf
operator|+
name|sb
operator|->
name|len
argument_list|,
name|sb
operator|->
name|alloc
operator|-
name|sb
operator|->
name|len
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|cnt
operator|<
literal|0
condition|)
block|{
name|strbuf_setlen
argument_list|(
name|sb
argument_list|,
name|oldlen
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
operator|!
name|cnt
condition|)
break|break;
name|sb
operator|->
name|len
operator|+=
name|cnt
expr_stmt|;
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
literal|8192
argument_list|)
expr_stmt|;
block|}
name|sb
operator|->
name|buf
index|[
name|sb
operator|->
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
return|return
name|sb
operator|->
name|len
operator|-
name|oldlen
return|;
block|}
end_function

begin_function
DECL|function|read_line
name|void
name|read_line
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|FILE
modifier|*
name|fp
parameter_list|,
name|int
name|term
parameter_list|)
block|{
name|int
name|ch
decl_stmt|;
if|if
condition|(
name|feof
argument_list|(
name|fp
argument_list|)
condition|)
block|{
name|strbuf_release
argument_list|(
name|sb
argument_list|)
expr_stmt|;
name|sb
operator|->
name|eof
operator|=
literal|1
expr_stmt|;
return|return;
block|}
name|strbuf_reset
argument_list|(
name|sb
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|ch
operator|=
name|fgetc
argument_list|(
name|fp
argument_list|)
operator|)
operator|!=
name|EOF
condition|)
block|{
if|if
condition|(
name|ch
operator|==
name|term
condition|)
break|break;
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|sb
operator|->
name|buf
index|[
name|sb
operator|->
name|len
operator|++
index|]
operator|=
name|ch
expr_stmt|;
block|}
if|if
condition|(
name|ch
operator|==
name|EOF
operator|&&
name|sb
operator|->
name|len
operator|==
literal|0
condition|)
block|{
name|strbuf_release
argument_list|(
name|sb
argument_list|)
expr_stmt|;
name|sb
operator|->
name|eof
operator|=
literal|1
expr_stmt|;
block|}
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|sb
operator|->
name|buf
index|[
name|sb
operator|->
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
block|}
end_function

end_unit

