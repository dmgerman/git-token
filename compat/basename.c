begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"../git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"../strbuf.h"
end_include

begin_comment
comment|/* Adapted from libiberty's basename.c.  */
end_comment

begin_function
DECL|function|gitbasename
name|char
modifier|*
name|gitbasename
parameter_list|(
name|char
modifier|*
name|path
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|base
decl_stmt|;
if|if
condition|(
name|path
condition|)
name|skip_dos_drive_prefix
argument_list|(
operator|&
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|path
operator|||
operator|!
operator|*
name|path
condition|)
return|return
literal|"."
return|;
for|for
control|(
name|base
operator|=
name|path
init|;
operator|*
name|path
condition|;
name|path
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|is_dir_sep
argument_list|(
operator|*
name|path
argument_list|)
condition|)
continue|continue;
do|do
block|{
name|path
operator|++
expr_stmt|;
block|}
do|while
condition|(
name|is_dir_sep
argument_list|(
operator|*
name|path
argument_list|)
condition|)
do|;
if|if
condition|(
operator|*
name|path
condition|)
name|base
operator|=
name|path
expr_stmt|;
else|else
while|while
condition|(
operator|--
name|path
operator|!=
name|base
operator|&&
name|is_dir_sep
argument_list|(
operator|*
name|path
argument_list|)
condition|)
operator|*
name|path
operator|=
literal|'\0'
expr_stmt|;
block|}
return|return
operator|(
name|char
operator|*
operator|)
name|base
return|;
block|}
end_function

begin_function
DECL|function|gitdirname
name|char
modifier|*
name|gitdirname
parameter_list|(
name|char
modifier|*
name|path
parameter_list|)
block|{
specifier|static
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|char
modifier|*
name|p
init|=
name|path
decl_stmt|,
modifier|*
name|slash
init|=
name|NULL
decl_stmt|,
name|c
decl_stmt|;
name|int
name|dos_drive_prefix
decl_stmt|;
if|if
condition|(
operator|!
name|p
condition|)
return|return
literal|"."
return|;
if|if
condition|(
operator|(
name|dos_drive_prefix
operator|=
name|skip_dos_drive_prefix
argument_list|(
operator|&
name|p
argument_list|)
operator|)
operator|&&
operator|!
operator|*
name|p
condition|)
goto|goto
name|dot
goto|;
comment|/* 	 * POSIX.1-2001 says dirname("/") should return "/", and dirname("//") 	 * should return "//", but dirname("///") should return "/" again. 	 */
if|if
condition|(
name|is_dir_sep
argument_list|(
operator|*
name|p
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|p
index|[
literal|1
index|]
operator|||
operator|(
name|is_dir_sep
argument_list|(
name|p
index|[
literal|1
index|]
argument_list|)
operator|&&
operator|!
name|p
index|[
literal|2
index|]
operator|)
condition|)
return|return
name|path
return|;
name|slash
operator|=
operator|++
name|p
expr_stmt|;
block|}
while|while
condition|(
operator|(
name|c
operator|=
operator|*
operator|(
name|p
operator|++
operator|)
operator|)
condition|)
if|if
condition|(
name|is_dir_sep
argument_list|(
name|c
argument_list|)
condition|)
block|{
name|char
modifier|*
name|tentative
init|=
name|p
operator|-
literal|1
decl_stmt|;
comment|/* POSIX.1-2001 says to ignore trailing slashes */
while|while
condition|(
name|is_dir_sep
argument_list|(
operator|*
name|p
argument_list|)
condition|)
name|p
operator|++
expr_stmt|;
if|if
condition|(
operator|*
name|p
condition|)
name|slash
operator|=
name|tentative
expr_stmt|;
block|}
if|if
condition|(
name|slash
condition|)
block|{
operator|*
name|slash
operator|=
literal|'\0'
expr_stmt|;
return|return
name|path
return|;
block|}
name|dot
label|:
name|strbuf_reset
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"%.*s."
argument_list|,
name|dos_drive_prefix
argument_list|,
name|path
argument_list|)
expr_stmt|;
return|return
name|buf
operator|.
name|buf
return|;
block|}
end_function

end_unit

