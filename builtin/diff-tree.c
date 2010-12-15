begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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

begin_include
include|#
directive|include
file|"log-tree.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"submodule.h"
end_include

begin_decl_stmt
DECL|variable|log_tree_opt
specifier|static
name|struct
name|rev_info
name|log_tree_opt
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|diff_tree_commit_sha1
specifier|static
name|int
name|diff_tree_commit_sha1
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
init|=
name|lookup_commit_reference
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|commit
condition|)
return|return
operator|-
literal|1
return|;
return|return
name|log_tree_commit
argument_list|(
operator|&
name|log_tree_opt
argument_list|,
name|commit
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* Diff one or more commits. */
end_comment

begin_function
DECL|function|stdin_diff_commit
specifier|static
name|int
name|stdin_diff_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|,
name|char
modifier|*
name|line
parameter_list|,
name|int
name|len
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
name|sha1
argument_list|)
condition|)
block|{
comment|/* Graft the fake parents locally to the commit */
name|int
name|pos
init|=
literal|41
decl_stmt|;
name|struct
name|commit_list
modifier|*
modifier|*
name|pptr
decl_stmt|,
modifier|*
name|parents
decl_stmt|;
comment|/* Free the real parent list */
for|for
control|(
name|parents
operator|=
name|commit
operator|->
name|parents
init|;
name|parents
condition|;
control|)
block|{
name|struct
name|commit_list
modifier|*
name|tmp
init|=
name|parents
operator|->
name|next
decl_stmt|;
name|free
argument_list|(
name|parents
argument_list|)
expr_stmt|;
name|parents
operator|=
name|tmp
expr_stmt|;
block|}
name|commit
operator|->
name|parents
operator|=
name|NULL
expr_stmt|;
name|pptr
operator|=
operator|&
operator|(
name|commit
operator|->
name|parents
operator|)
expr_stmt|;
while|while
condition|(
name|line
index|[
name|pos
index|]
operator|&&
operator|!
name|get_sha1_hex
argument_list|(
name|line
operator|+
name|pos
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|struct
name|commit
modifier|*
name|parent
init|=
name|lookup_commit
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
name|parent
condition|)
block|{
name|pptr
operator|=
operator|&
name|commit_list_insert
argument_list|(
name|parent
argument_list|,
name|pptr
argument_list|)
operator|->
name|next
expr_stmt|;
block|}
name|pos
operator|+=
literal|41
expr_stmt|;
block|}
block|}
return|return
name|log_tree_commit
argument_list|(
operator|&
name|log_tree_opt
argument_list|,
name|commit
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* Diff two trees. */
end_comment

begin_function
DECL|function|stdin_diff_trees
specifier|static
name|int
name|stdin_diff_trees
parameter_list|(
name|struct
name|tree
modifier|*
name|tree1
parameter_list|,
name|char
modifier|*
name|line
parameter_list|,
name|int
name|len
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
name|tree2
decl_stmt|;
if|if
condition|(
name|len
operator|!=
literal|82
operator|||
operator|!
name|isspace
argument_list|(
name|line
index|[
literal|40
index|]
argument_list|)
operator|||
name|get_sha1_hex
argument_list|(
name|line
operator|+
literal|41
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"Need exactly two trees, separated by a space"
argument_list|)
return|;
name|tree2
operator|=
name|lookup_tree
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree2
operator|||
name|parse_tree
argument_list|(
name|tree2
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|printf
argument_list|(
literal|"%s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|tree1
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|tree2
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|diff_tree_sha1
argument_list|(
name|tree1
operator|->
name|object
operator|.
name|sha1
argument_list|,
name|tree2
operator|->
name|object
operator|.
name|sha1
argument_list|,
literal|""
argument_list|,
operator|&
name|log_tree_opt
operator|.
name|diffopt
argument_list|)
expr_stmt|;
name|log_tree_diff_flush
argument_list|(
operator|&
name|log_tree_opt
argument_list|)
expr_stmt|;
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
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|object
modifier|*
name|obj
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
name|sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|obj
operator|=
name|lookup_unknown_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|obj
operator|||
operator|!
name|obj
operator|->
name|parsed
condition|)
name|obj
operator|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|obj
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_COMMIT
condition|)
return|return
name|stdin_diff_commit
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|obj
argument_list|,
name|line
argument_list|,
name|len
argument_list|)
return|;
if|if
condition|(
name|obj
operator|->
name|type
operator|==
name|OBJ_TREE
condition|)
return|return
name|stdin_diff_trees
argument_list|(
operator|(
expr|struct
name|tree
operator|*
operator|)
name|obj
argument_list|,
name|line
argument_list|,
name|len
argument_list|)
return|;
name|error
argument_list|(
literal|"Object %s is a %s, not a commit or tree"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|typename
argument_list|(
name|obj
operator|->
name|type
argument_list|)
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|diff_tree_usage
specifier|static
specifier|const
name|char
name|diff_tree_usage
index|[]
init|=
literal|"git diff-tree [--stdin] [-m] [-c] [--cc] [-s] [-v] [--pretty] [-t] [-r] [--root] "
literal|"[<common diff options>]<tree-ish> [<tree-ish>] [<path>...]\n"
literal|"  -r            diff recursively\n"
literal|"  --root        include the initial commit as diff against /dev/null\n"
name|COMMON_DIFF_OPTIONS_HELP
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|diff_tree_tweak_rev
specifier|static
name|void
name|diff_tree_tweak_rev
parameter_list|(
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|,
name|struct
name|setup_revision_opt
modifier|*
name|opt
parameter_list|)
block|{
if|if
condition|(
operator|!
name|rev
operator|->
name|diffopt
operator|.
name|output_format
condition|)
block|{
if|if
condition|(
name|rev
operator|->
name|dense_combined_merges
condition|)
name|rev
operator|->
name|diffopt
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_PATCH
expr_stmt|;
else|else
name|rev
operator|->
name|diffopt
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_RAW
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|cmd_diff_tree
name|int
name|cmd_diff_tree
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
name|nr_sha1
decl_stmt|;
name|char
name|line
index|[
literal|1000
index|]
decl_stmt|;
name|struct
name|object
modifier|*
name|tree1
decl_stmt|,
modifier|*
name|tree2
decl_stmt|;
specifier|static
name|struct
name|rev_info
modifier|*
name|opt
init|=
operator|&
name|log_tree_opt
decl_stmt|;
name|struct
name|setup_revision_opt
name|s_r_opt
decl_stmt|;
name|int
name|read_stdin
init|=
literal|0
decl_stmt|;
name|init_revisions
argument_list|(
name|opt
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|gitmodules_config
argument_list|()
expr_stmt|;
name|git_config
argument_list|(
name|git_diff_basic_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
comment|/* no "diff" UI options */
name|opt
operator|->
name|abbrev
operator|=
literal|0
expr_stmt|;
name|opt
operator|->
name|diff
operator|=
literal|1
expr_stmt|;
name|opt
operator|->
name|disable_stdin
operator|=
literal|1
expr_stmt|;
name|memset
argument_list|(
operator|&
name|s_r_opt
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|s_r_opt
argument_list|)
argument_list|)
expr_stmt|;
name|s_r_opt
operator|.
name|tweak
operator|=
name|diff_tree_tweak_rev
expr_stmt|;
name|argc
operator|=
name|setup_revisions
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|opt
argument_list|,
operator|&
name|s_r_opt
argument_list|)
expr_stmt|;
while|while
condition|(
operator|--
name|argc
operator|>
literal|0
condition|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
operator|*
operator|++
name|argv
decl_stmt|;
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
name|usage
argument_list|(
name|diff_tree_usage
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * NOTE! We expect "a ^b" to be equal to "a..b", so we 	 * reverse the order of the objects if the second one 	 * is marked UNINTERESTING. 	 */
name|nr_sha1
operator|=
name|opt
operator|->
name|pending
operator|.
name|nr
expr_stmt|;
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
name|tree1
operator|=
name|opt
operator|->
name|pending
operator|.
name|objects
index|[
literal|0
index|]
operator|.
name|item
expr_stmt|;
name|diff_tree_commit_sha1
argument_list|(
name|tree1
operator|->
name|sha1
argument_list|)
expr_stmt|;
break|break;
case|case
literal|2
case|:
name|tree1
operator|=
name|opt
operator|->
name|pending
operator|.
name|objects
index|[
literal|0
index|]
operator|.
name|item
expr_stmt|;
name|tree2
operator|=
name|opt
operator|->
name|pending
operator|.
name|objects
index|[
literal|1
index|]
operator|.
name|item
expr_stmt|;
if|if
condition|(
name|tree2
operator|->
name|flags
operator|&
name|UNINTERESTING
condition|)
block|{
name|struct
name|object
modifier|*
name|tmp
init|=
name|tree2
decl_stmt|;
name|tree2
operator|=
name|tree1
expr_stmt|;
name|tree1
operator|=
name|tmp
expr_stmt|;
block|}
name|diff_tree_sha1
argument_list|(
name|tree1
operator|->
name|sha1
argument_list|,
name|tree2
operator|->
name|sha1
argument_list|,
literal|""
argument_list|,
operator|&
name|opt
operator|->
name|diffopt
argument_list|)
expr_stmt|;
name|log_tree_diff_flush
argument_list|(
name|opt
argument_list|)
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|read_stdin
condition|)
block|{
if|if
condition|(
name|opt
operator|->
name|diffopt
operator|.
name|detect_rename
condition|)
name|opt
operator|->
name|diffopt
operator|.
name|setup
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
name|get_sha1_hex
argument_list|(
name|line
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|fputs
argument_list|(
name|line
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
block|}
else|else
name|diff_tree_stdin
argument_list|(
name|line
argument_list|)
expr_stmt|;
block|}
block|}
return|return
name|diff_result_code
argument_list|(
operator|&
name|opt
operator|->
name|diffopt
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

end_unit

