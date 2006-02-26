begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
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
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_function
DECL|function|path_name
specifier|static
name|char
modifier|*
name|path_name
parameter_list|(
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
name|name_path
modifier|*
name|p
decl_stmt|;
name|char
modifier|*
name|n
decl_stmt|,
modifier|*
name|m
decl_stmt|;
name|int
name|nlen
init|=
name|strlen
argument_list|(
name|name
argument_list|)
decl_stmt|;
name|int
name|len
init|=
name|nlen
operator|+
literal|1
decl_stmt|;
for|for
control|(
name|p
operator|=
name|path
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|up
control|)
block|{
if|if
condition|(
name|p
operator|->
name|elem_len
condition|)
name|len
operator|+=
name|p
operator|->
name|elem_len
operator|+
literal|1
expr_stmt|;
block|}
name|n
operator|=
name|xmalloc
argument_list|(
name|len
argument_list|)
expr_stmt|;
name|m
operator|=
name|n
operator|+
name|len
operator|-
operator|(
name|nlen
operator|+
literal|1
operator|)
expr_stmt|;
name|strcpy
argument_list|(
name|m
argument_list|,
name|name
argument_list|)
expr_stmt|;
for|for
control|(
name|p
operator|=
name|path
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|up
control|)
block|{
if|if
condition|(
name|p
operator|->
name|elem_len
condition|)
block|{
name|m
operator|-=
name|p
operator|->
name|elem_len
operator|+
literal|1
expr_stmt|;
name|memcpy
argument_list|(
name|m
argument_list|,
name|p
operator|->
name|elem
argument_list|,
name|p
operator|->
name|elem_len
argument_list|)
expr_stmt|;
name|m
index|[
name|p
operator|->
name|elem_len
index|]
operator|=
literal|'/'
expr_stmt|;
block|}
block|}
return|return
name|n
return|;
block|}
end_function

begin_function
DECL|function|add_object
name|struct
name|object_list
modifier|*
modifier|*
name|add_object
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|struct
name|object_list
modifier|*
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
name|object_list
modifier|*
name|entry
init|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|entry
argument_list|)
argument_list|)
decl_stmt|;
name|entry
operator|->
name|item
operator|=
name|obj
expr_stmt|;
name|entry
operator|->
name|next
operator|=
operator|*
name|p
expr_stmt|;
name|entry
operator|->
name|name
operator|=
name|path_name
argument_list|(
name|path
argument_list|,
name|name
argument_list|)
expr_stmt|;
operator|*
name|p
operator|=
name|entry
expr_stmt|;
return|return
operator|&
name|entry
operator|->
name|next
return|;
block|}
end_function

begin_function
DECL|function|mark_blob_uninteresting
specifier|static
name|void
name|mark_blob_uninteresting
parameter_list|(
name|struct
name|blob
modifier|*
name|blob
parameter_list|)
block|{
if|if
condition|(
name|blob
operator|->
name|object
operator|.
name|flags
operator|&
name|UNINTERESTING
condition|)
return|return;
name|blob
operator|->
name|object
operator|.
name|flags
operator||=
name|UNINTERESTING
expr_stmt|;
block|}
end_function

begin_function
DECL|function|mark_tree_uninteresting
name|void
name|mark_tree_uninteresting
parameter_list|(
name|struct
name|tree
modifier|*
name|tree
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
name|tree_entry_list
modifier|*
name|entry
decl_stmt|;
if|if
condition|(
name|obj
operator|->
name|flags
operator|&
name|UNINTERESTING
condition|)
return|return;
name|obj
operator|->
name|flags
operator||=
name|UNINTERESTING
expr_stmt|;
if|if
condition|(
operator|!
name|has_sha1_file
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
condition|)
return|return;
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
literal|"bad tree %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|entry
operator|=
name|tree
operator|->
name|entries
expr_stmt|;
name|tree
operator|->
name|entries
operator|=
name|NULL
expr_stmt|;
while|while
condition|(
name|entry
condition|)
block|{
name|struct
name|tree_entry_list
modifier|*
name|next
init|=
name|entry
operator|->
name|next
decl_stmt|;
if|if
condition|(
name|entry
operator|->
name|directory
condition|)
name|mark_tree_uninteresting
argument_list|(
name|entry
operator|->
name|item
operator|.
name|tree
argument_list|)
expr_stmt|;
else|else
name|mark_blob_uninteresting
argument_list|(
name|entry
operator|->
name|item
operator|.
name|blob
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|entry
argument_list|)
expr_stmt|;
name|entry
operator|=
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|mark_parents_uninteresting
name|void
name|mark_parents_uninteresting
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|parents
init|=
name|commit
operator|->
name|parents
decl_stmt|;
while|while
condition|(
name|parents
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|parents
operator|->
name|item
decl_stmt|;
name|commit
operator|->
name|object
operator|.
name|flags
operator||=
name|UNINTERESTING
expr_stmt|;
comment|/* 		 * Normally we haven't parsed the parent 		 * yet, so we won't have a parent of a parent 		 * here. However, it may turn out that we've 		 * reached this commit some other way (where it 		 * wasn't uninteresting), in which case we need 		 * to mark its parents recursively too.. 		 */
if|if
condition|(
name|commit
operator|->
name|parents
condition|)
name|mark_parents_uninteresting
argument_list|(
name|commit
argument_list|)
expr_stmt|;
comment|/* 		 * A missing commit is ok iff its parent is marked 		 * uninteresting. 		 * 		 * We just mark such a thing parsed, so that when 		 * it is popped next time around, we won't be trying 		 * to parse it and get an error. 		 */
if|if
condition|(
operator|!
name|has_sha1_file
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
name|commit
operator|->
name|object
operator|.
name|parsed
operator|=
literal|1
expr_stmt|;
name|parents
operator|=
name|parents
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|add_pending_object
specifier|static
name|void
name|add_pending_object
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|struct
name|object
modifier|*
name|obj
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|add_object
argument_list|(
name|obj
argument_list|,
operator|&
name|revs
operator|->
name|pending_objects
argument_list|,
name|NULL
argument_list|,
name|name
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_commit_reference
specifier|static
name|struct
name|commit
modifier|*
name|get_commit_reference
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|)
block|{
name|struct
name|object
modifier|*
name|object
decl_stmt|;
name|object
operator|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|object
condition|)
name|die
argument_list|(
literal|"bad object %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
comment|/* 	 * Tag object? Look what it points to.. 	 */
while|while
condition|(
name|object
operator|->
name|type
operator|==
name|tag_type
condition|)
block|{
name|struct
name|tag
modifier|*
name|tag
init|=
operator|(
expr|struct
name|tag
operator|*
operator|)
name|object
decl_stmt|;
name|object
operator|->
name|flags
operator||=
name|flags
expr_stmt|;
if|if
condition|(
name|revs
operator|->
name|tag_objects
operator|&&
operator|!
operator|(
name|object
operator|->
name|flags
operator|&
name|UNINTERESTING
operator|)
condition|)
name|add_pending_object
argument_list|(
name|revs
argument_list|,
name|object
argument_list|,
name|tag
operator|->
name|tag
argument_list|)
expr_stmt|;
name|object
operator|=
name|parse_object
argument_list|(
name|tag
operator|->
name|tagged
operator|->
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|object
condition|)
name|die
argument_list|(
literal|"bad object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|tag
operator|->
name|tagged
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * Commit object? Just return it, we'll do all the complex 	 * reachability crud. 	 */
if|if
condition|(
name|object
operator|->
name|type
operator|==
name|commit_type
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|object
decl_stmt|;
name|object
operator|->
name|flags
operator||=
name|flags
expr_stmt|;
if|if
condition|(
name|parse_commit
argument_list|(
name|commit
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to parse commit %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|UNINTERESTING
condition|)
name|mark_parents_uninteresting
argument_list|(
name|commit
argument_list|)
expr_stmt|;
return|return
name|commit
return|;
block|}
comment|/* 	 * Tree object? Either mark it uniniteresting, or add it 	 * to the list of objects to look at later.. 	 */
if|if
condition|(
name|object
operator|->
name|type
operator|==
name|tree_type
condition|)
block|{
name|struct
name|tree
modifier|*
name|tree
init|=
operator|(
expr|struct
name|tree
operator|*
operator|)
name|object
decl_stmt|;
if|if
condition|(
operator|!
name|revs
operator|->
name|tree_objects
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|flags
operator|&
name|UNINTERESTING
condition|)
block|{
name|mark_tree_uninteresting
argument_list|(
name|tree
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
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
name|NULL
return|;
block|}
comment|/* 	 * Blob object? You know the drill by now.. 	 */
if|if
condition|(
name|object
operator|->
name|type
operator|==
name|blob_type
condition|)
block|{
name|struct
name|blob
modifier|*
name|blob
init|=
operator|(
expr|struct
name|blob
operator|*
operator|)
name|object
decl_stmt|;
if|if
condition|(
operator|!
name|revs
operator|->
name|blob_objects
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|flags
operator|&
name|UNINTERESTING
condition|)
block|{
name|mark_blob_uninteresting
argument_list|(
name|blob
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
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
name|NULL
return|;
block|}
name|die
argument_list|(
literal|"%s is unknown object"
argument_list|,
name|name
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_one_commit
specifier|static
name|void
name|add_one_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
block|{
if|if
condition|(
operator|!
name|commit
operator|||
operator|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|SEEN
operator|)
condition|)
return|return;
name|commit
operator|->
name|object
operator|.
name|flags
operator||=
name|SEEN
expr_stmt|;
name|commit_list_insert
argument_list|(
name|commit
argument_list|,
operator|&
name|revs
operator|->
name|commits
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|all_flags
specifier|static
name|int
name|all_flags
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|all_revs
specifier|static
name|struct
name|rev_info
modifier|*
name|all_revs
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|handle_one_ref
specifier|static
name|int
name|handle_one_ref
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
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|get_commit_reference
argument_list|(
name|all_revs
argument_list|,
name|path
argument_list|,
name|sha1
argument_list|,
name|all_flags
argument_list|)
decl_stmt|;
name|add_one_commit
argument_list|(
name|commit
argument_list|,
name|all_revs
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|handle_all
specifier|static
name|void
name|handle_all
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|unsigned
name|flags
parameter_list|)
block|{
name|all_revs
operator|=
name|revs
expr_stmt|;
name|all_flags
operator|=
name|flags
expr_stmt|;
name|for_each_ref
argument_list|(
name|handle_one_ref
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * Parse revision information, filling in the "rev_info" structure,  * and removing the used arguments from the argument list.  *  * Returns the number of arguments left ("new argc").  */
end_comment

begin_function
DECL|function|setup_revisions
name|int
name|setup_revisions
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|flags
decl_stmt|,
name|seen_dashdash
decl_stmt|;
specifier|const
name|char
modifier|*
name|def
init|=
name|NULL
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|unrecognized
init|=
name|argv
operator|+
literal|1
decl_stmt|;
name|int
name|left
init|=
literal|1
decl_stmt|;
name|memset
argument_list|(
name|revs
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|revs
argument_list|)
argument_list|)
expr_stmt|;
name|revs
operator|->
name|lifo
operator|=
literal|1
expr_stmt|;
name|revs
operator|->
name|dense
operator|=
literal|1
expr_stmt|;
name|revs
operator|->
name|prefix
operator|=
name|setup_git_directory
argument_list|()
expr_stmt|;
name|revs
operator|->
name|max_age
operator|=
operator|-
literal|1
expr_stmt|;
name|revs
operator|->
name|min_age
operator|=
operator|-
literal|1
expr_stmt|;
name|revs
operator|->
name|max_count
operator|=
operator|-
literal|1
expr_stmt|;
comment|/* First, search for "--" */
name|seen_dashdash
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--"
argument_list|)
condition|)
continue|continue;
name|argv
index|[
name|i
index|]
operator|=
name|NULL
expr_stmt|;
name|argc
operator|=
name|i
expr_stmt|;
name|revs
operator|->
name|paths
operator|=
name|get_pathspec
argument_list|(
name|revs
operator|->
name|prefix
argument_list|,
name|argv
operator|+
name|i
operator|+
literal|1
argument_list|)
expr_stmt|;
name|seen_dashdash
operator|=
literal|1
expr_stmt|;
break|break;
block|}
name|flags
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|dotdot
decl_stmt|;
name|int
name|local_flags
decl_stmt|;
if|if
condition|(
operator|*
name|arg
operator|==
literal|'-'
condition|)
block|{
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--max-count="
argument_list|,
literal|12
argument_list|)
condition|)
block|{
name|revs
operator|->
name|max_count
operator|=
name|atoi
argument_list|(
name|arg
operator|+
literal|12
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--max-age="
argument_list|,
literal|10
argument_list|)
condition|)
block|{
name|revs
operator|->
name|max_age
operator|=
name|atoi
argument_list|(
name|arg
operator|+
literal|10
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--min-age="
argument_list|,
literal|10
argument_list|)
condition|)
block|{
name|revs
operator|->
name|min_age
operator|=
name|atoi
argument_list|(
name|arg
operator|+
literal|10
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--all"
argument_list|)
condition|)
block|{
name|handle_all
argument_list|(
name|revs
argument_list|,
name|flags
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--not"
argument_list|)
condition|)
block|{
name|flags
operator|^=
name|UNINTERESTING
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--default"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|++
name|i
operator|>=
name|argc
condition|)
name|die
argument_list|(
literal|"bad --default argument"
argument_list|)
expr_stmt|;
name|def
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--topo-order"
argument_list|)
condition|)
block|{
name|revs
operator|->
name|topo_order
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--date-order"
argument_list|)
condition|)
block|{
name|revs
operator|->
name|lifo
operator|=
literal|0
expr_stmt|;
name|revs
operator|->
name|topo_order
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--dense"
argument_list|)
condition|)
block|{
name|revs
operator|->
name|dense
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--sparse"
argument_list|)
condition|)
block|{
name|revs
operator|->
name|dense
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--remove-empty"
argument_list|)
condition|)
block|{
name|revs
operator|->
name|remove_empty_trees
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--objects"
argument_list|)
condition|)
block|{
name|revs
operator|->
name|tag_objects
operator|=
literal|1
expr_stmt|;
name|revs
operator|->
name|tree_objects
operator|=
literal|1
expr_stmt|;
name|revs
operator|->
name|blob_objects
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--objects-edge"
argument_list|)
condition|)
block|{
name|revs
operator|->
name|tag_objects
operator|=
literal|1
expr_stmt|;
name|revs
operator|->
name|tree_objects
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
name|edge_hint
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
operator|*
name|unrecognized
operator|++
operator|=
name|arg
expr_stmt|;
name|left
operator|++
expr_stmt|;
continue|continue;
block|}
name|dotdot
operator|=
name|strstr
argument_list|(
name|arg
argument_list|,
literal|".."
argument_list|)
expr_stmt|;
if|if
condition|(
name|dotdot
condition|)
block|{
name|unsigned
name|char
name|from_sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|next
init|=
name|dotdot
operator|+
literal|2
decl_stmt|;
operator|*
name|dotdot
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
operator|*
name|next
condition|)
name|next
operator|=
literal|"HEAD"
expr_stmt|;
if|if
condition|(
operator|!
name|get_sha1
argument_list|(
name|arg
argument_list|,
name|from_sha1
argument_list|)
operator|&&
operator|!
name|get_sha1
argument_list|(
name|next
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|struct
name|commit
modifier|*
name|exclude
decl_stmt|;
name|struct
name|commit
modifier|*
name|include
decl_stmt|;
name|exclude
operator|=
name|get_commit_reference
argument_list|(
name|revs
argument_list|,
name|arg
argument_list|,
name|from_sha1
argument_list|,
name|flags
operator|^
name|UNINTERESTING
argument_list|)
expr_stmt|;
name|include
operator|=
name|get_commit_reference
argument_list|(
name|revs
argument_list|,
name|next
argument_list|,
name|sha1
argument_list|,
name|flags
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|exclude
operator|||
operator|!
name|include
condition|)
name|die
argument_list|(
literal|"Invalid revision range %s..%s"
argument_list|,
name|arg
argument_list|,
name|next
argument_list|)
expr_stmt|;
name|add_one_commit
argument_list|(
name|exclude
argument_list|,
name|revs
argument_list|)
expr_stmt|;
name|add_one_commit
argument_list|(
name|include
argument_list|,
name|revs
argument_list|)
expr_stmt|;
continue|continue;
block|}
operator|*
name|dotdot
operator|=
literal|'.'
expr_stmt|;
block|}
name|local_flags
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|*
name|arg
operator|==
literal|'^'
condition|)
block|{
name|local_flags
operator|=
name|UNINTERESTING
expr_stmt|;
name|arg
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|get_sha1
argument_list|(
name|arg
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|int
name|j
decl_stmt|;
if|if
condition|(
name|seen_dashdash
operator|||
name|local_flags
condition|)
name|die
argument_list|(
literal|"bad revision '%s'"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
comment|/* If we didn't have a "--", all filenames must exist */
for|for
control|(
name|j
operator|=
name|i
init|;
name|j
operator|<
name|argc
condition|;
name|j
operator|++
control|)
block|{
if|if
condition|(
name|lstat
argument_list|(
name|argv
index|[
name|j
index|]
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"'%s': %s"
argument_list|,
name|arg
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|revs
operator|->
name|paths
operator|=
name|get_pathspec
argument_list|(
name|revs
operator|->
name|prefix
argument_list|,
name|argv
operator|+
name|i
argument_list|)
expr_stmt|;
break|break;
block|}
name|commit
operator|=
name|get_commit_reference
argument_list|(
name|revs
argument_list|,
name|arg
argument_list|,
name|sha1
argument_list|,
name|flags
operator|^
name|local_flags
argument_list|)
expr_stmt|;
name|add_one_commit
argument_list|(
name|commit
argument_list|,
name|revs
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|def
operator|&&
operator|!
name|revs
operator|->
name|commits
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|def
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"bad default revision '%s'"
argument_list|,
name|def
argument_list|)
expr_stmt|;
name|commit
operator|=
name|get_commit_reference
argument_list|(
name|revs
argument_list|,
name|def
argument_list|,
name|sha1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|add_one_commit
argument_list|(
name|commit
argument_list|,
name|revs
argument_list|)
expr_stmt|;
block|}
operator|*
name|unrecognized
operator|=
name|NULL
expr_stmt|;
return|return
name|left
return|;
block|}
end_function

end_unit

