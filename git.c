begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_decl_stmt
DECL|variable|git_usage_string
specifier|const
name|char
name|git_usage_string
index|[]
init|=
literal|"git [--version] [--exec-path[=GIT_EXEC_PATH]] [-p|--paginate] [--bare] [--git-dir=GIT_DIR] [--help] COMMAND [ARGS]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|prepend_to_path
specifier|static
name|void
name|prepend_to_path
parameter_list|(
specifier|const
name|char
modifier|*
name|dir
parameter_list|,
name|int
name|len
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|old_path
init|=
name|getenv
argument_list|(
literal|"PATH"
argument_list|)
decl_stmt|;
name|char
modifier|*
name|path
decl_stmt|;
name|int
name|path_len
init|=
name|len
decl_stmt|;
if|if
condition|(
operator|!
name|old_path
condition|)
name|old_path
operator|=
literal|"/usr/local/bin:/usr/bin:/bin"
expr_stmt|;
name|path_len
operator|=
name|len
operator|+
name|strlen
argument_list|(
name|old_path
argument_list|)
operator|+
literal|1
expr_stmt|;
name|path
operator|=
name|xmalloc
argument_list|(
name|path_len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|path
argument_list|,
name|dir
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|path
index|[
name|len
index|]
operator|=
literal|':'
expr_stmt|;
name|memcpy
argument_list|(
name|path
operator|+
name|len
operator|+
literal|1
argument_list|,
name|old_path
argument_list|,
name|path_len
operator|-
name|len
argument_list|)
expr_stmt|;
name|setenv
argument_list|(
literal|"PATH"
argument_list|,
name|path
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|handle_options
specifier|static
name|int
name|handle_options
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
modifier|*
name|argv
parameter_list|,
name|int
modifier|*
name|argc
parameter_list|)
block|{
name|int
name|handled
init|=
literal|0
decl_stmt|;
while|while
condition|(
operator|*
name|argc
operator|>
literal|0
condition|)
block|{
specifier|const
name|char
modifier|*
name|cmd
init|=
operator|(
operator|*
name|argv
operator|)
index|[
literal|0
index|]
decl_stmt|;
if|if
condition|(
name|cmd
index|[
literal|0
index|]
operator|!=
literal|'-'
condition|)
break|break;
comment|/* 		 * For legacy reasons, the "version" and "help" 		 * commands can be written with "--" prepended 		 * to make them look like flags. 		 */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"--help"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"--version"
argument_list|)
condition|)
break|break;
comment|/* 		 * Check remaining flags. 		 */
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|cmd
argument_list|,
literal|"--exec-path"
argument_list|,
literal|11
argument_list|)
condition|)
block|{
name|cmd
operator|+=
literal|11
expr_stmt|;
if|if
condition|(
operator|*
name|cmd
operator|==
literal|'='
condition|)
name|git_set_exec_path
argument_list|(
name|cmd
operator|+
literal|1
argument_list|)
expr_stmt|;
else|else
block|{
name|puts
argument_list|(
name|git_exec_path
argument_list|()
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"-p"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"--paginate"
argument_list|)
condition|)
block|{
name|setup_pager
argument_list|()
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"--git-dir"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|*
name|argc
operator|<
literal|2
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"No directory given for --git-dir.\n"
argument_list|)
expr_stmt|;
name|usage
argument_list|(
name|git_usage_string
argument_list|)
expr_stmt|;
block|}
name|setenv
argument_list|(
literal|"GIT_DIR"
argument_list|,
operator|(
operator|*
name|argv
operator|)
index|[
literal|1
index|]
argument_list|,
literal|1
argument_list|)
expr_stmt|;
operator|(
operator|*
name|argv
operator|)
operator|++
expr_stmt|;
operator|(
operator|*
name|argc
operator|)
operator|--
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|cmd
argument_list|,
literal|"--git-dir="
argument_list|,
literal|10
argument_list|)
condition|)
block|{
name|setenv
argument_list|(
literal|"GIT_DIR"
argument_list|,
name|cmd
operator|+
literal|10
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"--bare"
argument_list|)
condition|)
block|{
specifier|static
name|char
name|git_dir
index|[
name|PATH_MAX
operator|+
literal|1
index|]
decl_stmt|;
name|setenv
argument_list|(
literal|"GIT_DIR"
argument_list|,
name|getcwd
argument_list|(
name|git_dir
argument_list|,
sizeof|sizeof
argument_list|(
name|git_dir
argument_list|)
argument_list|)
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Unknown option: %s\n"
argument_list|,
name|cmd
argument_list|)
expr_stmt|;
name|usage
argument_list|(
name|git_usage_string
argument_list|)
expr_stmt|;
block|}
operator|(
operator|*
name|argv
operator|)
operator|++
expr_stmt|;
operator|(
operator|*
name|argc
operator|)
operator|--
expr_stmt|;
name|handled
operator|++
expr_stmt|;
block|}
return|return
name|handled
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|alias_command
specifier|static
specifier|const
name|char
modifier|*
name|alias_command
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|alias_string
specifier|static
name|char
modifier|*
name|alias_string
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|git_alias_config
specifier|static
name|int
name|git_alias_config
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
operator|!
name|strncmp
argument_list|(
name|var
argument_list|,
literal|"alias."
argument_list|,
literal|6
argument_list|)
operator|&&
operator|!
name|strcmp
argument_list|(
name|var
operator|+
literal|6
argument_list|,
name|alias_command
argument_list|)
condition|)
block|{
name|alias_string
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|split_cmdline
specifier|static
name|int
name|split_cmdline
parameter_list|(
name|char
modifier|*
name|cmdline
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|int
name|src
decl_stmt|,
name|dst
decl_stmt|,
name|count
init|=
literal|0
decl_stmt|,
name|size
init|=
literal|16
decl_stmt|;
name|char
name|quoted
init|=
literal|0
decl_stmt|;
operator|*
name|argv
operator|=
name|malloc
argument_list|(
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
operator|*
name|size
argument_list|)
expr_stmt|;
comment|/* split alias_string */
operator|(
operator|*
name|argv
operator|)
index|[
name|count
operator|++
index|]
operator|=
name|cmdline
expr_stmt|;
for|for
control|(
name|src
operator|=
name|dst
operator|=
literal|0
init|;
name|cmdline
index|[
name|src
index|]
condition|;
control|)
block|{
name|char
name|c
init|=
name|cmdline
index|[
name|src
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|quoted
operator|&&
name|isspace
argument_list|(
name|c
argument_list|)
condition|)
block|{
name|cmdline
index|[
name|dst
operator|++
index|]
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|cmdline
index|[
operator|++
name|src
index|]
operator|&&
name|isspace
argument_list|(
name|cmdline
index|[
name|src
index|]
argument_list|)
condition|)
empty_stmt|;
comment|/* skip */
if|if
condition|(
name|count
operator|>=
name|size
condition|)
block|{
name|size
operator|+=
literal|16
expr_stmt|;
operator|*
name|argv
operator|=
name|xrealloc
argument_list|(
operator|*
name|argv
argument_list|,
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
operator|*
name|size
argument_list|)
expr_stmt|;
block|}
operator|(
operator|*
name|argv
operator|)
index|[
name|count
operator|++
index|]
operator|=
name|cmdline
operator|+
name|dst
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|quoted
operator|&&
operator|(
name|c
operator|==
literal|'\''
operator|||
name|c
operator|==
literal|'"'
operator|)
condition|)
block|{
name|quoted
operator|=
name|c
expr_stmt|;
name|src
operator|++
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|c
operator|==
name|quoted
condition|)
block|{
name|quoted
operator|=
literal|0
expr_stmt|;
name|src
operator|++
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|c
operator|==
literal|'\\'
operator|&&
name|quoted
operator|!=
literal|'\''
condition|)
block|{
name|src
operator|++
expr_stmt|;
name|c
operator|=
name|cmdline
index|[
name|src
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|c
condition|)
block|{
name|free
argument_list|(
operator|*
name|argv
argument_list|)
expr_stmt|;
operator|*
name|argv
operator|=
name|NULL
expr_stmt|;
return|return
name|error
argument_list|(
literal|"cmdline ends with \\"
argument_list|)
return|;
block|}
block|}
name|cmdline
index|[
name|dst
operator|++
index|]
operator|=
name|c
expr_stmt|;
name|src
operator|++
expr_stmt|;
block|}
block|}
name|cmdline
index|[
name|dst
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|quoted
condition|)
block|{
name|free
argument_list|(
operator|*
name|argv
argument_list|)
expr_stmt|;
operator|*
name|argv
operator|=
name|NULL
expr_stmt|;
return|return
name|error
argument_list|(
literal|"unclosed quote"
argument_list|)
return|;
block|}
return|return
name|count
return|;
block|}
end_function

begin_function
DECL|function|handle_alias
specifier|static
name|int
name|handle_alias
parameter_list|(
name|int
modifier|*
name|argcp
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|int
name|nongit
init|=
literal|0
decl_stmt|,
name|ret
init|=
literal|0
decl_stmt|,
name|saved_errno
init|=
name|errno
decl_stmt|;
specifier|const
name|char
modifier|*
name|subdir
decl_stmt|;
name|int
name|count
decl_stmt|,
name|option_count
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|new_argv
decl_stmt|;
name|subdir
operator|=
name|setup_git_directory_gently
argument_list|(
operator|&
name|nongit
argument_list|)
expr_stmt|;
name|alias_command
operator|=
operator|(
operator|*
name|argv
operator|)
index|[
literal|0
index|]
expr_stmt|;
name|git_config
argument_list|(
name|git_alias_config
argument_list|)
expr_stmt|;
if|if
condition|(
name|alias_string
condition|)
block|{
name|count
operator|=
name|split_cmdline
argument_list|(
name|alias_string
argument_list|,
operator|&
name|new_argv
argument_list|)
expr_stmt|;
name|option_count
operator|=
name|handle_options
argument_list|(
operator|&
name|new_argv
argument_list|,
operator|&
name|count
argument_list|)
expr_stmt|;
name|memmove
argument_list|(
name|new_argv
operator|-
name|option_count
argument_list|,
name|new_argv
argument_list|,
name|count
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|new_argv
operator|-=
name|option_count
expr_stmt|;
if|if
condition|(
name|count
operator|<
literal|1
condition|)
name|die
argument_list|(
literal|"empty alias for %s"
argument_list|,
name|alias_command
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|alias_command
argument_list|,
name|new_argv
index|[
literal|0
index|]
argument_list|)
condition|)
name|die
argument_list|(
literal|"recursive alias: %s"
argument_list|,
name|alias_command
argument_list|)
expr_stmt|;
name|trace_argv_printf
argument_list|(
name|new_argv
argument_list|,
name|count
argument_list|,
literal|"trace: alias expansion: %s =>"
argument_list|,
name|alias_command
argument_list|)
expr_stmt|;
name|new_argv
operator|=
name|xrealloc
argument_list|(
name|new_argv
argument_list|,
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
operator|*
operator|(
name|count
operator|+
operator|*
name|argcp
operator|+
literal|1
operator|)
argument_list|)
expr_stmt|;
comment|/* insert after command name */
name|memcpy
argument_list|(
name|new_argv
operator|+
name|count
argument_list|,
operator|*
name|argv
operator|+
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
operator|*
operator|*
name|argcp
argument_list|)
expr_stmt|;
name|new_argv
index|[
name|count
operator|+
operator|*
name|argcp
index|]
operator|=
name|NULL
expr_stmt|;
operator|*
name|argv
operator|=
name|new_argv
expr_stmt|;
operator|*
name|argcp
operator|+=
name|count
operator|-
literal|1
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|subdir
condition|)
name|chdir
argument_list|(
name|subdir
argument_list|)
expr_stmt|;
name|errno
operator|=
name|saved_errno
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|git_version_string
specifier|const
name|char
name|git_version_string
index|[]
init|=
name|GIT_VERSION
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|RUN_SETUP
define|#
directive|define
name|RUN_SETUP
value|(1<<0)
end_define

begin_define
DECL|macro|USE_PAGER
define|#
directive|define
name|USE_PAGER
value|(1<<1)
end_define

begin_function
DECL|function|handle_internal_command
specifier|static
name|void
name|handle_internal_command
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
name|char
modifier|*
modifier|*
name|envp
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|cmd
init|=
name|argv
index|[
literal|0
index|]
decl_stmt|;
specifier|static
struct|struct
name|cmd_struct
block|{
specifier|const
name|char
modifier|*
name|cmd
decl_stmt|;
name|int
function_decl|(
modifier|*
name|fn
function_decl|)
parameter_list|(
name|int
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
name|int
name|option
decl_stmt|;
block|}
name|commands
index|[]
init|=
block|{
block|{
literal|"add"
block|,
name|cmd_add
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"annotate"
block|,
name|cmd_annotate
block|, }
block|,
block|{
literal|"apply"
block|,
name|cmd_apply
block|}
block|,
block|{
literal|"archive"
block|,
name|cmd_archive
block|}
block|,
block|{
literal|"blame"
block|,
name|cmd_blame
block|,
name|RUN_SETUP
operator||
name|USE_PAGER
block|}
block|,
block|{
literal|"branch"
block|,
name|cmd_branch
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"cat-file"
block|,
name|cmd_cat_file
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"checkout-index"
block|,
name|cmd_checkout_index
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"check-ref-format"
block|,
name|cmd_check_ref_format
block|}
block|,
block|{
literal|"cherry"
block|,
name|cmd_cherry
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"commit-tree"
block|,
name|cmd_commit_tree
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"count-objects"
block|,
name|cmd_count_objects
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"diff"
block|,
name|cmd_diff
block|,
name|RUN_SETUP
operator||
name|USE_PAGER
block|}
block|,
block|{
literal|"diff-files"
block|,
name|cmd_diff_files
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"diff-index"
block|,
name|cmd_diff_index
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"diff-stages"
block|,
name|cmd_diff_stages
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"diff-tree"
block|,
name|cmd_diff_tree
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"fmt-merge-msg"
block|,
name|cmd_fmt_merge_msg
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"for-each-ref"
block|,
name|cmd_for_each_ref
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"format-patch"
block|,
name|cmd_format_patch
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"get-tar-commit-id"
block|,
name|cmd_get_tar_commit_id
block|}
block|,
block|{
literal|"grep"
block|,
name|cmd_grep
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"help"
block|,
name|cmd_help
block|}
block|,
block|{
literal|"init-db"
block|,
name|cmd_init_db
block|}
block|,
block|{
literal|"log"
block|,
name|cmd_log
block|,
name|RUN_SETUP
operator||
name|USE_PAGER
block|}
block|,
block|{
literal|"ls-files"
block|,
name|cmd_ls_files
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"ls-tree"
block|,
name|cmd_ls_tree
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"mailinfo"
block|,
name|cmd_mailinfo
block|}
block|,
block|{
literal|"mailsplit"
block|,
name|cmd_mailsplit
block|}
block|,
block|{
literal|"merge-file"
block|,
name|cmd_merge_file
block|}
block|,
block|{
literal|"mv"
block|,
name|cmd_mv
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"name-rev"
block|,
name|cmd_name_rev
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"pack-objects"
block|,
name|cmd_pack_objects
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"pickaxe"
block|,
name|cmd_blame
block|,
name|RUN_SETUP
operator||
name|USE_PAGER
block|}
block|,
block|{
literal|"prune"
block|,
name|cmd_prune
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"prune-packed"
block|,
name|cmd_prune_packed
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"push"
block|,
name|cmd_push
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"read-tree"
block|,
name|cmd_read_tree
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"reflog"
block|,
name|cmd_reflog
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"repo-config"
block|,
name|cmd_repo_config
block|}
block|,
block|{
literal|"rerere"
block|,
name|cmd_rerere
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"rev-list"
block|,
name|cmd_rev_list
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"rev-parse"
block|,
name|cmd_rev_parse
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"rm"
block|,
name|cmd_rm
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"runstatus"
block|,
name|cmd_runstatus
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"shortlog"
block|,
name|cmd_shortlog
block|,
name|RUN_SETUP
operator||
name|USE_PAGER
block|}
block|,
block|{
literal|"show-branch"
block|,
name|cmd_show_branch
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"show"
block|,
name|cmd_show
block|,
name|RUN_SETUP
operator||
name|USE_PAGER
block|}
block|,
block|{
literal|"stripspace"
block|,
name|cmd_stripspace
block|}
block|,
block|{
literal|"symbolic-ref"
block|,
name|cmd_symbolic_ref
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"tar-tree"
block|,
name|cmd_tar_tree
block|}
block|,
block|{
literal|"unpack-objects"
block|,
name|cmd_unpack_objects
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"update-index"
block|,
name|cmd_update_index
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"update-ref"
block|,
name|cmd_update_ref
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"upload-archive"
block|,
name|cmd_upload_archive
block|}
block|,
block|{
literal|"version"
block|,
name|cmd_version
block|}
block|,
block|{
literal|"whatchanged"
block|,
name|cmd_whatchanged
block|,
name|RUN_SETUP
operator||
name|USE_PAGER
block|}
block|,
block|{
literal|"write-tree"
block|,
name|cmd_write_tree
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"verify-pack"
block|,
name|cmd_verify_pack
block|}
block|,
block|{
literal|"show-ref"
block|,
name|cmd_show_ref
block|,
name|RUN_SETUP
block|}
block|,
block|{
literal|"pack-refs"
block|,
name|cmd_pack_refs
block|,
name|RUN_SETUP
block|}
block|, 	}
struct|;
name|int
name|i
decl_stmt|;
comment|/* Turn "git cmd --help" into "git help cmd" */
if|if
condition|(
name|argc
operator|>
literal|1
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--help"
argument_list|)
condition|)
block|{
name|argv
index|[
literal|1
index|]
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
name|argv
index|[
literal|0
index|]
operator|=
name|cmd
operator|=
literal|"help"
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
name|ARRAY_SIZE
argument_list|(
name|commands
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|cmd_struct
modifier|*
name|p
init|=
name|commands
operator|+
name|i
decl_stmt|;
specifier|const
name|char
modifier|*
name|prefix
decl_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|p
operator|->
name|cmd
argument_list|,
name|cmd
argument_list|)
condition|)
continue|continue;
name|prefix
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|p
operator|->
name|option
operator|&
name|RUN_SETUP
condition|)
name|prefix
operator|=
name|setup_git_directory
argument_list|()
expr_stmt|;
if|if
condition|(
name|p
operator|->
name|option
operator|&
name|USE_PAGER
condition|)
name|setup_pager
argument_list|()
expr_stmt|;
name|trace_argv_printf
argument_list|(
name|argv
argument_list|,
name|argc
argument_list|,
literal|"trace: built-in: git"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
name|p
operator|->
name|fn
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
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
name|char
modifier|*
modifier|*
name|envp
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|cmd
init|=
name|argv
index|[
literal|0
index|]
condition|?
name|argv
index|[
literal|0
index|]
else|:
literal|"git-help"
decl_stmt|;
name|char
modifier|*
name|slash
init|=
name|strrchr
argument_list|(
name|cmd
argument_list|,
literal|'/'
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|exec_path
init|=
name|NULL
decl_stmt|;
name|int
name|done_alias
init|=
literal|0
decl_stmt|;
comment|/* 	 * Take the basename of argv[0] as the command 	 * name, and the dirname as the default exec_path 	 * if it's an absolute path and we don't have 	 * anything better. 	 */
if|if
condition|(
name|slash
condition|)
block|{
operator|*
name|slash
operator|++
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|*
name|cmd
operator|==
literal|'/'
condition|)
name|exec_path
operator|=
name|cmd
expr_stmt|;
name|cmd
operator|=
name|slash
expr_stmt|;
block|}
comment|/* 	 * "git-xxxx" is the same as "git xxxx", but we obviously: 	 * 	 *  - cannot take flags in between the "git" and the "xxxx". 	 *  - cannot execute it externally (since it would just do 	 *    the same thing over again) 	 * 	 * So we just directly call the internal command handler, and 	 * die if that one cannot handle it. 	 */
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|cmd
argument_list|,
literal|"git-"
argument_list|,
literal|4
argument_list|)
condition|)
block|{
name|cmd
operator|+=
literal|4
expr_stmt|;
name|argv
index|[
literal|0
index|]
operator|=
name|cmd
expr_stmt|;
name|handle_internal_command
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|envp
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"cannot handle %s internally"
argument_list|,
name|cmd
argument_list|)
expr_stmt|;
block|}
comment|/* Look for flags.. */
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|handle_options
argument_list|(
operator|&
name|argv
argument_list|,
operator|&
name|argc
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|>
literal|0
condition|)
block|{
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
literal|"--"
argument_list|,
literal|2
argument_list|)
condition|)
name|argv
index|[
literal|0
index|]
operator|+=
literal|2
expr_stmt|;
block|}
else|else
block|{
comment|/* Default command: "help" */
name|argv
index|[
literal|0
index|]
operator|=
literal|"help"
expr_stmt|;
name|argc
operator|=
literal|1
expr_stmt|;
block|}
name|cmd
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
comment|/* 	 * We search for git commands in the following order: 	 *  - git_exec_path() 	 *  - the path of the "git" command if we could find it 	 *    in $0 	 *  - the regular PATH. 	 */
if|if
condition|(
name|exec_path
condition|)
name|prepend_to_path
argument_list|(
name|exec_path
argument_list|,
name|strlen
argument_list|(
name|exec_path
argument_list|)
argument_list|)
expr_stmt|;
name|exec_path
operator|=
name|git_exec_path
argument_list|()
expr_stmt|;
name|prepend_to_path
argument_list|(
name|exec_path
argument_list|,
name|strlen
argument_list|(
name|exec_path
argument_list|)
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
comment|/* See if it's an internal command */
name|handle_internal_command
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|envp
argument_list|)
expr_stmt|;
comment|/* .. then try the external ones */
name|execv_git_cmd
argument_list|(
name|argv
argument_list|)
expr_stmt|;
comment|/* It could be an alias -- this works around the insanity 		 * of overriding "git log" with "git show" by having 		 * alias.log = show 		 */
if|if
condition|(
name|done_alias
operator|||
operator|!
name|handle_alias
argument_list|(
operator|&
name|argc
argument_list|,
operator|&
name|argv
argument_list|)
condition|)
break|break;
name|done_alias
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|errno
operator|==
name|ENOENT
condition|)
name|help_unknown_cmd
argument_list|(
name|cmd
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Failed to run command '%s': %s\n"
argument_list|,
name|cmd
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

end_unit

