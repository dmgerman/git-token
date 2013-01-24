begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * "git clean" builtin command  *  * Copyright (C) 2007 Shawn Bohrer  *  * Based on git-clean.sh by Pavel Roskin  */
end_comment

begin_include
include|#
directive|include
file|"builtin.h"
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

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_decl_stmt
DECL|variable|force
specifier|static
name|int
name|force
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|force
comment|/* unset */
end_comment

begin_decl_stmt
DECL|variable|builtin_clean_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|builtin_clean_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git clean [-d] [-f] [-n] [-q] [-e<pattern>] [-x | -X] [--]<paths>..."
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|msg_remove
specifier|static
specifier|const
name|char
modifier|*
name|msg_remove
init|=
name|N_
argument_list|(
literal|"Removing %s\n"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|msg_would_remove
specifier|static
specifier|const
name|char
modifier|*
name|msg_would_remove
init|=
name|N_
argument_list|(
literal|"Would remove %s\n"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|msg_skip_git_dir
specifier|static
specifier|const
name|char
modifier|*
name|msg_skip_git_dir
init|=
name|N_
argument_list|(
literal|"Skipping repository %s\n"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|msg_would_skip_git_dir
specifier|static
specifier|const
name|char
modifier|*
name|msg_would_skip_git_dir
init|=
name|N_
argument_list|(
literal|"Would skip repository %s\n"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|msg_warn_remove_failed
specifier|static
specifier|const
name|char
modifier|*
name|msg_warn_remove_failed
init|=
name|N_
argument_list|(
literal|"failed to remove %s"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|git_clean_config
specifier|static
name|int
name|git_clean_config
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
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"clean.requireforce"
argument_list|)
condition|)
name|force
operator|=
operator|!
name|git_config_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
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
DECL|function|exclude_cb
specifier|static
name|int
name|exclude_cb
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|unset
parameter_list|)
block|{
name|struct
name|string_list
modifier|*
name|exclude_list
init|=
name|opt
operator|->
name|value
decl_stmt|;
name|string_list_append
argument_list|(
name|exclude_list
argument_list|,
name|arg
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|remove_dirs
specifier|static
name|int
name|remove_dirs
parameter_list|(
name|struct
name|strbuf
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|int
name|force_flag
parameter_list|,
name|int
name|dry_run
parameter_list|,
name|int
name|quiet
parameter_list|,
name|int
modifier|*
name|dir_gone
parameter_list|)
block|{
name|DIR
modifier|*
name|dir
decl_stmt|;
name|struct
name|strbuf
name|quoted
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|dirent
modifier|*
name|e
decl_stmt|;
name|int
name|res
init|=
literal|0
decl_stmt|,
name|ret
init|=
literal|0
decl_stmt|,
name|gone
init|=
literal|1
decl_stmt|,
name|original_len
init|=
name|path
operator|->
name|len
decl_stmt|,
name|len
decl_stmt|,
name|i
decl_stmt|;
name|unsigned
name|char
name|submodule_head
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|string_list
name|dels
init|=
name|STRING_LIST_INIT_DUP
decl_stmt|;
operator|*
name|dir_gone
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|(
name|force_flag
operator|&
name|REMOVE_DIR_KEEP_NESTED_GIT
operator|)
operator|&&
operator|!
name|resolve_gitlink_ref
argument_list|(
name|path
operator|->
name|buf
argument_list|,
literal|"HEAD"
argument_list|,
name|submodule_head
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|quiet
condition|)
block|{
name|quote_path_relative
argument_list|(
name|path
operator|->
name|buf
argument_list|,
name|strlen
argument_list|(
name|path
operator|->
name|buf
argument_list|)
argument_list|,
operator|&
name|quoted
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|printf
argument_list|(
name|dry_run
condition|?
name|_
argument_list|(
name|msg_would_skip_git_dir
argument_list|)
else|:
name|_
argument_list|(
name|msg_skip_git_dir
argument_list|)
argument_list|,
name|quoted
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
operator|*
name|dir_gone
operator|=
literal|0
expr_stmt|;
return|return
literal|0
return|;
block|}
name|dir
operator|=
name|opendir
argument_list|(
name|path
operator|->
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dir
condition|)
block|{
comment|/* an empty dir could be removed even if it is unreadble */
name|res
operator|=
name|dry_run
condition|?
literal|0
else|:
name|rmdir
argument_list|(
name|path
operator|->
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|res
condition|)
block|{
name|quote_path_relative
argument_list|(
name|path
operator|->
name|buf
argument_list|,
name|strlen
argument_list|(
name|path
operator|->
name|buf
argument_list|)
argument_list|,
operator|&
name|quoted
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|warning
argument_list|(
name|_
argument_list|(
name|msg_warn_remove_failed
argument_list|)
argument_list|,
name|quoted
operator|.
name|buf
argument_list|)
expr_stmt|;
operator|*
name|dir_gone
operator|=
literal|0
expr_stmt|;
block|}
return|return
name|res
return|;
block|}
if|if
condition|(
name|path
operator|->
name|buf
index|[
name|original_len
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
name|strbuf_addch
argument_list|(
name|path
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
name|len
operator|=
name|path
operator|->
name|len
expr_stmt|;
while|while
condition|(
operator|(
name|e
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
name|st
decl_stmt|;
if|if
condition|(
name|is_dot_or_dotdot
argument_list|(
name|e
operator|->
name|d_name
argument_list|)
condition|)
continue|continue;
name|strbuf_setlen
argument_list|(
name|path
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
name|path
argument_list|,
name|e
operator|->
name|d_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|path
operator|->
name|buf
argument_list|,
operator|&
name|st
argument_list|)
condition|)
empty_stmt|;
comment|/* fall thru */
elseif|else
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
if|if
condition|(
name|remove_dirs
argument_list|(
name|path
argument_list|,
name|prefix
argument_list|,
name|force_flag
argument_list|,
name|dry_run
argument_list|,
name|quiet
argument_list|,
operator|&
name|gone
argument_list|)
condition|)
name|ret
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|gone
condition|)
block|{
name|quote_path_relative
argument_list|(
name|path
operator|->
name|buf
argument_list|,
name|strlen
argument_list|(
name|path
operator|->
name|buf
argument_list|)
argument_list|,
operator|&
name|quoted
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|string_list_append
argument_list|(
operator|&
name|dels
argument_list|,
name|quoted
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
else|else
operator|*
name|dir_gone
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
else|else
block|{
name|res
operator|=
name|dry_run
condition|?
literal|0
else|:
name|unlink
argument_list|(
name|path
operator|->
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|res
condition|)
block|{
name|quote_path_relative
argument_list|(
name|path
operator|->
name|buf
argument_list|,
name|strlen
argument_list|(
name|path
operator|->
name|buf
argument_list|)
argument_list|,
operator|&
name|quoted
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|string_list_append
argument_list|(
operator|&
name|dels
argument_list|,
name|quoted
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|quote_path_relative
argument_list|(
name|path
operator|->
name|buf
argument_list|,
name|strlen
argument_list|(
name|path
operator|->
name|buf
argument_list|)
argument_list|,
operator|&
name|quoted
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|warning
argument_list|(
name|_
argument_list|(
name|msg_warn_remove_failed
argument_list|)
argument_list|,
name|quoted
operator|.
name|buf
argument_list|)
expr_stmt|;
operator|*
name|dir_gone
operator|=
literal|0
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
block|}
continue|continue;
block|}
comment|/* path too long, stat fails, or non-directory still exists */
operator|*
name|dir_gone
operator|=
literal|0
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
break|break;
block|}
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
name|strbuf_setlen
argument_list|(
name|path
argument_list|,
name|original_len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|dir_gone
condition|)
block|{
name|res
operator|=
name|dry_run
condition|?
literal|0
else|:
name|rmdir
argument_list|(
name|path
operator|->
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|res
condition|)
operator|*
name|dir_gone
operator|=
literal|1
expr_stmt|;
else|else
block|{
name|quote_path_relative
argument_list|(
name|path
operator|->
name|buf
argument_list|,
name|strlen
argument_list|(
name|path
operator|->
name|buf
argument_list|)
argument_list|,
operator|&
name|quoted
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|warning
argument_list|(
name|_
argument_list|(
name|msg_warn_remove_failed
argument_list|)
argument_list|,
name|quoted
operator|.
name|buf
argument_list|)
expr_stmt|;
operator|*
name|dir_gone
operator|=
literal|0
expr_stmt|;
name|ret
operator|=
literal|1
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
operator|*
name|dir_gone
operator|&&
operator|!
name|quiet
condition|)
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|dels
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|printf
argument_list|(
name|dry_run
condition|?
name|_
argument_list|(
name|msg_would_remove
argument_list|)
else|:
name|_
argument_list|(
name|msg_remove
argument_list|)
argument_list|,
name|dels
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
block|}
name|string_list_clear
argument_list|(
operator|&
name|dels
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|cmd_clean
name|int
name|cmd_clean
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
name|res
decl_stmt|;
name|int
name|dry_run
init|=
literal|0
decl_stmt|,
name|remove_directories
init|=
literal|0
decl_stmt|,
name|quiet
init|=
literal|0
decl_stmt|,
name|ignored
init|=
literal|0
decl_stmt|;
name|int
name|ignored_only
init|=
literal|0
decl_stmt|,
name|config_set
init|=
literal|0
decl_stmt|,
name|errors
init|=
literal|0
decl_stmt|,
name|gone
init|=
literal|1
decl_stmt|;
name|int
name|rm_flags
init|=
name|REMOVE_DIR_KEEP_NESTED_GIT
decl_stmt|;
name|struct
name|strbuf
name|directory
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|dir_struct
name|dir
decl_stmt|;
specifier|static
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
decl_stmt|;
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|string_list
name|exclude_list
init|=
name|STRING_LIST_INIT_NODUP
decl_stmt|;
name|struct
name|exclude_list
modifier|*
name|el
decl_stmt|;
specifier|const
name|char
modifier|*
name|qname
decl_stmt|;
name|char
modifier|*
name|seen
init|=
name|NULL
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT__QUIET
argument_list|(
operator|&
name|quiet
argument_list|,
name|N_
argument_list|(
literal|"do not print names of files removed"
argument_list|)
argument_list|)
block|,
name|OPT__DRY_RUN
argument_list|(
operator|&
name|dry_run
argument_list|,
name|N_
argument_list|(
literal|"dry run"
argument_list|)
argument_list|)
block|,
name|OPT__FORCE
argument_list|(
operator|&
name|force
argument_list|,
name|N_
argument_list|(
literal|"force"
argument_list|)
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'d'
argument_list|,
name|NULL
argument_list|,
operator|&
name|remove_directories
argument_list|,
name|N_
argument_list|(
literal|"remove whole directories"
argument_list|)
argument_list|)
block|,
block|{
name|OPTION_CALLBACK
block|,
literal|'e'
block|,
literal|"exclude"
block|,
operator|&
name|exclude_list
block|,
name|N_
argument_list|(
literal|"pattern"
argument_list|)
block|,
name|N_
argument_list|(
literal|"add<pattern> to ignore rules"
argument_list|)
block|,
name|PARSE_OPT_NONEG
block|,
name|exclude_cb
block|}
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'x'
argument_list|,
name|NULL
argument_list|,
operator|&
name|ignored
argument_list|,
name|N_
argument_list|(
literal|"remove ignored files, too"
argument_list|)
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'X'
argument_list|,
name|NULL
argument_list|,
operator|&
name|ignored_only
argument_list|,
name|N_
argument_list|(
literal|"remove only ignored files"
argument_list|)
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|git_config
argument_list|(
name|git_clean_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|force
operator|<
literal|0
condition|)
name|force
operator|=
literal|0
expr_stmt|;
else|else
name|config_set
operator|=
literal|1
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
name|options
argument_list|,
name|builtin_clean_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
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
name|ignored_only
condition|)
name|dir
operator|.
name|flags
operator||=
name|DIR_SHOW_IGNORED
expr_stmt|;
if|if
condition|(
name|ignored
operator|&&
name|ignored_only
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"-x and -X cannot be used together"
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dry_run
operator|&&
operator|!
name|force
condition|)
block|{
if|if
condition|(
name|config_set
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"clean.requireForce set to true and neither -n nor -f given; "
literal|"refusing to clean"
argument_list|)
argument_list|)
expr_stmt|;
else|else
name|die
argument_list|(
name|_
argument_list|(
literal|"clean.requireForce defaults to true and neither -n nor -f given; "
literal|"refusing to clean"
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|force
operator|>
literal|1
condition|)
name|rm_flags
operator|=
literal|0
expr_stmt|;
name|dir
operator|.
name|flags
operator||=
name|DIR_SHOW_OTHER_DIRECTORIES
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
name|_
argument_list|(
literal|"index file corrupt"
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ignored
condition|)
name|setup_standard_excludes
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
name|el
operator|=
name|add_exclude_list
argument_list|(
operator|&
name|dir
argument_list|,
name|EXC_CMDL
argument_list|,
literal|"--exclude option"
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
name|exclude_list
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|add_exclude
argument_list|(
name|exclude_list
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|,
name|el
argument_list|,
operator|-
operator|(
name|i
operator|+
literal|1
operator|)
argument_list|)
expr_stmt|;
name|pathspec
operator|=
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
argument_list|)
expr_stmt|;
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
name|xmalloc
argument_list|(
name|argc
operator|>
literal|0
condition|?
name|argc
else|:
literal|1
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
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|dir_entry
modifier|*
name|ent
init|=
name|dir
operator|.
name|entries
index|[
name|i
index|]
decl_stmt|;
name|int
name|len
decl_stmt|,
name|pos
decl_stmt|;
name|int
name|matches
init|=
literal|0
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
comment|/* 		 * Remove the '/' at the end that directory 		 * walking adds for directory entries. 		 */
name|len
operator|=
name|ent
operator|->
name|len
expr_stmt|;
if|if
condition|(
name|len
operator|&&
name|ent
operator|->
name|name
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
name|len
operator|--
expr_stmt|;
name|pos
operator|=
name|cache_name_pos
argument_list|(
name|ent
operator|->
name|name
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|pos
condition|)
continue|continue;
comment|/* exact match */
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
operator|<
name|active_nr
condition|)
block|{
name|ce
operator|=
name|active_cache
index|[
name|pos
index|]
expr_stmt|;
if|if
condition|(
name|ce_namelen
argument_list|(
name|ce
argument_list|)
operator|==
name|len
operator|&&
operator|!
name|memcmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|ent
operator|->
name|name
argument_list|,
name|len
argument_list|)
condition|)
continue|continue;
comment|/* Yup, this one exists unmerged */
block|}
comment|/* 		 * we might have removed this as part of earlier 		 * recursive directory removal, so lstat() here could 		 * fail with ENOENT. 		 */
if|if
condition|(
name|lstat
argument_list|(
name|ent
operator|->
name|name
argument_list|,
operator|&
name|st
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|pathspec
condition|)
block|{
name|memset
argument_list|(
name|seen
argument_list|,
literal|0
argument_list|,
name|argc
operator|>
literal|0
condition|?
name|argc
else|:
literal|1
argument_list|)
expr_stmt|;
name|matches
operator|=
name|match_pathspec
argument_list|(
name|pathspec
argument_list|,
name|ent
operator|->
name|name
argument_list|,
name|len
argument_list|,
literal|0
argument_list|,
name|seen
argument_list|)
expr_stmt|;
block|}
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
name|strbuf_addstr
argument_list|(
operator|&
name|directory
argument_list|,
name|ent
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|remove_directories
operator|||
operator|(
name|matches
operator|==
name|MATCHED_EXACTLY
operator|)
condition|)
block|{
if|if
condition|(
name|remove_dirs
argument_list|(
operator|&
name|directory
argument_list|,
name|prefix
argument_list|,
name|rm_flags
argument_list|,
name|dry_run
argument_list|,
name|quiet
argument_list|,
operator|&
name|gone
argument_list|)
condition|)
name|errors
operator|++
expr_stmt|;
if|if
condition|(
name|gone
operator|&&
operator|!
name|quiet
condition|)
block|{
name|qname
operator|=
name|quote_path_relative
argument_list|(
name|directory
operator|.
name|buf
argument_list|,
name|directory
operator|.
name|len
argument_list|,
operator|&
name|buf
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|printf
argument_list|(
name|dry_run
condition|?
name|_
argument_list|(
name|msg_would_remove
argument_list|)
else|:
name|_
argument_list|(
name|msg_remove
argument_list|)
argument_list|,
name|qname
argument_list|)
expr_stmt|;
block|}
block|}
name|strbuf_reset
argument_list|(
operator|&
name|directory
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|pathspec
operator|&&
operator|!
name|matches
condition|)
continue|continue;
name|res
operator|=
name|dry_run
condition|?
literal|0
else|:
name|unlink
argument_list|(
name|ent
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|res
condition|)
block|{
name|qname
operator|=
name|quote_path_relative
argument_list|(
name|ent
operator|->
name|name
argument_list|,
operator|-
literal|1
argument_list|,
operator|&
name|buf
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|warning
argument_list|(
name|_
argument_list|(
name|msg_warn_remove_failed
argument_list|)
argument_list|,
name|qname
argument_list|)
expr_stmt|;
name|errors
operator|++
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|quiet
condition|)
block|{
name|qname
operator|=
name|quote_path_relative
argument_list|(
name|ent
operator|->
name|name
argument_list|,
operator|-
literal|1
argument_list|,
operator|&
name|buf
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|printf
argument_list|(
name|dry_run
condition|?
name|_
argument_list|(
name|msg_would_remove
argument_list|)
else|:
name|_
argument_list|(
name|msg_remove
argument_list|)
argument_list|,
name|qname
argument_list|)
expr_stmt|;
block|}
block|}
block|}
name|free
argument_list|(
name|seen
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|directory
argument_list|)
expr_stmt|;
name|string_list_clear
argument_list|(
operator|&
name|exclude_list
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return
operator|(
name|errors
operator|!=
literal|0
operator|)
return|;
block|}
end_function

end_unit

