begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|XDIFF_INTERFACE_H
end_ifndef

begin_define
DECL|macro|XDIFF_INTERFACE_H
define|#
directive|define
name|XDIFF_INTERFACE_H
end_define

begin_include
include|#
directive|include
file|"xdiff/xdiff.h"
end_include

begin_struct_decl
struct_decl|struct
name|xdiff_emit_state
struct_decl|;
end_struct_decl

begin_typedef
DECL|typedef|xdiff_emit_consume_fn
typedef|typedef
name|void
function_decl|(
modifier|*
name|xdiff_emit_consume_fn
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|,
name|char
modifier|*
parameter_list|,
name|unsigned
name|long
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|xdiff_emit_state
struct|struct
name|xdiff_emit_state
block|{
DECL|member|consume
name|xdiff_emit_consume_fn
name|consume
decl_stmt|;
DECL|member|remainder
name|char
modifier|*
name|remainder
decl_stmt|;
DECL|member|remainder_size
name|unsigned
name|long
name|remainder_size
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|xdiff_outf
parameter_list|(
name|void
modifier|*
name|priv_
parameter_list|,
name|mmbuffer_t
modifier|*
name|mb
parameter_list|,
name|int
name|nbuf
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

