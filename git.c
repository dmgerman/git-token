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
name|cmdname_cnt
condition|;
control|)
block|{
name|int
name|c
decl_stmt|;
name|printf
argument_list|(
literal|"  "
argument_list|)
expr_stmt|;
for|for
control|(
name|c
operator|=
name|cols
init|;
name|c
operator|&&
name|i
operator|<
name|cmdname_cnt
condition|;
name|i
operator|++
control|)
block|{
name|printf
argument_list|(
literal|"%s"
argument_list|,
name|cmdname
index|[
name|i
index|]
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|--
name|c
condition|)
name|mput_char
argument_list|(
literal|' '
argument_list|,
name|space
operator|-
name|cmdname
index|[
name|i
index|]
operator|->
name|len
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

begin_ifdef
ifdef|#
directive|ifdef
name|__GNUC__
end_ifdef

begin_function_decl
specifier|static
name|void
name|usage
parameter_list|(
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
function_decl|2
operator|,
function_decl|3
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
DECL|function|usage
specifier|static
name|void
name|usage
parameter_list|(
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
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
if|if
condition|(
name|exec_path
condition|)
name|list_commands
argument_list|(
name|exec_path
argument_list|,
literal|"git-*"
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
name|path
index|[
name|path_len
operator|+
literal|1
index|]
operator|=
literal|'\0'
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
name|char
modifier|*
name|git_cmd
parameter_list|)
block|{
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
name|page
operator|=
name|malloc
argument_list|(
name|page_len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|page
argument_list|,
literal|"git-"
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|page
operator|+
literal|4
argument_list|,
name|git_cmd
argument_list|)
expr_stmt|;
name|page
index|[
name|page_len
index|]
operator|=
literal|0
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
parameter_list|,
name|char
modifier|*
modifier|*
name|envp
parameter_list|)
block|{
name|char
name|git_command
index|[
name|PATH_MAX
operator|+
literal|1
index|]
decl_stmt|;
name|char
name|wd
index|[
name|PATH_MAX
operator|+
literal|1
index|]
decl_stmt|;
name|int
name|i
decl_stmt|,
name|len
decl_stmt|,
name|show_help
init|=
literal|0
decl_stmt|;
name|char
modifier|*
name|exec_path
init|=
name|getenv
argument_list|(
literal|"GIT_EXEC_PATH"
argument_list|)
decl_stmt|;
name|getcwd
argument_list|(
name|wd
argument_list|,
name|PATH_MAX
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|exec_path
condition|)
name|exec_path
operator|=
name|GIT_EXEC_PATH
expr_stmt|;
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
control|)
block|{
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--"
argument_list|,
literal|2
argument_list|)
condition|)
break|break;
name|arg
operator|+=
literal|2
expr_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"exec-path"
argument_list|,
literal|9
argument_list|)
condition|)
block|{
name|arg
operator|+=
literal|9
expr_stmt|;
if|if
condition|(
operator|*
name|arg
operator|==
literal|'='
condition|)
name|exec_path
operator|=
name|arg
operator|+
literal|1
expr_stmt|;
else|else
block|{
name|puts
argument_list|(
name|exec_path
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
name|arg
argument_list|,
literal|"version"
argument_list|)
condition|)
block|{
name|printf
argument_list|(
literal|"git version %s\n"
argument_list|,
name|GIT_VERSION
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|0
argument_list|)
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
literal|"help"
argument_list|)
condition|)
name|show_help
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|show_help
condition|)
name|usage
argument_list|(
name|NULL
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|i
operator|>=
name|argc
operator|||
name|show_help
condition|)
block|{
if|if
condition|(
name|i
operator|>=
name|argc
condition|)
name|usage
argument_list|(
name|exec_path
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|show_man_page
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|exec_path
operator|!=
literal|'/'
condition|)
block|{
if|if
condition|(
operator|!
name|getcwd
argument_list|(
name|git_command
argument_list|,
sizeof|sizeof
argument_list|(
name|git_command
argument_list|)
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"git: cannot determine current directory"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
name|len
operator|=
name|strlen
argument_list|(
name|git_command
argument_list|)
expr_stmt|;
comment|/* Trivial cleanup */
while|while
condition|(
operator|!
name|strncmp
argument_list|(
name|exec_path
argument_list|,
literal|"./"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
name|exec_path
operator|+=
literal|2
expr_stmt|;
while|while
condition|(
operator|*
name|exec_path
operator|==
literal|'/'
condition|)
name|exec_path
operator|++
expr_stmt|;
block|}
name|snprintf
argument_list|(
name|git_command
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|git_command
argument_list|)
operator|-
name|len
argument_list|,
literal|"/%s"
argument_list|,
name|exec_path
argument_list|)
expr_stmt|;
block|}
else|else
name|strcpy
argument_list|(
name|git_command
argument_list|,
name|exec_path
argument_list|)
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|git_command
argument_list|)
expr_stmt|;
name|prepend_to_path
argument_list|(
name|git_command
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|strncat
argument_list|(
operator|&
name|git_command
index|[
name|len
index|]
argument_list|,
literal|"/git-"
argument_list|,
sizeof|sizeof
argument_list|(
name|git_command
argument_list|)
operator|-
name|len
argument_list|)
expr_stmt|;
name|len
operator|+=
literal|5
expr_stmt|;
name|strncat
argument_list|(
operator|&
name|git_command
index|[
name|len
index|]
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
sizeof|sizeof
argument_list|(
name|git_command
argument_list|)
operator|-
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|access
argument_list|(
name|git_command
argument_list|,
name|X_OK
argument_list|)
condition|)
name|usage
argument_list|(
name|exec_path
argument_list|,
literal|"'%s' is not a git-command"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
comment|/* execve() can only ever return if it fails */
name|execve
argument_list|(
name|git_command
argument_list|,
operator|&
name|argv
index|[
name|i
index|]
argument_list|,
name|envp
argument_list|)
expr_stmt|;
name|printf
argument_list|(
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

