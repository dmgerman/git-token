begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_define
DECL|macro|WIN32_LEAN_AND_MEAN
define|#
directive|define
name|WIN32_LEAN_AND_MEAN
end_define

begin_include
include|#
directive|include
file|"../git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"win32.h"
end_include

begin_include
include|#
directive|include
file|"../cache.h"
end_include

begin_comment
comment|/* to read configuration */
end_comment

begin_function
DECL|function|filetime_to_timespec
specifier|static
specifier|inline
name|void
name|filetime_to_timespec
parameter_list|(
specifier|const
name|FILETIME
modifier|*
name|ft
parameter_list|,
name|struct
name|timespec
modifier|*
name|ts
parameter_list|)
block|{
name|long
name|long
name|winTime
init|=
operator|(
operator|(
name|long
name|long
operator|)
name|ft
operator|->
name|dwHighDateTime
operator|<<
literal|32
operator|)
operator|+
name|ft
operator|->
name|dwLowDateTime
decl_stmt|;
name|winTime
operator|-=
literal|116444736000000000LL
expr_stmt|;
comment|/* Windows to Unix Epoch conversion */
comment|/* convert 100-nsecond interval to seconds and nanoseconds */
name|ts
operator|->
name|tv_sec
operator|=
call|(
name|time_t
call|)
argument_list|(
name|winTime
operator|/
literal|10000000
argument_list|)
expr_stmt|;
name|ts
operator|->
name|tv_nsec
operator|=
call|(
name|long
call|)
argument_list|(
name|winTime
operator|-
name|ts
operator|->
name|tv_sec
operator|*
literal|10000000LL
argument_list|)
operator|*
literal|100
expr_stmt|;
block|}
end_function

begin_define
DECL|macro|size_to_blocks
define|#
directive|define
name|size_to_blocks
parameter_list|(
name|s
parameter_list|)
value|(((s)+511)/512)
end_define

begin_comment
comment|/* do_stat is a common implementation for cygwin_lstat and cygwin_stat.  *  * To simplify its logic, in the case of cygwin symlinks, this implementation  * falls back to the cygwin version of stat/lstat, which is provided as the  * last argument.  */
end_comment

begin_function
DECL|function|do_stat
specifier|static
name|int
name|do_stat
parameter_list|(
specifier|const
name|char
modifier|*
name|file_name
parameter_list|,
name|struct
name|stat
modifier|*
name|buf
parameter_list|,
name|stat_fn_t
name|cygstat
parameter_list|)
block|{
name|WIN32_FILE_ATTRIBUTE_DATA
name|fdata
decl_stmt|;
if|if
condition|(
name|file_name
index|[
literal|0
index|]
operator|==
literal|'/'
condition|)
return|return
name|cygstat
argument_list|(
name|file_name
argument_list|,
name|buf
argument_list|)
return|;
if|if
condition|(
operator|!
operator|(
name|errno
operator|=
name|get_file_attr
argument_list|(
name|file_name
argument_list|,
operator|&
name|fdata
argument_list|)
operator|)
condition|)
block|{
comment|/* 		 * If the system attribute is set and it is not a directory then 		 * it could be a symbol link created in the nowinsymlinks mode. 		 * Normally, Cygwin works in the winsymlinks mode, so this situation 		 * is very unlikely. For the sake of simplicity of our code, let's 		 * Cygwin to handle it. 		 */
if|if
condition|(
operator|(
name|fdata
operator|.
name|dwFileAttributes
operator|&
name|FILE_ATTRIBUTE_SYSTEM
operator|)
operator|&&
operator|!
operator|(
name|fdata
operator|.
name|dwFileAttributes
operator|&
name|FILE_ATTRIBUTE_DIRECTORY
operator|)
condition|)
return|return
name|cygstat
argument_list|(
name|file_name
argument_list|,
name|buf
argument_list|)
return|;
comment|/* fill out the stat structure */
name|buf
operator|->
name|st_dev
operator|=
name|buf
operator|->
name|st_rdev
operator|=
literal|0
expr_stmt|;
comment|/* not used by Git */
name|buf
operator|->
name|st_ino
operator|=
literal|0
expr_stmt|;
name|buf
operator|->
name|st_mode
operator|=
name|file_attr_to_st_mode
argument_list|(
name|fdata
operator|.
name|dwFileAttributes
argument_list|)
expr_stmt|;
name|buf
operator|->
name|st_nlink
operator|=
literal|1
expr_stmt|;
name|buf
operator|->
name|st_uid
operator|=
name|buf
operator|->
name|st_gid
operator|=
literal|0
expr_stmt|;
ifdef|#
directive|ifdef
name|__CYGWIN_USE_BIG_TYPES__
name|buf
operator|->
name|st_size
operator|=
operator|(
operator|(
name|_off64_t
operator|)
name|fdata
operator|.
name|nFileSizeHigh
operator|<<
literal|32
operator|)
operator|+
name|fdata
operator|.
name|nFileSizeLow
expr_stmt|;
else|#
directive|else
name|buf
operator|->
name|st_size
operator|=
operator|(
name|off_t
operator|)
name|fdata
operator|.
name|nFileSizeLow
expr_stmt|;
endif|#
directive|endif
name|buf
operator|->
name|st_blocks
operator|=
name|size_to_blocks
argument_list|(
name|buf
operator|->
name|st_size
argument_list|)
expr_stmt|;
name|filetime_to_timespec
argument_list|(
operator|&
name|fdata
operator|.
name|ftLastAccessTime
argument_list|,
operator|&
name|buf
operator|->
name|st_atim
argument_list|)
expr_stmt|;
name|filetime_to_timespec
argument_list|(
operator|&
name|fdata
operator|.
name|ftLastWriteTime
argument_list|,
operator|&
name|buf
operator|->
name|st_mtim
argument_list|)
expr_stmt|;
name|filetime_to_timespec
argument_list|(
operator|&
name|fdata
operator|.
name|ftCreationTime
argument_list|,
operator|&
name|buf
operator|->
name|st_ctim
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
elseif|else
if|if
condition|(
name|errno
operator|==
name|ENOENT
condition|)
block|{
comment|/* 		 * In the winsymlinks mode (which is the default), Cygwin 		 * emulates symbol links using Windows shortcut files. These 		 * files are formed by adding .lnk extension. So, if we have 		 * not found the specified file name, it could be that it is 		 * a symbol link. Let's Cygwin to deal with that. 		 */
return|return
name|cygstat
argument_list|(
name|file_name
argument_list|,
name|buf
argument_list|)
return|;
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_comment
comment|/* We provide our own lstat/stat functions, since the provided Cygwin versions  * of these functions are too slow. These stat functions are tailored for Git's  * usage, and therefore they are not meant to be complete and correct emulation  * of lstat/stat functionality.  */
end_comment

begin_function
DECL|function|cygwin_lstat
specifier|static
name|int
name|cygwin_lstat
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|struct
name|stat
modifier|*
name|buf
parameter_list|)
block|{
return|return
name|do_stat
argument_list|(
name|path
argument_list|,
name|buf
argument_list|,
name|lstat
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|cygwin_stat
specifier|static
name|int
name|cygwin_stat
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|struct
name|stat
modifier|*
name|buf
parameter_list|)
block|{
return|return
name|do_stat
argument_list|(
name|path
argument_list|,
name|buf
argument_list|,
name|stat
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * At start up, we are trying to determine whether Win32 API or cygwin stat  * functions should be used. The choice is determined by core.ignorecygwinfstricks.  * Reading this option is not always possible immediately as git_dir may be  * not be set yet. So until it is set, use cygwin lstat/stat functions.  * However, if the trust_executable_bit is set, we must use the Cygwin posix  * stat/lstat as the Windows stat fuctions do not determine posix filemode.  */
end_comment

begin_decl_stmt
DECL|variable|native_stat
specifier|static
name|int
name|native_stat
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|trust_executable_bit
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|git_cygwin_config
specifier|static
name|int
name|git_cygwin_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"core.ignorecygwinfstricks"
argument_list|)
condition|)
block|{
name|native_stat
operator|=
name|git_config_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
return|return
name|git_default_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
name|cb
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|init_stat
specifier|static
name|int
name|init_stat
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
name|have_git_dir
argument_list|()
condition|)
block|{
name|git_config
argument_list|(
name|git_cygwin_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|trust_executable_bit
operator|&&
name|native_stat
condition|)
block|{
name|cygwin_stat_fn
operator|=
name|cygwin_stat
expr_stmt|;
name|cygwin_lstat_fn
operator|=
name|cygwin_lstat
expr_stmt|;
block|}
else|else
block|{
name|cygwin_stat_fn
operator|=
name|stat
expr_stmt|;
name|cygwin_lstat_fn
operator|=
name|lstat
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|cygwin_stat_stub
specifier|static
name|int
name|cygwin_stat_stub
parameter_list|(
specifier|const
name|char
modifier|*
name|file_name
parameter_list|,
name|struct
name|stat
modifier|*
name|buf
parameter_list|)
block|{
return|return
operator|(
name|init_stat
argument_list|()
condition|?
name|stat
else|:
operator|*
name|cygwin_stat_fn
operator|)
operator|(
name|file_name
operator|,
name|buf
operator|)
return|;
block|}
end_function

begin_function
DECL|function|cygwin_lstat_stub
specifier|static
name|int
name|cygwin_lstat_stub
parameter_list|(
specifier|const
name|char
modifier|*
name|file_name
parameter_list|,
name|struct
name|stat
modifier|*
name|buf
parameter_list|)
block|{
return|return
operator|(
name|init_stat
argument_list|()
condition|?
name|lstat
else|:
operator|*
name|cygwin_lstat_fn
operator|)
operator|(
name|file_name
operator|,
name|buf
operator|)
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|cygwin_stat_fn
name|stat_fn_t
name|cygwin_stat_fn
init|=
name|cygwin_stat_stub
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cygwin_lstat_fn
name|stat_fn_t
name|cygwin_lstat_fn
init|=
name|cygwin_lstat_stub
decl_stmt|;
end_decl_stmt

end_unit

