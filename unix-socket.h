begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|UNIX_SOCKET_H
end_ifndef

begin_define
DECL|macro|UNIX_SOCKET_H
define|#
directive|define
name|UNIX_SOCKET_H
end_define

begin_function_decl
name|int
name|unix_stream_connect
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|unix_stream_listen
parameter_list|(
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
comment|/* UNIX_SOCKET_H */
end_comment

end_unit

