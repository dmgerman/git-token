begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"pkt-line.h"
end_include

begin_decl_stmt
DECL|variable|peek_remote_usage
specifier|static
specifier|const
name|char
name|peek_remote_usage
index|[]
init|=
literal|"git-peek-remote [--upload-pack=<git-upload-pack>] [<host>:]<directory>"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|uploadpack
specifier|static
specifier|const
name|char
modifier|*
name|uploadpack
init|=
literal|"git-upload-pack"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|peek_remote
specifier|static
name|int
name|peek_remote
parameter_list|(
name|int
name|fd
index|[
literal|2
index|]
parameter_list|,
name|unsigned
name|flags
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
name|get_remote_heads
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|,
operator|&
name|ref
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
name|flags
argument_list|)
expr_stmt|;
name|packet_flush
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
while|while
condition|(
name|ref
condition|)
block|{
name|printf
argument_list|(
literal|"%s	%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|ref
operator|->
name|old_sha1
argument_list|)
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|ref
operator|=
name|ref
operator|->
name|next
expr_stmt|;
block|}
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
name|int
name|i
decl_stmt|,
name|ret
decl_stmt|;
name|char
modifier|*
name|dest
init|=
name|NULL
decl_stmt|;
name|int
name|fd
index|[
literal|2
index|]
decl_stmt|;
name|pid_t
name|pid
decl_stmt|;
name|int
name|nongit
init|=
literal|0
decl_stmt|;
name|unsigned
name|flags
init|=
literal|0
decl_stmt|;
name|setup_git_directory_gently
argument_list|(
operator|&
name|nongit
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|*
name|arg
operator|==
literal|'-'
condition|)
block|{
if|if
condition|(
operator|!
name|strncmp
argument_list|(
literal|"--upload-pack="
argument_list|,
name|arg
argument_list|,
literal|14
argument_list|)
condition|)
block|{
name|uploadpack
operator|=
name|arg
operator|+
literal|14
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
literal|"--exec="
argument_list|,
name|arg
argument_list|,
literal|7
argument_list|)
condition|)
block|{
name|uploadpack
operator|=
name|arg
operator|+
literal|7
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"--tags"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|flags
operator||=
name|REF_TAGS
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"--heads"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|flags
operator||=
name|REF_HEADS
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"--refs"
argument_list|,
name|arg
argument_list|)
condition|)
block|{
name|flags
operator||=
name|REF_NORMAL
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|peek_remote_usage
argument_list|)
expr_stmt|;
block|}
name|dest
operator|=
name|arg
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|dest
operator|||
name|i
operator|!=
name|argc
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|peek_remote_usage
argument_list|)
expr_stmt|;
name|pid
operator|=
name|git_connect
argument_list|(
name|fd
argument_list|,
name|dest
argument_list|,
name|uploadpack
argument_list|)
expr_stmt|;
if|if
condition|(
name|pid
operator|<
literal|0
condition|)
return|return
literal|1
return|;
name|ret
operator|=
name|peek_remote
argument_list|(
name|fd
argument_list|,
name|flags
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|ret
operator||=
name|finish_connect
argument_list|(
name|pid
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|ret
return|;
block|}
end_function

end_unit

