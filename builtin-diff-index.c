begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_decl_stmt
DECL|variable|diff_cache_usage
specifier|static
specifier|const
name|char
name|diff_cache_usage
index|[]
init|=
literal|"git-diff-index [-m] [--cached] "
literal|"[<common diff options>]<tree-ish> [<path>...]"
name|COMMON_DIFF_OPTIONS_HELP
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_diff_index
name|int
name|cmd_diff_index
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
name|rev_info
name|rev
decl_stmt|;
name|int
name|cached
init|=
literal|0
decl_stmt|;
name|int
name|i
decl_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
comment|/* no "diff" UI options */
name|rev
operator|.
name|abbrev
operator|=
literal|0
expr_stmt|;
name|argc
operator|=
name|setup_revisions
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
operator|&
name|rev
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--cached"
argument_list|)
condition|)
name|cached
operator|=
literal|1
expr_stmt|;
else|else
name|usage
argument_list|(
name|diff_cache_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|rev
operator|.
name|diffopt
operator|.
name|output_format
condition|)
name|rev
operator|.
name|diffopt
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_RAW
expr_stmt|;
comment|/* 	 * Make sure there is one revision (i.e. pending object), 	 * and there is no revision filtering parameters. 	 */
if|if
condition|(
name|rev
operator|.
name|pending
operator|.
name|nr
operator|!=
literal|1
operator|||
name|rev
operator|.
name|max_count
operator|!=
operator|-
literal|1
operator|||
name|rev
operator|.
name|min_age
operator|!=
operator|-
literal|1
operator|||
name|rev
operator|.
name|max_age
operator|!=
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|diff_cache_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
block|{
name|perror
argument_list|(
literal|"read_cache"
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
return|return
name|run_diff_index
argument_list|(
operator|&
name|rev
argument_list|,
name|cached
argument_list|)
return|;
block|}
end_function

end_unit

