begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Implementation of git-merge-ours.sh as builtin  *  * Copyright (c) 2007 Thomas Harning Jr  * Original:  * Original Copyright (c) 2005 Junio C Hamano  *  * Pretend we resolved the heads, but declare our tree trumps everybody else.  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_decl_stmt
DECL|variable|diff_index_args
specifier|static
specifier|const
name|char
modifier|*
name|diff_index_args
index|[]
init|=
block|{
literal|"diff-index"
block|,
literal|"--quiet"
block|,
literal|"--cached"
block|,
literal|"HEAD"
block|,
literal|"--"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|NARGS
define|#
directive|define
name|NARGS
value|(ARRAY_SIZE(diff_index_args) - 1)
end_define

begin_function
DECL|function|cmd_merge_ours
name|int
name|cmd_merge_ours
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
comment|/* 	 * We need to exit with 2 if the index does not match our HEAD tree, 	 * because the current index is what we will be committing as the 	 * merge result. 	 */
if|if
condition|(
name|cmd_diff_index
argument_list|(
name|NARGS
argument_list|,
name|diff_index_args
argument_list|,
name|prefix
argument_list|)
condition|)
name|exit
argument_list|(
literal|2
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

