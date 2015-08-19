begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"branch.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"remote.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_struct
DECL|struct|tracking
struct|struct
name|tracking
block|{
DECL|member|spec
name|struct
name|refspec
name|spec
decl_stmt|;
DECL|member|src
name|char
modifier|*
name|src
decl_stmt|;
DECL|member|remote
specifier|const
name|char
modifier|*
name|remote
decl_stmt|;
DECL|member|matches
name|int
name|matches
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|find_tracked_branch
specifier|static
name|int
name|find_tracked_branch
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
name|void
modifier|*
name|priv
parameter_list|)
block|{
name|struct
name|tracking
modifier|*
name|tracking
init|=
name|priv
decl_stmt|;
if|if
condition|(
operator|!
name|remote_find_tracking
argument_list|(
name|remote
argument_list|,
operator|&
name|tracking
operator|->
name|spec
argument_list|)
condition|)
block|{
if|if
condition|(
operator|++
name|tracking
operator|->
name|matches
operator|==
literal|1
condition|)
block|{
name|tracking
operator|->
name|src
operator|=
name|tracking
operator|->
name|spec
operator|.
name|src
expr_stmt|;
name|tracking
operator|->
name|remote
operator|=
name|remote
operator|->
name|name
expr_stmt|;
block|}
else|else
block|{
name|free
argument_list|(
name|tracking
operator|->
name|spec
operator|.
name|src
argument_list|)
expr_stmt|;
if|if
condition|(
name|tracking
operator|->
name|src
condition|)
block|{
name|free
argument_list|(
name|tracking
operator|->
name|src
argument_list|)
expr_stmt|;
name|tracking
operator|->
name|src
operator|=
name|NULL
expr_stmt|;
block|}
block|}
name|tracking
operator|->
name|spec
operator|.
name|src
operator|=
name|NULL
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|should_setup_rebase
specifier|static
name|int
name|should_setup_rebase
parameter_list|(
specifier|const
name|char
modifier|*
name|origin
parameter_list|)
block|{
switch|switch
condition|(
name|autorebase
condition|)
block|{
case|case
name|AUTOREBASE_NEVER
case|:
return|return
literal|0
return|;
case|case
name|AUTOREBASE_LOCAL
case|:
return|return
name|origin
operator|==
name|NULL
return|;
case|case
name|AUTOREBASE_REMOTE
case|:
return|return
name|origin
operator|!=
name|NULL
return|;
case|case
name|AUTOREBASE_ALWAYS
case|:
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
DECL|function|install_branch_config
name|void
name|install_branch_config
parameter_list|(
name|int
name|flag
parameter_list|,
specifier|const
name|char
modifier|*
name|local
parameter_list|,
specifier|const
name|char
modifier|*
name|origin
parameter_list|,
specifier|const
name|char
modifier|*
name|remote
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|shortname
init|=
name|NULL
decl_stmt|;
name|struct
name|strbuf
name|key
init|=
name|STRBUF_INIT
decl_stmt|;
name|int
name|rebasing
init|=
name|should_setup_rebase
argument_list|(
name|origin
argument_list|)
decl_stmt|;
if|if
condition|(
name|skip_prefix
argument_list|(
name|remote
argument_list|,
literal|"refs/heads/"
argument_list|,
operator|&
name|shortname
argument_list|)
operator|&&
operator|!
name|strcmp
argument_list|(
name|local
argument_list|,
name|shortname
argument_list|)
operator|&&
operator|!
name|origin
condition|)
block|{
name|warning
argument_list|(
name|_
argument_list|(
literal|"Not setting branch %s as its own upstream."
argument_list|)
argument_list|,
name|local
argument_list|)
expr_stmt|;
return|return;
block|}
name|strbuf_addf
argument_list|(
operator|&
name|key
argument_list|,
literal|"branch.%s.remote"
argument_list|,
name|local
argument_list|)
expr_stmt|;
name|git_config_set
argument_list|(
name|key
operator|.
name|buf
argument_list|,
name|origin
condition|?
name|origin
else|:
literal|"."
argument_list|)
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|key
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|key
argument_list|,
literal|"branch.%s.merge"
argument_list|,
name|local
argument_list|)
expr_stmt|;
name|git_config_set
argument_list|(
name|key
operator|.
name|buf
argument_list|,
name|remote
argument_list|)
expr_stmt|;
if|if
condition|(
name|rebasing
condition|)
block|{
name|strbuf_reset
argument_list|(
operator|&
name|key
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|key
argument_list|,
literal|"branch.%s.rebase"
argument_list|,
name|local
argument_list|)
expr_stmt|;
name|git_config_set
argument_list|(
name|key
operator|.
name|buf
argument_list|,
literal|"true"
argument_list|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|key
argument_list|)
expr_stmt|;
if|if
condition|(
name|flag
operator|&
name|BRANCH_CONFIG_VERBOSE
condition|)
block|{
if|if
condition|(
name|shortname
condition|)
block|{
if|if
condition|(
name|origin
condition|)
name|printf_ln
argument_list|(
name|rebasing
condition|?
name|_
argument_list|(
literal|"Branch %s set up to track remote branch %s from %s by rebasing."
argument_list|)
else|:
name|_
argument_list|(
literal|"Branch %s set up to track remote branch %s from %s."
argument_list|)
argument_list|,
name|local
argument_list|,
name|shortname
argument_list|,
name|origin
argument_list|)
expr_stmt|;
else|else
name|printf_ln
argument_list|(
name|rebasing
condition|?
name|_
argument_list|(
literal|"Branch %s set up to track local branch %s by rebasing."
argument_list|)
else|:
name|_
argument_list|(
literal|"Branch %s set up to track local branch %s."
argument_list|)
argument_list|,
name|local
argument_list|,
name|shortname
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|origin
condition|)
name|printf_ln
argument_list|(
name|rebasing
condition|?
name|_
argument_list|(
literal|"Branch %s set up to track remote ref %s by rebasing."
argument_list|)
else|:
name|_
argument_list|(
literal|"Branch %s set up to track remote ref %s."
argument_list|)
argument_list|,
name|local
argument_list|,
name|remote
argument_list|)
expr_stmt|;
else|else
name|printf_ln
argument_list|(
name|rebasing
condition|?
name|_
argument_list|(
literal|"Branch %s set up to track local ref %s by rebasing."
argument_list|)
else|:
name|_
argument_list|(
literal|"Branch %s set up to track local ref %s."
argument_list|)
argument_list|,
name|local
argument_list|,
name|remote
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_comment
comment|/*  * This is called when new_ref is branched off of orig_ref, and tries  * to infer the settings for branch.<new_ref>.{remote,merge} from the  * config.  */
end_comment

begin_function
DECL|function|setup_tracking
specifier|static
name|int
name|setup_tracking
parameter_list|(
specifier|const
name|char
modifier|*
name|new_ref
parameter_list|,
specifier|const
name|char
modifier|*
name|orig_ref
parameter_list|,
name|enum
name|branch_track
name|track
parameter_list|,
name|int
name|quiet
parameter_list|)
block|{
name|struct
name|tracking
name|tracking
decl_stmt|;
name|int
name|config_flags
init|=
name|quiet
condition|?
literal|0
else|:
name|BRANCH_CONFIG_VERBOSE
decl_stmt|;
name|memset
argument_list|(
operator|&
name|tracking
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|tracking
argument_list|)
argument_list|)
expr_stmt|;
name|tracking
operator|.
name|spec
operator|.
name|dst
operator|=
operator|(
name|char
operator|*
operator|)
name|orig_ref
expr_stmt|;
if|if
condition|(
name|for_each_remote
argument_list|(
name|find_tracked_branch
argument_list|,
operator|&
name|tracking
argument_list|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
operator|!
name|tracking
operator|.
name|matches
condition|)
switch|switch
condition|(
name|track
condition|)
block|{
case|case
name|BRANCH_TRACK_ALWAYS
case|:
case|case
name|BRANCH_TRACK_EXPLICIT
case|:
case|case
name|BRANCH_TRACK_OVERRIDE
case|:
break|break;
default|default:
return|return
literal|1
return|;
block|}
if|if
condition|(
name|tracking
operator|.
name|matches
operator|>
literal|1
condition|)
return|return
name|error
argument_list|(
name|_
argument_list|(
literal|"Not tracking: ambiguous information for ref %s"
argument_list|)
argument_list|,
name|orig_ref
argument_list|)
return|;
name|install_branch_config
argument_list|(
name|config_flags
argument_list|,
name|new_ref
argument_list|,
name|tracking
operator|.
name|remote
argument_list|,
name|tracking
operator|.
name|src
condition|?
name|tracking
operator|.
name|src
else|:
name|orig_ref
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tracking
operator|.
name|src
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|read_branch_desc
name|int
name|read_branch_desc
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|branch_name
parameter_list|)
block|{
name|char
modifier|*
name|v
init|=
name|NULL
decl_stmt|;
name|struct
name|strbuf
name|name
init|=
name|STRBUF_INIT
decl_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|name
argument_list|,
literal|"branch.%s.description"
argument_list|,
name|branch_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|git_config_get_string
argument_list|(
name|name
operator|.
name|buf
argument_list|,
operator|&
name|v
argument_list|)
condition|)
block|{
name|strbuf_release
argument_list|(
operator|&
name|name
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|strbuf_addstr
argument_list|(
name|buf
argument_list|,
name|v
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|v
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|name
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|validate_new_branchname
name|int
name|validate_new_branchname
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|struct
name|strbuf
modifier|*
name|ref
parameter_list|,
name|int
name|force
parameter_list|,
name|int
name|attr_only
parameter_list|)
block|{
if|if
condition|(
name|strbuf_check_branch_ref
argument_list|(
name|ref
argument_list|,
name|name
argument_list|)
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"'%s' is not a valid branch name."
argument_list|)
argument_list|,
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ref_exists
argument_list|(
name|ref
operator|->
name|buf
argument_list|)
condition|)
return|return
literal|0
return|;
elseif|else
if|if
condition|(
operator|!
name|force
operator|&&
operator|!
name|attr_only
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"A branch named '%s' already exists."
argument_list|)
argument_list|,
name|ref
operator|->
name|buf
operator|+
name|strlen
argument_list|(
literal|"refs/heads/"
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|attr_only
condition|)
block|{
specifier|const
name|char
modifier|*
name|head
decl_stmt|;
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|head
operator|=
name|resolve_ref_unsafe
argument_list|(
literal|"HEAD"
argument_list|,
literal|0
argument_list|,
name|sha1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_bare_repository
argument_list|()
operator|&&
name|head
operator|&&
operator|!
name|strcmp
argument_list|(
name|head
argument_list|,
name|ref
operator|->
name|buf
argument_list|)
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"Cannot force update the current branch."
argument_list|)
argument_list|)
expr_stmt|;
block|}
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|check_tracking_branch
specifier|static
name|int
name|check_tracking_branch
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
name|char
modifier|*
name|tracking_branch
init|=
name|cb_data
decl_stmt|;
name|struct
name|refspec
name|query
decl_stmt|;
name|memset
argument_list|(
operator|&
name|query
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|refspec
argument_list|)
argument_list|)
expr_stmt|;
name|query
operator|.
name|dst
operator|=
name|tracking_branch
expr_stmt|;
return|return
operator|!
name|remote_find_tracking
argument_list|(
name|remote
argument_list|,
operator|&
name|query
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|validate_remote_tracking_branch
specifier|static
name|int
name|validate_remote_tracking_branch
parameter_list|(
name|char
modifier|*
name|ref
parameter_list|)
block|{
return|return
operator|!
name|for_each_remote
argument_list|(
name|check_tracking_branch
argument_list|,
name|ref
argument_list|)
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|upstream_not_branch
specifier|static
specifier|const
name|char
name|upstream_not_branch
index|[]
init|=
name|N_
argument_list|(
literal|"Cannot setup tracking information; starting point '%s' is not a branch."
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|upstream_missing
specifier|static
specifier|const
name|char
name|upstream_missing
index|[]
init|=
name|N_
argument_list|(
literal|"the requested upstream branch '%s' does not exist"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|upstream_advice
specifier|static
specifier|const
name|char
name|upstream_advice
index|[]
init|=
name|N_
argument_list|(
literal|"\n"
literal|"If you are planning on basing your work on an upstream\n"
literal|"branch that already exists at the remote, you may need to\n"
literal|"run \"git fetch\" to retrieve it.\n"
literal|"\n"
literal|"If you are planning to push out a new local branch that\n"
literal|"will track its remote counterpart, you may want to use\n"
literal|"\"git push -u\" to set the upstream config as you push."
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|create_branch
name|void
name|create_branch
parameter_list|(
specifier|const
name|char
modifier|*
name|head
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|start_name
parameter_list|,
name|int
name|force
parameter_list|,
name|int
name|reflog
parameter_list|,
name|int
name|clobber_head
parameter_list|,
name|int
name|quiet
parameter_list|,
name|enum
name|branch_track
name|track
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit
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
name|real_ref
decl_stmt|,
name|msg
index|[
name|PATH_MAX
operator|+
literal|20
index|]
decl_stmt|;
name|struct
name|strbuf
name|ref
init|=
name|STRBUF_INIT
decl_stmt|;
name|int
name|forcing
init|=
literal|0
decl_stmt|;
name|int
name|dont_change_ref
init|=
literal|0
decl_stmt|;
name|int
name|explicit_tracking
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|track
operator|==
name|BRANCH_TRACK_EXPLICIT
operator|||
name|track
operator|==
name|BRANCH_TRACK_OVERRIDE
condition|)
name|explicit_tracking
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|validate_new_branchname
argument_list|(
name|name
argument_list|,
operator|&
name|ref
argument_list|,
name|force
argument_list|,
name|track
operator|==
name|BRANCH_TRACK_OVERRIDE
operator|||
name|clobber_head
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|force
condition|)
name|dont_change_ref
operator|=
literal|1
expr_stmt|;
else|else
name|forcing
operator|=
literal|1
expr_stmt|;
block|}
name|real_ref
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|get_sha1
argument_list|(
name|start_name
argument_list|,
name|sha1
argument_list|)
condition|)
block|{
if|if
condition|(
name|explicit_tracking
condition|)
block|{
if|if
condition|(
name|advice_set_upstream_failure
condition|)
block|{
name|error
argument_list|(
name|_
argument_list|(
name|upstream_missing
argument_list|)
argument_list|,
name|start_name
argument_list|)
expr_stmt|;
name|advise
argument_list|(
name|_
argument_list|(
name|upstream_advice
argument_list|)
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
name|die
argument_list|(
name|_
argument_list|(
name|upstream_missing
argument_list|)
argument_list|,
name|start_name
argument_list|)
expr_stmt|;
block|}
name|die
argument_list|(
name|_
argument_list|(
literal|"Not a valid object name: '%s'."
argument_list|)
argument_list|,
name|start_name
argument_list|)
expr_stmt|;
block|}
switch|switch
condition|(
name|dwim_ref
argument_list|(
name|start_name
argument_list|,
name|strlen
argument_list|(
name|start_name
argument_list|)
argument_list|,
name|sha1
argument_list|,
operator|&
name|real_ref
argument_list|)
condition|)
block|{
case|case
literal|0
case|:
comment|/* Not branching from any existing branch */
if|if
condition|(
name|explicit_tracking
condition|)
name|die
argument_list|(
name|_
argument_list|(
name|upstream_not_branch
argument_list|)
argument_list|,
name|start_name
argument_list|)
expr_stmt|;
break|break;
case|case
literal|1
case|:
comment|/* Unique completion -- good, only if it is a real branch */
if|if
condition|(
operator|!
name|starts_with
argument_list|(
name|real_ref
argument_list|,
literal|"refs/heads/"
argument_list|)
operator|&&
name|validate_remote_tracking_branch
argument_list|(
name|real_ref
argument_list|)
condition|)
block|{
if|if
condition|(
name|explicit_tracking
condition|)
name|die
argument_list|(
name|_
argument_list|(
name|upstream_not_branch
argument_list|)
argument_list|,
name|start_name
argument_list|)
expr_stmt|;
else|else
name|real_ref
operator|=
name|NULL
expr_stmt|;
block|}
break|break;
default|default:
name|die
argument_list|(
name|_
argument_list|(
literal|"Ambiguous object name: '%s'."
argument_list|)
argument_list|,
name|start_name
argument_list|)
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|(
name|commit
operator|=
name|lookup_commit_reference
argument_list|(
name|sha1
argument_list|)
operator|)
operator|==
name|NULL
condition|)
name|die
argument_list|(
name|_
argument_list|(
literal|"Not a valid branch point: '%s'."
argument_list|)
argument_list|,
name|start_name
argument_list|)
expr_stmt|;
name|hashcpy
argument_list|(
name|sha1
argument_list|,
name|commit
operator|->
name|object
operator|.
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|forcing
condition|)
name|snprintf
argument_list|(
name|msg
argument_list|,
sizeof|sizeof
name|msg
argument_list|,
literal|"branch: Reset to %s"
argument_list|,
name|start_name
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|dont_change_ref
condition|)
name|snprintf
argument_list|(
name|msg
argument_list|,
sizeof|sizeof
name|msg
argument_list|,
literal|"branch: Created from %s"
argument_list|,
name|start_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|reflog
condition|)
name|log_all_ref_updates
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|dont_change_ref
condition|)
block|{
name|struct
name|ref_transaction
modifier|*
name|transaction
decl_stmt|;
name|struct
name|strbuf
name|err
init|=
name|STRBUF_INIT
decl_stmt|;
name|transaction
operator|=
name|ref_transaction_begin
argument_list|(
operator|&
name|err
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|transaction
operator|||
name|ref_transaction_update
argument_list|(
name|transaction
argument_list|,
name|ref
operator|.
name|buf
argument_list|,
name|sha1
argument_list|,
name|forcing
condition|?
name|NULL
else|:
name|null_sha1
argument_list|,
literal|0
argument_list|,
name|msg
argument_list|,
operator|&
name|err
argument_list|)
operator|||
name|ref_transaction_commit
argument_list|(
name|transaction
argument_list|,
operator|&
name|err
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s"
argument_list|,
name|err
operator|.
name|buf
argument_list|)
expr_stmt|;
name|ref_transaction_free
argument_list|(
name|transaction
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|err
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|real_ref
operator|&&
name|track
condition|)
name|setup_tracking
argument_list|(
name|ref
operator|.
name|buf
operator|+
literal|11
argument_list|,
name|real_ref
argument_list|,
name|track
argument_list|,
name|quiet
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|ref
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|real_ref
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|remove_branch_state
name|void
name|remove_branch_state
parameter_list|(
name|void
parameter_list|)
block|{
name|unlink
argument_list|(
name|git_path_cherry_pick_head
argument_list|()
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|git_path_revert_head
argument_list|()
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|git_path_merge_head
argument_list|()
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|git_path_merge_rr
argument_list|()
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|git_path_merge_msg
argument_list|()
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|git_path_merge_mode
argument_list|()
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|git_path_squash_msg
argument_list|()
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|check_linked_checkout
specifier|static
name|void
name|check_linked_checkout
parameter_list|(
specifier|const
name|char
modifier|*
name|branch
parameter_list|,
specifier|const
name|char
modifier|*
name|id
parameter_list|)
block|{
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|path
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|gitdir
init|=
name|STRBUF_INIT
decl_stmt|;
comment|/* 	 * $GIT_COMMON_DIR/HEAD is practically outside 	 * $GIT_DIR so resolve_ref_unsafe() won't work (it 	 * uses git_path). Parse the ref ourselves. 	 */
if|if
condition|(
name|id
condition|)
name|strbuf_addf
argument_list|(
operator|&
name|path
argument_list|,
literal|"%s/worktrees/%s/HEAD"
argument_list|,
name|get_git_common_dir
argument_list|()
argument_list|,
name|id
argument_list|)
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
operator|&
name|path
argument_list|,
literal|"%s/HEAD"
argument_list|,
name|get_git_common_dir
argument_list|()
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strbuf_readlink
argument_list|(
operator|&
name|sb
argument_list|,
name|path
operator|.
name|buf
argument_list|,
literal|0
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|starts_with
argument_list|(
name|sb
operator|.
name|buf
argument_list|,
literal|"refs/"
argument_list|)
operator|||
name|check_refname_format
argument_list|(
name|sb
operator|.
name|buf
argument_list|,
literal|0
argument_list|)
condition|)
goto|goto
name|done
goto|;
block|}
elseif|else
if|if
condition|(
name|strbuf_read_file
argument_list|(
operator|&
name|sb
argument_list|,
name|path
operator|.
name|buf
argument_list|,
literal|0
argument_list|)
operator|>=
literal|0
operator|&&
name|starts_with
argument_list|(
name|sb
operator|.
name|buf
argument_list|,
literal|"ref:"
argument_list|)
condition|)
block|{
name|strbuf_remove
argument_list|(
operator|&
name|sb
argument_list|,
literal|0
argument_list|,
name|strlen
argument_list|(
literal|"ref:"
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_trim
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
block|}
else|else
goto|goto
name|done
goto|;
if|if
condition|(
name|strcmp
argument_list|(
name|sb
operator|.
name|buf
argument_list|,
name|branch
argument_list|)
condition|)
goto|goto
name|done
goto|;
if|if
condition|(
name|id
condition|)
block|{
name|strbuf_reset
argument_list|(
operator|&
name|path
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|path
argument_list|,
literal|"%s/worktrees/%s/gitdir"
argument_list|,
name|get_git_common_dir
argument_list|()
argument_list|,
name|id
argument_list|)
expr_stmt|;
if|if
condition|(
name|strbuf_read_file
argument_list|(
operator|&
name|gitdir
argument_list|,
name|path
operator|.
name|buf
argument_list|,
literal|0
argument_list|)
operator|<=
literal|0
condition|)
goto|goto
name|done
goto|;
name|strbuf_rtrim
argument_list|(
operator|&
name|gitdir
argument_list|)
expr_stmt|;
block|}
else|else
name|strbuf_addstr
argument_list|(
operator|&
name|gitdir
argument_list|,
name|get_git_common_dir
argument_list|()
argument_list|)
expr_stmt|;
name|skip_prefix
argument_list|(
name|branch
argument_list|,
literal|"refs/heads/"
argument_list|,
operator|&
name|branch
argument_list|)
expr_stmt|;
name|strbuf_strip_suffix
argument_list|(
operator|&
name|gitdir
argument_list|,
literal|".git"
argument_list|)
expr_stmt|;
name|die
argument_list|(
name|_
argument_list|(
literal|"'%s' is already checked out at '%s'"
argument_list|)
argument_list|,
name|branch
argument_list|,
name|gitdir
operator|.
name|buf
argument_list|)
expr_stmt|;
name|done
label|:
name|strbuf_release
argument_list|(
operator|&
name|path
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|gitdir
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|die_if_checked_out
name|void
name|die_if_checked_out
parameter_list|(
specifier|const
name|char
modifier|*
name|branch
parameter_list|)
block|{
name|struct
name|strbuf
name|path
init|=
name|STRBUF_INIT
decl_stmt|;
name|DIR
modifier|*
name|dir
decl_stmt|;
name|struct
name|dirent
modifier|*
name|d
decl_stmt|;
name|check_linked_checkout
argument_list|(
name|branch
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|path
argument_list|,
literal|"%s/worktrees"
argument_list|,
name|get_git_common_dir
argument_list|()
argument_list|)
expr_stmt|;
name|dir
operator|=
name|opendir
argument_list|(
name|path
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dir
condition|)
return|return;
while|while
condition|(
operator|(
name|d
operator|=
name|readdir
argument_list|(
name|dir
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|d
operator|->
name|d_name
argument_list|,
literal|"."
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|d
operator|->
name|d_name
argument_list|,
literal|".."
argument_list|)
condition|)
continue|continue;
name|check_linked_checkout
argument_list|(
name|branch
argument_list|,
name|d
operator|->
name|d_name
argument_list|)
expr_stmt|;
block|}
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

