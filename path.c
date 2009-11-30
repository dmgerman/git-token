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
file|"strbuf.h"
end_include

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
DECL|function|get_pathname
specifier|static
name|char
modifier|*
name|get_pathname
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|char
name|pathname_array
index|[
literal|4
index|]
index|[
name|PATH_MAX
index|]
decl_stmt|;
specifier|static
name|int
name|index
decl_stmt|;
return|return
name|pathname_array
index|[
literal|3
operator|&
operator|++
name|index
index|]
return|;
block|}
end_function

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
DECL|function|mksnpath
name|char
modifier|*
name|mksnpath
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|n
parameter_list|,
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
name|buf
argument_list|,
name|n
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
name|n
condition|)
block|{
name|strlcpy
argument_list|(
name|buf
argument_list|,
name|bad_path
argument_list|,
name|n
argument_list|)
expr_stmt|;
return|return
name|buf
return|;
block|}
return|return
name|cleanup_path
argument_list|(
name|buf
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|git_vsnpath
specifier|static
name|char
modifier|*
name|git_vsnpath
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|n
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
name|va_list
name|args
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
name|size_t
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
name|n
operator|<
name|len
operator|+
literal|1
condition|)
goto|goto
name|bad
goto|;
name|memcpy
argument_list|(
name|buf
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
operator|!
name|is_dir_sep
argument_list|(
name|git_dir
index|[
name|len
operator|-
literal|1
index|]
argument_list|)
condition|)
name|buf
index|[
name|len
operator|++
index|]
operator|=
literal|'/'
expr_stmt|;
name|len
operator|+=
name|vsnprintf
argument_list|(
name|buf
operator|+
name|len
argument_list|,
name|n
operator|-
name|len
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>=
name|n
condition|)
goto|goto
name|bad
goto|;
return|return
name|cleanup_path
argument_list|(
name|buf
argument_list|)
return|;
name|bad
label|:
name|strlcpy
argument_list|(
name|buf
argument_list|,
name|bad_path
argument_list|,
name|n
argument_list|)
expr_stmt|;
return|return
name|buf
return|;
block|}
end_function

begin_function
DECL|function|git_snpath
name|char
modifier|*
name|git_snpath
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|n
parameter_list|,
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
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
operator|(
name|void
operator|)
name|git_vsnpath
argument_list|(
name|buf
argument_list|,
name|n
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
return|return
name|buf
return|;
block|}
end_function

begin_function
DECL|function|git_pathdup
name|char
modifier|*
name|git_pathdup
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|char
name|path
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|va_list
name|args
decl_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
operator|(
name|void
operator|)
name|git_vsnpath
argument_list|(
name|path
argument_list|,
sizeof|sizeof
argument_list|(
name|path
argument_list|)
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
return|return
name|xstrdup
argument_list|(
name|path
argument_list|)
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
name|char
modifier|*
name|pathname
init|=
name|get_pathname
argument_list|()
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
name|char
modifier|*
name|pathname
init|=
name|get_pathname
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
specifier|const
name|char
modifier|*
name|tmp
decl_stmt|;
name|size_t
name|n
decl_stmt|;
name|tmp
operator|=
name|getenv
argument_list|(
literal|"TMPDIR"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tmp
condition|)
name|tmp
operator|=
literal|"/tmp"
expr_stmt|;
name|n
operator|=
name|snprintf
argument_list|(
name|path
argument_list|,
name|len
argument_list|,
literal|"%s/%s"
argument_list|,
name|tmp
argument_list|,
name|template
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|<=
name|n
condition|)
block|{
name|errno
operator|=
name|ENAMETOOLONG
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
return|return
name|mkstemp
argument_list|(
name|path
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/* git_mkstemps() - create tmp file with suffix honoring TMPDIR variable. */
end_comment

begin_function
DECL|function|git_mkstemps
name|int
name|git_mkstemps
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
parameter_list|,
name|int
name|suffix_len
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|tmp
decl_stmt|;
name|size_t
name|n
decl_stmt|;
name|tmp
operator|=
name|getenv
argument_list|(
literal|"TMPDIR"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tmp
condition|)
name|tmp
operator|=
literal|"/tmp"
expr_stmt|;
name|n
operator|=
name|snprintf
argument_list|(
name|path
argument_list|,
name|len
argument_list|,
literal|"%s/%s"
argument_list|,
name|tmp
argument_list|,
name|template
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|<=
name|n
condition|)
block|{
name|errno
operator|=
name|ENAMETOOLONG
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
return|return
name|mkstemps
argument_list|(
name|path
argument_list|,
name|suffix_len
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|validate_headref
name|int
name|validate_headref
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
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|fd
decl_stmt|;
name|ssize_t
name|len
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
name|read_in_full
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
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|memcmp
argument_list|(
literal|"ref:"
argument_list|,
name|buffer
argument_list|,
literal|4
argument_list|)
condition|)
block|{
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
block|}
comment|/* 	 * Is this a detached HEAD? 	 */
if|if
condition|(
operator|!
name|get_sha1_hex
argument_list|(
name|buffer
argument_list|,
name|sha1
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
DECL|function|getpw_str
specifier|static
name|struct
name|passwd
modifier|*
name|getpw_str
parameter_list|(
specifier|const
name|char
modifier|*
name|username
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|struct
name|passwd
modifier|*
name|pw
decl_stmt|;
name|char
modifier|*
name|username_z
init|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|1
argument_list|)
decl_stmt|;
name|memcpy
argument_list|(
name|username_z
argument_list|,
name|username
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|username_z
index|[
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
name|pw
operator|=
name|getpwnam
argument_list|(
name|username_z
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|username_z
argument_list|)
expr_stmt|;
return|return
name|pw
return|;
block|}
end_function

begin_comment
comment|/*  * Return a string with ~ and ~user expanded via getpw*.  If buf != NULL,  * then it is a newly allocated string. Returns NULL on getpw failure or  * if path is NULL.  */
end_comment

begin_function
DECL|function|expand_user_path
name|char
modifier|*
name|expand_user_path
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|struct
name|strbuf
name|user_path
init|=
name|STRBUF_INIT
decl_stmt|;
specifier|const
name|char
modifier|*
name|first_slash
init|=
name|strchrnul
argument_list|(
name|path
argument_list|,
literal|'/'
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|to_copy
init|=
name|path
decl_stmt|;
if|if
condition|(
name|path
operator|==
name|NULL
condition|)
goto|goto
name|return_null
goto|;
if|if
condition|(
name|path
index|[
literal|0
index|]
operator|==
literal|'~'
condition|)
block|{
specifier|const
name|char
modifier|*
name|username
init|=
name|path
operator|+
literal|1
decl_stmt|;
name|size_t
name|username_len
init|=
name|first_slash
operator|-
name|username
decl_stmt|;
if|if
condition|(
name|username_len
operator|==
literal|0
condition|)
block|{
specifier|const
name|char
modifier|*
name|home
init|=
name|getenv
argument_list|(
literal|"HOME"
argument_list|)
decl_stmt|;
name|strbuf_add
argument_list|(
operator|&
name|user_path
argument_list|,
name|home
argument_list|,
name|strlen
argument_list|(
name|home
argument_list|)
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|struct
name|passwd
modifier|*
name|pw
init|=
name|getpw_str
argument_list|(
name|username
argument_list|,
name|username_len
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|pw
condition|)
goto|goto
name|return_null
goto|;
name|strbuf_add
argument_list|(
operator|&
name|user_path
argument_list|,
name|pw
operator|->
name|pw_dir
argument_list|,
name|strlen
argument_list|(
name|pw
operator|->
name|pw_dir
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|to_copy
operator|=
name|first_slash
expr_stmt|;
block|}
name|strbuf_add
argument_list|(
operator|&
name|user_path
argument_list|,
name|to_copy
argument_list|,
name|strlen
argument_list|(
name|to_copy
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|strbuf_detach
argument_list|(
operator|&
name|user_path
argument_list|,
name|NULL
argument_list|)
return|;
name|return_null
label|:
name|strbuf_release
argument_list|(
operator|&
name|user_path
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
end_function

begin_comment
comment|/*  * First, one directory to try is determined by the following algorithm.  *  * (0) If "strict" is given, the path is used as given and no DWIM is  *     done. Otherwise:  * (1) "~/path" to mean path under the running user's home directory;  * (2) "~user/path" to mean path under named user's home directory;  * (3) "relative/path" to mean cwd relative directory; or  * (4) "/absolute/path" to mean absolute directory.  *  * Unless "strict" is given, we try access() for existence of "%s.git/.git",  * "%s/.git", "%s.git", "%s" in this order.  The first one that exists is  * what we try.  *  * Second, we try chdir() to that.  Upon failure, we return NULL.  *  * Then, we try if the current directory is a valid git repository.  * Upon failure, we return NULL.  *  * If all goes well, we return the directory we used to chdir() (but  * before ~user is expanded), avoiding getcwd() resolving symbolic  * links.  User relative paths are also returned as they are given,  * except DWIM suffixing.  */
end_comment

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
specifier|static
name|char
name|used_path
index|[
name|PATH_MAX
index|]
decl_stmt|;
specifier|static
name|char
name|validated_path
index|[
name|PATH_MAX
index|]
decl_stmt|;
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
operator|!
name|strict
condition|)
block|{
specifier|static
specifier|const
name|char
modifier|*
name|suffix
index|[]
init|=
block|{
literal|".git/.git"
block|,
literal|"/.git"
block|,
literal|".git"
block|,
literal|""
block|,
name|NULL
block|, 		}
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|path
argument_list|)
decl_stmt|;
name|int
name|i
decl_stmt|;
while|while
condition|(
operator|(
literal|1
operator|<
name|len
operator|)
operator|&&
operator|(
name|path
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'/'
operator|)
condition|)
block|{
name|path
index|[
name|len
operator|-
literal|1
index|]
operator|=
literal|0
expr_stmt|;
name|len
operator|--
expr_stmt|;
block|}
if|if
condition|(
name|PATH_MAX
operator|<=
name|len
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|path
index|[
literal|0
index|]
operator|==
literal|'~'
condition|)
block|{
name|char
modifier|*
name|newpath
init|=
name|expand_user_path
argument_list|(
name|path
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|newpath
operator|||
operator|(
name|PATH_MAX
operator|-
literal|10
operator|<
name|strlen
argument_list|(
name|newpath
argument_list|)
operator|)
condition|)
block|{
name|free
argument_list|(
name|newpath
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
comment|/* 			 * Copy back into the static buffer. A pity 			 * since newpath was not bounded, but other 			 * branches of the if are limited by PATH_MAX 			 * anyway. 			 */
name|strcpy
argument_list|(
name|used_path
argument_list|,
name|newpath
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|newpath
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|validated_path
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|path
operator|=
name|used_path
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|PATH_MAX
operator|-
literal|10
operator|<
name|len
condition|)
return|return
name|NULL
return|;
else|else
block|{
name|path
operator|=
name|strcpy
argument_list|(
name|used_path
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|validated_path
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
name|len
operator|=
name|strlen
argument_list|(
name|path
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|suffix
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
name|strcpy
argument_list|(
name|path
operator|+
name|len
argument_list|,
name|suffix
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|access
argument_list|(
name|path
argument_list|,
name|F_OK
argument_list|)
condition|)
block|{
name|strcat
argument_list|(
name|validated_path
argument_list|,
name|suffix
index|[
name|i
index|]
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
operator|!
name|suffix
index|[
name|i
index|]
operator|||
name|chdir
argument_list|(
name|path
argument_list|)
condition|)
return|return
name|NULL
return|;
name|path
operator|=
name|validated_path
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|chdir
argument_list|(
name|path
argument_list|)
condition|)
return|return
name|NULL
return|;
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
name|validate_headref
argument_list|(
literal|"HEAD"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|setenv
argument_list|(
name|GIT_DIR_ENVIRONMENT
argument_list|,
literal|"."
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|check_repository_format
argument_list|()
expr_stmt|;
return|return
name|path
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|set_shared_perm
name|int
name|set_shared_perm
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|mode
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|int
name|tweak
decl_stmt|,
name|shared
decl_stmt|,
name|orig_mode
decl_stmt|;
if|if
condition|(
operator|!
name|shared_repository
condition|)
block|{
if|if
condition|(
name|mode
condition|)
return|return
name|chmod
argument_list|(
name|path
argument_list|,
name|mode
operator|&
operator|~
name|S_IFMT
argument_list|)
return|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|mode
condition|)
block|{
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
name|mode
operator|=
name|st
operator|.
name|st_mode
expr_stmt|;
name|orig_mode
operator|=
name|mode
expr_stmt|;
block|}
else|else
name|orig_mode
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|shared_repository
operator|<
literal|0
condition|)
name|shared
operator|=
operator|-
name|shared_repository
expr_stmt|;
else|else
name|shared
operator|=
name|shared_repository
expr_stmt|;
name|tweak
operator|=
name|shared
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|mode
operator|&
name|S_IWUSR
operator|)
condition|)
name|tweak
operator|&=
operator|~
literal|0222
expr_stmt|;
if|if
condition|(
name|mode
operator|&
name|S_IXUSR
condition|)
comment|/* Copy read bits to execute bits */
name|tweak
operator||=
operator|(
name|tweak
operator|&
literal|0444
operator|)
operator|>>
literal|2
expr_stmt|;
if|if
condition|(
name|shared_repository
operator|<
literal|0
condition|)
name|mode
operator|=
operator|(
name|mode
operator|&
operator|~
literal|0777
operator|)
operator||
name|tweak
expr_stmt|;
else|else
name|mode
operator||=
name|tweak
expr_stmt|;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
condition|)
block|{
comment|/* Copy read bits to execute bits */
name|mode
operator||=
operator|(
name|shared
operator|&
literal|0444
operator|)
operator|>>
literal|2
expr_stmt|;
name|mode
operator||=
name|FORCE_DIR_SET_GID
expr_stmt|;
block|}
if|if
condition|(
operator|(
operator|(
name|shared_repository
operator|<
literal|0
condition|?
operator|(
name|orig_mode
operator|&
operator|(
name|FORCE_DIR_SET_GID
operator||
literal|0777
operator|)
operator|)
else|:
operator|(
name|orig_mode
operator|&
name|mode
operator|)
operator|)
operator|!=
name|mode
operator|)
operator|&&
name|chmod
argument_list|(
name|path
argument_list|,
operator|(
name|mode
operator|&
operator|~
name|S_IFMT
operator|)
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|2
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|make_relative_path
specifier|const
name|char
modifier|*
name|make_relative_path
parameter_list|(
specifier|const
name|char
modifier|*
name|abs
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|)
block|{
specifier|static
name|char
name|buf
index|[
name|PATH_MAX
operator|+
literal|1
index|]
decl_stmt|;
name|int
name|baselen
decl_stmt|;
if|if
condition|(
operator|!
name|base
condition|)
return|return
name|abs
return|;
name|baselen
operator|=
name|strlen
argument_list|(
name|base
argument_list|)
expr_stmt|;
if|if
condition|(
name|prefixcmp
argument_list|(
name|abs
argument_list|,
name|base
argument_list|)
condition|)
return|return
name|abs
return|;
if|if
condition|(
name|abs
index|[
name|baselen
index|]
operator|==
literal|'/'
condition|)
name|baselen
operator|++
expr_stmt|;
elseif|else
if|if
condition|(
name|base
index|[
name|baselen
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
return|return
name|abs
return|;
name|strcpy
argument_list|(
name|buf
argument_list|,
name|abs
operator|+
name|baselen
argument_list|)
expr_stmt|;
return|return
name|buf
return|;
block|}
end_function

begin_comment
comment|/*  * It is okay if dst == src, but they should not overlap otherwise.  *  * Performs the following normalizations on src, storing the result in dst:  * - Ensures that components are separated by '/' (Windows only)  * - Squashes sequences of '/'.  * - Removes "." components.  * - Removes ".." components, and the components the precede them.  * Returns failure (non-zero) if a ".." component appears as first path  * component anytime during the normalization. Otherwise, returns success (0).  *  * Note that this function is purely textual.  It does not follow symlinks,  * verify the existence of the path, or make any system calls.  */
end_comment

begin_function
DECL|function|normalize_path_copy
name|int
name|normalize_path_copy
parameter_list|(
name|char
modifier|*
name|dst
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|)
block|{
name|char
modifier|*
name|dst0
decl_stmt|;
if|if
condition|(
name|has_dos_drive_prefix
argument_list|(
name|src
argument_list|)
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
operator|*
name|src
operator|++
expr_stmt|;
operator|*
name|dst
operator|++
operator|=
operator|*
name|src
operator|++
expr_stmt|;
block|}
name|dst0
operator|=
name|dst
expr_stmt|;
if|if
condition|(
name|is_dir_sep
argument_list|(
operator|*
name|src
argument_list|)
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
literal|'/'
expr_stmt|;
while|while
condition|(
name|is_dir_sep
argument_list|(
operator|*
name|src
argument_list|)
condition|)
name|src
operator|++
expr_stmt|;
block|}
for|for
control|(
init|;
condition|;
control|)
block|{
name|char
name|c
init|=
operator|*
name|src
decl_stmt|;
comment|/* 		 * A path component that begins with . could be 		 * special: 		 * (1) "." and ends   -- ignore and terminate. 		 * (2) "./"           -- ignore them, eat slash and continue. 		 * (3) ".." and ends  -- strip one and terminate. 		 * (4) "../"          -- strip one, eat slash and continue. 		 */
if|if
condition|(
name|c
operator|==
literal|'.'
condition|)
block|{
if|if
condition|(
operator|!
name|src
index|[
literal|1
index|]
condition|)
block|{
comment|/* (1) */
name|src
operator|++
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|is_dir_sep
argument_list|(
name|src
index|[
literal|1
index|]
argument_list|)
condition|)
block|{
comment|/* (2) */
name|src
operator|+=
literal|2
expr_stmt|;
while|while
condition|(
name|is_dir_sep
argument_list|(
operator|*
name|src
argument_list|)
condition|)
name|src
operator|++
expr_stmt|;
continue|continue;
block|}
elseif|else
if|if
condition|(
name|src
index|[
literal|1
index|]
operator|==
literal|'.'
condition|)
block|{
if|if
condition|(
operator|!
name|src
index|[
literal|2
index|]
condition|)
block|{
comment|/* (3) */
name|src
operator|+=
literal|2
expr_stmt|;
goto|goto
name|up_one
goto|;
block|}
elseif|else
if|if
condition|(
name|is_dir_sep
argument_list|(
name|src
index|[
literal|2
index|]
argument_list|)
condition|)
block|{
comment|/* (4) */
name|src
operator|+=
literal|3
expr_stmt|;
while|while
condition|(
name|is_dir_sep
argument_list|(
operator|*
name|src
argument_list|)
condition|)
name|src
operator|++
expr_stmt|;
goto|goto
name|up_one
goto|;
block|}
block|}
block|}
comment|/* copy up to the next '/', and eat all '/' */
while|while
condition|(
operator|(
name|c
operator|=
operator|*
name|src
operator|++
operator|)
operator|!=
literal|'\0'
operator|&&
operator|!
name|is_dir_sep
argument_list|(
name|c
argument_list|)
condition|)
operator|*
name|dst
operator|++
operator|=
name|c
expr_stmt|;
if|if
condition|(
name|is_dir_sep
argument_list|(
name|c
argument_list|)
condition|)
block|{
operator|*
name|dst
operator|++
operator|=
literal|'/'
expr_stmt|;
while|while
condition|(
name|is_dir_sep
argument_list|(
name|c
argument_list|)
condition|)
name|c
operator|=
operator|*
name|src
operator|++
expr_stmt|;
name|src
operator|--
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|c
condition|)
break|break;
continue|continue;
name|up_one
label|:
comment|/* 		 * dst0..dst is prefix portion, and dst[-1] is '/'; 		 * go up one level. 		 */
name|dst
operator|--
expr_stmt|;
comment|/* go to trailing '/' */
if|if
condition|(
name|dst
operator|<=
name|dst0
condition|)
return|return
operator|-
literal|1
return|;
comment|/* Windows: dst[-1] cannot be backslash anymore */
while|while
condition|(
name|dst0
operator|<
name|dst
operator|&&
name|dst
index|[
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
name|dst
operator|--
expr_stmt|;
block|}
operator|*
name|dst
operator|=
literal|'\0'
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * path = Canonical absolute path  * prefix_list = Colon-separated list of absolute paths  *  * Determines, for each path in prefix_list, whether the "prefix" really  * is an ancestor directory of path.  Returns the length of the longest  * ancestor directory, excluding any trailing slashes, or -1 if no prefix  * is an ancestor.  (Note that this means 0 is returned if prefix_list is  * "/".) "/foo" is not considered an ancestor of "/foobar".  Directories  * are not considered to be their own ancestors.  path must be in a  * canonical form: empty components, or "." or ".." components are not  * allowed.  prefix_list may be null, which is like "".  */
end_comment

begin_function
DECL|function|longest_ancestor_length
name|int
name|longest_ancestor_length
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix_list
parameter_list|)
block|{
name|char
name|buf
index|[
name|PATH_MAX
operator|+
literal|1
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|ceil
decl_stmt|,
modifier|*
name|colon
decl_stmt|;
name|int
name|len
decl_stmt|,
name|max_len
init|=
operator|-
literal|1
decl_stmt|;
if|if
condition|(
name|prefix_list
operator|==
name|NULL
operator|||
operator|!
name|strcmp
argument_list|(
name|path
argument_list|,
literal|"/"
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
for|for
control|(
name|colon
operator|=
name|ceil
operator|=
name|prefix_list
init|;
operator|*
name|colon
condition|;
name|ceil
operator|=
name|colon
operator|+
literal|1
control|)
block|{
for|for
control|(
name|colon
operator|=
name|ceil
init|;
operator|*
name|colon
operator|&&
operator|*
name|colon
operator|!=
name|PATH_SEP
condition|;
name|colon
operator|++
control|)
empty_stmt|;
name|len
operator|=
name|colon
operator|-
name|ceil
expr_stmt|;
if|if
condition|(
name|len
operator|==
literal|0
operator|||
name|len
operator|>
name|PATH_MAX
operator|||
operator|!
name|is_absolute_path
argument_list|(
name|ceil
argument_list|)
condition|)
continue|continue;
name|strlcpy
argument_list|(
name|buf
argument_list|,
name|ceil
argument_list|,
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|normalize_path_copy
argument_list|(
name|buf
argument_list|,
name|buf
argument_list|)
operator|<
literal|0
condition|)
continue|continue;
name|len
operator|=
name|strlen
argument_list|(
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|>
literal|0
operator|&&
name|buf
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
name|buf
index|[
operator|--
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|path
argument_list|,
name|buf
argument_list|,
name|len
argument_list|)
operator|&&
name|path
index|[
name|len
index|]
operator|==
literal|'/'
operator|&&
name|len
operator|>
name|max_len
condition|)
block|{
name|max_len
operator|=
name|len
expr_stmt|;
block|}
block|}
return|return
name|max_len
return|;
block|}
end_function

begin_comment
comment|/* strip arbitrary amount of directory separators at end of path */
end_comment

begin_function
DECL|function|chomp_trailing_dir_sep
specifier|static
specifier|inline
name|int
name|chomp_trailing_dir_sep
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|len
parameter_list|)
block|{
while|while
condition|(
name|len
operator|&&
name|is_dir_sep
argument_list|(
name|path
index|[
name|len
operator|-
literal|1
index|]
argument_list|)
condition|)
name|len
operator|--
expr_stmt|;
return|return
name|len
return|;
block|}
end_function

begin_comment
comment|/*  * If path ends with suffix (complete path components), returns the  * part before suffix (sans trailing directory separators).  * Otherwise returns NULL.  */
end_comment

begin_function
DECL|function|strip_path_suffix
name|char
modifier|*
name|strip_path_suffix
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|suffix
parameter_list|)
block|{
name|int
name|path_len
init|=
name|strlen
argument_list|(
name|path
argument_list|)
decl_stmt|,
name|suffix_len
init|=
name|strlen
argument_list|(
name|suffix
argument_list|)
decl_stmt|;
while|while
condition|(
name|suffix_len
condition|)
block|{
if|if
condition|(
operator|!
name|path_len
condition|)
return|return
name|NULL
return|;
if|if
condition|(
name|is_dir_sep
argument_list|(
name|path
index|[
name|path_len
operator|-
literal|1
index|]
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|is_dir_sep
argument_list|(
name|suffix
index|[
name|suffix_len
operator|-
literal|1
index|]
argument_list|)
condition|)
return|return
name|NULL
return|;
name|path_len
operator|=
name|chomp_trailing_dir_sep
argument_list|(
name|path
argument_list|,
name|path_len
argument_list|)
expr_stmt|;
name|suffix_len
operator|=
name|chomp_trailing_dir_sep
argument_list|(
name|suffix
argument_list|,
name|suffix_len
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|path
index|[
operator|--
name|path_len
index|]
operator|!=
name|suffix
index|[
operator|--
name|suffix_len
index|]
condition|)
return|return
name|NULL
return|;
block|}
if|if
condition|(
name|path_len
operator|&&
operator|!
name|is_dir_sep
argument_list|(
name|path
index|[
name|path_len
operator|-
literal|1
index|]
argument_list|)
condition|)
return|return
name|NULL
return|;
return|return
name|xstrndup
argument_list|(
name|path
argument_list|,
name|chomp_trailing_dir_sep
argument_list|(
name|path
argument_list|,
name|path_len
argument_list|)
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|daemon_avoid_alias
name|int
name|daemon_avoid_alias
parameter_list|(
specifier|const
name|char
modifier|*
name|p
parameter_list|)
block|{
name|int
name|sl
decl_stmt|,
name|ndot
decl_stmt|;
comment|/* 	 * This resurrects the belts and suspenders paranoia check by HPA 	 * done in<435560F7.4080006@zytor.com> thread, now enter_repo() 	 * does not do getcwd() based path canonicalizations. 	 * 	 * sl becomes true immediately after seeing '/' and continues to 	 * be true as long as dots continue after that without intervening 	 * non-dot character. 	 */
if|if
condition|(
operator|!
name|p
operator|||
operator|(
operator|*
name|p
operator|!=
literal|'/'
operator|&&
operator|*
name|p
operator|!=
literal|'~'
operator|)
condition|)
return|return
operator|-
literal|1
return|;
name|sl
operator|=
literal|1
expr_stmt|;
name|ndot
operator|=
literal|0
expr_stmt|;
name|p
operator|++
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|char
name|ch
init|=
operator|*
name|p
operator|++
decl_stmt|;
if|if
condition|(
name|sl
condition|)
block|{
if|if
condition|(
name|ch
operator|==
literal|'.'
condition|)
name|ndot
operator|++
expr_stmt|;
elseif|else
if|if
condition|(
name|ch
operator|==
literal|'/'
condition|)
block|{
if|if
condition|(
name|ndot
operator|<
literal|3
condition|)
comment|/* reject //, /./ and /../ */
return|return
operator|-
literal|1
return|;
name|ndot
operator|=
literal|0
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|ch
operator|==
literal|0
condition|)
block|{
if|if
condition|(
literal|0
operator|<
name|ndot
operator|&&
name|ndot
operator|<
literal|3
condition|)
comment|/* reject /.$ and /..$ */
return|return
operator|-
literal|1
return|;
return|return
literal|0
return|;
block|}
else|else
name|sl
operator|=
name|ndot
operator|=
literal|0
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|ch
operator|==
literal|0
condition|)
return|return
literal|0
return|;
elseif|else
if|if
condition|(
name|ch
operator|==
literal|'/'
condition|)
block|{
name|sl
operator|=
literal|1
expr_stmt|;
name|ndot
operator|=
literal|0
expr_stmt|;
block|}
block|}
block|}
end_function

end_unit

