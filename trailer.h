begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|TRAILER_H
end_ifndef

begin_define
DECL|macro|TRAILER_H
define|#
directive|define
name|TRAILER_H
end_define

begin_function_decl
name|void
name|process_trailers
parameter_list|(
specifier|const
name|char
modifier|*
name|file
parameter_list|,
name|int
name|in_place
parameter_list|,
name|int
name|trim_empty
parameter_list|,
name|struct
name|string_list
modifier|*
name|trailers
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* TRAILER_H */
end_comment

end_unit

