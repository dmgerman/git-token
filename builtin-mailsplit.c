begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Totally braindamaged mbox splitter program.  *  * It just splits a mbox into a list of files: "0001" "0002" ..  * so you can process them further from there.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_decl_stmt
DECL|variable|git_mailsplit_usage
specifier|static
specifier|const
name|char
name|git_mailsplit_usage
index|[]
init|=
literal|"git-mailsplit [-d<prec>] [-f<n>] [-b] -o<directory><mbox>..."
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|is_from_line
specifier|static
name|int
name|is_from_line
parameter_list|(
specifier|const
name|char
modifier|*
name|line
parameter_list|,
name|int
name|len
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|colon
decl_stmt|;
if|if
condition|(
name|len
operator|<
literal|20
operator|||
name|memcmp
argument_list|(
literal|"From "
argument_list|,
name|line
argument_list|,
literal|5
argument_list|)
condition|)
return|return
literal|0
return|;
name|colon
operator|=
name|line
operator|+
name|len
operator|-
literal|2
expr_stmt|;
name|line
operator|+=
literal|5
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|colon
operator|<
name|line
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|*
operator|--
name|colon
operator|==
literal|':'
condition|)
break|break;
block|}
if|if
condition|(
operator|!
name|isdigit
argument_list|(
name|colon
index|[
operator|-
literal|4
index|]
argument_list|)
operator|||
operator|!
name|isdigit
argument_list|(
name|colon
index|[
operator|-
literal|2
index|]
argument_list|)
operator|||
operator|!
name|isdigit
argument_list|(
name|colon
index|[
operator|-
literal|1
index|]
argument_list|)
operator|||
operator|!
name|isdigit
argument_list|(
name|colon
index|[
literal|1
index|]
argument_list|)
operator|||
operator|!
name|isdigit
argument_list|(
name|colon
index|[
literal|2
index|]
argument_list|)
condition|)
return|return
literal|0
return|;
comment|/* year */
if|if
condition|(
name|strtol
argument_list|(
name|colon
operator|+
literal|3
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
operator|<=
literal|90
condition|)
return|return
literal|0
return|;
comment|/* Ok, close enough */
return|return
literal|1
return|;
block|}
end_function

begin_comment
comment|/* Could be as small as 64, enough to hold a Unix "From " line. */
end_comment

begin_decl_stmt
DECL|variable|buf
specifier|static
name|char
name|buf
index|[
literal|4096
index|]
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Called with the first line (potentially partial)  * already in buf[] -- normally that should begin with  * the Unix "From " line.  Write it into the specified  * file.  */
end_comment

begin_function
DECL|function|split_one
specifier|static
name|int
name|split_one
parameter_list|(
name|FILE
modifier|*
name|mbox
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|allow_bare
parameter_list|)
block|{
name|FILE
modifier|*
name|output
init|=
name|NULL
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|buf
argument_list|)
decl_stmt|;
name|int
name|fd
decl_stmt|;
name|int
name|status
init|=
literal|0
decl_stmt|;
name|int
name|is_bare
init|=
operator|!
name|is_from_line
argument_list|(
name|buf
argument_list|,
name|len
argument_list|)
decl_stmt|;
if|if
condition|(
name|is_bare
operator|&&
operator|!
name|allow_bare
condition|)
goto|goto
name|corrupt
goto|;
name|fd
operator|=
name|open
argument_list|(
name|name
argument_list|,
name|O_WRONLY
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"cannot open output file %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|output
operator|=
name|fdopen
argument_list|(
name|fd
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
comment|/* Copy it out, while searching for a line that begins with 	 * "From " and having something that looks like a date format. 	 */
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|is_partial
init|=
operator|(
name|buf
index|[
name|len
operator|-
literal|1
index|]
operator|!=
literal|'\n'
operator|)
decl_stmt|;
if|if
condition|(
name|fputs
argument_list|(
name|buf
argument_list|,
name|output
argument_list|)
operator|==
name|EOF
condition|)
name|die
argument_list|(
literal|"cannot write output"
argument_list|)
expr_stmt|;
if|if
condition|(
name|fgets
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
name|mbox
argument_list|)
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|feof
argument_list|(
name|mbox
argument_list|)
condition|)
block|{
name|status
operator|=
literal|1
expr_stmt|;
break|break;
block|}
name|die
argument_list|(
literal|"cannot read mbox"
argument_list|)
expr_stmt|;
block|}
name|len
operator|=
name|strlen
argument_list|(
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_partial
operator|&&
operator|!
name|is_bare
operator|&&
name|is_from_line
argument_list|(
name|buf
argument_list|,
name|len
argument_list|)
condition|)
break|break;
comment|/* done with one message */
block|}
name|fclose
argument_list|(
name|output
argument_list|)
expr_stmt|;
return|return
name|status
return|;
name|corrupt
label|:
if|if
condition|(
name|output
condition|)
name|fclose
argument_list|(
name|output
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|name
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"corrupt mailbox\n"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|split_mbox
name|int
name|split_mbox
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|mbox
parameter_list|,
specifier|const
name|char
modifier|*
name|dir
parameter_list|,
name|int
name|allow_bare
parameter_list|,
name|int
name|nr_prec
parameter_list|,
name|int
name|skip
parameter_list|)
block|{
name|char
modifier|*
name|name
init|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|dir
argument_list|)
operator|+
literal|2
operator|+
literal|3
operator|*
sizeof|sizeof
argument_list|(
name|skip
argument_list|)
argument_list|)
decl_stmt|;
name|int
name|ret
init|=
operator|-
literal|1
decl_stmt|;
while|while
condition|(
operator|*
name|mbox
condition|)
block|{
specifier|const
name|char
modifier|*
name|file
init|=
operator|*
name|mbox
operator|++
decl_stmt|;
name|FILE
modifier|*
name|f
init|=
operator|!
name|strcmp
argument_list|(
name|file
argument_list|,
literal|"-"
argument_list|)
condition|?
name|stdin
else|:
name|fopen
argument_list|(
name|file
argument_list|,
literal|"r"
argument_list|)
decl_stmt|;
name|int
name|file_done
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|f
condition|)
block|{
name|error
argument_list|(
literal|"cannot open mbox %s"
argument_list|,
name|file
argument_list|)
expr_stmt|;
goto|goto
name|out
goto|;
block|}
if|if
condition|(
name|fgets
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
name|f
argument_list|)
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|f
operator|==
name|stdin
condition|)
break|break;
comment|/* empty stdin is OK */
name|error
argument_list|(
literal|"cannot read mbox %s"
argument_list|,
name|file
argument_list|)
expr_stmt|;
goto|goto
name|out
goto|;
block|}
while|while
condition|(
operator|!
name|file_done
condition|)
block|{
name|sprintf
argument_list|(
name|name
argument_list|,
literal|"%s/%0*d"
argument_list|,
name|dir
argument_list|,
name|nr_prec
argument_list|,
operator|++
name|skip
argument_list|)
expr_stmt|;
name|file_done
operator|=
name|split_one
argument_list|(
name|f
argument_list|,
name|name
argument_list|,
name|allow_bare
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|f
operator|!=
name|stdin
condition|)
name|fclose
argument_list|(
name|f
argument_list|)
expr_stmt|;
block|}
name|ret
operator|=
name|skip
expr_stmt|;
name|out
label|:
name|free
argument_list|(
name|name
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|cmd_mailsplit
name|int
name|cmd_mailsplit
parameter_list|(
name|int
name|argc
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
name|prefix
parameter_list|)
block|{
name|int
name|nr
init|=
literal|0
decl_stmt|,
name|nr_prec
init|=
literal|4
decl_stmt|,
name|ret
decl_stmt|;
name|int
name|allow_bare
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|dir
init|=
name|NULL
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|argp
decl_stmt|;
specifier|static
specifier|const
name|char
modifier|*
name|stdin_only
index|[]
init|=
block|{
literal|"-"
block|,
name|NULL
block|}
decl_stmt|;
for|for
control|(
name|argp
operator|=
name|argv
operator|+
literal|1
init|;
operator|*
name|argp
condition|;
name|argp
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
operator|*
name|argp
decl_stmt|;
if|if
condition|(
name|arg
index|[
literal|0
index|]
operator|!=
literal|'-'
condition|)
break|break;
comment|/* do flags here */
if|if
condition|(
name|arg
index|[
literal|1
index|]
operator|==
literal|'d'
condition|)
block|{
name|nr_prec
operator|=
name|strtol
argument_list|(
name|arg
operator|+
literal|2
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|nr_prec
operator|<
literal|3
operator|||
literal|10
operator|<=
name|nr_prec
condition|)
name|usage
argument_list|(
name|git_mailsplit_usage
argument_list|)
expr_stmt|;
continue|continue;
block|}
elseif|else
if|if
condition|(
name|arg
index|[
literal|1
index|]
operator|==
literal|'f'
condition|)
block|{
name|nr
operator|=
name|strtol
argument_list|(
name|arg
operator|+
literal|2
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|arg
index|[
literal|1
index|]
operator|==
literal|'b'
operator|&&
operator|!
name|arg
index|[
literal|2
index|]
condition|)
block|{
name|allow_bare
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|arg
index|[
literal|1
index|]
operator|==
literal|'o'
operator|&&
name|arg
index|[
literal|2
index|]
condition|)
block|{
name|dir
operator|=
name|arg
operator|+
literal|2
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|arg
index|[
literal|1
index|]
operator|==
literal|'-'
operator|&&
operator|!
name|arg
index|[
literal|2
index|]
condition|)
block|{
name|argp
operator|++
expr_stmt|;
comment|/* -- marks end of options */
break|break;
block|}
else|else
block|{
name|die
argument_list|(
literal|"unknown option: %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|dir
condition|)
block|{
comment|/* Backwards compatibility: if no -o specified, accept<mbox><dir> or just<dir> */
switch|switch
condition|(
name|argc
operator|-
operator|(
name|argp
operator|-
name|argv
operator|)
condition|)
block|{
case|case
literal|1
case|:
name|dir
operator|=
name|argp
index|[
literal|0
index|]
expr_stmt|;
name|argp
operator|=
name|stdin_only
expr_stmt|;
break|break;
case|case
literal|2
case|:
name|stdin_only
index|[
literal|0
index|]
operator|=
name|argp
index|[
literal|0
index|]
expr_stmt|;
name|dir
operator|=
name|argp
index|[
literal|1
index|]
expr_stmt|;
name|argp
operator|=
name|stdin_only
expr_stmt|;
break|break;
default|default:
name|usage
argument_list|(
name|git_mailsplit_usage
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
comment|/* New usage: if no more argument, parse stdin */
if|if
condition|(
operator|!
operator|*
name|argp
condition|)
name|argp
operator|=
name|stdin_only
expr_stmt|;
block|}
name|ret
operator|=
name|split_mbox
argument_list|(
name|argp
argument_list|,
name|dir
argument_list|,
name|allow_bare
argument_list|,
name|nr_prec
argument_list|,
name|nr
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|!=
operator|-
literal|1
condition|)
name|printf
argument_list|(
literal|"%d\n"
argument_list|,
name|ret
argument_list|)
expr_stmt|;
return|return
name|ret
operator|==
operator|-
literal|1
return|;
block|}
end_function

end_unit

