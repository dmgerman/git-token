begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"transport.h"
end_include

begin_include
include|#
directive|include
file|"remote.h"
end_include

begin_decl_stmt
DECL|variable|ls_remote_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|ls_remote_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git ls-remote [--heads] [--tags] [--refs] [--upload-pack=<exec>]\n"
literal|"                     [-q | --quiet] [--exit-code] [--get-url] [<repository> [<refs>...]]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Is there one among the list of patterns that match the tail part  * of the path?  */
end_comment

begin_function
DECL|function|tail_match
specifier|static
name|int
name|tail_match
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|p
decl_stmt|;
name|char
name|pathbuf
index|[
name|PATH_MAX
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|pattern
condition|)
return|return
literal|1
return|;
comment|/* no restriction */
if|if
condition|(
name|snprintf
argument_list|(
name|pathbuf
argument_list|,
sizeof|sizeof
argument_list|(
name|pathbuf
argument_list|)
argument_list|,
literal|"/%s"
argument_list|,
name|path
argument_list|)
operator|>
sizeof|sizeof
argument_list|(
name|pathbuf
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"insanely long ref %.*s..."
argument_list|,
literal|20
argument_list|,
name|path
argument_list|)
return|;
while|while
condition|(
operator|(
name|p
operator|=
operator|*
operator|(
name|pattern
operator|++
operator|)
operator|)
operator|!=
name|NULL
condition|)
block|{
if|if
condition|(
operator|!
name|wildmatch
argument_list|(
name|p
argument_list|,
name|pathbuf
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
condition|)
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|cmd_ls_remote
name|int
name|cmd_ls_remote
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
name|dest
init|=
name|NULL
decl_stmt|;
name|unsigned
name|flags
init|=
literal|0
decl_stmt|;
name|int
name|get_url
init|=
literal|0
decl_stmt|;
name|int
name|quiet
init|=
literal|0
decl_stmt|;
name|int
name|status
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|uploadpack
init|=
name|NULL
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|pattern
init|=
name|NULL
decl_stmt|;
name|struct
name|remote
modifier|*
name|remote
decl_stmt|;
name|struct
name|transport
modifier|*
name|transport
decl_stmt|;
specifier|const
name|struct
name|ref
modifier|*
name|ref
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT__QUIET
argument_list|(
operator|&
name|quiet
argument_list|,
name|N_
argument_list|(
literal|"do not print remote URL"
argument_list|)
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"upload-pack"
argument_list|,
operator|&
name|uploadpack
argument_list|,
name|N_
argument_list|(
literal|"exec"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"path of git-upload-pack on the remote host"
argument_list|)
argument_list|)
block|,
block|{
name|OPTION_STRING
block|,
literal|0
block|,
literal|"exec"
block|,
operator|&
name|uploadpack
block|,
name|N_
argument_list|(
literal|"exec"
argument_list|)
block|,
name|N_
argument_list|(
literal|"path of git-upload-pack on the remote host"
argument_list|)
block|,
name|PARSE_OPT_HIDDEN
block|}
block|,
name|OPT_BIT
argument_list|(
literal|'t'
argument_list|,
literal|"tags"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"limit to tags"
argument_list|)
argument_list|,
name|REF_TAGS
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|'h'
argument_list|,
literal|"heads"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"limit to heads"
argument_list|)
argument_list|,
name|REF_HEADS
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"refs"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"do not show peeled tags"
argument_list|)
argument_list|,
name|REF_NORMAL
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"get-url"
argument_list|,
operator|&
name|get_url
argument_list|,
name|N_
argument_list|(
literal|"take url.<base>.insteadOf into account"
argument_list|)
argument_list|)
block|,
name|OPT_SET_INT
argument_list|(
literal|0
argument_list|,
literal|"exit-code"
argument_list|,
operator|&
name|status
argument_list|,
name|N_
argument_list|(
literal|"exit with exit code 2 if no matching refs are found"
argument_list|)
argument_list|,
literal|2
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
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
name|ls_remote_usage
argument_list|,
name|PARSE_OPT_STOP_AT_NON_OPTION
argument_list|)
expr_stmt|;
name|dest
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
if|if
condition|(
name|argc
operator|>
literal|1
condition|)
block|{
name|int
name|i
decl_stmt|;
name|pattern
operator|=
name|xcalloc
argument_list|(
name|argc
argument_list|,
sizeof|sizeof
argument_list|(
specifier|const
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
name|pattern
index|[
name|i
operator|-
literal|1
index|]
operator|=
name|xstrfmt
argument_list|(
literal|"*/%s"
argument_list|,
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
name|remote
operator|=
name|remote_get
argument_list|(
name|dest
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|remote
condition|)
block|{
if|if
condition|(
name|dest
condition|)
name|die
argument_list|(
literal|"bad repository '%s'"
argument_list|,
name|dest
argument_list|)
expr_stmt|;
name|die
argument_list|(
literal|"No remote configured to list refs from."
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|remote
operator|->
name|url_nr
condition|)
name|die
argument_list|(
literal|"remote %s has no configured URL"
argument_list|,
name|dest
argument_list|)
expr_stmt|;
if|if
condition|(
name|get_url
condition|)
block|{
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
operator|*
name|remote
operator|->
name|url
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|transport
operator|=
name|transport_get
argument_list|(
name|remote
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|uploadpack
operator|!=
name|NULL
condition|)
name|transport_set_option
argument_list|(
name|transport
argument_list|,
name|TRANS_OPT_UPLOADPACK
argument_list|,
name|uploadpack
argument_list|)
expr_stmt|;
name|ref
operator|=
name|transport_get_remote_refs
argument_list|(
name|transport
argument_list|)
expr_stmt|;
if|if
condition|(
name|transport_disconnect
argument_list|(
name|transport
argument_list|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
operator|!
name|dest
operator|&&
operator|!
name|quiet
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"From %s\n"
argument_list|,
operator|*
name|remote
operator|->
name|url
argument_list|)
expr_stmt|;
for|for
control|(
init|;
name|ref
condition|;
name|ref
operator|=
name|ref
operator|->
name|next
control|)
block|{
if|if
condition|(
operator|!
name|check_ref_type
argument_list|(
name|ref
argument_list|,
name|flags
argument_list|)
condition|)
continue|continue;
if|if
condition|(
operator|!
name|tail_match
argument_list|(
name|pattern
argument_list|,
name|ref
operator|->
name|name
argument_list|)
condition|)
continue|continue;
name|printf
argument_list|(
literal|"%s	%s\n"
argument_list|,
name|oid_to_hex
argument_list|(
operator|&
name|ref
operator|->
name|old_oid
argument_list|)
argument_list|,
name|ref
operator|->
name|name
argument_list|)
expr_stmt|;
name|status
operator|=
literal|0
expr_stmt|;
comment|/* we found something */
block|}
return|return
name|status
return|;
block|}
end_function

end_unit

