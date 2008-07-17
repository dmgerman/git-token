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
specifier|const
name|char
modifier|*
modifier|*
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
name|ls_tree_usage
index|[]
init|=
literal|"git-ls-tree [-d] [-r] [-t] [-l] [-z] [--name-only] [--name-status] [--full-name] [--abbrev[=<n>]]<tree-ish> [path...]"
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
name|unsigned
name|long
name|size
decl_stmt|;
if|if
condition|(
name|S_ISGITLINK
argument_list|(
name|mode
argument_list|)
condition|)
block|{
comment|/* 		 * Maybe we want to have some recursive version here? 		 * 		 * Something like: 		 * 		if (show_subprojects(base, baselen, pathname)) { 			if (fork()) { 				chdir(base); 				exec ls-tree; 			} 			waitpid(); 		} 		 * 		 * ..or similar.. 		 */
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
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%06o %s %s %7lu\t"
argument_list|,
name|mode
argument_list|,
name|type
argument_list|,
name|abbrev
condition|?
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|abbrev
argument_list|)
else|:
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
else|else
name|printf
argument_list|(
literal|"%06o %s %s %7c\t"
argument_list|,
name|mode
argument_list|,
name|type
argument_list|,
name|abbrev
condition|?
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|abbrev
argument_list|)
else|:
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
literal|'-'
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
name|abbrev
condition|?
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|abbrev
argument_list|)
else|:
name|sha1_to_hex
argument_list|(
name|sha1
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
while|while
condition|(
literal|1
operator|<
name|argc
operator|&&
name|argv
index|[
literal|1
index|]
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
switch|switch
condition|(
name|argv
index|[
literal|1
index|]
index|[
literal|1
index|]
condition|)
block|{
case|case
literal|'z'
case|:
name|line_termination
operator|=
literal|0
expr_stmt|;
break|break;
case|case
literal|'r'
case|:
name|ls_options
operator||=
name|LS_RECURSIVE
expr_stmt|;
break|break;
case|case
literal|'d'
case|:
name|ls_options
operator||=
name|LS_TREE_ONLY
expr_stmt|;
break|break;
case|case
literal|'t'
case|:
name|ls_options
operator||=
name|LS_SHOW_TREES
expr_stmt|;
break|break;
case|case
literal|'l'
case|:
name|ls_options
operator||=
name|LS_SHOW_SIZE
expr_stmt|;
break|break;
case|case
literal|'-'
case|:
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
operator|+
literal|2
argument_list|,
literal|"name-only"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
operator|+
literal|2
argument_list|,
literal|"name-status"
argument_list|)
condition|)
block|{
name|ls_options
operator||=
name|LS_NAME_ONLY
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
operator|+
literal|2
argument_list|,
literal|"long"
argument_list|)
condition|)
block|{
name|ls_options
operator||=
name|LS_SHOW_SIZE
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
operator|+
literal|2
argument_list|,
literal|"full-name"
argument_list|)
condition|)
block|{
name|chomp_prefix
operator|=
literal|0
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|argv
index|[
literal|1
index|]
operator|+
literal|2
argument_list|,
literal|"abbrev="
argument_list|)
condition|)
block|{
name|abbrev
operator|=
name|strtoul
argument_list|(
name|argv
index|[
literal|1
index|]
operator|+
literal|9
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|abbrev
operator|&&
name|abbrev
operator|<
name|MINIMUM_ABBREV
condition|)
name|abbrev
operator|=
name|MINIMUM_ABBREV
expr_stmt|;
elseif|else
if|if
condition|(
name|abbrev
operator|>
literal|40
condition|)
name|abbrev
operator|=
literal|40
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
operator|+
literal|2
argument_list|,
literal|"abbrev"
argument_list|)
condition|)
block|{
name|abbrev
operator|=
name|DEFAULT_ABBREV
expr_stmt|;
break|break;
block|}
comment|/* otherwise fallthru */
default|default:
name|usage
argument_list|(
name|ls_tree_usage
argument_list|)
expr_stmt|;
block|}
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
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
literal|2
condition|)
name|usage
argument_list|(
name|ls_tree_usage
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
name|sha1
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
name|pathspec
operator|=
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
operator|+
literal|2
argument_list|)
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
name|pathspec
argument_list|,
name|show_tree
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

