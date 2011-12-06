begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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

begin_function
DECL|function|dump_one
specifier|static
name|void
name|dump_one
parameter_list|(
name|struct
name|cache_tree
modifier|*
name|it
parameter_list|,
specifier|const
name|char
modifier|*
name|pfx
parameter_list|,
specifier|const
name|char
modifier|*
name|x
parameter_list|)
block|{
if|if
condition|(
name|it
operator|->
name|entry_count
operator|<
literal|0
condition|)
name|printf
argument_list|(
literal|"%-40s %s%s (%d subtrees)\n"
argument_list|,
literal|"invalid"
argument_list|,
name|x
argument_list|,
name|pfx
argument_list|,
name|it
operator|->
name|subtree_nr
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s %s%s (%d entries, %d subtrees)\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|it
operator|->
name|sha1
argument_list|)
argument_list|,
name|x
argument_list|,
name|pfx
argument_list|,
name|it
operator|->
name|entry_count
argument_list|,
name|it
operator|->
name|subtree_nr
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|dump_cache_tree
specifier|static
name|int
name|dump_cache_tree
parameter_list|(
name|struct
name|cache_tree
modifier|*
name|it
parameter_list|,
name|struct
name|cache_tree
modifier|*
name|ref
parameter_list|,
specifier|const
name|char
modifier|*
name|pfx
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|errs
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|it
operator|||
operator|!
name|ref
condition|)
comment|/* missing in either */
return|return
literal|0
return|;
if|if
condition|(
name|it
operator|->
name|entry_count
operator|<
literal|0
condition|)
block|{
name|dump_one
argument_list|(
name|it
argument_list|,
name|pfx
argument_list|,
literal|""
argument_list|)
expr_stmt|;
name|dump_one
argument_list|(
name|ref
argument_list|,
name|pfx
argument_list|,
literal|"#(ref) "
argument_list|)
expr_stmt|;
if|if
condition|(
name|it
operator|->
name|subtree_nr
operator|!=
name|ref
operator|->
name|subtree_nr
condition|)
name|errs
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|dump_one
argument_list|(
name|it
argument_list|,
name|pfx
argument_list|,
literal|""
argument_list|)
expr_stmt|;
if|if
condition|(
name|hashcmp
argument_list|(
name|it
operator|->
name|sha1
argument_list|,
name|ref
operator|->
name|sha1
argument_list|)
operator|||
name|ref
operator|->
name|entry_count
operator|!=
name|it
operator|->
name|entry_count
operator|||
name|ref
operator|->
name|subtree_nr
operator|!=
name|it
operator|->
name|subtree_nr
condition|)
block|{
name|dump_one
argument_list|(
name|ref
argument_list|,
name|pfx
argument_list|,
literal|"#(ref) "
argument_list|)
expr_stmt|;
name|errs
operator|=
literal|1
expr_stmt|;
block|}
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|it
operator|->
name|subtree_nr
condition|;
name|i
operator|++
control|)
block|{
name|char
name|path
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|struct
name|cache_tree_sub
modifier|*
name|down
init|=
name|it
operator|->
name|down
index|[
name|i
index|]
decl_stmt|;
name|struct
name|cache_tree_sub
modifier|*
name|rdwn
decl_stmt|;
name|rdwn
operator|=
name|cache_tree_sub
argument_list|(
name|ref
argument_list|,
name|down
operator|->
name|name
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|path
argument_list|,
literal|"%s%.*s/"
argument_list|,
name|pfx
argument_list|,
name|down
operator|->
name|namelen
argument_list|,
name|down
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|dump_cache_tree
argument_list|(
name|down
operator|->
name|cache_tree
argument_list|,
name|rdwn
operator|->
name|cache_tree
argument_list|,
name|path
argument_list|)
condition|)
name|errs
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|errs
return|;
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|ac
parameter_list|,
name|char
modifier|*
modifier|*
name|av
parameter_list|)
block|{
name|struct
name|cache_tree
modifier|*
name|another
init|=
name|cache_tree
argument_list|()
decl_stmt|;
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to read index file"
argument_list|)
expr_stmt|;
name|cache_tree_update
argument_list|(
name|another
argument_list|,
name|active_cache
argument_list|,
name|active_nr
argument_list|,
literal|0
argument_list|,
literal|1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return
name|dump_cache_tree
argument_list|(
name|active_cache_tree
argument_list|,
name|another
argument_list|,
literal|""
argument_list|)
return|;
block|}
end_function

end_unit

