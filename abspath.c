begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_comment
comment|/*  * Do not use this for inspecting *tracked* content.  When path is a  * symlink to a directory, we do not want to say it is a directory when  * dealing with tracked content in the working tree.  */
end_comment

begin_function
DECL|function|is_directory
name|int
name|is_directory
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
return|return
operator|(
operator|!
name|stat
argument_list|(
name|path
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
operator|)
return|;
block|}
end_function

begin_comment
comment|/* We allow "recursive" symbolic links. Only within reason, though. */
end_comment

begin_define
DECL|macro|MAXDEPTH
define|#
directive|define
name|MAXDEPTH
value|5
end_define

begin_comment
comment|/*  * Return the real path (i.e., absolute path, with symlinks resolved  * and extra slashes removed) equivalent to the specified path.  (If  * you want an absolute path but don't mind links, use  * absolute_path().)  The return value is a pointer to a static  * buffer.  *  * The input and all intermediate paths must be shorter than MAX_PATH.  * The directory part of path (i.e., everything up to the last  * dir_sep) must denote a valid, existing directory, but the last  * component need not exist.  If die_on_error is set, then die with an  * informative error message if there is a problem.  Otherwise, return  * NULL on errors (without generating any output).  *  * If path is our buffer, then return path, as it's already what the  * user wants.  */
end_comment

begin_function
DECL|function|real_path_internal
specifier|static
specifier|const
name|char
modifier|*
name|real_path_internal
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|die_on_error
parameter_list|)
block|{
specifier|static
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|char
modifier|*
name|retval
init|=
name|NULL
decl_stmt|;
comment|/* 	 * If we have to temporarily chdir(), store the original CWD 	 * here so that we can chdir() back to it at the end of the 	 * function: 	 */
name|struct
name|strbuf
name|cwd
init|=
name|STRBUF_INIT
decl_stmt|;
name|int
name|depth
init|=
name|MAXDEPTH
decl_stmt|;
name|char
modifier|*
name|last_elem
init|=
name|NULL
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
comment|/* We've already done it */
if|if
condition|(
name|path
operator|==
name|sb
operator|.
name|buf
condition|)
return|return
name|path
return|;
if|if
condition|(
operator|!
operator|*
name|path
condition|)
block|{
if|if
condition|(
name|die_on_error
condition|)
name|die
argument_list|(
literal|"The empty string is not a valid path"
argument_list|)
expr_stmt|;
else|else
goto|goto
name|error_out
goto|;
block|}
name|strbuf_reset
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|sb
argument_list|,
name|path
argument_list|)
expr_stmt|;
while|while
condition|(
name|depth
operator|--
condition|)
block|{
if|if
condition|(
operator|!
name|is_directory
argument_list|(
name|sb
operator|.
name|buf
argument_list|)
condition|)
block|{
name|char
modifier|*
name|last_slash
init|=
name|find_last_dir_sep
argument_list|(
name|sb
operator|.
name|buf
argument_list|)
decl_stmt|;
if|if
condition|(
name|last_slash
condition|)
block|{
name|last_elem
operator|=
name|xstrdup
argument_list|(
name|last_slash
operator|+
literal|1
argument_list|)
expr_stmt|;
name|strbuf_setlen
argument_list|(
operator|&
name|sb
argument_list|,
name|last_slash
operator|-
name|sb
operator|.
name|buf
operator|+
literal|1
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|last_elem
operator|=
name|xmemdupz
argument_list|(
name|sb
operator|.
name|buf
argument_list|,
name|sb
operator|.
name|len
argument_list|)
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|sb
operator|.
name|len
condition|)
block|{
if|if
condition|(
operator|!
name|cwd
operator|.
name|len
operator|&&
name|strbuf_getcwd
argument_list|(
operator|&
name|cwd
argument_list|)
condition|)
block|{
if|if
condition|(
name|die_on_error
condition|)
name|die_errno
argument_list|(
literal|"Could not get current working directory"
argument_list|)
expr_stmt|;
else|else
goto|goto
name|error_out
goto|;
block|}
if|if
condition|(
name|chdir
argument_list|(
name|sb
operator|.
name|buf
argument_list|)
condition|)
block|{
if|if
condition|(
name|die_on_error
condition|)
name|die_errno
argument_list|(
literal|"Could not switch to '%s'"
argument_list|,
name|sb
operator|.
name|buf
argument_list|)
expr_stmt|;
else|else
goto|goto
name|error_out
goto|;
block|}
block|}
if|if
condition|(
name|strbuf_getcwd
argument_list|(
operator|&
name|sb
argument_list|)
condition|)
block|{
if|if
condition|(
name|die_on_error
condition|)
name|die_errno
argument_list|(
literal|"Could not get current working directory"
argument_list|)
expr_stmt|;
else|else
goto|goto
name|error_out
goto|;
block|}
if|if
condition|(
name|last_elem
condition|)
block|{
if|if
condition|(
name|sb
operator|.
name|len
operator|&&
operator|!
name|is_dir_sep
argument_list|(
name|sb
operator|.
name|buf
index|[
name|sb
operator|.
name|len
operator|-
literal|1
index|]
argument_list|)
condition|)
name|strbuf_addch
argument_list|(
operator|&
name|sb
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|sb
argument_list|,
name|last_elem
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|last_elem
argument_list|)
expr_stmt|;
name|last_elem
operator|=
name|NULL
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|lstat
argument_list|(
name|sb
operator|.
name|buf
argument_list|,
operator|&
name|st
argument_list|)
operator|&&
name|S_ISLNK
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
name|struct
name|strbuf
name|next_sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|ssize_t
name|len
init|=
name|strbuf_readlink
argument_list|(
operator|&
name|next_sb
argument_list|,
name|sb
operator|.
name|buf
argument_list|,
literal|0
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|die_on_error
condition|)
name|die_errno
argument_list|(
literal|"Invalid symlink '%s'"
argument_list|,
name|sb
operator|.
name|buf
argument_list|)
expr_stmt|;
else|else
goto|goto
name|error_out
goto|;
block|}
name|strbuf_swap
argument_list|(
operator|&
name|sb
argument_list|,
operator|&
name|next_sb
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|next_sb
argument_list|)
expr_stmt|;
block|}
else|else
break|break;
block|}
name|retval
operator|=
name|sb
operator|.
name|buf
expr_stmt|;
name|error_out
label|:
name|free
argument_list|(
name|last_elem
argument_list|)
expr_stmt|;
if|if
condition|(
name|cwd
operator|.
name|len
operator|&&
name|chdir
argument_list|(
name|cwd
operator|.
name|buf
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"Could not change back to '%s'"
argument_list|,
name|cwd
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|cwd
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|real_path
specifier|const
name|char
modifier|*
name|real_path
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
return|return
name|real_path_internal
argument_list|(
name|path
argument_list|,
literal|1
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|real_path_if_valid
specifier|const
name|char
modifier|*
name|real_path_if_valid
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
return|return
name|real_path_internal
argument_list|(
name|path
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * Use this to get an absolute path from a relative one. If you want  * to resolve links, you should use real_path.  */
end_comment

begin_function
DECL|function|absolute_path
specifier|const
name|char
modifier|*
name|absolute_path
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
specifier|static
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
name|strbuf_add_absolute_path
argument_list|(
operator|&
name|sb
argument_list|,
name|path
argument_list|)
expr_stmt|;
return|return
name|sb
operator|.
name|buf
return|;
block|}
end_function

begin_comment
comment|/*  * Unlike prefix_path, this should be used if the named file does  * not have to interact with index entry; i.e. name of a random file  * on the filesystem.  */
end_comment

begin_function
DECL|function|prefix_filename
specifier|const
name|char
modifier|*
name|prefix_filename
parameter_list|(
specifier|const
name|char
modifier|*
name|pfx
parameter_list|,
name|int
name|pfx_len
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|)
block|{
specifier|static
name|struct
name|strbuf
name|path
init|=
name|STRBUF_INIT
decl_stmt|;
ifndef|#
directive|ifndef
name|GIT_WINDOWS_NATIVE
if|if
condition|(
operator|!
name|pfx_len
operator|||
name|is_absolute_path
argument_list|(
name|arg
argument_list|)
condition|)
return|return
name|arg
return|;
name|strbuf_reset
argument_list|(
operator|&
name|path
argument_list|)
expr_stmt|;
name|strbuf_add
argument_list|(
operator|&
name|path
argument_list|,
name|pfx
argument_list|,
name|pfx_len
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|path
argument_list|,
name|arg
argument_list|)
expr_stmt|;
else|#
directive|else
name|char
modifier|*
name|p
decl_stmt|;
comment|/* don't add prefix to absolute paths, but still replace '\' by '/' */
name|strbuf_reset
argument_list|(
operator|&
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|is_absolute_path
argument_list|(
name|arg
argument_list|)
condition|)
name|pfx_len
operator|=
literal|0
expr_stmt|;
elseif|else
if|if
condition|(
name|pfx_len
condition|)
name|strbuf_add
argument_list|(
operator|&
name|path
argument_list|,
name|pfx
argument_list|,
name|pfx_len
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|path
argument_list|,
name|arg
argument_list|)
expr_stmt|;
for|for
control|(
name|p
operator|=
name|path
operator|.
name|buf
operator|+
name|pfx_len
init|;
operator|*
name|p
condition|;
name|p
operator|++
control|)
if|if
condition|(
operator|*
name|p
operator|==
literal|'\\'
condition|)
operator|*
name|p
operator|=
literal|'/'
expr_stmt|;
endif|#
directive|endif
return|return
name|path
operator|.
name|buf
return|;
block|}
end_function

end_unit

