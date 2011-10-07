begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|FMT_MERGE_MSG_H
end_ifndef

begin_define
DECL|macro|FMT_MERGE_MSG_H
define|#
directive|define
name|FMT_MERGE_MSG_H
end_define

begin_decl_stmt
specifier|extern
name|int
name|merge_log_config
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|extern
name|int
name|fmt_merge_msg_config
parameter_list|(
specifier|const
name|char
modifier|*
name|key
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* FMT_MERGE_MSG_H */
end_comment

end_unit

