begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"sha1-lookup.h"
end_include

begin_include
include|#
directive|include
file|"patch-ids.h"
end_include

begin_function
DECL|function|commit_patch_id
specifier|static
name|int
name|commit_patch_id
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|struct
name|diff_options
modifier|*
name|options
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
if|if
condition|(
name|commit
operator|->
name|parents
condition|)
name|diff_tree_sha1
argument_list|(
name|commit
operator|->
name|parents
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|,
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|,
literal|""
argument_list|,
name|options
argument_list|)
expr_stmt|;
else|else
name|diff_root_tree_sha1
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|,
literal|""
argument_list|,
name|options
argument_list|)
expr_stmt|;
name|diffcore_std
argument_list|(
name|options
argument_list|)
expr_stmt|;
return|return
name|diff_flush_patch_id
argument_list|(
name|options
argument_list|,
name|sha1
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|patch_id_access
specifier|static
specifier|const
name|unsigned
name|char
modifier|*
name|patch_id_access
parameter_list|(
name|size_t
name|index
parameter_list|,
name|void
modifier|*
name|table
parameter_list|)
block|{
name|struct
name|patch_id
modifier|*
modifier|*
name|id_table
init|=
name|table
decl_stmt|;
return|return
name|id_table
index|[
name|index
index|]
operator|->
name|patch_id
return|;
block|}
end_function

begin_function
DECL|function|patch_pos
specifier|static
name|int
name|patch_pos
parameter_list|(
name|struct
name|patch_id
modifier|*
modifier|*
name|table
parameter_list|,
name|int
name|nr
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|id
parameter_list|)
block|{
return|return
name|sha1_pos
argument_list|(
name|id
argument_list|,
name|table
argument_list|,
name|nr
argument_list|,
name|patch_id_access
argument_list|)
return|;
block|}
end_function

begin_define
DECL|macro|BUCKET_SIZE
define|#
directive|define
name|BUCKET_SIZE
value|190
end_define

begin_comment
DECL|macro|BUCKET_SIZE
comment|/* 190 * 21 = 3990, with slop close enough to 4K */
end_comment

begin_struct
DECL|struct|patch_id_bucket
struct|struct
name|patch_id_bucket
block|{
DECL|member|next
name|struct
name|patch_id_bucket
modifier|*
name|next
decl_stmt|;
DECL|member|nr
name|int
name|nr
decl_stmt|;
DECL|member|bucket
name|struct
name|patch_id
name|bucket
index|[
name|BUCKET_SIZE
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|init_patch_ids
name|int
name|init_patch_ids
parameter_list|(
name|struct
name|patch_ids
modifier|*
name|ids
parameter_list|)
block|{
name|memset
argument_list|(
name|ids
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|ids
argument_list|)
argument_list|)
expr_stmt|;
name|diff_setup
argument_list|(
operator|&
name|ids
operator|->
name|diffopts
argument_list|)
expr_stmt|;
name|DIFF_OPT_SET
argument_list|(
operator|&
name|ids
operator|->
name|diffopts
argument_list|,
name|RECURSIVE
argument_list|)
expr_stmt|;
name|diff_setup_done
argument_list|(
operator|&
name|ids
operator|->
name|diffopts
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|free_patch_ids
name|int
name|free_patch_ids
parameter_list|(
name|struct
name|patch_ids
modifier|*
name|ids
parameter_list|)
block|{
name|struct
name|patch_id_bucket
modifier|*
name|next
decl_stmt|,
modifier|*
name|patches
decl_stmt|;
name|free
argument_list|(
name|ids
operator|->
name|table
argument_list|)
expr_stmt|;
for|for
control|(
name|patches
operator|=
name|ids
operator|->
name|patches
init|;
name|patches
condition|;
name|patches
operator|=
name|next
control|)
block|{
name|next
operator|=
name|patches
operator|->
name|next
expr_stmt|;
name|free
argument_list|(
name|patches
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|add_commit
specifier|static
name|struct
name|patch_id
modifier|*
name|add_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|struct
name|patch_ids
modifier|*
name|ids
parameter_list|,
name|int
name|no_add
parameter_list|)
block|{
name|struct
name|patch_id_bucket
modifier|*
name|bucket
decl_stmt|;
name|struct
name|patch_id
modifier|*
name|ent
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|pos
decl_stmt|;
if|if
condition|(
name|commit_patch_id
argument_list|(
name|commit
argument_list|,
operator|&
name|ids
operator|->
name|diffopts
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
name|NULL
return|;
name|pos
operator|=
name|patch_pos
argument_list|(
name|ids
operator|->
name|table
argument_list|,
name|ids
operator|->
name|nr
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|pos
condition|)
return|return
name|ids
operator|->
name|table
index|[
name|pos
index|]
return|;
if|if
condition|(
name|no_add
condition|)
return|return
name|NULL
return|;
name|pos
operator|=
operator|-
literal|1
operator|-
name|pos
expr_stmt|;
name|bucket
operator|=
name|ids
operator|->
name|patches
expr_stmt|;
if|if
condition|(
operator|!
name|bucket
operator|||
operator|(
name|BUCKET_SIZE
operator|<=
name|bucket
operator|->
name|nr
operator|)
condition|)
block|{
name|bucket
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|bucket
argument_list|)
argument_list|)
expr_stmt|;
name|bucket
operator|->
name|next
operator|=
name|ids
operator|->
name|patches
expr_stmt|;
name|ids
operator|->
name|patches
operator|=
name|bucket
expr_stmt|;
block|}
name|ent
operator|=
operator|&
name|bucket
operator|->
name|bucket
index|[
name|bucket
operator|->
name|nr
operator|++
index|]
expr_stmt|;
name|hashcpy
argument_list|(
name|ent
operator|->
name|patch_id
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|ids
operator|->
name|table
argument_list|,
name|ids
operator|->
name|nr
operator|+
literal|1
argument_list|,
name|ids
operator|->
name|alloc
argument_list|)
expr_stmt|;
if|if
condition|(
name|pos
operator|<
name|ids
operator|->
name|nr
condition|)
name|memmove
argument_list|(
name|ids
operator|->
name|table
operator|+
name|pos
operator|+
literal|1
argument_list|,
name|ids
operator|->
name|table
operator|+
name|pos
argument_list|,
sizeof|sizeof
argument_list|(
name|ent
argument_list|)
operator|*
operator|(
name|ids
operator|->
name|nr
operator|-
name|pos
operator|)
argument_list|)
expr_stmt|;
name|ids
operator|->
name|nr
operator|++
expr_stmt|;
name|ids
operator|->
name|table
index|[
name|pos
index|]
operator|=
name|ent
expr_stmt|;
return|return
name|ids
operator|->
name|table
index|[
name|pos
index|]
return|;
block|}
end_function

begin_function
DECL|function|has_commit_patch_id
name|struct
name|patch_id
modifier|*
name|has_commit_patch_id
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|struct
name|patch_ids
modifier|*
name|ids
parameter_list|)
block|{
return|return
name|add_commit
argument_list|(
name|commit
argument_list|,
name|ids
argument_list|,
literal|1
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|add_commit_patch_id
name|struct
name|patch_id
modifier|*
name|add_commit_patch_id
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|struct
name|patch_ids
modifier|*
name|ids
parameter_list|)
block|{
return|return
name|add_commit
argument_list|(
name|commit
argument_list|,
name|ids
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

end_unit

