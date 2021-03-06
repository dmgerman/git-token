begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"../git-compat-util.h"
end_include

begin_comment
comment|/*  * A merge sort implementation, simplified from the qsort implementation  * by Mike Haertel, which is a part of the GNU C Library.  */
end_comment

begin_function
DECL|function|msort_with_tmp
specifier|static
name|void
name|msort_with_tmp
parameter_list|(
name|void
modifier|*
name|b
parameter_list|,
name|size_t
name|n
parameter_list|,
name|size_t
name|s
parameter_list|,
name|int
function_decl|(
modifier|*
name|cmp
function_decl|)
parameter_list|(
specifier|const
name|void
modifier|*
parameter_list|,
specifier|const
name|void
modifier|*
parameter_list|)
parameter_list|,
name|char
modifier|*
name|t
parameter_list|)
block|{
name|char
modifier|*
name|tmp
decl_stmt|;
name|char
modifier|*
name|b1
decl_stmt|,
modifier|*
name|b2
decl_stmt|;
name|size_t
name|n1
decl_stmt|,
name|n2
decl_stmt|;
if|if
condition|(
name|n
operator|<=
literal|1
condition|)
return|return;
name|n1
operator|=
name|n
operator|/
literal|2
expr_stmt|;
name|n2
operator|=
name|n
operator|-
name|n1
expr_stmt|;
name|b1
operator|=
name|b
expr_stmt|;
name|b2
operator|=
operator|(
name|char
operator|*
operator|)
name|b
operator|+
operator|(
name|n1
operator|*
name|s
operator|)
expr_stmt|;
name|msort_with_tmp
argument_list|(
name|b1
argument_list|,
name|n1
argument_list|,
name|s
argument_list|,
name|cmp
argument_list|,
name|t
argument_list|)
expr_stmt|;
name|msort_with_tmp
argument_list|(
name|b2
argument_list|,
name|n2
argument_list|,
name|s
argument_list|,
name|cmp
argument_list|,
name|t
argument_list|)
expr_stmt|;
name|tmp
operator|=
name|t
expr_stmt|;
while|while
condition|(
name|n1
operator|>
literal|0
operator|&&
name|n2
operator|>
literal|0
condition|)
block|{
if|if
condition|(
name|cmp
argument_list|(
name|b1
argument_list|,
name|b2
argument_list|)
operator|<=
literal|0
condition|)
block|{
name|memcpy
argument_list|(
name|tmp
argument_list|,
name|b1
argument_list|,
name|s
argument_list|)
expr_stmt|;
name|tmp
operator|+=
name|s
expr_stmt|;
name|b1
operator|+=
name|s
expr_stmt|;
operator|--
name|n1
expr_stmt|;
block|}
else|else
block|{
name|memcpy
argument_list|(
name|tmp
argument_list|,
name|b2
argument_list|,
name|s
argument_list|)
expr_stmt|;
name|tmp
operator|+=
name|s
expr_stmt|;
name|b2
operator|+=
name|s
expr_stmt|;
operator|--
name|n2
expr_stmt|;
block|}
block|}
if|if
condition|(
name|n1
operator|>
literal|0
condition|)
name|memcpy
argument_list|(
name|tmp
argument_list|,
name|b1
argument_list|,
name|n1
operator|*
name|s
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|b
argument_list|,
name|t
argument_list|,
operator|(
name|n
operator|-
name|n2
operator|)
operator|*
name|s
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_qsort
name|void
name|git_qsort
parameter_list|(
name|void
modifier|*
name|b
parameter_list|,
name|size_t
name|n
parameter_list|,
name|size_t
name|s
parameter_list|,
name|int
function_decl|(
modifier|*
name|cmp
function_decl|)
parameter_list|(
specifier|const
name|void
modifier|*
parameter_list|,
specifier|const
name|void
modifier|*
parameter_list|)
parameter_list|)
block|{
specifier|const
name|size_t
name|size
init|=
name|st_mult
argument_list|(
name|n
argument_list|,
name|s
argument_list|)
decl_stmt|;
name|char
name|buf
index|[
literal|1024
index|]
decl_stmt|;
if|if
condition|(
name|size
operator|<
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
condition|)
block|{
comment|/* The temporary array fits on the small on-stack buffer. */
name|msort_with_tmp
argument_list|(
name|b
argument_list|,
name|n
argument_list|,
name|s
argument_list|,
name|cmp
argument_list|,
name|buf
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/* It's somewhat large, so malloc it.  */
name|char
modifier|*
name|tmp
init|=
name|xmalloc
argument_list|(
name|size
argument_list|)
decl_stmt|;
name|msort_with_tmp
argument_list|(
name|b
argument_list|,
name|n
argument_list|,
name|s
argument_list|,
name|cmp
argument_list|,
name|tmp
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tmp
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit

