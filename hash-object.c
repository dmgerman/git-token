begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  * Copyright (C) Junio C Hamano, 2005  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_function
DECL|function|hash_object
specifier|static
name|void
name|hash_object
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|enum
name|object_type
name|type
parameter_list|,
name|int
name|write_object
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|path
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
operator|||
name|fstat
argument_list|(
name|fd
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
operator|||
name|index_fd
argument_list|(
name|sha1
argument_list|,
name|fd
argument_list|,
operator|&
name|st
argument_list|,
name|write_object
argument_list|,
name|type
argument_list|,
name|path
argument_list|)
condition|)
name|die
argument_list|(
name|write_object
condition|?
literal|"Unable to add %s to database"
else|:
literal|"Unable to hash %s"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|maybe_flush_or_die
argument_list|(
name|stdout
argument_list|,
literal|"hash to stdout"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|hash_stdin
specifier|static
name|void
name|hash_stdin
parameter_list|(
specifier|const
name|char
modifier|*
name|type
parameter_list|,
name|int
name|write_object
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|index_pipe
argument_list|(
name|sha1
argument_list|,
literal|0
argument_list|,
name|type
argument_list|,
name|write_object
argument_list|)
condition|)
name|die
argument_list|(
literal|"Unable to add stdin to database"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|hash_stdin_paths
specifier|static
name|void
name|hash_stdin_paths
parameter_list|(
specifier|const
name|char
modifier|*
name|type
parameter_list|,
name|int
name|write_objects
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
decl_stmt|,
name|nbuf
decl_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|buf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|nbuf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
while|while
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|buf
argument_list|,
name|stdin
argument_list|,
literal|'\n'
argument_list|)
operator|!=
name|EOF
condition|)
block|{
if|if
condition|(
name|buf
operator|.
name|buf
index|[
literal|0
index|]
operator|==
literal|'"'
condition|)
block|{
name|strbuf_reset
argument_list|(
operator|&
name|nbuf
argument_list|)
expr_stmt|;
if|if
condition|(
name|unquote_c_style
argument_list|(
operator|&
name|nbuf
argument_list|,
name|buf
operator|.
name|buf
argument_list|,
name|NULL
argument_list|)
condition|)
name|die
argument_list|(
literal|"line is badly quoted"
argument_list|)
expr_stmt|;
name|strbuf_swap
argument_list|(
operator|&
name|buf
argument_list|,
operator|&
name|nbuf
argument_list|)
expr_stmt|;
block|}
name|hash_object
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
name|type_from_string
argument_list|(
name|type
argument_list|)
argument_list|,
name|write_objects
argument_list|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|nbuf
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|hash_object_usage
specifier|static
specifier|const
name|char
name|hash_object_usage
index|[]
init|=
literal|"git hash-object [ [-t<type>] [-w] [--stdin]<file>... | --stdin-paths<<list-of-paths> ]"
decl_stmt|;
end_decl_stmt

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
decl_stmt|;
specifier|const
name|char
modifier|*
name|type
init|=
name|blob_type
decl_stmt|;
name|int
name|write_object
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|prefix
init|=
name|NULL
decl_stmt|;
name|int
name|prefix_length
init|=
operator|-
literal|1
decl_stmt|;
name|int
name|no_more_flags
init|=
literal|0
decl_stmt|;
name|int
name|hashstdin
init|=
literal|0
decl_stmt|;
name|int
name|stdin_paths
init|=
literal|0
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
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
if|if
condition|(
operator|!
name|no_more_flags
operator|&&
name|argv
index|[
name|i
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
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"-t"
argument_list|)
condition|)
block|{
if|if
condition|(
name|argc
operator|<=
operator|++
name|i
condition|)
name|usage
argument_list|(
name|hash_object_usage
argument_list|)
expr_stmt|;
name|type
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"-w"
argument_list|)
condition|)
block|{
if|if
condition|(
name|prefix_length
operator|<
literal|0
condition|)
block|{
name|prefix
operator|=
name|setup_git_directory
argument_list|()
expr_stmt|;
name|prefix_length
operator|=
name|prefix
condition|?
name|strlen
argument_list|(
name|prefix
argument_list|)
else|:
literal|0
expr_stmt|;
block|}
name|write_object
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--"
argument_list|)
condition|)
block|{
name|no_more_flags
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--help"
argument_list|)
condition|)
name|usage
argument_list|(
name|hash_object_usage
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--stdin-paths"
argument_list|)
condition|)
block|{
if|if
condition|(
name|hashstdin
condition|)
block|{
name|error
argument_list|(
literal|"Can't use --stdin-paths with --stdin"
argument_list|)
expr_stmt|;
name|usage
argument_list|(
name|hash_object_usage
argument_list|)
expr_stmt|;
block|}
name|stdin_paths
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--stdin"
argument_list|)
condition|)
block|{
if|if
condition|(
name|stdin_paths
condition|)
block|{
name|error
argument_list|(
literal|"Can't use %s with --stdin-paths"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|usage
argument_list|(
name|hash_object_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|hashstdin
condition|)
name|die
argument_list|(
literal|"Multiple --stdin arguments are not supported"
argument_list|)
expr_stmt|;
name|hashstdin
operator|=
literal|1
expr_stmt|;
block|}
else|else
name|usage
argument_list|(
name|hash_object_usage
argument_list|)
expr_stmt|;
block|}
else|else
block|{
specifier|const
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
name|stdin_paths
condition|)
block|{
name|error
argument_list|(
literal|"Can't specify files (such as \"%s\") with --stdin-paths"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|usage
argument_list|(
name|hash_object_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|hashstdin
condition|)
block|{
name|hash_stdin
argument_list|(
name|type
argument_list|,
name|write_object
argument_list|)
expr_stmt|;
name|hashstdin
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
literal|0
operator|<=
name|prefix_length
condition|)
name|arg
operator|=
name|prefix_filename
argument_list|(
name|prefix
argument_list|,
name|prefix_length
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|hash_object
argument_list|(
name|arg
argument_list|,
name|type_from_string
argument_list|(
name|type
argument_list|)
argument_list|,
name|write_object
argument_list|)
expr_stmt|;
name|no_more_flags
operator|=
literal|1
expr_stmt|;
block|}
block|}
if|if
condition|(
name|stdin_paths
condition|)
name|hash_stdin_paths
argument_list|(
name|type
argument_list|,
name|write_object
argument_list|)
expr_stmt|;
if|if
condition|(
name|hashstdin
condition|)
name|hash_stdin
argument_list|(
name|type
argument_list|,
name|write_object
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

