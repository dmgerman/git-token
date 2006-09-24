begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright 2006 Jon Loeliger  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|INTERPOLATE_H
end_ifndef

begin_define
DECL|macro|INTERPOLATE_H
define|#
directive|define
name|INTERPOLATE_H
end_define

begin_struct
DECL|struct|interp
struct|struct
name|interp
block|{
DECL|member|name
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|value
name|char
modifier|*
name|value
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|int
name|interpolate
parameter_list|(
name|char
modifier|*
name|result
parameter_list|,
name|int
name|reslen
parameter_list|,
name|char
modifier|*
name|orig
parameter_list|,
name|struct
name|interp
modifier|*
name|interps
parameter_list|,
name|int
name|ninterps
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* INTERPOLATE_H */
end_comment

end_unit

