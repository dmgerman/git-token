begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|RUN_COMMAND_H
end_ifndef

begin_define
DECL|macro|RUN_COMMAND_H
define|#
directive|define
name|RUN_COMMAND_H
end_define

begin_struct
DECL|struct|child_process
struct|struct
name|child_process
block|{
DECL|member|argv
specifier|const
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
DECL|member|pid
name|pid_t
name|pid
decl_stmt|;
comment|/* 	 * Using .in, .out, .err: 	 * - Specify 0 for no redirections (child inherits stdin, stdout, 	 *   stderr from parent). 	 * - Specify -1 to have a pipe allocated as follows: 	 *     .in: returns the writable pipe end; parent writes to it, 	 *          the readable pipe end becomes child's stdin 	 *     .out, .err: returns the readable pipe end; parent reads from 	 *          it, the writable pipe end becomes child's stdout/stderr 	 *   The caller of start_command() must close the returned FDs 	 *   after it has completed reading from/writing to it! 	 * - Specify> 0 to set a channel to a particular FD as follows: 	 *     .in: a readable FD, becomes child's stdin 	 *     .out: a writable FD, becomes child's stdout/stderr 	 *     .err: a writable FD, becomes child's stderr 	 *   The specified FD is closed by start_command(), even in case 	 *   of errors! 	 */
DECL|member|in
name|int
name|in
decl_stmt|;
DECL|member|out
name|int
name|out
decl_stmt|;
DECL|member|err
name|int
name|err
decl_stmt|;
DECL|member|dir
specifier|const
name|char
modifier|*
name|dir
decl_stmt|;
DECL|member|env
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|env
decl_stmt|;
DECL|member|no_stdin
name|unsigned
name|no_stdin
range|:
literal|1
decl_stmt|;
DECL|member|no_stdout
name|unsigned
name|no_stdout
range|:
literal|1
decl_stmt|;
DECL|member|no_stderr
name|unsigned
name|no_stderr
range|:
literal|1
decl_stmt|;
DECL|member|git_cmd
name|unsigned
name|git_cmd
range|:
literal|1
decl_stmt|;
comment|/* if this is to be git sub-command */
DECL|member|silent_exec_failure
name|unsigned
name|silent_exec_failure
range|:
literal|1
decl_stmt|;
DECL|member|stdout_to_stderr
name|unsigned
name|stdout_to_stderr
range|:
literal|1
decl_stmt|;
DECL|member|preexec_cb
name|void
function_decl|(
modifier|*
name|preexec_cb
function_decl|)
parameter_list|(
name|void
parameter_list|)
function_decl|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|start_command
parameter_list|(
name|struct
name|child_process
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|finish_command
parameter_list|(
name|struct
name|child_process
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|run_command
parameter_list|(
name|struct
name|child_process
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|run_hook
parameter_list|(
specifier|const
name|char
modifier|*
name|index_file
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|RUN_COMMAND_NO_STDIN
define|#
directive|define
name|RUN_COMMAND_NO_STDIN
value|1
end_define

begin_define
DECL|macro|RUN_GIT_CMD
define|#
directive|define
name|RUN_GIT_CMD
value|2
end_define

begin_comment
DECL|macro|RUN_GIT_CMD
comment|/*If this is to be git sub-command */
end_comment

begin_define
DECL|macro|RUN_COMMAND_STDOUT_TO_STDERR
define|#
directive|define
name|RUN_COMMAND_STDOUT_TO_STDERR
value|4
end_define

begin_define
DECL|macro|RUN_SILENT_EXEC_FAILURE
define|#
directive|define
name|RUN_SILENT_EXEC_FAILURE
value|8
end_define

begin_function_decl
name|int
name|run_command_v_opt
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|int
name|opt
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * env (the environment) is to be formatted like environ: "VAR=VALUE".  * To unset an environment variable use just "VAR".  */
end_comment

begin_function_decl
name|int
name|run_command_v_opt_cd_env
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|int
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|env
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * The purpose of the following functions is to feed a pipe by running  * a function asynchronously and providing output that the caller reads.  *  * It is expected that no synchronization and mutual exclusion between  * the caller and the feed function is necessary so that the function  * can run in a thread without interfering with the caller.  */
end_comment

begin_struct
DECL|struct|async
struct|struct
name|async
block|{
comment|/* 	 * proc reads from in; closes it before return 	 * proc writes to out; closes it before return 	 * returns 0 on success, non-zero on failure 	 */
DECL|member|proc
name|int
function_decl|(
modifier|*
name|proc
function_decl|)
parameter_list|(
name|int
name|in
parameter_list|,
name|int
name|out
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|in
name|int
name|in
decl_stmt|;
comment|/* caller writes here and closes it */
DECL|member|out
name|int
name|out
decl_stmt|;
comment|/* caller reads from here and closes it */
ifndef|#
directive|ifndef
name|WIN32
DECL|member|pid
name|pid_t
name|pid
decl_stmt|;
else|#
directive|else
DECL|member|tid
name|HANDLE
name|tid
decl_stmt|;
DECL|member|proc_in
name|int
name|proc_in
decl_stmt|;
DECL|member|proc_out
name|int
name|proc_out
decl_stmt|;
endif|#
directive|endif
block|}
struct|;
end_struct

begin_function_decl
name|int
name|start_async
parameter_list|(
name|struct
name|async
modifier|*
name|async
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|finish_async
parameter_list|(
name|struct
name|async
modifier|*
name|async
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

