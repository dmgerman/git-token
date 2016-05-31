begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_function
DECL|function|appended
name|int
name|appended
parameter_list|(
name|void
parameter_list|)
comment|// Begin of first part
block|{
name|int
name|i
decl_stmt|;
name|char
modifier|*
name|s
init|=
literal|"a string"
decl_stmt|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|s
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|99
init|;
name|i
operator|>=
literal|0
condition|;
name|i
operator|--
control|)
block|{
name|printf
argument_list|(
literal|"%d bottles of beer on the wall\n"
argument_list|,
name|i
argument_list|)
expr_stmt|;
block|}
name|printf
argument_list|(
literal|"End of first part\n"
argument_list|)
expr_stmt|;
end_function

end_unit

