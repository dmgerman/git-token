begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|VARINT_H
end_ifndef

begin_define
DECL|macro|VARINT_H
define|#
directive|define
name|VARINT_H
end_define

begin_function_decl
specifier|extern
name|int
name|encode_varint
parameter_list|(
name|uintmax_t
parameter_list|,
name|unsigned
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|uintmax_t
name|decode_varint
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* VARINT_H */
end_comment

end_unit

