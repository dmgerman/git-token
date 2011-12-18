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
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"bisect.h"
end_include

begin_decl_stmt
DECL|variable|git_bisect_helper_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|git_bisect_helper_usage
index|[]
init|=
block|{
literal|"git bisect--helper --next-all"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_bisect__helper
name|int
name|cmd_bisect__helper
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
name|int
name|next_all
init|=
literal|0
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"next-all"
argument_list|,
operator|&
name|next_all
argument_list|,
literal|"perform 'git bisect next'"
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
name|options
argument_list|,
name|git_bisect_helper_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|next_all
condition|)
name|usage_with_options
argument_list|(
name|git_bisect_helper_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
comment|/* next-all */
return|return
name|bisect_next_all
argument_list|(
name|prefix
argument_list|)
return|;
block|}
end_function

end_unit

