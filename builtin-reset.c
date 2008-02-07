begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * "git reset" builtin command  *  * Copyright (c) 2007 Carlos Rica  *  * Based on git-reset.sh, which is  *  * Copyright (c) 2005, 2006 Linus Torvalds and Junio C Hamano  */
end_comment

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
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"diffcore.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"branch.h"
end_include

begin_decl_stmt
DECL|variable|builtin_reset_usage
specifier|static
specifier|const
name|char
name|builtin_reset_usage
index|[]
init|=
literal|"git-reset [--mixed | --soft | --hard] [-q] [<commit-ish>] [ [--]<paths>...]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|args_to_str
specifier|static
name|char
modifier|*
name|args_to_str
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|char
modifier|*
name|buf
init|=
name|NULL
decl_stmt|;
name|unsigned
name|long
name|len
decl_stmt|,
name|space
init|=
literal|0
decl_stmt|,
name|nr
init|=
literal|0
decl_stmt|;
for|for
control|(
init|;
operator|*
name|argv
condition|;
name|argv
operator|++
control|)
block|{
name|len
operator|=
name|strlen
argument_list|(
operator|*
name|argv
argument_list|)
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|buf
argument_list|,
name|nr
operator|+
literal|1
operator|+
name|len
argument_list|,
name|space
argument_list|)
expr_stmt|;
if|if
condition|(
name|nr
condition|)
name|buf
index|[
name|nr
operator|++
index|]
operator|=
literal|' '
expr_stmt|;
name|memcpy
argument_list|(
name|buf
operator|+
name|nr
argument_list|,
operator|*
name|argv
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|nr
operator|+=
name|len
expr_stmt|;
block|}
name|ALLOC_GROW
argument_list|(
name|buf
argument_list|,
name|nr
operator|+
literal|1
argument_list|,
name|space
argument_list|)
expr_stmt|;
name|buf
index|[
name|nr
index|]
operator|=
literal|'\0'
expr_stmt|;
return|return
name|buf
return|;
block|}
end_function

begin_function
DECL|function|is_merge
specifier|static
specifier|inline
name|int
name|is_merge
parameter_list|(
name|void
parameter_list|)
block|{
return|return
operator|!
name|access
argument_list|(
name|git_path
argument_list|(
literal|"MERGE_HEAD"
argument_list|)
argument_list|,
name|F_OK
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|reset_index_file
specifier|static
name|int
name|reset_index_file
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|is_hard_reset
parameter_list|)
block|{
name|int
name|i
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|args
index|[
literal|6
index|]
decl_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
literal|"read-tree"
expr_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
literal|"-v"
expr_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
literal|"--reset"
expr_stmt|;
if|if
condition|(
name|is_hard_reset
condition|)
name|args
index|[
name|i
operator|++
index|]
operator|=
literal|"-u"
expr_stmt|;
name|args
index|[
name|i
operator|++
index|]
operator|=
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|args
index|[
name|i
index|]
operator|=
name|NULL
expr_stmt|;
return|return
name|run_command_v_opt
argument_list|(
name|args
argument_list|,
name|RUN_GIT_CMD
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|print_new_head_line
specifier|static
name|void
name|print_new_head_line
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|hex
decl_stmt|,
modifier|*
name|dots
init|=
literal|"..."
decl_stmt|,
modifier|*
name|body
decl_stmt|;
name|hex
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
if|if
condition|(
operator|!
name|hex
condition|)
block|{
name|hex
operator|=
name|sha1_to_hex
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
expr_stmt|;
name|dots
operator|=
literal|""
expr_stmt|;
block|}
name|printf
argument_list|(
literal|"HEAD is now at %s%s"
argument_list|,
name|hex
argument_list|,
name|dots
argument_list|)
expr_stmt|;
name|body
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
name|body
condition|)
block|{
specifier|const
name|char
modifier|*
name|eol
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|body
operator|+=
literal|2
expr_stmt|;
name|eol
operator|=
name|strchr
argument_list|(
name|body
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
name|len
operator|=
name|eol
condition|?
name|eol
operator|-
name|body
else|:
name|strlen
argument_list|(
name|body
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|" %.*s\n"
argument_list|,
operator|(
name|int
operator|)
name|len
argument_list|,
name|body
argument_list|)
expr_stmt|;
block|}
else|else
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|update_index_refresh
specifier|static
name|int
name|update_index_refresh
parameter_list|(
name|int
name|fd
parameter_list|,
name|struct
name|lock_file
modifier|*
name|index_lock
parameter_list|)
block|{
name|int
name|result
decl_stmt|;
if|if
condition|(
operator|!
name|index_lock
condition|)
block|{
name|index_lock
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|lock_file
argument_list|)
argument_list|)
expr_stmt|;
name|fd
operator|=
name|hold_locked_index
argument_list|(
name|index_lock
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"Could not read index"
argument_list|)
return|;
name|result
operator|=
name|refresh_cache
argument_list|(
literal|0
argument_list|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
if|if
condition|(
name|write_cache
argument_list|(
name|fd
argument_list|,
name|active_cache
argument_list|,
name|active_nr
argument_list|)
operator|||
name|commit_locked_index
argument_list|(
name|index_lock
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"Could not refresh index"
argument_list|)
return|;
return|return
name|result
return|;
block|}
end_function

begin_function
DECL|function|update_index_from_diff
specifier|static
name|void
name|update_index_from_diff
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
name|q
parameter_list|,
name|struct
name|diff_options
modifier|*
name|opt
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
modifier|*
name|discard_flag
init|=
name|data
decl_stmt|;
comment|/* do_diff_cache() mangled the index */
name|discard_cache
argument_list|()
expr_stmt|;
operator|*
name|discard_flag
operator|=
literal|1
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
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|diff_filespec
modifier|*
name|one
init|=
name|q
operator|->
name|queue
index|[
name|i
index|]
operator|->
name|one
decl_stmt|;
if|if
condition|(
name|one
operator|->
name|mode
condition|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|;
name|ce
operator|=
name|make_cache_entry
argument_list|(
name|one
operator|->
name|mode
argument_list|,
name|one
operator|->
name|sha1
argument_list|,
name|one
operator|->
name|path
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|add_cache_entry
argument_list|(
name|ce
argument_list|,
name|ADD_CACHE_OK_TO_ADD
operator||
name|ADD_CACHE_OK_TO_REPLACE
argument_list|)
expr_stmt|;
block|}
else|else
name|remove_file_from_cache
argument_list|(
name|one
operator|->
name|path
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|read_from_tree
specifier|static
name|int
name|read_from_tree
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|unsigned
name|char
modifier|*
name|tree_sha1
parameter_list|)
block|{
name|struct
name|lock_file
modifier|*
name|lock
init|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|lock_file
argument_list|)
argument_list|)
decl_stmt|;
name|int
name|index_fd
decl_stmt|,
name|index_was_discarded
init|=
literal|0
decl_stmt|;
name|struct
name|diff_options
name|opt
decl_stmt|;
name|memset
argument_list|(
operator|&
name|opt
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|opt
argument_list|)
argument_list|)
expr_stmt|;
name|diff_tree_setup_paths
argument_list|(
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
name|argv
argument_list|)
argument_list|,
operator|&
name|opt
argument_list|)
expr_stmt|;
name|opt
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_CALLBACK
expr_stmt|;
name|opt
operator|.
name|format_callback
operator|=
name|update_index_from_diff
expr_stmt|;
name|opt
operator|.
name|format_callback_data
operator|=
operator|&
name|index_was_discarded
expr_stmt|;
name|index_fd
operator|=
name|hold_locked_index
argument_list|(
name|lock
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|index_was_discarded
operator|=
literal|0
expr_stmt|;
name|read_cache
argument_list|()
expr_stmt|;
if|if
condition|(
name|do_diff_cache
argument_list|(
name|tree_sha1
argument_list|,
operator|&
name|opt
argument_list|)
condition|)
return|return
literal|1
return|;
name|diffcore_std
argument_list|(
operator|&
name|opt
argument_list|)
expr_stmt|;
name|diff_flush
argument_list|(
operator|&
name|opt
argument_list|)
expr_stmt|;
name|diff_tree_release_paths
argument_list|(
operator|&
name|opt
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|index_was_discarded
condition|)
comment|/* The index is still clobbered from do_diff_cache() */
name|discard_cache
argument_list|()
expr_stmt|;
return|return
name|update_index_refresh
argument_list|(
name|index_fd
argument_list|,
name|lock
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|prepend_reflog_action
specifier|static
name|void
name|prepend_reflog_action
parameter_list|(
specifier|const
name|char
modifier|*
name|action
parameter_list|,
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|sep
init|=
literal|": "
decl_stmt|;
specifier|const
name|char
modifier|*
name|rla
init|=
name|getenv
argument_list|(
literal|"GIT_REFLOG_ACTION"
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|rla
condition|)
name|rla
operator|=
name|sep
operator|=
literal|""
expr_stmt|;
if|if
condition|(
name|snprintf
argument_list|(
name|buf
argument_list|,
name|size
argument_list|,
literal|"%s%s%s"
argument_list|,
name|rla
argument_list|,
name|sep
argument_list|,
name|action
argument_list|)
operator|>=
name|size
condition|)
name|warning
argument_list|(
literal|"Reflog action message too long: %.*s..."
argument_list|,
literal|50
argument_list|,
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_enum
DECL|enum|reset_type
DECL|enumerator|MIXED
DECL|enumerator|SOFT
DECL|enumerator|HARD
DECL|enumerator|NONE
enum|enum
name|reset_type
block|{
name|MIXED
block|,
name|SOFT
block|,
name|HARD
block|,
name|NONE
block|}
enum|;
end_enum

begin_decl_stmt
DECL|variable|reset_type_names
specifier|static
specifier|const
name|char
modifier|*
name|reset_type_names
index|[]
init|=
block|{
literal|"mixed"
block|,
literal|"soft"
block|,
literal|"hard"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_reset
name|int
name|cmd_reset
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
name|int
name|i
init|=
literal|1
decl_stmt|,
name|reset_type
init|=
name|NONE
decl_stmt|,
name|update_ref_status
init|=
literal|0
decl_stmt|,
name|quiet
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|rev
init|=
literal|"HEAD"
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|,
modifier|*
name|orig
init|=
name|NULL
decl_stmt|,
name|sha1_orig
index|[
literal|20
index|]
decl_stmt|,
modifier|*
name|old_orig
init|=
name|NULL
decl_stmt|,
name|sha1_old_orig
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|char
modifier|*
name|reflog_action
decl_stmt|,
name|msg
index|[
literal|1024
index|]
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
name|reflog_action
operator|=
name|args_to_str
argument_list|(
name|argv
argument_list|)
expr_stmt|;
name|setenv
argument_list|(
literal|"GIT_REFLOG_ACTION"
argument_list|,
name|reflog_action
argument_list|,
literal|0
argument_list|)
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|argc
condition|)
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
literal|"--mixed"
argument_list|)
condition|)
block|{
name|reset_type
operator|=
name|MIXED
expr_stmt|;
name|i
operator|++
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
literal|"--soft"
argument_list|)
condition|)
block|{
name|reset_type
operator|=
name|SOFT
expr_stmt|;
name|i
operator|++
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
literal|"--hard"
argument_list|)
condition|)
block|{
name|reset_type
operator|=
name|HARD
expr_stmt|;
name|i
operator|++
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
literal|"-q"
argument_list|)
condition|)
block|{
name|quiet
operator|=
literal|1
expr_stmt|;
name|i
operator|++
expr_stmt|;
block|}
else|else
break|break;
block|}
if|if
condition|(
name|i
operator|<
name|argc
operator|&&
name|argv
index|[
name|i
index|]
index|[
literal|0
index|]
operator|!=
literal|'-'
condition|)
name|rev
operator|=
name|argv
index|[
name|i
operator|++
index|]
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|rev
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Failed to resolve '%s' as a valid ref."
argument_list|,
name|rev
argument_list|)
expr_stmt|;
name|commit
operator|=
name|lookup_commit_reference
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
literal|"Could not parse object '%s'."
argument_list|,
name|rev
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|sha1
argument_list|,
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|i
operator|<
name|argc
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--"
argument_list|)
condition|)
name|i
operator|++
expr_stmt|;
elseif|else
if|if
condition|(
name|i
operator|<
name|argc
operator|&&
name|argv
index|[
name|i
index|]
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
name|usage
argument_list|(
name|builtin_reset_usage
argument_list|)
expr_stmt|;
comment|/* git reset tree [--] paths... can be used to 	 * load chosen paths from the tree into the index without 	 * affecting the working tree nor HEAD. */
if|if
condition|(
name|i
operator|<
name|argc
condition|)
block|{
if|if
condition|(
name|reset_type
operator|==
name|MIXED
condition|)
name|warning
argument_list|(
literal|"--mixed option is deprecated with paths."
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|reset_type
operator|!=
name|NONE
condition|)
name|die
argument_list|(
literal|"Cannot do %s reset with paths."
argument_list|,
name|reset_type_names
index|[
name|reset_type
index|]
argument_list|)
expr_stmt|;
return|return
name|read_from_tree
argument_list|(
name|prefix
argument_list|,
name|argv
operator|+
name|i
argument_list|,
name|sha1
argument_list|)
return|;
block|}
if|if
condition|(
name|reset_type
operator|==
name|NONE
condition|)
name|reset_type
operator|=
name|MIXED
expr_stmt|;
comment|/* by default */
if|if
condition|(
name|reset_type
operator|==
name|HARD
operator|&&
name|is_bare_repository
argument_list|()
condition|)
name|die
argument_list|(
literal|"hard reset makes no sense in a bare repository"
argument_list|)
expr_stmt|;
comment|/* Soft reset does not touch the index file nor the working tree 	 * at all, but requires them in a good order.  Other resets reset 	 * the index file to the tree object we are switching to. */
if|if
condition|(
name|reset_type
operator|==
name|SOFT
condition|)
block|{
if|if
condition|(
name|is_merge
argument_list|()
operator|||
name|read_cache
argument_list|()
operator|<
literal|0
operator|||
name|unmerged_cache
argument_list|()
condition|)
name|die
argument_list|(
literal|"Cannot do a soft reset in the middle of a merge."
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|reset_index_file
argument_list|(
name|sha1
argument_list|,
operator|(
name|reset_type
operator|==
name|HARD
operator|)
argument_list|)
condition|)
name|die
argument_list|(
literal|"Could not reset index file to revision '%s'."
argument_list|,
name|rev
argument_list|)
expr_stmt|;
comment|/* Any resets update HEAD to the head being switched to, 	 * saving the previous head in ORIG_HEAD before. */
if|if
condition|(
operator|!
name|get_sha1
argument_list|(
literal|"ORIG_HEAD"
argument_list|,
name|sha1_old_orig
argument_list|)
condition|)
name|old_orig
operator|=
name|sha1_old_orig
expr_stmt|;
if|if
condition|(
operator|!
name|get_sha1
argument_list|(
literal|"HEAD"
argument_list|,
name|sha1_orig
argument_list|)
condition|)
block|{
name|orig
operator|=
name|sha1_orig
expr_stmt|;
name|prepend_reflog_action
argument_list|(
literal|"updating ORIG_HEAD"
argument_list|,
name|msg
argument_list|,
sizeof|sizeof
argument_list|(
name|msg
argument_list|)
argument_list|)
expr_stmt|;
name|update_ref
argument_list|(
name|msg
argument_list|,
literal|"ORIG_HEAD"
argument_list|,
name|orig
argument_list|,
name|old_orig
argument_list|,
literal|0
argument_list|,
name|MSG_ON_ERR
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|old_orig
condition|)
name|delete_ref
argument_list|(
literal|"ORIG_HEAD"
argument_list|,
name|old_orig
argument_list|)
expr_stmt|;
name|prepend_reflog_action
argument_list|(
literal|"updating HEAD"
argument_list|,
name|msg
argument_list|,
sizeof|sizeof
argument_list|(
name|msg
argument_list|)
argument_list|)
expr_stmt|;
name|update_ref_status
operator|=
name|update_ref
argument_list|(
name|msg
argument_list|,
literal|"HEAD"
argument_list|,
name|sha1
argument_list|,
name|orig
argument_list|,
literal|0
argument_list|,
name|MSG_ON_ERR
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|reset_type
condition|)
block|{
case|case
name|HARD
case|:
if|if
condition|(
operator|!
name|update_ref_status
operator|&&
operator|!
name|quiet
condition|)
name|print_new_head_line
argument_list|(
name|commit
argument_list|)
expr_stmt|;
break|break;
case|case
name|SOFT
case|:
comment|/* Nothing else to do. */
break|break;
case|case
name|MIXED
case|:
comment|/* Report what has not been updated. */
name|update_index_refresh
argument_list|(
literal|0
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
break|break;
block|}
name|remove_branch_state
argument_list|()
expr_stmt|;
name|free
argument_list|(
name|reflog_action
argument_list|)
expr_stmt|;
return|return
name|update_ref_status
return|;
block|}
end_function

end_unit

