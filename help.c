begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * builtin-help.c  *  * Builtin help-related commands (help, usage, version)  */
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

begin_include
include|#
directive|include
file|"common-cmds.h"
end_include

begin_comment
comment|/* most GUI terminals set COLUMNS (although some don't export it) */
end_comment

begin_function
DECL|function|term_columns
specifier|static
name|int
name|term_columns
parameter_list|(
name|void
parameter_list|)
block|{
name|char
modifier|*
name|col_string
init|=
name|getenv
argument_list|(
literal|"COLUMNS"
argument_list|)
decl_stmt|;
name|int
name|n_cols
decl_stmt|;
if|if
condition|(
name|col_string
operator|&&
operator|(
name|n_cols
operator|=
name|atoi
argument_list|(
name|col_string
argument_list|)
operator|)
operator|>
literal|0
condition|)
return|return
name|n_cols
return|;
ifdef|#
directive|ifdef
name|TIOCGWINSZ
block|{
name|struct
name|winsize
name|ws
decl_stmt|;
if|if
condition|(
operator|!
name|ioctl
argument_list|(
literal|1
argument_list|,
name|TIOCGWINSZ
argument_list|,
operator|&
name|ws
argument_list|)
condition|)
block|{
if|if
condition|(
name|ws
operator|.
name|ws_col
condition|)
return|return
name|ws
operator|.
name|ws_col
return|;
block|}
block|}
endif|#
directive|endif
return|return
literal|80
return|;
block|}
end_function

begin_function
DECL|function|mput_char
specifier|static
specifier|inline
name|void
name|mput_char
parameter_list|(
name|char
name|c
parameter_list|,
name|unsigned
name|int
name|num
parameter_list|)
block|{
while|while
condition|(
name|num
operator|--
condition|)
name|putchar
argument_list|(
name|c
argument_list|)
expr_stmt|;
block|}
end_function

begin_struct
DECL|struct|cmdnames
specifier|static
struct|struct
name|cmdnames
block|{
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|cnt
name|int
name|cnt
decl_stmt|;
DECL|struct|cmdname
struct|struct
name|cmdname
block|{
DECL|member|len
name|size_t
name|len
decl_stmt|;
DECL|member|name
name|char
name|name
index|[
literal|1
index|]
decl_stmt|;
DECL|member|names
block|}
modifier|*
modifier|*
name|names
struct|;
DECL|variable|main_cmds
DECL|variable|other_cmds
block|}
name|main_cmds
struct|,
name|other_cmds
struct|;
end_struct

begin_function
DECL|function|add_cmdname
specifier|static
name|void
name|add_cmdname
parameter_list|(
name|struct
name|cmdnames
modifier|*
name|cmds
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|len
parameter_list|)
block|{
name|struct
name|cmdname
modifier|*
name|ent
init|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|ent
argument_list|)
operator|+
name|len
argument_list|)
decl_stmt|;
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
name|name
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
name|ALLOC_GROW
argument_list|(
name|cmds
operator|->
name|names
argument_list|,
name|cmds
operator|->
name|cnt
operator|+
literal|1
argument_list|,
name|cmds
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|cmds
operator|->
name|names
index|[
name|cmds
operator|->
name|cnt
operator|++
index|]
operator|=
name|ent
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmdname_compare
specifier|static
name|int
name|cmdname_compare
parameter_list|(
specifier|const
name|void
modifier|*
name|a_
parameter_list|,
specifier|const
name|void
modifier|*
name|b_
parameter_list|)
block|{
name|struct
name|cmdname
modifier|*
name|a
init|=
operator|*
operator|(
expr|struct
name|cmdname
operator|*
operator|*
operator|)
name|a_
decl_stmt|;
name|struct
name|cmdname
modifier|*
name|b
init|=
operator|*
operator|(
expr|struct
name|cmdname
operator|*
operator|*
operator|)
name|b_
decl_stmt|;
return|return
name|strcmp
argument_list|(
name|a
operator|->
name|name
argument_list|,
name|b
operator|->
name|name
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|uniq
specifier|static
name|void
name|uniq
parameter_list|(
name|struct
name|cmdnames
modifier|*
name|cmds
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|j
decl_stmt|;
if|if
condition|(
operator|!
name|cmds
operator|->
name|cnt
condition|)
return|return;
for|for
control|(
name|i
operator|=
name|j
operator|=
literal|1
init|;
name|i
operator|<
name|cmds
operator|->
name|cnt
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|strcmp
argument_list|(
name|cmds
operator|->
name|names
index|[
name|i
index|]
operator|->
name|name
argument_list|,
name|cmds
operator|->
name|names
index|[
name|i
operator|-
literal|1
index|]
operator|->
name|name
argument_list|)
condition|)
name|cmds
operator|->
name|names
index|[
name|j
operator|++
index|]
operator|=
name|cmds
operator|->
name|names
index|[
name|i
index|]
expr_stmt|;
name|cmds
operator|->
name|cnt
operator|=
name|j
expr_stmt|;
block|}
end_function

begin_function
DECL|function|exclude_cmds
specifier|static
name|void
name|exclude_cmds
parameter_list|(
name|struct
name|cmdnames
modifier|*
name|cmds
parameter_list|,
name|struct
name|cmdnames
modifier|*
name|excludes
parameter_list|)
block|{
name|int
name|ci
decl_stmt|,
name|cj
decl_stmt|,
name|ei
decl_stmt|;
name|int
name|cmp
decl_stmt|;
name|ci
operator|=
name|cj
operator|=
name|ei
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|ci
operator|<
name|cmds
operator|->
name|cnt
operator|&&
name|ei
operator|<
name|excludes
operator|->
name|cnt
condition|)
block|{
name|cmp
operator|=
name|strcmp
argument_list|(
name|cmds
operator|->
name|names
index|[
name|ci
index|]
operator|->
name|name
argument_list|,
name|excludes
operator|->
name|names
index|[
name|ei
index|]
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
name|cmds
operator|->
name|names
index|[
name|cj
operator|++
index|]
operator|=
name|cmds
operator|->
name|names
index|[
name|ci
operator|++
index|]
expr_stmt|;
elseif|else
if|if
condition|(
name|cmp
operator|==
literal|0
condition|)
name|ci
operator|++
operator|,
name|ei
operator|++
expr_stmt|;
elseif|else
if|if
condition|(
name|cmp
operator|>
literal|0
condition|)
name|ei
operator|++
expr_stmt|;
block|}
while|while
condition|(
name|ci
operator|<
name|cmds
operator|->
name|cnt
condition|)
name|cmds
operator|->
name|names
index|[
name|cj
operator|++
index|]
operator|=
name|cmds
operator|->
name|names
index|[
name|ci
operator|++
index|]
expr_stmt|;
name|cmds
operator|->
name|cnt
operator|=
name|cj
expr_stmt|;
block|}
end_function

begin_function
DECL|function|pretty_print_string_list
specifier|static
name|void
name|pretty_print_string_list
parameter_list|(
name|struct
name|cmdnames
modifier|*
name|cmds
parameter_list|,
name|int
name|longest
parameter_list|)
block|{
name|int
name|cols
init|=
literal|1
decl_stmt|,
name|rows
decl_stmt|;
name|int
name|space
init|=
name|longest
operator|+
literal|1
decl_stmt|;
comment|/* min 1 SP between words */
name|int
name|max_cols
init|=
name|term_columns
argument_list|()
operator|-
literal|1
decl_stmt|;
comment|/* don't print *on* the edge */
name|int
name|i
decl_stmt|,
name|j
decl_stmt|;
if|if
condition|(
name|space
operator|<
name|max_cols
condition|)
name|cols
operator|=
name|max_cols
operator|/
name|space
expr_stmt|;
name|rows
operator|=
operator|(
name|cmds
operator|->
name|cnt
operator|+
name|cols
operator|-
literal|1
operator|)
operator|/
name|cols
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|rows
condition|;
name|i
operator|++
control|)
block|{
name|printf
argument_list|(
literal|"  "
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
name|cols
condition|;
name|j
operator|++
control|)
block|{
name|int
name|n
init|=
name|j
operator|*
name|rows
operator|+
name|i
decl_stmt|;
name|int
name|size
init|=
name|space
decl_stmt|;
if|if
condition|(
name|n
operator|>=
name|cmds
operator|->
name|cnt
condition|)
break|break;
if|if
condition|(
name|j
operator|==
name|cols
operator|-
literal|1
operator|||
name|n
operator|+
name|rows
operator|>=
name|cmds
operator|->
name|cnt
condition|)
name|size
operator|=
literal|1
expr_stmt|;
name|printf
argument_list|(
literal|"%-*s"
argument_list|,
name|size
argument_list|,
name|cmds
operator|->
name|names
index|[
name|n
index|]
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|list_commands_in_dir
specifier|static
name|unsigned
name|int
name|list_commands_in_dir
parameter_list|(
name|struct
name|cmdnames
modifier|*
name|cmds
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|unsigned
name|int
name|longest
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|prefix
init|=
literal|"git-"
decl_stmt|;
name|int
name|prefix_len
init|=
name|strlen
argument_list|(
name|prefix
argument_list|)
decl_stmt|;
name|DIR
modifier|*
name|dir
init|=
name|opendir
argument_list|(
name|path
argument_list|)
decl_stmt|;
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
if|if
condition|(
operator|!
name|dir
operator|||
name|chdir
argument_list|(
name|path
argument_list|)
condition|)
return|return
literal|0
return|;
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
name|st
decl_stmt|;
name|int
name|entlen
decl_stmt|;
if|if
condition|(
name|prefixcmp
argument_list|(
name|de
operator|->
name|d_name
argument_list|,
name|prefix
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|stat
argument_list|(
name|de
operator|->
name|d_name
argument_list|,
operator|&
name|st
argument_list|)
operator|||
comment|/* stat, not lstat */
operator|!
name|S_ISREG
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
operator|||
operator|!
operator|(
name|st
operator|.
name|st_mode
operator|&
name|S_IXUSR
operator|)
condition|)
continue|continue;
name|entlen
operator|=
name|strlen
argument_list|(
name|de
operator|->
name|d_name
argument_list|)
operator|-
name|prefix_len
expr_stmt|;
if|if
condition|(
name|has_extension
argument_list|(
name|de
operator|->
name|d_name
argument_list|,
literal|".exe"
argument_list|)
condition|)
name|entlen
operator|-=
literal|4
expr_stmt|;
if|if
condition|(
name|longest
operator|<
name|entlen
condition|)
name|longest
operator|=
name|entlen
expr_stmt|;
name|add_cmdname
argument_list|(
name|cmds
argument_list|,
name|de
operator|->
name|d_name
operator|+
name|prefix_len
argument_list|,
name|entlen
argument_list|)
expr_stmt|;
block|}
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
return|return
name|longest
return|;
block|}
end_function

begin_function
DECL|function|list_commands
specifier|static
name|void
name|list_commands
parameter_list|(
name|void
parameter_list|)
block|{
name|unsigned
name|int
name|longest
init|=
literal|0
decl_stmt|;
name|unsigned
name|int
name|len
decl_stmt|;
specifier|const
name|char
modifier|*
name|env_path
init|=
name|getenv
argument_list|(
literal|"PATH"
argument_list|)
decl_stmt|;
name|char
modifier|*
name|paths
decl_stmt|,
modifier|*
name|path
decl_stmt|,
modifier|*
name|colon
decl_stmt|;
specifier|const
name|char
modifier|*
name|exec_path
init|=
name|git_exec_path
argument_list|()
decl_stmt|;
if|if
condition|(
name|exec_path
condition|)
name|longest
operator|=
name|list_commands_in_dir
argument_list|(
operator|&
name|main_cmds
argument_list|,
name|exec_path
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|env_path
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"PATH not set\n"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
name|path
operator|=
name|paths
operator|=
name|xstrdup
argument_list|(
name|env_path
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
if|if
condition|(
operator|(
name|colon
operator|=
name|strchr
argument_list|(
name|path
argument_list|,
literal|':'
argument_list|)
operator|)
condition|)
operator|*
name|colon
operator|=
literal|0
expr_stmt|;
name|len
operator|=
name|list_commands_in_dir
argument_list|(
operator|&
name|other_cmds
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>
name|longest
condition|)
name|longest
operator|=
name|len
expr_stmt|;
if|if
condition|(
operator|!
name|colon
condition|)
break|break;
name|path
operator|=
name|colon
operator|+
literal|1
expr_stmt|;
block|}
name|free
argument_list|(
name|paths
argument_list|)
expr_stmt|;
name|qsort
argument_list|(
name|main_cmds
operator|.
name|names
argument_list|,
name|main_cmds
operator|.
name|cnt
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|main_cmds
operator|.
name|names
argument_list|)
argument_list|,
name|cmdname_compare
argument_list|)
expr_stmt|;
name|uniq
argument_list|(
operator|&
name|main_cmds
argument_list|)
expr_stmt|;
name|qsort
argument_list|(
name|other_cmds
operator|.
name|names
argument_list|,
name|other_cmds
operator|.
name|cnt
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|other_cmds
operator|.
name|names
argument_list|)
argument_list|,
name|cmdname_compare
argument_list|)
expr_stmt|;
name|uniq
argument_list|(
operator|&
name|other_cmds
argument_list|)
expr_stmt|;
name|exclude_cmds
argument_list|(
operator|&
name|other_cmds
argument_list|,
operator|&
name|main_cmds
argument_list|)
expr_stmt|;
if|if
condition|(
name|main_cmds
operator|.
name|cnt
condition|)
block|{
name|printf
argument_list|(
literal|"available git commands in '%s'\n"
argument_list|,
name|exec_path
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"----------------------------"
argument_list|)
expr_stmt|;
name|mput_char
argument_list|(
literal|'-'
argument_list|,
name|strlen
argument_list|(
name|exec_path
argument_list|)
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
name|pretty_print_string_list
argument_list|(
operator|&
name|main_cmds
argument_list|,
name|longest
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|other_cmds
operator|.
name|cnt
condition|)
block|{
name|printf
argument_list|(
literal|"git commands available from elsewhere on your $PATH\n"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"---------------------------------------------------\n"
argument_list|)
expr_stmt|;
name|pretty_print_string_list
argument_list|(
operator|&
name|other_cmds
argument_list|,
name|longest
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
DECL|function|list_common_cmds_help
name|void
name|list_common_cmds_help
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|longest
init|=
literal|0
decl_stmt|;
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
name|common_cmds
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|longest
operator|<
name|strlen
argument_list|(
name|common_cmds
index|[
name|i
index|]
operator|.
name|name
argument_list|)
condition|)
name|longest
operator|=
name|strlen
argument_list|(
name|common_cmds
index|[
name|i
index|]
operator|.
name|name
argument_list|)
expr_stmt|;
block|}
name|puts
argument_list|(
literal|"The most commonly used git commands are:"
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
name|ARRAY_SIZE
argument_list|(
name|common_cmds
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
name|printf
argument_list|(
literal|"   %s   "
argument_list|,
name|common_cmds
index|[
name|i
index|]
operator|.
name|name
argument_list|)
expr_stmt|;
name|mput_char
argument_list|(
literal|' '
argument_list|,
name|longest
operator|-
name|strlen
argument_list|(
name|common_cmds
index|[
name|i
index|]
operator|.
name|name
argument_list|)
argument_list|)
expr_stmt|;
name|puts
argument_list|(
name|common_cmds
index|[
name|i
index|]
operator|.
name|help
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|cmd_to_page
specifier|static
specifier|const
name|char
modifier|*
name|cmd_to_page
parameter_list|(
specifier|const
name|char
modifier|*
name|git_cmd
parameter_list|)
block|{
if|if
condition|(
operator|!
name|git_cmd
condition|)
return|return
literal|"git"
return|;
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|git_cmd
argument_list|,
literal|"git"
argument_list|)
condition|)
return|return
name|git_cmd
return|;
else|else
block|{
name|int
name|page_len
init|=
name|strlen
argument_list|(
name|git_cmd
argument_list|)
operator|+
literal|4
decl_stmt|;
name|char
modifier|*
name|p
init|=
name|xmalloc
argument_list|(
name|page_len
operator|+
literal|1
argument_list|)
decl_stmt|;
name|strcpy
argument_list|(
name|p
argument_list|,
literal|"git-"
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|p
operator|+
literal|4
argument_list|,
name|git_cmd
argument_list|)
expr_stmt|;
name|p
index|[
name|page_len
index|]
operator|=
literal|0
expr_stmt|;
return|return
name|p
return|;
block|}
block|}
end_function

begin_function
DECL|function|setup_man_path
specifier|static
name|void
name|setup_man_path
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|strbuf
name|new_path
decl_stmt|;
specifier|const
name|char
modifier|*
name|old_path
init|=
name|getenv
argument_list|(
literal|"MANPATH"
argument_list|)
decl_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|new_path
argument_list|,
literal|0
argument_list|)
expr_stmt|;
comment|/* We should always put ':' after our path. If there is no 	 * old_path, the ':' at the end will let 'man' to try 	 * system-wide paths after ours to find the manual page. If 	 * there is old_path, we need ':' as delimiter. */
name|strbuf_addstr
argument_list|(
operator|&
name|new_path
argument_list|,
name|GIT_MAN_PATH
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
operator|&
name|new_path
argument_list|,
literal|':'
argument_list|)
expr_stmt|;
if|if
condition|(
name|old_path
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|new_path
argument_list|,
name|old_path
argument_list|)
expr_stmt|;
name|setenv
argument_list|(
literal|"MANPATH"
argument_list|,
name|new_path
operator|.
name|buf
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|new_path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_man_page
specifier|static
name|void
name|show_man_page
parameter_list|(
specifier|const
name|char
modifier|*
name|git_cmd
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|page
init|=
name|cmd_to_page
argument_list|(
name|git_cmd
argument_list|)
decl_stmt|;
name|setup_man_path
argument_list|()
expr_stmt|;
name|execlp
argument_list|(
literal|"man"
argument_list|,
literal|"man"
argument_list|,
name|page
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_info_page
specifier|static
name|void
name|show_info_page
parameter_list|(
specifier|const
name|char
modifier|*
name|git_cmd
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|page
init|=
name|cmd_to_page
argument_list|(
name|git_cmd
argument_list|)
decl_stmt|;
name|setenv
argument_list|(
literal|"INFOPATH"
argument_list|,
name|GIT_INFO_PATH
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|execlp
argument_list|(
literal|"info"
argument_list|,
literal|"info"
argument_list|,
literal|"gitman"
argument_list|,
name|page
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_html_page
specifier|static
name|void
name|show_html_page
parameter_list|(
specifier|const
name|char
modifier|*
name|git_cmd
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|page
init|=
name|cmd_to_page
argument_list|(
name|git_cmd
argument_list|)
decl_stmt|;
name|execl_git_cmd
argument_list|(
literal|"browse-help"
argument_list|,
name|page
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|help_unknown_cmd
name|void
name|help_unknown_cmd
parameter_list|(
specifier|const
name|char
modifier|*
name|cmd
parameter_list|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"git: '%s' is not a git-command. See 'git --help'.\n"
argument_list|,
name|cmd
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_version
name|int
name|cmd_version
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
name|printf
argument_list|(
literal|"git version %s\n"
argument_list|,
name|git_version_string
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_help
name|int
name|cmd_help
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
name|help_cmd
init|=
name|argv
index|[
literal|1
index|]
decl_stmt|;
if|if
condition|(
name|argc
operator|<
literal|2
condition|)
block|{
name|printf
argument_list|(
literal|"usage: %s\n\n"
argument_list|,
name|git_usage_string
argument_list|)
expr_stmt|;
name|list_common_cmds_help
argument_list|()
expr_stmt|;
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|help_cmd
argument_list|,
literal|"--all"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|help_cmd
argument_list|,
literal|"-a"
argument_list|)
condition|)
block|{
name|printf
argument_list|(
literal|"usage: %s\n\n"
argument_list|,
name|git_usage_string
argument_list|)
expr_stmt|;
name|list_commands
argument_list|()
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|help_cmd
argument_list|,
literal|"--web"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|help_cmd
argument_list|,
literal|"-w"
argument_list|)
condition|)
block|{
name|show_html_page
argument_list|(
name|argc
operator|>
literal|2
condition|?
name|argv
index|[
literal|2
index|]
else|:
name|NULL
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|help_cmd
argument_list|,
literal|"--info"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|help_cmd
argument_list|,
literal|"-i"
argument_list|)
condition|)
block|{
name|show_info_page
argument_list|(
name|argc
operator|>
literal|2
condition|?
name|argv
index|[
literal|2
index|]
else|:
name|NULL
argument_list|)
expr_stmt|;
block|}
else|else
name|show_man_page
argument_list|(
name|help_cmd
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

