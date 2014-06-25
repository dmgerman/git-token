begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"pack.h"
end_include

begin_include
include|#
directive|include
file|"pack-objects.h"
end_include

begin_function
DECL|function|locate_object_entry_hash
specifier|static
name|uint32_t
name|locate_object_entry_hash
parameter_list|(
name|struct
name|packing_data
modifier|*
name|pdata
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
modifier|*
name|found
parameter_list|)
block|{
name|uint32_t
name|i
decl_stmt|,
name|hash
decl_stmt|,
name|mask
init|=
operator|(
name|pdata
operator|->
name|index_size
operator|-
literal|1
operator|)
decl_stmt|;
name|memcpy
argument_list|(
operator|&
name|hash
argument_list|,
name|sha1
argument_list|,
sizeof|sizeof
argument_list|(
name|uint32_t
argument_list|)
argument_list|)
expr_stmt|;
name|i
operator|=
name|hash
operator|&
name|mask
expr_stmt|;
while|while
condition|(
name|pdata
operator|->
name|index
index|[
name|i
index|]
operator|>
literal|0
condition|)
block|{
name|uint32_t
name|pos
init|=
name|pdata
operator|->
name|index
index|[
name|i
index|]
operator|-
literal|1
decl_stmt|;
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|sha1
argument_list|,
name|pdata
operator|->
name|objects
index|[
name|pos
index|]
operator|.
name|idx
operator|.
name|sha1
argument_list|)
condition|)
block|{
operator|*
name|found
operator|=
literal|1
expr_stmt|;
return|return
name|i
return|;
block|}
name|i
operator|=
operator|(
name|i
operator|+
literal|1
operator|)
operator|&
name|mask
expr_stmt|;
block|}
operator|*
name|found
operator|=
literal|0
expr_stmt|;
return|return
name|i
return|;
block|}
end_function

begin_function
DECL|function|closest_pow2
specifier|static
specifier|inline
name|uint32_t
name|closest_pow2
parameter_list|(
name|uint32_t
name|v
parameter_list|)
block|{
name|v
operator|=
name|v
operator|-
literal|1
expr_stmt|;
name|v
operator||=
name|v
operator|>>
literal|1
expr_stmt|;
name|v
operator||=
name|v
operator|>>
literal|2
expr_stmt|;
name|v
operator||=
name|v
operator|>>
literal|4
expr_stmt|;
name|v
operator||=
name|v
operator|>>
literal|8
expr_stmt|;
name|v
operator||=
name|v
operator|>>
literal|16
expr_stmt|;
return|return
name|v
operator|+
literal|1
return|;
block|}
end_function

begin_function
DECL|function|rehash_objects
specifier|static
name|void
name|rehash_objects
parameter_list|(
name|struct
name|packing_data
modifier|*
name|pdata
parameter_list|)
block|{
name|uint32_t
name|i
decl_stmt|;
name|struct
name|object_entry
modifier|*
name|entry
decl_stmt|;
name|pdata
operator|->
name|index_size
operator|=
name|closest_pow2
argument_list|(
name|pdata
operator|->
name|nr_objects
operator|*
literal|3
argument_list|)
expr_stmt|;
if|if
condition|(
name|pdata
operator|->
name|index_size
operator|<
literal|1024
condition|)
name|pdata
operator|->
name|index_size
operator|=
literal|1024
expr_stmt|;
name|free
argument_list|(
name|pdata
operator|->
name|index
argument_list|)
expr_stmt|;
name|pdata
operator|->
name|index
operator|=
name|xcalloc
argument_list|(
name|pdata
operator|->
name|index_size
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|pdata
operator|->
name|index
argument_list|)
argument_list|)
expr_stmt|;
name|entry
operator|=
name|pdata
operator|->
name|objects
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|pdata
operator|->
name|nr_objects
condition|;
name|i
operator|++
control|)
block|{
name|int
name|found
decl_stmt|;
name|uint32_t
name|ix
init|=
name|locate_object_entry_hash
argument_list|(
name|pdata
argument_list|,
name|entry
operator|->
name|idx
operator|.
name|sha1
argument_list|,
operator|&
name|found
argument_list|)
decl_stmt|;
if|if
condition|(
name|found
condition|)
name|die
argument_list|(
literal|"BUG: Duplicate object in hash"
argument_list|)
expr_stmt|;
name|pdata
operator|->
name|index
index|[
name|ix
index|]
operator|=
name|i
operator|+
literal|1
expr_stmt|;
name|entry
operator|++
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|packlist_find
name|struct
name|object_entry
modifier|*
name|packlist_find
parameter_list|(
name|struct
name|packing_data
modifier|*
name|pdata
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|uint32_t
modifier|*
name|index_pos
parameter_list|)
block|{
name|uint32_t
name|i
decl_stmt|;
name|int
name|found
decl_stmt|;
if|if
condition|(
operator|!
name|pdata
operator|->
name|index_size
condition|)
return|return
name|NULL
return|;
name|i
operator|=
name|locate_object_entry_hash
argument_list|(
name|pdata
argument_list|,
name|sha1
argument_list|,
operator|&
name|found
argument_list|)
expr_stmt|;
if|if
condition|(
name|index_pos
condition|)
operator|*
name|index_pos
operator|=
name|i
expr_stmt|;
if|if
condition|(
operator|!
name|found
condition|)
return|return
name|NULL
return|;
return|return
operator|&
name|pdata
operator|->
name|objects
index|[
name|pdata
operator|->
name|index
index|[
name|i
index|]
operator|-
literal|1
index|]
return|;
block|}
end_function

begin_function
DECL|function|packlist_alloc
name|struct
name|object_entry
modifier|*
name|packlist_alloc
parameter_list|(
name|struct
name|packing_data
modifier|*
name|pdata
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|uint32_t
name|index_pos
parameter_list|)
block|{
name|struct
name|object_entry
modifier|*
name|new_entry
decl_stmt|;
if|if
condition|(
name|pdata
operator|->
name|nr_objects
operator|>=
name|pdata
operator|->
name|nr_alloc
condition|)
block|{
name|pdata
operator|->
name|nr_alloc
operator|=
operator|(
name|pdata
operator|->
name|nr_alloc
operator|+
literal|1024
operator|)
operator|*
literal|3
operator|/
literal|2
expr_stmt|;
name|pdata
operator|->
name|objects
operator|=
name|xrealloc
argument_list|(
name|pdata
operator|->
name|objects
argument_list|,
name|pdata
operator|->
name|nr_alloc
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|new_entry
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|new_entry
operator|=
name|pdata
operator|->
name|objects
operator|+
name|pdata
operator|->
name|nr_objects
operator|++
expr_stmt|;
name|memset
argument_list|(
name|new_entry
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|new_entry
argument_list|)
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|new_entry
operator|->
name|idx
operator|.
name|sha1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|pdata
operator|->
name|index_size
operator|*
literal|3
operator|<=
name|pdata
operator|->
name|nr_objects
operator|*
literal|4
condition|)
name|rehash_objects
argument_list|(
name|pdata
argument_list|)
expr_stmt|;
else|else
name|pdata
operator|->
name|index
index|[
name|index_pos
index|]
operator|=
name|pdata
operator|->
name|nr_objects
expr_stmt|;
return|return
name|new_entry
return|;
block|}
end_function

end_unit

