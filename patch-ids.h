begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PATCH_IDS_H
end_ifndef

begin_define
DECL|macro|PATCH_IDS_H
define|#
directive|define
name|PATCH_IDS_H
end_define

begin_struct
DECL|struct|patch_id
struct|struct
name|patch_id
block|{
DECL|member|patch_id
name|unsigned
name|char
name|patch_id
index|[
literal|20
index|]
decl_stmt|;
DECL|member|seen
name|char
name|seen
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|patch_ids
struct|struct
name|patch_ids
block|{
DECL|member|diffopts
name|struct
name|diff_options
name|diffopts
decl_stmt|;
DECL|member|nr
DECL|member|alloc
name|int
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
DECL|member|table
name|struct
name|patch_id
modifier|*
modifier|*
name|table
decl_stmt|;
DECL|member|patches
name|struct
name|patch_id_bucket
modifier|*
name|patches
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|init_patch_ids
parameter_list|(
name|struct
name|patch_ids
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|free_patch_ids
parameter_list|(
name|struct
name|patch_ids
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|patch_id
modifier|*
name|add_commit_patch_id
parameter_list|(
name|struct
name|commit
modifier|*
parameter_list|,
name|struct
name|patch_ids
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|patch_id
modifier|*
name|has_commit_patch_id
parameter_list|(
name|struct
name|commit
modifier|*
parameter_list|,
name|struct
name|patch_ids
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PATCH_IDS_H */
end_comment

end_unit

