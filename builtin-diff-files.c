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

begin_decl_stmt
DECL|variable|diff_files_usage
specifier|static
specifier|const
name|char
name|diff_files_usage
index|[]
init|=
literal|"git-diff-files [-q] [-0/-1/2/3 |-c|--cc|-n|--no-index] [<common diff options>] [<path>...]"
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
name|nongit
init|=
literal|0
decl_stmt|;
name|prefix
operator|=
name|setup_git_directory_gently
argument_list|(
operator|&
name|nongit
argument_list|)
expr_stmt|;
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
if|if
condition|(
operator|!
name|setup_diff_no_index
argument_list|(
operator|&
name|rev
argument_list|,
name|argc
argument_list|,
name|argv
argument_list|,
name|nongit
argument_list|,
name|prefix
argument_list|)
condition|)
name|argc
operator|=
literal|0
expr_stmt|;
else|else
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
return|return
name|run_diff_files_cmd
argument_list|(
operator|&
name|rev
argument_list|,
name|argc
argument_list|,
name|argv
argument_list|)
return|;
block|}
end_function

end_unit

