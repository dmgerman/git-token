begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|LOCKFILE_H
end_ifndef

begin_define
DECL|macro|LOCKFILE_H
define|#
directive|define
name|LOCKFILE_H
end_define

begin_comment
comment|/*  * File write-locks as used by Git.  *  * The lockfile API serves two purposes:  *  * * Mutual exclusion and atomic file updates. When we want to change  *   a file, we create a lockfile `<filename>.lock`, write the new  *   file contents into it, and then rename the lockfile to its final  *   destination `<filename>`. We create the `<filename>.lock` file  *   with `O_CREAT|O_EXCL` so that we can notice and fail if somebody  *   else has already locked the file, then atomically rename the  *   lockfile to its final destination to commit the changes and  *   unlock the file.  *  * * Automatic cruft removal. If the program exits after we lock a  *   file but before the changes have been committed, we want to make  *   sure that we remove the lockfile. This is done by remembering the  *   lockfiles we have created in a linked list and setting up an  *   `atexit(3)` handler and a signal handler that clean up the  *   lockfiles. This mechanism ensures that outstanding lockfiles are  *   cleaned up if the program exits (including when `die()` is  *   called) or if the program is terminated by a signal.  *  * Please note that lockfiles only block other writers. Readers do not  * block, but they are guaranteed to see either the old contents of  * the file or the new contents of the file (assuming that the  * filesystem implements `rename(2)` atomically).  *  *  * Calling sequence  * ----------------  *  * The caller:  *  * * Allocates a `struct lock_file` either as a static variable or on  *   the heap, initialized to zeros. Once you use the structure to  *   call the `hold_lock_file_for_*()` family of functions, it belongs  *   to the lockfile subsystem and its storage must remain valid  *   throughout the life of the program (i.e. you cannot use an  *   on-stack variable to hold this structure).  *  * * Attempts to create a lockfile by calling  *   `hold_lock_file_for_update()` or `hold_lock_file_for_append()`.  *  * * Writes new content for the destination file by either:  *  *   * writing to the file descriptor returned by the  *     `hold_lock_file_for_*()` functions (also available via  *     `lock->fd`).  *  *   * calling `fdopen_lock_file()` to get a `FILE` pointer for the  *     open file and writing to the file using stdio.  *  * When finished writing, the caller can:  *  * * Close the file descriptor and rename the lockfile to its final  *   destination by calling `commit_lock_file()` or  *   `commit_lock_file_to()`.  *  * * Close the file descriptor and remove the lockfile by calling  *   `rollback_lock_file()`.  *  * * Close the file descriptor without removing or renaming the  *   lockfile by calling `close_lock_file()`, and later call  *   `commit_lock_file()`, `commit_lock_file_to()`,  *   `rollback_lock_file()`, or `reopen_lock_file()`.  *  * Even after the lockfile is committed or rolled back, the  * `lock_file` object must not be freed or altered by the caller.  * However, it may be reused; just pass it to another call of  * `hold_lock_file_for_update()` or `hold_lock_file_for_append()`.  *  * If the program exits before `commit_lock_file()`,  * `commit_lock_file_to()`, or `rollback_lock_file()` is called, an  * `atexit(3)` handler will close and remove the lockfile, thereby  * rolling back any uncommitted changes.  *  * If you need to close the file descriptor you obtained from a  * `hold_lock_file_for_*()` function yourself, do so by calling  * `close_lock_file()`. You should never call `close(2)` or  * `fclose(3)` yourself, otherwise the `struct lock_file` structure  * would still think that the file descriptor needs to be closed, and  * a commit or rollback would result in duplicate calls to `close(2)`.  * Worse yet, if you close and then later open another file descriptor  * for a completely different purpose, then a commit or rollback might  * close that unrelated file descriptor.  *  * Error handling  * --------------  *  * The `hold_lock_file_for_*()` functions return a file descriptor on  * success or -1 on failure (unless `LOCK_DIE_ON_ERROR` is used; see  * "flags" below). On errors, `errno` describes the reason for  * failure. Errors can be reported by passing `errno` to  * `unable_to_lock_message()` or `unable_to_lock_die()`.  *  * Similarly, `commit_lock_file`, `commit_lock_file_to`, and  * `close_lock_file` return 0 on success. On failure they set `errno`  * appropriately, do their best to roll back the lockfile, and return  * -1.  */
end_comment

begin_struct
DECL|struct|lock_file
struct|struct
name|lock_file
block|{
DECL|member|next
name|struct
name|lock_file
modifier|*
specifier|volatile
name|next
decl_stmt|;
DECL|member|active
specifier|volatile
name|sig_atomic_t
name|active
decl_stmt|;
DECL|member|fd
specifier|volatile
name|int
name|fd
decl_stmt|;
DECL|member|fp
name|FILE
modifier|*
specifier|volatile
name|fp
decl_stmt|;
DECL|member|owner
specifier|volatile
name|pid_t
name|owner
decl_stmt|;
DECL|member|on_list
name|char
name|on_list
decl_stmt|;
DECL|member|filename
name|struct
name|strbuf
name|filename
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* String appended to a filename to derive the lockfile name: */
end_comment

begin_define
DECL|macro|LOCK_SUFFIX
define|#
directive|define
name|LOCK_SUFFIX
value|".lock"
end_define

begin_define
DECL|macro|LOCK_SUFFIX_LEN
define|#
directive|define
name|LOCK_SUFFIX_LEN
value|5
end_define

begin_comment
comment|/*  * Flags  * -----  *  * The following flags can be passed to `hold_lock_file_for_update()`  * or `hold_lock_file_for_append()`.  */
end_comment

begin_comment
comment|/*  * If a lock is already taken for the file, `die()` with an error  * message. If this flag is not specified, trying to lock a file that  * is already locked returns -1 to the caller.  */
end_comment

begin_define
DECL|macro|LOCK_DIE_ON_ERROR
define|#
directive|define
name|LOCK_DIE_ON_ERROR
value|1
end_define

begin_comment
comment|/*  * Usually symbolic links in the destination path are resolved. This  * means that (1) the lockfile is created by adding ".lock" to the  * resolved path, and (2) upon commit, the resolved path is  * overwritten. However, if `LOCK_NO_DEREF` is set, then the lockfile  * is created by adding ".lock" to the path argument itself. This  * option is used, for example, when detaching a symbolic reference,  * which for backwards-compatibility reasons, can be a symbolic link  * containing the name of the referred-to-reference.  */
end_comment

begin_define
DECL|macro|LOCK_NO_DEREF
define|#
directive|define
name|LOCK_NO_DEREF
value|2
end_define

begin_comment
comment|/*  * Attempt to create a lockfile for the file at `path` and return a  * file descriptor for writing to it, or -1 on error. If the file is  * currently locked, retry with quadratic backoff for at least  * timeout_ms milliseconds. If timeout_ms is 0, try exactly once; if  * timeout_ms is -1, retry indefinitely. The flags argument and error  * handling are described above.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|hold_lock_file_for_update_timeout
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
parameter_list|,
name|long
name|timeout_ms
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Attempt to create a lockfile for the file at `path` and return a  * file descriptor for writing to it, or -1 on error. The flags  * argument and error handling are described above.  */
end_comment

begin_function
DECL|function|hold_lock_file_for_update
specifier|static
specifier|inline
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
return|return
name|hold_lock_file_for_update_timeout
argument_list|(
name|lk
argument_list|,
name|path
argument_list|,
name|flags
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * Like `hold_lock_file_for_update()`, but before returning copy the  * existing contents of the file (if any) to the lockfile and position  * its write pointer at the end of the file. The flags argument and  * error handling are described above.  */
end_comment

begin_function_decl
specifier|extern
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
function_decl|;
end_function_decl

begin_comment
comment|/*  * Append an appropriate error message to `buf` following the failure  * of `hold_lock_file_for_update()` or `hold_lock_file_for_append()`  * to lock `path`. `err` should be the `errno` set by the failing  * call.  */
end_comment

begin_function_decl
specifier|extern
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
function_decl|;
end_function_decl

begin_comment
comment|/*  * Emit an appropriate error message and `die()` following the failure  * of `hold_lock_file_for_update()` or `hold_lock_file_for_append()`  * to lock `path`. `err` should be the `errno` set by the failing  * call.  */
end_comment

begin_function_decl
specifier|extern
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
function_decl|;
end_function_decl

begin_comment
comment|/*  * Associate a stdio stream with the lockfile (which must still be  * open). Return `NULL` (*without* rolling back the lockfile) on  * error. The stream is closed automatically when `close_lock_file()`  * is called or when the file is committed or rolled back.  */
end_comment

begin_function_decl
specifier|extern
name|FILE
modifier|*
name|fdopen_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|,
specifier|const
name|char
modifier|*
name|mode
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Return the path of the lockfile. The return value is a pointer to a  * field within the lock_file object and should not be freed.  */
end_comment

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|get_lock_file_path
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|get_lock_file_fd
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|FILE
modifier|*
name|get_lock_file_fp
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Return the path of the file that is locked by the specified  * lock_file object. The caller must free the memory.  */
end_comment

begin_function_decl
specifier|extern
name|char
modifier|*
name|get_locked_file_path
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * If the lockfile is still open, close it (and the file pointer if it  * has been opened using `fdopen_lock_file()`) without renaming the  * lockfile over the file being locked. Return 0 upon success. On  * failure to `close(2)`, return a negative value and roll back the  * lock file. Usually `commit_lock_file()`, `commit_lock_file_to()`,  * or `rollback_lock_file()` should eventually be called if  * `close_lock_file()` succeeds.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|close_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Re-open a lockfile that has been closed using `close_lock_file()`  * but not yet committed or rolled back. This can be used to implement  * a sequence of operations like the following:  *  * * Lock file.  *  * * Write new contents to lockfile, then `close_lock_file()` to  *   cause the contents to be written to disk.  *  * * Pass the name of the lockfile to another program to allow it (and  *   nobody else) to inspect the contents you wrote, while still  *   holding the lock yourself.  *  * * `reopen_lock_file()` to reopen the lockfile. Make further updates  *   to the contents.  *  * * `commit_lock_file()` to make the final version permanent.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|reopen_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Commit the change represented by `lk`: close the file descriptor  * and/or file pointer if they are still open and rename the lockfile  * to its final destination. Return 0 upon success. On failure, roll  * back the lock file and return -1, with `errno` set to the value  * from the failing call to `close(2)` or `rename(2)`. It is a bug to  * call `commit_lock_file()` for a `lock_file` object that is not  * currently locked.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|commit_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Like `commit_lock_file()`, but rename the lockfile to the provided  * `path`. `path` must be on the same filesystem as the lock file.  */
end_comment

begin_function_decl
specifier|extern
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
function_decl|;
end_function_decl

begin_comment
comment|/*  * Roll back `lk`: close the file descriptor and/or file pointer and  * remove the lockfile. It is a NOOP to call `rollback_lock_file()`  * for a `lock_file` object that has already been committed or rolled  * back.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|rollback_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
name|lk
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* LOCKFILE_H */
end_comment

end_unit

