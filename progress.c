begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Simple text-based progress display module for GIT  *  * Copyright (c) 2007 by Nicolas Pitre<nico@cam.org>  *  * This code is free software; you can redistribute it and/or modify  * it under the terms of the GNU General Public License version 2 as  * published by the Free Software Foundation.  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"progress.h"
end_include

begin_define
DECL|macro|TP_IDX_MAX
define|#
directive|define
name|TP_IDX_MAX
value|8
end_define

begin_struct
DECL|struct|throughput
struct|struct
name|throughput
block|{
DECL|member|curr_total
name|off_t
name|curr_total
decl_stmt|;
DECL|member|prev_total
name|off_t
name|prev_total
decl_stmt|;
DECL|member|prev_tv
name|struct
name|timeval
name|prev_tv
decl_stmt|;
DECL|member|avg_bytes
name|unsigned
name|int
name|avg_bytes
decl_stmt|;
DECL|member|avg_misecs
name|unsigned
name|int
name|avg_misecs
decl_stmt|;
DECL|member|last_bytes
name|unsigned
name|int
name|last_bytes
index|[
name|TP_IDX_MAX
index|]
decl_stmt|;
DECL|member|last_misecs
name|unsigned
name|int
name|last_misecs
index|[
name|TP_IDX_MAX
index|]
decl_stmt|;
DECL|member|idx
name|unsigned
name|int
name|idx
decl_stmt|;
DECL|member|display
name|char
name|display
index|[
literal|32
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|progress
struct|struct
name|progress
block|{
DECL|member|title
specifier|const
name|char
modifier|*
name|title
decl_stmt|;
DECL|member|last_value
name|int
name|last_value
decl_stmt|;
DECL|member|total
name|unsigned
name|total
decl_stmt|;
DECL|member|last_percent
name|unsigned
name|last_percent
decl_stmt|;
DECL|member|delay
name|unsigned
name|delay
decl_stmt|;
DECL|member|delayed_percent_treshold
name|unsigned
name|delayed_percent_treshold
decl_stmt|;
DECL|member|throughput
name|struct
name|throughput
modifier|*
name|throughput
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|progress_update
specifier|static
specifier|volatile
name|sig_atomic_t
name|progress_update
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|progress_interval
specifier|static
name|void
name|progress_interval
parameter_list|(
name|int
name|signum
parameter_list|)
block|{
name|progress_update
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|set_progress_signal
specifier|static
name|void
name|set_progress_signal
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|sigaction
name|sa
decl_stmt|;
name|struct
name|itimerval
name|v
decl_stmt|;
name|progress_update
operator|=
literal|0
expr_stmt|;
name|memset
argument_list|(
operator|&
name|sa
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|sa
argument_list|)
argument_list|)
expr_stmt|;
name|sa
operator|.
name|sa_handler
operator|=
name|progress_interval
expr_stmt|;
name|sigemptyset
argument_list|(
operator|&
name|sa
operator|.
name|sa_mask
argument_list|)
expr_stmt|;
name|sa
operator|.
name|sa_flags
operator|=
name|SA_RESTART
expr_stmt|;
name|sigaction
argument_list|(
name|SIGALRM
argument_list|,
operator|&
name|sa
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|v
operator|.
name|it_interval
operator|.
name|tv_sec
operator|=
literal|1
expr_stmt|;
name|v
operator|.
name|it_interval
operator|.
name|tv_usec
operator|=
literal|0
expr_stmt|;
name|v
operator|.
name|it_value
operator|=
name|v
operator|.
name|it_interval
expr_stmt|;
name|setitimer
argument_list|(
name|ITIMER_REAL
argument_list|,
operator|&
name|v
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|clear_progress_signal
specifier|static
name|void
name|clear_progress_signal
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|itimerval
name|v
init|=
block|{
block|{
literal|0
block|,}
block|,}
decl_stmt|;
name|setitimer
argument_list|(
name|ITIMER_REAL
argument_list|,
operator|&
name|v
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|signal
argument_list|(
name|SIGALRM
argument_list|,
name|SIG_IGN
argument_list|)
expr_stmt|;
name|progress_update
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|display
specifier|static
name|int
name|display
parameter_list|(
name|struct
name|progress
modifier|*
name|progress
parameter_list|,
name|unsigned
name|n
parameter_list|,
specifier|const
name|char
modifier|*
name|done
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|eol
decl_stmt|,
modifier|*
name|tp
decl_stmt|;
if|if
condition|(
name|progress
operator|->
name|delay
condition|)
block|{
if|if
condition|(
operator|!
name|progress_update
operator|||
operator|--
name|progress
operator|->
name|delay
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|progress
operator|->
name|total
condition|)
block|{
name|unsigned
name|percent
init|=
name|n
operator|*
literal|100
operator|/
name|progress
operator|->
name|total
decl_stmt|;
if|if
condition|(
name|percent
operator|>
name|progress
operator|->
name|delayed_percent_treshold
condition|)
block|{
comment|/* inhibit this progress report entirely */
name|clear_progress_signal
argument_list|()
expr_stmt|;
name|progress
operator|->
name|delay
operator|=
operator|-
literal|1
expr_stmt|;
name|progress
operator|->
name|total
operator|=
literal|0
expr_stmt|;
return|return
literal|0
return|;
block|}
block|}
block|}
name|progress
operator|->
name|last_value
operator|=
name|n
expr_stmt|;
name|tp
operator|=
operator|(
name|progress
operator|->
name|throughput
operator|)
condition|?
name|progress
operator|->
name|throughput
operator|->
name|display
else|:
literal|""
expr_stmt|;
name|eol
operator|=
name|done
condition|?
name|done
else|:
literal|"   \r"
expr_stmt|;
if|if
condition|(
name|progress
operator|->
name|total
condition|)
block|{
name|unsigned
name|percent
init|=
name|n
operator|*
literal|100
operator|/
name|progress
operator|->
name|total
decl_stmt|;
if|if
condition|(
name|percent
operator|!=
name|progress
operator|->
name|last_percent
operator|||
name|progress_update
condition|)
block|{
name|progress
operator|->
name|last_percent
operator|=
name|percent
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s: %3u%% (%u/%u)%s%s"
argument_list|,
name|progress
operator|->
name|title
argument_list|,
name|percent
argument_list|,
name|n
argument_list|,
name|progress
operator|->
name|total
argument_list|,
name|tp
argument_list|,
name|eol
argument_list|)
expr_stmt|;
name|progress_update
operator|=
literal|0
expr_stmt|;
return|return
literal|1
return|;
block|}
block|}
elseif|else
if|if
condition|(
name|progress_update
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s: %u%s%s"
argument_list|,
name|progress
operator|->
name|title
argument_list|,
name|n
argument_list|,
name|tp
argument_list|,
name|eol
argument_list|)
expr_stmt|;
name|progress_update
operator|=
literal|0
expr_stmt|;
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|throughput_string
specifier|static
name|void
name|throughput_string
parameter_list|(
name|struct
name|throughput
modifier|*
name|tp
parameter_list|,
name|off_t
name|total
parameter_list|,
name|unsigned
name|int
name|rate
parameter_list|)
block|{
name|int
name|l
init|=
sizeof|sizeof
argument_list|(
name|tp
operator|->
name|display
argument_list|)
decl_stmt|;
if|if
condition|(
name|total
operator|>
literal|1
operator|<<
literal|30
condition|)
block|{
name|l
operator|-=
name|snprintf
argument_list|(
name|tp
operator|->
name|display
argument_list|,
name|l
argument_list|,
literal|", %u.%2.2u GiB"
argument_list|,
call|(
name|int
call|)
argument_list|(
name|total
operator|>>
literal|30
argument_list|)
argument_list|,
call|(
name|int
call|)
argument_list|(
name|total
operator|&
operator|(
operator|(
literal|1
operator|<<
literal|30
operator|)
operator|-
literal|1
operator|)
argument_list|)
operator|/
literal|10737419
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|total
operator|>
literal|1
operator|<<
literal|20
condition|)
block|{
name|l
operator|-=
name|snprintf
argument_list|(
name|tp
operator|->
name|display
argument_list|,
name|l
argument_list|,
literal|", %u.%2.2u MiB"
argument_list|,
call|(
name|int
call|)
argument_list|(
name|total
operator|>>
literal|20
argument_list|)
argument_list|,
operator|(
call|(
name|int
call|)
argument_list|(
name|total
operator|&
operator|(
operator|(
literal|1
operator|<<
literal|20
operator|)
operator|-
literal|1
operator|)
argument_list|)
operator|*
literal|100
operator|)
operator|>>
literal|20
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|total
operator|>
literal|1
operator|<<
literal|10
condition|)
block|{
name|l
operator|-=
name|snprintf
argument_list|(
name|tp
operator|->
name|display
argument_list|,
name|l
argument_list|,
literal|", %u.%2.2u KiB"
argument_list|,
call|(
name|int
call|)
argument_list|(
name|total
operator|>>
literal|10
argument_list|)
argument_list|,
operator|(
call|(
name|int
call|)
argument_list|(
name|total
operator|&
operator|(
operator|(
literal|1
operator|<<
literal|10
operator|)
operator|-
literal|1
operator|)
argument_list|)
operator|*
literal|100
operator|)
operator|>>
literal|10
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|l
operator|-=
name|snprintf
argument_list|(
name|tp
operator|->
name|display
argument_list|,
name|l
argument_list|,
literal|", %u bytes"
argument_list|,
operator|(
name|int
operator|)
name|total
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|rate
condition|)
name|snprintf
argument_list|(
name|tp
operator|->
name|display
operator|+
sizeof|sizeof
argument_list|(
name|tp
operator|->
name|display
argument_list|)
operator|-
name|l
argument_list|,
name|l
argument_list|,
literal|" | %u KiB/s"
argument_list|,
name|rate
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|display_throughput
name|void
name|display_throughput
parameter_list|(
name|struct
name|progress
modifier|*
name|progress
parameter_list|,
name|off_t
name|total
parameter_list|)
block|{
name|struct
name|throughput
modifier|*
name|tp
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
name|unsigned
name|int
name|misecs
decl_stmt|;
if|if
condition|(
operator|!
name|progress
condition|)
return|return;
name|tp
operator|=
name|progress
operator|->
name|throughput
expr_stmt|;
name|gettimeofday
argument_list|(
operator|&
name|tv
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tp
condition|)
block|{
name|progress
operator|->
name|throughput
operator|=
name|tp
operator|=
name|calloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|tp
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|tp
condition|)
block|{
name|tp
operator|->
name|prev_total
operator|=
name|tp
operator|->
name|curr_total
operator|=
name|total
expr_stmt|;
name|tp
operator|->
name|prev_tv
operator|=
name|tv
expr_stmt|;
block|}
return|return;
block|}
name|tp
operator|->
name|curr_total
operator|=
name|total
expr_stmt|;
comment|/* 	 * We have x = bytes and y = microsecs.  We want z = KiB/s: 	 * 	 *	z = (x / 1024) / (y / 1000000) 	 *	z = x / y * 1000000 / 1024 	 *	z = x / (y * 1024 / 1000000) 	 *	z = x / y' 	 * 	 * To simplify things we'll keep track of misecs, or 1024th of a sec 	 * obtained with: 	 * 	 *	y' = y * 1024 / 1000000 	 *	y' = y / (1000000 / 1024) 	 *	y' = y / 977 	 */
name|misecs
operator|=
operator|(
name|tv
operator|.
name|tv_sec
operator|-
name|tp
operator|->
name|prev_tv
operator|.
name|tv_sec
operator|)
operator|*
literal|1024
expr_stmt|;
name|misecs
operator|+=
call|(
name|int
call|)
argument_list|(
name|tv
operator|.
name|tv_usec
operator|-
name|tp
operator|->
name|prev_tv
operator|.
name|tv_usec
argument_list|)
operator|/
literal|977
expr_stmt|;
if|if
condition|(
name|misecs
operator|>
literal|512
condition|)
block|{
name|unsigned
name|int
name|count
decl_stmt|,
name|rate
decl_stmt|;
name|count
operator|=
name|total
operator|-
name|tp
operator|->
name|prev_total
expr_stmt|;
name|tp
operator|->
name|prev_total
operator|=
name|total
expr_stmt|;
name|tp
operator|->
name|prev_tv
operator|=
name|tv
expr_stmt|;
name|tp
operator|->
name|avg_bytes
operator|+=
name|count
expr_stmt|;
name|tp
operator|->
name|avg_misecs
operator|+=
name|misecs
expr_stmt|;
name|rate
operator|=
name|tp
operator|->
name|avg_bytes
operator|/
name|tp
operator|->
name|avg_misecs
expr_stmt|;
name|tp
operator|->
name|avg_bytes
operator|-=
name|tp
operator|->
name|last_bytes
index|[
name|tp
operator|->
name|idx
index|]
expr_stmt|;
name|tp
operator|->
name|avg_misecs
operator|-=
name|tp
operator|->
name|last_misecs
index|[
name|tp
operator|->
name|idx
index|]
expr_stmt|;
name|tp
operator|->
name|last_bytes
index|[
name|tp
operator|->
name|idx
index|]
operator|=
name|count
expr_stmt|;
name|tp
operator|->
name|last_misecs
index|[
name|tp
operator|->
name|idx
index|]
operator|=
name|misecs
expr_stmt|;
name|tp
operator|->
name|idx
operator|=
operator|(
name|tp
operator|->
name|idx
operator|+
literal|1
operator|)
operator|%
name|TP_IDX_MAX
expr_stmt|;
name|throughput_string
argument_list|(
name|tp
argument_list|,
name|total
argument_list|,
name|rate
argument_list|)
expr_stmt|;
if|if
condition|(
name|progress
operator|->
name|last_value
operator|!=
operator|-
literal|1
operator|&&
name|progress_update
condition|)
name|display
argument_list|(
name|progress
argument_list|,
name|progress
operator|->
name|last_value
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|display_progress
name|int
name|display_progress
parameter_list|(
name|struct
name|progress
modifier|*
name|progress
parameter_list|,
name|unsigned
name|n
parameter_list|)
block|{
return|return
name|progress
condition|?
name|display
argument_list|(
name|progress
argument_list|,
name|n
argument_list|,
name|NULL
argument_list|)
else|:
literal|0
return|;
block|}
end_function

begin_function
DECL|function|start_progress_delay
name|struct
name|progress
modifier|*
name|start_progress_delay
parameter_list|(
specifier|const
name|char
modifier|*
name|title
parameter_list|,
name|unsigned
name|total
parameter_list|,
name|unsigned
name|percent_treshold
parameter_list|,
name|unsigned
name|delay
parameter_list|)
block|{
name|struct
name|progress
modifier|*
name|progress
init|=
name|malloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|progress
argument_list|)
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|progress
condition|)
block|{
comment|/* unlikely, but here's a good fallback */
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s...\n"
argument_list|,
name|title
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|progress
operator|->
name|title
operator|=
name|title
expr_stmt|;
name|progress
operator|->
name|total
operator|=
name|total
expr_stmt|;
name|progress
operator|->
name|last_value
operator|=
operator|-
literal|1
expr_stmt|;
name|progress
operator|->
name|last_percent
operator|=
operator|-
literal|1
expr_stmt|;
name|progress
operator|->
name|delayed_percent_treshold
operator|=
name|percent_treshold
expr_stmt|;
name|progress
operator|->
name|delay
operator|=
name|delay
expr_stmt|;
name|progress
operator|->
name|throughput
operator|=
name|NULL
expr_stmt|;
name|set_progress_signal
argument_list|()
expr_stmt|;
return|return
name|progress
return|;
block|}
end_function

begin_function
DECL|function|start_progress
name|struct
name|progress
modifier|*
name|start_progress
parameter_list|(
specifier|const
name|char
modifier|*
name|title
parameter_list|,
name|unsigned
name|total
parameter_list|)
block|{
return|return
name|start_progress_delay
argument_list|(
name|title
argument_list|,
name|total
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|stop_progress
name|void
name|stop_progress
parameter_list|(
name|struct
name|progress
modifier|*
modifier|*
name|p_progress
parameter_list|)
block|{
name|stop_progress_msg
argument_list|(
name|p_progress
argument_list|,
literal|"done"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|stop_progress_msg
name|void
name|stop_progress_msg
parameter_list|(
name|struct
name|progress
modifier|*
modifier|*
name|p_progress
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|)
block|{
name|struct
name|progress
modifier|*
name|progress
init|=
operator|*
name|p_progress
decl_stmt|;
if|if
condition|(
operator|!
name|progress
condition|)
return|return;
operator|*
name|p_progress
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|progress
operator|->
name|last_value
operator|!=
operator|-
literal|1
condition|)
block|{
comment|/* Force the last update */
name|char
name|buf
index|[
name|strlen
argument_list|(
name|msg
argument_list|)
operator|+
literal|5
index|]
decl_stmt|;
name|struct
name|throughput
modifier|*
name|tp
init|=
name|progress
operator|->
name|throughput
decl_stmt|;
if|if
condition|(
name|tp
condition|)
block|{
name|unsigned
name|int
name|rate
init|=
operator|!
name|tp
operator|->
name|avg_misecs
condition|?
literal|0
else|:
name|tp
operator|->
name|avg_bytes
operator|/
name|tp
operator|->
name|avg_misecs
decl_stmt|;
name|throughput_string
argument_list|(
name|tp
argument_list|,
name|tp
operator|->
name|curr_total
argument_list|,
name|rate
argument_list|)
expr_stmt|;
block|}
name|progress_update
operator|=
literal|1
expr_stmt|;
name|sprintf
argument_list|(
name|buf
argument_list|,
literal|", %s.\n"
argument_list|,
name|msg
argument_list|)
expr_stmt|;
name|display
argument_list|(
name|progress
argument_list|,
name|progress
operator|->
name|last_value
argument_list|,
name|buf
argument_list|)
expr_stmt|;
block|}
name|clear_progress_signal
argument_list|()
expr_stmt|;
name|free
argument_list|(
name|progress
operator|->
name|throughput
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|progress
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

