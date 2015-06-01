begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_function
DECL|function|compare_untracked
specifier|static
name|int
name|compare_untracked
parameter_list|(
specifier|const
name|void
modifier|*
name|a_
parameter_list|,
specifier|const
name|void
modifier|*
name|b_
parameter_list|)
block|{
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|a
init|=
name|a_
decl_stmt|;
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|b
init|=
name|b_
decl_stmt|;
return|return
name|strcmp
argument_list|(
operator|*
name|a
argument_list|,
operator|*
name|b
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|compare_dir
specifier|static
name|int
name|compare_dir
parameter_list|(
specifier|const
name|void
modifier|*
name|a_
parameter_list|,
specifier|const
name|void
modifier|*
name|b_
parameter_list|)
block|{
specifier|const
name|struct
name|untracked_cache_dir
modifier|*
specifier|const
modifier|*
name|a
init|=
name|a_
decl_stmt|;
specifier|const
name|struct
name|untracked_cache_dir
modifier|*
specifier|const
modifier|*
name|b
init|=
name|b_
decl_stmt|;
return|return
name|strcmp
argument_list|(
operator|(
operator|*
name|a
operator|)
operator|->
name|name
argument_list|,
operator|(
operator|*
name|b
operator|)
operator|->
name|name
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|dump
specifier|static
name|void
name|dump
parameter_list|(
name|struct
name|untracked_cache_dir
modifier|*
name|ucd
parameter_list|,
name|struct
name|strbuf
modifier|*
name|base
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|len
decl_stmt|;
name|qsort
argument_list|(
name|ucd
operator|->
name|untracked
argument_list|,
name|ucd
operator|->
name|untracked_nr
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|ucd
operator|->
name|untracked
argument_list|)
argument_list|,
name|compare_untracked
argument_list|)
expr_stmt|;
name|qsort
argument_list|(
name|ucd
operator|->
name|dirs
argument_list|,
name|ucd
operator|->
name|dirs_nr
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|ucd
operator|->
name|dirs
argument_list|)
argument_list|,
name|compare_dir
argument_list|)
expr_stmt|;
name|len
operator|=
name|base
operator|->
name|len
expr_stmt|;
name|strbuf_addf
argument_list|(
name|base
argument_list|,
literal|"%s/"
argument_list|,
name|ucd
operator|->
name|name
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s %s"
argument_list|,
name|base
operator|->
name|buf
argument_list|,
name|sha1_to_hex
argument_list|(
name|ucd
operator|->
name|exclude_sha1
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ucd
operator|->
name|recurse
condition|)
name|fputs
argument_list|(
literal|" recurse"
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
name|ucd
operator|->
name|check_only
condition|)
name|fputs
argument_list|(
literal|" check_only"
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
name|ucd
operator|->
name|valid
condition|)
name|fputs
argument_list|(
literal|" valid"
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"\n"
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
name|ucd
operator|->
name|untracked_nr
condition|;
name|i
operator|++
control|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|ucd
operator|->
name|untracked
index|[
name|i
index|]
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
name|ucd
operator|->
name|dirs_nr
condition|;
name|i
operator|++
control|)
name|dump
argument_list|(
name|ucd
operator|->
name|dirs
index|[
name|i
index|]
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|strbuf_setlen
argument_list|(
name|base
argument_list|,
name|len
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|ac
parameter_list|,
name|char
modifier|*
modifier|*
name|av
parameter_list|)
block|{
name|struct
name|untracked_cache
modifier|*
name|uc
decl_stmt|;
name|struct
name|strbuf
name|base
init|=
name|STRBUF_INIT
decl_stmt|;
name|setup_git_directory
argument_list|()
expr_stmt|;
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"unable to read index file"
argument_list|)
expr_stmt|;
name|uc
operator|=
name|the_index
operator|.
name|untracked
expr_stmt|;
if|if
condition|(
operator|!
name|uc
condition|)
block|{
name|printf
argument_list|(
literal|"no untracked cache\n"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|printf
argument_list|(
literal|"info/exclude %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|uc
operator|->
name|ss_info_exclude
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"core.excludesfile %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|uc
operator|->
name|ss_excludes_file
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"exclude_per_dir %s\n"
argument_list|,
name|uc
operator|->
name|exclude_per_dir
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"flags %08x\n"
argument_list|,
name|uc
operator|->
name|dir_flags
argument_list|)
expr_stmt|;
if|if
condition|(
name|uc
operator|->
name|root
condition|)
name|dump
argument_list|(
name|uc
operator|->
name|root
argument_list|,
operator|&
name|base
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit
