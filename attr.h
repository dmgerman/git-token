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

begin_comment
comment|/*  * Given a string, return the gitattribute object that  * corresponds to it.  */
end_comment

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

begin_decl_stmt
specifier|extern
specifier|const
name|char
name|git_attr__true
index|[]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
specifier|const
name|char
name|git_attr__false
index|[]
decl_stmt|;
end_decl_stmt

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
value|((v) == git_attr__true)
end_define

begin_define
DECL|macro|ATTR_FALSE
define|#
directive|define
name|ATTR_FALSE
parameter_list|(
name|v
parameter_list|)
value|((v) == git_attr__false)
end_define

begin_define
DECL|macro|ATTR_UNSET
define|#
directive|define
name|ATTR_UNSET
parameter_list|(
name|v
parameter_list|)
value|((v) == NULL)
end_define

begin_comment
comment|/*  * Send one or more git_attr_check to git_checkattr(), and  * each 'value' member tells what its value is.  * Unset one is returned as NULL.  */
end_comment

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
specifier|const
name|char
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

begin_enum
DECL|enum|git_attr_direction
enum|enum
name|git_attr_direction
block|{
DECL|enumerator|GIT_ATTR_CHECKIN
name|GIT_ATTR_CHECKIN
block|,
DECL|enumerator|GIT_ATTR_CHECKOUT
name|GIT_ATTR_CHECKOUT
block|,
DECL|enumerator|GIT_ATTR_INDEX
name|GIT_ATTR_INDEX
block|, }
enum|;
end_enum

begin_function_decl
name|void
name|git_attr_set_direction
parameter_list|(
name|enum
name|git_attr_direction
parameter_list|,
name|struct
name|index_state
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

