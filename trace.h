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

begin_function_decl
specifier|extern
name|uint64_t
name|getnanotime
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|trace_command_performance
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|trace_verbatim
parameter_list|(
name|struct
name|trace_key
modifier|*
name|key
parameter_list|,
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|unsigned
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_ifndef
ifndef|#
directive|ifndef
name|HAVE_VARIADIC_MACROS
end_ifndef

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

begin_comment
comment|/* Prints elapsed time (in nanoseconds) if GIT_TRACE_PERFORMANCE is enabled. */
end_comment

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
name|trace_performance
parameter_list|(
name|uint64_t
name|nanos
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

begin_comment
comment|/* Prints elapsed time since 'start' if GIT_TRACE_PERFORMANCE is enabled. */
end_comment

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
name|trace_performance_since
parameter_list|(
name|uint64_t
name|start
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

begin_else
else|#
directive|else
end_else

begin_comment
comment|/*  * Macros to add file:line - see above for C-style declarations of how these  * should be used.  */
end_comment

begin_comment
comment|/*  * TRACE_CONTEXT may be set to __FUNCTION__ if the compiler supports it. The  * default is __FILE__, as it is consistent with assert(), and static function  * names are not necessarily unique.  *  * __FILE__ ":" __FUNCTION__ doesn't work with GNUC, as __FILE__ is supplied  * by the preprocessor as a string literal, and __FUNCTION__ is filled in by  * the compiler as a string constant.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|TRACE_CONTEXT
end_ifndef

begin_define
DECL|macro|TRACE_CONTEXT
define|#
directive|define
name|TRACE_CONTEXT
value|__FILE__
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  * Note: with C99 variadic macros, __VA_ARGS__ must include the last fixed  * parameter ('format' in this case). Otherwise, a call without variable  * arguments will have a surplus ','. E.g.:  *  *  #define foo(format, ...) bar(format, __VA_ARGS__)  *  foo("test");  *  * will expand to  *  *  bar("test",);  *  * which is invalid (note the ',)'). With GNUC, '##__VA_ARGS__' drops the  * comma, but this is non-standard.  */
end_comment

begin_define
DECL|macro|trace_printf
define|#
directive|define
name|trace_printf
parameter_list|(
modifier|...
parameter_list|)
define|\
value|trace_printf_key_fl(TRACE_CONTEXT, __LINE__, NULL, __VA_ARGS__)
end_define

begin_define
DECL|macro|trace_printf_key
define|#
directive|define
name|trace_printf_key
parameter_list|(
name|key
parameter_list|,
modifier|...
parameter_list|)
define|\
value|trace_printf_key_fl(TRACE_CONTEXT, __LINE__, key, __VA_ARGS__)
end_define

begin_define
DECL|macro|trace_argv_printf
define|#
directive|define
name|trace_argv_printf
parameter_list|(
name|argv
parameter_list|,
modifier|...
parameter_list|)
define|\
value|trace_argv_printf_fl(TRACE_CONTEXT, __LINE__, argv, __VA_ARGS__)
end_define

begin_define
DECL|macro|trace_strbuf
define|#
directive|define
name|trace_strbuf
parameter_list|(
name|key
parameter_list|,
name|data
parameter_list|)
define|\
value|trace_strbuf_fl(TRACE_CONTEXT, __LINE__, key, data)
end_define

begin_define
DECL|macro|trace_performance
define|#
directive|define
name|trace_performance
parameter_list|(
name|nanos
parameter_list|,
modifier|...
parameter_list|)
define|\
value|trace_performance_fl(TRACE_CONTEXT, __LINE__, nanos, __VA_ARGS__)
end_define

begin_define
DECL|macro|trace_performance_since
define|#
directive|define
name|trace_performance_since
parameter_list|(
name|start
parameter_list|,
modifier|...
parameter_list|)
define|\
value|trace_performance_fl(TRACE_CONTEXT, __LINE__, getnanotime() - (start), \ 			     __VA_ARGS__)
end_define

begin_comment
comment|/* backend functions, use non-*fl macros instead */
end_comment

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|4
argument|,
literal|5
argument|))
argument_list|)
end_macro

begin_function_decl
specifier|extern
name|void
name|trace_printf_key_fl
parameter_list|(
specifier|const
name|char
modifier|*
name|file
parameter_list|,
name|int
name|line
parameter_list|,
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

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|4
argument|,
literal|5
argument|))
argument_list|)
end_macro

begin_function_decl
specifier|extern
name|void
name|trace_argv_printf_fl
parameter_list|(
specifier|const
name|char
modifier|*
name|file
parameter_list|,
name|int
name|line
parameter_list|,
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
name|trace_strbuf_fl
parameter_list|(
specifier|const
name|char
modifier|*
name|file
parameter_list|,
name|int
name|line
parameter_list|,
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

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|4
argument|,
literal|5
argument|))
argument_list|)
end_macro

begin_function_decl
specifier|extern
name|void
name|trace_performance_fl
parameter_list|(
specifier|const
name|char
modifier|*
name|file
parameter_list|,
name|int
name|line
parameter_list|,
name|uint64_t
name|nanos
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

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* HAVE_VARIADIC_MACROS */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* TRACE_H */
end_comment

end_unit

