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
comment|/** To be provided by the particular implementation. **/
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
comment|/** Set to fetch the target tree. */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|get_tree
decl_stmt|;
end_decl_stmt

begin_comment
comment|/** Set to fetch the commit history. */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|get_history
decl_stmt|;
end_decl_stmt

begin_comment
comment|/** Set to fetch the trees in the commit history. **/
end_comment

begin_decl_stmt
specifier|extern
name|int
name|get_all
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Set to zero to skip the check for delta object base. */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|get_delta
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

begin_function_decl
specifier|extern
name|int
name|pull
parameter_list|(
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
comment|/* PULL_H */
end_comment

end_unit

