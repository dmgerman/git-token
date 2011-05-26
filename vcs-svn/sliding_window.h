begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SLIDING_WINDOW_H_
end_ifndef

begin_define
DECL|macro|SLIDING_WINDOW_H_
define|#
directive|define
name|SLIDING_WINDOW_H_
end_define

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_struct
DECL|struct|sliding_view
struct|struct
name|sliding_view
block|{
DECL|member|file
name|struct
name|line_buffer
modifier|*
name|file
decl_stmt|;
DECL|member|off
name|off_t
name|off
decl_stmt|;
DECL|member|width
name|size_t
name|width
decl_stmt|;
DECL|member|buf
name|struct
name|strbuf
name|buf
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|SLIDING_VIEW_INIT
define|#
directive|define
name|SLIDING_VIEW_INIT
parameter_list|(
name|input
parameter_list|)
value|{ (input), 0, 0, STRBUF_INIT }
end_define

begin_function_decl
specifier|extern
name|int
name|move_window
parameter_list|(
name|struct
name|sliding_view
modifier|*
name|view
parameter_list|,
name|off_t
name|off
parameter_list|,
name|size_t
name|width
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

