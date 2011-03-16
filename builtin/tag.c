begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git tag"  *  * Copyright (c) 2007 Kristian HÃ¸gsberg<krh@redhat.com>,  *                    Carlos Rica<jasampler@gmail.com>  * Based on git-tag.sh and mktag.c by Linus Torvalds.  */
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
file|"refs.h"
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
file|"parse-options.h"
end_include

begin_decl_stmt
DECL|variable|git_tag_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|git_tag_usage
index|[]
init|=
block|{
literal|"git tag [-a|-s|-u<key-id>] [-f] [-m<msg>|-F<file>]<tagname> [<head>]"
block|,
literal|"git tag -d<tagname>..."
block|,
literal|"git tag -l [-n[<num>]] [<pattern>]"
block|,
literal|"git tag -v<tagname>..."
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|signingkey
specifier|static
name|char
name|signingkey
index|[
literal|1000
index|]
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|tag_filter
struct|struct
name|tag_filter
block|{
DECL|member|pattern
specifier|const
name|char
modifier|*
name|pattern
decl_stmt|;
DECL|member|lines
name|int
name|lines
decl_stmt|;
DECL|member|with_commit
name|struct
name|commit_list
modifier|*
name|with_commit
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|show_reference
specifier|static
name|int
name|show_reference
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|flag
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
name|struct
name|tag_filter
modifier|*
name|filter
init|=
name|cb_data
decl_stmt|;
if|if
condition|(
operator|!
name|fnmatch
argument_list|(
name|filter
operator|->
name|pattern
argument_list|,
name|refname
argument_list|,
literal|0
argument_list|)
condition|)
block|{
name|int
name|i
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|,
modifier|*
name|sp
decl_stmt|,
modifier|*
name|eol
decl_stmt|;
name|size_t
name|len
decl_stmt|;
if|if
condition|(
name|filter
operator|->
name|with_commit
condition|)
block|{
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|commit
operator|=
name|lookup_commit_reference_gently
argument_list|(
name|sha1
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|commit
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|is_descendant_of
argument_list|(
name|commit
argument_list|,
name|filter
operator|->
name|with_commit
argument_list|)
condition|)
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|filter
operator|->
name|lines
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|refname
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|printf
argument_list|(
literal|"%-15s "
argument_list|,
name|refname
argument_list|)
expr_stmt|;
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
operator|||
operator|!
name|size
condition|)
return|return
literal|0
return|;
comment|/* skip header */
name|sp
operator|=
name|strstr
argument_list|(
name|buf
argument_list|,
literal|"\n\n"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|sp
condition|)
block|{
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
comment|/* only take up to "lines" lines, and strip the signature */
name|size
operator|=
name|parse_signature
argument_list|(
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
operator|,
name|sp
operator|+=
literal|2
init|;
name|i
operator|<
name|filter
operator|->
name|lines
operator|&&
name|sp
operator|<
name|buf
operator|+
name|size
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|i
condition|)
name|printf
argument_list|(
literal|"\n    "
argument_list|)
expr_stmt|;
name|eol
operator|=
name|memchr
argument_list|(
name|sp
argument_list|,
literal|'\n'
argument_list|,
name|size
operator|-
operator|(
name|sp
operator|-
name|buf
operator|)
argument_list|)
expr_stmt|;
name|len
operator|=
name|eol
condition|?
name|eol
operator|-
name|sp
else|:
name|size
operator|-
operator|(
name|sp
operator|-
name|buf
operator|)
expr_stmt|;
name|fwrite
argument_list|(
name|sp
argument_list|,
name|len
argument_list|,
literal|1
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|eol
condition|)
break|break;
name|sp
operator|=
name|eol
operator|+
literal|1
expr_stmt|;
block|}
name|putchar
argument_list|(
literal|'\n'
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|list_tags
specifier|static
name|int
name|list_tags
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
name|int
name|lines
parameter_list|,
name|struct
name|commit_list
modifier|*
name|with_commit
parameter_list|)
block|{
name|struct
name|tag_filter
name|filter
decl_stmt|;
if|if
condition|(
name|pattern
operator|==
name|NULL
condition|)
name|pattern
operator|=
literal|"*"
expr_stmt|;
name|filter
operator|.
name|pattern
operator|=
name|pattern
expr_stmt|;
name|filter
operator|.
name|lines
operator|=
name|lines
expr_stmt|;
name|filter
operator|.
name|with_commit
operator|=
name|with_commit
expr_stmt|;
name|for_each_tag_ref
argument_list|(
name|show_reference
argument_list|,
operator|(
name|void
operator|*
operator|)
operator|&
name|filter
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_typedef
DECL|typedef|each_tag_name_fn
typedef|typedef
name|int
function_decl|(
modifier|*
name|each_tag_name_fn
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_typedef

begin_function
DECL|function|for_each_tag_name
specifier|static
name|int
name|for_each_tag_name
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|each_tag_name_fn
name|fn
parameter_list|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|p
decl_stmt|;
name|char
name|ref
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|int
name|had_error
init|=
literal|0
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
for|for
control|(
name|p
operator|=
name|argv
init|;
operator|*
name|p
condition|;
name|p
operator|++
control|)
block|{
if|if
condition|(
name|snprintf
argument_list|(
name|ref
argument_list|,
sizeof|sizeof
argument_list|(
name|ref
argument_list|)
argument_list|,
literal|"refs/tags/%s"
argument_list|,
operator|*
name|p
argument_list|)
operator|>=
sizeof|sizeof
argument_list|(
name|ref
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"tag name too long: %.*s..."
argument_list|,
literal|50
argument_list|,
operator|*
name|p
argument_list|)
expr_stmt|;
name|had_error
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|resolve_ref
argument_list|(
name|ref
argument_list|,
name|sha1
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
name|error
argument_list|(
literal|"tag '%s' not found."
argument_list|,
operator|*
name|p
argument_list|)
expr_stmt|;
name|had_error
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|fn
argument_list|(
operator|*
name|p
argument_list|,
name|ref
argument_list|,
name|sha1
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

begin_function
DECL|function|delete_tag
specifier|static
name|int
name|delete_tag
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
if|if
condition|(
name|delete_ref
argument_list|(
name|ref
argument_list|,
name|sha1
argument_list|,
literal|0
argument_list|)
condition|)
return|return
literal|1
return|;
name|printf
argument_list|(
literal|"Deleted tag '%s' (was %s)\n"
argument_list|,
name|name
argument_list|,
name|find_unique_abbrev
argument_list|(
name|sha1
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|0
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
specifier|const
name|char
modifier|*
name|ref
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|argv_verify_tag
index|[]
init|=
block|{
literal|"verify-tag"
block|,
literal|"-v"
block|,
literal|"SHA1_HEX"
block|,
name|NULL
block|}
decl_stmt|;
name|argv_verify_tag
index|[
literal|2
index|]
operator|=
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|run_command_v_opt
argument_list|(
name|argv_verify_tag
argument_list|,
name|RUN_GIT_CMD
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"could not verify the tag '%s'"
argument_list|,
name|name
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|do_sign
specifier|static
name|int
name|do_sign
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buffer
parameter_list|)
block|{
name|struct
name|child_process
name|gpg
decl_stmt|;
specifier|const
name|char
modifier|*
name|args
index|[
literal|4
index|]
decl_stmt|;
name|char
modifier|*
name|bracket
decl_stmt|;
name|int
name|len
decl_stmt|;
name|int
name|i
decl_stmt|,
name|j
decl_stmt|;
if|if
condition|(
operator|!
operator|*
name|signingkey
condition|)
block|{
if|if
condition|(
name|strlcpy
argument_list|(
name|signingkey
argument_list|,
name|git_committer_info
argument_list|(
name|IDENT_ERROR_ON_NO_NAME
argument_list|)
argument_list|,
sizeof|sizeof
argument_list|(
name|signingkey
argument_list|)
argument_list|)
operator|>
sizeof|sizeof
argument_list|(
name|signingkey
argument_list|)
operator|-
literal|1
condition|)
return|return
name|error
argument_list|(
literal|"committer info too long."
argument_list|)
return|;
name|bracket
operator|=
name|strchr
argument_list|(
name|signingkey
argument_list|,
literal|'>'
argument_list|)
expr_stmt|;
if|if
condition|(
name|bracket
condition|)
name|bracket
index|[
literal|1
index|]
operator|=
literal|'\0'
expr_stmt|;
block|}
comment|/* When the username signingkey is bad, program could be terminated 	 * because gpg exits without reading and then write gets SIGPIPE. */
name|signal
argument_list|(
name|SIGPIPE
argument_list|,
name|SIG_IGN
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|gpg
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|gpg
argument_list|)
argument_list|)
expr_stmt|;
name|gpg
operator|.
name|argv
operator|=
name|args
expr_stmt|;
name|gpg
operator|.
name|in
operator|=
operator|-
literal|1
expr_stmt|;
name|gpg
operator|.
name|out
operator|=
operator|-
literal|1
expr_stmt|;
name|args
index|[
literal|0
index|]
operator|=
literal|"gpg"
expr_stmt|;
name|args
index|[
literal|1
index|]
operator|=
literal|"-bsau"
expr_stmt|;
name|args
index|[
literal|2
index|]
operator|=
name|signingkey
expr_stmt|;
name|args
index|[
literal|3
index|]
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|start_command
argument_list|(
operator|&
name|gpg
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"could not run gpg."
argument_list|)
return|;
if|if
condition|(
name|write_in_full
argument_list|(
name|gpg
operator|.
name|in
argument_list|,
name|buffer
operator|->
name|buf
argument_list|,
name|buffer
operator|->
name|len
argument_list|)
operator|!=
name|buffer
operator|->
name|len
condition|)
block|{
name|close
argument_list|(
name|gpg
operator|.
name|in
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|gpg
operator|.
name|out
argument_list|)
expr_stmt|;
name|finish_command
argument_list|(
operator|&
name|gpg
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"gpg did not accept the tag data"
argument_list|)
return|;
block|}
name|close
argument_list|(
name|gpg
operator|.
name|in
argument_list|)
expr_stmt|;
name|len
operator|=
name|strbuf_read
argument_list|(
name|buffer
argument_list|,
name|gpg
operator|.
name|out
argument_list|,
literal|1024
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|gpg
operator|.
name|out
argument_list|)
expr_stmt|;
if|if
condition|(
name|finish_command
argument_list|(
operator|&
name|gpg
argument_list|)
operator|||
operator|!
name|len
operator|||
name|len
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"gpg failed to sign the tag"
argument_list|)
return|;
comment|/* Strip CR from the line endings, in case we are on Windows. */
for|for
control|(
name|i
operator|=
name|j
operator|=
literal|0
init|;
name|i
operator|<
name|buffer
operator|->
name|len
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|buffer
operator|->
name|buf
index|[
name|i
index|]
operator|!=
literal|'\r'
condition|)
block|{
if|if
condition|(
name|i
operator|!=
name|j
condition|)
name|buffer
operator|->
name|buf
index|[
name|j
index|]
operator|=
name|buffer
operator|->
name|buf
index|[
name|i
index|]
expr_stmt|;
name|j
operator|++
expr_stmt|;
block|}
name|strbuf_setlen
argument_list|(
name|buffer
argument_list|,
name|j
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|tag_template
specifier|static
specifier|const
name|char
name|tag_template
index|[]
init|=
literal|"\n"
literal|"#\n"
literal|"# Write a tag message\n"
literal|"#\n"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|set_signingkey
specifier|static
name|void
name|set_signingkey
parameter_list|(
specifier|const
name|char
modifier|*
name|value
parameter_list|)
block|{
if|if
condition|(
name|strlcpy
argument_list|(
name|signingkey
argument_list|,
name|value
argument_list|,
sizeof|sizeof
argument_list|(
name|signingkey
argument_list|)
argument_list|)
operator|>=
sizeof|sizeof
argument_list|(
name|signingkey
argument_list|)
condition|)
name|die
argument_list|(
literal|"signing key value too long (%.10s...)"
argument_list|,
name|value
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_tag_config
specifier|static
name|int
name|git_tag_config
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
literal|"user.signingkey"
argument_list|)
condition|)
block|{
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
name|set_signingkey
argument_list|(
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
DECL|function|write_tag_body
specifier|static
name|void
name|write_tag_body
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|unsigned
name|long
name|size
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|,
modifier|*
name|sp
decl_stmt|;
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
return|return;
comment|/* skip header */
name|sp
operator|=
name|strstr
argument_list|(
name|buf
argument_list|,
literal|"\n\n"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|sp
operator|||
operator|!
name|size
operator|||
name|type
operator|!=
name|OBJ_TAG
condition|)
block|{
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
return|return;
block|}
name|sp
operator|+=
literal|2
expr_stmt|;
comment|/* skip the 2 LFs */
name|write_or_die
argument_list|(
name|fd
argument_list|,
name|sp
argument_list|,
name|parse_signature
argument_list|(
name|sp
argument_list|,
name|buf
operator|+
name|size
operator|-
name|sp
argument_list|)
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|build_tag_object
specifier|static
name|int
name|build_tag_object
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|int
name|sign
parameter_list|,
name|unsigned
name|char
modifier|*
name|result
parameter_list|)
block|{
if|if
condition|(
name|sign
operator|&&
name|do_sign
argument_list|(
name|buf
argument_list|)
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"unable to sign the tag"
argument_list|)
return|;
if|if
condition|(
name|write_sha1_file
argument_list|(
name|buf
operator|->
name|buf
argument_list|,
name|buf
operator|->
name|len
argument_list|,
name|tag_type
argument_list|,
name|result
argument_list|)
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"unable to write tag file"
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|create_tag
specifier|static
name|void
name|create_tag
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|object
parameter_list|,
specifier|const
name|char
modifier|*
name|tag
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|int
name|message
parameter_list|,
name|int
name|sign
parameter_list|,
name|unsigned
name|char
modifier|*
name|prev
parameter_list|,
name|unsigned
name|char
modifier|*
name|result
parameter_list|)
block|{
name|enum
name|object_type
name|type
decl_stmt|;
name|char
name|header_buf
index|[
literal|1024
index|]
decl_stmt|;
name|int
name|header_len
decl_stmt|;
name|char
modifier|*
name|path
init|=
name|NULL
decl_stmt|;
name|type
operator|=
name|sha1_object_info
argument_list|(
name|object
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|type
operator|<=
name|OBJ_NONE
condition|)
name|die
argument_list|(
literal|"bad object type."
argument_list|)
expr_stmt|;
name|header_len
operator|=
name|snprintf
argument_list|(
name|header_buf
argument_list|,
sizeof|sizeof
argument_list|(
name|header_buf
argument_list|)
argument_list|,
literal|"object %s\n"
literal|"type %s\n"
literal|"tag %s\n"
literal|"tagger %s\n\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|object
argument_list|)
argument_list|,
name|typename
argument_list|(
name|type
argument_list|)
argument_list|,
name|tag
argument_list|,
name|git_committer_info
argument_list|(
name|IDENT_ERROR_ON_NO_NAME
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|header_len
operator|>
sizeof|sizeof
argument_list|(
name|header_buf
argument_list|)
operator|-
literal|1
condition|)
name|die
argument_list|(
literal|"tag header too big."
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|message
condition|)
block|{
name|int
name|fd
decl_stmt|;
comment|/* write the template message before editing: */
name|path
operator|=
name|git_pathdup
argument_list|(
literal|"TAG_EDITMSG"
argument_list|)
expr_stmt|;
name|fd
operator|=
name|open
argument_list|(
name|path
argument_list|,
name|O_CREAT
operator||
name|O_TRUNC
operator||
name|O_WRONLY
argument_list|,
literal|0600
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"could not create file '%s'"
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_null_sha1
argument_list|(
name|prev
argument_list|)
condition|)
name|write_tag_body
argument_list|(
name|fd
argument_list|,
name|prev
argument_list|)
expr_stmt|;
else|else
name|write_or_die
argument_list|(
name|fd
argument_list|,
name|tag_template
argument_list|,
name|strlen
argument_list|(
name|tag_template
argument_list|)
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|launch_editor
argument_list|(
name|path
argument_list|,
name|buf
argument_list|,
name|NULL
argument_list|)
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Please supply the message using either -m or -F option.\n"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
name|stripspace
argument_list|(
name|buf
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|message
operator|&&
operator|!
name|buf
operator|->
name|len
condition|)
name|die
argument_list|(
literal|"no tag message?"
argument_list|)
expr_stmt|;
name|strbuf_insert
argument_list|(
name|buf
argument_list|,
literal|0
argument_list|,
name|header_buf
argument_list|,
name|header_len
argument_list|)
expr_stmt|;
if|if
condition|(
name|build_tag_object
argument_list|(
name|buf
argument_list|,
name|sign
argument_list|,
name|result
argument_list|)
operator|<
literal|0
condition|)
block|{
if|if
condition|(
name|path
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"The tag message has been left in %s\n"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|128
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|path
condition|)
block|{
name|unlink_or_warn
argument_list|(
name|path
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|path
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_struct
DECL|struct|msg_arg
struct|struct
name|msg_arg
block|{
DECL|member|given
name|int
name|given
decl_stmt|;
DECL|member|buf
name|struct
name|strbuf
name|buf
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|parse_msg_arg
specifier|static
name|int
name|parse_msg_arg
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
name|struct
name|msg_arg
modifier|*
name|msg
init|=
name|opt
operator|->
name|value
decl_stmt|;
if|if
condition|(
operator|!
name|arg
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|msg
operator|->
name|buf
operator|.
name|len
condition|)
name|strbuf_addstr
argument_list|(
operator|&
operator|(
name|msg
operator|->
name|buf
operator|)
argument_list|,
literal|"\n\n"
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
operator|(
name|msg
operator|->
name|buf
operator|)
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|msg
operator|->
name|given
operator|=
literal|1
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_tag
name|int
name|cmd_tag
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
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|unsigned
name|char
name|object
index|[
literal|20
index|]
decl_stmt|,
name|prev
index|[
literal|20
index|]
decl_stmt|;
name|char
name|ref
index|[
name|PATH_MAX
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|object_ref
decl_stmt|,
modifier|*
name|tag
decl_stmt|;
name|struct
name|ref_lock
modifier|*
name|lock
decl_stmt|;
name|int
name|annotate
init|=
literal|0
decl_stmt|,
name|sign
init|=
literal|0
decl_stmt|,
name|force
init|=
literal|0
decl_stmt|,
name|lines
init|=
operator|-
literal|1
decl_stmt|,
name|list
init|=
literal|0
decl_stmt|,
name|delete
init|=
literal|0
decl_stmt|,
name|verify
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|msgfile
init|=
name|NULL
decl_stmt|,
modifier|*
name|keyid
init|=
name|NULL
decl_stmt|;
name|struct
name|msg_arg
name|msg
init|=
block|{
literal|0
block|,
name|STRBUF_INIT
block|}
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|with_commit
init|=
name|NULL
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_BOOLEAN
argument_list|(
literal|'l'
argument_list|,
name|NULL
argument_list|,
operator|&
name|list
argument_list|,
literal|"list tag names"
argument_list|)
block|,
block|{
name|OPTION_INTEGER
block|,
literal|'n'
block|,
name|NULL
block|,
operator|&
name|lines
block|,
literal|"n"
block|,
literal|"print<n> lines of each tag message"
block|,
name|PARSE_OPT_OPTARG
block|,
name|NULL
block|,
literal|1
block|}
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'d'
argument_list|,
name|NULL
argument_list|,
operator|&
name|delete
argument_list|,
literal|"delete tags"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'v'
argument_list|,
name|NULL
argument_list|,
operator|&
name|verify
argument_list|,
literal|"verify tags"
argument_list|)
block|,
name|OPT_GROUP
argument_list|(
literal|"Tag creation options"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'a'
argument_list|,
name|NULL
argument_list|,
operator|&
name|annotate
argument_list|,
literal|"annotated tag, needs a message"
argument_list|)
block|,
name|OPT_CALLBACK
argument_list|(
literal|'m'
argument_list|,
name|NULL
argument_list|,
operator|&
name|msg
argument_list|,
literal|"message"
argument_list|,
literal|"tag message"
argument_list|,
name|parse_msg_arg
argument_list|)
block|,
name|OPT_FILENAME
argument_list|(
literal|'F'
argument_list|,
name|NULL
argument_list|,
operator|&
name|msgfile
argument_list|,
literal|"read message from file"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'s'
argument_list|,
name|NULL
argument_list|,
operator|&
name|sign
argument_list|,
literal|"annotated and GPG-signed tag"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|'u'
argument_list|,
name|NULL
argument_list|,
operator|&
name|keyid
argument_list|,
literal|"key-id"
argument_list|,
literal|"use another key to sign the tag"
argument_list|)
block|,
name|OPT__FORCE
argument_list|(
operator|&
name|force
argument_list|,
literal|"replace the tag if exists"
argument_list|)
block|,
name|OPT_GROUP
argument_list|(
literal|"Tag listing options"
argument_list|)
block|,
block|{
name|OPTION_CALLBACK
block|,
literal|0
block|,
literal|"contains"
block|,
operator|&
name|with_commit
block|,
literal|"commit"
block|,
literal|"print only tags that contain the commit"
block|,
name|PARSE_OPT_LASTARG_DEFAULT
block|,
name|parse_opt_with_commit
block|,
operator|(
name|intptr_t
operator|)
literal|"HEAD"
block|, 		}
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|git_config
argument_list|(
name|git_tag_config
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
name|options
argument_list|,
name|git_tag_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|keyid
condition|)
block|{
name|sign
operator|=
literal|1
expr_stmt|;
name|set_signingkey
argument_list|(
name|keyid
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|sign
condition|)
name|annotate
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|argc
operator|==
literal|0
operator|&&
operator|!
operator|(
name|delete
operator|||
name|verify
operator|)
condition|)
name|list
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|(
name|annotate
operator|||
name|msg
operator|.
name|given
operator|||
name|msgfile
operator|||
name|force
operator|)
operator|&&
operator|(
name|list
operator|||
name|delete
operator|||
name|verify
operator|)
condition|)
name|usage_with_options
argument_list|(
name|git_tag_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
name|list
operator|+
name|delete
operator|+
name|verify
operator|>
literal|1
condition|)
name|usage_with_options
argument_list|(
name|git_tag_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
name|list
condition|)
return|return
name|list_tags
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|,
name|lines
operator|==
operator|-
literal|1
condition|?
literal|0
else|:
name|lines
argument_list|,
name|with_commit
argument_list|)
return|;
if|if
condition|(
name|lines
operator|!=
operator|-
literal|1
condition|)
name|die
argument_list|(
literal|"-n option is only allowed with -l."
argument_list|)
expr_stmt|;
if|if
condition|(
name|with_commit
condition|)
name|die
argument_list|(
literal|"--contains option is only allowed with -l."
argument_list|)
expr_stmt|;
if|if
condition|(
name|delete
condition|)
return|return
name|for_each_tag_name
argument_list|(
name|argv
argument_list|,
name|delete_tag
argument_list|)
return|;
if|if
condition|(
name|verify
condition|)
return|return
name|for_each_tag_name
argument_list|(
name|argv
argument_list|,
name|verify_tag
argument_list|)
return|;
if|if
condition|(
name|msg
operator|.
name|given
operator|||
name|msgfile
condition|)
block|{
if|if
condition|(
name|msg
operator|.
name|given
operator|&&
name|msgfile
condition|)
name|die
argument_list|(
literal|"only one -F or -m option is allowed."
argument_list|)
expr_stmt|;
name|annotate
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|msg
operator|.
name|given
condition|)
name|strbuf_addbuf
argument_list|(
operator|&
name|buf
argument_list|,
operator|&
operator|(
name|msg
operator|.
name|buf
operator|)
argument_list|)
expr_stmt|;
else|else
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|msgfile
argument_list|,
literal|"-"
argument_list|)
condition|)
block|{
if|if
condition|(
name|strbuf_read
argument_list|(
operator|&
name|buf
argument_list|,
literal|0
argument_list|,
literal|1024
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"cannot read '%s'"
argument_list|,
name|msgfile
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|strbuf_read_file
argument_list|(
operator|&
name|buf
argument_list|,
name|msgfile
argument_list|,
literal|1024
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"could not open or read '%s'"
argument_list|,
name|msgfile
argument_list|)
expr_stmt|;
block|}
block|}
block|}
name|tag
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
name|object_ref
operator|=
name|argc
operator|==
literal|2
condition|?
name|argv
index|[
literal|1
index|]
else|:
literal|"HEAD"
expr_stmt|;
if|if
condition|(
name|argc
operator|>
literal|2
condition|)
name|die
argument_list|(
literal|"too many params"
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|object_ref
argument_list|,
name|object
argument_list|)
condition|)
name|die
argument_list|(
literal|"Failed to resolve '%s' as a valid ref."
argument_list|,
name|object_ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|snprintf
argument_list|(
name|ref
argument_list|,
sizeof|sizeof
argument_list|(
name|ref
argument_list|)
argument_list|,
literal|"refs/tags/%s"
argument_list|,
name|tag
argument_list|)
operator|>
sizeof|sizeof
argument_list|(
name|ref
argument_list|)
operator|-
literal|1
condition|)
name|die
argument_list|(
literal|"tag name too long: %.*s..."
argument_list|,
literal|50
argument_list|,
name|tag
argument_list|)
expr_stmt|;
if|if
condition|(
name|check_ref_format
argument_list|(
name|ref
argument_list|)
condition|)
name|die
argument_list|(
literal|"'%s' is not a valid tag name."
argument_list|,
name|tag
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|resolve_ref
argument_list|(
name|ref
argument_list|,
name|prev
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|)
condition|)
name|hashclr
argument_list|(
name|prev
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|force
condition|)
name|die
argument_list|(
literal|"tag '%s' already exists"
argument_list|,
name|tag
argument_list|)
expr_stmt|;
if|if
condition|(
name|annotate
condition|)
name|create_tag
argument_list|(
name|object
argument_list|,
name|tag
argument_list|,
operator|&
name|buf
argument_list|,
name|msg
operator|.
name|given
operator|||
name|msgfile
argument_list|,
name|sign
argument_list|,
name|prev
argument_list|,
name|object
argument_list|)
expr_stmt|;
name|lock
operator|=
name|lock_any_ref_for_update
argument_list|(
name|ref
argument_list|,
name|prev
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|lock
condition|)
name|die
argument_list|(
literal|"%s: cannot lock the ref"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|write_ref_sha1
argument_list|(
name|lock
argument_list|,
name|object
argument_list|,
name|NULL
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"%s: cannot update the ref"
argument_list|,
name|ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|force
operator|&&
name|hashcmp
argument_list|(
name|prev
argument_list|,
name|object
argument_list|)
condition|)
name|printf
argument_list|(
literal|"Updated tag '%s' (was %s)\n"
argument_list|,
name|tag
argument_list|,
name|find_unique_abbrev
argument_list|(
name|prev
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

