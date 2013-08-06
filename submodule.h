begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SUBMODULE_H
end_ifndef

begin_define
DECL|macro|SUBMODULE_H
define|#
directive|define
name|SUBMODULE_H
end_define

begin_struct_decl
struct_decl|struct
name|diff_options
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|argv_array
struct_decl|;
end_struct_decl

begin_enum
enum|enum
block|{
DECL|enumerator|RECURSE_SUBMODULES_ON_DEMAND
name|RECURSE_SUBMODULES_ON_DEMAND
init|=
operator|-
literal|1
block|,
DECL|enumerator|RECURSE_SUBMODULES_OFF
name|RECURSE_SUBMODULES_OFF
init|=
literal|0
block|,
DECL|enumerator|RECURSE_SUBMODULES_DEFAULT
name|RECURSE_SUBMODULES_DEFAULT
init|=
literal|1
block|,
DECL|enumerator|RECURSE_SUBMODULES_ON
name|RECURSE_SUBMODULES_ON
init|=
literal|2
block|}
enum|;
end_enum

begin_function_decl
name|int
name|is_staging_gitmodules_ok
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|update_path_in_gitmodules
parameter_list|(
specifier|const
name|char
modifier|*
name|oldpath
parameter_list|,
specifier|const
name|char
modifier|*
name|newpath
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|remove_path_from_gitmodules
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|stage_updated_gitmodules
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|set_diffopt_flags_from_submodule_config
parameter_list|(
name|struct
name|diff_options
modifier|*
name|diffopt
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|submodule_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|gitmodules_config
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_submodule_config_option
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|handle_ignore_submodules_arg
parameter_list|(
name|struct
name|diff_options
modifier|*
name|diffopt
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_fetch_recurse_submodules_arg
parameter_list|(
specifier|const
name|char
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|show_submodule_summary
parameter_list|(
name|FILE
modifier|*
name|f
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|line_prefix
parameter_list|,
name|unsigned
name|char
name|one
index|[
literal|20
index|]
parameter_list|,
name|unsigned
name|char
name|two
index|[
literal|20
index|]
parameter_list|,
name|unsigned
name|dirty_submodule
parameter_list|,
specifier|const
name|char
modifier|*
name|meta
parameter_list|,
specifier|const
name|char
modifier|*
name|del
parameter_list|,
specifier|const
name|char
modifier|*
name|add
parameter_list|,
specifier|const
name|char
modifier|*
name|reset
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|set_config_fetch_recurse_submodules
parameter_list|(
name|int
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|check_for_new_submodule_commits
parameter_list|(
name|unsigned
name|char
name|new_sha1
index|[
literal|20
index|]
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|fetch_populated_submodules
parameter_list|(
specifier|const
name|struct
name|argv_array
modifier|*
name|options
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|int
name|command_line_option
parameter_list|,
name|int
name|quiet
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|unsigned
name|is_submodule_modified
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|ignore_untracked
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|submodule_uses_gitfile
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ok_to_remove_submodule
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|merge_submodule
parameter_list|(
name|unsigned
name|char
name|result
index|[
literal|20
index|]
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
name|base
index|[
literal|20
index|]
parameter_list|,
specifier|const
name|unsigned
name|char
name|a
index|[
literal|20
index|]
parameter_list|,
specifier|const
name|unsigned
name|char
name|b
index|[
literal|20
index|]
parameter_list|,
name|int
name|search
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|find_unpushed_submodules
parameter_list|(
name|unsigned
name|char
name|new_sha1
index|[
literal|20
index|]
parameter_list|,
specifier|const
name|char
modifier|*
name|remotes_name
parameter_list|,
name|struct
name|string_list
modifier|*
name|needs_pushing
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|push_unpushed_submodules
parameter_list|(
name|unsigned
name|char
name|new_sha1
index|[
literal|20
index|]
parameter_list|,
specifier|const
name|char
modifier|*
name|remotes_name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|connect_work_tree_and_git_dir
parameter_list|(
specifier|const
name|char
modifier|*
name|work_tree
parameter_list|,
specifier|const
name|char
modifier|*
name|git_dir
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

