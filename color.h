begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|COLOR_H
end_ifndef

begin_define
DECL|macro|COLOR_H
define|#
directive|define
name|COLOR_H
end_define

begin_struct_decl
struct_decl|struct
name|strbuf
struct_decl|;
end_struct_decl

begin_comment
comment|/*  2 + (2 * num_attrs) + 8 + 1 + 8 + 'm' + NUL */
end_comment

begin_comment
comment|/* "\033[1;2;4;5;7;38;5;2xx;48;5;2xxm\0" */
end_comment

begin_comment
comment|/*  * The maximum length of ANSI color sequence we would generate:  * - leading ESC '['            2  * - attr + ';'                 3 * 10 (e.g. "1;")  * - fg color + ';'             17 (e.g. "38;2;255;255;255;")  * - bg color + ';'             17 (e.g. "48;2;255;255;255;")  * - terminating 'm' NUL        2  *  * The above overcounts attr (we only use 5 not 8) and one semicolon  * but it is close enough.  */
end_comment

begin_define
DECL|macro|COLOR_MAXLEN
define|#
directive|define
name|COLOR_MAXLEN
value|70
end_define

begin_comment
comment|/*  * IMPORTANT: Due to the way these color codes are emulated on Windows,  * write them only using printf(), fprintf(), and fputs(). In particular,  * do not use puts() or write().  */
end_comment

begin_define
DECL|macro|GIT_COLOR_NORMAL
define|#
directive|define
name|GIT_COLOR_NORMAL
value|""
end_define

begin_define
DECL|macro|GIT_COLOR_RESET
define|#
directive|define
name|GIT_COLOR_RESET
value|"\033[m"
end_define

begin_define
DECL|macro|GIT_COLOR_BOLD
define|#
directive|define
name|GIT_COLOR_BOLD
value|"\033[1m"
end_define

begin_define
DECL|macro|GIT_COLOR_RED
define|#
directive|define
name|GIT_COLOR_RED
value|"\033[31m"
end_define

begin_define
DECL|macro|GIT_COLOR_GREEN
define|#
directive|define
name|GIT_COLOR_GREEN
value|"\033[32m"
end_define

begin_define
DECL|macro|GIT_COLOR_YELLOW
define|#
directive|define
name|GIT_COLOR_YELLOW
value|"\033[33m"
end_define

begin_define
DECL|macro|GIT_COLOR_BLUE
define|#
directive|define
name|GIT_COLOR_BLUE
value|"\033[34m"
end_define

begin_define
DECL|macro|GIT_COLOR_MAGENTA
define|#
directive|define
name|GIT_COLOR_MAGENTA
value|"\033[35m"
end_define

begin_define
DECL|macro|GIT_COLOR_CYAN
define|#
directive|define
name|GIT_COLOR_CYAN
value|"\033[36m"
end_define

begin_define
DECL|macro|GIT_COLOR_BOLD_RED
define|#
directive|define
name|GIT_COLOR_BOLD_RED
value|"\033[1;31m"
end_define

begin_define
DECL|macro|GIT_COLOR_BOLD_GREEN
define|#
directive|define
name|GIT_COLOR_BOLD_GREEN
value|"\033[1;32m"
end_define

begin_define
DECL|macro|GIT_COLOR_BOLD_YELLOW
define|#
directive|define
name|GIT_COLOR_BOLD_YELLOW
value|"\033[1;33m"
end_define

begin_define
DECL|macro|GIT_COLOR_BOLD_BLUE
define|#
directive|define
name|GIT_COLOR_BOLD_BLUE
value|"\033[1;34m"
end_define

begin_define
DECL|macro|GIT_COLOR_BOLD_MAGENTA
define|#
directive|define
name|GIT_COLOR_BOLD_MAGENTA
value|"\033[1;35m"
end_define

begin_define
DECL|macro|GIT_COLOR_BOLD_CYAN
define|#
directive|define
name|GIT_COLOR_BOLD_CYAN
value|"\033[1;36m"
end_define

begin_define
DECL|macro|GIT_COLOR_BG_RED
define|#
directive|define
name|GIT_COLOR_BG_RED
value|"\033[41m"
end_define

begin_define
DECL|macro|GIT_COLOR_BG_GREEN
define|#
directive|define
name|GIT_COLOR_BG_GREEN
value|"\033[42m"
end_define

begin_define
DECL|macro|GIT_COLOR_BG_YELLOW
define|#
directive|define
name|GIT_COLOR_BG_YELLOW
value|"\033[43m"
end_define

begin_define
DECL|macro|GIT_COLOR_BG_BLUE
define|#
directive|define
name|GIT_COLOR_BG_BLUE
value|"\033[44m"
end_define

begin_define
DECL|macro|GIT_COLOR_BG_MAGENTA
define|#
directive|define
name|GIT_COLOR_BG_MAGENTA
value|"\033[45m"
end_define

begin_define
DECL|macro|GIT_COLOR_BG_CYAN
define|#
directive|define
name|GIT_COLOR_BG_CYAN
value|"\033[46m"
end_define

begin_comment
comment|/* A special value meaning "no color selected" */
end_comment

begin_define
DECL|macro|GIT_COLOR_NIL
define|#
directive|define
name|GIT_COLOR_NIL
value|"NIL"
end_define

begin_comment
comment|/*  * The first three are chosen to match common usage in the code, and what is  * returned from git_config_colorbool. The "auto" value can be returned from  * config_colorbool, and will be converted by want_color() into either 0 or 1.  */
end_comment

begin_define
DECL|macro|GIT_COLOR_UNKNOWN
define|#
directive|define
name|GIT_COLOR_UNKNOWN
value|-1
end_define

begin_define
DECL|macro|GIT_COLOR_NEVER
define|#
directive|define
name|GIT_COLOR_NEVER
value|0
end_define

begin_define
DECL|macro|GIT_COLOR_ALWAYS
define|#
directive|define
name|GIT_COLOR_ALWAYS
value|1
end_define

begin_define
DECL|macro|GIT_COLOR_AUTO
define|#
directive|define
name|GIT_COLOR_AUTO
value|2
end_define

begin_comment
comment|/* A default list of colors to use for commit graphs and show-branch output */
end_comment

begin_decl_stmt
specifier|extern
specifier|const
name|char
modifier|*
name|column_colors_ansi
index|[]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
specifier|const
name|int
name|column_colors_ansi_max
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Generally the color code will lazily figure this out itself, but  * this provides a mechanism for callers to override autodetection.  */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|color_stdout_is_tty
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Use the first one if you need only color config; the second is a convenience  * if you are just going to change to git_default_config, too.  */
end_comment

begin_function_decl
name|int
name|git_color_config
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
name|int
name|git_color_default_config
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

begin_comment
comment|/*  * Set the color buffer (which must be COLOR_MAXLEN bytes)  * to the raw color bytes; this is useful for initializing  * default color variables.  */
end_comment

begin_function_decl
name|void
name|color_set
parameter_list|(
name|char
modifier|*
name|dst
parameter_list|,
specifier|const
name|char
modifier|*
name|color_bytes
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|git_config_colorbool
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
name|int
name|want_color
parameter_list|(
name|int
name|var
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|color_parse
parameter_list|(
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|char
modifier|*
name|dst
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|color_parse_mem
parameter_list|(
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|int
name|len
parameter_list|,
name|char
modifier|*
name|dst
parameter_list|)
function_decl|;
end_function_decl

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|3
argument|,
literal|4
argument|))
argument_list|)
end_macro

begin_function_decl
name|int
name|color_fprintf
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|color
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

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|3
argument|,
literal|4
argument|))
argument_list|)
end_macro

begin_function_decl
name|int
name|color_fprintf_ln
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|color
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
name|color_print_strbuf
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|color
parameter_list|,
specifier|const
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|color_is_nil
parameter_list|(
specifier|const
name|char
modifier|*
name|color
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* COLOR_H */
end_comment

end_unit

