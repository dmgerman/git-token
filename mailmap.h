begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|MAILMAP_H
end_ifndef

begin_define
DECL|macro|MAILMAP_H
define|#
directive|define
name|MAILMAP_H
end_define

begin_function_decl
name|int
name|read_mailmap
parameter_list|(
name|struct
name|string_list
modifier|*
name|map
parameter_list|,
name|char
modifier|*
modifier|*
name|repo_abbrev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|clear_mailmap
parameter_list|(
name|struct
name|string_list
modifier|*
name|map
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|map_user
parameter_list|(
name|struct
name|string_list
modifier|*
name|mailmap
parameter_list|,
name|char
modifier|*
name|email
parameter_list|,
name|int
name|maxlen_email
parameter_list|,
name|char
modifier|*
name|name
parameter_list|,
name|int
name|maxlen_name
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

