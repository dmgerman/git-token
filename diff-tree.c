begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<ctype.h>
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_decl_stmt
DECL|variable|show_root_diff
specifier|static
name|int
name|show_root_diff
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|verbose_header
specifier|static
name|int
name|verbose_header
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ignore_merges
specifier|static
name|int
name|ignore_merges
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|recursive
specifier|static
name|int
name|recursive
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_tree_entry_in_recursive
specifier|static
name|int
name|show_tree_entry_in_recursive
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|read_stdin
specifier|static
name|int
name|read_stdin
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_output_format
specifier|static
name|int
name|diff_output_format
init|=
name|DIFF_FORMAT_HUMAN
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|detect_rename
specifier|static
name|int
name|detect_rename
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_setup_opt
specifier|static
name|int
name|diff_setup_opt
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_score_opt
specifier|static
name|int
name|diff_score_opt
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pickaxe
specifier|static
specifier|const
name|char
modifier|*
name|pickaxe
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pickaxe_opts
specifier|static
name|int
name|pickaxe_opts
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_break_opt
specifier|static
name|int
name|diff_break_opt
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|orderfile
specifier|static
specifier|const
name|char
modifier|*
name|orderfile
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|header
specifier|static
specifier|const
name|char
modifier|*
name|header
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|header_prefix
specifier|static
specifier|const
name|char
modifier|*
name|header_prefix
init|=
literal|""
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|commit_format
specifier|static
name|enum
name|cmit_fmt
name|commit_format
init|=
name|CMIT_FMT_RAW
decl_stmt|;
end_decl_stmt

begin_comment
comment|// What paths are we interested in?
end_comment

begin_decl_stmt
DECL|variable|nr_paths
specifier|static
name|int
name|nr_paths
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|paths
specifier|static
specifier|const
name|char
modifier|*
modifier|*
name|paths
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pathlens
specifier|static
name|int
modifier|*
name|pathlens
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|static
name|int
name|diff_tree_sha1
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|old
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|update_tree_entry
specifier|static
name|void
name|update_tree_entry
parameter_list|(
name|void
modifier|*
modifier|*
name|bufp
parameter_list|,
name|unsigned
name|long
modifier|*
name|sizep
parameter_list|)
block|{
name|void
modifier|*
name|buf
init|=
operator|*
name|bufp
decl_stmt|;
name|unsigned
name|long
name|size
init|=
operator|*
name|sizep
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|buf
argument_list|)
operator|+
literal|1
operator|+
literal|20
decl_stmt|;
if|if
condition|(
name|size
operator|<
name|len
condition|)
name|die
argument_list|(
literal|"corrupt tree file"
argument_list|)
expr_stmt|;
operator|*
name|bufp
operator|=
name|buf
operator|+
name|len
expr_stmt|;
operator|*
name|sizep
operator|=
name|size
operator|-
name|len
expr_stmt|;
block|}
end_function

begin_function
DECL|function|extract
specifier|static
specifier|const
name|unsigned
name|char
modifier|*
name|extract
parameter_list|(
name|void
modifier|*
name|tree
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathp
parameter_list|,
name|unsigned
name|int
modifier|*
name|modep
parameter_list|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|tree
argument_list|)
operator|+
literal|1
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|tree
operator|+
name|len
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
init|=
name|strchr
argument_list|(
name|tree
argument_list|,
literal|' '
argument_list|)
decl_stmt|;
name|unsigned
name|int
name|mode
decl_stmt|;
if|if
condition|(
operator|!
name|path
operator|||
name|size
operator|<
name|len
operator|+
literal|20
operator|||
name|sscanf
argument_list|(
name|tree
argument_list|,
literal|"%o"
argument_list|,
operator|&
name|mode
argument_list|)
operator|!=
literal|1
condition|)
name|die
argument_list|(
literal|"corrupt tree file"
argument_list|)
expr_stmt|;
operator|*
name|pathp
operator|=
name|path
operator|+
literal|1
expr_stmt|;
operator|*
name|modep
operator|=
name|DIFF_FILE_CANON_MODE
argument_list|(
name|mode
argument_list|)
expr_stmt|;
return|return
name|sha1
return|;
block|}
end_function

begin_function
DECL|function|malloc_base
specifier|static
name|char
modifier|*
name|malloc_base
parameter_list|(
specifier|const
name|char
modifier|*
name|base
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|pathlen
parameter_list|)
block|{
name|int
name|baselen
init|=
name|strlen
argument_list|(
name|base
argument_list|)
decl_stmt|;
name|char
modifier|*
name|newbase
init|=
name|xmalloc
argument_list|(
name|baselen
operator|+
name|pathlen
operator|+
literal|2
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|newbase
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|newbase
operator|+
name|baselen
argument_list|,
name|path
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|newbase
operator|+
name|baselen
operator|+
name|pathlen
argument_list|,
literal|"/"
argument_list|,
literal|2
argument_list|)
expr_stmt|;
return|return
name|newbase
return|;
block|}
end_function

begin_function_decl
specifier|static
name|void
name|show_file
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|void
modifier|*
name|tree
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|show_tree
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|void
modifier|*
name|tree
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* A file entry went away or appeared */
end_comment

begin_function
DECL|function|show_file
specifier|static
name|void
name|show_file
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|void
modifier|*
name|tree
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
name|unsigned
name|mode
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|extract
argument_list|(
name|tree
argument_list|,
name|size
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|mode
argument_list|)
decl_stmt|;
if|if
condition|(
name|recursive
operator|&&
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
block|{
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|char
modifier|*
name|newbase
init|=
name|malloc_base
argument_list|(
name|base
argument_list|,
name|path
argument_list|,
name|strlen
argument_list|(
name|path
argument_list|)
argument_list|)
decl_stmt|;
name|void
modifier|*
name|tree
decl_stmt|;
name|tree
operator|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree
operator|||
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tree"
argument_list|)
condition|)
name|die
argument_list|(
literal|"corrupt tree sha %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|show_tree
argument_list|(
name|prefix
argument_list|,
name|tree
argument_list|,
name|size
argument_list|,
name|newbase
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|newbase
argument_list|)
expr_stmt|;
return|return;
block|}
name|diff_addremove
argument_list|(
name|prefix
index|[
literal|0
index|]
argument_list|,
name|mode
argument_list|,
name|sha1
argument_list|,
name|base
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|compare_tree_entry
specifier|static
name|int
name|compare_tree_entry
parameter_list|(
name|void
modifier|*
name|tree1
parameter_list|,
name|unsigned
name|long
name|size1
parameter_list|,
name|void
modifier|*
name|tree2
parameter_list|,
name|unsigned
name|long
name|size2
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
name|unsigned
name|mode1
decl_stmt|,
name|mode2
decl_stmt|;
specifier|const
name|char
modifier|*
name|path1
decl_stmt|,
modifier|*
name|path2
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|,
modifier|*
name|sha2
decl_stmt|;
name|int
name|cmp
decl_stmt|,
name|pathlen1
decl_stmt|,
name|pathlen2
decl_stmt|;
name|sha1
operator|=
name|extract
argument_list|(
name|tree1
argument_list|,
name|size1
argument_list|,
operator|&
name|path1
argument_list|,
operator|&
name|mode1
argument_list|)
expr_stmt|;
name|sha2
operator|=
name|extract
argument_list|(
name|tree2
argument_list|,
name|size2
argument_list|,
operator|&
name|path2
argument_list|,
operator|&
name|mode2
argument_list|)
expr_stmt|;
name|pathlen1
operator|=
name|strlen
argument_list|(
name|path1
argument_list|)
expr_stmt|;
name|pathlen2
operator|=
name|strlen
argument_list|(
name|path2
argument_list|)
expr_stmt|;
name|cmp
operator|=
name|base_name_compare
argument_list|(
name|path1
argument_list|,
name|pathlen1
argument_list|,
name|mode1
argument_list|,
name|path2
argument_list|,
name|pathlen2
argument_list|,
name|mode2
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
block|{
name|show_file
argument_list|(
literal|"-"
argument_list|,
name|tree1
argument_list|,
name|size1
argument_list|,
name|base
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|cmp
operator|>
literal|0
condition|)
block|{
name|show_file
argument_list|(
literal|"+"
argument_list|,
name|tree2
argument_list|,
name|size2
argument_list|,
name|base
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|sha1
argument_list|,
name|sha2
argument_list|,
literal|20
argument_list|)
operator|&&
name|mode1
operator|==
name|mode2
condition|)
return|return
literal|0
return|;
comment|/* 	 * If the filemode has changed to/from a directory from/to a regular 	 * file, we need to consider it a remove and an add. 	 */
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode1
argument_list|)
operator|!=
name|S_ISDIR
argument_list|(
name|mode2
argument_list|)
condition|)
block|{
name|show_file
argument_list|(
literal|"-"
argument_list|,
name|tree1
argument_list|,
name|size1
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|show_file
argument_list|(
literal|"+"
argument_list|,
name|tree2
argument_list|,
name|size2
argument_list|,
name|base
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|recursive
operator|&&
name|S_ISDIR
argument_list|(
name|mode1
argument_list|)
condition|)
block|{
name|int
name|retval
decl_stmt|;
name|char
modifier|*
name|newbase
init|=
name|malloc_base
argument_list|(
name|base
argument_list|,
name|path1
argument_list|,
name|pathlen1
argument_list|)
decl_stmt|;
if|if
condition|(
name|show_tree_entry_in_recursive
condition|)
name|diff_change
argument_list|(
name|mode1
argument_list|,
name|mode2
argument_list|,
name|sha1
argument_list|,
name|sha2
argument_list|,
name|base
argument_list|,
name|path1
argument_list|)
expr_stmt|;
name|retval
operator|=
name|diff_tree_sha1
argument_list|(
name|sha1
argument_list|,
name|sha2
argument_list|,
name|newbase
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|newbase
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
name|diff_change
argument_list|(
name|mode1
argument_list|,
name|mode2
argument_list|,
name|sha1
argument_list|,
name|sha2
argument_list|,
name|base
argument_list|,
name|path1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|interesting
specifier|static
name|int
name|interesting
parameter_list|(
name|void
modifier|*
name|tree
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
name|unsigned
name|mode
decl_stmt|;
name|int
name|i
decl_stmt|;
name|int
name|baselen
decl_stmt|,
name|pathlen
decl_stmt|;
if|if
condition|(
operator|!
name|nr_paths
condition|)
return|return
literal|1
return|;
operator|(
name|void
operator|)
name|extract
argument_list|(
name|tree
argument_list|,
name|size
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|mode
argument_list|)
expr_stmt|;
name|pathlen
operator|=
name|strlen
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|baselen
operator|=
name|strlen
argument_list|(
name|base
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
name|nr_paths
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|match
init|=
name|paths
index|[
name|i
index|]
decl_stmt|;
name|int
name|matchlen
init|=
name|pathlens
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|baselen
operator|>=
name|matchlen
condition|)
block|{
comment|/* If it doesn't match, move along... */
if|if
condition|(
name|strncmp
argument_list|(
name|base
argument_list|,
name|match
argument_list|,
name|matchlen
argument_list|)
condition|)
continue|continue;
comment|/* The base is a subdirectory of a path which was specified. */
return|return
literal|1
return|;
block|}
comment|/* Does the base match? */
if|if
condition|(
name|strncmp
argument_list|(
name|base
argument_list|,
name|match
argument_list|,
name|baselen
argument_list|)
condition|)
continue|continue;
name|match
operator|+=
name|baselen
expr_stmt|;
name|matchlen
operator|-=
name|baselen
expr_stmt|;
if|if
condition|(
name|pathlen
operator|>
name|matchlen
condition|)
continue|continue;
if|if
condition|(
name|matchlen
operator|>
name|pathlen
condition|)
block|{
if|if
condition|(
name|match
index|[
name|pathlen
index|]
operator|!=
literal|'/'
condition|)
continue|continue;
if|if
condition|(
operator|!
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
continue|continue;
block|}
if|if
condition|(
name|strncmp
argument_list|(
name|path
argument_list|,
name|match
argument_list|,
name|pathlen
argument_list|)
condition|)
continue|continue;
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
comment|/* No matches */
block|}
end_function

begin_comment
comment|/* A whole sub-tree went away or appeared */
end_comment

begin_function
DECL|function|show_tree
specifier|static
name|void
name|show_tree
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|void
modifier|*
name|tree
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
while|while
condition|(
name|size
condition|)
block|{
if|if
condition|(
name|interesting
argument_list|(
name|tree
argument_list|,
name|size
argument_list|,
name|base
argument_list|)
condition|)
name|show_file
argument_list|(
name|prefix
argument_list|,
name|tree
argument_list|,
name|size
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
operator|&
name|tree
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|diff_tree
specifier|static
name|int
name|diff_tree
parameter_list|(
name|void
modifier|*
name|tree1
parameter_list|,
name|unsigned
name|long
name|size1
parameter_list|,
name|void
modifier|*
name|tree2
parameter_list|,
name|unsigned
name|long
name|size2
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
while|while
condition|(
name|size1
operator||
name|size2
condition|)
block|{
if|if
condition|(
name|nr_paths
operator|&&
name|size1
operator|&&
operator|!
name|interesting
argument_list|(
name|tree1
argument_list|,
name|size1
argument_list|,
name|base
argument_list|)
condition|)
block|{
name|update_tree_entry
argument_list|(
operator|&
name|tree1
argument_list|,
operator|&
name|size1
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|nr_paths
operator|&&
name|size2
operator|&&
operator|!
name|interesting
argument_list|(
name|tree2
argument_list|,
name|size2
argument_list|,
name|base
argument_list|)
condition|)
block|{
name|update_tree_entry
argument_list|(
operator|&
name|tree2
argument_list|,
operator|&
name|size2
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|size1
condition|)
block|{
name|show_file
argument_list|(
literal|"+"
argument_list|,
name|tree2
argument_list|,
name|size2
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
operator|&
name|tree2
argument_list|,
operator|&
name|size2
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|size2
condition|)
block|{
name|show_file
argument_list|(
literal|"-"
argument_list|,
name|tree1
argument_list|,
name|size1
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
operator|&
name|tree1
argument_list|,
operator|&
name|size1
argument_list|)
expr_stmt|;
continue|continue;
block|}
switch|switch
condition|(
name|compare_tree_entry
argument_list|(
name|tree1
argument_list|,
name|size1
argument_list|,
name|tree2
argument_list|,
name|size2
argument_list|,
name|base
argument_list|)
condition|)
block|{
case|case
operator|-
literal|1
case|:
name|update_tree_entry
argument_list|(
operator|&
name|tree1
argument_list|,
operator|&
name|size1
argument_list|)
expr_stmt|;
continue|continue;
case|case
literal|0
case|:
name|update_tree_entry
argument_list|(
operator|&
name|tree1
argument_list|,
operator|&
name|size1
argument_list|)
expr_stmt|;
comment|/* Fallthrough */
case|case
literal|1
case|:
name|update_tree_entry
argument_list|(
operator|&
name|tree2
argument_list|,
operator|&
name|size2
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|die
argument_list|(
literal|"git-diff-tree: internal error"
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|diff_tree_sha1
specifier|static
name|int
name|diff_tree_sha1
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|old
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
name|void
modifier|*
name|tree1
decl_stmt|,
modifier|*
name|tree2
decl_stmt|;
name|unsigned
name|long
name|size1
decl_stmt|,
name|size2
decl_stmt|;
name|int
name|retval
decl_stmt|;
name|tree1
operator|=
name|read_object_with_reference
argument_list|(
name|old
argument_list|,
literal|"tree"
argument_list|,
operator|&
name|size1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree1
condition|)
name|die
argument_list|(
literal|"unable to read source tree (%s)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|old
argument_list|)
argument_list|)
expr_stmt|;
name|tree2
operator|=
name|read_object_with_reference
argument_list|(
name|new
argument_list|,
literal|"tree"
argument_list|,
operator|&
name|size2
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree2
condition|)
name|die
argument_list|(
literal|"unable to read destination tree (%s)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|new
argument_list|)
argument_list|)
expr_stmt|;
name|retval
operator|=
name|diff_tree
argument_list|(
name|tree1
argument_list|,
name|size1
argument_list|,
name|tree2
argument_list|,
name|size2
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree2
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|call_diff_setup
specifier|static
name|void
name|call_diff_setup
parameter_list|(
name|void
parameter_list|)
block|{
name|diff_setup
argument_list|(
name|diff_setup_opt
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|call_diff_flush
specifier|static
name|int
name|call_diff_flush
parameter_list|(
name|void
parameter_list|)
block|{
name|diffcore_std
argument_list|(
literal|0
argument_list|,
name|detect_rename
argument_list|,
name|diff_score_opt
argument_list|,
name|pickaxe
argument_list|,
name|pickaxe_opts
argument_list|,
name|diff_break_opt
argument_list|,
name|orderfile
argument_list|)
expr_stmt|;
if|if
condition|(
name|diff_queue_is_empty
argument_list|()
condition|)
block|{
name|diff_flush
argument_list|(
name|DIFF_FORMAT_NO_OUTPUT
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|header
condition|)
block|{
specifier|const
name|char
modifier|*
name|fmt
init|=
literal|"%s"
decl_stmt|;
if|if
condition|(
name|diff_output_format
operator|==
name|DIFF_FORMAT_MACHINE
condition|)
name|fmt
operator|=
literal|"%s%c"
expr_stmt|;
name|printf
argument_list|(
name|fmt
argument_list|,
name|header
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|header
operator|=
name|NULL
expr_stmt|;
block|}
name|diff_flush
argument_list|(
name|diff_output_format
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|diff_tree_sha1_top
specifier|static
name|int
name|diff_tree_sha1_top
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|old
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
name|int
name|ret
decl_stmt|;
name|call_diff_setup
argument_list|()
expr_stmt|;
name|ret
operator|=
name|diff_tree_sha1
argument_list|(
name|old
argument_list|,
name|new
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|call_diff_flush
argument_list|()
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|diff_root_tree
specifier|static
name|int
name|diff_root_tree
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|new
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
name|int
name|retval
decl_stmt|;
name|void
modifier|*
name|tree
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|call_diff_setup
argument_list|()
expr_stmt|;
name|tree
operator|=
name|read_object_with_reference
argument_list|(
name|new
argument_list|,
literal|"tree"
argument_list|,
operator|&
name|size
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree
condition|)
name|die
argument_list|(
literal|"unable to read root tree (%s)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|new
argument_list|)
argument_list|)
expr_stmt|;
name|retval
operator|=
name|diff_tree
argument_list|(
literal|""
argument_list|,
literal|0
argument_list|,
name|tree
argument_list|,
name|size
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree
argument_list|)
expr_stmt|;
name|call_diff_flush
argument_list|()
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|generate_header
specifier|static
name|char
modifier|*
name|generate_header
parameter_list|(
specifier|const
name|char
modifier|*
name|commit
parameter_list|,
specifier|const
name|char
modifier|*
name|parent
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|,
name|unsigned
name|long
name|len
parameter_list|)
block|{
specifier|static
name|char
name|this_header
index|[
literal|16384
index|]
decl_stmt|;
name|int
name|offset
decl_stmt|;
name|offset
operator|=
name|sprintf
argument_list|(
name|this_header
argument_list|,
literal|"%s%s (from %s)\n"
argument_list|,
name|header_prefix
argument_list|,
name|commit
argument_list|,
name|parent
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose_header
condition|)
block|{
name|offset
operator|+=
name|pretty_print_commit
argument_list|(
name|commit_format
argument_list|,
name|msg
argument_list|,
name|len
argument_list|,
name|this_header
operator|+
name|offset
argument_list|,
sizeof|sizeof
argument_list|(
name|this_header
argument_list|)
operator|-
name|offset
argument_list|)
expr_stmt|;
name|this_header
index|[
name|offset
operator|++
index|]
operator|=
literal|'\n'
expr_stmt|;
name|this_header
index|[
name|offset
operator|++
index|]
operator|=
literal|0
expr_stmt|;
block|}
return|return
name|this_header
return|;
block|}
end_function

begin_function
DECL|function|diff_tree_commit
specifier|static
name|int
name|diff_tree_commit
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|commit
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|unsigned
name|long
name|size
decl_stmt|,
name|offset
decl_stmt|;
name|char
modifier|*
name|buf
init|=
name|read_object_with_reference
argument_list|(
name|commit
argument_list|,
literal|"commit"
argument_list|,
operator|&
name|size
argument_list|,
name|NULL
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|name
condition|)
block|{
specifier|static
name|char
name|commit_name
index|[
literal|60
index|]
decl_stmt|;
name|strcpy
argument_list|(
name|commit_name
argument_list|,
name|sha1_to_hex
argument_list|(
name|commit
argument_list|)
argument_list|)
expr_stmt|;
name|name
operator|=
name|commit_name
expr_stmt|;
block|}
comment|/* Root commit? */
if|if
condition|(
name|show_root_diff
operator|&&
name|memcmp
argument_list|(
name|buf
operator|+
literal|46
argument_list|,
literal|"parent "
argument_list|,
literal|7
argument_list|)
condition|)
block|{
name|header
operator|=
name|generate_header
argument_list|(
name|name
argument_list|,
literal|"root"
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|diff_root_tree
argument_list|(
name|commit
argument_list|,
literal|""
argument_list|)
expr_stmt|;
block|}
comment|/* More than one parent? */
if|if
condition|(
name|ignore_merges
condition|)
block|{
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|buf
operator|+
literal|46
operator|+
literal|48
argument_list|,
literal|"parent "
argument_list|,
literal|7
argument_list|)
condition|)
return|return
literal|0
return|;
block|}
name|offset
operator|=
literal|46
expr_stmt|;
while|while
condition|(
name|offset
operator|+
literal|48
operator|<
name|size
operator|&&
operator|!
name|memcmp
argument_list|(
name|buf
operator|+
name|offset
argument_list|,
literal|"parent "
argument_list|,
literal|7
argument_list|)
condition|)
block|{
name|unsigned
name|char
name|parent
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|buf
operator|+
name|offset
operator|+
literal|7
argument_list|,
name|parent
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|header
operator|=
name|generate_header
argument_list|(
name|name
argument_list|,
name|sha1_to_hex
argument_list|(
name|parent
argument_list|)
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|diff_tree_sha1_top
argument_list|(
name|parent
argument_list|,
name|commit
argument_list|,
literal|""
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|header
operator|&&
name|verbose_header
condition|)
block|{
name|header_prefix
operator|=
literal|"\ndiff-tree "
expr_stmt|;
comment|/* 			 * Don't print multiple merge entries if we 			 * don't print the diffs. 			 */
block|}
name|offset
operator|+=
literal|48
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|diff_tree_stdin
specifier|static
name|int
name|diff_tree_stdin
parameter_list|(
name|char
modifier|*
name|line
parameter_list|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|line
argument_list|)
decl_stmt|;
name|unsigned
name|char
name|commit
index|[
literal|20
index|]
decl_stmt|,
name|parent
index|[
literal|20
index|]
decl_stmt|;
specifier|static
name|char
name|this_header
index|[
literal|1000
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|len
operator|||
name|line
index|[
name|len
operator|-
literal|1
index|]
operator|!=
literal|'\n'
condition|)
return|return
operator|-
literal|1
return|;
name|line
index|[
name|len
operator|-
literal|1
index|]
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|line
argument_list|,
name|commit
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|isspace
argument_list|(
name|line
index|[
literal|40
index|]
argument_list|)
operator|&&
operator|!
name|get_sha1_hex
argument_list|(
name|line
operator|+
literal|41
argument_list|,
name|parent
argument_list|)
condition|)
block|{
name|line
index|[
literal|40
index|]
operator|=
literal|0
expr_stmt|;
name|line
index|[
literal|81
index|]
operator|=
literal|0
expr_stmt|;
name|sprintf
argument_list|(
name|this_header
argument_list|,
literal|"%s (from %s)\n"
argument_list|,
name|line
argument_list|,
name|line
operator|+
literal|41
argument_list|)
expr_stmt|;
name|header
operator|=
name|this_header
expr_stmt|;
return|return
name|diff_tree_sha1_top
argument_list|(
name|parent
argument_list|,
name|commit
argument_list|,
literal|""
argument_list|)
return|;
block|}
name|line
index|[
literal|40
index|]
operator|=
literal|0
expr_stmt|;
return|return
name|diff_tree_commit
argument_list|(
name|commit
argument_list|,
name|line
argument_list|)
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|diff_tree_usage
specifier|static
name|char
modifier|*
name|diff_tree_usage
init|=
literal|"git-diff-tree [-p] [-r] [-z] [--stdin] [-M] [-C] [-R] [-S<string>] [-O<orderfile>] [-m] [-s] [-v] [-t]<tree-ish><tree-ish>"
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
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|int
name|nr_sha1
decl_stmt|;
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|2
index|]
index|[
literal|20
index|]
decl_stmt|;
name|nr_sha1
operator|=
literal|0
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
decl_stmt|;
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|arg
operator|=
operator|*
name|argv
expr_stmt|;
if|if
condition|(
operator|!
name|arg
condition|)
break|break;
if|if
condition|(
operator|*
name|arg
operator|!=
literal|'-'
condition|)
block|{
if|if
condition|(
name|nr_sha1
operator|<
literal|2
operator|&&
operator|!
name|get_sha1
argument_list|(
name|arg
argument_list|,
name|sha1
index|[
name|nr_sha1
index|]
argument_list|)
condition|)
block|{
name|nr_sha1
operator|++
expr_stmt|;
continue|continue;
block|}
break|break;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--"
argument_list|)
condition|)
block|{
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-r"
argument_list|)
condition|)
block|{
name|recursive
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-t"
argument_list|)
condition|)
block|{
name|recursive
operator|=
name|show_tree_entry_in_recursive
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-R"
argument_list|)
condition|)
block|{
name|diff_setup_opt
operator||=
name|DIFF_SETUP_REVERSE
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-p"
argument_list|)
condition|)
block|{
name|diff_output_format
operator|=
name|DIFF_FORMAT_PATCH
expr_stmt|;
name|recursive
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"-S"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
name|pickaxe
operator|=
name|arg
operator|+
literal|2
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"-O"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
name|orderfile
operator|=
name|arg
operator|+
literal|2
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--pickaxe-all"
argument_list|)
condition|)
block|{
name|pickaxe_opts
operator|=
name|DIFF_PICKAXE_ALL
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"-M"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
name|detect_rename
operator|=
name|DIFF_DETECT_RENAME
expr_stmt|;
if|if
condition|(
operator|(
name|diff_score_opt
operator|=
name|diff_scoreopt_parse
argument_list|(
name|arg
argument_list|)
operator|)
operator|==
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|diff_tree_usage
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"-C"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
name|detect_rename
operator|=
name|DIFF_DETECT_COPY
expr_stmt|;
if|if
condition|(
operator|(
name|diff_score_opt
operator|=
name|diff_scoreopt_parse
argument_list|(
name|arg
argument_list|)
operator|)
operator|==
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|diff_tree_usage
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"-B"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
if|if
condition|(
operator|(
name|diff_break_opt
operator|=
name|diff_scoreopt_parse
argument_list|(
name|arg
argument_list|)
operator|)
operator|==
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|diff_tree_usage
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-z"
argument_list|)
condition|)
block|{
name|diff_output_format
operator|=
name|DIFF_FORMAT_MACHINE
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-m"
argument_list|)
condition|)
block|{
name|ignore_merges
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-s"
argument_list|)
condition|)
block|{
name|diff_output_format
operator|=
name|DIFF_FORMAT_NO_OUTPUT
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-v"
argument_list|)
condition|)
block|{
name|verbose_header
operator|=
literal|1
expr_stmt|;
name|header_prefix
operator|=
literal|"diff-tree "
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--stdin"
argument_list|)
condition|)
block|{
name|read_stdin
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--root"
argument_list|)
condition|)
block|{
name|show_root_diff
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|diff_tree_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|>
literal|0
condition|)
block|{
name|int
name|i
decl_stmt|;
name|paths
operator|=
name|argv
expr_stmt|;
name|nr_paths
operator|=
name|argc
expr_stmt|;
name|pathlens
operator|=
name|xmalloc
argument_list|(
name|nr_paths
operator|*
sizeof|sizeof
argument_list|(
name|int
argument_list|)
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
name|nr_paths
condition|;
name|i
operator|++
control|)
name|pathlens
index|[
name|i
index|]
operator|=
name|strlen
argument_list|(
name|paths
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
switch|switch
condition|(
name|nr_sha1
condition|)
block|{
case|case
literal|0
case|:
if|if
condition|(
operator|!
name|read_stdin
condition|)
name|usage
argument_list|(
name|diff_tree_usage
argument_list|)
expr_stmt|;
break|break;
case|case
literal|1
case|:
name|diff_tree_commit
argument_list|(
name|sha1
index|[
literal|0
index|]
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
break|break;
case|case
literal|2
case|:
name|diff_tree_sha1_top
argument_list|(
name|sha1
index|[
literal|0
index|]
argument_list|,
name|sha1
index|[
literal|1
index|]
argument_list|,
literal|""
argument_list|)
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|read_stdin
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|detect_rename
condition|)
name|diff_setup_opt
operator||=
operator|(
name|DIFF_SETUP_USE_SIZE_CACHE
operator||
name|DIFF_SETUP_USE_CACHE
operator|)
expr_stmt|;
while|while
condition|(
name|fgets
argument_list|(
name|line
argument_list|,
sizeof|sizeof
argument_list|(
name|line
argument_list|)
argument_list|,
name|stdin
argument_list|)
condition|)
name|diff_tree_stdin
argument_list|(
name|line
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

