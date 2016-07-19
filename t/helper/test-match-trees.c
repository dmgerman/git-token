begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_function
DECL|function|cmd_main
name|int
name|cmd_main
parameter_list|(
name|int
name|ac
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|av
parameter_list|)
block|{
name|struct
name|object_id
name|hash1
decl_stmt|,
name|hash2
decl_stmt|,
name|shifted
decl_stmt|;
name|struct
name|tree
modifier|*
name|one
decl_stmt|,
modifier|*
name|two
decl_stmt|;
name|setup_git_directory
argument_list|()
expr_stmt|;
if|if
condition|(
name|get_oid
argument_list|(
name|av
index|[
literal|1
index|]
argument_list|,
operator|&
name|hash1
argument_list|)
condition|)
name|die
argument_list|(
literal|"cannot parse %s as an object name"
argument_list|,
name|av
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_oid
argument_list|(
name|av
index|[
literal|2
index|]
argument_list|,
operator|&
name|hash2
argument_list|)
condition|)
name|die
argument_list|(
literal|"cannot parse %s as an object name"
argument_list|,
name|av
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
name|one
operator|=
name|parse_tree_indirect
argument_list|(
name|hash1
operator|.
name|hash
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|one
condition|)
name|die
argument_list|(
literal|"not a tree-ish %s"
argument_list|,
name|av
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|two
operator|=
name|parse_tree_indirect
argument_list|(
name|hash2
operator|.
name|hash
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|two
condition|)
name|die
argument_list|(
literal|"not a tree-ish %s"
argument_list|,
name|av
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
name|shift_tree
argument_list|(
operator|&
name|one
operator|->
name|object
operator|.
name|oid
argument_list|,
operator|&
name|two
operator|->
name|object
operator|.
name|oid
argument_list|,
operator|&
name|shifted
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"shifted: %s\n"
argument_list|,
name|oid_to_hex
argument_list|(
operator|&
name|shifted
argument_list|)
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

