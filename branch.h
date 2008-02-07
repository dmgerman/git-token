begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|BRANCH_H
end_ifndef

begin_define
DECL|macro|BRANCH_H
define|#
directive|define
name|BRANCH_H
end_define

begin_function_decl
name|void
name|create_branch
parameter_list|(
specifier|const
name|char
modifier|*
name|head
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|start_name
parameter_list|,
name|int
name|force
parameter_list|,
name|int
name|reflog
parameter_list|,
name|int
name|track
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

