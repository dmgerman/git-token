begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * SHA-1 implementation.  *  * Copyright (C) 2005 Paul Mackerras<paulus@samba.org>  */
end_comment

begin_include
include|#
directive|include
file|<stdint.h>
end_include

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|hash
name|uint32_t
name|hash
index|[
literal|5
index|]
decl_stmt|;
DECL|member|cnt
name|uint32_t
name|cnt
decl_stmt|;
DECL|member|len
name|uint64_t
name|len
decl_stmt|;
union|union
block|{
DECL|member|b
name|unsigned
name|char
name|b
index|[
literal|64
index|]
decl_stmt|;
DECL|member|l
name|uint64_t
name|l
index|[
literal|8
index|]
decl_stmt|;
DECL|member|buf
block|}
name|buf
union|;
DECL|typedef|ppc_SHA_CTX
block|}
name|ppc_SHA_CTX
typedef|;
end_typedef

begin_function_decl
name|int
name|ppc_SHA1_Init
parameter_list|(
name|ppc_SHA_CTX
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ppc_SHA1_Update
parameter_list|(
name|ppc_SHA_CTX
modifier|*
name|c
parameter_list|,
specifier|const
name|void
modifier|*
name|p
parameter_list|,
name|unsigned
name|long
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ppc_SHA1_Final
parameter_list|(
name|unsigned
name|char
modifier|*
name|hash
parameter_list|,
name|ppc_SHA_CTX
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|platform_SHA_CTX
define|#
directive|define
name|platform_SHA_CTX
value|ppc_SHA_CTX
end_define

begin_define
DECL|macro|platform_SHA1_Init
define|#
directive|define
name|platform_SHA1_Init
value|ppc_SHA1_Init
end_define

begin_define
DECL|macro|platform_SHA1_Update
define|#
directive|define
name|platform_SHA1_Update
value|ppc_SHA1_Update
end_define

begin_define
DECL|macro|platform_SHA1_Final
define|#
directive|define
name|platform_SHA1_Final
value|ppc_SHA1_Final
end_define

end_unit

