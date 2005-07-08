begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_comment
comment|/* Help to copy the thing properly quoted for the shell safety.  * any single quote is replaced with '\'', and the caller is  * expected to enclose the result within a single quote pair.  *  * E.g.  *  original     sq_quote     result  *  name     ==> name      ==> 'name'  *  a b      ==> a b       ==> 'a b'  *  a'b      ==> a'\''b    ==> 'a'\''b'  */
end_comment

begin_function
DECL|function|sq_quote
name|char
modifier|*
name|sq_quote
parameter_list|(
specifier|const
name|char
modifier|*
name|src
parameter_list|)
block|{
specifier|static
name|char
modifier|*
name|buf
init|=
name|NULL
decl_stmt|;
name|int
name|cnt
decl_stmt|,
name|c
decl_stmt|;
specifier|const
name|char
modifier|*
name|cp
decl_stmt|;
name|char
modifier|*
name|bp
decl_stmt|;
comment|/* count bytes needed to store the quoted string. */
for|for
control|(
name|cnt
operator|=
literal|3
operator|,
name|cp
operator|=
name|src
init|;
operator|*
name|cp
condition|;
name|cnt
operator|++
operator|,
name|cp
operator|++
control|)
if|if
condition|(
operator|*
name|cp
operator|==
literal|'\''
condition|)
name|cnt
operator|+=
literal|3
expr_stmt|;
name|buf
operator|=
name|xmalloc
argument_list|(
name|cnt
argument_list|)
expr_stmt|;
name|bp
operator|=
name|buf
expr_stmt|;
operator|*
name|bp
operator|++
operator|=
literal|'\''
expr_stmt|;
while|while
condition|(
operator|(
name|c
operator|=
operator|*
name|src
operator|++
operator|)
condition|)
block|{
if|if
condition|(
name|c
operator|!=
literal|'\''
condition|)
operator|*
name|bp
operator|++
operator|=
name|c
expr_stmt|;
else|else
block|{
name|bp
operator|=
name|strcpy
argument_list|(
name|bp
argument_list|,
literal|"'\\''"
argument_list|)
expr_stmt|;
name|bp
operator|+=
literal|4
expr_stmt|;
block|}
block|}
operator|*
name|bp
operator|++
operator|=
literal|'\''
expr_stmt|;
operator|*
name|bp
operator|=
literal|0
expr_stmt|;
return|return
name|buf
return|;
block|}
end_function

end_unit

