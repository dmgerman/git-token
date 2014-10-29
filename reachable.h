begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|REACHEABLE_H
end_ifndef

begin_define
DECL|macro|REACHEABLE_H
define|#
directive|define
name|REACHEABLE_H
end_define

begin_struct_decl
struct_decl|struct
name|progress
struct_decl|;
end_struct_decl

begin_function_decl
specifier|extern
name|int
name|add_unseen_recent_objects_to_traversal
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|mark_reachable_objects
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|int
name|mark_reflog
parameter_list|,
name|unsigned
name|long
name|mark_recent
parameter_list|,
name|struct
name|progress
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

