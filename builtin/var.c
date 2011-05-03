begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Eric Biederman, 2005  */
end_comment

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_decl_stmt
DECL|variable|var_usage
specifier|static
specifier|const
name|char
name|var_usage
index|[]
init|=
literal|"git var (-l |<variable>)"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|editor
specifier|static
specifier|const
name|char
modifier|*
name|editor
parameter_list|(
name|int
name|flag
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|pgm
init|=
name|git_editor
argument_list|()
decl_stmt|;
if|if
condition|(
operator|!
name|pgm
operator|&&
name|flag
operator|&
name|IDENT_ERROR_ON_NO_NAME
condition|)
name|die
argument_list|(
literal|"Terminal is dumb, but EDITOR unset"
argument_list|)
expr_stmt|;
return|return
name|pgm
return|;
block|}
end_function

begin_function
DECL|function|pager
specifier|static
specifier|const
name|char
modifier|*
name|pager
parameter_list|(
name|int
name|flag
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|pgm
init|=
name|git_pager
argument_list|(
literal|1
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|pgm
condition|)
name|pgm
operator|=
literal|"cat"
expr_stmt|;
return|return
name|pgm
return|;
block|}
end_function

begin_struct
DECL|struct|git_var
struct|struct
name|git_var
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|read
specifier|const
name|char
modifier|*
function_decl|(
modifier|*
name|read
function_decl|)
parameter_list|(
name|int
parameter_list|)
function_decl|;
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|git_vars
specifier|static
name|struct
name|git_var
name|git_vars
index|[]
init|=
block|{
block|{
literal|"GIT_COMMITTER_IDENT"
block|,
name|git_committer_info
block|}
block|,
block|{
literal|"GIT_AUTHOR_IDENT"
block|,
name|git_author_info
block|}
block|,
block|{
literal|"GIT_EDITOR"
block|,
name|editor
block|}
block|,
block|{
literal|"GIT_PAGER"
block|,
name|pager
block|}
block|,
block|{
literal|""
block|,
name|NULL
block|}
block|, }
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|list_vars
specifier|static
name|void
name|list_vars
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|git_var
modifier|*
name|ptr
decl_stmt|;
specifier|const
name|char
modifier|*
name|val
decl_stmt|;
for|for
control|(
name|ptr
operator|=
name|git_vars
init|;
name|ptr
operator|->
name|read
condition|;
name|ptr
operator|++
control|)
if|if
condition|(
operator|(
name|val
operator|=
name|ptr
operator|->
name|read
argument_list|(
literal|0
argument_list|)
operator|)
condition|)
name|printf
argument_list|(
literal|"%s=%s\n"
argument_list|,
name|ptr
operator|->
name|name
argument_list|,
name|val
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|read_var
specifier|static
specifier|const
name|char
modifier|*
name|read_var
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|)
block|{
name|struct
name|git_var
modifier|*
name|ptr
decl_stmt|;
specifier|const
name|char
modifier|*
name|val
decl_stmt|;
name|val
operator|=
name|NULL
expr_stmt|;
for|for
control|(
name|ptr
operator|=
name|git_vars
init|;
name|ptr
operator|->
name|read
condition|;
name|ptr
operator|++
control|)
block|{
if|if
condition|(
name|strcmp
argument_list|(
name|var
argument_list|,
name|ptr
operator|->
name|name
argument_list|)
operator|==
literal|0
condition|)
block|{
name|val
operator|=
name|ptr
operator|->
name|read
argument_list|(
name|IDENT_ERROR_ON_NO_NAME
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
return|return
name|val
return|;
block|}
end_function

begin_function
DECL|function|show_config
specifier|static
name|int
name|show_config
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
name|value
condition|)
name|printf
argument_list|(
literal|"%s=%s\n"
argument_list|,
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|var
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
DECL|function|cmd_var
name|int
name|cmd_var
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
name|val
init|=
name|NULL
decl_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|2
condition|)
name|usage
argument_list|(
name|var_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-l"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|git_config
argument_list|(
name|show_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|list_vars
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|val
operator|=
name|read_var
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|val
condition|)
name|usage
argument_list|(
name|var_usage
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|val
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

