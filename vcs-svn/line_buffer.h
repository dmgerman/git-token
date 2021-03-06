begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|LINE_BUFFER_H_
end_ifndef

begin_define
DECL|macro|LINE_BUFFER_H_
define|#
directive|define
name|LINE_BUFFER_H_
end_define

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_define
DECL|macro|LINE_BUFFER_LEN
define|#
directive|define
name|LINE_BUFFER_LEN
value|10000
end_define

begin_struct
DECL|struct|line_buffer
struct|struct
name|line_buffer
block|{
DECL|member|line_buffer
name|char
name|line_buffer
index|[
name|LINE_BUFFER_LEN
index|]
decl_stmt|;
DECL|member|infile
name|FILE
modifier|*
name|infile
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|LINE_BUFFER_INIT
define|#
directive|define
name|LINE_BUFFER_INIT
value|{ "", NULL }
end_define

begin_function_decl
name|int
name|buffer_init
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|filename
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|buffer_fdinit
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|,
name|int
name|fd
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|buffer_deinit
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|buffer_tmpfile_init
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|FILE
modifier|*
name|buffer_tmpfile_rewind
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* prepare to write. */
end_comment

begin_function_decl
name|long
name|buffer_tmpfile_prepare_to_read
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|buffer_ferror
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|char
modifier|*
name|buffer_read_line
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|buffer_read_char
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|size_t
name|buffer_read_binary
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Returns number of bytes read (not necessarily written). */
end_comment

begin_function_decl
name|off_t
name|buffer_copy_bytes
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|,
name|off_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|off_t
name|buffer_skip_bytes
parameter_list|(
name|struct
name|line_buffer
modifier|*
name|buf
parameter_list|,
name|off_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

