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
file|"wt-status.h"
end_include

begin_decl_stmt
specifier|extern
name|int
name|wt_status_use_color
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|runstatus_usage
specifier|static
specifier|const
name|char
name|runstatus_usage
index|[]
init|=
literal|"git-runstatus [--color|--nocolor] [--amend] [--verbose] [--untracked]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_runstatus
name|int
name|cmd_runstatus
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
name|wt_status
name|s
decl_stmt|;
name|int
name|i
decl_stmt|;
name|git_config
argument_list|(
name|git_status_config
argument_list|)
expr_stmt|;
name|wt_status_prepare
argument_list|(
operator|&
name|s
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--color"
argument_list|)
condition|)
name|wt_status_use_color
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
name|i
index|]
argument_list|,
literal|"--nocolor"
argument_list|)
condition|)
name|wt_status_use_color
operator|=
literal|0
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--amend"
argument_list|)
condition|)
block|{
name|s
operator|.
name|amend
operator|=
literal|1
expr_stmt|;
name|s
operator|.
name|reference
operator|=
literal|"HEAD^1"
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
name|i
index|]
argument_list|,
literal|"--verbose"
argument_list|)
condition|)
name|s
operator|.
name|verbose
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
name|i
index|]
argument_list|,
literal|"--untracked"
argument_list|)
condition|)
name|s
operator|.
name|untracked
operator|=
literal|1
expr_stmt|;
else|else
name|usage
argument_list|(
name|runstatus_usage
argument_list|)
expr_stmt|;
block|}
name|wt_status_print
argument_list|(
operator|&
name|s
argument_list|)
expr_stmt|;
return|return
name|s
operator|.
name|commitable
condition|?
literal|0
else|:
literal|1
return|;
block|}
end_function

end_unit

