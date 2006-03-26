begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_define
DECL|macro|TYPEFLAG_AUTO
define|#
directive|define
name|TYPEFLAG_AUTO
value|'\0'
end_define

begin_define
DECL|macro|TYPEFLAG_REG
define|#
directive|define
name|TYPEFLAG_REG
value|'0'
end_define

begin_define
DECL|macro|TYPEFLAG_LNK
define|#
directive|define
name|TYPEFLAG_LNK
value|'2'
end_define

begin_define
DECL|macro|TYPEFLAG_DIR
define|#
directive|define
name|TYPEFLAG_DIR
value|'5'
end_define

begin_define
DECL|macro|TYPEFLAG_GLOBAL_HEADER
define|#
directive|define
name|TYPEFLAG_GLOBAL_HEADER
value|'g'
end_define

begin_define
DECL|macro|TYPEFLAG_EXT_HEADER
define|#
directive|define
name|TYPEFLAG_EXT_HEADER
value|'x'
end_define

begin_struct
DECL|struct|ustar_header
struct|struct
name|ustar_header
block|{
DECL|member|name
name|char
name|name
index|[
literal|100
index|]
decl_stmt|;
comment|/*   0 */
DECL|member|mode
name|char
name|mode
index|[
literal|8
index|]
decl_stmt|;
comment|/* 100 */
DECL|member|uid
name|char
name|uid
index|[
literal|8
index|]
decl_stmt|;
comment|/* 108 */
DECL|member|gid
name|char
name|gid
index|[
literal|8
index|]
decl_stmt|;
comment|/* 116 */
DECL|member|size
name|char
name|size
index|[
literal|12
index|]
decl_stmt|;
comment|/* 124 */
DECL|member|mtime
name|char
name|mtime
index|[
literal|12
index|]
decl_stmt|;
comment|/* 136 */
DECL|member|chksum
name|char
name|chksum
index|[
literal|8
index|]
decl_stmt|;
comment|/* 148 */
DECL|member|typeflag
name|char
name|typeflag
index|[
literal|1
index|]
decl_stmt|;
comment|/* 156 */
DECL|member|linkname
name|char
name|linkname
index|[
literal|100
index|]
decl_stmt|;
comment|/* 157 */
DECL|member|magic
name|char
name|magic
index|[
literal|6
index|]
decl_stmt|;
comment|/* 257 */
DECL|member|version
name|char
name|version
index|[
literal|2
index|]
decl_stmt|;
comment|/* 263 */
DECL|member|uname
name|char
name|uname
index|[
literal|32
index|]
decl_stmt|;
comment|/* 265 */
DECL|member|gname
name|char
name|gname
index|[
literal|32
index|]
decl_stmt|;
comment|/* 297 */
DECL|member|devmajor
name|char
name|devmajor
index|[
literal|8
index|]
decl_stmt|;
comment|/* 329 */
DECL|member|devminor
name|char
name|devminor
index|[
literal|8
index|]
decl_stmt|;
comment|/* 337 */
DECL|member|prefix
name|char
name|prefix
index|[
literal|155
index|]
decl_stmt|;
comment|/* 345 */
block|}
struct|;
end_struct

end_unit

