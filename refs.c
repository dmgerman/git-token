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

begin_function
DECL|function|read_ref
specifier|static
name|int
name|read_ref
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
parameter_list|)
block|{
name|int
name|ret
init|=
operator|-
literal|1
decl_stmt|;
name|int
name|fd
init|=
name|open
argument_list|(
name|path
argument_list|,
name|O_RDONLY
argument_list|)
decl_stmt|;
if|if
condition|(
name|fd
operator|>=
literal|0
condition|)
block|{
name|char
name|buffer
index|[
literal|60
index|]
decl_stmt|;
if|if
condition|(
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
argument_list|)
operator|>=
literal|40
condition|)
name|ret
operator|=
name|get_sha1_hex
argument_list|(
name|buffer
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
block|}
return|return
name|ret
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
name|base
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
name|path
argument_list|,
name|sha1
argument_list|)
operator|<
literal|0
condition|)
continue|continue;
if|if
condition|(
operator|!
name|has_sha1_file
argument_list|(
name|sha1
argument_list|)
condition|)
continue|continue;
name|retval
operator|=
name|fn
argument_list|(
name|path
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
specifier|const
name|char
modifier|*
name|headpath
init|=
name|git_path
argument_list|(
literal|"HEAD"
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|read_ref
argument_list|(
name|headpath
argument_list|,
name|sha1
argument_list|)
condition|)
name|fn
argument_list|(
name|headpath
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
return|return
name|do_for_each_ref
argument_list|(
name|get_refs_directory
argument_list|()
argument_list|,
name|fn
argument_list|)
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
name|get_refs_directory
argument_list|()
argument_list|,
name|fn
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ref_file_name
specifier|static
name|char
modifier|*
name|ref_file_name
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|)
block|{
name|char
modifier|*
name|base
init|=
name|get_refs_directory
argument_list|()
decl_stmt|;
name|int
name|baselen
init|=
name|strlen
argument_list|(
name|base
argument_list|)
decl_stmt|;
name|int
name|reflen
init|=
name|strlen
argument_list|(
name|ref
argument_list|)
decl_stmt|;
name|char
modifier|*
name|ret
init|=
name|xmalloc
argument_list|(
name|baselen
operator|+
literal|2
operator|+
name|reflen
argument_list|)
decl_stmt|;
name|sprintf
argument_list|(
name|ret
argument_list|,
literal|"%s/%s"
argument_list|,
name|base
argument_list|,
name|ref
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|ref_lock_file_name
specifier|static
name|char
modifier|*
name|ref_lock_file_name
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|)
block|{
name|char
modifier|*
name|base
init|=
name|get_refs_directory
argument_list|()
decl_stmt|;
name|int
name|baselen
init|=
name|strlen
argument_list|(
name|base
argument_list|)
decl_stmt|;
name|int
name|reflen
init|=
name|strlen
argument_list|(
name|ref
argument_list|)
decl_stmt|;
name|char
modifier|*
name|ret
init|=
name|xmalloc
argument_list|(
name|baselen
operator|+
literal|7
operator|+
name|reflen
argument_list|)
decl_stmt|;
name|sprintf
argument_list|(
name|ret
argument_list|,
literal|"%s/%s.lock"
argument_list|,
name|base
argument_list|,
name|ref
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|read_ref_file
specifier|static
name|int
name|read_ref_file
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
name|int
name|fd
init|=
name|open
argument_list|(
name|filename
argument_list|,
name|O_RDONLY
argument_list|)
decl_stmt|;
name|char
name|hex
index|[
literal|41
index|]
decl_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
block|{
return|return
name|error
argument_list|(
literal|"Couldn't open %s\n"
argument_list|,
name|filename
argument_list|)
return|;
block|}
if|if
condition|(
operator|(
name|read
argument_list|(
name|fd
argument_list|,
name|hex
argument_list|,
literal|41
argument_list|)
operator|<
literal|41
operator|)
operator|||
operator|(
name|hex
index|[
literal|40
index|]
operator|!=
literal|'\n'
operator|)
operator|||
name|get_sha1_hex
argument_list|(
name|hex
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"Couldn't read a hash from %s\n"
argument_list|,
name|filename
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
return|return
literal|0
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
name|char
modifier|*
name|filename
decl_stmt|;
name|int
name|retval
decl_stmt|;
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
name|filename
operator|=
name|ref_file_name
argument_list|(
name|ref
argument_list|)
expr_stmt|;
name|retval
operator|=
name|read_ref_file
argument_list|(
name|filename
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|filename
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|lock_ref_file
specifier|static
name|int
name|lock_ref_file
parameter_list|(
specifier|const
name|char
modifier|*
name|filename
parameter_list|,
specifier|const
name|char
modifier|*
name|lock_filename
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|)
block|{
name|int
name|fd
init|=
name|open
argument_list|(
name|lock_filename
argument_list|,
name|O_WRONLY
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0666
argument_list|)
decl_stmt|;
name|unsigned
name|char
name|current_sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|retval
decl_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
block|{
return|return
name|error
argument_list|(
literal|"Couldn't open lock file for %s: %s"
argument_list|,
name|filename
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
block|}
name|retval
operator|=
name|read_ref_file
argument_list|(
name|filename
argument_list|,
name|current_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|old_sha1
condition|)
block|{
if|if
condition|(
name|retval
condition|)
block|{
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|lock_filename
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"Could not read the current value of %s"
argument_list|,
name|filename
argument_list|)
return|;
block|}
if|if
condition|(
name|memcmp
argument_list|(
name|current_sha1
argument_list|,
name|old_sha1
argument_list|,
literal|20
argument_list|)
condition|)
block|{
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|lock_filename
argument_list|)
expr_stmt|;
name|error
argument_list|(
literal|"The current value of %s is %s"
argument_list|,
name|filename
argument_list|,
name|sha1_to_hex
argument_list|(
name|current_sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"Expected %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|old_sha1
argument_list|)
argument_list|)
return|;
block|}
block|}
else|else
block|{
if|if
condition|(
operator|!
name|retval
condition|)
block|{
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|lock_filename
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"Unexpectedly found a value of %s for %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|current_sha1
argument_list|)
argument_list|,
name|filename
argument_list|)
return|;
block|}
block|}
return|return
name|fd
return|;
block|}
end_function

begin_function
DECL|function|lock_ref_sha1
name|int
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
parameter_list|)
block|{
name|char
modifier|*
name|filename
decl_stmt|;
name|char
modifier|*
name|lock_filename
decl_stmt|;
name|int
name|retval
decl_stmt|;
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
name|filename
operator|=
name|ref_file_name
argument_list|(
name|ref
argument_list|)
expr_stmt|;
name|lock_filename
operator|=
name|ref_lock_file_name
argument_list|(
name|ref
argument_list|)
expr_stmt|;
name|retval
operator|=
name|lock_ref_file
argument_list|(
name|filename
argument_list|,
name|lock_filename
argument_list|,
name|old_sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|filename
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|lock_filename
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

begin_function
DECL|function|write_ref_file
specifier|static
name|int
name|write_ref_file
parameter_list|(
specifier|const
name|char
modifier|*
name|filename
parameter_list|,
specifier|const
name|char
modifier|*
name|lock_filename
parameter_list|,
name|int
name|fd
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|char
modifier|*
name|hex
init|=
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|char
name|term
init|=
literal|'\n'
decl_stmt|;
if|if
condition|(
name|write
argument_list|(
name|fd
argument_list|,
name|hex
argument_list|,
literal|40
argument_list|)
operator|<
literal|40
operator|||
name|write
argument_list|(
name|fd
argument_list|,
operator|&
name|term
argument_list|,
literal|1
argument_list|)
operator|<
literal|1
condition|)
block|{
name|error
argument_list|(
literal|"Couldn't write %s\n"
argument_list|,
name|filename
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|rename
argument_list|(
name|lock_filename
argument_list|,
name|filename
argument_list|)
expr_stmt|;
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
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
name|int
name|fd
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|char
modifier|*
name|filename
decl_stmt|;
name|char
modifier|*
name|lock_filename
decl_stmt|;
name|int
name|retval
decl_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
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
name|filename
operator|=
name|ref_file_name
argument_list|(
name|ref
argument_list|)
expr_stmt|;
name|lock_filename
operator|=
name|ref_lock_file_name
argument_list|(
name|ref
argument_list|)
expr_stmt|;
name|retval
operator|=
name|write_ref_file
argument_list|(
name|filename
argument_list|,
name|lock_filename
argument_list|,
name|fd
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|filename
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|lock_filename
argument_list|)
expr_stmt|;
return|return
name|retval
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
name|char
modifier|*
name|middle
decl_stmt|;
if|if
condition|(
name|ref
index|[
literal|0
index|]
operator|==
literal|'.'
operator|||
name|ref
index|[
literal|0
index|]
operator|==
literal|'/'
condition|)
return|return
operator|-
literal|1
return|;
name|middle
operator|=
name|strchr
argument_list|(
name|ref
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|middle
operator|||
operator|!
name|middle
index|[
literal|1
index|]
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|strchr
argument_list|(
name|middle
operator|+
literal|1
argument_list|,
literal|'/'
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|write_ref_sha1_unlocked
name|int
name|write_ref_sha1_unlocked
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
name|sha1
parameter_list|)
block|{
name|char
modifier|*
name|filename
decl_stmt|;
name|char
modifier|*
name|lock_filename
decl_stmt|;
name|int
name|fd
decl_stmt|;
name|int
name|retval
decl_stmt|;
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
name|filename
operator|=
name|ref_file_name
argument_list|(
name|ref
argument_list|)
expr_stmt|;
name|lock_filename
operator|=
name|ref_lock_file_name
argument_list|(
name|ref
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|lock_filename
argument_list|,
name|O_WRONLY
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
block|{
name|error
argument_list|(
literal|"Writing %s"
argument_list|,
name|lock_filename
argument_list|)
expr_stmt|;
name|perror
argument_list|(
literal|"Open"
argument_list|)
expr_stmt|;
block|}
name|retval
operator|=
name|write_ref_file
argument_list|(
name|filename
argument_list|,
name|lock_filename
argument_list|,
name|fd
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|filename
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|lock_filename
argument_list|)
expr_stmt|;
return|return
name|retval
return|;
block|}
end_function

end_unit

