begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/* This is a phony C program that's only here to test xgettext message extraction */
end_comment

begin_decl_stmt
DECL|variable|help
specifier|const
name|char
name|help
index|[]
init|=
comment|/* TRANSLATORS: This is a test. You don't need to translate it. */
name|N_
argument_list|(
literal|"See 'git help COMMAND' for more information on a specific command."
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|void
parameter_list|)
block|{
comment|/* TRANSLATORS: This is a test. You don't need to translate it. */
name|puts
argument_list|(
name|_
argument_list|(
literal|"TEST: A C test string"
argument_list|)
argument_list|)
expr_stmt|;
comment|/* TRANSLATORS: This is a test. You don't need to translate it. */
name|printf
argument_list|(
name|_
argument_list|(
literal|"TEST: A C test string %s"
argument_list|)
argument_list|,
literal|"variable"
argument_list|)
expr_stmt|;
comment|/* TRANSLATORS: This is a test. You don't need to translate it. */
name|printf
argument_list|(
name|_
argument_list|(
literal|"TEST: Hello World!"
argument_list|)
argument_list|)
expr_stmt|;
comment|/* TRANSLATORS: This is a test. You don't need to translate it. */
name|printf
argument_list|(
name|_
argument_list|(
literal|"TEST: Old English Runes"
argument_list|)
argument_list|)
expr_stmt|;
comment|/* TRANSLATORS: This is a test. You don't need to translate it. */
name|printf
argument_list|(
name|_
argument_list|(
literal|"TEST: âsingleâ and âdoubleâ quotes"
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

