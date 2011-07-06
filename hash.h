begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|HASH_H
end_ifndef

begin_define
DECL|macro|HASH_H
define|#
directive|define
name|HASH_H
end_define

begin_comment
comment|/*  * These are some simple generic hash table helper functions.  * Not necessarily suitable for all users, but good for things  * where you want to just keep track of a list of things, and  * have a good hash to use on them.  *  * It keeps the hash table at roughly 50-75% free, so the memory  * cost of the hash table itself is roughly  *  *	3 * 2*sizeof(void *) * nr_of_objects  *  * bytes.  *  * FIXME: on 64-bit architectures, we waste memory. It would be  * good to have just 32-bit pointers, requiring a special allocator  * for hashed entries or something.  */
end_comment

begin_struct
DECL|struct|hash_table_entry
struct|struct
name|hash_table_entry
block|{
DECL|member|hash
name|unsigned
name|int
name|hash
decl_stmt|;
DECL|member|ptr
name|void
modifier|*
name|ptr
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|hash_table
struct|struct
name|hash_table
block|{
DECL|member|size
DECL|member|nr
name|unsigned
name|int
name|size
decl_stmt|,
name|nr
decl_stmt|;
DECL|member|array
name|struct
name|hash_table_entry
modifier|*
name|array
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|void
modifier|*
name|lookup_hash
parameter_list|(
name|unsigned
name|int
name|hash
parameter_list|,
specifier|const
name|struct
name|hash_table
modifier|*
name|table
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
modifier|*
name|insert_hash
parameter_list|(
name|unsigned
name|int
name|hash
parameter_list|,
name|void
modifier|*
name|ptr
parameter_list|,
name|struct
name|hash_table
modifier|*
name|table
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|for_each_hash
parameter_list|(
specifier|const
name|struct
name|hash_table
modifier|*
name|table
parameter_list|,
name|int
function_decl|(
modifier|*
name|fn
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|)
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|free_hash
parameter_list|(
name|struct
name|hash_table
modifier|*
name|table
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|init_hash
specifier|static
specifier|inline
name|void
name|init_hash
parameter_list|(
name|struct
name|hash_table
modifier|*
name|table
parameter_list|)
block|{
name|table
operator|->
name|size
operator|=
literal|0
expr_stmt|;
name|table
operator|->
name|nr
operator|=
literal|0
expr_stmt|;
name|table
operator|->
name|array
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

end_unit

