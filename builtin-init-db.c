begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
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
file|"exec_cmd.h"
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
value|"/usr/share/git-core/templates"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|NO_TRUSTABLE_FILEMODE
end_ifdef

begin_define
DECL|macro|TEST_FILEMODE
define|#
directive|define
name|TEST_FILEMODE
value|0
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|TEST_FILEMODE
define|#
directive|define
name|TEST_FILEMODE
value|1
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
parameter_list|,
name|int
name|share
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
elseif|else
if|if
condition|(
name|share
operator|&&
name|adjust_shared_perm
argument_list|(
name|dir
argument_list|)
condition|)
name|die
argument_list|(
literal|"Could not make %s writable by group\n"
argument_list|,
name|dir
argument_list|)
expr_stmt|;
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
comment|/* Note: if ".git/hooks" file exists in the repository being 	 * re-initialized, /etc/core-git/templates/hooks/update would 	 * cause git-init to fail here.  I think this is sane but 	 * it means that the set of templates we ship by default, along 	 * with the way the namespace under .git/ is organized, should 	 * be really carefully chosen. 	 */
name|safe_create_dir
argument_list|(
name|path
argument_list|,
literal|1
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
specifier|const
name|char
modifier|*
name|git_dir
init|=
name|get_git_dir
argument_list|()
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|git_dir
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|template_dir
condition|)
name|template_dir
operator|=
name|getenv
argument_list|(
name|TEMPLATE_DIR_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|template_dir
condition|)
block|{
comment|/* 		 * if the hard-coded template is relative, it is 		 * interpreted relative to the exec_dir 		 */
name|template_dir
operator|=
name|DEFAULT_GIT_TEMPLATE_DIR
expr_stmt|;
if|if
condition|(
operator|!
name|is_absolute_path
argument_list|(
name|template_dir
argument_list|)
condition|)
block|{
name|struct
name|strbuf
name|d
init|=
name|STRBUF_INIT
decl_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|d
argument_list|,
literal|"%s/%s"
argument_list|,
name|git_exec_path
argument_list|()
argument_list|,
name|template_dir
argument_list|)
expr_stmt|;
name|template_dir
operator|=
name|strbuf_detach
argument_list|(
operator|&
name|d
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
block|}
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
comment|/* Make sure that template is from the correct vintage */
name|strcpy
argument_list|(
name|template_path
operator|+
name|template_len
argument_list|,
literal|"config"
argument_list|)
expr_stmt|;
name|repository_format_version
operator|=
literal|0
expr_stmt|;
name|git_config_from_file
argument_list|(
name|check_repository_format_version
argument_list|,
name|template_path
argument_list|)
expr_stmt|;
name|template_path
index|[
name|template_len
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|repository_format_version
operator|&&
name|repository_format_version
operator|!=
name|GIT_REPO_VERSION
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"warning: not copying templates of "
literal|"a wrong format version %d from '%s'\n"
argument_list|,
name|repository_format_version
argument_list|,
name|template_dir
argument_list|)
expr_stmt|;
name|closedir
argument_list|(
name|dir
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
name|int
name|create_default_files
parameter_list|(
specifier|const
name|char
modifier|*
name|template_path
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|git_dir
init|=
name|get_git_dir
argument_list|()
decl_stmt|;
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
name|struct
name|stat
name|st1
decl_stmt|;
name|char
name|repo_version_string
index|[
literal|10
index|]
decl_stmt|;
name|char
name|junk
index|[
literal|2
index|]
decl_stmt|;
name|int
name|reinit
decl_stmt|;
name|int
name|filemode
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
name|safe_create_dir
argument_list|(
name|git_path
argument_list|(
literal|"refs"
argument_list|)
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|safe_create_dir
argument_list|(
name|git_path
argument_list|(
literal|"refs/heads"
argument_list|)
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|safe_create_dir
argument_list|(
name|git_path
argument_list|(
literal|"refs/tags"
argument_list|)
argument_list|,
literal|1
argument_list|)
expr_stmt|;
comment|/* First copy the templates -- we might have the default 	 * config file there, in which case we would want to read 	 * from it after installing. 	 */
name|copy_templates
argument_list|(
name|template_path
argument_list|)
expr_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
comment|/* 	 * We would have created the above under user's umask -- under 	 * shared-repository settings, we would need to fix them up. 	 */
if|if
condition|(
name|shared_repository
condition|)
block|{
name|adjust_shared_perm
argument_list|(
name|get_git_dir
argument_list|()
argument_list|)
expr_stmt|;
name|adjust_shared_perm
argument_list|(
name|git_path
argument_list|(
literal|"refs"
argument_list|)
argument_list|)
expr_stmt|;
name|adjust_shared_perm
argument_list|(
name|git_path
argument_list|(
literal|"refs/heads"
argument_list|)
argument_list|)
expr_stmt|;
name|adjust_shared_perm
argument_list|(
name|git_path
argument_list|(
literal|"refs/tags"
argument_list|)
argument_list|)
expr_stmt|;
block|}
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
name|reinit
operator|=
operator|(
operator|!
name|access
argument_list|(
name|path
argument_list|,
name|R_OK
argument_list|)
operator|||
name|readlink
argument_list|(
name|path
argument_list|,
name|junk
argument_list|,
sizeof|sizeof
argument_list|(
name|junk
argument_list|)
operator|-
literal|1
argument_list|)
operator|!=
operator|-
literal|1
operator|)
expr_stmt|;
if|if
condition|(
operator|!
name|reinit
condition|)
block|{
if|if
condition|(
name|create_symref
argument_list|(
literal|"HEAD"
argument_list|,
literal|"refs/heads/master"
argument_list|,
name|NULL
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
comment|/* This forces creation of new config file */
name|sprintf
argument_list|(
name|repo_version_string
argument_list|,
literal|"%d"
argument_list|,
name|GIT_REPO_VERSION
argument_list|)
expr_stmt|;
name|git_config_set
argument_list|(
literal|"core.repositoryformatversion"
argument_list|,
name|repo_version_string
argument_list|)
expr_stmt|;
name|path
index|[
name|len
index|]
operator|=
literal|0
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
comment|/* Check filemode trustability */
name|filemode
operator|=
name|TEST_FILEMODE
expr_stmt|;
if|if
condition|(
name|TEST_FILEMODE
operator|&&
operator|!
name|lstat
argument_list|(
name|path
argument_list|,
operator|&
name|st1
argument_list|)
condition|)
block|{
name|struct
name|stat
name|st2
decl_stmt|;
name|filemode
operator|=
operator|(
operator|!
name|chmod
argument_list|(
name|path
argument_list|,
name|st1
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
name|st1
operator|.
name|st_mode
operator|!=
name|st2
operator|.
name|st_mode
operator|)
expr_stmt|;
block|}
name|git_config_set
argument_list|(
literal|"core.filemode"
argument_list|,
name|filemode
condition|?
literal|"true"
else|:
literal|"false"
argument_list|)
expr_stmt|;
if|if
condition|(
name|is_bare_repository
argument_list|()
condition|)
name|git_config_set
argument_list|(
literal|"core.bare"
argument_list|,
literal|"true"
argument_list|)
expr_stmt|;
else|else
block|{
specifier|const
name|char
modifier|*
name|work_tree
init|=
name|get_git_work_tree
argument_list|()
decl_stmt|;
name|git_config_set
argument_list|(
literal|"core.bare"
argument_list|,
literal|"false"
argument_list|)
expr_stmt|;
comment|/* allow template config file to override the default */
if|if
condition|(
name|log_all_ref_updates
operator|==
operator|-
literal|1
condition|)
name|git_config_set
argument_list|(
literal|"core.logallrefupdates"
argument_list|,
literal|"true"
argument_list|)
expr_stmt|;
if|if
condition|(
name|prefixcmp
argument_list|(
name|git_dir
argument_list|,
name|work_tree
argument_list|)
operator|||
name|strcmp
argument_list|(
name|git_dir
operator|+
name|strlen
argument_list|(
name|work_tree
argument_list|)
argument_list|,
literal|"/.git"
argument_list|)
condition|)
block|{
name|git_config_set
argument_list|(
literal|"core.worktree"
argument_list|,
name|work_tree
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|reinit
condition|)
block|{
comment|/* Check if symlink is supported in the work tree */
name|path
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"tXXXXXX"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|close
argument_list|(
name|xmkstemp
argument_list|(
name|path
argument_list|)
argument_list|)
operator|&&
operator|!
name|unlink
argument_list|(
name|path
argument_list|)
operator|&&
operator|!
name|symlink
argument_list|(
literal|"testing"
argument_list|,
name|path
argument_list|)
operator|&&
operator|!
name|lstat
argument_list|(
name|path
argument_list|,
operator|&
name|st1
argument_list|)
operator|&&
name|S_ISLNK
argument_list|(
name|st1
operator|.
name|st_mode
argument_list|)
condition|)
name|unlink
argument_list|(
name|path
argument_list|)
expr_stmt|;
comment|/* good */
else|else
name|git_config_set
argument_list|(
literal|"core.symlinks"
argument_list|,
literal|"false"
argument_list|)
expr_stmt|;
comment|/* Check if the filesystem is case-insensitive */
name|path
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
literal|"CoNfIg"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|access
argument_list|(
name|path
argument_list|,
name|F_OK
argument_list|)
condition|)
name|git_config_set
argument_list|(
literal|"core.ignorecase"
argument_list|,
literal|"true"
argument_list|)
expr_stmt|;
block|}
return|return
name|reinit
return|;
block|}
end_function

begin_function
DECL|function|init_db
name|int
name|init_db
parameter_list|(
specifier|const
name|char
modifier|*
name|template_dir
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|sha1_dir
decl_stmt|;
name|char
modifier|*
name|path
decl_stmt|;
name|int
name|len
decl_stmt|,
name|reinit
decl_stmt|;
name|safe_create_dir
argument_list|(
name|get_git_dir
argument_list|()
argument_list|,
literal|0
argument_list|)
expr_stmt|;
comment|/* Check to see if the repository version is right. 	 * Note that a newly created repository does not have 	 * config file, so this will not fail.  What we are catching 	 * is an attempt to reinitialize new repository with an old tool. 	 */
name|check_repository_format
argument_list|()
expr_stmt|;
name|reinit
operator|=
name|create_default_files
argument_list|(
name|template_dir
argument_list|)
expr_stmt|;
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
argument_list|,
literal|1
argument_list|)
expr_stmt|;
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
argument_list|,
literal|1
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
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|shared_repository
condition|)
block|{
name|char
name|buf
index|[
literal|10
index|]
decl_stmt|;
comment|/* We do not spell "group" and such, so that 		 * the configuration can be read by older version 		 * of git. Note, we use octal numbers for new share modes, 		 * and compatibility values for PERM_GROUP and 		 * PERM_EVERYBODY. 		 */
if|if
condition|(
name|shared_repository
operator|==
name|PERM_GROUP
condition|)
name|sprintf
argument_list|(
name|buf
argument_list|,
literal|"%d"
argument_list|,
name|OLD_PERM_GROUP
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|shared_repository
operator|==
name|PERM_EVERYBODY
condition|)
name|sprintf
argument_list|(
name|buf
argument_list|,
literal|"%d"
argument_list|,
name|OLD_PERM_EVERYBODY
argument_list|)
expr_stmt|;
else|else
name|sprintf
argument_list|(
name|buf
argument_list|,
literal|"0%o"
argument_list|,
name|shared_repository
argument_list|)
expr_stmt|;
name|git_config_set
argument_list|(
literal|"core.sharedrepository"
argument_list|,
name|buf
argument_list|)
expr_stmt|;
name|git_config_set
argument_list|(
literal|"receive.denyNonFastforwards"
argument_list|,
literal|"true"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|INIT_DB_QUIET
operator|)
condition|)
name|printf
argument_list|(
literal|"%s%s Git repository in %s/\n"
argument_list|,
name|reinit
condition|?
literal|"Reinitialized existing"
else|:
literal|"Initialized empty"
argument_list|,
name|shared_repository
condition|?
literal|" shared"
else|:
literal|""
argument_list|,
name|get_git_dir
argument_list|()
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|guess_repository_type
specifier|static
name|int
name|guess_repository_type
parameter_list|(
specifier|const
name|char
modifier|*
name|git_dir
parameter_list|)
block|{
name|char
name|cwd
index|[
name|PATH_MAX
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|slash
decl_stmt|;
comment|/* 	 * "GIT_DIR=. git init" is always bare. 	 * "GIT_DIR=`pwd` git init" too. 	 */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"."
argument_list|,
name|git_dir
argument_list|)
condition|)
return|return
literal|1
return|;
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
condition|)
name|die
argument_list|(
literal|"cannot tell cwd"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|git_dir
argument_list|,
name|cwd
argument_list|)
condition|)
return|return
literal|1
return|;
comment|/* 	 * "GIT_DIR=.git or GIT_DIR=something/.git is usually not. 	 */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|git_dir
argument_list|,
literal|".git"
argument_list|)
condition|)
return|return
literal|0
return|;
name|slash
operator|=
name|strrchr
argument_list|(
name|git_dir
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
name|slash
operator|&&
operator|!
name|strcmp
argument_list|(
name|slash
argument_list|,
literal|"/.git"
argument_list|)
condition|)
return|return
literal|0
return|;
comment|/* 	 * Otherwise it is often bare.  At this point 	 * we are just guessing. 	 */
return|return
literal|1
return|;
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
literal|"git-init [-q | --quiet] [--template=<template-directory>] [--shared]"
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * If you want to, you can share the DB area with any number of branches.  * That has advantages: you can save space by sharing all the SHA1 objects.  * On the other hand, it might just make lookup slower and messier. You  * be the judge.  The default case is to have one DB per managed directory.  */
end_comment

begin_function
DECL|function|cmd_init_db
name|int
name|cmd_init_db
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
name|git_dir
decl_stmt|;
specifier|const
name|char
modifier|*
name|template_dir
init|=
name|NULL
decl_stmt|;
name|unsigned
name|int
name|flags
init|=
literal|0
decl_stmt|;
name|int
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
specifier|const
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
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--template="
argument_list|)
condition|)
name|template_dir
operator|=
name|arg
operator|+
literal|11
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--shared"
argument_list|)
condition|)
name|shared_repository
operator|=
name|PERM_GROUP
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--shared="
argument_list|)
condition|)
name|shared_repository
operator|=
name|git_config_perm
argument_list|(
literal|"arg"
argument_list|,
name|arg
operator|+
literal|9
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-q"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--quiet"
argument_list|)
condition|)
name|flags
operator||=
name|INIT_DB_QUIET
expr_stmt|;
else|else
name|usage
argument_list|(
name|init_db_usage
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * GIT_WORK_TREE makes sense only in conjunction with GIT_DIR 	 * without --bare.  Catch the error early. 	 */
name|git_dir
operator|=
name|getenv
argument_list|(
name|GIT_DIR_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
operator|!
name|git_dir
operator|||
name|is_bare_repository_cfg
operator|==
literal|1
operator|)
operator|&&
name|getenv
argument_list|(
name|GIT_WORK_TREE_ENVIRONMENT
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s (or --work-tree=<directory>) not allowed without "
literal|"specifying %s (or --git-dir=<directory>)"
argument_list|,
name|GIT_WORK_TREE_ENVIRONMENT
argument_list|,
name|GIT_DIR_ENVIRONMENT
argument_list|)
expr_stmt|;
comment|/* 	 * Set up the default .git directory contents 	 */
if|if
condition|(
operator|!
name|git_dir
condition|)
name|git_dir
operator|=
name|DEFAULT_GIT_DIR_ENVIRONMENT
expr_stmt|;
if|if
condition|(
name|is_bare_repository_cfg
operator|<
literal|0
condition|)
name|is_bare_repository_cfg
operator|=
name|guess_repository_type
argument_list|(
name|git_dir
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_bare_repository_cfg
condition|)
block|{
if|if
condition|(
name|git_dir
condition|)
block|{
specifier|const
name|char
modifier|*
name|git_dir_parent
init|=
name|strrchr
argument_list|(
name|git_dir
argument_list|,
literal|'/'
argument_list|)
decl_stmt|;
if|if
condition|(
name|git_dir_parent
condition|)
block|{
name|char
modifier|*
name|rel
init|=
name|xstrndup
argument_list|(
name|git_dir
argument_list|,
name|git_dir_parent
operator|-
name|git_dir
argument_list|)
decl_stmt|;
name|git_work_tree_cfg
operator|=
name|xstrdup
argument_list|(
name|make_absolute_path
argument_list|(
name|rel
argument_list|)
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|rel
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|git_work_tree_cfg
condition|)
block|{
name|git_work_tree_cfg
operator|=
name|xcalloc
argument_list|(
name|PATH_MAX
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|getcwd
argument_list|(
name|git_work_tree_cfg
argument_list|,
name|PATH_MAX
argument_list|)
condition|)
name|die
argument_list|(
literal|"Cannot access current working directory."
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|access
argument_list|(
name|get_git_work_tree
argument_list|()
argument_list|,
name|X_OK
argument_list|)
condition|)
name|die
argument_list|(
literal|"Cannot access work tree '%s'"
argument_list|,
name|get_git_work_tree
argument_list|()
argument_list|)
expr_stmt|;
block|}
name|set_git_dir
argument_list|(
name|make_absolute_path
argument_list|(
name|git_dir
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|init_db
argument_list|(
name|template_dir
argument_list|,
name|flags
argument_list|)
return|;
block|}
end_function

end_unit

