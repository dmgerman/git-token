begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_function
DECL|function|report
specifier|static
name|void
name|report
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
name|fputs
argument_list|(
name|prefix
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
name|vfprintf
argument_list|(
name|stderr
argument_list|,
name|err
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
literal|"\n"
argument_list|,
name|stderr
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
parameter_list|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"usage: %s\n"
argument_list|,
name|err
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
name|report
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
name|report
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
name|report
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

begin_comment
comment|/* If we are in a dlopen()ed .so write to a global variable would segfault  * (ugh), so keep things static. */
end_comment

begin_expr_stmt
specifier|static
name|void
argument_list|(
argument|*usage_routine
argument_list|)
operator|(
specifier|const
name|char
operator|*
name|err
operator|)
name|NORETURN
operator|=
name|usage_builtin
expr_stmt|;
end_expr_stmt

begin_expr_stmt
specifier|static
name|void
argument_list|(
argument|*die_routine
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
name|NORETURN
operator|=
name|die_builtin
expr_stmt|;
end_expr_stmt

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

begin_decl_stmt
DECL|function|set_usage_routine
name|void
name|set_usage_routine
argument_list|(
name|void
argument_list|(
argument|*routine
argument_list|)
operator|(
specifier|const
name|char
operator|*
name|err
operator|)
name|NORETURN
argument_list|)
block|{
name|usage_routine
operator|=
name|routine
expr_stmt|;
block|}
end_decl_stmt

begin_decl_stmt
DECL|function|set_die_routine
name|void
name|set_die_routine
argument_list|(
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
name|NORETURN
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
DECL|function|set_warn_routine
name|void
name|set_warn_routine
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
name|warn
parameter_list|,
name|va_list
name|params
parameter_list|)
parameter_list|)
block|{
name|warn_routine
operator|=
name|routine
expr_stmt|;
block|}
end_function

begin_function
DECL|function|usage
name|void
name|usage
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|)
block|{
name|usage_routine
argument_list|(
name|err
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|die
name|void
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

