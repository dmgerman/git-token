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

