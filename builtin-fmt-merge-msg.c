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
literal|"git fmt-merge-msg [--log|--no-log] [--file<file>]"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|merge_summary
specifier|static
name|int
name|merge_summary
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
specifier|static
name|int
name|found_merge_log
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"merge.log"
argument_list|,
name|key
argument_list|)
condition|)
block|{
name|found_merge_log
operator|=
literal|1
expr_stmt|;
name|merge_summary
operator|=
name|git_config_bool
argument_list|(
name|key
argument_list|,
name|value
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|found_merge_log
operator|&&
operator|!
name|strcmp
argument_list|(
literal|"merge.summary"
argument_list|,
name|key
argument_list|)
condition|)
name|merge_summary
operator|=
name|git_config_bool
argument_list|(
name|key
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_struct
DECL|struct|list
struct|struct
name|list
block|{
DECL|member|list
name|char
modifier|*
modifier|*
name|list
decl_stmt|;
DECL|member|payload
name|void
modifier|*
modifier|*
name|payload
decl_stmt|;
DECL|member|nr
DECL|member|alloc
name|unsigned
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|append_to_list
specifier|static
name|void
name|append_to_list
parameter_list|(
name|struct
name|list
modifier|*
name|list
parameter_list|,
name|char
modifier|*
name|value
parameter_list|,
name|void
modifier|*
name|payload
parameter_list|)
block|{
if|if
condition|(
name|list
operator|->
name|nr
operator|==
name|list
operator|->
name|alloc
condition|)
block|{
name|list
operator|->
name|alloc
operator|+=
literal|32
expr_stmt|;
name|list
operator|->
name|list
operator|=
name|xrealloc
argument_list|(
name|list
operator|->
name|list
argument_list|,
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
operator|*
name|list
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|list
operator|->
name|payload
operator|=
name|xrealloc
argument_list|(
name|list
operator|->
name|payload
argument_list|,
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
operator|*
name|list
operator|->
name|alloc
argument_list|)
expr_stmt|;
block|}
name|list
operator|->
name|payload
index|[
name|list
operator|->
name|nr
index|]
operator|=
name|payload
expr_stmt|;
name|list
operator|->
name|list
index|[
name|list
operator|->
name|nr
operator|++
index|]
operator|=
name|value
expr_stmt|;
block|}
end_function

begin_function
DECL|function|find_in_list
specifier|static
name|int
name|find_in_list
parameter_list|(
name|struct
name|list
modifier|*
name|list
parameter_list|,
name|char
modifier|*
name|value
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
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
condition|;
name|i
operator|++
control|)
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|list
operator|->
name|list
index|[
name|i
index|]
argument_list|,
name|value
argument_list|)
condition|)
return|return
name|i
return|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|free_list
specifier|static
name|void
name|free_list
parameter_list|(
name|struct
name|list
modifier|*
name|list
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
name|list
operator|->
name|alloc
operator|==
literal|0
condition|)
return|return;
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
condition|;
name|i
operator|++
control|)
block|{
name|free
argument_list|(
name|list
operator|->
name|list
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|list
operator|->
name|payload
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|list
operator|->
name|list
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|list
operator|->
name|payload
argument_list|)
expr_stmt|;
name|list
operator|->
name|nr
operator|=
name|list
operator|->
name|alloc
operator|=
literal|0
expr_stmt|;
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
name|list
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

begin_decl_stmt
DECL|variable|srcs
specifier|static
name|struct
name|list
name|srcs
init|=
block|{
name|NULL
block|,
name|NULL
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|origins
specifier|static
name|struct
name|list
name|origins
init|=
block|{
name|NULL
block|,
name|NULL
block|,
literal|0
block|,
literal|0
block|}
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
name|i
operator|=
name|find_in_list
argument_list|(
operator|&
name|srcs
argument_list|,
name|src
argument_list|)
expr_stmt|;
if|if
condition|(
name|i
operator|<
literal|0
condition|)
block|{
name|i
operator|=
name|srcs
operator|.
name|nr
expr_stmt|;
name|append_to_list
argument_list|(
operator|&
name|srcs
argument_list|,
name|xstrdup
argument_list|(
name|src
argument_list|)
argument_list|,
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
argument_list|)
expr_stmt|;
block|}
name|src_data
operator|=
name|srcs
operator|.
name|payload
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|pulling_head
condition|)
block|{
name|origin
operator|=
name|xstrdup
argument_list|(
name|src
argument_list|)
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
name|xstrdup
argument_list|(
name|line
operator|+
literal|7
argument_list|)
expr_stmt|;
name|append_to_list
argument_list|(
operator|&
name|src_data
operator|->
name|branch
argument_list|,
name|origin
argument_list|,
name|NULL
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
name|append_to_list
argument_list|(
operator|&
name|src_data
operator|->
name|tag
argument_list|,
name|xstrdup
argument_list|(
name|origin
operator|+
literal|4
argument_list|)
argument_list|,
name|NULL
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
literal|"remote branch "
argument_list|)
condition|)
block|{
name|origin
operator|=
name|xstrdup
argument_list|(
name|line
operator|+
literal|14
argument_list|)
expr_stmt|;
name|append_to_list
argument_list|(
operator|&
name|src_data
operator|->
name|r_branch
argument_list|,
name|origin
argument_list|,
name|NULL
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
name|xstrdup
argument_list|(
name|src
argument_list|)
expr_stmt|;
name|append_to_list
argument_list|(
operator|&
name|src_data
operator|->
name|generic
argument_list|,
name|xstrdup
argument_list|(
name|line
argument_list|)
argument_list|,
name|NULL
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
block|{
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
name|origin
operator|=
name|xstrdup
argument_list|(
name|origin
argument_list|)
expr_stmt|;
block|}
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
name|append_to_list
argument_list|(
operator|&
name|origins
argument_list|,
name|origin
argument_list|,
name|sha1
argument_list|)
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
name|list
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
name|list
index|[
literal|0
index|]
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
name|list
index|[
name|i
index|]
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
name|list
index|[
name|list
operator|->
name|nr
operator|-
literal|1
index|]
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
name|list
name|subjects
init|=
block|{
name|NULL
block|,
name|NULL
block|,
literal|0
block|,
literal|0
block|}
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
name|char
modifier|*
name|oneline
decl_stmt|,
modifier|*
name|bol
decl_stmt|,
modifier|*
name|eol
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
name|bol
operator|=
name|strstr
argument_list|(
name|commit
operator|->
name|buffer
argument_list|,
literal|"\n\n"
argument_list|)
expr_stmt|;
if|if
condition|(
name|bol
condition|)
block|{
name|unsigned
name|char
name|c
decl_stmt|;
do|do
block|{
name|c
operator|=
operator|*
operator|++
name|bol
expr_stmt|;
block|}
do|while
condition|(
name|isspace
argument_list|(
name|c
argument_list|)
condition|)
do|;
if|if
condition|(
operator|!
name|c
condition|)
name|bol
operator|=
name|NULL
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|bol
condition|)
block|{
name|append_to_list
argument_list|(
operator|&
name|subjects
argument_list|,
name|xstrdup
argument_list|(
name|sha1_to_hex
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|eol
operator|=
name|strchr
argument_list|(
name|bol
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
if|if
condition|(
name|eol
condition|)
block|{
name|oneline
operator|=
name|xmemdupz
argument_list|(
name|bol
argument_list|,
name|eol
operator|-
name|bol
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|oneline
operator|=
name|xstrdup
argument_list|(
name|bol
argument_list|)
expr_stmt|;
block|}
name|append_to_list
argument_list|(
operator|&
name|subjects
argument_list|,
name|oneline
argument_list|,
name|NULL
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
name|list
index|[
name|i
index|]
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
name|free_list
argument_list|(
operator|&
name|subjects
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|fmt_merge_msg
name|int
name|fmt_merge_msg
parameter_list|(
name|int
name|merge_summary
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
parameter_list|)
block|{
name|int
name|limit
init|=
literal|20
decl_stmt|,
name|i
init|=
literal|0
decl_stmt|,
name|pos
init|=
literal|0
decl_stmt|;
name|char
modifier|*
name|sep
init|=
literal|""
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
name|payload
index|[
name|i
index|]
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
name|list
index|[
name|i
index|]
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
literal|"remote branch "
argument_list|,
literal|"remote branches "
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
name|list
index|[
name|i
index|]
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
name|list
index|[
name|i
index|]
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
if|if
condition|(
name|merge_summary
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
name|lookup_commit
argument_list|(
name|head_sha1
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
name|list
index|[
name|i
index|]
argument_list|,
name|origins
operator|.
name|payload
index|[
name|i
index|]
argument_list|,
name|head
argument_list|,
operator|&
name|rev
argument_list|,
name|limit
argument_list|,
name|out
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
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
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"log"
argument_list|,
operator|&
name|merge_summary
argument_list|,
literal|"populate log with the shortlog"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"summary"
argument_list|,
operator|&
name|merge_summary
argument_list|,
literal|"alias for --log"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|'F'
argument_list|,
literal|"file"
argument_list|,
operator|&
name|inpath
argument_list|,
literal|"file"
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
name|die
argument_list|(
literal|"cannot open %s"
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
name|die
argument_list|(
literal|"could not read input file %s"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|ret
operator|=
name|fmt_merge_msg
argument_list|(
name|merge_summary
argument_list|,
operator|&
name|input
argument_list|,
operator|&
name|output
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

