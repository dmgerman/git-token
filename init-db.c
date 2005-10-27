begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_ifndef
ifndef|#
directive|ifndef
name|DEFAULT_GIT_TEMPLATE_DIR
end_ifndef

begin_define
DECL|macro|DEFAULT_GIT_TEMPLATE_DIR
define|#
directive|define
name|DEFAULT_GIT_TEMPLATE_DIR
value|"/usr/share/git-core/templates/"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|safe_create_dir
specifier|static
name|void
name|safe_create_dir
parameter_list|(
specifier|const
name|char
modifier|*
name|dir
parameter_list|)
block|{
if|if
condition|(
name|mkdir
argument_list|(
name|dir
argument_list|,
literal|0777
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|!=
name|EEXIST
condition|)
block|{
name|perror
argument_list|(
name|dir
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|copy_file
specifier|static
name|int
name|copy_file
parameter_list|(
specifier|const
name|char
modifier|*
name|dst
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|,
name|int
name|mode
parameter_list|)
block|{
name|int
name|fdi
decl_stmt|,
name|fdo
decl_stmt|;
name|mode
operator|=
operator|(
name|mode
operator|&
literal|0111
operator|)
condition|?
literal|0777
else|:
literal|0666
expr_stmt|;
if|if
condition|(
operator|(
name|fdi
operator|=
name|open
argument_list|(
name|src
argument_list|,
name|O_RDONLY
argument_list|)
operator|)
operator|<
literal|0
condition|)
return|return
name|fdi
return|;
if|if
condition|(
operator|(
name|fdo
operator|=
name|open
argument_list|(
name|dst
argument_list|,
name|O_WRONLY
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
name|mode
argument_list|)
operator|)
operator|<
literal|0
condition|)
block|{
name|close
argument_list|(
name|fdi
argument_list|)
expr_stmt|;
return|return
name|fdo
return|;
block|}
while|while
condition|(
literal|1
condition|)
block|{
name|char
name|buf
index|[
name|BUFSIZ
index|]
decl_stmt|;
name|ssize_t
name|leni
decl_stmt|,
name|leno
decl_stmt|,
name|ofs
decl_stmt|;
name|leni
operator|=
name|read
argument_list|(
name|fdi
argument_list|,
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|leni
operator|<
literal|0
condition|)
block|{
name|error_return
label|:
name|close
argument_list|(
name|fdo
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fdi
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
operator|!
name|leni
condition|)
break|break;
name|ofs
operator|=
literal|0
expr_stmt|;
do|do
block|{
name|leno
operator|=
name|write
argument_list|(
name|fdo
argument_list|,
name|buf
operator|+
name|ofs
argument_list|,
name|leni
argument_list|)
expr_stmt|;
if|if
condition|(
name|leno
operator|<
literal|0
condition|)
goto|goto
name|error_return
goto|;
name|leni
operator|-=
name|leno
expr_stmt|;
name|ofs
operator|+=
name|leno
expr_stmt|;
block|}
do|while
condition|(
literal|0
operator|<
name|leni
condition|)
do|;
block|}
name|close
argument_list|(
name|fdo
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fdi
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|copy_templates_1
specifier|static
name|void
name|copy_templates_1
parameter_list|(
name|char
modifier|*
name|path
parameter_list|,
name|int
name|baselen
parameter_list|,
name|char
modifier|*
name|template
parameter_list|,
name|int
name|template_baselen
parameter_list|,
name|DIR
modifier|*
name|dir
parameter_list|)
block|{
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
comment|/* Note: if ".git/hooks" file exists in the repository being 	 * re-initialized, /etc/core-git/templates/hooks/update would 	 * cause git-init-db to fail here.  I think this is sane but 	 * it means that the set of templates we ship by default, along 	 * with the way the namespace under .git/ is organized, should 	 * be really carefully chosen. 	 */
name|safe_create_dir
argument_list|(
name|path
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|de
operator|=
name|readdir
argument_list|(
name|dir
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|struct
name|stat
name|st_git
decl_stmt|,
name|st_template
decl_stmt|;
name|int
name|namelen
decl_stmt|;
name|int
name|exists
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|de
operator|->
name|d_name
index|[
literal|0
index|]
operator|==
literal|'.'
condition|)
continue|continue;
name|namelen
operator|=
name|strlen
argument_list|(
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|PATH_MAX
operator|<=
name|baselen
operator|+
name|namelen
operator|)
operator|||
operator|(
name|PATH_MAX
operator|<=
name|template_baselen
operator|+
name|namelen
operator|)
condition|)
name|die
argument_list|(
literal|"insanely long template name %s"
argument_list|,
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|path
operator|+
name|baselen
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
name|namelen
operator|+
literal|1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|template
operator|+
name|template_baselen
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
name|namelen
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|path
argument_list|,
operator|&
name|st_git
argument_list|)
condition|)
block|{
if|if
condition|(
name|errno
operator|!=
name|ENOENT
condition|)
name|die
argument_list|(
literal|"cannot stat %s"
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
else|else
name|exists
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|template
argument_list|,
operator|&
name|st_template
argument_list|)
condition|)
name|die
argument_list|(
literal|"cannot stat template %s"
argument_list|,
name|template
argument_list|)
expr_stmt|;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|st_template
operator|.
name|st_mode
argument_list|)
condition|)
block|{
name|DIR
modifier|*
name|subdir
init|=
name|opendir
argument_list|(
name|template
argument_list|)
decl_stmt|;
name|int
name|baselen_sub
init|=
name|baselen
operator|+
name|namelen
decl_stmt|;
name|int
name|template_baselen_sub
init|=
name|template_baselen
operator|+
name|namelen
decl_stmt|;
if|if
condition|(
operator|!
name|subdir
condition|)
name|die
argument_list|(
literal|"cannot opendir %s"
argument_list|,
name|template
argument_list|)
expr_stmt|;
name|path
index|[
name|baselen_sub
operator|++
index|]
operator|=
name|template
index|[
name|template_baselen_sub
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
name|path
index|[
name|baselen_sub
index|]
operator|=
name|template
index|[
name|template_baselen_sub
index|]
operator|=
literal|0
expr_stmt|;
name|copy_templates_1
argument_list|(
name|path
argument_list|,
name|baselen_sub
argument_list|,
name|template
argument_list|,
name|template_baselen_sub
argument_list|,
name|subdir
argument_list|)
expr_stmt|;
name|closedir
argument_list|(
name|subdir
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|exists
condition|)
continue|continue;
elseif|else
if|if
condition|(
name|S_ISLNK
argument_list|(
name|st_template
operator|.
name|st_mode
argument_list|)
condition|)
block|{
name|char
name|lnk
index|[
literal|256
index|]
decl_stmt|;
name|int
name|len
decl_stmt|;
name|len
operator|=
name|readlink
argument_list|(
name|template
argument_list|,
name|lnk
argument_list|,
sizeof|sizeof
argument_list|(
name|lnk
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"cannot readlink %s"
argument_list|,
name|template
argument_list|)
expr_stmt|;
if|if
condition|(
sizeof|sizeof
argument_list|(
name|lnk
argument_list|)
operator|<=
name|len
condition|)
name|die
argument_list|(
literal|"insanely long symlink %s"
argument_list|,
name|template
argument_list|)
expr_stmt|;
name|lnk
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|symlink
argument_list|(
name|lnk
argument_list|,
name|path
argument_list|)
condition|)
name|die
argument_list|(
literal|"cannot symlink %s %s"
argument_list|,
name|lnk
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|S_ISREG
argument_list|(
name|st_template
operator|.
name|st_mode
argument_list|)
condition|)
block|{
if|if
condition|(
name|copy_file
argument_list|(
name|path
argument_list|,
name|template
argument_list|,
name|st_template
operator|.
name|st_mode
argument_list|)
condition|)
name|die
argument_list|(
literal|"cannot copy %s to %s"
argument_list|,
name|template
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
else|else
name|error
argument_list|(
literal|"ignoring template %s"
argument_list|,
name|template
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|copy_templates
specifier|static
name|void
name|copy_templates
parameter_list|(
specifier|const
name|char
modifier|*
name|git_dir
parameter_list|,
name|int
name|len
parameter_list|,
name|char
modifier|*
name|template_dir
parameter_list|)
block|{
name|char
name|path
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|char
name|template_path
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|int
name|template_len
decl_stmt|;
name|DIR
modifier|*
name|dir
decl_stmt|;
if|if
condition|(
operator|!
name|template_dir
condition|)
name|template_dir
operator|=
name|DEFAULT_GIT_TEMPLATE_DIR
expr_stmt|;
name|strcpy
argument_list|(
name|template_path
argument_list|,
name|template_dir
argument_list|)
expr_stmt|;
name|template_len
operator|=
name|strlen
argument_list|(
name|template_path
argument_list|)
expr_stmt|;
if|if
condition|(
name|template_path
index|[
name|template_len
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
block|{
name|template_path
index|[
name|template_len
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
name|template_path
index|[
name|template_len
index|]
operator|=
literal|0
expr_stmt|;
block|}
name|dir
operator|=
name|opendir
argument_list|(
name|template_path
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dir
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"warning: templates not found %s\n"
argument_list|,
name|template_dir
argument_list|)
expr_stmt|;
return|return;
block|}
name|memcpy
argument_list|(
name|path
argument_list|,
name|git_dir
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|path
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|copy_templates_1
argument_list|(
name|path
argument_list|,
name|len
argument_list|,
name|template_path
argument_list|,
name|template_len
argument_list|,
name|dir
argument_list|)
expr_stmt|;
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|create_default_files
specifier|static
name|void
name|create_default_files
parameter_list|(
specifier|const
name|char
modifier|*
name|git_dir
parameter_list|,
name|char
modifier|*
name|template_path
parameter_list|)
block|{
name|unsigned
name|len
init|=
name|strlen
argument_list|(
name|git_dir
argument_list|)
decl_stmt|;
specifier|static
name|char
name|path
index|[
name|PATH_MAX
index|]
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
name|len
operator|>
sizeof|sizeof
argument_list|(
name|path
argument_list|)
operator|-
literal|50
condition|)
name|die
argument_list|(
literal|"insane git directory %s"
argument_list|,
name|git_dir
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|path
argument_list|,
name|git_dir
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|&&
name|path
index|[
name|len
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
name|path
index|[
name|len
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
comment|/* 	 * Create .git/refs/{heads,tags} 	 */
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"refs"
argument_list|)
expr_stmt|;
name|safe_create_dir
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"refs/heads"
argument_list|)
expr_stmt|;
name|safe_create_dir
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"refs/tags"
argument_list|)
expr_stmt|;
name|safe_create_dir
argument_list|(
name|path
argument_list|)
expr_stmt|;
comment|/* 	 * Create the default symlink from ".git/HEAD" to the "master" 	 * branch, if it does not exist yet. 	 */
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"HEAD"
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_ref
argument_list|(
name|path
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|create_symref
argument_list|(
name|path
argument_list|,
literal|"refs/heads/master"
argument_list|)
operator|<
literal|0
condition|)
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
name|path
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|copy_templates
argument_list|(
name|path
argument_list|,
name|len
argument_list|,
name|template_path
argument_list|)
expr_stmt|;
comment|/* 	 * Find out if we can trust the executable bit. 	 */
name|safe_create_dir
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"config"
argument_list|)
expr_stmt|;
if|if
condition|(
name|access
argument_list|(
name|path
argument_list|,
name|R_OK
argument_list|)
operator|<
literal|0
condition|)
block|{
specifier|static
specifier|const
name|char
name|contents
index|[]
init|=
literal|"#\n"
literal|"# This is the config file\n"
literal|"#\n"
literal|"\n"
literal|"; core variables\n"
literal|"[core]\n"
literal|"	; Don't trust file modes\n"
literal|"	filemode = false\n"
literal|"\n"
decl_stmt|;
name|FILE
modifier|*
name|config
init|=
name|fopen
argument_list|(
name|path
argument_list|,
literal|"w"
argument_list|)
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
if|if
condition|(
operator|!
name|config
condition|)
name|die
argument_list|(
literal|"Can not write to %s?"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|fwrite
argument_list|(
name|contents
argument_list|,
sizeof|sizeof
argument_list|(
name|contents
argument_list|)
operator|-
literal|1
argument_list|,
literal|1
argument_list|,
name|config
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|config
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lstat
argument_list|(
name|path
argument_list|,
operator|&
name|st
argument_list|)
condition|)
block|{
name|struct
name|stat
name|st2
decl_stmt|;
if|if
condition|(
operator|!
name|chmod
argument_list|(
name|path
argument_list|,
name|st
operator|.
name|st_mode
operator|^
name|S_IXUSR
argument_list|)
operator|&&
operator|!
name|lstat
argument_list|(
name|path
argument_list|,
operator|&
name|st2
argument_list|)
operator|&&
name|st
operator|.
name|st_mode
operator|!=
name|st2
operator|.
name|st_mode
condition|)
name|unlink
argument_list|(
name|path
argument_list|)
expr_stmt|;
else|else
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Ignoring file modes\n"
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_decl_stmt
DECL|variable|init_db_usage
specifier|static
specifier|const
name|char
name|init_db_usage
index|[]
init|=
literal|"git-init-db [--template=<template-directory>]"
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * If you want to, you can share the DB area with any number of branches.  * That has advantages: you can save space by sharing all the SHA1 objects.  * On the other hand, it might just make lookup slower and messier. You  * be the judge.  The default case is to have one DB per managed directory.  */
end_comment

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|git_dir
decl_stmt|;
specifier|const
name|char
modifier|*
name|sha1_dir
decl_stmt|;
name|char
modifier|*
name|path
decl_stmt|,
modifier|*
name|template_dir
init|=
name|NULL
decl_stmt|;
name|int
name|len
decl_stmt|,
name|i
decl_stmt|;
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
operator|,
name|argv
operator|++
control|)
block|{
name|char
modifier|*
name|arg
init|=
name|argv
index|[
literal|1
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
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--template="
argument_list|,
literal|11
argument_list|)
condition|)
name|template_dir
operator|=
name|arg
operator|+
literal|11
expr_stmt|;
else|else
name|die
argument_list|(
name|init_db_usage
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * Set up the default .git directory contents 	 */
name|git_dir
operator|=
name|getenv
argument_list|(
name|GIT_DIR_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|git_dir
condition|)
block|{
name|git_dir
operator|=
name|DEFAULT_GIT_DIR_ENVIRONMENT
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"defaulting to local storage area\n"
argument_list|)
expr_stmt|;
block|}
name|safe_create_dir
argument_list|(
name|git_dir
argument_list|)
expr_stmt|;
name|create_default_files
argument_list|(
name|git_dir
argument_list|,
name|template_dir
argument_list|)
expr_stmt|;
comment|/* 	 * And set up the object store. 	 */
name|sha1_dir
operator|=
name|get_object_directory
argument_list|()
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|sha1_dir
argument_list|)
expr_stmt|;
name|path
operator|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|40
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|path
argument_list|,
name|sha1_dir
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|safe_create_dir
argument_list|(
name|sha1_dir
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
literal|256
condition|;
name|i
operator|++
control|)
block|{
name|sprintf
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"/%02x"
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|safe_create_dir
argument_list|(
name|path
argument_list|)
expr_stmt|;
block|}
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"/pack"
argument_list|)
expr_stmt|;
name|safe_create_dir
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"/info"
argument_list|)
expr_stmt|;
name|safe_create_dir
argument_list|(
name|path
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

