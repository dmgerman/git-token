begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|COLUMN_H
end_ifndef

begin_define
DECL|macro|COLUMN_H
define|#
directive|define
name|COLUMN_H
end_define

begin_define
DECL|macro|COL_LAYOUT_MASK
define|#
directive|define
name|COL_LAYOUT_MASK
value|0x000F
end_define

begin_define
DECL|macro|COL_ENABLE_MASK
define|#
directive|define
name|COL_ENABLE_MASK
value|0x0030
end_define

begin_comment
DECL|macro|COL_ENABLE_MASK
comment|/* always, never or auto */
end_comment

begin_define
DECL|macro|COL_PARSEOPT
define|#
directive|define
name|COL_PARSEOPT
value|0x0040
end_define

begin_comment
DECL|macro|COL_PARSEOPT
comment|/* --column is given from cmdline */
end_comment

begin_define
DECL|macro|COL_DISABLED
define|#
directive|define
name|COL_DISABLED
value|0x0000
end_define

begin_comment
DECL|macro|COL_DISABLED
comment|/* must be zero */
end_comment

begin_define
DECL|macro|COL_ENABLED
define|#
directive|define
name|COL_ENABLED
value|0x0010
end_define

begin_define
DECL|macro|COL_AUTO
define|#
directive|define
name|COL_AUTO
value|0x0020
end_define

begin_define
DECL|macro|COL_LAYOUT
define|#
directive|define
name|COL_LAYOUT
parameter_list|(
name|c
parameter_list|)
value|((c)& COL_LAYOUT_MASK)
end_define

begin_define
DECL|macro|COL_COLUMN
define|#
directive|define
name|COL_COLUMN
value|0
end_define

begin_comment
DECL|macro|COL_COLUMN
comment|/* Fill columns before rows */
end_comment

begin_define
DECL|macro|COL_ROW
define|#
directive|define
name|COL_ROW
value|1
end_define

begin_comment
DECL|macro|COL_ROW
comment|/* Fill rows before columns */
end_comment

begin_define
DECL|macro|COL_PLAIN
define|#
directive|define
name|COL_PLAIN
value|15
end_define

begin_comment
DECL|macro|COL_PLAIN
comment|/* one column */
end_comment

begin_define
DECL|macro|explicitly_enable_column
define|#
directive|define
name|explicitly_enable_column
parameter_list|(
name|c
parameter_list|)
define|\
value|(((c)& COL_PARSEOPT)&& column_active(c))
end_define

begin_struct
DECL|struct|column_options
struct|struct
name|column_options
block|{
DECL|member|width
name|int
name|width
decl_stmt|;
DECL|member|padding
name|int
name|padding
decl_stmt|;
DECL|member|indent
specifier|const
name|char
modifier|*
name|indent
decl_stmt|;
DECL|member|nl
specifier|const
name|char
modifier|*
name|nl
decl_stmt|;
block|}
struct|;
end_struct

begin_struct_decl
struct_decl|struct
name|option
struct_decl|;
end_struct_decl

begin_function_decl
specifier|extern
name|int
name|parseopt_column_callback
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|git_column_config
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
specifier|const
name|char
modifier|*
name|command
parameter_list|,
name|unsigned
name|int
modifier|*
name|colopts
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|finalize_colopts
parameter_list|(
name|unsigned
name|int
modifier|*
name|colopts
parameter_list|,
name|int
name|stdout_is_tty
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|column_active
specifier|static
specifier|inline
name|int
name|column_active
parameter_list|(
name|unsigned
name|int
name|colopts
parameter_list|)
block|{
return|return
operator|(
name|colopts
operator|&
name|COL_ENABLE_MASK
operator|)
operator|==
name|COL_ENABLED
return|;
block|}
end_function

begin_function_decl
specifier|extern
name|void
name|print_columns
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
name|unsigned
name|int
name|colopts
parameter_list|,
specifier|const
name|struct
name|column_options
modifier|*
name|opts
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

