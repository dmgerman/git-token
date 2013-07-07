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

begin_include
include|#
directive|include
file|"submodule.h"
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
name|N_
argument_list|(
literal|"git push [<options>] [<repository> [<refspec>...]]"
argument_list|)
block|,
name|NULL
block|, }
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|thin
specifier|static
name|int
name|thin
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|deleterefs
specifier|static
name|int
name|deleterefs
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
DECL|variable|verbosity
specifier|static
name|int
name|verbosity
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|progress
specifier|static
name|int
name|progress
init|=
operator|-
literal|1
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

begin_decl_stmt
DECL|variable|refspec_alloc
specifier|static
name|int
name|refspec_alloc
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|default_matching_used
specifier|static
name|int
name|default_matching_used
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
name|refspec_nr
operator|++
expr_stmt|;
name|ALLOC_GROW
argument_list|(
name|refspec
argument_list|,
name|refspec_nr
argument_list|,
name|refspec_alloc
argument_list|)
expr_stmt|;
name|refspec
index|[
name|refspec_nr
operator|-
literal|1
index|]
operator|=
name|ref
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
name|_
argument_list|(
literal|"tag shorthand without<tag>"
argument_list|)
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
if|if
condition|(
name|deleterefs
condition|)
block|{
name|tag
operator|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|tag
argument_list|,
literal|":refs/tags/"
argument_list|)
expr_stmt|;
block|}
else|else
block|{
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
block|}
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
elseif|else
if|if
condition|(
name|deleterefs
operator|&&
operator|!
name|strchr
argument_list|(
name|ref
argument_list|,
literal|':'
argument_list|)
condition|)
block|{
name|char
modifier|*
name|delref
decl_stmt|;
name|int
name|len
init|=
name|strlen
argument_list|(
name|ref
argument_list|)
operator|+
literal|1
decl_stmt|;
name|delref
operator|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|delref
argument_list|,
literal|":"
argument_list|)
expr_stmt|;
name|strcat
argument_list|(
name|delref
argument_list|,
name|ref
argument_list|)
expr_stmt|;
name|ref
operator|=
name|delref
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|deleterefs
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"--delete only accepts plain target ref names"
argument_list|)
argument_list|)
expr_stmt|;
name|add_refspec
argument_list|(
name|ref
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|push_url_of_remote
specifier|static
name|int
name|push_url_of_remote
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
modifier|*
name|url_p
parameter_list|)
block|{
if|if
condition|(
name|remote
operator|->
name|pushurl_nr
condition|)
block|{
operator|*
name|url_p
operator|=
name|remote
operator|->
name|pushurl
expr_stmt|;
return|return
name|remote
operator|->
name|pushurl_nr
return|;
block|}
operator|*
name|url_p
operator|=
name|remote
operator|->
name|url
expr_stmt|;
return|return
name|remote
operator|->
name|url_nr
return|;
block|}
end_function

begin_function
DECL|function|die_push_simple
specifier|static
name|NORETURN
name|int
name|die_push_simple
parameter_list|(
name|struct
name|branch
modifier|*
name|branch
parameter_list|,
name|struct
name|remote
modifier|*
name|remote
parameter_list|)
block|{
comment|/* 	 * There's no point in using shorten_unambiguous_ref here, 	 * as the ambiguity would be on the remote side, not what 	 * we have locally. Plus, this is supposed to be the simple 	 * mode. If the user is doing something crazy like setting 	 * upstream to a non-branch, we should probably be showing 	 * them the big ugly fully qualified ref. 	 */
specifier|const
name|char
modifier|*
name|advice_maybe
init|=
literal|""
decl_stmt|;
specifier|const
name|char
modifier|*
name|short_upstream
init|=
name|skip_prefix
argument_list|(
name|branch
operator|->
name|merge
index|[
literal|0
index|]
operator|->
name|src
argument_list|,
literal|"refs/heads/"
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|short_upstream
condition|)
name|short_upstream
operator|=
name|branch
operator|->
name|merge
index|[
literal|0
index|]
operator|->
name|src
expr_stmt|;
comment|/* 	 * Don't show advice for people who explicitely set 	 * push.default. 	 */
if|if
condition|(
name|push_default
operator|==
name|PUSH_DEFAULT_UNSPECIFIED
condition|)
name|advice_maybe
operator|=
name|_
argument_list|(
literal|"\n"
literal|"To choose either option permanently, "
literal|"see push.default in 'git help config'."
argument_list|)
expr_stmt|;
name|die
argument_list|(
name|_
argument_list|(
literal|"The upstream branch of your current branch does not match\n"
literal|"the name of your current branch.  To push to the upstream branch\n"
literal|"on the remote, use\n"
literal|"\n"
literal|"    git push %s HEAD:%s\n"
literal|"\n"
literal|"To push to the branch of the same name on the remote, use\n"
literal|"\n"
literal|"    git push %s %s\n"
literal|"%s"
argument_list|)
argument_list|,
name|remote
operator|->
name|name
argument_list|,
name|short_upstream
argument_list|,
name|remote
operator|->
name|name
argument_list|,
name|branch
operator|->
name|name
argument_list|,
name|advice_maybe
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|setup_push_upstream
specifier|static
name|void
name|setup_push_upstream
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
name|int
name|simple
parameter_list|)
block|{
name|struct
name|strbuf
name|refspec
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|branch
modifier|*
name|branch
init|=
name|branch_get
argument_list|(
name|NULL
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|branch
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"You are not currently on a branch.\n"
literal|"To push the history leading to the current (detached HEAD)\n"
literal|"state now, use\n"
literal|"\n"
literal|"    git push %s HEAD:<name-of-remote-branch>\n"
argument_list|)
argument_list|,
name|remote
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|branch
operator|->
name|merge_nr
operator|||
operator|!
name|branch
operator|->
name|merge
operator|||
operator|!
name|branch
operator|->
name|remote_name
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"The current branch %s has no upstream branch.\n"
literal|"To push the current branch and set the remote as upstream, use\n"
literal|"\n"
literal|"    git push --set-upstream %s %s\n"
argument_list|)
argument_list|,
name|branch
operator|->
name|name
argument_list|,
name|remote
operator|->
name|name
argument_list|,
name|branch
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|branch
operator|->
name|merge_nr
operator|!=
literal|1
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"The current branch %s has multiple upstream branches, "
literal|"refusing to push."
argument_list|)
argument_list|,
name|branch
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|strcmp
argument_list|(
name|branch
operator|->
name|remote_name
argument_list|,
name|remote
operator|->
name|name
argument_list|)
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"You are pushing to remote '%s', which is not the upstream of\n"
literal|"your current branch '%s', without telling me what to push\n"
literal|"to update which remote branch."
argument_list|)
argument_list|,
name|remote
operator|->
name|name
argument_list|,
name|branch
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|simple
operator|&&
name|strcmp
argument_list|(
name|branch
operator|->
name|refname
argument_list|,
name|branch
operator|->
name|merge
index|[
literal|0
index|]
operator|->
name|src
argument_list|)
condition|)
name|die_push_simple
argument_list|(
name|branch
argument_list|,
name|remote
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|refspec
argument_list|,
literal|"%s:%s"
argument_list|,
name|branch
operator|->
name|name
argument_list|,
name|branch
operator|->
name|merge
index|[
literal|0
index|]
operator|->
name|src
argument_list|)
expr_stmt|;
name|add_refspec
argument_list|(
name|refspec
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|warn_unspecified_push_default_msg
specifier|static
name|char
name|warn_unspecified_push_default_msg
index|[]
init|=
name|N_
argument_list|(
literal|"push.default is unset; its implicit value is changing in\n"
literal|"Git 2.0 from 'matching' to 'simple'. To squelch this message\n"
literal|"and maintain the current behavior after the default changes, use:\n"
literal|"\n"
literal|"  git config --global push.default matching\n"
literal|"\n"
literal|"To squelch this message and adopt the new behavior now, use:\n"
literal|"\n"
literal|"  git config --global push.default simple\n"
literal|"\n"
literal|"See 'git help config' and search for 'push.default' for further information.\n"
literal|"(the 'simple' mode was introduced in Git 1.7.11. Use the similar mode\n"
literal|"'current' instead of 'simple' if you sometimes use older versions of Git)"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|warn_unspecified_push_default_configuration
specifier|static
name|void
name|warn_unspecified_push_default_configuration
parameter_list|(
name|void
parameter_list|)
block|{
specifier|static
name|int
name|warn_once
decl_stmt|;
if|if
condition|(
name|warn_once
operator|++
condition|)
return|return;
name|warning
argument_list|(
literal|"%s\n"
argument_list|,
name|_
argument_list|(
name|warn_unspecified_push_default_msg
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|setup_default_push_refspecs
specifier|static
name|void
name|setup_default_push_refspecs
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|)
block|{
switch|switch
condition|(
name|push_default
condition|)
block|{
default|default:
case|case
name|PUSH_DEFAULT_UNSPECIFIED
case|:
name|default_matching_used
operator|=
literal|1
expr_stmt|;
name|warn_unspecified_push_default_configuration
argument_list|()
expr_stmt|;
comment|/* fallthru */
case|case
name|PUSH_DEFAULT_MATCHING
case|:
name|add_refspec
argument_list|(
literal|":"
argument_list|)
expr_stmt|;
break|break;
case|case
name|PUSH_DEFAULT_SIMPLE
case|:
name|setup_push_upstream
argument_list|(
name|remote
argument_list|,
literal|1
argument_list|)
expr_stmt|;
break|break;
case|case
name|PUSH_DEFAULT_UPSTREAM
case|:
name|setup_push_upstream
argument_list|(
name|remote
argument_list|,
literal|0
argument_list|)
expr_stmt|;
break|break;
case|case
name|PUSH_DEFAULT_CURRENT
case|:
name|add_refspec
argument_list|(
literal|"HEAD"
argument_list|)
expr_stmt|;
break|break;
case|case
name|PUSH_DEFAULT_NOTHING
case|:
name|die
argument_list|(
name|_
argument_list|(
literal|"You didn't specify any refspecs to push, and "
literal|"push.default is \"nothing\"."
argument_list|)
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
end_function

begin_decl_stmt
DECL|variable|message_advice_pull_before_push
specifier|static
specifier|const
name|char
name|message_advice_pull_before_push
index|[]
init|=
name|N_
argument_list|(
literal|"Updates were rejected because the tip of your current branch is behind\n"
literal|"its remote counterpart. Integrate the remote changes (e.g.\n"
literal|"'git pull ...') before pushing again.\n"
literal|"See the 'Note about fast-forwards' in 'git push --help' for details."
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|message_advice_use_upstream
specifier|static
specifier|const
name|char
name|message_advice_use_upstream
index|[]
init|=
name|N_
argument_list|(
literal|"Updates were rejected because a pushed branch tip is behind its remote\n"
literal|"counterpart. If you did not intend to push that branch, you may want to\n"
literal|"specify branches to push or set the 'push.default' configuration variable\n"
literal|"to 'simple', 'current' or 'upstream' to push only the current branch."
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|message_advice_checkout_pull_push
specifier|static
specifier|const
name|char
name|message_advice_checkout_pull_push
index|[]
init|=
name|N_
argument_list|(
literal|"Updates were rejected because a pushed branch tip is behind its remote\n"
literal|"counterpart. Check out this branch and integrate the remote changes\n"
literal|"(e.g. 'git pull ...') before pushing again.\n"
literal|"See the 'Note about fast-forwards' in 'git push --help' for details."
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|message_advice_ref_fetch_first
specifier|static
specifier|const
name|char
name|message_advice_ref_fetch_first
index|[]
init|=
name|N_
argument_list|(
literal|"Updates were rejected because the remote contains work that you do\n"
literal|"not have locally. This is usually caused by another repository pushing\n"
literal|"to the same ref. You may want to first integrate the remote changes\n"
literal|"(e.g., 'git pull ...') before pushing again.\n"
literal|"See the 'Note about fast-forwards' in 'git push --help' for details."
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|message_advice_ref_already_exists
specifier|static
specifier|const
name|char
name|message_advice_ref_already_exists
index|[]
init|=
name|N_
argument_list|(
literal|"Updates were rejected because the tag already exists in the remote."
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|message_advice_ref_needs_force
specifier|static
specifier|const
name|char
name|message_advice_ref_needs_force
index|[]
init|=
name|N_
argument_list|(
literal|"You cannot update a remote ref that points at a non-commit object,\n"
literal|"or update a remote ref to make it point at a non-commit object,\n"
literal|"without using the '--force' option.\n"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|advise_pull_before_push
specifier|static
name|void
name|advise_pull_before_push
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|advice_push_non_ff_current
operator|||
operator|!
name|advice_push_update_rejected
condition|)
return|return;
name|advise
argument_list|(
name|_
argument_list|(
name|message_advice_pull_before_push
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|advise_use_upstream
specifier|static
name|void
name|advise_use_upstream
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|advice_push_non_ff_default
operator|||
operator|!
name|advice_push_update_rejected
condition|)
return|return;
name|advise
argument_list|(
name|_
argument_list|(
name|message_advice_use_upstream
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|advise_checkout_pull_push
specifier|static
name|void
name|advise_checkout_pull_push
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|advice_push_non_ff_matching
operator|||
operator|!
name|advice_push_update_rejected
condition|)
return|return;
name|advise
argument_list|(
name|_
argument_list|(
name|message_advice_checkout_pull_push
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|advise_ref_already_exists
specifier|static
name|void
name|advise_ref_already_exists
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|advice_push_already_exists
operator|||
operator|!
name|advice_push_update_rejected
condition|)
return|return;
name|advise
argument_list|(
name|_
argument_list|(
name|message_advice_ref_already_exists
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|advise_ref_fetch_first
specifier|static
name|void
name|advise_ref_fetch_first
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|advice_push_fetch_first
operator|||
operator|!
name|advice_push_update_rejected
condition|)
return|return;
name|advise
argument_list|(
name|_
argument_list|(
name|message_advice_ref_fetch_first
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|advise_ref_needs_force
specifier|static
name|void
name|advise_ref_needs_force
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
operator|!
name|advice_push_needs_force
operator|||
operator|!
name|advice_push_update_rejected
condition|)
return|return;
name|advise
argument_list|(
name|_
argument_list|(
name|message_advice_ref_needs_force
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|push_with_options
specifier|static
name|int
name|push_with_options
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
name|int
name|err
decl_stmt|;
name|unsigned
name|int
name|reject_reasons
decl_stmt|;
name|transport_set_verbosity
argument_list|(
name|transport
argument_list|,
name|verbosity
argument_list|,
name|progress
argument_list|)
expr_stmt|;
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
name|verbosity
operator|>
literal|0
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
name|_
argument_list|(
literal|"Pushing to %s\n"
argument_list|)
argument_list|,
name|transport
operator|->
name|url
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
argument_list|,
operator|&
name|reject_reasons
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
operator|!=
literal|0
condition|)
name|error
argument_list|(
name|_
argument_list|(
literal|"failed to push some refs to '%s'"
argument_list|)
argument_list|,
name|transport
operator|->
name|url
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
return|return
literal|0
return|;
if|if
condition|(
name|reject_reasons
operator|&
name|REJECT_NON_FF_HEAD
condition|)
block|{
name|advise_pull_before_push
argument_list|()
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|reject_reasons
operator|&
name|REJECT_NON_FF_OTHER
condition|)
block|{
if|if
condition|(
name|default_matching_used
condition|)
name|advise_use_upstream
argument_list|()
expr_stmt|;
else|else
name|advise_checkout_pull_push
argument_list|()
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|reject_reasons
operator|&
name|REJECT_ALREADY_EXISTS
condition|)
block|{
name|advise_ref_already_exists
argument_list|()
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|reject_reasons
operator|&
name|REJECT_FETCH_FIRST
condition|)
block|{
name|advise_ref_fetch_first
argument_list|()
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|reject_reasons
operator|&
name|REJECT_NEEDS_FORCE
condition|)
block|{
name|advise_ref_needs_force
argument_list|()
expr_stmt|;
block|}
return|return
literal|1
return|;
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
name|pushremote_get
argument_list|(
name|repo
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
modifier|*
name|url
decl_stmt|;
name|int
name|url_nr
decl_stmt|;
if|if
condition|(
operator|!
name|remote
condition|)
block|{
if|if
condition|(
name|repo
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"bad repository '%s'"
argument_list|)
argument_list|,
name|repo
argument_list|)
expr_stmt|;
name|die
argument_list|(
name|_
argument_list|(
literal|"No configured push destination.\n"
literal|"Either specify the URL from the command-line or configure a remote repository using\n"
literal|"\n"
literal|"    git remote add<name><url>\n"
literal|"\n"
literal|"and then push using the remote name\n"
literal|"\n"
literal|"    git push<name>\n"
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|remote
operator|->
name|mirror
condition|)
name|flags
operator||=
operator|(
name|TRANSPORT_PUSH_MIRROR
operator||
name|TRANSPORT_PUSH_FORCE
operator|)
expr_stmt|;
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
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
operator|*
name|refspec
argument_list|,
literal|"refs/tags/*"
argument_list|)
condition|)
return|return
name|error
argument_list|(
name|_
argument_list|(
literal|"--all and --tags are incompatible"
argument_list|)
argument_list|)
return|;
return|return
name|error
argument_list|(
name|_
argument_list|(
literal|"--all can't be combined with refspecs"
argument_list|)
argument_list|)
return|;
block|}
if|if
condition|(
operator|(
name|flags
operator|&
name|TRANSPORT_PUSH_MIRROR
operator|)
operator|&&
name|refspec
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
operator|*
name|refspec
argument_list|,
literal|"refs/tags/*"
argument_list|)
condition|)
return|return
name|error
argument_list|(
name|_
argument_list|(
literal|"--mirror and --tags are incompatible"
argument_list|)
argument_list|)
return|;
return|return
name|error
argument_list|(
name|_
argument_list|(
literal|"--mirror can't be combined with refspecs"
argument_list|)
argument_list|)
return|;
block|}
if|if
condition|(
operator|(
name|flags
operator|&
operator|(
name|TRANSPORT_PUSH_ALL
operator||
name|TRANSPORT_PUSH_MIRROR
operator|)
operator|)
operator|==
operator|(
name|TRANSPORT_PUSH_ALL
operator||
name|TRANSPORT_PUSH_MIRROR
operator|)
condition|)
block|{
return|return
name|error
argument_list|(
name|_
argument_list|(
literal|"--all and --mirror are incompatible"
argument_list|)
argument_list|)
return|;
block|}
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
condition|)
block|{
if|if
condition|(
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
elseif|else
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|TRANSPORT_PUSH_MIRROR
operator|)
condition|)
name|setup_default_push_refspecs
argument_list|(
name|remote
argument_list|)
expr_stmt|;
block|}
name|errs
operator|=
literal|0
expr_stmt|;
name|url_nr
operator|=
name|push_url_of_remote
argument_list|(
name|remote
argument_list|,
operator|&
name|url
argument_list|)
expr_stmt|;
if|if
condition|(
name|url_nr
condition|)
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
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
name|url
index|[
name|i
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
name|push_with_options
argument_list|(
name|transport
argument_list|,
name|flags
argument_list|)
condition|)
name|errs
operator|++
expr_stmt|;
block|}
block|}
else|else
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
name|NULL
argument_list|)
decl_stmt|;
if|if
condition|(
name|push_with_options
argument_list|(
name|transport
argument_list|,
name|flags
argument_list|)
condition|)
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
DECL|function|option_parse_recurse_submodules
specifier|static
name|int
name|option_parse_recurse_submodules
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
name|int
modifier|*
name|flags
init|=
name|opt
operator|->
name|value
decl_stmt|;
if|if
condition|(
operator|*
name|flags
operator|&
operator|(
name|TRANSPORT_RECURSE_SUBMODULES_CHECK
operator||
name|TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND
operator|)
condition|)
name|die
argument_list|(
literal|"%s can only be used once."
argument_list|,
name|opt
operator|->
name|long_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|arg
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"check"
argument_list|)
condition|)
operator|*
name|flags
operator||=
name|TRANSPORT_RECURSE_SUBMODULES_CHECK
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"on-demand"
argument_list|)
condition|)
operator|*
name|flags
operator||=
name|TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND
expr_stmt|;
else|else
name|die
argument_list|(
literal|"bad %s argument: %s"
argument_list|,
name|opt
operator|->
name|long_name
argument_list|,
name|arg
argument_list|)
expr_stmt|;
block|}
else|else
name|die
argument_list|(
literal|"option %s needs an argument (check|on-demand)"
argument_list|,
name|opt
operator|->
name|long_name
argument_list|)
expr_stmt|;
return|return
literal|0
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
name|tags
init|=
literal|0
decl_stmt|;
name|int
name|rc
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
name|OPT__VERBOSITY
argument_list|(
operator|&
name|verbosity
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
name|N_
argument_list|(
literal|"repository"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"repository"
argument_list|)
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"all"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"push all refs"
argument_list|)
argument_list|,
name|TRANSPORT_PUSH_ALL
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"mirror"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"mirror all refs"
argument_list|)
argument_list|,
operator|(
name|TRANSPORT_PUSH_MIRROR
operator||
name|TRANSPORT_PUSH_FORCE
operator|)
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"delete"
argument_list|,
operator|&
name|deleterefs
argument_list|,
name|N_
argument_list|(
literal|"delete refs"
argument_list|)
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
name|N_
argument_list|(
literal|"push tags (can't be used with --all or --mirror)"
argument_list|)
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|'n'
argument_list|,
literal|"dry-run"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"dry run"
argument_list|)
argument_list|,
name|TRANSPORT_PUSH_DRY_RUN
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"porcelain"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"machine-readable output"
argument_list|)
argument_list|,
name|TRANSPORT_PUSH_PORCELAIN
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|'f'
argument_list|,
literal|"force"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"force updates"
argument_list|)
argument_list|,
name|TRANSPORT_PUSH_FORCE
argument_list|)
block|,
block|{
name|OPTION_CALLBACK
block|,
literal|0
block|,
literal|"recurse-submodules"
block|,
operator|&
name|flags
block|,
name|N_
argument_list|(
literal|"check"
argument_list|)
block|,
name|N_
argument_list|(
literal|"control recursive pushing of submodules"
argument_list|)
block|,
name|PARSE_OPT_OPTARG
block|,
name|option_parse_recurse_submodules
block|}
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
name|N_
argument_list|(
literal|"use thin pack"
argument_list|)
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
name|N_
argument_list|(
literal|"receive pack program"
argument_list|)
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
name|N_
argument_list|(
literal|"receive pack program"
argument_list|)
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|'u'
argument_list|,
literal|"set-upstream"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"set upstream for git pull/status"
argument_list|)
argument_list|,
name|TRANSPORT_PUSH_SET_UPSTREAM
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"progress"
argument_list|,
operator|&
name|progress
argument_list|,
name|N_
argument_list|(
literal|"force progress reporting"
argument_list|)
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"prune"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"prune locally removed refs"
argument_list|)
argument_list|,
name|TRANSPORT_PUSH_PRUNE
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"no-verify"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"bypass pre-push hook"
argument_list|)
argument_list|,
name|TRANSPORT_PUSH_NO_HOOK
argument_list|)
block|,
name|OPT_BIT
argument_list|(
literal|0
argument_list|,
literal|"follow-tags"
argument_list|,
operator|&
name|flags
argument_list|,
name|N_
argument_list|(
literal|"push missing but relevant tags"
argument_list|)
argument_list|,
name|TRANSPORT_PUSH_FOLLOW_TAGS
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|packet_trace_identity
argument_list|(
literal|"push"
argument_list|)
expr_stmt|;
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
name|push_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|deleterefs
operator|&&
operator|(
name|tags
operator|||
operator|(
name|flags
operator|&
operator|(
name|TRANSPORT_PUSH_ALL
operator||
name|TRANSPORT_PUSH_MIRROR
operator|)
operator|)
operator|)
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"--delete is incompatible with --all, --mirror and --tags"
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|deleterefs
operator|&&
name|argc
operator|<
literal|2
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"--delete doesn't make sense without any refs"
argument_list|)
argument_list|)
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
name|rc
operator|=
name|do_push
argument_list|(
name|repo
argument_list|,
name|flags
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
operator|-
literal|1
condition|)
name|usage_with_options
argument_list|(
name|push_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
else|else
return|return
name|rc
return|;
block|}
end_function

end_unit

