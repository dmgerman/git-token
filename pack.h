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

begin_comment
comment|/*  * The packed object type is stored in 3 bits.  * The type value 0 is a reserved prefix if ever there is more than 7  * object types, or any future format extensions.  */
end_comment

begin_enum
DECL|enum|object_type
enum|enum
name|object_type
block|{
DECL|enumerator|OBJ_EXT
name|OBJ_EXT
init|=
literal|0
block|,
DECL|enumerator|OBJ_COMMIT
name|OBJ_COMMIT
init|=
literal|1
block|,
DECL|enumerator|OBJ_TREE
name|OBJ_TREE
init|=
literal|2
block|,
DECL|enumerator|OBJ_BLOB
name|OBJ_BLOB
init|=
literal|3
block|,
DECL|enumerator|OBJ_TAG
name|OBJ_TAG
init|=
literal|4
block|,
comment|/* 5/6 for future expansion */
DECL|enumerator|OBJ_DELTA
name|OBJ_DELTA
init|=
literal|7
block|, }
enum|;
end_enum

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

begin_function_decl
specifier|extern
name|int
name|check_reuse_pack_delta
parameter_list|(
name|struct
name|packed_git
modifier|*
parameter_list|,
name|unsigned
name|long
parameter_list|,
name|unsigned
name|char
modifier|*
parameter_list|,
name|unsigned
name|long
modifier|*
parameter_list|,
name|enum
name|object_type
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

