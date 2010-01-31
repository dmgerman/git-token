begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|GIT_FSCK_H
end_ifndef

begin_define
DECL|macro|GIT_FSCK_H
define|#
directive|define
name|GIT_FSCK_H
end_define

begin_define
DECL|macro|FSCK_ERROR
define|#
directive|define
name|FSCK_ERROR
value|1
end_define

begin_define
DECL|macro|FSCK_WARN
define|#
directive|define
name|FSCK_WARN
value|2
end_define

begin_comment
comment|/*  * callback function for fsck_walk  * type is the expected type of the object or OBJ_ANY  * the return value is:  *     0	everything OK  *<0	error signaled and abort  *>0	error signaled and do not abort  */
end_comment

begin_typedef
DECL|typedef|fsck_walk_func
typedef|typedef
name|int
function_decl|(
modifier|*
name|fsck_walk_func
function_decl|)
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|int
name|type
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_typedef

begin_comment
comment|/* callback for fsck_object, type is FSCK_ERROR or FSCK_WARN */
end_comment

begin_typedef
DECL|typedef|fsck_error
typedef|typedef
name|int
function_decl|(
modifier|*
name|fsck_error
function_decl|)
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|int
name|type
parameter_list|,
specifier|const
name|char
modifier|*
name|err
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|int
name|fsck_error_function
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|int
name|type
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* descend in all linked child objects  * the return value is:  *    -1	error in processing the object  *<0	return value of the callback, which lead to an abort  *>0	return value of the first signaled error>0 (in the case of no other errors)  *    0		everything OK  */
end_comment

begin_function_decl
name|int
name|fsck_walk
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|fsck_walk_func
name|walk
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|fsck_object
parameter_list|(
name|struct
name|object
modifier|*
name|obj
parameter_list|,
name|int
name|strict
parameter_list|,
name|fsck_error
name|error_func
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

