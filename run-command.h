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

begin_enum
enum|enum
block|{
DECL|enumerator|ERR_RUN_COMMAND_FORK
name|ERR_RUN_COMMAND_FORK
init|=
literal|10000
block|,
DECL|enumerator|ERR_RUN_COMMAND_EXEC
name|ERR_RUN_COMMAND_EXEC
block|,
DECL|enumerator|ERR_RUN_COMMAND_PIPE
name|ERR_RUN_COMMAND_PIPE
block|,
DECL|enumerator|ERR_RUN_COMMAND_WAITPID
name|ERR_RUN_COMMAND_WAITPID
block|,
DECL|enumerator|ERR_RUN_COMMAND_WAITPID_WRONG_PID
name|ERR_RUN_COMMAND_WAITPID_WRONG_PID
block|,
DECL|enumerator|ERR_RUN_COMMAND_WAITPID_SIGNAL
name|ERR_RUN_COMMAND_WAITPID_SIGNAL
block|,
DECL|enumerator|ERR_RUN_COMMAND_WAITPID_NOEXIT
name|ERR_RUN_COMMAND_WAITPID_NOEXIT
block|, }
enum|;
end_enum

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
DECL|member|in
name|int
name|in
decl_stmt|;
DECL|member|out
name|int
name|out
decl_stmt|;
DECL|member|close_in
name|unsigned
name|close_in
range|:
literal|1
decl_stmt|;
DECL|member|close_out
name|unsigned
name|close_out
range|:
literal|1
decl_stmt|;
DECL|member|no_stdin
name|unsigned
name|no_stdin
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
DECL|member|stdout_to_stderr
name|unsigned
name|stdout_to_stderr
range|:
literal|1
decl_stmt|;
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

begin_endif
endif|#
directive|endif
end_endif

end_unit

