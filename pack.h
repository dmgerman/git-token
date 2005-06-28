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

begin_enum
DECL|enum|object_type
enum|enum
name|object_type
block|{
DECL|enumerator|OBJ_NONE
name|OBJ_NONE
block|,
DECL|enumerator|OBJ_COMMIT
name|OBJ_COMMIT
block|,
DECL|enumerator|OBJ_TREE
name|OBJ_TREE
block|,
DECL|enumerator|OBJ_BLOB
name|OBJ_BLOB
block|,
DECL|enumerator|OBJ_TAG
name|OBJ_TAG
block|,
DECL|enumerator|OBJ_DELTA
name|OBJ_DELTA
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

begin_endif
endif|#
directive|endif
end_endif

end_unit

