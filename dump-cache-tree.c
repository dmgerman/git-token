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

begin_decl_stmt
DECL|variable|active_cache_sha1
specifier|static
name|unsigned
name|char
name|active_cache_sha1
index|[
literal|20
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|active_cache_tree
specifier|static
name|struct
name|cache_tree
modifier|*
name|active_cache_tree
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|dump_cache_tree
specifier|static
name|void
name|dump_cache_tree
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
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
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
literal|"%-40s %s\n"
argument_list|,
literal|"invalid"
argument_list|,
name|pfx
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s %s (%d entries)\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|it
operator|->
name|sha1
argument_list|)
argument_list|,
name|pfx
argument_list|,
name|it
operator|->
name|entry_count
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
name|dump_cache_tree
argument_list|(
name|down
operator|->
name|cache_tree
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
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
if|if
condition|(
name|read_cache_1
argument_list|(
name|active_cache_sha1
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to read index file"
argument_list|)
expr_stmt|;
name|active_cache_tree
operator|=
name|read_cache_tree
argument_list|(
name|active_cache_sha1
argument_list|)
expr_stmt|;
name|dump_cache_tree
argument_list|(
name|active_cache_tree
argument_list|,
literal|""
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

