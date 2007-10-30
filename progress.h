begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PROGRESS_H
end_ifndef

begin_define
DECL|macro|PROGRESS_H
define|#
directive|define
name|PROGRESS_H
end_define

begin_struct_decl
struct_decl|struct
name|progress
struct_decl|;
end_struct_decl

begin_function_decl
name|int
name|display_progress
parameter_list|(
name|struct
name|progress
modifier|*
name|progress
parameter_list|,
name|unsigned
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|progress
modifier|*
name|start_progress
parameter_list|(
specifier|const
name|char
modifier|*
name|title
parameter_list|,
name|unsigned
name|total
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|progress
modifier|*
name|start_progress_delay
parameter_list|(
specifier|const
name|char
modifier|*
name|title
parameter_list|,
name|unsigned
name|total
parameter_list|,
name|unsigned
name|percent_treshold
parameter_list|,
name|unsigned
name|delay
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|stop_progress
parameter_list|(
name|struct
name|progress
modifier|*
modifier|*
name|progress
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

