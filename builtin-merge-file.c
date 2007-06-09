begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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

begin_decl_stmt
DECL|variable|merge_file_usage
specifier|static
specifier|const
name|char
name|merge_file_usage
index|[]
init|=
literal|"git merge-file [-p | --stdout] [-q | --quiet] [-L name1 [-L orig [-L name2]]] file1 orig_file file2"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_merge_file
name|int
name|cmd_merge_file
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|char
modifier|*
modifier|*
name|envp
parameter_list|)
block|{
name|char
modifier|*
name|names
index|[
literal|3
index|]
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
name|xpparam_t
name|xpp
init|=
block|{
name|XDF_NEED_MINIMAL
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
while|while
condition|(
name|argc
operator|>
literal|4
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-L"
argument_list|)
operator|&&
name|i
operator|<
literal|3
condition|)
block|{
name|names
index|[
name|i
operator|++
index|]
operator|=
name|argv
index|[
literal|2
index|]
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-p"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--stdout"
argument_list|)
condition|)
name|to_stdout
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"-q"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"--quiet"
argument_list|)
condition|)
name|freopen
argument_list|(
literal|"/dev/null"
argument_list|,
literal|"w"
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
else|else
name|usage
argument_list|(
name|merge_file_usage
argument_list|)
expr_stmt|;
name|argc
operator|--
expr_stmt|;
name|argv
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|!=
literal|4
condition|)
name|usage
argument_list|(
name|merge_file_usage
argument_list|)
expr_stmt|;
for|for
control|(
init|;
name|i
operator|<
literal|3
condition|;
name|i
operator|++
control|)
name|names
index|[
name|i
index|]
operator|=
name|argv
index|[
name|i
operator|+
literal|1
index|]
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
block|{
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
operator|+
literal|1
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
operator|+
literal|1
index|]
argument_list|)
return|;
block|}
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
name|names
index|[
literal|0
index|]
argument_list|,
name|mmfs
operator|+
literal|2
argument_list|,
name|names
index|[
literal|2
index|]
argument_list|,
operator|&
name|xpp
argument_list|,
name|XDL_MERGE_ZEALOUS
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
name|char
modifier|*
name|filename
init|=
name|argv
index|[
literal|1
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

