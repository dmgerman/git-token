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
DECL|member|flags
name|unsigned
name|int
name|flags
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|type
specifier|const
name|char
modifier|*
name|type
decl_stmt|;
DECL|member|refs
name|struct
name|object_list
modifier|*
name|refs
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
specifier|extern
name|int
name|nr_objs
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

begin_function_decl
name|struct
name|object
modifier|*
name|lookup_object
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|created_object
parameter_list|(
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
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|add_ref
parameter_list|(
name|struct
name|object
modifier|*
name|refer
parameter_list|,
name|struct
name|object
modifier|*
name|target
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

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* OBJECT_H */
end_comment

end_unit

