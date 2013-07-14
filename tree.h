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
DECL|member|buffer
name|void
modifier|*
name|buffer
decl_stmt|;
DECL|member|size
name|unsigned
name|long
name|size
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
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_tree_buffer
parameter_list|(
name|struct
name|tree
modifier|*
name|item
parameter_list|,
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
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

begin_comment
comment|/* Parses and returns the tree in the given ent, chasing tags and commits. */
end_comment

begin_function_decl
name|struct
name|tree
modifier|*
name|parse_tree_indirect
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|READ_TREE_RECURSIVE
define|#
directive|define
name|READ_TREE_RECURSIVE
value|1
end_define

begin_typedef
DECL|typedef|read_tree_fn_t
typedef|typedef
name|int
function_decl|(
modifier|*
name|read_tree_fn_t
function_decl|)
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|unsigned
name|int
parameter_list|,
name|int
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
specifier|extern
name|int
name|read_tree_recursive
parameter_list|(
name|struct
name|tree
modifier|*
name|tree
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
name|int
name|stage
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|,
name|read_tree_fn_t
name|fn
parameter_list|,
name|void
modifier|*
name|context
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|read_tree
parameter_list|(
name|struct
name|tree
modifier|*
name|tree
parameter_list|,
name|int
name|stage
parameter_list|,
name|struct
name|pathspec
modifier|*
name|pathspec
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

