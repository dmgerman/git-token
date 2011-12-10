begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"credential.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"url.h"
end_include

begin_include
include|#
directive|include
file|"prompt.h"
end_include

begin_function
DECL|function|credential_init
name|void
name|credential_init
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|memset
argument_list|(
name|c
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|c
argument_list|)
argument_list|)
expr_stmt|;
name|c
operator|->
name|helpers
operator|.
name|strdup_strings
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_clear
name|void
name|credential_clear
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|free
argument_list|(
name|c
operator|->
name|protocol
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|c
operator|->
name|host
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|c
operator|->
name|path
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|c
operator|->
name|username
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|c
operator|->
name|password
argument_list|)
expr_stmt|;
name|string_list_clear
argument_list|(
operator|&
name|c
operator|->
name|helpers
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|credential_init
argument_list|(
name|c
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_match
name|int
name|credential_match
parameter_list|(
specifier|const
name|struct
name|credential
modifier|*
name|want
parameter_list|,
specifier|const
name|struct
name|credential
modifier|*
name|have
parameter_list|)
block|{
DECL|macro|CHECK
define|#
directive|define
name|CHECK
parameter_list|(
name|x
parameter_list|)
value|(!want->x || (have->x&& !strcmp(want->x, have->x)))
return|return
name|CHECK
argument_list|(
name|protocol
argument_list|)
operator|&&
name|CHECK
argument_list|(
name|host
argument_list|)
operator|&&
name|CHECK
argument_list|(
name|path
argument_list|)
operator|&&
name|CHECK
argument_list|(
name|username
argument_list|)
return|;
DECL|macro|CHECK
undef|#
directive|undef
name|CHECK
block|}
end_function

begin_function
DECL|function|credential_config_callback
specifier|static
name|int
name|credential_config_callback
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
name|data
parameter_list|)
block|{
name|struct
name|credential
modifier|*
name|c
init|=
name|data
decl_stmt|;
specifier|const
name|char
modifier|*
name|key
decl_stmt|,
modifier|*
name|dot
decl_stmt|;
name|key
operator|=
name|skip_prefix
argument_list|(
name|var
argument_list|,
literal|"credential."
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|key
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|value
condition|)
return|return
name|config_error_nonbool
argument_list|(
name|var
argument_list|)
return|;
name|dot
operator|=
name|strrchr
argument_list|(
name|key
argument_list|,
literal|'.'
argument_list|)
expr_stmt|;
if|if
condition|(
name|dot
condition|)
block|{
name|struct
name|credential
name|want
init|=
name|CREDENTIAL_INIT
decl_stmt|;
name|char
modifier|*
name|url
init|=
name|xmemdupz
argument_list|(
name|key
argument_list|,
name|dot
operator|-
name|key
argument_list|)
decl_stmt|;
name|int
name|matched
decl_stmt|;
name|credential_from_url
argument_list|(
operator|&
name|want
argument_list|,
name|url
argument_list|)
expr_stmt|;
name|matched
operator|=
name|credential_match
argument_list|(
operator|&
name|want
argument_list|,
name|c
argument_list|)
expr_stmt|;
name|credential_clear
argument_list|(
operator|&
name|want
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|url
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|matched
condition|)
return|return
literal|0
return|;
name|key
operator|=
name|dot
operator|+
literal|1
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"helper"
argument_list|)
condition|)
name|string_list_append
argument_list|(
operator|&
name|c
operator|->
name|helpers
argument_list|,
name|value
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"username"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|c
operator|->
name|username
condition|)
name|c
operator|->
name|username
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"usehttppath"
argument_list|)
condition|)
name|c
operator|->
name|use_http_path
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
end_function

begin_function
DECL|function|proto_is_http
specifier|static
name|int
name|proto_is_http
parameter_list|(
specifier|const
name|char
modifier|*
name|s
parameter_list|)
block|{
if|if
condition|(
operator|!
name|s
condition|)
return|return
literal|0
return|;
return|return
operator|!
name|strcmp
argument_list|(
name|s
argument_list|,
literal|"https"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|s
argument_list|,
literal|"http"
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|credential_apply_config
specifier|static
name|void
name|credential_apply_config
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
if|if
condition|(
name|c
operator|->
name|configured
condition|)
return|return;
name|git_config
argument_list|(
name|credential_config_callback
argument_list|,
name|c
argument_list|)
expr_stmt|;
name|c
operator|->
name|configured
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|c
operator|->
name|use_http_path
operator|&&
name|proto_is_http
argument_list|(
name|c
operator|->
name|protocol
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|c
operator|->
name|path
argument_list|)
expr_stmt|;
name|c
operator|->
name|path
operator|=
name|NULL
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|credential_describe
specifier|static
name|void
name|credential_describe
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|,
name|struct
name|strbuf
modifier|*
name|out
parameter_list|)
block|{
if|if
condition|(
operator|!
name|c
operator|->
name|protocol
condition|)
return|return;
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|"%s://"
argument_list|,
name|c
operator|->
name|protocol
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|username
operator|&&
operator|*
name|c
operator|->
name|username
condition|)
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|"%s@"
argument_list|,
name|c
operator|->
name|username
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|host
condition|)
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
name|c
operator|->
name|host
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|path
condition|)
name|strbuf_addf
argument_list|(
name|out
argument_list|,
literal|"/%s"
argument_list|,
name|c
operator|->
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_ask_one
specifier|static
name|char
modifier|*
name|credential_ask_one
parameter_list|(
specifier|const
name|char
modifier|*
name|what
parameter_list|,
name|struct
name|credential
modifier|*
name|c
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|struct
name|strbuf
name|desc
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|prompt
init|=
name|STRBUF_INIT
decl_stmt|;
name|char
modifier|*
name|r
decl_stmt|;
name|credential_describe
argument_list|(
name|c
argument_list|,
operator|&
name|desc
argument_list|)
expr_stmt|;
if|if
condition|(
name|desc
operator|.
name|len
condition|)
name|strbuf_addf
argument_list|(
operator|&
name|prompt
argument_list|,
literal|"%s for '%s': "
argument_list|,
name|what
argument_list|,
name|desc
operator|.
name|buf
argument_list|)
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
operator|&
name|prompt
argument_list|,
literal|"%s: "
argument_list|,
name|what
argument_list|)
expr_stmt|;
name|r
operator|=
name|git_prompt
argument_list|(
name|prompt
operator|.
name|buf
argument_list|,
name|flags
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|desc
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|prompt
argument_list|)
expr_stmt|;
return|return
name|xstrdup
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|credential_getpass
specifier|static
name|void
name|credential_getpass
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
if|if
condition|(
operator|!
name|c
operator|->
name|username
condition|)
name|c
operator|->
name|username
operator|=
name|credential_ask_one
argument_list|(
literal|"Username"
argument_list|,
name|c
argument_list|,
name|PROMPT_ASKPASS
operator||
name|PROMPT_ECHO
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|c
operator|->
name|password
condition|)
name|c
operator|->
name|password
operator|=
name|credential_ask_one
argument_list|(
literal|"Password"
argument_list|,
name|c
argument_list|,
name|PROMPT_ASKPASS
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_read
name|int
name|credential_read
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|,
name|FILE
modifier|*
name|fp
parameter_list|)
block|{
name|struct
name|strbuf
name|line
init|=
name|STRBUF_INIT
decl_stmt|;
while|while
condition|(
name|strbuf_getline
argument_list|(
operator|&
name|line
argument_list|,
name|fp
argument_list|,
literal|'\n'
argument_list|)
operator|!=
name|EOF
condition|)
block|{
name|char
modifier|*
name|key
init|=
name|line
operator|.
name|buf
decl_stmt|;
name|char
modifier|*
name|value
init|=
name|strchr
argument_list|(
name|key
argument_list|,
literal|'='
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|line
operator|.
name|len
condition|)
break|break;
if|if
condition|(
operator|!
name|value
condition|)
block|{
name|warning
argument_list|(
literal|"invalid credential line: %s"
argument_list|,
name|key
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|line
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
operator|*
name|value
operator|++
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"username"
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|c
operator|->
name|username
argument_list|)
expr_stmt|;
name|c
operator|->
name|username
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"password"
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|c
operator|->
name|password
argument_list|)
expr_stmt|;
name|c
operator|->
name|password
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"protocol"
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|c
operator|->
name|protocol
argument_list|)
expr_stmt|;
name|c
operator|->
name|protocol
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"host"
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|c
operator|->
name|host
argument_list|)
expr_stmt|;
name|c
operator|->
name|host
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|key
argument_list|,
literal|"path"
argument_list|)
condition|)
block|{
name|free
argument_list|(
name|c
operator|->
name|path
argument_list|)
expr_stmt|;
name|c
operator|->
name|path
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
block|}
comment|/* 		 * Ignore other lines; we don't know what they mean, but 		 * this future-proofs us when later versions of git do 		 * learn new lines, and the helpers are updated to match. 		 */
block|}
name|strbuf_release
argument_list|(
operator|&
name|line
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|credential_write_item
specifier|static
name|void
name|credential_write_item
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|key
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
if|if
condition|(
operator|!
name|value
condition|)
return|return;
name|fprintf
argument_list|(
name|fp
argument_list|,
literal|"%s=%s\n"
argument_list|,
name|key
argument_list|,
name|value
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_write
specifier|static
name|void
name|credential_write
parameter_list|(
specifier|const
name|struct
name|credential
modifier|*
name|c
parameter_list|,
name|FILE
modifier|*
name|fp
parameter_list|)
block|{
name|credential_write_item
argument_list|(
name|fp
argument_list|,
literal|"protocol"
argument_list|,
name|c
operator|->
name|protocol
argument_list|)
expr_stmt|;
name|credential_write_item
argument_list|(
name|fp
argument_list|,
literal|"host"
argument_list|,
name|c
operator|->
name|host
argument_list|)
expr_stmt|;
name|credential_write_item
argument_list|(
name|fp
argument_list|,
literal|"path"
argument_list|,
name|c
operator|->
name|path
argument_list|)
expr_stmt|;
name|credential_write_item
argument_list|(
name|fp
argument_list|,
literal|"username"
argument_list|,
name|c
operator|->
name|username
argument_list|)
expr_stmt|;
name|credential_write_item
argument_list|(
name|fp
argument_list|,
literal|"password"
argument_list|,
name|c
operator|->
name|password
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|run_credential_helper
specifier|static
name|int
name|run_credential_helper
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|,
specifier|const
name|char
modifier|*
name|cmd
parameter_list|,
name|int
name|want_output
parameter_list|)
block|{
name|struct
name|child_process
name|helper
decl_stmt|;
specifier|const
name|char
modifier|*
name|argv
index|[]
init|=
block|{
name|NULL
block|,
name|NULL
block|}
decl_stmt|;
name|FILE
modifier|*
name|fp
decl_stmt|;
name|memset
argument_list|(
operator|&
name|helper
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|helper
argument_list|)
argument_list|)
expr_stmt|;
name|argv
index|[
literal|0
index|]
operator|=
name|cmd
expr_stmt|;
name|helper
operator|.
name|argv
operator|=
name|argv
expr_stmt|;
name|helper
operator|.
name|use_shell
operator|=
literal|1
expr_stmt|;
name|helper
operator|.
name|in
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|want_output
condition|)
name|helper
operator|.
name|out
operator|=
operator|-
literal|1
expr_stmt|;
else|else
name|helper
operator|.
name|no_stdout
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|start_command
argument_list|(
operator|&
name|helper
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
name|fp
operator|=
name|xfdopen
argument_list|(
name|helper
operator|.
name|in
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
name|credential_write
argument_list|(
name|c
argument_list|,
name|fp
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|fp
argument_list|)
expr_stmt|;
if|if
condition|(
name|want_output
condition|)
block|{
name|int
name|r
decl_stmt|;
name|fp
operator|=
name|xfdopen
argument_list|(
name|helper
operator|.
name|out
argument_list|,
literal|"r"
argument_list|)
expr_stmt|;
name|r
operator|=
name|credential_read
argument_list|(
name|c
argument_list|,
name|fp
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|fp
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|<
literal|0
condition|)
block|{
name|finish_command
argument_list|(
operator|&
name|helper
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
block|}
if|if
condition|(
name|finish_command
argument_list|(
operator|&
name|helper
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|credential_do
specifier|static
name|int
name|credential_do
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|,
specifier|const
name|char
modifier|*
name|helper
parameter_list|,
specifier|const
name|char
modifier|*
name|operation
parameter_list|)
block|{
name|struct
name|strbuf
name|cmd
init|=
name|STRBUF_INIT
decl_stmt|;
name|int
name|r
decl_stmt|;
if|if
condition|(
name|helper
index|[
literal|0
index|]
operator|==
literal|'!'
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|cmd
argument_list|,
name|helper
operator|+
literal|1
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|is_absolute_path
argument_list|(
name|helper
argument_list|)
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|cmd
argument_list|,
name|helper
argument_list|)
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
operator|&
name|cmd
argument_list|,
literal|"git credential-%s"
argument_list|,
name|helper
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|cmd
argument_list|,
literal|" %s"
argument_list|,
name|operation
argument_list|)
expr_stmt|;
name|r
operator|=
name|run_credential_helper
argument_list|(
name|c
argument_list|,
name|cmd
operator|.
name|buf
argument_list|,
operator|!
name|strcmp
argument_list|(
name|operation
argument_list|,
literal|"get"
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|cmd
argument_list|)
expr_stmt|;
return|return
name|r
return|;
block|}
end_function

begin_function
DECL|function|credential_fill
name|void
name|credential_fill
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
name|c
operator|->
name|username
operator|&&
name|c
operator|->
name|password
condition|)
return|return;
name|credential_apply_config
argument_list|(
name|c
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
name|c
operator|->
name|helpers
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|credential_do
argument_list|(
name|c
argument_list|,
name|c
operator|->
name|helpers
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|,
literal|"get"
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|username
operator|&&
name|c
operator|->
name|password
condition|)
return|return;
block|}
name|credential_getpass
argument_list|(
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|c
operator|->
name|username
operator|&&
operator|!
name|c
operator|->
name|password
condition|)
name|die
argument_list|(
literal|"unable to get password from user"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_approve
name|void
name|credential_approve
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
name|c
operator|->
name|approved
condition|)
return|return;
if|if
condition|(
operator|!
name|c
operator|->
name|username
operator|||
operator|!
name|c
operator|->
name|password
condition|)
return|return;
name|credential_apply_config
argument_list|(
name|c
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
name|c
operator|->
name|helpers
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|credential_do
argument_list|(
name|c
argument_list|,
name|c
operator|->
name|helpers
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|,
literal|"store"
argument_list|)
expr_stmt|;
name|c
operator|->
name|approved
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_reject
name|void
name|credential_reject
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|credential_apply_config
argument_list|(
name|c
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
name|c
operator|->
name|helpers
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|credential_do
argument_list|(
name|c
argument_list|,
name|c
operator|->
name|helpers
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
argument_list|,
literal|"erase"
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|c
operator|->
name|username
argument_list|)
expr_stmt|;
name|c
operator|->
name|username
operator|=
name|NULL
expr_stmt|;
name|free
argument_list|(
name|c
operator|->
name|password
argument_list|)
expr_stmt|;
name|c
operator|->
name|password
operator|=
name|NULL
expr_stmt|;
name|c
operator|->
name|approved
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|credential_from_url
name|void
name|credential_from_url
parameter_list|(
name|struct
name|credential
modifier|*
name|c
parameter_list|,
specifier|const
name|char
modifier|*
name|url
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|at
decl_stmt|,
modifier|*
name|colon
decl_stmt|,
modifier|*
name|cp
decl_stmt|,
modifier|*
name|slash
decl_stmt|,
modifier|*
name|host
decl_stmt|,
modifier|*
name|proto_end
decl_stmt|;
name|credential_clear
argument_list|(
name|c
argument_list|)
expr_stmt|;
comment|/* 	 * Match one of: 	 *   (1) proto://<host>/... 	 *   (2) proto://<user>@<host>/... 	 *   (3) proto://<user>:<pass>@<host>/... 	 */
name|proto_end
operator|=
name|strstr
argument_list|(
name|url
argument_list|,
literal|"://"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|proto_end
condition|)
return|return;
name|cp
operator|=
name|proto_end
operator|+
literal|3
expr_stmt|;
name|at
operator|=
name|strchr
argument_list|(
name|cp
argument_list|,
literal|'@'
argument_list|)
expr_stmt|;
name|colon
operator|=
name|strchr
argument_list|(
name|cp
argument_list|,
literal|':'
argument_list|)
expr_stmt|;
name|slash
operator|=
name|strchrnul
argument_list|(
name|cp
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|at
operator|||
name|slash
operator|<=
name|at
condition|)
block|{
comment|/* Case (1) */
name|host
operator|=
name|cp
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|colon
operator|||
name|at
operator|<=
name|colon
condition|)
block|{
comment|/* Case (2) */
name|c
operator|->
name|username
operator|=
name|url_decode_mem
argument_list|(
name|cp
argument_list|,
name|at
operator|-
name|cp
argument_list|)
expr_stmt|;
name|host
operator|=
name|at
operator|+
literal|1
expr_stmt|;
block|}
else|else
block|{
comment|/* Case (3) */
name|c
operator|->
name|username
operator|=
name|url_decode_mem
argument_list|(
name|cp
argument_list|,
name|colon
operator|-
name|cp
argument_list|)
expr_stmt|;
name|c
operator|->
name|password
operator|=
name|url_decode_mem
argument_list|(
name|colon
operator|+
literal|1
argument_list|,
name|at
operator|-
operator|(
name|colon
operator|+
literal|1
operator|)
argument_list|)
expr_stmt|;
name|host
operator|=
name|at
operator|+
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|proto_end
operator|-
name|url
operator|>
literal|0
condition|)
name|c
operator|->
name|protocol
operator|=
name|xmemdupz
argument_list|(
name|url
argument_list|,
name|proto_end
operator|-
name|url
argument_list|)
expr_stmt|;
if|if
condition|(
name|slash
operator|-
name|host
operator|>
literal|0
condition|)
name|c
operator|->
name|host
operator|=
name|url_decode_mem
argument_list|(
name|host
argument_list|,
name|slash
operator|-
name|host
argument_list|)
expr_stmt|;
comment|/* Trim leading and trailing slashes from path */
while|while
condition|(
operator|*
name|slash
operator|==
literal|'/'
condition|)
name|slash
operator|++
expr_stmt|;
if|if
condition|(
operator|*
name|slash
condition|)
block|{
name|char
modifier|*
name|p
decl_stmt|;
name|c
operator|->
name|path
operator|=
name|url_decode
argument_list|(
name|slash
argument_list|)
expr_stmt|;
name|p
operator|=
name|c
operator|->
name|path
operator|+
name|strlen
argument_list|(
name|c
operator|->
name|path
argument_list|)
operator|-
literal|1
expr_stmt|;
while|while
condition|(
name|p
operator|>
name|c
operator|->
name|path
operator|&&
operator|*
name|p
operator|==
literal|'/'
condition|)
operator|*
name|p
operator|--
operator|=
literal|'\0'
expr_stmt|;
block|}
block|}
end_function

end_unit

