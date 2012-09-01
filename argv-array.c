begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"argv-array.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_decl_stmt
DECL|variable|empty_argv
specifier|const
name|char
modifier|*
name|empty_argv
index|[]
init|=
block|{
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|argv_array_init
name|void
name|argv_array_init
parameter_list|(
name|struct
name|argv_array
modifier|*
name|array
parameter_list|)
block|{
name|array
operator|->
name|argv
operator|=
name|empty_argv
expr_stmt|;
name|array
operator|->
name|argc
operator|=
literal|0
expr_stmt|;
name|array
operator|->
name|alloc
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|argv_array_push_nodup
specifier|static
name|void
name|argv_array_push_nodup
parameter_list|(
name|struct
name|argv_array
modifier|*
name|array
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
if|if
condition|(
name|array
operator|->
name|argv
operator|==
name|empty_argv
condition|)
name|array
operator|->
name|argv
operator|=
name|NULL
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|array
operator|->
name|argv
argument_list|,
name|array
operator|->
name|argc
operator|+
literal|2
argument_list|,
name|array
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|array
operator|->
name|argv
index|[
name|array
operator|->
name|argc
operator|++
index|]
operator|=
name|value
expr_stmt|;
name|array
operator|->
name|argv
index|[
name|array
operator|->
name|argc
index|]
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function
DECL|function|argv_array_push
name|void
name|argv_array_push
parameter_list|(
name|struct
name|argv_array
modifier|*
name|array
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
name|argv_array_push_nodup
argument_list|(
name|array
argument_list|,
name|xstrdup
argument_list|(
name|value
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|argv_array_pushf
name|void
name|argv_array_pushf
parameter_list|(
name|struct
name|argv_array
modifier|*
name|array
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|ap
decl_stmt|;
name|struct
name|strbuf
name|v
init|=
name|STRBUF_INIT
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|strbuf_vaddf
argument_list|(
operator|&
name|v
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
name|argv_array_push_nodup
argument_list|(
name|array
argument_list|,
name|strbuf_detach
argument_list|(
operator|&
name|v
argument_list|,
name|NULL
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|argv_array_pushl
name|void
name|argv_array_pushl
parameter_list|(
name|struct
name|argv_array
modifier|*
name|array
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|ap
decl_stmt|;
specifier|const
name|char
modifier|*
name|arg
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|array
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|arg
operator|=
name|va_arg
argument_list|(
name|ap
argument_list|,
specifier|const
name|char
operator|*
argument_list|)
operator|)
condition|)
name|argv_array_push
argument_list|(
name|array
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|argv_array_pop
name|void
name|argv_array_pop
parameter_list|(
name|struct
name|argv_array
modifier|*
name|array
parameter_list|)
block|{
if|if
condition|(
operator|!
name|array
operator|->
name|argc
condition|)
return|return;
name|free
argument_list|(
operator|(
name|char
operator|*
operator|)
name|array
operator|->
name|argv
index|[
name|array
operator|->
name|argc
operator|-
literal|1
index|]
argument_list|)
expr_stmt|;
name|array
operator|->
name|argv
index|[
name|array
operator|->
name|argc
operator|-
literal|1
index|]
operator|=
name|NULL
expr_stmt|;
name|array
operator|->
name|argc
operator|--
expr_stmt|;
block|}
end_function

begin_function
DECL|function|argv_array_clear
name|void
name|argv_array_clear
parameter_list|(
name|struct
name|argv_array
modifier|*
name|array
parameter_list|)
block|{
if|if
condition|(
name|array
operator|->
name|argv
operator|!=
name|empty_argv
condition|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|array
operator|->
name|argc
condition|;
name|i
operator|++
control|)
name|free
argument_list|(
operator|(
name|char
operator|*
operator|*
operator|)
name|array
operator|->
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|array
operator|->
name|argv
argument_list|)
expr_stmt|;
block|}
name|argv_array_init
argument_list|(
name|array
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

