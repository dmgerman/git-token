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

begin_decl_stmt
DECL|variable|track_object_refs
name|int
name|track_object_refs
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|refs_hash_size
DECL|variable|nr_object_refs
specifier|static
name|unsigned
name|int
name|refs_hash_size
decl_stmt|,
name|nr_object_refs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|refs_hash
specifier|static
name|struct
name|object_refs
modifier|*
modifier|*
name|refs_hash
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|hash_obj
specifier|static
name|unsigned
name|int
name|hash_obj
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|unsigned
name|int
name|n
parameter_list|)
block|{
name|unsigned
name|int
name|hash
init|=
operator|*
operator|(
name|unsigned
name|int
operator|*
operator|)
name|obj
operator|->
name|sha1
decl_stmt|;
return|return
name|hash
operator|%
name|n
return|;
block|}
end_function

begin_function
DECL|function|insert_ref_hash
specifier|static
name|void
name|insert_ref_hash
parameter_list|(
name|struct
name|object_refs
modifier|*
name|ref
parameter_list|,
name|struct
name|object_refs
modifier|*
modifier|*
name|hash
parameter_list|,
name|unsigned
name|int
name|size
parameter_list|)
block|{
name|int
name|j
init|=
name|hash_obj
argument_list|(
name|ref
operator|->
name|base
argument_list|,
name|size
argument_list|)
decl_stmt|;
while|while
condition|(
name|hash
index|[
name|j
index|]
condition|)
block|{
name|j
operator|++
expr_stmt|;
if|if
condition|(
name|j
operator|>=
name|size
condition|)
name|j
operator|=
literal|0
expr_stmt|;
block|}
name|hash
index|[
name|j
index|]
operator|=
name|ref
expr_stmt|;
block|}
end_function

begin_function
DECL|function|grow_refs_hash
specifier|static
name|void
name|grow_refs_hash
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|new_hash_size
init|=
operator|(
name|refs_hash_size
operator|+
literal|1000
operator|)
operator|*
literal|3
operator|/
literal|2
decl_stmt|;
name|struct
name|object_refs
modifier|*
modifier|*
name|new_hash
decl_stmt|;
name|new_hash
operator|=
name|xcalloc
argument_list|(
name|new_hash_size
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|object_refs
operator|*
argument_list|)
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
name|refs_hash_size
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|object_refs
modifier|*
name|ref
init|=
name|refs_hash
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|ref
condition|)
continue|continue;
name|insert_ref_hash
argument_list|(
name|ref
argument_list|,
name|new_hash
argument_list|,
name|new_hash_size
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|refs_hash
argument_list|)
expr_stmt|;
name|refs_hash
operator|=
name|new_hash
expr_stmt|;
name|refs_hash_size
operator|=
name|new_hash_size
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_object_refs
specifier|static
name|void
name|add_object_refs
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|struct
name|object_refs
modifier|*
name|ref
parameter_list|)
block|{
name|int
name|nr
init|=
name|nr_object_refs
operator|+
literal|1
decl_stmt|;
if|if
condition|(
name|nr
operator|>
name|refs_hash_size
operator|*
literal|2
operator|/
literal|3
condition|)
name|grow_refs_hash
argument_list|()
expr_stmt|;
name|ref
operator|->
name|base
operator|=
name|obj
expr_stmt|;
name|insert_ref_hash
argument_list|(
name|ref
argument_list|,
name|refs_hash
argument_list|,
name|refs_hash_size
argument_list|)
expr_stmt|;
name|nr_object_refs
operator|=
name|nr
expr_stmt|;
block|}
end_function

begin_function
DECL|function|lookup_object_refs
name|struct
name|object_refs
modifier|*
name|lookup_object_refs
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|)
block|{
name|int
name|j
init|=
name|hash_obj
argument_list|(
name|obj
argument_list|,
name|refs_hash_size
argument_list|)
decl_stmt|;
name|struct
name|object_refs
modifier|*
name|ref
decl_stmt|;
while|while
condition|(
operator|(
name|ref
operator|=
name|refs_hash
index|[
name|j
index|]
operator|)
operator|!=
name|NULL
condition|)
block|{
if|if
condition|(
name|ref
operator|->
name|base
operator|==
name|obj
condition|)
break|break;
name|j
operator|++
expr_stmt|;
if|if
condition|(
name|j
operator|>=
name|refs_hash_size
condition|)
name|j
operator|=
literal|0
expr_stmt|;
block|}
return|return
name|ref
return|;
block|}
end_function

begin_function
DECL|function|alloc_object_refs
name|struct
name|object_refs
modifier|*
name|alloc_object_refs
parameter_list|(
name|unsigned
name|count
parameter_list|)
block|{
name|struct
name|object_refs
modifier|*
name|refs
decl_stmt|;
name|size_t
name|size
init|=
sizeof|sizeof
argument_list|(
operator|*
name|refs
argument_list|)
operator|+
name|count
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|object
operator|*
argument_list|)
decl_stmt|;
name|refs
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|refs
operator|->
name|count
operator|=
name|count
expr_stmt|;
return|return
name|refs
return|;
block|}
end_function

begin_function
DECL|function|compare_object_pointers
specifier|static
name|int
name|compare_object_pointers
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
name|object
modifier|*
specifier|const
modifier|*
name|pa
init|=
name|a
decl_stmt|;
specifier|const
name|struct
name|object
modifier|*
specifier|const
modifier|*
name|pb
init|=
name|b
decl_stmt|;
if|if
condition|(
operator|*
name|pa
operator|==
operator|*
name|pb
condition|)
return|return
literal|0
return|;
elseif|else
if|if
condition|(
operator|*
name|pa
operator|<
operator|*
name|pb
condition|)
return|return
operator|-
literal|1
return|;
else|else
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|set_object_refs
name|void
name|set_object_refs
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|struct
name|object_refs
modifier|*
name|refs
parameter_list|)
block|{
name|unsigned
name|int
name|i
decl_stmt|,
name|j
decl_stmt|;
comment|/* Do not install empty list of references */
if|if
condition|(
name|refs
operator|->
name|count
operator|<
literal|1
condition|)
block|{
name|free
argument_list|(
name|refs
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* Sort the list and filter out duplicates */
name|qsort
argument_list|(
name|refs
operator|->
name|ref
argument_list|,
name|refs
operator|->
name|count
argument_list|,
sizeof|sizeof
argument_list|(
name|refs
operator|->
name|ref
index|[
literal|0
index|]
argument_list|)
argument_list|,
name|compare_object_pointers
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
name|j
operator|=
literal|1
init|;
name|i
operator|<
name|refs
operator|->
name|count
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|refs
operator|->
name|ref
index|[
name|i
index|]
operator|!=
name|refs
operator|->
name|ref
index|[
name|i
operator|-
literal|1
index|]
condition|)
name|refs
operator|->
name|ref
index|[
name|j
operator|++
index|]
operator|=
name|refs
operator|->
name|ref
index|[
name|i
index|]
expr_stmt|;
block|}
if|if
condition|(
name|j
operator|<
name|refs
operator|->
name|count
condition|)
block|{
comment|/* Duplicates were found - reallocate list */
name|size_t
name|size
init|=
sizeof|sizeof
argument_list|(
operator|*
name|refs
argument_list|)
operator|+
name|j
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|object
operator|*
argument_list|)
decl_stmt|;
name|refs
operator|->
name|count
operator|=
name|j
expr_stmt|;
name|refs
operator|=
name|xrealloc
argument_list|(
name|refs
argument_list|,
name|size
argument_list|)
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
name|refs
operator|->
name|count
condition|;
name|i
operator|++
control|)
name|refs
operator|->
name|ref
index|[
name|i
index|]
operator|->
name|used
operator|=
literal|1
expr_stmt|;
name|add_object_refs
argument_list|(
name|obj
argument_list|,
name|refs
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|mark_reachable
name|void
name|mark_reachable
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|unsigned
name|int
name|mask
parameter_list|)
block|{
specifier|const
name|struct
name|object_refs
modifier|*
name|refs
decl_stmt|;
if|if
condition|(
operator|!
name|track_object_refs
condition|)
name|die
argument_list|(
literal|"cannot do reachability with object refs turned off"
argument_list|)
expr_stmt|;
comment|/* nothing to lookup */
if|if
condition|(
operator|!
name|refs_hash_size
condition|)
return|return;
comment|/* If we've been here already, don't bother */
if|if
condition|(
name|obj
operator|->
name|flags
operator|&
name|mask
condition|)
return|return;
name|obj
operator|->
name|flags
operator||=
name|mask
expr_stmt|;
name|refs
operator|=
name|lookup_object_refs
argument_list|(
name|obj
argument_list|)
expr_stmt|;
if|if
condition|(
name|refs
condition|)
block|{
name|unsigned
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
name|refs
operator|->
name|count
condition|;
name|i
operator|++
control|)
name|mark_reachable
argument_list|(
name|refs
operator|->
name|ref
index|[
name|i
index|]
argument_list|,
name|mask
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit

