begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_decl_stmt
DECL|variable|cached_only
specifier|static
name|int
name|cached_only
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|recursive
specifier|static
name|int
name|recursive
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|line_termination
specifier|static
name|int
name|line_termination
init|=
literal|'\n'
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|static
name|int
name|diff_cache
parameter_list|(
name|void
modifier|*
name|tree
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|ac
parameter_list|,
name|int
name|entries
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|update_tree_entry
specifier|static
name|void
name|update_tree_entry
parameter_list|(
name|void
modifier|*
modifier|*
name|bufp
parameter_list|,
name|unsigned
name|long
modifier|*
name|sizep
parameter_list|)
block|{
name|void
modifier|*
name|buf
init|=
operator|*
name|bufp
decl_stmt|;
name|unsigned
name|long
name|size
init|=
operator|*
name|sizep
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|buf
argument_list|)
operator|+
literal|1
operator|+
literal|20
decl_stmt|;
if|if
condition|(
name|size
operator|<
name|len
condition|)
name|die
argument_list|(
literal|"corrupt tree file 1 (%s)"
argument_list|,
name|size
argument_list|)
expr_stmt|;
operator|*
name|bufp
operator|=
name|buf
operator|+
name|len
expr_stmt|;
operator|*
name|sizep
operator|=
name|size
operator|-
name|len
expr_stmt|;
block|}
end_function

begin_function
DECL|function|extract
specifier|static
specifier|const
name|unsigned
name|char
modifier|*
name|extract
parameter_list|(
name|void
modifier|*
name|tree
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathp
parameter_list|,
name|unsigned
name|int
modifier|*
name|modep
parameter_list|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|tree
argument_list|)
operator|+
literal|1
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|tree
operator|+
name|len
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
init|=
name|strchr
argument_list|(
name|tree
argument_list|,
literal|' '
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|path
operator|||
name|size
operator|<
name|len
operator|+
literal|20
operator|||
name|sscanf
argument_list|(
name|tree
argument_list|,
literal|"%o"
argument_list|,
name|modep
argument_list|)
operator|!=
literal|1
condition|)
name|die
argument_list|(
literal|"corrupt tree file 2 (%d)"
argument_list|,
name|size
argument_list|)
expr_stmt|;
operator|*
name|pathp
operator|=
name|path
operator|+
literal|1
expr_stmt|;
return|return
name|sha1
return|;
block|}
end_function

begin_function
DECL|function|malloc_base
specifier|static
name|char
modifier|*
name|malloc_base
parameter_list|(
specifier|const
name|char
modifier|*
name|base
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|pathlen
parameter_list|)
block|{
name|int
name|baselen
init|=
name|strlen
argument_list|(
name|base
argument_list|)
decl_stmt|;
name|char
modifier|*
name|newbase
init|=
name|malloc
argument_list|(
name|baselen
operator|+
name|pathlen
operator|+
literal|2
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|newbase
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|newbase
operator|+
name|baselen
argument_list|,
name|path
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|newbase
operator|+
name|baselen
operator|+
name|pathlen
argument_list|,
literal|"/"
argument_list|,
literal|2
argument_list|)
expr_stmt|;
return|return
name|newbase
return|;
block|}
end_function

begin_function_decl
specifier|static
name|void
name|show_file
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|int
name|mode
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* A whole sub-tree went away or appeared */
end_comment

begin_function
DECL|function|show_tree
specifier|static
name|void
name|show_tree
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|void
modifier|*
name|tree
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
while|while
condition|(
name|size
condition|)
block|{
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
name|unsigned
name|int
name|mode
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|extract
argument_list|(
name|tree
argument_list|,
name|size
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|mode
argument_list|)
decl_stmt|;
name|show_file
argument_list|(
name|prefix
argument_list|,
name|path
argument_list|,
name|mode
argument_list|,
name|sha1
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
operator|&
name|tree
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/* A file entry went away or appeared */
end_comment

begin_function
DECL|function|show_file
specifier|static
name|void
name|show_file
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|int
name|mode
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
if|if
condition|(
name|recursive
operator|&&
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
block|{
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|char
modifier|*
name|newbase
init|=
name|malloc_base
argument_list|(
name|base
argument_list|,
name|path
argument_list|,
name|strlen
argument_list|(
name|path
argument_list|)
argument_list|)
decl_stmt|;
name|void
modifier|*
name|tree
decl_stmt|;
name|tree
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
name|tree
operator|||
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tree"
argument_list|)
condition|)
name|die
argument_list|(
literal|"corrupt tree sha %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|show_tree
argument_list|(
name|prefix
argument_list|,
name|tree
argument_list|,
name|size
argument_list|,
name|newbase
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|newbase
argument_list|)
expr_stmt|;
return|return;
block|}
name|printf
argument_list|(
literal|"%s%o\t%s\t%s\t%s%s%c"
argument_list|,
name|prefix
argument_list|,
name|mode
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
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|base
argument_list|,
name|path
argument_list|,
name|line_termination
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|compare_tree_entry
specifier|static
name|int
name|compare_tree_entry
parameter_list|(
specifier|const
name|char
modifier|*
name|path1
parameter_list|,
name|unsigned
name|int
name|mode1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|ac
parameter_list|,
name|int
modifier|*
name|entries
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
name|int
name|baselen
init|=
name|strlen
argument_list|(
name|base
argument_list|)
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
init|=
operator|*
name|ac
decl_stmt|;
specifier|const
name|char
modifier|*
name|path2
init|=
name|ce
operator|->
name|name
operator|+
name|baselen
decl_stmt|;
name|unsigned
name|int
name|mode2
init|=
name|ntohl
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha2
init|=
name|ce
operator|->
name|sha1
decl_stmt|;
name|int
name|cmp
decl_stmt|,
name|pathlen1
decl_stmt|,
name|pathlen2
decl_stmt|;
name|char
name|old_sha1_hex
index|[
literal|50
index|]
decl_stmt|;
name|pathlen1
operator|=
name|strlen
argument_list|(
name|path1
argument_list|)
expr_stmt|;
name|pathlen2
operator|=
name|strlen
argument_list|(
name|path2
argument_list|)
expr_stmt|;
name|cmp
operator|=
name|cache_name_compare
argument_list|(
name|path1
argument_list|,
name|pathlen1
argument_list|,
name|path2
argument_list|,
name|pathlen2
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode1
argument_list|)
condition|)
block|{
name|char
name|type
index|[
literal|20
index|]
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|void
modifier|*
name|tree
init|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
name|type
argument_list|,
operator|&
name|size
argument_list|)
decl_stmt|;
name|char
modifier|*
name|newbase
init|=
name|malloc
argument_list|(
name|baselen
operator|+
literal|2
operator|+
name|pathlen1
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|newbase
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|newbase
operator|+
name|baselen
argument_list|,
name|path1
argument_list|,
name|pathlen1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|newbase
operator|+
name|baselen
operator|+
name|pathlen1
argument_list|,
literal|"/"
argument_list|,
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree
operator|||
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"tree"
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to read tree object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
operator|*
name|entries
operator|=
name|diff_cache
argument_list|(
name|tree
argument_list|,
name|size
argument_list|,
name|ac
argument_list|,
operator|*
name|entries
argument_list|,
name|newbase
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|newbase
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|show_file
argument_list|(
literal|"-"
argument_list|,
name|path1
argument_list|,
name|mode1
argument_list|,
name|sha1
argument_list|,
name|base
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
operator|!
name|cached_only
condition|)
block|{
specifier|static
name|unsigned
name|char
name|no_sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|fd
decl_stmt|,
name|changed
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
operator|||
name|fstat
argument_list|(
name|fd
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
block|{
name|show_file
argument_list|(
literal|"-"
argument_list|,
name|path1
argument_list|,
name|mode1
argument_list|,
name|sha1
argument_list|,
name|base
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
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
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|changed
condition|)
block|{
name|mode2
operator|=
name|st
operator|.
name|st_mode
expr_stmt|;
name|sha2
operator|=
name|no_sha1
expr_stmt|;
block|}
block|}
if|if
condition|(
name|cmp
operator|>
literal|0
condition|)
block|{
name|show_file
argument_list|(
literal|"+"
argument_list|,
name|path2
argument_list|,
name|mode2
argument_list|,
name|sha2
argument_list|,
name|base
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|sha1
argument_list|,
name|sha2
argument_list|,
literal|20
argument_list|)
operator|&&
name|mode1
operator|==
name|mode2
condition|)
return|return
literal|0
return|;
comment|/* 	 * If the filemode has changed to/from a directory from/to a regular 	 * file, we need to consider it a remove and an add. 	 */
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode1
argument_list|)
operator|||
name|S_ISDIR
argument_list|(
name|mode2
argument_list|)
condition|)
block|{
name|show_file
argument_list|(
literal|"-"
argument_list|,
name|path1
argument_list|,
name|mode1
argument_list|,
name|sha1
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|show_file
argument_list|(
literal|"+"
argument_list|,
name|path2
argument_list|,
name|mode2
argument_list|,
name|sha2
argument_list|,
name|base
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|strcpy
argument_list|(
name|old_sha1_hex
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"*%o->%o\t%s\t%s->%s\t%s%s%c"
argument_list|,
name|mode1
argument_list|,
name|mode2
argument_list|,
name|S_ISDIR
argument_list|(
name|mode1
argument_list|)
condition|?
literal|"tree"
else|:
literal|"blob"
argument_list|,
name|old_sha1_hex
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha2
argument_list|)
argument_list|,
name|base
argument_list|,
name|path1
argument_list|,
name|line_termination
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|diff_cache
specifier|static
name|int
name|diff_cache
parameter_list|(
name|void
modifier|*
name|tree
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|ac
parameter_list|,
name|int
name|entries
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
name|int
name|baselen
init|=
name|strlen
argument_list|(
name|base
argument_list|)
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|;
name|unsigned
name|int
name|mode
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|int
name|left
decl_stmt|;
comment|/* 		 * No entries in the cache (with this base)? 		 * Output the tree contents. 		 */
if|if
condition|(
operator|!
name|entries
operator|||
name|ce_namelen
argument_list|(
name|ce
operator|=
operator|*
name|ac
argument_list|)
operator|<
name|baselen
operator|||
name|memcmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|size
condition|)
return|return
name|entries
return|;
name|sha1
operator|=
name|extract
argument_list|(
name|tree
argument_list|,
name|size
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|mode
argument_list|)
expr_stmt|;
name|show_file
argument_list|(
literal|"-"
argument_list|,
name|path
argument_list|,
name|mode
argument_list|,
name|sha1
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|update_tree_entry
argument_list|(
operator|&
name|tree
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
continue|continue;
block|}
comment|/* 		 * No entries in the tree? Output the cache contents 		 */
if|if
condition|(
operator|!
name|size
condition|)
block|{
name|show_file
argument_list|(
literal|"+"
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|ntohl
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
argument_list|,
name|ce
operator|->
name|sha1
argument_list|,
literal|""
argument_list|)
expr_stmt|;
name|ac
operator|++
expr_stmt|;
name|entries
operator|--
expr_stmt|;
continue|continue;
block|}
name|sha1
operator|=
name|extract
argument_list|(
name|tree
argument_list|,
name|size
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|mode
argument_list|)
expr_stmt|;
name|left
operator|=
name|entries
expr_stmt|;
switch|switch
condition|(
name|compare_tree_entry
argument_list|(
name|path
argument_list|,
name|mode
argument_list|,
name|sha1
argument_list|,
name|ac
argument_list|,
operator|&
name|left
argument_list|,
name|base
argument_list|)
condition|)
block|{
case|case
operator|-
literal|1
case|:
name|update_tree_entry
argument_list|(
operator|&
name|tree
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|left
operator|<
name|entries
condition|)
block|{
name|ac
operator|+=
operator|(
name|entries
operator|-
name|left
operator|)
expr_stmt|;
name|entries
operator|=
name|left
expr_stmt|;
block|}
continue|continue;
case|case
literal|0
case|:
name|update_tree_entry
argument_list|(
operator|&
name|tree
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
comment|/* Fallthrough */
case|case
literal|1
case|:
name|ac
operator|++
expr_stmt|;
name|entries
operator|--
expr_stmt|;
continue|continue;
block|}
name|die
argument_list|(
literal|"diff-cache: internal error"
argument_list|)
expr_stmt|;
block|}
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
name|unsigned
name|char
name|tree_sha1
index|[
literal|20
index|]
decl_stmt|;
name|void
modifier|*
name|tree
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
name|read_cache
argument_list|()
expr_stmt|;
while|while
condition|(
name|argc
operator|>
literal|2
condition|)
block|{
name|char
modifier|*
name|arg
init|=
name|argv
index|[
literal|1
index|]
decl_stmt|;
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-r"
argument_list|)
condition|)
block|{
name|recursive
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
name|arg
argument_list|,
literal|"-z"
argument_list|)
condition|)
block|{
name|line_termination
operator|=
literal|'\0'
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--cached"
argument_list|)
condition|)
block|{
name|cached_only
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
literal|"diff-cache [-r] [-z]<tree sha1>"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|!=
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
condition|)
name|usage
argument_list|(
literal|"diff-cache [-r] [-z]<tree sha1>"
argument_list|)
expr_stmt|;
name|tree
operator|=
name|read_sha1_file
argument_list|(
name|tree_sha1
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
name|tree
condition|)
name|die
argument_list|(
literal|"bad tree object %s"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
comment|/* We allow people to feed us a commit object, just because we're nice */
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|type
argument_list|,
literal|"commit"
argument_list|)
condition|)
block|{
comment|/* tree sha1 is always at offset 5 ("tree ") */
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|tree
operator|+
literal|5
argument_list|,
name|tree_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"bad commit object %s"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tree
argument_list|)
expr_stmt|;
name|tree
operator|=
name|read_sha1_file
argument_list|(
name|tree_sha1
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
name|tree
condition|)
name|die
argument_list|(
literal|"unable to read tree object %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|tree_sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
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
literal|"bad tree object %s (%s)"
argument_list|,
name|sha1_to_hex
argument_list|(
name|tree_sha1
argument_list|)
argument_list|,
name|type
argument_list|)
expr_stmt|;
return|return
name|diff_cache
argument_list|(
name|tree
argument_list|,
name|size
argument_list|,
name|active_cache
argument_list|,
name|active_nr
argument_list|,
literal|""
argument_list|)
return|;
block|}
end_function

end_unit

