begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2005 Junio C Hamano  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"diffcore.h"
end_include

begin_include
include|#
directive|include
file|"delta.h"
end_include

begin_struct
DECL|struct|path_spec
struct|struct
name|path_spec
block|{
DECL|member|spec
specifier|const
name|char
modifier|*
name|spec
decl_stmt|;
DECL|member|len
name|int
name|len
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|matches_pathspec
specifier|static
name|int
name|matches_pathspec
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|struct
name|path_spec
modifier|*
name|s
parameter_list|,
name|int
name|cnt
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|namelen
decl_stmt|;
if|if
condition|(
name|cnt
operator|==
literal|0
condition|)
return|return
literal|1
return|;
name|namelen
operator|=
name|strlen
argument_list|(
name|name
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|cnt
condition|;
name|i
operator|++
control|)
block|{
name|int
name|len
init|=
name|s
index|[
name|i
index|]
operator|.
name|len
decl_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|s
index|[
name|i
index|]
operator|.
name|spec
argument_list|,
name|name
argument_list|,
name|len
argument_list|)
operator|&&
name|len
operator|<=
name|namelen
operator|&&
operator|(
name|name
index|[
name|len
index|]
operator|==
literal|0
operator|||
name|name
index|[
name|len
index|]
operator|==
literal|'/'
operator|)
condition|)
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|diffcore_pathspec
name|void
name|diffcore_pathspec
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
block|{
name|struct
name|diff_queue_struct
modifier|*
name|q
init|=
operator|&
name|diff_queued_diff
decl_stmt|;
name|int
name|i
decl_stmt|,
name|speccnt
decl_stmt|;
name|struct
name|diff_queue_struct
name|outq
decl_stmt|;
name|struct
name|path_spec
modifier|*
name|spec
decl_stmt|;
name|outq
operator|.
name|queue
operator|=
name|NULL
expr_stmt|;
name|outq
operator|.
name|nr
operator|=
name|outq
operator|.
name|alloc
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|pathspec
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
empty_stmt|;
name|speccnt
operator|=
name|i
expr_stmt|;
name|spec
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|spec
argument_list|)
operator|*
name|speccnt
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|pathspec
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
name|int
name|l
decl_stmt|;
name|spec
index|[
name|i
index|]
operator|.
name|spec
operator|=
name|pathspec
index|[
name|i
index|]
expr_stmt|;
name|l
operator|=
name|strlen
argument_list|(
name|pathspec
index|[
name|i
index|]
argument_list|)
expr_stmt|;
while|while
condition|(
name|l
operator|>
literal|0
operator|&&
name|pathspec
index|[
name|i
index|]
index|[
name|l
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
name|l
operator|--
expr_stmt|;
name|spec
index|[
name|i
index|]
operator|.
name|len
operator|=
name|l
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|diff_filepair
modifier|*
name|p
init|=
name|q
operator|->
name|queue
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|matches_pathspec
argument_list|(
name|p
operator|->
name|one
operator|->
name|path
argument_list|,
name|spec
argument_list|,
name|speccnt
argument_list|)
operator|||
name|matches_pathspec
argument_list|(
name|p
operator|->
name|two
operator|->
name|path
argument_list|,
name|spec
argument_list|,
name|speccnt
argument_list|)
condition|)
name|diff_q
argument_list|(
operator|&
name|outq
argument_list|,
name|p
argument_list|)
expr_stmt|;
else|else
name|diff_free_filepair
argument_list|(
name|p
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|q
operator|->
name|queue
argument_list|)
expr_stmt|;
operator|*
name|q
operator|=
name|outq
expr_stmt|;
return|return;
block|}
end_function

end_unit

