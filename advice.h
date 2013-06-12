begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|ADVICE_H
end_ifndef

begin_define
DECL|macro|ADVICE_H
define|#
directive|define
name|ADVICE_H
end_define

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_decl_stmt
specifier|extern
name|int
name|advice_push_update_rejected
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_push_non_ff_current
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_push_non_ff_default
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_push_non_ff_matching
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_push_already_exists
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_push_fetch_first
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_push_needs_force
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_status_hints
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_status_u_option
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_commit_before_merge
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_resolve_conflict
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_implicit_identity
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_detached_head
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_set_upstream_failure
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|advice_rm_hints
decl_stmt|;
end_decl_stmt

begin_function_decl
name|int
name|git_default_advice_config
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
name|advise
parameter_list|(
specifier|const
name|char
modifier|*
name|advice
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|error_resolve_conflict
parameter_list|(
specifier|const
name|char
modifier|*
name|me
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|NORETURN
name|die_resolve_conflict
parameter_list|(
specifier|const
name|char
modifier|*
name|me
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|detach_advice
parameter_list|(
specifier|const
name|char
modifier|*
name|new_name
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* ADVICE_H */
end_comment

end_unit

