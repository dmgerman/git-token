begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|CSUM_FILE_H
end_ifndef

begin_define
DECL|macro|CSUM_FILE_H
define|#
directive|define
name|CSUM_FILE_H
end_define

begin_comment
comment|/* A SHA1-protected file */
end_comment

begin_struct
DECL|struct|sha1file
struct|struct
name|sha1file
block|{
DECL|member|fd
DECL|member|error
name|int
name|fd
decl_stmt|,
name|error
decl_stmt|;
DECL|member|offset
name|unsigned
name|long
name|offset
decl_stmt|;
DECL|member|ctx
name|SHA_CTX
name|ctx
decl_stmt|;
DECL|member|buffer
name|unsigned
name|char
name|buffer
index|[
literal|8192
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|struct
name|sha1file
modifier|*
name|sha1create
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|sha1close
parameter_list|(
name|struct
name|sha1file
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|sha1write
parameter_list|(
name|struct
name|sha1file
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|,
name|unsigned
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|sha1write_compressed
parameter_list|(
name|struct
name|sha1file
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|,
name|unsigned
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

