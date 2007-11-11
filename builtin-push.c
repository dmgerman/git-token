begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * "git push"  */
end_comment

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
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"remote.h"
end_include

begin_include
include|#
directive|include
file|"transport.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_decl_stmt
DECL|variable|push_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|push_usage
index|[]
init|=
block|{
literal|"git-push [--all] [--dry-run] [--tags] [--receive-pack=<git-receive-pack>] [--repo=all] [-f | --force] [-v] [<repository><refspec>...]"
block|,
name|NULL
block|, }
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|thin
DECL|variable|verbose
specifier|static
name|int
name|thin
decl_stmt|,
name|verbose
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|receivepack
specifier|static
specifier|const
name|char
modifier|*
name|receivepack
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|refspec
specifier|static
specifier|const
name|char
modifier|*
modifier|*
name|refspec
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|refspec_nr
specifier|static
name|int
name|refspec_nr
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|add_refspec
specifier|static
name|void
name|add_refspec
parameter_list|(
specifier|const
name|char
modifier|*
name|ref
parameter_list|)
block|{
name|int
name|nr
init|=
name|refspec_nr
operator|+
literal|1
decl_stmt|;
name|refspec
operator|=
name|xrealloc
argument_list|(
name|refspec
argument_list|,
name|nr
operator|*
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|refspec
index|[
name|nr
operator|-
literal|1
index|]
operator|=
name|ref
expr_stmt|;
name|refspec_nr
operator|=
name|nr
expr_stmt|;
block|}
end_function

begin_function
DECL|function|set_refspecs
specifier|static
name|void
name|set_refspecs
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|refs
parameter_list|,
name|int
name|nr
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nr
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|ref
init|=
name|refs
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"tag"
argument_list|,
name|ref
argument_list|)
condition|)
block|{
name|char
modifier|*
name|tag
decl_stmt|;
name|int
name|len
decl_stmt|;
if|if
condition|(
name|nr
operator|<=
operator|++
name|i
condition|)
name|die
argument_list|(
literal|"tag shorthand without<tag>"
argument_list|)
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|refs
index|[
name|i
index|]
argument_list|)
operator|+
literal|11
expr_stmt|;
name|tag
operator|=
name|xmalloc
argument_list|(
name|len
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|tag
argument_list|,
literal|"refs/tags/"
argument_list|)
expr_stmt|;
name|strcat
argument_list|(
name|tag
argument_list|,
name|refs
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|ref
operator|=
name|tag
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
literal|"HEAD"
argument_list|,
name|ref
argument_list|)
condition|)
block|{
name|unsigned
name|char
name|sha1_dummy
index|[
literal|20
index|]
decl_stmt|;
name|ref
operator|=
name|resolve_ref
argument_list|(
name|ref
argument_list|,
name|sha1_dummy
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ref
condition|)
name|die
argument_list|(
literal|"HEAD cannot be resolved."
argument_list|)
expr_stmt|;
if|if
condition|(
name|prefixcmp
argument_list|(
name|ref
argument_list|,
literal|"refs/heads/"
argument_list|)
condition|)
name|die
argument_list|(
literal|"HEAD cannot be resolved to branch."
argument_list|)
expr_stmt|;
name|ref
operator|=
name|xstrdup
argument_list|(
name|ref
operator|+
literal|11
argument_list|)
expr_stmt|;
block|}
name|add_refspec
argument_list|(
name|ref
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|do_push
specifier|static
name|int
name|do_push
parameter_list|(
specifier|const
name|char
modifier|*
name|repo
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|errs
decl_stmt|;
name|struct
name|remote
modifier|*
name|remote
init|=
name|remote_get
argument_list|(
name|repo
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|remote
condition|)
name|die
argument_list|(
literal|"bad repository '%s'"
argument_list|,
name|repo
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|refspec
operator|&&
operator|!
operator|(
name|flags
operator|&
name|TRANSPORT_PUSH_ALL
operator|)
operator|&&
name|remote
operator|->
name|push_refspec_nr
condition|)
block|{
name|refspec
operator|=
name|remote
operator|->
name|push_refspec
expr_stmt|;
name|refspec_nr
operator|=
name|remote
operator|->
name|push_refspec_nr
expr_stmt|;
block|}
name|errs
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|remote
operator|->
name|url_nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|transport
modifier|*
name|transport
init|=
name|transport_get
argument_list|(
name|remote
argument_list|,
name|remote
operator|->
name|url
index|[
name|i
index|]
argument_list|)
decl_stmt|;
name|int
name|err
decl_stmt|;
if|if
condition|(
name|receivepack
condition|)
name|transport_set_option
argument_list|(
name|transport
argument_list|,
name|TRANS_OPT_RECEIVEPACK
argument_list|,
name|receivepack
argument_list|)
expr_stmt|;
if|if
condition|(
name|thin
condition|)
name|transport_set_option
argument_list|(
name|transport
argument_list|,
name|TRANS_OPT_THIN
argument_list|,
literal|"yes"
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Pushing to %s\n"
argument_list|,
name|remote
operator|->
name|url
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|err
operator|=
name|transport_push
argument_list|(
name|transport
argument_list|,
name|refspec_nr
argument_list|,
name|refspec
argument_list|,
name|flags
argument_list|)
expr_stmt|;
name|err
operator||=
name|transport_disconnect
argument_list|(
name|transport
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|err
condition|)
continue|continue;
name|error
argument_list|(
literal|"failed to push to '%s'"
argument_list|,
name|remote
operator|->
name|url
index|[
name|i
index|]
argument_list|)
expr_stmt|;
name|errs
operator|++
expr_stmt|;
block|}
return|return
operator|!
operator|!
name|errs
return|;
block|}
end_function

begin_function
DECL|function|cmd_push
name|int
name|cmd_push
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
name|flags
init|=
literal|0
decl_stmt|;
name|int
name|all
init|=
literal|0
decl_stmt|;
name|int
name|dry_run
init|=
literal|0
decl_stmt|;
name|int
name|force
init|=
literal|0
decl_stmt|;
name|int
name|tags
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|repo
init|=
name|NULL
decl_stmt|;
comment|/* default repository */
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT__VERBOSE
argument_list|(
operator|&
name|verbose
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"repo"
argument_list|,
operator|&
name|repo
argument_list|,
literal|"repository"
argument_list|,
literal|"repository"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"all"
argument_list|,
operator|&
name|all
argument_list|,
literal|"push all refs"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"tags"
argument_list|,
operator|&
name|tags
argument_list|,
literal|"push tags"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"dry-run"
argument_list|,
operator|&
name|dry_run
argument_list|,
literal|"dry run"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'f'
argument_list|,
literal|"force"
argument_list|,
operator|&
name|force
argument_list|,
literal|"force updates"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"thin"
argument_list|,
operator|&
name|thin
argument_list|,
literal|"use thin pack"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"receive-pack"
argument_list|,
operator|&
name|receivepack
argument_list|,
literal|"receive-pack"
argument_list|,
literal|"receive pack program"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"exec"
argument_list|,
operator|&
name|receivepack
argument_list|,
literal|"receive-pack"
argument_list|,
literal|"receive pack program"
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
name|options
argument_list|,
name|push_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|force
condition|)
name|flags
operator||=
name|TRANSPORT_PUSH_FORCE
expr_stmt|;
if|if
condition|(
name|dry_run
condition|)
name|flags
operator||=
name|TRANSPORT_PUSH_DRY_RUN
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
name|flags
operator||=
name|TRANSPORT_PUSH_VERBOSE
expr_stmt|;
if|if
condition|(
name|tags
condition|)
name|add_refspec
argument_list|(
literal|"refs/tags/*"
argument_list|)
expr_stmt|;
if|if
condition|(
name|all
condition|)
name|flags
operator||=
name|TRANSPORT_PUSH_ALL
expr_stmt|;
if|if
condition|(
name|argc
operator|>
literal|0
condition|)
block|{
name|repo
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
name|set_refspecs
argument_list|(
name|argv
operator|+
literal|1
argument_list|,
name|argc
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|flags
operator|&
name|TRANSPORT_PUSH_ALL
operator|)
operator|&&
name|refspec
condition|)
name|usage_with_options
argument_list|(
name|push_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
return|return
name|do_push
argument_list|(
name|repo
argument_list|,
name|flags
argument_list|)
return|;
block|}
end_function

end_unit

