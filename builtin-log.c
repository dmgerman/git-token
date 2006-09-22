begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git log" and related commands (show, whatchanged)  *  * (C) Copyright 2006 Linus Torvalds  *		 2006 Junio Hamano  */
end_comment

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
file|"log-tree.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|<time.h>
end_include

begin_include
include|#
directive|include
file|<sys/time.h>
end_include

begin_comment
comment|/* this is in builtin-diff.c */
end_comment

begin_function_decl
name|void
name|add_head
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|cmd_log_init
specifier|static
name|void
name|cmd_log_init
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
parameter_list|,
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|)
block|{
name|rev
operator|->
name|abbrev
operator|=
name|DEFAULT_ABBREV
expr_stmt|;
name|rev
operator|->
name|commit_format
operator|=
name|CMIT_FMT_DEFAULT
expr_stmt|;
name|rev
operator|->
name|verbose_header
operator|=
literal|1
expr_stmt|;
name|argc
operator|=
name|setup_revisions
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|rev
argument_list|,
literal|"HEAD"
argument_list|)
expr_stmt|;
if|if
condition|(
name|rev
operator|->
name|diffopt
operator|.
name|pickaxe
operator|||
name|rev
operator|->
name|diffopt
operator|.
name|filter
condition|)
name|rev
operator|->
name|always_show_header
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|argc
operator|>
literal|1
condition|)
name|die
argument_list|(
literal|"unrecognized argument: %s"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_log_walk
specifier|static
name|int
name|cmd_log_walk
parameter_list|(
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|prepare_revision_walk
argument_list|(
name|rev
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
name|log_tree_commit
argument_list|(
name|rev
argument_list|,
name|commit
argument_list|)
expr_stmt|;
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
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_whatchanged
name|int
name|cmd_whatchanged
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
name|rev_info
name|rev
decl_stmt|;
name|git_config
argument_list|(
name|git_diff_ui_config
argument_list|)
expr_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|rev
operator|.
name|diff
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|recursive
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|simplify_history
operator|=
literal|0
expr_stmt|;
name|cmd_log_init
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
operator|&
name|rev
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|rev
operator|.
name|diffopt
operator|.
name|output_format
condition|)
name|rev
operator|.
name|diffopt
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_RAW
expr_stmt|;
return|return
name|cmd_log_walk
argument_list|(
operator|&
name|rev
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|cmd_show
name|int
name|cmd_show
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
name|rev_info
name|rev
decl_stmt|;
name|git_config
argument_list|(
name|git_diff_ui_config
argument_list|)
expr_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|rev
operator|.
name|diff
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|recursive
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|combine_merges
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|dense_combined_merges
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|always_show_header
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|ignore_merges
operator|=
literal|0
expr_stmt|;
name|rev
operator|.
name|no_walk
operator|=
literal|1
expr_stmt|;
name|cmd_log_init
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
operator|&
name|rev
argument_list|)
expr_stmt|;
return|return
name|cmd_log_walk
argument_list|(
operator|&
name|rev
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|cmd_log
name|int
name|cmd_log
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
name|rev_info
name|rev
decl_stmt|;
name|git_config
argument_list|(
name|git_diff_ui_config
argument_list|)
expr_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|rev
operator|.
name|always_show_header
operator|=
literal|1
expr_stmt|;
name|cmd_log_init
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
operator|&
name|rev
argument_list|)
expr_stmt|;
return|return
name|cmd_log_walk
argument_list|(
operator|&
name|rev
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|istitlechar
specifier|static
name|int
name|istitlechar
parameter_list|(
name|char
name|c
parameter_list|)
block|{
return|return
operator|(
name|c
operator|>=
literal|'a'
operator|&&
name|c
operator|<=
literal|'z'
operator|)
operator|||
operator|(
name|c
operator|>=
literal|'A'
operator|&&
name|c
operator|<=
literal|'Z'
operator|)
operator|||
operator|(
name|c
operator|>=
literal|'0'
operator|&&
name|c
operator|<=
literal|'9'
operator|)
operator|||
name|c
operator|==
literal|'.'
operator|||
name|c
operator|==
literal|'_'
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|extra_headers
specifier|static
name|char
modifier|*
name|extra_headers
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|extra_headers_size
specifier|static
name|int
name|extra_headers_size
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|git_format_config
specifier|static
name|int
name|git_format_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"format.headers"
argument_list|)
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|value
argument_list|)
decl_stmt|;
name|extra_headers_size
operator|+=
name|len
operator|+
literal|1
expr_stmt|;
name|extra_headers
operator|=
name|xrealloc
argument_list|(
name|extra_headers
argument_list|,
name|extra_headers_size
argument_list|)
expr_stmt|;
name|extra_headers
index|[
name|extra_headers_size
operator|-
name|len
operator|-
literal|1
index|]
operator|=
literal|0
expr_stmt|;
name|strcat
argument_list|(
name|extra_headers
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"diff.color"
argument_list|)
condition|)
block|{
return|return
literal|0
return|;
block|}
return|return
name|git_diff_ui_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|realstdout
specifier|static
name|FILE
modifier|*
name|realstdout
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|output_directory
specifier|static
specifier|const
name|char
modifier|*
name|output_directory
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|reopen_stdout
specifier|static
name|void
name|reopen_stdout
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|int
name|nr
parameter_list|,
name|int
name|keep_subject
parameter_list|)
block|{
name|char
name|filename
index|[
literal|1024
index|]
decl_stmt|;
name|char
modifier|*
name|sol
decl_stmt|;
name|int
name|len
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|output_directory
condition|)
block|{
name|strlcpy
argument_list|(
name|filename
argument_list|,
name|output_directory
argument_list|,
literal|1010
argument_list|)
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|filename
argument_list|)
expr_stmt|;
if|if
condition|(
name|filename
index|[
name|len
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
name|filename
index|[
name|len
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
block|}
name|sprintf
argument_list|(
name|filename
operator|+
name|len
argument_list|,
literal|"%04d"
argument_list|,
name|nr
argument_list|)
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|filename
argument_list|)
expr_stmt|;
name|sol
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
name|sol
condition|)
block|{
name|int
name|j
decl_stmt|,
name|space
init|=
literal|1
decl_stmt|;
name|sol
operator|+=
literal|2
expr_stmt|;
comment|/* strip [PATCH] or [PATCH blabla] */
if|if
condition|(
operator|!
name|keep_subject
operator|&&
operator|!
name|strncmp
argument_list|(
name|sol
argument_list|,
literal|"[PATCH"
argument_list|,
literal|6
argument_list|)
condition|)
block|{
name|char
modifier|*
name|eos
init|=
name|strchr
argument_list|(
name|sol
operator|+
literal|6
argument_list|,
literal|']'
argument_list|)
decl_stmt|;
if|if
condition|(
name|eos
condition|)
block|{
while|while
condition|(
name|isspace
argument_list|(
operator|*
name|eos
argument_list|)
condition|)
name|eos
operator|++
expr_stmt|;
name|sol
operator|=
name|eos
expr_stmt|;
block|}
block|}
for|for
control|(
name|j
operator|=
literal|0
init|;
name|len
operator|<
literal|1024
operator|-
literal|6
operator|&&
name|sol
index|[
name|j
index|]
operator|&&
name|sol
index|[
name|j
index|]
operator|!=
literal|'\n'
condition|;
name|j
operator|++
control|)
block|{
if|if
condition|(
name|istitlechar
argument_list|(
name|sol
index|[
name|j
index|]
argument_list|)
condition|)
block|{
if|if
condition|(
name|space
condition|)
block|{
name|filename
index|[
name|len
operator|++
index|]
operator|=
literal|'-'
expr_stmt|;
name|space
operator|=
literal|0
expr_stmt|;
block|}
name|filename
index|[
name|len
operator|++
index|]
operator|=
name|sol
index|[
name|j
index|]
expr_stmt|;
if|if
condition|(
name|sol
index|[
name|j
index|]
operator|==
literal|'.'
condition|)
while|while
condition|(
name|sol
index|[
name|j
operator|+
literal|1
index|]
operator|==
literal|'.'
condition|)
name|j
operator|++
expr_stmt|;
block|}
else|else
name|space
operator|=
literal|1
expr_stmt|;
block|}
while|while
condition|(
name|filename
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'.'
operator|||
name|filename
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'-'
condition|)
name|len
operator|--
expr_stmt|;
block|}
name|strcpy
argument_list|(
name|filename
operator|+
name|len
argument_list|,
literal|".txt"
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|realstdout
argument_list|,
literal|"%s\n"
argument_list|,
name|filename
argument_list|)
expr_stmt|;
name|freopen
argument_list|(
name|filename
argument_list|,
literal|"w"
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_patch_id
specifier|static
name|int
name|get_patch_id
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
DECL|function|get_patch_ids
specifier|static
name|void
name|get_patch_ids
parameter_list|(
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|,
name|struct
name|diff_options
modifier|*
name|options
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|struct
name|rev_info
name|check_rev
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|struct
name|object
modifier|*
name|o1
decl_stmt|,
modifier|*
name|o2
decl_stmt|;
name|unsigned
name|flags1
decl_stmt|,
name|flags2
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|rev
operator|->
name|pending
operator|.
name|nr
operator|!=
literal|2
condition|)
name|die
argument_list|(
literal|"Need exactly one range."
argument_list|)
expr_stmt|;
name|o1
operator|=
name|rev
operator|->
name|pending
operator|.
name|objects
index|[
literal|0
index|]
operator|.
name|item
expr_stmt|;
name|flags1
operator|=
name|o1
operator|->
name|flags
expr_stmt|;
name|o2
operator|=
name|rev
operator|->
name|pending
operator|.
name|objects
index|[
literal|1
index|]
operator|.
name|item
expr_stmt|;
name|flags2
operator|=
name|o2
operator|->
name|flags
expr_stmt|;
if|if
condition|(
operator|(
name|flags1
operator|&
name|UNINTERESTING
operator|)
operator|==
operator|(
name|flags2
operator|&
name|UNINTERESTING
operator|)
condition|)
name|die
argument_list|(
literal|"Not a range."
argument_list|)
expr_stmt|;
name|diff_setup
argument_list|(
name|options
argument_list|)
expr_stmt|;
name|options
operator|->
name|recursive
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|diff_setup_done
argument_list|(
name|options
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"diff_setup_done failed"
argument_list|)
expr_stmt|;
comment|/* given a range a..b get all patch ids for b..a */
name|init_revisions
argument_list|(
operator|&
name|check_rev
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|o1
operator|->
name|flags
operator|^=
name|UNINTERESTING
expr_stmt|;
name|o2
operator|->
name|flags
operator|^=
name|UNINTERESTING
expr_stmt|;
name|add_pending_object
argument_list|(
operator|&
name|check_rev
argument_list|,
name|o1
argument_list|,
literal|"o1"
argument_list|)
expr_stmt|;
name|add_pending_object
argument_list|(
operator|&
name|check_rev
argument_list|,
name|o2
argument_list|,
literal|"o2"
argument_list|)
expr_stmt|;
name|prepare_revision_walk
argument_list|(
operator|&
name|check_rev
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|commit
operator|=
name|get_revision
argument_list|(
operator|&
name|check_rev
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
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
if|if
condition|(
operator|!
name|get_patch_id
argument_list|(
name|commit
argument_list|,
name|options
argument_list|,
name|sha1
argument_list|)
condition|)
name|created_object
argument_list|(
name|sha1
argument_list|,
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|object
argument_list|)
argument_list|)
argument_list|)
expr_stmt|;
block|}
comment|/* reset for next revision walk */
name|clear_commit_marks
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o1
argument_list|,
name|SEEN
operator||
name|UNINTERESTING
operator||
name|SHOWN
operator||
name|ADDED
argument_list|)
expr_stmt|;
name|clear_commit_marks
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|o2
argument_list|,
name|SEEN
operator||
name|UNINTERESTING
operator||
name|SHOWN
operator||
name|ADDED
argument_list|)
expr_stmt|;
name|o1
operator|->
name|flags
operator|=
name|flags1
expr_stmt|;
name|o2
operator|->
name|flags
operator|=
name|flags2
expr_stmt|;
block|}
end_function

begin_function
DECL|function|gen_message_id
specifier|static
name|void
name|gen_message_id
parameter_list|(
name|char
modifier|*
name|dest
parameter_list|,
name|unsigned
name|int
name|length
parameter_list|,
name|char
modifier|*
name|base
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|committer
init|=
name|git_committer_info
argument_list|(
literal|1
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|email_start
init|=
name|strrchr
argument_list|(
name|committer
argument_list|,
literal|'<'
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|email_end
init|=
name|strrchr
argument_list|(
name|committer
argument_list|,
literal|'>'
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|email_start
operator|||
operator|!
name|email_end
operator|||
name|email_start
operator|>
name|email_end
operator|-
literal|1
condition|)
name|die
argument_list|(
literal|"Could not extract email from committer identity."
argument_list|)
expr_stmt|;
name|snprintf
argument_list|(
name|dest
argument_list|,
name|length
argument_list|,
literal|"%s.%lu.git.%.*s"
argument_list|,
name|base
argument_list|,
operator|(
name|unsigned
name|long
operator|)
name|time
argument_list|(
name|NULL
argument_list|)
argument_list|,
call|(
name|int
call|)
argument_list|(
name|email_end
operator|-
name|email_start
operator|-
literal|1
argument_list|)
argument_list|,
name|email_start
operator|+
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_format_patch
name|int
name|cmd_format_patch
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
name|commit
modifier|*
name|commit
decl_stmt|;
name|struct
name|commit
modifier|*
modifier|*
name|list
init|=
name|NULL
decl_stmt|;
name|struct
name|rev_info
name|rev
decl_stmt|;
name|int
name|nr
init|=
literal|0
decl_stmt|,
name|total
decl_stmt|,
name|i
decl_stmt|,
name|j
decl_stmt|;
name|int
name|use_stdout
init|=
literal|0
decl_stmt|;
name|int
name|numbered
init|=
literal|0
decl_stmt|;
name|int
name|start_number
init|=
operator|-
literal|1
decl_stmt|;
name|int
name|keep_subject
init|=
literal|0
decl_stmt|;
name|int
name|ignore_if_in_upstream
init|=
literal|0
decl_stmt|;
name|int
name|thread
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|in_reply_to
init|=
name|NULL
decl_stmt|;
name|struct
name|diff_options
name|patch_id_opts
decl_stmt|;
name|char
modifier|*
name|add_signoff
init|=
name|NULL
decl_stmt|;
name|char
name|message_id
index|[
literal|1024
index|]
decl_stmt|;
name|char
name|ref_message_id
index|[
literal|1024
index|]
decl_stmt|;
name|setup_ident
argument_list|()
expr_stmt|;
name|git_config
argument_list|(
name|git_format_config
argument_list|)
expr_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|rev
operator|.
name|commit_format
operator|=
name|CMIT_FMT_EMAIL
expr_stmt|;
name|rev
operator|.
name|verbose_header
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|diff
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|combine_merges
operator|=
literal|0
expr_stmt|;
name|rev
operator|.
name|ignore_merges
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|msg_sep
operator|=
literal|""
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|recursive
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|extra_headers
operator|=
name|extra_headers
expr_stmt|;
name|output_directory
operator|=
name|prefix
expr_stmt|;
comment|/* 	 * Parse the arguments before setup_revisions(), or something 	 * like "git fmt-patch -o a123 HEAD^.." may fail; a123 is 	 * possibly a valid SHA1. 	 */
for|for
control|(
name|i
operator|=
literal|1
operator|,
name|j
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--stdout"
argument_list|)
condition|)
name|use_stdout
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"-n"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--numbered"
argument_list|)
condition|)
name|numbered
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--start-number="
argument_list|,
literal|15
argument_list|)
condition|)
name|start_number
operator|=
name|strtol
argument_list|(
name|argv
index|[
name|i
index|]
operator|+
literal|15
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--start-number"
argument_list|)
condition|)
block|{
name|i
operator|++
expr_stmt|;
if|if
condition|(
name|i
operator|==
name|argc
condition|)
name|die
argument_list|(
literal|"Need a number for --start-number"
argument_list|)
expr_stmt|;
name|start_number
operator|=
name|strtol
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"-k"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--keep-subject"
argument_list|)
condition|)
block|{
name|keep_subject
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|total
operator|=
operator|-
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--output-directory"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"-o"
argument_list|)
condition|)
block|{
name|i
operator|++
expr_stmt|;
if|if
condition|(
name|argc
operator|<=
name|i
condition|)
name|die
argument_list|(
literal|"Which directory?"
argument_list|)
expr_stmt|;
if|if
condition|(
name|output_directory
condition|)
name|die
argument_list|(
literal|"Two output directories?"
argument_list|)
expr_stmt|;
name|output_directory
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--signoff"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"-s"
argument_list|)
condition|)
block|{
specifier|const
name|char
modifier|*
name|committer
decl_stmt|;
specifier|const
name|char
modifier|*
name|endpos
decl_stmt|;
name|committer
operator|=
name|git_committer_info
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|endpos
operator|=
name|strchr
argument_list|(
name|committer
argument_list|,
literal|'>'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|endpos
condition|)
name|die
argument_list|(
literal|"bogos committer info %s\n"
argument_list|,
name|committer
argument_list|)
expr_stmt|;
name|add_signoff
operator|=
name|xmalloc
argument_list|(
name|endpos
operator|-
name|committer
operator|+
literal|2
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|add_signoff
argument_list|,
name|committer
argument_list|,
name|endpos
operator|-
name|committer
operator|+
literal|1
argument_list|)
expr_stmt|;
name|add_signoff
index|[
name|endpos
operator|-
name|committer
operator|+
literal|1
index|]
operator|=
literal|0
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--attach"
argument_list|)
condition|)
name|rev
operator|.
name|mime_boundary
operator|=
name|git_version_string
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--attach="
argument_list|,
literal|9
argument_list|)
condition|)
name|rev
operator|.
name|mime_boundary
operator|=
name|argv
index|[
name|i
index|]
operator|+
literal|9
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--ignore-if-in-upstream"
argument_list|)
condition|)
name|ignore_if_in_upstream
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--thread"
argument_list|)
condition|)
name|thread
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--in-reply-to="
argument_list|,
literal|14
argument_list|)
condition|)
name|in_reply_to
operator|=
name|argv
index|[
name|i
index|]
operator|+
literal|14
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--in-reply-to"
argument_list|)
condition|)
block|{
name|i
operator|++
expr_stmt|;
if|if
condition|(
name|i
operator|==
name|argc
condition|)
name|die
argument_list|(
literal|"Need a Message-Id for --in-reply-to"
argument_list|)
expr_stmt|;
name|in_reply_to
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
block|}
else|else
name|argv
index|[
name|j
operator|++
index|]
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
block|}
name|argc
operator|=
name|j
expr_stmt|;
if|if
condition|(
name|start_number
operator|<
literal|0
condition|)
name|start_number
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|numbered
operator|&&
name|keep_subject
condition|)
name|die
argument_list|(
literal|"-n and -k are mutually exclusive."
argument_list|)
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
name|rev
argument_list|,
literal|"HEAD"
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|>
literal|1
condition|)
name|die
argument_list|(
literal|"unrecognized argument: %s"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|rev
operator|.
name|diffopt
operator|.
name|output_format
condition|)
name|rev
operator|.
name|diffopt
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_DIFFSTAT
operator||
name|DIFF_FORMAT_PATCH
expr_stmt|;
if|if
condition|(
name|output_directory
condition|)
block|{
if|if
condition|(
name|use_stdout
condition|)
name|die
argument_list|(
literal|"standard output, or directory, which one?"
argument_list|)
expr_stmt|;
if|if
condition|(
name|mkdir
argument_list|(
name|output_directory
argument_list|,
literal|0777
argument_list|)
operator|<
literal|0
operator|&&
name|errno
operator|!=
name|EEXIST
condition|)
name|die
argument_list|(
literal|"Could not create directory %s"
argument_list|,
name|output_directory
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|rev
operator|.
name|pending
operator|.
name|nr
operator|==
literal|1
condition|)
block|{
name|rev
operator|.
name|pending
operator|.
name|objects
index|[
literal|0
index|]
operator|.
name|item
operator|->
name|flags
operator||=
name|UNINTERESTING
expr_stmt|;
name|add_head
argument_list|(
operator|&
name|rev
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|ignore_if_in_upstream
condition|)
name|get_patch_ids
argument_list|(
operator|&
name|rev
argument_list|,
operator|&
name|patch_id_opts
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|use_stdout
condition|)
name|realstdout
operator|=
name|fdopen
argument_list|(
name|dup
argument_list|(
literal|1
argument_list|)
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
name|prepare_revision_walk
argument_list|(
operator|&
name|rev
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|commit
operator|=
name|get_revision
argument_list|(
operator|&
name|rev
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
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
if|if
condition|(
name|ignore_if_in_upstream
operator|&&
operator|!
name|get_patch_id
argument_list|(
name|commit
argument_list|,
operator|&
name|patch_id_opts
argument_list|,
name|sha1
argument_list|)
operator|&&
name|lookup_object
argument_list|(
name|sha1
argument_list|)
condition|)
continue|continue;
name|nr
operator|++
expr_stmt|;
name|list
operator|=
name|xrealloc
argument_list|(
name|list
argument_list|,
name|nr
operator|*
sizeof|sizeof
argument_list|(
name|list
index|[
literal|0
index|]
argument_list|)
argument_list|)
expr_stmt|;
name|list
index|[
name|nr
operator|-
literal|1
index|]
operator|=
name|commit
expr_stmt|;
block|}
name|total
operator|=
name|nr
expr_stmt|;
if|if
condition|(
name|numbered
condition|)
name|rev
operator|.
name|total
operator|=
name|total
operator|+
name|start_number
operator|-
literal|1
expr_stmt|;
name|rev
operator|.
name|add_signoff
operator|=
name|add_signoff
expr_stmt|;
name|rev
operator|.
name|ref_message_id
operator|=
name|in_reply_to
expr_stmt|;
while|while
condition|(
literal|0
operator|<=
operator|--
name|nr
condition|)
block|{
name|int
name|shown
decl_stmt|;
name|commit
operator|=
name|list
index|[
name|nr
index|]
expr_stmt|;
name|rev
operator|.
name|nr
operator|=
name|total
operator|-
name|nr
operator|+
operator|(
name|start_number
operator|-
literal|1
operator|)
expr_stmt|;
comment|/* Make the second and subsequent mails replies to the first */
if|if
condition|(
name|thread
condition|)
block|{
if|if
condition|(
name|nr
operator|==
operator|(
name|total
operator|-
literal|2
operator|)
condition|)
block|{
name|strncpy
argument_list|(
name|ref_message_id
argument_list|,
name|message_id
argument_list|,
sizeof|sizeof
argument_list|(
name|ref_message_id
argument_list|)
argument_list|)
expr_stmt|;
name|ref_message_id
index|[
sizeof|sizeof
argument_list|(
name|ref_message_id
argument_list|)
operator|-
literal|1
index|]
operator|=
literal|'\0'
expr_stmt|;
name|rev
operator|.
name|ref_message_id
operator|=
name|ref_message_id
expr_stmt|;
block|}
name|gen_message_id
argument_list|(
name|message_id
argument_list|,
sizeof|sizeof
argument_list|(
name|message_id
argument_list|)
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
name|rev
operator|.
name|message_id
operator|=
name|message_id
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|use_stdout
condition|)
name|reopen_stdout
argument_list|(
name|commit
argument_list|,
name|rev
operator|.
name|nr
argument_list|,
name|keep_subject
argument_list|)
expr_stmt|;
name|shown
operator|=
name|log_tree_commit
argument_list|(
operator|&
name|rev
argument_list|,
name|commit
argument_list|)
expr_stmt|;
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
comment|/* We put one extra blank line between formatted 		 * patches and this flag is used by log-tree code 		 * to see if it needs to emit a LF before showing 		 * the log; when using one file per patch, we do 		 * not want the extra blank line. 		 */
if|if
condition|(
operator|!
name|use_stdout
condition|)
name|rev
operator|.
name|shown_one
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|shown
condition|)
block|{
if|if
condition|(
name|rev
operator|.
name|mime_boundary
condition|)
name|printf
argument_list|(
literal|"\n--%s%s--\n\n\n"
argument_list|,
name|mime_boundary_leader
argument_list|,
name|rev
operator|.
name|mime_boundary
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"-- \n%s\n\n"
argument_list|,
name|git_version_string
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|use_stdout
condition|)
name|fclose
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|list
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

