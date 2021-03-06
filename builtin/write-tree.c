begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"cache-tree.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_decl_stmt
DECL|variable|write_tree_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|write_tree_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git write-tree [--missing-ok] [--prefix=<prefix>/]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_write_tree
name|int
name|cmd_write_tree
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
name|unused_prefix
parameter_list|)
block|{
name|int
name|flags
init|=
literal|0
decl_stmt|,
name|ret
decl_stmt|;
specifier|const
name|char
modifier|*
name|prefix
init|=
name|NULL
decl_stmt|;
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
name|me
init|=
literal|"git-write-tree"
decl_stmt|;
name|struct
name|option
name|write_tree_options
index|[]
init|=
block|{
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"missing-ok"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"allow missing objects"
argument_list|)
argument_list|,
name|WRITE_TREE_MISSING_OK
argument_list|)
block|,
block|{
name|OPTION_STRING
block|,
literal|0
block|,
literal|"prefix"
block|,
operator|&
name|prefix
block|,
name|N_
argument_list|(
literal|"<prefix>/"
argument_list|)
block|,
name|N_
argument_list|(
literal|"write tree object for a subdirectory<prefix>"
argument_list|)
block|,
name|PARSE_OPT_LITERAL_ARGHELP
block|}
block|,
block|{
name|OPTION_BIT
block|,
literal|0
block|,
literal|"ignore-cache-tree"
block|,
operator|&
name|flags
block|,
name|NULL
block|,
name|N_
argument_list|(
literal|"only useful for debugging"
argument_list|)
block|,
name|PARSE_OPT_HIDDEN
operator||
name|PARSE_OPT_NOARG
block|,
name|NULL
block|,
name|WRITE_TREE_IGNORE_CACHE_TREE
block|}
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|unused_prefix
argument_list|,
name|write_tree_options
argument_list|,
name|write_tree_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ret
operator|=
name|write_cache_as_tree
argument_list|(
name|sha1
argument_list|,
name|flags
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|ret
condition|)
block|{
case|case
literal|0
case|:
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
break|break;
case|case
name|WRITE_TREE_UNREADABLE_INDEX
case|:
name|die
argument_list|(
literal|"%s: error reading the index"
argument_list|,
name|me
argument_list|)
expr_stmt|;
break|break;
case|case
name|WRITE_TREE_UNMERGED_INDEX
case|:
name|die
argument_list|(
literal|"%s: error building trees"
argument_list|,
name|me
argument_list|)
expr_stmt|;
break|break;
case|case
name|WRITE_TREE_PREFIX_ERROR
case|:
name|die
argument_list|(
literal|"%s: prefix %s not found"
argument_list|,
name|me
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
break|break;
block|}
return|return
name|ret
return|;
block|}
end_function

end_unit

