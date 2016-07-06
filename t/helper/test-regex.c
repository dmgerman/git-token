begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_function
DECL|function|cmd_main
name|int
name|cmd_main
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|char
modifier|*
name|pat
init|=
literal|"[^={} \t]+"
decl_stmt|;
name|char
modifier|*
name|str
init|=
literal|"={}\nfred"
decl_stmt|;
name|regex_t
name|r
decl_stmt|;
name|regmatch_t
name|m
index|[
literal|1
index|]
decl_stmt|;
if|if
condition|(
name|regcomp
argument_list|(
operator|&
name|r
argument_list|,
name|pat
argument_list|,
name|REG_EXTENDED
operator||
name|REG_NEWLINE
argument_list|)
condition|)
name|die
argument_list|(
literal|"failed regcomp() for pattern '%s'"
argument_list|,
name|pat
argument_list|)
expr_stmt|;
if|if
condition|(
name|regexec
argument_list|(
operator|&
name|r
argument_list|,
name|str
argument_list|,
literal|1
argument_list|,
name|m
argument_list|,
literal|0
argument_list|)
condition|)
name|die
argument_list|(
literal|"no match of pattern '%s' to string '%s'"
argument_list|,
name|pat
argument_list|,
name|str
argument_list|)
expr_stmt|;
comment|/* http://sourceware.org/bugzilla/show_bug.cgi?id=3957  */
if|if
condition|(
name|m
index|[
literal|0
index|]
operator|.
name|rm_so
operator|==
literal|3
condition|)
comment|/* matches '\n' when it should not */
name|die
argument_list|(
literal|"regex bug confirmed: re-build git with NO_REGEX=1"
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

