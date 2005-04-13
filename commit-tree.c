begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|<pwd.h>
end_include

begin_include
include|#
directive|include
file|<time.h>
end_include

begin_define
DECL|macro|BLOCKING
define|#
directive|define
name|BLOCKING
value|(1ul<< 14)
end_define

begin_define
DECL|macro|ORIG_OFFSET
define|#
directive|define
name|ORIG_OFFSET
value|(40)
end_define

begin_comment
comment|/*  * Leave space at the beginning to insert the tag  * once we know how big things are.  *  * FIXME! Share the code with "write-tree.c"  */
end_comment

begin_function
DECL|function|init_buffer
specifier|static
name|void
name|init_buffer
parameter_list|(
name|char
modifier|*
modifier|*
name|bufp
parameter_list|,
name|unsigned
name|int
modifier|*
name|sizep
parameter_list|)
block|{
name|char
modifier|*
name|buf
init|=
name|malloc
argument_list|(
name|BLOCKING
argument_list|)
decl_stmt|;
name|memset
argument_list|(
name|buf
argument_list|,
literal|0
argument_list|,
name|ORIG_OFFSET
argument_list|)
expr_stmt|;
operator|*
name|sizep
operator|=
name|ORIG_OFFSET
expr_stmt|;
operator|*
name|bufp
operator|=
name|buf
expr_stmt|;
block|}
end_function

begin_function
DECL|function|add_buffer
specifier|static
name|void
name|add_buffer
parameter_list|(
name|char
modifier|*
modifier|*
name|bufp
parameter_list|,
name|unsigned
name|int
modifier|*
name|sizep
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|char
name|one_line
index|[
literal|2048
index|]
decl_stmt|;
name|va_list
name|args
decl_stmt|;
name|int
name|len
decl_stmt|;
name|unsigned
name|long
name|alloc
decl_stmt|,
name|size
decl_stmt|,
name|newsize
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|len
operator|=
name|vsnprintf
argument_list|(
name|one_line
argument_list|,
sizeof|sizeof
argument_list|(
name|one_line
argument_list|)
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
name|size
operator|=
operator|*
name|sizep
expr_stmt|;
name|newsize
operator|=
name|size
operator|+
name|len
expr_stmt|;
name|alloc
operator|=
operator|(
name|size
operator|+
literal|32767
operator|)
operator|&
operator|~
literal|32767
expr_stmt|;
name|buf
operator|=
operator|*
name|bufp
expr_stmt|;
if|if
condition|(
name|newsize
operator|>
name|alloc
condition|)
block|{
name|alloc
operator|=
operator|(
name|newsize
operator|+
literal|32767
operator|)
operator|&
operator|~
literal|32767
expr_stmt|;
name|buf
operator|=
name|realloc
argument_list|(
name|buf
argument_list|,
name|alloc
argument_list|)
expr_stmt|;
operator|*
name|bufp
operator|=
name|buf
expr_stmt|;
block|}
operator|*
name|sizep
operator|=
name|newsize
expr_stmt|;
name|memcpy
argument_list|(
name|buf
operator|+
name|size
argument_list|,
name|one_line
argument_list|,
name|len
argument_list|)
expr_stmt|;
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

begin_function
DECL|function|finish_buffer
specifier|static
name|void
name|finish_buffer
parameter_list|(
name|char
modifier|*
name|tag
parameter_list|,
name|char
modifier|*
modifier|*
name|bufp
parameter_list|,
name|unsigned
name|int
modifier|*
name|sizep
parameter_list|)
block|{
name|int
name|taglen
decl_stmt|;
name|int
name|offset
decl_stmt|;
name|char
modifier|*
name|buf
init|=
operator|*
name|bufp
decl_stmt|;
name|unsigned
name|int
name|size
init|=
operator|*
name|sizep
decl_stmt|;
name|offset
operator|=
name|prepend_integer
argument_list|(
name|buf
argument_list|,
name|size
operator|-
name|ORIG_OFFSET
argument_list|,
name|ORIG_OFFSET
argument_list|)
expr_stmt|;
name|taglen
operator|=
name|strlen
argument_list|(
name|tag
argument_list|)
expr_stmt|;
name|offset
operator|-=
name|taglen
expr_stmt|;
name|buf
operator|+=
name|offset
expr_stmt|;
name|size
operator|-=
name|offset
expr_stmt|;
name|memcpy
argument_list|(
name|buf
argument_list|,
name|tag
argument_list|,
name|taglen
argument_list|)
expr_stmt|;
operator|*
name|bufp
operator|=
name|buf
expr_stmt|;
operator|*
name|sizep
operator|=
name|size
expr_stmt|;
block|}
end_function

begin_function
DECL|function|remove_special
specifier|static
name|void
name|remove_special
parameter_list|(
name|char
modifier|*
name|p
parameter_list|)
block|{
name|char
name|c
decl_stmt|;
name|char
modifier|*
name|dst
init|=
name|p
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|c
operator|=
operator|*
name|p
expr_stmt|;
name|p
operator|++
expr_stmt|;
switch|switch
condition|(
name|c
condition|)
block|{
case|case
literal|'\n'
case|:
case|case
literal|'<'
case|:
case|case
literal|'>'
case|:
continue|continue;
block|}
operator|*
name|dst
operator|++
operator|=
name|c
expr_stmt|;
if|if
condition|(
operator|!
name|c
condition|)
break|break;
block|}
block|}
end_function

begin_comment
comment|/*  * Having more than two parents may be strange, but hey, there's  * no conceptual reason why the file format couldn't accept multi-way  * merges. It might be the "union" of several packages, for example.  *  * I don't really expect that to happen, but this is here to make  * it clear that _conceptually_ it's ok..  */
end_comment

begin_define
DECL|macro|MAXPARENT
define|#
directive|define
name|MAXPARENT
value|(16)
end_define

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
name|len
decl_stmt|;
name|int
name|parents
init|=
literal|0
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
name|MAXPARENT
index|]
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|char
name|commit_sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|gecos
decl_stmt|,
modifier|*
name|realgecos
decl_stmt|;
name|char
modifier|*
name|email
decl_stmt|,
name|realemail
index|[
literal|1000
index|]
decl_stmt|;
name|char
modifier|*
name|date
decl_stmt|,
modifier|*
name|realdate
decl_stmt|;
name|char
name|comment
index|[
literal|1000
index|]
decl_stmt|;
name|struct
name|passwd
modifier|*
name|pw
decl_stmt|;
name|time_t
name|now
decl_stmt|;
name|char
modifier|*
name|buffer
decl_stmt|;
name|unsigned
name|int
name|size
decl_stmt|;
if|if
condition|(
name|argc
operator|<
literal|2
operator|||
name|get_sha1_hex
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|tree_sha1
argument_list|)
operator|<
literal|0
condition|)
name|usage
argument_list|(
literal|"commit-tree<sha1> [-p<sha1>]*< changelog"
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|2
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|+=
literal|2
control|)
block|{
name|char
modifier|*
name|a
decl_stmt|,
modifier|*
name|b
decl_stmt|;
name|a
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
name|b
operator|=
name|argv
index|[
name|i
operator|+
literal|1
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|b
operator|||
name|strcmp
argument_list|(
name|a
argument_list|,
literal|"-p"
argument_list|)
operator|||
name|get_sha1_hex
argument_list|(
name|b
argument_list|,
name|parent_sha1
index|[
name|parents
index|]
argument_list|)
condition|)
name|usage
argument_list|(
literal|"commit-tree<sha1> [-p<sha1>]*< changelog"
argument_list|)
expr_stmt|;
name|parents
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|parents
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Committing initial tree %s\n"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|pw
operator|=
name|getpwuid
argument_list|(
name|getuid
argument_list|()
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|pw
condition|)
name|die
argument_list|(
literal|"You don't exist. Go away!"
argument_list|)
expr_stmt|;
name|realgecos
operator|=
name|pw
operator|->
name|pw_gecos
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|pw
operator|->
name|pw_name
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|realemail
argument_list|,
name|pw
operator|->
name|pw_name
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|realemail
index|[
name|len
index|]
operator|=
literal|'@'
expr_stmt|;
name|gethostname
argument_list|(
name|realemail
operator|+
name|len
operator|+
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
name|realemail
argument_list|)
operator|-
name|len
operator|-
literal|1
argument_list|)
expr_stmt|;
name|time
argument_list|(
operator|&
name|now
argument_list|)
expr_stmt|;
name|realdate
operator|=
name|ctime
argument_list|(
operator|&
name|now
argument_list|)
expr_stmt|;
name|gecos
operator|=
name|getenv
argument_list|(
literal|"AUTHOR_NAME"
argument_list|)
condition|?
else|:
name|realgecos
expr_stmt|;
name|email
operator|=
name|getenv
argument_list|(
literal|"AUTHOR_EMAIL"
argument_list|)
condition|?
else|:
name|realemail
expr_stmt|;
name|date
operator|=
name|getenv
argument_list|(
literal|"AUTHOR_DATE"
argument_list|)
condition|?
else|:
name|realdate
expr_stmt|;
name|remove_special
argument_list|(
name|gecos
argument_list|)
expr_stmt|;
name|remove_special
argument_list|(
name|realgecos
argument_list|)
expr_stmt|;
name|remove_special
argument_list|(
name|email
argument_list|)
expr_stmt|;
name|remove_special
argument_list|(
name|realemail
argument_list|)
expr_stmt|;
name|remove_special
argument_list|(
name|date
argument_list|)
expr_stmt|;
name|remove_special
argument_list|(
name|realdate
argument_list|)
expr_stmt|;
name|init_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"tree %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|tree_sha1
argument_list|)
argument_list|)
expr_stmt|;
comment|/* 	 * NOTE! This ordering means that the same exact tree merged with a 	 * different order of parents will be a _different_ changeset even 	 * if everything else stays the same. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|parents
condition|;
name|i
operator|++
control|)
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"parent %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|parent_sha1
index|[
name|i
index|]
argument_list|)
argument_list|)
expr_stmt|;
comment|/* Person/date information */
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"author %s<%s> %s\n"
argument_list|,
name|gecos
argument_list|,
name|email
argument_list|,
name|date
argument_list|)
expr_stmt|;
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"committer %s<%s> %s\n\n"
argument_list|,
name|realgecos
argument_list|,
name|realemail
argument_list|,
name|realdate
argument_list|)
expr_stmt|;
comment|/* And add the comment */
while|while
condition|(
name|fgets
argument_list|(
name|comment
argument_list|,
sizeof|sizeof
argument_list|(
name|comment
argument_list|)
argument_list|,
name|stdin
argument_list|)
operator|!=
name|NULL
condition|)
name|add_buffer
argument_list|(
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|,
literal|"%s"
argument_list|,
name|comment
argument_list|)
expr_stmt|;
name|finish_buffer
argument_list|(
literal|"commit "
argument_list|,
operator|&
name|buffer
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
name|write_sha1_file
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|,
name|commit_sha1
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|commit_sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

