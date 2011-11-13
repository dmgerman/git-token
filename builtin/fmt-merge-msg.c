begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
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
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_decl_stmt
DECL|variable|fmt_merge_msg_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|fmt_merge_msg_usage
index|[]
init|=
block|{
literal|"git fmt-merge-msg [-m<message>] [--log[=<n>]|--no-log] [--file<file>]"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|shortlog_len
specifier|static
name|int
name|shortlog_len
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|fmt_merge_msg_config
specifier|static
name|int
name|fmt_merge_msg_config
parameter_list|(
specifier|const
name|char
modifier|*
name|key
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"merge.log"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"merge.summary"
argument_list|)
condition|)
block|{
name|int
name|is_bool
decl_stmt|;
name|shortlog_len
operator|=
name|git_config_bool_or_int
argument_list|(
name|key
argument_list|,
name|value
argument_list|,
operator|&
name|is_bool
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_bool
operator|&&
name|shortlog_len
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"%s: negative length %s"
argument_list|,
name|key
argument_list|,
name|value
argument_list|)
return|;
if|if
condition|(
name|is_bool
operator|&&
name|shortlog_len
condition|)
name|shortlog_len
operator|=
name|DEFAULT_MERGE_LOG_LEN
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_struct
DECL|struct|src_data
struct|struct
name|src_data
block|{
DECL|member|branch
DECL|member|tag
DECL|member|r_branch
DECL|member|generic
name|struct
name|string_list
name|branch
decl_stmt|,
name|tag
decl_stmt|,
name|r_branch
decl_stmt|,
name|generic
decl_stmt|;
DECL|member|head_status
name|int
name|head_status
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|init_src_data
specifier|static
name|void
name|init_src_data
parameter_list|(
name|struct
name|src_data
modifier|*
name|data
parameter_list|)
block|{
name|data
operator|->
name|branch
operator|.
name|strdup_strings
operator|=
literal|1
expr_stmt|;
name|data
operator|->
name|tag
operator|.
name|strdup_strings
operator|=
literal|1
expr_stmt|;
name|data
operator|->
name|r_branch
operator|.
name|strdup_strings
operator|=
literal|1
expr_stmt|;
name|data
operator|->
name|generic
operator|.
name|strdup_strings
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|srcs
specifier|static
name|struct
name|string_list
name|srcs
init|=
name|STRING_LIST_INIT_DUP
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|origins
specifier|static
name|struct
name|string_list
name|origins
init|=
name|STRING_LIST_INIT_DUP
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|handle_line
specifier|static
name|int
name|handle_line
parameter_list|(
name|char
modifier|*
name|line
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|len
init|=
name|strlen
argument_list|(
name|line
argument_list|)
decl_stmt|;
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|char
modifier|*
name|src
decl_stmt|,
modifier|*
name|origin
decl_stmt|;
name|struct
name|src_data
modifier|*
name|src_data
decl_stmt|;
name|struct
name|string_list_item
modifier|*
name|item
decl_stmt|;
name|int
name|pulling_head
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|len
operator|<
literal|43
operator|||
name|line
index|[
literal|40
index|]
operator|!=
literal|'\t'
condition|)
return|return
literal|1
return|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|line
operator|+
literal|41
argument_list|,
literal|"not-for-merge"
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|line
index|[
literal|41
index|]
operator|!=
literal|'\t'
condition|)
return|return
literal|2
return|;
name|line
index|[
literal|40
index|]
operator|=
literal|0
expr_stmt|;
name|sha1
operator|=
name|xmalloc
argument_list|(
literal|20
argument_list|)
expr_stmt|;
name|i
operator|=
name|get_sha1
argument_list|(
name|line
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|line
index|[
literal|40
index|]
operator|=
literal|'\t'
expr_stmt|;
if|if
condition|(
name|i
condition|)
return|return
literal|3
return|;
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
name|len
operator|-
literal|1
index|]
operator|=
literal|0
expr_stmt|;
name|line
operator|+=
literal|42
expr_stmt|;
name|src
operator|=
name|strstr
argument_list|(
name|line
argument_list|,
literal|" of "
argument_list|)
expr_stmt|;
if|if
condition|(
name|src
condition|)
block|{
operator|*
name|src
operator|=
literal|0
expr_stmt|;
name|src
operator|+=
literal|4
expr_stmt|;
name|pulling_head
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|src
operator|=
name|line
expr_stmt|;
name|pulling_head
operator|=
literal|1
expr_stmt|;
block|}
name|item
operator|=
name|unsorted_string_list_lookup
argument_list|(
operator|&
name|srcs
argument_list|,
name|src
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|item
condition|)
block|{
name|item
operator|=
name|string_list_append
argument_list|(
operator|&
name|srcs
argument_list|,
name|src
argument_list|)
expr_stmt|;
name|item
operator|->
name|util
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|src_data
argument_list|)
argument_list|)
expr_stmt|;
name|init_src_data
argument_list|(
name|item
operator|->
name|util
argument_list|)
expr_stmt|;
block|}
name|src_data
operator|=
name|item
operator|->
name|util
expr_stmt|;
if|if
condition|(
name|pulling_head
condition|)
block|{
name|origin
operator|=
name|src
expr_stmt|;
name|src_data
operator|->
name|head_status
operator||=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|line
argument_list|,
literal|"branch "
argument_list|)
condition|)
block|{
name|origin
operator|=
name|line
operator|+
literal|7
expr_stmt|;
name|string_list_append
argument_list|(
operator|&
name|src_data
operator|->
name|branch
argument_list|,
name|origin
argument_list|)
expr_stmt|;
name|src_data
operator|->
name|head_status
operator||=
literal|2
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|line
argument_list|,
literal|"tag "
argument_list|)
condition|)
block|{
name|origin
operator|=
name|line
expr_stmt|;
name|string_list_append
argument_list|(
operator|&
name|src_data
operator|->
name|tag
argument_list|,
name|origin
operator|+
literal|4
argument_list|)
expr_stmt|;
name|src_data
operator|->
name|head_status
operator||=
literal|2
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|line
argument_list|,
literal|"remote-tracking branch "
argument_list|)
condition|)
block|{
name|origin
operator|=
name|line
operator|+
name|strlen
argument_list|(
literal|"remote-tracking branch "
argument_list|)
expr_stmt|;
name|string_list_append
argument_list|(
operator|&
name|src_data
operator|->
name|r_branch
argument_list|,
name|origin
argument_list|)
expr_stmt|;
name|src_data
operator|->
name|head_status
operator||=
literal|2
expr_stmt|;
block|}
else|else
block|{
name|origin
operator|=
name|src
expr_stmt|;
name|string_list_append
argument_list|(
operator|&
name|src_data
operator|->
name|generic
argument_list|,
name|line
argument_list|)
expr_stmt|;
name|src_data
operator|->
name|head_status
operator||=
literal|2
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"."
argument_list|,
name|src
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|src
argument_list|,
name|origin
argument_list|)
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|origin
argument_list|)
decl_stmt|;
if|if
condition|(
name|origin
index|[
literal|0
index|]
operator|==
literal|'\''
operator|&&
name|origin
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\''
condition|)
name|origin
operator|=
name|xmemdupz
argument_list|(
name|origin
operator|+
literal|1
argument_list|,
name|len
operator|-
literal|2
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|char
modifier|*
name|new_origin
init|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|origin
argument_list|)
operator|+
name|strlen
argument_list|(
name|src
argument_list|)
operator|+
literal|5
argument_list|)
decl_stmt|;
name|sprintf
argument_list|(
name|new_origin
argument_list|,
literal|"%s of %s"
argument_list|,
name|origin
argument_list|,
name|src
argument_list|)
expr_stmt|;
name|origin
operator|=
name|new_origin
expr_stmt|;
block|}
name|string_list_append
argument_list|(
operator|&
name|origins
argument_list|,
name|origin
argument_list|)
operator|->
name|util
operator|=
name|sha1
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|print_joined
specifier|static
name|void
name|print_joined
parameter_list|(
specifier|const
name|char
modifier|*
name|singular
parameter_list|,
specifier|const
name|char
modifier|*
name|plural
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|struct
name|strbuf
modifier|*
name|out
parameter_list|)
block|{
if|if
condition|(
name|list
operator|->
name|nr
operator|==
literal|0
condition|)
return|return;
if|if
condition|(
name|list
operator|->
name|nr
operator|==
literal|1
condition|)
block|{
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|"%s%s"
argument_list|,
name|singular
argument_list|,
name|list
operator|->
name|items
index|[
literal|0
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|int
name|i
decl_stmt|;
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
name|plural
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
name|list
operator|->
name|nr
operator|-
literal|1
condition|;
name|i
operator|++
control|)
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|"%s%s"
argument_list|,
name|i
operator|>
literal|0
condition|?
literal|", "
else|:
literal|""
argument_list|,
name|list
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|" and %s"
argument_list|,
name|list
operator|->
name|items
index|[
name|list
operator|->
name|nr
operator|-
literal|1
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|shortlog
specifier|static
name|void
name|shortlog
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|struct
name|commit
modifier|*
name|head
parameter_list|,
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|,
name|int
name|limit
parameter_list|,
name|struct
name|strbuf
modifier|*
name|out
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|count
init|=
literal|0
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|struct
name|object
modifier|*
name|branch
decl_stmt|;
name|struct
name|string_list
name|subjects
init|=
name|STRING_LIST_INIT_DUP
decl_stmt|;
name|int
name|flags
init|=
name|UNINTERESTING
operator||
name|TREESAME
operator||
name|SEEN
operator||
name|SHOWN
operator||
name|ADDED
decl_stmt|;
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|branch
operator|=
name|deref_tag
argument_list|(
name|parse_object
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
literal|40
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|branch
operator|||
name|branch
operator|->
name|type
operator|!=
name|OBJ_COMMIT
condition|)
return|return;
name|setup_revisions
argument_list|(
literal|0
argument_list|,
name|NULL
argument_list|,
name|rev
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|rev
operator|->
name|ignore_merges
operator|=
literal|1
expr_stmt|;
name|add_pending_object
argument_list|(
name|rev
argument_list|,
name|branch
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|add_pending_object
argument_list|(
name|rev
argument_list|,
operator|&
name|head
operator|->
name|object
argument_list|,
literal|"^HEAD"
argument_list|)
expr_stmt|;
name|head
operator|->
name|object
operator|.
name|flags
operator||=
name|UNINTERESTING
expr_stmt|;
if|if
condition|(
name|prepare_revision_walk
argument_list|(
name|rev
argument_list|)
condition|)
name|die
argument_list|(
literal|"revision walk setup failed"
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|commit
operator|=
name|get_revision
argument_list|(
name|rev
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|struct
name|pretty_print_context
name|ctx
init|=
block|{
literal|0
block|}
decl_stmt|;
comment|/* ignore merges */
if|if
condition|(
name|commit
operator|->
name|parents
operator|&&
name|commit
operator|->
name|parents
operator|->
name|next
condition|)
continue|continue;
name|count
operator|++
expr_stmt|;
if|if
condition|(
name|subjects
operator|.
name|nr
operator|>
name|limit
condition|)
continue|continue;
name|format_commit_message
argument_list|(
name|commit
argument_list|,
literal|"%s"
argument_list|,
operator|&
name|sb
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
name|strbuf_ltrim
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|sb
operator|.
name|len
condition|)
name|string_list_append
argument_list|(
operator|&
name|subjects
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
else|else
name|string_list_append
argument_list|(
operator|&
name|subjects
argument_list|,
name|strbuf_detach
argument_list|(
operator|&
name|sb
argument_list|,
name|NULL
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|count
operator|>
name|limit
condition|)
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|"\n* %s: (%d commits)\n"
argument_list|,
name|name
argument_list|,
name|count
argument_list|)
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|"\n* %s:\n"
argument_list|,
name|name
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
name|subjects
operator|.
name|nr
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|i
operator|>=
name|limit
condition|)
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|"  ...\n"
argument_list|)
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|"  %s\n"
argument_list|,
name|subjects
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
name|clear_commit_marks
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|branch
argument_list|,
name|flags
argument_list|)
expr_stmt|;
name|clear_commit_marks
argument_list|(
name|head
argument_list|,
name|flags
argument_list|)
expr_stmt|;
name|free_commit_list
argument_list|(
name|rev
operator|->
name|commits
argument_list|)
expr_stmt|;
name|rev
operator|->
name|commits
operator|=
name|NULL
expr_stmt|;
name|rev
operator|->
name|pending
operator|.
name|nr
operator|=
literal|0
expr_stmt|;
name|string_list_clear
argument_list|(
operator|&
name|subjects
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|do_fmt_merge_msg_title
specifier|static
name|void
name|do_fmt_merge_msg_title
parameter_list|(
name|struct
name|strbuf
modifier|*
name|out
parameter_list|,
specifier|const
name|char
modifier|*
name|current_branch
parameter_list|)
block|{
name|int
name|i
init|=
literal|0
decl_stmt|;
name|char
modifier|*
name|sep
init|=
literal|""
decl_stmt|;
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
literal|"Merge "
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
name|srcs
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|src_data
modifier|*
name|src_data
init|=
name|srcs
operator|.
name|items
index|[
name|i
index|]
operator|.
name|util
decl_stmt|;
specifier|const
name|char
modifier|*
name|subsep
init|=
literal|""
decl_stmt|;
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
name|sep
argument_list|)
expr_stmt|;
name|sep
operator|=
literal|"; "
expr_stmt|;
if|if
condition|(
name|src_data
operator|->
name|head_status
operator|==
literal|1
condition|)
block|{
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
name|srcs
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|src_data
operator|->
name|head_status
operator|==
literal|3
condition|)
block|{
name|subsep
operator|=
literal|", "
expr_stmt|;
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
literal|"HEAD"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|src_data
operator|->
name|branch
operator|.
name|nr
condition|)
block|{
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
name|subsep
argument_list|)
expr_stmt|;
name|subsep
operator|=
literal|", "
expr_stmt|;
name|print_joined
argument_list|(
literal|"branch "
argument_list|,
literal|"branches "
argument_list|,
operator|&
name|src_data
operator|->
name|branch
argument_list|,
name|out
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|src_data
operator|->
name|r_branch
operator|.
name|nr
condition|)
block|{
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
name|subsep
argument_list|)
expr_stmt|;
name|subsep
operator|=
literal|", "
expr_stmt|;
name|print_joined
argument_list|(
literal|"remote-tracking branch "
argument_list|,
literal|"remote-tracking branches "
argument_list|,
operator|&
name|src_data
operator|->
name|r_branch
argument_list|,
name|out
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|src_data
operator|->
name|tag
operator|.
name|nr
condition|)
block|{
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
name|subsep
argument_list|)
expr_stmt|;
name|subsep
operator|=
literal|", "
expr_stmt|;
name|print_joined
argument_list|(
literal|"tag "
argument_list|,
literal|"tags "
argument_list|,
operator|&
name|src_data
operator|->
name|tag
argument_list|,
name|out
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|src_data
operator|->
name|generic
operator|.
name|nr
condition|)
block|{
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
name|subsep
argument_list|)
expr_stmt|;
name|print_joined
argument_list|(
literal|"commit "
argument_list|,
literal|"commits "
argument_list|,
operator|&
name|src_data
operator|->
name|generic
argument_list|,
name|out
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|strcmp
argument_list|(
literal|"."
argument_list|,
name|srcs
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
condition|)
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|" of %s"
argument_list|,
name|srcs
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"master"
argument_list|,
name|current_branch
argument_list|)
condition|)
name|strbuf_addch
argument_list|(
name|out
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|" into %s\n"
argument_list|,
name|current_branch
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|do_fmt_merge_msg
specifier|static
name|int
name|do_fmt_merge_msg
parameter_list|(
name|int
name|merge_title
parameter_list|,
name|struct
name|strbuf
modifier|*
name|in
parameter_list|,
name|struct
name|strbuf
modifier|*
name|out
parameter_list|,
name|int
name|shortlog_len
parameter_list|)
block|{
name|int
name|i
init|=
literal|0
decl_stmt|,
name|pos
init|=
literal|0
decl_stmt|;
name|unsigned
name|char
name|head_sha1
index|[
literal|20
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|current_branch
decl_stmt|;
comment|/* get current branch */
name|current_branch
operator|=
name|resolve_ref
argument_list|(
literal|"HEAD"
argument_list|,
name|head_sha1
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|current_branch
condition|)
name|die
argument_list|(
literal|"No current branch"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|current_branch
argument_list|,
literal|"refs/heads/"
argument_list|)
condition|)
name|current_branch
operator|+=
literal|11
expr_stmt|;
name|current_branch
operator|=
name|xstrdup
argument_list|(
name|current_branch
argument_list|)
expr_stmt|;
comment|/* get a line */
while|while
condition|(
name|pos
operator|<
name|in
operator|->
name|len
condition|)
block|{
name|int
name|len
decl_stmt|;
name|char
modifier|*
name|newline
decl_stmt|,
modifier|*
name|p
init|=
name|in
operator|->
name|buf
operator|+
name|pos
decl_stmt|;
name|newline
operator|=
name|strchr
argument_list|(
name|p
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
name|len
operator|=
name|newline
condition|?
name|newline
operator|-
name|p
else|:
name|strlen
argument_list|(
name|p
argument_list|)
expr_stmt|;
name|pos
operator|+=
name|len
operator|+
operator|!
operator|!
name|newline
expr_stmt|;
name|i
operator|++
expr_stmt|;
name|p
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|handle_line
argument_list|(
name|p
argument_list|)
condition|)
name|die
argument_list|(
literal|"Error in line %d: %.*s"
argument_list|,
name|i
argument_list|,
name|len
argument_list|,
name|p
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|srcs
operator|.
name|nr
condition|)
block|{
name|free
argument_list|(
operator|(
name|char
operator|*
operator|)
name|current_branch
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|merge_title
condition|)
name|do_fmt_merge_msg_title
argument_list|(
name|out
argument_list|,
name|current_branch
argument_list|)
expr_stmt|;
if|if
condition|(
name|shortlog_len
condition|)
block|{
name|struct
name|commit
modifier|*
name|head
decl_stmt|;
name|struct
name|rev_info
name|rev
decl_stmt|;
name|head
operator|=
name|lookup_commit_or_die
argument_list|(
name|head_sha1
argument_list|,
literal|"HEAD"
argument_list|)
expr_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|rev
operator|.
name|commit_format
operator|=
name|CMIT_FMT_ONELINE
expr_stmt|;
name|rev
operator|.
name|ignore_merges
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|limited
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|suffixcmp
argument_list|(
name|out
operator|->
name|buf
argument_list|,
literal|"\n"
argument_list|)
condition|)
name|strbuf_addch
argument_list|(
name|out
argument_list|,
literal|'\n'
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
name|origins
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|shortlog
argument_list|(
name|origins
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|,
name|origins
operator|.
name|items
index|[
name|i
index|]
operator|.
name|util
argument_list|,
name|head
argument_list|,
operator|&
name|rev
argument_list|,
name|shortlog_len
argument_list|,
name|out
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
operator|(
name|char
operator|*
operator|)
name|current_branch
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fmt_merge_msg
name|int
name|fmt_merge_msg
parameter_list|(
name|struct
name|strbuf
modifier|*
name|in
parameter_list|,
name|struct
name|strbuf
modifier|*
name|out
parameter_list|,
name|int
name|merge_title
parameter_list|,
name|int
name|shortlog_len
parameter_list|)
block|{
return|return
name|do_fmt_merge_msg
argument_list|(
name|merge_title
argument_list|,
name|in
argument_list|,
name|out
argument_list|,
name|shortlog_len
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|cmd_fmt_merge_msg
name|int
name|cmd_fmt_merge_msg
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
specifier|const
name|char
modifier|*
name|inpath
init|=
name|NULL
decl_stmt|;
specifier|const
name|char
modifier|*
name|message
init|=
name|NULL
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
block|{
name|OPTION_INTEGER
block|,
literal|0
block|,
literal|"log"
block|,
operator|&
name|shortlog_len
block|,
literal|"n"
block|,
literal|"populate log with at most<n> entries from shortlog"
block|,
name|PARSE_OPT_OPTARG
block|,
name|NULL
block|,
name|DEFAULT_MERGE_LOG_LEN
block|}
block|,
block|{
name|OPTION_INTEGER
block|,
literal|0
block|,
literal|"summary"
block|,
operator|&
name|shortlog_len
block|,
literal|"n"
block|,
literal|"alias for --log (deprecated)"
block|,
name|PARSE_OPT_OPTARG
operator||
name|PARSE_OPT_HIDDEN
block|,
name|NULL
block|,
name|DEFAULT_MERGE_LOG_LEN
block|}
block|,
name|OPT_STRING
argument_list|(
literal|'m'
argument_list|,
literal|"message"
argument_list|,
operator|&
name|message
argument_list|,
literal|"text"
argument_list|,
literal|"use<text> as start of message"
argument_list|)
block|,
name|OPT_FILENAME
argument_list|(
literal|'F'
argument_list|,
literal|"file"
argument_list|,
operator|&
name|inpath
argument_list|,
literal|"file to read from"
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|FILE
modifier|*
name|in
init|=
name|stdin
decl_stmt|;
name|struct
name|strbuf
name|input
init|=
name|STRBUF_INIT
decl_stmt|,
name|output
init|=
name|STRBUF_INIT
decl_stmt|;
name|int
name|ret
decl_stmt|;
name|git_config
argument_list|(
name|fmt_merge_msg_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
name|options
argument_list|,
name|fmt_merge_msg_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|>
literal|0
condition|)
name|usage_with_options
argument_list|(
name|fmt_merge_msg_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
name|message
operator|&&
operator|!
name|shortlog_len
condition|)
block|{
name|char
name|nl
init|=
literal|'\n'
decl_stmt|;
name|write_in_full
argument_list|(
name|STDOUT_FILENO
argument_list|,
name|message
argument_list|,
name|strlen
argument_list|(
name|message
argument_list|)
argument_list|)
expr_stmt|;
name|write_in_full
argument_list|(
name|STDOUT_FILENO
argument_list|,
operator|&
name|nl
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|shortlog_len
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Negative --log=%d"
argument_list|,
name|shortlog_len
argument_list|)
expr_stmt|;
if|if
condition|(
name|inpath
operator|&&
name|strcmp
argument_list|(
name|inpath
argument_list|,
literal|"-"
argument_list|)
condition|)
block|{
name|in
operator|=
name|fopen
argument_list|(
name|inpath
argument_list|,
literal|"r"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|in
condition|)
name|die_errno
argument_list|(
literal|"cannot open '%s'"
argument_list|,
name|inpath
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|strbuf_read
argument_list|(
operator|&
name|input
argument_list|,
name|fileno
argument_list|(
name|in
argument_list|)
argument_list|,
literal|0
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"could not read input file"
argument_list|)
expr_stmt|;
if|if
condition|(
name|message
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|output
argument_list|,
name|message
argument_list|)
expr_stmt|;
name|ret
operator|=
name|fmt_merge_msg
argument_list|(
operator|&
name|input
argument_list|,
operator|&
name|output
argument_list|,
name|message
condition|?
literal|0
else|:
literal|1
argument_list|,
name|shortlog_len
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
return|return
name|ret
return|;
name|write_in_full
argument_list|(
name|STDOUT_FILENO
argument_list|,
name|output
operator|.
name|buf
argument_list|,
name|output
operator|.
name|len
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

