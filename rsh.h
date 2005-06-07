begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|RSH_H
end_ifndef

begin_define
DECL|macro|RSH_H
define|#
directive|define
name|RSH_H
end_define

begin_function_decl
name|int
name|setup_connection
parameter_list|(
name|int
modifier|*
name|fd_in
parameter_list|,
name|int
modifier|*
name|fd_out
parameter_list|,
specifier|const
name|char
modifier|*
name|remote_prog
parameter_list|,
name|char
modifier|*
name|url
parameter_list|,
name|int
name|rmt_argc
parameter_list|,
name|char
modifier|*
modifier|*
name|rmt_argv
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

