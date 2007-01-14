begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PACK_H
end_ifndef

begin_define
DECL|macro|PACK_H
define|#
directive|define
name|PACK_H
end_define

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_comment
comment|/*  * Packed object header  */
end_comment

begin_define
DECL|macro|PACK_SIGNATURE
define|#
directive|define
name|PACK_SIGNATURE
value|0x5041434b
end_define

begin_comment
DECL|macro|PACK_SIGNATURE
comment|/* "PACK" */
end_comment

begin_define
DECL|macro|PACK_VERSION
define|#
directive|define
name|PACK_VERSION
value|2
end_define

begin_define
DECL|macro|pack_version_ok
define|#
directive|define
name|pack_version_ok
parameter_list|(
name|v
parameter_list|)
value|((v) == htonl(2) || (v) == htonl(3))
end_define

begin_struct
DECL|struct|pack_header
struct|struct
name|pack_header
block|{
DECL|member|hdr_signature
name|unsigned
name|int
name|hdr_signature
decl_stmt|;
DECL|member|hdr_version
name|unsigned
name|int
name|hdr_version
decl_stmt|;
DECL|member|hdr_entries
name|unsigned
name|int
name|hdr_entries
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|int
name|verify_pack
parameter_list|(
name|struct
name|packed_git
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

