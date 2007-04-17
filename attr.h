begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|ATTR_H
end_ifndef

begin_define
DECL|macro|ATTR_H
define|#
directive|define
name|ATTR_H
end_define

begin_comment
comment|/* An attribute is a pointer to this opaque structure */
end_comment

begin_struct_decl
struct_decl|struct
name|git_attr
struct_decl|;
end_struct_decl

begin_function_decl
name|struct
name|git_attr
modifier|*
name|git_attr
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Internal use */
end_comment

begin_define
DECL|macro|ATTR__TRUE
define|#
directive|define
name|ATTR__TRUE
value|((void *) 1)
end_define

begin_define
DECL|macro|ATTR__FALSE
define|#
directive|define
name|ATTR__FALSE
value|((void *) 0)
end_define

begin_define
DECL|macro|ATTR__UNSET
define|#
directive|define
name|ATTR__UNSET
value|((void *) -1)
end_define

begin_comment
comment|/* For public to check git_attr_check results */
end_comment

begin_define
DECL|macro|ATTR_TRUE
define|#
directive|define
name|ATTR_TRUE
parameter_list|(
name|v
parameter_list|)
value|((v) == ATTR__TRUE)
end_define

begin_define
DECL|macro|ATTR_FALSE
define|#
directive|define
name|ATTR_FALSE
parameter_list|(
name|v
parameter_list|)
value|((v) == ATTR__FALSE)
end_define

begin_define
DECL|macro|ATTR_UNSET
define|#
directive|define
name|ATTR_UNSET
parameter_list|(
name|v
parameter_list|)
value|((v) == ATTR__UNSET)
end_define

begin_struct
DECL|struct|git_attr_check
struct|struct
name|git_attr_check
block|{
DECL|member|attr
name|struct
name|git_attr
modifier|*
name|attr
decl_stmt|;
DECL|member|value
name|void
modifier|*
name|value
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|git_checkattr
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
parameter_list|,
name|struct
name|git_attr_check
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* ATTR_H */
end_comment

end_unit

