begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_decl_stmt
DECL|variable|error_handle
specifier|static
name|FILE
modifier|*
name|error_handle
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tweaked_error_buffering
specifier|static
name|int
name|tweaked_error_buffering
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|vreportf
name|void
name|vreportf
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
block|{
name|FILE
modifier|*
name|fh
init|=
name|error_handle
condition|?
name|error_handle
else|:
name|stderr
decl_stmt|;
name|fflush
argument_list|(
name|fh
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tweaked_error_buffering
condition|)
block|{
name|setvbuf
argument_list|(
name|fh
argument_list|,
name|NULL
argument_list|,
name|_IOLBF
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|tweaked_error_buffering
operator|=
literal|1
expr_stmt|;
block|}
name|fputs
argument_list|(
name|prefix
argument_list|,
name|fh
argument_list|)
expr_stmt|;
name|vfprintf
argument_list|(
name|fh
argument_list|,
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|fputc
argument_list|(
literal|'\n'
argument_list|,
name|fh
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|usage_builtin
specifier|static
name|NORETURN
name|void
name|usage_builtin
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
block|{
name|vreportf
argument_list|(
literal|"usage: "
argument_list|,
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|129
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|die_builtin
specifier|static
name|NORETURN
name|void
name|die_builtin
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
block|{
name|vreportf
argument_list|(
literal|"fatal: "
argument_list|,
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|error_builtin
specifier|static
name|void
name|error_builtin
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
block|{
name|vreportf
argument_list|(
literal|"error: "
argument_list|,
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|warn_builtin
specifier|static
name|void
name|warn_builtin
parameter_list|(
specifier|const
name|char
modifier|*
name|warn
parameter_list|,
name|va_list
name|params
parameter_list|)
block|{
name|vreportf
argument_list|(
literal|"warning: "
argument_list|,
name|warn
argument_list|,
name|params
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|die_is_recursing_builtin
specifier|static
name|int
name|die_is_recursing_builtin
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|int
name|dying
decl_stmt|;
return|return
name|dying
operator|++
return|;
block|}
end_function

begin_comment
comment|/* If we are in a dlopen()ed .so write to a global variable would segfault  * (ugh), so keep things static. */
end_comment

begin_function_decl
DECL|variable|usage_routine
specifier|static
name|NORETURN_PTR
name|void
function_decl|(
modifier|*
name|usage_routine
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
init|=
name|usage_builtin
function_decl|;
end_function_decl

begin_function_decl
DECL|variable|die_routine
specifier|static
name|NORETURN_PTR
name|void
function_decl|(
modifier|*
name|die_routine
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
init|=
name|die_builtin
function_decl|;
end_function_decl

begin_function_decl
DECL|variable|error_routine
specifier|static
name|void
function_decl|(
modifier|*
name|error_routine
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
init|=
name|error_builtin
function_decl|;
end_function_decl

begin_function_decl
DECL|variable|warn_routine
specifier|static
name|void
function_decl|(
modifier|*
name|warn_routine
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
init|=
name|warn_builtin
function_decl|;
end_function_decl

begin_function_decl
DECL|variable|die_is_recursing
specifier|static
name|int
function_decl|(
modifier|*
name|die_is_recursing
function_decl|)
parameter_list|(
name|void
parameter_list|)
init|=
name|die_is_recursing_builtin
function_decl|;
end_function_decl

begin_decl_stmt
DECL|function|set_die_routine
name|void
name|set_die_routine
argument_list|(
name|NORETURN_PTR
name|void
argument_list|(
argument|*routine
argument_list|)
operator|(
specifier|const
name|char
operator|*
name|err
operator|,
name|va_list
name|params
operator|)
argument_list|)
block|{
name|die_routine
operator|=
name|routine
expr_stmt|;
block|}
end_decl_stmt

begin_function
DECL|function|set_error_routine
name|void
name|set_error_routine
parameter_list|(
name|void
function_decl|(
modifier|*
name|routine
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
name|va_list
name|params
parameter_list|)
parameter_list|)
block|{
name|error_routine
operator|=
name|routine
expr_stmt|;
block|}
end_function

begin_function
DECL|function|set_die_is_recursing_routine
name|void
name|set_die_is_recursing_routine
parameter_list|(
name|int
function_decl|(
modifier|*
name|routine
function_decl|)
parameter_list|(
name|void
parameter_list|)
parameter_list|)
block|{
name|die_is_recursing
operator|=
name|routine
expr_stmt|;
block|}
end_function

begin_function
DECL|function|set_error_handle
name|void
name|set_error_handle
parameter_list|(
name|FILE
modifier|*
name|fh
parameter_list|)
block|{
name|error_handle
operator|=
name|fh
expr_stmt|;
name|tweaked_error_buffering
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|usagef
name|void
name|NORETURN
name|usagef
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|params
decl_stmt|;
name|va_start
argument_list|(
name|params
argument_list|,
name|err
argument_list|)
expr_stmt|;
name|usage_routine
argument_list|(
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|usage
name|void
name|NORETURN
name|usage
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|)
block|{
name|usagef
argument_list|(
literal|"%s"
argument_list|,
name|err
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|die
name|void
name|NORETURN
name|die
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|params
decl_stmt|;
if|if
condition|(
name|die_is_recursing
argument_list|()
condition|)
block|{
name|fputs
argument_list|(
literal|"fatal: recursion detected in die handler\n"
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
block|}
name|va_start
argument_list|(
name|params
argument_list|,
name|err
argument_list|)
expr_stmt|;
name|die_routine
argument_list|(
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|fmt_with_err
specifier|static
specifier|const
name|char
modifier|*
name|fmt_with_err
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|int
name|n
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|)
block|{
name|char
name|str_error
index|[
literal|256
index|]
decl_stmt|,
modifier|*
name|err
decl_stmt|;
name|int
name|i
decl_stmt|,
name|j
decl_stmt|;
name|err
operator|=
name|strerror
argument_list|(
name|errno
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
name|j
operator|=
literal|0
init|;
name|err
index|[
name|i
index|]
operator|&&
name|j
operator|<
sizeof|sizeof
argument_list|(
name|str_error
argument_list|)
operator|-
literal|1
condition|;
control|)
block|{
if|if
condition|(
operator|(
name|str_error
index|[
name|j
operator|++
index|]
operator|=
name|err
index|[
name|i
operator|++
index|]
operator|)
operator|!=
literal|'%'
condition|)
continue|continue;
if|if
condition|(
name|j
operator|<
sizeof|sizeof
argument_list|(
name|str_error
argument_list|)
operator|-
literal|1
condition|)
block|{
name|str_error
index|[
name|j
operator|++
index|]
operator|=
literal|'%'
expr_stmt|;
block|}
else|else
block|{
comment|/* No room to double the '%', so we overwrite it with 			 * '\0' below */
name|j
operator|--
expr_stmt|;
break|break;
block|}
block|}
name|str_error
index|[
name|j
index|]
operator|=
literal|0
expr_stmt|;
name|snprintf
argument_list|(
name|buf
argument_list|,
name|n
argument_list|,
literal|"%s: %s"
argument_list|,
name|fmt
argument_list|,
name|str_error
argument_list|)
expr_stmt|;
return|return
name|buf
return|;
block|}
end_function

begin_function
DECL|function|die_errno
name|void
name|NORETURN
name|die_errno
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|char
name|buf
index|[
literal|1024
index|]
decl_stmt|;
name|va_list
name|params
decl_stmt|;
if|if
condition|(
name|die_is_recursing
argument_list|()
condition|)
block|{
name|fputs
argument_list|(
literal|"fatal: recursion detected in die_errno handler\n"
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
block|}
name|va_start
argument_list|(
name|params
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|die_routine
argument_list|(
name|fmt_with_err
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
name|fmt
argument_list|)
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
block|}
end_function

begin_undef
DECL|macro|error_errno
undef|#
directive|undef
name|error_errno
end_undef

begin_function
DECL|function|error_errno
name|int
name|error_errno
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|char
name|buf
index|[
literal|1024
index|]
decl_stmt|;
name|va_list
name|params
decl_stmt|;
name|va_start
argument_list|(
name|params
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|error_routine
argument_list|(
name|fmt_with_err
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
name|fmt
argument_list|)
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_undef
DECL|macro|error
undef|#
directive|undef
name|error
end_undef

begin_function
DECL|function|error
name|int
name|error
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|params
decl_stmt|;
name|va_start
argument_list|(
name|params
argument_list|,
name|err
argument_list|)
expr_stmt|;
name|error_routine
argument_list|(
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|warning_errno
name|void
name|warning_errno
parameter_list|(
specifier|const
name|char
modifier|*
name|warn
parameter_list|,
modifier|...
parameter_list|)
block|{
name|char
name|buf
index|[
literal|1024
index|]
decl_stmt|;
name|va_list
name|params
decl_stmt|;
name|va_start
argument_list|(
name|params
argument_list|,
name|warn
argument_list|)
expr_stmt|;
name|warn_routine
argument_list|(
name|fmt_with_err
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
name|warn
argument_list|)
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|warning
name|void
name|warning
parameter_list|(
specifier|const
name|char
modifier|*
name|warn
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|params
decl_stmt|;
name|va_start
argument_list|(
name|params
argument_list|,
name|warn
argument_list|)
expr_stmt|;
name|warn_routine
argument_list|(
name|warn
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

