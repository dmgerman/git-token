begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  *  The order of the following two lines is important.  *  *  FREAD_READS_DIRECTORIES is undefined before including git-compat-util.h  *  to avoid the redefinition of fopen within git-compat-util.h. This is  *  necessary since fopen is a macro on some platforms which may be set  *  based on compiler options. For example, on AIX fopen is set to fopen64  *  when _LARGE_FILES is defined. The previous technique of merely undefining  *  fopen after including git-compat-util.h is inadequate in this case.  */
end_comment

begin_undef
DECL|macro|FREAD_READS_DIRECTORIES
undef|#
directive|undef
name|FREAD_READS_DIRECTORIES
end_undef

begin_include
include|#
directive|include
file|"../git-compat-util.h"
end_include

begin_function
DECL|function|git_fopen
name|FILE
modifier|*
name|git_fopen
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|mode
parameter_list|)
block|{
name|FILE
modifier|*
name|fp
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
if|if
condition|(
name|mode
index|[
literal|0
index|]
operator|==
literal|'w'
operator|||
name|mode
index|[
literal|0
index|]
operator|==
literal|'a'
condition|)
return|return
name|fopen
argument_list|(
name|path
argument_list|,
name|mode
argument_list|)
return|;
if|if
condition|(
operator|!
operator|(
name|fp
operator|=
name|fopen
argument_list|(
name|path
argument_list|,
name|mode
argument_list|)
operator|)
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|fstat
argument_list|(
name|fileno
argument_list|(
name|fp
argument_list|)
argument_list|,
operator|&
name|st
argument_list|)
condition|)
block|{
name|fclose
argument_list|(
name|fp
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
if|if
condition|(
name|S_ISDIR
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
name|fclose
argument_list|(
name|fp
argument_list|)
expr_stmt|;
name|errno
operator|=
name|EISDIR
expr_stmt|;
return|return
name|NULL
return|;
block|}
return|return
name|fp
return|;
block|}
end_function

end_unit

