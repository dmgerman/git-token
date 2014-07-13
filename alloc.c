begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * alloc.c  - specialized allocator for internal objects  *  * Copyright (C) 2006 Linus Torvalds  *  * The standard malloc/free wastes too much space for objects, partly because  * it maintains all the allocation infrastructure (which isn't needed, since  * we never free an object descriptor anyway), but even more because it ends  * up with maximal alignment because it doesn't know what the object alignment  * for the new allocation is.  */
end_comment

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

begin_define
DECL|macro|BLOCKING
define|#
directive|define
name|BLOCKING
value|1024
end_define

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
DECL|member|blob
name|struct
name|blob
name|blob
decl_stmt|;
DECL|member|tree
name|struct
name|tree
name|tree
decl_stmt|;
DECL|member|commit
name|struct
name|commit
name|commit
decl_stmt|;
DECL|member|tag
name|struct
name|tag
name|tag
decl_stmt|;
block|}
union|;
end_union

begin_struct
DECL|struct|alloc_state
struct|struct
name|alloc_state
block|{
DECL|member|count
name|int
name|count
decl_stmt|;
comment|/* total number of nodes allocated */
DECL|member|nr
name|int
name|nr
decl_stmt|;
comment|/* number of nodes left in current allocation */
DECL|member|p
name|void
modifier|*
name|p
decl_stmt|;
comment|/* first free node in current allocation */
block|}
struct|;
end_struct

begin_function
DECL|function|alloc_node
specifier|static
specifier|inline
name|void
modifier|*
name|alloc_node
parameter_list|(
name|struct
name|alloc_state
modifier|*
name|s
parameter_list|,
name|size_t
name|node_size
parameter_list|)
block|{
name|void
modifier|*
name|ret
decl_stmt|;
if|if
condition|(
operator|!
name|s
operator|->
name|nr
condition|)
block|{
name|s
operator|->
name|nr
operator|=
name|BLOCKING
expr_stmt|;
name|s
operator|->
name|p
operator|=
name|xmalloc
argument_list|(
name|BLOCKING
operator|*
name|node_size
argument_list|)
expr_stmt|;
block|}
name|s
operator|->
name|nr
operator|--
expr_stmt|;
name|s
operator|->
name|count
operator|++
expr_stmt|;
name|ret
operator|=
name|s
operator|->
name|p
expr_stmt|;
name|s
operator|->
name|p
operator|=
operator|(
name|char
operator|*
operator|)
name|s
operator|->
name|p
operator|+
name|node_size
expr_stmt|;
name|memset
argument_list|(
name|ret
argument_list|,
literal|0
argument_list|,
name|node_size
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|blob_state
specifier|static
name|struct
name|alloc_state
name|blob_state
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|alloc_blob_node
name|void
modifier|*
name|alloc_blob_node
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|blob
modifier|*
name|b
init|=
name|alloc_node
argument_list|(
operator|&
name|blob_state
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|blob
argument_list|)
argument_list|)
decl_stmt|;
return|return
name|b
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|tree_state
specifier|static
name|struct
name|alloc_state
name|tree_state
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|alloc_tree_node
name|void
modifier|*
name|alloc_tree_node
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|tree
modifier|*
name|t
init|=
name|alloc_node
argument_list|(
operator|&
name|tree_state
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|tree
argument_list|)
argument_list|)
decl_stmt|;
return|return
name|t
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|tag_state
specifier|static
name|struct
name|alloc_state
name|tag_state
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|alloc_tag_node
name|void
modifier|*
name|alloc_tag_node
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|tag
modifier|*
name|t
init|=
name|alloc_node
argument_list|(
operator|&
name|tag_state
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|tag
argument_list|)
argument_list|)
decl_stmt|;
return|return
name|t
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|object_state
specifier|static
name|struct
name|alloc_state
name|object_state
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|alloc_object_node
name|void
modifier|*
name|alloc_object_node
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
init|=
name|alloc_node
argument_list|(
operator|&
name|object_state
argument_list|,
sizeof|sizeof
argument_list|(
expr|union
name|any_object
argument_list|)
argument_list|)
decl_stmt|;
return|return
name|obj
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|commit_state
specifier|static
name|struct
name|alloc_state
name|commit_state
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|alloc_commit_node
name|void
modifier|*
name|alloc_commit_node
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|int
name|commit_count
decl_stmt|;
name|struct
name|commit
modifier|*
name|c
init|=
name|alloc_node
argument_list|(
operator|&
name|commit_state
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|commit
argument_list|)
argument_list|)
decl_stmt|;
name|c
operator|->
name|index
operator|=
name|commit_count
operator|++
expr_stmt|;
return|return
name|c
return|;
block|}
end_function

begin_function
DECL|function|report
specifier|static
name|void
name|report
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|int
name|count
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%10s: %8u (%"
name|PRIuMAX
literal|" kB)\n"
argument_list|,
name|name
argument_list|,
name|count
argument_list|,
operator|(
name|uintmax_t
operator|)
name|size
argument_list|)
expr_stmt|;
block|}
end_function

begin_define
DECL|macro|REPORT
define|#
directive|define
name|REPORT
parameter_list|(
name|name
parameter_list|,
name|type
parameter_list|)
define|\
value|report(#name, name##_state.count, name##_state.count * sizeof(type)>> 10)
end_define

begin_function
DECL|function|alloc_report
name|void
name|alloc_report
parameter_list|(
name|void
parameter_list|)
block|{
name|REPORT
argument_list|(
name|blob
argument_list|,
expr|struct
name|blob
argument_list|)
expr_stmt|;
name|REPORT
argument_list|(
name|tree
argument_list|,
expr|struct
name|tree
argument_list|)
expr_stmt|;
name|REPORT
argument_list|(
name|commit
argument_list|,
expr|struct
name|commit
argument_list|)
expr_stmt|;
name|REPORT
argument_list|(
name|tag
argument_list|,
expr|struct
name|tag
argument_list|)
expr_stmt|;
name|REPORT
argument_list|(
name|object
argument_list|,
expr|union
name|any_object
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

