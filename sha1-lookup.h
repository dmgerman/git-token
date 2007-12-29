begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SHA1_LOOKUP_H
end_ifndef

begin_define
DECL|macro|SHA1_LOOKUP_H
define|#
directive|define
name|SHA1_LOOKUP_H
end_define

begin_function_decl
specifier|extern
name|int
name|sha1_entry_pos
parameter_list|(
specifier|const
name|void
modifier|*
name|table
parameter_list|,
name|size_t
name|elem_size
parameter_list|,
name|size_t
name|key_offset
parameter_list|,
name|unsigned
name|lo
parameter_list|,
name|unsigned
name|hi
parameter_list|,
name|unsigned
name|nr
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|key
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

