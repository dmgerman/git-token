begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|OBJECT_H
end_ifndef

begin_define
DECL|macro|OBJECT_H
define|#
directive|define
name|OBJECT_H
end_define

begin_struct
DECL|struct|object_list
struct|struct
name|object_list
block|{
DECL|member|item
name|struct
name|object
modifier|*
name|item
decl_stmt|;
DECL|member|next
name|struct
name|object_list
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|object_refs
struct|struct
name|object_refs
block|{
DECL|member|count
name|unsigned
name|count
decl_stmt|;
DECL|member|base
name|struct
name|object
modifier|*
name|base
decl_stmt|;
DECL|member|ref
name|struct
name|object
modifier|*
name|ref
index|[
name|FLEX_ARRAY
index|]
decl_stmt|;
comment|/* more */
block|}
struct|;
end_struct

begin_struct
DECL|struct|object_array
struct|struct
name|object_array
block|{
DECL|member|nr
name|unsigned
name|int
name|nr
decl_stmt|;
DECL|member|alloc
name|unsigned
name|int
name|alloc
decl_stmt|;
DECL|struct|object_array_entry
struct|struct
name|object_array_entry
block|{
DECL|member|item
name|struct
name|object
modifier|*
name|item
decl_stmt|;
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|objects
block|}
modifier|*
name|objects
struct|;
block|}
struct|;
end_struct

begin_define
DECL|macro|TYPE_BITS
define|#
directive|define
name|TYPE_BITS
value|3
end_define

begin_define
DECL|macro|FLAG_BITS
define|#
directive|define
name|FLAG_BITS
value|27
end_define

begin_define
DECL|macro|TYPE_NONE
define|#
directive|define
name|TYPE_NONE
value|0
end_define

begin_define
DECL|macro|TYPE_BLOB
define|#
directive|define
name|TYPE_BLOB
value|1
end_define

begin_define
DECL|macro|TYPE_TREE
define|#
directive|define
name|TYPE_TREE
value|2
end_define

begin_define
DECL|macro|TYPE_COMMIT
define|#
directive|define
name|TYPE_COMMIT
value|3
end_define

begin_define
DECL|macro|TYPE_TAG
define|#
directive|define
name|TYPE_TAG
value|4
end_define

begin_define
DECL|macro|TYPE_BAD
define|#
directive|define
name|TYPE_BAD
value|5
end_define

begin_struct
DECL|struct|object
struct|struct
name|object
block|{
DECL|member|parsed
name|unsigned
name|parsed
range|:
literal|1
decl_stmt|;
DECL|member|used
name|unsigned
name|used
range|:
literal|1
decl_stmt|;
DECL|member|type
name|unsigned
name|type
range|:
name|TYPE_BITS
decl_stmt|;
DECL|member|flags
name|unsigned
name|flags
range|:
name|FLAG_BITS
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
specifier|extern
name|int
name|track_object_refs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|obj_allocs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|struct
name|object
modifier|*
modifier|*
name|objs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
specifier|const
name|char
modifier|*
name|type_names
index|[]
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|typename
specifier|static
specifier|inline
specifier|const
name|char
modifier|*
name|typename
parameter_list|(
name|unsigned
name|int
name|type
parameter_list|)
block|{
return|return
name|type_names
index|[
name|type
operator|>
name|TYPE_TAG
condition|?
name|TYPE_BAD
else|:
name|type
index|]
return|;
block|}
end_function

begin_function_decl
specifier|extern
name|struct
name|object_refs
modifier|*
name|lookup_object_refs
parameter_list|(
name|struct
name|object
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Internal only **/
end_comment

begin_function_decl
name|struct
name|object
modifier|*
name|lookup_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Returns the object, having looked it up as being the given type. **/
end_comment

begin_function_decl
name|struct
name|object
modifier|*
name|lookup_object_type
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|type
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|created_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|struct
name|object
modifier|*
name|obj
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Returns the object, having parsed it to find out what it is. **/
end_comment

begin_function_decl
name|struct
name|object
modifier|*
name|parse_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Returns the object, with potentially excess memory allocated. **/
end_comment

begin_function_decl
name|struct
name|object
modifier|*
name|lookup_unknown_object
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
name|struct
name|object_refs
modifier|*
name|alloc_object_refs
parameter_list|(
name|unsigned
name|count
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|set_object_refs
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|struct
name|object_refs
modifier|*
name|refs
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|mark_reachable
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|unsigned
name|int
name|mask
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|object_list
modifier|*
name|object_list_insert
parameter_list|(
name|struct
name|object
modifier|*
name|item
parameter_list|,
name|struct
name|object_list
modifier|*
modifier|*
name|list_p
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|object_list_append
parameter_list|(
name|struct
name|object
modifier|*
name|item
parameter_list|,
name|struct
name|object_list
modifier|*
modifier|*
name|list_p
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|unsigned
name|object_list_length
parameter_list|(
name|struct
name|object_list
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|object_list_contains
parameter_list|(
name|struct
name|object_list
modifier|*
name|list
parameter_list|,
name|struct
name|object
modifier|*
name|obj
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Object array handling .. */
end_comment

begin_function_decl
name|void
name|add_object_array
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|struct
name|object_array
modifier|*
name|array
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* OBJECT_H */
end_comment

end_unit

