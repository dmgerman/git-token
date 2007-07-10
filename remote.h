begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|REMOTE_H
end_ifndef

begin_define
DECL|macro|REMOTE_H
define|#
directive|define
name|REMOTE_H
end_define

begin_struct
DECL|struct|remote
struct|struct
name|remote
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|uri
specifier|const
name|char
modifier|*
modifier|*
name|uri
decl_stmt|;
DECL|member|uri_nr
name|int
name|uri_nr
decl_stmt|;
DECL|member|push_refspec
specifier|const
name|char
modifier|*
modifier|*
name|push_refspec
decl_stmt|;
DECL|member|push
name|struct
name|refspec
modifier|*
name|push
decl_stmt|;
DECL|member|push_refspec_nr
name|int
name|push_refspec_nr
decl_stmt|;
DECL|member|fetch_refspec
specifier|const
name|char
modifier|*
modifier|*
name|fetch_refspec
decl_stmt|;
DECL|member|fetch
name|struct
name|refspec
modifier|*
name|fetch
decl_stmt|;
DECL|member|fetch_refspec_nr
name|int
name|fetch_refspec_nr
decl_stmt|;
DECL|member|receivepack
specifier|const
name|char
modifier|*
name|receivepack
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|remote
modifier|*
name|remote_get
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|remote_has_uri
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
specifier|const
name|char
modifier|*
name|uri
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|refspec
struct|struct
name|refspec
block|{
DECL|member|force
name|unsigned
name|force
range|:
literal|1
decl_stmt|;
DECL|member|pattern
name|unsigned
name|pattern
range|:
literal|1
decl_stmt|;
DECL|member|src
specifier|const
name|char
modifier|*
name|src
decl_stmt|;
DECL|member|dst
name|char
modifier|*
name|dst
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|ref
modifier|*
name|alloc_ref
parameter_list|(
name|unsigned
name|namelen
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Frees the entire list and peers of elements.  */
end_comment

begin_function_decl
name|void
name|free_refs
parameter_list|(
name|struct
name|ref
modifier|*
name|ref
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|match_refs
parameter_list|(
name|struct
name|ref
modifier|*
name|src
parameter_list|,
name|struct
name|ref
modifier|*
name|dst
parameter_list|,
name|struct
name|ref
modifier|*
modifier|*
modifier|*
name|dst_tail
parameter_list|,
name|int
name|nr_refspec
parameter_list|,
name|char
modifier|*
modifier|*
name|refspec
parameter_list|,
name|int
name|all
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * For the given remote, reads the refspec's src and sets the other fields.  */
end_comment

begin_function_decl
name|int
name|remote_find_tracking
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
name|struct
name|refspec
modifier|*
name|refspec
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

