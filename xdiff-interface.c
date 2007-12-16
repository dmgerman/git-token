begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"xdiff-interface.h"
end_include

begin_function
DECL|function|parse_num
specifier|static
name|int
name|parse_num
parameter_list|(
name|char
modifier|*
modifier|*
name|cp_p
parameter_list|,
name|int
modifier|*
name|num_p
parameter_list|)
block|{
name|char
modifier|*
name|cp
init|=
operator|*
name|cp_p
decl_stmt|;
name|int
name|num
init|=
literal|0
decl_stmt|;
name|int
name|read_some
decl_stmt|;
while|while
condition|(
literal|'0'
operator|<=
operator|*
name|cp
operator|&&
operator|*
name|cp
operator|<=
literal|'9'
condition|)
name|num
operator|=
name|num
operator|*
literal|10
operator|+
operator|*
name|cp
operator|++
operator|-
literal|'0'
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|read_some
operator|=
name|cp
operator|-
operator|*
name|cp_p
operator|)
condition|)
return|return
operator|-
literal|1
return|;
operator|*
name|cp_p
operator|=
name|cp
expr_stmt|;
operator|*
name|num_p
operator|=
name|num
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|parse_hunk_header
name|int
name|parse_hunk_header
parameter_list|(
name|char
modifier|*
name|line
parameter_list|,
name|int
name|len
parameter_list|,
name|int
modifier|*
name|ob
parameter_list|,
name|int
modifier|*
name|on
parameter_list|,
name|int
modifier|*
name|nb
parameter_list|,
name|int
modifier|*
name|nn
parameter_list|)
block|{
name|char
modifier|*
name|cp
decl_stmt|;
name|cp
operator|=
name|line
operator|+
literal|4
expr_stmt|;
if|if
condition|(
name|parse_num
argument_list|(
operator|&
name|cp
argument_list|,
name|ob
argument_list|)
condition|)
block|{
name|bad_line
label|:
return|return
name|error
argument_list|(
literal|"malformed diff output: %s"
argument_list|,
name|line
argument_list|)
return|;
block|}
if|if
condition|(
operator|*
name|cp
operator|==
literal|','
condition|)
block|{
name|cp
operator|++
expr_stmt|;
if|if
condition|(
name|parse_num
argument_list|(
operator|&
name|cp
argument_list|,
name|on
argument_list|)
condition|)
goto|goto
name|bad_line
goto|;
block|}
else|else
operator|*
name|on
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|*
name|cp
operator|++
operator|!=
literal|' '
operator|||
operator|*
name|cp
operator|++
operator|!=
literal|'+'
condition|)
goto|goto
name|bad_line
goto|;
if|if
condition|(
name|parse_num
argument_list|(
operator|&
name|cp
argument_list|,
name|nb
argument_list|)
condition|)
goto|goto
name|bad_line
goto|;
if|if
condition|(
operator|*
name|cp
operator|==
literal|','
condition|)
block|{
name|cp
operator|++
expr_stmt|;
if|if
condition|(
name|parse_num
argument_list|(
operator|&
name|cp
argument_list|,
name|nn
argument_list|)
condition|)
goto|goto
name|bad_line
goto|;
block|}
else|else
operator|*
name|nn
operator|=
literal|1
expr_stmt|;
return|return
operator|-
operator|!
operator|!
name|memcmp
argument_list|(
name|cp
argument_list|,
literal|" @@"
argument_list|,
literal|3
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|consume_one
specifier|static
name|void
name|consume_one
parameter_list|(
name|void
modifier|*
name|priv_
parameter_list|,
name|char
modifier|*
name|s
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|struct
name|xdiff_emit_state
modifier|*
name|priv
init|=
name|priv_
decl_stmt|;
name|char
modifier|*
name|ep
decl_stmt|;
while|while
condition|(
name|size
condition|)
block|{
name|unsigned
name|long
name|this_size
decl_stmt|;
name|ep
operator|=
name|memchr
argument_list|(
name|s
argument_list|,
literal|'\n'
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|this_size
operator|=
operator|(
name|ep
operator|==
name|NULL
operator|)
condition|?
name|size
else|:
operator|(
name|ep
operator|-
name|s
operator|+
literal|1
operator|)
expr_stmt|;
name|priv
operator|->
name|consume
argument_list|(
name|priv
argument_list|,
name|s
argument_list|,
name|this_size
argument_list|)
expr_stmt|;
name|size
operator|-=
name|this_size
expr_stmt|;
name|s
operator|+=
name|this_size
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|xdiff_outf
name|int
name|xdiff_outf
parameter_list|(
name|void
modifier|*
name|priv_
parameter_list|,
name|mmbuffer_t
modifier|*
name|mb
parameter_list|,
name|int
name|nbuf
parameter_list|)
block|{
name|struct
name|xdiff_emit_state
modifier|*
name|priv
init|=
name|priv_
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
name|nbuf
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|mb
index|[
name|i
index|]
operator|.
name|ptr
index|[
name|mb
index|[
name|i
index|]
operator|.
name|size
operator|-
literal|1
index|]
operator|!=
literal|'\n'
condition|)
block|{
comment|/* Incomplete line */
name|priv
operator|->
name|remainder
operator|=
name|xrealloc
argument_list|(
name|priv
operator|->
name|remainder
argument_list|,
name|priv
operator|->
name|remainder_size
operator|+
name|mb
index|[
name|i
index|]
operator|.
name|size
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|priv
operator|->
name|remainder
operator|+
name|priv
operator|->
name|remainder_size
argument_list|,
name|mb
index|[
name|i
index|]
operator|.
name|ptr
argument_list|,
name|mb
index|[
name|i
index|]
operator|.
name|size
argument_list|)
expr_stmt|;
name|priv
operator|->
name|remainder_size
operator|+=
name|mb
index|[
name|i
index|]
operator|.
name|size
expr_stmt|;
continue|continue;
block|}
comment|/* we have a complete line */
if|if
condition|(
operator|!
name|priv
operator|->
name|remainder
condition|)
block|{
name|consume_one
argument_list|(
name|priv
argument_list|,
name|mb
index|[
name|i
index|]
operator|.
name|ptr
argument_list|,
name|mb
index|[
name|i
index|]
operator|.
name|size
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|priv
operator|->
name|remainder
operator|=
name|xrealloc
argument_list|(
name|priv
operator|->
name|remainder
argument_list|,
name|priv
operator|->
name|remainder_size
operator|+
name|mb
index|[
name|i
index|]
operator|.
name|size
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|priv
operator|->
name|remainder
operator|+
name|priv
operator|->
name|remainder_size
argument_list|,
name|mb
index|[
name|i
index|]
operator|.
name|ptr
argument_list|,
name|mb
index|[
name|i
index|]
operator|.
name|size
argument_list|)
expr_stmt|;
name|consume_one
argument_list|(
name|priv
argument_list|,
name|priv
operator|->
name|remainder
argument_list|,
name|priv
operator|->
name|remainder_size
operator|+
name|mb
index|[
name|i
index|]
operator|.
name|size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|priv
operator|->
name|remainder
argument_list|)
expr_stmt|;
name|priv
operator|->
name|remainder
operator|=
name|NULL
expr_stmt|;
name|priv
operator|->
name|remainder_size
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|priv
operator|->
name|remainder
condition|)
block|{
name|consume_one
argument_list|(
name|priv
argument_list|,
name|priv
operator|->
name|remainder
argument_list|,
name|priv
operator|->
name|remainder_size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|priv
operator|->
name|remainder
argument_list|)
expr_stmt|;
name|priv
operator|->
name|remainder
operator|=
name|NULL
expr_stmt|;
name|priv
operator|->
name|remainder_size
operator|=
literal|0
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * Trim down common substring at the end of the buffers,  * but leave at least ctx lines at the end.  */
end_comment

begin_function
DECL|function|trim_common_tail
specifier|static
name|void
name|trim_common_tail
parameter_list|(
name|mmfile_t
modifier|*
name|a
parameter_list|,
name|mmfile_t
modifier|*
name|b
parameter_list|,
name|long
name|ctx
parameter_list|)
block|{
specifier|const
name|int
name|blk
init|=
literal|1024
decl_stmt|;
name|long
name|trimmed
init|=
literal|0
decl_stmt|,
name|recovered
init|=
literal|0
decl_stmt|;
name|char
modifier|*
name|ap
init|=
name|a
operator|->
name|ptr
operator|+
name|a
operator|->
name|size
decl_stmt|;
name|char
modifier|*
name|bp
init|=
name|b
operator|->
name|ptr
operator|+
name|b
operator|->
name|size
decl_stmt|;
name|long
name|smaller
init|=
operator|(
name|a
operator|->
name|size
operator|<
name|b
operator|->
name|size
operator|)
condition|?
name|a
operator|->
name|size
else|:
name|b
operator|->
name|size
decl_stmt|;
while|while
condition|(
name|blk
operator|+
name|trimmed
operator|<=
name|smaller
operator|&&
operator|!
name|memcmp
argument_list|(
name|ap
operator|-
name|blk
argument_list|,
name|bp
operator|-
name|blk
argument_list|,
name|blk
argument_list|)
condition|)
block|{
name|trimmed
operator|+=
name|blk
expr_stmt|;
name|ap
operator|-=
name|blk
expr_stmt|;
name|bp
operator|-=
name|blk
expr_stmt|;
block|}
while|while
condition|(
name|recovered
operator|<
name|trimmed
operator|&&
literal|0
operator|<=
name|ctx
condition|)
if|if
condition|(
name|ap
index|[
name|recovered
operator|++
index|]
operator|==
literal|'\n'
condition|)
name|ctx
operator|--
expr_stmt|;
name|a
operator|->
name|size
operator|-=
operator|(
name|trimmed
operator|-
name|recovered
operator|)
expr_stmt|;
name|b
operator|->
name|size
operator|-=
operator|(
name|trimmed
operator|-
name|recovered
operator|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|xdi_diff
name|int
name|xdi_diff
parameter_list|(
name|mmfile_t
modifier|*
name|mf1
parameter_list|,
name|mmfile_t
modifier|*
name|mf2
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|xdemitconf_t
specifier|const
modifier|*
name|xecfg
parameter_list|,
name|xdemitcb_t
modifier|*
name|xecb
parameter_list|)
block|{
name|mmfile_t
name|a
init|=
operator|*
name|mf1
decl_stmt|;
name|mmfile_t
name|b
init|=
operator|*
name|mf2
decl_stmt|;
name|trim_common_tail
argument_list|(
operator|&
name|a
argument_list|,
operator|&
name|b
argument_list|,
name|xecfg
operator|->
name|ctxlen
argument_list|)
expr_stmt|;
return|return
name|xdl_diff
argument_list|(
operator|&
name|a
argument_list|,
operator|&
name|b
argument_list|,
name|xpp
argument_list|,
name|xecfg
argument_list|,
name|xecb
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|read_mmfile
name|int
name|read_mmfile
parameter_list|(
name|mmfile_t
modifier|*
name|ptr
parameter_list|,
specifier|const
name|char
modifier|*
name|filename
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|FILE
modifier|*
name|f
decl_stmt|;
name|size_t
name|sz
decl_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|filename
argument_list|,
operator|&
name|st
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"Could not stat %s"
argument_list|,
name|filename
argument_list|)
return|;
if|if
condition|(
operator|(
name|f
operator|=
name|fopen
argument_list|(
name|filename
argument_list|,
literal|"rb"
argument_list|)
operator|)
operator|==
name|NULL
condition|)
return|return
name|error
argument_list|(
literal|"Could not open %s"
argument_list|,
name|filename
argument_list|)
return|;
name|sz
operator|=
name|xsize_t
argument_list|(
name|st
operator|.
name|st_size
argument_list|)
expr_stmt|;
name|ptr
operator|->
name|ptr
operator|=
name|xmalloc
argument_list|(
name|sz
argument_list|)
expr_stmt|;
if|if
condition|(
name|fread
argument_list|(
name|ptr
operator|->
name|ptr
argument_list|,
name|sz
argument_list|,
literal|1
argument_list|,
name|f
argument_list|)
operator|!=
literal|1
condition|)
return|return
name|error
argument_list|(
literal|"Could not read %s"
argument_list|,
name|filename
argument_list|)
return|;
name|fclose
argument_list|(
name|f
argument_list|)
expr_stmt|;
name|ptr
operator|->
name|size
operator|=
name|sz
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_define
DECL|macro|FIRST_FEW_BYTES
define|#
directive|define
name|FIRST_FEW_BYTES
value|8000
end_define

begin_function
DECL|function|buffer_is_binary
name|int
name|buffer_is_binary
parameter_list|(
specifier|const
name|char
modifier|*
name|ptr
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
if|if
condition|(
name|FIRST_FEW_BYTES
operator|<
name|size
condition|)
name|size
operator|=
name|FIRST_FEW_BYTES
expr_stmt|;
return|return
operator|!
operator|!
name|memchr
argument_list|(
name|ptr
argument_list|,
literal|0
argument_list|,
name|size
argument_list|)
return|;
block|}
end_function

begin_struct
DECL|struct|ff_regs
struct|struct
name|ff_regs
block|{
DECL|member|nr
name|int
name|nr
decl_stmt|;
DECL|struct|ff_reg
struct|struct
name|ff_reg
block|{
DECL|member|re
name|regex_t
name|re
decl_stmt|;
DECL|member|negate
name|int
name|negate
decl_stmt|;
DECL|member|array
block|}
modifier|*
name|array
struct|;
block|}
struct|;
end_struct

begin_function
DECL|function|ff_regexp
specifier|static
name|long
name|ff_regexp
parameter_list|(
specifier|const
name|char
modifier|*
name|line
parameter_list|,
name|long
name|len
parameter_list|,
name|char
modifier|*
name|buffer
parameter_list|,
name|long
name|buffer_size
parameter_list|,
name|void
modifier|*
name|priv
parameter_list|)
block|{
name|char
modifier|*
name|line_buffer
init|=
name|xstrndup
argument_list|(
name|line
argument_list|,
name|len
argument_list|)
decl_stmt|;
comment|/* make NUL terminated */
name|struct
name|ff_regs
modifier|*
name|regs
init|=
name|priv
decl_stmt|;
name|regmatch_t
name|pmatch
index|[
literal|2
index|]
decl_stmt|;
name|int
name|result
init|=
literal|0
decl_stmt|,
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
name|regs
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|ff_reg
modifier|*
name|reg
init|=
name|regs
operator|->
name|array
operator|+
name|i
decl_stmt|;
if|if
condition|(
name|reg
operator|->
name|negate
operator|^
operator|!
operator|!
name|regexec
argument_list|(
operator|&
name|reg
operator|->
name|re
argument_list|,
name|line_buffer
argument_list|,
literal|2
argument_list|,
name|pmatch
argument_list|,
literal|0
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|line_buffer
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
block|}
name|i
operator|=
name|pmatch
index|[
literal|1
index|]
operator|.
name|rm_so
operator|>=
literal|0
condition|?
literal|1
else|:
literal|0
expr_stmt|;
name|line
operator|+=
name|pmatch
index|[
name|i
index|]
operator|.
name|rm_so
expr_stmt|;
name|result
operator|=
name|pmatch
index|[
name|i
index|]
operator|.
name|rm_eo
operator|-
name|pmatch
index|[
name|i
index|]
operator|.
name|rm_so
expr_stmt|;
if|if
condition|(
name|result
operator|>
name|buffer_size
condition|)
name|result
operator|=
name|buffer_size
expr_stmt|;
else|else
while|while
condition|(
name|result
operator|>
literal|0
operator|&&
operator|(
name|isspace
argument_list|(
name|line
index|[
name|result
operator|-
literal|1
index|]
argument_list|)
operator|||
name|line
index|[
name|result
operator|-
literal|1
index|]
operator|==
literal|'\n'
operator|)
condition|)
name|result
operator|--
expr_stmt|;
name|memcpy
argument_list|(
name|buffer
argument_list|,
name|line
argument_list|,
name|result
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|line_buffer
argument_list|)
expr_stmt|;
return|return
name|result
return|;
block|}
end_function

begin_function
DECL|function|xdiff_set_find_func
name|void
name|xdiff_set_find_func
parameter_list|(
name|xdemitconf_t
modifier|*
name|xecfg
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|struct
name|ff_regs
modifier|*
name|regs
decl_stmt|;
name|xecfg
operator|->
name|find_func
operator|=
name|ff_regexp
expr_stmt|;
name|regs
operator|=
name|xecfg
operator|->
name|find_func_priv
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|ff_regs
argument_list|)
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
operator|,
name|regs
operator|->
name|nr
operator|=
literal|1
init|;
name|value
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|value
index|[
name|i
index|]
operator|==
literal|'\n'
condition|)
name|regs
operator|->
name|nr
operator|++
expr_stmt|;
name|regs
operator|->
name|array
operator|=
name|xmalloc
argument_list|(
name|regs
operator|->
name|nr
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|ff_reg
argument_list|)
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
name|regs
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|ff_reg
modifier|*
name|reg
init|=
name|regs
operator|->
name|array
operator|+
name|i
decl_stmt|;
specifier|const
name|char
modifier|*
name|ep
init|=
name|strchr
argument_list|(
name|value
argument_list|,
literal|'\n'
argument_list|)
decl_stmt|,
modifier|*
name|expression
decl_stmt|;
name|char
modifier|*
name|buffer
init|=
name|NULL
decl_stmt|;
name|reg
operator|->
name|negate
operator|=
operator|(
operator|*
name|value
operator|==
literal|'!'
operator|)
expr_stmt|;
if|if
condition|(
name|reg
operator|->
name|negate
operator|&&
name|i
operator|==
name|regs
operator|->
name|nr
operator|-
literal|1
condition|)
name|die
argument_list|(
literal|"Last expression must not be negated: %s"
argument_list|,
name|value
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|value
operator|==
literal|'!'
condition|)
name|value
operator|++
expr_stmt|;
if|if
condition|(
name|ep
condition|)
name|expression
operator|=
name|buffer
operator|=
name|xstrndup
argument_list|(
name|value
argument_list|,
name|ep
operator|-
name|value
argument_list|)
expr_stmt|;
else|else
name|expression
operator|=
name|value
expr_stmt|;
if|if
condition|(
name|regcomp
argument_list|(
operator|&
name|reg
operator|->
name|re
argument_list|,
name|expression
argument_list|,
literal|0
argument_list|)
condition|)
name|die
argument_list|(
literal|"Invalid regexp to look for hunk header: %s"
argument_list|,
name|expression
argument_list|)
expr_stmt|;
if|if
condition|(
name|buffer
condition|)
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
name|value
operator|=
name|ep
operator|+
literal|1
expr_stmt|;
block|}
block|}
end_function

end_unit

