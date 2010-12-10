begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|FAST_EXPORT_H_
end_ifndef

begin_define
DECL|macro|FAST_EXPORT_H_
define|#
directive|define
name|FAST_EXPORT_H_
end_define

begin_struct_decl
struct_decl|struct
name|strbuf
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|line_buffer
struct_decl|;
end_struct_decl

begin_function_decl
name|void
name|fast_export_init
parameter_list|(
name|int
name|fd
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|fast_export_deinit
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|fast_export_reset
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|fast_export_delete
parameter_list|(
name|uint32_t
name|depth
parameter_list|,
specifier|const
name|uint32_t
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|fast_export_modify
parameter_list|(
name|uint32_t
name|depth
parameter_list|,
specifier|const
name|uint32_t
modifier|*
name|path
parameter_list|,
name|uint32_t
name|mode
parameter_list|,
specifier|const
name|char
modifier|*
name|dataref
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|fast_export_begin_commit
parameter_list|(
name|uint32_t
name|revision
parameter_list|,
name|uint32_t
name|author
parameter_list|,
name|char
modifier|*
name|log
parameter_list|,
name|uint32_t
name|uuid
parameter_list|,
name|uint32_t
name|url
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|fast_export_end_commit
parameter_list|(
name|uint32_t
name|revision
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|fast_export_data
parameter_list|(
name|uint32_t
name|mode
parameter_list|,
name|uint32_t
name|len
parameter_list|,
name|struct
name|line_buffer
modifier|*
name|input
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* If there is no such file at that rev, returns -1, errno == ENOENT. */
end_comment

begin_function_decl
name|int
name|fast_export_ls_rev
parameter_list|(
name|uint32_t
name|rev
parameter_list|,
name|uint32_t
name|depth
parameter_list|,
specifier|const
name|uint32_t
modifier|*
name|path
parameter_list|,
name|uint32_t
modifier|*
name|mode_out
parameter_list|,
name|struct
name|strbuf
modifier|*
name|dataref_out
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|fast_export_ls
parameter_list|(
name|uint32_t
name|depth
parameter_list|,
specifier|const
name|uint32_t
modifier|*
name|path
parameter_list|,
name|uint32_t
modifier|*
name|mode_out
parameter_list|,
name|struct
name|strbuf
modifier|*
name|dataref_out
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

