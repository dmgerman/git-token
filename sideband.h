begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SIDEBAND_H
end_ifndef

begin_define
DECL|macro|SIDEBAND_H
define|#
directive|define
name|SIDEBAND_H
end_define

begin_define
DECL|macro|SIDEBAND_PROTOCOL_ERROR
define|#
directive|define
name|SIDEBAND_PROTOCOL_ERROR
value|-2
end_define

begin_define
DECL|macro|SIDEBAND_REMOTE_ERROR
define|#
directive|define
name|SIDEBAND_REMOTE_ERROR
value|-1
end_define

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

begin_function_decl
name|int
name|recv_sideband
parameter_list|(
specifier|const
name|char
modifier|*
name|me
parameter_list|,
name|int
name|in_stream
parameter_list|,
name|int
name|out
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ssize_t
name|send_sideband
parameter_list|(
name|int
name|fd
parameter_list|,
name|int
name|band
parameter_list|,
specifier|const
name|char
modifier|*
name|data
parameter_list|,
name|ssize_t
name|sz
parameter_list|,
name|int
name|packet_max
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

