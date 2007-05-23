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

begin_struct
DECL|struct|progress
struct|struct
name|progress
block|{
DECL|member|prefix
specifier|const
name|char
modifier|*
name|prefix
decl_stmt|;
DECL|member|total
name|unsigned
name|total
decl_stmt|;
DECL|member|last_percent
name|unsigned
name|last_percent
decl_stmt|;
DECL|member|delay
name|unsigned
name|delay
decl_stmt|;
DECL|member|delayed_percent_treshold
name|unsigned
name|delayed_percent_treshold
decl_stmt|;
DECL|member|delayed_title
specifier|const
name|char
modifier|*
name|delayed_title
decl_stmt|;
DECL|member|need_lf
name|int
name|need_lf
decl_stmt|;
block|}
struct|;
end_struct

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
name|void
name|start_progress
parameter_list|(
name|struct
name|progress
modifier|*
name|progress
parameter_list|,
specifier|const
name|char
modifier|*
name|title
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|unsigned
name|total
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|start_progress_delay
parameter_list|(
name|struct
name|progress
modifier|*
name|progress
parameter_list|,
specifier|const
name|char
modifier|*
name|title
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
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
name|progress
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

