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
name|char
modifier|*
name|name
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
literal|"diff -L %s -u -N  - %s"
argument_list|,
name|name
argument_list|,
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
if|if
condition|(
name|old_size
condition|)
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
DECL|function|show_diff_empty
specifier|static
name|void
name|show_diff_empty
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|)
block|{
name|char
modifier|*
name|old
decl_stmt|;
name|unsigned
name|long
name|int
name|size
decl_stmt|;
name|int
name|lines
init|=
literal|0
decl_stmt|;
name|unsigned
name|char
name|type
index|[
literal|20
index|]
decl_stmt|,
modifier|*
name|p
decl_stmt|,
modifier|*
name|end
decl_stmt|;
name|old
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
if|if
condition|(
name|size
operator|>
literal|0
condition|)
block|{
name|int
name|startline
init|=
literal|1
decl_stmt|;
name|int
name|c
init|=
literal|0
decl_stmt|;
name|printf
argument_list|(
literal|"--- %s\n"
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"+++ /dev/null\n"
argument_list|)
expr_stmt|;
name|p
operator|=
name|old
expr_stmt|;
name|end
operator|=
name|old
operator|+
name|size
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|end
condition|)
if|if
condition|(
operator|*
name|p
operator|++
operator|==
literal|'\n'
condition|)
name|lines
operator|++
expr_stmt|;
name|printf
argument_list|(
literal|"@@ -1,%d +0,0 @@\n"
argument_list|,
name|lines
argument_list|)
expr_stmt|;
name|p
operator|=
name|old
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|end
condition|)
block|{
name|c
operator|=
operator|*
name|p
operator|++
expr_stmt|;
if|if
condition|(
name|startline
condition|)
block|{
name|putchar
argument_list|(
literal|'-'
argument_list|)
expr_stmt|;
name|startline
operator|=
literal|0
expr_stmt|;
block|}
name|putchar
argument_list|(
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|==
literal|'\n'
condition|)
name|startline
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|c
operator|!=
literal|'\n'
condition|)
name|printf
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
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
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|int
name|silent
init|=
literal|0
decl_stmt|;
name|int
name|silent_on_nonexisting_files
init|=
literal|0
decl_stmt|;
name|int
name|entries
init|=
name|read_cache
argument_list|()
decl_stmt|;
name|int
name|i
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
literal|"-s"
argument_list|)
condition|)
block|{
name|silent_on_nonexisting_files
operator|=
name|silent
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
name|argv
index|[
name|i
index|]
argument_list|,
literal|"-q"
argument_list|)
condition|)
block|{
name|silent_on_nonexisting_files
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
literal|"show-diff [-s] [-q]"
argument_list|)
expr_stmt|;
block|}
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
if|if
condition|(
name|errno
operator|==
name|ENOENT
operator|&&
name|silent_on_nonexisting_files
condition|)
continue|continue;
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
if|if
condition|(
name|errno
operator|==
name|ENOENT
condition|)
name|show_diff_empty
argument_list|(
name|ce
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
continue|continue;
name|printf
argument_list|(
literal|"%.*s:  "
argument_list|,
name|ce_namelen
argument_list|(
name|ce
argument_list|)
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
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
name|silent
condition|)
continue|continue;
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
operator|->
name|name
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

