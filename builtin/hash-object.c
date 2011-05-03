begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  * Copyright (C) Junio C Hamano, 2005  */
end_comment

begin_include
include|#
directive|include
file|"builtin.h"
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

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_function
DECL|function|hash_fd
specifier|static
name|void
name|hash_fd
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|char
modifier|*
name|type
parameter_list|,
name|int
name|write_object
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
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
if|if
condition|(
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
name|type_from_string
argument_list|(
name|type
argument_list|)
argument_list|,
name|path
argument_list|,
literal|1
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
specifier|const
name|char
modifier|*
name|type
parameter_list|,
name|int
name|write_object
parameter_list|,
specifier|const
name|char
modifier|*
name|vpath
parameter_list|)
block|{
name|int
name|fd
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
condition|)
name|die_errno
argument_list|(
literal|"Cannot open '%s'"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|hash_fd
argument_list|(
name|fd
argument_list|,
name|type
argument_list|,
name|write_object
argument_list|,
name|vpath
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|no_filters
specifier|static
name|int
name|no_filters
decl_stmt|;
end_decl_stmt

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
init|=
name|STRBUF_INIT
decl_stmt|,
name|nbuf
init|=
name|STRBUF_INIT
decl_stmt|;
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
name|type
argument_list|,
name|write_objects
argument_list|,
name|no_filters
condition|?
name|NULL
else|:
name|buf
operator|.
name|buf
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
modifier|*
specifier|const
name|hash_object_usage
index|[]
init|=
block|{
literal|"git hash-object [-t<type>] [-w] [--path=<file>|--no-filters] [--stdin] [--]<file>..."
block|,
literal|"git hash-object  --stdin-paths<<list-of-paths>"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|type
specifier|static
specifier|const
name|char
modifier|*
name|type
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|write_object
specifier|static
name|int
name|write_object
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|hashstdin
specifier|static
name|int
name|hashstdin
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|stdin_paths
specifier|static
name|int
name|stdin_paths
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|vpath
specifier|static
specifier|const
name|char
modifier|*
name|vpath
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|hash_object_options
specifier|static
specifier|const
name|struct
name|option
name|hash_object_options
index|[]
init|=
block|{
name|OPT_STRING
argument_list|(
literal|'t'
argument_list|,
name|NULL
argument_list|,
operator|&
name|type
argument_list|,
literal|"type"
argument_list|,
literal|"object type"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'w'
argument_list|,
name|NULL
argument_list|,
operator|&
name|write_object
argument_list|,
literal|"write the object into the object database"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"stdin"
argument_list|,
operator|&
name|hashstdin
argument_list|,
literal|"read the object from stdin"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"stdin-paths"
argument_list|,
operator|&
name|stdin_paths
argument_list|,
literal|"read file names from stdin"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"no-filters"
argument_list|,
operator|&
name|no_filters
argument_list|,
literal|"store file as is without filters"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"path"
argument_list|,
operator|&
name|vpath
argument_list|,
literal|"file"
argument_list|,
literal|"process file as it were from this path"
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_hash_object
name|int
name|cmd_hash_object
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
name|int
name|i
decl_stmt|;
name|int
name|prefix_length
init|=
operator|-
literal|1
decl_stmt|;
specifier|const
name|char
modifier|*
name|errstr
init|=
name|NULL
decl_stmt|;
name|type
operator|=
name|blob_type
expr_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|NULL
argument_list|,
name|hash_object_options
argument_list|,
name|hash_object_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|write_object
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
if|if
condition|(
name|vpath
operator|&&
name|prefix
condition|)
name|vpath
operator|=
name|prefix_filename
argument_list|(
name|prefix
argument_list|,
name|prefix_length
argument_list|,
name|vpath
argument_list|)
expr_stmt|;
block|}
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|stdin_paths
condition|)
block|{
if|if
condition|(
name|hashstdin
condition|)
name|errstr
operator|=
literal|"Can't use --stdin-paths with --stdin"
expr_stmt|;
elseif|else
if|if
condition|(
name|argc
condition|)
name|errstr
operator|=
literal|"Can't specify files with --stdin-paths"
expr_stmt|;
elseif|else
if|if
condition|(
name|vpath
condition|)
name|errstr
operator|=
literal|"Can't use --stdin-paths with --path"
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|hashstdin
operator|>
literal|1
condition|)
name|errstr
operator|=
literal|"Multiple --stdin arguments are not supported"
expr_stmt|;
if|if
condition|(
name|vpath
operator|&&
name|no_filters
condition|)
name|errstr
operator|=
literal|"Can't use --path with --no-filters"
expr_stmt|;
block|}
if|if
condition|(
name|errstr
condition|)
block|{
name|error
argument_list|(
literal|"%s"
argument_list|,
name|errstr
argument_list|)
expr_stmt|;
name|usage_with_options
argument_list|(
name|hash_object_usage
argument_list|,
name|hash_object_options
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|hashstdin
condition|)
name|hash_fd
argument_list|(
literal|0
argument_list|,
name|type
argument_list|,
name|write_object
argument_list|,
name|vpath
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
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
name|type
argument_list|,
name|write_object
argument_list|,
name|no_filters
condition|?
name|NULL
else|:
name|vpath
condition|?
name|vpath
else|:
name|arg
argument_list|)
expr_stmt|;
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
return|return
literal|0
return|;
block|}
end_function

end_unit

