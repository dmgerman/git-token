begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SVNDIFF_H_
end_ifndef

begin_define
DECL|macro|SVNDIFF_H_
define|#
directive|define
name|SVNDIFF_H_
end_define

begin_struct_decl
struct_decl|struct
name|line_buffer
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|sliding_view
struct_decl|;
end_struct_decl

begin_function_decl
specifier|extern
name|int
name|svndiff0_apply
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|delta
parameter_list|,
name|off_t
name|delta_len
parameter_list|,
name|struct
name|sliding_view
modifier|*
name|preimage
parameter_list|,
name|FILE
modifier|*
name|postimage
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

