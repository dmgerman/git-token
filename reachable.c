begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"reachable.h"
end_include

begin_include
include|#
directive|include
file|"cache-tree.h"
end_include

begin_include
include|#
directive|include
file|"progress.h"
end_include

begin_include
include|#
directive|include
file|"list-objects.h"
end_include

begin_struct
DECL|struct|connectivity_progress
struct|struct
name|connectivity_progress
block|{
DECL|member|progress
name|struct
name|progress
modifier|*
name|progress
decl_stmt|;
DECL|member|count
name|unsigned
name|long
name|count
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|update_progress
specifier|static
name|void
name|update_progress
parameter_list|(
name|struct
name|connectivity_progress
modifier|*
name|cp
parameter_list|)
block|{
name|cp
operator|->
name|count
operator|++
expr_stmt|;
if|if
condition|(
operator|(
name|cp
operator|->
name|count
operator|&
literal|1023
operator|)
operator|==
literal|0
condition|)
name|display_progress
argument_list|(
name|cp
operator|->
name|progress
argument_list|,
name|cp
operator|->
name|count
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_one_ref
specifier|static
name|int
name|add_one_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|flag
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|object
init|=
name|parse_object_or_die
argument_list|(
name|sha1
argument_list|,
name|path
argument_list|)
decl_stmt|;
name|struct
name|rev_info
modifier|*
name|revs
init|=
operator|(
expr|struct
name|rev_info
operator|*
operator|)
name|cb_data
decl_stmt|;
name|add_pending_object
argument_list|(
name|revs
argument_list|,
name|object
argument_list|,
literal|""
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|add_one_tree
specifier|static
name|void
name|add_one_tree
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
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
if|if
condition|(
name|tree
condition|)
name|add_pending_object
argument_list|(
name|revs
argument_list|,
operator|&
name|tree
operator|->
name|object
argument_list|,
literal|""
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_cache_tree
specifier|static
name|void
name|add_cache_tree
parameter_list|(
name|struct
name|cache_tree
modifier|*
name|it
parameter_list|,
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
name|it
operator|->
name|entry_count
operator|>=
literal|0
condition|)
name|add_one_tree
argument_list|(
name|it
operator|->
name|sha1
argument_list|,
name|revs
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
name|it
operator|->
name|subtree_nr
condition|;
name|i
operator|++
control|)
name|add_cache_tree
argument_list|(
name|it
operator|->
name|down
index|[
name|i
index|]
operator|->
name|cache_tree
argument_list|,
name|revs
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_cache_refs
specifier|static
name|void
name|add_cache_refs
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|read_cache
argument_list|()
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|active_nr
condition|;
name|i
operator|++
control|)
block|{
comment|/* 		 * The index can contain blobs and GITLINKs, GITLINKs are hashes 		 * that don't actually point to objects in the repository, it's 		 * almost guaranteed that they are NOT blobs, so we don't call 		 * lookup_blob() on them, to avoid populating the hash table 		 * with invalid information 		 */
if|if
condition|(
name|S_ISGITLINK
argument_list|(
name|active_cache
index|[
name|i
index|]
operator|->
name|ce_mode
argument_list|)
condition|)
continue|continue;
name|lookup_blob
argument_list|(
name|active_cache
index|[
name|i
index|]
operator|->
name|sha1
argument_list|)
expr_stmt|;
comment|/* 		 * We could add the blobs to the pending list, but quite 		 * frankly, we don't care. Once we've looked them up, and 		 * added them as objects, we've really done everything 		 * there is to do for a blob 		 */
block|}
if|if
condition|(
name|active_cache_tree
condition|)
name|add_cache_tree
argument_list|(
name|active_cache_tree
argument_list|,
name|revs
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * The traversal will have already marked us as SEEN, so we  * only need to handle any progress reporting here.  */
end_comment

begin_function
DECL|function|mark_object
specifier|static
name|void
name|mark_object
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
specifier|const
name|struct
name|name_path
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|update_progress
argument_list|(
name|data
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|mark_commit
specifier|static
name|void
name|mark_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|c
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|mark_object
argument_list|(
operator|&
name|c
operator|->
name|object
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|,
name|data
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|mark_reachable_objects
name|void
name|mark_reachable_objects
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|int
name|mark_reflog
parameter_list|,
name|struct
name|progress
modifier|*
name|progress
parameter_list|)
block|{
name|struct
name|connectivity_progress
name|cp
decl_stmt|;
comment|/* 	 * Set up revision parsing, and mark us as being interested 	 * in all object types, not just commits. 	 */
name|revs
operator|->
name|tag_objects
operator|=
literal|1
expr_stmt|;
name|revs
operator|->
name|blob_objects
operator|=
literal|1
expr_stmt|;
name|revs
operator|->
name|tree_objects
operator|=
literal|1
expr_stmt|;
comment|/* Add all refs from the index file */
name|add_cache_refs
argument_list|(
name|revs
argument_list|)
expr_stmt|;
comment|/* Add all external refs */
name|for_each_ref
argument_list|(
name|add_one_ref
argument_list|,
name|revs
argument_list|)
expr_stmt|;
comment|/* detached HEAD is not included in the list above */
name|head_ref
argument_list|(
name|add_one_ref
argument_list|,
name|revs
argument_list|)
expr_stmt|;
comment|/* Add all reflog info */
if|if
condition|(
name|mark_reflog
condition|)
name|add_reflogs_to_pending
argument_list|(
name|revs
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|cp
operator|.
name|progress
operator|=
name|progress
expr_stmt|;
name|cp
operator|.
name|count
operator|=
literal|0
expr_stmt|;
comment|/* 	 * Set up the revision walk - this will move all commits 	 * from the pending list to the commit walking list. 	 */
if|if
condition|(
name|prepare_revision_walk
argument_list|(
name|revs
argument_list|)
condition|)
name|die
argument_list|(
literal|"revision walk setup failed"
argument_list|)
expr_stmt|;
name|traverse_commit_list
argument_list|(
name|revs
argument_list|,
name|mark_commit
argument_list|,
name|mark_object
argument_list|,
operator|&
name|cp
argument_list|)
expr_stmt|;
name|display_progress
argument_list|(
name|cp
operator|.
name|progress
argument_list|,
name|cp
operator|.
name|count
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

