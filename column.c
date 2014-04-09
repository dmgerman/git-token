begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"column.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"utf8.h"
end_include

begin_define
DECL|macro|XY2LINEAR
define|#
directive|define
name|XY2LINEAR
parameter_list|(
name|d
parameter_list|,
name|x
parameter_list|,
name|y
parameter_list|)
value|(COL_LAYOUT((d)->colopts) == COL_COLUMN ? \ 			    (x) * (d)->rows + (y) : \ 			    (y) * (d)->cols + (x))
end_define

begin_struct
DECL|struct|column_data
struct|struct
name|column_data
block|{
DECL|member|list
specifier|const
name|struct
name|string_list
modifier|*
name|list
decl_stmt|;
DECL|member|colopts
name|unsigned
name|int
name|colopts
decl_stmt|;
DECL|member|opts
name|struct
name|column_options
name|opts
decl_stmt|;
DECL|member|rows
DECL|member|cols
name|int
name|rows
decl_stmt|,
name|cols
decl_stmt|;
DECL|member|len
name|int
modifier|*
name|len
decl_stmt|;
comment|/* cell length */
DECL|member|width
name|int
modifier|*
name|width
decl_stmt|;
comment|/* index to the longest row in column */
block|}
struct|;
end_struct

begin_comment
comment|/* return length of 's' in letters, ANSI escapes stripped */
end_comment

begin_function
DECL|function|item_length
specifier|static
name|int
name|item_length
parameter_list|(
name|unsigned
name|int
name|colopts
parameter_list|,
specifier|const
name|char
modifier|*
name|s
parameter_list|)
block|{
name|int
name|len
decl_stmt|,
name|i
init|=
literal|0
decl_stmt|;
name|struct
name|strbuf
name|str
init|=
name|STRBUF_INIT
decl_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|str
argument_list|,
name|s
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|s
operator|=
name|strstr
argument_list|(
name|str
operator|.
name|buf
operator|+
name|i
argument_list|,
literal|"\033["
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|int
name|len
init|=
name|strspn
argument_list|(
name|s
operator|+
literal|2
argument_list|,
literal|"0123456789;"
argument_list|)
decl_stmt|;
name|i
operator|=
name|s
operator|-
name|str
operator|.
name|buf
expr_stmt|;
name|strbuf_remove
argument_list|(
operator|&
name|str
argument_list|,
name|i
argument_list|,
name|len
operator|+
literal|3
argument_list|)
expr_stmt|;
comment|/* \033[<len><func char> */
block|}
name|len
operator|=
name|utf8_strwidth
argument_list|(
name|str
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|str
argument_list|)
expr_stmt|;
return|return
name|len
return|;
block|}
end_function

begin_comment
comment|/*  * Calculate cell width, rows and cols for a table of equal cells, given  * table width and how many spaces between cells.  */
end_comment

begin_function
DECL|function|layout
specifier|static
name|void
name|layout
parameter_list|(
name|struct
name|column_data
modifier|*
name|data
parameter_list|,
name|int
modifier|*
name|width
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
operator|*
name|width
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|data
operator|->
name|list
operator|->
name|nr
condition|;
name|i
operator|++
control|)
if|if
condition|(
operator|*
name|width
operator|<
name|data
operator|->
name|len
index|[
name|i
index|]
condition|)
operator|*
name|width
operator|=
name|data
operator|->
name|len
index|[
name|i
index|]
expr_stmt|;
operator|*
name|width
operator|+=
name|data
operator|->
name|opts
operator|.
name|padding
expr_stmt|;
name|data
operator|->
name|cols
operator|=
operator|(
name|data
operator|->
name|opts
operator|.
name|width
operator|-
name|strlen
argument_list|(
name|data
operator|->
name|opts
operator|.
name|indent
argument_list|)
operator|)
operator|/
operator|*
name|width
expr_stmt|;
if|if
condition|(
name|data
operator|->
name|cols
operator|==
literal|0
condition|)
name|data
operator|->
name|cols
operator|=
literal|1
expr_stmt|;
name|data
operator|->
name|rows
operator|=
name|DIV_ROUND_UP
argument_list|(
name|data
operator|->
name|list
operator|->
name|nr
argument_list|,
name|data
operator|->
name|cols
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|compute_column_width
specifier|static
name|void
name|compute_column_width
parameter_list|(
name|struct
name|column_data
modifier|*
name|data
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|x
decl_stmt|,
name|y
decl_stmt|;
for|for
control|(
name|x
operator|=
literal|0
init|;
name|x
operator|<
name|data
operator|->
name|cols
condition|;
name|x
operator|++
control|)
block|{
name|data
operator|->
name|width
index|[
name|x
index|]
operator|=
name|XY2LINEAR
argument_list|(
name|data
argument_list|,
name|x
argument_list|,
literal|0
argument_list|)
expr_stmt|;
for|for
control|(
name|y
operator|=
literal|0
init|;
name|y
operator|<
name|data
operator|->
name|rows
condition|;
name|y
operator|++
control|)
block|{
name|i
operator|=
name|XY2LINEAR
argument_list|(
name|data
argument_list|,
name|x
argument_list|,
name|y
argument_list|)
expr_stmt|;
if|if
condition|(
name|i
operator|<
name|data
operator|->
name|list
operator|->
name|nr
operator|&&
name|data
operator|->
name|len
index|[
name|data
operator|->
name|width
index|[
name|x
index|]
index|]
operator|<
name|data
operator|->
name|len
index|[
name|i
index|]
condition|)
name|data
operator|->
name|width
index|[
name|x
index|]
operator|=
name|i
expr_stmt|;
block|}
block|}
block|}
end_function

begin_comment
comment|/*  * Shrink all columns by shortening them one row each time (and adding  * more columns along the way). Hopefully the longest cell will be  * moved to the next column, column is shrunk so we have more space  * for new columns. The process ends when the whole thing no longer  * fits in data->total_width.  */
end_comment

begin_function
DECL|function|shrink_columns
specifier|static
name|void
name|shrink_columns
parameter_list|(
name|struct
name|column_data
modifier|*
name|data
parameter_list|)
block|{
name|data
operator|->
name|width
operator|=
name|xrealloc
argument_list|(
name|data
operator|->
name|width
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|data
operator|->
name|width
argument_list|)
operator|*
name|data
operator|->
name|cols
argument_list|)
expr_stmt|;
while|while
condition|(
name|data
operator|->
name|rows
operator|>
literal|1
condition|)
block|{
name|int
name|x
decl_stmt|,
name|total_width
decl_stmt|,
name|cols
decl_stmt|,
name|rows
decl_stmt|;
name|rows
operator|=
name|data
operator|->
name|rows
expr_stmt|;
name|cols
operator|=
name|data
operator|->
name|cols
expr_stmt|;
name|data
operator|->
name|rows
operator|--
expr_stmt|;
name|data
operator|->
name|cols
operator|=
name|DIV_ROUND_UP
argument_list|(
name|data
operator|->
name|list
operator|->
name|nr
argument_list|,
name|data
operator|->
name|rows
argument_list|)
expr_stmt|;
if|if
condition|(
name|data
operator|->
name|cols
operator|!=
name|cols
condition|)
name|data
operator|->
name|width
operator|=
name|xrealloc
argument_list|(
name|data
operator|->
name|width
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|data
operator|->
name|width
argument_list|)
operator|*
name|data
operator|->
name|cols
argument_list|)
expr_stmt|;
name|compute_column_width
argument_list|(
name|data
argument_list|)
expr_stmt|;
name|total_width
operator|=
name|strlen
argument_list|(
name|data
operator|->
name|opts
operator|.
name|indent
argument_list|)
expr_stmt|;
for|for
control|(
name|x
operator|=
literal|0
init|;
name|x
operator|<
name|data
operator|->
name|cols
condition|;
name|x
operator|++
control|)
block|{
name|total_width
operator|+=
name|data
operator|->
name|len
index|[
name|data
operator|->
name|width
index|[
name|x
index|]
index|]
expr_stmt|;
name|total_width
operator|+=
name|data
operator|->
name|opts
operator|.
name|padding
expr_stmt|;
block|}
if|if
condition|(
name|total_width
operator|>
name|data
operator|->
name|opts
operator|.
name|width
condition|)
block|{
name|data
operator|->
name|rows
operator|=
name|rows
expr_stmt|;
name|data
operator|->
name|cols
operator|=
name|cols
expr_stmt|;
break|break;
block|}
block|}
name|compute_column_width
argument_list|(
name|data
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Display without layout when not enabled */
end_comment

begin_function
DECL|function|display_plain
specifier|static
name|void
name|display_plain
parameter_list|(
specifier|const
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|indent
parameter_list|,
specifier|const
name|char
modifier|*
name|nl
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|list
operator|->
name|nr
condition|;
name|i
operator|++
control|)
name|printf
argument_list|(
literal|"%s%s%s"
argument_list|,
name|indent
argument_list|,
name|list
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|,
name|nl
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/* Print a cell to stdout with all necessary leading/traling space */
end_comment

begin_function
DECL|function|display_cell
specifier|static
name|int
name|display_cell
parameter_list|(
name|struct
name|column_data
modifier|*
name|data
parameter_list|,
name|int
name|initial_width
parameter_list|,
specifier|const
name|char
modifier|*
name|empty_cell
parameter_list|,
name|int
name|x
parameter_list|,
name|int
name|y
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|len
decl_stmt|,
name|newline
decl_stmt|;
name|i
operator|=
name|XY2LINEAR
argument_list|(
name|data
argument_list|,
name|x
argument_list|,
name|y
argument_list|)
expr_stmt|;
if|if
condition|(
name|i
operator|>=
name|data
operator|->
name|list
operator|->
name|nr
condition|)
return|return
operator|-
literal|1
return|;
name|len
operator|=
name|data
operator|->
name|len
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|data
operator|->
name|width
operator|&&
name|data
operator|->
name|len
index|[
name|data
operator|->
name|width
index|[
name|x
index|]
index|]
operator|<
name|initial_width
condition|)
block|{
comment|/* 		 * empty_cell has initial_width chars, if real column 		 * is narrower, increase len a bit so we fill less 		 * space. 		 */
name|len
operator|+=
name|initial_width
operator|-
name|data
operator|->
name|len
index|[
name|data
operator|->
name|width
index|[
name|x
index|]
index|]
expr_stmt|;
name|len
operator|-=
name|data
operator|->
name|opts
operator|.
name|padding
expr_stmt|;
block|}
if|if
condition|(
name|COL_LAYOUT
argument_list|(
name|data
operator|->
name|colopts
argument_list|)
operator|==
name|COL_COLUMN
condition|)
name|newline
operator|=
name|i
operator|+
name|data
operator|->
name|rows
operator|>=
name|data
operator|->
name|list
operator|->
name|nr
expr_stmt|;
else|else
name|newline
operator|=
name|x
operator|==
name|data
operator|->
name|cols
operator|-
literal|1
operator|||
name|i
operator|==
name|data
operator|->
name|list
operator|->
name|nr
operator|-
literal|1
expr_stmt|;
name|printf
argument_list|(
literal|"%s%s%s"
argument_list|,
name|x
operator|==
literal|0
condition|?
name|data
operator|->
name|opts
operator|.
name|indent
else|:
literal|""
argument_list|,
name|data
operator|->
name|list
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|,
name|newline
condition|?
name|data
operator|->
name|opts
operator|.
name|nl
else|:
name|empty_cell
operator|+
name|len
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/* Display COL_COLUMN or COL_ROW */
end_comment

begin_function
DECL|function|display_table
specifier|static
name|void
name|display_table
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
block|{
name|struct
name|column_data
name|data
decl_stmt|;
name|int
name|x
decl_stmt|,
name|y
decl_stmt|,
name|i
decl_stmt|,
name|initial_width
decl_stmt|;
name|char
modifier|*
name|empty_cell
decl_stmt|;
name|memset
argument_list|(
operator|&
name|data
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|data
argument_list|)
argument_list|)
expr_stmt|;
name|data
operator|.
name|list
operator|=
name|list
expr_stmt|;
name|data
operator|.
name|colopts
operator|=
name|colopts
expr_stmt|;
name|data
operator|.
name|opts
operator|=
operator|*
name|opts
expr_stmt|;
name|data
operator|.
name|len
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|data
operator|.
name|len
argument_list|)
operator|*
name|list
operator|->
name|nr
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|list
operator|->
name|nr
condition|;
name|i
operator|++
control|)
name|data
operator|.
name|len
index|[
name|i
index|]
operator|=
name|item_length
argument_list|(
name|colopts
argument_list|,
name|list
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|)
expr_stmt|;
name|layout
argument_list|(
operator|&
name|data
argument_list|,
operator|&
name|initial_width
argument_list|)
expr_stmt|;
if|if
condition|(
name|colopts
operator|&
name|COL_DENSE
condition|)
name|shrink_columns
argument_list|(
operator|&
name|data
argument_list|)
expr_stmt|;
name|empty_cell
operator|=
name|xmalloc
argument_list|(
name|initial_width
operator|+
literal|1
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|empty_cell
argument_list|,
literal|' '
argument_list|,
name|initial_width
argument_list|)
expr_stmt|;
name|empty_cell
index|[
name|initial_width
index|]
operator|=
literal|'\0'
expr_stmt|;
for|for
control|(
name|y
operator|=
literal|0
init|;
name|y
operator|<
name|data
operator|.
name|rows
condition|;
name|y
operator|++
control|)
block|{
for|for
control|(
name|x
operator|=
literal|0
init|;
name|x
operator|<
name|data
operator|.
name|cols
condition|;
name|x
operator|++
control|)
if|if
condition|(
name|display_cell
argument_list|(
operator|&
name|data
argument_list|,
name|initial_width
argument_list|,
name|empty_cell
argument_list|,
name|x
argument_list|,
name|y
argument_list|)
condition|)
break|break;
block|}
name|free
argument_list|(
name|data
operator|.
name|len
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|data
operator|.
name|width
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|empty_cell
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|print_columns
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
block|{
name|struct
name|column_options
name|nopts
decl_stmt|;
if|if
condition|(
operator|!
name|list
operator|->
name|nr
condition|)
return|return;
name|assert
argument_list|(
operator|(
name|colopts
operator|&
name|COL_ENABLE_MASK
operator|)
operator|!=
name|COL_AUTO
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|nopts
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|nopts
argument_list|)
argument_list|)
expr_stmt|;
name|nopts
operator|.
name|indent
operator|=
name|opts
operator|&&
name|opts
operator|->
name|indent
condition|?
name|opts
operator|->
name|indent
else|:
literal|""
expr_stmt|;
name|nopts
operator|.
name|nl
operator|=
name|opts
operator|&&
name|opts
operator|->
name|nl
condition|?
name|opts
operator|->
name|nl
else|:
literal|"\n"
expr_stmt|;
name|nopts
operator|.
name|padding
operator|=
name|opts
condition|?
name|opts
operator|->
name|padding
else|:
literal|1
expr_stmt|;
name|nopts
operator|.
name|width
operator|=
name|opts
operator|&&
name|opts
operator|->
name|width
condition|?
name|opts
operator|->
name|width
else|:
name|term_columns
argument_list|()
operator|-
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|column_active
argument_list|(
name|colopts
argument_list|)
condition|)
block|{
name|display_plain
argument_list|(
name|list
argument_list|,
literal|""
argument_list|,
literal|"\n"
argument_list|)
expr_stmt|;
return|return;
block|}
switch|switch
condition|(
name|COL_LAYOUT
argument_list|(
name|colopts
argument_list|)
condition|)
block|{
case|case
name|COL_PLAIN
case|:
name|display_plain
argument_list|(
name|list
argument_list|,
name|nopts
operator|.
name|indent
argument_list|,
name|nopts
operator|.
name|nl
argument_list|)
expr_stmt|;
break|break;
case|case
name|COL_ROW
case|:
case|case
name|COL_COLUMN
case|:
name|display_table
argument_list|(
name|list
argument_list|,
name|colopts
argument_list|,
operator|&
name|nopts
argument_list|)
expr_stmt|;
break|break;
default|default:
name|die
argument_list|(
literal|"BUG: invalid layout mode %d"
argument_list|,
name|COL_LAYOUT
argument_list|(
name|colopts
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|finalize_colopts
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
block|{
if|if
condition|(
operator|(
operator|*
name|colopts
operator|&
name|COL_ENABLE_MASK
operator|)
operator|==
name|COL_AUTO
condition|)
block|{
if|if
condition|(
name|stdout_is_tty
operator|<
literal|0
condition|)
name|stdout_is_tty
operator|=
name|isatty
argument_list|(
literal|1
argument_list|)
expr_stmt|;
operator|*
name|colopts
operator|&=
operator|~
name|COL_ENABLE_MASK
expr_stmt|;
if|if
condition|(
name|stdout_is_tty
condition|)
operator|*
name|colopts
operator||=
name|COL_ENABLED
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_struct
DECL|struct|colopt
struct|struct
name|colopt
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|value
name|unsigned
name|int
name|value
decl_stmt|;
DECL|member|mask
name|unsigned
name|int
name|mask
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|LAYOUT_SET
define|#
directive|define
name|LAYOUT_SET
value|1
end_define

begin_define
DECL|macro|ENABLE_SET
define|#
directive|define
name|ENABLE_SET
value|2
end_define

begin_function
DECL|function|parse_option
specifier|static
name|int
name|parse_option
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|len
parameter_list|,
name|unsigned
name|int
modifier|*
name|colopts
parameter_list|,
name|int
modifier|*
name|group_set
parameter_list|)
block|{
name|struct
name|colopt
name|opts
index|[]
init|=
block|{
block|{
literal|"always"
block|,
name|COL_ENABLED
block|,
name|COL_ENABLE_MASK
block|}
block|,
block|{
literal|"never"
block|,
name|COL_DISABLED
block|,
name|COL_ENABLE_MASK
block|}
block|,
block|{
literal|"auto"
block|,
name|COL_AUTO
block|,
name|COL_ENABLE_MASK
block|}
block|,
block|{
literal|"plain"
block|,
name|COL_PLAIN
block|,
name|COL_LAYOUT_MASK
block|}
block|,
block|{
literal|"column"
block|,
name|COL_COLUMN
block|,
name|COL_LAYOUT_MASK
block|}
block|,
block|{
literal|"row"
block|,
name|COL_ROW
block|,
name|COL_LAYOUT_MASK
block|}
block|,
block|{
literal|"dense"
block|,
name|COL_DENSE
block|,
literal|0
block|}
block|, 	}
decl_stmt|;
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ARRAY_SIZE
argument_list|(
name|opts
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
name|int
name|set
init|=
literal|1
decl_stmt|,
name|arg_len
init|=
name|len
decl_stmt|,
name|name_len
decl_stmt|;
specifier|const
name|char
modifier|*
name|arg_str
init|=
name|arg
decl_stmt|;
if|if
condition|(
operator|!
name|opts
index|[
name|i
index|]
operator|.
name|mask
condition|)
block|{
if|if
condition|(
name|arg_len
operator|>
literal|2
operator|&&
operator|!
name|strncmp
argument_list|(
name|arg_str
argument_list|,
literal|"no"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
name|arg_str
operator|+=
literal|2
expr_stmt|;
name|arg_len
operator|-=
literal|2
expr_stmt|;
name|set
operator|=
literal|0
expr_stmt|;
block|}
block|}
name|name_len
operator|=
name|strlen
argument_list|(
name|opts
index|[
name|i
index|]
operator|.
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|arg_len
operator|!=
name|name_len
operator|||
name|strncmp
argument_list|(
name|arg_str
argument_list|,
name|opts
index|[
name|i
index|]
operator|.
name|name
argument_list|,
name|name_len
argument_list|)
condition|)
continue|continue;
switch|switch
condition|(
name|opts
index|[
name|i
index|]
operator|.
name|mask
condition|)
block|{
case|case
name|COL_ENABLE_MASK
case|:
operator|*
name|group_set
operator||=
name|ENABLE_SET
expr_stmt|;
break|break;
case|case
name|COL_LAYOUT_MASK
case|:
operator|*
name|group_set
operator||=
name|LAYOUT_SET
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|opts
index|[
name|i
index|]
operator|.
name|mask
condition|)
operator|*
name|colopts
operator|=
operator|(
operator|*
name|colopts
operator|&
operator|~
name|opts
index|[
name|i
index|]
operator|.
name|mask
operator|)
operator||
name|opts
index|[
name|i
index|]
operator|.
name|value
expr_stmt|;
else|else
block|{
if|if
condition|(
name|set
condition|)
operator|*
name|colopts
operator||=
name|opts
index|[
name|i
index|]
operator|.
name|value
expr_stmt|;
else|else
operator|*
name|colopts
operator|&=
operator|~
name|opts
index|[
name|i
index|]
operator|.
name|value
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
return|return
name|error
argument_list|(
literal|"unsupported option '%s'"
argument_list|,
name|arg
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|parse_config
specifier|static
name|int
name|parse_config
parameter_list|(
name|unsigned
name|int
modifier|*
name|colopts
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|sep
init|=
literal|" ,"
decl_stmt|;
name|int
name|group_set
init|=
literal|0
decl_stmt|;
while|while
condition|(
operator|*
name|value
condition|)
block|{
name|int
name|len
init|=
name|strcspn
argument_list|(
name|value
argument_list|,
name|sep
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
condition|)
block|{
if|if
condition|(
name|parse_option
argument_list|(
name|value
argument_list|,
name|len
argument_list|,
name|colopts
argument_list|,
operator|&
name|group_set
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|value
operator|+=
name|len
expr_stmt|;
block|}
name|value
operator|+=
name|strspn
argument_list|(
name|value
argument_list|,
name|sep
argument_list|)
expr_stmt|;
block|}
comment|/* 	 * If none of "always", "never", and "auto" is specified, then setting 	 * layout implies "always". 	 * 	 * Current value in COL_ENABLE_MASK is disregarded. This means if 	 * you set column.ui = auto and pass --column=row, then "auto" 	 * will become "always". 	 */
if|if
condition|(
operator|(
name|group_set
operator|&
name|LAYOUT_SET
operator|)
operator|&&
operator|!
operator|(
name|group_set
operator|&
name|ENABLE_SET
operator|)
condition|)
operator|*
name|colopts
operator|=
operator|(
operator|*
name|colopts
operator|&
operator|~
name|COL_ENABLE_MASK
operator|)
operator||
name|COL_ENABLED
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|column_config
specifier|static
name|int
name|column_config
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
name|key
parameter_list|,
name|unsigned
name|int
modifier|*
name|colopts
parameter_list|)
block|{
if|if
condition|(
operator|!
name|value
condition|)
return|return
name|config_error_nonbool
argument_list|(
name|var
argument_list|)
return|;
if|if
condition|(
name|parse_config
argument_list|(
name|colopts
argument_list|,
name|value
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"invalid column.%s mode %s"
argument_list|,
name|key
argument_list|,
name|value
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|git_column_config
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
block|{
specifier|const
name|char
modifier|*
name|it
init|=
name|skip_prefix
argument_list|(
name|var
argument_list|,
literal|"column."
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|it
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|it
argument_list|,
literal|"ui"
argument_list|)
condition|)
return|return
name|column_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
literal|"ui"
argument_list|,
name|colopts
argument_list|)
return|;
if|if
condition|(
name|command
operator|&&
operator|!
name|strcmp
argument_list|(
name|it
argument_list|,
name|command
argument_list|)
condition|)
return|return
name|column_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
name|it
argument_list|,
name|colopts
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|parseopt_column_callback
name|int
name|parseopt_column_callback
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|unset
parameter_list|)
block|{
name|unsigned
name|int
modifier|*
name|colopts
init|=
name|opt
operator|->
name|value
decl_stmt|;
operator|*
name|colopts
operator||=
name|COL_PARSEOPT
expr_stmt|;
operator|*
name|colopts
operator|&=
operator|~
name|COL_ENABLE_MASK
expr_stmt|;
if|if
condition|(
name|unset
condition|)
comment|/* --no-column == never */
return|return
literal|0
return|;
comment|/* --column == always unless "arg" states otherwise */
operator|*
name|colopts
operator||=
name|COL_ENABLED
expr_stmt|;
if|if
condition|(
name|arg
condition|)
return|return
name|parse_config
argument_list|(
name|colopts
argument_list|,
name|arg
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|fd_out
specifier|static
name|int
name|fd_out
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|column_process
specifier|static
name|struct
name|child_process
name|column_process
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|run_column_filter
name|int
name|run_column_filter
parameter_list|(
name|int
name|colopts
parameter_list|,
specifier|const
name|struct
name|column_options
modifier|*
name|opts
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|av
index|[
literal|10
index|]
decl_stmt|;
name|int
name|ret
decl_stmt|,
name|ac
init|=
literal|0
decl_stmt|;
name|struct
name|strbuf
name|sb_colopt
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|sb_width
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|sb_padding
init|=
name|STRBUF_INIT
decl_stmt|;
if|if
condition|(
name|fd_out
operator|!=
operator|-
literal|1
condition|)
return|return
operator|-
literal|1
return|;
name|av
index|[
name|ac
operator|++
index|]
operator|=
literal|"column"
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|sb_colopt
argument_list|,
literal|"--raw-mode=%d"
argument_list|,
name|colopts
argument_list|)
expr_stmt|;
name|av
index|[
name|ac
operator|++
index|]
operator|=
name|sb_colopt
operator|.
name|buf
expr_stmt|;
if|if
condition|(
name|opts
operator|&&
name|opts
operator|->
name|width
condition|)
block|{
name|strbuf_addf
argument_list|(
operator|&
name|sb_width
argument_list|,
literal|"--width=%d"
argument_list|,
name|opts
operator|->
name|width
argument_list|)
expr_stmt|;
name|av
index|[
name|ac
operator|++
index|]
operator|=
name|sb_width
operator|.
name|buf
expr_stmt|;
block|}
if|if
condition|(
name|opts
operator|&&
name|opts
operator|->
name|indent
condition|)
block|{
name|av
index|[
name|ac
operator|++
index|]
operator|=
literal|"--indent"
expr_stmt|;
name|av
index|[
name|ac
operator|++
index|]
operator|=
name|opts
operator|->
name|indent
expr_stmt|;
block|}
if|if
condition|(
name|opts
operator|&&
name|opts
operator|->
name|padding
condition|)
block|{
name|strbuf_addf
argument_list|(
operator|&
name|sb_padding
argument_list|,
literal|"--padding=%d"
argument_list|,
name|opts
operator|->
name|padding
argument_list|)
expr_stmt|;
name|av
index|[
name|ac
operator|++
index|]
operator|=
name|sb_padding
operator|.
name|buf
expr_stmt|;
block|}
name|av
index|[
name|ac
index|]
operator|=
name|NULL
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|column_process
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|column_process
argument_list|)
argument_list|)
expr_stmt|;
name|column_process
operator|.
name|in
operator|=
operator|-
literal|1
expr_stmt|;
name|column_process
operator|.
name|out
operator|=
name|dup
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|column_process
operator|.
name|git_cmd
operator|=
literal|1
expr_stmt|;
name|column_process
operator|.
name|argv
operator|=
name|av
expr_stmt|;
name|ret
operator|=
name|start_command
argument_list|(
operator|&
name|column_process
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|sb_colopt
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|sb_width
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|sb_padding
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
return|return
operator|-
literal|2
return|;
name|fd_out
operator|=
name|dup
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|close
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|dup2
argument_list|(
name|column_process
operator|.
name|in
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|column_process
operator|.
name|in
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|stop_column_filter
name|int
name|stop_column_filter
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
name|fd_out
operator|==
operator|-
literal|1
condition|)
return|return
operator|-
literal|1
return|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
name|close
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|finish_command
argument_list|(
operator|&
name|column_process
argument_list|)
expr_stmt|;
name|dup2
argument_list|(
name|fd_out
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd_out
argument_list|)
expr_stmt|;
name|fd_out
operator|=
operator|-
literal|1
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

