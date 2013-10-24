begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PACK_OBJECTS_H
end_ifndef

begin_define
DECL|macro|PACK_OBJECTS_H
define|#
directive|define
name|PACK_OBJECTS_H
end_define

begin_struct
DECL|struct|object_entry
struct|struct
name|object_entry
block|{
DECL|member|idx
name|struct
name|pack_idx_entry
name|idx
decl_stmt|;
DECL|member|size
name|unsigned
name|long
name|size
decl_stmt|;
comment|/* uncompressed size */
DECL|member|in_pack
name|struct
name|packed_git
modifier|*
name|in_pack
decl_stmt|;
comment|/* already in pack */
DECL|member|in_pack_offset
name|off_t
name|in_pack_offset
decl_stmt|;
DECL|member|delta
name|struct
name|object_entry
modifier|*
name|delta
decl_stmt|;
comment|/* delta base object */
DECL|member|delta_child
name|struct
name|object_entry
modifier|*
name|delta_child
decl_stmt|;
comment|/* deltified objects who bases me */
DECL|member|delta_sibling
name|struct
name|object_entry
modifier|*
name|delta_sibling
decl_stmt|;
comment|/* other deltified objects who 					     * uses the same base as me 					     */
DECL|member|delta_data
name|void
modifier|*
name|delta_data
decl_stmt|;
comment|/* cached delta (uncompressed) */
DECL|member|delta_size
name|unsigned
name|long
name|delta_size
decl_stmt|;
comment|/* delta data size (uncompressed) */
DECL|member|z_delta_size
name|unsigned
name|long
name|z_delta_size
decl_stmt|;
comment|/* delta data size (compressed) */
DECL|member|type
name|enum
name|object_type
name|type
decl_stmt|;
DECL|member|in_pack_type
name|enum
name|object_type
name|in_pack_type
decl_stmt|;
comment|/* could be delta */
DECL|member|hash
name|uint32_t
name|hash
decl_stmt|;
comment|/* name hint hash */
DECL|member|in_pack_header_size
name|unsigned
name|char
name|in_pack_header_size
decl_stmt|;
DECL|member|preferred_base
name|unsigned
name|preferred_base
range|:
literal|1
decl_stmt|;
comment|/* 				    * we do not pack this, but is available 				    * to be used as the base object to delta 				    * objects against. 				    */
DECL|member|no_try_delta
name|unsigned
name|no_try_delta
range|:
literal|1
decl_stmt|;
DECL|member|tagged
name|unsigned
name|tagged
range|:
literal|1
decl_stmt|;
comment|/* near the very tip of refs */
DECL|member|filled
name|unsigned
name|filled
range|:
literal|1
decl_stmt|;
comment|/* assigned write-order */
block|}
struct|;
end_struct

begin_struct
DECL|struct|packing_data
struct|struct
name|packing_data
block|{
DECL|member|objects
name|struct
name|object_entry
modifier|*
name|objects
decl_stmt|;
DECL|member|nr_objects
DECL|member|nr_alloc
name|uint32_t
name|nr_objects
decl_stmt|,
name|nr_alloc
decl_stmt|;
DECL|member|index
name|int32_t
modifier|*
name|index
decl_stmt|;
DECL|member|index_size
name|uint32_t
name|index_size
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|object_entry
modifier|*
name|packlist_alloc
parameter_list|(
name|struct
name|packing_data
modifier|*
name|pdata
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|uint32_t
name|index_pos
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|object_entry
modifier|*
name|packlist_find
parameter_list|(
name|struct
name|packing_data
modifier|*
name|pdata
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|uint32_t
modifier|*
name|index_pos
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

