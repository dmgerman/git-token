begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * "git add" builtin command  *  * Copyright (C) 2006 Linus Torvalds  */
end_comment

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
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_include
include|#
directive|include
file|"cache-tree.h"
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
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_decl_stmt
DECL|variable|builtin_add_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|builtin_add_usage
index|[]
init|=
block|{
literal|"git-add [options] [--]<filepattern>..."
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|take_worktree_changes
specifier|static
name|int
name|take_worktree_changes
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|prune_directory
specifier|static
name|void
name|prune_directory
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|,
name|int
name|prefix
parameter_list|)
block|{
name|char
modifier|*
name|seen
decl_stmt|;
name|int
name|i
decl_stmt|,
name|specs
decl_stmt|;
name|struct
name|dir_entry
modifier|*
modifier|*
name|src
decl_stmt|,
modifier|*
modifier|*
name|dst
decl_stmt|;
for|for
control|(
name|specs
operator|=
literal|0
init|;
name|pathspec
index|[
name|specs
index|]
condition|;
name|specs
operator|++
control|)
comment|/* nothing */
empty_stmt|;
name|seen
operator|=
name|xcalloc
argument_list|(
name|specs
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|src
operator|=
name|dst
operator|=
name|dir
operator|->
name|entries
expr_stmt|;
name|i
operator|=
name|dir
operator|->
name|nr
expr_stmt|;
while|while
condition|(
operator|--
name|i
operator|>=
literal|0
condition|)
block|{
name|struct
name|dir_entry
modifier|*
name|entry
init|=
operator|*
name|src
operator|++
decl_stmt|;
if|if
condition|(
name|match_pathspec
argument_list|(
name|pathspec
argument_list|,
name|entry
operator|->
name|name
argument_list|,
name|entry
operator|->
name|len
argument_list|,
name|prefix
argument_list|,
name|seen
argument_list|)
condition|)
operator|*
name|dst
operator|++
operator|=
name|entry
expr_stmt|;
block|}
name|dir
operator|->
name|nr
operator|=
name|dst
operator|-
name|dir
operator|->
name|entries
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|specs
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|seen
index|[
name|i
index|]
operator|&&
operator|!
name|file_exists
argument_list|(
name|pathspec
index|[
name|i
index|]
argument_list|)
condition|)
name|die
argument_list|(
literal|"pathspec '%s' did not match any files"
argument_list|,
name|pathspec
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|seen
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|fill_directory
specifier|static
name|void
name|fill_directory
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|,
name|int
name|ignored_too
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|path
decl_stmt|,
modifier|*
name|base
decl_stmt|;
name|int
name|baselen
decl_stmt|;
comment|/* Set up the default git porcelain excludes */
name|memset
argument_list|(
name|dir
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|dir
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ignored_too
condition|)
block|{
name|dir
operator|->
name|collect_ignored
operator|=
literal|1
expr_stmt|;
name|setup_standard_excludes
argument_list|(
name|dir
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * Calculate common prefix for the pathspec, and 	 * use that to optimize the directory walk 	 */
name|baselen
operator|=
name|common_prefix
argument_list|(
name|pathspec
argument_list|)
expr_stmt|;
name|path
operator|=
literal|"."
expr_stmt|;
name|base
operator|=
literal|""
expr_stmt|;
if|if
condition|(
name|baselen
condition|)
name|path
operator|=
name|base
operator|=
name|xmemdupz
argument_list|(
operator|*
name|pathspec
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
comment|/* Read the directory and prune it */
name|read_directory
argument_list|(
name|dir
argument_list|,
name|path
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|,
name|pathspec
argument_list|)
expr_stmt|;
if|if
condition|(
name|pathspec
condition|)
name|prune_directory
argument_list|(
name|dir
argument_list|,
name|pathspec
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|update_callback
specifier|static
name|void
name|update_callback
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
name|cbdata
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|verbose
decl_stmt|;
name|verbose
operator|=
operator|*
operator|(
operator|(
name|int
operator|*
operator|)
name|cbdata
operator|)
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
name|diff_filepair
modifier|*
name|p
init|=
name|q
operator|->
name|queue
index|[
name|i
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
init|=
name|p
operator|->
name|one
operator|->
name|path
decl_stmt|;
switch|switch
condition|(
name|p
operator|->
name|status
condition|)
block|{
default|default:
name|die
argument_list|(
literal|"unexpected diff status %c"
argument_list|,
name|p
operator|->
name|status
argument_list|)
expr_stmt|;
case|case
name|DIFF_STATUS_UNMERGED
case|:
case|case
name|DIFF_STATUS_MODIFIED
case|:
case|case
name|DIFF_STATUS_TYPE_CHANGED
case|:
name|add_file_to_cache
argument_list|(
name|path
argument_list|,
name|verbose
argument_list|)
expr_stmt|;
break|break;
case|case
name|DIFF_STATUS_DELETED
case|:
name|remove_file_from_cache
argument_list|(
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|printf
argument_list|(
literal|"remove '%s'\n"
argument_list|,
name|path
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
block|}
end_function

begin_function
DECL|function|add_files_to_cache
name|void
name|add_files_to_cache
parameter_list|(
name|int
name|verbose
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|files
parameter_list|)
block|{
name|struct
name|rev_info
name|rev
decl_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|setup_revisions
argument_list|(
literal|0
argument_list|,
name|NULL
argument_list|,
operator|&
name|rev
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|rev
operator|.
name|prune_data
operator|=
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|files
argument_list|)
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_CALLBACK
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|format_callback
operator|=
name|update_callback
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|format_callback_data
operator|=
operator|&
name|verbose
expr_stmt|;
name|run_diff_files
argument_list|(
operator|&
name|rev
argument_list|,
name|DIFF_RACY_IS_MODIFIED
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|refresh
specifier|static
name|void
name|refresh
parameter_list|(
name|int
name|verbose
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
block|{
name|char
modifier|*
name|seen
decl_stmt|;
name|int
name|i
decl_stmt|,
name|specs
decl_stmt|;
for|for
control|(
name|specs
operator|=
literal|0
init|;
name|pathspec
index|[
name|specs
index|]
condition|;
name|specs
operator|++
control|)
comment|/* nothing */
empty_stmt|;
name|seen
operator|=
name|xcalloc
argument_list|(
name|specs
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"index file corrupt"
argument_list|)
expr_stmt|;
name|refresh_index
argument_list|(
operator|&
name|the_index
argument_list|,
name|verbose
condition|?
literal|0
else|:
name|REFRESH_QUIET
argument_list|,
name|pathspec
argument_list|,
name|seen
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
name|specs
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|seen
index|[
name|i
index|]
condition|)
name|die
argument_list|(
literal|"pathspec '%s' did not match any files"
argument_list|,
name|pathspec
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|seen
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|interactive_add
name|int
name|interactive_add
parameter_list|(
name|void
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|argv
index|[
literal|2
index|]
init|=
block|{
literal|"add--interactive"
block|,
name|NULL
block|}
decl_stmt|;
return|return
name|run_command_v_opt
argument_list|(
name|argv
argument_list|,
name|RUN_GIT_CMD
argument_list|)
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|lock_file
specifier|static
name|struct
name|lock_file
name|lock_file
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ignore_error
specifier|static
specifier|const
name|char
name|ignore_error
index|[]
init|=
literal|"The following paths are ignored by one of your .gitignore files:\n"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|verbose
DECL|variable|show_only
DECL|variable|ignored_too
DECL|variable|refresh_only
specifier|static
name|int
name|verbose
init|=
literal|0
decl_stmt|,
name|show_only
init|=
literal|0
decl_stmt|,
name|ignored_too
init|=
literal|0
decl_stmt|,
name|refresh_only
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|add_interactive
specifier|static
name|int
name|add_interactive
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|builtin_add_options
specifier|static
name|struct
name|option
name|builtin_add_options
index|[]
init|=
block|{
name|OPT__DRY_RUN
argument_list|(
operator|&
name|show_only
argument_list|)
block|,
name|OPT__VERBOSE
argument_list|(
operator|&
name|verbose
argument_list|)
block|,
name|OPT_GROUP
argument_list|(
literal|""
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'i'
argument_list|,
literal|"interactive"
argument_list|,
operator|&
name|add_interactive
argument_list|,
literal|"interactive picking"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'f'
argument_list|,
name|NULL
argument_list|,
operator|&
name|ignored_too
argument_list|,
literal|"allow adding otherwise ignored files"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'u'
argument_list|,
name|NULL
argument_list|,
operator|&
name|take_worktree_changes
argument_list|,
literal|"update tracked files"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"refresh"
argument_list|,
operator|&
name|refresh_only
argument_list|,
literal|"don't add, only refresh the index"
argument_list|)
block|,
name|OPT_END
argument_list|()
block|, }
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_add
name|int
name|cmd_add
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
decl_stmt|,
name|newfd
decl_stmt|,
name|orig_argc
init|=
name|argc
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
decl_stmt|;
name|struct
name|dir_struct
name|dir
decl_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|builtin_add_options
argument_list|,
name|builtin_add_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|add_interactive
condition|)
block|{
if|if
condition|(
name|add_interactive
operator|!=
literal|1
operator|||
name|orig_argc
operator|!=
literal|2
condition|)
name|die
argument_list|(
literal|"add --interactive does not take any parameters"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
name|interactive_add
argument_list|()
argument_list|)
expr_stmt|;
block|}
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
name|newfd
operator|=
name|hold_locked_index
argument_list|(
operator|&
name|lock_file
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|take_worktree_changes
condition|)
block|{
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"index file corrupt"
argument_list|)
expr_stmt|;
name|add_files_to_cache
argument_list|(
name|verbose
argument_list|,
name|prefix
argument_list|,
name|argv
argument_list|)
expr_stmt|;
goto|goto
name|finish
goto|;
block|}
if|if
condition|(
name|argc
operator|==
literal|0
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Nothing specified, nothing added.\n"
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Maybe you wanted to say 'git add .'?\n"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|pathspec
operator|=
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
argument_list|)
expr_stmt|;
if|if
condition|(
name|refresh_only
condition|)
block|{
name|refresh
argument_list|(
name|verbose
argument_list|,
name|pathspec
argument_list|)
expr_stmt|;
goto|goto
name|finish
goto|;
block|}
name|fill_directory
argument_list|(
operator|&
name|dir
argument_list|,
name|pathspec
argument_list|,
name|ignored_too
argument_list|)
expr_stmt|;
if|if
condition|(
name|show_only
condition|)
block|{
specifier|const
name|char
modifier|*
name|sep
init|=
literal|""
decl_stmt|,
modifier|*
name|eof
init|=
literal|""
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|dir
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|printf
argument_list|(
literal|"%s%s"
argument_list|,
name|sep
argument_list|,
name|dir
operator|.
name|entries
index|[
name|i
index|]
operator|->
name|name
argument_list|)
expr_stmt|;
name|sep
operator|=
literal|" "
expr_stmt|;
name|eof
operator|=
literal|"\n"
expr_stmt|;
block|}
name|fputs
argument_list|(
name|eof
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"index file corrupt"
argument_list|)
expr_stmt|;
if|if
condition|(
name|dir
operator|.
name|ignored_nr
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
name|ignore_error
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
name|dir
operator|.
name|ignored_nr
condition|;
name|i
operator|++
control|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s\n"
argument_list|,
name|dir
operator|.
name|ignored
index|[
name|i
index|]
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Use -f if you really want to add them.\n"
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"no files added"
argument_list|)
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|dir
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|add_file_to_cache
argument_list|(
name|dir
operator|.
name|entries
index|[
name|i
index|]
operator|->
name|name
argument_list|,
name|verbose
argument_list|)
expr_stmt|;
name|finish
label|:
if|if
condition|(
name|active_cache_changed
condition|)
block|{
if|if
condition|(
name|write_cache
argument_list|(
name|newfd
argument_list|,
name|active_cache
argument_list|,
name|active_nr
argument_list|)
operator|||
name|close
argument_list|(
name|newfd
argument_list|)
operator|||
name|commit_locked_index
argument_list|(
operator|&
name|lock_file
argument_list|)
condition|)
name|die
argument_list|(
literal|"Unable to write new index file"
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

