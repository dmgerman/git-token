begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|BLOB_H
end_ifndef

begin_define
DECL|macro|BLOB_H
define|#
directive|define
name|BLOB_H
end_define

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_decl_stmt
specifier|extern
specifier|const
name|char
modifier|*
name|blob_type
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|blob
struct|struct
name|blob
block|{
DECL|member|object
name|struct
name|object
name|object
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|blob
modifier|*
name|lookup_blob
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_blob_buffer
parameter_list|(
name|struct
name|blob
modifier|*
name|item
parameter_list|,
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Blobs do not contain references to other objects and do not have  * structured data that needs parsing. However, code may use the  * "parsed" bit in the struct object for a blob to determine whether  * its content has been found to actually be available, so  * parse_blob_buffer() is used (by object.c) to flag that the object  * has been read successfully from the database.  **/
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* BLOB_H */
end_comment

end_unit

