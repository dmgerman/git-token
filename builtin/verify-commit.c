begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git commit-commit"  *  * Copyright (c) 2014 Michael J Gruber<git@drmicha.warpmail.net>  *  * Based on git-verify-tag  */
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
file|"commit.h"
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
DECL|variable|verify_commit_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|verify_commit_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git verify-commit [-v | --verbose]<commit>..."
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|run_gpg_verify
specifier|static
name|int
name|run_gpg_verify
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|int
name|verbose
parameter_list|)
block|{
name|struct
name|signature_check
name|signature_check
decl_stmt|;
name|memset
argument_list|(
operator|&
name|signature_check
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|signature_check
argument_list|)
argument_list|)
expr_stmt|;
name|check_commit_signature
argument_list|(
name|lookup_commit
argument_list|(
name|sha1
argument_list|)
argument_list|,
operator|&
name|signature_check
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
operator|&&
name|signature_check
operator|.
name|payload
condition|)
name|fputs
argument_list|(
name|signature_check
operator|.
name|payload
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
name|signature_check
operator|.
name|gpg_output
condition|)
name|fputs
argument_list|(
name|signature_check
operator|.
name|gpg_output
argument_list|,
name|stderr
argument_list|)
expr_stmt|;
name|signature_check_clear
argument_list|(
operator|&
name|signature_check
argument_list|)
expr_stmt|;
return|return
name|signature_check
operator|.
name|result
operator|!=
literal|'G'
return|;
block|}
end_function

begin_function
DECL|function|verify_commit
specifier|static
name|int
name|verify_commit
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|verbose
parameter_list|)
block|{
name|enum
name|object_type
name|type
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|int
name|ret
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
return|return
name|error
argument_list|(
literal|"commit '%s' not found."
argument_list|,
name|name
argument_list|)
return|;
name|buf
operator|=
name|read_sha1_file
argument_list|(
name|sha1
argument_list|,
operator|&
name|type
argument_list|,
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buf
condition|)
return|return
name|error
argument_list|(
literal|"%s: unable to read file."
argument_list|,
name|name
argument_list|)
return|;
if|if
condition|(
name|type
operator|!=
name|OBJ_COMMIT
condition|)
return|return
name|error
argument_list|(
literal|"%s: cannot verify a non-commit object of type %s."
argument_list|,
name|name
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
argument_list|)
return|;
name|ret
operator|=
name|run_gpg_verify
argument_list|(
name|sha1
argument_list|,
name|buf
argument_list|,
name|size
argument_list|,
name|verbose
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|git_verify_commit_config
specifier|static
name|int
name|git_verify_commit_config
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
DECL|function|cmd_verify_commit
name|int
name|cmd_verify_commit
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
specifier|const
name|struct
name|option
name|verify_commit_options
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
literal|"print commit contents"
argument_list|)
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|git_config
argument_list|(
name|git_verify_commit_config
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
name|verify_commit_options
argument_list|,
name|verify_commit_usage
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
name|verify_commit_usage
argument_list|,
name|verify_commit_options
argument_list|)
expr_stmt|;
comment|/* sometimes the program was terminated because this signal 	 * was received in the process of writing the gpg input: */
name|signal
argument_list|(
name|SIGPIPE
argument_list|,
name|SIG_IGN
argument_list|)
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|argc
condition|)
if|if
condition|(
name|verify_commit
argument_list|(
name|argv
index|[
name|i
operator|++
index|]
argument_list|,
name|verbose
argument_list|)
condition|)
name|had_error
operator|=
literal|1
expr_stmt|;
return|return
name|had_error
return|;
block|}
end_function

end_unit

