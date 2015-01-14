begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * GIT - The information manager from hell  *  * Copyright (C) 2000-2002 Michael R. Elkins<me@mutt.org>  * Copyright (C) 2002-2004 Oswald Buddenhagen<ossi@users.sf.net>  * Copyright (C) 2004 Theodore Y. Ts'o<tytso@mit.edu>  * Copyright (C) 2006 Mike McCormack  * Copyright (C) 2006 Christian Couder  *  *  This program is free software; you can redistribute it and/or modify  *  it under the terms of the GNU General Public License as published by  *  the Free Software Foundation; either version 2 of the License, or  *  (at your option) any later version.  *  *  This program is distributed in the hope that it will be useful,  *  but WITHOUT ANY WARRANTY; without even the implied warranty of  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  *  GNU General Public License for more details.  *  *  You should have received a copy of the GNU General Public License  *  along with this program; if not, write to the Free Software  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_comment
comment|/* Get a trace file descriptor from "key" env variable. */
end_comment

begin_function
DECL|function|get_trace_fd
specifier|static
name|int
name|get_trace_fd
parameter_list|(
name|struct
name|trace_key
modifier|*
name|key
parameter_list|)
block|{
specifier|static
name|struct
name|trace_key
name|trace_default
init|=
block|{
literal|"GIT_TRACE"
block|}
decl_stmt|;
specifier|const
name|char
modifier|*
name|trace
decl_stmt|;
comment|/* use default "GIT_TRACE" if NULL */
if|if
condition|(
operator|!
name|key
condition|)
name|key
operator|=
operator|&
name|trace_default
expr_stmt|;
comment|/* don't open twice */
if|if
condition|(
name|key
operator|->
name|initialized
condition|)
return|return
name|key
operator|->
name|fd
return|;
name|trace
operator|=
name|getenv
argument_list|(
name|key
operator|->
name|key
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|trace
operator|||
operator|!
name|strcmp
argument_list|(
name|trace
argument_list|,
literal|""
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|trace
argument_list|,
literal|"0"
argument_list|)
operator|||
operator|!
name|strcasecmp
argument_list|(
name|trace
argument_list|,
literal|"false"
argument_list|)
condition|)
name|key
operator|->
name|fd
operator|=
literal|0
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|trace
argument_list|,
literal|"1"
argument_list|)
operator|||
operator|!
name|strcasecmp
argument_list|(
name|trace
argument_list|,
literal|"true"
argument_list|)
condition|)
name|key
operator|->
name|fd
operator|=
name|STDERR_FILENO
expr_stmt|;
elseif|else
if|if
condition|(
name|strlen
argument_list|(
name|trace
argument_list|)
operator|==
literal|1
operator|&&
name|isdigit
argument_list|(
operator|*
name|trace
argument_list|)
condition|)
name|key
operator|->
name|fd
operator|=
name|atoi
argument_list|(
name|trace
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|is_absolute_path
argument_list|(
name|trace
argument_list|)
condition|)
block|{
name|int
name|fd
init|=
name|open
argument_list|(
name|trace
argument_list|,
name|O_WRONLY
operator||
name|O_APPEND
operator||
name|O_CREAT
argument_list|,
literal|0666
argument_list|)
decl_stmt|;
if|if
condition|(
name|fd
operator|==
operator|-
literal|1
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Could not open '%s' for tracing: %s\n"
literal|"Defaulting to tracing on stderr...\n"
argument_list|,
name|trace
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
name|key
operator|->
name|fd
operator|=
name|STDERR_FILENO
expr_stmt|;
block|}
else|else
block|{
name|key
operator|->
name|fd
operator|=
name|fd
expr_stmt|;
name|key
operator|->
name|need_close
operator|=
literal|1
expr_stmt|;
block|}
block|}
else|else
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"What does '%s' for %s mean?\n"
literal|"If you want to trace into a file, then please set "
literal|"%s to an absolute pathname (starting with /).\n"
literal|"Defaulting to tracing on stderr...\n"
argument_list|,
name|trace
argument_list|,
name|key
operator|->
name|key
argument_list|,
name|key
operator|->
name|key
argument_list|)
expr_stmt|;
name|key
operator|->
name|fd
operator|=
name|STDERR_FILENO
expr_stmt|;
block|}
name|key
operator|->
name|initialized
operator|=
literal|1
expr_stmt|;
return|return
name|key
operator|->
name|fd
return|;
block|}
end_function

begin_function
DECL|function|trace_disable
name|void
name|trace_disable
parameter_list|(
name|struct
name|trace_key
modifier|*
name|key
parameter_list|)
block|{
if|if
condition|(
name|key
operator|->
name|need_close
condition|)
name|close
argument_list|(
name|key
operator|->
name|fd
argument_list|)
expr_stmt|;
name|key
operator|->
name|fd
operator|=
literal|0
expr_stmt|;
name|key
operator|->
name|initialized
operator|=
literal|1
expr_stmt|;
name|key
operator|->
name|need_close
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|err_msg
specifier|static
specifier|const
name|char
name|err_msg
index|[]
init|=
literal|"Could not trace into fd given by "
literal|"GIT_TRACE environment variable"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|prepare_trace_line
specifier|static
name|int
name|prepare_trace_line
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
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|)
block|{
specifier|static
name|struct
name|trace_key
name|trace_bare
init|=
name|TRACE_KEY_INIT
argument_list|(
name|BARE
argument_list|)
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
name|struct
name|tm
name|tm
decl_stmt|;
name|time_t
name|secs
decl_stmt|;
if|if
condition|(
operator|!
name|trace_want
argument_list|(
name|key
argument_list|)
condition|)
return|return
literal|0
return|;
name|set_try_to_free_routine
argument_list|(
name|NULL
argument_list|)
expr_stmt|;
comment|/* is never reset */
comment|/* unit tests may want to disable additional trace output */
if|if
condition|(
name|trace_want
argument_list|(
operator|&
name|trace_bare
argument_list|)
condition|)
return|return
literal|1
return|;
comment|/* print current timestamp */
name|gettimeofday
argument_list|(
operator|&
name|tv
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|secs
operator|=
name|tv
operator|.
name|tv_sec
expr_stmt|;
name|localtime_r
argument_list|(
operator|&
name|secs
argument_list|,
operator|&
name|tm
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
name|buf
argument_list|,
literal|"%02d:%02d:%02d.%06ld "
argument_list|,
name|tm
operator|.
name|tm_hour
argument_list|,
name|tm
operator|.
name|tm_min
argument_list|,
name|tm
operator|.
name|tm_sec
argument_list|,
operator|(
name|long
operator|)
name|tv
operator|.
name|tv_usec
argument_list|)
expr_stmt|;
ifdef|#
directive|ifdef
name|HAVE_VARIADIC_MACROS
comment|/* print file:line */
name|strbuf_addf
argument_list|(
name|buf
argument_list|,
literal|"%s:%d "
argument_list|,
name|file
argument_list|,
name|line
argument_list|)
expr_stmt|;
comment|/* align trace output (column 40 catches most files names in git) */
while|while
condition|(
name|buf
operator|->
name|len
operator|<
literal|40
condition|)
name|strbuf_addch
argument_list|(
name|buf
argument_list|,
literal|' '
argument_list|)
expr_stmt|;
endif|#
directive|endif
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|print_trace_line
specifier|static
name|void
name|print_trace_line
parameter_list|(
name|struct
name|trace_key
modifier|*
name|key
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|)
block|{
name|strbuf_complete_line
argument_list|(
name|buf
argument_list|)
expr_stmt|;
name|write_or_whine_pipe
argument_list|(
name|get_trace_fd
argument_list|(
name|key
argument_list|)
argument_list|,
name|buf
operator|->
name|buf
argument_list|,
name|buf
operator|->
name|len
argument_list|,
name|err_msg
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_vprintf_fl
specifier|static
name|void
name|trace_vprintf_fl
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
name|va_list
name|ap
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
if|if
condition|(
operator|!
name|prepare_trace_line
argument_list|(
name|file
argument_list|,
name|line
argument_list|,
name|key
argument_list|,
operator|&
name|buf
argument_list|)
condition|)
return|return;
name|strbuf_vaddf
argument_list|(
operator|&
name|buf
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|print_trace_line
argument_list|(
name|key
argument_list|,
operator|&
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_argv_vprintf_fl
specifier|static
name|void
name|trace_argv_vprintf_fl
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
name|va_list
name|ap
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
if|if
condition|(
operator|!
name|prepare_trace_line
argument_list|(
name|file
argument_list|,
name|line
argument_list|,
name|NULL
argument_list|,
operator|&
name|buf
argument_list|)
condition|)
return|return;
name|strbuf_vaddf
argument_list|(
operator|&
name|buf
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|sq_quote_argv
argument_list|(
operator|&
name|buf
argument_list|,
name|argv
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|print_trace_line
argument_list|(
name|NULL
argument_list|,
operator|&
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_strbuf_fl
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
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
if|if
condition|(
operator|!
name|prepare_trace_line
argument_list|(
name|file
argument_list|,
name|line
argument_list|,
name|key
argument_list|,
operator|&
name|buf
argument_list|)
condition|)
return|return;
name|strbuf_addbuf
argument_list|(
operator|&
name|buf
argument_list|,
name|data
argument_list|)
expr_stmt|;
name|print_trace_line
argument_list|(
name|key
argument_list|,
operator|&
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|trace_perf_key
specifier|static
name|struct
name|trace_key
name|trace_perf_key
init|=
name|TRACE_KEY_INIT
argument_list|(
name|PERFORMANCE
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|trace_performance_vprintf_fl
specifier|static
name|void
name|trace_performance_vprintf_fl
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
name|format
parameter_list|,
name|va_list
name|ap
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
if|if
condition|(
operator|!
name|prepare_trace_line
argument_list|(
name|file
argument_list|,
name|line
argument_list|,
operator|&
name|trace_perf_key
argument_list|,
operator|&
name|buf
argument_list|)
condition|)
return|return;
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"performance: %.9f s"
argument_list|,
operator|(
name|double
operator|)
name|nanos
operator|/
literal|1000000000
argument_list|)
expr_stmt|;
if|if
condition|(
name|format
operator|&&
operator|*
name|format
condition|)
block|{
name|strbuf_addstr
argument_list|(
operator|&
name|buf
argument_list|,
literal|": "
argument_list|)
expr_stmt|;
name|strbuf_vaddf
argument_list|(
operator|&
name|buf
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
block|}
name|print_trace_line
argument_list|(
operator|&
name|trace_perf_key
argument_list|,
operator|&
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_ifndef
ifndef|#
directive|ifndef
name|HAVE_VARIADIC_MACROS
end_ifndef

begin_function
DECL|function|trace_printf
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
block|{
name|va_list
name|ap
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|trace_vprintf_fl
argument_list|(
name|NULL
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_printf_key
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
block|{
name|va_list
name|ap
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|trace_vprintf_fl
argument_list|(
name|NULL
argument_list|,
literal|0
argument_list|,
name|key
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_argv_printf
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
block|{
name|va_list
name|ap
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|trace_argv_vprintf_fl
argument_list|(
name|NULL
argument_list|,
literal|0
argument_list|,
name|argv
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_strbuf
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
block|{
name|trace_strbuf_fl
argument_list|(
name|NULL
argument_list|,
literal|0
argument_list|,
name|key
argument_list|,
name|data
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_performance
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
block|{
name|va_list
name|ap
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|trace_performance_vprintf_fl
argument_list|(
name|NULL
argument_list|,
literal|0
argument_list|,
name|nanos
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_performance_since
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
block|{
name|va_list
name|ap
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|trace_performance_vprintf_fl
argument_list|(
name|NULL
argument_list|,
literal|0
argument_list|,
name|getnanotime
argument_list|()
operator|-
name|start
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
block|}
end_function

begin_else
else|#
directive|else
end_else

begin_function
DECL|function|trace_printf_key_fl
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
block|{
name|va_list
name|ap
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|trace_vprintf_fl
argument_list|(
name|file
argument_list|,
name|line
argument_list|,
name|key
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_argv_printf_fl
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
block|{
name|va_list
name|ap
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|trace_argv_vprintf_fl
argument_list|(
name|file
argument_list|,
name|line
argument_list|,
name|argv
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_performance_fl
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
name|format
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|ap
decl_stmt|;
name|va_start
argument_list|(
name|ap
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|trace_performance_vprintf_fl
argument_list|(
name|file
argument_list|,
name|line
argument_list|,
name|nanos
argument_list|,
name|format
argument_list|,
name|ap
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|ap
argument_list|)
expr_stmt|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* HAVE_VARIADIC_MACROS */
end_comment

begin_function
DECL|function|quote_crnl
specifier|static
specifier|const
name|char
modifier|*
name|quote_crnl
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
specifier|static
name|char
name|new_path
index|[
name|PATH_MAX
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|p2
init|=
name|path
decl_stmt|;
name|char
modifier|*
name|p1
init|=
name|new_path
decl_stmt|;
if|if
condition|(
operator|!
name|path
condition|)
return|return
name|NULL
return|;
while|while
condition|(
operator|*
name|p2
condition|)
block|{
switch|switch
condition|(
operator|*
name|p2
condition|)
block|{
case|case
literal|'\\'
case|:
operator|*
name|p1
operator|++
operator|=
literal|'\\'
expr_stmt|;
operator|*
name|p1
operator|++
operator|=
literal|'\\'
expr_stmt|;
break|break;
case|case
literal|'\n'
case|:
operator|*
name|p1
operator|++
operator|=
literal|'\\'
expr_stmt|;
operator|*
name|p1
operator|++
operator|=
literal|'n'
expr_stmt|;
break|break;
case|case
literal|'\r'
case|:
operator|*
name|p1
operator|++
operator|=
literal|'\\'
expr_stmt|;
operator|*
name|p1
operator|++
operator|=
literal|'r'
expr_stmt|;
break|break;
default|default:
operator|*
name|p1
operator|++
operator|=
operator|*
name|p2
expr_stmt|;
block|}
name|p2
operator|++
expr_stmt|;
block|}
operator|*
name|p1
operator|=
literal|'\0'
expr_stmt|;
return|return
name|new_path
return|;
block|}
end_function

begin_comment
comment|/* FIXME: move prefix to startup_info struct and get rid of this arg */
end_comment

begin_function
DECL|function|trace_repo_setup
name|void
name|trace_repo_setup
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
specifier|static
name|struct
name|trace_key
name|key
init|=
name|TRACE_KEY_INIT
argument_list|(
name|SETUP
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|git_work_tree
decl_stmt|;
name|char
modifier|*
name|cwd
decl_stmt|;
if|if
condition|(
operator|!
name|trace_want
argument_list|(
operator|&
name|key
argument_list|)
condition|)
return|return;
name|cwd
operator|=
name|xgetcwd
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|git_work_tree
operator|=
name|get_git_work_tree
argument_list|()
operator|)
condition|)
name|git_work_tree
operator|=
literal|"(null)"
expr_stmt|;
if|if
condition|(
operator|!
name|prefix
condition|)
name|prefix
operator|=
literal|"(null)"
expr_stmt|;
name|trace_printf_key
argument_list|(
operator|&
name|key
argument_list|,
literal|"setup: git_dir: %s\n"
argument_list|,
name|quote_crnl
argument_list|(
name|get_git_dir
argument_list|()
argument_list|)
argument_list|)
expr_stmt|;
name|trace_printf_key
argument_list|(
operator|&
name|key
argument_list|,
literal|"setup: worktree: %s\n"
argument_list|,
name|quote_crnl
argument_list|(
name|git_work_tree
argument_list|)
argument_list|)
expr_stmt|;
name|trace_printf_key
argument_list|(
operator|&
name|key
argument_list|,
literal|"setup: cwd: %s\n"
argument_list|,
name|quote_crnl
argument_list|(
name|cwd
argument_list|)
argument_list|)
expr_stmt|;
name|trace_printf_key
argument_list|(
operator|&
name|key
argument_list|,
literal|"setup: prefix: %s\n"
argument_list|,
name|quote_crnl
argument_list|(
name|prefix
argument_list|)
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|cwd
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_want
name|int
name|trace_want
parameter_list|(
name|struct
name|trace_key
modifier|*
name|key
parameter_list|)
block|{
return|return
operator|!
operator|!
name|get_trace_fd
argument_list|(
name|key
argument_list|)
return|;
block|}
end_function

begin_if
if|#
directive|if
name|defined
argument_list|(
name|HAVE_CLOCK_GETTIME
argument_list|)
operator|&&
name|defined
argument_list|(
name|HAVE_CLOCK_MONOTONIC
argument_list|)
end_if

begin_function
DECL|function|highres_nanos
specifier|static
specifier|inline
name|uint64_t
name|highres_nanos
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|timespec
name|ts
decl_stmt|;
if|if
condition|(
name|clock_gettime
argument_list|(
name|CLOCK_MONOTONIC
argument_list|,
operator|&
name|ts
argument_list|)
condition|)
return|return
literal|0
return|;
return|return
operator|(
name|uint64_t
operator|)
name|ts
operator|.
name|tv_sec
operator|*
literal|1000000000
operator|+
name|ts
operator|.
name|tv_nsec
return|;
block|}
end_function

begin_elif
elif|#
directive|elif
name|defined
argument_list|(
name|GIT_WINDOWS_NATIVE
argument_list|)
end_elif

begin_function
DECL|function|highres_nanos
specifier|static
specifier|inline
name|uint64_t
name|highres_nanos
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|uint64_t
name|high_ns
decl_stmt|,
name|scaled_low_ns
decl_stmt|;
specifier|static
name|int
name|scale
decl_stmt|;
name|LARGE_INTEGER
name|cnt
decl_stmt|;
if|if
condition|(
operator|!
name|scale
condition|)
block|{
if|if
condition|(
operator|!
name|QueryPerformanceFrequency
argument_list|(
operator|&
name|cnt
argument_list|)
condition|)
return|return
literal|0
return|;
comment|/* high_ns = number of ns per cnt.HighPart */
name|high_ns
operator|=
operator|(
literal|1000000000LL
operator|<<
literal|32
operator|)
operator|/
operator|(
name|uint64_t
operator|)
name|cnt
operator|.
name|QuadPart
expr_stmt|;
comment|/* 		 * Number of ns per cnt.LowPart is 10^9 / frequency (or 		 * high_ns>> 32). For maximum precision, we scale this factor 		 * so that it just fits within 32 bit (i.e. won't overflow if 		 * multiplied with cnt.LowPart). 		 */
name|scaled_low_ns
operator|=
name|high_ns
expr_stmt|;
name|scale
operator|=
literal|32
expr_stmt|;
while|while
condition|(
name|scaled_low_ns
operator|>=
literal|0x100000000LL
condition|)
block|{
name|scaled_low_ns
operator|>>=
literal|1
expr_stmt|;
name|scale
operator|--
expr_stmt|;
block|}
block|}
comment|/* if QPF worked on initialization, we expect QPC to work as well */
name|QueryPerformanceCounter
argument_list|(
operator|&
name|cnt
argument_list|)
expr_stmt|;
return|return
operator|(
name|high_ns
operator|*
name|cnt
operator|.
name|HighPart
operator|)
operator|+
operator|(
operator|(
name|scaled_low_ns
operator|*
name|cnt
operator|.
name|LowPart
operator|)
operator|>>
name|scale
operator|)
return|;
block|}
end_function

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|highres_nanos
define|#
directive|define
name|highres_nanos
parameter_list|()
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|gettimeofday_nanos
specifier|static
specifier|inline
name|uint64_t
name|gettimeofday_nanos
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|timeval
name|tv
decl_stmt|;
name|gettimeofday
argument_list|(
operator|&
name|tv
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
return|return
operator|(
name|uint64_t
operator|)
name|tv
operator|.
name|tv_sec
operator|*
literal|1000000000
operator|+
name|tv
operator|.
name|tv_usec
operator|*
literal|1000
return|;
block|}
end_function

begin_comment
comment|/*  * Returns nanoseconds since the epoch (01/01/1970), for performance tracing  * (i.e. favoring high precision over wall clock time accuracy).  */
end_comment

begin_function
DECL|function|getnanotime
name|uint64_t
name|getnanotime
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|uint64_t
name|offset
decl_stmt|;
if|if
condition|(
name|offset
operator|>
literal|1
condition|)
block|{
comment|/* initialization succeeded, return offset + high res time */
return|return
name|offset
operator|+
name|highres_nanos
argument_list|()
return|;
block|}
elseif|else
if|if
condition|(
name|offset
operator|==
literal|1
condition|)
block|{
comment|/* initialization failed, fall back to gettimeofday */
return|return
name|gettimeofday_nanos
argument_list|()
return|;
block|}
else|else
block|{
comment|/* initialize offset if high resolution timer works */
name|uint64_t
name|now
init|=
name|gettimeofday_nanos
argument_list|()
decl_stmt|;
name|uint64_t
name|highres
init|=
name|highres_nanos
argument_list|()
decl_stmt|;
if|if
condition|(
name|highres
condition|)
name|offset
operator|=
name|now
operator|-
name|highres
expr_stmt|;
else|else
name|offset
operator|=
literal|1
expr_stmt|;
return|return
name|now
return|;
block|}
block|}
end_function

begin_decl_stmt
DECL|variable|command_start_time
specifier|static
name|uint64_t
name|command_start_time
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|command_line
specifier|static
name|struct
name|strbuf
name|command_line
init|=
name|STRBUF_INIT
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|print_command_performance_atexit
specifier|static
name|void
name|print_command_performance_atexit
parameter_list|(
name|void
parameter_list|)
block|{
name|trace_performance_since
argument_list|(
name|command_start_time
argument_list|,
literal|"git command:%s"
argument_list|,
name|command_line
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|trace_command_performance
name|void
name|trace_command_performance
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
if|if
condition|(
operator|!
name|trace_want
argument_list|(
operator|&
name|trace_perf_key
argument_list|)
condition|)
return|return;
if|if
condition|(
operator|!
name|command_start_time
condition|)
name|atexit
argument_list|(
name|print_command_performance_atexit
argument_list|)
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|command_line
argument_list|)
expr_stmt|;
name|sq_quote_argv
argument_list|(
operator|&
name|command_line
argument_list|,
name|argv
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|command_start_time
operator|=
name|getnanotime
argument_list|()
expr_stmt|;
block|}
end_function

end_unit

