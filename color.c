begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"color.h"
end_include

begin_decl_stmt
DECL|variable|git_use_color_default
name|int
name|git_use_color_default
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|parse_color
specifier|static
name|int
name|parse_color
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
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|color_names
index|[]
init|=
block|{
literal|"normal"
block|,
literal|"black"
block|,
literal|"red"
block|,
literal|"green"
block|,
literal|"yellow"
block|,
literal|"blue"
block|,
literal|"magenta"
block|,
literal|"cyan"
block|,
literal|"white"
block|}
decl_stmt|;
name|char
modifier|*
name|end
decl_stmt|;
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
name|color_names
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|str
init|=
name|color_names
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strncasecmp
argument_list|(
name|name
argument_list|,
name|str
argument_list|,
name|len
argument_list|)
operator|&&
operator|!
name|str
index|[
name|len
index|]
condition|)
return|return
name|i
operator|-
literal|1
return|;
block|}
name|i
operator|=
name|strtol
argument_list|(
name|name
argument_list|,
operator|&
name|end
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|end
operator|-
name|name
operator|==
name|len
operator|&&
name|i
operator|>=
operator|-
literal|1
operator|&&
name|i
operator|<=
literal|255
condition|)
return|return
name|i
return|;
return|return
operator|-
literal|2
return|;
block|}
end_function

begin_function
DECL|function|parse_attr
specifier|static
name|int
name|parse_attr
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
specifier|static
specifier|const
name|int
name|attr_values
index|[]
init|=
block|{
literal|1
block|,
literal|2
block|,
literal|4
block|,
literal|5
block|,
literal|7
block|}
decl_stmt|;
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|attr_names
index|[]
init|=
block|{
literal|"bold"
block|,
literal|"dim"
block|,
literal|"ul"
block|,
literal|"blink"
block|,
literal|"reverse"
block|}
decl_stmt|;
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
name|attr_names
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|str
init|=
name|attr_names
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strncasecmp
argument_list|(
name|name
argument_list|,
name|str
argument_list|,
name|len
argument_list|)
operator|&&
operator|!
name|str
index|[
name|len
index|]
condition|)
return|return
name|attr_values
index|[
name|i
index|]
return|;
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|color_parse
name|void
name|color_parse
parameter_list|(
specifier|const
name|char
modifier|*
name|value
parameter_list|,
specifier|const
name|char
modifier|*
name|var
parameter_list|,
name|char
modifier|*
name|dst
parameter_list|)
block|{
name|color_parse_mem
argument_list|(
name|value
argument_list|,
name|strlen
argument_list|(
name|value
argument_list|)
argument_list|,
name|var
argument_list|,
name|dst
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|color_parse_mem
name|void
name|color_parse_mem
parameter_list|(
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|int
name|value_len
parameter_list|,
specifier|const
name|char
modifier|*
name|var
parameter_list|,
name|char
modifier|*
name|dst
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|ptr
init|=
name|value
decl_stmt|;
name|int
name|len
init|=
name|value_len
decl_stmt|;
name|int
name|attr
init|=
operator|-
literal|1
decl_stmt|;
name|int
name|fg
init|=
operator|-
literal|2
decl_stmt|;
name|int
name|bg
init|=
operator|-
literal|2
decl_stmt|;
if|if
condition|(
operator|!
name|strncasecmp
argument_list|(
name|value
argument_list|,
literal|"reset"
argument_list|,
name|len
argument_list|)
condition|)
block|{
name|strcpy
argument_list|(
name|dst
argument_list|,
name|GIT_COLOR_RESET
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* [fg [bg]] [attr] */
while|while
condition|(
name|len
operator|>
literal|0
condition|)
block|{
specifier|const
name|char
modifier|*
name|word
init|=
name|ptr
decl_stmt|;
name|int
name|val
decl_stmt|,
name|wordlen
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|len
operator|>
literal|0
operator|&&
operator|!
name|isspace
argument_list|(
name|word
index|[
name|wordlen
index|]
argument_list|)
condition|)
block|{
name|wordlen
operator|++
expr_stmt|;
name|len
operator|--
expr_stmt|;
block|}
name|ptr
operator|=
name|word
operator|+
name|wordlen
expr_stmt|;
while|while
condition|(
name|len
operator|>
literal|0
operator|&&
name|isspace
argument_list|(
operator|*
name|ptr
argument_list|)
condition|)
block|{
name|ptr
operator|++
expr_stmt|;
name|len
operator|--
expr_stmt|;
block|}
name|val
operator|=
name|parse_color
argument_list|(
name|word
argument_list|,
name|wordlen
argument_list|)
expr_stmt|;
if|if
condition|(
name|val
operator|>=
operator|-
literal|1
condition|)
block|{
if|if
condition|(
name|fg
operator|==
operator|-
literal|2
condition|)
block|{
name|fg
operator|=
name|val
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|bg
operator|==
operator|-
literal|2
condition|)
block|{
name|bg
operator|=
name|val
expr_stmt|;
continue|continue;
block|}
goto|goto
name|bad
goto|;
block|}
name|val
operator|=
name|parse_attr
argument_list|(
name|word
argument_list|,
name|wordlen
argument_list|)
expr_stmt|;
if|if
condition|(
name|val
operator|<
literal|0
operator|||
name|attr
operator|!=
operator|-
literal|1
condition|)
goto|goto
name|bad
goto|;
name|attr
operator|=
name|val
expr_stmt|;
block|}
if|if
condition|(
name|attr
operator|>=
literal|0
operator|||
name|fg
operator|>=
literal|0
operator|||
name|bg
operator|>=
literal|0
condition|)
block|{
name|int
name|sep
init|=
literal|0
decl_stmt|;
operator|*
name|dst
operator|++
operator|=
literal|'\033'
expr_stmt|;
operator|*
name|dst
operator|++
operator|=
literal|'['
expr_stmt|;
if|if
condition|(
name|attr
operator|>=
literal|0
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
literal|'0'
operator|+
name|attr
expr_stmt|;
name|sep
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|fg
operator|>=
literal|0
condition|)
block|{
if|if
condition|(
name|sep
operator|++
condition|)
operator|*
name|dst
operator|++
operator|=
literal|';'
expr_stmt|;
if|if
condition|(
name|fg
operator|<
literal|8
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
literal|'3'
expr_stmt|;
operator|*
name|dst
operator|++
operator|=
literal|'0'
operator|+
name|fg
expr_stmt|;
block|}
else|else
block|{
name|dst
operator|+=
name|sprintf
argument_list|(
name|dst
argument_list|,
literal|"38;5;%d"
argument_list|,
name|fg
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|bg
operator|>=
literal|0
condition|)
block|{
if|if
condition|(
name|sep
operator|++
condition|)
operator|*
name|dst
operator|++
operator|=
literal|';'
expr_stmt|;
if|if
condition|(
name|bg
operator|<
literal|8
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
literal|'4'
expr_stmt|;
operator|*
name|dst
operator|++
operator|=
literal|'0'
operator|+
name|bg
expr_stmt|;
block|}
else|else
block|{
name|dst
operator|+=
name|sprintf
argument_list|(
name|dst
argument_list|,
literal|"48;5;%d"
argument_list|,
name|bg
argument_list|)
expr_stmt|;
block|}
block|}
operator|*
name|dst
operator|++
operator|=
literal|'m'
expr_stmt|;
block|}
operator|*
name|dst
operator|=
literal|0
expr_stmt|;
return|return;
name|bad
label|:
name|die
argument_list|(
literal|"bad color value '%.*s' for variable '%s'"
argument_list|,
name|value_len
argument_list|,
name|value
argument_list|,
name|var
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_config_colorbool
name|int
name|git_config_colorbool
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
name|int
name|stdout_is_tty
parameter_list|)
block|{
if|if
condition|(
name|value
condition|)
block|{
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|value
argument_list|,
literal|"never"
argument_list|)
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
literal|"always"
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
literal|"auto"
argument_list|)
condition|)
goto|goto
name|auto_color
goto|;
block|}
if|if
condition|(
operator|!
name|var
condition|)
return|return
operator|-
literal|1
return|;
comment|/* Missing or explicit false to turn off colorization */
if|if
condition|(
operator|!
name|git_config_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
condition|)
return|return
literal|0
return|;
comment|/* any normal truth value defaults to 'auto' */
name|auto_color
label|:
if|if
condition|(
name|stdout_is_tty
operator|<
literal|0
condition|)
name|stdout_is_tty
operator|=
name|isatty
argument_list|(
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|stdout_is_tty
operator|||
operator|(
name|pager_in_use
argument_list|()
operator|&&
name|pager_use_color
operator|)
condition|)
block|{
name|char
modifier|*
name|term
init|=
name|getenv
argument_list|(
literal|"TERM"
argument_list|)
decl_stmt|;
if|if
condition|(
name|term
operator|&&
name|strcmp
argument_list|(
name|term
argument_list|,
literal|"dumb"
argument_list|)
condition|)
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|git_color_default_config
name|int
name|git_color_default_config
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
literal|"color.ui"
argument_list|)
condition|)
block|{
name|git_use_color_default
operator|=
name|git_config_colorbool
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
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
DECL|function|color_vfprintf
specifier|static
name|int
name|color_vfprintf
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|color
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
name|va_list
name|args
parameter_list|,
specifier|const
name|char
modifier|*
name|trail
parameter_list|)
block|{
name|int
name|r
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|*
name|color
condition|)
name|r
operator|+=
name|fprintf
argument_list|(
name|fp
argument_list|,
literal|"%s"
argument_list|,
name|color
argument_list|)
expr_stmt|;
name|r
operator|+=
name|vfprintf
argument_list|(
name|fp
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|color
condition|)
name|r
operator|+=
name|fprintf
argument_list|(
name|fp
argument_list|,
literal|"%s"
argument_list|,
name|GIT_COLOR_RESET
argument_list|)
expr_stmt|;
if|if
condition|(
name|trail
condition|)
name|r
operator|+=
name|fprintf
argument_list|(
name|fp
argument_list|,
literal|"%s"
argument_list|,
name|trail
argument_list|)
expr_stmt|;
return|return
name|r
return|;
block|}
end_function

begin_function
DECL|function|color_fprintf
name|int
name|color_fprintf
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|color
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|args
decl_stmt|;
name|int
name|r
decl_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|r
operator|=
name|color_vfprintf
argument_list|(
name|fp
argument_list|,
name|color
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
return|return
name|r
return|;
block|}
end_function

begin_function
DECL|function|color_fprintf_ln
name|int
name|color_fprintf_ln
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|color
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|args
decl_stmt|;
name|int
name|r
decl_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|r
operator|=
name|color_vfprintf
argument_list|(
name|fp
argument_list|,
name|color
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|,
literal|"\n"
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
return|return
name|r
return|;
block|}
end_function

begin_comment
comment|/*  * This function splits the buffer by newlines and colors the lines individually.  *  * Returns 0 on success.  */
end_comment

begin_function
DECL|function|color_fwrite_lines
name|int
name|color_fwrite_lines
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|color
parameter_list|,
name|size_t
name|count
parameter_list|,
specifier|const
name|char
modifier|*
name|buf
parameter_list|)
block|{
if|if
condition|(
operator|!
operator|*
name|color
condition|)
return|return
name|fwrite
argument_list|(
name|buf
argument_list|,
name|count
argument_list|,
literal|1
argument_list|,
name|fp
argument_list|)
operator|!=
literal|1
return|;
while|while
condition|(
name|count
condition|)
block|{
name|char
modifier|*
name|p
init|=
name|memchr
argument_list|(
name|buf
argument_list|,
literal|'\n'
argument_list|,
name|count
argument_list|)
decl_stmt|;
if|if
condition|(
name|p
operator|!=
name|buf
operator|&&
operator|(
name|fputs
argument_list|(
name|color
argument_list|,
name|fp
argument_list|)
operator|<
literal|0
operator|||
name|fwrite
argument_list|(
name|buf
argument_list|,
name|p
condition|?
name|p
operator|-
name|buf
else|:
name|count
argument_list|,
literal|1
argument_list|,
name|fp
argument_list|)
operator|!=
literal|1
operator|||
name|fputs
argument_list|(
name|GIT_COLOR_RESET
argument_list|,
name|fp
argument_list|)
operator|<
literal|0
operator|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|p
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|fputc
argument_list|(
literal|'\n'
argument_list|,
name|fp
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
name|count
operator|-=
name|p
operator|+
literal|1
operator|-
name|buf
expr_stmt|;
name|buf
operator|=
name|p
operator|+
literal|1
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

