begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|ARGV_ARRAY_H
end_ifndef

begin_define
DECL|macro|ARGV_ARRAY_H
define|#
directive|define
name|ARGV_ARRAY_H
end_define

begin_decl_stmt
specifier|extern
specifier|const
name|char
modifier|*
name|empty_argv
index|[]
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|argv_array
struct|struct
name|argv_array
block|{
DECL|member|argv
specifier|const
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
DECL|member|argc
name|int
name|argc
decl_stmt|;
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|ARGV_ARRAY_INIT
define|#
directive|define
name|ARGV_ARRAY_INIT
value|{ empty_argv, 0, 0 }
end_define

begin_function_decl
name|void
name|argv_array_init
parameter_list|(
name|struct
name|argv_array
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|argv_array_push
parameter_list|(
name|struct
name|argv_array
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|2
argument|,
literal|3
argument|))
argument_list|)
end_macro

begin_function_decl
name|void
name|argv_array_pushf
parameter_list|(
name|struct
name|argv_array
modifier|*
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

begin_function_decl
name|void
name|argv_array_pushl
parameter_list|(
name|struct
name|argv_array
modifier|*
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|argv_array_pop
parameter_list|(
name|struct
name|argv_array
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|argv_array_clear
parameter_list|(
name|struct
name|argv_array
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|const
name|char
modifier|*
modifier|*
name|argv_array_detach
parameter_list|(
name|struct
name|argv_array
modifier|*
name|array
parameter_list|,
name|int
modifier|*
name|argc
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|argv_array_free_detached
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* ARGV_ARRAY_H */
end_comment

end_unit

