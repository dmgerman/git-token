begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_decl_stmt
DECL|variable|builtin_check_ref_format_usage
specifier|static
specifier|const
name|char
name|builtin_check_ref_format_usage
index|[]
init|=
literal|"git check-ref-format [--print]<refname>\n"
literal|"   or: git check-ref-format --branch<branchname-shorthand>"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_check_ref_format
name|int
name|cmd_check_ref_format
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
literal|"--branch"
argument_list|)
condition|)
block|{
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
if|if
condition|(
name|strbuf_check_branch_ref
argument_list|(
operator|&
name|sb
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
condition|)
name|die
argument_list|(
literal|"'%s' is not a valid branch name"
argument_list|,
name|argv
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sb
operator|.
name|buf
operator|+
literal|11
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|!=
literal|2
condition|)
name|usage
argument_list|(
name|builtin_check_ref_format_usage
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|check_ref_format
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
return|;
block|}
end_function

end_unit

