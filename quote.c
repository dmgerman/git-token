begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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
comment|/* Help to copy the thing properly quoted for the shell safety.  * any single quote is replaced with '\'', and the caller is  * expected to enclose the result within a single quote pair.  *  * E.g.  *  original     sq_quote     result  *  name     ==> name      ==> 'name'  *  a b      ==> a b       ==> 'a b'  *  a'b      ==> a'\''b    ==> 'a'\''b'  */
end_comment

begin_function
DECL|function|sq_quote
name|char
modifier|*
name|sq_quote
parameter_list|(
specifier|const
name|char
modifier|*
name|src
parameter_list|)
block|{
specifier|static
name|char
modifier|*
name|buf
init|=
name|NULL
decl_stmt|;
name|int
name|cnt
decl_stmt|,
name|c
decl_stmt|;
specifier|const
name|char
modifier|*
name|cp
decl_stmt|;
name|char
modifier|*
name|bp
decl_stmt|;
comment|/* count bytes needed to store the quoted string. */
for|for
control|(
name|cnt
operator|=
literal|3
operator|,
name|cp
operator|=
name|src
init|;
operator|*
name|cp
condition|;
name|cnt
operator|++
operator|,
name|cp
operator|++
control|)
if|if
condition|(
operator|*
name|cp
operator|==
literal|'\''
condition|)
name|cnt
operator|+=
literal|3
expr_stmt|;
name|buf
operator|=
name|xmalloc
argument_list|(
name|cnt
argument_list|)
expr_stmt|;
name|bp
operator|=
name|buf
expr_stmt|;
operator|*
name|bp
operator|++
operator|=
literal|'\''
expr_stmt|;
while|while
condition|(
operator|(
name|c
operator|=
operator|*
name|src
operator|++
operator|)
condition|)
block|{
if|if
condition|(
name|c
operator|!=
literal|'\''
condition|)
operator|*
name|bp
operator|++
operator|=
name|c
expr_stmt|;
else|else
block|{
name|bp
operator|=
name|strcpy
argument_list|(
name|bp
argument_list|,
literal|"'\\''"
argument_list|)
expr_stmt|;
name|bp
operator|+=
literal|4
expr_stmt|;
block|}
block|}
operator|*
name|bp
operator|++
operator|=
literal|'\''
expr_stmt|;
operator|*
name|bp
operator|=
literal|0
expr_stmt|;
return|return
name|buf
return|;
block|}
end_function

begin_comment
comment|/*  * C-style name quoting.  *  * Does one of three things:  *  * (1) if outbuf and outfp are both NULL, inspect the input name and  *     counts the number of bytes that are needed to hold c_style  *     quoted version of name, counting the double quotes around  *     it but not terminating NUL, and returns it.  However, if name  *     does not need c_style quoting, it returns 0.  *  * (2) if outbuf is not NULL, it must point at a buffer large enough  *     to hold the c_style quoted version of name, enclosing double  *     quotes, and terminating NUL.  Fills outbuf with c_style quoted  *     version of name enclosed in double-quote pair.  Return value  *     is undefined.  *  * (3) if outfp is not NULL, outputs c_style quoted version of name,  *     but not enclosed in double-quote pair.  Return value is undefined.  */
end_comment

begin_function
DECL|function|quote_c_style
name|int
name|quote_c_style
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|char
modifier|*
name|outbuf
parameter_list|,
name|FILE
modifier|*
name|outfp
parameter_list|,
name|int
name|no_dq
parameter_list|)
block|{
DECL|macro|EMIT
undef|#
directive|undef
name|EMIT
DECL|macro|EMIT
define|#
directive|define
name|EMIT
parameter_list|(
name|c
parameter_list|)
define|\
value|(outbuf ? (*outbuf++ = (c)) : outfp ? fputc(c, outfp) : (count++))
DECL|macro|EMITQ
define|#
directive|define
name|EMITQ
parameter_list|()
value|EMIT('\\')
specifier|const
name|char
modifier|*
name|sp
decl_stmt|;
name|int
name|ch
decl_stmt|,
name|count
init|=
literal|0
decl_stmt|,
name|needquote
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|no_dq
condition|)
name|EMIT
argument_list|(
literal|'"'
argument_list|)
expr_stmt|;
for|for
control|(
name|sp
operator|=
name|name
init|;
operator|(
name|ch
operator|=
operator|*
name|sp
operator|++
operator|)
condition|;
control|)
block|{
if|if
condition|(
operator|(
name|ch
operator|<=
literal|' '
operator|)
operator|||
operator|(
name|ch
operator|==
literal|'"'
operator|)
operator|||
operator|(
name|ch
operator|==
literal|'\\'
operator|)
operator|||
operator|(
name|ch
operator|==
literal|0177
operator|)
condition|)
block|{
name|needquote
operator|=
literal|1
expr_stmt|;
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'\a'
case|:
name|EMITQ
argument_list|()
expr_stmt|;
name|ch
operator|=
literal|'a'
expr_stmt|;
break|break;
case|case
literal|'\b'
case|:
name|EMITQ
argument_list|()
expr_stmt|;
name|ch
operator|=
literal|'b'
expr_stmt|;
break|break;
case|case
literal|'\f'
case|:
name|EMITQ
argument_list|()
expr_stmt|;
name|ch
operator|=
literal|'f'
expr_stmt|;
break|break;
case|case
literal|'\n'
case|:
name|EMITQ
argument_list|()
expr_stmt|;
name|ch
operator|=
literal|'n'
expr_stmt|;
break|break;
case|case
literal|'\r'
case|:
name|EMITQ
argument_list|()
expr_stmt|;
name|ch
operator|=
literal|'r'
expr_stmt|;
break|break;
case|case
literal|'\t'
case|:
name|EMITQ
argument_list|()
expr_stmt|;
name|ch
operator|=
literal|'t'
expr_stmt|;
break|break;
case|case
literal|'\v'
case|:
name|EMITQ
argument_list|()
expr_stmt|;
name|ch
operator|=
literal|'v'
expr_stmt|;
break|break;
case|case
literal|'\\'
case|:
comment|/* fallthru */
case|case
literal|'"'
case|:
name|EMITQ
argument_list|()
expr_stmt|;
break|break;
case|case
literal|' '
case|:
break|break;
default|default:
comment|/* octal */
name|EMITQ
argument_list|()
expr_stmt|;
name|EMIT
argument_list|(
operator|(
operator|(
name|ch
operator|>>
literal|6
operator|)
operator|&
literal|03
operator|)
operator|+
literal|'0'
argument_list|)
expr_stmt|;
name|EMIT
argument_list|(
operator|(
operator|(
name|ch
operator|>>
literal|3
operator|)
operator|&
literal|07
operator|)
operator|+
literal|'0'
argument_list|)
expr_stmt|;
name|ch
operator|=
operator|(
name|ch
operator|&
literal|07
operator|)
operator|+
literal|'0'
expr_stmt|;
break|break;
block|}
block|}
name|EMIT
argument_list|(
name|ch
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|no_dq
condition|)
name|EMIT
argument_list|(
literal|'"'
argument_list|)
expr_stmt|;
if|if
condition|(
name|outbuf
condition|)
operator|*
name|outbuf
operator|=
literal|0
expr_stmt|;
return|return
name|needquote
condition|?
name|count
else|:
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * C-style name unquoting.  *  * Quoted should point at the opening double quote.  Returns  * an allocated memory that holds unquoted name, which the caller  * should free when done.  Updates endp pointer to point at  * one past the ending double quote if given.  */
end_comment

begin_function
DECL|function|unquote_c_style
name|char
modifier|*
name|unquote_c_style
parameter_list|(
specifier|const
name|char
modifier|*
name|quoted
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|endp
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|sp
decl_stmt|;
name|char
modifier|*
name|name
init|=
name|NULL
decl_stmt|,
modifier|*
name|outp
init|=
name|NULL
decl_stmt|;
name|int
name|count
init|=
literal|0
decl_stmt|,
name|ch
decl_stmt|,
name|ac
decl_stmt|;
DECL|macro|EMIT
undef|#
directive|undef
name|EMIT
DECL|macro|EMIT
define|#
directive|define
name|EMIT
parameter_list|(
name|c
parameter_list|)
value|(outp ? (*outp++ = (c)) : (count++))
if|if
condition|(
operator|*
name|quoted
operator|++
operator|!=
literal|'"'
condition|)
return|return
name|NULL
return|;
while|while
condition|(
literal|1
condition|)
block|{
comment|/* first pass counts and allocates, second pass fills */
for|for
control|(
name|sp
operator|=
name|quoted
init|;
operator|(
name|ch
operator|=
operator|*
name|sp
operator|++
operator|)
operator|!=
literal|'"'
condition|;
control|)
block|{
if|if
condition|(
name|ch
operator|==
literal|'\\'
condition|)
block|{
switch|switch
condition|(
name|ch
operator|=
operator|*
name|sp
operator|++
condition|)
block|{
case|case
literal|'a'
case|:
name|ch
operator|=
literal|'\a'
expr_stmt|;
break|break;
case|case
literal|'b'
case|:
name|ch
operator|=
literal|'\b'
expr_stmt|;
break|break;
case|case
literal|'f'
case|:
name|ch
operator|=
literal|'\f'
expr_stmt|;
break|break;
case|case
literal|'n'
case|:
name|ch
operator|=
literal|'\n'
expr_stmt|;
break|break;
case|case
literal|'r'
case|:
name|ch
operator|=
literal|'\r'
expr_stmt|;
break|break;
case|case
literal|'t'
case|:
name|ch
operator|=
literal|'\t'
expr_stmt|;
break|break;
case|case
literal|'v'
case|:
name|ch
operator|=
literal|'\v'
expr_stmt|;
break|break;
case|case
literal|'\\'
case|:
case|case
literal|'"'
case|:
break|break;
comment|/* verbatim */
case|case
literal|'0'
operator|...
literal|'7'
case|:
comment|/* octal */
name|ac
operator|=
operator|(
operator|(
name|ch
operator|-
literal|'0'
operator|)
operator|<<
literal|6
operator|)
expr_stmt|;
if|if
condition|(
operator|(
name|ch
operator|=
operator|*
name|sp
operator|++
operator|)
operator|<
literal|'0'
operator|||
literal|'7'
operator|<
name|ch
condition|)
return|return
name|NULL
return|;
name|ac
operator||=
operator|(
operator|(
name|ch
operator|-
literal|'0'
operator|)
operator|<<
literal|3
operator|)
expr_stmt|;
if|if
condition|(
operator|(
name|ch
operator|=
operator|*
name|sp
operator|++
operator|)
operator|<
literal|'0'
operator|||
literal|'7'
operator|<
name|ch
condition|)
return|return
name|NULL
return|;
name|ac
operator||=
operator|(
name|ch
operator|-
literal|'0'
operator|)
expr_stmt|;
name|ch
operator|=
name|ac
expr_stmt|;
break|break;
default|default:
return|return
name|NULL
return|;
comment|/* malformed */
block|}
block|}
name|EMIT
argument_list|(
name|ch
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|name
condition|)
block|{
operator|*
name|outp
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|endp
condition|)
operator|*
name|endp
operator|=
name|sp
expr_stmt|;
return|return
name|name
return|;
block|}
name|outp
operator|=
name|name
operator|=
name|xmalloc
argument_list|(
name|count
operator|+
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|write_name_quoted
name|void
name|write_name_quoted
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|quote
parameter_list|,
name|FILE
modifier|*
name|out
parameter_list|)
block|{
name|int
name|needquote
decl_stmt|;
if|if
condition|(
operator|!
name|quote
condition|)
block|{
name|no_quote
label|:
if|if
condition|(
name|prefix
operator|&&
name|prefix
index|[
literal|0
index|]
condition|)
name|fputs
argument_list|(
name|prefix
argument_list|,
name|out
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
name|name
argument_list|,
name|out
argument_list|)
expr_stmt|;
return|return;
block|}
name|needquote
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|prefix
operator|&&
name|prefix
index|[
literal|0
index|]
condition|)
name|needquote
operator|=
name|quote_c_style
argument_list|(
name|prefix
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|needquote
condition|)
name|needquote
operator|=
name|quote_c_style
argument_list|(
name|name
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|needquote
condition|)
block|{
name|fputc
argument_list|(
literal|'"'
argument_list|,
name|out
argument_list|)
expr_stmt|;
if|if
condition|(
name|prefix
operator|&&
name|prefix
index|[
literal|0
index|]
condition|)
name|quote_c_style
argument_list|(
name|prefix
argument_list|,
name|NULL
argument_list|,
name|out
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|quote_c_style
argument_list|(
name|name
argument_list|,
name|NULL
argument_list|,
name|out
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|fputc
argument_list|(
literal|'"'
argument_list|,
name|out
argument_list|)
expr_stmt|;
block|}
else|else
goto|goto
name|no_quote
goto|;
block|}
end_function

end_unit

