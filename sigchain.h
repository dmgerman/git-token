begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SIGCHAIN_H
end_ifndef

begin_define
DECL|macro|SIGCHAIN_H
define|#
directive|define
name|SIGCHAIN_H
end_define

begin_typedef
DECL|typedef|sigchain_fun
typedef|typedef
name|void
function_decl|(
modifier|*
name|sigchain_fun
function_decl|)
parameter_list|(
name|int
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|int
name|sigchain_push
parameter_list|(
name|int
name|sig
parameter_list|,
name|sigchain_fun
name|f
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|sigchain_pop
parameter_list|(
name|int
name|sig
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|sigchain_push_common
parameter_list|(
name|sigchain_fun
name|f
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* SIGCHAIN_H */
end_comment

end_unit

