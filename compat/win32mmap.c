begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"../git-compat-util.h"
end_include

begin_comment
comment|/*  * Note that this doesn't return the actual pagesize, but  * the allocation granularity. If future Windows specific git code  * needs the real getpagesize function, we need to find another solution.  */
end_comment

begin_function
DECL|function|mingw_getpagesize
name|int
name|mingw_getpagesize
parameter_list|(
name|void
parameter_list|)
block|{
name|SYSTEM_INFO
name|si
decl_stmt|;
name|GetSystemInfo
argument_list|(
operator|&
name|si
argument_list|)
expr_stmt|;
return|return
name|si
operator|.
name|dwAllocationGranularity
return|;
block|}
end_function

begin_function
DECL|function|git_mmap
name|void
modifier|*
name|git_mmap
parameter_list|(
name|void
modifier|*
name|start
parameter_list|,
name|size_t
name|length
parameter_list|,
name|int
name|prot
parameter_list|,
name|int
name|flags
parameter_list|,
name|int
name|fd
parameter_list|,
name|off_t
name|offset
parameter_list|)
block|{
name|HANDLE
name|hmap
decl_stmt|;
name|void
modifier|*
name|temp
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
name|uint64_t
name|o
init|=
name|offset
decl_stmt|;
name|uint32_t
name|l
init|=
name|o
operator|&
literal|0xFFFFFFFF
decl_stmt|;
name|uint32_t
name|h
init|=
operator|(
name|o
operator|>>
literal|32
operator|)
operator|&
literal|0xFFFFFFFF
decl_stmt|;
if|if
condition|(
operator|!
name|fstat
argument_list|(
name|fd
argument_list|,
operator|&
name|st
argument_list|)
condition|)
name|len
operator|=
name|xsize_t
argument_list|(
name|st
operator|.
name|st_size
argument_list|)
expr_stmt|;
else|else
name|die
argument_list|(
literal|"mmap: could not determine filesize"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|length
operator|+
name|offset
operator|)
operator|>
name|len
condition|)
name|length
operator|=
name|len
operator|-
name|offset
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|MAP_PRIVATE
operator|)
condition|)
name|die
argument_list|(
literal|"Invalid usage of mmap when built with USE_WIN32_MMAP"
argument_list|)
expr_stmt|;
name|hmap
operator|=
name|CreateFileMapping
argument_list|(
operator|(
name|HANDLE
operator|)
name|_get_osfhandle
argument_list|(
name|fd
argument_list|)
argument_list|,
literal|0
argument_list|,
name|PAGE_WRITECOPY
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|hmap
condition|)
return|return
name|MAP_FAILED
return|;
name|temp
operator|=
name|MapViewOfFileEx
argument_list|(
name|hmap
argument_list|,
name|FILE_MAP_COPY
argument_list|,
name|h
argument_list|,
name|l
argument_list|,
name|length
argument_list|,
name|start
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|CloseHandle
argument_list|(
name|hmap
argument_list|)
condition|)
name|warning
argument_list|(
literal|"unable to close file mapping handle\n"
argument_list|)
expr_stmt|;
return|return
name|temp
condition|?
name|temp
else|:
name|MAP_FAILED
return|;
block|}
end_function

begin_function
DECL|function|git_munmap
name|int
name|git_munmap
parameter_list|(
name|void
modifier|*
name|start
parameter_list|,
name|size_t
name|length
parameter_list|)
block|{
return|return
operator|!
name|UnmapViewOfFile
argument_list|(
name|start
argument_list|)
return|;
block|}
end_function

end_unit

