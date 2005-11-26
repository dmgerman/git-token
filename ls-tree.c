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
file|"blob.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_decl_stmt
DECL|variable|line_termination
specifier|static
name|int
name|line_termination
init|=
literal|'\n'
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|LS_RECURSIVE
define|#
directive|define
name|LS_RECURSIVE
value|1
end_define

begin_define
DECL|macro|LS_TREE_ONLY
define|#
directive|define
name|LS_TREE_ONLY
value|2
end_define

begin_decl_stmt
DECL|variable|ls_options
specifier|static
name|int
name|ls_options
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|root_entry
specifier|static
name|struct
name|tree_entry_list
name|root_entry
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|prepare_root
specifier|static
name|void
name|prepare_root
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|unsigned
name|char
name|rsha
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
name|buf
decl_stmt|;
name|struct
name|tree
modifier|*
name|root_tree
decl_stmt|;
name|buf
operator|=
name|read_object_with_reference
argument_list|(
name|sha1
argument_list|,
literal|"tree"
argument_list|,
operator|&
name|size
argument_list|,
name|rsha
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
name|die
argument_list|(
literal|"Could not read %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|root_tree
operator|=
name|lookup_tree
argument_list|(
name|rsha
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|root_tree
condition|)
name|die
argument_list|(
literal|"Could not read %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
comment|/* Prepare a fake entry */
name|root_entry
operator|.
name|directory
operator|=
literal|1
expr_stmt|;
name|root_entry
operator|.
name|executable
operator|=
name|root_entry
operator|.
name|symlink
operator|=
literal|0
expr_stmt|;
name|root_entry
operator|.
name|mode
operator|=
name|S_IFDIR
expr_stmt|;
name|root_entry
operator|.
name|name
operator|=
literal|""
expr_stmt|;
name|root_entry
operator|.
name|item
operator|.
name|tree
operator|=
name|root_tree
expr_stmt|;
name|root_entry
operator|.
name|parent
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function
DECL|function|prepare_children
specifier|static
name|int
name|prepare_children
parameter_list|(
name|struct
name|tree_entry_list
modifier|*
name|elem
parameter_list|)
block|{
if|if
condition|(
operator|!
name|elem
operator|->
name|directory
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|elem
operator|->
name|item
operator|.
name|tree
operator|->
name|object
operator|.
name|parsed
condition|)
block|{
name|struct
name|tree_entry_list
modifier|*
name|e
decl_stmt|;
if|if
condition|(
name|parse_tree
argument_list|(
name|elem
operator|->
name|item
operator|.
name|tree
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
comment|/* Set up the parent link */
for|for
control|(
name|e
operator|=
name|elem
operator|->
name|item
operator|.
name|tree
operator|->
name|entries
init|;
name|e
condition|;
name|e
operator|=
name|e
operator|->
name|next
control|)
name|e
operator|->
name|parent
operator|=
name|elem
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|find_entry
specifier|static
name|struct
name|tree_entry_list
modifier|*
name|find_entry
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|char
modifier|*
name|pathbuf
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|next
decl_stmt|,
modifier|*
name|slash
decl_stmt|;
name|int
name|len
decl_stmt|;
name|struct
name|tree_entry_list
modifier|*
name|elem
init|=
operator|&
name|root_entry
decl_stmt|,
modifier|*
name|oldelem
init|=
name|NULL
decl_stmt|;
operator|*
operator|(
name|pathbuf
operator|)
operator|=
literal|'\0'
expr_stmt|;
comment|/* Find tree element, descending from root, that 	 * corresponds to the named path, lazily expanding 	 * the tree if possible. 	 */
while|while
condition|(
name|path
condition|)
block|{
comment|/* The fact we still have path means that the caller 		 * wants us to make sure that elem at this point is a 		 * directory, and possibly descend into it.  Even what 		 * is left is just trailing slashes, we loop back to 		 * here, and this call to prepare_children() will 		 * catch elem not being a tree.  Nice. 		 */
if|if
condition|(
name|prepare_children
argument_list|(
name|elem
argument_list|)
condition|)
return|return
name|NULL
return|;
name|slash
operator|=
name|strchr
argument_list|(
name|path
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|slash
condition|)
block|{
name|len
operator|=
name|strlen
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|next
operator|=
name|NULL
expr_stmt|;
block|}
else|else
block|{
name|next
operator|=
name|slash
operator|+
literal|1
expr_stmt|;
name|len
operator|=
name|slash
operator|-
name|path
expr_stmt|;
block|}
if|if
condition|(
name|len
condition|)
block|{
if|if
condition|(
name|oldelem
condition|)
block|{
name|pathbuf
operator|+=
name|sprintf
argument_list|(
name|pathbuf
argument_list|,
literal|"%s/"
argument_list|,
name|oldelem
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
comment|/* (len == 0) if the original path was "drivers/char/" 			 * and we have run already two rounds, having elem 			 * pointing at the drivers/char directory. 			 */
name|elem
operator|=
name|elem
operator|->
name|item
operator|.
name|tree
operator|->
name|entries
expr_stmt|;
while|while
condition|(
name|elem
condition|)
block|{
if|if
condition|(
operator|(
name|strlen
argument_list|(
name|elem
operator|->
name|name
argument_list|)
operator|==
name|len
operator|)
operator|&&
operator|!
name|strncmp
argument_list|(
name|elem
operator|->
name|name
argument_list|,
name|path
argument_list|,
name|len
argument_list|)
condition|)
block|{
comment|/* found */
break|break;
block|}
name|elem
operator|=
name|elem
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|elem
condition|)
return|return
name|NULL
return|;
name|oldelem
operator|=
name|elem
expr_stmt|;
block|}
name|path
operator|=
name|next
expr_stmt|;
block|}
return|return
name|elem
return|;
block|}
end_function

begin_function
DECL|function|entry_type
specifier|static
specifier|const
name|char
modifier|*
name|entry_type
parameter_list|(
name|struct
name|tree_entry_list
modifier|*
name|e
parameter_list|)
block|{
return|return
operator|(
name|e
operator|->
name|directory
condition|?
literal|"tree"
else|:
literal|"blob"
operator|)
return|;
block|}
end_function

begin_function
DECL|function|entry_hex
specifier|static
specifier|const
name|char
modifier|*
name|entry_hex
parameter_list|(
name|struct
name|tree_entry_list
modifier|*
name|e
parameter_list|)
block|{
return|return
name|sha1_to_hex
argument_list|(
name|e
operator|->
name|directory
condition|?
name|e
operator|->
name|item
operator|.
name|tree
operator|->
name|object
operator|.
name|sha1
else|:
name|e
operator|->
name|item
operator|.
name|blob
operator|->
name|object
operator|.
name|sha1
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* forward declaration for mutually recursive routines */
end_comment

begin_function_decl
specifier|static
name|int
name|show_entry
parameter_list|(
name|struct
name|tree_entry_list
modifier|*
parameter_list|,
name|int
parameter_list|,
name|char
modifier|*
name|pathbuf
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|show_children
specifier|static
name|int
name|show_children
parameter_list|(
name|struct
name|tree_entry_list
modifier|*
name|e
parameter_list|,
name|int
name|level
parameter_list|,
name|char
modifier|*
name|pathbuf
parameter_list|)
block|{
name|int
name|oldlen
init|=
name|strlen
argument_list|(
name|pathbuf
argument_list|)
decl_stmt|;
if|if
condition|(
name|e
operator|!=
operator|&
name|root_entry
condition|)
name|sprintf
argument_list|(
name|pathbuf
operator|+
name|oldlen
argument_list|,
literal|"%s/"
argument_list|,
name|e
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|prepare_children
argument_list|(
name|e
argument_list|)
condition|)
name|die
argument_list|(
literal|"internal error: ls-tree show_children called with non tree"
argument_list|)
expr_stmt|;
name|e
operator|=
name|e
operator|->
name|item
operator|.
name|tree
operator|->
name|entries
expr_stmt|;
while|while
condition|(
name|e
condition|)
block|{
name|show_entry
argument_list|(
name|e
argument_list|,
name|level
argument_list|,
name|pathbuf
argument_list|)
expr_stmt|;
name|e
operator|=
name|e
operator|->
name|next
expr_stmt|;
block|}
name|pathbuf
index|[
name|oldlen
index|]
operator|=
literal|'\0'
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|show_entry
specifier|static
name|int
name|show_entry
parameter_list|(
name|struct
name|tree_entry_list
modifier|*
name|e
parameter_list|,
name|int
name|level
parameter_list|,
name|char
modifier|*
name|pathbuf
parameter_list|)
block|{
name|int
name|err
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|e
operator|!=
operator|&
name|root_entry
condition|)
block|{
name|int
name|pathlen
init|=
name|strlen
argument_list|(
name|pathbuf
argument_list|)
decl_stmt|;
name|printf
argument_list|(
literal|"%06o %s %s	"
argument_list|,
name|e
operator|->
name|mode
argument_list|,
name|entry_type
argument_list|(
name|e
argument_list|)
argument_list|,
name|entry_hex
argument_list|(
name|e
argument_list|)
argument_list|)
expr_stmt|;
name|write_name_quoted
argument_list|(
name|pathbuf
argument_list|,
name|pathlen
argument_list|,
name|e
operator|->
name|name
argument_list|,
name|line_termination
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
name|line_termination
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|e
operator|->
name|directory
condition|)
block|{
comment|/* If this is a directory, we have the following cases: 		 * (1) This is the top-level request (explicit path from the 		 *     command line, or "root" if there is no command line). 		 *  a. Without any flag.  We show direct children.  We do not  		 *     recurse into them. 		 *  b. With -r.  We do recurse into children. 		 *  c. With -d.  We do not recurse into children. 		 * (2) We came here because our caller is either (1-a) or 		 *     (1-b). 		 *  a. Without any flag.  We do not show our children (which 		 *     are grandchildren for the original request). 		 *  b. With -r.  We continue to recurse into our children. 		 *  c. With -d.  We should not have come here to begin with. 		 */
if|if
condition|(
name|level
operator|==
literal|0
operator|&&
operator|!
operator|(
name|ls_options
operator|&
name|LS_TREE_ONLY
operator|)
condition|)
comment|/* case (1)-a and (1)-b */
name|err
operator|=
name|err
operator||
name|show_children
argument_list|(
name|e
argument_list|,
name|level
operator|+
literal|1
argument_list|,
name|pathbuf
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|level
operator|&&
name|ls_options
operator|&
name|LS_RECURSIVE
condition|)
comment|/* case (2)-b */
name|err
operator|=
name|err
operator||
name|show_children
argument_list|(
name|e
argument_list|,
name|level
operator|+
literal|1
argument_list|,
name|pathbuf
argument_list|)
expr_stmt|;
block|}
return|return
name|err
return|;
block|}
end_function

begin_function
DECL|function|list_one
specifier|static
name|int
name|list_one
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|int
name|err
init|=
literal|0
decl_stmt|;
name|char
name|pathbuf
index|[
name|MAXPATHLEN
operator|+
literal|1
index|]
decl_stmt|;
name|struct
name|tree_entry_list
modifier|*
name|e
init|=
name|find_entry
argument_list|(
name|path
argument_list|,
name|pathbuf
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|e
condition|)
block|{
comment|/* traditionally ls-tree does not complain about 		 * missing path.  We may change this later to match 		 * what "/bin/ls -a" does, which is to complain. 		 */
return|return
name|err
return|;
block|}
name|err
operator|=
name|err
operator||
name|show_entry
argument_list|(
name|e
argument_list|,
literal|0
argument_list|,
name|pathbuf
argument_list|)
expr_stmt|;
return|return
name|err
return|;
block|}
end_function

begin_function
DECL|function|list
specifier|static
name|int
name|list
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|path
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|err
init|=
literal|0
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|path
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
name|err
operator|=
name|err
operator||
name|list_one
argument_list|(
name|path
index|[
name|i
index|]
argument_list|)
expr_stmt|;
return|return
name|err
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|ls_tree_usage
specifier|static
specifier|const
name|char
name|ls_tree_usage
index|[]
init|=
literal|"git-ls-tree [-d] [-r] [-z]<tree-ish> [path...]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
specifier|static
specifier|const
name|char
modifier|*
name|path0
index|[]
init|=
block|{
literal|""
block|,
name|NULL
block|}
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|path
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|nongit
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|prefix
init|=
name|setup_git_directory_gently
argument_list|(
operator|&
name|nongit
argument_list|)
decl_stmt|;
if|if
condition|(
name|prefix
condition|)
name|path0
index|[
literal|0
index|]
operator|=
name|prefix
expr_stmt|;
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
name|ls_options
operator||=
name|LS_RECURSIVE
expr_stmt|;
break|break;
case|case
literal|'d'
case|:
name|ls_options
operator||=
name|LS_TREE_ONLY
expr_stmt|;
break|break;
default|default:
name|usage
argument_list|(
name|ls_tree_usage
argument_list|)
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
operator|<
literal|2
condition|)
name|usage
argument_list|(
name|ls_tree_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1
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
name|usage
argument_list|(
name|ls_tree_usage
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|==
literal|2
condition|)
name|path
operator|=
name|path0
expr_stmt|;
else|else
name|path
operator|=
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
operator|+
literal|2
argument_list|)
expr_stmt|;
name|prepare_root
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|list
argument_list|(
name|path
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

