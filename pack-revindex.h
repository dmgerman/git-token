begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PACK_REVINDEX_H
end_ifndef

begin_define
DECL|macro|PACK_REVINDEX_H
define|#
directive|define
name|PACK_REVINDEX_H
end_define

begin_struct
DECL|struct|revindex_entry
struct|struct
name|revindex_entry
block|{
DECL|member|offset
name|off_t
name|offset
decl_stmt|;
DECL|member|nr
name|unsigned
name|int
name|nr
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|revindex_entry
modifier|*
name|find_pack_revindex
parameter_list|(
name|struct
name|packed_git
modifier|*
name|p
parameter_list|,
name|off_t
name|ofs
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|discard_revindex
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

