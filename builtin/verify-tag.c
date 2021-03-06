begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git verify-tag"  *  * Copyright (c) 2007 Carlos Rica<jasampler@gmail.com>  *  * Based on git-verify-tag.sh  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|<signal.h>
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"gpg-interface.h"
end_include

begin_decl_stmt
DECL|variable|verify_tag_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|verify_tag_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git verify-tag [-v | --verbose]<tag>..."
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|git_verify_tag_config
specifier|static
name|int
name|git_verify_tag_config
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
name|int
name|status
init|=
name|git_gpg_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
name|cb
argument_list|)
decl_stmt|;
if|if
condition|(
name|status
condition|)
return|return
name|status
return|;
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
DECL|function|cmd_verify_tag
name|int
name|cmd_verify_tag
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
name|int
name|i
init|=
literal|1
decl_stmt|,
name|verbose
init|=
literal|0
decl_stmt|,
name|had_error
init|=
literal|0
decl_stmt|;
name|unsigned
name|flags
init|=
literal|0
decl_stmt|;
specifier|const
name|struct
name|option
name|verify_tag_options
index|[]
init|=
block|{
name|OPT__VERBOSE
argument_list|(
operator|&
name|verbose
argument_list|,
name|N_
argument_list|(
literal|"print tag contents"
argument_list|)
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"raw"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"print raw gpg status output"
argument_list|)
argument_list|,
name|GPG_VERIFY_RAW
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|git_config
argument_list|(
name|git_verify_tag_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
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
name|verify_tag_options
argument_list|,
name|verify_tag_usage
argument_list|,
name|PARSE_OPT_KEEP_ARGV0
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|<=
name|i
condition|)
name|usage_with_options
argument_list|(
name|verify_tag_usage
argument_list|,
name|verify_tag_options
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|flags
operator||=
name|GPG_VERIFY_VERBOSE
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|argc
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|name
init|=
name|argv
index|[
name|i
operator|++
index|]
decl_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|name
argument_list|,
name|sha1
argument_list|)
condition|)
name|had_error
operator|=
operator|!
operator|!
name|error
argument_list|(
literal|"tag '%s' not found."
argument_list|,
name|name
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|gpg_verify_tag
argument_list|(
name|sha1
argument_list|,
name|name
argument_list|,
name|flags
argument_list|)
condition|)
name|had_error
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|had_error
return|;
block|}
end_function

end_unit

