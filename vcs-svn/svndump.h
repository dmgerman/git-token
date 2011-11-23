begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SVNDUMP_H_
end_ifndef

begin_define
DECL|macro|SVNDUMP_H_
define|#
directive|define
name|SVNDUMP_H_
end_define

begin_function_decl
name|int
name|svndump_init
parameter_list|(
specifier|const
name|char
modifier|*
name|filename
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|svndump_read
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|svndump_deinit
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|svndump_reset
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

