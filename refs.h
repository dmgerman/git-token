begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|REFS_H
end_ifndef

begin_define
DECL|macro|REFS_H
define|#
directive|define
name|REFS_H
end_define

begin_comment
comment|/*  * Calls the specified function for each ref file until it returns nonzero,  * and returns the value  */
end_comment

begin_function_decl
specifier|extern
name|int
name|for_each_ref
parameter_list|(
name|int
function_decl|(
modifier|*
name|fn
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Reads the refs file specified into sha1 **/
end_comment

begin_function_decl
specifier|extern
name|int
name|get_ref_sha1
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Locks ref and returns the fd to give to write_ref_sha1() if the ref  * has the given value currently; otherwise, returns -1.  **/
end_comment

begin_function_decl
specifier|extern
name|int
name|lock_ref_sha1
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Writes sha1 into the refs file specified, locked with the given fd. **/
end_comment

begin_function_decl
specifier|extern
name|int
name|write_ref_sha1
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
name|int
name|fd
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Writes sha1 into the refs file specified. **/
end_comment

begin_function_decl
specifier|extern
name|int
name|write_ref_sha1_unlocked
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Returns 0 if target has the right format for a ref. **/
end_comment

begin_function_decl
specifier|extern
name|int
name|check_ref_format
parameter_list|(
specifier|const
name|char
modifier|*
name|target
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* REFS_H */
end_comment

end_unit

