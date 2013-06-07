begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PRIO_QUEUE_H
end_ifndef

begin_define
DECL|macro|PRIO_QUEUE_H
define|#
directive|define
name|PRIO_QUEUE_H
end_define

begin_comment
comment|/*  * A priority queue implementation, primarily for keeping track of  * commits in the 'date-order' so that we process them from new to old  * as they are discovered, but can be used to hold any pointer to  * struct.  The caller is responsible for supplying a function to  * compare two "things".  *  * Alternatively, this data structure can also be used as a LIFO stack  * by specifying NULL as the comparison function.  */
end_comment

begin_comment
comment|/*  * Compare two "things", one and two; the third parameter is cb_data  * in the prio_queue structure.  The result is returned as a sign of  * the return value, being the same as the sign of the result of  * subtracting "two" from "one" (i.e. negative if "one" sorts earlier  * than "two").  */
end_comment

begin_typedef
DECL|typedef|prio_queue_compare_fn
typedef|typedef
name|int
function_decl|(
modifier|*
name|prio_queue_compare_fn
function_decl|)
parameter_list|(
specifier|const
name|void
modifier|*
name|one
parameter_list|,
specifier|const
name|void
modifier|*
name|two
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|prio_queue
struct|struct
name|prio_queue
block|{
DECL|member|compare
name|prio_queue_compare_fn
name|compare
decl_stmt|;
DECL|member|cb_data
name|void
modifier|*
name|cb_data
decl_stmt|;
DECL|member|alloc
DECL|member|nr
name|int
name|alloc
decl_stmt|,
name|nr
decl_stmt|;
DECL|member|array
name|void
modifier|*
modifier|*
name|array
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * Add the "thing" to the queue.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|prio_queue_put
parameter_list|(
name|struct
name|prio_queue
modifier|*
parameter_list|,
name|void
modifier|*
name|thing
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Extract the "thing" that compares the smallest out of the queue,  * or NULL.  If compare function is NULL, the queue acts as a LIFO  * stack.  */
end_comment

begin_function_decl
specifier|extern
name|void
modifier|*
name|prio_queue_get
parameter_list|(
name|struct
name|prio_queue
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|clear_prio_queue
parameter_list|(
name|struct
name|prio_queue
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Reverse the LIFO elements */
end_comment

begin_function_decl
specifier|extern
name|void
name|prio_queue_reverse
parameter_list|(
name|struct
name|prio_queue
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PRIO_QUEUE_H */
end_comment

end_unit

