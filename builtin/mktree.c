begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - the stupid content tracker  *  * Copyright (c) Junio C Hamano, 2006, 2009  */
end_comment

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_struct
DECL|struct|treeent
specifier|static
struct|struct
name|treeent
block|{
DECL|member|mode
name|unsigned
name|mode
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|len
name|int
name|len
decl_stmt|;
DECL|member|name
name|char
name|name
index|[
name|FLEX_ARRAY
index|]
decl_stmt|;
DECL|variable|entries
block|}
modifier|*
modifier|*
name|entries
struct|;
end_struct

begin_decl_stmt
DECL|variable|alloc
DECL|variable|used
specifier|static
name|int
name|alloc
decl_stmt|,
name|used
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|append_to_tree
specifier|static
name|void
name|append_to_tree
parameter_list|(
name|unsigned
name|mode
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|char
modifier|*
name|path
parameter_list|)
block|{
name|struct
name|treeent
modifier|*
name|ent
decl_stmt|;
name|size_t
name|len
init|=
name|strlen
argument_list|(
name|path
argument_list|)
decl_stmt|;
if|if
condition|(
name|strchr
argument_list|(
name|path
argument_list|,
literal|'/'
argument_list|)
condition|)
name|die
argument_list|(
literal|"path %s contains slash"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|FLEX_ALLOC_MEM
argument_list|(
name|ent
argument_list|,
name|name
argument_list|,
name|path
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|ent
operator|->
name|mode
operator|=
name|mode
expr_stmt|;
name|ent
operator|->
name|len
operator|=
name|len
expr_stmt|;
name|hashcpy
argument_list|(
name|ent
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|entries
argument_list|,
name|used
operator|+
literal|1
argument_list|,
name|alloc
argument_list|)
expr_stmt|;
name|entries
index|[
name|used
operator|++
index|]
operator|=
name|ent
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ent_compare
specifier|static
name|int
name|ent_compare
parameter_list|(
specifier|const
name|void
modifier|*
name|a_
parameter_list|,
specifier|const
name|void
modifier|*
name|b_
parameter_list|)
block|{
name|struct
name|treeent
modifier|*
name|a
init|=
operator|*
operator|(
expr|struct
name|treeent
operator|*
operator|*
operator|)
name|a_
decl_stmt|;
name|struct
name|treeent
modifier|*
name|b
init|=
operator|*
operator|(
expr|struct
name|treeent
operator|*
operator|*
operator|)
name|b_
decl_stmt|;
return|return
name|base_name_compare
argument_list|(
name|a
operator|->
name|name
argument_list|,
name|a
operator|->
name|len
argument_list|,
name|a
operator|->
name|mode
argument_list|,
name|b
operator|->
name|name
argument_list|,
name|b
operator|->
name|len
argument_list|,
name|b
operator|->
name|mode
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|write_tree
specifier|static
name|void
name|write_tree
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
decl_stmt|;
name|size_t
name|size
decl_stmt|;
name|int
name|i
decl_stmt|;
name|qsort
argument_list|(
name|entries
argument_list|,
name|used
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|entries
argument_list|)
argument_list|,
name|ent_compare
argument_list|)
expr_stmt|;
for|for
control|(
name|size
operator|=
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|used
condition|;
name|i
operator|++
control|)
name|size
operator|+=
literal|32
operator|+
name|entries
index|[
name|i
index|]
operator|->
name|len
expr_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|buf
argument_list|,
name|size
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
name|used
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|treeent
modifier|*
name|ent
init|=
name|entries
index|[
name|i
index|]
decl_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"%o %s%c"
argument_list|,
name|ent
operator|->
name|mode
argument_list|,
name|ent
operator|->
name|name
argument_list|,
literal|'\0'
argument_list|)
expr_stmt|;
name|strbuf_add
argument_list|(
operator|&
name|buf
argument_list|,
name|ent
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
block|}
name|write_sha1_file
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
name|buf
operator|.
name|len
argument_list|,
name|tree_type
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|mktree_usage
specifier|static
specifier|const
name|char
modifier|*
name|mktree_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git mktree [-z] [--missing] [--batch]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|mktree_line
specifier|static
name|void
name|mktree_line
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|,
name|int
name|line_termination
parameter_list|,
name|int
name|allow_missing
parameter_list|)
block|{
name|char
modifier|*
name|ptr
decl_stmt|,
modifier|*
name|ntr
decl_stmt|;
name|unsigned
name|mode
decl_stmt|;
name|enum
name|object_type
name|mode_type
decl_stmt|;
comment|/* object type derived from mode */
name|enum
name|object_type
name|obj_type
decl_stmt|;
comment|/* object type derived from sha */
name|char
modifier|*
name|path
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|ptr
operator|=
name|buf
expr_stmt|;
comment|/* 	 * Read non-recursive ls-tree output format: 	 *     mode SP type SP sha1 TAB name 	 */
name|mode
operator|=
name|strtoul
argument_list|(
name|ptr
argument_list|,
operator|&
name|ntr
argument_list|,
literal|8
argument_list|)
expr_stmt|;
if|if
condition|(
name|ptr
operator|==
name|ntr
operator|||
operator|!
name|ntr
operator|||
operator|*
name|ntr
operator|!=
literal|' '
condition|)
name|die
argument_list|(
literal|"input format error: %s"
argument_list|,
name|buf
argument_list|)
expr_stmt|;
name|ptr
operator|=
name|ntr
operator|+
literal|1
expr_stmt|;
comment|/* type */
name|ntr
operator|=
name|strchr
argument_list|(
name|ptr
argument_list|,
literal|' '
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ntr
operator|||
name|buf
operator|+
name|len
operator|<=
name|ntr
operator|+
literal|40
operator|||
name|ntr
index|[
literal|41
index|]
operator|!=
literal|'\t'
operator|||
name|get_sha1_hex
argument_list|(
name|ntr
operator|+
literal|1
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"input format error: %s"
argument_list|,
name|buf
argument_list|)
expr_stmt|;
comment|/* It is perfectly normal if we do not have a commit from a submodule */
if|if
condition|(
name|S_ISGITLINK
argument_list|(
name|mode
argument_list|)
condition|)
name|allow_missing
operator|=
literal|1
expr_stmt|;
operator|*
name|ntr
operator|++
operator|=
literal|0
expr_stmt|;
comment|/* now at the beginning of SHA1 */
name|path
operator|=
name|ntr
operator|+
literal|41
expr_stmt|;
comment|/* at the beginning of name */
if|if
condition|(
name|line_termination
operator|&&
name|path
index|[
literal|0
index|]
operator|==
literal|'"'
condition|)
block|{
name|struct
name|strbuf
name|p_uq
init|=
name|STRBUF_INIT
decl_stmt|;
if|if
condition|(
name|unquote_c_style
argument_list|(
operator|&
name|p_uq
argument_list|,
name|path
argument_list|,
name|NULL
argument_list|)
condition|)
name|die
argument_list|(
literal|"invalid quoting"
argument_list|)
expr_stmt|;
name|path
operator|=
name|strbuf_detach
argument_list|(
operator|&
name|p_uq
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * Object type is redundantly derivable three ways. 	 * These should all agree. 	 */
name|mode_type
operator|=
name|object_type
argument_list|(
name|mode
argument_list|)
expr_stmt|;
if|if
condition|(
name|mode_type
operator|!=
name|type_from_string
argument_list|(
name|ptr
argument_list|)
condition|)
block|{
name|die
argument_list|(
literal|"entry '%s' object type (%s) doesn't match mode type (%s)"
argument_list|,
name|path
argument_list|,
name|ptr
argument_list|,
name|typename
argument_list|(
name|mode_type
argument_list|)
argument_list|)
expr_stmt|;
block|}
comment|/* Check the type of object identified by sha1 */
name|obj_type
operator|=
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|obj_type
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|allow_missing
condition|)
block|{
empty_stmt|;
comment|/* no problem - missing objects are presumed to be of the right type */
block|}
else|else
block|{
name|die
argument_list|(
literal|"entry '%s' object %s is unavailable"
argument_list|,
name|path
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
if|if
condition|(
name|obj_type
operator|!=
name|mode_type
condition|)
block|{
comment|/* 			 * The object exists but is of the wrong type. 			 * This is a problem regardless of allow_missing 			 * because the new tree entry will never be correct. 			 */
name|die
argument_list|(
literal|"entry '%s' object %s is a %s but specified type was (%s)"
argument_list|,
name|path
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|typename
argument_list|(
name|obj_type
argument_list|)
argument_list|,
name|typename
argument_list|(
name|mode_type
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
name|append_to_tree
argument_list|(
name|mode
argument_list|,
name|sha1
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_mktree
name|int
name|cmd_mktree
parameter_list|(
name|int
name|ac
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|av
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|line_termination
init|=
literal|'\n'
decl_stmt|;
name|int
name|allow_missing
init|=
literal|0
decl_stmt|;
name|int
name|is_batch_mode
init|=
literal|0
decl_stmt|;
name|int
name|got_eof
init|=
literal|0
decl_stmt|;
specifier|const
name|struct
name|option
name|option
index|[]
init|=
block|{
name|OPT_SET_INT
argument_list|(
literal|'z'
argument_list|,
name|NULL
argument_list|,
operator|&
name|line_termination
argument_list|,
name|N_
argument_list|(
literal|"input is NUL terminated"
argument_list|)
argument_list|,
literal|'\0'
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"missing"
argument_list|,
operator|&
name|allow_missing
argument_list|,
name|N_
argument_list|(
literal|"allow missing objects"
argument_list|)
argument_list|,
literal|1
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"batch"
argument_list|,
operator|&
name|is_batch_mode
argument_list|,
name|N_
argument_list|(
literal|"allow creation of more than one tree"
argument_list|)
argument_list|,
literal|1
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|ac
operator|=
name|parse_options
argument_list|(
name|ac
argument_list|,
name|av
argument_list|,
name|prefix
argument_list|,
name|option
argument_list|,
name|mktree_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
while|while
condition|(
operator|!
name|got_eof
condition|)
block|{
while|while
condition|(
literal|1
condition|)
block|{
if|if
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|sb
argument_list|,
name|stdin
argument_list|,
name|line_termination
argument_list|)
operator|==
name|EOF
condition|)
block|{
name|got_eof
operator|=
literal|1
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|sb
operator|.
name|buf
index|[
literal|0
index|]
operator|==
literal|'\0'
condition|)
block|{
comment|/* empty lines denote tree boundaries in batch mode */
if|if
condition|(
name|is_batch_mode
condition|)
break|break;
name|die
argument_list|(
literal|"input format error: (blank line only valid in batch mode)"
argument_list|)
expr_stmt|;
block|}
name|mktree_line
argument_list|(
name|sb
operator|.
name|buf
argument_list|,
name|sb
operator|.
name|len
argument_list|,
name|line_termination
argument_list|,
name|allow_missing
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|is_batch_mode
operator|&&
name|got_eof
operator|&&
name|used
operator|<
literal|1
condition|)
block|{
comment|/* 			 * Execution gets here if the last tree entry is terminated with a 			 * new-line.  The final new-line has been made optional to be 			 * consistent with the original non-batch behaviour of mktree. 			 */
empty_stmt|;
comment|/* skip creating an empty tree */
block|}
else|else
block|{
name|write_tree
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|puts
argument_list|(
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
block|}
name|used
operator|=
literal|0
expr_stmt|;
comment|/* reset tree entry buffer for re-use in batch mode */
block|}
name|strbuf_release
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

