begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_function
DECL|function|prefix_path
specifier|const
name|char
modifier|*
name|prefix_path
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|int
name|len
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|orig
init|=
name|path
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|char
name|c
decl_stmt|;
if|if
condition|(
operator|*
name|path
operator|!=
literal|'.'
condition|)
break|break;
name|c
operator|=
name|path
index|[
literal|1
index|]
expr_stmt|;
comment|/* "." */
if|if
condition|(
operator|!
name|c
condition|)
block|{
name|path
operator|++
expr_stmt|;
break|break;
block|}
comment|/* "./" */
if|if
condition|(
name|c
operator|==
literal|'/'
condition|)
block|{
name|path
operator|+=
literal|2
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|c
operator|!=
literal|'.'
condition|)
break|break;
name|c
operator|=
name|path
index|[
literal|2
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|c
condition|)
name|path
operator|+=
literal|2
expr_stmt|;
elseif|else
if|if
condition|(
name|c
operator|==
literal|'/'
condition|)
name|path
operator|+=
literal|3
expr_stmt|;
else|else
break|break;
comment|/* ".." and "../" */
comment|/* Remove last component of the prefix */
do|do
block|{
if|if
condition|(
operator|!
name|len
condition|)
name|die
argument_list|(
literal|"'%s' is outside repository"
argument_list|,
name|orig
argument_list|)
expr_stmt|;
name|len
operator|--
expr_stmt|;
block|}
do|while
condition|(
name|len
operator|&&
name|prefix
index|[
name|len
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
do|;
continue|continue;
block|}
if|if
condition|(
name|len
condition|)
block|{
name|int
name|speclen
init|=
name|strlen
argument_list|(
name|path
argument_list|)
decl_stmt|;
name|char
modifier|*
name|n
init|=
name|xmalloc
argument_list|(
name|speclen
operator|+
name|len
operator|+
literal|1
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|n
argument_list|,
name|prefix
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|n
operator|+
name|len
argument_list|,
name|path
argument_list|,
name|speclen
operator|+
literal|1
argument_list|)
expr_stmt|;
name|path
operator|=
name|n
expr_stmt|;
block|}
return|return
name|path
return|;
block|}
end_function

begin_comment
comment|/*   * Unlike prefix_path, this should be used if the named file does  * not have to interact with index entry; i.e. name of a random file  * on the filesystem.  */
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
name|char
name|path
index|[
name|PATH_MAX
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|pfx
operator|||
operator|!
operator|*
name|pfx
operator|||
name|arg
index|[
literal|0
index|]
operator|==
literal|'/'
condition|)
return|return
name|arg
return|;
name|memcpy
argument_list|(
name|path
argument_list|,
name|pfx
argument_list|,
name|pfx_len
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|path
operator|+
name|pfx_len
argument_list|,
name|arg
argument_list|)
expr_stmt|;
return|return
name|path
return|;
block|}
end_function

begin_function
DECL|function|get_pathspec
specifier|const
name|char
modifier|*
modifier|*
name|get_pathspec
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
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|entry
init|=
operator|*
name|pathspec
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|p
decl_stmt|;
name|int
name|prefixlen
decl_stmt|;
if|if
condition|(
operator|!
name|prefix
operator|&&
operator|!
name|entry
condition|)
return|return
name|NULL
return|;
if|if
condition|(
operator|!
name|entry
condition|)
block|{
specifier|static
specifier|const
name|char
modifier|*
name|spec
index|[
literal|2
index|]
decl_stmt|;
name|spec
index|[
literal|0
index|]
operator|=
name|prefix
expr_stmt|;
name|spec
index|[
literal|1
index|]
operator|=
name|NULL
expr_stmt|;
return|return
name|spec
return|;
block|}
comment|/* Otherwise we have to re-write the entries.. */
name|p
operator|=
name|pathspec
expr_stmt|;
name|prefixlen
operator|=
name|prefix
condition|?
name|strlen
argument_list|(
name|prefix
argument_list|)
else|:
literal|0
expr_stmt|;
do|do
block|{
operator|*
name|p
operator|=
name|prefix_path
argument_list|(
name|prefix
argument_list|,
name|prefixlen
argument_list|,
name|entry
argument_list|)
expr_stmt|;
block|}
do|while
condition|(
operator|(
name|entry
operator|=
operator|*
operator|++
name|p
operator|)
operator|!=
name|NULL
condition|)
do|;
return|return
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
name|pathspec
return|;
block|}
end_function

begin_comment
comment|/*  * Test if it looks like we're at the top level git directory.  * We want to see:  *  *  - either a .git/objects/ directory _or_ the proper  *    GIT_OBJECT_DIRECTORY environment variable  *  - a refs/ directory under ".git"  *  - either a HEAD symlink or a HEAD file that is formatted as  *    a proper "ref:".  */
end_comment

begin_function
DECL|function|is_toplevel_directory
specifier|static
name|int
name|is_toplevel_directory
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
name|access
argument_list|(
literal|".git/refs/"
argument_list|,
name|X_OK
argument_list|)
operator|||
name|access
argument_list|(
name|getenv
argument_list|(
name|DB_ENVIRONMENT
argument_list|)
condition|?
name|getenv
argument_list|(
name|DB_ENVIRONMENT
argument_list|)
else|:
literal|".git/objects/"
argument_list|,
name|X_OK
argument_list|)
operator|||
name|validate_symref
argument_list|(
literal|".git/HEAD"
argument_list|)
condition|)
return|return
literal|0
return|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|setup_git_directory_gently
specifier|const
name|char
modifier|*
name|setup_git_directory_gently
parameter_list|(
name|int
modifier|*
name|nongit_ok
parameter_list|)
block|{
specifier|static
name|char
name|cwd
index|[
name|PATH_MAX
operator|+
literal|1
index|]
decl_stmt|;
name|int
name|len
decl_stmt|,
name|offset
decl_stmt|;
comment|/* 	 * If GIT_DIR is set explicitly, we're not going 	 * to do any discovery, but we still do repository 	 * validation. 	 */
if|if
condition|(
name|getenv
argument_list|(
name|GIT_DIR_ENVIRONMENT
argument_list|)
condition|)
block|{
name|char
name|path
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|getenv
argument_list|(
name|GIT_DIR_ENVIRONMENT
argument_list|)
argument_list|)
decl_stmt|;
if|if
condition|(
sizeof|sizeof
argument_list|(
name|path
argument_list|)
operator|-
literal|40
operator|<
name|len
condition|)
name|die
argument_list|(
literal|"'$%s' too big"
argument_list|,
name|GIT_DIR_ENVIRONMENT
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|path
argument_list|,
name|getenv
argument_list|(
name|GIT_DIR_ENVIRONMENT
argument_list|)
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"/refs"
argument_list|)
expr_stmt|;
if|if
condition|(
name|access
argument_list|(
name|path
argument_list|,
name|X_OK
argument_list|)
condition|)
goto|goto
name|bad_dir_environ
goto|;
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"/HEAD"
argument_list|)
expr_stmt|;
if|if
condition|(
name|validate_symref
argument_list|(
name|path
argument_list|)
condition|)
goto|goto
name|bad_dir_environ
goto|;
if|if
condition|(
name|getenv
argument_list|(
name|DB_ENVIRONMENT
argument_list|)
condition|)
block|{
if|if
condition|(
name|access
argument_list|(
name|getenv
argument_list|(
name|DB_ENVIRONMENT
argument_list|)
argument_list|,
name|X_OK
argument_list|)
condition|)
goto|goto
name|bad_dir_environ
goto|;
block|}
else|else
block|{
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"/objects"
argument_list|)
expr_stmt|;
if|if
condition|(
name|access
argument_list|(
name|path
argument_list|,
name|X_OK
argument_list|)
condition|)
goto|goto
name|bad_dir_environ
goto|;
block|}
return|return
name|NULL
return|;
name|bad_dir_environ
label|:
name|path
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|die
argument_list|(
literal|"Not a git repository: '%s'"
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|getcwd
argument_list|(
name|cwd
argument_list|,
sizeof|sizeof
argument_list|(
name|cwd
argument_list|)
argument_list|)
operator|||
name|cwd
index|[
literal|0
index|]
operator|!=
literal|'/'
condition|)
name|die
argument_list|(
literal|"Unable to read current working directory"
argument_list|)
expr_stmt|;
name|offset
operator|=
name|len
operator|=
name|strlen
argument_list|(
name|cwd
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|is_toplevel_directory
argument_list|()
condition|)
break|break;
name|chdir
argument_list|(
literal|".."
argument_list|)
expr_stmt|;
do|do
block|{
if|if
condition|(
operator|!
name|offset
condition|)
block|{
if|if
condition|(
name|nongit_ok
condition|)
block|{
if|if
condition|(
name|chdir
argument_list|(
name|cwd
argument_list|)
condition|)
name|die
argument_list|(
literal|"Cannot come back to cwd"
argument_list|)
expr_stmt|;
operator|*
name|nongit_ok
operator|=
literal|1
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|die
argument_list|(
literal|"Not a git repository"
argument_list|)
expr_stmt|;
block|}
block|}
do|while
condition|(
name|cwd
index|[
operator|--
name|offset
index|]
operator|!=
literal|'/'
condition|)
do|;
block|}
if|if
condition|(
name|offset
operator|==
name|len
condition|)
return|return
name|NULL
return|;
comment|/* Make "offset" point to past the '/', and add a '/' at the end */
name|offset
operator|++
expr_stmt|;
name|cwd
index|[
name|len
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
name|cwd
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
return|return
name|cwd
operator|+
name|offset
return|;
block|}
end_function

begin_function
DECL|function|check_repository_format_version
name|int
name|check_repository_format_version
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
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"core.repositoryformatversion"
argument_list|)
operator|==
literal|0
condition|)
name|repository_format_version
operator|=
name|git_config_int
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
end_function

begin_function
DECL|function|check_repository_format
name|int
name|check_repository_format
parameter_list|(
name|void
parameter_list|)
block|{
name|git_config
argument_list|(
name|check_repository_format_version
argument_list|)
expr_stmt|;
if|if
condition|(
name|GIT_REPO_VERSION
operator|<
name|repository_format_version
condition|)
name|die
argument_list|(
literal|"Expected git repo version<= %d, found %d"
argument_list|,
name|GIT_REPO_VERSION
argument_list|,
name|repository_format_version
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|setup_git_directory
specifier|const
name|char
modifier|*
name|setup_git_directory
parameter_list|(
name|void
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|retval
init|=
name|setup_git_directory_gently
argument_list|(
name|NULL
argument_list|)
decl_stmt|;
name|check_repository_format
argument_list|()
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

end_unit

