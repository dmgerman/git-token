begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_decl_stmt
DECL|variable|line_termination
name|int
name|line_termination
init|=
literal|'\n'
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|recursive
name|int
name|recursive
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|path_prefix
struct|struct
name|path_prefix
block|{
DECL|member|prev
name|struct
name|path_prefix
modifier|*
name|prev
decl_stmt|;
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|print_path_prefix
specifier|static
name|void
name|print_path_prefix
parameter_list|(
name|struct
name|path_prefix
modifier|*
name|prefix
parameter_list|)
block|{
if|if
condition|(
name|prefix
condition|)
block|{
if|if
condition|(
name|prefix
operator|->
name|prev
condition|)
name|print_path_prefix
argument_list|(
name|prefix
operator|->
name|prev
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
name|prefix
operator|->
name|name
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
literal|'/'
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|list_recursive
specifier|static
name|void
name|list_recursive
parameter_list|(
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|char
modifier|*
name|type
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|struct
name|path_prefix
modifier|*
name|prefix
parameter_list|)
block|{
name|struct
name|path_prefix
name|this_prefix
decl_stmt|;
name|this_prefix
operator|.
name|prev
operator|=
name|prefix
expr_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tree"
argument_list|)
condition|)
name|die
argument_list|(
literal|"expected a 'tree' node"
argument_list|)
expr_stmt|;
while|while
condition|(
name|size
condition|)
block|{
name|int
name|namelen
init|=
name|strlen
argument_list|(
name|buffer
argument_list|)
operator|+
literal|1
decl_stmt|;
name|void
modifier|*
name|eltbuf
decl_stmt|;
name|char
name|elttype
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|long
name|eltsize
decl_stmt|;
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|buffer
operator|+
name|namelen
decl_stmt|;
name|char
modifier|*
name|path
init|=
name|strchr
argument_list|(
name|buffer
argument_list|,
literal|' '
argument_list|)
operator|+
literal|1
decl_stmt|;
name|unsigned
name|int
name|mode
decl_stmt|;
if|if
condition|(
name|size
operator|<
name|namelen
operator|+
literal|20
operator|||
name|sscanf
argument_list|(
name|buffer
argument_list|,
literal|"%o"
argument_list|,
operator|&
name|mode
argument_list|)
operator|!=
literal|1
condition|)
name|die
argument_list|(
literal|"corrupt 'tree' file"
argument_list|)
expr_stmt|;
name|buffer
operator|=
name|sha1
operator|+
literal|20
expr_stmt|;
name|size
operator|-=
name|namelen
operator|+
literal|20
expr_stmt|;
comment|/* XXX: We do some ugly mode heuristics here. 		 * It seems not worth it to read each file just to get this 		 * and the file size. -- pasky@ucw.cz 		 * ... that is, when we are not recursive -- junkio@cox.net 		 */
name|eltbuf
operator|=
operator|(
name|recursive
condition|?
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
name|elttype
argument_list|,
operator|&
name|eltsize
argument_list|)
else|:
name|NULL
operator|)
expr_stmt|;
if|if
condition|(
operator|!
name|eltbuf
condition|)
block|{
if|if
condition|(
name|recursive
condition|)
name|error
argument_list|(
literal|"cannot read %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|type
operator|=
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|?
literal|"tree"
else|:
literal|"blob"
expr_stmt|;
block|}
else|else
name|type
operator|=
name|elttype
expr_stmt|;
name|printf
argument_list|(
literal|"%03o\t%s\t%s\t"
argument_list|,
name|mode
argument_list|,
name|type
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|print_path_prefix
argument_list|(
name|prefix
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
name|path
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
name|line_termination
argument_list|)
expr_stmt|;
if|if
condition|(
name|eltbuf
operator|&&
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tree"
argument_list|)
condition|)
block|{
name|this_prefix
operator|.
name|name
operator|=
name|path
expr_stmt|;
name|list_recursive
argument_list|(
name|eltbuf
argument_list|,
name|elttype
argument_list|,
name|eltsize
argument_list|,
operator|&
name|this_prefix
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|eltbuf
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|list
specifier|static
name|int
name|list
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|void
modifier|*
name|buffer
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
name|buffer
operator|=
name|read_sha1_file
argument_list|(
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
operator|!
name|buffer
condition|)
name|die
argument_list|(
literal|"unable to read sha1 file"
argument_list|)
expr_stmt|;
name|list_recursive
argument_list|(
name|buffer
argument_list|,
name|type
argument_list|,
name|size
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|_usage
specifier|static
name|void
name|_usage
parameter_list|(
name|void
parameter_list|)
block|{
name|usage
argument_list|(
literal|"ls-tree [-r] [-z]<key>"
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
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
while|while
condition|(
literal|1
operator|<
name|argc
operator|&&
name|argv
index|[
literal|1
index|]
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
switch|switch
condition|(
name|argv
index|[
literal|1
index|]
index|[
literal|1
index|]
condition|)
block|{
case|case
literal|'z'
case|:
name|line_termination
operator|=
literal|0
expr_stmt|;
break|break;
case|case
literal|'r'
case|:
name|recursive
operator|=
literal|1
expr_stmt|;
break|break;
default|default:
name|_usage
argument_list|()
expr_stmt|;
block|}
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|!=
literal|2
condition|)
name|_usage
argument_list|()
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
name|_usage
argument_list|()
expr_stmt|;
name|sha1_file_directory
operator|=
name|getenv
argument_list|(
name|DB_ENVIRONMENT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|sha1_file_directory
condition|)
name|sha1_file_directory
operator|=
name|DEFAULT_DB_ENVIRONMENT
expr_stmt|;
if|if
condition|(
name|list
argument_list|(
name|sha1
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"list failed"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

