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

begin_define
DECL|macro|MAX_RUN_COMMAND_ARGS
define|#
directive|define
name|MAX_RUN_COMMAND_ARGS
value|256
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
DECL|macro|RUN_COMMAND_NO_STDIO
define|#
directive|define
name|RUN_COMMAND_NO_STDIO
value|1
end_define

begin_function_decl
name|int
name|run_command_v_opt
parameter_list|(
name|int
name|argc
parameter_list|,
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

begin_function_decl
name|int
name|run_command_v
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|run_command
parameter_list|(
specifier|const
name|char
modifier|*
name|cmd
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

