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

begin_comment
comment|/*  * Silly packetized line writing interface  */
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

begin_endif
endif|#
directive|endif
end_endif

end_unit

