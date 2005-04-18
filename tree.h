begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|TREE_H
end_ifndef

begin_define
DECL|macro|TREE_H
define|#
directive|define
name|TREE_H
end_define

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_decl_stmt
specifier|extern
specifier|const
name|char
modifier|*
name|tree_type
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|tree
struct|struct
name|tree
block|{
DECL|member|object
name|struct
name|object
name|object
decl_stmt|;
DECL|member|has_full_path
name|unsigned
name|has_full_path
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|tree
modifier|*
name|lookup_tree
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_tree
parameter_list|(
name|struct
name|tree
modifier|*
name|tree
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* TREE_H */
end_comment

end_unit

