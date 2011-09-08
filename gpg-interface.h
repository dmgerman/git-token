begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|GPG_INTERFACE_H
end_ifndef

begin_define
DECL|macro|GPG_INTERFACE_H
define|#
directive|define
name|GPG_INTERFACE_H
end_define

begin_function_decl
specifier|extern
name|int
name|sign_buffer
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buffer
parameter_list|,
name|struct
name|strbuf
modifier|*
name|signature
parameter_list|,
specifier|const
name|char
modifier|*
name|signing_key
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|verify_signed_buffer
parameter_list|(
specifier|const
name|char
modifier|*
name|payload
parameter_list|,
name|size_t
name|payload_size
parameter_list|,
specifier|const
name|char
modifier|*
name|signature
parameter_list|,
name|size_t
name|signature_size
parameter_list|,
name|struct
name|strbuf
modifier|*
name|gpg_output
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|git_gpg_config
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|set_signing_key
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|get_signing_key
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

