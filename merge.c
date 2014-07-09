begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"resolve-undo.h"
end_include

begin_include
include|#
directive|include
file|"tree-walk.h"
end_include

begin_include
include|#
directive|include
file|"unpack-trees.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_function
DECL|function|merge_argument
specifier|static
specifier|const
name|char
modifier|*
name|merge_argument
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
if|if
condition|(
name|commit
condition|)
return|return
name|sha1_to_hex
argument_list|(
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
return|;
else|else
return|return
name|EMPTY_TREE_SHA1_HEX
return|;
block|}
end_function

begin_function
DECL|function|try_merge_command
name|int
name|try_merge_command
parameter_list|(
specifier|const
name|char
modifier|*
name|strategy
parameter_list|,
name|size_t
name|xopts_nr
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|xopts
parameter_list|,
name|struct
name|commit_list
modifier|*
name|common
parameter_list|,
specifier|const
name|char
modifier|*
name|head_arg
parameter_list|,
name|struct
name|commit_list
modifier|*
name|remotes
parameter_list|)
block|{
name|struct
name|argv_array
name|args
init|=
name|ARGV_ARRAY_INIT
decl_stmt|;
name|int
name|i
decl_stmt|,
name|ret
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|j
decl_stmt|;
name|argv_array_pushf
argument_list|(
operator|&
name|args
argument_list|,
literal|"merge-%s"
argument_list|,
name|strategy
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
name|xopts_nr
condition|;
name|i
operator|++
control|)
name|argv_array_pushf
argument_list|(
operator|&
name|args
argument_list|,
literal|"--%s"
argument_list|,
name|xopts
index|[
name|i
index|]
argument_list|)
expr_stmt|;
for|for
control|(
name|j
operator|=
name|common
init|;
name|j
condition|;
name|j
operator|=
name|j
operator|->
name|next
control|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|merge_argument
argument_list|(
name|j
operator|->
name|item
argument_list|)
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
literal|"--"
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|head_arg
argument_list|)
expr_stmt|;
for|for
control|(
name|j
operator|=
name|remotes
init|;
name|j
condition|;
name|j
operator|=
name|j
operator|->
name|next
control|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|merge_argument
argument_list|(
name|j
operator|->
name|item
argument_list|)
argument_list|)
expr_stmt|;
name|ret
operator|=
name|run_command_v_opt
argument_list|(
name|args
operator|.
name|argv
argument_list|,
name|RUN_GIT_CMD
argument_list|)
expr_stmt|;
name|argv_array_clear
argument_list|(
operator|&
name|args
argument_list|)
expr_stmt|;
name|discard_cache
argument_list|()
expr_stmt|;
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"failed to read the cache"
argument_list|)
argument_list|)
expr_stmt|;
name|resolve_undo_clear
argument_list|()
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|checkout_fast_forward
name|int
name|checkout_fast_forward
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|head
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|remote
parameter_list|,
name|int
name|overwrite_ignore
parameter_list|)
block|{
name|struct
name|tree
modifier|*
name|trees
index|[
name|MAX_UNPACK_TREES
index|]
decl_stmt|;
name|struct
name|unpack_trees_options
name|opts
decl_stmt|;
name|struct
name|tree_desc
name|t
index|[
name|MAX_UNPACK_TREES
index|]
decl_stmt|;
name|int
name|i
decl_stmt|,
name|fd
decl_stmt|,
name|nr_trees
init|=
literal|0
decl_stmt|;
name|struct
name|dir_struct
name|dir
decl_stmt|;
name|struct
name|lock_file
modifier|*
name|lock_file
init|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|lock_file
argument_list|)
argument_list|)
decl_stmt|;
name|refresh_cache
argument_list|(
name|REFRESH_QUIET
argument_list|)
expr_stmt|;
name|fd
operator|=
name|hold_locked_index
argument_list|(
name|lock_file
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|trees
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|trees
argument_list|)
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|opts
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|opts
argument_list|)
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|t
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|overwrite_ignore
condition|)
block|{
name|memset
argument_list|(
operator|&
name|dir
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|dir
argument_list|)
argument_list|)
expr_stmt|;
name|dir
operator|.
name|flags
operator||=
name|DIR_SHOW_IGNORED
expr_stmt|;
name|setup_standard_excludes
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
name|opts
operator|.
name|dir
operator|=
operator|&
name|dir
expr_stmt|;
block|}
name|opts
operator|.
name|head_idx
operator|=
literal|1
expr_stmt|;
name|opts
operator|.
name|src_index
operator|=
operator|&
name|the_index
expr_stmt|;
name|opts
operator|.
name|dst_index
operator|=
operator|&
name|the_index
expr_stmt|;
name|opts
operator|.
name|update
operator|=
literal|1
expr_stmt|;
name|opts
operator|.
name|verbose_update
operator|=
literal|1
expr_stmt|;
name|opts
operator|.
name|merge
operator|=
literal|1
expr_stmt|;
name|opts
operator|.
name|fn
operator|=
name|twoway_merge
expr_stmt|;
name|setup_unpack_trees_porcelain
argument_list|(
operator|&
name|opts
argument_list|,
literal|"merge"
argument_list|)
expr_stmt|;
name|trees
index|[
name|nr_trees
index|]
operator|=
name|parse_tree_indirect
argument_list|(
name|head
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|trees
index|[
name|nr_trees
operator|++
index|]
condition|)
return|return
operator|-
literal|1
return|;
name|trees
index|[
name|nr_trees
index|]
operator|=
name|parse_tree_indirect
argument_list|(
name|remote
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|trees
index|[
name|nr_trees
operator|++
index|]
condition|)
return|return
operator|-
literal|1
return|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_trees
condition|;
name|i
operator|++
control|)
block|{
name|parse_tree
argument_list|(
name|trees
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|init_tree_desc
argument_list|(
name|t
operator|+
name|i
argument_list|,
name|trees
index|[
name|i
index|]
operator|->
name|buffer
argument_list|,
name|trees
index|[
name|i
index|]
operator|->
name|size
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|unpack_trees
argument_list|(
name|nr_trees
argument_list|,
name|t
argument_list|,
operator|&
name|opts
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|write_cache
argument_list|(
name|fd
argument_list|,
name|active_cache
argument_list|,
name|active_nr
argument_list|)
operator|||
name|commit_locked_index
argument_list|(
name|lock_file
argument_list|)
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"unable to write new index file"
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

