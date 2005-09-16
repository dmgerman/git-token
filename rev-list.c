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
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"epoch.h"
end_include

begin_define
DECL|macro|SEEN
define|#
directive|define
name|SEEN
value|(1u<< 0)
end_define

begin_define
DECL|macro|INTERESTING
define|#
directive|define
name|INTERESTING
value|(1u<< 1)
end_define

begin_define
DECL|macro|COUNTED
define|#
directive|define
name|COUNTED
value|(1u<< 2)
end_define

begin_define
DECL|macro|SHOWN
define|#
directive|define
name|SHOWN
value|(1u<< 3)
end_define

begin_decl_stmt
DECL|variable|rev_list_usage
specifier|static
specifier|const
name|char
name|rev_list_usage
index|[]
init|=
literal|"git-rev-list [OPTION] commit-id<commit-id>\n"
literal|"  --max-count=nr\n"
literal|"  --max-age=epoch\n"
literal|"  --min-age=epoch\n"
literal|"  --parents\n"
literal|"  --bisect\n"
literal|"  --objects\n"
literal|"  --unpacked\n"
literal|"  --header\n"
literal|"  --pretty\n"
literal|"  --no-merges\n"
literal|"  --merge-order [ --show-breaks ]\n"
literal|"  --topo-order"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|unpacked
specifier|static
name|int
name|unpacked
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|bisect_list
specifier|static
name|int
name|bisect_list
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tag_objects
specifier|static
name|int
name|tag_objects
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tree_objects
specifier|static
name|int
name|tree_objects
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|blob_objects
specifier|static
name|int
name|blob_objects
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|verbose_header
specifier|static
name|int
name|verbose_header
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_parents
specifier|static
name|int
name|show_parents
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|hdr_termination
specifier|static
name|int
name|hdr_termination
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|commit_prefix
specifier|static
specifier|const
name|char
modifier|*
name|commit_prefix
init|=
literal|""
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|max_age
specifier|static
name|unsigned
name|long
name|max_age
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|min_age
specifier|static
name|unsigned
name|long
name|min_age
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|max_count
specifier|static
name|int
name|max_count
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|commit_format
specifier|static
name|enum
name|cmit_fmt
name|commit_format
init|=
name|CMIT_FMT_RAW
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|merge_order
specifier|static
name|int
name|merge_order
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_breaks
specifier|static
name|int
name|show_breaks
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|stop_traversal
specifier|static
name|int
name|stop_traversal
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|topo_order
specifier|static
name|int
name|topo_order
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|no_merges
specifier|static
name|int
name|no_merges
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|show_commit
specifier|static
name|void
name|show_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
name|commit
operator|->
name|object
operator|.
name|flags
operator||=
name|SHOWN
expr_stmt|;
if|if
condition|(
name|show_breaks
condition|)
block|{
name|commit_prefix
operator|=
literal|"| "
expr_stmt|;
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|DISCONTINUITY
condition|)
block|{
name|commit_prefix
operator|=
literal|"^ "
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|BOUNDARY
condition|)
block|{
name|commit_prefix
operator|=
literal|"= "
expr_stmt|;
block|}
block|}
name|printf
argument_list|(
literal|"%s%s"
argument_list|,
name|commit_prefix
argument_list|,
name|sha1_to_hex
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|show_parents
condition|)
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
name|printf
argument_list|(
literal|" %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|parents
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|parents
operator|=
name|parents
operator|->
name|next
expr_stmt|;
block|}
block|}
if|if
condition|(
name|commit_format
operator|==
name|CMIT_FMT_ONELINE
condition|)
name|putchar
argument_list|(
literal|' '
argument_list|)
expr_stmt|;
else|else
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose_header
condition|)
block|{
specifier|static
name|char
name|pretty_header
index|[
literal|16384
index|]
decl_stmt|;
name|pretty_print_commit
argument_list|(
name|commit_format
argument_list|,
name|commit
operator|->
name|buffer
argument_list|,
operator|~
literal|0
argument_list|,
name|pretty_header
argument_list|,
sizeof|sizeof
argument_list|(
name|pretty_header
argument_list|)
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s%c"
argument_list|,
name|pretty_header
argument_list|,
name|hdr_termination
argument_list|)
expr_stmt|;
block|}
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|filter_commit
specifier|static
name|int
name|filter_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
if|if
condition|(
name|stop_traversal
operator|&&
operator|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|BOUNDARY
operator|)
condition|)
return|return
name|STOP
return|;
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
operator|(
name|UNINTERESTING
operator||
name|SHOWN
operator|)
condition|)
return|return
name|CONTINUE
return|;
if|if
condition|(
name|min_age
operator|!=
operator|-
literal|1
operator|&&
operator|(
name|commit
operator|->
name|date
operator|>
name|min_age
operator|)
condition|)
return|return
name|CONTINUE
return|;
if|if
condition|(
name|max_age
operator|!=
operator|-
literal|1
operator|&&
operator|(
name|commit
operator|->
name|date
operator|<
name|max_age
operator|)
condition|)
block|{
name|stop_traversal
operator|=
literal|1
expr_stmt|;
return|return
name|merge_order
condition|?
name|CONTINUE
else|:
name|STOP
return|;
block|}
if|if
condition|(
name|max_count
operator|!=
operator|-
literal|1
operator|&&
operator|!
name|max_count
operator|--
condition|)
return|return
name|STOP
return|;
if|if
condition|(
name|no_merges
operator|&&
operator|(
name|commit
operator|->
name|parents
operator|&&
name|commit
operator|->
name|parents
operator|->
name|next
operator|)
condition|)
return|return
name|CONTINUE
return|;
return|return
name|DO
return|;
block|}
end_function

begin_function
DECL|function|process_commit
specifier|static
name|int
name|process_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
name|int
name|action
init|=
name|filter_commit
argument_list|(
name|commit
argument_list|)
decl_stmt|;
if|if
condition|(
name|action
operator|==
name|STOP
condition|)
block|{
return|return
name|STOP
return|;
block|}
if|if
condition|(
name|action
operator|==
name|CONTINUE
condition|)
block|{
return|return
name|CONTINUE
return|;
block|}
name|show_commit
argument_list|(
name|commit
argument_list|)
expr_stmt|;
return|return
name|CONTINUE
return|;
block|}
end_function

begin_function
DECL|function|add_object
specifier|static
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
name|name
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
DECL|function|process_blob
specifier|static
name|struct
name|object_list
modifier|*
modifier|*
name|process_blob
parameter_list|(
name|struct
name|blob
modifier|*
name|blob
parameter_list|,
name|struct
name|object_list
modifier|*
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
name|blob
operator|->
name|object
decl_stmt|;
if|if
condition|(
operator|!
name|blob_objects
condition|)
return|return
name|p
return|;
if|if
condition|(
name|obj
operator|->
name|flags
operator|&
operator|(
name|UNINTERESTING
operator||
name|SEEN
operator|)
condition|)
return|return
name|p
return|;
name|obj
operator|->
name|flags
operator||=
name|SEEN
expr_stmt|;
return|return
name|add_object
argument_list|(
name|obj
argument_list|,
name|p
argument_list|,
name|name
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|process_tree
specifier|static
name|struct
name|object_list
modifier|*
modifier|*
name|process_tree
parameter_list|(
name|struct
name|tree
modifier|*
name|tree
parameter_list|,
name|struct
name|object_list
modifier|*
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
operator|!
name|tree_objects
condition|)
return|return
name|p
return|;
if|if
condition|(
name|obj
operator|->
name|flags
operator|&
operator|(
name|UNINTERESTING
operator||
name|SEEN
operator|)
condition|)
return|return
name|p
return|;
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
name|obj
operator|->
name|flags
operator||=
name|SEEN
expr_stmt|;
name|p
operator|=
name|add_object
argument_list|(
name|obj
argument_list|,
name|p
argument_list|,
name|name
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
name|p
operator|=
name|process_tree
argument_list|(
name|entry
operator|->
name|item
operator|.
name|tree
argument_list|,
name|p
argument_list|,
name|entry
operator|->
name|name
argument_list|)
expr_stmt|;
else|else
name|p
operator|=
name|process_blob
argument_list|(
name|entry
operator|->
name|item
operator|.
name|blob
argument_list|,
name|p
argument_list|,
name|entry
operator|->
name|name
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
return|return
name|p
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|pending_objects
specifier|static
name|struct
name|object_list
modifier|*
name|pending_objects
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|show_commit_list
specifier|static
name|void
name|show_commit_list
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|)
block|{
name|struct
name|object_list
modifier|*
name|objects
init|=
name|NULL
decl_stmt|,
modifier|*
modifier|*
name|p
init|=
operator|&
name|objects
decl_stmt|,
modifier|*
name|pending
decl_stmt|;
while|while
condition|(
name|list
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|pop_most_recent_commit
argument_list|(
operator|&
name|list
argument_list|,
name|SEEN
argument_list|)
decl_stmt|;
name|p
operator|=
name|process_tree
argument_list|(
name|commit
operator|->
name|tree
argument_list|,
name|p
argument_list|,
literal|""
argument_list|)
expr_stmt|;
if|if
condition|(
name|process_commit
argument_list|(
name|commit
argument_list|)
operator|==
name|STOP
condition|)
break|break;
block|}
for|for
control|(
name|pending
operator|=
name|pending_objects
init|;
name|pending
condition|;
name|pending
operator|=
name|pending
operator|->
name|next
control|)
block|{
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
name|flags
operator|&
operator|(
name|UNINTERESTING
operator||
name|SEEN
operator|)
condition|)
continue|continue;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|tag_type
condition|)
block|{
name|obj
operator|->
name|flags
operator||=
name|SEEN
expr_stmt|;
name|p
operator|=
name|add_object
argument_list|(
name|obj
argument_list|,
name|p
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
name|tree_type
condition|)
block|{
name|p
operator|=
name|process_tree
argument_list|(
operator|(
expr|struct
name|tree
operator|*
operator|)
name|obj
argument_list|,
name|p
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
name|blob_type
condition|)
block|{
name|p
operator|=
name|process_blob
argument_list|(
operator|(
expr|struct
name|blob
operator|*
operator|)
name|obj
argument_list|,
name|p
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
while|while
condition|(
name|objects
condition|)
block|{
name|printf
argument_list|(
literal|"%s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|objects
operator|->
name|item
operator|->
name|sha1
argument_list|)
argument_list|,
name|objects
operator|->
name|name
argument_list|)
expr_stmt|;
name|objects
operator|=
name|objects
operator|->
name|next
expr_stmt|;
block|}
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
operator|!
name|blob_objects
condition|)
return|return;
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
specifier|static
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
operator|!
name|tree_objects
condition|)
return|return;
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
specifier|static
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
comment|/* 		 * A missing commit is ok iff its parent is marked  		 * uninteresting. 		 * 		 * We just mark such a thing parsed, so that when 		 * it is popped next time around, we won't be trying 		 * to parse it and get an error. 		 */
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
DECL|function|everybody_uninteresting
specifier|static
name|int
name|everybody_uninteresting
parameter_list|(
name|struct
name|commit_list
modifier|*
name|orig
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|list
init|=
name|orig
decl_stmt|;
while|while
condition|(
name|list
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|list
operator|->
name|item
decl_stmt|;
name|list
operator|=
name|list
operator|->
name|next
expr_stmt|;
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|UNINTERESTING
condition|)
continue|continue;
return|return
literal|0
return|;
block|}
return|return
literal|1
return|;
block|}
end_function

begin_comment
comment|/*  * This is a truly stupid algorithm, but it's only  * used for bisection, and we just don't care enough.  *  * We care just barely enough to avoid recursing for  * non-merge entries.  */
end_comment

begin_function
DECL|function|count_distance
specifier|static
name|int
name|count_distance
parameter_list|(
name|struct
name|commit_list
modifier|*
name|entry
parameter_list|)
block|{
name|int
name|nr
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|entry
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|entry
operator|->
name|item
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|p
decl_stmt|;
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
operator|(
name|UNINTERESTING
operator||
name|COUNTED
operator|)
condition|)
break|break;
name|nr
operator|++
expr_stmt|;
name|commit
operator|->
name|object
operator|.
name|flags
operator||=
name|COUNTED
expr_stmt|;
name|p
operator|=
name|commit
operator|->
name|parents
expr_stmt|;
name|entry
operator|=
name|p
expr_stmt|;
if|if
condition|(
name|p
condition|)
block|{
name|p
operator|=
name|p
operator|->
name|next
expr_stmt|;
while|while
condition|(
name|p
condition|)
block|{
name|nr
operator|+=
name|count_distance
argument_list|(
name|p
argument_list|)
expr_stmt|;
name|p
operator|=
name|p
operator|->
name|next
expr_stmt|;
block|}
block|}
block|}
return|return
name|nr
return|;
block|}
end_function

begin_function
DECL|function|clear_distance
specifier|static
name|void
name|clear_distance
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|)
block|{
while|while
condition|(
name|list
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|list
operator|->
name|item
decl_stmt|;
name|commit
operator|->
name|object
operator|.
name|flags
operator|&=
operator|~
name|COUNTED
expr_stmt|;
name|list
operator|=
name|list
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|find_bisection
specifier|static
name|struct
name|commit_list
modifier|*
name|find_bisection
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|)
block|{
name|int
name|nr
decl_stmt|,
name|closest
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|p
decl_stmt|,
modifier|*
name|best
decl_stmt|;
name|nr
operator|=
literal|0
expr_stmt|;
name|p
operator|=
name|list
expr_stmt|;
while|while
condition|(
name|p
condition|)
block|{
name|nr
operator|++
expr_stmt|;
name|p
operator|=
name|p
operator|->
name|next
expr_stmt|;
block|}
name|closest
operator|=
literal|0
expr_stmt|;
name|best
operator|=
name|list
expr_stmt|;
name|p
operator|=
name|list
expr_stmt|;
while|while
condition|(
name|p
condition|)
block|{
name|int
name|distance
init|=
name|count_distance
argument_list|(
name|p
argument_list|)
decl_stmt|;
name|clear_distance
argument_list|(
name|list
argument_list|)
expr_stmt|;
if|if
condition|(
name|nr
operator|-
name|distance
operator|<
name|distance
condition|)
name|distance
operator|=
name|nr
operator|-
name|distance
expr_stmt|;
if|if
condition|(
name|distance
operator|>
name|closest
condition|)
block|{
name|best
operator|=
name|p
expr_stmt|;
name|closest
operator|=
name|distance
expr_stmt|;
block|}
name|p
operator|=
name|p
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
name|best
condition|)
name|best
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
return|return
name|best
return|;
block|}
end_function

begin_function
DECL|function|mark_edges_uninteresting
specifier|static
name|void
name|mark_edges_uninteresting
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|)
block|{
for|for
control|(
init|;
name|list
condition|;
name|list
operator|=
name|list
operator|->
name|next
control|)
block|{
name|struct
name|commit_list
modifier|*
name|parents
init|=
name|list
operator|->
name|item
operator|->
name|parents
decl_stmt|;
for|for
control|(
init|;
name|parents
condition|;
name|parents
operator|=
name|parents
operator|->
name|next
control|)
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
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|UNINTERESTING
condition|)
name|mark_tree_uninteresting
argument_list|(
name|commit
operator|->
name|tree
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|limit_list
specifier|static
name|struct
name|commit_list
modifier|*
name|limit_list
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|newlist
init|=
name|NULL
decl_stmt|;
name|struct
name|commit_list
modifier|*
modifier|*
name|p
init|=
operator|&
name|newlist
decl_stmt|;
while|while
condition|(
name|list
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|pop_most_recent_commit
argument_list|(
operator|&
name|list
argument_list|,
name|SEEN
argument_list|)
decl_stmt|;
name|struct
name|object
modifier|*
name|obj
init|=
operator|&
name|commit
operator|->
name|object
decl_stmt|;
if|if
condition|(
name|unpacked
operator|&&
name|has_sha1_pack
argument_list|(
name|obj
operator|->
name|sha1
argument_list|)
condition|)
name|obj
operator|->
name|flags
operator||=
name|UNINTERESTING
expr_stmt|;
if|if
condition|(
name|obj
operator|->
name|flags
operator|&
name|UNINTERESTING
condition|)
block|{
name|mark_parents_uninteresting
argument_list|(
name|commit
argument_list|)
expr_stmt|;
if|if
condition|(
name|everybody_uninteresting
argument_list|(
name|list
argument_list|)
condition|)
break|break;
continue|continue;
block|}
name|p
operator|=
operator|&
name|commit_list_insert
argument_list|(
name|commit
argument_list|,
name|p
argument_list|)
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
name|tree_objects
condition|)
name|mark_edges_uninteresting
argument_list|(
name|newlist
argument_list|)
expr_stmt|;
if|if
condition|(
name|bisect_list
condition|)
name|newlist
operator|=
name|find_bisection
argument_list|(
name|newlist
argument_list|)
expr_stmt|;
return|return
name|newlist
return|;
block|}
end_function

begin_function
DECL|function|add_pending_object
specifier|static
name|void
name|add_pending_object
parameter_list|(
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
name|pending_objects
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
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|object
modifier|*
name|object
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|name
argument_list|,
name|sha1
argument_list|)
condition|)
name|usage
argument_list|(
name|rev_list_usage
argument_list|)
expr_stmt|;
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
DECL|function|handle_one_commit
specifier|static
name|void
name|handle_one_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|com
parameter_list|,
name|struct
name|commit_list
modifier|*
modifier|*
name|lst
parameter_list|)
block|{
if|if
condition|(
operator|!
name|com
operator|||
name|com
operator|->
name|object
operator|.
name|flags
operator|&
name|SEEN
condition|)
return|return;
name|com
operator|->
name|object
operator|.
name|flags
operator||=
name|SEEN
expr_stmt|;
name|commit_list_insert
argument_list|(
name|com
argument_list|,
name|lst
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|list
init|=
name|NULL
decl_stmt|;
name|int
name|i
decl_stmt|,
name|limited
init|=
literal|0
decl_stmt|;
name|setup_git_directory
argument_list|()
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
name|int
name|flags
decl_stmt|;
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
name|char
modifier|*
name|dotdot
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
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
literal|"--header"
argument_list|)
condition|)
block|{
name|verbose_header
operator|=
literal|1
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
literal|"--pretty"
argument_list|,
literal|8
argument_list|)
condition|)
block|{
name|commit_format
operator|=
name|get_commit_format
argument_list|(
name|arg
operator|+
literal|8
argument_list|)
expr_stmt|;
name|verbose_header
operator|=
literal|1
expr_stmt|;
name|hdr_termination
operator|=
literal|'\n'
expr_stmt|;
if|if
condition|(
name|commit_format
operator|==
name|CMIT_FMT_ONELINE
condition|)
name|commit_prefix
operator|=
literal|""
expr_stmt|;
else|else
name|commit_prefix
operator|=
literal|"commit "
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
literal|"--no-merges"
argument_list|,
literal|11
argument_list|)
condition|)
block|{
name|no_merges
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
literal|"--parents"
argument_list|)
condition|)
block|{
name|show_parents
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
literal|"--bisect"
argument_list|)
condition|)
block|{
name|bisect_list
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
name|tag_objects
operator|=
literal|1
expr_stmt|;
name|tree_objects
operator|=
literal|1
expr_stmt|;
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
literal|"--unpacked"
argument_list|)
condition|)
block|{
name|unpacked
operator|=
literal|1
expr_stmt|;
name|limited
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
literal|"--merge-order"
argument_list|)
condition|)
block|{
name|merge_order
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
literal|"--show-breaks"
argument_list|)
condition|)
block|{
name|show_breaks
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
literal|"--topo-order"
argument_list|)
condition|)
block|{
name|topo_order
operator|=
literal|1
expr_stmt|;
name|limited
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|show_breaks
operator|&&
operator|!
name|merge_order
condition|)
name|usage
argument_list|(
name|rev_list_usage
argument_list|)
expr_stmt|;
name|flags
operator|=
literal|0
expr_stmt|;
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
name|char
modifier|*
name|next
init|=
name|dotdot
operator|+
literal|2
decl_stmt|;
name|struct
name|commit
modifier|*
name|exclude
init|=
name|NULL
decl_stmt|;
name|struct
name|commit
modifier|*
name|include
init|=
name|NULL
decl_stmt|;
operator|*
name|dotdot
operator|=
literal|0
expr_stmt|;
name|exclude
operator|=
name|get_commit_reference
argument_list|(
name|arg
argument_list|,
name|UNINTERESTING
argument_list|)
expr_stmt|;
name|include
operator|=
name|get_commit_reference
argument_list|(
name|next
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|exclude
operator|&&
name|include
condition|)
block|{
name|limited
operator|=
literal|1
expr_stmt|;
name|handle_one_commit
argument_list|(
name|exclude
argument_list|,
operator|&
name|list
argument_list|)
expr_stmt|;
name|handle_one_commit
argument_list|(
name|include
argument_list|,
operator|&
name|list
argument_list|)
expr_stmt|;
continue|continue;
block|}
operator|*
name|next
operator|=
literal|'.'
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|arg
operator|==
literal|'^'
condition|)
block|{
name|flags
operator|=
name|UNINTERESTING
expr_stmt|;
name|arg
operator|++
expr_stmt|;
name|limited
operator|=
literal|1
expr_stmt|;
block|}
name|commit
operator|=
name|get_commit_reference
argument_list|(
name|arg
argument_list|,
name|flags
argument_list|)
expr_stmt|;
name|handle_one_commit
argument_list|(
name|commit
argument_list|,
operator|&
name|list
argument_list|)
expr_stmt|;
block|}
name|save_commit_buffer
operator|=
name|verbose_header
expr_stmt|;
if|if
condition|(
operator|!
name|merge_order
condition|)
block|{
name|sort_by_date
argument_list|(
operator|&
name|list
argument_list|)
expr_stmt|;
if|if
condition|(
name|limited
condition|)
name|list
operator|=
name|limit_list
argument_list|(
name|list
argument_list|)
expr_stmt|;
if|if
condition|(
name|topo_order
condition|)
name|sort_in_topological_order
argument_list|(
operator|&
name|list
argument_list|)
expr_stmt|;
name|show_commit_list
argument_list|(
name|list
argument_list|)
expr_stmt|;
block|}
else|else
block|{
ifndef|#
directive|ifndef
name|NO_OPENSSL
if|if
condition|(
name|sort_list_in_merge_order
argument_list|(
name|list
argument_list|,
operator|&
name|process_commit
argument_list|)
condition|)
block|{
name|die
argument_list|(
literal|"merge order sort failed\n"
argument_list|)
expr_stmt|;
block|}
else|#
directive|else
name|die
argument_list|(
literal|"merge order sort unsupported, OpenSSL not linked"
argument_list|)
expr_stmt|;
endif|#
directive|endif
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

