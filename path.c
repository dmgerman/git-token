begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * I'm tired of doing "vsnprintf()" etc just to open a  * file, so here's a "return static buffer with printf"  * interface for paths.  *  * It's obviously not thread-safe. Sue me. But it's quite  * useful for doing things like  *  *   f = open(mkpath("%s/%s.git", base, name), O_RDONLY);  *  * which is what it's designed for.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|<pwd.h>
end_include

begin_decl_stmt
DECL|variable|pathname
specifier|static
name|char
name|pathname
index|[
name|PATH_MAX
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|bad_path
specifier|static
name|char
name|bad_path
index|[]
init|=
literal|"/bad-path/"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cleanup_path
specifier|static
name|char
modifier|*
name|cleanup_path
parameter_list|(
name|char
modifier|*
name|path
parameter_list|)
block|{
comment|/* Clean it up */
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|path
argument_list|,
literal|"./"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
name|path
operator|+=
literal|2
expr_stmt|;
while|while
condition|(
operator|*
name|path
operator|==
literal|'/'
condition|)
name|path
operator|++
expr_stmt|;
block|}
return|return
name|path
return|;
block|}
end_function

begin_function
DECL|function|mkpath
name|char
modifier|*
name|mkpath
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
name|args
decl_stmt|;
name|unsigned
name|len
decl_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|len
operator|=
name|vsnprintf
argument_list|(
name|pathname
argument_list|,
name|PATH_MAX
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>=
name|PATH_MAX
condition|)
return|return
name|bad_path
return|;
return|return
name|cleanup_path
argument_list|(
name|pathname
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|git_path
name|char
modifier|*
name|git_path
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|git_dir
init|=
name|get_git_dir
argument_list|()
decl_stmt|;
name|va_list
name|args
decl_stmt|;
name|unsigned
name|len
decl_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|git_dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>
name|PATH_MAX
operator|-
literal|100
condition|)
return|return
name|bad_path
return|;
name|memcpy
argument_list|(
name|pathname
argument_list|,
name|git_dir
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|&&
name|git_dir
index|[
name|len
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
name|pathname
index|[
name|len
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|len
operator|+=
name|vsnprintf
argument_list|(
name|pathname
operator|+
name|len
argument_list|,
name|PATH_MAX
operator|-
name|len
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>=
name|PATH_MAX
condition|)
return|return
name|bad_path
return|;
return|return
name|cleanup_path
argument_list|(
name|pathname
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* git_mkstemp() - create tmp file honoring TMPDIR variable */
end_comment

begin_function
DECL|function|git_mkstemp
name|int
name|git_mkstemp
parameter_list|(
name|char
modifier|*
name|path
parameter_list|,
name|size_t
name|len
parameter_list|,
specifier|const
name|char
modifier|*
name|template
parameter_list|)
block|{
name|char
modifier|*
name|env
decl_stmt|,
modifier|*
name|pch
init|=
name|path
decl_stmt|;
if|if
condition|(
operator|(
name|env
operator|=
name|getenv
argument_list|(
literal|"TMPDIR"
argument_list|)
operator|)
operator|==
name|NULL
condition|)
block|{
name|strcpy
argument_list|(
name|pch
argument_list|,
literal|"/tmp/"
argument_list|)
expr_stmt|;
name|len
operator|-=
literal|5
expr_stmt|;
name|pch
operator|+=
literal|5
expr_stmt|;
block|}
else|else
block|{
name|size_t
name|n
init|=
name|snprintf
argument_list|(
name|pch
argument_list|,
name|len
argument_list|,
literal|"%s/"
argument_list|,
name|env
argument_list|)
decl_stmt|;
name|len
operator|-=
name|n
expr_stmt|;
name|pch
operator|+=
name|n
expr_stmt|;
block|}
name|safe_strncpy
argument_list|(
name|pch
argument_list|,
name|template
argument_list|,
name|len
argument_list|)
expr_stmt|;
return|return
name|mkstemp
argument_list|(
name|path
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|safe_strncpy
name|char
modifier|*
name|safe_strncpy
parameter_list|(
name|char
modifier|*
name|dest
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|,
name|size_t
name|n
parameter_list|)
block|{
name|strncpy
argument_list|(
name|dest
argument_list|,
name|src
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|dest
index|[
name|n
operator|-
literal|1
index|]
operator|=
literal|'\0'
expr_stmt|;
return|return
name|dest
return|;
block|}
end_function

begin_function
DECL|function|validate_symref
name|int
name|validate_symref
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|,
name|buffer
index|[
literal|256
index|]
decl_stmt|;
name|int
name|len
decl_stmt|,
name|fd
decl_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|path
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
comment|/* Make sure it is a "refs/.." symlink */
if|if
condition|(
name|S_ISLNK
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
name|len
operator|=
name|readlink
argument_list|(
name|path
argument_list|,
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>=
literal|5
operator|&&
operator|!
name|memcmp
argument_list|(
literal|"refs/"
argument_list|,
name|buffer
argument_list|,
literal|5
argument_list|)
condition|)
return|return
literal|0
return|;
return|return
operator|-
literal|1
return|;
block|}
comment|/* 	 * Anything else, just open it and try to see if it is a symbolic ref. 	 */
name|fd
operator|=
name|open
argument_list|(
name|path
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
name|len
operator|=
name|read
argument_list|(
name|fd
argument_list|,
name|buffer
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
comment|/* 	 * Is it a symbolic ref? 	 */
if|if
condition|(
name|len
operator|<
literal|4
operator|||
name|memcmp
argument_list|(
literal|"ref:"
argument_list|,
name|buffer
argument_list|,
literal|4
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|buf
operator|=
name|buffer
operator|+
literal|4
expr_stmt|;
name|len
operator|-=
literal|4
expr_stmt|;
while|while
condition|(
name|len
operator|&&
name|isspace
argument_list|(
operator|*
name|buf
argument_list|)
condition|)
name|buf
operator|++
operator|,
name|len
operator|--
expr_stmt|;
if|if
condition|(
name|len
operator|>=
literal|5
operator|&&
operator|!
name|memcmp
argument_list|(
literal|"refs/"
argument_list|,
name|buf
argument_list|,
literal|5
argument_list|)
condition|)
return|return
literal|0
return|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|current_dir
specifier|static
name|char
modifier|*
name|current_dir
parameter_list|(
name|void
parameter_list|)
block|{
return|return
name|getcwd
argument_list|(
name|pathname
argument_list|,
sizeof|sizeof
argument_list|(
name|pathname
argument_list|)
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|user_chdir
specifier|static
name|int
name|user_chdir
parameter_list|(
name|char
modifier|*
name|path
parameter_list|)
block|{
name|char
modifier|*
name|dir
init|=
name|path
decl_stmt|;
if|if
condition|(
operator|*
name|dir
operator|==
literal|'~'
condition|)
block|{
comment|/* user-relative path */
name|struct
name|passwd
modifier|*
name|pw
decl_stmt|;
name|char
modifier|*
name|slash
init|=
name|strchr
argument_list|(
name|dir
argument_list|,
literal|'/'
argument_list|)
decl_stmt|;
name|dir
operator|++
expr_stmt|;
comment|/* '~/' and '~' (no slash) means users own home-dir */
if|if
condition|(
operator|!
operator|*
name|dir
operator|||
operator|*
name|dir
operator|==
literal|'/'
condition|)
name|pw
operator|=
name|getpwuid
argument_list|(
name|getuid
argument_list|()
argument_list|)
expr_stmt|;
else|else
block|{
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
name|pw
operator|=
name|getpwnam
argument_list|(
name|dir
argument_list|)
expr_stmt|;
operator|*
name|slash
operator|=
literal|'/'
expr_stmt|;
block|}
else|else
name|pw
operator|=
name|getpwnam
argument_list|(
name|dir
argument_list|)
expr_stmt|;
block|}
comment|/* make sure we got something back that we can chdir() to */
if|if
condition|(
operator|!
name|pw
operator|||
name|chdir
argument_list|(
name|pw
operator|->
name|pw_dir
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|slash
operator|||
operator|!
name|slash
index|[
literal|1
index|]
condition|)
comment|/* no path following username */
return|return
literal|0
return|;
name|dir
operator|=
name|slash
operator|+
literal|1
expr_stmt|;
block|}
comment|/* ~foo/path/to/repo is now path/to/repo and we're in foo's homedir */
if|if
condition|(
name|chdir
argument_list|(
name|dir
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|enter_repo
name|char
modifier|*
name|enter_repo
parameter_list|(
name|char
modifier|*
name|path
parameter_list|,
name|int
name|strict
parameter_list|)
block|{
if|if
condition|(
operator|!
name|path
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|strict
condition|)
block|{
if|if
condition|(
operator|(
name|path
index|[
literal|0
index|]
operator|!=
literal|'/'
operator|)
operator|||
name|chdir
argument_list|(
name|path
argument_list|)
operator|<
literal|0
condition|)
return|return
name|NULL
return|;
block|}
else|else
block|{
if|if
condition|(
operator|!
operator|*
name|path
condition|)
empty_stmt|;
comment|/* happy -- no chdir */
elseif|else
if|if
condition|(
operator|!
name|user_chdir
argument_list|(
name|path
argument_list|)
condition|)
empty_stmt|;
comment|/* happy -- as given */
elseif|else
if|if
condition|(
operator|!
name|user_chdir
argument_list|(
name|mkpath
argument_list|(
literal|"%s.git"
argument_list|,
name|path
argument_list|)
argument_list|)
condition|)
empty_stmt|;
comment|/* happy -- uemacs --> uemacs.git */
else|else
return|return
name|NULL
return|;
operator|(
name|void
operator|)
name|chdir
argument_list|(
literal|".git"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|access
argument_list|(
literal|"objects"
argument_list|,
name|X_OK
argument_list|)
operator|==
literal|0
operator|&&
name|access
argument_list|(
literal|"refs"
argument_list|,
name|X_OK
argument_list|)
operator|==
literal|0
operator|&&
name|validate_symref
argument_list|(
literal|"HEAD"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|putenv
argument_list|(
literal|"GIT_DIR=."
argument_list|)
expr_stmt|;
return|return
name|current_dir
argument_list|()
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

end_unit

