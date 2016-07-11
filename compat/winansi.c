begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright 2008 Peter Harris<git@peter.is-a-geek.org>  */
end_comment

begin_undef
DECL|macro|NOGDI
undef|#
directive|undef
name|NOGDI
end_undef

begin_include
include|#
directive|include
file|"../git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|<wingdi.h>
end_include

begin_include
include|#
directive|include
file|<winreg.h>
end_include

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

begin_decl_stmt
DECL|variable|non_ascii_used
specifier|static
name|int
name|non_ascii_used
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|hthread
DECL|variable|hread
DECL|variable|hwrite
specifier|static
name|HANDLE
name|hthread
decl_stmt|,
name|hread
decl_stmt|,
name|hwrite
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|hconsole1
DECL|variable|hconsole2
specifier|static
name|HANDLE
name|hconsole1
decl_stmt|,
name|hconsole2
decl_stmt|;
end_decl_stmt

begin_ifdef
ifdef|#
directive|ifdef
name|__MINGW32__
end_ifdef

begin_if
if|#
directive|if
operator|!
name|defined
argument_list|(
name|__MINGW64_VERSION_MAJOR
argument_list|)
operator|||
name|__MINGW64_VERSION_MAJOR
operator|<
literal|5
end_if

begin_typedef
DECL|struct|_CONSOLE_FONT_INFOEX
typedef|typedef
struct|struct
name|_CONSOLE_FONT_INFOEX
block|{
DECL|member|cbSize
name|ULONG
name|cbSize
decl_stmt|;
DECL|member|nFont
name|DWORD
name|nFont
decl_stmt|;
DECL|member|dwFontSize
name|COORD
name|dwFontSize
decl_stmt|;
DECL|member|FontFamily
name|UINT
name|FontFamily
decl_stmt|;
DECL|member|FontWeight
name|UINT
name|FontWeight
decl_stmt|;
DECL|member|FaceName
name|WCHAR
name|FaceName
index|[
name|LF_FACESIZE
index|]
decl_stmt|;
DECL|typedef|CONSOLE_FONT_INFOEX
DECL|typedef|PCONSOLE_FONT_INFOEX
block|}
name|CONSOLE_FONT_INFOEX
operator|,
typedef|*
name|PCONSOLE_FONT_INFOEX
typedef|;
end_typedef

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_typedef
DECL|typedef|PGETCURRENTCONSOLEFONTEX
typedef|typedef
name|BOOL
function_decl|(
name|WINAPI
modifier|*
name|PGETCURRENTCONSOLEFONTEX
function_decl|)
parameter_list|(
name|HANDLE
parameter_list|,
name|BOOL
parameter_list|,
name|PCONSOLE_FONT_INFOEX
parameter_list|)
function_decl|;
end_typedef

begin_function
DECL|function|warn_if_raster_font
specifier|static
name|void
name|warn_if_raster_font
parameter_list|(
name|void
parameter_list|)
block|{
name|DWORD
name|fontFamily
init|=
literal|0
decl_stmt|;
name|PGETCURRENTCONSOLEFONTEX
name|pGetCurrentConsoleFontEx
decl_stmt|;
comment|/* don't bother if output was ascii only */
if|if
condition|(
operator|!
name|non_ascii_used
condition|)
return|return;
comment|/* GetCurrentConsoleFontEx is available since Vista */
name|pGetCurrentConsoleFontEx
operator|=
operator|(
name|PGETCURRENTCONSOLEFONTEX
operator|)
name|GetProcAddress
argument_list|(
name|GetModuleHandle
argument_list|(
literal|"kernel32.dll"
argument_list|)
argument_list|,
literal|"GetCurrentConsoleFontEx"
argument_list|)
expr_stmt|;
if|if
condition|(
name|pGetCurrentConsoleFontEx
condition|)
block|{
name|CONSOLE_FONT_INFOEX
name|cfi
decl_stmt|;
name|cfi
operator|.
name|cbSize
operator|=
sizeof|sizeof
argument_list|(
name|cfi
argument_list|)
expr_stmt|;
if|if
condition|(
name|pGetCurrentConsoleFontEx
argument_list|(
name|console
argument_list|,
literal|0
argument_list|,
operator|&
name|cfi
argument_list|)
condition|)
name|fontFamily
operator|=
name|cfi
operator|.
name|FontFamily
expr_stmt|;
block|}
else|else
block|{
comment|/* pre-Vista: check default console font in registry */
name|HKEY
name|hkey
decl_stmt|;
if|if
condition|(
name|ERROR_SUCCESS
operator|==
name|RegOpenKeyExA
argument_list|(
name|HKEY_CURRENT_USER
argument_list|,
literal|"Console"
argument_list|,
literal|0
argument_list|,
name|KEY_READ
argument_list|,
operator|&
name|hkey
argument_list|)
condition|)
block|{
name|DWORD
name|size
init|=
sizeof|sizeof
argument_list|(
name|fontFamily
argument_list|)
decl_stmt|;
name|RegQueryValueExA
argument_list|(
name|hkey
argument_list|,
literal|"FontFamily"
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|,
operator|(
name|LPVOID
operator|)
operator|&
name|fontFamily
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
name|RegCloseKey
argument_list|(
name|hkey
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
operator|(
name|fontFamily
operator|&
name|TMPF_TRUETYPE
operator|)
condition|)
block|{
specifier|const
name|wchar_t
modifier|*
name|msg
init|=
literal|L"\nWarning: Your console font probably "
literal|L"doesn\'t support Unicode. If you experience strange "
literal|L"characters in the output, consider switching to a "
literal|L"TrueType font such as Consolas!\n"
decl_stmt|;
name|DWORD
name|dummy
decl_stmt|;
name|WriteConsoleW
argument_list|(
name|console
argument_list|,
name|msg
argument_list|,
name|wcslen
argument_list|(
name|msg
argument_list|)
argument_list|,
operator|&
name|dummy
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|is_console
specifier|static
name|int
name|is_console
parameter_list|(
name|int
name|fd
parameter_list|)
block|{
name|CONSOLE_SCREEN_BUFFER_INFO
name|sbi
decl_stmt|;
name|HANDLE
name|hcon
decl_stmt|;
specifier|static
name|int
name|initialized
init|=
literal|0
decl_stmt|;
comment|/* get OS handle of the file descriptor */
name|hcon
operator|=
operator|(
name|HANDLE
operator|)
name|_get_osfhandle
argument_list|(
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|hcon
operator|==
name|INVALID_HANDLE_VALUE
condition|)
return|return
literal|0
return|;
comment|/* check if its a device (i.e. console, printer, serial port) */
if|if
condition|(
name|GetFileType
argument_list|(
name|hcon
argument_list|)
operator|!=
name|FILE_TYPE_CHAR
condition|)
return|return
literal|0
return|;
comment|/* check if its a handle to a console output screen buffer */
if|if
condition|(
operator|!
name|GetConsoleScreenBufferInfo
argument_list|(
name|hcon
argument_list|,
operator|&
name|sbi
argument_list|)
condition|)
return|return
literal|0
return|;
comment|/* initialize attributes */
if|if
condition|(
operator|!
name|initialized
condition|)
block|{
name|console
operator|=
name|hcon
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
return|return
literal|1
return|;
block|}
end_function

begin_define
DECL|macro|BUFFER_SIZE
define|#
directive|define
name|BUFFER_SIZE
value|4096
end_define

begin_define
DECL|macro|MAX_PARAMS
define|#
directive|define
name|MAX_PARAMS
value|16
end_define

begin_function
DECL|function|write_console
specifier|static
name|void
name|write_console
parameter_list|(
name|unsigned
name|char
modifier|*
name|str
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
comment|/* only called from console_thread, so a static buffer will do */
specifier|static
name|wchar_t
name|wbuf
index|[
literal|2
operator|*
name|BUFFER_SIZE
operator|+
literal|1
index|]
decl_stmt|;
name|DWORD
name|dummy
decl_stmt|;
comment|/* convert utf-8 to utf-16 */
name|int
name|wlen
init|=
name|xutftowcsn
argument_list|(
name|wbuf
argument_list|,
operator|(
name|char
operator|*
operator|)
name|str
argument_list|,
name|ARRAY_SIZE
argument_list|(
name|wbuf
argument_list|)
argument_list|,
name|len
argument_list|)
decl_stmt|;
comment|/* write directly to console */
name|WriteConsoleW
argument_list|(
name|console
argument_list|,
name|wbuf
argument_list|,
name|wlen
argument_list|,
operator|&
name|dummy
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
comment|/* remember if non-ascii characters are printed */
if|if
condition|(
name|wlen
operator|!=
name|len
condition|)
name|non_ascii_used
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
name|DWORD
name|dummy
decl_stmt|;
comment|/* Needed for Windows 7 (or Vista) regression */
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
operator|&
name|dummy
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|set_attr
specifier|static
name|void
name|set_attr
parameter_list|(
name|char
name|func
parameter_list|,
specifier|const
name|int
modifier|*
name|params
parameter_list|,
name|int
name|paramlen
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
switch|switch
condition|(
name|func
condition|)
block|{
case|case
literal|'m'
case|:
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|paramlen
condition|;
name|i
operator|++
control|)
block|{
switch|switch
condition|(
name|params
index|[
name|i
index|]
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
block|}
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
block|}
end_function

begin_enum
enum|enum
block|{
DECL|enumerator|TEXT
DECL|enumerator|ESCAPE
DECL|enumerator|BRACKET
name|TEXT
init|=
literal|0
block|,
name|ESCAPE
init|=
literal|033
block|,
name|BRACKET
init|=
literal|'['
block|}
enum|;
end_enum

begin_function
DECL|function|console_thread
specifier|static
name|DWORD
name|WINAPI
name|console_thread
parameter_list|(
name|LPVOID
name|unused
parameter_list|)
block|{
name|unsigned
name|char
name|buffer
index|[
name|BUFFER_SIZE
index|]
decl_stmt|;
name|DWORD
name|bytes
decl_stmt|;
name|int
name|start
decl_stmt|,
name|end
init|=
literal|0
decl_stmt|,
name|c
decl_stmt|,
name|parampos
init|=
literal|0
decl_stmt|,
name|state
init|=
name|TEXT
decl_stmt|;
name|int
name|params
index|[
name|MAX_PARAMS
index|]
decl_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
comment|/* read next chunk of bytes from the pipe */
if|if
condition|(
operator|!
name|ReadFile
argument_list|(
name|hread
argument_list|,
name|buffer
operator|+
name|end
argument_list|,
name|BUFFER_SIZE
operator|-
name|end
argument_list|,
operator|&
name|bytes
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
comment|/* exit if pipe has been closed or disconnected */
if|if
condition|(
name|GetLastError
argument_list|()
operator|==
name|ERROR_PIPE_NOT_CONNECTED
operator|||
name|GetLastError
argument_list|()
operator|==
name|ERROR_BROKEN_PIPE
condition|)
break|break;
comment|/* ignore other errors */
continue|continue;
block|}
comment|/* scan the bytes and handle ANSI control codes */
name|bytes
operator|+=
name|end
expr_stmt|;
name|start
operator|=
name|end
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|end
operator|<
name|bytes
condition|)
block|{
name|c
operator|=
name|buffer
index|[
name|end
operator|++
index|]
expr_stmt|;
switch|switch
condition|(
name|state
condition|)
block|{
case|case
name|TEXT
case|:
if|if
condition|(
name|c
operator|==
name|ESCAPE
condition|)
block|{
comment|/* print text seen so far */
if|if
condition|(
name|end
operator|-
literal|1
operator|>
name|start
condition|)
name|write_console
argument_list|(
name|buffer
operator|+
name|start
argument_list|,
name|end
operator|-
literal|1
operator|-
name|start
argument_list|)
expr_stmt|;
comment|/* then start parsing escape sequence */
name|start
operator|=
name|end
operator|-
literal|1
expr_stmt|;
name|memset
argument_list|(
name|params
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|params
argument_list|)
argument_list|)
expr_stmt|;
name|parampos
operator|=
literal|0
expr_stmt|;
name|state
operator|=
name|ESCAPE
expr_stmt|;
block|}
break|break;
case|case
name|ESCAPE
case|:
comment|/* continue if "\033[", otherwise bail out */
name|state
operator|=
operator|(
name|c
operator|==
name|BRACKET
operator|)
condition|?
name|BRACKET
else|:
name|TEXT
expr_stmt|;
break|break;
case|case
name|BRACKET
case|:
comment|/* parse [0-9;]* into array of parameters */
if|if
condition|(
name|c
operator|>=
literal|'0'
operator|&&
name|c
operator|<=
literal|'9'
condition|)
block|{
name|params
index|[
name|parampos
index|]
operator|*=
literal|10
expr_stmt|;
name|params
index|[
name|parampos
index|]
operator|+=
name|c
operator|-
literal|'0'
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|c
operator|==
literal|';'
condition|)
block|{
comment|/* 					 * next parameter, bail out if out of 					 * bounds 					 */
name|parampos
operator|++
expr_stmt|;
if|if
condition|(
name|parampos
operator|>=
name|MAX_PARAMS
condition|)
name|state
operator|=
name|TEXT
expr_stmt|;
block|}
else|else
block|{
comment|/* 					 * end of escape sequence, change 					 * console attributes 					 */
name|set_attr
argument_list|(
name|c
argument_list|,
name|params
argument_list|,
name|parampos
operator|+
literal|1
argument_list|)
expr_stmt|;
name|start
operator|=
name|end
expr_stmt|;
name|state
operator|=
name|TEXT
expr_stmt|;
block|}
break|break;
block|}
block|}
comment|/* print remaining text unless parsing an escape sequence */
if|if
condition|(
name|state
operator|==
name|TEXT
operator|&&
name|end
operator|>
name|start
condition|)
block|{
comment|/* check for incomplete UTF-8 sequences and fix end */
if|if
condition|(
name|buffer
index|[
name|end
operator|-
literal|1
index|]
operator|>=
literal|0x80
condition|)
block|{
if|if
condition|(
name|buffer
index|[
name|end
operator|-
literal|1
index|]
operator|>=
literal|0xc0
condition|)
name|end
operator|--
expr_stmt|;
elseif|else
if|if
condition|(
name|end
operator|-
literal|1
operator|>
name|start
operator|&&
name|buffer
index|[
name|end
operator|-
literal|2
index|]
operator|>=
literal|0xe0
condition|)
name|end
operator|-=
literal|2
expr_stmt|;
elseif|else
if|if
condition|(
name|end
operator|-
literal|2
operator|>
name|start
operator|&&
name|buffer
index|[
name|end
operator|-
literal|3
index|]
operator|>=
literal|0xf0
condition|)
name|end
operator|-=
literal|3
expr_stmt|;
block|}
comment|/* print remaining complete UTF-8 sequences */
if|if
condition|(
name|end
operator|>
name|start
condition|)
name|write_console
argument_list|(
name|buffer
operator|+
name|start
argument_list|,
name|end
operator|-
name|start
argument_list|)
expr_stmt|;
comment|/* move remaining bytes to the front */
if|if
condition|(
name|end
operator|<
name|bytes
condition|)
name|memmove
argument_list|(
name|buffer
argument_list|,
name|buffer
operator|+
name|end
argument_list|,
name|bytes
operator|-
name|end
argument_list|)
expr_stmt|;
name|end
operator|=
name|bytes
operator|-
name|end
expr_stmt|;
block|}
else|else
block|{
comment|/* all data has been consumed, mark buffer empty */
name|end
operator|=
literal|0
expr_stmt|;
block|}
block|}
comment|/* check if the console font supports unicode */
name|warn_if_raster_font
argument_list|()
expr_stmt|;
name|CloseHandle
argument_list|(
name|hread
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|winansi_exit
specifier|static
name|void
name|winansi_exit
parameter_list|(
name|void
parameter_list|)
block|{
comment|/* flush all streams */
name|_flushall
argument_list|()
expr_stmt|;
comment|/* signal console thread to exit */
name|FlushFileBuffers
argument_list|(
name|hwrite
argument_list|)
expr_stmt|;
name|DisconnectNamedPipe
argument_list|(
name|hwrite
argument_list|)
expr_stmt|;
comment|/* wait for console thread to copy remaining data */
name|WaitForSingleObject
argument_list|(
name|hthread
argument_list|,
name|INFINITE
argument_list|)
expr_stmt|;
comment|/* cleanup handles... */
name|CloseHandle
argument_list|(
name|hwrite
argument_list|)
expr_stmt|;
name|CloseHandle
argument_list|(
name|hthread
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|die_lasterr
specifier|static
name|void
name|die_lasterr
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|params
decl_stmt|;
name|va_start
argument_list|(
name|params
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|errno
operator|=
name|err_win_to_posix
argument_list|(
name|GetLastError
argument_list|()
argument_list|)
expr_stmt|;
name|die_errno
argument_list|(
name|fmt
argument_list|,
name|params
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|params
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|duplicate_handle
specifier|static
name|HANDLE
name|duplicate_handle
parameter_list|(
name|HANDLE
name|hnd
parameter_list|)
block|{
name|HANDLE
name|hresult
decl_stmt|,
name|hproc
init|=
name|GetCurrentProcess
argument_list|()
decl_stmt|;
if|if
condition|(
operator|!
name|DuplicateHandle
argument_list|(
name|hproc
argument_list|,
name|hnd
argument_list|,
name|hproc
argument_list|,
operator|&
name|hresult
argument_list|,
literal|0
argument_list|,
name|TRUE
argument_list|,
name|DUPLICATE_SAME_ACCESS
argument_list|)
condition|)
name|die_lasterr
argument_list|(
literal|"DuplicateHandle(%li) failed"
argument_list|,
operator|(
name|long
operator|)
operator|(
name|intptr_t
operator|)
name|hnd
argument_list|)
expr_stmt|;
return|return
name|hresult
return|;
block|}
end_function

begin_comment
comment|/*  * Make MSVCRT's internal file descriptor control structure accessible  * so that we can tweak OS handles and flags directly (we need MSVCRT  * to treat our pipe handle as if it were a console).  *  * We assume that the ioinfo structure (exposed by MSVCRT.dll via  * __pioinfo) starts with the OS handle and the flags. The exact size  * varies between MSVCRT versions, so we try different sizes until  * toggling the FDEV bit of _pioinfo(1)->osflags is reflected in  * isatty(1).  */
end_comment

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|osfhnd
name|HANDLE
name|osfhnd
decl_stmt|;
DECL|member|osflags
name|char
name|osflags
decl_stmt|;
DECL|typedef|ioinfo
block|}
name|ioinfo
typedef|;
end_typedef

begin_extern
extern|extern __declspec(dllimport
end_extern

begin_decl_stmt
unit|)
name|ioinfo
modifier|*
name|__pioinfo
index|[]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|sizeof_ioinfo
specifier|static
name|size_t
name|sizeof_ioinfo
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|IOINFO_L2E
define|#
directive|define
name|IOINFO_L2E
value|5
end_define

begin_define
DECL|macro|IOINFO_ARRAY_ELTS
define|#
directive|define
name|IOINFO_ARRAY_ELTS
value|(1<< IOINFO_L2E)
end_define

begin_define
DECL|macro|FPIPE
define|#
directive|define
name|FPIPE
value|0x08
end_define

begin_define
DECL|macro|FDEV
define|#
directive|define
name|FDEV
value|0x40
end_define

begin_function
DECL|function|_pioinfo
specifier|static
specifier|inline
name|ioinfo
modifier|*
name|_pioinfo
parameter_list|(
name|int
name|fd
parameter_list|)
block|{
return|return
operator|(
name|ioinfo
operator|*
operator|)
operator|(
operator|(
name|char
operator|*
operator|)
name|__pioinfo
index|[
name|fd
operator|>>
name|IOINFO_L2E
index|]
operator|+
operator|(
name|fd
operator|&
operator|(
name|IOINFO_ARRAY_ELTS
operator|-
literal|1
operator|)
operator|)
operator|*
name|sizeof_ioinfo
operator|)
return|;
block|}
end_function

begin_function
DECL|function|init_sizeof_ioinfo
specifier|static
name|int
name|init_sizeof_ioinfo
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|istty
decl_stmt|,
name|wastty
decl_stmt|;
comment|/* don't init twice */
if|if
condition|(
name|sizeof_ioinfo
condition|)
return|return
name|sizeof_ioinfo
operator|>=
literal|256
return|;
name|sizeof_ioinfo
operator|=
sizeof|sizeof
argument_list|(
name|ioinfo
argument_list|)
expr_stmt|;
name|wastty
operator|=
name|isatty
argument_list|(
literal|1
argument_list|)
expr_stmt|;
while|while
condition|(
name|sizeof_ioinfo
operator|<
literal|256
condition|)
block|{
comment|/* toggle FDEV flag, check isatty, then toggle back */
name|_pioinfo
argument_list|(
literal|1
argument_list|)
operator|->
name|osflags
operator|^=
name|FDEV
expr_stmt|;
name|istty
operator|=
name|isatty
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|_pioinfo
argument_list|(
literal|1
argument_list|)
operator|->
name|osflags
operator|^=
name|FDEV
expr_stmt|;
comment|/* return if we found the correct size */
if|if
condition|(
name|istty
operator|!=
name|wastty
condition|)
return|return
literal|0
return|;
name|sizeof_ioinfo
operator|+=
sizeof|sizeof
argument_list|(
name|void
operator|*
argument_list|)
expr_stmt|;
block|}
name|error
argument_list|(
literal|"Tweaking file descriptors doesn't work with this MSVCRT.dll"
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|swap_osfhnd
specifier|static
name|HANDLE
name|swap_osfhnd
parameter_list|(
name|int
name|fd
parameter_list|,
name|HANDLE
name|new_handle
parameter_list|)
block|{
name|ioinfo
modifier|*
name|pioinfo
decl_stmt|;
name|HANDLE
name|old_handle
decl_stmt|;
comment|/* init ioinfo size if we haven't done so */
if|if
condition|(
name|init_sizeof_ioinfo
argument_list|()
condition|)
return|return
name|INVALID_HANDLE_VALUE
return|;
comment|/* get ioinfo pointer and change the handles */
name|pioinfo
operator|=
name|_pioinfo
argument_list|(
name|fd
argument_list|)
expr_stmt|;
name|old_handle
operator|=
name|pioinfo
operator|->
name|osfhnd
expr_stmt|;
name|pioinfo
operator|->
name|osfhnd
operator|=
name|new_handle
expr_stmt|;
return|return
name|old_handle
return|;
block|}
end_function

begin_ifdef
ifdef|#
directive|ifdef
name|DETECT_MSYS_TTY
end_ifdef

begin_include
include|#
directive|include
file|<winternl.h>
end_include

begin_include
include|#
directive|include
file|<ntstatus.h>
end_include

begin_function
DECL|function|detect_msys_tty
specifier|static
name|void
name|detect_msys_tty
parameter_list|(
name|int
name|fd
parameter_list|)
block|{
name|ULONG
name|result
decl_stmt|;
name|BYTE
name|buffer
index|[
literal|1024
index|]
decl_stmt|;
name|POBJECT_NAME_INFORMATION
name|nameinfo
init|=
operator|(
name|POBJECT_NAME_INFORMATION
operator|)
name|buffer
decl_stmt|;
name|PWSTR
name|name
decl_stmt|;
comment|/* check if fd is a pipe */
name|HANDLE
name|h
init|=
operator|(
name|HANDLE
operator|)
name|_get_osfhandle
argument_list|(
name|fd
argument_list|)
decl_stmt|;
if|if
condition|(
name|GetFileType
argument_list|(
name|h
argument_list|)
operator|!=
name|FILE_TYPE_PIPE
condition|)
return|return;
comment|/* get pipe name */
if|if
condition|(
operator|!
name|NT_SUCCESS
argument_list|(
name|NtQueryObject
argument_list|(
name|h
argument_list|,
name|ObjectNameInformation
argument_list|,
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
operator|-
literal|2
argument_list|,
operator|&
name|result
argument_list|)
argument_list|)
condition|)
return|return;
name|name
operator|=
name|nameinfo
operator|->
name|Name
operator|.
name|Buffer
expr_stmt|;
name|name
index|[
name|nameinfo
operator|->
name|Name
operator|.
name|Length
index|]
operator|=
literal|0
expr_stmt|;
comment|/* check if this could be a MSYS2 pty pipe ('msys-XXXX-ptyN-XX') */
if|if
condition|(
operator|!
name|wcsstr
argument_list|(
name|name
argument_list|,
literal|L"msys-"
argument_list|)
operator|||
operator|!
name|wcsstr
argument_list|(
name|name
argument_list|,
literal|L"-pty"
argument_list|)
condition|)
return|return;
comment|/* init ioinfo size if we haven't done so */
if|if
condition|(
name|init_sizeof_ioinfo
argument_list|()
condition|)
return|return;
comment|/* set FDEV flag, reset FPIPE flag */
name|_pioinfo
argument_list|(
name|fd
argument_list|)
operator|->
name|osflags
operator|&=
operator|~
name|FPIPE
expr_stmt|;
name|_pioinfo
argument_list|(
name|fd
argument_list|)
operator|->
name|osflags
operator||=
name|FDEV
expr_stmt|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|winansi_init
name|void
name|winansi_init
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|con1
decl_stmt|,
name|con2
decl_stmt|;
name|char
name|name
index|[
literal|32
index|]
decl_stmt|;
comment|/* check if either stdout or stderr is a console output screen buffer */
name|con1
operator|=
name|is_console
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|con2
operator|=
name|is_console
argument_list|(
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|con1
operator|&&
operator|!
name|con2
condition|)
block|{
ifdef|#
directive|ifdef
name|DETECT_MSYS_TTY
comment|/* check if stdin / stdout / stderr are MSYS2 pty pipes */
name|detect_msys_tty
argument_list|(
literal|0
argument_list|)
expr_stmt|;
name|detect_msys_tty
argument_list|(
literal|1
argument_list|)
expr_stmt|;
name|detect_msys_tty
argument_list|(
literal|2
argument_list|)
expr_stmt|;
endif|#
directive|endif
return|return;
block|}
comment|/* create a named pipe to communicate with the console thread */
name|xsnprintf
argument_list|(
name|name
argument_list|,
sizeof|sizeof
argument_list|(
name|name
argument_list|)
argument_list|,
literal|"\\\\.\\pipe\\winansi%lu"
argument_list|,
name|GetCurrentProcessId
argument_list|()
argument_list|)
expr_stmt|;
name|hwrite
operator|=
name|CreateNamedPipe
argument_list|(
name|name
argument_list|,
name|PIPE_ACCESS_OUTBOUND
argument_list|,
name|PIPE_TYPE_BYTE
operator||
name|PIPE_WAIT
argument_list|,
literal|1
argument_list|,
name|BUFFER_SIZE
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|hwrite
operator|==
name|INVALID_HANDLE_VALUE
condition|)
name|die_lasterr
argument_list|(
literal|"CreateNamedPipe failed"
argument_list|)
expr_stmt|;
name|hread
operator|=
name|CreateFile
argument_list|(
name|name
argument_list|,
name|GENERIC_READ
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
name|OPEN_EXISTING
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|hread
operator|==
name|INVALID_HANDLE_VALUE
condition|)
name|die_lasterr
argument_list|(
literal|"CreateFile for named pipe failed"
argument_list|)
expr_stmt|;
comment|/* start console spool thread on the pipe's read end */
name|hthread
operator|=
name|CreateThread
argument_list|(
name|NULL
argument_list|,
literal|0
argument_list|,
name|console_thread
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|hthread
operator|==
name|INVALID_HANDLE_VALUE
condition|)
name|die_lasterr
argument_list|(
literal|"CreateThread(console_thread) failed"
argument_list|)
expr_stmt|;
comment|/* schedule cleanup routine */
if|if
condition|(
name|atexit
argument_list|(
name|winansi_exit
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"atexit(winansi_exit) failed"
argument_list|)
expr_stmt|;
comment|/* redirect stdout / stderr to the pipe */
if|if
condition|(
name|con1
condition|)
name|hconsole1
operator|=
name|swap_osfhnd
argument_list|(
literal|1
argument_list|,
name|duplicate_handle
argument_list|(
name|hwrite
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|con2
condition|)
name|hconsole2
operator|=
name|swap_osfhnd
argument_list|(
literal|2
argument_list|,
name|duplicate_handle
argument_list|(
name|hwrite
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * Returns the real console handle if stdout / stderr is a pipe redirecting  * to the console. Allows spawn / exec to pass the console to the next process.  */
end_comment

begin_function
DECL|function|winansi_get_osfhandle
name|HANDLE
name|winansi_get_osfhandle
parameter_list|(
name|int
name|fd
parameter_list|)
block|{
name|HANDLE
name|hnd
init|=
operator|(
name|HANDLE
operator|)
name|_get_osfhandle
argument_list|(
name|fd
argument_list|)
decl_stmt|;
if|if
condition|(
name|isatty
argument_list|(
name|fd
argument_list|)
operator|&&
name|GetFileType
argument_list|(
name|hnd
argument_list|)
operator|==
name|FILE_TYPE_PIPE
condition|)
block|{
if|if
condition|(
name|fd
operator|==
literal|1
operator|&&
name|hconsole1
condition|)
return|return
name|hconsole1
return|;
elseif|else
if|if
condition|(
name|fd
operator|==
literal|2
operator|&&
name|hconsole2
condition|)
return|return
name|hconsole2
return|;
block|}
return|return
name|hnd
return|;
block|}
end_function

end_unit

