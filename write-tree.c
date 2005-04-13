begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_function
DECL|function|check_valid_sha1
specifier|static
name|int
name|check_valid_sha1
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|char
modifier|*
name|filename
init|=
name|sha1_file_name
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|int
name|ret
decl_stmt|;
comment|/* If we were anal, we'd check that the sha1 of the contents actually matches */
name|ret
operator|=
name|access
argument_list|(
name|filename
argument_list|,
name|R_OK
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
name|perror
argument_list|(
name|filename
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|prepend_integer
specifier|static
name|int
name|prepend_integer
parameter_list|(
name|char
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|val
parameter_list|,
name|int
name|i
parameter_list|)
block|{
name|buffer
index|[
operator|--
name|i
index|]
operator|=
literal|'\0'
expr_stmt|;
do|do
block|{
name|buffer
index|[
operator|--
name|i
index|]
operator|=
literal|'0'
operator|+
operator|(
name|val
operator|%
literal|10
operator|)
expr_stmt|;
name|val
operator|/=
literal|10
expr_stmt|;
block|}
do|while
condition|(
name|val
condition|)
do|;
return|return
name|i
return|;
block|}
end_function

begin_define
DECL|macro|ORIG_OFFSET
define|#
directive|define
name|ORIG_OFFSET
value|(40)
end_define

begin_comment
DECL|macro|ORIG_OFFSET
comment|/* Enough space to add the header of "tree<size>\0" */
end_comment

begin_function
DECL|function|write_tree
specifier|static
name|int
name|write_tree
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|cachep
parameter_list|,
name|int
name|maxentries
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
name|unsigned
name|char
modifier|*
name|returnsha1
parameter_list|)
block|{
name|unsigned
name|char
name|subdir_sha1
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|,
name|offset
decl_stmt|;
name|char
modifier|*
name|buffer
decl_stmt|;
name|int
name|i
decl_stmt|,
name|nr
decl_stmt|;
comment|/* Guess at some random initial size */
name|size
operator|=
literal|8192
expr_stmt|;
name|buffer
operator|=
name|malloc
argument_list|(
name|size
argument_list|)
expr_stmt|;
name|offset
operator|=
name|ORIG_OFFSET
expr_stmt|;
name|nr
operator|=
literal|0
expr_stmt|;
do|do
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|cachep
index|[
name|nr
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|pathname
init|=
name|ce
operator|->
name|name
decl_stmt|,
modifier|*
name|filename
decl_stmt|,
modifier|*
name|dirname
decl_stmt|;
name|int
name|pathlen
init|=
name|ce
operator|->
name|namelen
decl_stmt|,
name|entrylen
decl_stmt|;
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|unsigned
name|int
name|mode
decl_stmt|;
comment|/* Did we hit the end of the directory? Return how many we wrote */
if|if
condition|(
name|baselen
operator|>=
name|pathlen
operator|||
name|memcmp
argument_list|(
name|base
argument_list|,
name|pathname
argument_list|,
name|baselen
argument_list|)
condition|)
break|break;
name|sha1
operator|=
name|ce
operator|->
name|sha1
expr_stmt|;
name|mode
operator|=
name|ce
operator|->
name|st_mode
expr_stmt|;
comment|/* Do we have _further_ subdirectories? */
name|filename
operator|=
name|pathname
operator|+
name|baselen
expr_stmt|;
name|dirname
operator|=
name|strchr
argument_list|(
name|filename
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
name|dirname
condition|)
block|{
name|int
name|subdir_written
decl_stmt|;
name|subdir_written
operator|=
name|write_tree
argument_list|(
name|cachep
operator|+
name|nr
argument_list|,
name|maxentries
operator|-
name|nr
argument_list|,
name|pathname
argument_list|,
name|dirname
operator|-
name|pathname
operator|+
literal|1
argument_list|,
name|subdir_sha1
argument_list|)
expr_stmt|;
name|nr
operator|+=
name|subdir_written
expr_stmt|;
comment|/* Now we need to write out the directory entry into this tree.. */
name|mode
operator|=
name|S_IFDIR
expr_stmt|;
name|pathlen
operator|=
name|dirname
operator|-
name|pathname
expr_stmt|;
comment|/* ..but the directory entry doesn't count towards the total count */
name|nr
operator|--
expr_stmt|;
name|sha1
operator|=
name|subdir_sha1
expr_stmt|;
block|}
if|if
condition|(
name|check_valid_sha1
argument_list|(
name|sha1
argument_list|)
operator|<
literal|0
condition|)
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|entrylen
operator|=
name|pathlen
operator|-
name|baselen
expr_stmt|;
if|if
condition|(
name|offset
operator|+
name|entrylen
operator|+
literal|100
operator|>
name|size
condition|)
block|{
name|size
operator|=
name|alloc_nr
argument_list|(
name|offset
operator|+
name|entrylen
operator|+
literal|100
argument_list|)
expr_stmt|;
name|buffer
operator|=
name|realloc
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
name|offset
operator|+=
name|sprintf
argument_list|(
name|buffer
operator|+
name|offset
argument_list|,
literal|"%o %.*s"
argument_list|,
name|mode
argument_list|,
name|entrylen
argument_list|,
name|filename
argument_list|)
expr_stmt|;
name|buffer
index|[
name|offset
operator|++
index|]
operator|=
literal|0
expr_stmt|;
name|memcpy
argument_list|(
name|buffer
operator|+
name|offset
argument_list|,
name|sha1
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|offset
operator|+=
literal|20
expr_stmt|;
name|nr
operator|++
expr_stmt|;
block|}
do|while
condition|(
name|nr
operator|<
name|maxentries
condition|)
do|;
name|i
operator|=
name|prepend_integer
argument_list|(
name|buffer
argument_list|,
name|offset
operator|-
name|ORIG_OFFSET
argument_list|,
name|ORIG_OFFSET
argument_list|)
expr_stmt|;
name|i
operator|-=
literal|5
expr_stmt|;
name|memcpy
argument_list|(
name|buffer
operator|+
name|i
argument_list|,
literal|"tree "
argument_list|,
literal|5
argument_list|)
expr_stmt|;
name|buffer
operator|+=
name|i
expr_stmt|;
name|offset
operator|-=
name|i
expr_stmt|;
name|write_sha1_file
argument_list|(
name|buffer
argument_list|,
name|offset
argument_list|,
name|returnsha1
argument_list|)
expr_stmt|;
return|return
name|nr
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
name|entries
init|=
name|read_cache
argument_list|()
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
if|if
condition|(
name|entries
operator|<=
literal|0
condition|)
name|die
argument_list|(
literal|"write-tree: no cache contents to write"
argument_list|)
expr_stmt|;
if|if
condition|(
name|write_tree
argument_list|(
name|active_cache
argument_list|,
name|entries
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|,
name|sha1
argument_list|)
operator|!=
name|entries
condition|)
name|die
argument_list|(
literal|"write-tree: internal error"
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s\n"
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

end_unit

