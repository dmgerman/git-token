begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|FETCH_PACK_H
end_ifndef

begin_define
DECL|macro|FETCH_PACK_H
define|#
directive|define
name|FETCH_PACK_H
end_define

begin_struct
DECL|struct|fetch_pack_args
struct|struct
name|fetch_pack_args
block|{
DECL|member|uploadpack
specifier|const
name|char
modifier|*
name|uploadpack
decl_stmt|;
DECL|member|unpacklimit
name|int
name|unpacklimit
decl_stmt|;
DECL|member|depth
name|int
name|depth
decl_stmt|;
DECL|member|quiet
name|unsigned
name|quiet
range|:
literal|1
decl_stmt|,
DECL|member|keep_pack
name|keep_pack
range|:
literal|1
decl_stmt|,
DECL|member|lock_pack
name|lock_pack
range|:
literal|1
decl_stmt|,
DECL|member|use_thin_pack
name|use_thin_pack
range|:
literal|1
decl_stmt|,
DECL|member|fetch_all
name|fetch_all
range|:
literal|1
decl_stmt|,
DECL|member|verbose
name|verbose
range|:
literal|1
decl_stmt|,
DECL|member|no_progress
name|no_progress
range|:
literal|1
decl_stmt|,
DECL|member|include_tag
name|include_tag
range|:
literal|1
decl_stmt|,
DECL|member|stateless_rpc
name|stateless_rpc
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|ref
modifier|*
name|fetch_pack
parameter_list|(
name|struct
name|fetch_pack_args
modifier|*
name|args
parameter_list|,
name|int
name|fd
index|[]
parameter_list|,
name|struct
name|child_process
modifier|*
name|conn
parameter_list|,
specifier|const
name|struct
name|ref
modifier|*
name|ref
parameter_list|,
specifier|const
name|char
modifier|*
name|dest
parameter_list|,
name|int
name|nr_heads
parameter_list|,
name|char
modifier|*
modifier|*
name|heads
parameter_list|,
name|char
modifier|*
modifier|*
name|pack_lockfile
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

