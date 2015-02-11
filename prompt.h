begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PROMPT_H
end_ifndef

begin_define
DECL|macro|PROMPT_H
define|#
directive|define
name|PROMPT_H
end_define

begin_define
DECL|macro|PROMPT_ASKPASS
define|#
directive|define
name|PROMPT_ASKPASS
value|(1<<0)
end_define

begin_define
DECL|macro|PROMPT_ECHO
define|#
directive|define
name|PROMPT_ECHO
value|(1<<1)
end_define

begin_function_decl
name|char
modifier|*
name|git_prompt
parameter_list|(
specifier|const
name|char
modifier|*
name|prompt
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PROMPT_H */
end_comment

end_unit

