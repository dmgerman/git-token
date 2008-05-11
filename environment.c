begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * We put all the git config variables in this same object  * file, so that programs can link against the config parser  * without having to link against all the rest of git.  *  * In particular, no need to bring in libz etc unless needed,  * even if you might want to know where the git directory etc  * are.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_decl_stmt
DECL|variable|git_default_email
name|char
name|git_default_email
index|[
name|MAX_GITNAME
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_default_name
name|char
name|git_default_name
index|[
name|MAX_GITNAME
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|trust_executable_bit
name|int
name|trust_executable_bit
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|quote_path_fully
name|int
name|quote_path_fully
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|has_symlinks
name|int
name|has_symlinks
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ignore_case
name|int
name|ignore_case
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|assume_unchanged
name|int
name|assume_unchanged
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|prefer_symlink_refs
name|int
name|prefer_symlink_refs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|is_bare_repository_cfg
name|int
name|is_bare_repository_cfg
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|is_bare_repository_cfg
comment|/* unspecified */
end_comment

begin_decl_stmt
DECL|variable|log_all_ref_updates
name|int
name|log_all_ref_updates
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|log_all_ref_updates
comment|/* unspecified */
end_comment

begin_decl_stmt
DECL|variable|warn_ambiguous_refs
name|int
name|warn_ambiguous_refs
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|repository_format_version
name|int
name|repository_format_version
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_commit_encoding
specifier|const
name|char
modifier|*
name|git_commit_encoding
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_log_output_encoding
specifier|const
name|char
modifier|*
name|git_log_output_encoding
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|shared_repository
name|int
name|shared_repository
init|=
name|PERM_UMASK
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|apply_default_whitespace
specifier|const
name|char
modifier|*
name|apply_default_whitespace
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|zlib_compression_level
name|int
name|zlib_compression_level
init|=
name|Z_BEST_SPEED
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|core_compression_level
name|int
name|core_compression_level
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|core_compression_seen
name|int
name|core_compression_seen
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|packed_git_window_size
name|size_t
name|packed_git_window_size
init|=
name|DEFAULT_PACKED_GIT_WINDOW_SIZE
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|packed_git_limit
name|size_t
name|packed_git_limit
init|=
name|DEFAULT_PACKED_GIT_LIMIT
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|delta_base_cache_limit
name|size_t
name|delta_base_cache_limit
init|=
literal|16
operator|*
literal|1024
operator|*
literal|1024
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pager_program
specifier|const
name|char
modifier|*
name|pager_program
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pager_use_color
name|int
name|pager_use_color
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|editor_program
specifier|const
name|char
modifier|*
name|editor_program
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|excludes_file
specifier|const
name|char
modifier|*
name|excludes_file
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|auto_crlf
name|int
name|auto_crlf
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|auto_crlf
comment|/* 1: both ways, -1: only when adding git objects */
end_comment

begin_decl_stmt
DECL|variable|safe_crlf
name|enum
name|safe_crlf
name|safe_crlf
init|=
name|SAFE_CRLF_WARN
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|whitespace_rule_cfg
name|unsigned
name|whitespace_rule_cfg
init|=
name|WS_DEFAULT_RULE
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_branch_track
name|enum
name|branch_track
name|git_branch_track
init|=
name|BRANCH_TRACK_REMOTE
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* This is set by setup_git_dir_gently() and/or git_default_config() */
end_comment

begin_decl_stmt
DECL|variable|git_work_tree_cfg
name|char
modifier|*
name|git_work_tree_cfg
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|work_tree
specifier|static
specifier|const
name|char
modifier|*
name|work_tree
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_dir
specifier|static
specifier|const
name|char
modifier|*
name|git_dir
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_object_dir
DECL|variable|git_index_file
DECL|variable|git_refs_dir
DECL|variable|git_graft_file
specifier|static
name|char
modifier|*
name|git_object_dir
decl_stmt|,
modifier|*
name|git_index_file
decl_stmt|,
modifier|*
name|git_refs_dir
decl_stmt|,
modifier|*
name|git_graft_file
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|setup_git_env
specifier|static
name|void
name|setup_git_env
parameter_list|(
name|void
parameter_list|)
block|{
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
name|git_dir
operator|=
name|read_gitfile_gently
argument_list|(
name|DEFAULT_GIT_DIR_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|git_dir
condition|)
name|git_dir
operator|=
name|DEFAULT_GIT_DIR_ENVIRONMENT
expr_stmt|;
name|git_object_dir
operator|=
name|getenv
argument_list|(
name|DB_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|git_object_dir
condition|)
block|{
name|git_object_dir
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|git_dir
argument_list|)
operator|+
literal|9
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|git_object_dir
argument_list|,
literal|"%s/objects"
argument_list|,
name|git_dir
argument_list|)
expr_stmt|;
block|}
name|git_refs_dir
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|git_dir
argument_list|)
operator|+
literal|6
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|git_refs_dir
argument_list|,
literal|"%s/refs"
argument_list|,
name|git_dir
argument_list|)
expr_stmt|;
name|git_index_file
operator|=
name|getenv
argument_list|(
name|INDEX_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|git_index_file
condition|)
block|{
name|git_index_file
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|git_dir
argument_list|)
operator|+
literal|7
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|git_index_file
argument_list|,
literal|"%s/index"
argument_list|,
name|git_dir
argument_list|)
expr_stmt|;
block|}
name|git_graft_file
operator|=
name|getenv
argument_list|(
name|GRAFT_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|git_graft_file
condition|)
name|git_graft_file
operator|=
name|xstrdup
argument_list|(
name|git_path
argument_list|(
literal|"info/grafts"
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|is_bare_repository
name|int
name|is_bare_repository
parameter_list|(
name|void
parameter_list|)
block|{
comment|/* if core.bare is not 'false', let's see if there is a work tree */
return|return
name|is_bare_repository_cfg
operator|&&
operator|!
name|get_git_work_tree
argument_list|()
return|;
block|}
end_function

begin_function
DECL|function|get_git_dir
specifier|const
name|char
modifier|*
name|get_git_dir
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|git_dir
condition|)
name|setup_git_env
argument_list|()
expr_stmt|;
return|return
name|git_dir
return|;
block|}
end_function

begin_function
DECL|function|get_git_work_tree
specifier|const
name|char
modifier|*
name|get_git_work_tree
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|int
name|initialized
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|initialized
condition|)
block|{
name|work_tree
operator|=
name|getenv
argument_list|(
name|GIT_WORK_TREE_ENVIRONMENT
argument_list|)
expr_stmt|;
comment|/* core.bare = true overrides implicit and config work tree */
if|if
condition|(
operator|!
name|work_tree
operator|&&
name|is_bare_repository_cfg
operator|<
literal|1
condition|)
block|{
name|work_tree
operator|=
name|git_work_tree_cfg
expr_stmt|;
comment|/* make_absolute_path also normalizes the path */
if|if
condition|(
name|work_tree
operator|&&
operator|!
name|is_absolute_path
argument_list|(
name|work_tree
argument_list|)
condition|)
name|work_tree
operator|=
name|xstrdup
argument_list|(
name|make_absolute_path
argument_list|(
name|git_path
argument_list|(
name|work_tree
argument_list|)
argument_list|)
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|work_tree
condition|)
name|work_tree
operator|=
name|xstrdup
argument_list|(
name|make_absolute_path
argument_list|(
name|work_tree
argument_list|)
argument_list|)
expr_stmt|;
name|initialized
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|work_tree
condition|)
name|is_bare_repository_cfg
operator|=
literal|0
expr_stmt|;
block|}
return|return
name|work_tree
return|;
block|}
end_function

begin_function
DECL|function|get_object_directory
name|char
modifier|*
name|get_object_directory
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|git_object_dir
condition|)
name|setup_git_env
argument_list|()
expr_stmt|;
return|return
name|git_object_dir
return|;
block|}
end_function

begin_function
DECL|function|get_refs_directory
name|char
modifier|*
name|get_refs_directory
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|git_refs_dir
condition|)
name|setup_git_env
argument_list|()
expr_stmt|;
return|return
name|git_refs_dir
return|;
block|}
end_function

begin_function
DECL|function|get_index_file
name|char
modifier|*
name|get_index_file
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|git_index_file
condition|)
name|setup_git_env
argument_list|()
expr_stmt|;
return|return
name|git_index_file
return|;
block|}
end_function

begin_function
DECL|function|get_graft_file
name|char
modifier|*
name|get_graft_file
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|git_graft_file
condition|)
name|setup_git_env
argument_list|()
expr_stmt|;
return|return
name|git_graft_file
return|;
block|}
end_function

begin_function
DECL|function|set_git_dir
name|int
name|set_git_dir
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
if|if
condition|(
name|setenv
argument_list|(
name|GIT_DIR_ENVIRONMENT
argument_list|,
name|path
argument_list|,
literal|1
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"Could not set GIT_DIR to '%s'"
argument_list|,
name|path
argument_list|)
return|;
name|setup_git_env
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

