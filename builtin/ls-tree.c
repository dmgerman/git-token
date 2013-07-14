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
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"pathspec.h"
end_include

begin_decl_stmt
DECL|variable|line_termination
specifier|static
name|int
name|line_termination
init|=
literal|'\n'
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|LS_RECURSIVE
define|#
directive|define
name|LS_RECURSIVE
value|1
end_define

begin_define
DECL|macro|LS_TREE_ONLY
define|#
directive|define
name|LS_TREE_ONLY
value|2
end_define

begin_define
DECL|macro|LS_SHOW_TREES
define|#
directive|define
name|LS_SHOW_TREES
value|4
end_define

begin_define
DECL|macro|LS_NAME_ONLY
define|#
directive|define
name|LS_NAME_ONLY
value|8
end_define

begin_define
DECL|macro|LS_SHOW_SIZE
define|#
directive|define
name|LS_SHOW_SIZE
value|16
end_define

begin_decl_stmt
DECL|variable|abbrev
specifier|static
name|int
name|abbrev
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ls_options
specifier|static
name|int
name|ls_options
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pathspec
specifier|static
name|struct
name|pathspec
name|pathspec
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|chomp_prefix
specifier|static
name|int
name|chomp_prefix
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ls_tree_prefix
specifier|static
specifier|const
name|char
modifier|*
name|ls_tree_prefix
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ls_tree_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|ls_tree_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git ls-tree [<options>]<tree-ish> [<path>...]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|show_recursive
specifier|static
name|int
name|show_recursive
parameter_list|(
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
specifier|const
name|char
modifier|*
name|pathname
parameter_list|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|s
decl_stmt|;
if|if
condition|(
name|ls_options
operator|&
name|LS_RECURSIVE
condition|)
return|return
literal|1
return|;
name|s
operator|=
name|pathspec
operator|.
name|_raw
expr_stmt|;
if|if
condition|(
operator|!
name|s
condition|)
return|return
literal|0
return|;
for|for
control|(
init|;
condition|;
control|)
block|{
specifier|const
name|char
modifier|*
name|spec
init|=
operator|*
name|s
operator|++
decl_stmt|;
name|int
name|len
decl_stmt|,
name|speclen
decl_stmt|;
if|if
condition|(
operator|!
name|spec
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|strncmp
argument_list|(
name|base
argument_list|,
name|spec
argument_list|,
name|baselen
argument_list|)
condition|)
continue|continue;
name|len
operator|=
name|strlen
argument_list|(
name|pathname
argument_list|)
expr_stmt|;
name|spec
operator|+=
name|baselen
expr_stmt|;
name|speclen
operator|=
name|strlen
argument_list|(
name|spec
argument_list|)
expr_stmt|;
if|if
condition|(
name|speclen
operator|<=
name|len
condition|)
continue|continue;
if|if
condition|(
name|spec
index|[
name|len
index|]
operator|&&
name|spec
index|[
name|len
index|]
operator|!=
literal|'/'
condition|)
continue|continue;
if|if
condition|(
name|memcmp
argument_list|(
name|pathname
argument_list|,
name|spec
argument_list|,
name|len
argument_list|)
condition|)
continue|continue;
return|return
literal|1
return|;
block|}
block|}
end_function

begin_function
DECL|function|show_tree
specifier|static
name|int
name|show_tree
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
specifier|const
name|char
modifier|*
name|pathname
parameter_list|,
name|unsigned
name|mode
parameter_list|,
name|int
name|stage
parameter_list|,
name|void
modifier|*
name|context
parameter_list|)
block|{
name|int
name|retval
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|type
init|=
name|blob_type
decl_stmt|;
if|if
condition|(
name|S_ISGITLINK
argument_list|(
name|mode
argument_list|)
condition|)
block|{
comment|/* 		 * Maybe we want to have some recursive version here? 		 * 		 * Something similar to this incomplete example: 		 * 		if (show_subprojects(base, baselen, pathname)) 			retval = READ_TREE_RECURSIVE; 		 * 		 */
name|type
operator|=
name|commit_type
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
block|{
if|if
condition|(
name|show_recursive
argument_list|(
name|base
argument_list|,
name|baselen
argument_list|,
name|pathname
argument_list|)
condition|)
block|{
name|retval
operator|=
name|READ_TREE_RECURSIVE
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|ls_options
operator|&
name|LS_SHOW_TREES
operator|)
condition|)
return|return
name|retval
return|;
block|}
name|type
operator|=
name|tree_type
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|ls_options
operator|&
name|LS_TREE_ONLY
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|chomp_prefix
operator|&&
operator|(
name|baselen
operator|<
name|chomp_prefix
operator|||
name|memcmp
argument_list|(
name|ls_tree_prefix
argument_list|,
name|base
argument_list|,
name|chomp_prefix
argument_list|)
operator|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
operator|(
name|ls_options
operator|&
name|LS_NAME_ONLY
operator|)
condition|)
block|{
if|if
condition|(
name|ls_options
operator|&
name|LS_SHOW_SIZE
condition|)
block|{
name|char
name|size_text
index|[
literal|24
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
name|blob_type
argument_list|)
condition|)
block|{
name|unsigned
name|long
name|size
decl_stmt|;
if|if
condition|(
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
operator|&
name|size
argument_list|)
operator|==
name|OBJ_BAD
condition|)
name|strcpy
argument_list|(
name|size_text
argument_list|,
literal|"BAD"
argument_list|)
expr_stmt|;
else|else
name|snprintf
argument_list|(
name|size_text
argument_list|,
sizeof|sizeof
argument_list|(
name|size_text
argument_list|)
argument_list|,
literal|"%lu"
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
else|else
name|strcpy
argument_list|(
name|size_text
argument_list|,
literal|"-"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%06o %s %s %7s\t"
argument_list|,
name|mode
argument_list|,
name|type
argument_list|,
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|abbrev
argument_list|)
argument_list|,
name|size_text
argument_list|)
expr_stmt|;
block|}
else|else
name|printf
argument_list|(
literal|"%06o %s %s\t"
argument_list|,
name|mode
argument_list|,
name|type
argument_list|,
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|abbrev
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|write_name_quotedpfx
argument_list|(
name|base
operator|+
name|chomp_prefix
argument_list|,
name|baselen
operator|-
name|chomp_prefix
argument_list|,
name|pathname
argument_list|,
name|stdout
argument_list|,
name|line_termination
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|cmd_ls_tree
name|int
name|cmd_ls_tree
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
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|tree
modifier|*
name|tree
decl_stmt|;
name|int
name|i
decl_stmt|,
name|full_tree
init|=
literal|0
decl_stmt|;
specifier|const
name|struct
name|option
name|ls_tree_options
index|[]
init|=
block|{
name|OPT_BIT
argument_list|(
literal|'d'
argument_list|,
name|NULL
argument_list|,
operator|&
name|ls_options
argument_list|,
name|N_
argument_list|(
literal|"only show trees"
argument_list|)
argument_list|,
name|LS_TREE_ONLY
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|'r'
argument_list|,
name|NULL
argument_list|,
operator|&
name|ls_options
argument_list|,
name|N_
argument_list|(
literal|"recurse into subtrees"
argument_list|)
argument_list|,
name|LS_RECURSIVE
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|'t'
argument_list|,
name|NULL
argument_list|,
operator|&
name|ls_options
argument_list|,
name|N_
argument_list|(
literal|"show trees when recursing"
argument_list|)
argument_list|,
name|LS_SHOW_TREES
argument_list|)
block|,
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
literal|"terminate entries with NUL byte"
argument_list|)
argument_list|,
literal|0
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|'l'
argument_list|,
literal|"long"
argument_list|,
operator|&
name|ls_options
argument_list|,
name|N_
argument_list|(
literal|"include object size"
argument_list|)
argument_list|,
name|LS_SHOW_SIZE
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"name-only"
argument_list|,
operator|&
name|ls_options
argument_list|,
name|N_
argument_list|(
literal|"list only filenames"
argument_list|)
argument_list|,
name|LS_NAME_ONLY
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"name-status"
argument_list|,
operator|&
name|ls_options
argument_list|,
name|N_
argument_list|(
literal|"list only filenames"
argument_list|)
argument_list|,
name|LS_NAME_ONLY
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"full-name"
argument_list|,
operator|&
name|chomp_prefix
argument_list|,
name|N_
argument_list|(
literal|"use full path names"
argument_list|)
argument_list|,
literal|0
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"full-tree"
argument_list|,
operator|&
name|full_tree
argument_list|,
name|N_
argument_list|(
literal|"list entire tree; not just current directory "
literal|"(implies --full-name)"
argument_list|)
argument_list|)
block|,
name|OPT__ABBREV
argument_list|(
operator|&
name|abbrev
argument_list|)
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
name|ls_tree_prefix
operator|=
name|prefix
expr_stmt|;
if|if
condition|(
name|prefix
operator|&&
operator|*
name|prefix
condition|)
name|chomp_prefix
operator|=
name|strlen
argument_list|(
name|prefix
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
name|prefix
argument_list|,
name|ls_tree_options
argument_list|,
name|ls_tree_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|full_tree
condition|)
block|{
name|ls_tree_prefix
operator|=
name|prefix
operator|=
name|NULL
expr_stmt|;
name|chomp_prefix
operator|=
literal|0
expr_stmt|;
block|}
comment|/* -d -r should imply -t, but -d by itself should not have to. */
if|if
condition|(
operator|(
name|LS_TREE_ONLY
operator||
name|LS_RECURSIVE
operator|)
operator|==
operator|(
operator|(
name|LS_TREE_ONLY
operator||
name|LS_RECURSIVE
operator|)
operator|&
name|ls_options
operator|)
condition|)
name|ls_options
operator||=
name|LS_SHOW_TREES
expr_stmt|;
if|if
condition|(
name|argc
operator|<
literal|1
condition|)
name|usage_with_options
argument_list|(
name|ls_tree_usage
argument_list|,
name|ls_tree_options
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Not a valid object name %s"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
comment|/* 	 * show_recursive() rolls its own matching code and is 	 * generally ignorant of 'struct pathspec'. The magic mask 	 * cannot be lifted until it is converted to use 	 * match_pathspec_depth() or tree_entry_interesting() 	 */
name|parse_pathspec
argument_list|(
operator|&
name|pathspec
argument_list|,
literal|0
argument_list|,
name|PATHSPEC_PREFER_CWD
argument_list|,
name|prefix
argument_list|,
name|argv
operator|+
literal|1
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
name|pathspec
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|pathspec
operator|.
name|items
index|[
name|i
index|]
operator|.
name|nowildcard_len
operator|=
name|pathspec
operator|.
name|items
index|[
name|i
index|]
operator|.
name|len
expr_stmt|;
name|pathspec
operator|.
name|has_wildcard
operator|=
literal|0
expr_stmt|;
name|tree
operator|=
name|parse_tree_indirect
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree
condition|)
name|die
argument_list|(
literal|"not a tree object"
argument_list|)
expr_stmt|;
return|return
operator|!
operator|!
name|read_tree_recursive
argument_list|(
name|tree
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|,
operator|&
name|pathspec
argument_list|,
name|show_tree
argument_list|,
name|NULL
argument_list|)
return|;
block|}
end_function

end_unit

