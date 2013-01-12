begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|MERGE_BLOBS_H
end_ifndef

begin_define
DECL|macro|MERGE_BLOBS_H
define|#
directive|define
name|MERGE_BLOBS_H
end_define

begin_include
include|#
directive|include
file|"blob.h"
end_include

begin_function_decl
specifier|extern
name|void
modifier|*
name|merge_blobs
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|struct
name|blob
modifier|*
parameter_list|,
name|struct
name|blob
modifier|*
parameter_list|,
name|struct
name|blob
modifier|*
parameter_list|,
name|unsigned
name|long
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* MERGE_BLOBS_H */
end_comment

end_unit

