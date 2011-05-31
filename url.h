begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|URL_H
end_ifndef

begin_define
DECL|macro|URL_H
define|#
directive|define
name|URL_H
end_define

begin_function_decl
specifier|extern
name|int
name|is_url
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|is_urlschemechar
parameter_list|(
name|int
name|first_flag
parameter_list|,
name|int
name|ch
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|url_decode
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|url_decode_parameter_name
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|query
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|url_decode_parameter_value
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|query
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|end_url_with_slash
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|url
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|str_end_url_with_slash
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|,
name|char
modifier|*
modifier|*
name|dest
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* URL_H */
end_comment

end_unit

