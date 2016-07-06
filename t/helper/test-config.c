begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_comment
comment|/*  * This program exposes the C API of the configuration mechanism  * as a set of simple commands in order to facilitate testing.  *  * Reads stdin and prints result of command to stdout:  *  * get_value -> prints the value with highest priority for the entered key  *  * get_value_multi -> prints all values for the entered key in increasing order  *		     of priority  *  * get_int -> print integer value for the entered key or die  *  * get_bool -> print bool value for the entered key or die  *  * get_string -> print string value for the entered key or die  *  * configset_get_value -> returns value with the highest priority for the entered key  * 			from a config_set constructed from files entered as arguments.  *  * configset_get_value_multi -> returns value_list for the entered key sorted in  * 				ascending order of priority from a config_set  * 				constructed from files entered as arguments.  *  * Examples:  *  * To print the value with highest priority for key "foo.bAr Baz.rock":  * 	test-config get_value "foo.bAr Baz.rock"  *  */
end_comment

begin_function
DECL|function|cmd_main
name|int
name|cmd_main
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
name|int
name|i
decl_stmt|,
name|val
decl_stmt|;
specifier|const
name|char
modifier|*
name|v
decl_stmt|;
specifier|const
name|struct
name|string_list
modifier|*
name|strptr
decl_stmt|;
name|struct
name|config_set
name|cs
decl_stmt|;
name|git_configset_init
argument_list|(
operator|&
name|cs
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|<
literal|2
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Please, provide a command name on the command-line\n"
argument_list|)
expr_stmt|;
goto|goto
name|exit1
goto|;
block|}
elseif|else
if|if
condition|(
name|argc
operator|==
literal|3
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"get_value"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|git_config_get_value
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
operator|&
name|v
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|v
condition|)
name|printf
argument_list|(
literal|"(NULL)\n"
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|v
argument_list|)
expr_stmt|;
goto|goto
name|exit0
goto|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"Value not found for \"%s\"\n"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
goto|goto
name|exit1
goto|;
block|}
block|}
elseif|else
if|if
condition|(
name|argc
operator|==
literal|3
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"get_value_multi"
argument_list|)
condition|)
block|{
name|strptr
operator|=
name|git_config_get_value_multi
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|strptr
condition|)
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|strptr
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|v
operator|=
name|strptr
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
expr_stmt|;
if|if
condition|(
operator|!
name|v
condition|)
name|printf
argument_list|(
literal|"(NULL)\n"
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|v
argument_list|)
expr_stmt|;
block|}
goto|goto
name|exit0
goto|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"Value not found for \"%s\"\n"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
goto|goto
name|exit1
goto|;
block|}
block|}
elseif|else
if|if
condition|(
name|argc
operator|==
literal|3
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"get_int"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|git_config_get_int
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
operator|&
name|val
argument_list|)
condition|)
block|{
name|printf
argument_list|(
literal|"%d\n"
argument_list|,
name|val
argument_list|)
expr_stmt|;
goto|goto
name|exit0
goto|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"Value not found for \"%s\"\n"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
goto|goto
name|exit1
goto|;
block|}
block|}
elseif|else
if|if
condition|(
name|argc
operator|==
literal|3
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"get_bool"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|git_config_get_bool
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
operator|&
name|val
argument_list|)
condition|)
block|{
name|printf
argument_list|(
literal|"%d\n"
argument_list|,
name|val
argument_list|)
expr_stmt|;
goto|goto
name|exit0
goto|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"Value not found for \"%s\"\n"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
goto|goto
name|exit1
goto|;
block|}
block|}
elseif|else
if|if
condition|(
name|argc
operator|==
literal|3
operator|&&
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"get_string"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|git_config_get_string_const
argument_list|(
name|argv
index|[
literal|2
index|]
argument_list|,
operator|&
name|v
argument_list|)
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|v
argument_list|)
expr_stmt|;
goto|goto
name|exit0
goto|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"Value not found for \"%s\"\n"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
goto|goto
name|exit1
goto|;
block|}
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
literal|"configset_get_value"
argument_list|)
condition|)
block|{
for|for
control|(
name|i
operator|=
literal|3
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
name|int
name|err
decl_stmt|;
if|if
condition|(
operator|(
name|err
operator|=
name|git_configset_add_file
argument_list|(
operator|&
name|cs
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
operator|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Error (%d) reading configuration file %s.\n"
argument_list|,
name|err
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
goto|goto
name|exit2
goto|;
block|}
block|}
if|if
condition|(
operator|!
name|git_configset_get_value
argument_list|(
operator|&
name|cs
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|,
operator|&
name|v
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|v
condition|)
name|printf
argument_list|(
literal|"(NULL)\n"
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|v
argument_list|)
expr_stmt|;
goto|goto
name|exit0
goto|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"Value not found for \"%s\"\n"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
goto|goto
name|exit1
goto|;
block|}
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
literal|"configset_get_value_multi"
argument_list|)
condition|)
block|{
for|for
control|(
name|i
operator|=
literal|3
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
name|int
name|err
decl_stmt|;
if|if
condition|(
operator|(
name|err
operator|=
name|git_configset_add_file
argument_list|(
operator|&
name|cs
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
operator|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Error (%d) reading configuration file %s.\n"
argument_list|,
name|err
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
goto|goto
name|exit2
goto|;
block|}
block|}
name|strptr
operator|=
name|git_configset_get_value_multi
argument_list|(
operator|&
name|cs
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|strptr
condition|)
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|strptr
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|v
operator|=
name|strptr
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
expr_stmt|;
if|if
condition|(
operator|!
name|v
condition|)
name|printf
argument_list|(
literal|"(NULL)\n"
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|v
argument_list|)
expr_stmt|;
block|}
goto|goto
name|exit0
goto|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"Value not found for \"%s\"\n"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
goto|goto
name|exit1
goto|;
block|}
block|}
name|die
argument_list|(
literal|"%s: Please check the syntax and the function name"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|exit0
label|:
name|git_configset_clear
argument_list|(
operator|&
name|cs
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
name|exit1
label|:
name|git_configset_clear
argument_list|(
operator|&
name|cs
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
name|exit2
label|:
name|git_configset_clear
argument_list|(
operator|&
name|cs
argument_list|)
expr_stmt|;
return|return
literal|2
return|;
block|}
end_function

end_unit

