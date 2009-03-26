begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|BISECT_H
end_ifndef

begin_define
DECL|macro|BISECT_H
define|#
directive|define
name|BISECT_H
end_define

begin_function_decl
specifier|extern
name|struct
name|commit_list
modifier|*
name|find_bisection
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|,
name|int
modifier|*
name|reaches
parameter_list|,
name|int
modifier|*
name|all
parameter_list|,
name|int
name|find_all
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|show_bisect_vars
parameter_list|(
name|int
name|reaches
parameter_list|,
name|int
name|all
parameter_list|,
name|int
name|show_all
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

