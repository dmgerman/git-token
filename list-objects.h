begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|LIST_OBJECTS_H
end_ifndef

begin_define
DECL|macro|LIST_OBJECTS_H
define|#
directive|define
name|LIST_OBJECTS_H
end_define

begin_function_decl
name|void
name|traverse_commit_list
parameter_list|(
name|struct
name|rev_info
modifier|*
name|revs
parameter_list|,
name|void
function_decl|(
modifier|*
name|show_commit
function_decl|)
parameter_list|(
name|struct
name|commit
modifier|*
parameter_list|)
parameter_list|,
name|void
function_decl|(
modifier|*
name|show_object
function_decl|)
parameter_list|(
name|struct
name|object_array_entry
modifier|*
parameter_list|)
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

