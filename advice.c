begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_decl_stmt
DECL|variable|advice_push_nonfastforward
name|int
name|advice_push_nonfastforward
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|advice_status_hints
name|int
name|advice_status_hints
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|advice_commit_before_merge
name|int
name|advice_commit_before_merge
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|advice_implicit_identity
name|int
name|advice_implicit_identity
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_struct
specifier|static
struct|struct
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|preference
name|int
modifier|*
name|preference
decl_stmt|;
DECL|variable|advice_config
block|}
name|advice_config
index|[]
init|=
block|{
block|{
literal|"pushnonfastforward"
block|,
operator|&
name|advice_push_nonfastforward
block|}
block|,
block|{
literal|"statushints"
block|,
operator|&
name|advice_status_hints
block|}
block|,
block|{
literal|"commitbeforemerge"
block|,
operator|&
name|advice_commit_before_merge
block|}
block|,
block|{
literal|"implicitidentity"
block|,
operator|&
name|advice_implicit_identity
block|}
block|, }
struct|;
end_struct

begin_function
DECL|function|git_default_advice_config
name|int
name|git_default_advice_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|k
init|=
name|skip_prefix
argument_list|(
name|var
argument_list|,
literal|"advice."
argument_list|)
decl_stmt|;
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ARRAY_SIZE
argument_list|(
name|advice_config
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|strcmp
argument_list|(
name|k
argument_list|,
name|advice_config
index|[
name|i
index|]
operator|.
name|name
argument_list|)
condition|)
continue|continue;
operator|*
name|advice_config
index|[
name|i
index|]
operator|.
name|preference
operator|=
name|git_config_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

