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

begin_comment
comment|/*  * Convert a NUL-terminated string in buffer orig,  * performing substitutions on %-named sub-strings from  * the interpretation table.  */
end_comment

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
name|void
name|interp_set_entry
parameter_list|(
name|struct
name|interp
modifier|*
name|table
parameter_list|,
name|int
name|slot
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|interp_clear_table
parameter_list|(
name|struct
name|interp
modifier|*
name|table
parameter_list|,
name|int
name|ninterps
parameter_list|)
function_decl|;
end_function_decl

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
specifier|const
name|char
modifier|*
name|orig
parameter_list|,
specifier|const
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

