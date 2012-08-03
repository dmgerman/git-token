begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|VERSION_H
end_ifndef

begin_define
DECL|macro|VERSION_H
define|#
directive|define
name|VERSION_H
end_define

begin_decl_stmt
specifier|extern
specifier|const
name|char
name|git_version_string
index|[]
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|const
name|char
modifier|*
name|git_user_agent
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|const
name|char
modifier|*
name|git_user_agent_sanitized
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* VERSION_H */
end_comment

end_unit

