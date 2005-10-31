begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_include
include|#
directive|include
file|<unistd.h>
end_include

begin_include
include|#
directive|include
file|<errno.h>
end_include

begin_include
include|#
directive|include
file|"../cache.h"
end_include

begin_function
DECL|function|gitfakemmap
name|void
modifier|*
name|gitfakemmap
parameter_list|(
name|void
modifier|*
name|start
parameter_list|,
name|size_t
name|length
parameter_list|,
name|int
name|prot
parameter_list|,
name|int
name|flags
parameter_list|,
name|int
name|fd
parameter_list|,
name|off_t
name|offset
parameter_list|)
block|{
name|int
name|n
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|start
operator|!=
name|NULL
operator|||
operator|!
operator|(
name|flags
operator|&
name|MAP_PRIVATE
operator|)
condition|)
name|die
argument_list|(
literal|"Invalid usage of gitfakemmap."
argument_list|)
expr_stmt|;
if|if
condition|(
name|lseek
argument_list|(
name|fd
argument_list|,
name|offset
argument_list|,
name|SEEK_SET
argument_list|)
operator|<
literal|0
condition|)
block|{
name|errno
operator|=
name|EINVAL
expr_stmt|;
return|return
name|MAP_FAILED
return|;
block|}
name|start
operator|=
name|xmalloc
argument_list|(
name|length
argument_list|)
expr_stmt|;
if|if
condition|(
name|start
operator|==
name|NULL
condition|)
block|{
name|errno
operator|=
name|ENOMEM
expr_stmt|;
return|return
name|MAP_FAILED
return|;
block|}
while|while
condition|(
name|n
operator|<
name|length
condition|)
block|{
name|int
name|count
init|=
name|read
argument_list|(
name|fd
argument_list|,
name|start
operator|+
name|n
argument_list|,
name|length
operator|-
name|n
argument_list|)
decl_stmt|;
if|if
condition|(
name|count
operator|==
literal|0
condition|)
block|{
name|memset
argument_list|(
name|start
operator|+
name|n
argument_list|,
literal|0
argument_list|,
name|length
operator|-
name|n
argument_list|)
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|count
operator|<
literal|0
condition|)
block|{
name|free
argument_list|(
name|start
argument_list|)
expr_stmt|;
name|errno
operator|=
name|EACCES
expr_stmt|;
return|return
name|MAP_FAILED
return|;
block|}
name|n
operator|+=
name|count
expr_stmt|;
block|}
return|return
name|start
return|;
block|}
end_function

begin_function
DECL|function|gitfakemunmap
name|int
name|gitfakemunmap
parameter_list|(
name|void
modifier|*
name|start
parameter_list|,
name|size_t
name|length
parameter_list|)
block|{
name|free
argument_list|(
name|start
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

