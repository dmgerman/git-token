begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"submodule-config.h"
end_include

begin_include
include|#
directive|include
file|"submodule.h"
end_include

begin_function
DECL|function|die_usage
specifier|static
name|void
name|die_usage
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s\n"
argument_list|,
name|msg
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Usage: %s [<commit><submodulepath>] ...\n"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_test_config
specifier|static
name|int
name|git_test_config
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
return|return
name|parse_submodule_config_option
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
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
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|char
modifier|*
modifier|*
name|arg
init|=
name|argv
decl_stmt|;
name|int
name|my_argc
init|=
name|argc
decl_stmt|;
name|int
name|output_url
init|=
literal|0
decl_stmt|;
name|int
name|lookup_name
init|=
literal|0
decl_stmt|;
name|arg
operator|++
expr_stmt|;
name|my_argc
operator|--
expr_stmt|;
while|while
condition|(
name|arg
index|[
literal|0
index|]
operator|&&
name|starts_with
argument_list|(
name|arg
index|[
literal|0
index|]
argument_list|,
literal|"--"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
index|[
literal|0
index|]
argument_list|,
literal|"--url"
argument_list|)
condition|)
name|output_url
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
index|[
literal|0
index|]
argument_list|,
literal|"--name"
argument_list|)
condition|)
name|lookup_name
operator|=
literal|1
expr_stmt|;
name|arg
operator|++
expr_stmt|;
name|my_argc
operator|--
expr_stmt|;
block|}
if|if
condition|(
name|my_argc
operator|%
literal|2
operator|!=
literal|0
condition|)
name|die_usage
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
literal|"Wrong number of arguments."
argument_list|)
expr_stmt|;
name|setup_git_directory
argument_list|()
expr_stmt|;
name|gitmodules_config
argument_list|()
expr_stmt|;
name|git_config
argument_list|(
name|git_test_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
while|while
condition|(
operator|*
name|arg
condition|)
block|{
name|unsigned
name|char
name|commit_sha1
index|[
literal|20
index|]
decl_stmt|;
specifier|const
name|struct
name|submodule
modifier|*
name|submodule
decl_stmt|;
specifier|const
name|char
modifier|*
name|commit
decl_stmt|;
specifier|const
name|char
modifier|*
name|path_or_name
decl_stmt|;
name|commit
operator|=
name|arg
index|[
literal|0
index|]
expr_stmt|;
name|path_or_name
operator|=
name|arg
index|[
literal|1
index|]
expr_stmt|;
if|if
condition|(
name|commit
index|[
literal|0
index|]
operator|==
literal|'\0'
condition|)
name|hashcpy
argument_list|(
name|commit_sha1
argument_list|,
name|null_sha1
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|get_sha1
argument_list|(
name|commit
argument_list|,
name|commit_sha1
argument_list|)
operator|<
literal|0
condition|)
name|die_usage
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
literal|"Commit not found."
argument_list|)
expr_stmt|;
if|if
condition|(
name|lookup_name
condition|)
block|{
name|submodule
operator|=
name|submodule_from_name
argument_list|(
name|commit_sha1
argument_list|,
name|path_or_name
argument_list|)
expr_stmt|;
block|}
else|else
name|submodule
operator|=
name|submodule_from_path
argument_list|(
name|commit_sha1
argument_list|,
name|path_or_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|submodule
condition|)
name|die_usage
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
literal|"Submodule not found."
argument_list|)
expr_stmt|;
if|if
condition|(
name|output_url
condition|)
name|printf
argument_list|(
literal|"Submodule url: '%s' for path '%s'\n"
argument_list|,
name|submodule
operator|->
name|url
argument_list|,
name|submodule
operator|->
name|path
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"Submodule name: '%s' for path '%s'\n"
argument_list|,
name|submodule
operator|->
name|name
argument_list|,
name|submodule
operator|->
name|path
argument_list|)
expr_stmt|;
name|arg
operator|+=
literal|2
expr_stmt|;
block|}
name|submodule_free
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

