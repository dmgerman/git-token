begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_decl_stmt
DECL|variable|update_server_info_usage
specifier|static
specifier|const
name|char
name|update_server_info_usage
index|[]
init|=
literal|"git-update-server-info [--force]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|ac
parameter_list|,
name|char
modifier|*
modifier|*
name|av
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|force
init|=
literal|0
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|ac
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|av
index|[
name|i
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
literal|"--force"
argument_list|,
name|av
index|[
name|i
index|]
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
literal|"-f"
argument_list|,
name|av
index|[
name|i
index|]
argument_list|)
condition|)
name|force
operator|=
literal|1
expr_stmt|;
else|else
name|usage
argument_list|(
name|update_server_info_usage
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|i
operator|!=
name|ac
condition|)
name|usage
argument_list|(
name|update_server_info_usage
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|update_server_info
argument_list|(
name|force
argument_list|)
return|;
block|}
end_function

end_unit

