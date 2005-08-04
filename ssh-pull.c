begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"rsh.h"
end_include

begin_include
include|#
directive|include
file|"pull.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_decl_stmt
DECL|variable|fd_in
specifier|static
name|int
name|fd_in
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|fd_out
specifier|static
name|int
name|fd_out
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|remote_version
specifier|static
name|unsigned
name|char
name|remote_version
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|local_version
specifier|static
name|unsigned
name|char
name|local_version
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|force_write
specifier|static
name|ssize_t
name|force_write
parameter_list|(
name|int
name|fd
parameter_list|,
name|void
modifier|*
name|buffer
parameter_list|,
name|size_t
name|length
parameter_list|)
block|{
name|ssize_t
name|ret
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|ret
operator|<
name|length
condition|)
block|{
name|ssize_t
name|size
init|=
name|write
argument_list|(
name|fd
argument_list|,
name|buffer
operator|+
name|ret
argument_list|,
name|length
operator|-
name|ret
argument_list|)
decl_stmt|;
if|if
condition|(
name|size
operator|<
literal|0
condition|)
block|{
return|return
name|size
return|;
block|}
if|if
condition|(
name|size
operator|==
literal|0
condition|)
block|{
return|return
name|ret
return|;
block|}
name|ret
operator|+=
name|size
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|prefetch
name|void
name|prefetch
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|char
name|type
init|=
literal|'o'
decl_stmt|;
name|force_write
argument_list|(
name|fd_out
argument_list|,
operator|&
name|type
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|force_write
argument_list|(
name|fd_out
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
comment|//memcpy(requested + 20 * prefetches++, sha1, 20);
block|}
end_function

begin_decl_stmt
DECL|variable|conn_buf
specifier|static
name|char
name|conn_buf
index|[
literal|4096
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|conn_buf_posn
specifier|static
name|size_t
name|conn_buf_posn
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|fetch
name|int
name|fetch
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|int
name|ret
decl_stmt|;
name|signed
name|char
name|remote
decl_stmt|;
if|if
condition|(
name|conn_buf_posn
condition|)
block|{
name|remote
operator|=
name|conn_buf
index|[
literal|0
index|]
expr_stmt|;
name|memmove
argument_list|(
name|conn_buf
argument_list|,
name|conn_buf
operator|+
literal|1
argument_list|,
operator|--
name|conn_buf_posn
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|read
argument_list|(
name|fd_in
argument_list|,
operator|&
name|remote
argument_list|,
literal|1
argument_list|)
operator|<
literal|1
condition|)
return|return
operator|-
literal|1
return|;
block|}
comment|//fprintf(stderr, "Got %d\n", remote);
if|if
condition|(
name|remote
operator|<
literal|0
condition|)
return|return
name|remote
return|;
name|ret
operator|=
name|write_sha1_from_fd
argument_list|(
name|sha1
argument_list|,
name|fd_in
argument_list|,
name|conn_buf
argument_list|,
literal|4096
argument_list|,
operator|&
name|conn_buf_posn
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
name|pull_say
argument_list|(
literal|"got %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|get_version
specifier|static
name|int
name|get_version
parameter_list|(
name|void
parameter_list|)
block|{
name|char
name|type
init|=
literal|'v'
decl_stmt|;
name|write
argument_list|(
name|fd_out
argument_list|,
operator|&
name|type
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|write
argument_list|(
name|fd_out
argument_list|,
operator|&
name|local_version
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|read
argument_list|(
name|fd_in
argument_list|,
operator|&
name|remote_version
argument_list|,
literal|1
argument_list|)
operator|<
literal|1
condition|)
block|{
return|return
name|error
argument_list|(
literal|"Couldn't read version from remote end"
argument_list|)
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fetch_ref
name|int
name|fetch_ref
parameter_list|(
name|char
modifier|*
name|ref
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|signed
name|char
name|remote
decl_stmt|;
name|char
name|type
init|=
literal|'r'
decl_stmt|;
name|write
argument_list|(
name|fd_out
argument_list|,
operator|&
name|type
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|write
argument_list|(
name|fd_out
argument_list|,
name|ref
argument_list|,
name|strlen
argument_list|(
name|ref
argument_list|)
operator|+
literal|1
argument_list|)
expr_stmt|;
name|read
argument_list|(
name|fd_in
argument_list|,
operator|&
name|remote
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|remote
operator|<
literal|0
condition|)
return|return
name|remote
return|;
name|read
argument_list|(
name|fd_in
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|char
modifier|*
name|commit_id
decl_stmt|;
name|char
modifier|*
name|url
decl_stmt|;
name|int
name|arg
init|=
literal|1
decl_stmt|;
specifier|const
name|char
modifier|*
name|prog
init|=
name|getenv
argument_list|(
literal|"GIT_SSH_PUSH"
argument_list|)
condition|?
else|:
literal|"git-ssh-push"
decl_stmt|;
while|while
condition|(
name|arg
operator|<
name|argc
operator|&&
name|argv
index|[
name|arg
index|]
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'t'
condition|)
block|{
name|get_tree
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'c'
condition|)
block|{
name|get_history
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'a'
condition|)
block|{
name|get_all
operator|=
literal|1
expr_stmt|;
name|get_tree
operator|=
literal|1
expr_stmt|;
name|get_history
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'v'
condition|)
block|{
name|get_verbosely
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'w'
condition|)
block|{
name|write_ref
operator|=
name|argv
index|[
name|arg
operator|+
literal|1
index|]
expr_stmt|;
name|arg
operator|++
expr_stmt|;
block|}
name|arg
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|<
name|arg
operator|+
literal|2
condition|)
block|{
name|usage
argument_list|(
literal|"git-ssh-pull [-c] [-t] [-a] [-v] [-d] [--recover] [-w ref] commit-id url"
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
name|commit_id
operator|=
name|argv
index|[
name|arg
index|]
expr_stmt|;
name|url
operator|=
name|argv
index|[
name|arg
operator|+
literal|1
index|]
expr_stmt|;
if|if
condition|(
name|setup_connection
argument_list|(
operator|&
name|fd_in
argument_list|,
operator|&
name|fd_out
argument_list|,
name|prog
argument_list|,
name|url
argument_list|,
name|arg
argument_list|,
name|argv
operator|+
literal|1
argument_list|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|get_version
argument_list|()
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|pull
argument_list|(
name|commit_id
argument_list|)
condition|)
return|return
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

end_unit

