begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"bundle.h"
end_include

begin_comment
comment|/*  * Basic handler for bundle files to connect repositories via sneakernet.  * Invocation must include action.  * This function can create a bundle or provide information on an existing  * bundle supporting "fetch", "pull", and "ls-remote".  */
end_comment

begin_decl_stmt
DECL|variable|builtin_bundle_usage
specifier|static
specifier|const
name|char
name|builtin_bundle_usage
index|[]
init|=
literal|"git bundle create<file><git-rev-list args>\n"
literal|"   or: git bundle verify<file>\n"
literal|"   or: git bundle list-heads<file> [refname...]\n"
literal|"   or: git bundle unbundle<file> [refname...]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_bundle
name|int
name|cmd_bundle
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
name|struct
name|bundle_header
name|header
decl_stmt|;
name|int
name|nongit
decl_stmt|;
specifier|const
name|char
modifier|*
name|cmd
decl_stmt|,
modifier|*
name|bundle_file
decl_stmt|;
name|int
name|bundle_fd
init|=
operator|-
literal|1
decl_stmt|;
name|char
name|buffer
index|[
name|PATH_MAX
index|]
decl_stmt|;
if|if
condition|(
name|argc
operator|<
literal|3
condition|)
name|usage
argument_list|(
name|builtin_bundle_usage
argument_list|)
expr_stmt|;
name|cmd
operator|=
name|argv
index|[
literal|1
index|]
expr_stmt|;
name|bundle_file
operator|=
name|argv
index|[
literal|2
index|]
expr_stmt|;
name|argc
operator|-=
literal|2
expr_stmt|;
name|argv
operator|+=
literal|2
expr_stmt|;
name|prefix
operator|=
name|setup_git_directory_gently
argument_list|(
operator|&
name|nongit
argument_list|)
expr_stmt|;
if|if
condition|(
name|prefix
operator|&&
name|bundle_file
index|[
literal|0
index|]
operator|!=
literal|'/'
condition|)
block|{
name|snprintf
argument_list|(
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
argument_list|,
literal|"%s/%s"
argument_list|,
name|prefix
argument_list|,
name|bundle_file
argument_list|)
expr_stmt|;
name|bundle_file
operator|=
name|buffer
expr_stmt|;
block|}
name|memset
argument_list|(
operator|&
name|header
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|header
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"create"
argument_list|)
operator|&&
operator|(
name|bundle_fd
operator|=
name|read_bundle_header
argument_list|(
name|bundle_file
argument_list|,
operator|&
name|header
argument_list|)
operator|)
operator|<
literal|0
condition|)
return|return
literal|1
return|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"verify"
argument_list|)
condition|)
block|{
name|close
argument_list|(
name|bundle_fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|verify_bundle
argument_list|(
operator|&
name|header
argument_list|,
literal|1
argument_list|)
condition|)
return|return
literal|1
return|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s is okay\n"
argument_list|,
name|bundle_file
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"list-heads"
argument_list|)
condition|)
block|{
name|close
argument_list|(
name|bundle_fd
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|list_bundle_refs
argument_list|(
operator|&
name|header
argument_list|,
name|argc
argument_list|,
name|argv
argument_list|)
return|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"create"
argument_list|)
condition|)
block|{
if|if
condition|(
name|nongit
condition|)
name|die
argument_list|(
literal|"Need a repository to create a bundle."
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|create_bundle
argument_list|(
operator|&
name|header
argument_list|,
name|bundle_file
argument_list|,
name|argc
argument_list|,
name|argv
argument_list|)
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|cmd
argument_list|,
literal|"unbundle"
argument_list|)
condition|)
block|{
if|if
condition|(
name|nongit
condition|)
name|die
argument_list|(
literal|"Need a repository to unbundle."
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|unbundle
argument_list|(
operator|&
name|header
argument_list|,
name|bundle_fd
argument_list|)
operator|||
name|list_bundle_refs
argument_list|(
operator|&
name|header
argument_list|,
name|argc
argument_list|,
name|argv
argument_list|)
return|;
block|}
else|else
name|usage
argument_list|(
name|builtin_bundle_usage
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

