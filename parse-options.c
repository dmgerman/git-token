begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
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

begin_define
DECL|macro|OPT_SHORT
define|#
directive|define
name|OPT_SHORT
value|1
end_define

begin_define
DECL|macro|OPT_UNSET
define|#
directive|define
name|OPT_UNSET
value|2
end_define

begin_function
DECL|function|opterror
specifier|static
name|int
name|opterror
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|reason
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
if|if
condition|(
name|flags
operator|&
name|OPT_SHORT
condition|)
return|return
name|error
argument_list|(
literal|"switch `%c' %s"
argument_list|,
name|opt
operator|->
name|short_name
argument_list|,
name|reason
argument_list|)
return|;
if|if
condition|(
name|flags
operator|&
name|OPT_UNSET
condition|)
return|return
name|error
argument_list|(
literal|"option `no-%s' %s"
argument_list|,
name|opt
operator|->
name|long_name
argument_list|,
name|reason
argument_list|)
return|;
return|return
name|error
argument_list|(
literal|"option `%s' %s"
argument_list|,
name|opt
operator|->
name|long_name
argument_list|,
name|reason
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|get_arg
specifier|static
name|int
name|get_arg
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|p
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
name|int
name|flags
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|arg
parameter_list|)
block|{
if|if
condition|(
name|p
operator|->
name|opt
condition|)
block|{
operator|*
name|arg
operator|=
name|p
operator|->
name|opt
expr_stmt|;
name|p
operator|->
name|opt
operator|=
name|NULL
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|p
operator|->
name|argc
operator|==
literal|1
operator|&&
operator|(
name|opt
operator|->
name|flags
operator|&
name|PARSE_OPT_LASTARG_DEFAULT
operator|)
condition|)
block|{
operator|*
name|arg
operator|=
operator|(
specifier|const
name|char
operator|*
operator|)
name|opt
operator|->
name|defval
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|p
operator|->
name|argc
operator|>
literal|1
condition|)
block|{
name|p
operator|->
name|argc
operator|--
expr_stmt|;
operator|*
name|arg
operator|=
operator|*
operator|++
name|p
operator|->
name|argv
expr_stmt|;
block|}
else|else
return|return
name|opterror
argument_list|(
name|opt
argument_list|,
literal|"requires a value"
argument_list|,
name|flags
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|get_value
specifier|static
name|int
name|get_value
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|p
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|s
decl_stmt|,
modifier|*
name|arg
decl_stmt|;
specifier|const
name|int
name|unset
init|=
name|flags
operator|&
name|OPT_UNSET
decl_stmt|;
if|if
condition|(
name|unset
operator|&&
name|p
operator|->
name|opt
condition|)
return|return
name|opterror
argument_list|(
name|opt
argument_list|,
literal|"takes no value"
argument_list|,
name|flags
argument_list|)
return|;
if|if
condition|(
name|unset
operator|&&
operator|(
name|opt
operator|->
name|flags
operator|&
name|PARSE_OPT_NONEG
operator|)
condition|)
return|return
name|opterror
argument_list|(
name|opt
argument_list|,
literal|"isn't available"
argument_list|,
name|flags
argument_list|)
return|;
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|OPT_SHORT
operator|)
operator|&&
name|p
operator|->
name|opt
condition|)
block|{
switch|switch
condition|(
name|opt
operator|->
name|type
condition|)
block|{
case|case
name|OPTION_CALLBACK
case|:
if|if
condition|(
operator|!
operator|(
name|opt
operator|->
name|flags
operator|&
name|PARSE_OPT_NOARG
operator|)
condition|)
break|break;
comment|/* FALLTHROUGH */
case|case
name|OPTION_BOOLEAN
case|:
case|case
name|OPTION_BIT
case|:
case|case
name|OPTION_SET_INT
case|:
case|case
name|OPTION_SET_PTR
case|:
return|return
name|opterror
argument_list|(
name|opt
argument_list|,
literal|"takes no value"
argument_list|,
name|flags
argument_list|)
return|;
default|default:
break|break;
block|}
block|}
switch|switch
condition|(
name|opt
operator|->
name|type
condition|)
block|{
case|case
name|OPTION_BIT
case|:
if|if
condition|(
name|unset
condition|)
operator|*
operator|(
name|int
operator|*
operator|)
name|opt
operator|->
name|value
operator|&=
operator|~
name|opt
operator|->
name|defval
expr_stmt|;
else|else
operator|*
operator|(
name|int
operator|*
operator|)
name|opt
operator|->
name|value
operator||=
name|opt
operator|->
name|defval
expr_stmt|;
return|return
literal|0
return|;
case|case
name|OPTION_BOOLEAN
case|:
operator|*
operator|(
name|int
operator|*
operator|)
name|opt
operator|->
name|value
operator|=
name|unset
condition|?
literal|0
else|:
operator|*
operator|(
name|int
operator|*
operator|)
name|opt
operator|->
name|value
operator|+
literal|1
expr_stmt|;
return|return
literal|0
return|;
case|case
name|OPTION_SET_INT
case|:
operator|*
operator|(
name|int
operator|*
operator|)
name|opt
operator|->
name|value
operator|=
name|unset
condition|?
literal|0
else|:
name|opt
operator|->
name|defval
expr_stmt|;
return|return
literal|0
return|;
case|case
name|OPTION_SET_PTR
case|:
operator|*
operator|(
name|void
operator|*
operator|*
operator|)
name|opt
operator|->
name|value
operator|=
name|unset
condition|?
name|NULL
else|:
operator|(
name|void
operator|*
operator|)
name|opt
operator|->
name|defval
expr_stmt|;
return|return
literal|0
return|;
case|case
name|OPTION_STRING
case|:
if|if
condition|(
name|unset
condition|)
operator|*
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
name|opt
operator|->
name|value
operator|=
name|NULL
expr_stmt|;
elseif|else
if|if
condition|(
name|opt
operator|->
name|flags
operator|&
name|PARSE_OPT_OPTARG
operator|&&
operator|!
name|p
operator|->
name|opt
condition|)
operator|*
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
name|opt
operator|->
name|value
operator|=
operator|(
specifier|const
name|char
operator|*
operator|)
name|opt
operator|->
name|defval
expr_stmt|;
else|else
return|return
name|get_arg
argument_list|(
name|p
argument_list|,
name|opt
argument_list|,
name|flags
argument_list|,
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
name|opt
operator|->
name|value
argument_list|)
return|;
return|return
literal|0
return|;
case|case
name|OPTION_CALLBACK
case|:
if|if
condition|(
name|unset
condition|)
return|return
operator|(
operator|*
name|opt
operator|->
name|callback
operator|)
operator|(
name|opt
operator|,
name|NULL
operator|,
literal|1
operator|)
condition|?
operator|(
operator|-
literal|1
operator|)
else|:
literal|0
return|;
if|if
condition|(
name|opt
operator|->
name|flags
operator|&
name|PARSE_OPT_NOARG
condition|)
return|return
operator|(
operator|*
name|opt
operator|->
name|callback
operator|)
operator|(
name|opt
operator|,
name|NULL
operator|,
literal|0
operator|)
condition|?
operator|(
operator|-
literal|1
operator|)
else|:
literal|0
return|;
if|if
condition|(
name|opt
operator|->
name|flags
operator|&
name|PARSE_OPT_OPTARG
operator|&&
operator|!
name|p
operator|->
name|opt
condition|)
return|return
operator|(
operator|*
name|opt
operator|->
name|callback
operator|)
operator|(
name|opt
operator|,
name|NULL
operator|,
literal|0
operator|)
condition|?
operator|(
operator|-
literal|1
operator|)
else|:
literal|0
return|;
if|if
condition|(
name|get_arg
argument_list|(
name|p
argument_list|,
name|opt
argument_list|,
name|flags
argument_list|,
operator|&
name|arg
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
return|return
operator|(
operator|*
name|opt
operator|->
name|callback
operator|)
operator|(
name|opt
operator|,
name|arg
operator|,
literal|0
operator|)
condition|?
operator|(
operator|-
literal|1
operator|)
else|:
literal|0
return|;
case|case
name|OPTION_INTEGER
case|:
if|if
condition|(
name|unset
condition|)
block|{
operator|*
operator|(
name|int
operator|*
operator|)
name|opt
operator|->
name|value
operator|=
literal|0
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|opt
operator|->
name|flags
operator|&
name|PARSE_OPT_OPTARG
operator|&&
operator|!
name|p
operator|->
name|opt
condition|)
block|{
operator|*
operator|(
name|int
operator|*
operator|)
name|opt
operator|->
name|value
operator|=
name|opt
operator|->
name|defval
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|get_arg
argument_list|(
name|p
argument_list|,
name|opt
argument_list|,
name|flags
argument_list|,
operator|&
name|arg
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
operator|*
operator|(
name|int
operator|*
operator|)
name|opt
operator|->
name|value
operator|=
name|strtol
argument_list|(
name|arg
argument_list|,
operator|(
name|char
operator|*
operator|*
operator|)
operator|&
name|s
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|s
condition|)
return|return
name|opterror
argument_list|(
name|opt
argument_list|,
literal|"expects a numerical value"
argument_list|,
name|flags
argument_list|)
return|;
return|return
literal|0
return|;
default|default:
name|die
argument_list|(
literal|"should not happen, someone must be hit on the forehead"
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|parse_short_opt
specifier|static
name|int
name|parse_short_opt
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|p
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|options
parameter_list|)
block|{
for|for
control|(
init|;
name|options
operator|->
name|type
operator|!=
name|OPTION_END
condition|;
name|options
operator|++
control|)
block|{
if|if
condition|(
name|options
operator|->
name|short_name
operator|==
operator|*
name|p
operator|->
name|opt
condition|)
block|{
name|p
operator|->
name|opt
operator|=
name|p
operator|->
name|opt
index|[
literal|1
index|]
condition|?
name|p
operator|->
name|opt
operator|+
literal|1
else|:
name|NULL
expr_stmt|;
return|return
name|get_value
argument_list|(
name|p
argument_list|,
name|options
argument_list|,
name|OPT_SHORT
argument_list|)
return|;
block|}
block|}
return|return
operator|-
literal|2
return|;
block|}
end_function

begin_function
DECL|function|parse_long_opt
specifier|static
name|int
name|parse_long_opt
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|p
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|options
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|arg_end
init|=
name|strchr
argument_list|(
name|arg
argument_list|,
literal|'='
argument_list|)
decl_stmt|;
specifier|const
name|struct
name|option
modifier|*
name|abbrev_option
init|=
name|NULL
decl_stmt|,
modifier|*
name|ambiguous_option
init|=
name|NULL
decl_stmt|;
name|int
name|abbrev_flags
init|=
literal|0
decl_stmt|,
name|ambiguous_flags
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|arg_end
condition|)
name|arg_end
operator|=
name|arg
operator|+
name|strlen
argument_list|(
name|arg
argument_list|)
expr_stmt|;
for|for
control|(
init|;
name|options
operator|->
name|type
operator|!=
name|OPTION_END
condition|;
name|options
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|rest
decl_stmt|;
name|int
name|flags
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|options
operator|->
name|long_name
condition|)
continue|continue;
name|rest
operator|=
name|skip_prefix
argument_list|(
name|arg
argument_list|,
name|options
operator|->
name|long_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|options
operator|->
name|type
operator|==
name|OPTION_ARGUMENT
condition|)
block|{
if|if
condition|(
operator|!
name|rest
condition|)
continue|continue;
if|if
condition|(
operator|*
name|rest
operator|==
literal|'='
condition|)
return|return
name|opterror
argument_list|(
name|options
argument_list|,
literal|"takes no value"
argument_list|,
name|flags
argument_list|)
return|;
if|if
condition|(
operator|*
name|rest
condition|)
continue|continue;
name|p
operator|->
name|out
index|[
name|p
operator|->
name|cpidx
operator|++
index|]
operator|=
name|arg
operator|-
literal|2
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|rest
condition|)
block|{
comment|/* abbreviated? */
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|options
operator|->
name|long_name
argument_list|,
name|arg
argument_list|,
name|arg_end
operator|-
name|arg
argument_list|)
condition|)
block|{
name|is_abbreviated
label|:
if|if
condition|(
name|abbrev_option
condition|)
block|{
comment|/* 					 * If this is abbreviated, it is 					 * ambiguous. So when there is no 					 * exact match later, we need to 					 * error out. 					 */
name|ambiguous_option
operator|=
name|abbrev_option
expr_stmt|;
name|ambiguous_flags
operator|=
name|abbrev_flags
expr_stmt|;
block|}
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|OPT_UNSET
operator|)
operator|&&
operator|*
name|arg_end
condition|)
name|p
operator|->
name|opt
operator|=
name|arg_end
operator|+
literal|1
expr_stmt|;
name|abbrev_option
operator|=
name|options
expr_stmt|;
name|abbrev_flags
operator|=
name|flags
expr_stmt|;
continue|continue;
block|}
comment|/* negated and abbreviated very much? */
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
literal|"no-"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|flags
operator||=
name|OPT_UNSET
expr_stmt|;
goto|goto
name|is_abbreviated
goto|;
block|}
comment|/* negated? */
if|if
condition|(
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"no-"
argument_list|,
literal|3
argument_list|)
condition|)
continue|continue;
name|flags
operator||=
name|OPT_UNSET
expr_stmt|;
name|rest
operator|=
name|skip_prefix
argument_list|(
name|arg
operator|+
literal|3
argument_list|,
name|options
operator|->
name|long_name
argument_list|)
expr_stmt|;
comment|/* abbreviated and negated? */
if|if
condition|(
operator|!
name|rest
operator|&&
operator|!
name|prefixcmp
argument_list|(
name|options
operator|->
name|long_name
argument_list|,
name|arg
operator|+
literal|3
argument_list|)
condition|)
goto|goto
name|is_abbreviated
goto|;
if|if
condition|(
operator|!
name|rest
condition|)
continue|continue;
block|}
if|if
condition|(
operator|*
name|rest
condition|)
block|{
if|if
condition|(
operator|*
name|rest
operator|!=
literal|'='
condition|)
continue|continue;
name|p
operator|->
name|opt
operator|=
name|rest
operator|+
literal|1
expr_stmt|;
block|}
return|return
name|get_value
argument_list|(
name|p
argument_list|,
name|options
argument_list|,
name|flags
argument_list|)
return|;
block|}
if|if
condition|(
name|ambiguous_option
condition|)
return|return
name|error
argument_list|(
literal|"Ambiguous option: %s "
literal|"(could be --%s%s or --%s%s)"
argument_list|,
name|arg
argument_list|,
operator|(
name|ambiguous_flags
operator|&
name|OPT_UNSET
operator|)
condition|?
literal|"no-"
else|:
literal|""
argument_list|,
name|ambiguous_option
operator|->
name|long_name
argument_list|,
operator|(
name|abbrev_flags
operator|&
name|OPT_UNSET
operator|)
condition|?
literal|"no-"
else|:
literal|""
argument_list|,
name|abbrev_option
operator|->
name|long_name
argument_list|)
return|;
if|if
condition|(
name|abbrev_option
condition|)
return|return
name|get_value
argument_list|(
name|p
argument_list|,
name|abbrev_option
argument_list|,
name|abbrev_flags
argument_list|)
return|;
return|return
operator|-
literal|2
return|;
block|}
end_function

begin_function
DECL|function|check_typos
specifier|static
name|void
name|check_typos
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|options
parameter_list|)
block|{
if|if
condition|(
name|strlen
argument_list|(
name|arg
argument_list|)
operator|<
literal|3
condition|)
return|return;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"no-"
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"did you mean `--%s` (with two dashes ?)"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|129
argument_list|)
expr_stmt|;
block|}
for|for
control|(
init|;
name|options
operator|->
name|type
operator|!=
name|OPTION_END
condition|;
name|options
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|options
operator|->
name|long_name
condition|)
continue|continue;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|options
operator|->
name|long_name
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"did you mean `--%s` (with two dashes ?)"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|129
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|parse_options_start
name|void
name|parse_options_start
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|ctx
parameter_list|,
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|memset
argument_list|(
name|ctx
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|ctx
argument_list|)
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|argc
operator|=
name|argc
operator|-
literal|1
expr_stmt|;
name|ctx
operator|->
name|argv
operator|=
name|argv
operator|+
literal|1
expr_stmt|;
name|ctx
operator|->
name|out
operator|=
name|argv
expr_stmt|;
name|ctx
operator|->
name|cpidx
operator|=
operator|(
operator|(
name|flags
operator|&
name|PARSE_OPT_KEEP_ARGV0
operator|)
operator|!=
literal|0
operator|)
expr_stmt|;
name|ctx
operator|->
name|flags
operator|=
name|flags
expr_stmt|;
block|}
end_function

begin_function_decl
specifier|static
name|int
name|usage_with_options_internal
parameter_list|(
specifier|const
name|char
modifier|*
specifier|const
modifier|*
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|parse_options_step
name|int
name|parse_options_step
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|ctx
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|options
parameter_list|,
specifier|const
name|char
modifier|*
specifier|const
name|usagestr
index|[]
parameter_list|)
block|{
name|int
name|internal_help
init|=
operator|!
operator|(
name|ctx
operator|->
name|flags
operator|&
name|PARSE_OPT_NO_INTERNAL_HELP
operator|)
decl_stmt|;
comment|/* we must reset ->opt, unknown short option leave it dangling */
name|ctx
operator|->
name|opt
operator|=
name|NULL
expr_stmt|;
for|for
control|(
init|;
name|ctx
operator|->
name|argc
condition|;
name|ctx
operator|->
name|argc
operator|--
operator|,
name|ctx
operator|->
name|argv
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|ctx
operator|->
name|argv
index|[
literal|0
index|]
decl_stmt|;
if|if
condition|(
operator|*
name|arg
operator|!=
literal|'-'
operator|||
operator|!
name|arg
index|[
literal|1
index|]
condition|)
block|{
if|if
condition|(
name|ctx
operator|->
name|flags
operator|&
name|PARSE_OPT_STOP_AT_NON_OPTION
condition|)
break|break;
name|ctx
operator|->
name|out
index|[
name|ctx
operator|->
name|cpidx
operator|++
index|]
operator|=
name|ctx
operator|->
name|argv
index|[
literal|0
index|]
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|arg
index|[
literal|1
index|]
operator|!=
literal|'-'
condition|)
block|{
name|ctx
operator|->
name|opt
operator|=
name|arg
operator|+
literal|1
expr_stmt|;
if|if
condition|(
name|internal_help
operator|&&
operator|*
name|ctx
operator|->
name|opt
operator|==
literal|'h'
condition|)
return|return
name|parse_options_usage
argument_list|(
name|usagestr
argument_list|,
name|options
argument_list|)
return|;
switch|switch
condition|(
name|parse_short_opt
argument_list|(
name|ctx
argument_list|,
name|options
argument_list|)
condition|)
block|{
case|case
operator|-
literal|1
case|:
return|return
name|parse_options_usage
argument_list|(
name|usagestr
argument_list|,
name|options
argument_list|)
return|;
case|case
operator|-
literal|2
case|:
goto|goto
name|unknown
goto|;
block|}
if|if
condition|(
name|ctx
operator|->
name|opt
condition|)
name|check_typos
argument_list|(
name|arg
operator|+
literal|1
argument_list|,
name|options
argument_list|)
expr_stmt|;
while|while
condition|(
name|ctx
operator|->
name|opt
condition|)
block|{
if|if
condition|(
name|internal_help
operator|&&
operator|*
name|ctx
operator|->
name|opt
operator|==
literal|'h'
condition|)
return|return
name|parse_options_usage
argument_list|(
name|usagestr
argument_list|,
name|options
argument_list|)
return|;
switch|switch
condition|(
name|parse_short_opt
argument_list|(
name|ctx
argument_list|,
name|options
argument_list|)
condition|)
block|{
case|case
operator|-
literal|1
case|:
return|return
name|parse_options_usage
argument_list|(
name|usagestr
argument_list|,
name|options
argument_list|)
return|;
case|case
operator|-
literal|2
case|:
comment|/* fake a short option thing to hide the fact that we may have 					 * started to parse aggregated stuff 					 * 					 * This is leaky, too bad. 					 */
name|ctx
operator|->
name|argv
index|[
literal|0
index|]
operator|=
name|xstrdup
argument_list|(
name|ctx
operator|->
name|opt
operator|-
literal|1
argument_list|)
expr_stmt|;
operator|*
operator|(
name|char
operator|*
operator|)
name|ctx
operator|->
name|argv
index|[
literal|0
index|]
operator|=
literal|'-'
expr_stmt|;
goto|goto
name|unknown
goto|;
block|}
block|}
continue|continue;
block|}
if|if
condition|(
operator|!
name|arg
index|[
literal|2
index|]
condition|)
block|{
comment|/* "--" */
if|if
condition|(
operator|!
operator|(
name|ctx
operator|->
name|flags
operator|&
name|PARSE_OPT_KEEP_DASHDASH
operator|)
condition|)
block|{
name|ctx
operator|->
name|argc
operator|--
expr_stmt|;
name|ctx
operator|->
name|argv
operator|++
expr_stmt|;
block|}
break|break;
block|}
if|if
condition|(
name|internal_help
operator|&&
operator|!
name|strcmp
argument_list|(
name|arg
operator|+
literal|2
argument_list|,
literal|"help-all"
argument_list|)
condition|)
return|return
name|usage_with_options_internal
argument_list|(
name|usagestr
argument_list|,
name|options
argument_list|,
literal|1
argument_list|)
return|;
if|if
condition|(
name|internal_help
operator|&&
operator|!
name|strcmp
argument_list|(
name|arg
operator|+
literal|2
argument_list|,
literal|"help"
argument_list|)
condition|)
return|return
name|parse_options_usage
argument_list|(
name|usagestr
argument_list|,
name|options
argument_list|)
return|;
switch|switch
condition|(
name|parse_long_opt
argument_list|(
name|ctx
argument_list|,
name|arg
operator|+
literal|2
argument_list|,
name|options
argument_list|)
condition|)
block|{
case|case
operator|-
literal|1
case|:
return|return
name|parse_options_usage
argument_list|(
name|usagestr
argument_list|,
name|options
argument_list|)
return|;
case|case
operator|-
literal|2
case|:
goto|goto
name|unknown
goto|;
block|}
continue|continue;
name|unknown
label|:
if|if
condition|(
operator|!
operator|(
name|ctx
operator|->
name|flags
operator|&
name|PARSE_OPT_KEEP_UNKNOWN
operator|)
condition|)
return|return
name|PARSE_OPT_UNKNOWN
return|;
name|ctx
operator|->
name|out
index|[
name|ctx
operator|->
name|cpidx
operator|++
index|]
operator|=
name|ctx
operator|->
name|argv
index|[
literal|0
index|]
expr_stmt|;
name|ctx
operator|->
name|opt
operator|=
name|NULL
expr_stmt|;
block|}
return|return
name|PARSE_OPT_DONE
return|;
block|}
end_function

begin_function
DECL|function|parse_options_end
name|int
name|parse_options_end
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|memmove
argument_list|(
name|ctx
operator|->
name|out
operator|+
name|ctx
operator|->
name|cpidx
argument_list|,
name|ctx
operator|->
name|argv
argument_list|,
name|ctx
operator|->
name|argc
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|ctx
operator|->
name|out
argument_list|)
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|out
index|[
name|ctx
operator|->
name|cpidx
operator|+
name|ctx
operator|->
name|argc
index|]
operator|=
name|NULL
expr_stmt|;
return|return
name|ctx
operator|->
name|cpidx
operator|+
name|ctx
operator|->
name|argc
return|;
block|}
end_function

begin_function
DECL|function|parse_options
name|int
name|parse_options
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
name|struct
name|option
modifier|*
name|options
parameter_list|,
specifier|const
name|char
modifier|*
specifier|const
name|usagestr
index|[]
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|struct
name|parse_opt_ctx_t
name|ctx
decl_stmt|;
name|parse_options_start
argument_list|(
operator|&
name|ctx
argument_list|,
name|argc
argument_list|,
name|argv
argument_list|,
name|flags
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|parse_options_step
argument_list|(
operator|&
name|ctx
argument_list|,
name|options
argument_list|,
name|usagestr
argument_list|)
condition|)
block|{
case|case
name|PARSE_OPT_HELP
case|:
name|exit
argument_list|(
literal|129
argument_list|)
expr_stmt|;
case|case
name|PARSE_OPT_DONE
case|:
break|break;
default|default:
comment|/* PARSE_OPT_UNKNOWN */
if|if
condition|(
name|ctx
operator|.
name|argv
index|[
literal|0
index|]
index|[
literal|1
index|]
operator|==
literal|'-'
condition|)
block|{
name|error
argument_list|(
literal|"unknown option `%s'"
argument_list|,
name|ctx
operator|.
name|argv
index|[
literal|0
index|]
operator|+
literal|2
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|error
argument_list|(
literal|"unknown switch `%c'"
argument_list|,
operator|*
name|ctx
operator|.
name|opt
argument_list|)
expr_stmt|;
block|}
name|usage_with_options
argument_list|(
name|usagestr
argument_list|,
name|options
argument_list|)
expr_stmt|;
block|}
return|return
name|parse_options_end
argument_list|(
operator|&
name|ctx
argument_list|)
return|;
block|}
end_function

begin_define
DECL|macro|USAGE_OPTS_WIDTH
define|#
directive|define
name|USAGE_OPTS_WIDTH
value|24
end_define

begin_define
DECL|macro|USAGE_GAP
define|#
directive|define
name|USAGE_GAP
value|2
end_define

begin_function
DECL|function|usage_with_options_internal
name|int
name|usage_with_options_internal
parameter_list|(
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|usagestr
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|opts
parameter_list|,
name|int
name|full
parameter_list|)
block|{
if|if
condition|(
operator|!
name|usagestr
condition|)
return|return
name|PARSE_OPT_HELP
return|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"usage: %s\n"
argument_list|,
operator|*
name|usagestr
operator|++
argument_list|)
expr_stmt|;
while|while
condition|(
operator|*
name|usagestr
operator|&&
operator|*
operator|*
name|usagestr
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"   or: %s\n"
argument_list|,
operator|*
name|usagestr
operator|++
argument_list|)
expr_stmt|;
while|while
condition|(
operator|*
name|usagestr
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s%s\n"
argument_list|,
operator|*
operator|*
name|usagestr
condition|?
literal|"    "
else|:
literal|""
argument_list|,
operator|*
name|usagestr
argument_list|)
expr_stmt|;
name|usagestr
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|opts
operator|->
name|type
operator|!=
name|OPTION_GROUP
condition|)
name|fputc
argument_list|(
literal|'\n'
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
for|for
control|(
init|;
name|opts
operator|->
name|type
operator|!=
name|OPTION_END
condition|;
name|opts
operator|++
control|)
block|{
name|size_t
name|pos
decl_stmt|;
name|int
name|pad
decl_stmt|;
if|if
condition|(
name|opts
operator|->
name|type
operator|==
name|OPTION_GROUP
condition|)
block|{
name|fputc
argument_list|(
literal|'\n'
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|opts
operator|->
name|help
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s\n"
argument_list|,
name|opts
operator|->
name|help
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|full
operator|&&
operator|(
name|opts
operator|->
name|flags
operator|&
name|PARSE_OPT_HIDDEN
operator|)
condition|)
continue|continue;
name|pos
operator|=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"    "
argument_list|)
expr_stmt|;
if|if
condition|(
name|opts
operator|->
name|short_name
condition|)
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"-%c"
argument_list|,
name|opts
operator|->
name|short_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|opts
operator|->
name|long_name
operator|&&
name|opts
operator|->
name|short_name
condition|)
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|", "
argument_list|)
expr_stmt|;
if|if
condition|(
name|opts
operator|->
name|long_name
condition|)
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"--%s"
argument_list|,
name|opts
operator|->
name|long_name
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|opts
operator|->
name|type
condition|)
block|{
case|case
name|OPTION_ARGUMENT
case|:
break|break;
case|case
name|OPTION_INTEGER
case|:
if|if
condition|(
name|opts
operator|->
name|flags
operator|&
name|PARSE_OPT_OPTARG
condition|)
if|if
condition|(
name|opts
operator|->
name|long_name
condition|)
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"[=<n>]"
argument_list|)
expr_stmt|;
else|else
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"[<n>]"
argument_list|)
expr_stmt|;
else|else
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"<n>"
argument_list|)
expr_stmt|;
break|break;
case|case
name|OPTION_CALLBACK
case|:
if|if
condition|(
name|opts
operator|->
name|flags
operator|&
name|PARSE_OPT_NOARG
condition|)
break|break;
comment|/* FALLTHROUGH */
case|case
name|OPTION_STRING
case|:
if|if
condition|(
name|opts
operator|->
name|argh
condition|)
block|{
if|if
condition|(
name|opts
operator|->
name|flags
operator|&
name|PARSE_OPT_OPTARG
condition|)
if|if
condition|(
name|opts
operator|->
name|long_name
condition|)
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"[=<%s>]"
argument_list|,
name|opts
operator|->
name|argh
argument_list|)
expr_stmt|;
else|else
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"[<%s>]"
argument_list|,
name|opts
operator|->
name|argh
argument_list|)
expr_stmt|;
else|else
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"<%s>"
argument_list|,
name|opts
operator|->
name|argh
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|opts
operator|->
name|flags
operator|&
name|PARSE_OPT_OPTARG
condition|)
if|if
condition|(
name|opts
operator|->
name|long_name
condition|)
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"[=...]"
argument_list|)
expr_stmt|;
else|else
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"[...]"
argument_list|)
expr_stmt|;
else|else
name|pos
operator|+=
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|" ..."
argument_list|)
expr_stmt|;
block|}
break|break;
default|default:
comment|/* OPTION_{BIT,BOOLEAN,SET_INT,SET_PTR} */
break|break;
block|}
if|if
condition|(
name|pos
operator|<=
name|USAGE_OPTS_WIDTH
condition|)
name|pad
operator|=
name|USAGE_OPTS_WIDTH
operator|-
name|pos
expr_stmt|;
else|else
block|{
name|fputc
argument_list|(
literal|'\n'
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
name|pad
operator|=
name|USAGE_OPTS_WIDTH
expr_stmt|;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%*s%s\n"
argument_list|,
name|pad
operator|+
name|USAGE_GAP
argument_list|,
literal|""
argument_list|,
name|opts
operator|->
name|help
argument_list|)
expr_stmt|;
block|}
name|fputc
argument_list|(
literal|'\n'
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
return|return
name|PARSE_OPT_HELP
return|;
block|}
end_function

begin_function
DECL|function|usage_with_options
name|void
name|usage_with_options
parameter_list|(
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|usagestr
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|opts
parameter_list|)
block|{
name|usage_with_options_internal
argument_list|(
name|usagestr
argument_list|,
name|opts
argument_list|,
literal|0
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
DECL|function|parse_options_usage
name|int
name|parse_options_usage
parameter_list|(
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|usagestr
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|opts
parameter_list|)
block|{
return|return
name|usage_with_options_internal
argument_list|(
name|usagestr
argument_list|,
name|opts
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*----- some often used options -----*/
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_function
DECL|function|parse_opt_abbrev_cb
name|int
name|parse_opt_abbrev_cb
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|unset
parameter_list|)
block|{
name|int
name|v
decl_stmt|;
if|if
condition|(
operator|!
name|arg
condition|)
block|{
name|v
operator|=
name|unset
condition|?
literal|0
else|:
name|DEFAULT_ABBREV
expr_stmt|;
block|}
else|else
block|{
name|v
operator|=
name|strtol
argument_list|(
name|arg
argument_list|,
operator|(
name|char
operator|*
operator|*
operator|)
operator|&
name|arg
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|arg
condition|)
return|return
name|opterror
argument_list|(
name|opt
argument_list|,
literal|"expects a numerical value"
argument_list|,
literal|0
argument_list|)
return|;
if|if
condition|(
name|v
operator|&&
name|v
operator|<
name|MINIMUM_ABBREV
condition|)
name|v
operator|=
name|MINIMUM_ABBREV
expr_stmt|;
elseif|else
if|if
condition|(
name|v
operator|>
literal|40
condition|)
name|v
operator|=
literal|40
expr_stmt|;
block|}
operator|*
operator|(
name|int
operator|*
operator|)
operator|(
name|opt
operator|->
name|value
operator|)
operator|=
name|v
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|parse_opt_approxidate_cb
name|int
name|parse_opt_approxidate_cb
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|unset
parameter_list|)
block|{
operator|*
operator|(
name|unsigned
name|long
operator|*
operator|)
operator|(
name|opt
operator|->
name|value
operator|)
operator|=
name|approxidate
argument_list|(
name|arg
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|parse_opt_verbosity_cb
name|int
name|parse_opt_verbosity_cb
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|unset
parameter_list|)
block|{
name|int
modifier|*
name|target
init|=
name|opt
operator|->
name|value
decl_stmt|;
if|if
condition|(
name|unset
condition|)
comment|/* --no-quiet, --no-verbose */
operator|*
name|target
operator|=
literal|0
expr_stmt|;
elseif|else
if|if
condition|(
name|opt
operator|->
name|short_name
operator|==
literal|'v'
condition|)
block|{
if|if
condition|(
operator|*
name|target
operator|>=
literal|0
condition|)
operator|(
operator|*
name|target
operator|)
operator|++
expr_stmt|;
else|else
operator|*
name|target
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
operator|*
name|target
operator|<=
literal|0
condition|)
operator|(
operator|*
name|target
operator|)
operator|--
expr_stmt|;
else|else
operator|*
name|target
operator|=
operator|-
literal|1
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|parse_opt_with_commit
name|int
name|parse_opt_with_commit
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|unset
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
if|if
condition|(
operator|!
name|arg
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|get_sha1
argument_list|(
name|arg
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"malformed object name %s"
argument_list|,
name|arg
argument_list|)
return|;
name|commit
operator|=
name|lookup_commit_reference
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
condition|)
return|return
name|error
argument_list|(
literal|"no such commit %s"
argument_list|,
name|arg
argument_list|)
return|;
name|commit_list_insert
argument_list|(
name|commit
argument_list|,
name|opt
operator|->
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * This should really be OPTION_FILENAME type as a part of  * parse_options that take prefix to do this while parsing.  */
end_comment

begin_function
DECL|function|parse_options_fix_filename
specifier|extern
specifier|const
name|char
modifier|*
name|parse_options_fix_filename
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
name|file
parameter_list|)
block|{
if|if
condition|(
operator|!
name|file
operator|||
operator|!
name|prefix
operator|||
name|is_absolute_path
argument_list|(
name|file
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"-"
argument_list|,
name|file
argument_list|)
condition|)
return|return
name|file
return|;
return|return
name|prefix_filename
argument_list|(
name|prefix
argument_list|,
name|strlen
argument_list|(
name|prefix
argument_list|)
argument_list|,
name|file
argument_list|)
return|;
block|}
end_function

end_unit

