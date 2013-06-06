begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|CONNECTED_H
end_ifndef

begin_define
DECL|macro|CONNECTED_H
define|#
directive|define
name|CONNECTED_H
end_define

begin_struct_decl
struct_decl|struct
name|transport
struct_decl|;
end_struct_decl

begin_comment
comment|/*  * Take callback data, and return next object name in the buffer.  * When called after returning the name for the last object, return -1  * to signal EOF, otherwise return 0.  */
end_comment

begin_typedef
DECL|typedef|sha1_iterate_fn
typedef|typedef
name|int
function_decl|(
modifier|*
name|sha1_iterate_fn
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|,
name|unsigned
name|char
index|[
literal|20
index|]
parameter_list|)
function_decl|;
end_typedef

begin_comment
comment|/*  * Make sure that our object store has all the commits necessary to  * connect the ancestry chain to some of our existing refs, and all  * the trees and blobs that these commits use.  *  * Return 0 if Ok, non zero otherwise (i.e. some missing objects)  */
end_comment

begin_function_decl
specifier|extern
name|int
name|check_everything_connected
parameter_list|(
name|sha1_iterate_fn
parameter_list|,
name|int
name|quiet
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|check_everything_connected_with_transport
parameter_list|(
name|sha1_iterate_fn
parameter_list|,
name|int
name|quiet
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|,
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* CONNECTED_H */
end_comment

end_unit

