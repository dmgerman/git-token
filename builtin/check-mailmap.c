begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"mailmap.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_decl_stmt
DECL|variable|use_stdin
specifier|static
name|int
name|use_stdin
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|check_mailmap_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|check_mailmap_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git check-mailmap [<options>]<contact>..."
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|check_mailmap_options
specifier|static
specifier|const
name|struct
name|option
name|check_mailmap_options
index|[]
init|=
block|{
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"stdin"
argument_list|,
operator|&
name|use_stdin
argument_list|,
name|N_
argument_list|(
literal|"also read contacts from stdin"
argument_list|)
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|check_mailmap
specifier|static
name|void
name|check_mailmap
parameter_list|(
name|struct
name|string_list
modifier|*
name|mailmap
parameter_list|,
specifier|const
name|char
modifier|*
name|contact
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|name
decl_stmt|,
modifier|*
name|mail
decl_stmt|;
name|size_t
name|namelen
decl_stmt|,
name|maillen
decl_stmt|;
name|struct
name|ident_split
name|ident
decl_stmt|;
if|if
condition|(
name|split_ident_line
argument_list|(
operator|&
name|ident
argument_list|,
name|contact
argument_list|,
name|strlen
argument_list|(
name|contact
argument_list|)
argument_list|)
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"unable to parse contact: %s"
argument_list|)
argument_list|,
name|contact
argument_list|)
expr_stmt|;
name|name
operator|=
name|ident
operator|.
name|name_begin
expr_stmt|;
name|namelen
operator|=
name|ident
operator|.
name|name_end
operator|-
name|ident
operator|.
name|name_begin
expr_stmt|;
name|mail
operator|=
name|ident
operator|.
name|mail_begin
expr_stmt|;
name|maillen
operator|=
name|ident
operator|.
name|mail_end
operator|-
name|ident
operator|.
name|mail_begin
expr_stmt|;
name|map_user
argument_list|(
name|mailmap
argument_list|,
operator|&
name|mail
argument_list|,
operator|&
name|maillen
argument_list|,
operator|&
name|name
argument_list|,
operator|&
name|namelen
argument_list|)
expr_stmt|;
if|if
condition|(
name|namelen
condition|)
name|printf
argument_list|(
literal|"%.*s "
argument_list|,
operator|(
name|int
operator|)
name|namelen
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"<%.*s>\n"
argument_list|,
operator|(
name|int
operator|)
name|maillen
argument_list|,
name|mail
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|cmd_check_mailmap
name|int
name|cmd_check_mailmap
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
decl_stmt|;
name|struct
name|string_list
name|mailmap
init|=
name|STRING_LIST_INIT_NODUP
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
name|check_mailmap_options
argument_list|,
name|check_mailmap_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|==
literal|0
operator|&&
operator|!
name|use_stdin
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"no contacts specified"
argument_list|)
argument_list|)
expr_stmt|;
name|read_mailmap
argument_list|(
operator|&
name|mailmap
argument_list|,
name|NULL
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
name|argc
condition|;
operator|++
name|i
control|)
name|check_mailmap
argument_list|(
operator|&
name|mailmap
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|maybe_flush_or_die
argument_list|(
name|stdout
argument_list|,
literal|"stdout"
argument_list|)
expr_stmt|;
if|if
condition|(
name|use_stdin
condition|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
while|while
condition|(
name|strbuf_getline_lf
argument_list|(
operator|&
name|buf
argument_list|,
name|stdin
argument_list|)
operator|!=
name|EOF
condition|)
block|{
name|check_mailmap
argument_list|(
operator|&
name|mailmap
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|maybe_flush_or_die
argument_list|(
name|stdout
argument_list|,
literal|"stdout"
argument_list|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
block|}
name|clear_mailmap
argument_list|(
operator|&
name|mailmap
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

