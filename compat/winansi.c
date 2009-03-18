begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright 2008 Peter Harris<git@peter.is-a-geek.org>  */
end_comment

begin_include
include|#
directive|include
file|<windows.h>
end_include

begin_include
include|#
directive|include
file|"../git-compat-util.h"
end_include

begin_comment
comment|/*  Functions to be wrapped: */
end_comment

begin_undef
DECL|macro|printf
undef|#
directive|undef
name|printf
end_undef

begin_undef
DECL|macro|fprintf
undef|#
directive|undef
name|fprintf
end_undef

begin_undef
DECL|macro|fputs
undef|#
directive|undef
name|fputs
end_undef

begin_comment
comment|/* TODO: write */
end_comment

begin_comment
comment|/*  ANSI codes used by git: m, K   This file is git-specific. Therefore, this file does not attempt  to implement any codes that are not used by git. */
end_comment

begin_decl_stmt
DECL|variable|console
specifier|static
name|HANDLE
name|console
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|plain_attr
specifier|static
name|WORD
name|plain_attr
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|attr
specifier|static
name|WORD
name|attr
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|negative
specifier|static
name|int
name|negative
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|init
specifier|static
name|void
name|init
parameter_list|(
name|void
parameter_list|)
block|{
name|CONSOLE_SCREEN_BUFFER_INFO
name|sbi
decl_stmt|;
specifier|static
name|int
name|initialized
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|initialized
condition|)
return|return;
name|console
operator|=
name|GetStdHandle
argument_list|(
name|STD_OUTPUT_HANDLE
argument_list|)
expr_stmt|;
if|if
condition|(
name|console
operator|==
name|INVALID_HANDLE_VALUE
condition|)
name|console
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
operator|!
name|console
condition|)
return|return;
name|GetConsoleScreenBufferInfo
argument_list|(
name|console
argument_list|,
operator|&
name|sbi
argument_list|)
expr_stmt|;
name|attr
operator|=
name|plain_attr
operator|=
name|sbi
operator|.
name|wAttributes
expr_stmt|;
name|negative
operator|=
literal|0
expr_stmt|;
name|initialized
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_define
DECL|macro|FOREGROUND_ALL
define|#
directive|define
name|FOREGROUND_ALL
value|(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
end_define

begin_define
DECL|macro|BACKGROUND_ALL
define|#
directive|define
name|BACKGROUND_ALL
value|(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
end_define

begin_function
DECL|function|set_console_attr
specifier|static
name|void
name|set_console_attr
parameter_list|(
name|void
parameter_list|)
block|{
name|WORD
name|attributes
init|=
name|attr
decl_stmt|;
if|if
condition|(
name|negative
condition|)
block|{
name|attributes
operator|&=
operator|~
name|FOREGROUND_ALL
expr_stmt|;
name|attributes
operator|&=
operator|~
name|BACKGROUND_ALL
expr_stmt|;
comment|/* This could probably use a bitmask 		   instead of a series of ifs */
if|if
condition|(
name|attr
operator|&
name|FOREGROUND_RED
condition|)
name|attributes
operator||=
name|BACKGROUND_RED
expr_stmt|;
if|if
condition|(
name|attr
operator|&
name|FOREGROUND_GREEN
condition|)
name|attributes
operator||=
name|BACKGROUND_GREEN
expr_stmt|;
if|if
condition|(
name|attr
operator|&
name|FOREGROUND_BLUE
condition|)
name|attributes
operator||=
name|BACKGROUND_BLUE
expr_stmt|;
if|if
condition|(
name|attr
operator|&
name|BACKGROUND_RED
condition|)
name|attributes
operator||=
name|FOREGROUND_RED
expr_stmt|;
if|if
condition|(
name|attr
operator|&
name|BACKGROUND_GREEN
condition|)
name|attributes
operator||=
name|FOREGROUND_GREEN
expr_stmt|;
if|if
condition|(
name|attr
operator|&
name|BACKGROUND_BLUE
condition|)
name|attributes
operator||=
name|FOREGROUND_BLUE
expr_stmt|;
block|}
name|SetConsoleTextAttribute
argument_list|(
name|console
argument_list|,
name|attributes
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|erase_in_line
specifier|static
name|void
name|erase_in_line
parameter_list|(
name|void
parameter_list|)
block|{
name|CONSOLE_SCREEN_BUFFER_INFO
name|sbi
decl_stmt|;
if|if
condition|(
operator|!
name|console
condition|)
return|return;
name|GetConsoleScreenBufferInfo
argument_list|(
name|console
argument_list|,
operator|&
name|sbi
argument_list|)
expr_stmt|;
name|FillConsoleOutputCharacterA
argument_list|(
name|console
argument_list|,
literal|' '
argument_list|,
name|sbi
operator|.
name|dwSize
operator|.
name|X
operator|-
name|sbi
operator|.
name|dwCursorPosition
operator|.
name|X
argument_list|,
name|sbi
operator|.
name|dwCursorPosition
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|set_attr
specifier|static
specifier|const
name|char
modifier|*
name|set_attr
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|func
decl_stmt|;
name|size_t
name|len
init|=
name|strspn
argument_list|(
name|str
argument_list|,
literal|"0123456789;"
argument_list|)
decl_stmt|;
name|func
operator|=
name|str
operator|+
name|len
expr_stmt|;
switch|switch
condition|(
operator|*
name|func
condition|)
block|{
case|case
literal|'m'
case|:
do|do
block|{
name|long
name|val
init|=
name|strtol
argument_list|(
name|str
argument_list|,
operator|(
name|char
operator|*
operator|*
operator|)
operator|&
name|str
argument_list|,
literal|10
argument_list|)
decl_stmt|;
switch|switch
condition|(
name|val
condition|)
block|{
case|case
literal|0
case|:
comment|/* reset */
name|attr
operator|=
name|plain_attr
expr_stmt|;
name|negative
operator|=
literal|0
expr_stmt|;
break|break;
case|case
literal|1
case|:
comment|/* bold */
name|attr
operator||=
name|FOREGROUND_INTENSITY
expr_stmt|;
break|break;
case|case
literal|2
case|:
comment|/* faint */
case|case
literal|22
case|:
comment|/* normal */
name|attr
operator|&=
operator|~
name|FOREGROUND_INTENSITY
expr_stmt|;
break|break;
case|case
literal|3
case|:
comment|/* italic */
comment|/* Unsupported */
break|break;
case|case
literal|4
case|:
comment|/* underline */
case|case
literal|21
case|:
comment|/* double underline */
comment|/* Wikipedia says this flag does nothing */
comment|/* Furthermore, mingw doesn't define this flag 				attr |= COMMON_LVB_UNDERSCORE; */
break|break;
case|case
literal|24
case|:
comment|/* no underline */
comment|/* attr&= ~COMMON_LVB_UNDERSCORE; */
break|break;
case|case
literal|5
case|:
comment|/* slow blink */
case|case
literal|6
case|:
comment|/* fast blink */
comment|/* We don't have blink, but we do have 				   background intensity */
name|attr
operator||=
name|BACKGROUND_INTENSITY
expr_stmt|;
break|break;
case|case
literal|25
case|:
comment|/* no blink */
name|attr
operator|&=
operator|~
name|BACKGROUND_INTENSITY
expr_stmt|;
break|break;
case|case
literal|7
case|:
comment|/* negative */
name|negative
operator|=
literal|1
expr_stmt|;
break|break;
case|case
literal|27
case|:
comment|/* positive */
name|negative
operator|=
literal|0
expr_stmt|;
break|break;
case|case
literal|8
case|:
comment|/* conceal */
case|case
literal|28
case|:
comment|/* reveal */
comment|/* Unsupported */
break|break;
case|case
literal|30
case|:
comment|/* Black */
name|attr
operator|&=
operator|~
name|FOREGROUND_ALL
expr_stmt|;
break|break;
case|case
literal|31
case|:
comment|/* Red */
name|attr
operator|&=
operator|~
name|FOREGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|FOREGROUND_RED
expr_stmt|;
break|break;
case|case
literal|32
case|:
comment|/* Green */
name|attr
operator|&=
operator|~
name|FOREGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|FOREGROUND_GREEN
expr_stmt|;
break|break;
case|case
literal|33
case|:
comment|/* Yellow */
name|attr
operator|&=
operator|~
name|FOREGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|FOREGROUND_RED
operator||
name|FOREGROUND_GREEN
expr_stmt|;
break|break;
case|case
literal|34
case|:
comment|/* Blue */
name|attr
operator|&=
operator|~
name|FOREGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|FOREGROUND_BLUE
expr_stmt|;
break|break;
case|case
literal|35
case|:
comment|/* Magenta */
name|attr
operator|&=
operator|~
name|FOREGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|FOREGROUND_RED
operator||
name|FOREGROUND_BLUE
expr_stmt|;
break|break;
case|case
literal|36
case|:
comment|/* Cyan */
name|attr
operator|&=
operator|~
name|FOREGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|FOREGROUND_GREEN
operator||
name|FOREGROUND_BLUE
expr_stmt|;
break|break;
case|case
literal|37
case|:
comment|/* White */
name|attr
operator||=
name|FOREGROUND_RED
operator||
name|FOREGROUND_GREEN
operator||
name|FOREGROUND_BLUE
expr_stmt|;
break|break;
case|case
literal|38
case|:
comment|/* Unknown */
break|break;
case|case
literal|39
case|:
comment|/* reset */
name|attr
operator|&=
operator|~
name|FOREGROUND_ALL
expr_stmt|;
name|attr
operator||=
operator|(
name|plain_attr
operator|&
name|FOREGROUND_ALL
operator|)
expr_stmt|;
break|break;
case|case
literal|40
case|:
comment|/* Black */
name|attr
operator|&=
operator|~
name|BACKGROUND_ALL
expr_stmt|;
break|break;
case|case
literal|41
case|:
comment|/* Red */
name|attr
operator|&=
operator|~
name|BACKGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|BACKGROUND_RED
expr_stmt|;
break|break;
case|case
literal|42
case|:
comment|/* Green */
name|attr
operator|&=
operator|~
name|BACKGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|BACKGROUND_GREEN
expr_stmt|;
break|break;
case|case
literal|43
case|:
comment|/* Yellow */
name|attr
operator|&=
operator|~
name|BACKGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|BACKGROUND_RED
operator||
name|BACKGROUND_GREEN
expr_stmt|;
break|break;
case|case
literal|44
case|:
comment|/* Blue */
name|attr
operator|&=
operator|~
name|BACKGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|BACKGROUND_BLUE
expr_stmt|;
break|break;
case|case
literal|45
case|:
comment|/* Magenta */
name|attr
operator|&=
operator|~
name|BACKGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|BACKGROUND_RED
operator||
name|BACKGROUND_BLUE
expr_stmt|;
break|break;
case|case
literal|46
case|:
comment|/* Cyan */
name|attr
operator|&=
operator|~
name|BACKGROUND_ALL
expr_stmt|;
name|attr
operator||=
name|BACKGROUND_GREEN
operator||
name|BACKGROUND_BLUE
expr_stmt|;
break|break;
case|case
literal|47
case|:
comment|/* White */
name|attr
operator||=
name|BACKGROUND_RED
operator||
name|BACKGROUND_GREEN
operator||
name|BACKGROUND_BLUE
expr_stmt|;
break|break;
case|case
literal|48
case|:
comment|/* Unknown */
break|break;
case|case
literal|49
case|:
comment|/* reset */
name|attr
operator|&=
operator|~
name|BACKGROUND_ALL
expr_stmt|;
name|attr
operator||=
operator|(
name|plain_attr
operator|&
name|BACKGROUND_ALL
operator|)
expr_stmt|;
break|break;
default|default:
comment|/* Unsupported code */
break|break;
block|}
name|str
operator|++
expr_stmt|;
block|}
do|while
condition|(
operator|*
operator|(
name|str
operator|-
literal|1
operator|)
operator|==
literal|';'
condition|)
do|;
name|set_console_attr
argument_list|()
expr_stmt|;
break|break;
case|case
literal|'K'
case|:
name|erase_in_line
argument_list|()
expr_stmt|;
break|break;
default|default:
comment|/* Unsupported code */
break|break;
block|}
return|return
name|func
operator|+
literal|1
return|;
block|}
end_function

begin_function
DECL|function|ansi_emulate
specifier|static
name|int
name|ansi_emulate
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
name|FILE
modifier|*
name|stream
parameter_list|)
block|{
name|int
name|rv
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|pos
init|=
name|str
decl_stmt|;
while|while
condition|(
operator|*
name|pos
condition|)
block|{
name|pos
operator|=
name|strstr
argument_list|(
name|str
argument_list|,
literal|"\033["
argument_list|)
expr_stmt|;
if|if
condition|(
name|pos
condition|)
block|{
name|size_t
name|len
init|=
name|pos
operator|-
name|str
decl_stmt|;
if|if
condition|(
name|len
condition|)
block|{
name|size_t
name|out_len
init|=
name|fwrite
argument_list|(
name|str
argument_list|,
literal|1
argument_list|,
name|len
argument_list|,
name|stream
argument_list|)
decl_stmt|;
name|rv
operator|+=
name|out_len
expr_stmt|;
if|if
condition|(
name|out_len
operator|<
name|len
condition|)
return|return
name|rv
return|;
block|}
name|str
operator|=
name|pos
operator|+
literal|2
expr_stmt|;
name|rv
operator|+=
literal|2
expr_stmt|;
name|fflush
argument_list|(
name|stream
argument_list|)
expr_stmt|;
name|pos
operator|=
name|set_attr
argument_list|(
name|str
argument_list|)
expr_stmt|;
name|rv
operator|+=
name|pos
operator|-
name|str
expr_stmt|;
name|str
operator|=
name|pos
expr_stmt|;
block|}
else|else
block|{
name|rv
operator|+=
name|strlen
argument_list|(
name|str
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
name|str
argument_list|,
name|stream
argument_list|)
expr_stmt|;
return|return
name|rv
return|;
block|}
block|}
return|return
name|rv
return|;
block|}
end_function

begin_function
DECL|function|winansi_fputs
name|int
name|winansi_fputs
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
name|FILE
modifier|*
name|stream
parameter_list|)
block|{
name|int
name|rv
decl_stmt|;
if|if
condition|(
operator|!
name|isatty
argument_list|(
name|fileno
argument_list|(
name|stream
argument_list|)
argument_list|)
condition|)
return|return
name|fputs
argument_list|(
name|str
argument_list|,
name|stream
argument_list|)
return|;
name|init
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|console
condition|)
return|return
name|fputs
argument_list|(
name|str
argument_list|,
name|stream
argument_list|)
return|;
name|rv
operator|=
name|ansi_emulate
argument_list|(
name|str
argument_list|,
name|stream
argument_list|)
expr_stmt|;
if|if
condition|(
name|rv
operator|>=
literal|0
condition|)
return|return
literal|0
return|;
else|else
return|return
name|EOF
return|;
block|}
end_function

begin_function
DECL|function|winansi_vfprintf
specifier|static
name|int
name|winansi_vfprintf
parameter_list|(
name|FILE
modifier|*
name|stream
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
name|va_list
name|list
parameter_list|)
block|{
name|int
name|len
decl_stmt|,
name|rv
decl_stmt|;
name|char
name|small_buf
index|[
literal|256
index|]
decl_stmt|;
name|char
modifier|*
name|buf
init|=
name|small_buf
decl_stmt|;
name|va_list
name|cp
decl_stmt|;
if|if
condition|(
operator|!
name|isatty
argument_list|(
name|fileno
argument_list|(
name|stream
argument_list|)
argument_list|)
condition|)
goto|goto
name|abort
goto|;
name|init
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
name|console
condition|)
goto|goto
name|abort
goto|;
name|va_copy
argument_list|(
name|cp
argument_list|,
name|list
argument_list|)
expr_stmt|;
name|len
operator|=
name|vsnprintf
argument_list|(
name|small_buf
argument_list|,
sizeof|sizeof
argument_list|(
name|small_buf
argument_list|)
argument_list|,
name|format
argument_list|,
name|cp
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|cp
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>
sizeof|sizeof
argument_list|(
name|small_buf
argument_list|)
operator|-
literal|1
condition|)
block|{
name|buf
operator|=
name|malloc
argument_list|(
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
goto|goto
name|abort
goto|;
name|len
operator|=
name|vsnprintf
argument_list|(
name|buf
argument_list|,
name|len
operator|+
literal|1
argument_list|,
name|format
argument_list|,
name|list
argument_list|)
expr_stmt|;
block|}
name|rv
operator|=
name|ansi_emulate
argument_list|(
name|buf
argument_list|,
name|stream
argument_list|)
expr_stmt|;
if|if
condition|(
name|buf
operator|!=
name|small_buf
condition|)
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return
name|rv
return|;
name|abort
label|:
name|rv
operator|=
name|vfprintf
argument_list|(
name|stream
argument_list|,
name|format
argument_list|,
name|list
argument_list|)
expr_stmt|;
return|return
name|rv
return|;
block|}
end_function

begin_function
DECL|function|winansi_fprintf
name|int
name|winansi_fprintf
parameter_list|(
name|FILE
modifier|*
name|stream
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|list
decl_stmt|;
name|int
name|rv
decl_stmt|;
name|va_start
argument_list|(
name|list
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|rv
operator|=
name|winansi_vfprintf
argument_list|(
name|stream
argument_list|,
name|format
argument_list|,
name|list
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|list
argument_list|)
expr_stmt|;
return|return
name|rv
return|;
block|}
end_function

begin_function
DECL|function|winansi_printf
name|int
name|winansi_printf
parameter_list|(
specifier|const
name|char
modifier|*
name|format
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|list
decl_stmt|;
name|int
name|rv
decl_stmt|;
name|va_start
argument_list|(
name|list
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|rv
operator|=
name|winansi_vfprintf
argument_list|(
name|stdout
argument_list|,
name|format
argument_list|,
name|list
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|list
argument_list|)
expr_stmt|;
return|return
name|rv
return|;
block|}
end_function

end_unit

