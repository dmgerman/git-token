begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SHA1_ARRAY_H
end_ifndef

begin_define
DECL|macro|SHA1_ARRAY_H
define|#
directive|define
name|SHA1_ARRAY_H
end_define

begin_struct
DECL|struct|sha1_array
struct|struct
name|sha1_array
block|{
DECL|member|sha1
name|unsigned
name|char
argument_list|(
operator|*
name|sha1
argument_list|)
decl|[20]
decl_stmt|;
DECL|member|nr
name|int
name|nr
decl_stmt|;
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|sorted
name|int
name|sorted
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|SHA1_ARRAY_INIT
define|#
directive|define
name|SHA1_ARRAY_INIT
value|{ NULL, 0, 0, 0 }
end_define

begin_function_decl
name|void
name|sha1_array_append
parameter_list|(
name|struct
name|sha1_array
modifier|*
name|array
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|sha1_array_sort
parameter_list|(
name|struct
name|sha1_array
modifier|*
name|array
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|sha1_array_lookup
parameter_list|(
name|struct
name|sha1_array
modifier|*
name|array
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|sha1_array_clear
parameter_list|(
name|struct
name|sha1_array
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
comment|/* SHA1_ARRAY_H */
end_comment

end_unit

