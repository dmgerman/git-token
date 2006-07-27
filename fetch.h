begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PULL_H
end_ifndef

begin_define
DECL|macro|PULL_H
define|#
directive|define
name|PULL_H
end_define

begin_comment
comment|/*  * Fetch object given SHA1 from the remote, and store it locally under  * GIT_OBJECT_DIRECTORY.  Return 0 on success, -1 on failure.  To be  * provided by the particular implementation.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|fetch
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Fetch the specified object and store it locally; fetch() will be  * called later to determine success. To be provided by the particular  * implementation.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|prefetch
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Fetch ref (relative to $GIT_DIR/refs) from the remote, and store  * the 20-byte SHA1 in sha1.  Return 0 on success, -1 on failure.  To  * be provided by the particular implementation.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|fetch_ref
parameter_list|(
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
comment|/* Set to fetch the target tree. */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|get_tree
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Set to fetch the commit history. */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|get_history
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Set to fetch the trees in the commit history. */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|get_all
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Set to be verbose */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|get_verbosely
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Set to check on all reachable objects. */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|get_recover
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Report what we got under get_verbosely */
end_comment

begin_function_decl
specifier|extern
name|void
name|pull_say
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* If write_ref is set, the ref filename to write the target value to. */
end_comment

begin_comment
comment|/* If write_ref_log_details is set, additional text will appear in the ref log. */
end_comment

begin_function_decl
specifier|extern
name|int
name|pull
parameter_list|(
name|int
name|targets
parameter_list|,
name|char
modifier|*
modifier|*
name|target
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|write_ref
parameter_list|,
specifier|const
name|char
modifier|*
name|write_ref_log_details
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PULL_H */
end_comment

end_unit

