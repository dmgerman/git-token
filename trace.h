begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|TRACE_H
end_ifndef

begin_define
DECL|macro|TRACE_H
define|#
directive|define
name|TRACE_H
end_define

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_struct
DECL|struct|trace_key
struct|struct
name|trace_key
block|{
DECL|member|key
specifier|const
name|char
modifier|*
specifier|const
name|key
decl_stmt|;
DECL|member|fd
name|int
name|fd
decl_stmt|;
DECL|member|initialized
name|unsigned
name|int
name|initialized
range|:
literal|1
decl_stmt|;
DECL|member|need_close
name|unsigned
name|int
name|need_close
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|TRACE_KEY_INIT
define|#
directive|define
name|TRACE_KEY_INIT
parameter_list|(
name|name
parameter_list|)
value|{ "GIT_TRACE_" #name, 0, 0, 0 }
end_define

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|1
argument|,
literal|2
argument|))
argument_list|)
end_macro

begin_function_decl
specifier|extern
name|void
name|trace_printf
parameter_list|(
specifier|const
name|char
modifier|*
name|format
parameter_list|,
modifier|...
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
specifier|extern
name|void
name|trace_argv_printf
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|trace_repo_setup
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|trace_want
parameter_list|(
name|struct
name|trace_key
modifier|*
name|key
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|trace_disable
parameter_list|(
name|struct
name|trace_key
modifier|*
name|key
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
specifier|extern
name|void
name|trace_printf_key
parameter_list|(
name|struct
name|trace_key
modifier|*
name|key
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|trace_strbuf
parameter_list|(
name|struct
name|trace_key
modifier|*
name|key
parameter_list|,
specifier|const
name|struct
name|strbuf
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* TRACE_H */
end_comment

end_unit

