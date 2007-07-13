begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2005, Junio C Hamano  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_decl_stmt
DECL|variable|lock_file_list
specifier|static
name|struct
name|lock_file
modifier|*
name|lock_file_list
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|alternate_index_output
specifier|static
specifier|const
name|char
modifier|*
name|alternate_index_output
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|remove_lock_file
specifier|static
name|void
name|remove_lock_file
parameter_list|(
name|void
parameter_list|)
block|{
name|pid_t
name|me
init|=
name|getpid
argument_list|()
decl_stmt|;
while|while
condition|(
name|lock_file_list
condition|)
block|{
if|if
condition|(
name|lock_file_list
operator|->
name|owner
operator|==
name|me
operator|&&
name|lock_file_list
operator|->
name|filename
index|[
literal|0
index|]
condition|)
name|unlink
argument_list|(
name|lock_file_list
operator|->
name|filename
argument_list|)
expr_stmt|;
name|lock_file_list
operator|=
name|lock_file_list
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|remove_lock_file_on_signal
specifier|static
name|void
name|remove_lock_file_on_signal
parameter_list|(
name|int
name|signo
parameter_list|)
block|{
name|remove_lock_file
argument_list|()
expr_stmt|;
name|signal
argument_list|(
name|SIGINT
argument_list|,
name|SIG_DFL
argument_list|)
expr_stmt|;
name|raise
argument_list|(
name|signo
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|lock_file
specifier|static
name|int
name|lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|sprintf
argument_list|(
name|lk
operator|->
name|filename
argument_list|,
literal|"%s.lock"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|lk
operator|->
name|filename
argument_list|,
name|O_RDWR
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|fd
condition|)
block|{
if|if
condition|(
operator|!
name|lock_file_list
condition|)
block|{
name|signal
argument_list|(
name|SIGINT
argument_list|,
name|remove_lock_file_on_signal
argument_list|)
expr_stmt|;
name|atexit
argument_list|(
name|remove_lock_file
argument_list|)
expr_stmt|;
block|}
name|lk
operator|->
name|owner
operator|=
name|getpid
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|lk
operator|->
name|on_list
condition|)
block|{
name|lk
operator|->
name|next
operator|=
name|lock_file_list
expr_stmt|;
name|lock_file_list
operator|=
name|lk
expr_stmt|;
name|lk
operator|->
name|on_list
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|adjust_shared_perm
argument_list|(
name|lk
operator|->
name|filename
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"cannot fix permission bits on %s"
argument_list|,
name|lk
operator|->
name|filename
argument_list|)
return|;
block|}
else|else
name|lk
operator|->
name|filename
index|[
literal|0
index|]
operator|=
literal|0
expr_stmt|;
return|return
name|fd
return|;
block|}
end_function

begin_function
DECL|function|hold_lock_file_for_update
name|int
name|hold_lock_file_for_update
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|die_on_error
parameter_list|)
block|{
name|int
name|fd
init|=
name|lock_file
argument_list|(
name|lk
argument_list|,
name|path
argument_list|)
decl_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
operator|&&
name|die_on_error
condition|)
name|die
argument_list|(
literal|"unable to create '%s.lock': %s"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|fd
return|;
block|}
end_function

begin_function
DECL|function|commit_lock_file
name|int
name|commit_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
block|{
name|char
name|result_file
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|int
name|i
decl_stmt|;
name|strcpy
argument_list|(
name|result_file
argument_list|,
name|lk
operator|->
name|filename
argument_list|)
expr_stmt|;
name|i
operator|=
name|strlen
argument_list|(
name|result_file
argument_list|)
operator|-
literal|5
expr_stmt|;
comment|/* .lock */
name|result_file
index|[
name|i
index|]
operator|=
literal|0
expr_stmt|;
name|i
operator|=
name|rename
argument_list|(
name|lk
operator|->
name|filename
argument_list|,
name|result_file
argument_list|)
expr_stmt|;
name|lk
operator|->
name|filename
index|[
literal|0
index|]
operator|=
literal|0
expr_stmt|;
return|return
name|i
return|;
block|}
end_function

begin_function
DECL|function|hold_locked_index
name|int
name|hold_locked_index
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|,
name|int
name|die_on_error
parameter_list|)
block|{
return|return
name|hold_lock_file_for_update
argument_list|(
name|lk
argument_list|,
name|get_index_file
argument_list|()
argument_list|,
name|die_on_error
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|set_alternate_index_output
name|void
name|set_alternate_index_output
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|alternate_index_output
operator|=
name|name
expr_stmt|;
block|}
end_function

begin_function
DECL|function|commit_locked_index
name|int
name|commit_locked_index
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
block|{
if|if
condition|(
name|alternate_index_output
condition|)
block|{
name|int
name|result
init|=
name|rename
argument_list|(
name|lk
operator|->
name|filename
argument_list|,
name|alternate_index_output
argument_list|)
decl_stmt|;
name|lk
operator|->
name|filename
index|[
literal|0
index|]
operator|=
literal|0
expr_stmt|;
return|return
name|result
return|;
block|}
else|else
return|return
name|commit_lock_file
argument_list|(
name|lk
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|rollback_lock_file
name|void
name|rollback_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
block|{
if|if
condition|(
name|lk
operator|->
name|filename
index|[
literal|0
index|]
condition|)
name|unlink
argument_list|(
name|lk
operator|->
name|filename
argument_list|)
expr_stmt|;
name|lk
operator|->
name|filename
index|[
literal|0
index|]
operator|=
literal|0
expr_stmt|;
block|}
end_function

end_unit

