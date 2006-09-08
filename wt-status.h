begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|STATUS_H
end_ifndef

begin_define
DECL|macro|STATUS_H
define|#
directive|define
name|STATUS_H
end_define

begin_enum
DECL|enum|color_wt_status
enum|enum
name|color_wt_status
block|{
DECL|enumerator|WT_STATUS_HEADER
name|WT_STATUS_HEADER
block|,
DECL|enumerator|WT_STATUS_UPDATED
name|WT_STATUS_UPDATED
block|,
DECL|enumerator|WT_STATUS_CHANGED
name|WT_STATUS_CHANGED
block|,
DECL|enumerator|WT_STATUS_UNTRACKED
name|WT_STATUS_UNTRACKED
block|, }
enum|;
end_enum

begin_struct
DECL|struct|wt_status
struct|struct
name|wt_status
block|{
DECL|member|is_initial
name|int
name|is_initial
decl_stmt|;
DECL|member|branch
name|char
modifier|*
name|branch
decl_stmt|;
DECL|member|reference
specifier|const
name|char
modifier|*
name|reference
decl_stmt|;
DECL|member|commitable
name|int
name|commitable
decl_stmt|;
DECL|member|verbose
name|int
name|verbose
decl_stmt|;
DECL|member|amend
name|int
name|amend
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|git_status_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|wt_status_prepare
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|wt_status_print
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* STATUS_H */
end_comment

end_unit

