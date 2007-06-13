begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"object.h"
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

begin_include
include|#
directive|include
file|"wt-status.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_include
include|#
directive|include
file|"utf8.h"
end_include

begin_comment
comment|/*  * This implements the builtins revert and cherry-pick.  *  * Copyright (c) 2007 Johannes E. Schindelin  *  * Based on git-revert.sh, which is  *  * Copyright (c) 2005 Linus Torvalds  * Copyright (c) 2005 Junio C Hamano  */
end_comment

begin_decl_stmt
DECL|variable|revert_usage
specifier|static
specifier|const
name|char
modifier|*
name|revert_usage
init|=
literal|"git-revert [--edit | --no-edit] [-n]<commit-ish>"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cherry_pick_usage
specifier|static
specifier|const
name|char
modifier|*
name|cherry_pick_usage
init|=
literal|"git-cherry-pick [--edit] [-n] [-r] [-x]<commit-ish>"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|edit
specifier|static
name|int
name|edit
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|replay
specifier|static
name|int
name|replay
decl_stmt|;
end_decl_stmt

begin_enum
DECL|enumerator|REVERT
DECL|enumerator|CHERRY_PICK
DECL|variable|action
specifier|static
enum|enum
block|{
name|REVERT
block|,
name|CHERRY_PICK
block|}
name|action
enum|;
end_enum

begin_decl_stmt
DECL|variable|no_commit
specifier|static
name|int
name|no_commit
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|commit
specifier|static
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|needed_deref
specifier|static
name|int
name|needed_deref
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|me
specifier|static
specifier|const
name|char
modifier|*
name|me
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|GIT_REFLOG_ACTION
define|#
directive|define
name|GIT_REFLOG_ACTION
value|"GIT_REFLOG_ACTION"
end_define

begin_function
DECL|function|parse_options
specifier|static
name|void
name|parse_options
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|usage_str
init|=
name|action
operator|==
name|REVERT
condition|?
name|revert_usage
else|:
name|cherry_pick_usage
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|arg
decl_stmt|;
name|int
name|i
decl_stmt|;
if|if
condition|(
name|argc
operator|<
literal|2
condition|)
name|usage
argument_list|(
name|usage_str
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
name|arg
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|arg
index|[
literal|0
index|]
operator|!=
literal|'-'
condition|)
break|break;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-n"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--no-commit"
argument_list|)
condition|)
name|no_commit
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-e"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--edit"
argument_list|)
condition|)
name|edit
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--no-edit"
argument_list|)
condition|)
name|edit
operator|=
literal|0
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-x"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--i-really-want-"
literal|"to-expose-my-private-commit-object-name"
argument_list|)
condition|)
name|replay
operator|=
literal|0
expr_stmt|;
elseif|else
if|if
condition|(
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-r"
argument_list|)
condition|)
name|usage
argument_list|(
name|usage_str
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|i
operator|!=
name|argc
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|usage_str
argument_list|)
expr_stmt|;
name|arg
operator|=
name|argv
index|[
name|argc
operator|-
literal|1
index|]
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|arg
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Cannot find '%s'"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|commit
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|parse_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
condition|)
name|die
argument_list|(
literal|"Could not find %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|type
operator|==
name|OBJ_TAG
condition|)
block|{
name|commit
operator|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|deref_tag
argument_list|(
operator|(
expr|struct
name|object
operator|*
operator|)
name|commit
argument_list|,
name|arg
argument_list|,
name|strlen
argument_list|(
name|arg
argument_list|)
argument_list|)
expr_stmt|;
name|needed_deref
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|type
operator|!=
name|OBJ_COMMIT
condition|)
name|die
argument_list|(
literal|"'%s' does not point to a commit"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_oneline
specifier|static
name|char
modifier|*
name|get_oneline
parameter_list|(
specifier|const
name|char
modifier|*
name|message
parameter_list|)
block|{
name|char
modifier|*
name|result
decl_stmt|;
specifier|const
name|char
modifier|*
name|p
init|=
name|message
decl_stmt|,
modifier|*
name|abbrev
decl_stmt|,
modifier|*
name|eol
decl_stmt|;
name|int
name|abbrev_len
decl_stmt|,
name|oneline_len
decl_stmt|;
if|if
condition|(
operator|!
name|p
condition|)
name|die
argument_list|(
literal|"Could not read commit message of %s"
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
while|while
condition|(
operator|*
name|p
operator|&&
operator|(
operator|*
name|p
operator|!=
literal|'\n'
operator|||
name|p
index|[
literal|1
index|]
operator|!=
literal|'\n'
operator|)
condition|)
name|p
operator|++
expr_stmt|;
if|if
condition|(
operator|*
name|p
condition|)
block|{
name|p
operator|+=
literal|2
expr_stmt|;
for|for
control|(
name|eol
operator|=
name|p
operator|+
literal|1
init|;
operator|*
name|eol
operator|&&
operator|*
name|eol
operator|!=
literal|'\n'
condition|;
name|eol
operator|++
control|)
empty_stmt|;
comment|/* do nothing */
block|}
else|else
name|eol
operator|=
name|p
expr_stmt|;
name|abbrev
operator|=
name|find_unique_abbrev
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
expr_stmt|;
name|abbrev_len
operator|=
name|strlen
argument_list|(
name|abbrev
argument_list|)
expr_stmt|;
name|oneline_len
operator|=
name|eol
operator|-
name|p
expr_stmt|;
name|result
operator|=
name|xmalloc
argument_list|(
name|abbrev_len
operator|+
literal|5
operator|+
name|oneline_len
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|result
argument_list|,
name|abbrev
argument_list|,
name|abbrev_len
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|result
operator|+
name|abbrev_len
argument_list|,
literal|"... "
argument_list|,
literal|4
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|result
operator|+
name|abbrev_len
operator|+
literal|4
argument_list|,
name|p
argument_list|,
name|oneline_len
argument_list|)
expr_stmt|;
name|result
index|[
name|abbrev_len
operator|+
literal|4
operator|+
name|oneline_len
index|]
operator|=
literal|'\0'
expr_stmt|;
return|return
name|result
return|;
block|}
end_function

begin_function
DECL|function|get_encoding
specifier|static
name|char
modifier|*
name|get_encoding
parameter_list|(
specifier|const
name|char
modifier|*
name|message
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|p
init|=
name|message
decl_stmt|,
modifier|*
name|eol
decl_stmt|;
if|if
condition|(
operator|!
name|p
condition|)
name|die
argument_list|(
literal|"Could not read commit message of %s"
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
while|while
condition|(
operator|*
name|p
operator|&&
operator|*
name|p
operator|!=
literal|'\n'
condition|)
block|{
for|for
control|(
name|eol
operator|=
name|p
operator|+
literal|1
init|;
operator|*
name|eol
operator|&&
operator|*
name|eol
operator|!=
literal|'\n'
condition|;
name|eol
operator|++
control|)
empty_stmt|;
comment|/* do nothing */
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|p
argument_list|,
literal|"encoding "
argument_list|)
condition|)
block|{
name|char
modifier|*
name|result
init|=
name|xmalloc
argument_list|(
name|eol
operator|-
literal|8
operator|-
name|p
argument_list|)
decl_stmt|;
name|strlcpy
argument_list|(
name|result
argument_list|,
name|p
operator|+
literal|9
argument_list|,
name|eol
operator|-
literal|8
operator|-
name|p
argument_list|)
expr_stmt|;
return|return
name|result
return|;
block|}
name|p
operator|=
name|eol
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|==
literal|'\n'
condition|)
name|p
operator|++
expr_stmt|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|msg_file
specifier|static
name|struct
name|lock_file
name|msg_file
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|msg_fd
specifier|static
name|int
name|msg_fd
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|add_to_msg
specifier|static
name|void
name|add_to_msg
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|string
argument_list|)
decl_stmt|;
if|if
condition|(
name|write_in_full
argument_list|(
name|msg_fd
argument_list|,
name|string
argument_list|,
name|len
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Could not write to MERGE_MSG"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_message_to_msg
specifier|static
name|void
name|add_message_to_msg
parameter_list|(
specifier|const
name|char
modifier|*
name|message
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|p
init|=
name|message
decl_stmt|;
while|while
condition|(
operator|*
name|p
operator|&&
operator|(
operator|*
name|p
operator|!=
literal|'\n'
operator|||
name|p
index|[
literal|1
index|]
operator|!=
literal|'\n'
operator|)
condition|)
name|p
operator|++
expr_stmt|;
if|if
condition|(
operator|!
operator|*
name|p
condition|)
name|add_to_msg
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
expr_stmt|;
name|p
operator|+=
literal|2
expr_stmt|;
name|add_to_msg
argument_list|(
name|p
argument_list|)
expr_stmt|;
return|return;
block|}
end_function

begin_function
DECL|function|set_author_ident_env
specifier|static
name|void
name|set_author_ident_env
parameter_list|(
specifier|const
name|char
modifier|*
name|message
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|p
init|=
name|message
decl_stmt|;
if|if
condition|(
operator|!
name|p
condition|)
name|die
argument_list|(
literal|"Could not read commit message of %s"
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
while|while
condition|(
operator|*
name|p
operator|&&
operator|*
name|p
operator|!=
literal|'\n'
condition|)
block|{
specifier|const
name|char
modifier|*
name|eol
decl_stmt|;
for|for
control|(
name|eol
operator|=
name|p
init|;
operator|*
name|eol
operator|&&
operator|*
name|eol
operator|!=
literal|'\n'
condition|;
name|eol
operator|++
control|)
empty_stmt|;
comment|/* do nothing */
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|p
argument_list|,
literal|"author "
argument_list|)
condition|)
block|{
name|char
modifier|*
name|line
decl_stmt|,
modifier|*
name|pend
decl_stmt|,
modifier|*
name|email
decl_stmt|,
modifier|*
name|timestamp
decl_stmt|;
name|p
operator|+=
literal|7
expr_stmt|;
name|line
operator|=
name|xmalloc
argument_list|(
name|eol
operator|+
literal|1
operator|-
name|p
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|line
argument_list|,
name|p
argument_list|,
name|eol
operator|-
name|p
argument_list|)
expr_stmt|;
name|line
index|[
name|eol
operator|-
name|p
index|]
operator|=
literal|'\0'
expr_stmt|;
name|email
operator|=
name|strchr
argument_list|(
name|line
argument_list|,
literal|'<'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|email
condition|)
name|die
argument_list|(
literal|"Could not extract author email from %s"
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
name|email
operator|==
name|line
condition|)
name|pend
operator|=
name|line
expr_stmt|;
else|else
for|for
control|(
name|pend
operator|=
name|email
init|;
name|pend
operator|!=
name|line
operator|+
literal|1
operator|&&
name|isspace
argument_list|(
name|pend
index|[
operator|-
literal|1
index|]
argument_list|)
condition|;
name|pend
operator|--
control|)
empty_stmt|;
empty_stmt|;
comment|/* do nothing */
operator|*
name|pend
operator|=
literal|'\0'
expr_stmt|;
name|email
operator|++
expr_stmt|;
name|timestamp
operator|=
name|strchr
argument_list|(
name|email
argument_list|,
literal|'>'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|timestamp
condition|)
name|die
argument_list|(
literal|"Could not extract author email from %s"
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
operator|*
name|timestamp
operator|=
literal|'\0'
expr_stmt|;
for|for
control|(
name|timestamp
operator|++
init|;
operator|*
name|timestamp
operator|&&
name|isspace
argument_list|(
operator|*
name|timestamp
argument_list|)
condition|;
name|timestamp
operator|++
control|)
empty_stmt|;
comment|/* do nothing */
name|setenv
argument_list|(
literal|"GIT_AUTHOR_NAME"
argument_list|,
name|line
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|setenv
argument_list|(
literal|"GIT_AUTHOR_EMAIL"
argument_list|,
name|email
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|setenv
argument_list|(
literal|"GIT_AUTHOR_DATE"
argument_list|,
name|timestamp
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|line
argument_list|)
expr_stmt|;
return|return;
block|}
name|p
operator|=
name|eol
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|==
literal|'\n'
condition|)
name|p
operator|++
expr_stmt|;
block|}
name|die
argument_list|(
literal|"No author information found in %s"
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

begin_function
DECL|function|merge_recursive
specifier|static
name|int
name|merge_recursive
parameter_list|(
specifier|const
name|char
modifier|*
name|base_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|head_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|head_name
parameter_list|,
specifier|const
name|char
modifier|*
name|next_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|next_name
parameter_list|)
block|{
name|char
name|buffer
index|[
literal|256
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|argv
index|[
literal|6
index|]
decl_stmt|;
name|sprintf
argument_list|(
name|buffer
argument_list|,
literal|"GITHEAD_%s"
argument_list|,
name|head_sha1
argument_list|)
expr_stmt|;
name|setenv
argument_list|(
name|buffer
argument_list|,
name|head_name
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|buffer
argument_list|,
literal|"GITHEAD_%s"
argument_list|,
name|next_sha1
argument_list|)
expr_stmt|;
name|setenv
argument_list|(
name|buffer
argument_list|,
name|next_name
argument_list|,
literal|1
argument_list|)
expr_stmt|;
comment|/* 	 * This three way merge is an interesting one.  We are at 	 * $head, and would want to apply the change between $commit 	 * and $prev on top of us (when reverting), or the change between 	 * $prev and $commit on top of us (when cherry-picking or replaying). 	 */
name|argv
index|[
literal|0
index|]
operator|=
literal|"merge-recursive"
expr_stmt|;
name|argv
index|[
literal|1
index|]
operator|=
name|base_sha1
expr_stmt|;
name|argv
index|[
literal|2
index|]
operator|=
literal|"--"
expr_stmt|;
name|argv
index|[
literal|3
index|]
operator|=
name|head_sha1
expr_stmt|;
name|argv
index|[
literal|4
index|]
operator|=
name|next_sha1
expr_stmt|;
name|argv
index|[
literal|5
index|]
operator|=
name|NULL
expr_stmt|;
return|return
name|run_command_v_opt
argument_list|(
name|argv
argument_list|,
name|RUN_COMMAND_NO_STDIN
operator||
name|RUN_GIT_CMD
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|revert_or_cherry_pick
specifier|static
name|int
name|revert_or_cherry_pick
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|unsigned
name|char
name|head
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|commit
modifier|*
name|base
decl_stmt|,
modifier|*
name|next
decl_stmt|;
name|int
name|i
decl_stmt|;
name|char
modifier|*
name|oneline
decl_stmt|,
modifier|*
name|reencoded_message
init|=
name|NULL
decl_stmt|;
specifier|const
name|char
modifier|*
name|message
decl_stmt|,
modifier|*
name|encoding
decl_stmt|;
specifier|const
name|char
modifier|*
name|defmsg
init|=
name|xstrdup
argument_list|(
name|git_path
argument_list|(
literal|"MERGE_MSG"
argument_list|)
argument_list|)
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
name|me
operator|=
name|action
operator|==
name|REVERT
condition|?
literal|"revert"
else|:
literal|"cherry-pick"
expr_stmt|;
name|setenv
argument_list|(
name|GIT_REFLOG_ACTION
argument_list|,
name|me
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|)
expr_stmt|;
comment|/* this is copied from the shell script, but it's never triggered... */
if|if
condition|(
name|action
operator|==
name|REVERT
operator|&&
name|replay
condition|)
name|die
argument_list|(
literal|"revert is incompatible with replay"
argument_list|)
expr_stmt|;
if|if
condition|(
name|no_commit
condition|)
block|{
comment|/* 		 * We do not intend to commit immediately.  We just want to 		 * merge the differences in. 		 */
if|if
condition|(
name|write_tree
argument_list|(
name|head
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
condition|)
name|die
argument_list|(
literal|"Your index file is unmerged."
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|struct
name|wt_status
name|s
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
literal|"HEAD"
argument_list|,
name|head
argument_list|)
condition|)
name|die
argument_list|(
literal|"You do not have a valid HEAD"
argument_list|)
expr_stmt|;
name|wt_status_prepare
argument_list|(
operator|&
name|s
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|.
name|commitable
operator|||
name|s
operator|.
name|workdir_dirty
condition|)
name|die
argument_list|(
literal|"Dirty index: cannot %s"
argument_list|,
name|me
argument_list|)
expr_stmt|;
name|discard_cache
argument_list|()
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|commit
operator|->
name|parents
condition|)
name|die
argument_list|(
literal|"Cannot %s a root commit"
argument_list|,
name|me
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit
operator|->
name|parents
operator|->
name|next
condition|)
name|die
argument_list|(
literal|"Cannot %s a multi-parent commit."
argument_list|,
name|me
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|message
operator|=
name|commit
operator|->
name|buffer
operator|)
condition|)
name|die
argument_list|(
literal|"Cannot get commit message for %s"
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
comment|/* 	 * "commit" is an existing commit.  We would want to apply 	 * the difference it introduces since its first parent "prev" 	 * on top of the current HEAD if we are cherry-pick.  Or the 	 * reverse of it if we are revert. 	 */
name|msg_fd
operator|=
name|hold_lock_file_for_update
argument_list|(
operator|&
name|msg_file
argument_list|,
name|defmsg
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|encoding
operator|=
name|get_encoding
argument_list|(
name|message
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|encoding
condition|)
name|encoding
operator|=
literal|"utf-8"
expr_stmt|;
if|if
condition|(
operator|!
name|git_commit_encoding
condition|)
name|git_commit_encoding
operator|=
literal|"utf-8"
expr_stmt|;
if|if
condition|(
operator|(
name|reencoded_message
operator|=
name|reencode_string
argument_list|(
name|message
argument_list|,
name|git_commit_encoding
argument_list|,
name|encoding
argument_list|)
operator|)
condition|)
name|message
operator|=
name|reencoded_message
expr_stmt|;
name|oneline
operator|=
name|get_oneline
argument_list|(
name|message
argument_list|)
expr_stmt|;
if|if
condition|(
name|action
operator|==
name|REVERT
condition|)
block|{
name|char
modifier|*
name|oneline_body
init|=
name|strchr
argument_list|(
name|oneline
argument_list|,
literal|' '
argument_list|)
decl_stmt|;
name|base
operator|=
name|commit
expr_stmt|;
name|next
operator|=
name|commit
operator|->
name|parents
operator|->
name|item
expr_stmt|;
name|add_to_msg
argument_list|(
literal|"Revert \""
argument_list|)
expr_stmt|;
name|add_to_msg
argument_list|(
name|oneline_body
operator|+
literal|1
argument_list|)
expr_stmt|;
name|add_to_msg
argument_list|(
literal|"\"\n\nThis reverts commit "
argument_list|)
expr_stmt|;
name|add_to_msg
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
expr_stmt|;
name|add_to_msg
argument_list|(
literal|".\n"
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|base
operator|=
name|commit
operator|->
name|parents
operator|->
name|item
expr_stmt|;
name|next
operator|=
name|commit
expr_stmt|;
name|set_author_ident_env
argument_list|(
name|message
argument_list|)
expr_stmt|;
name|add_message_to_msg
argument_list|(
name|message
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|replay
condition|)
block|{
name|add_to_msg
argument_list|(
literal|"(cherry picked from commit "
argument_list|)
expr_stmt|;
name|add_to_msg
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
expr_stmt|;
name|add_to_msg
argument_list|(
literal|")\n"
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|needed_deref
condition|)
block|{
name|add_to_msg
argument_list|(
literal|"(original 'git "
argument_list|)
expr_stmt|;
name|add_to_msg
argument_list|(
name|me
argument_list|)
expr_stmt|;
name|add_to_msg
argument_list|(
literal|"' arguments: "
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
name|argc
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|i
condition|)
name|add_to_msg
argument_list|(
literal|" "
argument_list|)
expr_stmt|;
name|add_to_msg
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
name|add_to_msg
argument_list|(
literal|")\n"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|merge_recursive
argument_list|(
name|sha1_to_hex
argument_list|(
name|base
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|head
argument_list|)
argument_list|,
literal|"HEAD"
argument_list|,
name|sha1_to_hex
argument_list|(
name|next
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|,
name|oneline
argument_list|)
operator|||
name|write_tree
argument_list|(
name|head
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
name|add_to_msg
argument_list|(
literal|"\nConflicts:\n\n"
argument_list|)
expr_stmt|;
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
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|active_cache
index|[
name|i
operator|++
index|]
decl_stmt|;
if|if
condition|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|)
block|{
name|add_to_msg
argument_list|(
literal|"\t"
argument_list|)
expr_stmt|;
name|add_to_msg
argument_list|(
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
name|add_to_msg
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|active_nr
operator|&&
operator|!
name|strcmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|active_cache
index|[
name|i
index|]
operator|->
name|name
argument_list|)
condition|)
name|i
operator|++
expr_stmt|;
block|}
block|}
if|if
condition|(
name|close
argument_list|(
name|msg_fd
argument_list|)
operator|||
name|commit_lock_file
argument_list|(
operator|&
name|msg_file
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Error wrapping up %s"
argument_list|,
name|defmsg
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Automatic %s failed.  "
literal|"After resolving the conflicts,\n"
literal|"mark the corrected paths with 'git-add<paths>'\n"
literal|"and commit the result.\n"
argument_list|,
name|me
argument_list|)
expr_stmt|;
if|if
condition|(
name|action
operator|==
name|CHERRY_PICK
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"When commiting, use the option "
literal|"'-c %s' to retain authorship and message.\n"
argument_list|,
name|find_unique_abbrev
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|close
argument_list|(
name|msg_fd
argument_list|)
operator|||
name|commit_lock_file
argument_list|(
operator|&
name|msg_file
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Error wrapping up %s"
argument_list|,
name|defmsg
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Finished one %s.\n"
argument_list|,
name|me
argument_list|)
expr_stmt|;
comment|/* 	 * 	 * If we are cherry-pick, and if the merge did not result in 	 * hand-editing, we will hit this commit and inherit the original 	 * author date and name. 	 * If we are revert, or if our cherry-pick results in a hand merge, 	 * we had better say that the current user is responsible for that. 	 */
if|if
condition|(
operator|!
name|no_commit
condition|)
block|{
if|if
condition|(
name|edit
condition|)
return|return
name|execl_git_cmd
argument_list|(
literal|"commit"
argument_list|,
literal|"-n"
argument_list|,
name|NULL
argument_list|)
return|;
else|else
return|return
name|execl_git_cmd
argument_list|(
literal|"commit"
argument_list|,
literal|"-n"
argument_list|,
literal|"-F"
argument_list|,
name|defmsg
argument_list|,
name|NULL
argument_list|)
return|;
block|}
if|if
condition|(
name|reencoded_message
condition|)
name|free
argument_list|(
name|reencoded_message
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_revert
name|int
name|cmd_revert
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
if|if
condition|(
name|isatty
argument_list|(
literal|0
argument_list|)
condition|)
name|edit
operator|=
literal|1
expr_stmt|;
name|action
operator|=
name|REVERT
expr_stmt|;
return|return
name|revert_or_cherry_pick
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|cmd_cherry_pick
name|int
name|cmd_cherry_pick
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
name|replay
operator|=
literal|1
expr_stmt|;
name|action
operator|=
name|CHERRY_PICK
expr_stmt|;
return|return
name|revert_or_cherry_pick
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|)
return|;
block|}
end_function

end_unit

