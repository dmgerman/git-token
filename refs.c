begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|<errno.h>
end_include

begin_comment
comment|/* We allow "recursive" symbolic refs. Only within reason, though */
end_comment

begin_define
DECL|macro|MAXDEPTH
define|#
directive|define
name|MAXDEPTH
value|5
end_define

begin_function
DECL|function|resolve_ref
specifier|const
name|char
modifier|*
name|resolve_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|reading
parameter_list|)
block|{
name|int
name|depth
init|=
name|MAXDEPTH
decl_stmt|,
name|len
decl_stmt|;
name|char
name|buffer
index|[
literal|256
index|]
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|;
name|int
name|fd
decl_stmt|;
if|if
condition|(
operator|--
name|depth
operator|<
literal|0
condition|)
return|return
name|NULL
return|;
comment|/* Special case: non-existing file. 		 * Not having the refs/heads/new-branch is OK 		 * if we are writing into it, so is .git/HEAD 		 * that points at refs/heads/master still to be 		 * born.  It is NOT OK if we are resolving for 		 * reading. 		 */
if|if
condition|(
name|lstat
argument_list|(
name|path
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
name|reading
operator|||
name|errno
operator|!=
name|ENOENT
condition|)
return|return
name|NULL
return|;
name|hashclr
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
return|return
name|path
return|;
block|}
comment|/* Follow "normalized" - ie "refs/.." symlinks by hand */
if|if
condition|(
name|S_ISLNK
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
name|len
operator|=
name|readlink
argument_list|(
name|path
argument_list|,
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>=
literal|5
operator|&&
operator|!
name|memcmp
argument_list|(
literal|"refs/"
argument_list|,
name|buffer
argument_list|,
literal|5
argument_list|)
condition|)
block|{
name|path
operator|=
name|git_path
argument_list|(
literal|"%.*s"
argument_list|,
name|len
argument_list|,
name|buffer
argument_list|)
expr_stmt|;
continue|continue;
block|}
block|}
comment|/* 		 * Anything else, just open it and try to use it as 		 * a ref 		 */
name|fd
operator|=
name|open
argument_list|(
name|path
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
name|NULL
return|;
name|len
operator|=
name|read
argument_list|(
name|fd
argument_list|,
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
comment|/* 		 * Is it a symbolic ref? 		 */
if|if
condition|(
name|len
operator|<
literal|4
operator|||
name|memcmp
argument_list|(
literal|"ref:"
argument_list|,
name|buffer
argument_list|,
literal|4
argument_list|)
condition|)
break|break;
name|buf
operator|=
name|buffer
operator|+
literal|4
expr_stmt|;
name|len
operator|-=
literal|4
expr_stmt|;
while|while
condition|(
name|len
operator|&&
name|isspace
argument_list|(
operator|*
name|buf
argument_list|)
condition|)
name|buf
operator|++
operator|,
name|len
operator|--
expr_stmt|;
while|while
condition|(
name|len
operator|&&
name|isspace
argument_list|(
name|buf
index|[
name|len
operator|-
literal|1
index|]
argument_list|)
condition|)
name|buf
index|[
operator|--
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|path
operator|=
name|git_path
argument_list|(
literal|"%.*s"
argument_list|,
name|len
argument_list|,
name|buf
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|len
operator|<
literal|40
operator|||
name|get_sha1_hex
argument_list|(
name|buffer
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
name|NULL
return|;
return|return
name|path
return|;
block|}
end_function

begin_function
DECL|function|create_symref
name|int
name|create_symref
parameter_list|(
specifier|const
name|char
modifier|*
name|git_HEAD
parameter_list|,
specifier|const
name|char
modifier|*
name|refs_heads_master
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|lockpath
decl_stmt|;
name|char
name|ref
index|[
literal|1000
index|]
decl_stmt|;
name|int
name|fd
decl_stmt|,
name|len
decl_stmt|,
name|written
decl_stmt|;
ifndef|#
directive|ifndef
name|NO_SYMLINK_HEAD
if|if
condition|(
name|prefer_symlink_refs
condition|)
block|{
name|unlink
argument_list|(
name|git_HEAD
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|symlink
argument_list|(
name|refs_heads_master
argument_list|,
name|git_HEAD
argument_list|)
condition|)
return|return
literal|0
return|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"no symlink - falling back to symbolic ref\n"
argument_list|)
expr_stmt|;
block|}
endif|#
directive|endif
name|len
operator|=
name|snprintf
argument_list|(
name|ref
argument_list|,
sizeof|sizeof
argument_list|(
name|ref
argument_list|)
argument_list|,
literal|"ref: %s\n"
argument_list|,
name|refs_heads_master
argument_list|)
expr_stmt|;
if|if
condition|(
sizeof|sizeof
argument_list|(
name|ref
argument_list|)
operator|<=
name|len
condition|)
block|{
name|error
argument_list|(
literal|"refname too long: %s"
argument_list|,
name|refs_heads_master
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|lockpath
operator|=
name|mkpath
argument_list|(
literal|"%s.lock"
argument_list|,
name|git_HEAD
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|lockpath
argument_list|,
name|O_CREAT
operator||
name|O_EXCL
operator||
name|O_WRONLY
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
name|written
operator|=
name|write
argument_list|(
name|fd
argument_list|,
name|ref
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|written
operator|!=
name|len
condition|)
block|{
name|unlink
argument_list|(
name|lockpath
argument_list|)
expr_stmt|;
name|error
argument_list|(
literal|"Unable to write to %s"
argument_list|,
name|lockpath
argument_list|)
expr_stmt|;
return|return
operator|-
literal|2
return|;
block|}
if|if
condition|(
name|rename
argument_list|(
name|lockpath
argument_list|,
name|git_HEAD
argument_list|)
operator|<
literal|0
condition|)
block|{
name|unlink
argument_list|(
name|lockpath
argument_list|)
expr_stmt|;
name|error
argument_list|(
literal|"Unable to create %s"
argument_list|,
name|git_HEAD
argument_list|)
expr_stmt|;
return|return
operator|-
literal|3
return|;
block|}
if|if
condition|(
name|adjust_shared_perm
argument_list|(
name|git_HEAD
argument_list|)
condition|)
block|{
name|unlink
argument_list|(
name|lockpath
argument_list|)
expr_stmt|;
name|error
argument_list|(
literal|"Unable to fix permissions on %s"
argument_list|,
name|lockpath
argument_list|)
expr_stmt|;
return|return
operator|-
literal|4
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|read_ref
name|int
name|read_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|filename
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
if|if
condition|(
name|resolve_ref
argument_list|(
name|filename
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|)
condition|)
return|return
literal|0
return|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|do_for_each_ref
specifier|static
name|int
name|do_for_each_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
function_decl|(
modifier|*
name|fn
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
parameter_list|,
name|int
name|trim
parameter_list|)
block|{
name|int
name|retval
init|=
literal|0
decl_stmt|;
name|DIR
modifier|*
name|dir
init|=
name|opendir
argument_list|(
name|git_path
argument_list|(
literal|"%s"
argument_list|,
name|base
argument_list|)
argument_list|)
decl_stmt|;
if|if
condition|(
name|dir
condition|)
block|{
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
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
name|path
init|=
name|xmalloc
argument_list|(
name|baselen
operator|+
literal|257
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|base
argument_list|,
literal|"./"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
name|base
operator|+=
literal|2
expr_stmt|;
name|baselen
operator|-=
literal|2
expr_stmt|;
block|}
name|memcpy
argument_list|(
name|path
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
if|if
condition|(
name|baselen
operator|&&
name|base
index|[
name|baselen
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
name|path
index|[
name|baselen
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
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
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|int
name|namelen
decl_stmt|;
if|if
condition|(
name|de
operator|->
name|d_name
index|[
literal|0
index|]
operator|==
literal|'.'
condition|)
continue|continue;
name|namelen
operator|=
name|strlen
argument_list|(
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|namelen
operator|>
literal|255
condition|)
continue|continue;
if|if
condition|(
name|has_extension
argument_list|(
name|de
operator|->
name|d_name
argument_list|,
literal|".lock"
argument_list|)
condition|)
continue|continue;
name|memcpy
argument_list|(
name|path
operator|+
name|baselen
argument_list|,
name|de
operator|->
name|d_name
argument_list|,
name|namelen
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|git_path
argument_list|(
literal|"%s"
argument_list|,
name|path
argument_list|)
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
continue|continue;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
name|retval
operator|=
name|do_for_each_ref
argument_list|(
name|path
argument_list|,
name|fn
argument_list|,
name|trim
argument_list|)
expr_stmt|;
if|if
condition|(
name|retval
condition|)
break|break;
continue|continue;
block|}
if|if
condition|(
name|read_ref
argument_list|(
name|git_path
argument_list|(
literal|"%s"
argument_list|,
name|path
argument_list|)
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
block|{
name|error
argument_list|(
literal|"%s points nowhere!"
argument_list|,
name|path
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|has_sha1_file
argument_list|(
name|sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"%s does not point to a valid "
literal|"commit object!"
argument_list|,
name|path
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|retval
operator|=
name|fn
argument_list|(
name|path
operator|+
name|trim
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|retval
condition|)
break|break;
block|}
name|free
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
block|}
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|head_ref
name|int
name|head_ref
parameter_list|(
name|int
function_decl|(
modifier|*
name|fn
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
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
name|read_ref
argument_list|(
name|git_path
argument_list|(
literal|"HEAD"
argument_list|)
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
name|fn
argument_list|(
literal|"HEAD"
argument_list|,
name|sha1
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|for_each_ref
name|int
name|for_each_ref
parameter_list|(
name|int
function_decl|(
modifier|*
name|fn
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
parameter_list|)
block|{
return|return
name|do_for_each_ref
argument_list|(
literal|"refs"
argument_list|,
name|fn
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|for_each_tag_ref
name|int
name|for_each_tag_ref
parameter_list|(
name|int
function_decl|(
modifier|*
name|fn
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
parameter_list|)
block|{
return|return
name|do_for_each_ref
argument_list|(
literal|"refs/tags"
argument_list|,
name|fn
argument_list|,
literal|10
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|for_each_branch_ref
name|int
name|for_each_branch_ref
parameter_list|(
name|int
function_decl|(
modifier|*
name|fn
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
parameter_list|)
block|{
return|return
name|do_for_each_ref
argument_list|(
literal|"refs/heads"
argument_list|,
name|fn
argument_list|,
literal|11
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|for_each_remote_ref
name|int
name|for_each_remote_ref
parameter_list|(
name|int
function_decl|(
modifier|*
name|fn
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
parameter_list|)
block|{
return|return
name|do_for_each_ref
argument_list|(
literal|"refs/remotes"
argument_list|,
name|fn
argument_list|,
literal|13
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|get_ref_sha1
name|int
name|get_ref_sha1
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
if|if
condition|(
name|check_ref_format
argument_list|(
name|ref
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
return|return
name|read_ref
argument_list|(
name|git_path
argument_list|(
literal|"refs/%s"
argument_list|,
name|ref
argument_list|)
argument_list|,
name|sha1
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * Make sure "ref" is something reasonable to have under ".git/refs/";  * We do not like it if:  *  * - any path component of it begins with ".", or  * - it has double dots "..", or  * - it has ASCII control character, "~", "^", ":" or SP, anywhere, or  * - it ends with a "/".  */
end_comment

begin_function
DECL|function|bad_ref_char
specifier|static
specifier|inline
name|int
name|bad_ref_char
parameter_list|(
name|int
name|ch
parameter_list|)
block|{
return|return
operator|(
operator|(
operator|(
name|unsigned
operator|)
name|ch
operator|)
operator|<=
literal|' '
operator|||
name|ch
operator|==
literal|'~'
operator|||
name|ch
operator|==
literal|'^'
operator|||
name|ch
operator|==
literal|':'
operator|||
comment|/* 2.13 Pattern Matching Notation */
name|ch
operator|==
literal|'?'
operator|||
name|ch
operator|==
literal|'*'
operator|||
name|ch
operator|==
literal|'['
operator|)
return|;
block|}
end_function

begin_function
DECL|function|check_ref_format
name|int
name|check_ref_format
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|)
block|{
name|int
name|ch
decl_stmt|,
name|level
decl_stmt|;
specifier|const
name|char
modifier|*
name|cp
init|=
name|ref
decl_stmt|;
name|level
operator|=
literal|0
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
while|while
condition|(
operator|(
name|ch
operator|=
operator|*
name|cp
operator|++
operator|)
operator|==
literal|'/'
condition|)
empty_stmt|;
comment|/* tolerate duplicated slashes */
if|if
condition|(
operator|!
name|ch
condition|)
return|return
operator|-
literal|1
return|;
comment|/* should not end with slashes */
comment|/* we are at the beginning of the path component */
if|if
condition|(
name|ch
operator|==
literal|'.'
operator|||
name|bad_ref_char
argument_list|(
name|ch
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
comment|/* scan the rest of the path component */
while|while
condition|(
operator|(
name|ch
operator|=
operator|*
name|cp
operator|++
operator|)
operator|!=
literal|0
condition|)
block|{
if|if
condition|(
name|bad_ref_char
argument_list|(
name|ch
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|ch
operator|==
literal|'/'
condition|)
break|break;
if|if
condition|(
name|ch
operator|==
literal|'.'
operator|&&
operator|*
name|cp
operator|==
literal|'.'
condition|)
return|return
operator|-
literal|1
return|;
block|}
name|level
operator|++
expr_stmt|;
if|if
condition|(
operator|!
name|ch
condition|)
block|{
if|if
condition|(
name|level
operator|<
literal|2
condition|)
return|return
operator|-
literal|1
return|;
comment|/* at least of form "heads/blah" */
return|return
literal|0
return|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|verify_lock
specifier|static
name|struct
name|ref_lock
modifier|*
name|verify_lock
parameter_list|(
name|struct
name|ref_lock
modifier|*
name|lock
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
name|int
name|mustexist
parameter_list|)
block|{
name|char
name|buf
index|[
literal|40
index|]
decl_stmt|;
name|int
name|nr
decl_stmt|,
name|fd
init|=
name|open
argument_list|(
name|lock
operator|->
name|ref_file
argument_list|,
name|O_RDONLY
argument_list|)
decl_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
operator|&&
operator|(
name|mustexist
operator|||
name|errno
operator|!=
name|ENOENT
operator|)
condition|)
block|{
name|error
argument_list|(
literal|"Can't verify ref %s"
argument_list|,
name|lock
operator|->
name|ref_file
argument_list|)
expr_stmt|;
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|nr
operator|=
name|read
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
literal|40
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|nr
operator|!=
literal|40
operator|||
name|get_sha1_hex
argument_list|(
name|buf
argument_list|,
name|lock
operator|->
name|old_sha1
argument_list|)
operator|<
literal|0
condition|)
block|{
name|error
argument_list|(
literal|"Can't verify ref %s"
argument_list|,
name|lock
operator|->
name|ref_file
argument_list|)
expr_stmt|;
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
if|if
condition|(
name|hashcmp
argument_list|(
name|lock
operator|->
name|old_sha1
argument_list|,
name|old_sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"Ref %s is at %s but expected %s"
argument_list|,
name|lock
operator|->
name|ref_file
argument_list|,
name|sha1_to_hex
argument_list|(
name|lock
operator|->
name|old_sha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|old_sha1
argument_list|)
argument_list|)
expr_stmt|;
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
return|return
name|lock
return|;
block|}
end_function

begin_function
DECL|function|lock_ref_sha1_basic
specifier|static
name|struct
name|ref_lock
modifier|*
name|lock_ref_sha1_basic
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|plen
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
name|int
name|mustexist
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|orig_path
init|=
name|path
decl_stmt|;
name|struct
name|ref_lock
modifier|*
name|lock
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|lock
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|ref_lock
argument_list|)
argument_list|)
expr_stmt|;
name|lock
operator|->
name|lock_fd
operator|=
operator|-
literal|1
expr_stmt|;
name|plen
operator|=
name|strlen
argument_list|(
name|path
argument_list|)
operator|-
name|plen
expr_stmt|;
name|path
operator|=
name|resolve_ref
argument_list|(
name|path
argument_list|,
name|lock
operator|->
name|old_sha1
argument_list|,
name|mustexist
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|path
condition|)
block|{
name|int
name|last_errno
init|=
name|errno
decl_stmt|;
name|error
argument_list|(
literal|"unable to resolve reference %s: %s"
argument_list|,
name|orig_path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
name|errno
operator|=
name|last_errno
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|lock
operator|->
name|lk
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|lock_file
argument_list|)
argument_list|)
expr_stmt|;
name|lock
operator|->
name|ref_file
operator|=
name|strdup
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|lock
operator|->
name|log_file
operator|=
name|strdup
argument_list|(
name|git_path
argument_list|(
literal|"logs/%s"
argument_list|,
name|lock
operator|->
name|ref_file
operator|+
name|plen
argument_list|)
argument_list|)
expr_stmt|;
name|lock
operator|->
name|force_write
operator|=
name|lstat
argument_list|(
name|lock
operator|->
name|ref_file
argument_list|,
operator|&
name|st
argument_list|)
operator|&&
name|errno
operator|==
name|ENOENT
expr_stmt|;
if|if
condition|(
name|safe_create_leading_directories
argument_list|(
name|lock
operator|->
name|ref_file
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to create directory for %s"
argument_list|,
name|lock
operator|->
name|ref_file
argument_list|)
expr_stmt|;
name|lock
operator|->
name|lock_fd
operator|=
name|hold_lock_file_for_update
argument_list|(
name|lock
operator|->
name|lk
argument_list|,
name|lock
operator|->
name|ref_file
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
name|old_sha1
condition|?
name|verify_lock
argument_list|(
name|lock
argument_list|,
name|old_sha1
argument_list|,
name|mustexist
argument_list|)
else|:
name|lock
return|;
block|}
end_function

begin_function
DECL|function|lock_ref_sha1
name|struct
name|ref_lock
modifier|*
name|lock_ref_sha1
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
name|int
name|mustexist
parameter_list|)
block|{
if|if
condition|(
name|check_ref_format
argument_list|(
name|ref
argument_list|)
condition|)
return|return
name|NULL
return|;
return|return
name|lock_ref_sha1_basic
argument_list|(
name|git_path
argument_list|(
literal|"refs/%s"
argument_list|,
name|ref
argument_list|)
argument_list|,
literal|5
operator|+
name|strlen
argument_list|(
name|ref
argument_list|)
argument_list|,
name|old_sha1
argument_list|,
name|mustexist
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|lock_any_ref_for_update
name|struct
name|ref_lock
modifier|*
name|lock_any_ref_for_update
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
name|int
name|mustexist
parameter_list|)
block|{
return|return
name|lock_ref_sha1_basic
argument_list|(
name|git_path
argument_list|(
literal|"%s"
argument_list|,
name|ref
argument_list|)
argument_list|,
name|strlen
argument_list|(
name|ref
argument_list|)
argument_list|,
name|old_sha1
argument_list|,
name|mustexist
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|unlock_ref
name|void
name|unlock_ref
parameter_list|(
name|struct
name|ref_lock
modifier|*
name|lock
parameter_list|)
block|{
if|if
condition|(
name|lock
operator|->
name|lock_fd
operator|>=
literal|0
condition|)
block|{
name|close
argument_list|(
name|lock
operator|->
name|lock_fd
argument_list|)
expr_stmt|;
comment|/* Do not free lock->lk -- atexit() still looks at them */
if|if
condition|(
name|lock
operator|->
name|lk
condition|)
name|rollback_lock_file
argument_list|(
name|lock
operator|->
name|lk
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|lock
operator|->
name|ref_file
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|lock
operator|->
name|log_file
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|lock
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|log_ref_write
specifier|static
name|int
name|log_ref_write
parameter_list|(
name|struct
name|ref_lock
modifier|*
name|lock
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
name|msg
parameter_list|)
block|{
name|int
name|logfd
decl_stmt|,
name|written
decl_stmt|,
name|oflags
init|=
name|O_APPEND
operator||
name|O_WRONLY
decl_stmt|;
name|unsigned
name|maxlen
decl_stmt|,
name|len
decl_stmt|;
name|char
modifier|*
name|logrec
decl_stmt|;
specifier|const
name|char
modifier|*
name|committer
decl_stmt|;
if|if
condition|(
name|log_all_ref_updates
condition|)
block|{
if|if
condition|(
name|safe_create_leading_directories
argument_list|(
name|lock
operator|->
name|log_file
argument_list|)
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"unable to create directory for %s"
argument_list|,
name|lock
operator|->
name|log_file
argument_list|)
return|;
name|oflags
operator||=
name|O_CREAT
expr_stmt|;
block|}
name|logfd
operator|=
name|open
argument_list|(
name|lock
operator|->
name|log_file
argument_list|,
name|oflags
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
if|if
condition|(
name|logfd
operator|<
literal|0
condition|)
block|{
if|if
condition|(
operator|!
name|log_all_ref_updates
operator|&&
name|errno
operator|==
name|ENOENT
condition|)
return|return
literal|0
return|;
return|return
name|error
argument_list|(
literal|"Unable to append to %s: %s"
argument_list|,
name|lock
operator|->
name|log_file
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
block|}
name|committer
operator|=
name|git_committer_info
argument_list|(
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|msg
condition|)
block|{
name|maxlen
operator|=
name|strlen
argument_list|(
name|committer
argument_list|)
operator|+
name|strlen
argument_list|(
name|msg
argument_list|)
operator|+
literal|2
operator|*
literal|40
operator|+
literal|5
expr_stmt|;
name|logrec
operator|=
name|xmalloc
argument_list|(
name|maxlen
argument_list|)
expr_stmt|;
name|len
operator|=
name|snprintf
argument_list|(
name|logrec
argument_list|,
name|maxlen
argument_list|,
literal|"%s %s %s\t%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|lock
operator|->
name|old_sha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|committer
argument_list|,
name|msg
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|maxlen
operator|=
name|strlen
argument_list|(
name|committer
argument_list|)
operator|+
literal|2
operator|*
literal|40
operator|+
literal|4
expr_stmt|;
name|logrec
operator|=
name|xmalloc
argument_list|(
name|maxlen
argument_list|)
expr_stmt|;
name|len
operator|=
name|snprintf
argument_list|(
name|logrec
argument_list|,
name|maxlen
argument_list|,
literal|"%s %s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|lock
operator|->
name|old_sha1
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|committer
argument_list|)
expr_stmt|;
block|}
name|written
operator|=
name|len
operator|<=
name|maxlen
condition|?
name|write
argument_list|(
name|logfd
argument_list|,
name|logrec
argument_list|,
name|len
argument_list|)
else|:
operator|-
literal|1
expr_stmt|;
name|free
argument_list|(
name|logrec
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|logfd
argument_list|)
expr_stmt|;
if|if
condition|(
name|written
operator|!=
name|len
condition|)
return|return
name|error
argument_list|(
literal|"Unable to append to %s"
argument_list|,
name|lock
operator|->
name|log_file
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|write_ref_sha1
name|int
name|write_ref_sha1
parameter_list|(
name|struct
name|ref_lock
modifier|*
name|lock
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
name|logmsg
parameter_list|)
block|{
specifier|static
name|char
name|term
init|=
literal|'\n'
decl_stmt|;
if|if
condition|(
operator|!
name|lock
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|lock
operator|->
name|force_write
operator|&&
operator|!
name|hashcmp
argument_list|(
name|lock
operator|->
name|old_sha1
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
name|write
argument_list|(
name|lock
operator|->
name|lock_fd
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
literal|40
argument_list|)
operator|!=
literal|40
operator|||
name|write
argument_list|(
name|lock
operator|->
name|lock_fd
argument_list|,
operator|&
name|term
argument_list|,
literal|1
argument_list|)
operator|!=
literal|1
operator|||
name|close
argument_list|(
name|lock
operator|->
name|lock_fd
argument_list|)
operator|<
literal|0
condition|)
block|{
name|error
argument_list|(
literal|"Couldn't write %s"
argument_list|,
name|lock
operator|->
name|lk
operator|->
name|filename
argument_list|)
expr_stmt|;
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|log_ref_write
argument_list|(
name|lock
argument_list|,
name|sha1
argument_list|,
name|logmsg
argument_list|)
operator|<
literal|0
condition|)
block|{
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|commit_lock_file
argument_list|(
name|lock
operator|->
name|lk
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"Couldn't set %s"
argument_list|,
name|lock
operator|->
name|ref_file
argument_list|)
expr_stmt|;
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|lock
operator|->
name|lock_fd
operator|=
operator|-
literal|1
expr_stmt|;
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|read_ref_at
name|int
name|read_ref_at
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
name|unsigned
name|long
name|at_time
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|logfile
decl_stmt|,
modifier|*
name|logdata
decl_stmt|,
modifier|*
name|logend
decl_stmt|,
modifier|*
name|rec
decl_stmt|,
modifier|*
name|lastgt
decl_stmt|,
modifier|*
name|lastrec
decl_stmt|;
name|char
modifier|*
name|tz_c
decl_stmt|;
name|int
name|logfd
decl_stmt|,
name|tz
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|unsigned
name|long
name|date
decl_stmt|;
name|unsigned
name|char
name|logged_sha1
index|[
literal|20
index|]
decl_stmt|;
name|logfile
operator|=
name|git_path
argument_list|(
literal|"logs/%s"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
name|logfd
operator|=
name|open
argument_list|(
name|logfile
argument_list|,
name|O_RDONLY
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|logfd
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"Unable to read log %s: %s"
argument_list|,
name|logfile
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|fstat
argument_list|(
name|logfd
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|st
operator|.
name|st_size
condition|)
name|die
argument_list|(
literal|"Log %s is empty."
argument_list|,
name|logfile
argument_list|)
expr_stmt|;
name|logdata
operator|=
name|mmap
argument_list|(
name|NULL
argument_list|,
name|st
operator|.
name|st_size
argument_list|,
name|PROT_READ
argument_list|,
name|MAP_PRIVATE
argument_list|,
name|logfd
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|logfd
argument_list|)
expr_stmt|;
name|lastrec
operator|=
name|NULL
expr_stmt|;
name|rec
operator|=
name|logend
operator|=
name|logdata
operator|+
name|st
operator|.
name|st_size
expr_stmt|;
while|while
condition|(
name|logdata
operator|<
name|rec
condition|)
block|{
if|if
condition|(
name|logdata
operator|<
name|rec
operator|&&
operator|*
operator|(
name|rec
operator|-
literal|1
operator|)
operator|==
literal|'\n'
condition|)
name|rec
operator|--
expr_stmt|;
name|lastgt
operator|=
name|NULL
expr_stmt|;
while|while
condition|(
name|logdata
operator|<
name|rec
operator|&&
operator|*
operator|(
name|rec
operator|-
literal|1
operator|)
operator|!=
literal|'\n'
condition|)
block|{
name|rec
operator|--
expr_stmt|;
if|if
condition|(
operator|*
name|rec
operator|==
literal|'>'
condition|)
name|lastgt
operator|=
name|rec
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|lastgt
condition|)
name|die
argument_list|(
literal|"Log %s is corrupt."
argument_list|,
name|logfile
argument_list|)
expr_stmt|;
name|date
operator|=
name|strtoul
argument_list|(
name|lastgt
operator|+
literal|1
argument_list|,
operator|&
name|tz_c
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|date
operator|<=
name|at_time
condition|)
block|{
if|if
condition|(
name|lastrec
condition|)
block|{
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|lastrec
argument_list|,
name|logged_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Log %s is corrupt."
argument_list|,
name|logfile
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|rec
operator|+
literal|41
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Log %s is corrupt."
argument_list|,
name|logfile
argument_list|)
expr_stmt|;
if|if
condition|(
name|hashcmp
argument_list|(
name|logged_sha1
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|tz
operator|=
name|strtoul
argument_list|(
name|tz_c
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"warning: Log %s has gap after %s.\n"
argument_list|,
name|logfile
argument_list|,
name|show_rfc2822_date
argument_list|(
name|date
argument_list|,
name|tz
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
elseif|else
if|if
condition|(
name|date
operator|==
name|at_time
condition|)
block|{
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|rec
operator|+
literal|41
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Log %s is corrupt."
argument_list|,
name|logfile
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|rec
operator|+
literal|41
argument_list|,
name|logged_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Log %s is corrupt."
argument_list|,
name|logfile
argument_list|)
expr_stmt|;
if|if
condition|(
name|hashcmp
argument_list|(
name|logged_sha1
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|tz
operator|=
name|strtoul
argument_list|(
name|tz_c
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"warning: Log %s unexpectedly ended on %s.\n"
argument_list|,
name|logfile
argument_list|,
name|show_rfc2822_date
argument_list|(
name|date
argument_list|,
name|tz
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
name|munmap
argument_list|(
operator|(
name|void
operator|*
operator|)
name|logdata
argument_list|,
name|st
operator|.
name|st_size
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|lastrec
operator|=
name|rec
expr_stmt|;
block|}
name|rec
operator|=
name|logdata
expr_stmt|;
while|while
condition|(
name|rec
operator|<
name|logend
operator|&&
operator|*
name|rec
operator|!=
literal|'>'
operator|&&
operator|*
name|rec
operator|!=
literal|'\n'
condition|)
name|rec
operator|++
expr_stmt|;
if|if
condition|(
name|rec
operator|==
name|logend
operator|||
operator|*
name|rec
operator|==
literal|'\n'
condition|)
name|die
argument_list|(
literal|"Log %s is corrupt."
argument_list|,
name|logfile
argument_list|)
expr_stmt|;
name|date
operator|=
name|strtoul
argument_list|(
name|rec
operator|+
literal|1
argument_list|,
operator|&
name|tz_c
argument_list|,
literal|10
argument_list|)
expr_stmt|;
name|tz
operator|=
name|strtoul
argument_list|(
name|tz_c
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|logdata
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"Log %s is corrupt."
argument_list|,
name|logfile
argument_list|)
expr_stmt|;
name|munmap
argument_list|(
operator|(
name|void
operator|*
operator|)
name|logdata
argument_list|,
name|st
operator|.
name|st_size
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"warning: Log %s only goes back to %s.\n"
argument_list|,
name|logfile
argument_list|,
name|show_rfc2822_date
argument_list|(
name|date
argument_list|,
name|tz
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

