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

begin_typedef
DECL|typedef|compare_strings_fn
typedef|typedef
name|int
function_decl|(
modifier|*
name|compare_strings_fn
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_typedef

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
DECL|member|cmp
name|compare_strings_fn
name|cmp
decl_stmt|;
comment|/* NULL uses strcmp() */
block|}
struct|;
end_struct

begin_define
DECL|macro|STRING_LIST_INIT_NODUP
define|#
directive|define
name|STRING_LIST_INIT_NODUP
value|{ NULL, 0, 0, 0 }
end_define

begin_define
DECL|macro|STRING_LIST_INIT_DUP
define|#
directive|define
name|STRING_LIST_INIT_DUP
value|{ NULL, 0, 0, 1 }
end_define

begin_function_decl
name|void
name|print_string_list
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|p
parameter_list|,
specifier|const
name|char
modifier|*
name|text
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
comment|/* Use this function or the macro below to iterate over each item */
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
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|string_list_each_func_t
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|for_each_string_list_item
define|#
directive|define
name|for_each_string_list_item
parameter_list|(
name|item
parameter_list|,
name|list
parameter_list|)
define|\
value|for (item = (list)->items; item< (list)->items + (list)->nr; ++item)
end_define

begin_comment
comment|/*  * Apply want to each item in list, retaining only the ones for which  * the function returns true.  If free_util is true, call free() on  * the util members of any items that have to be deleted.  Preserve  * the order of the items that are retained.  */
end_comment

begin_function_decl
name|void
name|filter_string_list
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|int
name|free_util
parameter_list|,
name|string_list_each_func_t
name|want
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Remove any empty strings from the list.  If free_util is true, call  * free() on the util members of any items that have to be deleted.  * Preserve the order of the items that are retained.  */
end_comment

begin_function_decl
name|void
name|string_list_remove_empty_items
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
comment|/*  * Return the longest string in prefixes that is a prefix (in the  * sense of prefixcmp()) of string, or NULL if no such prefix exists.  * This function does not require the string_list to be sorted (it  * does a linear search).  */
end_comment

begin_function_decl
name|char
modifier|*
name|string_list_longest_prefix
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|prefixes
parameter_list|,
specifier|const
name|char
modifier|*
name|string
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
name|struct
name|string_list_item
modifier|*
name|string_list_insert_at_index
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|int
name|insert_at
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|string_list_item
modifier|*
name|string_list_lookup
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

begin_comment
comment|/*  * Remove all but the first of consecutive entries with the same  * string value.  If free_util is true, call free() on the util  * members of any items that have to be deleted.  */
end_comment

begin_function_decl
name|void
name|string_list_remove_duplicates
parameter_list|(
name|struct
name|string_list
modifier|*
name|sorted_list
parameter_list|,
name|int
name|free_util
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Use these functions only on unsorted lists: */
end_comment

begin_comment
comment|/*  * Add string to the end of list.  If list->strdup_string is set, then  * string is copied; otherwise the new string_list_entry refers to the  * input string.  */
end_comment

begin_function_decl
name|struct
name|string_list_item
modifier|*
name|string_list_append
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

begin_comment
comment|/*  * Like string_list_append(), except string is never copied.  When  * list->strdup_strings is set, this function can be used to hand  * ownership of a malloc()ed string to list without making an extra  * copy.  */
end_comment

begin_function_decl
name|struct
name|string_list_item
modifier|*
name|string_list_append_nodup
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|char
modifier|*
name|string
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

begin_function_decl
name|struct
name|string_list_item
modifier|*
name|unsorted_string_list_lookup
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

begin_function_decl
name|void
name|unsorted_string_list_delete_item
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|int
name|i
parameter_list|,
name|int
name|free_util
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Split string into substrings on character delim and append the  * substrings to list.  The input string is not modified.  * list->strdup_strings must be set, as new memory needs to be  * allocated to hold the substrings.  If maxsplit is non-negative,  * then split at most maxsplit times.  Return the number of substrings  * appended to list.  *  * Examples:  *   string_list_split(l, "foo:bar:baz", ':', -1) -> ["foo", "bar", "baz"]  *   string_list_split(l, "foo:bar:baz", ':', 0) -> ["foo:bar:baz"]  *   string_list_split(l, "foo:bar:baz", ':', 1) -> ["foo", "bar:baz"]  *   string_list_split(l, "foo:bar:", ':', -1) -> ["foo", "bar", ""]  *   string_list_split(l, "", ':', -1) -> [""]  *   string_list_split(l, ":", ':', -1) -> ["", ""]  */
end_comment

begin_function_decl
name|int
name|string_list_split
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
parameter_list|,
name|int
name|delim
parameter_list|,
name|int
name|maxsplit
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Like string_list_split(), except that string is split in-place: the  * delimiter characters in string are overwritten with NULs, and the  * new string_list_items point into string (which therefore must not  * be modified or freed while the string_list is in use).  * list->strdup_strings must *not* be set.  */
end_comment

begin_function_decl
name|int
name|string_list_split_in_place
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|char
modifier|*
name|string
parameter_list|,
name|int
name|delim
parameter_list|,
name|int
name|maxsplit
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

