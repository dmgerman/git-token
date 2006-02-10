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
DECL|variable|no_commit_id
specifier|static
name|int
name|no_commit_id
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
DECL|variable|combine_merges
specifier|static
name|int
name|combine_merges
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|dense_combined_merges
specifier|static
name|int
name|dense_combined_merges
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
DECL|variable|always_show_header
specifier|static
name|int
name|always_show_header
init|=
literal|0
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

begin_decl_stmt
DECL|variable|diff_options
specifier|static
name|struct
name|diff_options
name|diff_options
decl_stmt|;
end_decl_stmt

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
operator|&
name|diff_options
argument_list|)
expr_stmt|;
if|if
condition|(
name|diff_queue_is_empty
argument_list|()
condition|)
block|{
name|int
name|saved_fmt
init|=
name|diff_options
operator|.
name|output_format
decl_stmt|;
name|diff_options
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_NO_OUTPUT
expr_stmt|;
name|diff_flush
argument_list|(
operator|&
name|diff_options
argument_list|)
expr_stmt|;
name|diff_options
operator|.
name|output_format
operator|=
name|saved_fmt
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
if|if
condition|(
operator|!
name|no_commit_id
condition|)
name|printf
argument_list|(
literal|"%s%c"
argument_list|,
name|header
argument_list|,
name|diff_options
operator|.
name|line_termination
argument_list|)
expr_stmt|;
name|header
operator|=
name|NULL
expr_stmt|;
block|}
name|diff_flush
argument_list|(
operator|&
name|diff_options
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
name|ret
operator|=
name|diff_tree_sha1
argument_list|(
name|old
argument_list|,
name|new
argument_list|,
name|base
argument_list|,
operator|&
name|diff_options
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
name|struct
name|tree_desc
name|empty
decl_stmt|,
name|real
decl_stmt|;
name|tree
operator|=
name|read_object_with_reference
argument_list|(
name|new
argument_list|,
literal|"tree"
argument_list|,
operator|&
name|real
operator|.
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
name|real
operator|.
name|buf
operator|=
name|tree
expr_stmt|;
name|empty
operator|.
name|buf
operator|=
literal|""
expr_stmt|;
name|empty
operator|.
name|size
operator|=
literal|0
expr_stmt|;
name|retval
operator|=
name|diff_tree
argument_list|(
operator|&
name|empty
argument_list|,
operator|&
name|real
argument_list|,
name|base
argument_list|,
operator|&
name|diff_options
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
specifier|const
name|char
modifier|*
name|generate_header
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|commit_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|parent_sha1
parameter_list|,
specifier|const
name|struct
name|commit
modifier|*
name|commit
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
name|unsigned
name|long
name|len
decl_stmt|;
name|int
name|abbrev
init|=
name|diff_options
operator|.
name|abbrev
decl_stmt|;
specifier|const
name|char
modifier|*
name|msg
init|=
name|commit
operator|->
name|buffer
decl_stmt|;
if|if
condition|(
operator|!
name|verbose_header
condition|)
return|return
name|sha1_to_hex
argument_list|(
name|commit_sha1
argument_list|)
return|;
name|len
operator|=
name|strlen
argument_list|(
name|msg
argument_list|)
expr_stmt|;
name|offset
operator|=
name|sprintf
argument_list|(
name|this_header
argument_list|,
literal|"%s%s "
argument_list|,
name|header_prefix
argument_list|,
name|diff_unique_abbrev
argument_list|(
name|commit_sha1
argument_list|,
name|abbrev
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit_sha1
operator|!=
name|parent_sha1
condition|)
name|offset
operator|+=
name|sprintf
argument_list|(
name|this_header
operator|+
name|offset
argument_list|,
literal|"(from %s)\n"
argument_list|,
name|parent_sha1
condition|?
name|diff_unique_abbrev
argument_list|(
name|parent_sha1
argument_list|,
name|abbrev
argument_list|)
else|:
literal|"root"
argument_list|)
expr_stmt|;
else|else
name|offset
operator|+=
name|sprintf
argument_list|(
name|this_header
operator|+
name|offset
argument_list|,
literal|"(from parents)\n"
argument_list|)
expr_stmt|;
name|offset
operator|+=
name|pretty_print_commit
argument_list|(
name|commit_format
argument_list|,
name|commit
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
argument_list|,
name|abbrev
argument_list|)
expr_stmt|;
if|if
condition|(
name|always_show_header
condition|)
block|{
name|puts
argument_list|(
name|this_header
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
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
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
name|struct
name|commit_list
modifier|*
name|parents
decl_stmt|;
name|unsigned
specifier|const
name|char
modifier|*
name|sha1
init|=
name|commit
operator|->
name|object
operator|.
name|sha1
decl_stmt|;
comment|/* Root commit? */
if|if
condition|(
name|show_root_diff
operator|&&
operator|!
name|commit
operator|->
name|parents
condition|)
block|{
name|header
operator|=
name|generate_header
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|,
name|commit
argument_list|)
expr_stmt|;
name|diff_root_tree
argument_list|(
name|sha1
argument_list|,
literal|""
argument_list|)
expr_stmt|;
block|}
comment|/* More than one parent? */
if|if
condition|(
name|commit
operator|->
name|parents
operator|&&
name|commit
operator|->
name|parents
operator|->
name|next
condition|)
block|{
if|if
condition|(
name|ignore_merges
condition|)
return|return
literal|0
return|;
elseif|else
if|if
condition|(
name|combine_merges
condition|)
block|{
name|header
operator|=
name|generate_header
argument_list|(
name|sha1
argument_list|,
name|sha1
argument_list|,
name|commit
argument_list|)
expr_stmt|;
name|header
operator|=
name|diff_tree_combined_merge
argument_list|(
name|sha1
argument_list|,
name|header
argument_list|,
name|dense_combined_merges
argument_list|,
operator|&
name|diff_options
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|header
operator|&&
name|verbose_header
condition|)
name|header_prefix
operator|=
literal|"\ndiff-tree "
expr_stmt|;
return|return
literal|0
return|;
block|}
block|}
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
name|parents
operator|=
name|parents
operator|->
name|next
control|)
block|{
name|struct
name|commit
modifier|*
name|parent
init|=
name|parents
operator|->
name|item
decl_stmt|;
name|header
operator|=
name|generate_header
argument_list|(
name|sha1
argument_list|,
name|parent
operator|->
name|object
operator|.
name|sha1
argument_list|,
name|commit
argument_list|)
expr_stmt|;
name|diff_tree_sha1_top
argument_list|(
name|parent
operator|->
name|object
operator|.
name|sha1
argument_list|,
name|sha1
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
block|}
return|return
literal|0
return|;
block|}
end_function

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
name|diff_tree_commit
argument_list|(
name|commit
argument_list|)
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
name|commit
modifier|*
name|commit
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
name|commit
operator|=
name|lookup_commit
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
operator|||
name|parse_commit
argument_list|(
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
name|diff_tree_commit
argument_list|(
name|commit
argument_list|)
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
literal|"git-diff-tree [--stdin] [-m] [-c] [--cc] [-s] [-v] [--pretty] [-t] [-r] [--root] "
literal|"[<common diff options>]<tree-ish> [<tree-ish>] [<path>...]\n"
literal|"  -r            diff recursively\n"
literal|"  --root        include the initial commit as diff against /dev/null\n"
name|COMMON_DIFF_OPTIONS_HELP
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
specifier|const
name|char
modifier|*
name|prefix
init|=
name|setup_git_directory
argument_list|()
decl_stmt|;
name|git_config
argument_list|(
name|git_diff_config
argument_list|)
expr_stmt|;
name|nr_sha1
operator|=
literal|0
expr_stmt|;
name|diff_setup
argument_list|(
operator|&
name|diff_options
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|diff_opt_cnt
decl_stmt|;
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
name|diff_opt_cnt
operator|=
name|diff_opt_parse
argument_list|(
operator|&
name|diff_options
argument_list|,
name|argv
argument_list|,
name|argc
argument_list|)
expr_stmt|;
if|if
condition|(
name|diff_opt_cnt
operator|<
literal|0
condition|)
name|usage
argument_list|(
name|diff_tree_usage
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|diff_opt_cnt
condition|)
block|{
name|argv
operator|+=
name|diff_opt_cnt
operator|-
literal|1
expr_stmt|;
name|argc
operator|-=
name|diff_opt_cnt
operator|-
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
name|diff_options
operator|.
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
name|diff_options
operator|.
name|recursive
operator|=
literal|1
expr_stmt|;
name|diff_options
operator|.
name|tree_in_recursive
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
literal|"-m"
argument_list|)
condition|)
block|{
name|combine_merges
operator|=
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
literal|"-c"
argument_list|)
condition|)
block|{
name|combine_merges
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
literal|"--cc"
argument_list|)
condition|)
block|{
name|dense_combined_merges
operator|=
name|combine_merges
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
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--pretty"
argument_list|,
literal|8
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
name|commit_format
operator|=
name|get_commit_format
argument_list|(
name|arg
operator|+
literal|8
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--no-commit-id"
argument_list|)
condition|)
block|{
name|no_commit_id
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
literal|"--always"
argument_list|)
condition|)
block|{
name|always_show_header
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
name|combine_merges
condition|)
name|ignore_merges
operator|=
literal|0
expr_stmt|;
comment|/* We can only do dense combined merges with diff output */
if|if
condition|(
name|dense_combined_merges
condition|)
name|diff_options
operator|.
name|output_format
operator|=
name|DIFF_FORMAT_PATCH
expr_stmt|;
if|if
condition|(
name|diff_options
operator|.
name|output_format
operator|==
name|DIFF_FORMAT_PATCH
condition|)
name|diff_options
operator|.
name|recursive
operator|=
literal|1
expr_stmt|;
name|diff_tree_setup_paths
argument_list|(
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
argument_list|)
argument_list|)
expr_stmt|;
name|diff_setup_done
argument_list|(
operator|&
name|diff_options
argument_list|)
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
name|diff_tree_commit_sha1
argument_list|(
name|sha1
index|[
literal|0
index|]
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
name|diff_options
operator|.
name|detect_rename
condition|)
name|diff_options
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

