begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

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

begin_include
include|#
directive|include
file|"submodule.h"
end_include

begin_decl_stmt
DECL|variable|diff_files_usage
specifier|static
specifier|const
name|char
name|diff_files_usage
index|[]
init|=
literal|"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
name|COMMON_DIFF_OPTIONS_HELP
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_diff_files
name|int
name|cmd_diff_files
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
name|result
decl_stmt|;
name|unsigned
name|options
init|=
literal|0
decl_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|gitmodules_config
argument_list|()
expr_stmt|;
name|git_config
argument_list|(
name|git_diff_basic_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
comment|/* no "diff" UI options */
name|rev
operator|.
name|abbrev
operator|=
literal|0
expr_stmt|;
name|precompose_argv
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|)
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
while|while
condition|(
literal|1
operator|<
name|argc
operator|&&
name|argv
index|[
literal|1
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
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--base"
argument_list|)
condition|)
name|rev
operator|.
name|max_count
operator|=
literal|1
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
literal|"--ours"
argument_list|)
condition|)
name|rev
operator|.
name|max_count
operator|=
literal|2
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
literal|"--theirs"
argument_list|)
condition|)
name|rev
operator|.
name|max_count
operator|=
literal|3
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
literal|"-q"
argument_list|)
condition|)
name|options
operator||=
name|DIFF_SILENT_ON_REMOVED
expr_stmt|;
else|else
name|usage
argument_list|(
name|diff_files_usage
argument_list|)
expr_stmt|;
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
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
comment|/* 	 * Make sure there are NO revision (i.e. pending object) parameter, 	 * rev.max_count is reasonable (0<= n<= 3), and 	 * there is no other revision filtering parameters. 	 */
if|if
condition|(
name|rev
operator|.
name|pending
operator|.
name|nr
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
operator|||
literal|3
operator|<
name|rev
operator|.
name|max_count
condition|)
name|usage
argument_list|(
name|diff_files_usage
argument_list|)
expr_stmt|;
comment|/* 	 * "diff-files --base -p" should not combine merges because it 	 * was not asked to.  "diff-files -c -p" should not densify 	 * (the user should ask with "diff-files --cc" explicitly). 	 */
if|if
condition|(
name|rev
operator|.
name|max_count
operator|==
operator|-
literal|1
operator|&&
operator|!
name|rev
operator|.
name|combine_merges
operator|&&
operator|(
name|rev
operator|.
name|diffopt
operator|.
name|output_format
operator|&
name|DIFF_FORMAT_PATCH
operator|)
condition|)
name|rev
operator|.
name|combine_merges
operator|=
name|rev
operator|.
name|dense_combined_merges
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|read_cache_preload
argument_list|(
operator|&
name|rev
operator|.
name|diffopt
operator|.
name|pathspec
argument_list|)
operator|<
literal|0
condition|)
block|{
name|perror
argument_list|(
literal|"read_cache_preload"
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|result
operator|=
name|run_diff_files
argument_list|(
operator|&
name|rev
argument_list|,
name|options
argument_list|)
expr_stmt|;
return|return
name|diff_result_code
argument_list|(
operator|&
name|rev
operator|.
name|diffopt
argument_list|,
name|result
argument_list|)
return|;
block|}
end_function

end_unit

