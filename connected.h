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
comment|/*  * Named-arguments struct for check_connected. All arguments are  * optional, and can be left to defaults as set by CHECK_CONNECTED_INIT.  */
end_comment

begin_struct
DECL|struct|check_connected_options
struct|struct
name|check_connected_options
block|{
comment|/* Avoid printing any errors to stderr. */
DECL|member|quiet
name|int
name|quiet
decl_stmt|;
comment|/* --shallow-file to pass to rev-list sub-process */
DECL|member|shallow_file
specifier|const
name|char
modifier|*
name|shallow_file
decl_stmt|;
comment|/* Transport whose objects we are checking, if available. */
DECL|member|transport
name|struct
name|transport
modifier|*
name|transport
decl_stmt|;
comment|/* 	 * If non-zero, send error messages to this descriptor rather 	 * than stderr. The descriptor is closed before check_connected 	 * returns. 	 */
DECL|member|err_fd
name|int
name|err_fd
decl_stmt|;
comment|/* If non-zero, show progress as we traverse the objects. */
DECL|member|progress
name|int
name|progress
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|CHECK_CONNECTED_INIT
define|#
directive|define
name|CHECK_CONNECTED_INIT
value|{ 0 }
end_define

begin_comment
comment|/*  * Make sure that our object store has all the commits necessary to  * connect the ancestry chain to some of our existing refs, and all  * the trees and blobs that these commits use.  *  * Return 0 if Ok, non zero otherwise (i.e. some missing objects)  *  * If "opt" is NULL, behaves as if CHECK_CONNECTED_INIT was passed.  */
end_comment

begin_function_decl
name|int
name|check_connected
parameter_list|(
name|sha1_iterate_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|,
name|struct
name|check_connected_options
modifier|*
name|opt
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

