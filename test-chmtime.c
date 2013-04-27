begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * This program can either change modification time of the given  * file(s) or just print it. The program does not change atime nor  * ctime (their values are explicitly preserved).  *  * The mtime can be changed to an absolute value:  *  *	test-chmtime =<seconds> file...  *  * Relative to the current time as returned by time(3):  *  *	test-chmtime =+<seconds> (or =-<seconds>) file...  *  * Or relative to the current mtime of the file:  *  *	test-chmtime<seconds> file...  *	test-chmtime +<seconds> (or -<seconds>) file...  *  * Examples:  *  * To just print the mtime use --verbose and set the file mtime offset to 0:  *  *	test-chmtime -v +0 file  *  * To set the mtime to current time:  *  *	test-chmtime =+0 file  *  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|<utime.h>
end_include

begin_decl_stmt
DECL|variable|usage_str
specifier|static
specifier|const
name|char
name|usage_str
index|[]
init|=
literal|"-v|--verbose (+|=|=+|=-|-)<seconds><file>..."
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|timespec_arg
specifier|static
name|int
name|timespec_arg
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|long
name|int
modifier|*
name|set_time
parameter_list|,
name|int
modifier|*
name|set_eq
parameter_list|)
block|{
name|char
modifier|*
name|test
decl_stmt|;
specifier|const
name|char
modifier|*
name|timespec
init|=
name|arg
decl_stmt|;
operator|*
name|set_eq
operator|=
operator|(
operator|*
name|timespec
operator|==
literal|'='
operator|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
if|if
condition|(
operator|*
name|set_eq
condition|)
block|{
name|timespec
operator|++
expr_stmt|;
if|if
condition|(
operator|*
name|timespec
operator|==
literal|'+'
condition|)
block|{
operator|*
name|set_eq
operator|=
literal|2
expr_stmt|;
comment|/* relative "in the future" */
name|timespec
operator|++
expr_stmt|;
block|}
block|}
operator|*
name|set_time
operator|=
name|strtol
argument_list|(
name|timespec
argument_list|,
operator|&
name|test
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|test
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Not a base-10 integer: %s\n"
argument_list|,
name|arg
operator|+
literal|1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|(
operator|*
name|set_eq
operator|&&
operator|*
name|set_time
operator|<
literal|0
operator|)
operator|||
operator|*
name|set_eq
operator|==
literal|2
condition|)
block|{
name|time_t
name|now
init|=
name|time
argument_list|(
name|NULL
argument_list|)
decl_stmt|;
operator|*
name|set_time
operator|+=
name|now
expr_stmt|;
block|}
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
name|argv
index|[]
parameter_list|)
block|{
specifier|static
name|int
name|verbose
decl_stmt|;
name|int
name|i
init|=
literal|1
decl_stmt|;
comment|/* no mtime change by default */
name|int
name|set_eq
init|=
literal|0
decl_stmt|;
name|long
name|int
name|set_time
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|argc
operator|<
literal|3
condition|)
goto|goto
name|usage
goto|;
if|if
condition|(
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"--verbose"
argument_list|)
operator|==
literal|0
operator|||
name|strcmp
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
literal|"-v"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|verbose
operator|=
literal|1
expr_stmt|;
operator|++
name|i
expr_stmt|;
block|}
if|if
condition|(
name|timespec_arg
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
operator|&
name|set_time
argument_list|,
operator|&
name|set_eq
argument_list|)
condition|)
operator|++
name|i
expr_stmt|;
else|else
goto|goto
name|usage
goto|;
for|for
control|(
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|stat
name|sb
decl_stmt|;
name|struct
name|utimbuf
name|utb
decl_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
operator|&
name|sb
argument_list|)
operator|<
literal|0
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Failed to stat %s: %s\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
ifdef|#
directive|ifdef
name|WIN32
if|if
condition|(
operator|!
operator|(
name|sb
operator|.
name|st_mode
operator|&
name|S_IWUSR
operator|)
operator|&&
name|chmod
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
name|sb
operator|.
name|st_mode
operator||
name|S_IWUSR
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Could not make user-writable %s: %s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
endif|#
directive|endif
name|utb
operator|.
name|actime
operator|=
name|sb
operator|.
name|st_atime
expr_stmt|;
name|utb
operator|.
name|modtime
operator|=
name|set_eq
condition|?
name|set_time
else|:
name|sb
operator|.
name|st_mtime
operator|+
name|set_time
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
block|{
name|uintmax_t
name|mtime
init|=
name|utb
operator|.
name|modtime
operator|<
literal|0
condition|?
literal|0
else|:
name|utb
operator|.
name|modtime
decl_stmt|;
name|printf
argument_list|(
literal|"%"
name|PRIuMAX
literal|"\t%s\n"
argument_list|,
name|mtime
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|utb
operator|.
name|modtime
operator|!=
name|sb
operator|.
name|st_mtime
operator|&&
name|utime
argument_list|(
name|argv
index|[
name|i
index|]
argument_list|,
operator|&
name|utb
argument_list|)
operator|<
literal|0
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Failed to modify time on %s: %s\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
block|}
return|return
literal|0
return|;
name|usage
label|:
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"usage: %s %s\n"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|,
name|usage_str
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

end_unit

