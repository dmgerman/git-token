begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<ctype.h>
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_define
DECL|macro|MAXNAME
define|#
directive|define
name|MAXNAME
value|(256)
end_define

begin_decl_stmt
DECL|variable|config_file
specifier|static
name|FILE
modifier|*
name|config_file
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|config_linenr
specifier|static
name|int
name|config_linenr
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|get_next_char
specifier|static
name|int
name|get_next_char
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|c
decl_stmt|;
name|FILE
modifier|*
name|f
decl_stmt|;
name|c
operator|=
literal|'\n'
expr_stmt|;
if|if
condition|(
operator|(
name|f
operator|=
name|config_file
operator|)
operator|!=
name|NULL
condition|)
block|{
name|c
operator|=
name|fgetc
argument_list|(
name|f
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|==
literal|'\n'
condition|)
name|config_linenr
operator|++
expr_stmt|;
if|if
condition|(
name|c
operator|==
name|EOF
condition|)
block|{
name|config_file
operator|=
name|NULL
expr_stmt|;
name|c
operator|=
literal|'\n'
expr_stmt|;
block|}
block|}
return|return
name|c
return|;
block|}
end_function

begin_function
DECL|function|parse_value
specifier|static
name|char
modifier|*
name|parse_value
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|char
name|value
index|[
literal|1024
index|]
decl_stmt|;
name|int
name|quote
init|=
literal|0
decl_stmt|,
name|comment
init|=
literal|0
decl_stmt|,
name|len
init|=
literal|0
decl_stmt|,
name|space
init|=
literal|0
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|c
init|=
name|get_next_char
argument_list|()
decl_stmt|;
if|if
condition|(
name|len
operator|>=
sizeof|sizeof
argument_list|(
name|value
argument_list|)
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|c
operator|==
literal|'\n'
condition|)
block|{
if|if
condition|(
name|quote
condition|)
return|return
name|NULL
return|;
name|value
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
return|return
name|value
return|;
block|}
if|if
condition|(
name|comment
condition|)
continue|continue;
if|if
condition|(
name|isspace
argument_list|(
name|c
argument_list|)
operator|&&
operator|!
name|quote
condition|)
block|{
name|space
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|space
condition|)
block|{
if|if
condition|(
name|len
condition|)
name|value
index|[
name|len
operator|++
index|]
operator|=
literal|' '
expr_stmt|;
name|space
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|c
operator|==
literal|'\\'
condition|)
block|{
name|c
operator|=
name|get_next_char
argument_list|()
expr_stmt|;
switch|switch
condition|(
name|c
condition|)
block|{
case|case
literal|'\n'
case|:
continue|continue;
case|case
literal|'t'
case|:
name|c
operator|=
literal|'\t'
expr_stmt|;
break|break;
case|case
literal|'b'
case|:
name|c
operator|=
literal|'\b'
expr_stmt|;
break|break;
case|case
literal|'n'
case|:
name|c
operator|=
literal|'\n'
expr_stmt|;
break|break;
return|return
name|NULL
return|;
block|}
name|value
index|[
name|len
operator|++
index|]
operator|=
name|c
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|c
operator|==
literal|'"'
condition|)
block|{
name|quote
operator|=
literal|1
operator|-
name|quote
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|quote
condition|)
block|{
if|if
condition|(
name|c
operator|==
literal|';'
operator|||
name|c
operator|==
literal|'#'
condition|)
block|{
name|comment
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
block|}
name|value
index|[
name|len
operator|++
index|]
operator|=
name|c
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|get_value
specifier|static
name|int
name|get_value
parameter_list|(
name|config_fn_t
name|fn
parameter_list|,
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|int
name|len
parameter_list|)
block|{
name|int
name|c
decl_stmt|;
name|char
modifier|*
name|value
decl_stmt|;
comment|/* Get the full name */
for|for
control|(
init|;
condition|;
control|)
block|{
name|c
operator|=
name|get_next_char
argument_list|()
expr_stmt|;
if|if
condition|(
name|c
operator|==
name|EOF
condition|)
break|break;
if|if
condition|(
operator|!
name|isalnum
argument_list|(
name|c
argument_list|)
condition|)
break|break;
name|name
index|[
name|len
operator|++
index|]
operator|=
name|tolower
argument_list|(
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>=
name|MAXNAME
condition|)
return|return
operator|-
literal|1
return|;
block|}
name|name
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|c
operator|==
literal|' '
operator|||
name|c
operator|==
literal|'\t'
condition|)
name|c
operator|=
name|get_next_char
argument_list|()
expr_stmt|;
name|value
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|c
operator|!=
literal|'\n'
condition|)
block|{
if|if
condition|(
name|c
operator|!=
literal|'='
condition|)
return|return
operator|-
literal|1
return|;
name|value
operator|=
name|parse_value
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|value
condition|)
return|return
operator|-
literal|1
return|;
block|}
return|return
name|fn
argument_list|(
name|name
argument_list|,
name|value
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|get_base_var
specifier|static
name|int
name|get_base_var
parameter_list|(
name|char
modifier|*
name|name
parameter_list|)
block|{
name|int
name|baselen
init|=
literal|0
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|c
init|=
name|get_next_char
argument_list|()
decl_stmt|;
if|if
condition|(
name|c
operator|==
name|EOF
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|c
operator|==
literal|']'
condition|)
return|return
name|baselen
return|;
if|if
condition|(
operator|!
name|isalnum
argument_list|(
name|c
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|baselen
operator|>
name|MAXNAME
operator|/
literal|2
condition|)
return|return
operator|-
literal|1
return|;
name|name
index|[
name|baselen
operator|++
index|]
operator|=
name|tolower
argument_list|(
name|c
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|git_parse_file
specifier|static
name|int
name|git_parse_file
parameter_list|(
name|config_fn_t
name|fn
parameter_list|)
block|{
name|int
name|comment
init|=
literal|0
decl_stmt|;
name|int
name|baselen
init|=
literal|0
decl_stmt|;
specifier|static
name|char
name|var
index|[
name|MAXNAME
index|]
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|c
init|=
name|get_next_char
argument_list|()
decl_stmt|;
if|if
condition|(
name|c
operator|==
literal|'\n'
condition|)
block|{
comment|/* EOF? */
if|if
condition|(
operator|!
name|config_file
condition|)
return|return
literal|0
return|;
name|comment
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|comment
operator|||
name|isspace
argument_list|(
name|c
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|c
operator|==
literal|'#'
operator|||
name|c
operator|==
literal|';'
condition|)
block|{
name|comment
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|c
operator|==
literal|'['
condition|)
block|{
name|baselen
operator|=
name|get_base_var
argument_list|(
name|var
argument_list|)
expr_stmt|;
if|if
condition|(
name|baselen
operator|<=
literal|0
condition|)
break|break;
name|var
index|[
name|baselen
operator|++
index|]
operator|=
literal|'.'
expr_stmt|;
name|var
index|[
name|baselen
index|]
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|isalpha
argument_list|(
name|c
argument_list|)
condition|)
break|break;
name|var
index|[
name|baselen
index|]
operator|=
name|c
expr_stmt|;
if|if
condition|(
name|get_value
argument_list|(
name|fn
argument_list|,
name|var
argument_list|,
name|baselen
operator|+
literal|1
argument_list|)
operator|<
literal|0
condition|)
break|break;
block|}
name|die
argument_list|(
literal|"bad config file line %d"
argument_list|,
name|config_linenr
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_config_int
name|int
name|git_config_int
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
if|if
condition|(
name|value
operator|&&
operator|*
name|value
condition|)
block|{
name|char
modifier|*
name|end
decl_stmt|;
name|int
name|val
init|=
name|strtol
argument_list|(
name|value
argument_list|,
operator|&
name|end
argument_list|,
literal|0
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
operator|*
name|end
condition|)
return|return
name|val
return|;
block|}
name|die
argument_list|(
literal|"bad config value for '%s'"
argument_list|,
name|name
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_config_bool
name|int
name|git_config_bool
parameter_list|(
specifier|const
name|char
modifier|*
name|name
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
name|value
condition|)
return|return
literal|1
return|;
if|if
condition|(
operator|!
operator|*
name|value
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|value
argument_list|,
literal|"true"
argument_list|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|value
argument_list|,
literal|"false"
argument_list|)
condition|)
return|return
literal|0
return|;
return|return
name|git_config_int
argument_list|(
name|name
argument_list|,
name|value
argument_list|)
operator|!=
literal|0
return|;
block|}
end_function

begin_function
DECL|function|git_default_config
name|int
name|git_default_config
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
comment|/* This needs a better name */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"core.filemode"
argument_list|)
condition|)
block|{
name|trust_executable_bit
operator|=
name|git_config_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
comment|/* Add other config variables here.. */
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|git_config
name|int
name|git_config
parameter_list|(
name|config_fn_t
name|fn
parameter_list|)
block|{
name|int
name|ret
decl_stmt|;
name|FILE
modifier|*
name|f
init|=
name|fopen
argument_list|(
name|git_path
argument_list|(
literal|"config"
argument_list|)
argument_list|,
literal|"r"
argument_list|)
decl_stmt|;
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|f
condition|)
block|{
name|config_file
operator|=
name|f
expr_stmt|;
name|config_linenr
operator|=
literal|1
expr_stmt|;
name|ret
operator|=
name|git_parse_file
argument_list|(
name|fn
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|f
argument_list|)
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

end_unit

