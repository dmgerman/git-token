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

begin_struct
DECL|struct|recent_data
struct|struct
name|recent_data
block|{
DECL|member|revs
name|struct
name|rev_info
modifier|*
name|revs
decl_stmt|;
DECL|member|timestamp
name|unsigned
name|long
name|timestamp
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|add_recent_object
specifier|static
name|void
name|add_recent_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
name|long
name|mtime
parameter_list|,
name|struct
name|recent_data
modifier|*
name|data
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|obj
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
if|if
condition|(
name|mtime
operator|<=
name|data
operator|->
name|timestamp
condition|)
return|return;
comment|/* 	 * We do not want to call parse_object here, because 	 * inflating blobs and trees could be very expensive. 	 * However, we do need to know the correct type for 	 * later processing, and the revision machinery expects 	 * commits and tags to have been parsed. 	 */
name|type
operator|=
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to get object info for %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|type
condition|)
block|{
case|case
name|OBJ_TAG
case|:
case|case
name|OBJ_COMMIT
case|:
name|obj
operator|=
name|parse_object_or_die
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
break|break;
case|case
name|OBJ_TREE
case|:
name|obj
operator|=
operator|(
expr|struct
name|object
operator|*
operator|)
name|lookup_tree
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
break|break;
case|case
name|OBJ_BLOB
case|:
name|obj
operator|=
operator|(
expr|struct
name|object
operator|*
operator|)
name|lookup_blob
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
break|break;
default|default:
name|die
argument_list|(
literal|"unknown object type for %s: %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|obj
condition|)
name|die
argument_list|(
literal|"unable to lookup %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|add_pending_object
argument_list|(
name|data
operator|->
name|revs
argument_list|,
name|obj
argument_list|,
literal|""
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_recent_loose
specifier|static
name|int
name|add_recent_loose
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
name|path
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
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
name|obj
operator|&&
name|obj
operator|->
name|flags
operator|&
name|SEEN
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|stat
argument_list|(
name|path
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
block|{
comment|/* 		 * It's OK if an object went away during our iteration; this 		 * could be due to a simultaneous repack. But anything else 		 * we should abort, since we might then fail to mark objects 		 * which should not be pruned. 		 */
if|if
condition|(
name|errno
operator|==
name|ENOENT
condition|)
return|return
literal|0
return|;
return|return
name|error
argument_list|(
literal|"unable to stat %s: %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
block|}
name|add_recent_object
argument_list|(
name|sha1
argument_list|,
name|st
operator|.
name|st_mtime
argument_list|,
name|data
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|add_recent_packed
specifier|static
name|int
name|add_recent_packed
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|struct
name|packed_git
modifier|*
name|p
parameter_list|,
name|uint32_t
name|pos
parameter_list|,
name|void
modifier|*
name|data
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
name|obj
operator|&&
name|obj
operator|->
name|flags
operator|&
name|SEEN
condition|)
return|return
literal|0
return|;
name|add_recent_object
argument_list|(
name|sha1
argument_list|,
name|p
operator|->
name|mtime
argument_list|,
name|data
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|add_unseen_recent_objects_to_traversal
name|int
name|add_unseen_recent_objects_to_traversal
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|)
block|{
name|struct
name|recent_data
name|data
decl_stmt|;
name|int
name|r
decl_stmt|;
name|data
operator|.
name|revs
operator|=
name|revs
expr_stmt|;
name|data
operator|.
name|timestamp
operator|=
name|timestamp
expr_stmt|;
name|r
operator|=
name|for_each_loose_object
argument_list|(
name|add_recent_loose
argument_list|,
operator|&
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
condition|)
return|return
name|r
return|;
return|return
name|for_each_packed_object
argument_list|(
name|add_recent_packed
argument_list|,
operator|&
name|data
argument_list|)
return|;
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
name|unsigned
name|long
name|mark_recent
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
name|add_index_objects_to_pending
argument_list|(
name|revs
argument_list|,
literal|0
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
if|if
condition|(
name|mark_recent
condition|)
block|{
name|revs
operator|->
name|ignore_missing_links
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|add_unseen_recent_objects_to_traversal
argument_list|(
name|revs
argument_list|,
name|mark_recent
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to mark recent objects"
argument_list|)
expr_stmt|;
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
block|}
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

