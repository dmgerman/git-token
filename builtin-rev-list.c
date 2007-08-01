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
file|"tree-walk.h"
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
file|"list-objects.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_comment
comment|/* bits #0-15 in revision.h */
end_comment

begin_define
DECL|macro|COUNTED
define|#
directive|define
name|COUNTED
value|(1u<<16)
end_define

begin_decl_stmt
DECL|variable|rev_list_usage
specifier|static
specifier|const
name|char
name|rev_list_usage
index|[]
init|=
literal|"git-rev-list [OPTION]<commit-id>... [ -- paths... ]\n"
literal|"  limiting output:\n"
literal|"    --max-count=nr\n"
literal|"    --max-age=epoch\n"
literal|"    --min-age=epoch\n"
literal|"    --sparse\n"
literal|"    --no-merges\n"
literal|"    --remove-empty\n"
literal|"    --all\n"
literal|"    --stdin\n"
literal|"  ordering output:\n"
literal|"    --topo-order\n"
literal|"    --date-order\n"
literal|"  formatting output:\n"
literal|"    --parents\n"
literal|"    --objects | --objects-edge\n"
literal|"    --unpacked\n"
literal|"    --header | --pretty\n"
literal|"    --abbrev=nr | --no-abbrev\n"
literal|"    --abbrev-commit\n"
literal|"    --left-right\n"
literal|"  special purpose:\n"
literal|"    --bisect\n"
literal|"    --bisect-vars"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|revs
specifier|static
name|struct
name|rev_info
name|revs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|bisect_list
specifier|static
name|int
name|bisect_list
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_timestamp
specifier|static
name|int
name|show_timestamp
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|hdr_termination
specifier|static
name|int
name|hdr_termination
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|header_prefix
specifier|static
specifier|const
name|char
modifier|*
name|header_prefix
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
if|if
condition|(
name|show_timestamp
condition|)
name|printf
argument_list|(
literal|"%lu "
argument_list|,
name|commit
operator|->
name|date
argument_list|)
expr_stmt|;
if|if
condition|(
name|header_prefix
condition|)
name|fputs
argument_list|(
name|header_prefix
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
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
name|putchar
argument_list|(
literal|'-'
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|revs
operator|.
name|left_right
condition|)
block|{
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|SYMMETRIC_LEFT
condition|)
name|putchar
argument_list|(
literal|'<'
argument_list|)
expr_stmt|;
else|else
name|putchar
argument_list|(
literal|'>'
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|revs
operator|.
name|abbrev_commit
operator|&&
name|revs
operator|.
name|abbrev
condition|)
name|fputs
argument_list|(
name|find_unique_abbrev
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|,
name|revs
operator|.
name|abbrev
argument_list|)
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
else|else
name|fputs
argument_list|(
name|sha1_to_hex
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
name|revs
operator|.
name|parents
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
name|revs
operator|.
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
name|revs
operator|.
name|verbose_header
condition|)
block|{
name|char
modifier|*
name|buf
init|=
name|NULL
decl_stmt|;
name|unsigned
name|long
name|buflen
init|=
literal|0
decl_stmt|;
name|pretty_print_commit
argument_list|(
name|revs
operator|.
name|commit_format
argument_list|,
name|commit
argument_list|,
operator|~
literal|0
argument_list|,
operator|&
name|buf
argument_list|,
operator|&
name|buflen
argument_list|,
name|revs
operator|.
name|abbrev
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|,
name|revs
operator|.
name|date_mode
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s%c"
argument_list|,
name|buf
argument_list|,
name|hdr_termination
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
block|}
name|maybe_flush_or_die
argument_list|(
name|stdout
argument_list|,
literal|"stdout"
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit
operator|->
name|parents
condition|)
block|{
name|free_commit_list
argument_list|(
name|commit
operator|->
name|parents
argument_list|)
expr_stmt|;
name|commit
operator|->
name|parents
operator|=
name|NULL
expr_stmt|;
block|}
name|free
argument_list|(
name|commit
operator|->
name|buffer
argument_list|)
expr_stmt|;
name|commit
operator|->
name|buffer
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_object
specifier|static
name|void
name|show_object
parameter_list|(
name|struct
name|object_array_entry
modifier|*
name|p
parameter_list|)
block|{
comment|/* An object with name "foo\n0000000..." can be used to 	 * confuse downstream git-pack-objects very badly. 	 */
specifier|const
name|char
modifier|*
name|ep
init|=
name|strchr
argument_list|(
name|p
operator|->
name|name
argument_list|,
literal|'\n'
argument_list|)
decl_stmt|;
if|if
condition|(
name|p
operator|->
name|item
operator|->
name|type
operator|==
name|OBJ_BLOB
operator|&&
operator|!
name|has_sha1_file
argument_list|(
name|p
operator|->
name|item
operator|->
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"missing blob object '%s'"
argument_list|,
name|sha1_to_hex
argument_list|(
name|p
operator|->
name|item
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ep
condition|)
block|{
name|printf
argument_list|(
literal|"%s %.*s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|p
operator|->
name|item
operator|->
name|sha1
argument_list|)
argument_list|,
call|(
name|int
call|)
argument_list|(
name|ep
operator|-
name|p
operator|->
name|name
argument_list|)
argument_list|,
name|p
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
else|else
name|printf
argument_list|(
literal|"%s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|p
operator|->
name|item
operator|->
name|sha1
argument_list|)
argument_list|,
name|p
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_edge
specifier|static
name|void
name|show_edge
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
name|printf
argument_list|(
literal|"-%s\n"
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
if|if
condition|(
operator|!
name|revs
operator|.
name|prune_fn
operator|||
operator|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|TREECHANGE
operator|)
condition|)
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

begin_define
DECL|macro|DEBUG_BISECT
define|#
directive|define
name|DEBUG_BISECT
value|0
end_define

begin_function
DECL|function|weight
specifier|static
specifier|inline
name|int
name|weight
parameter_list|(
name|struct
name|commit_list
modifier|*
name|elem
parameter_list|)
block|{
return|return
operator|*
operator|(
operator|(
name|int
operator|*
operator|)
operator|(
name|elem
operator|->
name|item
operator|->
name|util
operator|)
operator|)
return|;
block|}
end_function

begin_function
DECL|function|weight_set
specifier|static
specifier|inline
name|void
name|weight_set
parameter_list|(
name|struct
name|commit_list
modifier|*
name|elem
parameter_list|,
name|int
name|weight
parameter_list|)
block|{
operator|*
operator|(
operator|(
name|int
operator|*
operator|)
operator|(
name|elem
operator|->
name|item
operator|->
name|util
operator|)
operator|)
operator|=
name|weight
expr_stmt|;
block|}
end_function

begin_function
DECL|function|count_interesting_parents
specifier|static
name|int
name|count_interesting_parents
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
name|p
decl_stmt|;
name|int
name|count
decl_stmt|;
for|for
control|(
name|count
operator|=
literal|0
operator|,
name|p
operator|=
name|commit
operator|->
name|parents
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
if|if
condition|(
name|p
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&
name|UNINTERESTING
condition|)
continue|continue;
name|count
operator|++
expr_stmt|;
block|}
return|return
name|count
return|;
block|}
end_function

begin_function
DECL|function|halfway
specifier|static
specifier|inline
name|int
name|halfway
parameter_list|(
name|struct
name|commit_list
modifier|*
name|p
parameter_list|,
name|int
name|distance
parameter_list|,
name|int
name|nr
parameter_list|)
block|{
comment|/* 	 * Don't short-cut something we are not going to return! 	 */
if|if
condition|(
name|revs
operator|.
name|prune_fn
operator|&&
operator|!
operator|(
name|p
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&
name|TREECHANGE
operator|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|DEBUG_BISECT
condition|)
return|return
literal|0
return|;
comment|/* 	 * 2 and 3 are halfway of 5. 	 * 3 is halfway of 6 but 2 and 4 are not. 	 */
name|distance
operator|*=
literal|2
expr_stmt|;
switch|switch
condition|(
name|distance
operator|-
name|nr
condition|)
block|{
case|case
operator|-
literal|1
case|:
case|case
literal|0
case|:
case|case
literal|1
case|:
return|return
literal|1
return|;
default|default:
return|return
literal|0
return|;
block|}
block|}
end_function

begin_if
if|#
directive|if
operator|!
name|DEBUG_BISECT
end_if

begin_define
DECL|macro|show_list
define|#
directive|define
name|show_list
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|,
name|c
parameter_list|,
name|d
parameter_list|)
value|do { ; } while (0)
end_define

begin_else
else|#
directive|else
end_else

begin_function
DECL|function|show_list
specifier|static
name|void
name|show_list
parameter_list|(
specifier|const
name|char
modifier|*
name|debug
parameter_list|,
name|int
name|counted
parameter_list|,
name|int
name|nr
parameter_list|,
name|struct
name|commit_list
modifier|*
name|list
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|p
decl_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s (%d/%d)\n"
argument_list|,
name|debug
argument_list|,
name|counted
argument_list|,
name|nr
argument_list|)
expr_stmt|;
for|for
control|(
name|p
operator|=
name|list
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
name|struct
name|commit_list
modifier|*
name|pp
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
init|=
name|p
operator|->
name|item
decl_stmt|;
name|unsigned
name|flags
init|=
name|commit
operator|->
name|object
operator|.
name|flags
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|char
modifier|*
name|buf
init|=
name|read_sha1_file
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|,
operator|&
name|type
argument_list|,
operator|&
name|size
argument_list|)
decl_stmt|;
name|char
modifier|*
name|ep
decl_stmt|,
modifier|*
name|sp
decl_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%c%c%c "
argument_list|,
operator|(
name|flags
operator|&
name|TREECHANGE
operator|)
condition|?
literal|'T'
else|:
literal|' '
argument_list|,
operator|(
name|flags
operator|&
name|UNINTERESTING
operator|)
condition|?
literal|'U'
else|:
literal|' '
argument_list|,
operator|(
name|flags
operator|&
name|COUNTED
operator|)
condition|?
literal|'C'
else|:
literal|' '
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit
operator|->
name|util
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%3d"
argument_list|,
name|weight
argument_list|(
name|p
argument_list|)
argument_list|)
expr_stmt|;
else|else
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"---"
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|" %.*s"
argument_list|,
literal|8
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
for|for
control|(
name|pp
operator|=
name|commit
operator|->
name|parents
init|;
name|pp
condition|;
name|pp
operator|=
name|pp
operator|->
name|next
control|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|" %.*s"
argument_list|,
literal|8
argument_list|,
name|sha1_to_hex
argument_list|(
name|pp
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|sp
operator|=
name|strstr
argument_list|(
name|buf
argument_list|,
literal|"\n\n"
argument_list|)
expr_stmt|;
if|if
condition|(
name|sp
condition|)
block|{
name|sp
operator|+=
literal|2
expr_stmt|;
for|for
control|(
name|ep
operator|=
name|sp
init|;
operator|*
name|ep
operator|&&
operator|*
name|ep
operator|!=
literal|'\n'
condition|;
name|ep
operator|++
control|)
empty_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|" %.*s"
argument_list|,
call|(
name|int
call|)
argument_list|(
name|ep
operator|-
name|sp
argument_list|)
argument_list|,
name|sp
argument_list|)
expr_stmt|;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"\n"
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* DEBUG_BISECT */
end_comment

begin_comment
comment|/*  * zero or positive weight is the number of interesting commits it can  * reach, including itself.  Especially, weight = 0 means it does not  * reach any tree-changing commits (e.g. just above uninteresting one  * but traversal is with pathspec).  *  * weight = -1 means it has one parent and its distance is yet to  * be computed.  *  * weight = -2 means it has more than one parent and its distance is  * unknown.  After running count_distance() first, they will get zero  * or positive distance.  */
end_comment

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
parameter_list|,
name|int
modifier|*
name|reaches
parameter_list|,
name|int
modifier|*
name|all
parameter_list|)
block|{
name|int
name|n
decl_stmt|,
name|nr
decl_stmt|,
name|on_list
decl_stmt|,
name|counted
decl_stmt|,
name|distance
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|p
decl_stmt|,
modifier|*
name|best
decl_stmt|,
modifier|*
name|next
decl_stmt|,
modifier|*
name|last
decl_stmt|;
name|int
modifier|*
name|weights
decl_stmt|;
name|show_list
argument_list|(
literal|"bisection 2 entry"
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|,
name|list
argument_list|)
expr_stmt|;
comment|/* 	 * Count the number of total and tree-changing items on the 	 * list, while reversing the list. 	 */
for|for
control|(
name|nr
operator|=
name|on_list
operator|=
literal|0
operator|,
name|last
operator|=
name|NULL
operator|,
name|p
operator|=
name|list
init|;
name|p
condition|;
name|p
operator|=
name|next
control|)
block|{
name|unsigned
name|flags
init|=
name|p
operator|->
name|item
operator|->
name|object
operator|.
name|flags
decl_stmt|;
name|next
operator|=
name|p
operator|->
name|next
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|UNINTERESTING
condition|)
continue|continue;
name|p
operator|->
name|next
operator|=
name|last
expr_stmt|;
name|last
operator|=
name|p
expr_stmt|;
if|if
condition|(
operator|!
name|revs
operator|.
name|prune_fn
operator|||
operator|(
name|flags
operator|&
name|TREECHANGE
operator|)
condition|)
name|nr
operator|++
expr_stmt|;
name|on_list
operator|++
expr_stmt|;
block|}
name|list
operator|=
name|last
expr_stmt|;
name|show_list
argument_list|(
literal|"bisection 2 sorted"
argument_list|,
literal|0
argument_list|,
name|nr
argument_list|,
name|list
argument_list|)
expr_stmt|;
operator|*
name|all
operator|=
name|nr
expr_stmt|;
name|weights
operator|=
name|xcalloc
argument_list|(
name|on_list
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|weights
argument_list|)
argument_list|)
expr_stmt|;
name|counted
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|0
operator|,
name|p
operator|=
name|list
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|p
operator|->
name|item
decl_stmt|;
name|unsigned
name|flags
init|=
name|commit
operator|->
name|object
operator|.
name|flags
decl_stmt|;
name|p
operator|->
name|item
operator|->
name|util
operator|=
operator|&
name|weights
index|[
name|n
operator|++
index|]
expr_stmt|;
switch|switch
condition|(
name|count_interesting_parents
argument_list|(
name|commit
argument_list|)
condition|)
block|{
case|case
literal|0
case|:
if|if
condition|(
operator|!
name|revs
operator|.
name|prune_fn
operator|||
operator|(
name|flags
operator|&
name|TREECHANGE
operator|)
condition|)
block|{
name|weight_set
argument_list|(
name|p
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|counted
operator|++
expr_stmt|;
name|show_list
argument_list|(
literal|"bisection 2 count one"
argument_list|,
name|counted
argument_list|,
name|nr
argument_list|,
name|list
argument_list|)
expr_stmt|;
block|}
comment|/* 			 * otherwise, it is known not to reach any 			 * tree-changing commit and gets weight 0. 			 */
break|break;
case|case
literal|1
case|:
name|weight_set
argument_list|(
name|p
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
break|break;
default|default:
name|weight_set
argument_list|(
name|p
argument_list|,
operator|-
literal|2
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
name|show_list
argument_list|(
literal|"bisection 2 initialize"
argument_list|,
name|counted
argument_list|,
name|nr
argument_list|,
name|list
argument_list|)
expr_stmt|;
comment|/* 	 * If you have only one parent in the resulting set 	 * then you can reach one commit more than that parent 	 * can reach.  So we do not have to run the expensive 	 * count_distance() for single strand of pearls. 	 * 	 * However, if you have more than one parents, you cannot 	 * just add their distance and one for yourself, since 	 * they usually reach the same ancestor and you would 	 * end up counting them twice that way. 	 * 	 * So we will first count distance of merges the usual 	 * way, and then fill the blanks using cheaper algorithm. 	 */
for|for
control|(
name|p
operator|=
name|list
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
if|if
condition|(
name|p
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&
name|UNINTERESTING
condition|)
continue|continue;
name|n
operator|=
name|weight
argument_list|(
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|!=
operator|-
literal|2
condition|)
continue|continue;
name|distance
operator|=
name|count_distance
argument_list|(
name|p
argument_list|)
expr_stmt|;
name|clear_distance
argument_list|(
name|list
argument_list|)
expr_stmt|;
name|weight_set
argument_list|(
name|p
argument_list|,
name|distance
argument_list|)
expr_stmt|;
comment|/* Does it happen to be at exactly half-way? */
if|if
condition|(
name|halfway
argument_list|(
name|p
argument_list|,
name|distance
argument_list|,
name|nr
argument_list|)
condition|)
block|{
name|p
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
operator|*
name|reaches
operator|=
name|distance
expr_stmt|;
name|free
argument_list|(
name|weights
argument_list|)
expr_stmt|;
return|return
name|p
return|;
block|}
name|counted
operator|++
expr_stmt|;
block|}
name|show_list
argument_list|(
literal|"bisection 2 count_distance"
argument_list|,
name|counted
argument_list|,
name|nr
argument_list|,
name|list
argument_list|)
expr_stmt|;
while|while
condition|(
name|counted
operator|<
name|nr
condition|)
block|{
for|for
control|(
name|p
operator|=
name|list
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
name|struct
name|commit_list
modifier|*
name|q
decl_stmt|;
name|unsigned
name|flags
init|=
name|p
operator|->
name|item
operator|->
name|object
operator|.
name|flags
decl_stmt|;
if|if
condition|(
literal|0
operator|<=
name|weight
argument_list|(
name|p
argument_list|)
condition|)
continue|continue;
for|for
control|(
name|q
operator|=
name|p
operator|->
name|item
operator|->
name|parents
init|;
name|q
condition|;
name|q
operator|=
name|q
operator|->
name|next
control|)
block|{
if|if
condition|(
name|q
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&
name|UNINTERESTING
condition|)
continue|continue;
if|if
condition|(
literal|0
operator|<=
name|weight
argument_list|(
name|q
argument_list|)
condition|)
break|break;
block|}
if|if
condition|(
operator|!
name|q
condition|)
continue|continue;
comment|/* 			 * weight for p is unknown but q is known. 			 * add one for p itself if p is to be counted, 			 * otherwise inherit it from q directly. 			 */
if|if
condition|(
operator|!
name|revs
operator|.
name|prune_fn
operator|||
operator|(
name|flags
operator|&
name|TREECHANGE
operator|)
condition|)
block|{
name|weight_set
argument_list|(
name|p
argument_list|,
name|weight
argument_list|(
name|q
argument_list|)
operator|+
literal|1
argument_list|)
expr_stmt|;
name|counted
operator|++
expr_stmt|;
name|show_list
argument_list|(
literal|"bisection 2 count one"
argument_list|,
name|counted
argument_list|,
name|nr
argument_list|,
name|list
argument_list|)
expr_stmt|;
block|}
else|else
name|weight_set
argument_list|(
name|p
argument_list|,
name|weight
argument_list|(
name|q
argument_list|)
argument_list|)
expr_stmt|;
comment|/* Does it happen to be at exactly half-way? */
name|distance
operator|=
name|weight
argument_list|(
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
name|halfway
argument_list|(
name|p
argument_list|,
name|distance
argument_list|,
name|nr
argument_list|)
condition|)
block|{
name|p
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
operator|*
name|reaches
operator|=
name|distance
expr_stmt|;
name|free
argument_list|(
name|weights
argument_list|)
expr_stmt|;
return|return
name|p
return|;
block|}
block|}
block|}
name|show_list
argument_list|(
literal|"bisection 2 counted all"
argument_list|,
name|counted
argument_list|,
name|nr
argument_list|,
name|list
argument_list|)
expr_stmt|;
comment|/* Then find the best one */
name|counted
operator|=
operator|-
literal|1
expr_stmt|;
name|best
operator|=
name|list
expr_stmt|;
for|for
control|(
name|p
operator|=
name|list
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
name|unsigned
name|flags
init|=
name|p
operator|->
name|item
operator|->
name|object
operator|.
name|flags
decl_stmt|;
if|if
condition|(
name|revs
operator|.
name|prune_fn
operator|&&
operator|!
operator|(
name|flags
operator|&
name|TREECHANGE
operator|)
condition|)
continue|continue;
name|distance
operator|=
name|weight
argument_list|(
name|p
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
name|counted
condition|)
block|{
name|best
operator|=
name|p
expr_stmt|;
name|counted
operator|=
name|distance
expr_stmt|;
operator|*
name|reaches
operator|=
name|weight
argument_list|(
name|p
argument_list|)
expr_stmt|;
block|}
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
name|free
argument_list|(
name|weights
argument_list|)
expr_stmt|;
return|return
name|best
return|;
block|}
end_function

begin_function
DECL|function|read_revisions_from_stdin
specifier|static
name|void
name|read_revisions_from_stdin
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
block|{
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
while|while
condition|(
name|fgets
argument_list|(
name|line
argument_list|,
sizeof|sizeof
argument_list|(
name|line
argument_list|)
argument_list|,
name|stdin
argument_list|)
operator|!=
name|NULL
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|line
argument_list|)
decl_stmt|;
if|if
condition|(
name|line
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
name|line
index|[
operator|--
name|len
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
name|len
condition|)
break|break;
if|if
condition|(
name|line
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
name|die
argument_list|(
literal|"options not supported in --stdin mode"
argument_list|)
expr_stmt|;
if|if
condition|(
name|handle_revision_arg
argument_list|(
name|line
argument_list|,
name|revs
argument_list|,
literal|0
argument_list|,
literal|1
argument_list|)
condition|)
name|die
argument_list|(
literal|"bad revision '%s'"
argument_list|,
name|line
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|cmd_rev_list
name|int
name|cmd_rev_list
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
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|list
decl_stmt|;
name|int
name|i
decl_stmt|;
name|int
name|read_from_stdin
init|=
literal|0
decl_stmt|;
name|int
name|bisect_show_vars
init|=
literal|0
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
name|init_revisions
argument_list|(
operator|&
name|revs
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|revs
operator|.
name|abbrev
operator|=
literal|0
expr_stmt|;
name|revs
operator|.
name|commit_format
operator|=
name|CMIT_FMT_UNSPECIFIED
expr_stmt|;
name|argc
operator|=
name|setup_revisions
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
operator|&
name|revs
argument_list|,
name|NULL
argument_list|)
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
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--header"
argument_list|)
condition|)
block|{
name|revs
operator|.
name|verbose_header
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
literal|"--timestamp"
argument_list|)
condition|)
block|{
name|show_timestamp
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
literal|"--bisect-vars"
argument_list|)
condition|)
block|{
name|bisect_list
operator|=
literal|1
expr_stmt|;
name|bisect_show_vars
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
literal|"--stdin"
argument_list|)
condition|)
block|{
if|if
condition|(
name|read_from_stdin
operator|++
condition|)
name|die
argument_list|(
literal|"--stdin given twice?"
argument_list|)
expr_stmt|;
name|read_revisions_from_stdin
argument_list|(
operator|&
name|revs
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|rev_list_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|revs
operator|.
name|commit_format
operator|!=
name|CMIT_FMT_UNSPECIFIED
condition|)
block|{
comment|/* The command line has a --pretty  */
name|hdr_termination
operator|=
literal|'\n'
expr_stmt|;
if|if
condition|(
name|revs
operator|.
name|commit_format
operator|==
name|CMIT_FMT_ONELINE
condition|)
name|header_prefix
operator|=
literal|""
expr_stmt|;
else|else
name|header_prefix
operator|=
literal|"commit "
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|revs
operator|.
name|verbose_header
condition|)
comment|/* Only --header was specified */
name|revs
operator|.
name|commit_format
operator|=
name|CMIT_FMT_RAW
expr_stmt|;
name|list
operator|=
name|revs
operator|.
name|commits
expr_stmt|;
if|if
condition|(
operator|(
operator|!
name|list
operator|&&
operator|(
operator|!
operator|(
name|revs
operator|.
name|tag_objects
operator|||
name|revs
operator|.
name|tree_objects
operator|||
name|revs
operator|.
name|blob_objects
operator|)
operator|&&
operator|!
name|revs
operator|.
name|pending
operator|.
name|nr
operator|)
operator|)
operator|||
name|revs
operator|.
name|diff
condition|)
name|usage
argument_list|(
name|rev_list_usage
argument_list|)
expr_stmt|;
name|save_commit_buffer
operator|=
name|revs
operator|.
name|verbose_header
operator|||
name|revs
operator|.
name|grep_filter
expr_stmt|;
name|track_object_refs
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|bisect_list
condition|)
name|revs
operator|.
name|limited
operator|=
literal|1
expr_stmt|;
name|prepare_revision_walk
argument_list|(
operator|&
name|revs
argument_list|)
expr_stmt|;
if|if
condition|(
name|revs
operator|.
name|tree_objects
condition|)
name|mark_edges_uninteresting
argument_list|(
name|revs
operator|.
name|commits
argument_list|,
operator|&
name|revs
argument_list|,
name|show_edge
argument_list|)
expr_stmt|;
if|if
condition|(
name|bisect_list
condition|)
block|{
name|int
name|reaches
init|=
name|reaches
decl_stmt|,
name|all
init|=
name|all
decl_stmt|;
name|revs
operator|.
name|commits
operator|=
name|find_bisection
argument_list|(
name|revs
operator|.
name|commits
argument_list|,
operator|&
name|reaches
argument_list|,
operator|&
name|all
argument_list|)
expr_stmt|;
if|if
condition|(
name|bisect_show_vars
condition|)
block|{
name|int
name|cnt
decl_stmt|;
if|if
condition|(
operator|!
name|revs
operator|.
name|commits
condition|)
return|return
literal|1
return|;
comment|/* 			 * revs.commits can reach "reaches" commits among 			 * "all" commits.  If it is good, then there are 			 * (all-reaches) commits left to be bisected. 			 * On the other hand, if it is bad, then the set 			 * to bisect is "reaches". 			 * A bisect set of size N has (N-1) commits further 			 * to test, as we already know one bad one. 			 */
name|cnt
operator|=
name|all
operator|-
name|reaches
expr_stmt|;
if|if
condition|(
name|cnt
operator|<
name|reaches
condition|)
name|cnt
operator|=
name|reaches
expr_stmt|;
name|printf
argument_list|(
literal|"bisect_rev=%s\n"
literal|"bisect_nr=%d\n"
literal|"bisect_good=%d\n"
literal|"bisect_bad=%d\n"
literal|"bisect_all=%d\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|revs
operator|.
name|commits
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|,
name|cnt
operator|-
literal|1
argument_list|,
name|all
operator|-
name|reaches
operator|-
literal|1
argument_list|,
name|reaches
operator|-
literal|1
argument_list|,
name|all
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
block|}
name|traverse_commit_list
argument_list|(
operator|&
name|revs
argument_list|,
name|show_commit
argument_list|,
name|show_object
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

