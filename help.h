begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|HELP_H
end_ifndef

begin_define
DECL|macro|HELP_H
define|#
directive|define
name|HELP_H
end_define

begin_struct
DECL|struct|cmdnames
struct|struct
name|cmdnames
block|{
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|cnt
name|int
name|cnt
decl_stmt|;
DECL|struct|cmdname
struct|struct
name|cmdname
block|{
DECL|member|len
name|size_t
name|len
decl_stmt|;
comment|/* also used for similarity index in help.c */
DECL|member|name
name|char
name|name
index|[
name|FLEX_ARRAY
index|]
decl_stmt|;
DECL|member|names
block|}
modifier|*
modifier|*
name|names
struct|;
block|}
struct|;
end_struct

begin_function
DECL|function|mput_char
specifier|static
specifier|inline
name|void
name|mput_char
parameter_list|(
name|char
name|c
parameter_list|,
name|unsigned
name|int
name|num
parameter_list|)
block|{
while|while
condition|(
name|num
operator|--
condition|)
name|putchar
argument_list|(
name|c
argument_list|)
expr_stmt|;
block|}
end_function

begin_function_decl
specifier|extern
name|void
name|list_common_cmds_help
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|help_unknown_cmd
parameter_list|(
specifier|const
name|char
modifier|*
name|cmd
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|load_command_list
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|struct
name|cmdnames
modifier|*
name|main_cmds
parameter_list|,
name|struct
name|cmdnames
modifier|*
name|other_cmds
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|add_cmdname
parameter_list|(
name|struct
name|cmdnames
modifier|*
name|cmds
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Here we require that excludes is a sorted list. */
end_comment

begin_function_decl
specifier|extern
name|void
name|exclude_cmds
parameter_list|(
name|struct
name|cmdnames
modifier|*
name|cmds
parameter_list|,
name|struct
name|cmdnames
modifier|*
name|excludes
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|is_in_cmdlist
parameter_list|(
name|struct
name|cmdnames
modifier|*
name|cmds
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|list_commands
parameter_list|(
name|struct
name|cmdnames
modifier|*
name|main_cmds
parameter_list|,
name|struct
name|cmdnames
modifier|*
name|other_cmds
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* HELP_H */
end_comment

end_unit

