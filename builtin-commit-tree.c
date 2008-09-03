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
name|enum
name|object_type
name|expect
parameter_list|)
block|{
name|enum
name|object_type
name|type
init|=
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|)
decl_stmt|;
if|if
condition|(
name|type
operator|<
literal|0
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
name|type
operator|!=
name|expect
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
name|typename
argument_list|(
name|expect
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

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
name|void
name|new_parent
parameter_list|(
name|struct
name|commit
modifier|*
name|parent
parameter_list|,
name|struct
name|commit_list
modifier|*
modifier|*
name|parents_p
parameter_list|)
block|{
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|parent
operator|->
name|object
operator|.
name|sha1
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|parents
decl_stmt|;
for|for
control|(
name|parents
operator|=
operator|*
name|parents_p
init|;
name|parents
condition|;
name|parents
operator|=
name|parents
operator|->
name|next
control|)
block|{
if|if
condition|(
name|parents
operator|->
name|item
operator|==
name|parent
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
return|return;
block|}
name|parents_p
operator|=
operator|&
name|parents
operator|->
name|next
expr_stmt|;
block|}
name|commit_list_insert
argument_list|(
name|parent
argument_list|,
name|parents_p
argument_list|)
expr_stmt|;
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
DECL|function|commit_tree
name|int
name|commit_tree
parameter_list|(
specifier|const
name|char
modifier|*
name|msg
parameter_list|,
name|unsigned
name|char
modifier|*
name|tree
parameter_list|,
name|struct
name|commit_list
modifier|*
name|parents
parameter_list|,
name|unsigned
name|char
modifier|*
name|ret
parameter_list|)
block|{
name|int
name|encoding_is_utf8
decl_stmt|;
name|struct
name|strbuf
name|buffer
decl_stmt|;
name|check_valid
argument_list|(
name|tree
argument_list|,
name|OBJ_TREE
argument_list|)
expr_stmt|;
comment|/* Not having i18n.commitencoding is the same as having utf-8 */
name|encoding_is_utf8
operator|=
name|is_encoding_utf8
argument_list|(
name|git_commit_encoding
argument_list|)
expr_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|buffer
argument_list|,
literal|8192
argument_list|)
expr_stmt|;
comment|/* should avoid reallocs for the headers */
name|strbuf_addf
argument_list|(
operator|&
name|buffer
argument_list|,
literal|"tree %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|tree
argument_list|)
argument_list|)
expr_stmt|;
comment|/* 	 * NOTE! This ordering means that the same exact tree merged with a 	 * different order of parents will be a _different_ changeset even 	 * if everything else stays the same. 	 */
while|while
condition|(
name|parents
condition|)
block|{
name|struct
name|commit_list
modifier|*
name|next
init|=
name|parents
operator|->
name|next
decl_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|buffer
argument_list|,
literal|"parent %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|parents
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|parents
argument_list|)
expr_stmt|;
name|parents
operator|=
name|next
expr_stmt|;
block|}
comment|/* Person/date information */
name|strbuf_addf
argument_list|(
operator|&
name|buffer
argument_list|,
literal|"author %s\n"
argument_list|,
name|git_author_info
argument_list|(
name|IDENT_ERROR_ON_NO_NAME
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|buffer
argument_list|,
literal|"committer %s\n"
argument_list|,
name|git_committer_info
argument_list|(
name|IDENT_ERROR_ON_NO_NAME
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|encoding_is_utf8
condition|)
name|strbuf_addf
argument_list|(
operator|&
name|buffer
argument_list|,
literal|"encoding %s\n"
argument_list|,
name|git_commit_encoding
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
operator|&
name|buffer
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
comment|/* And add the comment */
name|strbuf_addstr
argument_list|(
operator|&
name|buffer
argument_list|,
name|msg
argument_list|)
expr_stmt|;
comment|/* And check the encoding */
if|if
condition|(
name|encoding_is_utf8
operator|&&
operator|!
name|is_utf8
argument_list|(
name|buffer
operator|.
name|buf
argument_list|)
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
name|commit_utf8_warn
argument_list|)
expr_stmt|;
return|return
name|write_sha1_file
argument_list|(
name|buffer
operator|.
name|buf
argument_list|,
name|buffer
operator|.
name|len
argument_list|,
name|commit_type
argument_list|,
name|ret
argument_list|)
return|;
block|}
end_function

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
name|struct
name|commit_list
modifier|*
name|parents
init|=
name|NULL
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
name|struct
name|strbuf
name|buffer
init|=
name|STRBUF_INIT
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
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
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
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
name|get_sha1
argument_list|(
name|b
argument_list|,
name|sha1
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
name|sha1
argument_list|,
name|OBJ_COMMIT
argument_list|)
expr_stmt|;
name|new_parent
argument_list|(
name|lookup_commit
argument_list|(
name|sha1
argument_list|)
argument_list|,
operator|&
name|parents
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|strbuf_read
argument_list|(
operator|&
name|buffer
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"git commit-tree: read returned %s"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit_tree
argument_list|(
name|buffer
operator|.
name|buf
argument_list|,
name|tree_sha1
argument_list|,
name|parents
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

