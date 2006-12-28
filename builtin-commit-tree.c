begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

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
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"utf8.h"
end_include

begin_define
DECL|macro|BLOCKING
define|#
directive|define
name|BLOCKING
value|(1ul<< 14)
end_define

begin_comment
comment|/*  * FIXME! Share the code with "write-tree.c"  */
end_comment

begin_function
DECL|function|init_buffer
specifier|static
name|void
name|init_buffer
parameter_list|(
name|char
modifier|*
modifier|*
name|bufp
parameter_list|,
name|unsigned
name|int
modifier|*
name|sizep
parameter_list|)
block|{
name|char
modifier|*
name|buf
init|=
name|xmalloc
argument_list|(
name|BLOCKING
argument_list|)
decl_stmt|;
operator|*
name|sizep
operator|=
literal|0
expr_stmt|;
operator|*
name|bufp
operator|=
name|buf
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_buffer
specifier|static
name|void
name|add_buffer
parameter_list|(
name|char
modifier|*
modifier|*
name|bufp
parameter_list|,
name|unsigned
name|int
modifier|*
name|sizep
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|char
name|one_line
index|[
literal|2048
index|]
decl_stmt|;
name|va_list
name|args
decl_stmt|;
name|int
name|len
decl_stmt|;
name|unsigned
name|long
name|alloc
decl_stmt|,
name|size
decl_stmt|,
name|newsize
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|len
operator|=
name|vsnprintf
argument_list|(
name|one_line
argument_list|,
sizeof|sizeof
argument_list|(
name|one_line
argument_list|)
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
name|size
operator|=
operator|*
name|sizep
expr_stmt|;
name|newsize
operator|=
name|size
operator|+
name|len
operator|+
literal|1
expr_stmt|;
name|alloc
operator|=
operator|(
name|size
operator|+
literal|32767
operator|)
operator|&
operator|~
literal|32767
expr_stmt|;
name|buf
operator|=
operator|*
name|bufp
expr_stmt|;
if|if
condition|(
name|newsize
operator|>
name|alloc
condition|)
block|{
name|alloc
operator|=
operator|(
name|newsize
operator|+
literal|32767
operator|)
operator|&
operator|~
literal|32767
expr_stmt|;
name|buf
operator|=
name|xrealloc
argument_list|(
name|buf
argument_list|,
name|alloc
argument_list|)
expr_stmt|;
operator|*
name|bufp
operator|=
name|buf
expr_stmt|;
block|}
operator|*
name|sizep
operator|=
name|newsize
operator|-
literal|1
expr_stmt|;
name|memcpy
argument_list|(
name|buf
operator|+
name|size
argument_list|,
name|one_line
argument_list|,
name|len
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|check_valid
specifier|static
name|void
name|check_valid
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|expect
parameter_list|)
block|{
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
name|NULL
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s is not a valid object"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|expect
operator|&&
name|strcmp
argument_list|(
name|type
argument_list|,
name|expect
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s is not a valid '%s' object"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|expect
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * Having more than two parents is not strange at all, and this is  * how multi-way merges are represented.  */
end_comment

begin_define
DECL|macro|MAXPARENT
define|#
directive|define
name|MAXPARENT
value|(16)
end_define

begin_decl_stmt
DECL|variable|parent_sha1
specifier|static
name|unsigned
name|char
name|parent_sha1
index|[
name|MAXPARENT
index|]
index|[
literal|20
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|commit_tree_usage
specifier|static
specifier|const
name|char
name|commit_tree_usage
index|[]
init|=
literal|"git-commit-tree<sha1> [-p<sha1>]*< changelog"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|new_parent
specifier|static
name|int
name|new_parent
parameter_list|(
name|int
name|idx
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|parent_sha1
index|[
name|idx
index|]
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|idx
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|parent_sha1
index|[
name|i
index|]
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"duplicate parent %s ignored"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
block|}
return|return
literal|1
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|commit_utf8_warn
specifier|static
specifier|const
name|char
name|commit_utf8_warn
index|[]
init|=
literal|"Warning: commit message does not conform to UTF-8.\n"
literal|"You may want to amend it after fixing the message, or set the config\n"
literal|"variable i18n.commitencoding to the encoding your project uses.\n"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_commit_tree
name|int
name|cmd_commit_tree
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
name|parents
init|=
literal|0
decl_stmt|;
name|unsigned
name|char
name|tree_sha1
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|char
name|commit_sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
name|comment
index|[
literal|1000
index|]
decl_stmt|;
name|char
modifier|*
name|buffer
decl_stmt|;
name|unsigned
name|int
name|size
decl_stmt|;
name|int
name|encoding_is_utf8
decl_stmt|;
name|setup_ident
argument_list|()
expr_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|<
literal|2
condition|)
name|usage
argument_list|(
name|commit_tree_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|tree_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Not a valid object name %s"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|check_valid
argument_list|(
name|tree_sha1
argument_list|,
name|tree_type
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|2
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|+=
literal|2
control|)
block|{
specifier|const
name|char
modifier|*
name|a
decl_stmt|,
modifier|*
name|b
decl_stmt|;
name|a
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
name|b
operator|=
name|argv
index|[
name|i
operator|+
literal|1
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|b
operator|||
name|strcmp
argument_list|(
name|a
argument_list|,
literal|"-p"
argument_list|)
condition|)
name|usage
argument_list|(
name|commit_tree_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|parents
operator|>=
name|MAXPARENT
condition|)
name|die
argument_list|(
literal|"Too many parents (%d max)"
argument_list|,
name|MAXPARENT
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|b
argument_list|,
name|parent_sha1
index|[
name|parents
index|]
argument_list|)
condition|)
name|die
argument_list|(
literal|"Not a valid object name %s"
argument_list|,
name|b
argument_list|)
expr_stmt|;
name|check_valid
argument_list|(
name|parent_sha1
index|[
name|parents
index|]
argument_list|,
name|commit_type
argument_list|)
expr_stmt|;
if|if
condition|(
name|new_parent
argument_list|(
name|parents
argument_list|)
condition|)
name|parents
operator|++
expr_stmt|;
block|}
comment|/* Not having i18n.commitencoding is the same as having utf-8 */
name|encoding_is_utf8
operator|=
operator|(
operator|!
name|git_commit_encoding
operator|||
operator|!
name|strcmp
argument_list|(
name|git_commit_encoding
argument_list|,
literal|"utf-8"
argument_list|)
operator|)
expr_stmt|;
name|init_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"tree %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|tree_sha1
argument_list|)
argument_list|)
expr_stmt|;
comment|/* 	 * NOTE! This ordering means that the same exact tree merged with a 	 * different order of parents will be a _different_ changeset even 	 * if everything else stays the same. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|parents
condition|;
name|i
operator|++
control|)
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"parent %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|parent_sha1
index|[
name|i
index|]
argument_list|)
argument_list|)
expr_stmt|;
comment|/* Person/date information */
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"author %s\n"
argument_list|,
name|git_author_info
argument_list|(
literal|1
argument_list|)
argument_list|)
expr_stmt|;
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"committer %s\n"
argument_list|,
name|git_committer_info
argument_list|(
literal|1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|encoding_is_utf8
condition|)
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"encoding %s\n"
argument_list|,
name|git_commit_encoding
argument_list|)
expr_stmt|;
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"\n"
argument_list|)
expr_stmt|;
comment|/* And add the comment */
while|while
condition|(
name|fgets
argument_list|(
name|comment
argument_list|,
sizeof|sizeof
argument_list|(
name|comment
argument_list|)
argument_list|,
name|stdin
argument_list|)
operator|!=
name|NULL
condition|)
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"%s"
argument_list|,
name|comment
argument_list|)
expr_stmt|;
comment|/* And check the encoding */
name|buffer
index|[
name|size
index|]
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
name|encoding_is_utf8
operator|&&
operator|!
name|is_utf8
argument_list|(
name|buffer
argument_list|)
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
name|commit_utf8_warn
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|write_sha1_file
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|,
name|commit_type
argument_list|,
name|commit_sha1
argument_list|)
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|commit_sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
else|else
return|return
literal|1
return|;
block|}
end_function

end_unit

