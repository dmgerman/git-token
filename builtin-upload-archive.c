begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2006 Franck Bui-Huu  */
end_comment

begin_include
include|#
directive|include
file|<time.h>
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"archive.h"
end_include

begin_include
include|#
directive|include
file|"pkt-line.h"
end_include

begin_decl_stmt
DECL|variable|upload_archive_usage
specifier|static
specifier|const
name|char
name|upload_archive_usage
index|[]
init|=
literal|"git-upload-archive<repo>"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_upload_archive
name|int
name|cmd_upload_archive
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|struct
name|archiver
name|ar
decl_stmt|;
specifier|const
name|char
modifier|*
name|sent_argv
index|[
name|MAX_ARGS
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|arg_cmd
init|=
literal|"argument "
decl_stmt|;
name|char
modifier|*
name|p
decl_stmt|,
name|buf
index|[
literal|4096
index|]
decl_stmt|;
name|int
name|treeish_idx
decl_stmt|;
name|int
name|sent_argc
decl_stmt|;
name|int
name|len
decl_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|2
condition|)
name|usage
argument_list|(
name|upload_archive_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|strlen
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
operator|>
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
condition|)
name|die
argument_list|(
literal|"insanely long repository name"
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|buf
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
comment|/* enter-repo smudges its argument */
if|if
condition|(
operator|!
name|enter_repo
argument_list|(
name|buf
argument_list|,
literal|0
argument_list|)
condition|)
name|die
argument_list|(
literal|"not a git archive"
argument_list|)
expr_stmt|;
comment|/* put received options in sent_argv[] */
name|sent_argc
operator|=
literal|1
expr_stmt|;
name|sent_argv
index|[
literal|0
index|]
operator|=
literal|"git-upload-archive"
expr_stmt|;
for|for
control|(
name|p
operator|=
name|buf
init|;
condition|;
control|)
block|{
comment|/* This will die if not enough free space in buf */
name|len
operator|=
name|packet_read_line
argument_list|(
literal|0
argument_list|,
name|p
argument_list|,
operator|(
name|buf
operator|+
sizeof|sizeof
name|buf
operator|)
operator|-
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|==
literal|0
condition|)
break|break;
comment|/* got a flush */
if|if
condition|(
name|sent_argc
operator|>
name|MAX_ARGS
operator|-
literal|2
condition|)
name|die
argument_list|(
literal|"Too many options (>29)"
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
condition|)
block|{
name|p
index|[
operator|--
name|len
index|]
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|len
operator|<
name|strlen
argument_list|(
name|arg_cmd
argument_list|)
operator|||
name|strncmp
argument_list|(
name|arg_cmd
argument_list|,
name|p
argument_list|,
name|strlen
argument_list|(
name|arg_cmd
argument_list|)
argument_list|)
condition|)
name|die
argument_list|(
literal|"'argument' token or flush expected"
argument_list|)
expr_stmt|;
name|len
operator|-=
name|strlen
argument_list|(
name|arg_cmd
argument_list|)
expr_stmt|;
name|memmove
argument_list|(
name|p
argument_list|,
name|p
operator|+
name|strlen
argument_list|(
name|arg_cmd
argument_list|)
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|sent_argv
index|[
name|sent_argc
operator|++
index|]
operator|=
name|p
expr_stmt|;
name|p
operator|+=
name|len
expr_stmt|;
operator|*
name|p
operator|++
operator|=
literal|0
expr_stmt|;
block|}
name|sent_argv
index|[
name|sent_argc
index|]
operator|=
name|NULL
expr_stmt|;
comment|/* parse all options sent by the client */
name|treeish_idx
operator|=
name|parse_archive_args
argument_list|(
name|sent_argc
argument_list|,
name|sent_argv
argument_list|,
operator|&
name|ar
argument_list|)
expr_stmt|;
name|parse_treeish_arg
argument_list|(
name|sent_argv
operator|+
name|treeish_idx
argument_list|,
operator|&
name|ar
operator|.
name|args
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|parse_pathspec_arg
argument_list|(
name|sent_argv
operator|+
name|treeish_idx
operator|+
literal|1
argument_list|,
operator|&
name|ar
operator|.
name|args
argument_list|)
expr_stmt|;
name|packet_write
argument_list|(
literal|1
argument_list|,
literal|"ACK\n"
argument_list|)
expr_stmt|;
name|packet_flush
argument_list|(
literal|1
argument_list|)
expr_stmt|;
return|return
name|ar
operator|.
name|write_archive
argument_list|(
operator|&
name|ar
operator|.
name|args
argument_list|)
return|;
block|}
end_function

end_unit

