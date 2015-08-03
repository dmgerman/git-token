begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * We put all the git config variables in this same object  * file, so that programs can link against the config parser  * without having to link against all the rest of git.  *  * In particular, no need to bring in libz etc unless needed,  * even if you might want to know where the git directory etc  * are.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"fmt-merge-msg.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_decl_stmt
DECL|variable|trust_executable_bit
name|int
name|trust_executable_bit
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|trust_ctime
name|int
name|trust_ctime
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|check_stat
name|int
name|check_stat
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
DECL|variable|minimum_abbrev
DECL|variable|default_abbrev
name|int
name|minimum_abbrev
init|=
literal|4
decl_stmt|,
name|default_abbrev
init|=
literal|7
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
DECL|variable|warn_on_object_refname_ambiguity
name|int
name|warn_on_object_refname_ambiguity
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ref_paranoia
name|int
name|ref_paranoia
init|=
operator|-
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
DECL|variable|apply_default_ignorewhitespace
specifier|const
name|char
modifier|*
name|apply_default_ignorewhitespace
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_attributes_file
specifier|const
name|char
modifier|*
name|git_attributes_file
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
DECL|variable|fsync_object_files
name|int
name|fsync_object_files
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
literal|96
operator|*
literal|1024
operator|*
literal|1024
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|big_file_threshold
name|unsigned
name|long
name|big_file_threshold
init|=
literal|512
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
DECL|variable|askpass_program
specifier|const
name|char
modifier|*
name|askpass_program
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
name|enum
name|auto_crlf
name|auto_crlf
init|=
name|AUTO_CRLF_FALSE
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|check_replace_refs
name|int
name|check_replace_refs
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_replace_ref_base
name|char
modifier|*
name|git_replace_ref_base
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|core_eol
name|enum
name|eol
name|core_eol
init|=
name|EOL_UNSET
decl_stmt|;
end_decl_stmt

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

begin_decl_stmt
DECL|variable|autorebase
name|enum
name|rebase_setup_type
name|autorebase
init|=
name|AUTOREBASE_NEVER
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|push_default
name|enum
name|push_default_type
name|push_default
init|=
name|PUSH_DEFAULT_UNSPECIFIED
decl_stmt|;
end_decl_stmt

begin_ifndef
ifndef|#
directive|ifndef
name|OBJECT_CREATION_MODE
end_ifndef

begin_define
DECL|macro|OBJECT_CREATION_MODE
define|#
directive|define
name|OBJECT_CREATION_MODE
value|OBJECT_CREATION_USES_HARDLINKS
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|object_creation_mode
name|enum
name|object_creation_mode
name|object_creation_mode
init|=
name|OBJECT_CREATION_MODE
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|notes_ref_name
name|char
modifier|*
name|notes_ref_name
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|grafts_replace_parents
name|int
name|grafts_replace_parents
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|core_apply_sparse_checkout
name|int
name|core_apply_sparse_checkout
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|merge_log_config
name|int
name|merge_log_config
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|precomposed_unicode
name|int
name|precomposed_unicode
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|precomposed_unicode
comment|/* see probe_utf8_pathname_composition() */
end_comment

begin_decl_stmt
DECL|variable|startup_info
name|struct
name|startup_info
modifier|*
name|startup_info
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pack_size_limit_cfg
name|unsigned
name|long
name|pack_size_limit_cfg
decl_stmt|;
end_decl_stmt

begin_ifndef
ifndef|#
directive|ifndef
name|PROTECT_HFS_DEFAULT
end_ifndef

begin_define
DECL|macro|PROTECT_HFS_DEFAULT
define|#
directive|define
name|PROTECT_HFS_DEFAULT
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|protect_hfs
name|int
name|protect_hfs
init|=
name|PROTECT_HFS_DEFAULT
decl_stmt|;
end_decl_stmt

begin_ifndef
ifndef|#
directive|ifndef
name|PROTECT_NTFS_DEFAULT
end_ifndef

begin_define
DECL|macro|PROTECT_NTFS_DEFAULT
define|#
directive|define
name|PROTECT_NTFS_DEFAULT
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|protect_ntfs
name|int
name|protect_ntfs
init|=
name|PROTECT_NTFS_DEFAULT
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * The character that begins a commented line in user-editable file  * that is subject to stripspace.  */
end_comment

begin_decl_stmt
DECL|variable|comment_line_char
name|char
name|comment_line_char
init|=
literal|'#'
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|auto_comment_line_char
name|int
name|auto_comment_line_char
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Parallel index stat data preload? */
end_comment

begin_decl_stmt
DECL|variable|core_preload_index
name|int
name|core_preload_index
init|=
literal|1
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
name|char
modifier|*
name|work_tree
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|namespace
specifier|static
specifier|const
name|char
modifier|*
name|namespace
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|namespace_len
specifier|static
name|size_t
name|namespace_len
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_dir
DECL|variable|git_common_dir
specifier|static
specifier|const
name|char
modifier|*
name|git_dir
decl_stmt|,
modifier|*
name|git_common_dir
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_object_dir
DECL|variable|git_index_file
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
name|git_graft_file
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|git_db_env
DECL|variable|git_index_env
DECL|variable|git_graft_env
DECL|variable|git_common_dir_env
name|int
name|git_db_env
decl_stmt|,
name|git_index_env
decl_stmt|,
name|git_graft_env
decl_stmt|,
name|git_common_dir_env
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Repository-local GIT_* environment variables; see cache.h for details.  */
end_comment

begin_decl_stmt
DECL|variable|local_repo_env
specifier|const
name|char
modifier|*
specifier|const
name|local_repo_env
index|[]
init|=
block|{
name|ALTERNATE_DB_ENVIRONMENT
block|,
name|CONFIG_ENVIRONMENT
block|,
name|CONFIG_DATA_ENVIRONMENT
block|,
name|DB_ENVIRONMENT
block|,
name|GIT_DIR_ENVIRONMENT
block|,
name|GIT_WORK_TREE_ENVIRONMENT
block|,
name|GIT_IMPLICIT_WORK_TREE_ENVIRONMENT
block|,
name|GRAFT_ENVIRONMENT
block|,
name|INDEX_ENVIRONMENT
block|,
name|NO_REPLACE_OBJECTS_ENVIRONMENT
block|,
name|GIT_REPLACE_REF_BASE_ENVIRONMENT
block|,
name|GIT_PREFIX_ENVIRONMENT
block|,
name|GIT_SHALLOW_FILE_ENVIRONMENT
block|,
name|GIT_COMMON_DIR_ENVIRONMENT
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|expand_namespace
specifier|static
name|char
modifier|*
name|expand_namespace
parameter_list|(
specifier|const
name|char
modifier|*
name|raw_namespace
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
modifier|*
modifier|*
name|components
decl_stmt|,
modifier|*
modifier|*
name|c
decl_stmt|;
if|if
condition|(
operator|!
name|raw_namespace
operator|||
operator|!
operator|*
name|raw_namespace
condition|)
return|return
name|xstrdup
argument_list|(
literal|""
argument_list|)
return|;
name|strbuf_addstr
argument_list|(
operator|&
name|buf
argument_list|,
name|raw_namespace
argument_list|)
expr_stmt|;
name|components
operator|=
name|strbuf_split
argument_list|(
operator|&
name|buf
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
for|for
control|(
name|c
operator|=
name|components
init|;
operator|*
name|c
condition|;
name|c
operator|++
control|)
if|if
condition|(
name|strcmp
argument_list|(
operator|(
operator|*
name|c
operator|)
operator|->
name|buf
argument_list|,
literal|"/"
argument_list|)
operator|!=
literal|0
condition|)
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"refs/namespaces/%s"
argument_list|,
operator|(
operator|*
name|c
operator|)
operator|->
name|buf
argument_list|)
expr_stmt|;
name|strbuf_list_free
argument_list|(
name|components
argument_list|)
expr_stmt|;
if|if
condition|(
name|check_refname_format
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|0
argument_list|)
condition|)
name|die
argument_list|(
literal|"bad git namespace path \"%s\""
argument_list|,
name|raw_namespace
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
operator|&
name|buf
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
return|return
name|strbuf_detach
argument_list|(
operator|&
name|buf
argument_list|,
name|NULL
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|git_path_from_env
specifier|static
name|char
modifier|*
name|git_path_from_env
parameter_list|(
specifier|const
name|char
modifier|*
name|envvar
parameter_list|,
specifier|const
name|char
modifier|*
name|git_dir
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
modifier|*
name|fromenv
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|value
init|=
name|getenv
argument_list|(
name|envvar
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|value
condition|)
block|{
name|char
modifier|*
name|buf
init|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|git_dir
argument_list|)
operator|+
name|strlen
argument_list|(
name|path
argument_list|)
operator|+
literal|2
argument_list|)
decl_stmt|;
name|sprintf
argument_list|(
name|buf
argument_list|,
literal|"%s/%s"
argument_list|,
name|git_dir
argument_list|,
name|path
argument_list|)
expr_stmt|;
return|return
name|buf
return|;
block|}
if|if
condition|(
name|fromenv
condition|)
operator|*
name|fromenv
operator|=
literal|1
expr_stmt|;
return|return
name|xstrdup
argument_list|(
name|value
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|setup_git_env
specifier|static
name|void
name|setup_git_env
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
specifier|const
name|char
modifier|*
name|gitfile
decl_stmt|;
specifier|const
name|char
modifier|*
name|shallow_file
decl_stmt|;
specifier|const
name|char
modifier|*
name|replace_ref_base
decl_stmt|;
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
name|DEFAULT_GIT_DIR_ENVIRONMENT
expr_stmt|;
name|gitfile
operator|=
name|read_gitfile
argument_list|(
name|git_dir
argument_list|)
expr_stmt|;
name|git_dir
operator|=
name|xstrdup
argument_list|(
name|gitfile
condition|?
name|gitfile
else|:
name|git_dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_common_dir
argument_list|(
operator|&
name|sb
argument_list|,
name|git_dir
argument_list|)
condition|)
name|git_common_dir_env
operator|=
literal|1
expr_stmt|;
name|git_common_dir
operator|=
name|strbuf_detach
argument_list|(
operator|&
name|sb
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|git_object_dir
operator|=
name|git_path_from_env
argument_list|(
name|DB_ENVIRONMENT
argument_list|,
name|git_common_dir
argument_list|,
literal|"objects"
argument_list|,
operator|&
name|git_db_env
argument_list|)
expr_stmt|;
name|git_index_file
operator|=
name|git_path_from_env
argument_list|(
name|INDEX_ENVIRONMENT
argument_list|,
name|git_dir
argument_list|,
literal|"index"
argument_list|,
operator|&
name|git_index_env
argument_list|)
expr_stmt|;
name|git_graft_file
operator|=
name|git_path_from_env
argument_list|(
name|GRAFT_ENVIRONMENT
argument_list|,
name|git_common_dir
argument_list|,
literal|"info/grafts"
argument_list|,
operator|&
name|git_graft_env
argument_list|)
expr_stmt|;
if|if
condition|(
name|getenv
argument_list|(
name|NO_REPLACE_OBJECTS_ENVIRONMENT
argument_list|)
condition|)
name|check_replace_refs
operator|=
literal|0
expr_stmt|;
name|replace_ref_base
operator|=
name|getenv
argument_list|(
name|GIT_REPLACE_REF_BASE_ENVIRONMENT
argument_list|)
expr_stmt|;
name|git_replace_ref_base
operator|=
name|xstrdup
argument_list|(
name|replace_ref_base
condition|?
name|replace_ref_base
else|:
literal|"refs/replace/"
argument_list|)
expr_stmt|;
name|namespace
operator|=
name|expand_namespace
argument_list|(
name|getenv
argument_list|(
name|GIT_NAMESPACE_ENVIRONMENT
argument_list|)
argument_list|)
expr_stmt|;
name|namespace_len
operator|=
name|strlen
argument_list|(
name|namespace
argument_list|)
expr_stmt|;
name|shallow_file
operator|=
name|getenv
argument_list|(
name|GIT_SHALLOW_FILE_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
name|shallow_file
condition|)
name|set_alternate_shallow_file
argument_list|(
name|shallow_file
argument_list|,
literal|0
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
DECL|function|get_git_common_dir
specifier|const
name|char
modifier|*
name|get_git_common_dir
parameter_list|(
name|void
parameter_list|)
block|{
return|return
name|git_common_dir
return|;
block|}
end_function

begin_function
DECL|function|get_git_namespace
specifier|const
name|char
modifier|*
name|get_git_namespace
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|namespace
condition|)
name|setup_git_env
argument_list|()
expr_stmt|;
return|return
name|namespace
return|;
block|}
end_function

begin_function
DECL|function|strip_namespace
specifier|const
name|char
modifier|*
name|strip_namespace
parameter_list|(
specifier|const
name|char
modifier|*
name|namespaced_ref
parameter_list|)
block|{
if|if
condition|(
operator|!
name|starts_with
argument_list|(
name|namespaced_ref
argument_list|,
name|get_git_namespace
argument_list|()
argument_list|)
condition|)
return|return
name|NULL
return|;
return|return
name|namespaced_ref
operator|+
name|namespace_len
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|git_work_tree_initialized
specifier|static
name|int
name|git_work_tree_initialized
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Note.  This works only before you used a work tree.  This was added  * primarily to support git-clone to work in a new repository it just  * created, and is not meant to flip between different work trees.  */
end_comment

begin_function
DECL|function|set_git_work_tree
name|void
name|set_git_work_tree
parameter_list|(
specifier|const
name|char
modifier|*
name|new_work_tree
parameter_list|)
block|{
if|if
condition|(
name|git_work_tree_initialized
condition|)
block|{
name|new_work_tree
operator|=
name|real_path
argument_list|(
name|new_work_tree
argument_list|)
expr_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|new_work_tree
argument_list|,
name|work_tree
argument_list|)
condition|)
name|die
argument_list|(
literal|"internal error: work tree has already been set\n"
literal|"Current worktree: %s\nNew worktree: %s"
argument_list|,
name|work_tree
argument_list|,
name|new_work_tree
argument_list|)
expr_stmt|;
return|return;
block|}
name|git_work_tree_initialized
operator|=
literal|1
expr_stmt|;
name|work_tree
operator|=
name|xstrdup
argument_list|(
name|real_path
argument_list|(
name|new_work_tree
argument_list|)
argument_list|)
expr_stmt|;
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
DECL|function|odb_mkstemp
name|int
name|odb_mkstemp
parameter_list|(
name|char
modifier|*
name|template
parameter_list|,
name|size_t
name|limit
parameter_list|,
specifier|const
name|char
modifier|*
name|pattern
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
comment|/* 	 * we let the umask do its job, don't try to be more 	 * restrictive except to remove write permission. 	 */
name|int
name|mode
init|=
literal|0444
decl_stmt|;
name|snprintf
argument_list|(
name|template
argument_list|,
name|limit
argument_list|,
literal|"%s/%s"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|,
name|pattern
argument_list|)
expr_stmt|;
name|fd
operator|=
name|git_mkstemp_mode
argument_list|(
name|template
argument_list|,
name|mode
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|fd
condition|)
return|return
name|fd
return|;
comment|/* slow path */
comment|/* some mkstemp implementations erase template on failure */
name|snprintf
argument_list|(
name|template
argument_list|,
name|limit
argument_list|,
literal|"%s/%s"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|,
name|pattern
argument_list|)
expr_stmt|;
name|safe_create_leading_directories
argument_list|(
name|template
argument_list|)
expr_stmt|;
return|return
name|xmkstemp_mode
argument_list|(
name|template
argument_list|,
name|mode
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|odb_pack_keep
name|int
name|odb_pack_keep
parameter_list|(
name|char
modifier|*
name|name
parameter_list|,
name|size_t
name|namesz
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|snprintf
argument_list|(
name|name
argument_list|,
name|namesz
argument_list|,
literal|"%s/pack/pack-%s.keep"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|name
argument_list|,
name|O_RDWR
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0600
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|fd
condition|)
return|return
name|fd
return|;
comment|/* slow path */
name|safe_create_leading_directories
argument_list|(
name|name
argument_list|)
expr_stmt|;
return|return
name|open
argument_list|(
name|name
argument_list|,
name|O_RDWR
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0600
argument_list|)
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

begin_function
DECL|function|get_log_output_encoding
specifier|const
name|char
modifier|*
name|get_log_output_encoding
parameter_list|(
name|void
parameter_list|)
block|{
return|return
name|git_log_output_encoding
condition|?
name|git_log_output_encoding
else|:
name|get_commit_output_encoding
argument_list|()
return|;
block|}
end_function

begin_function
DECL|function|get_commit_output_encoding
specifier|const
name|char
modifier|*
name|get_commit_output_encoding
parameter_list|(
name|void
parameter_list|)
block|{
return|return
name|git_commit_encoding
condition|?
name|git_commit_encoding
else|:
literal|"UTF-8"
return|;
block|}
end_function

end_unit

