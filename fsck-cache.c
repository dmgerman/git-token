begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|<sys/types.h>
end_include

begin_include
include|#
directive|include
file|<dirent.h>
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_decl_stmt
DECL|variable|show_unreachable
specifier|static
name|int
name|show_unreachable
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|head_sha1
specifier|static
name|unsigned
name|char
name|head_sha1
index|[
literal|20
index|]
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|check_connectivity
specifier|static
name|void
name|check_connectivity
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
comment|/* Look up all the requirements, warn about missing objects.. */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr_revs
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|revision
modifier|*
name|rev
init|=
name|revs
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|show_unreachable
operator|&&
operator|!
operator|(
name|rev
operator|->
name|flags
operator|&
name|REACHABLE
operator|)
condition|)
block|{
name|printf
argument_list|(
literal|"unreachable %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|rev
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
continue|continue;
block|}
switch|switch
condition|(
name|rev
operator|->
name|flags
operator|&
operator|(
name|SEEN
operator||
name|USED
operator|)
condition|)
block|{
case|case
literal|0
case|:
name|printf
argument_list|(
literal|"bad %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|rev
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
break|break;
case|case
name|USED
case|:
name|printf
argument_list|(
literal|"missing %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|rev
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
break|break;
case|case
name|SEEN
case|:
name|printf
argument_list|(
literal|"dangling %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|rev
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
block|}
end_function

begin_function
DECL|function|mark_needs_sha1
specifier|static
name|void
name|mark_needs_sha1
parameter_list|(
name|unsigned
name|char
modifier|*
name|parent
parameter_list|,
specifier|const
name|char
modifier|*
name|tag
parameter_list|,
name|unsigned
name|char
modifier|*
name|child
parameter_list|)
block|{
name|struct
name|revision
modifier|*
name|child_rev
init|=
name|add_relationship
argument_list|(
name|lookup_rev
argument_list|(
name|parent
argument_list|)
argument_list|,
name|child
argument_list|)
decl_stmt|;
name|child_rev
operator|->
name|flags
operator||=
name|USED
expr_stmt|;
block|}
end_function

begin_function
DECL|function|mark_sha1_seen
specifier|static
name|int
name|mark_sha1_seen
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|char
modifier|*
name|tag
parameter_list|)
block|{
name|struct
name|revision
modifier|*
name|rev
init|=
name|lookup_rev
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|rev
operator|->
name|flags
operator||=
name|SEEN
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fsck_tree
specifier|static
name|int
name|fsck_tree
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|int
name|warn_old_tree
init|=
literal|1
decl_stmt|;
while|while
condition|(
name|size
condition|)
block|{
name|int
name|len
init|=
literal|1
operator|+
name|strlen
argument_list|(
name|data
argument_list|)
decl_stmt|;
name|unsigned
name|char
modifier|*
name|file_sha1
init|=
name|data
operator|+
name|len
decl_stmt|;
name|char
modifier|*
name|path
init|=
name|strchr
argument_list|(
name|data
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
name|size
operator|<
name|len
operator|+
literal|20
operator|||
operator|!
name|path
operator|||
name|sscanf
argument_list|(
name|data
argument_list|,
literal|"%o"
argument_list|,
operator|&
name|mode
argument_list|)
operator|!=
literal|1
condition|)
return|return
operator|-
literal|1
return|;
comment|/* Warn about trees that don't do the recursive thing.. */
if|if
condition|(
name|warn_old_tree
operator|&&
name|strchr
argument_list|(
name|path
argument_list|,
literal|'/'
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"warning: fsck-cache: tree %s has full pathnames in it\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|warn_old_tree
operator|=
literal|0
expr_stmt|;
block|}
name|data
operator|+=
name|len
operator|+
literal|20
expr_stmt|;
name|size
operator|-=
name|len
operator|+
literal|20
expr_stmt|;
name|mark_needs_sha1
argument_list|(
name|sha1
argument_list|,
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|?
literal|"tree"
else|:
literal|"blob"
argument_list|,
name|file_sha1
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fsck_commit
specifier|static
name|int
name|fsck_commit
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|int
name|parents
decl_stmt|;
name|unsigned
name|char
name|tree_sha1
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|char
name|parent_sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|memcmp
argument_list|(
name|data
argument_list|,
literal|"tree "
argument_list|,
literal|5
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|data
operator|+
literal|5
argument_list|,
name|tree_sha1
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
name|mark_needs_sha1
argument_list|(
name|sha1
argument_list|,
literal|"tree"
argument_list|,
name|tree_sha1
argument_list|)
expr_stmt|;
name|data
operator|+=
literal|5
operator|+
literal|40
operator|+
literal|1
expr_stmt|;
comment|/* "tree " +<hex sha1> + '\n' */
name|parents
operator|=
literal|0
expr_stmt|;
while|while
condition|(
operator|!
name|memcmp
argument_list|(
name|data
argument_list|,
literal|"parent "
argument_list|,
literal|7
argument_list|)
condition|)
block|{
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|data
operator|+
literal|7
argument_list|,
name|parent_sha1
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
name|mark_needs_sha1
argument_list|(
name|sha1
argument_list|,
literal|"commit"
argument_list|,
name|parent_sha1
argument_list|)
expr_stmt|;
name|data
operator|+=
literal|7
operator|+
literal|40
operator|+
literal|1
expr_stmt|;
comment|/* "parent " +<hex sha1> + '\n' */
name|parents
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|parents
condition|)
name|printf
argument_list|(
literal|"root %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fsck_entry
specifier|static
name|int
name|fsck_entry
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|char
modifier|*
name|tag
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|tag
argument_list|,
literal|"blob"
argument_list|)
condition|)
block|{
comment|/* Nothing to check */
empty_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|tag
argument_list|,
literal|"tree"
argument_list|)
condition|)
block|{
if|if
condition|(
name|fsck_tree
argument_list|(
name|sha1
argument_list|,
name|data
argument_list|,
name|size
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|tag
argument_list|,
literal|"commit"
argument_list|)
condition|)
block|{
if|if
condition|(
name|fsck_commit
argument_list|(
name|sha1
argument_list|,
name|data
argument_list|,
name|size
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
block|}
else|else
return|return
operator|-
literal|1
return|;
return|return
name|mark_sha1_seen
argument_list|(
name|sha1
argument_list|,
name|tag
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|fsck_name
specifier|static
name|int
name|fsck_name
parameter_list|(
name|char
modifier|*
name|hex
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
operator|!
name|get_sha1_hex
argument_list|(
name|hex
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|unsigned
name|long
name|mapsize
decl_stmt|;
name|void
modifier|*
name|map
init|=
name|map_sha1_file
argument_list|(
name|sha1
argument_list|,
operator|&
name|mapsize
argument_list|)
decl_stmt|;
if|if
condition|(
name|map
condition|)
block|{
name|char
name|type
index|[
literal|100
index|]
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|void
modifier|*
name|buffer
init|=
name|NULL
decl_stmt|;
if|if
condition|(
operator|!
name|check_sha1_signature
argument_list|(
name|sha1
argument_list|,
name|map
argument_list|,
name|mapsize
argument_list|)
condition|)
name|buffer
operator|=
name|unpack_sha1_file
argument_list|(
name|map
argument_list|,
name|mapsize
argument_list|,
name|type
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
name|munmap
argument_list|(
name|map
argument_list|,
name|mapsize
argument_list|)
expr_stmt|;
if|if
condition|(
name|buffer
operator|&&
operator|!
name|fsck_entry
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
condition|)
return|return
literal|0
return|;
block|}
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|fsck_dir
specifier|static
name|int
name|fsck_dir
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
block|{
return|return
name|error
argument_list|(
literal|"missing sha1 directory '%s'"
argument_list|,
name|path
argument_list|)
return|;
block|}
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
operator|!
name|fsck_name
argument_list|(
name|name
argument_list|)
condition|)
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
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|heads
decl_stmt|;
name|char
modifier|*
name|sha1_dir
decl_stmt|;
name|sha1_dir
operator|=
name|getenv
argument_list|(
name|DB_ENVIRONMENT
argument_list|)
condition|?
else|:
name|DEFAULT_DB_ENVIRONMENT
expr_stmt|;
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
name|sha1_dir
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|fsck_dir
argument_list|(
name|i
argument_list|,
name|dir
argument_list|)
expr_stmt|;
block|}
name|heads
operator|=
literal|0
expr_stmt|;
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
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--unreachable"
argument_list|)
condition|)
block|{
name|show_unreachable
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|get_sha1_hex
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
name|head_sha1
argument_list|)
condition|)
block|{
name|mark_reachable
argument_list|(
name|lookup_rev
argument_list|(
name|head_sha1
argument_list|)
argument_list|,
name|REACHABLE
argument_list|)
expr_stmt|;
name|heads
operator|++
expr_stmt|;
continue|continue;
block|}
name|error
argument_list|(
literal|"fsck-cache [[--unreachable]<head-sha1>*]"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|heads
condition|)
block|{
if|if
condition|(
name|show_unreachable
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"unable to do reachability without a head\n"
argument_list|)
expr_stmt|;
name|show_unreachable
operator|=
literal|0
expr_stmt|;
block|}
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"expect dangling commits - potential heads - due to lack of head information\n"
argument_list|)
expr_stmt|;
block|}
name|check_connectivity
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

