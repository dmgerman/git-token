begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * This handles recursive filename detection with exclude  * files, index knowledge etc..  *  * Copyright (C) Linus Torvalds, 2005-2006  *		 Junio Hamano, 2005-2006  */
end_comment

begin_include
include|#
directive|include
file|<dirent.h>
end_include

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
file|"dir.h"
end_include

begin_function
DECL|function|common_prefix
name|int
name|common_prefix
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|path
decl_stmt|,
modifier|*
name|slash
decl_stmt|,
modifier|*
name|next
decl_stmt|;
name|int
name|prefix
decl_stmt|;
if|if
condition|(
operator|!
name|pathspec
condition|)
return|return
literal|0
return|;
name|path
operator|=
operator|*
name|pathspec
expr_stmt|;
name|slash
operator|=
name|strrchr
argument_list|(
name|path
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|slash
condition|)
return|return
literal|0
return|;
name|prefix
operator|=
name|slash
operator|-
name|path
operator|+
literal|1
expr_stmt|;
while|while
condition|(
operator|(
name|next
operator|=
operator|*
operator|++
name|pathspec
operator|)
operator|!=
name|NULL
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|next
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
operator|>=
name|prefix
operator|&&
operator|!
name|memcmp
argument_list|(
name|path
argument_list|,
name|next
argument_list|,
name|len
argument_list|)
condition|)
continue|continue;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
operator|!
name|len
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|next
index|[
operator|--
name|len
index|]
operator|!=
literal|'/'
condition|)
continue|continue;
if|if
condition|(
name|memcmp
argument_list|(
name|path
argument_list|,
name|next
argument_list|,
name|len
operator|+
literal|1
argument_list|)
condition|)
continue|continue;
name|prefix
operator|=
name|len
operator|+
literal|1
expr_stmt|;
break|break;
block|}
block|}
return|return
name|prefix
return|;
block|}
end_function

begin_function
DECL|function|match_one
specifier|static
name|int
name|match_one
parameter_list|(
specifier|const
name|char
modifier|*
name|match
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|namelen
parameter_list|)
block|{
name|int
name|matchlen
decl_stmt|;
comment|/* If the match was just the prefix, we matched */
name|matchlen
operator|=
name|strlen
argument_list|(
name|match
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|matchlen
condition|)
return|return
literal|1
return|;
comment|/* 	 * If we don't match the matchstring exactly, 	 * we need to match by fnmatch 	 */
if|if
condition|(
name|strncmp
argument_list|(
name|match
argument_list|,
name|name
argument_list|,
name|matchlen
argument_list|)
condition|)
return|return
operator|!
name|fnmatch
argument_list|(
name|match
argument_list|,
name|name
argument_list|,
literal|0
argument_list|)
return|;
comment|/* 	 * If we did match the string exactly, we still 	 * need to make sure that it happened on a path 	 * component boundary (ie either the last character 	 * of the match was '/', or the next character of 	 * the name was '/' or the terminating NUL. 	 */
return|return
name|match
index|[
name|matchlen
operator|-
literal|1
index|]
operator|==
literal|'/'
operator|||
name|name
index|[
name|matchlen
index|]
operator|==
literal|'/'
operator|||
operator|!
name|name
index|[
name|matchlen
index|]
return|;
block|}
end_function

begin_function
DECL|function|match_pathspec
name|int
name|match_pathspec
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|namelen
parameter_list|,
name|int
name|prefix
parameter_list|,
name|char
modifier|*
name|seen
parameter_list|)
block|{
name|int
name|retval
decl_stmt|;
specifier|const
name|char
modifier|*
name|match
decl_stmt|;
name|name
operator|+=
name|prefix
expr_stmt|;
name|namelen
operator|-=
name|prefix
expr_stmt|;
for|for
control|(
name|retval
operator|=
literal|0
init|;
operator|(
name|match
operator|=
operator|*
name|pathspec
operator|++
operator|)
operator|!=
name|NULL
condition|;
name|seen
operator|++
control|)
block|{
if|if
condition|(
name|retval
operator|&
operator|*
name|seen
condition|)
continue|continue;
name|match
operator|+=
name|prefix
expr_stmt|;
if|if
condition|(
name|match_one
argument_list|(
name|match
argument_list|,
name|name
argument_list|,
name|namelen
argument_list|)
condition|)
block|{
name|retval
operator|=
literal|1
expr_stmt|;
operator|*
name|seen
operator|=
literal|1
expr_stmt|;
block|}
block|}
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|add_exclude
name|void
name|add_exclude
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
name|struct
name|exclude_list
modifier|*
name|which
parameter_list|)
block|{
name|struct
name|exclude
modifier|*
name|x
init|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|x
argument_list|)
argument_list|)
decl_stmt|;
name|x
operator|->
name|pattern
operator|=
name|string
expr_stmt|;
name|x
operator|->
name|base
operator|=
name|base
expr_stmt|;
name|x
operator|->
name|baselen
operator|=
name|baselen
expr_stmt|;
if|if
condition|(
name|which
operator|->
name|nr
operator|==
name|which
operator|->
name|alloc
condition|)
block|{
name|which
operator|->
name|alloc
operator|=
name|alloc_nr
argument_list|(
name|which
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|which
operator|->
name|excludes
operator|=
name|xrealloc
argument_list|(
name|which
operator|->
name|excludes
argument_list|,
name|which
operator|->
name|alloc
operator|*
sizeof|sizeof
argument_list|(
name|x
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|which
operator|->
name|excludes
index|[
name|which
operator|->
name|nr
operator|++
index|]
operator|=
name|x
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_excludes_from_file_1
specifier|static
name|int
name|add_excludes_from_file_1
parameter_list|(
specifier|const
name|char
modifier|*
name|fname
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
name|struct
name|exclude_list
modifier|*
name|which
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|int
name|fd
decl_stmt|,
name|i
decl_stmt|;
name|long
name|size
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|,
modifier|*
name|entry
decl_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|fname
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
operator|||
name|fstat
argument_list|(
name|fd
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
goto|goto
name|err
goto|;
name|size
operator|=
name|st
operator|.
name|st_size
expr_stmt|;
if|if
condition|(
name|size
operator|==
literal|0
condition|)
block|{
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|buf
operator|=
name|xmalloc
argument_list|(
name|size
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|read
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
operator|!=
name|size
condition|)
goto|goto
name|err
goto|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|buf
index|[
name|size
operator|++
index|]
operator|=
literal|'\n'
expr_stmt|;
name|entry
operator|=
name|buf
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|size
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|buf
index|[
name|i
index|]
operator|==
literal|'\n'
condition|)
block|{
if|if
condition|(
name|entry
operator|!=
name|buf
operator|+
name|i
operator|&&
name|entry
index|[
literal|0
index|]
operator|!=
literal|'#'
condition|)
block|{
name|buf
index|[
name|i
operator|-
operator|(
name|i
operator|&&
name|buf
index|[
name|i
operator|-
literal|1
index|]
operator|==
literal|'\r'
operator|)
index|]
operator|=
literal|0
expr_stmt|;
name|add_exclude
argument_list|(
name|entry
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|,
name|which
argument_list|)
expr_stmt|;
block|}
name|entry
operator|=
name|buf
operator|+
name|i
operator|+
literal|1
expr_stmt|;
block|}
block|}
return|return
literal|0
return|;
name|err
label|:
if|if
condition|(
literal|0
operator|<=
name|fd
condition|)
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|add_excludes_from_file
name|void
name|add_excludes_from_file
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|fname
parameter_list|)
block|{
if|if
condition|(
name|add_excludes_from_file_1
argument_list|(
name|fname
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|,
operator|&
name|dir
operator|->
name|exclude_list
index|[
name|EXC_FILE
index|]
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"cannot use %s as an exclude file"
argument_list|,
name|fname
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|push_exclude_per_directory
specifier|static
name|int
name|push_exclude_per_directory
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|)
block|{
name|char
name|exclude_file
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|struct
name|exclude_list
modifier|*
name|el
init|=
operator|&
name|dir
operator|->
name|exclude_list
index|[
name|EXC_DIRS
index|]
decl_stmt|;
name|int
name|current_nr
init|=
name|el
operator|->
name|nr
decl_stmt|;
if|if
condition|(
name|dir
operator|->
name|exclude_per_dir
condition|)
block|{
name|memcpy
argument_list|(
name|exclude_file
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|exclude_file
operator|+
name|baselen
argument_list|,
name|dir
operator|->
name|exclude_per_dir
argument_list|)
expr_stmt|;
name|add_excludes_from_file_1
argument_list|(
name|exclude_file
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|,
name|el
argument_list|)
expr_stmt|;
block|}
return|return
name|current_nr
return|;
block|}
end_function

begin_function
DECL|function|pop_exclude_per_directory
specifier|static
name|void
name|pop_exclude_per_directory
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
name|int
name|stk
parameter_list|)
block|{
name|struct
name|exclude_list
modifier|*
name|el
init|=
operator|&
name|dir
operator|->
name|exclude_list
index|[
name|EXC_DIRS
index|]
decl_stmt|;
while|while
condition|(
name|stk
operator|<
name|el
operator|->
name|nr
condition|)
name|free
argument_list|(
name|el
operator|->
name|excludes
index|[
operator|--
name|el
operator|->
name|nr
index|]
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Scan the list and let the last match determines the fate.  * Return 1 for exclude, 0 for include and -1 for undecided.  */
end_comment

begin_function
DECL|function|excluded_1
specifier|static
name|int
name|excluded_1
parameter_list|(
specifier|const
name|char
modifier|*
name|pathname
parameter_list|,
name|int
name|pathlen
parameter_list|,
name|struct
name|exclude_list
modifier|*
name|el
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
name|el
operator|->
name|nr
condition|)
block|{
for|for
control|(
name|i
operator|=
name|el
operator|->
name|nr
operator|-
literal|1
init|;
literal|0
operator|<=
name|i
condition|;
name|i
operator|--
control|)
block|{
name|struct
name|exclude
modifier|*
name|x
init|=
name|el
operator|->
name|excludes
index|[
name|i
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|exclude
init|=
name|x
operator|->
name|pattern
decl_stmt|;
name|int
name|to_exclude
init|=
literal|1
decl_stmt|;
if|if
condition|(
operator|*
name|exclude
operator|==
literal|'!'
condition|)
block|{
name|to_exclude
operator|=
literal|0
expr_stmt|;
name|exclude
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|strchr
argument_list|(
name|exclude
argument_list|,
literal|'/'
argument_list|)
condition|)
block|{
comment|/* match basename */
specifier|const
name|char
modifier|*
name|basename
init|=
name|strrchr
argument_list|(
name|pathname
argument_list|,
literal|'/'
argument_list|)
decl_stmt|;
name|basename
operator|=
operator|(
name|basename
operator|)
condition|?
name|basename
operator|+
literal|1
else|:
name|pathname
expr_stmt|;
if|if
condition|(
name|fnmatch
argument_list|(
name|exclude
argument_list|,
name|basename
argument_list|,
literal|0
argument_list|)
operator|==
literal|0
condition|)
return|return
name|to_exclude
return|;
block|}
else|else
block|{
comment|/* match with FNM_PATHNAME: 				 * exclude has base (baselen long) implicitly 				 * in front of it. 				 */
name|int
name|baselen
init|=
name|x
operator|->
name|baselen
decl_stmt|;
if|if
condition|(
operator|*
name|exclude
operator|==
literal|'/'
condition|)
name|exclude
operator|++
expr_stmt|;
if|if
condition|(
name|pathlen
operator|<
name|baselen
operator|||
operator|(
name|baselen
operator|&&
name|pathname
index|[
name|baselen
operator|-
literal|1
index|]
operator|!=
literal|'/'
operator|)
operator|||
name|strncmp
argument_list|(
name|pathname
argument_list|,
name|x
operator|->
name|base
argument_list|,
name|baselen
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|fnmatch
argument_list|(
name|exclude
argument_list|,
name|pathname
operator|+
name|baselen
argument_list|,
name|FNM_PATHNAME
argument_list|)
operator|==
literal|0
condition|)
return|return
name|to_exclude
return|;
block|}
block|}
block|}
return|return
operator|-
literal|1
return|;
comment|/* undecided */
block|}
end_function

begin_function
DECL|function|excluded
name|int
name|excluded
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|pathname
parameter_list|)
block|{
name|int
name|pathlen
init|=
name|strlen
argument_list|(
name|pathname
argument_list|)
decl_stmt|;
name|int
name|st
decl_stmt|;
for|for
control|(
name|st
operator|=
name|EXC_CMDL
init|;
name|st
operator|<=
name|EXC_FILE
condition|;
name|st
operator|++
control|)
block|{
switch|switch
condition|(
name|excluded_1
argument_list|(
name|pathname
argument_list|,
name|pathlen
argument_list|,
operator|&
name|dir
operator|->
name|exclude_list
index|[
name|st
index|]
argument_list|)
condition|)
block|{
case|case
literal|0
case|:
return|return
literal|0
return|;
case|case
literal|1
case|:
return|return
literal|1
return|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|add_name
specifier|static
name|void
name|add_name
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|pathname
parameter_list|,
name|int
name|len
parameter_list|)
block|{
name|struct
name|dir_entry
modifier|*
name|ent
decl_stmt|;
if|if
condition|(
name|cache_name_pos
argument_list|(
name|pathname
argument_list|,
name|len
argument_list|)
operator|>=
literal|0
condition|)
return|return;
if|if
condition|(
name|dir
operator|->
name|nr
operator|==
name|dir
operator|->
name|alloc
condition|)
block|{
name|int
name|alloc
init|=
name|alloc_nr
argument_list|(
name|dir
operator|->
name|alloc
argument_list|)
decl_stmt|;
name|dir
operator|->
name|alloc
operator|=
name|alloc
expr_stmt|;
name|dir
operator|->
name|entries
operator|=
name|xrealloc
argument_list|(
name|dir
operator|->
name|entries
argument_list|,
name|alloc
operator|*
sizeof|sizeof
argument_list|(
name|ent
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|ent
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|ent
argument_list|)
operator|+
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|ent
operator|->
name|len
operator|=
name|len
expr_stmt|;
name|memcpy
argument_list|(
name|ent
operator|->
name|name
argument_list|,
name|pathname
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|ent
operator|->
name|name
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|dir
operator|->
name|entries
index|[
name|dir
operator|->
name|nr
operator|++
index|]
operator|=
name|ent
expr_stmt|;
block|}
end_function

begin_function
DECL|function|dir_exists
specifier|static
name|int
name|dir_exists
parameter_list|(
specifier|const
name|char
modifier|*
name|dirname
parameter_list|,
name|int
name|len
parameter_list|)
block|{
name|int
name|pos
init|=
name|cache_name_pos
argument_list|(
name|dirname
argument_list|,
name|len
argument_list|)
decl_stmt|;
if|if
condition|(
name|pos
operator|>=
literal|0
condition|)
return|return
literal|1
return|;
name|pos
operator|=
operator|-
name|pos
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|pos
operator|>=
name|active_nr
condition|)
comment|/* can't */
return|return
literal|0
return|;
return|return
operator|!
name|strncmp
argument_list|(
name|active_cache
index|[
name|pos
index|]
operator|->
name|name
argument_list|,
name|dirname
argument_list|,
name|len
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * Read a directory tree. We currently ignore anything but  * directories, regular files and symlinks. That's because git  * doesn't handle them at all yet. Maybe that will change some  * day.  *  * Also, we ignore the name ".git" (even if it is not a directory).  * That likely will not change.  */
end_comment

begin_function
DECL|function|read_directory_recursive
specifier|static
name|int
name|read_directory_recursive
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
name|int
name|check_only
parameter_list|)
block|{
name|DIR
modifier|*
name|fdir
init|=
name|opendir
argument_list|(
name|path
argument_list|)
decl_stmt|;
name|int
name|contents
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|fdir
condition|)
block|{
name|int
name|exclude_stk
decl_stmt|;
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
name|char
name|fullname
index|[
name|PATH_MAX
operator|+
literal|1
index|]
decl_stmt|;
name|memcpy
argument_list|(
name|fullname
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
name|exclude_stk
operator|=
name|push_exclude_per_directory
argument_list|(
name|dir
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|de
operator|=
name|readdir
argument_list|(
name|fdir
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|int
name|len
decl_stmt|;
if|if
condition|(
operator|(
name|de
operator|->
name|d_name
index|[
literal|0
index|]
operator|==
literal|'.'
operator|)
operator|&&
operator|(
name|de
operator|->
name|d_name
index|[
literal|1
index|]
operator|==
literal|0
operator|||
operator|!
name|strcmp
argument_list|(
name|de
operator|->
name|d_name
operator|+
literal|1
argument_list|,
literal|"."
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|de
operator|->
name|d_name
operator|+
literal|1
argument_list|,
literal|"git"
argument_list|)
operator|)
condition|)
continue|continue;
name|len
operator|=
name|strlen
argument_list|(
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|fullname
operator|+
name|baselen
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|excluded
argument_list|(
name|dir
argument_list|,
name|fullname
argument_list|)
operator|!=
name|dir
operator|->
name|show_ignored
condition|)
block|{
if|if
condition|(
operator|!
name|dir
operator|->
name|show_ignored
operator|||
name|DTYPE
argument_list|(
name|de
argument_list|)
operator|!=
name|DT_DIR
condition|)
block|{
continue|continue;
block|}
block|}
switch|switch
condition|(
name|DTYPE
argument_list|(
name|de
argument_list|)
condition|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
default|default:
continue|continue;
case|case
name|DT_UNKNOWN
case|:
if|if
condition|(
name|lstat
argument_list|(
name|fullname
argument_list|,
operator|&
name|st
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|S_ISREG
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
operator|||
name|S_ISLNK
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
break|break;
if|if
condition|(
operator|!
name|S_ISDIR
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
continue|continue;
comment|/* fallthrough */
case|case
name|DT_DIR
case|:
name|memcpy
argument_list|(
name|fullname
operator|+
name|baselen
operator|+
name|len
argument_list|,
literal|"/"
argument_list|,
literal|2
argument_list|)
expr_stmt|;
name|len
operator|++
expr_stmt|;
if|if
condition|(
name|dir
operator|->
name|show_other_directories
operator|&&
operator|!
name|dir_exists
argument_list|(
name|fullname
argument_list|,
name|baselen
operator|+
name|len
argument_list|)
condition|)
block|{
if|if
condition|(
name|dir
operator|->
name|hide_empty_directories
operator|&&
operator|!
name|read_directory_recursive
argument_list|(
name|dir
argument_list|,
name|fullname
argument_list|,
name|fullname
argument_list|,
name|baselen
operator|+
name|len
argument_list|,
literal|1
argument_list|)
condition|)
continue|continue;
break|break;
block|}
name|contents
operator|+=
name|read_directory_recursive
argument_list|(
name|dir
argument_list|,
name|fullname
argument_list|,
name|fullname
argument_list|,
name|baselen
operator|+
name|len
argument_list|,
literal|0
argument_list|)
expr_stmt|;
continue|continue;
case|case
name|DT_REG
case|:
case|case
name|DT_LNK
case|:
break|break;
block|}
name|contents
operator|++
expr_stmt|;
if|if
condition|(
name|check_only
condition|)
goto|goto
name|exit_early
goto|;
else|else
name|add_name
argument_list|(
name|dir
argument_list|,
name|fullname
argument_list|,
name|baselen
operator|+
name|len
argument_list|)
expr_stmt|;
block|}
name|exit_early
label|:
name|closedir
argument_list|(
name|fdir
argument_list|)
expr_stmt|;
name|pop_exclude_per_directory
argument_list|(
name|dir
argument_list|,
name|exclude_stk
argument_list|)
expr_stmt|;
block|}
return|return
name|contents
return|;
block|}
end_function

begin_function
DECL|function|cmp_name
specifier|static
name|int
name|cmp_name
parameter_list|(
specifier|const
name|void
modifier|*
name|p1
parameter_list|,
specifier|const
name|void
modifier|*
name|p2
parameter_list|)
block|{
specifier|const
name|struct
name|dir_entry
modifier|*
name|e1
init|=
operator|*
operator|(
specifier|const
expr|struct
name|dir_entry
operator|*
operator|*
operator|)
name|p1
decl_stmt|;
specifier|const
name|struct
name|dir_entry
modifier|*
name|e2
init|=
operator|*
operator|(
specifier|const
expr|struct
name|dir_entry
operator|*
operator|*
operator|)
name|p2
decl_stmt|;
return|return
name|cache_name_compare
argument_list|(
name|e1
operator|->
name|name
argument_list|,
name|e1
operator|->
name|len
argument_list|,
name|e2
operator|->
name|name
argument_list|,
name|e2
operator|->
name|len
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|read_directory
name|int
name|read_directory
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|)
block|{
comment|/* 	 * Make sure to do the per-directory exclude for all the 	 * directories leading up to our base. 	 */
if|if
condition|(
name|baselen
condition|)
block|{
if|if
condition|(
name|dir
operator|->
name|exclude_per_dir
condition|)
block|{
name|char
modifier|*
name|p
decl_stmt|,
modifier|*
name|pp
init|=
name|xmalloc
argument_list|(
name|baselen
operator|+
literal|1
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|pp
argument_list|,
name|base
argument_list|,
name|baselen
operator|+
literal|1
argument_list|)
expr_stmt|;
name|p
operator|=
name|pp
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|char
name|save
init|=
operator|*
name|p
decl_stmt|;
operator|*
name|p
operator|=
literal|0
expr_stmt|;
name|push_exclude_per_directory
argument_list|(
name|dir
argument_list|,
name|pp
argument_list|,
name|p
operator|-
name|pp
argument_list|)
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|save
expr_stmt|;
if|if
condition|(
operator|!
name|save
condition|)
break|break;
name|p
operator|=
name|strchr
argument_list|(
name|p
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
condition|)
name|p
operator|++
expr_stmt|;
else|else
name|p
operator|=
name|pp
operator|+
name|baselen
expr_stmt|;
block|}
name|free
argument_list|(
name|pp
argument_list|)
expr_stmt|;
block|}
block|}
name|read_directory_recursive
argument_list|(
name|dir
argument_list|,
name|path
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|qsort
argument_list|(
name|dir
operator|->
name|entries
argument_list|,
name|dir
operator|->
name|nr
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|dir_entry
operator|*
argument_list|)
argument_list|,
name|cmp_name
argument_list|)
expr_stmt|;
return|return
name|dir
operator|->
name|nr
return|;
block|}
end_function

begin_function
name|int
DECL|function|file_exists
name|file_exists
parameter_list|(
specifier|const
name|char
modifier|*
name|f
parameter_list|)
block|{
name|struct
name|stat
name|sb
decl_stmt|;
return|return
name|stat
argument_list|(
name|f
argument_list|,
operator|&
name|sb
argument_list|)
operator|==
literal|0
return|;
block|}
end_function

end_unit

