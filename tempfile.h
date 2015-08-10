begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|TEMPFILE_H
end_ifndef

begin_define
DECL|macro|TEMPFILE_H
define|#
directive|define
name|TEMPFILE_H
end_define

begin_comment
comment|/*  * Handle temporary files.  *  * The tempfile API allows temporary files to be created, deleted, and  * atomically renamed. Temporary files that are still active when the  * program ends are cleaned up automatically. Lockfiles (see  * "lockfile.h") are built on top of this API.  *  *  * Calling sequence  * ----------------  *  * The caller:  *  * * Allocates a `struct tempfile` either as a static variable or on  *   the heap, initialized to zeros. Once you use the structure to  *   call `create_tempfile()`, it belongs to the tempfile subsystem  *   and its storage must remain valid throughout the life of the  *   program (i.e. you cannot use an on-stack variable to hold this  *   structure).  *  * * Attempts to create a temporary file by calling  *   `create_tempfile()`.  *  * * Writes new content to the file by either:  *  *   * writing to the file descriptor returned by `create_tempfile()`  *     (also available via `tempfile->fd`).  *  *   * calling `fdopen_tempfile()` to get a `FILE` pointer for the  *     open file and writing to the file using stdio.  *  * When finished writing, the caller can:  *  * * Close the file descriptor and remove the temporary file by  *   calling `delete_tempfile()`.  *  * * Close the temporary file and rename it atomically to a specified  *   filename by calling `rename_tempfile()`. This relinquishes  *   control of the file.  *  * * Close the file descriptor without removing or renaming the  *   temporary file by calling `close_tempfile()`, and later call  *   `delete_tempfile()` or `rename_tempfile()`.  *  * Even after the temporary file is renamed or deleted, the `tempfile`  * object must not be freed or altered by the caller. However, it may  * be reused; just pass it to another call of `create_tempfile()`.  *  * If the program exits before `rename_tempfile()` or  * `delete_tempfile()` is called, an `atexit(3)` handler will close  * and remove the temporary file.  *  * If you need to close the file descriptor yourself, do so by calling  * `close_tempfile()`. You should never call `close(2)` or `fclose(3)`  * yourself, otherwise the `struct tempfile` structure would still  * think that the file descriptor needs to be closed, and a later  * cleanup would result in duplicate calls to `close(2)`. Worse yet,  * if you close and then later open another file descriptor for a  * completely different purpose, then the unrelated file descriptor  * might get closed.  *  *  * Error handling  * --------------  *  * `create_tempfile()` returns a file descriptor on success or -1 on  * failure. On errors, `errno` describes the reason for failure.  *  * `delete_tempfile()`, `rename_tempfile()`, and `close_tempfile()`  * return 0 on success. On failure they set `errno` appropriately, do  * their best to delete the temporary file, and return -1.  */
end_comment

begin_struct
DECL|struct|tempfile
struct|struct
name|tempfile
block|{
DECL|member|next
name|struct
name|tempfile
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
comment|/*  * Attempt to create a temporary file at the specified `path`. Return  * a file descriptor for writing to it, or -1 on error. It is an error  * if a file already exists at that path.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|create_tempfile
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * mks_tempfile functions  *  * The following functions attempt to create and open temporary files  * with names derived automatically from a template, in the manner of  * mkstemps(), and arrange for them to be deleted if the program ends  * before they are deleted explicitly. There is a whole family of such  * functions, named according to the following pattern:  *  *     x?mks_tempfile_t?s?m?()  *  * The optional letters have the following meanings:  *  *   x - die if the temporary file cannot be created.  *  *   t - create the temporary file under $TMPDIR (as opposed to  *       relative to the current directory). When these variants are  *       used, template should be the pattern for the filename alone,  *       without a path.  *  *   s - template includes a suffix that is suffixlen characters long.  *  *   m - the temporary file should be created with the specified mode  *       (otherwise, the mode is set to 0600).  *  * None of these functions modify template. If the caller wants to  * know the (absolute) path of the file that was created, it can be  * read from tempfile->filename.  *  * On success, the functions return a file descriptor that is open for  * writing the temporary file. On errors, they return -1 and set errno  * appropriately (except for the "x" variants, which die() on errors).  */
end_comment

begin_comment
comment|/* See "mks_tempfile functions" above. */
end_comment

begin_function_decl
specifier|extern
name|int
name|mks_tempfile_sm
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|,
name|int
name|suffixlen
parameter_list|,
name|int
name|mode
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* See "mks_tempfile functions" above. */
end_comment

begin_function
DECL|function|mks_tempfile_s
specifier|static
specifier|inline
name|int
name|mks_tempfile_s
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|,
name|int
name|suffixlen
parameter_list|)
block|{
return|return
name|mks_tempfile_sm
argument_list|(
name|tempfile
argument_list|,
name|template
argument_list|,
name|suffixlen
argument_list|,
literal|0600
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* See "mks_tempfile functions" above. */
end_comment

begin_function
DECL|function|mks_tempfile_m
specifier|static
specifier|inline
name|int
name|mks_tempfile_m
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|,
name|int
name|mode
parameter_list|)
block|{
return|return
name|mks_tempfile_sm
argument_list|(
name|tempfile
argument_list|,
name|template
argument_list|,
literal|0
argument_list|,
name|mode
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* See "mks_tempfile functions" above. */
end_comment

begin_function
DECL|function|mks_tempfile
specifier|static
specifier|inline
name|int
name|mks_tempfile
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|)
block|{
return|return
name|mks_tempfile_sm
argument_list|(
name|tempfile
argument_list|,
name|template
argument_list|,
literal|0
argument_list|,
literal|0600
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* See "mks_tempfile functions" above. */
end_comment

begin_function_decl
specifier|extern
name|int
name|mks_tempfile_tsm
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|,
name|int
name|suffixlen
parameter_list|,
name|int
name|mode
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* See "mks_tempfile functions" above. */
end_comment

begin_function
DECL|function|mks_tempfile_ts
specifier|static
specifier|inline
name|int
name|mks_tempfile_ts
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|,
name|int
name|suffixlen
parameter_list|)
block|{
return|return
name|mks_tempfile_tsm
argument_list|(
name|tempfile
argument_list|,
name|template
argument_list|,
name|suffixlen
argument_list|,
literal|0600
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* See "mks_tempfile functions" above. */
end_comment

begin_function
DECL|function|mks_tempfile_tm
specifier|static
specifier|inline
name|int
name|mks_tempfile_tm
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|,
name|int
name|mode
parameter_list|)
block|{
return|return
name|mks_tempfile_tsm
argument_list|(
name|tempfile
argument_list|,
name|template
argument_list|,
literal|0
argument_list|,
name|mode
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* See "mks_tempfile functions" above. */
end_comment

begin_function
DECL|function|mks_tempfile_t
specifier|static
specifier|inline
name|int
name|mks_tempfile_t
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|)
block|{
return|return
name|mks_tempfile_tsm
argument_list|(
name|tempfile
argument_list|,
name|template
argument_list|,
literal|0
argument_list|,
literal|0600
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* See "mks_tempfile functions" above. */
end_comment

begin_function_decl
specifier|extern
name|int
name|xmks_tempfile_m
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|,
name|int
name|mode
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* See "mks_tempfile functions" above. */
end_comment

begin_function
DECL|function|xmks_tempfile
specifier|static
specifier|inline
name|int
name|xmks_tempfile
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|)
block|{
return|return
name|xmks_tempfile_m
argument_list|(
name|tempfile
argument_list|,
name|template
argument_list|,
literal|0600
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * Associate a stdio stream with the temporary file (which must still  * be open). Return `NULL` (*without* deleting the file) on error. The  * stream is closed automatically when `close_tempfile()` is called or  * when the file is deleted or renamed.  */
end_comment

begin_function_decl
specifier|extern
name|FILE
modifier|*
name|fdopen_tempfile
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|mode
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|is_tempfile_active
specifier|static
specifier|inline
name|int
name|is_tempfile_active
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|)
block|{
return|return
name|tempfile
operator|->
name|active
return|;
block|}
end_function

begin_comment
comment|/*  * Return the path of the lockfile. The return value is a pointer to a  * field within the lock_file object and should not be freed.  */
end_comment

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|get_tempfile_path
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|get_tempfile_fd
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|FILE
modifier|*
name|get_tempfile_fp
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * If the temporary file is still open, close it (and the file pointer  * too, if it has been opened using `fdopen_tempfile()`) without  * deleting the file. Return 0 upon success. On failure to `close(2)`,  * return a negative value and delete the file. Usually  * `delete_tempfile()` or `rename_tempfile()` should eventually be  * called if `close_tempfile()` succeeds.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|close_tempfile
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Re-open a temporary file that has been closed using  * `close_tempfile()` but not yet deleted or renamed. This can be used  * to implement a sequence of operations like the following:  *  * * Create temporary file.  *  * * Write new contents to file, then `close_tempfile()` to cause the  *   contents to be written to disk.  *  * * Pass the name of the temporary file to another program to allow  *   it (and nobody else) to inspect or even modify the file's  *   contents.  *  * * `reopen_tempfile()` to reopen the temporary file. Make further  *   updates to the contents.  *  * * `rename_tempfile()` to move the file to its permanent location.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|reopen_tempfile
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Close the file descriptor and/or file pointer and remove the  * temporary file associated with `tempfile`. It is a NOOP to call  * `delete_tempfile()` for a `tempfile` object that has already been  * deleted or renamed.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|delete_tempfile
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Close the file descriptor and/or file pointer if they are still  * open, and atomically rename the temporary file to `path`. `path`  * must be on the same filesystem as the lock file. Return 0 on  * success. On failure, delete the temporary file and return -1, with  * `errno` set to the value from the failing call to `close(2)` or  * `rename(2)`. It is a bug to call `rename_tempfile()` for a  * `tempfile` object that is not currently active.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|rename_tempfile
parameter_list|(
name|struct
name|tempfile
modifier|*
name|tempfile
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* TEMPFILE_H */
end_comment

end_unit

