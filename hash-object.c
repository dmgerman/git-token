begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  * Copyright (C) Junio C Hamano, 2005   */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
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
specifier|const
name|char
modifier|*
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
literal|"git-hash-object [-t<type>] [-w]<file>..."
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
literal|"blob"
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
decl_stmt|;
name|int
name|prefix_length
init|=
operator|-
literal|1
decl_stmt|;
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
name|die
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
else|else
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
argument_list|)
expr_stmt|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

