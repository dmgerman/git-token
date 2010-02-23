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
name|uint32_t
name|hdr_signature
decl_stmt|;
DECL|member|hdr_version
name|uint32_t
name|hdr_version
decl_stmt|;
DECL|member|hdr_entries
name|uint32_t
name|hdr_entries
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * The first four bytes of index formats later than version 1 should  * start with this signature, as all older git binaries would find this  * value illegal and abort reading the file.  *  * This is the case because the number of objects in a packfile  * cannot exceed 1,431,660,000 as every object would need at least  * 3 bytes of data and the overall packfile cannot exceed 4 GiB with  * version 1 of the index file due to the offsets limited to 32 bits.  * Clearly the signature exceeds this maximum.  *  * Very old git binaries will also compare the first 4 bytes to the  * next 4 bytes in the index and abort with a "non-monotonic index"  * error if the second 4 byte word is smaller than the first 4  * byte word.  This would be true in the proposed future index  * format as idx_signature would be greater than idx_version.  */
end_comment

begin_define
DECL|macro|PACK_IDX_SIGNATURE
define|#
directive|define
name|PACK_IDX_SIGNATURE
value|0xff744f63
end_define

begin_comment
DECL|macro|PACK_IDX_SIGNATURE
comment|/* "\377tOc" */
end_comment

begin_comment
comment|/* These may be overridden by command-line parameters */
end_comment

begin_decl_stmt
specifier|extern
name|uint32_t
name|pack_idx_default_version
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|uint32_t
name|pack_idx_off32_limit
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Packed object index header  */
end_comment

begin_struct
DECL|struct|pack_idx_header
struct|struct
name|pack_idx_header
block|{
DECL|member|idx_signature
name|uint32_t
name|idx_signature
decl_stmt|;
DECL|member|idx_version
name|uint32_t
name|idx_version
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * Common part of object structure used for write_idx_file  */
end_comment

begin_struct
DECL|struct|pack_idx_entry
struct|struct
name|pack_idx_entry
block|{
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|crc32
name|uint32_t
name|crc32
decl_stmt|;
DECL|member|offset
name|off_t
name|offset
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|write_idx_file
parameter_list|(
specifier|const
name|char
modifier|*
name|index_name
parameter_list|,
name|struct
name|pack_idx_entry
modifier|*
modifier|*
name|objects
parameter_list|,
name|int
name|nr_objects
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|check_pack_crc
parameter_list|(
name|struct
name|packed_git
modifier|*
name|p
parameter_list|,
name|struct
name|pack_window
modifier|*
modifier|*
name|w_curs
parameter_list|,
name|off_t
name|offset
parameter_list|,
name|off_t
name|len
parameter_list|,
name|unsigned
name|int
name|nr
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|verify_pack
parameter_list|(
name|struct
name|packed_git
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|fixup_pack_header_footer
parameter_list|(
name|int
parameter_list|,
name|unsigned
name|char
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|uint32_t
parameter_list|,
name|unsigned
name|char
modifier|*
parameter_list|,
name|off_t
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|index_pack_lockfile
parameter_list|(
name|int
name|fd
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|encode_in_pack_object_header
parameter_list|(
name|enum
name|object_type
parameter_list|,
name|uintmax_t
parameter_list|,
name|unsigned
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|PH_ERROR_EOF
define|#
directive|define
name|PH_ERROR_EOF
value|(-1)
end_define

begin_define
DECL|macro|PH_ERROR_PACK_SIGNATURE
define|#
directive|define
name|PH_ERROR_PACK_SIGNATURE
value|(-2)
end_define

begin_define
DECL|macro|PH_ERROR_PROTOCOL
define|#
directive|define
name|PH_ERROR_PROTOCOL
value|(-3)
end_define

begin_function_decl
specifier|extern
name|int
name|read_pack_header
parameter_list|(
name|int
name|fd
parameter_list|,
name|struct
name|pack_header
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

