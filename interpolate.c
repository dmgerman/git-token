begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright 2006 Jon Loeliger  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"interpolate.h"
end_include

begin_function
DECL|function|interp_set_entry
name|void
name|interp_set_entry
parameter_list|(
name|struct
name|interp
modifier|*
name|table
parameter_list|,
name|int
name|slot
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
name|char
modifier|*
name|oldval
init|=
name|table
index|[
name|slot
index|]
operator|.
name|value
decl_stmt|;
name|char
modifier|*
name|newval
init|=
name|NULL
decl_stmt|;
if|if
condition|(
name|oldval
condition|)
name|free
argument_list|(
name|oldval
argument_list|)
expr_stmt|;
if|if
condition|(
name|value
condition|)
name|newval
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
name|table
index|[
name|slot
index|]
operator|.
name|value
operator|=
name|newval
expr_stmt|;
block|}
end_function

begin_function
DECL|function|interp_clear_table
name|void
name|interp_clear_table
parameter_list|(
name|struct
name|interp
modifier|*
name|table
parameter_list|,
name|int
name|ninterps
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
name|ninterps
condition|;
name|i
operator|++
control|)
block|{
name|interp_set_entry
argument_list|(
name|table
argument_list|,
name|i
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/*  * Convert a NUL-terminated string in buffer orig  * into the supplied buffer, result, whose length is reslen,  * performing substitutions on %-named sub-strings from  * the table, interps, with ninterps entries.  *  * Example interps:  *    {  *        { "%H", "example.org"},  *        { "%port", "123"},  *        { "%%", "%"},  *    }  *  * Returns 1 on a successful substitution pass that fits in result,  * Returns 0 on a failed or overflowing substitution pass.  */
end_comment

begin_function
DECL|function|interpolate
name|int
name|interpolate
parameter_list|(
name|char
modifier|*
name|result
parameter_list|,
name|int
name|reslen
parameter_list|,
specifier|const
name|char
modifier|*
name|orig
parameter_list|,
specifier|const
name|struct
name|interp
modifier|*
name|interps
parameter_list|,
name|int
name|ninterps
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|src
init|=
name|orig
decl_stmt|;
name|char
modifier|*
name|dest
init|=
name|result
decl_stmt|;
name|int
name|newlen
init|=
literal|0
decl_stmt|;
name|char
modifier|*
name|name
decl_stmt|,
modifier|*
name|value
decl_stmt|;
name|int
name|namelen
decl_stmt|,
name|valuelen
decl_stmt|;
name|int
name|i
decl_stmt|;
name|char
name|c
decl_stmt|;
name|memset
argument_list|(
name|result
argument_list|,
literal|0
argument_list|,
name|reslen
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|c
operator|=
operator|*
name|src
operator|)
operator|&&
name|newlen
operator|<
name|reslen
operator|-
literal|1
condition|)
block|{
if|if
condition|(
name|c
operator|==
literal|'%'
condition|)
block|{
comment|/* Try to match an interpolation string. */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ninterps
condition|;
name|i
operator|++
control|)
block|{
name|name
operator|=
name|interps
index|[
name|i
index|]
operator|.
name|name
expr_stmt|;
name|namelen
operator|=
name|strlen
argument_list|(
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|strncmp
argument_list|(
name|src
argument_list|,
name|name
argument_list|,
name|namelen
argument_list|)
operator|==
literal|0
condition|)
block|{
break|break;
block|}
block|}
comment|/* Check for valid interpolation. */
if|if
condition|(
name|i
operator|<
name|ninterps
condition|)
block|{
name|value
operator|=
name|interps
index|[
name|i
index|]
operator|.
name|value
expr_stmt|;
name|valuelen
operator|=
name|strlen
argument_list|(
name|value
argument_list|)
expr_stmt|;
if|if
condition|(
name|newlen
operator|+
name|valuelen
operator|<
name|reslen
operator|-
literal|1
condition|)
block|{
comment|/* Substitute. */
name|strncpy
argument_list|(
name|dest
argument_list|,
name|value
argument_list|,
name|valuelen
argument_list|)
expr_stmt|;
name|newlen
operator|+=
name|valuelen
expr_stmt|;
name|dest
operator|+=
name|valuelen
expr_stmt|;
name|src
operator|+=
name|namelen
expr_stmt|;
block|}
else|else
block|{
comment|/* Something's not fitting. */
return|return
literal|0
return|;
block|}
block|}
else|else
block|{
comment|/* Skip bogus interpolation. */
operator|*
name|dest
operator|++
operator|=
operator|*
name|src
operator|++
expr_stmt|;
name|newlen
operator|++
expr_stmt|;
block|}
block|}
else|else
block|{
comment|/* Straight copy one non-interpolation character. */
operator|*
name|dest
operator|++
operator|=
operator|*
name|src
operator|++
expr_stmt|;
name|newlen
operator|++
expr_stmt|;
block|}
block|}
return|return
name|newlen
operator|<
name|reslen
operator|-
literal|1
return|;
block|}
end_function

end_unit

