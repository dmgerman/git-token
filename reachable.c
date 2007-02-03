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

begin_function
DECL|function|process_blob
specifier|static
name|void
name|process_blob
parameter_list|(
name|struct
name|blob
modifier|*
name|blob
parameter_list|,
name|struct
name|object_array
modifier|*
name|p
parameter_list|,
name|struct
name|name_path
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
init|=
operator|&
name|blob
operator|->
name|object
decl_stmt|;
if|if
condition|(
name|obj
operator|->
name|flags
operator|&
name|SEEN
condition|)
return|return;
name|obj
operator|->
name|flags
operator||=
name|SEEN
expr_stmt|;
comment|/* Nothing to do, really .. The blob lookup was the important part */
block|}
end_function

begin_function
DECL|function|process_tree
specifier|static
name|void
name|process_tree
parameter_list|(
name|struct
name|tree
modifier|*
name|tree
parameter_list|,
name|struct
name|object_array
modifier|*
name|p
parameter_list|,
name|struct
name|name_path
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
init|=
operator|&
name|tree
operator|->
name|object
decl_stmt|;
name|struct
name|tree_desc
name|desc
decl_stmt|;
name|struct
name|name_entry
name|entry
decl_stmt|;
name|struct
name|name_path
name|me
decl_stmt|;
if|if
condition|(
name|obj
operator|->
name|flags
operator|&
name|SEEN
condition|)
return|return;
name|obj
operator|->
name|flags
operator||=
name|SEEN
expr_stmt|;
if|if
condition|(
name|parse_tree
argument_list|(
name|tree
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"bad tree object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|name
operator|=
name|xstrdup
argument_list|(
name|name
argument_list|)
expr_stmt|;
name|add_object
argument_list|(
name|obj
argument_list|,
name|p
argument_list|,
name|path
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|me
operator|.
name|up
operator|=
name|path
expr_stmt|;
name|me
operator|.
name|elem
operator|=
name|name
expr_stmt|;
name|me
operator|.
name|elem_len
operator|=
name|strlen
argument_list|(
name|name
argument_list|)
expr_stmt|;
name|desc
operator|.
name|buf
operator|=
name|tree
operator|->
name|buffer
expr_stmt|;
name|desc
operator|.
name|size
operator|=
name|tree
operator|->
name|size
expr_stmt|;
while|while
condition|(
name|tree_entry
argument_list|(
operator|&
name|desc
argument_list|,
operator|&
name|entry
argument_list|)
condition|)
block|{
if|if
condition|(
name|S_ISDIR
argument_list|(
name|entry
operator|.
name|mode
argument_list|)
condition|)
name|process_tree
argument_list|(
name|lookup_tree
argument_list|(
name|entry
operator|.
name|sha1
argument_list|)
argument_list|,
name|p
argument_list|,
operator|&
name|me
argument_list|,
name|entry
operator|.
name|path
argument_list|)
expr_stmt|;
else|else
name|process_blob
argument_list|(
name|lookup_blob
argument_list|(
name|entry
operator|.
name|sha1
argument_list|)
argument_list|,
name|p
argument_list|,
operator|&
name|me
argument_list|,
name|entry
operator|.
name|path
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|tree
operator|->
name|buffer
argument_list|)
expr_stmt|;
name|tree
operator|->
name|buffer
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function
DECL|function|process_tag
specifier|static
name|void
name|process_tag
parameter_list|(
name|struct
name|tag
modifier|*
name|tag
parameter_list|,
name|struct
name|object_array
modifier|*
name|p
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
init|=
operator|&
name|tag
operator|->
name|object
decl_stmt|;
name|struct
name|name_path
name|me
decl_stmt|;
if|if
condition|(
name|obj
operator|->
name|flags
operator|&
name|SEEN
condition|)
return|return;
name|obj
operator|->
name|flags
operator||=
name|SEEN
expr_stmt|;
name|me
operator|.
name|up
operator|=
name|NULL
expr_stmt|;
name|me
operator|.
name|elem
operator|=
literal|"tag:/"
expr_stmt|;
name|me
operator|.
name|elem_len
operator|=
literal|5
expr_stmt|;
if|if
condition|(
name|parse_tag
argument_list|(
name|tag
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"bad tag object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|add_object
argument_list|(
name|tag
operator|->
name|tagged
argument_list|,
name|p
argument_list|,
name|NULL
argument_list|,
name|name
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|walk_commit_list
specifier|static
name|void
name|walk_commit_list
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
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|struct
name|object_array
name|objects
init|=
block|{
literal|0
block|,
literal|0
block|,
name|NULL
block|}
decl_stmt|;
comment|/* Walk all commits, process their trees */
while|while
condition|(
operator|(
name|commit
operator|=
name|get_revision
argument_list|(
name|revs
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
name|process_tree
argument_list|(
name|commit
operator|->
name|tree
argument_list|,
operator|&
name|objects
argument_list|,
name|NULL
argument_list|,
literal|""
argument_list|)
expr_stmt|;
comment|/* Then walk all the pending objects, recursively processing them too */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|revs
operator|->
name|pending
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|object_array_entry
modifier|*
name|pending
init|=
name|revs
operator|->
name|pending
operator|.
name|objects
operator|+
name|i
decl_stmt|;
name|struct
name|object
modifier|*
name|obj
init|=
name|pending
operator|->
name|item
decl_stmt|;
specifier|const
name|char
modifier|*
name|name
init|=
name|pending
operator|->
name|name
decl_stmt|;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_TAG
condition|)
block|{
name|process_tag
argument_list|(
operator|(
expr|struct
name|tag
operator|*
operator|)
name|obj
argument_list|,
operator|&
name|objects
argument_list|,
name|name
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_TREE
condition|)
block|{
name|process_tree
argument_list|(
operator|(
expr|struct
name|tree
operator|*
operator|)
name|obj
argument_list|,
operator|&
name|objects
argument_list|,
name|NULL
argument_list|,
name|name
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_BLOB
condition|)
block|{
name|process_blob
argument_list|(
operator|(
expr|struct
name|blob
operator|*
operator|)
name|obj
argument_list|,
operator|&
name|objects
argument_list|,
name|NULL
argument_list|,
name|name
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|die
argument_list|(
literal|"unknown pending object %s (%s)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|,
name|name
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|add_one_reflog_ent
specifier|static
name|int
name|add_one_reflog_ent
parameter_list|(
name|unsigned
name|char
modifier|*
name|osha1
parameter_list|,
name|unsigned
name|char
modifier|*
name|nsha1
parameter_list|,
specifier|const
name|char
modifier|*
name|email
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|,
name|int
name|tz
parameter_list|,
specifier|const
name|char
modifier|*
name|message
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
name|object
operator|=
name|parse_object
argument_list|(
name|osha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|object
condition|)
name|add_pending_object
argument_list|(
name|revs
argument_list|,
name|object
argument_list|,
literal|""
argument_list|)
expr_stmt|;
name|object
operator|=
name|parse_object
argument_list|(
name|nsha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|object
condition|)
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
name|parse_object
argument_list|(
name|sha1
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
if|if
condition|(
operator|!
name|object
condition|)
name|die
argument_list|(
literal|"bad object ref: %s:%s"
argument_list|,
name|path
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
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
DECL|function|add_one_reflog
specifier|static
name|int
name|add_one_reflog
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
name|for_each_reflog_ent
argument_list|(
name|path
argument_list|,
name|add_one_reflog_ent
argument_list|,
name|cb_data
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
parameter_list|)
block|{
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
comment|/* Add all reflog info */
if|if
condition|(
name|mark_reflog
condition|)
name|for_each_reflog
argument_list|(
name|add_one_reflog
argument_list|,
name|revs
argument_list|)
expr_stmt|;
comment|/* 	 * Set up the revision walk - this will move all commits 	 * from the pending list to the commit walking list. 	 */
name|prepare_revision_walk
argument_list|(
name|revs
argument_list|)
expr_stmt|;
name|walk_commit_list
argument_list|(
name|revs
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

