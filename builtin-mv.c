begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * "git mv" builtin command  *  * Copyright (C) 2006 Johannes Schindelin  */
end_comment

begin_include
include|#
directive|include
file|<fnmatch.h>
end_include

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
file|"cache-tree.h"
end_include

begin_include
include|#
directive|include
file|"path-list.h"
end_include

begin_decl_stmt
DECL|variable|builtin_mv_usage
specifier|static
specifier|const
name|char
name|builtin_mv_usage
index|[]
init|=
literal|"git-mv [-n] [-f] (<source><destination> | [-k]<source>...<destination>)"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|copy_pathspec
specifier|static
specifier|const
name|char
modifier|*
modifier|*
name|copy_pathspec
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
name|count
parameter_list|,
name|int
name|base_name
parameter_list|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|result
init|=
name|xmalloc
argument_list|(
operator|(
name|count
operator|+
literal|1
operator|)
operator|*
sizeof|sizeof
argument_list|(
specifier|const
name|char
operator|*
argument_list|)
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|result
argument_list|,
name|pathspec
argument_list|,
name|count
operator|*
sizeof|sizeof
argument_list|(
specifier|const
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|result
index|[
name|count
index|]
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|base_name
condition|)
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
name|count
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|last_slash
init|=
name|strrchr
argument_list|(
name|result
index|[
name|i
index|]
argument_list|,
literal|'/'
argument_list|)
decl_stmt|;
if|if
condition|(
name|last_slash
condition|)
name|result
index|[
name|i
index|]
operator|=
name|last_slash
operator|+
literal|1
expr_stmt|;
block|}
block|}
return|return
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|result
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|show_list
specifier|static
name|void
name|show_list
parameter_list|(
specifier|const
name|char
modifier|*
name|label
parameter_list|,
name|struct
name|path_list
modifier|*
name|list
parameter_list|)
block|{
if|if
condition|(
name|list
operator|->
name|nr
operator|>
literal|0
condition|)
block|{
name|int
name|i
decl_stmt|;
name|printf
argument_list|(
literal|"%s"
argument_list|,
name|label
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
condition|;
name|i
operator|++
control|)
name|printf
argument_list|(
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
name|path
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|add_slash
specifier|static
specifier|const
name|char
modifier|*
name|add_slash
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|path
argument_list|)
decl_stmt|;
if|if
condition|(
name|path
index|[
name|len
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
block|{
name|char
modifier|*
name|with_slash
init|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|2
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|with_slash
argument_list|,
name|path
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|with_slash
index|[
name|len
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
name|with_slash
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
return|return
name|with_slash
return|;
block|}
return|return
name|path
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

begin_function
DECL|function|cmd_mv
name|int
name|cmd_mv
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
name|count
decl_stmt|;
name|int
name|verbose
init|=
literal|0
decl_stmt|,
name|show_only
init|=
literal|0
decl_stmt|,
name|force
init|=
literal|0
decl_stmt|,
name|ignore_errors
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|source
decl_stmt|,
modifier|*
modifier|*
name|destination
decl_stmt|,
modifier|*
modifier|*
name|dest_path
decl_stmt|;
enum|enum
name|update_mode
block|{
name|BOTH
init|=
literal|0
block|,
name|WORKING_DIRECTORY
block|,
name|INDEX
block|}
modifier|*
name|modes
enum|;
name|struct
name|stat
name|st
decl_stmt|;
name|struct
name|path_list
name|overwritten
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|;
name|struct
name|path_list
name|src_for_dst
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|;
name|struct
name|path_list
name|added
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|;
name|struct
name|path_list
name|deleted
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|;
name|struct
name|path_list
name|changed
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
name|newfd
operator|=
name|hold_lock_file_for_update
argument_list|(
operator|&
name|lock_file
argument_list|,
name|get_index_file
argument_list|()
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
literal|"--"
argument_list|)
condition|)
block|{
name|i
operator|++
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-n"
argument_list|)
condition|)
block|{
name|show_only
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
literal|"-f"
argument_list|)
condition|)
block|{
name|force
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
literal|"-k"
argument_list|)
condition|)
block|{
name|ignore_errors
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|builtin_mv_usage
argument_list|)
expr_stmt|;
block|}
name|count
operator|=
name|argc
operator|-
name|i
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|count
operator|<
literal|1
condition|)
name|usage
argument_list|(
name|builtin_mv_usage
argument_list|)
expr_stmt|;
name|source
operator|=
name|copy_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
operator|+
name|i
argument_list|,
name|count
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|modes
operator|=
name|xcalloc
argument_list|(
name|count
argument_list|,
sizeof|sizeof
argument_list|(
expr|enum
name|update_mode
argument_list|)
argument_list|)
expr_stmt|;
name|dest_path
operator|=
name|copy_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
operator|+
name|argc
operator|-
literal|1
argument_list|,
literal|1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lstat
argument_list|(
name|dest_path
index|[
literal|0
index|]
argument_list|,
operator|&
name|st
argument_list|)
operator|&&
name|S_ISDIR
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
name|dest_path
index|[
literal|0
index|]
operator|=
name|add_slash
argument_list|(
name|dest_path
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|destination
operator|=
name|copy_pathspec
argument_list|(
name|dest_path
index|[
literal|0
index|]
argument_list|,
name|argv
operator|+
name|i
argument_list|,
name|count
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|count
operator|!=
literal|1
condition|)
name|usage
argument_list|(
name|builtin_mv_usage
argument_list|)
expr_stmt|;
name|destination
operator|=
name|dest_path
expr_stmt|;
block|}
comment|/* Checking */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|count
condition|;
name|i
operator|++
control|)
block|{
name|int
name|length
decl_stmt|;
specifier|const
name|char
modifier|*
name|bad
init|=
name|NULL
decl_stmt|;
if|if
condition|(
name|show_only
condition|)
name|printf
argument_list|(
literal|"Checking rename of '%s' to '%s'\n"
argument_list|,
name|source
index|[
name|i
index|]
argument_list|,
name|destination
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|source
index|[
name|i
index|]
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
name|bad
operator|=
literal|"bad source"
expr_stmt|;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
specifier|const
name|char
modifier|*
name|dir
init|=
name|source
index|[
name|i
index|]
decl_stmt|,
modifier|*
name|dest_dir
init|=
name|destination
index|[
name|i
index|]
decl_stmt|;
name|int
name|first
decl_stmt|,
name|last
decl_stmt|,
name|len
init|=
name|strlen
argument_list|(
name|dir
argument_list|)
decl_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|dest_dir
argument_list|,
operator|&
name|st
argument_list|)
operator|==
literal|0
condition|)
block|{
name|bad
operator|=
literal|"cannot move directory over file"
expr_stmt|;
goto|goto
name|next
goto|;
block|}
name|modes
index|[
name|i
index|]
operator|=
name|WORKING_DIRECTORY
expr_stmt|;
name|first
operator|=
name|cache_name_pos
argument_list|(
name|source
index|[
name|i
index|]
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|first
operator|>=
literal|0
condition|)
name|die
argument_list|(
literal|"Huh? %s/ is in index?"
argument_list|,
name|dir
argument_list|)
expr_stmt|;
name|first
operator|=
operator|-
literal|1
operator|-
name|first
expr_stmt|;
for|for
control|(
name|last
operator|=
name|first
init|;
name|last
operator|<
name|active_nr
condition|;
name|last
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|active_cache
index|[
name|last
index|]
operator|->
name|name
decl_stmt|;
if|if
condition|(
name|strncmp
argument_list|(
name|path
argument_list|,
name|dir
argument_list|,
name|len
argument_list|)
operator|||
name|path
index|[
name|len
index|]
operator|!=
literal|'/'
condition|)
break|break;
block|}
if|if
condition|(
name|last
operator|-
name|first
operator|<
literal|1
condition|)
name|bad
operator|=
literal|"source directory is empty"
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|bad
condition|)
block|{
name|int
name|j
decl_stmt|,
name|dst_len
init|=
name|strlen
argument_list|(
name|dest_dir
argument_list|)
decl_stmt|;
if|if
condition|(
name|last
operator|-
name|first
operator|>
literal|0
condition|)
block|{
name|source
operator|=
name|realloc
argument_list|(
name|source
argument_list|,
operator|(
name|count
operator|+
name|last
operator|-
name|first
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|destination
operator|=
name|realloc
argument_list|(
name|destination
argument_list|,
operator|(
name|count
operator|+
name|last
operator|-
name|first
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|modes
operator|=
name|realloc
argument_list|(
name|modes
argument_list|,
operator|(
name|count
operator|+
name|last
operator|-
name|first
operator|)
operator|*
sizeof|sizeof
argument_list|(
expr|enum
name|update_mode
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|dest_dir
operator|=
name|add_slash
argument_list|(
name|dest_dir
argument_list|)
expr_stmt|;
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|last
operator|-
name|first
condition|;
name|j
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|active_cache
index|[
name|first
operator|+
name|j
index|]
operator|->
name|name
decl_stmt|;
name|source
index|[
name|count
operator|+
name|j
index|]
operator|=
name|path
expr_stmt|;
name|destination
index|[
name|count
operator|+
name|j
index|]
operator|=
name|prefix_path
argument_list|(
name|dest_dir
argument_list|,
name|dst_len
argument_list|,
name|path
operator|+
name|len
argument_list|)
expr_stmt|;
name|modes
index|[
name|count
operator|+
name|j
index|]
operator|=
name|INDEX
expr_stmt|;
block|}
name|count
operator|+=
name|last
operator|-
name|first
expr_stmt|;
block|}
goto|goto
name|next
goto|;
block|}
if|if
condition|(
operator|!
name|bad
operator|&&
name|lstat
argument_list|(
name|destination
index|[
name|i
index|]
argument_list|,
operator|&
name|st
argument_list|)
operator|==
literal|0
condition|)
block|{
name|bad
operator|=
literal|"destination exists"
expr_stmt|;
if|if
condition|(
name|force
condition|)
block|{
comment|/* 				 * only files can overwrite each other: 				 * check both source and destination 				 */
if|if
condition|(
name|S_ISREG
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Warning: %s;"
literal|" will overwrite!\n"
argument_list|,
name|bad
argument_list|)
expr_stmt|;
name|bad
operator|=
name|NULL
expr_stmt|;
name|path_list_insert
argument_list|(
name|destination
index|[
name|i
index|]
argument_list|,
operator|&
name|overwritten
argument_list|)
expr_stmt|;
block|}
else|else
name|bad
operator|=
literal|"Cannot overwrite"
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|bad
operator|&&
operator|(
name|length
operator|=
name|strlen
argument_list|(
name|source
index|[
name|i
index|]
argument_list|)
operator|)
operator|>=
literal|0
operator|&&
operator|!
name|strncmp
argument_list|(
name|destination
index|[
name|i
index|]
argument_list|,
name|source
index|[
name|i
index|]
argument_list|,
name|length
argument_list|)
operator|&&
operator|(
name|destination
index|[
name|i
index|]
index|[
name|length
index|]
operator|==
literal|0
operator|||
name|destination
index|[
name|i
index|]
index|[
name|length
index|]
operator|==
literal|'/'
operator|)
condition|)
name|bad
operator|=
literal|"can not move directory into itself"
expr_stmt|;
if|if
condition|(
operator|!
name|bad
operator|&&
name|cache_name_pos
argument_list|(
name|source
index|[
name|i
index|]
argument_list|,
name|strlen
argument_list|(
name|source
index|[
name|i
index|]
argument_list|)
argument_list|)
operator|<
literal|0
condition|)
name|bad
operator|=
literal|"not under version control"
expr_stmt|;
if|if
condition|(
operator|!
name|bad
condition|)
block|{
if|if
condition|(
name|path_list_has_path
argument_list|(
operator|&
name|src_for_dst
argument_list|,
name|destination
index|[
name|i
index|]
argument_list|)
condition|)
name|bad
operator|=
literal|"multiple sources for the same target"
expr_stmt|;
else|else
name|path_list_insert
argument_list|(
name|destination
index|[
name|i
index|]
argument_list|,
operator|&
name|src_for_dst
argument_list|)
expr_stmt|;
block|}
name|next
label|:
if|if
condition|(
name|bad
condition|)
block|{
if|if
condition|(
name|ignore_errors
condition|)
block|{
if|if
condition|(
operator|--
name|count
operator|>
literal|0
condition|)
block|{
name|memmove
argument_list|(
name|source
operator|+
name|i
argument_list|,
name|source
operator|+
name|i
operator|+
literal|1
argument_list|,
operator|(
name|count
operator|-
name|i
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|memmove
argument_list|(
name|destination
operator|+
name|i
argument_list|,
name|destination
operator|+
name|i
operator|+
literal|1
argument_list|,
operator|(
name|count
operator|-
name|i
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
else|else
name|die
argument_list|(
literal|"%s, source=%s, destination=%s"
argument_list|,
name|bad
argument_list|,
name|source
index|[
name|i
index|]
argument_list|,
name|destination
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|count
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|show_only
operator|||
name|verbose
condition|)
name|printf
argument_list|(
literal|"Renaming %s to %s\n"
argument_list|,
name|source
index|[
name|i
index|]
argument_list|,
name|destination
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|show_only
operator|&&
name|modes
index|[
name|i
index|]
operator|!=
name|INDEX
operator|&&
name|rename
argument_list|(
name|source
index|[
name|i
index|]
argument_list|,
name|destination
index|[
name|i
index|]
argument_list|)
operator|<
literal|0
operator|&&
operator|!
name|ignore_errors
condition|)
name|die
argument_list|(
literal|"renaming %s failed: %s"
argument_list|,
name|source
index|[
name|i
index|]
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|modes
index|[
name|i
index|]
operator|==
name|WORKING_DIRECTORY
condition|)
continue|continue;
if|if
condition|(
name|cache_name_pos
argument_list|(
name|source
index|[
name|i
index|]
argument_list|,
name|strlen
argument_list|(
name|source
index|[
name|i
index|]
argument_list|)
argument_list|)
operator|>=
literal|0
condition|)
block|{
name|path_list_insert
argument_list|(
name|source
index|[
name|i
index|]
argument_list|,
operator|&
name|deleted
argument_list|)
expr_stmt|;
comment|/* destination can be a directory with 1 file inside */
if|if
condition|(
name|path_list_has_path
argument_list|(
operator|&
name|overwritten
argument_list|,
name|destination
index|[
name|i
index|]
argument_list|)
condition|)
name|path_list_insert
argument_list|(
name|destination
index|[
name|i
index|]
argument_list|,
operator|&
name|changed
argument_list|)
expr_stmt|;
else|else
name|path_list_insert
argument_list|(
name|destination
index|[
name|i
index|]
argument_list|,
operator|&
name|added
argument_list|)
expr_stmt|;
block|}
else|else
name|path_list_insert
argument_list|(
name|destination
index|[
name|i
index|]
argument_list|,
operator|&
name|added
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|show_only
condition|)
block|{
name|show_list
argument_list|(
literal|"Changed  : "
argument_list|,
operator|&
name|changed
argument_list|)
expr_stmt|;
name|show_list
argument_list|(
literal|"Adding   : "
argument_list|,
operator|&
name|added
argument_list|)
expr_stmt|;
name|show_list
argument_list|(
literal|"Deleting : "
argument_list|,
operator|&
name|deleted
argument_list|)
expr_stmt|;
block|}
else|else
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|changed
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|changed
operator|.
name|items
index|[
name|i
index|]
operator|.
name|path
decl_stmt|;
name|int
name|i
init|=
name|cache_name_pos
argument_list|(
name|path
argument_list|,
name|strlen
argument_list|(
name|path
argument_list|)
argument_list|)
decl_stmt|;
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
name|i
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Huh? Cache entry for %s unknown?"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|refresh_cache_entry
argument_list|(
name|ce
argument_list|,
literal|0
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
name|added
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|added
operator|.
name|items
index|[
name|i
index|]
operator|.
name|path
decl_stmt|;
name|add_file_to_index
argument_list|(
name|path
argument_list|,
name|verbose
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
name|deleted
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|deleted
operator|.
name|items
index|[
name|i
index|]
operator|.
name|path
decl_stmt|;
name|remove_file_from_cache
argument_list|(
name|path
argument_list|)
expr_stmt|;
block|}
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
name|commit_lock_file
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
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

