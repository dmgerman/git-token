begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PKTLINE_H
end_ifndef

begin_define
DECL|macro|PKTLINE_H
define|#
directive|define
name|PKTLINE_H
end_define

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_comment
comment|/*  * Write a packetized stream, where each line is preceded by  * its length (including the header) as a 4-byte hex number.  * A length of 'zero' means end of stream (and a length of 1-3  * would be an error).  *  * This is all pretty stupid, but we use this packetized line  * format to make a streaming format possible without ever  * over-running the read buffers. That way we'll never read  * into what might be the pack data (which should go to another  * process entirely).  *  * The writing side could use stdio, but since the reading  * side can't, we stay with pure read/write interfaces.  */
end_comment

begin_function_decl
name|void
name|packet_flush
parameter_list|(
name|int
name|fd
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|packet_write
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
function_decl|__attribute__
parameter_list|(
function_decl|(format
parameter_list|(
name|printf
parameter_list|,
function_decl|2
operator|,
function_decl|3
end_function_decl

begin_empty_stmt
unit|)))
empty_stmt|;
end_empty_stmt

begin_function_decl
name|void
name|packet_buf_flush
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|packet_buf_write
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
function_decl|__attribute__
parameter_list|(
function_decl|(format
parameter_list|(
name|printf
parameter_list|,
function_decl|2
operator|,
function_decl|3
end_function_decl

begin_empty_stmt
unit|)))
empty_stmt|;
end_empty_stmt

begin_function_decl
name|int
name|packet_read_line
parameter_list|(
name|int
name|fd
parameter_list|,
name|char
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|packet_read
parameter_list|(
name|int
name|fd
parameter_list|,
name|char
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|packet_get_line
parameter_list|(
name|struct
name|strbuf
modifier|*
name|out
parameter_list|,
name|char
modifier|*
modifier|*
name|src_buf
parameter_list|,
name|size_t
modifier|*
name|src_len
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

