begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|DIFF_H
end_ifndef

begin_define
DECL|macro|DIFF_H
define|#
directive|define
name|DIFF_H
end_define

begin_function_decl
specifier|extern
name|void
name|prepare_diff_cmd
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|show_differences
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
comment|/* filename on the filesystem */
specifier|const
name|char
modifier|*
name|label
parameter_list|,
comment|/* diff label to use */
name|void
modifier|*
name|old_contents
parameter_list|,
comment|/* contents in core */
name|unsigned
name|long
name|long
name|old_size
parameter_list|,
comment|/* size in core */
name|int
name|reverse
comment|/* 0: diff core file 					    1: diff file core */
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|show_diff_empty
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|reverse
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* DIFF_H */
end_comment

end_unit

