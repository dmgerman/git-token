begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Licensed under a two-clause BSD-style license.  * See LICENSE for details.  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"string_pool.h"
end_include

begin_include
include|#
directive|include
file|"repo_tree.h"
end_include

begin_include
include|#
directive|include
file|"obj_pool.h"
end_include

begin_include
include|#
directive|include
file|"fast_export.h"
end_include

begin_include
include|#
directive|include
file|"trp.h"
end_include

begin_struct
DECL|struct|repo_dirent
struct|struct
name|repo_dirent
block|{
DECL|member|name_offset
name|uint32_t
name|name_offset
decl_stmt|;
DECL|member|children
name|struct
name|trp_node
name|children
decl_stmt|;
DECL|member|mode
name|uint32_t
name|mode
decl_stmt|;
DECL|member|content_offset
name|uint32_t
name|content_offset
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|repo_dir
struct|struct
name|repo_dir
block|{
DECL|member|entries
name|struct
name|trp_root
name|entries
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|repo_commit
struct|struct
name|repo_commit
block|{
DECL|member|root_dir_offset
name|uint32_t
name|root_dir_offset
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* Memory pools for commit, dir and dirent */
end_comment

begin_macro
name|obj_pool_gen
argument_list|(
argument|commit
argument_list|,
argument|struct repo_commit
argument_list|,
literal|4096
argument_list|)
end_macro

begin_macro
name|obj_pool_gen
argument_list|(
argument|dir
argument_list|,
argument|struct repo_dir
argument_list|,
literal|4096
argument_list|)
end_macro

begin_macro
name|obj_pool_gen
argument_list|(
argument|dent
argument_list|,
argument|struct repo_dirent
argument_list|,
literal|4096
argument_list|)
end_macro

begin_decl_stmt
DECL|variable|active_commit
specifier|static
name|uint32_t
name|active_commit
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|mark
specifier|static
name|uint32_t
name|mark
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|static
name|int
name|repo_dirent_name_cmp
parameter_list|(
specifier|const
name|void
modifier|*
name|a
parameter_list|,
specifier|const
name|void
modifier|*
name|b
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Treap for directory entries */
end_comment

begin_expr_stmt
name|trp_gen
argument_list|(
specifier|static
argument_list|,
name|dent_
argument_list|,
expr|struct
name|repo_dirent
argument_list|,
name|children
argument_list|,
name|dent
argument_list|,
name|repo_dirent_name_cmp
argument_list|)
expr_stmt|;
end_expr_stmt

begin_function
DECL|function|next_blob_mark
name|uint32_t
name|next_blob_mark
parameter_list|(
name|void
parameter_list|)
block|{
return|return
name|mark
operator|++
return|;
block|}
end_function

begin_function
DECL|function|repo_commit_root_dir
specifier|static
name|struct
name|repo_dir
modifier|*
name|repo_commit_root_dir
parameter_list|(
name|struct
name|repo_commit
modifier|*
name|commit
parameter_list|)
block|{
return|return
name|dir_pointer
argument_list|(
name|commit
operator|->
name|root_dir_offset
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|repo_first_dirent
specifier|static
name|struct
name|repo_dirent
modifier|*
name|repo_first_dirent
parameter_list|(
name|struct
name|repo_dir
modifier|*
name|dir
parameter_list|)
block|{
return|return
name|dent_first
argument_list|(
operator|&
name|dir
operator|->
name|entries
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|repo_dirent_name_cmp
specifier|static
name|int
name|repo_dirent_name_cmp
parameter_list|(
specifier|const
name|void
modifier|*
name|a
parameter_list|,
specifier|const
name|void
modifier|*
name|b
parameter_list|)
block|{
specifier|const
name|struct
name|repo_dirent
modifier|*
name|dent1
init|=
name|a
decl_stmt|,
modifier|*
name|dent2
init|=
name|b
decl_stmt|;
name|uint32_t
name|a_offset
init|=
name|dent1
operator|->
name|name_offset
decl_stmt|;
name|uint32_t
name|b_offset
init|=
name|dent2
operator|->
name|name_offset
decl_stmt|;
return|return
operator|(
name|a_offset
operator|>
name|b_offset
operator|)
operator|-
operator|(
name|a_offset
operator|<
name|b_offset
operator|)
return|;
block|}
end_function

begin_function
DECL|function|repo_dirent_is_dir
specifier|static
name|int
name|repo_dirent_is_dir
parameter_list|(
name|struct
name|repo_dirent
modifier|*
name|dent
parameter_list|)
block|{
return|return
name|dent
operator|!=
name|NULL
operator|&&
name|dent
operator|->
name|mode
operator|==
name|REPO_MODE_DIR
return|;
block|}
end_function

begin_function
DECL|function|repo_dir_from_dirent
specifier|static
name|struct
name|repo_dir
modifier|*
name|repo_dir_from_dirent
parameter_list|(
name|struct
name|repo_dirent
modifier|*
name|dent
parameter_list|)
block|{
if|if
condition|(
operator|!
name|repo_dirent_is_dir
argument_list|(
name|dent
argument_list|)
condition|)
return|return
name|NULL
return|;
return|return
name|dir_pointer
argument_list|(
name|dent
operator|->
name|content_offset
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|repo_clone_dir
specifier|static
name|struct
name|repo_dir
modifier|*
name|repo_clone_dir
parameter_list|(
name|struct
name|repo_dir
modifier|*
name|orig_dir
parameter_list|)
block|{
name|uint32_t
name|orig_o
decl_stmt|,
name|new_o
decl_stmt|;
name|orig_o
operator|=
name|dir_offset
argument_list|(
name|orig_dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|orig_o
operator|>=
name|dir_pool
operator|.
name|committed
condition|)
return|return
name|orig_dir
return|;
name|new_o
operator|=
name|dir_alloc
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|orig_dir
operator|=
name|dir_pointer
argument_list|(
name|orig_o
argument_list|)
expr_stmt|;
operator|*
name|dir_pointer
argument_list|(
name|new_o
argument_list|)
operator|=
operator|*
name|orig_dir
expr_stmt|;
return|return
name|dir_pointer
argument_list|(
name|new_o
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|repo_read_dirent
specifier|static
name|struct
name|repo_dirent
modifier|*
name|repo_read_dirent
parameter_list|(
name|uint32_t
name|revision
parameter_list|,
specifier|const
name|uint32_t
modifier|*
name|path
parameter_list|)
block|{
name|uint32_t
name|name
init|=
literal|0
decl_stmt|;
name|struct
name|repo_dirent
modifier|*
name|key
init|=
name|dent_pointer
argument_list|(
name|dent_alloc
argument_list|(
literal|1
argument_list|)
argument_list|)
decl_stmt|;
name|struct
name|repo_dir
modifier|*
name|dir
init|=
name|NULL
decl_stmt|;
name|struct
name|repo_dirent
modifier|*
name|dent
init|=
name|NULL
decl_stmt|;
name|dir
operator|=
name|repo_commit_root_dir
argument_list|(
name|commit_pointer
argument_list|(
name|revision
argument_list|)
argument_list|)
expr_stmt|;
while|while
condition|(
operator|~
operator|(
name|name
operator|=
operator|*
name|path
operator|++
operator|)
condition|)
block|{
name|key
operator|->
name|name_offset
operator|=
name|name
expr_stmt|;
name|dent
operator|=
name|dent_search
argument_list|(
operator|&
name|dir
operator|->
name|entries
argument_list|,
name|key
argument_list|)
expr_stmt|;
if|if
condition|(
name|dent
operator|==
name|NULL
operator|||
operator|!
name|repo_dirent_is_dir
argument_list|(
name|dent
argument_list|)
condition|)
break|break;
name|dir
operator|=
name|repo_dir_from_dirent
argument_list|(
name|dent
argument_list|)
expr_stmt|;
block|}
name|dent_free
argument_list|(
literal|1
argument_list|)
expr_stmt|;
return|return
name|dent
return|;
block|}
end_function

begin_function
DECL|function|repo_write_dirent
specifier|static
name|void
name|repo_write_dirent
parameter_list|(
specifier|const
name|uint32_t
modifier|*
name|path
parameter_list|,
name|uint32_t
name|mode
parameter_list|,
name|uint32_t
name|content_offset
parameter_list|,
name|uint32_t
name|del
parameter_list|)
block|{
name|uint32_t
name|name
decl_stmt|,
name|revision
decl_stmt|,
name|dir_o
init|=
operator|~
literal|0
decl_stmt|,
name|parent_dir_o
init|=
operator|~
literal|0
decl_stmt|;
name|struct
name|repo_dir
modifier|*
name|dir
decl_stmt|;
name|struct
name|repo_dirent
modifier|*
name|key
decl_stmt|;
name|struct
name|repo_dirent
modifier|*
name|dent
init|=
name|NULL
decl_stmt|;
name|revision
operator|=
name|active_commit
expr_stmt|;
name|dir
operator|=
name|repo_commit_root_dir
argument_list|(
name|commit_pointer
argument_list|(
name|revision
argument_list|)
argument_list|)
expr_stmt|;
name|dir
operator|=
name|repo_clone_dir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
name|commit_pointer
argument_list|(
name|revision
argument_list|)
operator|->
name|root_dir_offset
operator|=
name|dir_offset
argument_list|(
name|dir
argument_list|)
expr_stmt|;
while|while
condition|(
operator|~
operator|(
name|name
operator|=
operator|*
name|path
operator|++
operator|)
condition|)
block|{
name|parent_dir_o
operator|=
name|dir_offset
argument_list|(
name|dir
argument_list|)
expr_stmt|;
name|key
operator|=
name|dent_pointer
argument_list|(
name|dent_alloc
argument_list|(
literal|1
argument_list|)
argument_list|)
expr_stmt|;
name|key
operator|->
name|name_offset
operator|=
name|name
expr_stmt|;
name|dent
operator|=
name|dent_search
argument_list|(
operator|&
name|dir
operator|->
name|entries
argument_list|,
name|key
argument_list|)
expr_stmt|;
if|if
condition|(
name|dent
operator|==
name|NULL
condition|)
name|dent
operator|=
name|key
expr_stmt|;
else|else
name|dent_free
argument_list|(
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|dent
operator|==
name|key
condition|)
block|{
name|dent
operator|->
name|mode
operator|=
name|REPO_MODE_DIR
expr_stmt|;
name|dent
operator|->
name|content_offset
operator|=
literal|0
expr_stmt|;
name|dent
operator|=
name|dent_insert
argument_list|(
operator|&
name|dir
operator|->
name|entries
argument_list|,
name|dent
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|dent_offset
argument_list|(
name|dent
argument_list|)
operator|<
name|dent_pool
operator|.
name|committed
condition|)
block|{
name|dir_o
operator|=
name|repo_dirent_is_dir
argument_list|(
name|dent
argument_list|)
condition|?
name|dent
operator|->
name|content_offset
else|:
operator|~
literal|0
expr_stmt|;
name|dent_remove
argument_list|(
operator|&
name|dir
operator|->
name|entries
argument_list|,
name|dent
argument_list|)
expr_stmt|;
name|dent
operator|=
name|dent_pointer
argument_list|(
name|dent_alloc
argument_list|(
literal|1
argument_list|)
argument_list|)
expr_stmt|;
name|dent
operator|->
name|name_offset
operator|=
name|name
expr_stmt|;
name|dent
operator|->
name|mode
operator|=
name|REPO_MODE_DIR
expr_stmt|;
name|dent
operator|->
name|content_offset
operator|=
name|dir_o
expr_stmt|;
name|dent
operator|=
name|dent_insert
argument_list|(
operator|&
name|dir
operator|->
name|entries
argument_list|,
name|dent
argument_list|)
expr_stmt|;
block|}
name|dir
operator|=
name|repo_dir_from_dirent
argument_list|(
name|dent
argument_list|)
expr_stmt|;
name|dir
operator|=
name|repo_clone_dir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
name|dent
operator|->
name|content_offset
operator|=
name|dir_offset
argument_list|(
name|dir
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|dent
operator|==
name|NULL
condition|)
return|return;
name|dent
operator|->
name|mode
operator|=
name|mode
expr_stmt|;
name|dent
operator|->
name|content_offset
operator|=
name|content_offset
expr_stmt|;
if|if
condition|(
name|del
operator|&&
operator|~
name|parent_dir_o
condition|)
name|dent_remove
argument_list|(
operator|&
name|dir_pointer
argument_list|(
name|parent_dir_o
argument_list|)
operator|->
name|entries
argument_list|,
name|dent
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|repo_read_path
name|uint32_t
name|repo_read_path
parameter_list|(
specifier|const
name|uint32_t
modifier|*
name|path
parameter_list|)
block|{
name|uint32_t
name|content_offset
init|=
literal|0
decl_stmt|;
name|struct
name|repo_dirent
modifier|*
name|dent
init|=
name|repo_read_dirent
argument_list|(
name|active_commit
argument_list|,
name|path
argument_list|)
decl_stmt|;
if|if
condition|(
name|dent
operator|!=
name|NULL
condition|)
name|content_offset
operator|=
name|dent
operator|->
name|content_offset
expr_stmt|;
return|return
name|content_offset
return|;
block|}
end_function

begin_function
DECL|function|repo_read_mode
name|uint32_t
name|repo_read_mode
parameter_list|(
specifier|const
name|uint32_t
modifier|*
name|path
parameter_list|)
block|{
name|struct
name|repo_dirent
modifier|*
name|dent
init|=
name|repo_read_dirent
argument_list|(
name|active_commit
argument_list|,
name|path
argument_list|)
decl_stmt|;
if|if
condition|(
name|dent
operator|==
name|NULL
condition|)
name|die
argument_list|(
literal|"invalid dump: path to be modified is missing"
argument_list|)
expr_stmt|;
return|return
name|dent
operator|->
name|mode
return|;
block|}
end_function

begin_function
DECL|function|repo_copy
name|void
name|repo_copy
parameter_list|(
name|uint32_t
name|revision
parameter_list|,
specifier|const
name|uint32_t
modifier|*
name|src
parameter_list|,
specifier|const
name|uint32_t
modifier|*
name|dst
parameter_list|)
block|{
name|uint32_t
name|mode
init|=
literal|0
decl_stmt|,
name|content_offset
init|=
literal|0
decl_stmt|;
name|struct
name|repo_dirent
modifier|*
name|src_dent
decl_stmt|;
name|src_dent
operator|=
name|repo_read_dirent
argument_list|(
name|revision
argument_list|,
name|src
argument_list|)
expr_stmt|;
if|if
condition|(
name|src_dent
operator|!=
name|NULL
condition|)
block|{
name|mode
operator|=
name|src_dent
operator|->
name|mode
expr_stmt|;
name|content_offset
operator|=
name|src_dent
operator|->
name|content_offset
expr_stmt|;
name|repo_write_dirent
argument_list|(
name|dst
argument_list|,
name|mode
argument_list|,
name|content_offset
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|repo_add
name|void
name|repo_add
parameter_list|(
name|uint32_t
modifier|*
name|path
parameter_list|,
name|uint32_t
name|mode
parameter_list|,
name|uint32_t
name|blob_mark
parameter_list|)
block|{
name|repo_write_dirent
argument_list|(
name|path
argument_list|,
name|mode
argument_list|,
name|blob_mark
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|repo_delete
name|void
name|repo_delete
parameter_list|(
name|uint32_t
modifier|*
name|path
parameter_list|)
block|{
name|repo_write_dirent
argument_list|(
name|path
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function_decl
specifier|static
name|void
name|repo_git_add_r
parameter_list|(
name|uint32_t
name|depth
parameter_list|,
name|uint32_t
modifier|*
name|path
parameter_list|,
name|struct
name|repo_dir
modifier|*
name|dir
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|repo_git_add
specifier|static
name|void
name|repo_git_add
parameter_list|(
name|uint32_t
name|depth
parameter_list|,
name|uint32_t
modifier|*
name|path
parameter_list|,
name|struct
name|repo_dirent
modifier|*
name|dent
parameter_list|)
block|{
if|if
condition|(
name|repo_dirent_is_dir
argument_list|(
name|dent
argument_list|)
condition|)
name|repo_git_add_r
argument_list|(
name|depth
argument_list|,
name|path
argument_list|,
name|repo_dir_from_dirent
argument_list|(
name|dent
argument_list|)
argument_list|)
expr_stmt|;
else|else
name|fast_export_modify
argument_list|(
name|depth
argument_list|,
name|path
argument_list|,
name|dent
operator|->
name|mode
argument_list|,
name|dent
operator|->
name|content_offset
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|repo_git_add_r
specifier|static
name|void
name|repo_git_add_r
parameter_list|(
name|uint32_t
name|depth
parameter_list|,
name|uint32_t
modifier|*
name|path
parameter_list|,
name|struct
name|repo_dir
modifier|*
name|dir
parameter_list|)
block|{
name|struct
name|repo_dirent
modifier|*
name|de
init|=
name|repo_first_dirent
argument_list|(
name|dir
argument_list|)
decl_stmt|;
while|while
condition|(
name|de
condition|)
block|{
name|path
index|[
name|depth
index|]
operator|=
name|de
operator|->
name|name_offset
expr_stmt|;
name|repo_git_add
argument_list|(
name|depth
operator|+
literal|1
argument_list|,
name|path
argument_list|,
name|de
argument_list|)
expr_stmt|;
name|de
operator|=
name|dent_next
argument_list|(
operator|&
name|dir
operator|->
name|entries
argument_list|,
name|de
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|repo_diff_r
specifier|static
name|void
name|repo_diff_r
parameter_list|(
name|uint32_t
name|depth
parameter_list|,
name|uint32_t
modifier|*
name|path
parameter_list|,
name|struct
name|repo_dir
modifier|*
name|dir1
parameter_list|,
name|struct
name|repo_dir
modifier|*
name|dir2
parameter_list|)
block|{
name|struct
name|repo_dirent
modifier|*
name|de1
decl_stmt|,
modifier|*
name|de2
decl_stmt|;
name|de1
operator|=
name|repo_first_dirent
argument_list|(
name|dir1
argument_list|)
expr_stmt|;
name|de2
operator|=
name|repo_first_dirent
argument_list|(
name|dir2
argument_list|)
expr_stmt|;
while|while
condition|(
name|de1
operator|&&
name|de2
condition|)
block|{
if|if
condition|(
name|de1
operator|->
name|name_offset
operator|<
name|de2
operator|->
name|name_offset
condition|)
block|{
name|path
index|[
name|depth
index|]
operator|=
name|de1
operator|->
name|name_offset
expr_stmt|;
name|fast_export_delete
argument_list|(
name|depth
operator|+
literal|1
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|de1
operator|=
name|dent_next
argument_list|(
operator|&
name|dir1
operator|->
name|entries
argument_list|,
name|de1
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|de1
operator|->
name|name_offset
operator|>
name|de2
operator|->
name|name_offset
condition|)
block|{
name|path
index|[
name|depth
index|]
operator|=
name|de2
operator|->
name|name_offset
expr_stmt|;
name|repo_git_add
argument_list|(
name|depth
operator|+
literal|1
argument_list|,
name|path
argument_list|,
name|de2
argument_list|)
expr_stmt|;
name|de2
operator|=
name|dent_next
argument_list|(
operator|&
name|dir2
operator|->
name|entries
argument_list|,
name|de2
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|path
index|[
name|depth
index|]
operator|=
name|de1
operator|->
name|name_offset
expr_stmt|;
if|if
condition|(
name|de1
operator|->
name|mode
operator|==
name|de2
operator|->
name|mode
operator|&&
name|de1
operator|->
name|content_offset
operator|==
name|de2
operator|->
name|content_offset
condition|)
block|{
empty_stmt|;
comment|/* No change. */
block|}
elseif|else
if|if
condition|(
name|repo_dirent_is_dir
argument_list|(
name|de1
argument_list|)
operator|&&
name|repo_dirent_is_dir
argument_list|(
name|de2
argument_list|)
condition|)
block|{
name|repo_diff_r
argument_list|(
name|depth
operator|+
literal|1
argument_list|,
name|path
argument_list|,
name|repo_dir_from_dirent
argument_list|(
name|de1
argument_list|)
argument_list|,
name|repo_dir_from_dirent
argument_list|(
name|de2
argument_list|)
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|repo_dirent_is_dir
argument_list|(
name|de1
argument_list|)
operator|&&
operator|!
name|repo_dirent_is_dir
argument_list|(
name|de2
argument_list|)
condition|)
block|{
name|repo_git_add
argument_list|(
name|depth
operator|+
literal|1
argument_list|,
name|path
argument_list|,
name|de2
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|fast_export_delete
argument_list|(
name|depth
operator|+
literal|1
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|repo_git_add
argument_list|(
name|depth
operator|+
literal|1
argument_list|,
name|path
argument_list|,
name|de2
argument_list|)
expr_stmt|;
block|}
name|de1
operator|=
name|dent_next
argument_list|(
operator|&
name|dir1
operator|->
name|entries
argument_list|,
name|de1
argument_list|)
expr_stmt|;
name|de2
operator|=
name|dent_next
argument_list|(
operator|&
name|dir2
operator|->
name|entries
argument_list|,
name|de2
argument_list|)
expr_stmt|;
block|}
while|while
condition|(
name|de1
condition|)
block|{
name|path
index|[
name|depth
index|]
operator|=
name|de1
operator|->
name|name_offset
expr_stmt|;
name|fast_export_delete
argument_list|(
name|depth
operator|+
literal|1
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|de1
operator|=
name|dent_next
argument_list|(
operator|&
name|dir1
operator|->
name|entries
argument_list|,
name|de1
argument_list|)
expr_stmt|;
block|}
while|while
condition|(
name|de2
condition|)
block|{
name|path
index|[
name|depth
index|]
operator|=
name|de2
operator|->
name|name_offset
expr_stmt|;
name|repo_git_add
argument_list|(
name|depth
operator|+
literal|1
argument_list|,
name|path
argument_list|,
name|de2
argument_list|)
expr_stmt|;
name|de2
operator|=
name|dent_next
argument_list|(
operator|&
name|dir2
operator|->
name|entries
argument_list|,
name|de2
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_decl_stmt
DECL|variable|path_stack
specifier|static
name|uint32_t
name|path_stack
index|[
name|REPO_MAX_PATH_DEPTH
index|]
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|repo_diff
name|void
name|repo_diff
parameter_list|(
name|uint32_t
name|r1
parameter_list|,
name|uint32_t
name|r2
parameter_list|)
block|{
name|repo_diff_r
argument_list|(
literal|0
argument_list|,
name|path_stack
argument_list|,
name|repo_commit_root_dir
argument_list|(
name|commit_pointer
argument_list|(
name|r1
argument_list|)
argument_list|)
argument_list|,
name|repo_commit_root_dir
argument_list|(
name|commit_pointer
argument_list|(
name|r2
argument_list|)
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|repo_commit
name|void
name|repo_commit
parameter_list|(
name|uint32_t
name|revision
parameter_list|,
name|uint32_t
name|author
parameter_list|,
name|char
modifier|*
name|log
parameter_list|,
name|uint32_t
name|uuid
parameter_list|,
name|uint32_t
name|url
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|)
block|{
name|fast_export_commit
argument_list|(
name|revision
argument_list|,
name|author
argument_list|,
name|log
argument_list|,
name|uuid
argument_list|,
name|url
argument_list|,
name|timestamp
argument_list|)
expr_stmt|;
name|dent_commit
argument_list|()
expr_stmt|;
name|dir_commit
argument_list|()
expr_stmt|;
name|active_commit
operator|=
name|commit_alloc
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|commit_pointer
argument_list|(
name|active_commit
argument_list|)
operator|->
name|root_dir_offset
operator|=
name|commit_pointer
argument_list|(
name|active_commit
operator|-
literal|1
argument_list|)
operator|->
name|root_dir_offset
expr_stmt|;
block|}
end_function

begin_function
DECL|function|mark_init
specifier|static
name|void
name|mark_init
parameter_list|(
name|void
parameter_list|)
block|{
name|uint32_t
name|i
decl_stmt|;
name|mark
operator|=
literal|1024
operator|*
literal|1024
operator|*
literal|1024
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|dent_pool
operator|.
name|size
condition|;
name|i
operator|++
control|)
if|if
condition|(
operator|!
name|repo_dirent_is_dir
argument_list|(
name|dent_pointer
argument_list|(
name|i
argument_list|)
argument_list|)
operator|&&
name|dent_pointer
argument_list|(
name|i
argument_list|)
operator|->
name|content_offset
operator|>
name|mark
condition|)
name|mark
operator|=
name|dent_pointer
argument_list|(
name|i
argument_list|)
operator|->
name|content_offset
expr_stmt|;
name|mark
operator|++
expr_stmt|;
block|}
end_function

begin_function
DECL|function|repo_init
name|void
name|repo_init
parameter_list|(
name|void
parameter_list|)
block|{
name|mark_init
argument_list|()
expr_stmt|;
if|if
condition|(
name|commit_pool
operator|.
name|size
operator|==
literal|0
condition|)
block|{
comment|/* Create empty tree for commit 0. */
name|commit_alloc
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|commit_pointer
argument_list|(
literal|0
argument_list|)
operator|->
name|root_dir_offset
operator|=
name|dir_alloc
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|dir_pointer
argument_list|(
literal|0
argument_list|)
operator|->
name|entries
operator|.
name|trp_root
operator|=
operator|~
literal|0
expr_stmt|;
name|dir_commit
argument_list|()
expr_stmt|;
block|}
comment|/* Preallocate next commit, ready for changes. */
name|active_commit
operator|=
name|commit_alloc
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|commit_pointer
argument_list|(
name|active_commit
argument_list|)
operator|->
name|root_dir_offset
operator|=
name|commit_pointer
argument_list|(
name|active_commit
operator|-
literal|1
argument_list|)
operator|->
name|root_dir_offset
expr_stmt|;
block|}
end_function

begin_function
DECL|function|repo_reset
name|void
name|repo_reset
parameter_list|(
name|void
parameter_list|)
block|{
name|pool_reset
argument_list|()
expr_stmt|;
name|commit_reset
argument_list|()
expr_stmt|;
name|dir_reset
argument_list|()
expr_stmt|;
name|dent_reset
argument_list|()
expr_stmt|;
block|}
end_function

end_unit

