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

begin_comment
comment|/*  2 + (2 * num_attrs) + 8 + 1 + 8 + 'm' + NUL */
end_comment

begin_comment
comment|/* "\033[1;2;4;5;7;38;5;2xx;48;5;2xxm\0" */
end_comment

begin_comment
comment|/*  * The maximum length of ANSI color sequence we would generate:  * - leading ESC '['            2  * - attr + ';'                 2 * 8 (e.g. "1;")  * - fg color + ';'             9 (e.g. "38;5;2xx;")  * - fg color + ';'             9 (e.g. "48;5;2xx;")  * - terminating 'm' NUL        2  *  * The above overcounts attr (we only use 5 not 8) and one semicolon  * but it is close enough.  */
end_comment

begin_define
DECL|macro|COLOR_MAXLEN
define|#
directive|define
name|COLOR_MAXLEN
value|40
end_define

begin_comment
comment|/*  * This variable stores the value of color.ui  */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|git_use_color_default
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Use this instead of git_default_config if you need the value of color.ui.  */
end_comment

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
parameter_list|,
name|int
name|stdout_is_tty
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|color_parse
parameter_list|(
specifier|const
name|char
modifier|*
name|value
parameter_list|,
specifier|const
name|char
modifier|*
name|var
parameter_list|,
name|char
modifier|*
name|dst
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
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
specifier|const
name|char
modifier|*
name|var
parameter_list|,
name|char
modifier|*
name|dst
parameter_list|)
function_decl|;
end_function_decl

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
name|int
name|color_fwrite_lines
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
name|size_t
name|count
parameter_list|,
specifier|const
name|char
modifier|*
name|buf
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

