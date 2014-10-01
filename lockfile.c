begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2005, Junio C Hamano  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"lockfile.h"
end_include

begin_include
include|#
directive|include
file|"sigchain.h"
end_include

begin_decl_stmt
DECL|variable|lock_file_list
specifier|static
name|struct
name|lock_file
modifier|*
specifier|volatile
name|lock_file_list
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|remove_lock_files
specifier|static
name|void
name|remove_lock_files
parameter_list|(
name|void
parameter_list|)
block|{
name|pid_t
name|me
init|=
name|getpid
argument_list|()
decl_stmt|;
while|while
condition|(
name|lock_file_list
condition|)
block|{
if|if
condition|(
name|lock_file_list
operator|->
name|owner
operator|==
name|me
condition|)
name|rollback_lock_file
argument_list|(
name|lock_file_list
argument_list|)
expr_stmt|;
name|lock_file_list
operator|=
name|lock_file_list
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|remove_lock_files_on_signal
specifier|static
name|void
name|remove_lock_files_on_signal
parameter_list|(
name|int
name|signo
parameter_list|)
block|{
name|remove_lock_files
argument_list|()
expr_stmt|;
name|sigchain_pop
argument_list|(
name|signo
argument_list|)
expr_stmt|;
name|raise
argument_list|(
name|signo
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * path = absolute or relative path name  *  * Remove the last path name element from path (leaving the preceding  * "/", if any).  If path is empty or the root directory ("/"), set  * path to the empty string.  */
end_comment

begin_function
DECL|function|trim_last_path_component
specifier|static
name|void
name|trim_last_path_component
parameter_list|(
name|struct
name|strbuf
modifier|*
name|path
parameter_list|)
block|{
name|int
name|i
init|=
name|path
operator|->
name|len
decl_stmt|;
comment|/* back up past trailing slashes, if any */
while|while
condition|(
name|i
operator|&&
name|path
operator|->
name|buf
index|[
name|i
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
name|i
operator|--
expr_stmt|;
comment|/* 	 * then go backwards until a slash, or the beginning of the 	 * string 	 */
while|while
condition|(
name|i
operator|&&
name|path
operator|->
name|buf
index|[
name|i
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
name|i
operator|--
expr_stmt|;
name|strbuf_setlen
argument_list|(
name|path
argument_list|,
name|i
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/* We allow "recursive" symbolic links. Only within reason, though */
end_comment

begin_define
DECL|macro|MAXDEPTH
define|#
directive|define
name|MAXDEPTH
value|5
end_define

begin_comment
comment|/*  * path contains a path that might be a symlink.  *  * If path is a symlink, attempt to overwrite it with a path to the  * real file or directory (which may or may not exist), following a  * chain of symlinks if necessary.  Otherwise, leave path unmodified.  *  * This is a best-effort routine.  If an error occurs, path will  * either be left unmodified or will name a different symlink in a  * symlink chain that started with the original path.  */
end_comment

begin_function
DECL|function|resolve_symlink
specifier|static
name|void
name|resolve_symlink
parameter_list|(
name|struct
name|strbuf
modifier|*
name|path
parameter_list|)
block|{
name|int
name|depth
init|=
name|MAXDEPTH
decl_stmt|;
specifier|static
name|struct
name|strbuf
name|link
init|=
name|STRBUF_INIT
decl_stmt|;
while|while
condition|(
name|depth
operator|--
condition|)
block|{
if|if
condition|(
name|strbuf_readlink
argument_list|(
operator|&
name|link
argument_list|,
name|path
operator|->
name|buf
argument_list|,
name|path
operator|->
name|len
argument_list|)
operator|<
literal|0
condition|)
break|break;
if|if
condition|(
name|is_absolute_path
argument_list|(
name|link
operator|.
name|buf
argument_list|)
condition|)
comment|/* absolute path simply replaces p */
name|strbuf_reset
argument_list|(
name|path
argument_list|)
expr_stmt|;
else|else
comment|/* 			 * link is a relative path, so replace the 			 * last element of p with it. 			 */
name|trim_last_path_component
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|strbuf_addbuf
argument_list|(
name|path
argument_list|,
operator|&
name|link
argument_list|)
expr_stmt|;
block|}
name|strbuf_reset
argument_list|(
operator|&
name|link
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Make sure errno contains a meaningful value on error */
end_comment

begin_function
DECL|function|lock_file
specifier|static
name|int
name|lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|size_t
name|pathlen
init|=
name|strlen
argument_list|(
name|path
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|lock_file_list
condition|)
block|{
comment|/* One-time initialization */
name|sigchain_push_common
argument_list|(
name|remove_lock_files_on_signal
argument_list|)
expr_stmt|;
name|atexit
argument_list|(
name|remove_lock_files
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|lk
operator|->
name|active
condition|)
name|die
argument_list|(
literal|"BUG: cannot lock_file(\"%s\") using active struct lock_file"
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lk
operator|->
name|on_list
condition|)
block|{
comment|/* Initialize *lk and add it to lock_file_list: */
name|lk
operator|->
name|fd
operator|=
operator|-
literal|1
expr_stmt|;
name|lk
operator|->
name|active
operator|=
literal|0
expr_stmt|;
name|lk
operator|->
name|owner
operator|=
literal|0
expr_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|lk
operator|->
name|filename
argument_list|,
name|pathlen
operator|+
name|LOCK_SUFFIX_LEN
argument_list|)
expr_stmt|;
name|lk
operator|->
name|next
operator|=
name|lock_file_list
expr_stmt|;
name|lock_file_list
operator|=
name|lk
expr_stmt|;
name|lk
operator|->
name|on_list
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|lk
operator|->
name|filename
operator|.
name|len
condition|)
block|{
comment|/* This shouldn't happen, but better safe than sorry. */
name|die
argument_list|(
literal|"BUG: lock_file(\"%s\") called with improperly-reset lock_file object"
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
name|strbuf_add
argument_list|(
operator|&
name|lk
operator|->
name|filename
argument_list|,
name|path
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|LOCK_NO_DEREF
operator|)
condition|)
name|resolve_symlink
argument_list|(
operator|&
name|lk
operator|->
name|filename
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|lk
operator|->
name|filename
argument_list|,
name|LOCK_SUFFIX
argument_list|)
expr_stmt|;
name|lk
operator|->
name|fd
operator|=
name|open
argument_list|(
name|lk
operator|->
name|filename
operator|.
name|buf
argument_list|,
name|O_RDWR
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
name|lk
operator|->
name|fd
operator|<
literal|0
condition|)
block|{
name|strbuf_reset
argument_list|(
operator|&
name|lk
operator|->
name|filename
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|lk
operator|->
name|owner
operator|=
name|getpid
argument_list|()
expr_stmt|;
name|lk
operator|->
name|active
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|adjust_shared_perm
argument_list|(
name|lk
operator|->
name|filename
operator|.
name|buf
argument_list|)
condition|)
block|{
name|int
name|save_errno
init|=
name|errno
decl_stmt|;
name|error
argument_list|(
literal|"cannot fix permission bits on %s"
argument_list|,
name|lk
operator|->
name|filename
operator|.
name|buf
argument_list|)
expr_stmt|;
name|rollback_lock_file
argument_list|(
name|lk
argument_list|)
expr_stmt|;
name|errno
operator|=
name|save_errno
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
return|return
name|lk
operator|->
name|fd
return|;
block|}
end_function

begin_function
DECL|function|unable_to_lock_message
name|void
name|unable_to_lock_message
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|err
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|)
block|{
if|if
condition|(
name|err
operator|==
name|EEXIST
condition|)
block|{
name|strbuf_addf
argument_list|(
name|buf
argument_list|,
literal|"Unable to create '%s.lock': %s.\n\n"
literal|"If no other git process is currently running, this probably means a\n"
literal|"git process crashed in this repository earlier. Make sure no other git\n"
literal|"process is running and remove the file manually to continue."
argument_list|,
name|absolute_path
argument_list|(
name|path
argument_list|)
argument_list|,
name|strerror
argument_list|(
name|err
argument_list|)
argument_list|)
expr_stmt|;
block|}
else|else
name|strbuf_addf
argument_list|(
name|buf
argument_list|,
literal|"Unable to create '%s.lock': %s"
argument_list|,
name|absolute_path
argument_list|(
name|path
argument_list|)
argument_list|,
name|strerror
argument_list|(
name|err
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|unable_to_lock_error
name|int
name|unable_to_lock_error
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|err
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|unable_to_lock_message
argument_list|(
name|path
argument_list|,
name|err
argument_list|,
operator|&
name|buf
argument_list|)
expr_stmt|;
name|error
argument_list|(
literal|"%s"
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|unable_to_lock_die
name|NORETURN
name|void
name|unable_to_lock_die
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|err
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|unable_to_lock_message
argument_list|(
name|path
argument_list|,
name|err
argument_list|,
operator|&
name|buf
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"%s"
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/* This should return a meaningful errno on failure */
end_comment

begin_function
DECL|function|hold_lock_file_for_update
name|int
name|hold_lock_file_for_update
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|int
name|fd
init|=
name|lock_file
argument_list|(
name|lk
argument_list|,
name|path
argument_list|,
name|flags
argument_list|)
decl_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
operator|&&
operator|(
name|flags
operator|&
name|LOCK_DIE_ON_ERROR
operator|)
condition|)
name|unable_to_lock_die
argument_list|(
name|path
argument_list|,
name|errno
argument_list|)
expr_stmt|;
return|return
name|fd
return|;
block|}
end_function

begin_function
DECL|function|hold_lock_file_for_append
name|int
name|hold_lock_file_for_append
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|int
name|fd
decl_stmt|,
name|orig_fd
decl_stmt|;
name|fd
operator|=
name|lock_file
argument_list|(
name|lk
argument_list|,
name|path
argument_list|,
name|flags
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|flags
operator|&
name|LOCK_DIE_ON_ERROR
condition|)
name|unable_to_lock_die
argument_list|(
name|path
argument_list|,
name|errno
argument_list|)
expr_stmt|;
return|return
name|fd
return|;
block|}
name|orig_fd
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
name|orig_fd
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|errno
operator|!=
name|ENOENT
condition|)
block|{
name|int
name|save_errno
init|=
name|errno
decl_stmt|;
if|if
condition|(
name|flags
operator|&
name|LOCK_DIE_ON_ERROR
condition|)
name|die
argument_list|(
literal|"cannot open '%s' for copying"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|rollback_lock_file
argument_list|(
name|lk
argument_list|)
expr_stmt|;
name|error
argument_list|(
literal|"cannot open '%s' for copying"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|errno
operator|=
name|save_errno
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
block|}
elseif|else
if|if
condition|(
name|copy_fd
argument_list|(
name|orig_fd
argument_list|,
name|fd
argument_list|)
condition|)
block|{
name|int
name|save_errno
init|=
name|errno
decl_stmt|;
if|if
condition|(
name|flags
operator|&
name|LOCK_DIE_ON_ERROR
condition|)
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
name|rollback_lock_file
argument_list|(
name|lk
argument_list|)
expr_stmt|;
name|errno
operator|=
name|save_errno
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
return|return
name|fd
return|;
block|}
end_function

begin_function
DECL|function|get_locked_file_path
name|char
modifier|*
name|get_locked_file_path
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
block|{
if|if
condition|(
operator|!
name|lk
operator|->
name|active
condition|)
name|die
argument_list|(
literal|"BUG: get_locked_file_path() called for unlocked object"
argument_list|)
expr_stmt|;
if|if
condition|(
name|lk
operator|->
name|filename
operator|.
name|len
operator|<=
name|LOCK_SUFFIX_LEN
condition|)
name|die
argument_list|(
literal|"BUG: get_locked_file_path() called for malformed lock object"
argument_list|)
expr_stmt|;
return|return
name|xmemdupz
argument_list|(
name|lk
operator|->
name|filename
operator|.
name|buf
argument_list|,
name|lk
operator|->
name|filename
operator|.
name|len
operator|-
name|LOCK_SUFFIX_LEN
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|close_lock_file
name|int
name|close_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
block|{
name|int
name|fd
init|=
name|lk
operator|->
name|fd
decl_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
literal|0
return|;
name|lk
operator|->
name|fd
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|close
argument_list|(
name|fd
argument_list|)
condition|)
block|{
name|int
name|save_errno
init|=
name|errno
decl_stmt|;
name|rollback_lock_file
argument_list|(
name|lk
argument_list|)
expr_stmt|;
name|errno
operator|=
name|save_errno
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|reopen_lock_file
name|int
name|reopen_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
block|{
if|if
condition|(
literal|0
operator|<=
name|lk
operator|->
name|fd
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"BUG: reopen a lockfile that is still open"
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lk
operator|->
name|active
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"BUG: reopen a lockfile that has been committed"
argument_list|)
argument_list|)
expr_stmt|;
name|lk
operator|->
name|fd
operator|=
name|open
argument_list|(
name|lk
operator|->
name|filename
operator|.
name|buf
argument_list|,
name|O_WRONLY
argument_list|)
expr_stmt|;
return|return
name|lk
operator|->
name|fd
return|;
block|}
end_function

begin_function
DECL|function|commit_lock_file_to
name|int
name|commit_lock_file_to
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
if|if
condition|(
operator|!
name|lk
operator|->
name|active
condition|)
name|die
argument_list|(
literal|"BUG: attempt to commit unlocked object to \"%s\""
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|close_lock_file
argument_list|(
name|lk
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|rename
argument_list|(
name|lk
operator|->
name|filename
operator|.
name|buf
argument_list|,
name|path
argument_list|)
condition|)
block|{
name|int
name|save_errno
init|=
name|errno
decl_stmt|;
name|rollback_lock_file
argument_list|(
name|lk
argument_list|)
expr_stmt|;
name|errno
operator|=
name|save_errno
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|lk
operator|->
name|active
operator|=
literal|0
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|lk
operator|->
name|filename
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|commit_lock_file
name|int
name|commit_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
block|{
specifier|static
name|struct
name|strbuf
name|result_file
init|=
name|STRBUF_INIT
decl_stmt|;
name|int
name|err
decl_stmt|;
if|if
condition|(
operator|!
name|lk
operator|->
name|active
condition|)
name|die
argument_list|(
literal|"BUG: attempt to commit unlocked object"
argument_list|)
expr_stmt|;
if|if
condition|(
name|lk
operator|->
name|filename
operator|.
name|len
operator|<=
name|LOCK_SUFFIX_LEN
operator|||
name|strcmp
argument_list|(
name|lk
operator|->
name|filename
operator|.
name|buf
operator|+
name|lk
operator|->
name|filename
operator|.
name|len
operator|-
name|LOCK_SUFFIX_LEN
argument_list|,
name|LOCK_SUFFIX
argument_list|)
condition|)
name|die
argument_list|(
literal|"BUG: lockfile filename corrupt"
argument_list|)
expr_stmt|;
comment|/* remove ".lock": */
name|strbuf_add
argument_list|(
operator|&
name|result_file
argument_list|,
name|lk
operator|->
name|filename
operator|.
name|buf
argument_list|,
name|lk
operator|->
name|filename
operator|.
name|len
operator|-
name|LOCK_SUFFIX_LEN
argument_list|)
expr_stmt|;
name|err
operator|=
name|commit_lock_file_to
argument_list|(
name|lk
argument_list|,
name|result_file
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|result_file
argument_list|)
expr_stmt|;
return|return
name|err
return|;
block|}
end_function

begin_function
DECL|function|rollback_lock_file
name|void
name|rollback_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
block|{
if|if
condition|(
operator|!
name|lk
operator|->
name|active
condition|)
return|return;
if|if
condition|(
operator|!
name|close_lock_file
argument_list|(
name|lk
argument_list|)
condition|)
block|{
name|unlink_or_warn
argument_list|(
name|lk
operator|->
name|filename
operator|.
name|buf
argument_list|)
expr_stmt|;
name|lk
operator|->
name|active
operator|=
literal|0
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|lk
operator|->
name|filename
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit

