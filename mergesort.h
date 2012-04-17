begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|MERGESORT_H
end_ifndef

begin_define
DECL|macro|MERGESORT_H
define|#
directive|define
name|MERGESORT_H
end_define

begin_comment
comment|/*  * Sort linked list in place.  * - get_next_fn() returns the next element given an element of a linked list.  * - set_next_fn() takes two elements A and B, and makes B the "next" element  *   of A on the list.  * - compare_fn() takes two elements A and B, and returns negative, 0, positive  *   as the same sign as "subtracting" B from A.  */
end_comment

begin_function_decl
name|void
modifier|*
name|llist_mergesort
parameter_list|(
name|void
modifier|*
name|list
parameter_list|,
name|void
modifier|*
function_decl|(
modifier|*
name|get_next_fn
function_decl|)
parameter_list|(
specifier|const
name|void
modifier|*
parameter_list|)
parameter_list|,
name|void
function_decl|(
modifier|*
name|set_next_fn
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|)
parameter_list|,
name|int
function_decl|(
modifier|*
name|compare_fn
function_decl|)
parameter_list|(
specifier|const
name|void
modifier|*
parameter_list|,
specifier|const
name|void
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

