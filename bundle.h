begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|BUNDLE_H
end_ifndef

begin_define
DECL|macro|BUNDLE_H
define|#
directive|define
name|BUNDLE_H
end_define

begin_struct
DECL|struct|ref_list
struct|struct
name|ref_list
block|{
DECL|member|nr
DECL|member|alloc
name|unsigned
name|int
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
DECL|struct|ref_list_entry
struct|struct
name|ref_list_entry
block|{
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|name
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|list
block|}
modifier|*
name|list
struct|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|bundle_header
struct|struct
name|bundle_header
block|{
DECL|member|prerequisites
name|struct
name|ref_list
name|prerequisites
decl_stmt|;
DECL|member|references
name|struct
name|ref_list
name|references
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|read_bundle_header
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|struct
name|bundle_header
modifier|*
name|header
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|create_bundle
parameter_list|(
name|struct
name|bundle_header
modifier|*
name|header
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|verify_bundle
parameter_list|(
name|struct
name|bundle_header
modifier|*
name|header
parameter_list|,
name|int
name|verbose
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|unbundle
parameter_list|(
name|struct
name|bundle_header
modifier|*
name|header
parameter_list|,
name|int
name|bundle_fd
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|list_bundle_refs
parameter_list|(
name|struct
name|bundle_header
modifier|*
name|header
parameter_list|,
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

