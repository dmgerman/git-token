begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|FETCH_PACK_H
end_ifndef

begin_define
DECL|macro|FETCH_PACK_H
define|#
directive|define
name|FETCH_PACK_H
end_define

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_struct
DECL|struct|fetch_pack_args
struct|struct
name|fetch_pack_args
block|{
DECL|member|uploadpack
specifier|const
name|char
modifier|*
name|uploadpack
decl_stmt|;
DECL|member|unpacklimit
name|int
name|unpacklimit
decl_stmt|;
DECL|member|depth
name|int
name|depth
decl_stmt|;
DECL|member|quiet
name|unsigned
name|quiet
range|:
literal|1
decl_stmt|;
DECL|member|keep_pack
name|unsigned
name|keep_pack
range|:
literal|1
decl_stmt|;
DECL|member|lock_pack
name|unsigned
name|lock_pack
range|:
literal|1
decl_stmt|;
DECL|member|use_thin_pack
name|unsigned
name|use_thin_pack
range|:
literal|1
decl_stmt|;
DECL|member|fetch_all
name|unsigned
name|fetch_all
range|:
literal|1
decl_stmt|;
DECL|member|stdin_refs
name|unsigned
name|stdin_refs
range|:
literal|1
decl_stmt|;
DECL|member|verbose
name|unsigned
name|verbose
range|:
literal|1
decl_stmt|;
DECL|member|no_progress
name|unsigned
name|no_progress
range|:
literal|1
decl_stmt|;
DECL|member|include_tag
name|unsigned
name|include_tag
range|:
literal|1
decl_stmt|;
DECL|member|stateless_rpc
name|unsigned
name|stateless_rpc
range|:
literal|1
decl_stmt|;
DECL|member|check_self_contained_and_connected
name|unsigned
name|check_self_contained_and_connected
range|:
literal|1
decl_stmt|;
DECL|member|self_contained_and_connected
name|unsigned
name|self_contained_and_connected
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * sought represents remote references that should be updated from.  * On return, the names that were found on the remote will have been  * marked as such.  */
end_comment

begin_function_decl
name|struct
name|ref
modifier|*
name|fetch_pack
parameter_list|(
name|struct
name|fetch_pack_args
modifier|*
name|args
parameter_list|,
name|int
name|fd
index|[]
parameter_list|,
name|struct
name|child_process
modifier|*
name|conn
parameter_list|,
specifier|const
name|struct
name|ref
modifier|*
name|ref
parameter_list|,
specifier|const
name|char
modifier|*
name|dest
parameter_list|,
name|struct
name|ref
modifier|*
modifier|*
name|sought
parameter_list|,
name|int
name|nr_sought
parameter_list|,
name|char
modifier|*
modifier|*
name|pack_lockfile
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

