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
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
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
file|"revision.h"
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
literal|"git add [options] [--]<filepattern>..."
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|patch_interactive
DECL|variable|add_interactive
DECL|variable|edit_interactive
specifier|static
name|int
name|patch_interactive
decl_stmt|,
name|add_interactive
decl_stmt|,
name|edit_interactive
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|take_worktree_changes
specifier|static
name|int
name|take_worktree_changes
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|update_callback_data
struct|struct
name|update_callback_data
block|{
DECL|member|flags
name|int
name|flags
decl_stmt|;
DECL|member|add_errors
name|int
name|add_errors
decl_stmt|;
block|}
struct|;
end_struct

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
decl_stmt|;
name|struct
name|update_callback_data
modifier|*
name|data
init|=
name|cbdata
decl_stmt|;
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
comment|/* 			 * ADD_CACHE_IGNORE_REMOVAL is unset if "git 			 * add -u" is calling us, In such a case, a 			 * missing work tree file needs to be removed 			 * if there is an unmerged entry at stage #2, 			 * but such a diff record is followed by 			 * another with DIFF_STATUS_DELETED (and if 			 * there is no stage #2, we won't see DELETED 			 * nor MODIFIED).  We can simply continue 			 * either way. 			 */
if|if
condition|(
operator|!
operator|(
name|data
operator|->
name|flags
operator|&
name|ADD_CACHE_IGNORE_REMOVAL
operator|)
condition|)
continue|continue;
comment|/* 			 * Otherwise, it is "git add path" is asking 			 * to explicitly add it; we fall through.  A 			 * missing work tree file is an error and is 			 * caught by add_file_to_index() in such a 			 * case. 			 */
case|case
name|DIFF_STATUS_MODIFIED
case|:
case|case
name|DIFF_STATUS_TYPE_CHANGED
case|:
if|if
condition|(
name|add_file_to_index
argument_list|(
operator|&
name|the_index
argument_list|,
name|path
argument_list|,
name|data
operator|->
name|flags
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|data
operator|->
name|flags
operator|&
name|ADD_CACHE_IGNORE_ERRORS
operator|)
condition|)
name|die
argument_list|(
literal|"updating files failed"
argument_list|)
expr_stmt|;
name|data
operator|->
name|add_errors
operator|++
expr_stmt|;
block|}
break|break;
case|case
name|DIFF_STATUS_DELETED
case|:
if|if
condition|(
name|data
operator|->
name|flags
operator|&
name|ADD_CACHE_IGNORE_REMOVAL
condition|)
break|break;
if|if
condition|(
operator|!
operator|(
name|data
operator|->
name|flags
operator|&
name|ADD_CACHE_PRETEND
operator|)
condition|)
name|remove_file_from_index
argument_list|(
operator|&
name|the_index
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|data
operator|->
name|flags
operator|&
operator|(
name|ADD_CACHE_PRETEND
operator||
name|ADD_CACHE_VERBOSE
operator|)
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
name|int
name|add_files_to_cache
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
name|pathspec
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|struct
name|update_callback_data
name|data
decl_stmt|;
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
name|pathspec
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
name|data
operator|.
name|flags
operator|=
name|flags
expr_stmt|;
name|data
operator|.
name|add_errors
operator|=
literal|0
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|format_callback_data
operator|=
operator|&
name|data
expr_stmt|;
name|run_diff_files
argument_list|(
operator|&
name|rev
argument_list|,
name|DIFF_RACY_IS_MODIFIED
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|data
operator|.
name|add_errors
return|;
block|}
end_function

begin_function
DECL|function|fill_pathspec_matches
specifier|static
name|void
name|fill_pathspec_matches
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|,
name|char
modifier|*
name|seen
parameter_list|,
name|int
name|specs
parameter_list|)
block|{
name|int
name|num_unmatched
init|=
literal|0
decl_stmt|,
name|i
decl_stmt|;
comment|/* 	 * Since we are walking the index as if we were walking the directory, 	 * we have to mark the matched pathspec as seen; otherwise we will 	 * mistakenly think that the user gave a pathspec that did not match 	 * anything. 	 */
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
if|if
condition|(
operator|!
name|seen
index|[
name|i
index|]
condition|)
name|num_unmatched
operator|++
expr_stmt|;
if|if
condition|(
operator|!
name|num_unmatched
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
name|active_nr
condition|;
name|i
operator|++
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
index|]
decl_stmt|;
name|match_pathspec
argument_list|(
name|pathspec
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|ce_namelen
argument_list|(
name|ce
argument_list|)
argument_list|,
literal|0
argument_list|,
name|seen
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|find_used_pathspec
specifier|static
name|char
modifier|*
name|find_used_pathspec
parameter_list|(
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
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|pathspec
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
empty_stmt|;
comment|/* just counting */
name|seen
operator|=
name|xcalloc
argument_list|(
name|i
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|fill_pathspec_matches
argument_list|(
name|pathspec
argument_list|,
name|seen
argument_list|,
name|i
argument_list|)
expr_stmt|;
return|return
name|seen
return|;
block|}
end_function

begin_function
DECL|function|prune_directory
specifier|static
name|char
modifier|*
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
name|fill_pathspec_matches
argument_list|(
name|pathspec
argument_list|,
name|seen
argument_list|,
name|specs
argument_list|)
expr_stmt|;
return|return
name|seen
return|;
block|}
end_function

begin_function
DECL|function|treat_gitlinks
specifier|static
name|void
name|treat_gitlinks
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
operator|!
name|pathspec
operator|||
operator|!
operator|*
name|pathspec
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
name|active_nr
condition|;
name|i
operator|++
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
index|]
decl_stmt|;
if|if
condition|(
name|S_ISGITLINK
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
condition|)
block|{
name|int
name|len
init|=
name|ce_namelen
argument_list|(
name|ce
argument_list|)
decl_stmt|,
name|j
decl_stmt|;
for|for
control|(
name|j
operator|=
literal|0
init|;
name|pathspec
index|[
name|j
index|]
condition|;
name|j
operator|++
control|)
block|{
name|int
name|len2
init|=
name|strlen
argument_list|(
name|pathspec
index|[
name|j
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
name|len2
operator|<=
name|len
operator|||
name|pathspec
index|[
name|j
index|]
index|[
name|len
index|]
operator|!=
literal|'/'
operator|||
name|memcmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|pathspec
index|[
name|j
index|]
argument_list|,
name|len
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|len2
operator|==
name|len
operator|+
literal|1
condition|)
comment|/* strip trailing slash */
name|pathspec
index|[
name|j
index|]
operator|=
name|xstrndup
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|len
argument_list|)
expr_stmt|;
else|else
name|die
argument_list|(
literal|"Path '%s' is in submodule '%.*s'"
argument_list|,
name|pathspec
index|[
name|j
index|]
argument_list|,
name|len
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
block|}
block|}
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
name|refresh_index
argument_list|(
operator|&
name|the_index
argument_list|,
name|verbose
condition|?
name|REFRESH_IN_PORCELAIN
else|:
name|REFRESH_QUIET
argument_list|,
name|pathspec
argument_list|,
name|seen
argument_list|,
literal|"Unstaged changes after refreshing the index:"
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
DECL|function|validate_pathspec
specifier|static
specifier|const
name|char
modifier|*
modifier|*
name|validate_pathspec
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
modifier|*
name|pathspec
init|=
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
argument_list|)
decl_stmt|;
if|if
condition|(
name|pathspec
condition|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|p
decl_stmt|;
for|for
control|(
name|p
operator|=
name|pathspec
init|;
operator|*
name|p
condition|;
name|p
operator|++
control|)
block|{
if|if
condition|(
name|has_symlink_leading_path
argument_list|(
operator|*
name|p
argument_list|,
name|strlen
argument_list|(
operator|*
name|p
argument_list|)
argument_list|)
condition|)
block|{
name|int
name|len
init|=
name|prefix
condition|?
name|strlen
argument_list|(
name|prefix
argument_list|)
else|:
literal|0
decl_stmt|;
name|die
argument_list|(
literal|"'%s' is beyond a symbolic link"
argument_list|,
operator|*
name|p
operator|+
name|len
argument_list|)
expr_stmt|;
block|}
block|}
block|}
return|return
name|pathspec
return|;
block|}
end_function

begin_function
DECL|function|run_add_interactive
name|int
name|run_add_interactive
parameter_list|(
specifier|const
name|char
modifier|*
name|revision
parameter_list|,
specifier|const
name|char
modifier|*
name|patch_mode
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
block|{
name|int
name|status
decl_stmt|,
name|ac
decl_stmt|,
name|pc
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|args
decl_stmt|;
if|if
condition|(
name|pathspec
condition|)
while|while
condition|(
name|pathspec
index|[
name|pc
index|]
condition|)
name|pc
operator|++
expr_stmt|;
name|args
operator|=
name|xcalloc
argument_list|(
sizeof|sizeof
argument_list|(
specifier|const
name|char
operator|*
argument_list|)
argument_list|,
operator|(
name|pc
operator|+
literal|5
operator|)
argument_list|)
expr_stmt|;
name|ac
operator|=
literal|0
expr_stmt|;
name|args
index|[
name|ac
operator|++
index|]
operator|=
literal|"add--interactive"
expr_stmt|;
if|if
condition|(
name|patch_mode
condition|)
name|args
index|[
name|ac
operator|++
index|]
operator|=
name|patch_mode
expr_stmt|;
if|if
condition|(
name|revision
condition|)
name|args
index|[
name|ac
operator|++
index|]
operator|=
name|revision
expr_stmt|;
name|args
index|[
name|ac
operator|++
index|]
operator|=
literal|"--"
expr_stmt|;
if|if
condition|(
name|pc
condition|)
block|{
name|memcpy
argument_list|(
operator|&
operator|(
name|args
index|[
name|ac
index|]
operator|)
argument_list|,
name|pathspec
argument_list|,
sizeof|sizeof
argument_list|(
specifier|const
name|char
operator|*
argument_list|)
operator|*
name|pc
argument_list|)
expr_stmt|;
name|ac
operator|+=
name|pc
expr_stmt|;
block|}
name|args
index|[
name|ac
index|]
operator|=
name|NULL
expr_stmt|;
name|status
operator|=
name|run_command_v_opt
argument_list|(
name|args
argument_list|,
name|RUN_GIT_CMD
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|args
argument_list|)
expr_stmt|;
return|return
name|status
return|;
block|}
end_function

begin_function
DECL|function|interactive_add
name|int
name|interactive_add
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
modifier|*
name|pathspec
init|=
name|NULL
decl_stmt|;
if|if
condition|(
name|argc
condition|)
block|{
name|pathspec
operator|=
name|validate_pathspec
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pathspec
condition|)
return|return
operator|-
literal|1
return|;
block|}
return|return
name|run_add_interactive
argument_list|(
name|NULL
argument_list|,
name|patch_interactive
condition|?
literal|"--patch"
else|:
name|NULL
argument_list|,
name|pathspec
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|edit_patch
specifier|static
name|int
name|edit_patch
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
name|char
modifier|*
name|file
init|=
name|xstrdup
argument_list|(
name|git_path
argument_list|(
literal|"ADD_EDIT.patch"
argument_list|)
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|apply_argv
index|[]
init|=
block|{
literal|"apply"
block|,
literal|"--recount"
block|,
literal|"--cached"
block|,
name|NULL
block|,
name|NULL
block|}
decl_stmt|;
name|struct
name|child_process
name|child
decl_stmt|;
name|struct
name|rev_info
name|rev
decl_stmt|;
name|int
name|out
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|apply_argv
index|[
literal|3
index|]
operator|=
name|file
expr_stmt|;
name|git_config
argument_list|(
name|git_diff_basic_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
comment|/* no "diff" UI options */
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Could not read the index"
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
name|diffopt
operator|.
name|context
operator|=
literal|7
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
name|NULL
argument_list|)
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_PATCH
expr_stmt|;
name|out
operator|=
name|open
argument_list|(
name|file
argument_list|,
name|O_CREAT
operator||
name|O_WRONLY
argument_list|,
literal|0644
argument_list|)
expr_stmt|;
if|if
condition|(
name|out
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Could not open '%s' for writing."
argument_list|,
name|file
argument_list|)
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|file
operator|=
name|xfdopen
argument_list|(
name|out
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|close_file
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|run_diff_files
argument_list|(
operator|&
name|rev
argument_list|,
literal|0
argument_list|)
condition|)
name|die
argument_list|(
literal|"Could not write patch"
argument_list|)
expr_stmt|;
name|launch_editor
argument_list|(
name|file
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|file
argument_list|,
operator|&
name|st
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"Could not stat '%s'"
argument_list|,
name|file
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|st
operator|.
name|st_size
condition|)
name|die
argument_list|(
literal|"Empty patch. Aborted."
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|child
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|child
argument_list|)
argument_list|)
expr_stmt|;
name|child
operator|.
name|git_cmd
operator|=
literal|1
expr_stmt|;
name|child
operator|.
name|argv
operator|=
name|apply_argv
expr_stmt|;
if|if
condition|(
name|run_command
argument_list|(
operator|&
name|child
argument_list|)
condition|)
name|die
argument_list|(
literal|"Could not apply '%s'"
argument_list|,
name|file
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|file
argument_list|)
expr_stmt|;
return|return
literal|0
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
DECL|variable|ignore_add_errors
DECL|variable|addremove
DECL|variable|intent_to_add
DECL|variable|ignore_missing
specifier|static
name|int
name|ignore_add_errors
decl_stmt|,
name|addremove
decl_stmt|,
name|intent_to_add
decl_stmt|,
name|ignore_missing
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
argument_list|,
literal|"dry run"
argument_list|)
block|,
name|OPT__VERBOSE
argument_list|(
operator|&
name|verbose
argument_list|,
literal|"be verbose"
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
literal|'p'
argument_list|,
literal|"patch"
argument_list|,
operator|&
name|patch_interactive
argument_list|,
literal|"interactive patching"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'e'
argument_list|,
literal|"edit"
argument_list|,
operator|&
name|edit_interactive
argument_list|,
literal|"edit current diff and apply"
argument_list|)
block|,
name|OPT__FORCE
argument_list|(
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
literal|"update"
argument_list|,
operator|&
name|take_worktree_changes
argument_list|,
literal|"update tracked files"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'N'
argument_list|,
literal|"intent-to-add"
argument_list|,
operator|&
name|intent_to_add
argument_list|,
literal|"record only the fact that the path will be added later"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'A'
argument_list|,
literal|"all"
argument_list|,
operator|&
name|addremove
argument_list|,
literal|"add all, noticing removal of tracked files"
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
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"ignore-errors"
argument_list|,
operator|&
name|ignore_add_errors
argument_list|,
literal|"just skip files which cannot be added because of errors"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"ignore-missing"
argument_list|,
operator|&
name|ignore_missing
argument_list|,
literal|"check if - even missing - files are ignored in dry run"
argument_list|)
block|,
name|OPT_END
argument_list|()
block|, }
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|add_config
specifier|static
name|int
name|add_config
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
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|var
argument_list|,
literal|"add.ignore-errors"
argument_list|)
condition|)
block|{
name|ignore_add_errors
operator|=
name|git_config_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
return|return
name|git_default_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
name|cb
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|add_files
specifier|static
name|int
name|add_files
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|exit_status
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|dir
operator|->
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
operator|->
name|ignored_nr
condition|;
name|i
operator|++
control|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s\n"
argument_list|,
name|dir
operator|->
name|ignored
index|[
name|i
index|]
operator|->
name|name
argument_list|)
expr_stmt|;
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
operator|->
name|nr
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|add_file_to_cache
argument_list|(
name|dir
operator|->
name|entries
index|[
name|i
index|]
operator|->
name|name
argument_list|,
name|flags
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|ignore_add_errors
condition|)
name|die
argument_list|(
literal|"adding files failed"
argument_list|)
expr_stmt|;
name|exit_status
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|exit_status
return|;
block|}
end_function

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
name|exit_status
init|=
literal|0
decl_stmt|;
name|int
name|newfd
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
name|int
name|flags
decl_stmt|;
name|int
name|add_new_files
decl_stmt|;
name|int
name|require_pathspec
decl_stmt|;
name|char
modifier|*
name|seen
init|=
name|NULL
decl_stmt|;
name|git_config
argument_list|(
name|add_config
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
name|builtin_add_options
argument_list|,
name|builtin_add_usage
argument_list|,
name|PARSE_OPT_KEEP_ARGV0
argument_list|)
expr_stmt|;
if|if
condition|(
name|patch_interactive
condition|)
name|add_interactive
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|add_interactive
condition|)
name|exit
argument_list|(
name|interactive_add
argument_list|(
name|argc
operator|-
literal|1
argument_list|,
name|argv
operator|+
literal|1
argument_list|,
name|prefix
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|edit_interactive
condition|)
return|return
operator|(
name|edit_patch
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|)
operator|)
return|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
if|if
condition|(
name|addremove
operator|&&
name|take_worktree_changes
condition|)
name|die
argument_list|(
literal|"-A and -u are mutually incompatible"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|show_only
operator|&&
name|ignore_missing
condition|)
name|die
argument_list|(
literal|"Option --ignore-missing can only be used together with --dry-run"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|addremove
operator|||
name|take_worktree_changes
operator|)
operator|&&
operator|!
name|argc
condition|)
block|{
specifier|static
specifier|const
name|char
modifier|*
name|here
index|[
literal|2
index|]
init|=
block|{
literal|"."
block|,
name|NULL
block|}
decl_stmt|;
name|argc
operator|=
literal|1
expr_stmt|;
name|argv
operator|=
name|here
expr_stmt|;
block|}
name|add_new_files
operator|=
operator|!
name|take_worktree_changes
operator|&&
operator|!
name|refresh_only
expr_stmt|;
name|require_pathspec
operator|=
operator|!
name|take_worktree_changes
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
name|flags
operator|=
operator|(
operator|(
name|verbose
condition|?
name|ADD_CACHE_VERBOSE
else|:
literal|0
operator|)
operator||
operator|(
name|show_only
condition|?
name|ADD_CACHE_PRETEND
else|:
literal|0
operator|)
operator||
operator|(
name|intent_to_add
condition|?
name|ADD_CACHE_INTENT
else|:
literal|0
operator|)
operator||
operator|(
name|ignore_add_errors
condition|?
name|ADD_CACHE_IGNORE_ERRORS
else|:
literal|0
operator|)
operator||
operator|(
operator|!
operator|(
name|addremove
operator|||
name|take_worktree_changes
operator|)
condition|?
name|ADD_CACHE_IGNORE_REMOVAL
else|:
literal|0
operator|)
operator|)
expr_stmt|;
if|if
condition|(
name|require_pathspec
operator|&&
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
name|validate_pathspec
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
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
name|treat_gitlinks
argument_list|(
name|pathspec
argument_list|)
expr_stmt|;
if|if
condition|(
name|add_new_files
condition|)
block|{
name|int
name|baselen
decl_stmt|;
comment|/* Set up the default git porcelain excludes */
name|memset
argument_list|(
operator|&
name|dir
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
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
operator|.
name|flags
operator||=
name|DIR_COLLECT_IGNORED
expr_stmt|;
name|setup_standard_excludes
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
block|}
comment|/* This picks up the paths that are not tracked */
name|baselen
operator|=
name|fill_directory
argument_list|(
operator|&
name|dir
argument_list|,
name|pathspec
argument_list|)
expr_stmt|;
if|if
condition|(
name|pathspec
condition|)
name|seen
operator|=
name|prune_directory
argument_list|(
operator|&
name|dir
argument_list|,
name|pathspec
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
block|}
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
if|if
condition|(
name|pathspec
condition|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
operator|!
name|seen
condition|)
name|seen
operator|=
name|find_used_pathspec
argument_list|(
name|pathspec
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|pathspec
index|[
name|i
index|]
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
name|pathspec
index|[
name|i
index|]
index|[
literal|0
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
block|{
if|if
condition|(
name|ignore_missing
condition|)
block|{
if|if
condition|(
name|excluded
argument_list|(
operator|&
name|dir
argument_list|,
name|pathspec
index|[
name|i
index|]
argument_list|,
name|DT_UNKNOWN
argument_list|)
condition|)
name|dir_add_ignored
argument_list|(
operator|&
name|dir
argument_list|,
name|pathspec
index|[
name|i
index|]
argument_list|,
name|strlen
argument_list|(
name|pathspec
index|[
name|i
index|]
argument_list|)
argument_list|)
expr_stmt|;
block|}
else|else
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
block|}
name|free
argument_list|(
name|seen
argument_list|)
expr_stmt|;
block|}
name|exit_status
operator||=
name|add_files_to_cache
argument_list|(
name|prefix
argument_list|,
name|pathspec
argument_list|,
name|flags
argument_list|)
expr_stmt|;
if|if
condition|(
name|add_new_files
condition|)
name|exit_status
operator||=
name|add_files
argument_list|(
operator|&
name|dir
argument_list|,
name|flags
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
name|exit_status
return|;
block|}
end_function

end_unit

