begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<sys/types.h>
end_include

begin_include
include|#
directive|include
file|<sys/stat.h>
end_include

begin_include
include|#
directive|include
file|<dirent.h>
end_include

begin_include
include|#
directive|include
file|<unistd.h>
end_include

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_include
include|#
directive|include
file|<errno.h>
end_include

begin_include
include|#
directive|include
file|<limits.h>
end_include

begin_include
include|#
directive|include
file|<stdarg.h>
end_include

begin_include
include|#
directive|include
file|<sys/ioctl.h>
end_include

begin_include
include|#
directive|include
file|"git-compat-util.h"
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

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_ifndef
ifndef|#
directive|ifndef
name|PATH_MAX
end_ifndef

begin_define
DECL|macro|PATH_MAX
define|#
directive|define
name|PATH_MAX
value|4096
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|git_usage
specifier|static
specifier|const
name|char
name|git_usage
index|[]
init|=
literal|"Usage: git [--version] [--exec-path[=GIT_EXEC_PATH]] [--help] COMMAND [ ARGS ]"
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* most gui terms set COLUMNS (although some don't export it) */
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
init|=
literal|0
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
DECL|function|oom
specifier|static
name|void
name|oom
parameter_list|(
name|void
parameter_list|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"git: out of memory\n"
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
DECL|struct|cmdname
specifier|static
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
DECL|variable|cmdname
block|}
modifier|*
modifier|*
name|cmdname
struct|;
end_struct

begin_decl_stmt
DECL|variable|cmdname_alloc
DECL|variable|cmdname_cnt
specifier|static
name|int
name|cmdname_alloc
decl_stmt|,
name|cmdname_cnt
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|add_cmdname
specifier|static
name|void
name|add_cmdname
parameter_list|(
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
decl_stmt|;
if|if
condition|(
name|cmdname_alloc
operator|<=
name|cmdname_cnt
condition|)
block|{
name|cmdname_alloc
operator|=
name|cmdname_alloc
operator|+
literal|200
expr_stmt|;
name|cmdname
operator|=
name|realloc
argument_list|(
name|cmdname
argument_list|,
name|cmdname_alloc
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|cmdname
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cmdname
condition|)
name|oom
argument_list|()
expr_stmt|;
block|}
name|ent
operator|=
name|malloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|ent
argument_list|)
operator|+
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ent
condition|)
name|oom
argument_list|()
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
name|cmdname
index|[
name|cmdname_cnt
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
DECL|function|pretty_print_string_list
specifier|static
name|void
name|pretty_print_string_list
parameter_list|(
name|struct
name|cmdname
modifier|*
modifier|*
name|cmdname
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
name|cmdname_cnt
operator|+
name|cols
operator|-
literal|1
operator|)
operator|/
name|cols
expr_stmt|;
name|qsort
argument_list|(
name|cmdname
argument_list|,
name|cmdname_cnt
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|cmdname
argument_list|)
argument_list|,
name|cmdname_compare
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
name|cmdname_cnt
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
name|cmdname_cnt
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
name|cmdname
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
DECL|function|list_commands
specifier|static
name|void
name|list_commands
parameter_list|(
specifier|const
name|char
modifier|*
name|exec_path
parameter_list|,
specifier|const
name|char
modifier|*
name|pattern
parameter_list|)
block|{
name|unsigned
name|int
name|longest
init|=
literal|0
decl_stmt|;
name|char
name|path
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|int
name|dirlen
decl_stmt|;
name|DIR
modifier|*
name|dir
init|=
name|opendir
argument_list|(
name|exec_path
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
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"git: '%s': %s\n"
argument_list|,
name|exec_path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
name|dirlen
operator|=
name|strlen
argument_list|(
name|exec_path
argument_list|)
expr_stmt|;
if|if
condition|(
name|PATH_MAX
operator|-
literal|20
operator|<
name|dirlen
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"git: insanely long exec-path '%s'\n"
argument_list|,
name|exec_path
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
name|memcpy
argument_list|(
name|path
argument_list|,
name|exec_path
argument_list|,
name|dirlen
argument_list|)
expr_stmt|;
name|path
index|[
name|dirlen
operator|++
index|]
operator|=
literal|'/'
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
name|st
decl_stmt|;
name|int
name|entlen
decl_stmt|;
if|if
condition|(
name|strncmp
argument_list|(
name|de
operator|->
name|d_name
argument_list|,
literal|"git-"
argument_list|,
literal|4
argument_list|)
condition|)
continue|continue;
name|strcpy
argument_list|(
name|path
operator|+
name|dirlen
argument_list|,
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|path
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
expr_stmt|;
if|if
condition|(
literal|4
operator|<
name|entlen
operator|&&
operator|!
name|strcmp
argument_list|(
name|de
operator|->
name|d_name
operator|+
name|entlen
operator|-
literal|4
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
name|de
operator|->
name|d_name
operator|+
literal|4
argument_list|,
name|entlen
operator|-
literal|4
argument_list|)
expr_stmt|;
block|}
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"git commands available in '%s'\n"
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
name|cmdname
argument_list|,
name|longest
operator|-
literal|4
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|list_common_cmds_help
specifier|static
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
literal|"    %s"
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
operator|+
literal|4
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
name|puts
argument_list|(
literal|"(use 'git help -a' to get a list of all installed git commands)"
argument_list|)
expr_stmt|;
block|}
end_function

begin_ifdef
ifdef|#
directive|ifdef
name|__GNUC__
end_ifdef

begin_function_decl
specifier|static
name|void
name|cmd_usage
parameter_list|(
name|int
name|show_all
parameter_list|,
specifier|const
name|char
modifier|*
name|exec_path
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
function_decl|__attribute__
parameter_list|(
function_decl|(__format__
parameter_list|(
name|__printf__
parameter_list|,
function_decl|3
operator|,
function_decl|4
end_function_decl

begin_operator
unit|)
operator|,
end_operator

begin_expr_stmt
name|__noreturn__
end_expr_stmt

begin_empty_stmt
unit|))
empty_stmt|;
end_empty_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|cmd_usage
specifier|static
name|void
name|cmd_usage
parameter_list|(
name|int
name|show_all
parameter_list|,
specifier|const
name|char
modifier|*
name|exec_path
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
if|if
condition|(
name|fmt
condition|)
block|{
name|va_list
name|ap
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"git: "
argument_list|)
expr_stmt|;
name|vprintf
argument_list|(
name|fmt
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
block|}
else|else
name|puts
argument_list|(
name|git_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|exec_path
condition|)
block|{
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
if|if
condition|(
name|show_all
condition|)
name|list_commands
argument_list|(
name|exec_path
argument_list|,
literal|"git-*"
argument_list|)
expr_stmt|;
else|else
name|list_common_cmds_help
argument_list|()
expr_stmt|;
block|}
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

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
name|char
modifier|*
name|path
decl_stmt|,
modifier|*
name|old_path
init|=
name|getenv
argument_list|(
literal|"PATH"
argument_list|)
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
name|malloc
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
decl_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|git_cmd
argument_list|,
literal|"git"
argument_list|,
literal|3
argument_list|)
condition|)
name|page
operator|=
name|git_cmd
expr_stmt|;
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
name|malloc
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
name|page
operator|=
name|p
expr_stmt|;
block|}
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
DECL|function|cmd_version
specifier|static
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
name|char
modifier|*
modifier|*
name|envp
parameter_list|)
block|{
name|printf
argument_list|(
literal|"git version %s\n"
argument_list|,
name|GIT_VERSION
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_help
specifier|static
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
name|char
modifier|*
modifier|*
name|envp
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
operator|!
name|help_cmd
condition|)
name|cmd_usage
argument_list|(
literal|0
argument_list|,
name|git_exec_path
argument_list|()
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
elseif|else
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
name|cmd_usage
argument_list|(
literal|1
argument_list|,
name|git_exec_path
argument_list|()
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
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

begin_define
DECL|macro|LOGSIZE
define|#
directive|define
name|LOGSIZE
value|(65536)
end_define

begin_function
DECL|function|cmd_log
specifier|static
name|int
name|cmd_log
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
name|struct
name|rev_info
name|rev
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|char
modifier|*
name|buf
init|=
name|xmalloc
argument_list|(
name|LOGSIZE
argument_list|)
decl_stmt|;
specifier|static
name|enum
name|cmit_fmt
name|commit_format
init|=
name|CMIT_FMT_DEFAULT
decl_stmt|;
name|int
name|abbrev
init|=
name|DEFAULT_ABBREV
decl_stmt|;
specifier|const
name|char
modifier|*
name|commit_prefix
init|=
literal|"commit "
decl_stmt|;
name|argc
operator|=
name|setup_revisions
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
operator|&
name|rev
argument_list|,
literal|"HEAD"
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
operator|<
name|argc
condition|)
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
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--pretty"
argument_list|,
literal|8
argument_list|)
condition|)
block|{
name|commit_format
operator|=
name|get_commit_format
argument_list|(
name|arg
operator|+
literal|8
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit_format
operator|==
name|CMIT_FMT_ONELINE
condition|)
name|commit_prefix
operator|=
literal|""
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--no-abbrev"
argument_list|)
condition|)
block|{
name|abbrev
operator|=
literal|0
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--abbrev="
argument_list|,
literal|9
argument_list|)
condition|)
block|{
name|abbrev
operator|=
name|strtoul
argument_list|(
name|arg
operator|+
literal|9
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|abbrev
operator|&&
name|abbrev
operator|<
name|MINIMUM_ABBREV
condition|)
name|abbrev
operator|=
name|MINIMUM_ABBREV
expr_stmt|;
elseif|else
if|if
condition|(
literal|40
operator|<
name|abbrev
condition|)
name|abbrev
operator|=
literal|40
expr_stmt|;
block|}
else|else
name|die
argument_list|(
literal|"unrecognized argument: %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
name|prepare_revision_walk
argument_list|(
operator|&
name|rev
argument_list|)
expr_stmt|;
name|setup_pager
argument_list|()
expr_stmt|;
while|while
condition|(
operator|(
name|commit
operator|=
name|get_revision
argument_list|(
operator|&
name|rev
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|printf
argument_list|(
literal|"%s%s"
argument_list|,
name|commit_prefix
argument_list|,
name|sha1_to_hex
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|rev
operator|.
name|parents
condition|)
block|{
name|struct
name|commit_list
modifier|*
name|parents
init|=
name|commit
operator|->
name|parents
decl_stmt|;
while|while
condition|(
name|parents
condition|)
block|{
name|struct
name|object
modifier|*
name|o
init|=
operator|&
operator|(
name|parents
operator|->
name|item
operator|->
name|object
operator|)
decl_stmt|;
name|parents
operator|=
name|parents
operator|->
name|next
expr_stmt|;
if|if
condition|(
name|o
operator|->
name|flags
operator|&
name|TMP_MARK
condition|)
continue|continue;
name|printf
argument_list|(
literal|" %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|o
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|o
operator|->
name|flags
operator||=
name|TMP_MARK
expr_stmt|;
block|}
comment|/* TMP_MARK is a general purpose flag that can 			 * be used locally, but the user should clean 			 * things up after it is done with them. 			 */
for|for
control|(
name|parents
operator|=
name|commit
operator|->
name|parents
init|;
name|parents
condition|;
name|parents
operator|=
name|parents
operator|->
name|next
control|)
name|parents
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator|&=
operator|~
name|TMP_MARK
expr_stmt|;
block|}
if|if
condition|(
name|commit_format
operator|==
name|CMIT_FMT_ONELINE
condition|)
name|putchar
argument_list|(
literal|' '
argument_list|)
expr_stmt|;
else|else
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
name|pretty_print_commit
argument_list|(
name|commit_format
argument_list|,
name|commit
argument_list|,
operator|~
literal|0
argument_list|,
name|buf
argument_list|,
name|LOGSIZE
argument_list|,
name|abbrev
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|buf
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

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
name|char
modifier|*
modifier|*
parameter_list|)
function_decl|;
block|}
name|commands
index|[]
init|=
block|{
block|{
literal|"version"
block|,
name|cmd_version
block|}
block|,
block|{
literal|"help"
block|,
name|cmd_help
block|}
block|,
block|{
literal|"log"
block|,
name|cmd_log
block|}
block|, 	}
struct|;
name|int
name|i
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
name|envp
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
name|char
name|git_command
index|[
name|PATH_MAX
operator|+
literal|1
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|exec_path
init|=
name|NULL
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
comment|/* Default command: "help" */
name|cmd
operator|=
literal|"help"
expr_stmt|;
comment|/* Look for flags.. */
while|while
condition|(
name|argc
operator|>
literal|1
condition|)
block|{
name|cmd
operator|=
operator|*
operator|++
name|argv
expr_stmt|;
name|argc
operator|--
expr_stmt|;
if|if
condition|(
name|strncmp
argument_list|(
name|cmd
argument_list|,
literal|"--"
argument_list|,
literal|2
argument_list|)
condition|)
break|break;
name|cmd
operator|+=
literal|2
expr_stmt|;
comment|/* 		 * For legacy reasons, the "version" and "help" 		 * commands can be written with "--" prepended 		 * to make them look like flags. 		 */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"help"
argument_list|)
condition|)
break|break;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"version"
argument_list|)
condition|)
break|break;
comment|/* 		 * Check remaining flags (which by now must be 		 * "--exec-path", but maybe we will accept 		 * other arguments some day) 		 */
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|cmd
argument_list|,
literal|"exec-path"
argument_list|,
literal|9
argument_list|)
condition|)
block|{
name|cmd
operator|+=
literal|9
expr_stmt|;
if|if
condition|(
operator|*
name|cmd
operator|==
literal|'='
condition|)
block|{
name|git_set_exec_path
argument_list|(
name|cmd
operator|+
literal|1
argument_list|)
expr_stmt|;
continue|continue;
block|}
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
name|cmd_usage
argument_list|(
literal|0
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
name|argv
index|[
literal|0
index|]
operator|=
name|cmd
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
if|if
condition|(
name|errno
operator|==
name|ENOENT
condition|)
name|cmd_usage
argument_list|(
literal|0
argument_list|,
name|exec_path
argument_list|,
literal|"'%s' is not a git-command"
argument_list|,
name|cmd
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Failed to run command '%s': %s\n"
argument_list|,
name|git_command
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

