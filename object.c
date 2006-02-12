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
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_decl_stmt
DECL|variable|objs
name|struct
name|object
modifier|*
modifier|*
name|objs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nr_objs
specifier|static
name|int
name|nr_objs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|obj_allocs
name|int
name|obj_allocs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|track_object_refs
name|int
name|track_object_refs
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|hashtable_index
specifier|static
name|int
name|hashtable_index
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|unsigned
name|int
name|i
decl_stmt|;
name|memcpy
argument_list|(
operator|&
name|i
argument_list|,
name|sha1
argument_list|,
expr|sizeof
operator|(
name|unsigned
name|int
operator|)
argument_list|)
expr_stmt|;
return|return
call|(
name|int
call|)
argument_list|(
name|i
operator|%
name|obj_allocs
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|find_object
specifier|static
name|int
name|find_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
operator|!
name|objs
condition|)
return|return
operator|-
literal|1
return|;
name|i
operator|=
name|hashtable_index
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
while|while
condition|(
name|objs
index|[
name|i
index|]
condition|)
block|{
if|if
condition|(
name|memcmp
argument_list|(
name|sha1
argument_list|,
name|objs
index|[
name|i
index|]
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
operator|==
literal|0
condition|)
return|return
name|i
return|;
name|i
operator|++
expr_stmt|;
if|if
condition|(
name|i
operator|==
name|obj_allocs
condition|)
name|i
operator|=
literal|0
expr_stmt|;
block|}
return|return
operator|-
literal|1
operator|-
name|i
return|;
block|}
end_function

begin_function
DECL|function|lookup_object
name|struct
name|object
modifier|*
name|lookup_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|pos
init|=
name|find_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
name|pos
operator|>=
literal|0
condition|)
return|return
name|objs
index|[
name|pos
index|]
return|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|created_object
name|void
name|created_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|struct
name|object
modifier|*
name|obj
parameter_list|)
block|{
name|int
name|pos
decl_stmt|;
name|obj
operator|->
name|parsed
operator|=
literal|0
expr_stmt|;
name|memcpy
argument_list|(
name|obj
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|obj
operator|->
name|type
operator|=
name|NULL
expr_stmt|;
name|obj
operator|->
name|refs
operator|=
name|NULL
expr_stmt|;
name|obj
operator|->
name|used
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|obj_allocs
operator|-
literal|1
operator|<=
name|nr_objs
operator|*
literal|2
condition|)
block|{
name|int
name|i
decl_stmt|,
name|count
init|=
name|obj_allocs
decl_stmt|;
name|obj_allocs
operator|=
operator|(
name|obj_allocs
operator|<
literal|32
condition|?
literal|32
else|:
literal|2
operator|*
name|obj_allocs
operator|)
expr_stmt|;
name|objs
operator|=
name|xrealloc
argument_list|(
name|objs
argument_list|,
name|obj_allocs
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|object
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|objs
operator|+
name|count
argument_list|,
literal|0
argument_list|,
operator|(
name|obj_allocs
operator|-
name|count
operator|)
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|object
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
name|obj_allocs
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|objs
index|[
name|i
index|]
condition|)
block|{
name|int
name|j
init|=
name|find_object
argument_list|(
name|objs
index|[
name|i
index|]
operator|->
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
name|j
operator|!=
name|i
condition|)
block|{
name|j
operator|=
operator|-
literal|1
operator|-
name|j
expr_stmt|;
name|objs
index|[
name|j
index|]
operator|=
name|objs
index|[
name|i
index|]
expr_stmt|;
name|objs
index|[
name|i
index|]
operator|=
name|NULL
expr_stmt|;
block|}
block|}
block|}
name|pos
operator|=
name|find_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|pos
operator|>=
literal|0
condition|)
name|die
argument_list|(
literal|"Inserting %s twice\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|pos
operator|=
operator|-
name|pos
operator|-
literal|1
expr_stmt|;
name|objs
index|[
name|pos
index|]
operator|=
name|obj
expr_stmt|;
name|nr_objs
operator|++
expr_stmt|;
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
name|xmalloc
argument_list|(
name|size
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|refs
argument_list|,
literal|0
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
name|obj
operator|->
name|refs
operator|=
name|refs
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
if|if
condition|(
name|obj
operator|->
name|refs
condition|)
block|{
specifier|const
name|struct
name|object_refs
modifier|*
name|refs
init|=
name|obj
operator|->
name|refs
decl_stmt|;
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

begin_function
DECL|function|lookup_object_type
name|struct
name|object
modifier|*
name|lookup_object_type
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|type
parameter_list|)
block|{
if|if
condition|(
operator|!
name|type
condition|)
block|{
return|return
name|lookup_unknown_object
argument_list|(
name|sha1
argument_list|)
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
name|blob_type
argument_list|)
condition|)
block|{
return|return
operator|&
name|lookup_blob
argument_list|(
name|sha1
argument_list|)
operator|->
name|object
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
name|tree_type
argument_list|)
condition|)
block|{
return|return
operator|&
name|lookup_tree
argument_list|(
name|sha1
argument_list|)
operator|->
name|object
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
name|commit_type
argument_list|)
condition|)
block|{
return|return
operator|&
name|lookup_commit
argument_list|(
name|sha1
argument_list|)
operator|->
name|object
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
name|tag_type
argument_list|)
condition|)
block|{
return|return
operator|&
name|lookup_tag
argument_list|(
name|sha1
argument_list|)
operator|->
name|object
return|;
block|}
else|else
block|{
name|error
argument_list|(
literal|"Unknown type %s"
argument_list|,
name|type
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
block|}
end_function

begin_union
DECL|union|any_object
union|union
name|any_object
block|{
DECL|member|object
name|struct
name|object
name|object
decl_stmt|;
DECL|member|commit
name|struct
name|commit
name|commit
decl_stmt|;
DECL|member|tree
name|struct
name|tree
name|tree
decl_stmt|;
DECL|member|blob
name|struct
name|blob
name|blob
decl_stmt|;
DECL|member|tag
name|struct
name|tag
name|tag
decl_stmt|;
block|}
union|;
end_union

begin_function
DECL|function|lookup_unknown_object
name|struct
name|object
modifier|*
name|lookup_unknown_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
init|=
name|lookup_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|obj
condition|)
block|{
name|union
name|any_object
modifier|*
name|ret
init|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|ret
argument_list|)
argument_list|)
decl_stmt|;
name|memset
argument_list|(
name|ret
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|ret
argument_list|)
argument_list|)
expr_stmt|;
name|created_object
argument_list|(
name|sha1
argument_list|,
operator|&
name|ret
operator|->
name|object
argument_list|)
expr_stmt|;
name|ret
operator|->
name|object
operator|.
name|type
operator|=
name|NULL
expr_stmt|;
return|return
operator|&
name|ret
operator|->
name|object
return|;
block|}
return|return
name|obj
return|;
block|}
end_function

begin_function
DECL|function|parse_object
name|struct
name|object
modifier|*
name|parse_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|unsigned
name|long
name|size
decl_stmt|;
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|void
modifier|*
name|buffer
init|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
name|buffer
condition|)
block|{
name|struct
name|object
modifier|*
name|obj
decl_stmt|;
if|if
condition|(
name|check_sha1_signature
argument_list|(
name|sha1
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|,
name|type
argument_list|)
operator|<
literal|0
condition|)
name|printf
argument_list|(
literal|"sha1 mismatch %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"blob"
argument_list|)
condition|)
block|{
name|struct
name|blob
modifier|*
name|blob
init|=
name|lookup_blob
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|parse_blob_buffer
argument_list|(
name|blob
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|obj
operator|=
operator|&
name|blob
operator|->
name|object
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tree"
argument_list|)
condition|)
block|{
name|struct
name|tree
modifier|*
name|tree
init|=
name|lookup_tree
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|parse_tree_buffer
argument_list|(
name|tree
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|obj
operator|=
operator|&
name|tree
operator|->
name|object
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"commit"
argument_list|)
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|lookup_commit
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|parse_commit_buffer
argument_list|(
name|commit
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
operator|->
name|buffer
condition|)
block|{
name|commit
operator|->
name|buffer
operator|=
name|buffer
expr_stmt|;
name|buffer
operator|=
name|NULL
expr_stmt|;
block|}
name|obj
operator|=
operator|&
name|commit
operator|->
name|object
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tag"
argument_list|)
condition|)
block|{
name|struct
name|tag
modifier|*
name|tag
init|=
name|lookup_tag
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|parse_tag_buffer
argument_list|(
name|tag
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|obj
operator|=
operator|&
name|tag
operator|->
name|object
expr_stmt|;
block|}
else|else
block|{
name|obj
operator|=
name|NULL
expr_stmt|;
block|}
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
return|return
name|obj
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|object_list_insert
name|struct
name|object_list
modifier|*
name|object_list_insert
parameter_list|(
name|struct
name|object
modifier|*
name|item
parameter_list|,
name|struct
name|object_list
modifier|*
modifier|*
name|list_p
parameter_list|)
block|{
name|struct
name|object_list
modifier|*
name|new_list
init|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|object_list
argument_list|)
argument_list|)
decl_stmt|;
name|new_list
operator|->
name|item
operator|=
name|item
expr_stmt|;
name|new_list
operator|->
name|next
operator|=
operator|*
name|list_p
expr_stmt|;
operator|*
name|list_p
operator|=
name|new_list
expr_stmt|;
return|return
name|new_list
return|;
block|}
end_function

begin_function
DECL|function|object_list_append
name|void
name|object_list_append
parameter_list|(
name|struct
name|object
modifier|*
name|item
parameter_list|,
name|struct
name|object_list
modifier|*
modifier|*
name|list_p
parameter_list|)
block|{
while|while
condition|(
operator|*
name|list_p
condition|)
block|{
name|list_p
operator|=
operator|&
operator|(
operator|(
operator|*
name|list_p
operator|)
operator|->
name|next
operator|)
expr_stmt|;
block|}
operator|*
name|list_p
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|object_list
argument_list|)
argument_list|)
expr_stmt|;
operator|(
operator|*
name|list_p
operator|)
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
operator|(
operator|*
name|list_p
operator|)
operator|->
name|item
operator|=
name|item
expr_stmt|;
block|}
end_function

begin_function
DECL|function|object_list_length
name|unsigned
name|object_list_length
parameter_list|(
name|struct
name|object_list
modifier|*
name|list
parameter_list|)
block|{
name|unsigned
name|ret
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|list
condition|)
block|{
name|list
operator|=
name|list
operator|->
name|next
expr_stmt|;
name|ret
operator|++
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|object_list_contains
name|int
name|object_list_contains
parameter_list|(
name|struct
name|object_list
modifier|*
name|list
parameter_list|,
name|struct
name|object
modifier|*
name|obj
parameter_list|)
block|{
while|while
condition|(
name|list
condition|)
block|{
if|if
condition|(
name|list
operator|->
name|item
operator|==
name|obj
condition|)
return|return
literal|1
return|;
name|list
operator|=
name|list
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

