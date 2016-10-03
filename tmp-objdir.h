begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|TMP_OBJDIR_H
end_ifndef

begin_define
DECL|macro|TMP_OBJDIR_H
define|#
directive|define
name|TMP_OBJDIR_H
end_define

begin_comment
comment|/*  * This API allows you to create a temporary object directory, advertise it to  * sub-processes via GIT_OBJECT_DIRECTORY and GIT_ALTERNATE_OBJECT_DIRECTORIES,  * and then either migrate its object into the main object directory, or remove  * it. The library handles unexpected signal/exit death by cleaning up the  * temporary directory.  *  * Example:  *  *	struct tmp_objdir *t = tmp_objdir_create();  *	if (!run_command_v_opt_cd_env(cmd, 0, NULL, tmp_objdir_env(t))&&  *	    !tmp_objdir_migrate(t))  *		printf("success!\n");  *	else  *		die("failed...tmp_objdir will clean up for us");  *  */
end_comment

begin_struct_decl
struct_decl|struct
name|tmp_objdir
struct_decl|;
end_struct_decl

begin_comment
comment|/*  * Create a new temporary object directory; returns NULL on failure.  */
end_comment

begin_function_decl
name|struct
name|tmp_objdir
modifier|*
name|tmp_objdir_create
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Return a list of environment strings, suitable for use with  * child_process.env, that can be passed to child programs to make use of the  * temporary object directory.  */
end_comment

begin_function_decl
specifier|const
name|char
modifier|*
modifier|*
name|tmp_objdir_env
parameter_list|(
specifier|const
name|struct
name|tmp_objdir
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Finalize a temporary object directory by migrating its objects into the main  * object database, removing the temporary directory, and freeing any  * associated resources.  */
end_comment

begin_function_decl
name|int
name|tmp_objdir_migrate
parameter_list|(
name|struct
name|tmp_objdir
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Destroy a temporary object directory, discarding any objects it contains.  */
end_comment

begin_function_decl
name|int
name|tmp_objdir_destroy
parameter_list|(
name|struct
name|tmp_objdir
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Add the temporary object directory as an alternate object store in the  * current process.  */
end_comment

begin_function_decl
name|void
name|tmp_objdir_add_as_alternate
parameter_list|(
specifier|const
name|struct
name|tmp_objdir
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* TMP_OBJDIR_H */
end_comment

end_unit

