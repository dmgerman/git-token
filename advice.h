begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|ADVICE_H
end_ifndef

begin_define
DECL|macro|ADVICE_H
define|#
directive|define
name|ADVICE_H
end_define

begin_decl_stmt
specifier|extern
name|int
name|advice_push_nonfastforward
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_status_hints
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_commit_before_merge
decl_stmt|;
end_decl_stmt

begin_function_decl
name|int
name|git_default_advice_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* ADVICE_H */
end_comment

end_unit

