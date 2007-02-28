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
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"reachable.h"
end_include

begin_decl_stmt
DECL|variable|prune_usage
specifier|static
specifier|const
name|char
name|prune_usage
index|[]
init|=
literal|"git-prune [-n]"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_only
specifier|static
name|int
name|show_only
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|prune_object
specifier|static
name|int
name|prune_object
parameter_list|(
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|filename
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
if|if
condition|(
name|show_only
condition|)
block|{
name|enum
name|object_type
name|type
init|=
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|)
decl_stmt|;
name|printf
argument_list|(
literal|"%s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
operator|(
name|type
operator|>
literal|0
operator|)
condition|?
name|typename
argument_list|(
name|type
argument_list|)
else|:
literal|"unknown"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|unlink
argument_list|(
name|mkpath
argument_list|(
literal|"%s/%s"
argument_list|,
name|path
argument_list|,
name|filename
argument_list|)
argument_list|)
expr_stmt|;
name|rmdir
argument_list|(
name|path
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|prune_dir
specifier|static
name|int
name|prune_dir
parameter_list|(
name|int
name|i
parameter_list|,
name|char
modifier|*
name|path
parameter_list|)
block|{
name|DIR
modifier|*
name|dir
init|=
name|opendir
argument_list|(
name|path
argument_list|)
decl_stmt|;
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
if|if
condition|(
operator|!
name|dir
condition|)
return|return
literal|0
return|;
while|while
condition|(
operator|(
name|de
operator|=
name|readdir
argument_list|(
name|dir
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|char
name|name
index|[
literal|100
index|]
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|de
operator|->
name|d_name
argument_list|)
decl_stmt|;
switch|switch
condition|(
name|len
condition|)
block|{
case|case
literal|2
case|:
if|if
condition|(
name|de
operator|->
name|d_name
index|[
literal|1
index|]
operator|!=
literal|'.'
condition|)
break|break;
case|case
literal|1
case|:
if|if
condition|(
name|de
operator|->
name|d_name
index|[
literal|0
index|]
operator|!=
literal|'.'
condition|)
break|break;
continue|continue;
case|case
literal|38
case|:
name|sprintf
argument_list|(
name|name
argument_list|,
literal|"%02x"
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|name
operator|+
literal|2
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|name
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
break|break;
comment|/* 			 * Do we know about this object? 			 * It must have been reachable 			 */
if|if
condition|(
name|lookup_object
argument_list|(
name|sha1
argument_list|)
condition|)
continue|continue;
name|prune_object
argument_list|(
name|path
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"bad sha1 file: %s/%s\n"
argument_list|,
name|path
argument_list|,
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
block|}
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|prune_object_dir
specifier|static
name|void
name|prune_object_dir
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|256
condition|;
name|i
operator|++
control|)
block|{
specifier|static
name|char
name|dir
index|[
literal|4096
index|]
decl_stmt|;
name|sprintf
argument_list|(
name|dir
argument_list|,
literal|"%s/%02x"
argument_list|,
name|path
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|prune_dir
argument_list|(
name|i
argument_list|,
name|dir
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|cmd_prune
name|int
name|cmd_prune
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
name|i
decl_stmt|;
name|struct
name|rev_info
name|revs
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-n"
argument_list|)
condition|)
block|{
name|show_only
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|prune_usage
argument_list|)
expr_stmt|;
block|}
name|save_commit_buffer
operator|=
literal|0
expr_stmt|;
name|init_revisions
argument_list|(
operator|&
name|revs
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
name|mark_reachable_objects
argument_list|(
operator|&
name|revs
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|prune_object_dir
argument_list|(
name|get_object_directory
argument_list|()
argument_list|)
expr_stmt|;
name|sync
argument_list|()
expr_stmt|;
name|prune_packed_objects
argument_list|(
name|show_only
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

