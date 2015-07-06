begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"argv-array.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_decl_stmt
DECL|variable|worktree_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|worktree_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git worktree add [<options>]<path><branch>"
argument_list|)
block|,
name|N_
argument_list|(
literal|"git worktree prune [<options>]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_only
specifier|static
name|int
name|show_only
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|verbose
specifier|static
name|int
name|verbose
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|expire
specifier|static
name|unsigned
name|long
name|expire
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|prune_worktree
specifier|static
name|int
name|prune_worktree
parameter_list|(
specifier|const
name|char
modifier|*
name|id
parameter_list|,
name|struct
name|strbuf
modifier|*
name|reason
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
name|char
modifier|*
name|path
decl_stmt|;
name|int
name|fd
decl_stmt|,
name|len
decl_stmt|;
if|if
condition|(
operator|!
name|is_directory
argument_list|(
name|git_path
argument_list|(
literal|"worktrees/%s"
argument_list|,
name|id
argument_list|)
argument_list|)
condition|)
block|{
name|strbuf_addf
argument_list|(
name|reason
argument_list|,
name|_
argument_list|(
literal|"Removing worktrees/%s: not a valid directory"
argument_list|)
argument_list|,
name|id
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
if|if
condition|(
name|file_exists
argument_list|(
name|git_path
argument_list|(
literal|"worktrees/%s/locked"
argument_list|,
name|id
argument_list|)
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|stat
argument_list|(
name|git_path
argument_list|(
literal|"worktrees/%s/gitdir"
argument_list|,
name|id
argument_list|)
argument_list|,
operator|&
name|st
argument_list|)
condition|)
block|{
name|strbuf_addf
argument_list|(
name|reason
argument_list|,
name|_
argument_list|(
literal|"Removing worktrees/%s: gitdir file does not exist"
argument_list|)
argument_list|,
name|id
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
name|fd
operator|=
name|open
argument_list|(
name|git_path
argument_list|(
literal|"worktrees/%s/gitdir"
argument_list|,
name|id
argument_list|)
argument_list|,
name|O_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
block|{
name|strbuf_addf
argument_list|(
name|reason
argument_list|,
name|_
argument_list|(
literal|"Removing worktrees/%s: unable to read gitdir file (%s)"
argument_list|)
argument_list|,
name|id
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
name|len
operator|=
name|st
operator|.
name|st_size
expr_stmt|;
name|path
operator|=
name|xmalloc
argument_list|(
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|read_in_full
argument_list|(
name|fd
argument_list|,
name|path
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|fd
argument_list|)
expr_stmt|;
while|while
condition|(
name|len
operator|&&
operator|(
name|path
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\n'
operator|||
name|path
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'\r'
operator|)
condition|)
name|len
operator|--
expr_stmt|;
if|if
condition|(
operator|!
name|len
condition|)
block|{
name|strbuf_addf
argument_list|(
name|reason
argument_list|,
name|_
argument_list|(
literal|"Removing worktrees/%s: invalid gitdir file"
argument_list|)
argument_list|,
name|id
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|path
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
name|path
index|[
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
operator|!
name|file_exists
argument_list|(
name|path
argument_list|)
condition|)
block|{
name|struct
name|stat
name|st_link
decl_stmt|;
name|free
argument_list|(
name|path
argument_list|)
expr_stmt|;
comment|/* 		 * the repo is moved manually and has not been 		 * accessed since? 		 */
if|if
condition|(
operator|!
name|stat
argument_list|(
name|git_path
argument_list|(
literal|"worktrees/%s/link"
argument_list|,
name|id
argument_list|)
argument_list|,
operator|&
name|st_link
argument_list|)
operator|&&
name|st_link
operator|.
name|st_nlink
operator|>
literal|1
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|st
operator|.
name|st_mtime
operator|<=
name|expire
condition|)
block|{
name|strbuf_addf
argument_list|(
name|reason
argument_list|,
name|_
argument_list|(
literal|"Removing worktrees/%s: gitdir file points to non-existent location"
argument_list|)
argument_list|,
name|id
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
else|else
block|{
return|return
literal|0
return|;
block|}
block|}
name|free
argument_list|(
name|path
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|prune_worktrees
specifier|static
name|void
name|prune_worktrees
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|strbuf
name|reason
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|path
init|=
name|STRBUF_INIT
decl_stmt|;
name|DIR
modifier|*
name|dir
init|=
name|opendir
argument_list|(
name|git_path
argument_list|(
literal|"worktrees"
argument_list|)
argument_list|)
decl_stmt|;
name|struct
name|dirent
modifier|*
name|d
decl_stmt|;
name|int
name|ret
decl_stmt|;
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
name|strbuf_reset
argument_list|(
operator|&
name|reason
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|prune_worktree
argument_list|(
name|d
operator|->
name|d_name
argument_list|,
operator|&
name|reason
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|show_only
operator|||
name|verbose
condition|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|reason
operator|.
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|show_only
condition|)
continue|continue;
name|strbuf_reset
argument_list|(
operator|&
name|path
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|path
argument_list|,
name|git_path
argument_list|(
literal|"worktrees/%s"
argument_list|,
name|d
operator|->
name|d_name
argument_list|)
argument_list|)
expr_stmt|;
name|ret
operator|=
name|remove_dir_recursively
argument_list|(
operator|&
name|path
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|<
literal|0
operator|&&
name|errno
operator|==
name|ENOTDIR
condition|)
name|ret
operator|=
name|unlink
argument_list|(
name|path
operator|.
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
name|error
argument_list|(
name|_
argument_list|(
literal|"failed to remove: %s"
argument_list|)
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|closedir
argument_list|(
name|dir
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|show_only
condition|)
name|rmdir
argument_list|(
name|git_path
argument_list|(
literal|"worktrees"
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|reason
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|prune
specifier|static
name|int
name|prune
parameter_list|(
name|int
name|ac
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|av
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT__DRY_RUN
argument_list|(
operator|&
name|show_only
argument_list|,
name|N_
argument_list|(
literal|"do not remove, show only"
argument_list|)
argument_list|)
block|,
name|OPT__VERBOSE
argument_list|(
operator|&
name|verbose
argument_list|,
name|N_
argument_list|(
literal|"report pruned objects"
argument_list|)
argument_list|)
block|,
name|OPT_EXPIRY_DATE
argument_list|(
literal|0
argument_list|,
literal|"expire"
argument_list|,
operator|&
name|expire
argument_list|,
name|N_
argument_list|(
literal|"expire objects older than<time>"
argument_list|)
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|expire
operator|=
name|ULONG_MAX
expr_stmt|;
name|ac
operator|=
name|parse_options
argument_list|(
name|ac
argument_list|,
name|av
argument_list|,
name|prefix
argument_list|,
name|options
argument_list|,
name|worktree_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|ac
condition|)
name|usage_with_options
argument_list|(
name|worktree_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
name|prune_worktrees
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|add
specifier|static
name|int
name|add
parameter_list|(
name|int
name|ac
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|av
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|struct
name|child_process
name|c
decl_stmt|;
name|int
name|force
init|=
literal|0
decl_stmt|,
name|detach
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
decl_stmt|,
modifier|*
name|branch
decl_stmt|;
name|struct
name|argv_array
name|cmd
init|=
name|ARGV_ARRAY_INIT
decl_stmt|;
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT__FORCE
argument_list|(
operator|&
name|force
argument_list|,
name|N_
argument_list|(
literal|"checkout<branch> even if already checked out in other worktree"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|0
argument_list|,
literal|"detach"
argument_list|,
operator|&
name|detach
argument_list|,
name|N_
argument_list|(
literal|"detach HEAD at named commit"
argument_list|)
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
name|ac
operator|=
name|parse_options
argument_list|(
name|ac
argument_list|,
name|av
argument_list|,
name|prefix
argument_list|,
name|options
argument_list|,
name|worktree_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|ac
operator|!=
literal|2
condition|)
name|usage_with_options
argument_list|(
name|worktree_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
name|path
operator|=
name|prefix
condition|?
name|prefix_filename
argument_list|(
name|prefix
argument_list|,
name|strlen
argument_list|(
name|prefix
argument_list|)
argument_list|,
name|av
index|[
literal|0
index|]
argument_list|)
else|:
name|av
index|[
literal|0
index|]
expr_stmt|;
name|branch
operator|=
name|av
index|[
literal|1
index|]
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|cmd
argument_list|,
literal|"checkout"
argument_list|)
expr_stmt|;
name|argv_array_pushl
argument_list|(
operator|&
name|cmd
argument_list|,
literal|"--to"
argument_list|,
name|path
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|force
condition|)
name|argv_array_push
argument_list|(
operator|&
name|cmd
argument_list|,
literal|"--ignore-other-worktrees"
argument_list|)
expr_stmt|;
if|if
condition|(
name|detach
condition|)
name|argv_array_push
argument_list|(
operator|&
name|cmd
argument_list|,
literal|"--detach"
argument_list|)
expr_stmt|;
name|argv_array_push
argument_list|(
operator|&
name|cmd
argument_list|,
name|branch
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|c
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|c
argument_list|)
argument_list|)
expr_stmt|;
name|c
operator|.
name|git_cmd
operator|=
literal|1
expr_stmt|;
name|c
operator|.
name|argv
operator|=
name|cmd
operator|.
name|argv
expr_stmt|;
return|return
name|run_command
argument_list|(
operator|&
name|c
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|cmd_worktree
name|int
name|cmd_worktree
parameter_list|(
name|int
name|ac
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|av
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|struct
name|option
name|options
index|[]
init|=
block|{
name|OPT_END
argument_list|()
block|}
decl_stmt|;
if|if
condition|(
name|ac
operator|<
literal|2
condition|)
name|usage_with_options
argument_list|(
name|worktree_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|av
index|[
literal|1
index|]
argument_list|,
literal|"add"
argument_list|)
condition|)
return|return
name|add
argument_list|(
name|ac
operator|-
literal|1
argument_list|,
name|av
operator|+
literal|1
argument_list|,
name|prefix
argument_list|)
return|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|av
index|[
literal|1
index|]
argument_list|,
literal|"prune"
argument_list|)
condition|)
return|return
name|prune
argument_list|(
name|ac
operator|-
literal|1
argument_list|,
name|av
operator|+
literal|1
argument_list|,
name|prefix
argument_list|)
return|;
name|usage_with_options
argument_list|(
name|worktree_usage
argument_list|,
name|options
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

