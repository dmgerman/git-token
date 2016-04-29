begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"lockfile.h"
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
DECL|variable|index_lock
specifier|static
name|struct
name|lock_file
name|index_lock
decl_stmt|;
end_decl_stmt

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
name|hold_locked_index
argument_list|(
operator|&
name|index_lock
argument_list|,
literal|1
argument_list|)
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
literal|"unable to read index file"
argument_list|)
expr_stmt|;
name|active_cache_tree
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|write_locked_index
argument_list|(
operator|&
name|the_index
argument_list|,
operator|&
name|index_lock
argument_list|,
name|COMMIT_LOCK
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to write index file"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

