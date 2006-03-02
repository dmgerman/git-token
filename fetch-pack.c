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
file|"pkt-line.h"
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
DECL|variable|keep_pack
specifier|static
name|int
name|keep_pack
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|quiet
specifier|static
name|int
name|quiet
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|verbose
specifier|static
name|int
name|verbose
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|fetch_pack_usage
specifier|static
specifier|const
name|char
name|fetch_pack_usage
index|[]
init|=
literal|"git-fetch-pack [-q] [-v] [-k] [--thin] [--exec=upload-pack] [host:]directory<refs>..."
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|exec
specifier|static
specifier|const
name|char
modifier|*
name|exec
init|=
literal|"git-upload-pack"
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|COMPLETE
define|#
directive|define
name|COMPLETE
value|(1U<< 0)
end_define

begin_define
DECL|macro|COMMON
define|#
directive|define
name|COMMON
value|(1U<< 1)
end_define

begin_define
DECL|macro|COMMON_REF
define|#
directive|define
name|COMMON_REF
value|(1U<< 2)
end_define

begin_define
DECL|macro|SEEN
define|#
directive|define
name|SEEN
value|(1U<< 3)
end_define

begin_define
DECL|macro|POPPED
define|#
directive|define
name|POPPED
value|(1U<< 4)
end_define

begin_decl_stmt
DECL|variable|rev_list
specifier|static
name|struct
name|commit_list
modifier|*
name|rev_list
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|non_common_revs
DECL|variable|multi_ack
DECL|variable|use_thin_pack
specifier|static
name|int
name|non_common_revs
init|=
literal|0
decl_stmt|,
name|multi_ack
init|=
literal|0
decl_stmt|,
name|use_thin_pack
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|rev_list_push
specifier|static
name|void
name|rev_list_push
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|int
name|mark
parameter_list|)
block|{
if|if
condition|(
operator|!
operator|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|mark
operator|)
condition|)
block|{
name|commit
operator|->
name|object
operator|.
name|flags
operator||=
name|mark
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|commit
operator|->
name|object
operator|.
name|parsed
operator|)
condition|)
name|parse_commit
argument_list|(
name|commit
argument_list|)
expr_stmt|;
name|insert_by_date
argument_list|(
name|commit
argument_list|,
operator|&
name|rev_list
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|COMMON
operator|)
condition|)
name|non_common_revs
operator|++
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|rev_list_insert_ref
specifier|static
name|int
name|rev_list_insert_ref
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
name|object
modifier|*
name|o
init|=
name|deref_tag
argument_list|(
name|parse_object
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|path
argument_list|,
literal|0
argument_list|)
decl_stmt|;
if|if
condition|(
name|o
operator|&&
name|o
operator|->
name|type
operator|==
name|commit_type
condition|)
name|rev_list_push
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o
argument_list|,
name|SEEN
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*    This function marks a rev and its ancestors as common.    In some cases, it is desirable to mark only the ancestors (for example    when only the server does not yet know that they are common). */
end_comment

begin_function
DECL|function|mark_common
specifier|static
name|void
name|mark_common
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|int
name|ancestors_only
parameter_list|,
name|int
name|dont_parse
parameter_list|)
block|{
if|if
condition|(
name|commit
operator|!=
name|NULL
operator|&&
operator|!
operator|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|COMMON
operator|)
condition|)
block|{
name|struct
name|object
modifier|*
name|o
init|=
operator|(
expr|struct
name|object
operator|*
operator|)
name|commit
decl_stmt|;
if|if
condition|(
operator|!
name|ancestors_only
condition|)
name|o
operator|->
name|flags
operator||=
name|COMMON
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|o
operator|->
name|flags
operator|&
name|SEEN
operator|)
condition|)
name|rev_list_push
argument_list|(
name|commit
argument_list|,
name|SEEN
argument_list|)
expr_stmt|;
else|else
block|{
name|struct
name|commit_list
modifier|*
name|parents
decl_stmt|;
if|if
condition|(
operator|!
name|ancestors_only
operator|&&
operator|!
operator|(
name|o
operator|->
name|flags
operator|&
name|POPPED
operator|)
condition|)
name|non_common_revs
operator|--
expr_stmt|;
if|if
condition|(
operator|!
name|o
operator|->
name|parsed
operator|&&
operator|!
name|dont_parse
condition|)
name|parse_commit
argument_list|(
name|commit
argument_list|)
expr_stmt|;
for|for
control|(
name|parents
operator|=
name|commit
operator|->
name|parents
init|;
name|parents
condition|;
name|parents
operator|=
name|parents
operator|->
name|next
control|)
name|mark_common
argument_list|(
name|parents
operator|->
name|item
argument_list|,
literal|0
argument_list|,
name|dont_parse
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_comment
comment|/*   Get the next rev to send, ignoring the common. */
end_comment

begin_function
DECL|function|get_rev
specifier|static
specifier|const
name|unsigned
name|char
modifier|*
name|get_rev
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|NULL
decl_stmt|;
while|while
condition|(
name|commit
operator|==
name|NULL
condition|)
block|{
name|unsigned
name|int
name|mark
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|parents
decl_stmt|;
if|if
condition|(
name|rev_list
operator|==
name|NULL
operator|||
name|non_common_revs
operator|==
literal|0
condition|)
return|return
name|NULL
return|;
name|commit
operator|=
name|rev_list
operator|->
name|item
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|commit
operator|->
name|object
operator|.
name|parsed
operator|)
condition|)
name|parse_commit
argument_list|(
name|commit
argument_list|)
expr_stmt|;
name|commit
operator|->
name|object
operator|.
name|flags
operator||=
name|POPPED
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|COMMON
operator|)
condition|)
name|non_common_revs
operator|--
expr_stmt|;
name|parents
operator|=
name|commit
operator|->
name|parents
expr_stmt|;
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|COMMON
condition|)
block|{
comment|/* do not send "have", and ignore ancestors */
name|commit
operator|=
name|NULL
expr_stmt|;
name|mark
operator|=
name|COMMON
operator||
name|SEEN
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
name|COMMON_REF
condition|)
comment|/* send "have", and ignore ancestors */
name|mark
operator|=
name|COMMON
operator||
name|SEEN
expr_stmt|;
else|else
comment|/* send "have", also for its ancestors */
name|mark
operator|=
name|SEEN
expr_stmt|;
while|while
condition|(
name|parents
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|parents
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&
name|SEEN
operator|)
condition|)
name|rev_list_push
argument_list|(
name|parents
operator|->
name|item
argument_list|,
name|mark
argument_list|)
expr_stmt|;
if|if
condition|(
name|mark
operator|&
name|COMMON
condition|)
name|mark_common
argument_list|(
name|parents
operator|->
name|item
argument_list|,
literal|1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|parents
operator|=
name|parents
operator|->
name|next
expr_stmt|;
block|}
name|rev_list
operator|=
name|rev_list
operator|->
name|next
expr_stmt|;
block|}
return|return
name|commit
operator|->
name|object
operator|.
name|sha1
return|;
block|}
end_function

begin_function
DECL|function|find_common
specifier|static
name|int
name|find_common
parameter_list|(
name|int
name|fd
index|[
literal|2
index|]
parameter_list|,
name|unsigned
name|char
modifier|*
name|result_sha1
parameter_list|,
name|struct
name|ref
modifier|*
name|refs
parameter_list|)
block|{
name|int
name|fetching
decl_stmt|;
name|int
name|count
init|=
literal|0
decl_stmt|,
name|flushes
init|=
literal|0
decl_stmt|,
name|retval
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|for_each_ref
argument_list|(
name|rev_list_insert_ref
argument_list|)
expr_stmt|;
name|fetching
operator|=
literal|0
expr_stmt|;
for|for
control|(
init|;
name|refs
condition|;
name|refs
operator|=
name|refs
operator|->
name|next
control|)
block|{
name|unsigned
name|char
modifier|*
name|remote
init|=
name|refs
operator|->
name|old_sha1
decl_stmt|;
name|struct
name|object
modifier|*
name|o
decl_stmt|;
comment|/* 		 * If that object is complete (i.e. it is an ancestor of a 		 * local ref), we tell them we have it but do not have to 		 * tell them about its ancestors, which they already know 		 * about. 		 * 		 * We use lookup_object here because we are only 		 * interested in the case we *know* the object is 		 * reachable and we have already scanned it. 		 */
if|if
condition|(
operator|(
operator|(
name|o
operator|=
name|lookup_object
argument_list|(
name|remote
argument_list|)
operator|)
operator|!=
name|NULL
operator|)
operator|&&
operator|(
name|o
operator|->
name|flags
operator|&
name|COMPLETE
operator|)
condition|)
block|{
continue|continue;
block|}
name|packet_write
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|,
literal|"want %s%s%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|remote
argument_list|)
argument_list|,
operator|(
name|multi_ack
condition|?
literal|" multi_ack"
else|:
literal|""
operator|)
argument_list|,
operator|(
name|use_thin_pack
condition|?
literal|" thin-pack"
else|:
literal|""
operator|)
argument_list|)
expr_stmt|;
name|fetching
operator|++
expr_stmt|;
block|}
name|packet_flush
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|fetching
condition|)
return|return
literal|1
return|;
name|flushes
operator|=
literal|0
expr_stmt|;
name|retval
operator|=
operator|-
literal|1
expr_stmt|;
while|while
condition|(
operator|(
name|sha1
operator|=
name|get_rev
argument_list|()
operator|)
condition|)
block|{
name|packet_write
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|,
literal|"have %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"have %s\n"
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
operator|(
literal|31
operator|&
operator|++
name|count
operator|)
condition|)
block|{
name|int
name|ack
decl_stmt|;
name|packet_flush
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|flushes
operator|++
expr_stmt|;
comment|/* 			 * We keep one window "ahead" of the other side, and 			 * will wait for an ACK only on the next one 			 */
if|if
condition|(
name|count
operator|==
literal|32
condition|)
continue|continue;
do|do
block|{
name|ack
operator|=
name|get_ack
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|result_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
operator|&&
name|ack
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"got ack %d %s\n"
argument_list|,
name|ack
argument_list|,
name|sha1_to_hex
argument_list|(
name|result_sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ack
operator|==
literal|1
condition|)
block|{
name|flushes
operator|=
literal|0
expr_stmt|;
name|multi_ack
operator|=
literal|0
expr_stmt|;
name|retval
operator|=
literal|0
expr_stmt|;
goto|goto
name|done
goto|;
block|}
elseif|else
if|if
condition|(
name|ack
operator|==
literal|2
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|lookup_commit
argument_list|(
name|result_sha1
argument_list|)
decl_stmt|;
name|mark_common
argument_list|(
name|commit
argument_list|,
literal|0
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|retval
operator|=
literal|0
expr_stmt|;
block|}
block|}
do|while
condition|(
name|ack
condition|)
do|;
name|flushes
operator|--
expr_stmt|;
block|}
block|}
name|done
label|:
name|packet_write
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|,
literal|"done\n"
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"done\n"
argument_list|)
expr_stmt|;
if|if
condition|(
name|retval
operator|!=
literal|0
condition|)
block|{
name|multi_ack
operator|=
literal|0
expr_stmt|;
name|flushes
operator|++
expr_stmt|;
block|}
while|while
condition|(
name|flushes
operator|||
name|multi_ack
condition|)
block|{
name|int
name|ack
init|=
name|get_ack
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
name|result_sha1
argument_list|)
decl_stmt|;
if|if
condition|(
name|ack
condition|)
block|{
if|if
condition|(
name|verbose
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"got ack (%d) %s\n"
argument_list|,
name|ack
argument_list|,
name|sha1_to_hex
argument_list|(
name|result_sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ack
operator|==
literal|1
condition|)
return|return
literal|0
return|;
name|multi_ack
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|flushes
operator|--
expr_stmt|;
block|}
return|return
name|retval
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|complete
specifier|static
name|struct
name|commit_list
modifier|*
name|complete
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|mark_complete
specifier|static
name|int
name|mark_complete
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
name|object
modifier|*
name|o
init|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
while|while
condition|(
name|o
operator|&&
name|o
operator|->
name|type
operator|==
name|tag_type
condition|)
block|{
name|struct
name|tag
modifier|*
name|t
init|=
operator|(
expr|struct
name|tag
operator|*
operator|)
name|o
decl_stmt|;
if|if
condition|(
operator|!
name|t
operator|->
name|tagged
condition|)
break|break;
comment|/* broken repository */
name|o
operator|->
name|flags
operator||=
name|COMPLETE
expr_stmt|;
name|o
operator|=
name|parse_object
argument_list|(
name|t
operator|->
name|tagged
operator|->
name|sha1
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|o
operator|&&
name|o
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
name|o
decl_stmt|;
name|commit
operator|->
name|object
operator|.
name|flags
operator||=
name|COMPLETE
expr_stmt|;
name|insert_by_date
argument_list|(
name|commit
argument_list|,
operator|&
name|complete
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|mark_recent_complete_commits
specifier|static
name|void
name|mark_recent_complete_commits
parameter_list|(
name|unsigned
name|long
name|cutoff
parameter_list|)
block|{
while|while
condition|(
name|complete
operator|&&
name|cutoff
operator|<=
name|complete
operator|->
name|item
operator|->
name|date
condition|)
block|{
if|if
condition|(
name|verbose
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Marking %s as complete\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|complete
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|pop_most_recent_commit
argument_list|(
operator|&
name|complete
argument_list|,
name|COMPLETE
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|filter_refs
specifier|static
name|void
name|filter_refs
parameter_list|(
name|struct
name|ref
modifier|*
modifier|*
name|refs
parameter_list|,
name|int
name|nr_match
parameter_list|,
name|char
modifier|*
modifier|*
name|match
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|prev
decl_stmt|,
modifier|*
name|current
decl_stmt|,
modifier|*
name|next
decl_stmt|;
for|for
control|(
name|prev
operator|=
name|NULL
operator|,
name|current
operator|=
operator|*
name|refs
init|;
name|current
condition|;
name|current
operator|=
name|next
control|)
block|{
name|next
operator|=
name|current
operator|->
name|next
expr_stmt|;
if|if
condition|(
operator|(
operator|!
name|memcmp
argument_list|(
name|current
operator|->
name|name
argument_list|,
literal|"refs/"
argument_list|,
literal|5
argument_list|)
operator|&&
name|check_ref_format
argument_list|(
name|current
operator|->
name|name
operator|+
literal|5
argument_list|)
operator|)
operator|||
operator|!
name|path_match
argument_list|(
name|current
operator|->
name|name
argument_list|,
name|nr_match
argument_list|,
name|match
argument_list|)
condition|)
block|{
if|if
condition|(
name|prev
operator|==
name|NULL
condition|)
operator|*
name|refs
operator|=
name|next
expr_stmt|;
else|else
name|prev
operator|->
name|next
operator|=
name|next
expr_stmt|;
name|free
argument_list|(
name|current
argument_list|)
expr_stmt|;
block|}
else|else
name|prev
operator|=
name|current
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|everything_local
specifier|static
name|int
name|everything_local
parameter_list|(
name|struct
name|ref
modifier|*
modifier|*
name|refs
parameter_list|,
name|int
name|nr_match
parameter_list|,
name|char
modifier|*
modifier|*
name|match
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
name|int
name|retval
decl_stmt|;
name|unsigned
name|long
name|cutoff
init|=
literal|0
decl_stmt|;
name|track_object_refs
operator|=
literal|0
expr_stmt|;
name|save_commit_buffer
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|ref
operator|=
operator|*
name|refs
init|;
name|ref
condition|;
name|ref
operator|=
name|ref
operator|->
name|next
control|)
block|{
name|struct
name|object
modifier|*
name|o
decl_stmt|;
name|o
operator|=
name|parse_object
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|o
condition|)
continue|continue;
comment|/* We already have it -- which may mean that we were 		 * in sync with the other side at some time after 		 * that (it is OK if we guess wrong here). 		 */
if|if
condition|(
name|o
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
name|o
decl_stmt|;
if|if
condition|(
operator|!
name|cutoff
operator|||
name|cutoff
operator|<
name|commit
operator|->
name|date
condition|)
name|cutoff
operator|=
name|commit
operator|->
name|date
expr_stmt|;
block|}
block|}
name|for_each_ref
argument_list|(
name|mark_complete
argument_list|)
expr_stmt|;
if|if
condition|(
name|cutoff
condition|)
name|mark_recent_complete_commits
argument_list|(
name|cutoff
argument_list|)
expr_stmt|;
comment|/* 	 * Mark all complete remote refs as common refs. 	 * Don't mark them common yet; the server has to be told so first. 	 */
for|for
control|(
name|ref
operator|=
operator|*
name|refs
init|;
name|ref
condition|;
name|ref
operator|=
name|ref
operator|->
name|next
control|)
block|{
name|struct
name|object
modifier|*
name|o
init|=
name|deref_tag
argument_list|(
name|lookup_object
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|)
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|o
operator|||
name|o
operator|->
name|type
operator|!=
name|commit_type
operator|||
operator|!
operator|(
name|o
operator|->
name|flags
operator|&
name|COMPLETE
operator|)
condition|)
continue|continue;
if|if
condition|(
operator|!
operator|(
name|o
operator|->
name|flags
operator|&
name|SEEN
operator|)
condition|)
block|{
name|rev_list_push
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o
argument_list|,
name|COMMON_REF
operator||
name|SEEN
argument_list|)
expr_stmt|;
name|mark_common
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o
argument_list|,
literal|1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
name|filter_refs
argument_list|(
name|refs
argument_list|,
name|nr_match
argument_list|,
name|match
argument_list|)
expr_stmt|;
for|for
control|(
name|retval
operator|=
literal|1
operator|,
name|ref
operator|=
operator|*
name|refs
init|;
name|ref
condition|;
name|ref
operator|=
name|ref
operator|->
name|next
control|)
block|{
specifier|const
name|unsigned
name|char
modifier|*
name|remote
init|=
name|ref
operator|->
name|old_sha1
decl_stmt|;
name|unsigned
name|char
name|local
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|object
modifier|*
name|o
decl_stmt|;
name|o
operator|=
name|lookup_object
argument_list|(
name|remote
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|o
operator|||
operator|!
operator|(
name|o
operator|->
name|flags
operator|&
name|COMPLETE
operator|)
condition|)
block|{
name|retval
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
name|verbose
condition|)
continue|continue;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"want %s (%s)\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|remote
argument_list|)
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|memcpy
argument_list|(
name|ref
operator|->
name|new_sha1
argument_list|,
name|local
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|verbose
condition|)
continue|continue;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"already have %s (%s)\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|remote
argument_list|)
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|fetch_pack
specifier|static
name|int
name|fetch_pack
parameter_list|(
name|int
name|fd
index|[
literal|2
index|]
parameter_list|,
name|int
name|nr_match
parameter_list|,
name|char
modifier|*
modifier|*
name|match
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|status
decl_stmt|;
name|get_remote_heads
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
operator|&
name|ref
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|server_supports
argument_list|(
literal|"multi_ack"
argument_list|)
condition|)
block|{
if|if
condition|(
name|verbose
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Server supports multi_ack\n"
argument_list|)
expr_stmt|;
name|multi_ack
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|ref
condition|)
block|{
name|packet_flush
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"no matching remote head"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|everything_local
argument_list|(
operator|&
name|ref
argument_list|,
name|nr_match
argument_list|,
name|match
argument_list|)
condition|)
block|{
name|packet_flush
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
goto|goto
name|all_done
goto|;
block|}
if|if
condition|(
name|find_common
argument_list|(
name|fd
argument_list|,
name|sha1
argument_list|,
name|ref
argument_list|)
operator|<
literal|0
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"warning: no common commits\n"
argument_list|)
expr_stmt|;
if|if
condition|(
name|keep_pack
condition|)
name|status
operator|=
name|receive_keep_pack
argument_list|(
name|fd
argument_list|,
literal|"git-fetch-pack"
argument_list|,
name|quiet
argument_list|)
expr_stmt|;
else|else
name|status
operator|=
name|receive_unpack_pack
argument_list|(
name|fd
argument_list|,
literal|"git-fetch-pack"
argument_list|,
name|quiet
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
condition|)
name|die
argument_list|(
literal|"git-fetch-pack: fetch failed."
argument_list|)
expr_stmt|;
name|all_done
label|:
while|while
condition|(
name|ref
condition|)
block|{
name|printf
argument_list|(
literal|"%s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|)
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|ref
operator|=
name|ref
operator|->
name|next
expr_stmt|;
block|}
return|return
literal|0
return|;
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
name|int
name|i
decl_stmt|,
name|ret
decl_stmt|,
name|nr_heads
decl_stmt|;
name|char
modifier|*
name|dest
init|=
name|NULL
decl_stmt|,
modifier|*
modifier|*
name|heads
decl_stmt|;
name|int
name|fd
index|[
literal|2
index|]
decl_stmt|;
name|pid_t
name|pid
decl_stmt|;
name|setup_git_directory
argument_list|()
expr_stmt|;
name|nr_heads
operator|=
literal|0
expr_stmt|;
name|heads
operator|=
name|NULL
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
literal|"--exec="
argument_list|,
name|arg
argument_list|,
literal|7
argument_list|)
condition|)
block|{
name|exec
operator|=
name|arg
operator|+
literal|7
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"--quiet"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"-q"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|quiet
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
literal|"--keep"
argument_list|,
name|arg
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"-k"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|keep_pack
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
literal|"--thin"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|use_thin_pack
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
literal|"-v"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|verbose
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|fetch_pack_usage
argument_list|)
expr_stmt|;
block|}
name|dest
operator|=
name|arg
expr_stmt|;
name|heads
operator|=
name|argv
operator|+
name|i
operator|+
literal|1
expr_stmt|;
name|nr_heads
operator|=
name|argc
operator|-
name|i
operator|-
literal|1
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|dest
condition|)
name|usage
argument_list|(
name|fetch_pack_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|keep_pack
condition|)
name|use_thin_pack
operator|=
literal|0
expr_stmt|;
name|pid
operator|=
name|git_connect
argument_list|(
name|fd
argument_list|,
name|dest
argument_list|,
name|exec
argument_list|)
expr_stmt|;
if|if
condition|(
name|pid
operator|<
literal|0
condition|)
return|return
literal|1
return|;
name|ret
operator|=
name|fetch_pack
argument_list|(
name|fd
argument_list|,
name|nr_heads
argument_list|,
name|heads
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|finish_connect
argument_list|(
name|pid
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
operator|&&
name|nr_heads
condition|)
block|{
comment|/* If the heads to pull were given, we should have 		 * consumed all of them by matching the remote. 		 * Otherwise, 'git-fetch remote no-such-ref' would 		 * silently succeed without issuing an error. 		 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_heads
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|heads
index|[
name|i
index|]
operator|&&
name|heads
index|[
name|i
index|]
index|[
literal|0
index|]
condition|)
block|{
name|error
argument_list|(
literal|"no such remote ref %s"
argument_list|,
name|heads
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
block|}
block|}
return|return
name|ret
return|;
block|}
end_function

end_unit

