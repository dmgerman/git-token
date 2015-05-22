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
comment|/*  * File write-locks as used by Git.  *  * For an overview of how to use the lockfile API, please see  *  *     Documentation/technical/api-lockfile.txt  *  * This module keeps track of all locked files in lock_file_list for  * use at cleanup. This list and the lock_file objects that comprise  * it must be kept in self-consistent states at all time, because the  * program can be interrupted any time by a signal, in which case the  * signal handler will walk through the list attempting to clean up  * any open lock files.  *  * A lockfile is owned by the process that created it. The lock_file  * object has an "owner" field that records its owner. This field is  * used to prevent a forked process from closing a lockfile created by  * its parent.  *  * The possible states of a lock_file object are as follows:  *  * - Uninitialized.  In this state the object's on_list field must be  *   zero but the rest of its contents need not be initialized.  As  *   soon as the object is used in any way, it is irrevocably  *   registered in the lock_file_list, and on_list is set.  *  * - Locked, lockfile open (after hold_lock_file_for_update(),  *   hold_lock_file_for_append(), or reopen_lock_file()). In this  *   state:  *   - the lockfile exists  *   - active is set  *   - filename holds the filename of the lockfile  *   - fd holds a file descriptor open for writing to the lockfile  *   - fp holds a pointer to an open FILE object if and only if  *     fdopen_lock_file() has been called on the object  *   - owner holds the PID of the process that locked the file  *  * - Locked, lockfile closed (after successful close_lock_file()).  *   Same as the previous state, except that the lockfile is closed  *   and fd is -1.  *  * - Unlocked (after commit_lock_file(), commit_lock_file_to(),  *   rollback_lock_file(), a failed attempt to lock, or a failed  *   close_lock_file()).  In this state:  *   - active is unset  *   - filename is empty (usually, though there are transitory  *     states in which this condition doesn't hold). Client code should  *     *not* rely on the filename being empty in this state.  *   - fd is -1  *   - the object is left registered in the lock_file_list, and  *     on_list is set.  */
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

begin_define
DECL|macro|LOCK_DIE_ON_ERROR
define|#
directive|define
name|LOCK_DIE_ON_ERROR
value|1
end_define

begin_define
DECL|macro|LOCK_NO_DEREF
define|#
directive|define
name|LOCK_NO_DEREF
value|2
end_define

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

begin_function_decl
specifier|extern
name|FILE
modifier|*
name|fdopen_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|mode
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|get_locked_file_path
parameter_list|(
name|struct
name|lock_file
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|commit_lock_file_to
parameter_list|(
name|struct
name|lock_file
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|commit_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|reopen_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|close_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|rollback_lock_file
parameter_list|(
name|struct
name|lock_file
modifier|*
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

