begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"sha1-array.h"
end_include

begin_function
DECL|function|print_sha1
specifier|static
name|int
name|print_sha1
parameter_list|(
specifier|const
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|puts
argument_list|(
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

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
name|struct
name|sha1_array
name|array
init|=
name|SHA1_ARRAY_INIT
decl_stmt|;
name|struct
name|strbuf
name|line
init|=
name|STRBUF_INIT
decl_stmt|;
while|while
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|line
argument_list|,
name|stdin
argument_list|)
operator|!=
name|EOF
condition|)
block|{
specifier|const
name|char
modifier|*
name|arg
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|skip_prefix
argument_list|(
name|line
operator|.
name|buf
argument_list|,
literal|"append "
argument_list|,
operator|&
name|arg
argument_list|)
condition|)
block|{
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|arg
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"not a hexadecimal SHA1: %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|sha1_array_append
argument_list|(
operator|&
name|array
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|skip_prefix
argument_list|(
name|line
operator|.
name|buf
argument_list|,
literal|"lookup "
argument_list|,
operator|&
name|arg
argument_list|)
condition|)
block|{
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|arg
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"not a hexadecimal SHA1: %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%d\n"
argument_list|,
name|sha1_array_lookup
argument_list|(
operator|&
name|array
argument_list|,
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|line
operator|.
name|buf
argument_list|,
literal|"clear"
argument_list|)
condition|)
name|sha1_array_clear
argument_list|(
operator|&
name|array
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|line
operator|.
name|buf
argument_list|,
literal|"for_each_unique"
argument_list|)
condition|)
name|sha1_array_for_each_unique
argument_list|(
operator|&
name|array
argument_list|,
name|print_sha1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
else|else
name|die
argument_list|(
literal|"unknown command: %s"
argument_list|,
name|line
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

