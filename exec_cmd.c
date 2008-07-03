begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_define
DECL|macro|MAX_ARGS
define|#
directive|define
name|MAX_ARGS
value|32
end_define

begin_decl_stmt
specifier|extern
name|char
modifier|*
modifier|*
name|environ
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|argv_exec_path
specifier|static
specifier|const
name|char
modifier|*
name|argv_exec_path
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|builtin_exec_path
specifier|static
specifier|const
name|char
modifier|*
name|builtin_exec_path
parameter_list|(
name|void
parameter_list|)
block|{
ifndef|#
directive|ifndef
name|__MINGW32__
return|return
name|GIT_EXEC_PATH
return|;
else|#
directive|else
name|int
name|len
decl_stmt|;
name|char
modifier|*
name|p
decl_stmt|,
modifier|*
name|q
decl_stmt|,
modifier|*
name|sl
decl_stmt|;
specifier|static
name|char
modifier|*
name|ep
decl_stmt|;
if|if
condition|(
name|ep
condition|)
return|return
name|ep
return|;
name|len
operator|=
name|strlen
argument_list|(
name|_pgmptr
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|<
literal|2
condition|)
return|return
name|ep
operator|=
literal|"."
return|;
name|p
operator|=
name|ep
operator|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|q
operator|=
name|_pgmptr
expr_stmt|;
name|sl
operator|=
name|NULL
expr_stmt|;
comment|/* copy program name, turn '\\' into '/', skip last part */
while|while
condition|(
operator|(
operator|*
name|p
operator|=
operator|*
name|q
operator|)
condition|)
block|{
if|if
condition|(
operator|*
name|q
operator|==
literal|'\\'
operator|||
operator|*
name|q
operator|==
literal|'/'
condition|)
block|{
operator|*
name|p
operator|=
literal|'/'
expr_stmt|;
name|sl
operator|=
name|p
expr_stmt|;
block|}
name|p
operator|++
operator|,
name|q
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|sl
condition|)
operator|*
name|sl
operator|=
literal|'\0'
expr_stmt|;
else|else
name|ep
index|[
literal|0
index|]
operator|=
literal|'.'
operator|,
name|ep
index|[
literal|1
index|]
operator|=
literal|'\0'
expr_stmt|;
return|return
name|ep
return|;
endif|#
directive|endif
block|}
end_function

begin_function
DECL|function|git_set_argv_exec_path
name|void
name|git_set_argv_exec_path
parameter_list|(
specifier|const
name|char
modifier|*
name|exec_path
parameter_list|)
block|{
name|argv_exec_path
operator|=
name|exec_path
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Returns the highest-priority, location to look for git programs. */
end_comment

begin_function
DECL|function|git_exec_path
specifier|const
name|char
modifier|*
name|git_exec_path
parameter_list|(
name|void
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|env
decl_stmt|;
if|if
condition|(
name|argv_exec_path
condition|)
return|return
name|argv_exec_path
return|;
name|env
operator|=
name|getenv
argument_list|(
name|EXEC_PATH_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
name|env
operator|&&
operator|*
name|env
condition|)
block|{
return|return
name|env
return|;
block|}
return|return
name|builtin_exec_path
argument_list|()
return|;
block|}
end_function

begin_function
DECL|function|add_path
specifier|static
name|void
name|add_path
parameter_list|(
name|struct
name|strbuf
modifier|*
name|out
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
if|if
condition|(
name|path
operator|&&
operator|*
name|path
condition|)
block|{
if|if
condition|(
name|is_absolute_path
argument_list|(
name|path
argument_list|)
condition|)
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
name|path
argument_list|)
expr_stmt|;
else|else
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
name|make_absolute_path
argument_list|(
name|path
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
name|out
argument_list|,
name|PATH_SEP
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|setup_path
name|void
name|setup_path
parameter_list|(
specifier|const
name|char
modifier|*
name|cmd_path
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
name|struct
name|strbuf
name|new_path
decl_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|new_path
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|add_path
argument_list|(
operator|&
name|new_path
argument_list|,
name|argv_exec_path
argument_list|)
expr_stmt|;
name|add_path
argument_list|(
operator|&
name|new_path
argument_list|,
name|getenv
argument_list|(
name|EXEC_PATH_ENVIRONMENT
argument_list|)
argument_list|)
expr_stmt|;
name|add_path
argument_list|(
operator|&
name|new_path
argument_list|,
name|builtin_exec_path
argument_list|()
argument_list|)
expr_stmt|;
name|add_path
argument_list|(
operator|&
name|new_path
argument_list|,
name|cmd_path
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
else|else
name|strbuf_addstr
argument_list|(
operator|&
name|new_path
argument_list|,
literal|"/usr/local/bin:/usr/bin:/bin"
argument_list|)
expr_stmt|;
name|setenv
argument_list|(
literal|"PATH"
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
DECL|function|execv_git_cmd
name|int
name|execv_git_cmd
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|int
name|argc
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|nargv
decl_stmt|;
for|for
control|(
name|argc
operator|=
literal|0
init|;
name|argv
index|[
name|argc
index|]
condition|;
name|argc
operator|++
control|)
empty_stmt|;
comment|/* just counting */
name|nargv
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|nargv
argument_list|)
operator|*
operator|(
name|argc
operator|+
literal|2
operator|)
argument_list|)
expr_stmt|;
name|nargv
index|[
literal|0
index|]
operator|=
literal|"git"
expr_stmt|;
for|for
control|(
name|argc
operator|=
literal|0
init|;
name|argv
index|[
name|argc
index|]
condition|;
name|argc
operator|++
control|)
name|nargv
index|[
name|argc
operator|+
literal|1
index|]
operator|=
name|argv
index|[
name|argc
index|]
expr_stmt|;
name|nargv
index|[
name|argc
operator|+
literal|1
index|]
operator|=
name|NULL
expr_stmt|;
name|trace_argv_printf
argument_list|(
name|nargv
argument_list|,
literal|"trace: exec:"
argument_list|)
expr_stmt|;
comment|/* execvp() can only ever return if it fails */
name|execvp
argument_list|(
literal|"git"
argument_list|,
operator|(
name|char
operator|*
operator|*
operator|)
name|nargv
argument_list|)
expr_stmt|;
name|trace_printf
argument_list|(
literal|"trace: exec failed: %s\n"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|nargv
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|execl_git_cmd
name|int
name|execl_git_cmd
parameter_list|(
specifier|const
name|char
modifier|*
name|cmd
parameter_list|,
modifier|...
parameter_list|)
block|{
name|int
name|argc
decl_stmt|;
specifier|const
name|char
modifier|*
name|argv
index|[
name|MAX_ARGS
operator|+
literal|1
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|arg
decl_stmt|;
name|va_list
name|param
decl_stmt|;
name|va_start
argument_list|(
name|param
argument_list|,
name|cmd
argument_list|)
expr_stmt|;
name|argv
index|[
literal|0
index|]
operator|=
name|cmd
expr_stmt|;
name|argc
operator|=
literal|1
expr_stmt|;
while|while
condition|(
name|argc
operator|<
name|MAX_ARGS
condition|)
block|{
name|arg
operator|=
name|argv
index|[
name|argc
operator|++
index|]
operator|=
name|va_arg
argument_list|(
name|param
argument_list|,
name|char
operator|*
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|arg
condition|)
break|break;
block|}
name|va_end
argument_list|(
name|param
argument_list|)
expr_stmt|;
if|if
condition|(
name|MAX_ARGS
operator|<=
name|argc
condition|)
return|return
name|error
argument_list|(
literal|"too many args to run %s"
argument_list|,
name|cmd
argument_list|)
return|;
name|argv
index|[
name|argc
index|]
operator|=
name|NULL
expr_stmt|;
return|return
name|execv_git_cmd
argument_list|(
name|argv
argument_list|)
return|;
block|}
end_function

end_unit

