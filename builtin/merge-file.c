begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"xdiff/xdiff.h"
end_include

begin_include
include|#
directive|include
file|"xdiff-interface.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_decl_stmt
DECL|variable|merge_file_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|merge_file_usage
index|[]
init|=
block|{
literal|"git merge-file [options] [-L name1 [-L orig [-L name2]]] file1 orig_file file2"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|label_cb
specifier|static
name|int
name|label_cb
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|unset
parameter_list|)
block|{
specifier|static
name|int
name|label_count
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|names
init|=
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
name|opt
operator|->
name|value
decl_stmt|;
if|if
condition|(
name|label_count
operator|>=
literal|3
condition|)
return|return
name|error
argument_list|(
literal|"too many labels on the command line"
argument_list|)
return|;
name|names
index|[
name|label_count
operator|++
index|]
operator|=
name|arg
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_merge_file
name|int
name|cmd_merge_file
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
specifier|const
name|char
modifier|*
name|names
index|[
literal|3
index|]
init|=
block|{
name|NULL
block|,
name|NULL
block|,
name|NULL
block|}
decl_stmt|;
name|mmfile_t
name|mmfs
index|[
literal|3
index|]
decl_stmt|;
name|mmbuffer_t
name|result
init|=
block|{
name|NULL
block|,
literal|0
block|}
decl_stmt|;
name|xmparam_t
name|xmp
init|=
block|{
block|{
literal|0
block|}
block|}
decl_stmt|;
name|int
name|ret
init|=
literal|0
decl_stmt|,
name|i
init|=
literal|0
decl_stmt|,
name|to_stdout
init|=
literal|0
decl_stmt|;
name|int
name|quiet
init|=
literal|0
decl_stmt|;
name|int
name|nongit
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_BOOLEAN
argument_list|(
literal|'p'
argument_list|,
literal|"stdout"
argument_list|,
operator|&
name|to_stdout
argument_list|,
literal|"send results to standard output"
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"diff3"
argument_list|,
operator|&
name|xmp
operator|.
name|style
argument_list|,
literal|"use a diff3 based merge"
argument_list|,
name|XDL_MERGE_DIFF3
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"ours"
argument_list|,
operator|&
name|xmp
operator|.
name|favor
argument_list|,
literal|"for conflicts, use our version"
argument_list|,
name|XDL_MERGE_FAVOR_OURS
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"theirs"
argument_list|,
operator|&
name|xmp
operator|.
name|favor
argument_list|,
literal|"for conflicts, use their version"
argument_list|,
name|XDL_MERGE_FAVOR_THEIRS
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"union"
argument_list|,
operator|&
name|xmp
operator|.
name|favor
argument_list|,
literal|"for conflicts, use a union version"
argument_list|,
name|XDL_MERGE_FAVOR_UNION
argument_list|)
block|,
name|OPT_INTEGER
argument_list|(
literal|0
argument_list|,
literal|"marker-size"
argument_list|,
operator|&
name|xmp
operator|.
name|marker_size
argument_list|,
literal|"for conflicts, use this marker size"
argument_list|)
block|,
name|OPT__QUIET
argument_list|(
operator|&
name|quiet
argument_list|)
block|,
name|OPT_CALLBACK
argument_list|(
literal|'L'
argument_list|,
name|NULL
argument_list|,
name|names
argument_list|,
literal|"name"
argument_list|,
literal|"set labels for file1/orig_file/file2"
argument_list|,
operator|&
name|label_cb
argument_list|)
block|,
name|OPT_END
argument_list|()
block|, 	}
decl_stmt|;
name|xmp
operator|.
name|level
operator|=
name|XDL_MERGE_ZEALOUS_ALNUM
expr_stmt|;
name|xmp
operator|.
name|style
operator|=
literal|0
expr_stmt|;
name|xmp
operator|.
name|favor
operator|=
literal|0
expr_stmt|;
name|prefix
operator|=
name|setup_git_directory_gently
argument_list|(
operator|&
name|nongit
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|nongit
condition|)
block|{
comment|/* Read the configuration file */
name|git_config
argument_list|(
name|git_xmerge_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|git_xmerge_style
condition|)
name|xmp
operator|.
name|style
operator|=
name|git_xmerge_style
expr_stmt|;
block|}
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
name|options
argument_list|,
name|merge_file_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|!=
literal|3
condition|)
name|usage_with_options
argument_list|(
name|merge_file_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
name|quiet
condition|)
block|{
if|if
condition|(
operator|!
name|freopen
argument_list|(
literal|"/dev/null"
argument_list|,
literal|"w"
argument_list|,
name|stderr
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"failed to redirect stderr to /dev/null: "
literal|"%s\n"
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|3
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|names
index|[
name|i
index|]
condition|)
name|names
index|[
name|i
index|]
operator|=
name|argv
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|read_mmfile
argument_list|(
name|mmfs
operator|+
name|i
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|buffer_is_binary
argument_list|(
name|mmfs
index|[
name|i
index|]
operator|.
name|ptr
argument_list|,
name|mmfs
index|[
name|i
index|]
operator|.
name|size
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"Cannot merge binary files: %s\n"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
return|;
block|}
name|xmp
operator|.
name|ancestor
operator|=
name|names
index|[
literal|1
index|]
expr_stmt|;
name|xmp
operator|.
name|file1
operator|=
name|names
index|[
literal|0
index|]
expr_stmt|;
name|xmp
operator|.
name|file2
operator|=
name|names
index|[
literal|2
index|]
expr_stmt|;
name|ret
operator|=
name|xdl_merge
argument_list|(
name|mmfs
operator|+
literal|1
argument_list|,
name|mmfs
operator|+
literal|0
argument_list|,
name|mmfs
operator|+
literal|2
argument_list|,
operator|&
name|xmp
argument_list|,
operator|&
name|result
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|3
condition|;
name|i
operator|++
control|)
name|free
argument_list|(
name|mmfs
index|[
name|i
index|]
operator|.
name|ptr
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|>=
literal|0
condition|)
block|{
specifier|const
name|char
modifier|*
name|filename
init|=
name|argv
index|[
literal|0
index|]
decl_stmt|;
name|FILE
modifier|*
name|f
init|=
name|to_stdout
condition|?
name|stdout
else|:
name|fopen
argument_list|(
name|filename
argument_list|,
literal|"wb"
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|f
condition|)
name|ret
operator|=
name|error
argument_list|(
literal|"Could not open %s for writing"
argument_list|,
name|filename
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|result
operator|.
name|size
operator|&&
name|fwrite
argument_list|(
name|result
operator|.
name|ptr
argument_list|,
name|result
operator|.
name|size
argument_list|,
literal|1
argument_list|,
name|f
argument_list|)
operator|!=
literal|1
condition|)
name|ret
operator|=
name|error
argument_list|(
literal|"Could not write to %s"
argument_list|,
name|filename
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|fclose
argument_list|(
name|f
argument_list|)
condition|)
name|ret
operator|=
name|error
argument_list|(
literal|"Could not close %s"
argument_list|,
name|filename
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|result
operator|.
name|ptr
argument_list|)
expr_stmt|;
block|}
return|return
name|ret
return|;
block|}
end_function

end_unit

