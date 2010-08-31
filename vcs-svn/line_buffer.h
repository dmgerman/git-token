begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|LINE_BUFFER_H_
end_ifndef

begin_define
DECL|macro|LINE_BUFFER_H_
define|#
directive|define
name|LINE_BUFFER_H_
end_define

begin_function_decl
name|int
name|buffer_init
parameter_list|(
specifier|const
name|char
modifier|*
name|filename
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|buffer_deinit
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|char
modifier|*
name|buffer_read_line
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|char
modifier|*
name|buffer_read_string
parameter_list|(
name|uint32_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|buffer_copy_bytes
parameter_list|(
name|uint32_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|buffer_skip_bytes
parameter_list|(
name|uint32_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|buffer_reset
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

