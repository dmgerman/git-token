begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
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
parameter_list|)
block|{
name|sb
operator|->
name|buf
operator|=
literal|0
expr_stmt|;
name|sb
operator|->
name|eof
operator|=
name|sb
operator|->
name|alloc
operator|=
name|sb
operator|->
name|len
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_begin
specifier|static
name|void
name|strbuf_begin
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
name|strbuf_init
argument_list|(
name|sb
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_add
specifier|static
name|void
specifier|inline
name|strbuf_add
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|int
name|ch
parameter_list|)
block|{
if|if
condition|(
name|sb
operator|->
name|alloc
operator|<=
name|sb
operator|->
name|len
condition|)
block|{
name|sb
operator|->
name|alloc
operator|=
name|sb
operator|->
name|alloc
operator|*
literal|3
operator|/
literal|2
operator|+
literal|16
expr_stmt|;
name|sb
operator|->
name|buf
operator|=
name|xrealloc
argument_list|(
name|sb
operator|->
name|buf
argument_list|,
name|sb
operator|->
name|alloc
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
operator|++
index|]
operator|=
name|ch
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_end
specifier|static
name|void
name|strbuf_end
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
block|{
name|strbuf_add
argument_list|(
name|sb
argument_list|,
literal|0
argument_list|)
expr_stmt|;
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
name|strbuf_begin
argument_list|(
name|sb
argument_list|)
expr_stmt|;
if|if
condition|(
name|feof
argument_list|(
name|fp
argument_list|)
condition|)
block|{
name|sb
operator|->
name|eof
operator|=
literal|1
expr_stmt|;
return|return;
block|}
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
name|strbuf_add
argument_list|(
name|sb
argument_list|,
name|ch
argument_list|)
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
name|sb
operator|->
name|eof
operator|=
literal|1
expr_stmt|;
name|strbuf_end
argument_list|(
name|sb
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

