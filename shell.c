begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_define
DECL|macro|COMMAND_DIR
define|#
directive|define
name|COMMAND_DIR
value|"git-shell-commands"
end_define

begin_define
DECL|macro|HELP_COMMAND
define|#
directive|define
name|HELP_COMMAND
value|COMMAND_DIR "/help"
end_define

begin_function
DECL|function|do_generic_cmd
specifier|static
name|int
name|do_generic_cmd
parameter_list|(
specifier|const
name|char
modifier|*
name|me
parameter_list|,
name|char
modifier|*
name|arg
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|my_argv
index|[
literal|4
index|]
decl_stmt|;
name|setup_path
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|arg
operator|||
operator|!
operator|(
name|arg
operator|=
name|sq_dequote
argument_list|(
name|arg
argument_list|)
operator|)
condition|)
name|die
argument_list|(
literal|"bad argument"
argument_list|)
expr_stmt|;
if|if
condition|(
name|prefixcmp
argument_list|(
name|me
argument_list|,
literal|"git-"
argument_list|)
condition|)
name|die
argument_list|(
literal|"bad command"
argument_list|)
expr_stmt|;
name|my_argv
index|[
literal|0
index|]
operator|=
name|me
operator|+
literal|4
expr_stmt|;
name|my_argv
index|[
literal|1
index|]
operator|=
name|arg
expr_stmt|;
name|my_argv
index|[
literal|2
index|]
operator|=
name|NULL
expr_stmt|;
return|return
name|execv_git_cmd
argument_list|(
name|my_argv
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|do_cvs_cmd
specifier|static
name|int
name|do_cvs_cmd
parameter_list|(
specifier|const
name|char
modifier|*
name|me
parameter_list|,
name|char
modifier|*
name|arg
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|cvsserver_argv
index|[
literal|3
index|]
init|=
block|{
literal|"cvsserver"
block|,
literal|"server"
block|,
name|NULL
block|}
decl_stmt|;
if|if
condition|(
operator|!
name|arg
operator|||
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"server"
argument_list|)
condition|)
name|die
argument_list|(
literal|"git-cvsserver only handles server: %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|setup_path
argument_list|()
expr_stmt|;
return|return
name|execv_git_cmd
argument_list|(
name|cvsserver_argv
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|is_valid_cmd_name
specifier|static
name|int
name|is_valid_cmd_name
parameter_list|(
specifier|const
name|char
modifier|*
name|cmd
parameter_list|)
block|{
comment|/* Test command contains no . or / characters */
return|return
name|cmd
index|[
name|strcspn
argument_list|(
name|cmd
argument_list|,
literal|"./"
argument_list|)
index|]
operator|==
literal|'\0'
return|;
block|}
end_function

begin_function
DECL|function|make_cmd
specifier|static
name|char
modifier|*
name|make_cmd
parameter_list|(
specifier|const
name|char
modifier|*
name|prog
parameter_list|)
block|{
name|char
modifier|*
name|prefix
init|=
name|xmalloc
argument_list|(
operator|(
name|strlen
argument_list|(
name|prog
argument_list|)
operator|+
name|strlen
argument_list|(
name|COMMAND_DIR
argument_list|)
operator|+
literal|2
operator|)
argument_list|)
decl_stmt|;
name|strcpy
argument_list|(
name|prefix
argument_list|,
name|COMMAND_DIR
argument_list|)
expr_stmt|;
name|strcat
argument_list|(
name|prefix
argument_list|,
literal|"/"
argument_list|)
expr_stmt|;
name|strcat
argument_list|(
name|prefix
argument_list|,
name|prog
argument_list|)
expr_stmt|;
return|return
name|prefix
return|;
block|}
end_function

begin_function
DECL|function|cd_to_homedir
specifier|static
name|void
name|cd_to_homedir
parameter_list|(
name|void
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|home
init|=
name|getenv
argument_list|(
literal|"HOME"
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|home
condition|)
name|die
argument_list|(
literal|"could not determine user's home directory; HOME is unset"
argument_list|)
expr_stmt|;
if|if
condition|(
name|chdir
argument_list|(
name|home
argument_list|)
operator|==
operator|-
literal|1
condition|)
name|die
argument_list|(
literal|"could not chdir to user's home directory"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|run_shell
specifier|static
name|void
name|run_shell
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|done
init|=
literal|0
decl_stmt|;
specifier|static
specifier|const
name|char
modifier|*
name|help_argv
index|[]
init|=
block|{
name|HELP_COMMAND
block|,
name|NULL
block|}
decl_stmt|;
comment|/* Print help if enabled */
name|run_command_v_opt
argument_list|(
name|help_argv
argument_list|,
name|RUN_SILENT_EXEC_FAILURE
argument_list|)
expr_stmt|;
do|do
block|{
name|struct
name|strbuf
name|line
init|=
name|STRBUF_INIT
decl_stmt|;
specifier|const
name|char
modifier|*
name|prog
decl_stmt|;
name|char
modifier|*
name|full_cmd
decl_stmt|;
name|char
modifier|*
name|rawargs
decl_stmt|;
name|char
modifier|*
name|split_args
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
name|int
name|code
decl_stmt|;
name|int
name|count
decl_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"git> "
argument_list|)
expr_stmt|;
if|if
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|line
argument_list|,
name|stdin
argument_list|,
literal|'\n'
argument_list|)
operator|==
name|EOF
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"\n"
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|line
argument_list|)
expr_stmt|;
break|break;
block|}
name|strbuf_trim
argument_list|(
operator|&
name|line
argument_list|)
expr_stmt|;
name|rawargs
operator|=
name|strbuf_detach
argument_list|(
operator|&
name|line
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|split_args
operator|=
name|xstrdup
argument_list|(
name|rawargs
argument_list|)
expr_stmt|;
name|count
operator|=
name|split_cmdline
argument_list|(
name|split_args
argument_list|,
operator|&
name|argv
argument_list|)
expr_stmt|;
if|if
condition|(
name|count
operator|<
literal|0
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"invalid command format '%s': %s\n"
argument_list|,
name|rawargs
argument_list|,
name|split_cmdline_strerror
argument_list|(
name|count
argument_list|)
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|split_args
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|rawargs
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|prog
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|prog
argument_list|,
literal|""
argument_list|)
condition|)
block|{ 		}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|prog
argument_list|,
literal|"quit"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|prog
argument_list|,
literal|"logout"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|prog
argument_list|,
literal|"exit"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|prog
argument_list|,
literal|"bye"
argument_list|)
condition|)
block|{
name|done
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|is_valid_cmd_name
argument_list|(
name|prog
argument_list|)
condition|)
block|{
name|full_cmd
operator|=
name|make_cmd
argument_list|(
name|prog
argument_list|)
expr_stmt|;
name|argv
index|[
literal|0
index|]
operator|=
name|full_cmd
expr_stmt|;
name|code
operator|=
name|run_command_v_opt
argument_list|(
name|argv
argument_list|,
name|RUN_SILENT_EXEC_FAILURE
argument_list|)
expr_stmt|;
if|if
condition|(
name|code
operator|==
operator|-
literal|1
operator|&&
name|errno
operator|==
name|ENOENT
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"unrecognized command '%s'\n"
argument_list|,
name|prog
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|full_cmd
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"invalid command format '%s'\n"
argument_list|,
name|prog
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|argv
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|rawargs
argument_list|)
expr_stmt|;
block|}
do|while
condition|(
operator|!
name|done
condition|)
do|;
block|}
end_function

begin_struct
DECL|struct|commands
specifier|static
struct|struct
name|commands
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|exec
name|int
function_decl|(
modifier|*
name|exec
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|me
parameter_list|,
name|char
modifier|*
name|arg
parameter_list|)
function_decl|;
DECL|variable|cmd_list
block|}
name|cmd_list
index|[]
init|=
block|{
block|{
literal|"git-receive-pack"
block|,
name|do_generic_cmd
block|}
block|,
block|{
literal|"git-upload-pack"
block|,
name|do_generic_cmd
block|}
block|,
block|{
literal|"git-upload-archive"
block|,
name|do_generic_cmd
block|}
block|,
block|{
literal|"cvs"
block|,
name|do_cvs_cmd
block|}
block|,
block|{
name|NULL
block|}
block|, }
struct|;
end_struct

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
name|char
modifier|*
name|prog
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|user_argv
decl_stmt|;
name|struct
name|commands
modifier|*
name|cmd
decl_stmt|;
name|int
name|devnull_fd
decl_stmt|;
name|int
name|count
decl_stmt|;
name|git_setup_gettext
argument_list|()
expr_stmt|;
name|git_extract_argv0_path
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
comment|/* 	 * Always open file descriptors 0/1/2 to avoid clobbering files 	 * in die().  It also avoids not messing up when the pipes are 	 * dup'ed onto stdin/stdout/stderr in the child processes we spawn. 	 */
name|devnull_fd
operator|=
name|open
argument_list|(
literal|"/dev/null"
argument_list|,
name|O_RDWR
argument_list|)
expr_stmt|;
while|while
condition|(
name|devnull_fd
operator|>=
literal|0
operator|&&
name|devnull_fd
operator|<=
literal|2
condition|)
name|devnull_fd
operator|=
name|dup
argument_list|(
name|devnull_fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|devnull_fd
operator|==
operator|-
literal|1
condition|)
name|die_errno
argument_list|(
literal|"opening /dev/null failed"
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|devnull_fd
argument_list|)
expr_stmt|;
comment|/* 	 * Special hack to pretend to be a CVS server 	 */
if|if
condition|(
name|argc
operator|==
literal|2
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"cvs server"
argument_list|)
condition|)
block|{
name|argv
operator|--
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argc
operator|==
literal|1
condition|)
block|{
comment|/* Allow the user to run an interactive shell */
name|cd_to_homedir
argument_list|()
expr_stmt|;
if|if
condition|(
name|access
argument_list|(
name|COMMAND_DIR
argument_list|,
name|R_OK
operator||
name|X_OK
argument_list|)
operator|==
operator|-
literal|1
condition|)
block|{
name|die
argument_list|(
literal|"Interactive git shell is not enabled.\n"
literal|"hint: ~/"
name|COMMAND_DIR
literal|" should exist "
literal|"and have read and execute access."
argument_list|)
expr_stmt|;
block|}
name|run_shell
argument_list|()
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
name|argc
operator|!=
literal|3
operator|||
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-c"
argument_list|)
condition|)
block|{
comment|/* 		 * We do not accept any other modes except "-c" followed by 		 * "cmd arg", where "cmd" is a very limited subset of git 		 * commands or a command in the COMMAND_DIR 		 */
name|die
argument_list|(
literal|"Run with no arguments or with -c cmd"
argument_list|)
expr_stmt|;
block|}
name|prog
operator|=
name|xstrdup
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|prog
argument_list|,
literal|"git"
argument_list|,
literal|3
argument_list|)
operator|&&
name|isspace
argument_list|(
name|prog
index|[
literal|3
index|]
argument_list|)
condition|)
comment|/* Accept "git foo" as if the caller said "git-foo". */
name|prog
index|[
literal|3
index|]
operator|=
literal|'-'
expr_stmt|;
for|for
control|(
name|cmd
operator|=
name|cmd_list
init|;
name|cmd
operator|->
name|name
condition|;
name|cmd
operator|++
control|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|cmd
operator|->
name|name
argument_list|)
decl_stmt|;
name|char
modifier|*
name|arg
decl_stmt|;
if|if
condition|(
name|strncmp
argument_list|(
name|cmd
operator|->
name|name
argument_list|,
name|prog
argument_list|,
name|len
argument_list|)
condition|)
continue|continue;
name|arg
operator|=
name|NULL
expr_stmt|;
switch|switch
condition|(
name|prog
index|[
name|len
index|]
condition|)
block|{
case|case
literal|'\0'
case|:
name|arg
operator|=
name|NULL
expr_stmt|;
break|break;
case|case
literal|' '
case|:
name|arg
operator|=
name|prog
operator|+
name|len
operator|+
literal|1
expr_stmt|;
break|break;
default|default:
continue|continue;
block|}
name|exit
argument_list|(
name|cmd
operator|->
name|exec
argument_list|(
name|cmd
operator|->
name|name
argument_list|,
name|arg
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|cd_to_homedir
argument_list|()
expr_stmt|;
name|count
operator|=
name|split_cmdline
argument_list|(
name|prog
argument_list|,
operator|&
name|user_argv
argument_list|)
expr_stmt|;
if|if
condition|(
name|count
operator|>=
literal|0
condition|)
block|{
if|if
condition|(
name|is_valid_cmd_name
argument_list|(
name|user_argv
index|[
literal|0
index|]
argument_list|)
condition|)
block|{
name|prog
operator|=
name|make_cmd
argument_list|(
name|user_argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|user_argv
index|[
literal|0
index|]
operator|=
name|prog
expr_stmt|;
name|execv
argument_list|(
name|user_argv
index|[
literal|0
index|]
argument_list|,
operator|(
name|char
operator|*
specifier|const
operator|*
operator|)
name|user_argv
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|prog
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|user_argv
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"unrecognized command '%s'"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|free
argument_list|(
name|prog
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"invalid command format '%s': %s"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|,
name|split_cmdline_strerror
argument_list|(
name|count
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit

