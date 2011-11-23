begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|MERGE_FILE_H
end_ifndef

begin_define
DECL|macro|MERGE_FILE_H
define|#
directive|define
name|MERGE_FILE_H
end_define

begin_function_decl
specifier|extern
name|void
modifier|*
name|merge_file
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|struct
name|blob
modifier|*
name|base
parameter_list|,
name|struct
name|blob
modifier|*
name|our
parameter_list|,
name|struct
name|blob
modifier|*
name|their
parameter_list|,
name|unsigned
name|long
modifier|*
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

