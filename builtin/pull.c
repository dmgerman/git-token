begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git pull"  *  * Based on git-pull.sh by Junio C Hamano  *  * Fetch one or more remote refs and merge it/them into the current HEAD.  */
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
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"exec_cmd.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_decl_stmt
DECL|variable|pull_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|pull_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git pull [options] [<repository> [<refspec>...]]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Shared options */
end_comment

begin_decl_stmt
DECL|variable|opt_verbosity
specifier|static
name|int
name|opt_verbosity
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_progress
specifier|static
name|char
modifier|*
name|opt_progress
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Options passed to git-merge */
end_comment

begin_decl_stmt
DECL|variable|opt_diffstat
specifier|static
name|char
modifier|*
name|opt_diffstat
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_log
specifier|static
name|char
modifier|*
name|opt_log
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_squash
specifier|static
name|char
modifier|*
name|opt_squash
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_commit
specifier|static
name|char
modifier|*
name|opt_commit
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_edit
specifier|static
name|char
modifier|*
name|opt_edit
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_ff
specifier|static
name|char
modifier|*
name|opt_ff
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_verify_signatures
specifier|static
name|char
modifier|*
name|opt_verify_signatures
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_strategies
specifier|static
name|struct
name|argv_array
name|opt_strategies
init|=
name|ARGV_ARRAY_INIT
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_strategy_opts
specifier|static
name|struct
name|argv_array
name|opt_strategy_opts
init|=
name|ARGV_ARRAY_INIT
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_gpg_sign
specifier|static
name|char
modifier|*
name|opt_gpg_sign
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Options passed to git-fetch */
end_comment

begin_decl_stmt
DECL|variable|opt_all
specifier|static
name|char
modifier|*
name|opt_all
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_append
specifier|static
name|char
modifier|*
name|opt_append
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_upload_pack
specifier|static
name|char
modifier|*
name|opt_upload_pack
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_force
specifier|static
name|int
name|opt_force
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_tags
specifier|static
name|char
modifier|*
name|opt_tags
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_prune
specifier|static
name|char
modifier|*
name|opt_prune
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_recurse_submodules
specifier|static
name|char
modifier|*
name|opt_recurse_submodules
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_dry_run
specifier|static
name|int
name|opt_dry_run
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_keep
specifier|static
name|char
modifier|*
name|opt_keep
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_depth
specifier|static
name|char
modifier|*
name|opt_depth
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_unshallow
specifier|static
name|char
modifier|*
name|opt_unshallow
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_update_shallow
specifier|static
name|char
modifier|*
name|opt_update_shallow
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|opt_refmap
specifier|static
name|char
modifier|*
name|opt_refmap
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pull_options
specifier|static
name|struct
name|option
name|pull_options
index|[]
init|=
block|{
comment|/* Shared options */
name|OPT__VERBOSITY
argument_list|(
operator|&
name|opt_verbosity
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"progress"
argument_list|,
operator|&
name|opt_progress
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"force progress reporting"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
comment|/* Options passed to git-merge */
name|OPT_GROUP
argument_list|(
name|N_
argument_list|(
literal|"Options related to merging"
argument_list|)
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|'n'
argument_list|,
name|NULL
argument_list|,
operator|&
name|opt_diffstat
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"do not show a diffstat at the end of the merge"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
operator||
name|PARSE_OPT_NONEG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"stat"
argument_list|,
operator|&
name|opt_diffstat
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"show a diffstat at the end of the merge"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"summary"
argument_list|,
operator|&
name|opt_diffstat
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"(synonym to --stat)"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
operator||
name|PARSE_OPT_HIDDEN
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"log"
argument_list|,
operator|&
name|opt_log
argument_list|,
name|N_
argument_list|(
literal|"n"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"add (at most<n>) entries from shortlog to merge commit message"
argument_list|)
argument_list|,
name|PARSE_OPT_OPTARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"squash"
argument_list|,
operator|&
name|opt_squash
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"create a single commit instead of doing a merge"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"commit"
argument_list|,
operator|&
name|opt_commit
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"perform a commit if the merge succeeds (default)"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"edit"
argument_list|,
operator|&
name|opt_edit
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"edit message before committing"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"ff"
argument_list|,
operator|&
name|opt_ff
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"allow fast-forward"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"ff-only"
argument_list|,
operator|&
name|opt_ff
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"abort if fast-forward is not possible"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
operator||
name|PARSE_OPT_NONEG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"verify-signatures"
argument_list|,
operator|&
name|opt_verify_signatures
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"verify that the named commit has a valid GPG signature"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU_ARGV
argument_list|(
literal|'s'
argument_list|,
literal|"strategy"
argument_list|,
operator|&
name|opt_strategies
argument_list|,
name|N_
argument_list|(
literal|"strategy"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"merge strategy to use"
argument_list|)
argument_list|,
literal|0
argument_list|)
block|,
name|OPT_PASSTHRU_ARGV
argument_list|(
literal|'X'
argument_list|,
literal|"strategy-option"
argument_list|,
operator|&
name|opt_strategy_opts
argument_list|,
name|N_
argument_list|(
literal|"option=value"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"option for selected merge strategy"
argument_list|)
argument_list|,
literal|0
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|'S'
argument_list|,
literal|"gpg-sign"
argument_list|,
operator|&
name|opt_gpg_sign
argument_list|,
name|N_
argument_list|(
literal|"key-id"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"GPG sign commit"
argument_list|)
argument_list|,
name|PARSE_OPT_OPTARG
argument_list|)
block|,
comment|/* Options passed to git-fetch */
name|OPT_GROUP
argument_list|(
name|N_
argument_list|(
literal|"Options related to fetching"
argument_list|)
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"all"
argument_list|,
operator|&
name|opt_all
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"fetch from all remotes"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|'a'
argument_list|,
literal|"append"
argument_list|,
operator|&
name|opt_append
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"append to .git/FETCH_HEAD instead of overwriting"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"upload-pack"
argument_list|,
operator|&
name|opt_upload_pack
argument_list|,
name|N_
argument_list|(
literal|"path"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"path to upload pack on remote end"
argument_list|)
argument_list|,
literal|0
argument_list|)
block|,
name|OPT__FORCE
argument_list|(
operator|&
name|opt_force
argument_list|,
name|N_
argument_list|(
literal|"force overwrite of local branch"
argument_list|)
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|'t'
argument_list|,
literal|"tags"
argument_list|,
operator|&
name|opt_tags
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"fetch all tags and associated objects"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|'p'
argument_list|,
literal|"prune"
argument_list|,
operator|&
name|opt_prune
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"prune remote-tracking branches no longer on remote"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"recurse-submodules"
argument_list|,
operator|&
name|opt_recurse_submodules
argument_list|,
name|N_
argument_list|(
literal|"on-demand"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"control recursive fetching of submodules"
argument_list|)
argument_list|,
name|PARSE_OPT_OPTARG
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"dry-run"
argument_list|,
operator|&
name|opt_dry_run
argument_list|,
name|N_
argument_list|(
literal|"dry run"
argument_list|)
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|'k'
argument_list|,
literal|"keep"
argument_list|,
operator|&
name|opt_keep
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"keep downloaded pack"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"depth"
argument_list|,
operator|&
name|opt_depth
argument_list|,
name|N_
argument_list|(
literal|"depth"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"deepen history of shallow clone"
argument_list|)
argument_list|,
literal|0
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"unshallow"
argument_list|,
operator|&
name|opt_unshallow
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"convert to a complete repository"
argument_list|)
argument_list|,
name|PARSE_OPT_NONEG
operator||
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"update-shallow"
argument_list|,
operator|&
name|opt_update_shallow
argument_list|,
name|NULL
argument_list|,
name|N_
argument_list|(
literal|"accept refs that update .git/shallow"
argument_list|)
argument_list|,
name|PARSE_OPT_NOARG
argument_list|)
block|,
name|OPT_PASSTHRU
argument_list|(
literal|0
argument_list|,
literal|"refmap"
argument_list|,
operator|&
name|opt_refmap
argument_list|,
name|N_
argument_list|(
literal|"refmap"
argument_list|)
argument_list|,
name|N_
argument_list|(
literal|"specify fetch refmap"
argument_list|)
argument_list|,
name|PARSE_OPT_NONEG
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
end_decl_stmt

begin_comment
comment|/**  * Pushes "-q" or "-v" switches into arr to match the opt_verbosity level.  */
end_comment

begin_function
DECL|function|argv_push_verbosity
specifier|static
name|void
name|argv_push_verbosity
parameter_list|(
name|struct
name|argv_array
modifier|*
name|arr
parameter_list|)
block|{
name|int
name|verbosity
decl_stmt|;
for|for
control|(
name|verbosity
operator|=
name|opt_verbosity
init|;
name|verbosity
operator|>
literal|0
condition|;
name|verbosity
operator|--
control|)
name|argv_array_push
argument_list|(
name|arr
argument_list|,
literal|"-v"
argument_list|)
expr_stmt|;
for|for
control|(
name|verbosity
operator|=
name|opt_verbosity
init|;
name|verbosity
operator|<
literal|0
condition|;
name|verbosity
operator|++
control|)
name|argv_array_push
argument_list|(
name|arr
argument_list|,
literal|"-q"
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/**  * Pushes "-f" switches into arr to match the opt_force level.  */
end_comment

begin_function
DECL|function|argv_push_force
specifier|static
name|void
name|argv_push_force
parameter_list|(
name|struct
name|argv_array
modifier|*
name|arr
parameter_list|)
block|{
name|int
name|force
init|=
name|opt_force
decl_stmt|;
while|while
condition|(
name|force
operator|--
operator|>
literal|0
condition|)
name|argv_array_push
argument_list|(
name|arr
argument_list|,
literal|"-f"
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/**  * Parses argv into [<repo> [<refspecs>...]], returning their values in `repo`  * as a string and `refspecs` as a null-terminated array of strings. If `repo`  * is not provided in argv, it is set to NULL.  */
end_comment

begin_function
DECL|function|parse_repo_refspecs
specifier|static
name|void
name|parse_repo_refspecs
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
modifier|*
name|repo
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
modifier|*
name|refspecs
parameter_list|)
block|{
if|if
condition|(
name|argc
operator|>
literal|0
condition|)
block|{
operator|*
name|repo
operator|=
operator|*
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
block|}
else|else
operator|*
name|repo
operator|=
name|NULL
expr_stmt|;
operator|*
name|refspecs
operator|=
name|argv
expr_stmt|;
block|}
end_function

begin_comment
comment|/**  * Runs git-fetch, returning its exit status. `repo` and `refspecs` are the  * repository and refspecs to fetch, or NULL if they are not provided.  */
end_comment

begin_function
DECL|function|run_fetch
specifier|static
name|int
name|run_fetch
parameter_list|(
specifier|const
name|char
modifier|*
name|repo
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspecs
parameter_list|)
block|{
name|struct
name|argv_array
name|args
init|=
name|ARGV_ARRAY_INIT
decl_stmt|;
name|int
name|ret
decl_stmt|;
name|argv_array_pushl
argument_list|(
operator|&
name|args
argument_list|,
literal|"fetch"
argument_list|,
literal|"--update-head-ok"
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
comment|/* Shared options */
name|argv_push_verbosity
argument_list|(
operator|&
name|args
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_progress
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_progress
argument_list|)
expr_stmt|;
comment|/* Options passed to git-fetch */
if|if
condition|(
name|opt_all
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_all
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_append
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_append
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_upload_pack
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_upload_pack
argument_list|)
expr_stmt|;
name|argv_push_force
argument_list|(
operator|&
name|args
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_tags
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_tags
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_prune
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_prune
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_recurse_submodules
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_recurse_submodules
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_dry_run
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
literal|"--dry-run"
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_keep
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_keep
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_depth
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_depth
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_unshallow
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_unshallow
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_update_shallow
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_update_shallow
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_refmap
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_refmap
argument_list|)
expr_stmt|;
if|if
condition|(
name|repo
condition|)
block|{
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|repo
argument_list|)
expr_stmt|;
name|argv_array_pushv
argument_list|(
operator|&
name|args
argument_list|,
name|refspecs
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|*
name|refspecs
condition|)
name|die
argument_list|(
literal|"BUG: refspecs without repo?"
argument_list|)
expr_stmt|;
name|ret
operator|=
name|run_command_v_opt
argument_list|(
name|args
operator|.
name|argv
argument_list|,
name|RUN_GIT_CMD
argument_list|)
expr_stmt|;
name|argv_array_clear
argument_list|(
operator|&
name|args
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_comment
comment|/**  * Runs git-merge, returning its exit status.  */
end_comment

begin_function
DECL|function|run_merge
specifier|static
name|int
name|run_merge
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|ret
decl_stmt|;
name|struct
name|argv_array
name|args
init|=
name|ARGV_ARRAY_INIT
decl_stmt|;
name|argv_array_pushl
argument_list|(
operator|&
name|args
argument_list|,
literal|"merge"
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
comment|/* Shared options */
name|argv_push_verbosity
argument_list|(
operator|&
name|args
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_progress
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_progress
argument_list|)
expr_stmt|;
comment|/* Options passed to git-merge */
if|if
condition|(
name|opt_diffstat
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_diffstat
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_log
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_log
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_squash
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_squash
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_commit
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_commit
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_edit
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_edit
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_ff
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_ff
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_verify_signatures
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_verify_signatures
argument_list|)
expr_stmt|;
name|argv_array_pushv
argument_list|(
operator|&
name|args
argument_list|,
name|opt_strategies
operator|.
name|argv
argument_list|)
expr_stmt|;
name|argv_array_pushv
argument_list|(
operator|&
name|args
argument_list|,
name|opt_strategy_opts
operator|.
name|argv
argument_list|)
expr_stmt|;
if|if
condition|(
name|opt_gpg_sign
condition|)
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
name|opt_gpg_sign
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|args
argument_list|,
literal|"FETCH_HEAD"
argument_list|)
expr_stmt|;
name|ret
operator|=
name|run_command_v_opt
argument_list|(
name|args
operator|.
name|argv
argument_list|,
name|RUN_GIT_CMD
argument_list|)
expr_stmt|;
name|argv_array_clear
argument_list|(
operator|&
name|args
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|cmd_pull
name|int
name|cmd_pull
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
name|repo
decl_stmt|,
modifier|*
modifier|*
name|refspecs
decl_stmt|;
if|if
condition|(
operator|!
name|getenv
argument_list|(
literal|"_GIT_USE_BUILTIN_PULL"
argument_list|)
condition|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|mkpath
argument_list|(
literal|"%s/git-pull"
argument_list|,
name|git_exec_path
argument_list|()
argument_list|)
decl_stmt|;
if|if
condition|(
name|sane_execvp
argument_list|(
name|path
argument_list|,
operator|(
name|char
operator|*
operator|*
operator|)
name|argv
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"could not exec %s"
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
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
name|pull_options
argument_list|,
name|pull_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|parse_repo_refspecs
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
operator|&
name|repo
argument_list|,
operator|&
name|refspecs
argument_list|)
expr_stmt|;
if|if
condition|(
name|run_fetch
argument_list|(
name|repo
argument_list|,
name|refspecs
argument_list|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|opt_dry_run
condition|)
return|return
literal|0
return|;
return|return
name|run_merge
argument_list|()
return|;
block|}
end_function

end_unit

