begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|STRBUF_H
end_ifndef

begin_define
DECL|macro|STRBUF_H
define|#
directive|define
name|STRBUF_H
end_define

begin_struct
DECL|struct|strbuf
struct|struct
name|strbuf
block|{
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|len
name|int
name|len
decl_stmt|;
DECL|member|eof
name|int
name|eof
decl_stmt|;
DECL|member|buf
name|char
modifier|*
name|buf
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|void
name|strbuf_init
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|read_line
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|FILE
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

begin_comment
comment|/* STRBUF_H */
end_comment

end_unit

