begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|STRING_LIST_H
end_ifndef

begin_define
DECL|macro|STRING_LIST_H
define|#
directive|define
name|STRING_LIST_H
end_define

begin_struct
DECL|struct|string_list_item
struct|struct
name|string_list_item
block|{
DECL|member|string
name|char
modifier|*
name|string
decl_stmt|;
DECL|member|util
name|void
modifier|*
name|util
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|string_list
struct|struct
name|string_list
block|{
DECL|member|items
name|struct
name|string_list_item
modifier|*
name|items
decl_stmt|;
DECL|member|nr
DECL|member|alloc
name|unsigned
name|int
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
DECL|member|strdup_strings
name|unsigned
name|int
name|strdup_strings
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|void
name|print_string_list
parameter_list|(
specifier|const
name|char
modifier|*
name|text
parameter_list|,
specifier|const
name|struct
name|string_list
modifier|*
name|p
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|string_list_clear
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|int
name|free_util
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Use this function to call a custom clear function on each util pointer */
end_comment

begin_comment
comment|/* The string associated with the util pointer is passed as the second argument */
end_comment

begin_typedef
DECL|typedef|string_list_clear_func_t
typedef|typedef
name|void
function_decl|(
modifier|*
name|string_list_clear_func_t
function_decl|)
parameter_list|(
name|void
modifier|*
name|p
parameter_list|,
specifier|const
name|char
modifier|*
name|str
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|void
name|string_list_clear_func
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|string_list_clear_func_t
name|clearfunc
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Use this function to iterate over each item */
end_comment

begin_typedef
DECL|typedef|string_list_each_func_t
typedef|typedef
name|int
function_decl|(
modifier|*
name|string_list_each_func_t
function_decl|)
parameter_list|(
name|struct
name|string_list_item
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|int
name|for_each_string_list
parameter_list|(
name|string_list_each_func_t
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Use these functions only on sorted lists: */
end_comment

begin_function_decl
name|int
name|string_list_has_string
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|string_list_find_insert_index
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|int
name|negative_existing_index
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|string_list_item
modifier|*
name|string_list_insert
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|string_list_item
modifier|*
name|string_list_insert_at_index
parameter_list|(
name|int
name|insert_at
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|string_list_item
modifier|*
name|string_list_lookup
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Use these functions only on unsorted lists: */
end_comment

begin_function_decl
name|struct
name|string_list_item
modifier|*
name|string_list_append
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|sort_string_list
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|unsorted_string_list_has_string
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* STRING_LIST_H */
end_comment

end_unit

