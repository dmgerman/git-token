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

begin_typedef
DECL|typedef|show_commit_fn
typedef|typedef
name|void
function_decl|(
modifier|*
name|show_commit_fn
function_decl|)
parameter_list|(
name|struct
name|commit
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|show_object_fn
typedef|typedef
name|void
function_decl|(
modifier|*
name|show_object_fn
function_decl|)
parameter_list|(
name|struct
name|object
modifier|*
parameter_list|,
specifier|const
name|struct
name|name_path
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|show_edge_fn
typedef|typedef
name|void
function_decl|(
modifier|*
name|show_edge_fn
function_decl|)
parameter_list|(
name|struct
name|commit
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|void
name|traverse_commit_list
parameter_list|(
name|struct
name|rev_info
modifier|*
parameter_list|,
name|show_commit_fn
parameter_list|,
name|show_object_fn
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|mark_edges_uninteresting
parameter_list|(
name|struct
name|commit_list
modifier|*
parameter_list|,
name|struct
name|rev_info
modifier|*
parameter_list|,
name|show_edge_fn
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

