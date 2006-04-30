begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|<regex.h>
end_include

begin_decl_stmt
DECL|variable|git_config_set_usage
specifier|static
specifier|const
name|char
name|git_config_set_usage
index|[]
init|=
literal|"git-repo-config [ --bool | --int ] [--get | --get-all | --replace-all | --unset | --unset-all] name [value [value_regex]] | --list"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|key
specifier|static
name|char
modifier|*
name|key
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|value
specifier|static
name|char
modifier|*
name|value
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|regexp
specifier|static
name|regex_t
modifier|*
name|regexp
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|do_all
specifier|static
name|int
name|do_all
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|do_not_match
specifier|static
name|int
name|do_not_match
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|seen
specifier|static
name|int
name|seen
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_enum
DECL|enumerator|T_RAW
DECL|enumerator|T_INT
DECL|enumerator|T_BOOL
DECL|variable|type
specifier|static
enum|enum
block|{
name|T_RAW
block|,
name|T_INT
block|,
name|T_BOOL
block|}
name|type
init|=
name|T_RAW
enum|;
end_enum

begin_function
DECL|function|show_all_config
specifier|static
name|int
name|show_all_config
parameter_list|(
specifier|const
name|char
modifier|*
name|key_
parameter_list|,
specifier|const
name|char
modifier|*
name|value_
parameter_list|)
block|{
if|if
condition|(
name|value_
condition|)
name|printf
argument_list|(
literal|"%s=%s\n"
argument_list|,
name|key_
argument_list|,
name|value_
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|key_
argument_list|)
expr_stmt|;
return|return
literal|0
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
name|key_
parameter_list|,
specifier|const
name|char
modifier|*
name|value_
parameter_list|)
block|{
if|if
condition|(
name|value_
operator|==
name|NULL
condition|)
name|value_
operator|=
literal|""
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key_
argument_list|,
name|key
argument_list|)
operator|&&
operator|(
name|regexp
operator|==
name|NULL
operator|||
operator|(
name|do_not_match
operator|^
operator|!
name|regexec
argument_list|(
name|regexp
argument_list|,
name|value_
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
operator|)
operator|)
condition|)
block|{
if|if
condition|(
name|do_all
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|value_
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|seen
operator|>
literal|0
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"More than one value: %s\n"
argument_list|,
name|value
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|type
operator|==
name|T_INT
condition|)
block|{
name|value
operator|=
name|malloc
argument_list|(
literal|256
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|value
argument_list|,
literal|"%d"
argument_list|,
name|git_config_int
argument_list|(
name|key_
argument_list|,
name|value_
argument_list|)
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|type
operator|==
name|T_BOOL
condition|)
block|{
name|value
operator|=
name|malloc
argument_list|(
literal|256
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|value
argument_list|,
literal|"%s"
argument_list|,
name|git_config_bool
argument_list|(
name|key_
argument_list|,
name|value_
argument_list|)
condition|?
literal|"true"
else|:
literal|"false"
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|value
operator|=
name|strdup
argument_list|(
name|value_
argument_list|)
expr_stmt|;
block|}
name|seen
operator|++
expr_stmt|;
block|}
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
specifier|const
name|char
modifier|*
name|key_
parameter_list|,
specifier|const
name|char
modifier|*
name|regex_
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|key
operator|=
name|malloc
argument_list|(
name|strlen
argument_list|(
name|key_
argument_list|)
operator|+
literal|1
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|key_
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
name|key
index|[
name|i
index|]
operator|=
name|tolower
argument_list|(
name|key_
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|key
index|[
name|i
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|regex_
condition|)
block|{
if|if
condition|(
name|regex_
index|[
literal|0
index|]
operator|==
literal|'!'
condition|)
block|{
name|do_not_match
operator|=
literal|1
expr_stmt|;
name|regex_
operator|++
expr_stmt|;
block|}
name|regexp
operator|=
operator|(
name|regex_t
operator|*
operator|)
name|malloc
argument_list|(
sizeof|sizeof
argument_list|(
name|regex_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|regcomp
argument_list|(
name|regexp
argument_list|,
name|regex_
argument_list|,
name|REG_EXTENDED
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Invalid pattern: %s\n"
argument_list|,
name|regex_
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
block|}
name|git_config
argument_list|(
name|show_config
argument_list|)
expr_stmt|;
if|if
condition|(
name|value
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|value
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|key
argument_list|)
expr_stmt|;
if|if
condition|(
name|regexp
condition|)
block|{
name|regfree
argument_list|(
name|regexp
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|regexp
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|do_all
condition|)
return|return
literal|0
return|;
return|return
name|seen
operator|==
literal|1
condition|?
literal|0
else|:
literal|1
return|;
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
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|setup_git_directory
argument_list|()
expr_stmt|;
while|while
condition|(
literal|1
operator|<
name|argc
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--int"
argument_list|)
condition|)
name|type
operator|=
name|T_INT
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--bool"
argument_list|)
condition|)
name|type
operator|=
name|T_BOOL
expr_stmt|;
else|else
break|break;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--list"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-l"
argument_list|)
condition|)
return|return
name|git_config
argument_list|(
name|show_all_config
argument_list|)
return|;
switch|switch
condition|(
name|argc
condition|)
block|{
case|case
literal|2
case|:
return|return
name|get_value
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|NULL
argument_list|)
return|;
case|case
literal|3
case|:
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--unset"
argument_list|)
condition|)
return|return
name|git_config_set
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|NULL
argument_list|)
return|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--unset-all"
argument_list|)
condition|)
return|return
name|git_config_set_multivar
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|,
literal|1
argument_list|)
return|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--get"
argument_list|)
condition|)
return|return
name|get_value
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|NULL
argument_list|)
return|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--get-all"
argument_list|)
condition|)
block|{
name|do_all
operator|=
literal|1
expr_stmt|;
return|return
name|get_value
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|NULL
argument_list|)
return|;
block|}
else|else
return|return
name|git_config_set
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
return|;
case|case
literal|4
case|:
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--unset"
argument_list|)
condition|)
return|return
name|git_config_set_multivar
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|NULL
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|,
literal|0
argument_list|)
return|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--unset-all"
argument_list|)
condition|)
return|return
name|git_config_set_multivar
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|NULL
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|,
literal|1
argument_list|)
return|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--get"
argument_list|)
condition|)
return|return
name|get_value
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|)
return|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--get-all"
argument_list|)
condition|)
block|{
name|do_all
operator|=
literal|1
expr_stmt|;
return|return
name|get_value
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|)
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--replace-all"
argument_list|)
condition|)
return|return
name|git_config_set_multivar
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|,
name|NULL
argument_list|,
literal|1
argument_list|)
return|;
else|else
return|return
name|git_config_set_multivar
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|,
literal|0
argument_list|)
return|;
case|case
literal|5
case|:
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--replace-all"
argument_list|)
condition|)
return|return
name|git_config_set_multivar
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
name|argv
index|[
literal|3
index|]
argument_list|,
name|argv
index|[
literal|4
index|]
argument_list|,
literal|1
argument_list|)
return|;
case|case
literal|1
case|:
default|default:
name|usage
argument_list|(
name|git_config_set_usage
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

