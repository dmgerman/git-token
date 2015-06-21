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
DECL|function|run_gpg_verify
specifier|static
name|int
name|run_gpg_verify
parameter_list|(
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
name|sigc
decl_stmt|;
name|int
name|len
decl_stmt|;
name|int
name|ret
decl_stmt|;
name|memset
argument_list|(
operator|&
name|sigc
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|sigc
argument_list|)
argument_list|)
expr_stmt|;
name|len
operator|=
name|parse_signature
argument_list|(
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|size
operator|==
name|len
condition|)
block|{
if|if
condition|(
name|verbose
condition|)
name|write_in_full
argument_list|(
literal|1
argument_list|,
name|buf
argument_list|,
name|len
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"no signature found"
argument_list|)
return|;
block|}
name|ret
operator|=
name|check_signature
argument_list|(
name|buf
argument_list|,
name|len
argument_list|,
name|buf
operator|+
name|len
argument_list|,
name|size
operator|-
name|len
argument_list|,
operator|&
name|sigc
argument_list|)
expr_stmt|;
name|print_signature_buffer
argument_list|(
operator|&
name|sigc
argument_list|,
name|verbose
condition|?
name|GPG_VERIFY_VERBOSE
else|:
literal|0
argument_list|)
expr_stmt|;
name|signature_check_clear
argument_list|(
operator|&
name|sigc
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|verify_tag
specifier|static
name|int
name|verify_tag
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
literal|"tag '%s' not found."
argument_list|,
name|name
argument_list|)
return|;
name|type
operator|=
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|!=
name|OBJ_TAG
condition|)
return|return
name|error
argument_list|(
literal|"%s: cannot verify a non-tag object of type %s."
argument_list|,
name|name
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
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
name|ret
operator|=
name|run_gpg_verify
argument_list|(
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
name|verify_tag
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

