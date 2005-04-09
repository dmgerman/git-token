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
DECL|function|show_differences
specifier|static
name|void
name|show_differences
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|struct
name|stat
modifier|*
name|cur
parameter_list|,
name|void
modifier|*
name|old_contents
parameter_list|,
name|unsigned
name|long
name|long
name|old_size
parameter_list|)
block|{
specifier|static
name|char
name|cmd
index|[
literal|1000
index|]
decl_stmt|;
name|FILE
modifier|*
name|f
decl_stmt|;
name|snprintf
argument_list|(
name|cmd
argument_list|,
sizeof|sizeof
argument_list|(
name|cmd
argument_list|)
argument_list|,
literal|"diff -u - %s"
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
name|f
operator|=
name|popen
argument_list|(
name|cmd
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
name|fwrite
argument_list|(
name|old_contents
argument_list|,
name|old_size
argument_list|,
literal|1
argument_list|,
name|f
argument_list|)
expr_stmt|;
name|pclose
argument_list|(
name|f
argument_list|)
expr_stmt|;
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
name|int
name|i
decl_stmt|;
if|if
condition|(
name|entries
operator|<
literal|0
condition|)
block|{
name|perror
argument_list|(
literal|"read_cache"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|entries
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|active_cache
index|[
name|i
index|]
decl_stmt|;
name|int
name|n
decl_stmt|,
name|changed
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|void
modifier|*
name|new
decl_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|ce
operator|->
name|name
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
block|{
name|printf
argument_list|(
literal|"%s: %s\n"
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|changed
operator|=
name|cache_match_stat
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|changed
condition|)
block|{
name|printf
argument_list|(
literal|"%s: ok\n"
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|printf
argument_list|(
literal|"%.*s:  "
argument_list|,
name|ce
operator|->
name|namelen
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
literal|20
condition|;
name|n
operator|++
control|)
name|printf
argument_list|(
literal|"%02x"
argument_list|,
name|ce
operator|->
name|sha1
index|[
name|n
index|]
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
name|new
operator|=
name|read_sha1_file
argument_list|(
name|ce
operator|->
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
name|show_differences
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|,
name|new
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|new
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

