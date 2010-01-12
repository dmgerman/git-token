begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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
file|"dir.h"
end_include

begin_function
DECL|function|create_directories
specifier|static
name|void
name|create_directories
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|path_len
parameter_list|,
specifier|const
name|struct
name|checkout
modifier|*
name|state
parameter_list|)
block|{
name|char
modifier|*
name|buf
init|=
name|xmalloc
argument_list|(
name|path_len
operator|+
literal|1
argument_list|)
decl_stmt|;
name|int
name|len
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|len
operator|<
name|path_len
condition|)
block|{
do|do
block|{
name|buf
index|[
name|len
index|]
operator|=
name|path
index|[
name|len
index|]
expr_stmt|;
name|len
operator|++
expr_stmt|;
block|}
do|while
condition|(
name|len
operator|<
name|path_len
operator|&&
name|path
index|[
name|len
index|]
operator|!=
literal|'/'
condition|)
do|;
if|if
condition|(
name|len
operator|>=
name|path_len
condition|)
break|break;
name|buf
index|[
name|len
index|]
operator|=
literal|0
expr_stmt|;
comment|/* 		 * For 'checkout-index --prefix=<dir>',<dir> is 		 * allowed to be a symlink to an existing directory, 		 * and we set 'state->base_dir_len' below, such that 		 * we test the path components of the prefix with the 		 * stat() function instead of the lstat() function. 		 */
if|if
condition|(
name|has_dirs_only_path
argument_list|(
name|buf
argument_list|,
name|len
argument_list|,
name|state
operator|->
name|base_dir_len
argument_list|)
condition|)
continue|continue;
comment|/* ok, it is already a directory. */
comment|/* 		 * If this mkdir() would fail, it could be that there 		 * is already a symlink or something else exists 		 * there, therefore we then try to unlink it and try 		 * one more time to create the directory. 		 */
if|if
condition|(
name|mkdir
argument_list|(
name|buf
argument_list|,
literal|0777
argument_list|)
condition|)
block|{
if|if
condition|(
name|errno
operator|==
name|EEXIST
operator|&&
name|state
operator|->
name|force
operator|&&
operator|!
name|unlink_or_warn
argument_list|(
name|buf
argument_list|)
operator|&&
operator|!
name|mkdir
argument_list|(
name|buf
argument_list|,
literal|0777
argument_list|)
condition|)
continue|continue;
name|die_errno
argument_list|(
literal|"cannot create directory at '%s'"
argument_list|,
name|buf
argument_list|)
expr_stmt|;
block|}
block|}
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|remove_subtree
specifier|static
name|void
name|remove_subtree
parameter_list|(
specifier|const
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
name|char
name|pathbuf
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|char
modifier|*
name|name
decl_stmt|;
if|if
condition|(
operator|!
name|dir
condition|)
name|die_errno
argument_list|(
literal|"cannot opendir '%s'"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|pathbuf
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|name
operator|=
name|pathbuf
operator|+
name|strlen
argument_list|(
name|path
argument_list|)
expr_stmt|;
operator|*
name|name
operator|++
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
name|struct
name|stat
name|st
decl_stmt|;
if|if
condition|(
name|is_dot_or_dotdot
argument_list|(
name|de
operator|->
name|d_name
argument_list|)
condition|)
continue|continue;
name|strcpy
argument_list|(
name|name
argument_list|,
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|pathbuf
argument_list|,
operator|&
name|st
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"cannot lstat '%s'"
argument_list|,
name|pathbuf
argument_list|)
expr_stmt|;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
name|remove_subtree
argument_list|(
name|pathbuf
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|unlink
argument_list|(
name|pathbuf
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"cannot unlink '%s'"
argument_list|,
name|pathbuf
argument_list|)
expr_stmt|;
block|}
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|rmdir
argument_list|(
name|path
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"cannot rmdir '%s'"
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|create_file
specifier|static
name|int
name|create_file
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|int
name|mode
parameter_list|)
block|{
name|mode
operator|=
operator|(
name|mode
operator|&
literal|0100
operator|)
condition|?
literal|0777
else|:
literal|0666
expr_stmt|;
return|return
name|open
argument_list|(
name|path
argument_list|,
name|O_WRONLY
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
name|mode
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|read_blob_entry
specifier|static
name|void
modifier|*
name|read_blob_entry
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|unsigned
name|long
modifier|*
name|size
parameter_list|)
block|{
name|enum
name|object_type
name|type
decl_stmt|;
name|void
modifier|*
name|new
init|=
name|read_sha1_file
argument_list|(
name|ce
operator|->
name|sha1
argument_list|,
operator|&
name|type
argument_list|,
name|size
argument_list|)
decl_stmt|;
if|if
condition|(
name|new
condition|)
block|{
if|if
condition|(
name|type
operator|==
name|OBJ_BLOB
condition|)
return|return
name|new
return|;
name|free
argument_list|(
name|new
argument_list|)
expr_stmt|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|write_entry
specifier|static
name|int
name|write_entry
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|struct
name|checkout
modifier|*
name|state
parameter_list|,
name|int
name|to_tempfile
parameter_list|)
block|{
name|unsigned
name|int
name|ce_mode_s_ifmt
init|=
name|ce
operator|->
name|ce_mode
operator|&
name|S_IFMT
decl_stmt|;
name|int
name|fd
decl_stmt|,
name|ret
decl_stmt|,
name|fstat_done
init|=
literal|0
decl_stmt|;
name|char
modifier|*
name|new
decl_stmt|;
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|size_t
name|wrote
decl_stmt|,
name|newsize
init|=
literal|0
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
switch|switch
condition|(
name|ce_mode_s_ifmt
condition|)
block|{
case|case
name|S_IFREG
case|:
case|case
name|S_IFLNK
case|:
name|new
operator|=
name|read_blob_entry
argument_list|(
name|ce
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|new
condition|)
return|return
name|error
argument_list|(
literal|"git checkout-index: unable to read sha1 file of %s (%s)"
argument_list|,
name|path
argument_list|,
name|sha1_to_hex
argument_list|(
name|ce
operator|->
name|sha1
argument_list|)
argument_list|)
return|;
if|if
condition|(
name|ce_mode_s_ifmt
operator|==
name|S_IFLNK
operator|&&
name|has_symlinks
operator|&&
operator|!
name|to_tempfile
condition|)
block|{
name|ret
operator|=
name|symlink
argument_list|(
name|new
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|new
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
return|return
name|error
argument_list|(
literal|"git checkout-index: unable to create symlink %s (%s)"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
break|break;
block|}
comment|/* 		 * Convert from git internal format to working tree format 		 */
if|if
condition|(
name|ce_mode_s_ifmt
operator|==
name|S_IFREG
operator|&&
name|convert_to_working_tree
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|new
argument_list|,
name|size
argument_list|,
operator|&
name|buf
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|new
argument_list|)
expr_stmt|;
name|new
operator|=
name|strbuf_detach
argument_list|(
operator|&
name|buf
argument_list|,
operator|&
name|newsize
argument_list|)
expr_stmt|;
name|size
operator|=
name|newsize
expr_stmt|;
block|}
if|if
condition|(
name|to_tempfile
condition|)
block|{
if|if
condition|(
name|ce_mode_s_ifmt
operator|==
name|S_IFREG
condition|)
name|strcpy
argument_list|(
name|path
argument_list|,
literal|".merge_file_XXXXXX"
argument_list|)
expr_stmt|;
else|else
name|strcpy
argument_list|(
name|path
argument_list|,
literal|".merge_link_XXXXXX"
argument_list|)
expr_stmt|;
name|fd
operator|=
name|mkstemp
argument_list|(
name|path
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|ce_mode_s_ifmt
operator|==
name|S_IFREG
condition|)
block|{
name|fd
operator|=
name|create_file
argument_list|(
name|path
argument_list|,
name|ce
operator|->
name|ce_mode
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|fd
operator|=
name|create_file
argument_list|(
name|path
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
block|{
name|free
argument_list|(
name|new
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"git checkout-index: unable to create file %s (%s)"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
block|}
name|wrote
operator|=
name|write_in_full
argument_list|(
name|fd
argument_list|,
name|new
argument_list|,
name|size
argument_list|)
expr_stmt|;
comment|/* use fstat() only when path == ce->name */
if|if
condition|(
name|fstat_is_reliable
argument_list|()
operator|&&
name|state
operator|->
name|refresh_cache
operator|&&
operator|!
name|to_tempfile
operator|&&
operator|!
name|state
operator|->
name|base_dir_len
condition|)
block|{
name|fstat
argument_list|(
name|fd
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
name|fstat_done
operator|=
literal|1
expr_stmt|;
block|}
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|new
argument_list|)
expr_stmt|;
if|if
condition|(
name|wrote
operator|!=
name|size
condition|)
return|return
name|error
argument_list|(
literal|"git checkout-index: unable to write file %s"
argument_list|,
name|path
argument_list|)
return|;
break|break;
case|case
name|S_IFGITLINK
case|:
if|if
condition|(
name|to_tempfile
condition|)
return|return
name|error
argument_list|(
literal|"git checkout-index: cannot create temporary subproject %s"
argument_list|,
name|path
argument_list|)
return|;
if|if
condition|(
name|mkdir
argument_list|(
name|path
argument_list|,
literal|0777
argument_list|)
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"git checkout-index: cannot create subproject directory %s"
argument_list|,
name|path
argument_list|)
return|;
break|break;
default|default:
return|return
name|error
argument_list|(
literal|"git checkout-index: unknown file mode for %s"
argument_list|,
name|path
argument_list|)
return|;
block|}
if|if
condition|(
name|state
operator|->
name|refresh_cache
condition|)
block|{
if|if
condition|(
operator|!
name|fstat_done
condition|)
name|lstat
argument_list|(
name|ce
operator|->
name|name
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
name|fill_stat_cache_info
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * This is like 'lstat()', except it refuses to follow symlinks  * in the path, after skipping "skiplen".  */
end_comment

begin_function
DECL|function|check_path
specifier|static
name|int
name|check_path
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|len
parameter_list|,
name|struct
name|stat
modifier|*
name|st
parameter_list|,
name|int
name|skiplen
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|slash
init|=
name|path
operator|+
name|len
decl_stmt|;
while|while
condition|(
name|path
operator|<
name|slash
operator|&&
operator|*
name|slash
operator|!=
literal|'/'
condition|)
name|slash
operator|--
expr_stmt|;
if|if
condition|(
operator|!
name|has_dirs_only_path
argument_list|(
name|path
argument_list|,
name|slash
operator|-
name|path
argument_list|,
name|skiplen
argument_list|)
condition|)
block|{
name|errno
operator|=
name|ENOENT
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
return|return
name|lstat
argument_list|(
name|path
argument_list|,
name|st
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|checkout_entry
name|int
name|checkout_entry
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
specifier|const
name|struct
name|checkout
modifier|*
name|state
parameter_list|,
name|char
modifier|*
name|topath
parameter_list|)
block|{
specifier|static
name|char
name|path
index|[
name|PATH_MAX
operator|+
literal|1
index|]
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|int
name|len
init|=
name|state
operator|->
name|base_dir_len
decl_stmt|;
if|if
condition|(
name|topath
condition|)
return|return
name|write_entry
argument_list|(
name|ce
argument_list|,
name|topath
argument_list|,
name|state
argument_list|,
literal|1
argument_list|)
return|;
name|memcpy
argument_list|(
name|path
argument_list|,
name|state
operator|->
name|base_dir
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
name|len
operator|+=
name|ce_namelen
argument_list|(
name|ce
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|check_path
argument_list|(
name|path
argument_list|,
name|len
argument_list|,
operator|&
name|st
argument_list|,
name|state
operator|->
name|base_dir_len
argument_list|)
condition|)
block|{
name|unsigned
name|changed
init|=
name|ce_match_stat
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|,
name|CE_MATCH_IGNORE_VALID
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|changed
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|state
operator|->
name|force
condition|)
block|{
if|if
condition|(
operator|!
name|state
operator|->
name|quiet
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"git-checkout-index: %s already exists\n"
argument_list|,
name|path
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
comment|/* 		 * We unlink the old file, to get the new one with the 		 * right permissions (including umask, which is nasty 		 * to emulate by hand - much easier to let the system 		 * just do the right thing) 		 */
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
comment|/* If it is a gitlink, leave it alone! */
if|if
condition|(
name|S_ISGITLINK
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|state
operator|->
name|force
condition|)
return|return
name|error
argument_list|(
literal|"%s is a directory"
argument_list|,
name|path
argument_list|)
return|;
name|remove_subtree
argument_list|(
name|path
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|unlink
argument_list|(
name|path
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"unable to unlink old '%s' (%s)"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
block|}
elseif|else
if|if
condition|(
name|state
operator|->
name|not_new
condition|)
return|return
literal|0
return|;
name|create_directories
argument_list|(
name|path
argument_list|,
name|len
argument_list|,
name|state
argument_list|)
expr_stmt|;
return|return
name|write_entry
argument_list|(
name|ce
argument_list|,
name|path
argument_list|,
name|state
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

end_unit

