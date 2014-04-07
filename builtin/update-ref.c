begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_include
include|#
directive|include
file|"argv-array.h"
end_include

begin_decl_stmt
DECL|variable|git_update_ref_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|git_update_ref_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git update-ref [options] -d<refname> [<oldval>]"
argument_list|)
block|,
name|N_
argument_list|(
literal|"git update-ref [options]<refname><newval> [<oldval>]"
argument_list|)
block|,
name|N_
argument_list|(
literal|"git update-ref [options] --stdin [-z]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|updates_alloc
specifier|static
name|int
name|updates_alloc
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|updates_count
specifier|static
name|int
name|updates_count
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|updates
specifier|static
name|struct
name|ref_update
modifier|*
modifier|*
name|updates
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|line_termination
specifier|static
name|char
name|line_termination
init|=
literal|'\n'
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|update_flags
specifier|static
name|int
name|update_flags
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|update_alloc
specifier|static
name|struct
name|ref_update
modifier|*
name|update_alloc
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|ref_update
modifier|*
name|update
decl_stmt|;
comment|/* Allocate and zero-init a struct ref_update */
name|update
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|update
argument_list|)
argument_list|)
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|updates
argument_list|,
name|updates_count
operator|+
literal|1
argument_list|,
name|updates_alloc
argument_list|)
expr_stmt|;
name|updates
index|[
name|updates_count
operator|++
index|]
operator|=
name|update
expr_stmt|;
comment|/* Store and reset accumulated options */
name|update
operator|->
name|flags
operator|=
name|update_flags
expr_stmt|;
name|update_flags
operator|=
literal|0
expr_stmt|;
return|return
name|update
return|;
block|}
end_function

begin_function
DECL|function|update_store_new_sha1
specifier|static
name|void
name|update_store_new_sha1
parameter_list|(
specifier|const
name|char
modifier|*
name|command
parameter_list|,
name|struct
name|ref_update
modifier|*
name|update
parameter_list|,
specifier|const
name|char
modifier|*
name|newvalue
parameter_list|)
block|{
if|if
condition|(
operator|*
name|newvalue
operator|&&
name|get_sha1
argument_list|(
name|newvalue
argument_list|,
name|update
operator|->
name|new_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s %s: invalid new value: %s"
argument_list|,
name|command
argument_list|,
name|update
operator|->
name|ref_name
argument_list|,
name|newvalue
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|update_store_old_sha1
specifier|static
name|void
name|update_store_old_sha1
parameter_list|(
specifier|const
name|char
modifier|*
name|command
parameter_list|,
name|struct
name|ref_update
modifier|*
name|update
parameter_list|,
specifier|const
name|char
modifier|*
name|oldvalue
parameter_list|)
block|{
if|if
condition|(
operator|*
name|oldvalue
operator|&&
name|get_sha1
argument_list|(
name|oldvalue
argument_list|,
name|update
operator|->
name|old_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s %s: invalid old value: %s"
argument_list|,
name|command
argument_list|,
name|update
operator|->
name|ref_name
argument_list|,
name|oldvalue
argument_list|)
expr_stmt|;
comment|/* We have an old value if non-empty, or if empty without -z */
name|update
operator|->
name|have_old
operator|=
operator|*
name|oldvalue
operator|||
name|line_termination
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * Parse one whitespace- or NUL-terminated, possibly C-quoted argument  * and append the result to arg.  Return a pointer to the terminator.  * Die if there is an error in how the argument is C-quoted.  This  * function is only used if not -z.  */
end_comment

begin_function
DECL|function|parse_arg
specifier|static
specifier|const
name|char
modifier|*
name|parse_arg
parameter_list|(
specifier|const
name|char
modifier|*
name|next
parameter_list|,
name|struct
name|strbuf
modifier|*
name|arg
parameter_list|)
block|{
if|if
condition|(
operator|*
name|next
operator|==
literal|'"'
condition|)
block|{
specifier|const
name|char
modifier|*
name|orig
init|=
name|next
decl_stmt|;
if|if
condition|(
name|unquote_c_style
argument_list|(
name|arg
argument_list|,
name|next
argument_list|,
operator|&
name|next
argument_list|)
condition|)
name|die
argument_list|(
literal|"badly quoted argument: %s"
argument_list|,
name|orig
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|next
operator|&&
operator|!
name|isspace
argument_list|(
operator|*
name|next
argument_list|)
condition|)
name|die
argument_list|(
literal|"unexpected character after quoted argument: %s"
argument_list|,
name|orig
argument_list|)
expr_stmt|;
block|}
else|else
block|{
while|while
condition|(
operator|*
name|next
operator|&&
operator|!
name|isspace
argument_list|(
operator|*
name|next
argument_list|)
condition|)
name|strbuf_addch
argument_list|(
name|arg
argument_list|,
operator|*
name|next
operator|++
argument_list|)
expr_stmt|;
block|}
return|return
name|next
return|;
block|}
end_function

begin_comment
comment|/*  * Parse the reference name immediately after "command SP".  If not  * -z, then handle C-quoting.  Return a pointer to a newly allocated  * string containing the name of the reference, or NULL if there was  * an error.  Update *next to point at the character that terminates  * the argument.  Die if C-quoting is malformed or the reference name  * is invalid.  */
end_comment

begin_function
DECL|function|parse_refname
specifier|static
name|char
modifier|*
name|parse_refname
parameter_list|(
name|struct
name|strbuf
modifier|*
name|input
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|next
parameter_list|)
block|{
name|struct
name|strbuf
name|ref
init|=
name|STRBUF_INIT
decl_stmt|;
if|if
condition|(
name|line_termination
condition|)
block|{
comment|/* Without -z, use the next argument */
operator|*
name|next
operator|=
name|parse_arg
argument_list|(
operator|*
name|next
argument_list|,
operator|&
name|ref
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/* With -z, use everything up to the next NUL */
name|strbuf_addstr
argument_list|(
operator|&
name|ref
argument_list|,
operator|*
name|next
argument_list|)
expr_stmt|;
operator|*
name|next
operator|+=
name|ref
operator|.
name|len
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|ref
operator|.
name|len
condition|)
block|{
name|strbuf_release
argument_list|(
operator|&
name|ref
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
if|if
condition|(
name|check_refname_format
argument_list|(
name|ref
operator|.
name|buf
argument_list|,
name|REFNAME_ALLOW_ONELEVEL
argument_list|)
condition|)
name|die
argument_list|(
literal|"invalid ref format: %s"
argument_list|,
name|ref
operator|.
name|buf
argument_list|)
expr_stmt|;
return|return
name|strbuf_detach
argument_list|(
operator|&
name|ref
argument_list|,
name|NULL
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * Parse a SP/NUL separator followed by the next SP- or NUL-terminated  * argument, if any.  If there is an argument, write it to arg, set  * *next to point at the character terminating the argument, and  * return 0.  If there is no argument at all (not even the empty  * string), return a non-zero result and leave *next unchanged.  */
end_comment

begin_function
DECL|function|parse_next_arg
specifier|static
name|int
name|parse_next_arg
parameter_list|(
name|struct
name|strbuf
modifier|*
name|input
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|next
parameter_list|,
name|struct
name|strbuf
modifier|*
name|arg
parameter_list|)
block|{
name|strbuf_reset
argument_list|(
name|arg
argument_list|)
expr_stmt|;
if|if
condition|(
name|line_termination
condition|)
block|{
comment|/* Without -z, consume SP and use next argument */
if|if
condition|(
operator|!
operator|*
operator|*
name|next
operator|||
operator|*
operator|*
name|next
operator|==
name|line_termination
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|*
operator|*
name|next
operator|!=
literal|' '
condition|)
name|die
argument_list|(
literal|"expected SP but got: %s"
argument_list|,
operator|*
name|next
argument_list|)
expr_stmt|;
operator|(
operator|*
name|next
operator|)
operator|++
expr_stmt|;
operator|*
name|next
operator|=
name|parse_arg
argument_list|(
operator|*
name|next
argument_list|,
name|arg
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/* With -z, read the next NUL-terminated line */
if|if
condition|(
operator|*
operator|*
name|next
condition|)
name|die
argument_list|(
literal|"expected NUL but got: %s"
argument_list|,
operator|*
name|next
argument_list|)
expr_stmt|;
operator|(
operator|*
name|next
operator|)
operator|++
expr_stmt|;
if|if
condition|(
operator|*
name|next
operator|==
name|input
operator|->
name|buf
operator|+
name|input
operator|->
name|len
condition|)
return|return
operator|-
literal|1
return|;
name|strbuf_addstr
argument_list|(
name|arg
argument_list|,
operator|*
name|next
argument_list|)
expr_stmt|;
operator|*
name|next
operator|+=
name|arg
operator|->
name|len
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * The following five parse_cmd_*() functions parse the corresponding  * command.  In each case, next points at the character following the  * command name and the following space.  They each return a pointer  * to the character terminating the command, and die with an  * explanatory message if there are any parsing problems.  All of  * these functions handle either text or binary format input,  * depending on how line_termination is set.  */
end_comment

begin_function
DECL|function|parse_cmd_update
specifier|static
specifier|const
name|char
modifier|*
name|parse_cmd_update
parameter_list|(
name|struct
name|strbuf
modifier|*
name|input
parameter_list|,
specifier|const
name|char
modifier|*
name|next
parameter_list|)
block|{
name|struct
name|strbuf
name|newvalue
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|oldvalue
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|ref_update
modifier|*
name|update
decl_stmt|;
name|update
operator|=
name|update_alloc
argument_list|()
expr_stmt|;
name|update
operator|->
name|ref_name
operator|=
name|parse_refname
argument_list|(
name|input
argument_list|,
operator|&
name|next
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|update
operator|->
name|ref_name
condition|)
name|die
argument_list|(
literal|"update line missing<ref>"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|parse_next_arg
argument_list|(
name|input
argument_list|,
operator|&
name|next
argument_list|,
operator|&
name|newvalue
argument_list|)
condition|)
name|update_store_new_sha1
argument_list|(
literal|"update"
argument_list|,
name|update
argument_list|,
name|newvalue
operator|.
name|buf
argument_list|)
expr_stmt|;
else|else
name|die
argument_list|(
literal|"update %s missing<newvalue>"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|parse_next_arg
argument_list|(
name|input
argument_list|,
operator|&
name|next
argument_list|,
operator|&
name|oldvalue
argument_list|)
condition|)
block|{
name|update_store_old_sha1
argument_list|(
literal|"update"
argument_list|,
name|update
argument_list|,
name|oldvalue
operator|.
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|next
operator|!=
name|line_termination
condition|)
name|die
argument_list|(
literal|"update %s has extra input: %s"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|,
name|next
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|line_termination
condition|)
name|die
argument_list|(
literal|"update %s missing [<oldvalue>] NUL"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|)
expr_stmt|;
return|return
name|next
return|;
block|}
end_function

begin_function
DECL|function|parse_cmd_create
specifier|static
specifier|const
name|char
modifier|*
name|parse_cmd_create
parameter_list|(
name|struct
name|strbuf
modifier|*
name|input
parameter_list|,
specifier|const
name|char
modifier|*
name|next
parameter_list|)
block|{
name|struct
name|strbuf
name|newvalue
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|ref_update
modifier|*
name|update
decl_stmt|;
name|update
operator|=
name|update_alloc
argument_list|()
expr_stmt|;
name|update
operator|->
name|ref_name
operator|=
name|parse_refname
argument_list|(
name|input
argument_list|,
operator|&
name|next
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|update
operator|->
name|ref_name
condition|)
name|die
argument_list|(
literal|"create line missing<ref>"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|parse_next_arg
argument_list|(
name|input
argument_list|,
operator|&
name|next
argument_list|,
operator|&
name|newvalue
argument_list|)
condition|)
name|update_store_new_sha1
argument_list|(
literal|"create"
argument_list|,
name|update
argument_list|,
name|newvalue
operator|.
name|buf
argument_list|)
expr_stmt|;
else|else
name|die
argument_list|(
literal|"create %s missing<newvalue>"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|is_null_sha1
argument_list|(
name|update
operator|->
name|new_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"create %s given zero new value"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|next
operator|!=
name|line_termination
condition|)
name|die
argument_list|(
literal|"create %s has extra input: %s"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|,
name|next
argument_list|)
expr_stmt|;
return|return
name|next
return|;
block|}
end_function

begin_function
DECL|function|parse_cmd_delete
specifier|static
specifier|const
name|char
modifier|*
name|parse_cmd_delete
parameter_list|(
name|struct
name|strbuf
modifier|*
name|input
parameter_list|,
specifier|const
name|char
modifier|*
name|next
parameter_list|)
block|{
name|struct
name|strbuf
name|oldvalue
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|ref_update
modifier|*
name|update
decl_stmt|;
name|update
operator|=
name|update_alloc
argument_list|()
expr_stmt|;
name|update
operator|->
name|ref_name
operator|=
name|parse_refname
argument_list|(
name|input
argument_list|,
operator|&
name|next
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|update
operator|->
name|ref_name
condition|)
name|die
argument_list|(
literal|"delete line missing<ref>"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|parse_next_arg
argument_list|(
name|input
argument_list|,
operator|&
name|next
argument_list|,
operator|&
name|oldvalue
argument_list|)
condition|)
block|{
name|update_store_old_sha1
argument_list|(
literal|"delete"
argument_list|,
name|update
argument_list|,
name|oldvalue
operator|.
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|update
operator|->
name|have_old
operator|&&
name|is_null_sha1
argument_list|(
name|update
operator|->
name|old_sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"delete %s given zero old value"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|line_termination
condition|)
name|die
argument_list|(
literal|"delete %s missing [<oldvalue>] NUL"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|next
operator|!=
name|line_termination
condition|)
name|die
argument_list|(
literal|"delete %s has extra input: %s"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|,
name|next
argument_list|)
expr_stmt|;
return|return
name|next
return|;
block|}
end_function

begin_function
DECL|function|parse_cmd_verify
specifier|static
specifier|const
name|char
modifier|*
name|parse_cmd_verify
parameter_list|(
name|struct
name|strbuf
modifier|*
name|input
parameter_list|,
specifier|const
name|char
modifier|*
name|next
parameter_list|)
block|{
name|struct
name|strbuf
name|value
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|ref_update
modifier|*
name|update
decl_stmt|;
name|update
operator|=
name|update_alloc
argument_list|()
expr_stmt|;
name|update
operator|->
name|ref_name
operator|=
name|parse_refname
argument_list|(
name|input
argument_list|,
operator|&
name|next
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|update
operator|->
name|ref_name
condition|)
name|die
argument_list|(
literal|"verify line missing<ref>"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|parse_next_arg
argument_list|(
name|input
argument_list|,
operator|&
name|next
argument_list|,
operator|&
name|value
argument_list|)
condition|)
block|{
name|update_store_old_sha1
argument_list|(
literal|"verify"
argument_list|,
name|update
argument_list|,
name|value
operator|.
name|buf
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|update
operator|->
name|new_sha1
argument_list|,
name|update
operator|->
name|old_sha1
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|line_termination
condition|)
name|die
argument_list|(
literal|"verify %s missing [<oldvalue>] NUL"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|next
operator|!=
name|line_termination
condition|)
name|die
argument_list|(
literal|"verify %s has extra input: %s"
argument_list|,
name|update
operator|->
name|ref_name
argument_list|,
name|next
argument_list|)
expr_stmt|;
return|return
name|next
return|;
block|}
end_function

begin_function
DECL|function|parse_cmd_option
specifier|static
specifier|const
name|char
modifier|*
name|parse_cmd_option
parameter_list|(
name|struct
name|strbuf
modifier|*
name|input
parameter_list|,
specifier|const
name|char
modifier|*
name|next
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|next
argument_list|,
literal|"no-deref"
argument_list|,
literal|8
argument_list|)
operator|&&
name|next
index|[
literal|8
index|]
operator|==
name|line_termination
condition|)
name|update_flags
operator||=
name|REF_NODEREF
expr_stmt|;
else|else
name|die
argument_list|(
literal|"option unknown: %s"
argument_list|,
name|next
argument_list|)
expr_stmt|;
return|return
name|next
operator|+
literal|8
return|;
block|}
end_function

begin_function
DECL|function|update_refs_stdin
specifier|static
name|void
name|update_refs_stdin
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|strbuf
name|input
init|=
name|STRBUF_INIT
decl_stmt|;
specifier|const
name|char
modifier|*
name|next
decl_stmt|;
if|if
condition|(
name|strbuf_read
argument_list|(
operator|&
name|input
argument_list|,
literal|0
argument_list|,
literal|1000
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"could not read from stdin"
argument_list|)
expr_stmt|;
name|next
operator|=
name|input
operator|.
name|buf
expr_stmt|;
comment|/* Read each line dispatch its command */
while|while
condition|(
name|next
operator|<
name|input
operator|.
name|buf
operator|+
name|input
operator|.
name|len
condition|)
block|{
if|if
condition|(
operator|*
name|next
operator|==
name|line_termination
condition|)
name|die
argument_list|(
literal|"empty command in input"
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|isspace
argument_list|(
operator|*
name|next
argument_list|)
condition|)
name|die
argument_list|(
literal|"whitespace before command: %s"
argument_list|,
name|next
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|starts_with
argument_list|(
name|next
argument_list|,
literal|"update "
argument_list|)
condition|)
name|next
operator|=
name|parse_cmd_update
argument_list|(
operator|&
name|input
argument_list|,
name|next
operator|+
literal|7
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|starts_with
argument_list|(
name|next
argument_list|,
literal|"create "
argument_list|)
condition|)
name|next
operator|=
name|parse_cmd_create
argument_list|(
operator|&
name|input
argument_list|,
name|next
operator|+
literal|7
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|starts_with
argument_list|(
name|next
argument_list|,
literal|"delete "
argument_list|)
condition|)
name|next
operator|=
name|parse_cmd_delete
argument_list|(
operator|&
name|input
argument_list|,
name|next
operator|+
literal|7
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|starts_with
argument_list|(
name|next
argument_list|,
literal|"verify "
argument_list|)
condition|)
name|next
operator|=
name|parse_cmd_verify
argument_list|(
operator|&
name|input
argument_list|,
name|next
operator|+
literal|7
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|starts_with
argument_list|(
name|next
argument_list|,
literal|"option "
argument_list|)
condition|)
name|next
operator|=
name|parse_cmd_option
argument_list|(
operator|&
name|input
argument_list|,
name|next
operator|+
literal|7
argument_list|)
expr_stmt|;
else|else
name|die
argument_list|(
literal|"unknown command: %s"
argument_list|,
name|next
argument_list|)
expr_stmt|;
name|next
operator|++
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|input
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_update_ref
name|int
name|cmd_update_ref
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
name|refname
decl_stmt|,
modifier|*
name|oldval
decl_stmt|,
modifier|*
name|msg
init|=
name|NULL
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|,
name|oldsha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|delete
init|=
literal|0
decl_stmt|,
name|no_deref
init|=
literal|0
decl_stmt|,
name|read_stdin
init|=
literal|0
decl_stmt|,
name|end_null
init|=
literal|0
decl_stmt|,
name|flags
init|=
literal|0
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_STRING
argument_list|(
literal|'m'
argument_list|,
name|NULL
argument_list|,
operator|&
name|msg
argument_list|,
name|N_
argument_list|(
literal|"reason"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"reason of the update"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|'d'
argument_list|,
name|NULL
argument_list|,
operator|&
name|delete
argument_list|,
name|N_
argument_list|(
literal|"delete the reference"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"no-deref"
argument_list|,
operator|&
name|no_deref
argument_list|,
name|N_
argument_list|(
literal|"update<refname> not the one it points to"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|'z'
argument_list|,
name|NULL
argument_list|,
operator|&
name|end_null
argument_list|,
name|N_
argument_list|(
literal|"stdin has NUL-terminated arguments"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"stdin"
argument_list|,
operator|&
name|read_stdin
argument_list|,
name|N_
argument_list|(
literal|"read updates from stdin"
argument_list|)
argument_list|)
block|,
name|OPT_END
argument_list|()
block|, 	}
decl_stmt|;
name|git_config
argument_list|(
name|git_default_config
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
name|git_update_ref_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|msg
operator|&&
operator|!
operator|*
name|msg
condition|)
name|die
argument_list|(
literal|"Refusing to perform update with empty message."
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_stdin
condition|)
block|{
if|if
condition|(
name|delete
operator|||
name|no_deref
operator|||
name|argc
operator|>
literal|0
condition|)
name|usage_with_options
argument_list|(
name|git_update_ref_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
name|end_null
condition|)
name|line_termination
operator|=
literal|'\0'
expr_stmt|;
name|update_refs_stdin
argument_list|()
expr_stmt|;
return|return
name|update_refs
argument_list|(
name|msg
argument_list|,
name|updates
argument_list|,
name|updates_count
argument_list|,
name|UPDATE_REFS_DIE_ON_ERR
argument_list|)
return|;
block|}
if|if
condition|(
name|end_null
condition|)
name|usage_with_options
argument_list|(
name|git_update_ref_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
name|delete
condition|)
block|{
if|if
condition|(
name|argc
operator|<
literal|1
operator|||
name|argc
operator|>
literal|2
condition|)
name|usage_with_options
argument_list|(
name|git_update_ref_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
name|refname
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
name|oldval
operator|=
name|argv
index|[
literal|1
index|]
expr_stmt|;
block|}
else|else
block|{
specifier|const
name|char
modifier|*
name|value
decl_stmt|;
if|if
condition|(
name|argc
operator|<
literal|2
operator|||
name|argc
operator|>
literal|3
condition|)
name|usage_with_options
argument_list|(
name|git_update_ref_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
name|refname
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
name|value
operator|=
name|argv
index|[
literal|1
index|]
expr_stmt|;
name|oldval
operator|=
name|argv
index|[
literal|2
index|]
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|value
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s: not a valid SHA1"
argument_list|,
name|value
argument_list|)
expr_stmt|;
block|}
name|hashclr
argument_list|(
name|oldsha1
argument_list|)
expr_stmt|;
comment|/* all-zero hash in case oldval is the empty string */
if|if
condition|(
name|oldval
operator|&&
operator|*
name|oldval
operator|&&
name|get_sha1
argument_list|(
name|oldval
argument_list|,
name|oldsha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s: not a valid old SHA1"
argument_list|,
name|oldval
argument_list|)
expr_stmt|;
if|if
condition|(
name|no_deref
condition|)
name|flags
operator|=
name|REF_NODEREF
expr_stmt|;
if|if
condition|(
name|delete
condition|)
return|return
name|delete_ref
argument_list|(
name|refname
argument_list|,
name|oldval
condition|?
name|oldsha1
else|:
name|NULL
argument_list|,
name|flags
argument_list|)
return|;
else|else
return|return
name|update_ref
argument_list|(
name|msg
argument_list|,
name|refname
argument_list|,
name|sha1
argument_list|,
name|oldval
condition|?
name|oldsha1
else|:
name|NULL
argument_list|,
name|flags
argument_list|,
name|UPDATE_REFS_DIE_ON_ERR
argument_list|)
return|;
block|}
end_function

end_unit

