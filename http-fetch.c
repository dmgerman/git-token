begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_include
include|#
directive|include
file|"http.h"
end_include

begin_include
include|#
directive|include
file|"walker.h"
end_include

begin_decl_stmt
DECL|variable|http_fetch_usage
specifier|static
specifier|const
name|char
name|http_fetch_usage
index|[]
init|=
literal|"git http-fetch "
literal|"[-c] [-t] [-a] [-v] [--recover] [-w ref] [--stdin] commit-id url"
decl_stmt|;
end_decl_stmt

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
name|struct
name|walker
modifier|*
name|walker
decl_stmt|;
name|int
name|commits_on_stdin
init|=
literal|0
decl_stmt|;
name|int
name|commits
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|write_ref
init|=
name|NULL
decl_stmt|;
name|char
modifier|*
modifier|*
name|commit_id
decl_stmt|;
name|char
modifier|*
name|url
init|=
name|NULL
decl_stmt|;
name|int
name|arg
init|=
literal|1
decl_stmt|;
name|int
name|rc
init|=
literal|0
decl_stmt|;
name|int
name|get_tree
init|=
literal|0
decl_stmt|;
name|int
name|get_history
init|=
literal|0
decl_stmt|;
name|int
name|get_all
init|=
literal|0
decl_stmt|;
name|int
name|get_verbosely
init|=
literal|0
decl_stmt|;
name|int
name|get_recover
init|=
literal|0
decl_stmt|;
name|git_extract_argv0_path
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
while|while
condition|(
name|arg
operator|<
name|argc
operator|&&
name|argv
index|[
name|arg
index|]
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'t'
condition|)
block|{
name|get_tree
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'c'
condition|)
block|{
name|get_history
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'a'
condition|)
block|{
name|get_all
operator|=
literal|1
expr_stmt|;
name|get_tree
operator|=
literal|1
expr_stmt|;
name|get_history
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'v'
condition|)
block|{
name|get_verbosely
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'w'
condition|)
block|{
name|write_ref
operator|=
operator|&
name|argv
index|[
name|arg
operator|+
literal|1
index|]
expr_stmt|;
name|arg
operator|++
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'h'
condition|)
block|{
name|usage
argument_list|(
name|http_fetch_usage
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|arg
index|]
argument_list|,
literal|"--recover"
argument_list|)
condition|)
block|{
name|get_recover
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|arg
index|]
argument_list|,
literal|"--stdin"
argument_list|)
condition|)
block|{
name|commits_on_stdin
operator|=
literal|1
expr_stmt|;
block|}
name|arg
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|!=
name|arg
operator|+
literal|2
operator|-
name|commits_on_stdin
condition|)
name|usage
argument_list|(
name|http_fetch_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|commits_on_stdin
condition|)
block|{
name|commits
operator|=
name|walker_targets_stdin
argument_list|(
operator|&
name|commit_id
argument_list|,
operator|&
name|write_ref
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|commit_id
operator|=
operator|(
name|char
operator|*
operator|*
operator|)
operator|&
name|argv
index|[
name|arg
operator|++
index|]
expr_stmt|;
name|commits
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|argv
index|[
name|arg
index|]
condition|)
name|str_end_url_with_slash
argument_list|(
name|argv
index|[
name|arg
index|]
argument_list|,
operator|&
name|url
argument_list|)
expr_stmt|;
name|setup_git_directory
argument_list|()
expr_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|http_init
argument_list|(
name|NULL
argument_list|,
name|url
argument_list|)
expr_stmt|;
name|walker
operator|=
name|get_http_walker
argument_list|(
name|url
argument_list|)
expr_stmt|;
name|walker
operator|->
name|get_tree
operator|=
name|get_tree
expr_stmt|;
name|walker
operator|->
name|get_history
operator|=
name|get_history
expr_stmt|;
name|walker
operator|->
name|get_all
operator|=
name|get_all
expr_stmt|;
name|walker
operator|->
name|get_verbosely
operator|=
name|get_verbosely
expr_stmt|;
name|walker
operator|->
name|get_recover
operator|=
name|get_recover
expr_stmt|;
name|rc
operator|=
name|walker_fetch
argument_list|(
name|walker
argument_list|,
name|commits
argument_list|,
name|commit_id
argument_list|,
name|write_ref
argument_list|,
name|url
argument_list|)
expr_stmt|;
if|if
condition|(
name|commits_on_stdin
condition|)
name|walker_targets_free
argument_list|(
name|commits
argument_list|,
name|commit_id
argument_list|,
name|write_ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|walker
operator|->
name|corrupt_object_found
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Some loose object were found to be corrupt, but they might be just\n"
literal|"a false '404 Not Found' error message sent with incorrect HTTP\n"
literal|"status code.  Suggest running 'git fsck'.\n"
argument_list|)
expr_stmt|;
block|}
name|walker_free
argument_list|(
name|walker
argument_list|)
expr_stmt|;
name|http_cleanup
argument_list|()
expr_stmt|;
name|free
argument_list|(
name|url
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
end_function

end_unit

