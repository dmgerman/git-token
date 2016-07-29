begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|MRU_H
end_ifndef

begin_define
DECL|macro|MRU_H
define|#
directive|define
name|MRU_H
end_define

begin_comment
comment|/**  * A simple most-recently-used cache, backed by a doubly-linked list.  *  * Usage is roughly:  *  *   // Create a list.  Zero-initialization is required.  *   static struct mru cache;  *   mru_append(&cache, item);  *   ...  *  *   // Iterate in MRU order.  *   struct mru_entry *p;  *   for (p = cache.head; p; p = p->next) {  *	if (matches(p->item))  *		break;  *   }  *  *   // Mark an item as used, moving it to the front of the list.  *   mru_mark(&cache, p);  *  *   // Reset the list to empty, cleaning up all resources.  *   mru_clear(&cache);  *  * Note that you SHOULD NOT call mru_mark() and then continue traversing the  * list; it reorders the marked item to the front of the list, and therefore  * you will begin traversing the whole list again.  */
end_comment

begin_struct
DECL|struct|mru_entry
struct|struct
name|mru_entry
block|{
DECL|member|item
name|void
modifier|*
name|item
decl_stmt|;
DECL|member|prev
DECL|member|next
name|struct
name|mru_entry
modifier|*
name|prev
decl_stmt|,
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|mru
struct|struct
name|mru
block|{
DECL|member|head
DECL|member|tail
name|struct
name|mru_entry
modifier|*
name|head
decl_stmt|,
modifier|*
name|tail
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|void
name|mru_append
parameter_list|(
name|struct
name|mru
modifier|*
name|mru
parameter_list|,
name|void
modifier|*
name|item
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|mru_mark
parameter_list|(
name|struct
name|mru
modifier|*
name|mru
parameter_list|,
name|struct
name|mru_entry
modifier|*
name|entry
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|mru_clear
parameter_list|(
name|struct
name|mru
modifier|*
name|mru
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* MRU_H */
end_comment

end_unit

