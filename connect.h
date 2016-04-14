begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|CONNECT_H
end_ifndef

begin_define
DECL|macro|CONNECT_H
define|#
directive|define
name|CONNECT_H
end_define

begin_define
DECL|macro|CONNECT_VERBOSE
define|#
directive|define
name|CONNECT_VERBOSE
value|(1u<< 0)
end_define

begin_define
DECL|macro|CONNECT_DIAG_URL
define|#
directive|define
name|CONNECT_DIAG_URL
value|(1u<< 1)
end_define

begin_define
DECL|macro|CONNECT_IPV4
define|#
directive|define
name|CONNECT_IPV4
value|(1u<< 2)
end_define

begin_define
DECL|macro|CONNECT_IPV6
define|#
directive|define
name|CONNECT_IPV6
value|(1u<< 3)
end_define

begin_function_decl
specifier|extern
name|struct
name|child_process
modifier|*
name|git_connect
parameter_list|(
name|int
name|fd
index|[
literal|2
index|]
parameter_list|,
specifier|const
name|char
modifier|*
name|url
parameter_list|,
specifier|const
name|char
modifier|*
name|prog
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|finish_connect
parameter_list|(
name|struct
name|child_process
modifier|*
name|conn
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|git_connection_is_socket
parameter_list|(
name|struct
name|child_process
modifier|*
name|conn
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|server_supports
parameter_list|(
specifier|const
name|char
modifier|*
name|feature
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|parse_feature_request
parameter_list|(
specifier|const
name|char
modifier|*
name|features
parameter_list|,
specifier|const
name|char
modifier|*
name|feature
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|server_feature_value
parameter_list|(
specifier|const
name|char
modifier|*
name|feature
parameter_list|,
name|int
modifier|*
name|len_ret
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|url_is_local_not_ssh
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

