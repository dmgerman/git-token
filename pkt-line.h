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

begin_comment
comment|/*  * Read a packetized line into the buffer, which must be at least size bytes  * long. The return value specifies the number of bytes read into the buffer.  *  * If src_buffer is not NULL (and nor is *src_buffer), it should point to a  * buffer containing the packet data to parse, of at least *src_len bytes.  * After the function returns, src_buf will be incremented and src_len  * decremented by the number of bytes consumed.  *  * If src_buffer (or *src_buffer) is NULL, then data is read from the  * descriptor "fd".  *  * If options does not contain PACKET_READ_GENTLE_ON_EOF, we will die under any  * of the following conditions:  *  *   1. Read error from descriptor.  *  *   2. Protocol error from the remote (e.g., bogus length characters).  *  *   3. Receiving a packet larger than "size" bytes.  *  *   4. Truncated output from the remote (e.g., we expected a packet but got  *      EOF, or we got a partial packet followed by EOF).  *  * If options does contain PACKET_READ_GENTLE_ON_EOF, we will not die on  * condition 4 (truncated input), but instead return -1. However, we will still  * die for the other 3 conditions.  *  * If options contains PACKET_READ_CHOMP_NEWLINE, a trailing newline (if  * present) is removed from the buffer before returning.  */
end_comment

begin_define
DECL|macro|PACKET_READ_GENTLE_ON_EOF
define|#
directive|define
name|PACKET_READ_GENTLE_ON_EOF
value|(1u<<0)
end_define

begin_define
DECL|macro|PACKET_READ_CHOMP_NEWLINE
define|#
directive|define
name|PACKET_READ_CHOMP_NEWLINE
value|(1u<<1)
end_define

begin_function_decl
name|int
name|packet_read
parameter_list|(
name|int
name|fd
parameter_list|,
name|char
modifier|*
modifier|*
name|src_buffer
parameter_list|,
name|size_t
modifier|*
name|src_len
parameter_list|,
name|char
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|size
parameter_list|,
name|int
name|options
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Convenience wrapper for packet_read that is not gentle, and sets the  * CHOMP_NEWLINE option. The return value is NULL for a flush packet,  * and otherwise points to a static buffer (that may be overwritten by  * subsequent calls). If the size parameter is not NULL, the length of the  * packet is written to it.  */
end_comment

begin_function_decl
name|char
modifier|*
name|packet_read_line
parameter_list|(
name|int
name|fd
parameter_list|,
name|int
modifier|*
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Same as packet_read_line, but read from a buf rather than a descriptor;  * see packet_read for details on how src_* is used.  */
end_comment

begin_function_decl
name|char
modifier|*
name|packet_read_line_buf
parameter_list|(
name|char
modifier|*
modifier|*
name|src_buf
parameter_list|,
name|size_t
modifier|*
name|src_len
parameter_list|,
name|int
modifier|*
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|DEFAULT_PACKET_MAX
define|#
directive|define
name|DEFAULT_PACKET_MAX
value|1000
end_define

begin_define
DECL|macro|LARGE_PACKET_MAX
define|#
directive|define
name|LARGE_PACKET_MAX
value|65520
end_define

begin_decl_stmt
specifier|extern
name|char
name|packet_buffer
index|[
name|LARGE_PACKET_MAX
index|]
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

end_unit

