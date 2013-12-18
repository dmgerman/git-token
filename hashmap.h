begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|HASHMAP_H
end_ifndef

begin_define
DECL|macro|HASHMAP_H
define|#
directive|define
name|HASHMAP_H
end_define

begin_comment
comment|/*  * Generic implementation of hash-based key-value mappings.  * See Documentation/technical/api-hashmap.txt.  */
end_comment

begin_comment
comment|/* FNV-1 functions */
end_comment

begin_function_decl
specifier|extern
name|unsigned
name|int
name|strhash
parameter_list|(
specifier|const
name|char
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|unsigned
name|int
name|strihash
parameter_list|(
specifier|const
name|char
modifier|*
name|buf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|unsigned
name|int
name|memhash
parameter_list|(
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|unsigned
name|int
name|memihash
parameter_list|(
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* data structures */
end_comment

begin_struct
DECL|struct|hashmap_entry
struct|struct
name|hashmap_entry
block|{
DECL|member|next
name|struct
name|hashmap_entry
modifier|*
name|next
decl_stmt|;
DECL|member|hash
name|unsigned
name|int
name|hash
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|typedef|hashmap_cmp_fn
typedef|typedef
name|int
function_decl|(
modifier|*
name|hashmap_cmp_fn
function_decl|)
parameter_list|(
specifier|const
name|void
modifier|*
name|entry
parameter_list|,
specifier|const
name|void
modifier|*
name|entry_or_key
parameter_list|,
specifier|const
name|void
modifier|*
name|keydata
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|hashmap
struct|struct
name|hashmap
block|{
DECL|member|table
name|struct
name|hashmap_entry
modifier|*
modifier|*
name|table
decl_stmt|;
DECL|member|cmpfn
name|hashmap_cmp_fn
name|cmpfn
decl_stmt|;
DECL|member|size
DECL|member|tablesize
DECL|member|grow_at
DECL|member|shrink_at
name|unsigned
name|int
name|size
decl_stmt|,
name|tablesize
decl_stmt|,
name|grow_at
decl_stmt|,
name|shrink_at
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|hashmap_iter
struct|struct
name|hashmap_iter
block|{
DECL|member|map
name|struct
name|hashmap
modifier|*
name|map
decl_stmt|;
DECL|member|next
name|struct
name|hashmap_entry
modifier|*
name|next
decl_stmt|;
DECL|member|tablepos
name|unsigned
name|int
name|tablepos
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* hashmap functions */
end_comment

begin_function_decl
specifier|extern
name|void
name|hashmap_init
parameter_list|(
name|struct
name|hashmap
modifier|*
name|map
parameter_list|,
name|hashmap_cmp_fn
name|equals_function
parameter_list|,
name|size_t
name|initial_size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|hashmap_free
parameter_list|(
name|struct
name|hashmap
modifier|*
name|map
parameter_list|,
name|int
name|free_entries
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* hashmap_entry functions */
end_comment

begin_function
DECL|function|hashmap_entry_init
specifier|static
specifier|inline
name|void
name|hashmap_entry_init
parameter_list|(
name|void
modifier|*
name|entry
parameter_list|,
name|unsigned
name|int
name|hash
parameter_list|)
block|{
name|struct
name|hashmap_entry
modifier|*
name|e
init|=
name|entry
decl_stmt|;
name|e
operator|->
name|hash
operator|=
name|hash
expr_stmt|;
name|e
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function_decl
specifier|extern
name|void
modifier|*
name|hashmap_get
parameter_list|(
specifier|const
name|struct
name|hashmap
modifier|*
name|map
parameter_list|,
specifier|const
name|void
modifier|*
name|key
parameter_list|,
specifier|const
name|void
modifier|*
name|keydata
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|hashmap_get_next
parameter_list|(
specifier|const
name|struct
name|hashmap
modifier|*
name|map
parameter_list|,
specifier|const
name|void
modifier|*
name|entry
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|hashmap_add
parameter_list|(
name|struct
name|hashmap
modifier|*
name|map
parameter_list|,
name|void
modifier|*
name|entry
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|hashmap_put
parameter_list|(
name|struct
name|hashmap
modifier|*
name|map
parameter_list|,
name|void
modifier|*
name|entry
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|hashmap_remove
parameter_list|(
name|struct
name|hashmap
modifier|*
name|map
parameter_list|,
specifier|const
name|void
modifier|*
name|key
parameter_list|,
specifier|const
name|void
modifier|*
name|keydata
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* hashmap_iter functions */
end_comment

begin_function_decl
specifier|extern
name|void
name|hashmap_iter_init
parameter_list|(
name|struct
name|hashmap
modifier|*
name|map
parameter_list|,
name|struct
name|hashmap_iter
modifier|*
name|iter
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|hashmap_iter_next
parameter_list|(
name|struct
name|hashmap_iter
modifier|*
name|iter
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|hashmap_iter_first
specifier|static
specifier|inline
name|void
modifier|*
name|hashmap_iter_first
parameter_list|(
name|struct
name|hashmap
modifier|*
name|map
parameter_list|,
name|struct
name|hashmap_iter
modifier|*
name|iter
parameter_list|)
block|{
name|hashmap_iter_init
argument_list|(
name|map
argument_list|,
name|iter
argument_list|)
expr_stmt|;
return|return
name|hashmap_iter_next
argument_list|(
name|iter
argument_list|)
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

end_unit

