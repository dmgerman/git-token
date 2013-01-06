begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PATHSPEC_H
end_ifndef

begin_define
DECL|macro|PATHSPEC_H
define|#
directive|define
name|PATHSPEC_H
end_define

begin_function_decl
specifier|extern
name|char
modifier|*
name|find_used_pathspec
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|fill_pathspec_matches
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|,
name|char
modifier|*
name|seen
parameter_list|,
name|int
name|specs
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PATHSPEC_H */
end_comment

end_unit

