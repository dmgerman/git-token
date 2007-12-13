begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Whitespace rules  *  * Copyright (c) 2007 Junio C Hamano  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"attr.h"
end_include

begin_struct
DECL|struct|whitespace_rule
specifier|static
struct|struct
name|whitespace_rule
block|{
DECL|member|rule_name
specifier|const
name|char
modifier|*
name|rule_name
decl_stmt|;
DECL|member|rule_bits
name|unsigned
name|rule_bits
decl_stmt|;
DECL|variable|whitespace_rule_names
block|}
name|whitespace_rule_names
index|[]
init|=
block|{
block|{
literal|"trailing-space"
block|,
name|WS_TRAILING_SPACE
block|}
block|,
block|{
literal|"space-before-tab"
block|,
name|WS_SPACE_BEFORE_TAB
block|}
block|,
block|{
literal|"indent-with-non-tab"
block|,
name|WS_INDENT_WITH_NON_TAB
block|}
block|, }
struct|;
end_struct

begin_function
DECL|function|parse_whitespace_rule
name|unsigned
name|parse_whitespace_rule
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|)
block|{
name|unsigned
name|rule
init|=
name|WS_DEFAULT_RULE
decl_stmt|;
while|while
condition|(
name|string
condition|)
block|{
name|int
name|i
decl_stmt|;
name|size_t
name|len
decl_stmt|;
specifier|const
name|char
modifier|*
name|ep
decl_stmt|;
name|int
name|negated
init|=
literal|0
decl_stmt|;
name|string
operator|=
name|string
operator|+
name|strspn
argument_list|(
name|string
argument_list|,
literal|", \t\n\r"
argument_list|)
expr_stmt|;
name|ep
operator|=
name|strchr
argument_list|(
name|string
argument_list|,
literal|','
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ep
condition|)
name|len
operator|=
name|strlen
argument_list|(
name|string
argument_list|)
expr_stmt|;
else|else
name|len
operator|=
name|ep
operator|-
name|string
expr_stmt|;
if|if
condition|(
operator|*
name|string
operator|==
literal|'-'
condition|)
block|{
name|negated
operator|=
literal|1
expr_stmt|;
name|string
operator|++
expr_stmt|;
name|len
operator|--
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|len
condition|)
break|break;
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
name|whitespace_rule_names
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|strncmp
argument_list|(
name|whitespace_rule_names
index|[
name|i
index|]
operator|.
name|rule_name
argument_list|,
name|string
argument_list|,
name|len
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|negated
condition|)
name|rule
operator|&=
operator|~
name|whitespace_rule_names
index|[
name|i
index|]
operator|.
name|rule_bits
expr_stmt|;
else|else
name|rule
operator||=
name|whitespace_rule_names
index|[
name|i
index|]
operator|.
name|rule_bits
expr_stmt|;
break|break;
block|}
name|string
operator|=
name|ep
expr_stmt|;
block|}
return|return
name|rule
return|;
block|}
end_function

begin_function
DECL|function|setup_whitespace_attr_check
specifier|static
name|void
name|setup_whitespace_attr_check
parameter_list|(
name|struct
name|git_attr_check
modifier|*
name|check
parameter_list|)
block|{
specifier|static
name|struct
name|git_attr
modifier|*
name|attr_whitespace
decl_stmt|;
if|if
condition|(
operator|!
name|attr_whitespace
condition|)
name|attr_whitespace
operator|=
name|git_attr
argument_list|(
literal|"whitespace"
argument_list|,
literal|10
argument_list|)
expr_stmt|;
name|check
index|[
literal|0
index|]
operator|.
name|attr
operator|=
name|attr_whitespace
expr_stmt|;
block|}
end_function

begin_function
DECL|function|whitespace_rule
name|unsigned
name|whitespace_rule
parameter_list|(
specifier|const
name|char
modifier|*
name|pathname
parameter_list|)
block|{
name|struct
name|git_attr_check
name|attr_whitespace_rule
decl_stmt|;
name|setup_whitespace_attr_check
argument_list|(
operator|&
name|attr_whitespace_rule
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|git_checkattr
argument_list|(
name|pathname
argument_list|,
literal|1
argument_list|,
operator|&
name|attr_whitespace_rule
argument_list|)
condition|)
block|{
specifier|const
name|char
modifier|*
name|value
decl_stmt|;
name|value
operator|=
name|attr_whitespace_rule
operator|.
name|value
expr_stmt|;
if|if
condition|(
name|ATTR_TRUE
argument_list|(
name|value
argument_list|)
condition|)
block|{
comment|/* true (whitespace) */
name|unsigned
name|all_rule
init|=
literal|0
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
name|whitespace_rule_names
argument_list|)
condition|;
name|i
operator|++
control|)
name|all_rule
operator||=
name|whitespace_rule_names
index|[
name|i
index|]
operator|.
name|rule_bits
expr_stmt|;
return|return
name|all_rule
return|;
block|}
elseif|else
if|if
condition|(
name|ATTR_FALSE
argument_list|(
name|value
argument_list|)
condition|)
block|{
comment|/* false (-whitespace) */
return|return
literal|0
return|;
block|}
elseif|else
if|if
condition|(
name|ATTR_UNSET
argument_list|(
name|value
argument_list|)
condition|)
block|{
comment|/* reset to default (!whitespace) */
return|return
name|whitespace_rule_cfg
return|;
block|}
else|else
block|{
comment|/* string */
return|return
name|parse_whitespace_rule
argument_list|(
name|value
argument_list|)
return|;
block|}
block|}
else|else
block|{
return|return
name|whitespace_rule_cfg
return|;
block|}
block|}
end_function

begin_comment
comment|/* The returned string should be freed by the caller. */
end_comment

begin_function
DECL|function|whitespace_error_string
name|char
modifier|*
name|whitespace_error_string
parameter_list|(
name|unsigned
name|ws
parameter_list|)
block|{
name|struct
name|strbuf
name|err
decl_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|err
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|ws
operator|&
name|WS_TRAILING_SPACE
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|err
argument_list|,
literal|"Adds trailing whitespace"
argument_list|)
expr_stmt|;
if|if
condition|(
name|ws
operator|&
name|WS_SPACE_BEFORE_TAB
condition|)
block|{
if|if
condition|(
name|err
operator|.
name|len
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|err
argument_list|,
literal|", "
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|err
argument_list|,
literal|"Space in indent is followed by a tab"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|ws
operator|&
name|WS_INDENT_WITH_NON_TAB
condition|)
block|{
if|if
condition|(
name|err
operator|.
name|len
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|err
argument_list|,
literal|", "
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|err
argument_list|,
literal|"Indent more than 8 places with spaces"
argument_list|)
expr_stmt|;
block|}
return|return
name|strbuf_detach
argument_list|(
operator|&
name|err
argument_list|,
name|NULL
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* If stream is non-NULL, emits the line after checking. */
end_comment

begin_function
DECL|function|check_and_emit_line
name|unsigned
name|check_and_emit_line
parameter_list|(
specifier|const
name|char
modifier|*
name|line
parameter_list|,
name|int
name|len
parameter_list|,
name|unsigned
name|ws_rule
parameter_list|,
name|FILE
modifier|*
name|stream
parameter_list|,
specifier|const
name|char
modifier|*
name|set
parameter_list|,
specifier|const
name|char
modifier|*
name|reset
parameter_list|,
specifier|const
name|char
modifier|*
name|ws
parameter_list|)
block|{
name|unsigned
name|result
init|=
literal|0
decl_stmt|;
name|int
name|leading_space
init|=
operator|-
literal|1
decl_stmt|;
name|int
name|trailing_whitespace
init|=
operator|-
literal|1
decl_stmt|;
name|int
name|trailing_newline
init|=
literal|0
decl_stmt|;
name|int
name|i
decl_stmt|;
comment|/* Logic is simpler if we temporarily ignore the trailing newline. */
if|if
condition|(
name|len
operator|>
literal|0
operator|&&
name|line
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
block|{
name|trailing_newline
operator|=
literal|1
expr_stmt|;
name|len
operator|--
expr_stmt|;
block|}
comment|/* Check for trailing whitespace. */
if|if
condition|(
name|ws_rule
operator|&
name|WS_TRAILING_SPACE
condition|)
block|{
for|for
control|(
name|i
operator|=
name|len
operator|-
literal|1
init|;
name|i
operator|>=
literal|0
condition|;
name|i
operator|--
control|)
block|{
if|if
condition|(
name|isspace
argument_list|(
name|line
index|[
name|i
index|]
argument_list|)
condition|)
block|{
name|trailing_whitespace
operator|=
name|i
expr_stmt|;
name|result
operator||=
name|WS_TRAILING_SPACE
expr_stmt|;
block|}
else|else
break|break;
block|}
block|}
comment|/* Check for space before tab in initial indent. */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|len
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|line
index|[
name|i
index|]
operator|==
literal|'\t'
condition|)
block|{
if|if
condition|(
operator|(
name|ws_rule
operator|&
name|WS_SPACE_BEFORE_TAB
operator|)
operator|&&
operator|(
name|leading_space
operator|!=
operator|-
literal|1
operator|)
condition|)
name|result
operator||=
name|WS_SPACE_BEFORE_TAB
expr_stmt|;
break|break;
block|}
elseif|else
if|if
condition|(
name|line
index|[
name|i
index|]
operator|==
literal|' '
condition|)
name|leading_space
operator|=
name|i
expr_stmt|;
else|else
break|break;
block|}
comment|/* Check for indent using non-tab. */
if|if
condition|(
operator|(
name|ws_rule
operator|&
name|WS_INDENT_WITH_NON_TAB
operator|)
operator|&&
name|leading_space
operator|>=
literal|8
condition|)
name|result
operator||=
name|WS_INDENT_WITH_NON_TAB
expr_stmt|;
if|if
condition|(
name|stream
condition|)
block|{
comment|/* Highlight errors in leading whitespace. */
if|if
condition|(
operator|(
name|result
operator|&
name|WS_SPACE_BEFORE_TAB
operator|)
operator|||
operator|(
name|result
operator|&
name|WS_INDENT_WITH_NON_TAB
operator|)
condition|)
block|{
name|fputs
argument_list|(
name|ws
argument_list|,
name|stream
argument_list|)
expr_stmt|;
name|fwrite
argument_list|(
name|line
argument_list|,
name|leading_space
operator|+
literal|1
argument_list|,
literal|1
argument_list|,
name|stream
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
name|reset
argument_list|,
name|stream
argument_list|)
expr_stmt|;
name|leading_space
operator|++
expr_stmt|;
block|}
else|else
name|leading_space
operator|=
literal|0
expr_stmt|;
comment|/* Now the rest of the line starts at leading_space. 		 * The non-highlighted part ends at trailing_whitespace. */
if|if
condition|(
name|trailing_whitespace
operator|==
operator|-
literal|1
condition|)
name|trailing_whitespace
operator|=
name|len
expr_stmt|;
comment|/* Emit non-highlighted (middle) segment. */
if|if
condition|(
name|trailing_whitespace
operator|-
name|leading_space
operator|>
literal|0
condition|)
block|{
name|fputs
argument_list|(
name|set
argument_list|,
name|stream
argument_list|)
expr_stmt|;
name|fwrite
argument_list|(
name|line
operator|+
name|leading_space
argument_list|,
name|trailing_whitespace
operator|-
name|leading_space
argument_list|,
literal|1
argument_list|,
name|stream
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
name|reset
argument_list|,
name|stream
argument_list|)
expr_stmt|;
block|}
comment|/* Highlight errors in trailing whitespace. */
if|if
condition|(
name|trailing_whitespace
operator|!=
name|len
condition|)
block|{
name|fputs
argument_list|(
name|ws
argument_list|,
name|stream
argument_list|)
expr_stmt|;
name|fwrite
argument_list|(
name|line
operator|+
name|trailing_whitespace
argument_list|,
name|len
operator|-
name|trailing_whitespace
argument_list|,
literal|1
argument_list|,
name|stream
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
name|reset
argument_list|,
name|stream
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|trailing_newline
condition|)
name|fputc
argument_list|(
literal|'\n'
argument_list|,
name|stream
argument_list|)
expr_stmt|;
block|}
return|return
name|result
return|;
block|}
end_function

end_unit

