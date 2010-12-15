begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|TREE_WALK_H
end_ifndef

begin_define
DECL|macro|TREE_WALK_H
define|#
directive|define
name|TREE_WALK_H
end_define

begin_struct
DECL|struct|name_entry
struct|struct
name|name_entry
block|{
DECL|member|sha1
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
DECL|member|path
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
DECL|member|mode
name|unsigned
name|int
name|mode
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|tree_desc
struct|struct
name|tree_desc
block|{
DECL|member|buffer
specifier|const
name|void
modifier|*
name|buffer
decl_stmt|;
DECL|member|entry
name|struct
name|name_entry
name|entry
decl_stmt|;
DECL|member|size
name|unsigned
name|int
name|size
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|tree_entry_extract
specifier|static
specifier|inline
specifier|const
name|unsigned
name|char
modifier|*
name|tree_entry_extract
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|pathp
parameter_list|,
name|unsigned
name|int
modifier|*
name|modep
parameter_list|)
block|{
operator|*
name|pathp
operator|=
name|desc
operator|->
name|entry
operator|.
name|path
expr_stmt|;
operator|*
name|modep
operator|=
name|canon_mode
argument_list|(
name|desc
operator|->
name|entry
operator|.
name|mode
argument_list|)
expr_stmt|;
return|return
name|desc
operator|->
name|entry
operator|.
name|sha1
return|;
block|}
end_function

begin_function
DECL|function|tree_entry_len
specifier|static
specifier|inline
name|int
name|tree_entry_len
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
return|return
operator|(
specifier|const
name|char
operator|*
operator|)
name|sha1
operator|-
name|name
operator|-
literal|1
return|;
block|}
end_function

begin_function_decl
name|void
name|update_tree_entry
parameter_list|(
name|struct
name|tree_desc
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|init_tree_desc
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|,
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Helper function that does both tree_entry_extract() and update_tree_entry()  * and returns true for success  */
end_comment

begin_function_decl
name|int
name|tree_entry
parameter_list|(
name|struct
name|tree_desc
modifier|*
parameter_list|,
name|struct
name|name_entry
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
modifier|*
name|fill_tree_descriptor
parameter_list|(
name|struct
name|tree_desc
modifier|*
name|desc
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_struct_decl
struct_decl|struct
name|traverse_info
struct_decl|;
end_struct_decl

begin_typedef
DECL|typedef|traverse_callback_t
typedef|typedef
name|int
function_decl|(
modifier|*
name|traverse_callback_t
function_decl|)
parameter_list|(
name|int
name|n
parameter_list|,
name|unsigned
name|long
name|mask
parameter_list|,
name|unsigned
name|long
name|dirmask
parameter_list|,
name|struct
name|name_entry
modifier|*
name|entry
parameter_list|,
name|struct
name|traverse_info
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|int
name|traverse_trees
parameter_list|(
name|int
name|n
parameter_list|,
name|struct
name|tree_desc
modifier|*
name|t
parameter_list|,
name|struct
name|traverse_info
modifier|*
name|info
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|traverse_info
struct|struct
name|traverse_info
block|{
DECL|member|prev
name|struct
name|traverse_info
modifier|*
name|prev
decl_stmt|;
DECL|member|name
name|struct
name|name_entry
name|name
decl_stmt|;
DECL|member|pathlen
name|int
name|pathlen
decl_stmt|;
DECL|member|conflicts
name|unsigned
name|long
name|conflicts
decl_stmt|;
DECL|member|fn
name|traverse_callback_t
name|fn
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|show_all_errors
name|int
name|show_all_errors
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|get_tree_entry
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
name|unsigned
name|char
modifier|*
parameter_list|,
name|unsigned
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|make_traverse_path
parameter_list|(
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|struct
name|traverse_info
modifier|*
name|info
parameter_list|,
specifier|const
name|struct
name|name_entry
modifier|*
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|setup_traverse_info
parameter_list|(
name|struct
name|traverse_info
modifier|*
name|info
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|traverse_path_len
specifier|static
specifier|inline
name|int
name|traverse_path_len
parameter_list|(
specifier|const
name|struct
name|traverse_info
modifier|*
name|info
parameter_list|,
specifier|const
name|struct
name|name_entry
modifier|*
name|n
parameter_list|)
block|{
return|return
name|info
operator|->
name|pathlen
operator|+
name|tree_entry_len
argument_list|(
name|n
operator|->
name|path
argument_list|,
name|n
operator|->
name|sha1
argument_list|)
return|;
block|}
end_function

begin_function_decl
specifier|extern
name|int
name|tree_entry_interesting
parameter_list|(
specifier|const
name|struct
name|name_entry
modifier|*
parameter_list|,
specifier|const
name|struct
name|strbuf
modifier|*
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|ps
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

