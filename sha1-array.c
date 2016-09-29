begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"sha1-array.h"
end_include

begin_include
include|#
directive|include
file|"sha1-lookup.h"
end_include

begin_function
DECL|function|sha1_array_append
name|void
name|sha1_array_append
parameter_list|(
name|struct
name|sha1_array
modifier|*
name|array
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|ALLOC_GROW
argument_list|(
name|array
operator|->
name|sha1
argument_list|,
name|array
operator|->
name|nr
operator|+
literal|1
argument_list|,
name|array
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|array
operator|->
name|sha1
index|[
name|array
operator|->
name|nr
operator|++
index|]
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|array
operator|->
name|sorted
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|void_hashcmp
specifier|static
name|int
name|void_hashcmp
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
return|return
name|hashcmp
argument_list|(
name|a
argument_list|,
name|b
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|sha1_array_sort
specifier|static
name|void
name|sha1_array_sort
parameter_list|(
name|struct
name|sha1_array
modifier|*
name|array
parameter_list|)
block|{
name|QSORT
argument_list|(
name|array
operator|->
name|sha1
argument_list|,
name|array
operator|->
name|nr
argument_list|,
name|void_hashcmp
argument_list|)
expr_stmt|;
name|array
operator|->
name|sorted
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|sha1_access
specifier|static
specifier|const
name|unsigned
name|char
modifier|*
name|sha1_access
parameter_list|(
name|size_t
name|index
parameter_list|,
name|void
modifier|*
name|table
parameter_list|)
block|{
name|unsigned
name|char
argument_list|(
operator|*
name|array
argument_list|)
decl|[20]
init|=
name|table
decl_stmt|;
return|return
name|array
index|[
name|index
index|]
return|;
block|}
end_function

begin_function
DECL|function|sha1_array_lookup
name|int
name|sha1_array_lookup
parameter_list|(
name|struct
name|sha1_array
modifier|*
name|array
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
if|if
condition|(
operator|!
name|array
operator|->
name|sorted
condition|)
name|sha1_array_sort
argument_list|(
name|array
argument_list|)
expr_stmt|;
return|return
name|sha1_pos
argument_list|(
name|sha1
argument_list|,
name|array
operator|->
name|sha1
argument_list|,
name|array
operator|->
name|nr
argument_list|,
name|sha1_access
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|sha1_array_clear
name|void
name|sha1_array_clear
parameter_list|(
name|struct
name|sha1_array
modifier|*
name|array
parameter_list|)
block|{
name|free
argument_list|(
name|array
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|array
operator|->
name|sha1
operator|=
name|NULL
expr_stmt|;
name|array
operator|->
name|nr
operator|=
literal|0
expr_stmt|;
name|array
operator|->
name|alloc
operator|=
literal|0
expr_stmt|;
name|array
operator|->
name|sorted
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|sha1_array_for_each_unique
name|void
name|sha1_array_for_each_unique
parameter_list|(
name|struct
name|sha1_array
modifier|*
name|array
parameter_list|,
name|for_each_sha1_fn
name|fn
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
operator|!
name|array
operator|->
name|sorted
condition|)
name|sha1_array_sort
argument_list|(
name|array
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
name|array
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|i
operator|>
literal|0
operator|&&
operator|!
name|hashcmp
argument_list|(
name|array
operator|->
name|sha1
index|[
name|i
index|]
argument_list|,
name|array
operator|->
name|sha1
index|[
name|i
operator|-
literal|1
index|]
argument_list|)
condition|)
continue|continue;
name|fn
argument_list|(
name|array
operator|->
name|sha1
index|[
name|i
index|]
argument_list|,
name|data
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit

