begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"mergesort.h"
end_include

begin_struct
DECL|struct|line
struct|struct
name|line
block|{
DECL|member|text
name|char
modifier|*
name|text
decl_stmt|;
DECL|member|next
name|struct
name|line
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|get_next
specifier|static
name|void
modifier|*
name|get_next
parameter_list|(
specifier|const
name|void
modifier|*
name|a
parameter_list|)
block|{
return|return
operator|(
operator|(
specifier|const
expr|struct
name|line
operator|*
operator|)
name|a
operator|)
operator|->
name|next
return|;
block|}
end_function

begin_function
DECL|function|set_next
specifier|static
name|void
name|set_next
parameter_list|(
name|void
modifier|*
name|a
parameter_list|,
name|void
modifier|*
name|b
parameter_list|)
block|{
operator|(
operator|(
expr|struct
name|line
operator|*
operator|)
name|a
operator|)
operator|->
name|next
operator|=
name|b
expr_stmt|;
block|}
end_function

begin_function
DECL|function|compare_strings
specifier|static
name|int
name|compare_strings
parameter_list|(
specifier|const
name|void
modifier|*
name|a
parameter_list|,
specifier|const
name|void
modifier|*
name|b
parameter_list|)
block|{
specifier|const
name|struct
name|line
modifier|*
name|x
init|=
name|a
decl_stmt|,
modifier|*
name|y
init|=
name|b
decl_stmt|;
return|return
name|strcmp
argument_list|(
name|x
operator|->
name|text
argument_list|,
name|y
operator|->
name|text
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|cmd_main
name|int
name|cmd_main
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|struct
name|line
modifier|*
name|line
decl_stmt|,
modifier|*
name|p
init|=
name|NULL
decl_stmt|,
modifier|*
name|lines
init|=
name|NULL
decl_stmt|;
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|strbuf_getwholeline
argument_list|(
operator|&
name|sb
argument_list|,
name|stdin
argument_list|,
literal|'\n'
argument_list|)
condition|)
break|break;
name|line
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|line
argument_list|)
argument_list|)
expr_stmt|;
name|line
operator|->
name|text
operator|=
name|strbuf_detach
argument_list|(
operator|&
name|sb
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
condition|)
block|{
name|line
operator|->
name|next
operator|=
name|p
operator|->
name|next
expr_stmt|;
name|p
operator|->
name|next
operator|=
name|line
expr_stmt|;
block|}
else|else
block|{
name|line
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|lines
operator|=
name|line
expr_stmt|;
block|}
name|p
operator|=
name|line
expr_stmt|;
block|}
name|lines
operator|=
name|llist_mergesort
argument_list|(
name|lines
argument_list|,
name|get_next
argument_list|,
name|set_next
argument_list|,
name|compare_strings
argument_list|)
expr_stmt|;
while|while
condition|(
name|lines
condition|)
block|{
name|printf
argument_list|(
literal|"%s"
argument_list|,
name|lines
operator|->
name|text
argument_list|)
expr_stmt|;
name|lines
operator|=
name|lines
operator|->
name|next
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

