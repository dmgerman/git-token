begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2005 Junio C Hamano  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_decl_stmt
DECL|variable|diff_output_format
specifier|static
name|int
name|diff_output_format
init|=
name|DIFF_FORMAT_HUMAN
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|detect_rename
specifier|static
name|int
name|detect_rename
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|find_copies_harder
specifier|static
name|int
name|find_copies_harder
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_setup_opt
specifier|static
name|int
name|diff_setup_opt
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_score_opt
specifier|static
name|int
name|diff_score_opt
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pickaxe
specifier|static
specifier|const
name|char
modifier|*
name|pickaxe
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pickaxe_opts
specifier|static
name|int
name|pickaxe_opts
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_break_opt
specifier|static
name|int
name|diff_break_opt
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|orderfile
specifier|static
specifier|const
name|char
modifier|*
name|orderfile
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_filter
specifier|static
specifier|const
name|char
modifier|*
name|diff_filter
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|diff_stages_usage
specifier|static
name|char
modifier|*
name|diff_stages_usage
init|=
literal|"git-diff-stages [<common diff options>]<stage1><stage2> [<path>...]"
name|COMMON_DIFF_OPTIONS_HELP
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|diff_stages
specifier|static
name|void
name|diff_stages
parameter_list|(
name|int
name|stage1
parameter_list|,
name|int
name|stage2
parameter_list|)
block|{
name|int
name|i
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|i
operator|<
name|active_nr
condition|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|,
modifier|*
name|stages
index|[
literal|4
index|]
init|=
block|{
name|NULL
block|, }
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|one
decl_stmt|,
modifier|*
name|two
decl_stmt|;
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
name|int
name|len
decl_stmt|;
name|ce
operator|=
name|active_cache
index|[
name|i
index|]
expr_stmt|;
name|len
operator|=
name|ce_namelen
argument_list|(
name|ce
argument_list|)
expr_stmt|;
name|name
operator|=
name|ce
operator|->
name|name
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|stage
init|=
name|ce_stage
argument_list|(
name|ce
argument_list|)
decl_stmt|;
name|stages
index|[
name|stage
index|]
operator|=
name|ce
expr_stmt|;
if|if
condition|(
name|active_nr
operator|<=
operator|++
name|i
condition|)
break|break;
name|ce
operator|=
name|active_cache
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|ce_namelen
argument_list|(
name|ce
argument_list|)
operator|!=
name|len
operator|||
name|memcmp
argument_list|(
name|name
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|len
argument_list|)
condition|)
break|break;
block|}
name|one
operator|=
name|stages
index|[
name|stage1
index|]
expr_stmt|;
name|two
operator|=
name|stages
index|[
name|stage2
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|one
operator|&&
operator|!
name|two
condition|)
continue|continue;
if|if
condition|(
operator|!
name|one
condition|)
name|diff_addremove
argument_list|(
literal|'+'
argument_list|,
name|ntohl
argument_list|(
name|two
operator|->
name|ce_mode
argument_list|)
argument_list|,
name|two
operator|->
name|sha1
argument_list|,
name|name
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|two
condition|)
name|diff_addremove
argument_list|(
literal|'-'
argument_list|,
name|ntohl
argument_list|(
name|one
operator|->
name|ce_mode
argument_list|)
argument_list|,
name|one
operator|->
name|sha1
argument_list|,
name|name
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|memcmp
argument_list|(
name|one
operator|->
name|sha1
argument_list|,
name|two
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
operator|||
operator|(
name|one
operator|->
name|ce_mode
operator|!=
name|two
operator|->
name|ce_mode
operator|)
operator|||
name|find_copies_harder
condition|)
name|diff_change
argument_list|(
name|ntohl
argument_list|(
name|one
operator|->
name|ce_mode
argument_list|)
argument_list|,
name|ntohl
argument_list|(
name|two
operator|->
name|ce_mode
argument_list|)
argument_list|,
name|one
operator|->
name|sha1
argument_list|,
name|two
operator|->
name|sha1
argument_list|,
name|name
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
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
specifier|const
name|char
modifier|*
modifier|*
name|av
parameter_list|)
block|{
name|int
name|stage1
decl_stmt|,
name|stage2
decl_stmt|;
name|read_cache
argument_list|()
expr_stmt|;
while|while
condition|(
literal|1
operator|<
name|ac
operator|&&
name|av
index|[
literal|1
index|]
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|av
index|[
literal|1
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-r"
argument_list|)
condition|)
empty_stmt|;
comment|/* as usual */
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-p"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-u"
argument_list|)
condition|)
name|diff_output_format
operator|=
name|DIFF_FORMAT_PATCH
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"-B"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
if|if
condition|(
operator|(
name|diff_break_opt
operator|=
name|diff_scoreopt_parse
argument_list|(
name|arg
argument_list|)
operator|)
operator|==
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|diff_stages_usage
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"-M"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
name|detect_rename
operator|=
name|DIFF_DETECT_RENAME
expr_stmt|;
if|if
condition|(
operator|(
name|diff_score_opt
operator|=
name|diff_scoreopt_parse
argument_list|(
name|arg
argument_list|)
operator|)
operator|==
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|diff_stages_usage
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"-C"
argument_list|,
literal|2
argument_list|)
condition|)
block|{
name|detect_rename
operator|=
name|DIFF_DETECT_COPY
expr_stmt|;
if|if
condition|(
operator|(
name|diff_score_opt
operator|=
name|diff_scoreopt_parse
argument_list|(
name|arg
argument_list|)
operator|)
operator|==
operator|-
literal|1
condition|)
name|usage
argument_list|(
name|diff_stages_usage
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--find-copies-harder"
argument_list|)
condition|)
name|find_copies_harder
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-z"
argument_list|)
condition|)
name|diff_output_format
operator|=
name|DIFF_FORMAT_MACHINE
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--name-only"
argument_list|)
condition|)
name|diff_output_format
operator|=
name|DIFF_FORMAT_NAME
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--name-only-z"
argument_list|)
condition|)
name|diff_output_format
operator|=
name|DIFF_FORMAT_NAME_Z
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-R"
argument_list|)
condition|)
name|diff_setup_opt
operator||=
name|DIFF_SETUP_REVERSE
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"-S"
argument_list|,
literal|2
argument_list|)
condition|)
name|pickaxe
operator|=
name|arg
operator|+
literal|2
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"-O"
argument_list|,
literal|2
argument_list|)
condition|)
name|orderfile
operator|=
name|arg
operator|+
literal|2
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|arg
argument_list|,
literal|"--diff-filter="
argument_list|,
literal|14
argument_list|)
condition|)
name|diff_filter
operator|=
name|arg
operator|+
literal|14
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--pickaxe-all"
argument_list|)
condition|)
name|pickaxe_opts
operator|=
name|DIFF_PICKAXE_ALL
expr_stmt|;
else|else
name|usage
argument_list|(
name|diff_stages_usage
argument_list|)
expr_stmt|;
name|ac
operator|--
expr_stmt|;
name|av
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|ac
operator|<
literal|3
operator|||
name|sscanf
argument_list|(
name|av
index|[
literal|1
index|]
argument_list|,
literal|"%d"
argument_list|,
operator|&
name|stage1
argument_list|)
operator|!=
literal|1
operator|||
operator|!
operator|(
literal|0
operator|<=
name|stage1
operator|&&
name|stage1
operator|<=
literal|3
operator|)
operator|||
name|sscanf
argument_list|(
name|av
index|[
literal|2
index|]
argument_list|,
literal|"%d"
argument_list|,
operator|&
name|stage2
argument_list|)
operator|!=
literal|1
operator|||
operator|!
operator|(
literal|0
operator|<=
name|stage2
operator|&&
name|stage2
operator|<=
literal|3
operator|)
operator|||
operator|(
name|find_copies_harder
operator|&&
name|detect_rename
operator|!=
name|DIFF_DETECT_COPY
operator|)
condition|)
name|usage
argument_list|(
name|diff_stages_usage
argument_list|)
expr_stmt|;
name|av
operator|+=
literal|3
expr_stmt|;
comment|/* The rest from av[0] are for paths restriction. */
name|diff_setup
argument_list|(
name|diff_setup_opt
argument_list|)
expr_stmt|;
name|diff_stages
argument_list|(
name|stage1
argument_list|,
name|stage2
argument_list|)
expr_stmt|;
name|diffcore_std
argument_list|(
name|av
argument_list|,
name|detect_rename
argument_list|,
name|diff_score_opt
argument_list|,
name|pickaxe
argument_list|,
name|pickaxe_opts
argument_list|,
name|diff_break_opt
argument_list|,
name|orderfile
argument_list|,
name|diff_filter
argument_list|)
expr_stmt|;
name|diff_flush
argument_list|(
name|diff_output_format
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

