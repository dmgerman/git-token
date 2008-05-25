begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git clone"  *  * Copyright (c) 2007 Kristian HÃ¸gsberg<krh@redhat.com>,  *		 2008 Daniel Barkalow<barkalow@iabervon.org>  * Based on git-commit.sh by Junio C Hamano and Linus Torvalds  *  * Clone a repository into a different directory that does not yet exist.  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_include
include|#
directive|include
file|"fetch-pack.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_include
include|#
directive|include
file|"tree-walk.h"
end_include

begin_include
include|#
directive|include
file|"unpack-trees.h"
end_include

begin_include
include|#
directive|include
file|"transport.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_comment
comment|/*  * Overall FIXMEs:  *  - respect DB_ENVIRONMENT for .git/objects.  *  * Implementation notes:  *  - dropping use-separate-remote and no-separate-remote compatibility  *  */
end_comment

begin_decl_stmt
DECL|variable|builtin_clone_usage
specifier|static
specifier|const
name|char
modifier|*
specifier|const
name|builtin_clone_usage
index|[]
init|=
block|{
literal|"git-clone [options] [--]<repo> [<dir>]"
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|option_quiet
DECL|variable|option_no_checkout
DECL|variable|option_bare
specifier|static
name|int
name|option_quiet
decl_stmt|,
name|option_no_checkout
decl_stmt|,
name|option_bare
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|option_local
DECL|variable|option_no_hardlinks
DECL|variable|option_shared
specifier|static
name|int
name|option_local
decl_stmt|,
name|option_no_hardlinks
decl_stmt|,
name|option_shared
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|option_template
DECL|variable|option_reference
DECL|variable|option_depth
specifier|static
name|char
modifier|*
name|option_template
decl_stmt|,
modifier|*
name|option_reference
decl_stmt|,
modifier|*
name|option_depth
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|option_origin
specifier|static
name|char
modifier|*
name|option_origin
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|option_upload_pack
specifier|static
name|char
modifier|*
name|option_upload_pack
init|=
literal|"git-upload-pack"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|builtin_clone_options
specifier|static
name|struct
name|option
name|builtin_clone_options
index|[]
init|=
block|{
name|OPT__QUIET
argument_list|(
operator|&
name|option_quiet
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'n'
argument_list|,
literal|"no-checkout"
argument_list|,
operator|&
name|option_no_checkout
argument_list|,
literal|"don't create a checkout"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"bare"
argument_list|,
operator|&
name|option_bare
argument_list|,
literal|"create a bare repository"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"naked"
argument_list|,
operator|&
name|option_bare
argument_list|,
literal|"create a bare repository"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'l'
argument_list|,
literal|"local"
argument_list|,
operator|&
name|option_local
argument_list|,
literal|"to clone from a local repository"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|0
argument_list|,
literal|"no-hardlinks"
argument_list|,
operator|&
name|option_no_hardlinks
argument_list|,
literal|"don't use local hardlinks, always copy"
argument_list|)
block|,
name|OPT_BOOLEAN
argument_list|(
literal|'s'
argument_list|,
literal|"shared"
argument_list|,
operator|&
name|option_shared
argument_list|,
literal|"setup as shared repository"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"template"
argument_list|,
operator|&
name|option_template
argument_list|,
literal|"path"
argument_list|,
literal|"path the template repository"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"reference"
argument_list|,
operator|&
name|option_reference
argument_list|,
literal|"repo"
argument_list|,
literal|"reference repository"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|'o'
argument_list|,
literal|"origin"
argument_list|,
operator|&
name|option_origin
argument_list|,
literal|"branch"
argument_list|,
literal|"use<branch> instead or 'origin' to track upstream"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|'u'
argument_list|,
literal|"upload-pack"
argument_list|,
operator|&
name|option_upload_pack
argument_list|,
literal|"path"
argument_list|,
literal|"path to git-upload-pack on the remote"
argument_list|)
block|,
name|OPT_STRING
argument_list|(
literal|0
argument_list|,
literal|"depth"
argument_list|,
operator|&
name|option_depth
argument_list|,
literal|"depth"
argument_list|,
literal|"create a shallow clone of that depth"
argument_list|)
block|,
name|OPT_END
argument_list|()
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|get_repo_path
specifier|static
name|char
modifier|*
name|get_repo_path
parameter_list|(
specifier|const
name|char
modifier|*
name|repo
parameter_list|,
name|int
modifier|*
name|is_bundle
parameter_list|)
block|{
specifier|static
name|char
modifier|*
name|suffix
index|[]
init|=
block|{
literal|"/.git"
block|,
literal|".git"
block|,
literal|""
block|}
decl_stmt|;
specifier|static
name|char
modifier|*
name|bundle_suffix
index|[]
init|=
block|{
literal|".bundle"
block|,
literal|""
block|}
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
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
name|ARRAY_SIZE
argument_list|(
name|suffix
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
name|path
operator|=
name|mkpath
argument_list|(
literal|"%s%s"
argument_list|,
name|repo
argument_list|,
name|suffix
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|stat
argument_list|(
name|path
argument_list|,
operator|&
name|st
argument_list|)
operator|&&
name|S_ISDIR
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
operator|*
name|is_bundle
operator|=
literal|0
expr_stmt|;
return|return
name|xstrdup
argument_list|(
name|make_absolute_path
argument_list|(
name|path
argument_list|)
argument_list|)
return|;
block|}
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ARRAY_SIZE
argument_list|(
name|bundle_suffix
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
name|path
operator|=
name|mkpath
argument_list|(
literal|"%s%s"
argument_list|,
name|repo
argument_list|,
name|bundle_suffix
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|stat
argument_list|(
name|path
argument_list|,
operator|&
name|st
argument_list|)
operator|&&
name|S_ISREG
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
block|{
operator|*
name|is_bundle
operator|=
literal|1
expr_stmt|;
return|return
name|xstrdup
argument_list|(
name|make_absolute_path
argument_list|(
name|path
argument_list|)
argument_list|)
return|;
block|}
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|guess_dir_name
specifier|static
name|char
modifier|*
name|guess_dir_name
parameter_list|(
specifier|const
name|char
modifier|*
name|repo
parameter_list|,
name|int
name|is_bundle
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|p
decl_stmt|,
modifier|*
name|start
decl_stmt|,
modifier|*
name|end
decl_stmt|,
modifier|*
name|limit
decl_stmt|;
name|int
name|after_slash_or_colon
decl_stmt|;
comment|/* Guess dir name from repository: strip trailing '/', 	 * strip trailing '[:/]*.{git,bundle}', strip leading '.*[/:]'. */
name|after_slash_or_colon
operator|=
literal|1
expr_stmt|;
name|limit
operator|=
name|repo
operator|+
name|strlen
argument_list|(
name|repo
argument_list|)
expr_stmt|;
name|start
operator|=
name|repo
expr_stmt|;
name|end
operator|=
name|limit
expr_stmt|;
for|for
control|(
name|p
operator|=
name|repo
init|;
name|p
operator|<
name|limit
condition|;
name|p
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|prefix
init|=
name|is_bundle
condition|?
literal|".bundle"
else|:
literal|".git"
decl_stmt|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|p
argument_list|,
name|prefix
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|after_slash_or_colon
condition|)
name|end
operator|=
name|p
expr_stmt|;
name|p
operator|+=
name|strlen
argument_list|(
name|prefix
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|p
argument_list|,
literal|".bundle"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|after_slash_or_colon
condition|)
name|end
operator|=
name|p
expr_stmt|;
name|p
operator|+=
literal|7
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|*
name|p
operator|==
literal|'/'
operator|||
operator|*
name|p
operator|==
literal|':'
condition|)
block|{
if|if
condition|(
name|end
operator|==
name|limit
condition|)
name|end
operator|=
name|p
expr_stmt|;
name|after_slash_or_colon
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|after_slash_or_colon
condition|)
block|{
name|start
operator|=
name|p
expr_stmt|;
name|end
operator|=
name|limit
expr_stmt|;
name|after_slash_or_colon
operator|=
literal|0
expr_stmt|;
block|}
block|}
return|return
name|xstrndup
argument_list|(
name|start
argument_list|,
name|end
operator|-
name|start
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|is_directory
specifier|static
name|int
name|is_directory
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|struct
name|stat
name|buf
decl_stmt|;
return|return
operator|!
name|stat
argument_list|(
name|path
argument_list|,
operator|&
name|buf
argument_list|)
operator|&&
name|S_ISDIR
argument_list|(
name|buf
operator|.
name|st_mode
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|setup_reference
specifier|static
name|void
name|setup_reference
parameter_list|(
specifier|const
name|char
modifier|*
name|repo
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|ref_git
decl_stmt|;
name|char
modifier|*
name|ref_git_copy
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
name|extra
decl_stmt|;
name|ref_git
operator|=
name|make_absolute_path
argument_list|(
name|option_reference
argument_list|)
expr_stmt|;
if|if
condition|(
name|is_directory
argument_list|(
name|mkpath
argument_list|(
literal|"%s/.git/objects"
argument_list|,
name|ref_git
argument_list|)
argument_list|)
condition|)
name|ref_git
operator|=
name|mkpath
argument_list|(
literal|"%s/.git"
argument_list|,
name|ref_git
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|is_directory
argument_list|(
name|mkpath
argument_list|(
literal|"%s/objects"
argument_list|,
name|ref_git
argument_list|)
argument_list|)
condition|)
name|die
argument_list|(
literal|"reference repository '%s' is not a local directory."
argument_list|,
name|option_reference
argument_list|)
expr_stmt|;
name|ref_git_copy
operator|=
name|xstrdup
argument_list|(
name|ref_git
argument_list|)
expr_stmt|;
name|add_to_alternates_file
argument_list|(
name|ref_git_copy
argument_list|)
expr_stmt|;
name|remote
operator|=
name|remote_get
argument_list|(
name|ref_git_copy
argument_list|)
expr_stmt|;
name|transport
operator|=
name|transport_get
argument_list|(
name|remote
argument_list|,
name|ref_git_copy
argument_list|)
expr_stmt|;
for|for
control|(
name|extra
operator|=
name|transport_get_remote_refs
argument_list|(
name|transport
argument_list|)
init|;
name|extra
condition|;
name|extra
operator|=
name|extra
operator|->
name|next
control|)
name|add_extra_ref
argument_list|(
name|extra
operator|->
name|name
argument_list|,
name|extra
operator|->
name|old_sha1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|transport_disconnect
argument_list|(
name|transport
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|ref_git_copy
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|copy_or_link_directory
specifier|static
name|void
name|copy_or_link_directory
parameter_list|(
name|char
modifier|*
name|src
parameter_list|,
name|char
modifier|*
name|dest
parameter_list|)
block|{
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
name|struct
name|stat
name|buf
decl_stmt|;
name|int
name|src_len
decl_stmt|,
name|dest_len
decl_stmt|;
name|DIR
modifier|*
name|dir
decl_stmt|;
name|dir
operator|=
name|opendir
argument_list|(
name|src
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dir
condition|)
name|die
argument_list|(
literal|"failed to open %s\n"
argument_list|,
name|src
argument_list|)
expr_stmt|;
if|if
condition|(
name|mkdir
argument_list|(
name|dest
argument_list|,
literal|0777
argument_list|)
condition|)
block|{
if|if
condition|(
name|errno
operator|!=
name|EEXIST
condition|)
name|die
argument_list|(
literal|"failed to create directory %s\n"
argument_list|,
name|dest
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|stat
argument_list|(
name|dest
argument_list|,
operator|&
name|buf
argument_list|)
condition|)
name|die
argument_list|(
literal|"failed to stat %s\n"
argument_list|,
name|dest
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|S_ISDIR
argument_list|(
name|buf
operator|.
name|st_mode
argument_list|)
condition|)
name|die
argument_list|(
literal|"%s exists and is not a directory\n"
argument_list|,
name|dest
argument_list|)
expr_stmt|;
block|}
name|src_len
operator|=
name|strlen
argument_list|(
name|src
argument_list|)
expr_stmt|;
name|src
index|[
name|src_len
index|]
operator|=
literal|'/'
expr_stmt|;
name|dest_len
operator|=
name|strlen
argument_list|(
name|dest
argument_list|)
expr_stmt|;
name|dest
index|[
name|dest_len
index|]
operator|=
literal|'/'
expr_stmt|;
while|while
condition|(
operator|(
name|de
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
name|strcpy
argument_list|(
name|src
operator|+
name|src_len
operator|+
literal|1
argument_list|,
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|dest
operator|+
name|dest_len
operator|+
literal|1
argument_list|,
name|de
operator|->
name|d_name
argument_list|)
expr_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|src
argument_list|,
operator|&
name|buf
argument_list|)
condition|)
block|{
name|warning
argument_list|(
literal|"failed to stat %s\n"
argument_list|,
name|src
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|S_ISDIR
argument_list|(
name|buf
operator|.
name|st_mode
argument_list|)
condition|)
block|{
if|if
condition|(
name|de
operator|->
name|d_name
index|[
literal|0
index|]
operator|!=
literal|'.'
condition|)
name|copy_or_link_directory
argument_list|(
name|src
argument_list|,
name|dest
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|unlink
argument_list|(
name|dest
argument_list|)
operator|&&
name|errno
operator|!=
name|ENOENT
condition|)
name|die
argument_list|(
literal|"failed to unlink %s\n"
argument_list|,
name|dest
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|option_no_hardlinks
condition|)
block|{
if|if
condition|(
operator|!
name|link
argument_list|(
name|src
argument_list|,
name|dest
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|option_local
condition|)
name|die
argument_list|(
literal|"failed to create link %s\n"
argument_list|,
name|dest
argument_list|)
expr_stmt|;
name|option_no_hardlinks
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|copy_file
argument_list|(
name|dest
argument_list|,
name|src
argument_list|,
literal|0666
argument_list|)
condition|)
name|die
argument_list|(
literal|"failed to copy file to %s\n"
argument_list|,
name|dest
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

begin_function
DECL|function|clone_local
specifier|static
specifier|const
name|struct
name|ref
modifier|*
name|clone_local
parameter_list|(
specifier|const
name|char
modifier|*
name|src_repo
parameter_list|,
specifier|const
name|char
modifier|*
name|dest_repo
parameter_list|)
block|{
specifier|const
name|struct
name|ref
modifier|*
name|ret
decl_stmt|;
name|char
name|src
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|char
name|dest
index|[
name|PATH_MAX
index|]
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
if|if
condition|(
name|option_shared
condition|)
name|add_to_alternates_file
argument_list|(
name|src_repo
argument_list|)
expr_stmt|;
else|else
block|{
name|snprintf
argument_list|(
name|src
argument_list|,
name|PATH_MAX
argument_list|,
literal|"%s/objects"
argument_list|,
name|src_repo
argument_list|)
expr_stmt|;
name|snprintf
argument_list|(
name|dest
argument_list|,
name|PATH_MAX
argument_list|,
literal|"%s/objects"
argument_list|,
name|dest_repo
argument_list|)
expr_stmt|;
name|copy_or_link_directory
argument_list|(
name|src
argument_list|,
name|dest
argument_list|)
expr_stmt|;
block|}
name|remote
operator|=
name|remote_get
argument_list|(
name|src_repo
argument_list|)
expr_stmt|;
name|transport
operator|=
name|transport_get
argument_list|(
name|remote
argument_list|,
name|src_repo
argument_list|)
expr_stmt|;
name|ret
operator|=
name|transport_get_remote_refs
argument_list|(
name|transport
argument_list|)
expr_stmt|;
name|transport_disconnect
argument_list|(
name|transport
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|junk_work_tree
specifier|static
specifier|const
name|char
modifier|*
name|junk_work_tree
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|junk_git_dir
specifier|static
specifier|const
name|char
modifier|*
name|junk_git_dir
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|junk_pid
name|pid_t
name|junk_pid
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|remove_junk
specifier|static
name|void
name|remove_junk
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|strbuf
name|sb
decl_stmt|;
if|if
condition|(
name|getpid
argument_list|()
operator|!=
name|junk_pid
condition|)
return|return;
name|strbuf_init
argument_list|(
operator|&
name|sb
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|junk_git_dir
condition|)
block|{
name|strbuf_addstr
argument_list|(
operator|&
name|sb
argument_list|,
name|junk_git_dir
argument_list|)
expr_stmt|;
name|remove_dir_recursively
argument_list|(
operator|&
name|sb
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|junk_work_tree
condition|)
block|{
name|strbuf_addstr
argument_list|(
operator|&
name|sb
argument_list|,
name|junk_work_tree
argument_list|)
expr_stmt|;
name|remove_dir_recursively
argument_list|(
operator|&
name|sb
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|remove_junk_on_signal
specifier|static
name|void
name|remove_junk_on_signal
parameter_list|(
name|int
name|signo
parameter_list|)
block|{
name|remove_junk
argument_list|()
expr_stmt|;
name|signal
argument_list|(
name|SIGINT
argument_list|,
name|SIG_DFL
argument_list|)
expr_stmt|;
name|raise
argument_list|(
name|signo
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|locate_head
specifier|static
specifier|const
name|struct
name|ref
modifier|*
name|locate_head
parameter_list|(
specifier|const
name|struct
name|ref
modifier|*
name|refs
parameter_list|,
specifier|const
name|struct
name|ref
modifier|*
name|mapped_refs
parameter_list|,
specifier|const
name|struct
name|ref
modifier|*
modifier|*
name|remote_head_p
parameter_list|)
block|{
specifier|const
name|struct
name|ref
modifier|*
name|remote_head
init|=
name|NULL
decl_stmt|;
specifier|const
name|struct
name|ref
modifier|*
name|remote_master
init|=
name|NULL
decl_stmt|;
specifier|const
name|struct
name|ref
modifier|*
name|r
decl_stmt|;
for|for
control|(
name|r
operator|=
name|refs
init|;
name|r
condition|;
name|r
operator|=
name|r
operator|->
name|next
control|)
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|r
operator|->
name|name
argument_list|,
literal|"HEAD"
argument_list|)
condition|)
name|remote_head
operator|=
name|r
expr_stmt|;
for|for
control|(
name|r
operator|=
name|mapped_refs
init|;
name|r
condition|;
name|r
operator|=
name|r
operator|->
name|next
control|)
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|r
operator|->
name|name
argument_list|,
literal|"refs/heads/master"
argument_list|)
condition|)
name|remote_master
operator|=
name|r
expr_stmt|;
if|if
condition|(
name|remote_head_p
condition|)
operator|*
name|remote_head_p
operator|=
name|remote_head
expr_stmt|;
comment|/* If there's no HEAD value at all, never mind. */
if|if
condition|(
operator|!
name|remote_head
condition|)
return|return
name|NULL
return|;
comment|/* If refs/heads/master could be right, it is. */
if|if
condition|(
name|remote_master
operator|&&
operator|!
name|hashcmp
argument_list|(
name|remote_master
operator|->
name|old_sha1
argument_list|,
name|remote_head
operator|->
name|old_sha1
argument_list|)
condition|)
return|return
name|remote_master
return|;
comment|/* Look for another ref that points there */
for|for
control|(
name|r
operator|=
name|mapped_refs
init|;
name|r
condition|;
name|r
operator|=
name|r
operator|->
name|next
control|)
if|if
condition|(
name|r
operator|!=
name|remote_head
operator|&&
operator|!
name|hashcmp
argument_list|(
name|r
operator|->
name|old_sha1
argument_list|,
name|remote_head
operator|->
name|old_sha1
argument_list|)
condition|)
return|return
name|r
return|;
comment|/* Nothing is the same */
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|write_remote_refs
specifier|static
name|struct
name|ref
modifier|*
name|write_remote_refs
parameter_list|(
specifier|const
name|struct
name|ref
modifier|*
name|refs
parameter_list|,
name|struct
name|refspec
modifier|*
name|refspec
parameter_list|,
specifier|const
name|char
modifier|*
name|reflog
parameter_list|)
block|{
name|struct
name|ref
modifier|*
name|local_refs
init|=
name|NULL
decl_stmt|;
name|struct
name|ref
modifier|*
modifier|*
name|tail
init|=
operator|&
name|local_refs
decl_stmt|;
name|struct
name|ref
modifier|*
name|r
decl_stmt|;
name|get_fetch_map
argument_list|(
name|refs
argument_list|,
name|refspec
argument_list|,
operator|&
name|tail
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|get_fetch_map
argument_list|(
name|refs
argument_list|,
name|tag_refspec
argument_list|,
operator|&
name|tail
argument_list|,
literal|0
argument_list|)
expr_stmt|;
for|for
control|(
name|r
operator|=
name|local_refs
init|;
name|r
condition|;
name|r
operator|=
name|r
operator|->
name|next
control|)
name|update_ref
argument_list|(
name|reflog
argument_list|,
name|r
operator|->
name|peer_ref
operator|->
name|name
argument_list|,
name|r
operator|->
name|old_sha1
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
name|DIE_ON_ERR
argument_list|)
expr_stmt|;
return|return
name|local_refs
return|;
block|}
end_function

begin_function
DECL|function|cmd_clone
name|int
name|cmd_clone
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
name|use_local_hardlinks
init|=
literal|1
decl_stmt|;
name|int
name|use_separate_remote
init|=
literal|1
decl_stmt|;
name|int
name|is_bundle
init|=
literal|0
decl_stmt|;
name|struct
name|stat
name|buf
decl_stmt|;
specifier|const
name|char
modifier|*
name|repo_name
decl_stmt|,
modifier|*
name|repo
decl_stmt|,
modifier|*
name|work_tree
decl_stmt|,
modifier|*
name|git_dir
decl_stmt|;
name|char
modifier|*
name|path
decl_stmt|,
modifier|*
name|dir
decl_stmt|;
specifier|const
name|struct
name|ref
modifier|*
name|refs
decl_stmt|,
modifier|*
name|head_points_at
decl_stmt|,
modifier|*
name|remote_head
decl_stmt|,
modifier|*
name|mapped_refs
decl_stmt|;
name|char
name|branch_top
index|[
literal|256
index|]
decl_stmt|,
name|key
index|[
literal|256
index|]
decl_stmt|,
name|value
index|[
literal|256
index|]
decl_stmt|;
name|struct
name|strbuf
name|reflog_msg
decl_stmt|;
name|struct
name|refspec
name|refspec
decl_stmt|;
name|junk_pid
operator|=
name|getpid
argument_list|()
expr_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|builtin_clone_options
argument_list|,
name|builtin_clone_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|==
literal|0
condition|)
name|die
argument_list|(
literal|"You must specify a repository to clone."
argument_list|)
expr_stmt|;
if|if
condition|(
name|option_no_hardlinks
condition|)
name|use_local_hardlinks
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|option_bare
condition|)
block|{
if|if
condition|(
name|option_origin
condition|)
name|die
argument_list|(
literal|"--bare and --origin %s options are incompatible."
argument_list|,
name|option_origin
argument_list|)
expr_stmt|;
name|option_no_checkout
operator|=
literal|1
expr_stmt|;
name|use_separate_remote
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|option_origin
condition|)
name|option_origin
operator|=
literal|"origin"
expr_stmt|;
name|repo_name
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
name|path
operator|=
name|get_repo_path
argument_list|(
name|repo_name
argument_list|,
operator|&
name|is_bundle
argument_list|)
expr_stmt|;
if|if
condition|(
name|path
condition|)
name|repo
operator|=
name|path
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strchr
argument_list|(
name|repo_name
argument_list|,
literal|':'
argument_list|)
condition|)
name|repo
operator|=
name|xstrdup
argument_list|(
name|make_absolute_path
argument_list|(
name|repo_name
argument_list|)
argument_list|)
expr_stmt|;
else|else
name|repo
operator|=
name|repo_name
expr_stmt|;
if|if
condition|(
name|argc
operator|==
literal|2
condition|)
name|dir
operator|=
name|xstrdup
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
else|else
name|dir
operator|=
name|guess_dir_name
argument_list|(
name|repo_name
argument_list|,
name|is_bundle
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|stat
argument_list|(
name|dir
argument_list|,
operator|&
name|buf
argument_list|)
condition|)
name|die
argument_list|(
literal|"destination directory '%s' already exists."
argument_list|,
name|dir
argument_list|)
expr_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|reflog_msg
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|reflog_msg
argument_list|,
literal|"clone: from %s"
argument_list|,
name|repo
argument_list|)
expr_stmt|;
if|if
condition|(
name|option_bare
condition|)
name|work_tree
operator|=
name|NULL
expr_stmt|;
else|else
block|{
name|work_tree
operator|=
name|getenv
argument_list|(
literal|"GIT_WORK_TREE"
argument_list|)
expr_stmt|;
if|if
condition|(
name|work_tree
operator|&&
operator|!
name|stat
argument_list|(
name|work_tree
argument_list|,
operator|&
name|buf
argument_list|)
condition|)
name|die
argument_list|(
literal|"working tree '%s' already exists."
argument_list|,
name|work_tree
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|option_bare
operator|||
name|work_tree
condition|)
name|git_dir
operator|=
name|xstrdup
argument_list|(
name|dir
argument_list|)
expr_stmt|;
else|else
block|{
name|work_tree
operator|=
name|dir
expr_stmt|;
name|git_dir
operator|=
name|xstrdup
argument_list|(
name|mkpath
argument_list|(
literal|"%s/.git"
argument_list|,
name|dir
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|option_bare
condition|)
block|{
name|junk_work_tree
operator|=
name|work_tree
expr_stmt|;
if|if
condition|(
name|mkdir
argument_list|(
name|work_tree
argument_list|,
literal|0755
argument_list|)
condition|)
name|die
argument_list|(
literal|"could not create work tree dir '%s'."
argument_list|,
name|work_tree
argument_list|)
expr_stmt|;
name|set_git_work_tree
argument_list|(
name|work_tree
argument_list|)
expr_stmt|;
block|}
name|junk_git_dir
operator|=
name|git_dir
expr_stmt|;
name|atexit
argument_list|(
name|remove_junk
argument_list|)
expr_stmt|;
name|signal
argument_list|(
name|SIGINT
argument_list|,
name|remove_junk_on_signal
argument_list|)
expr_stmt|;
name|setenv
argument_list|(
name|CONFIG_ENVIRONMENT
argument_list|,
name|xstrdup
argument_list|(
name|mkpath
argument_list|(
literal|"%s/config"
argument_list|,
name|git_dir
argument_list|)
argument_list|)
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|set_git_dir
argument_list|(
name|make_absolute_path
argument_list|(
name|git_dir
argument_list|)
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Initialize %s\n"
argument_list|,
name|git_dir
argument_list|)
expr_stmt|;
name|init_db
argument_list|(
name|option_template
argument_list|,
name|option_quiet
condition|?
name|INIT_DB_QUIET
else|:
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|option_reference
condition|)
name|setup_reference
argument_list|(
name|git_dir
argument_list|)
expr_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
if|if
condition|(
name|option_bare
condition|)
block|{
name|strcpy
argument_list|(
name|branch_top
argument_list|,
literal|"refs/heads/"
argument_list|)
expr_stmt|;
name|git_config_set
argument_list|(
literal|"core.bare"
argument_list|,
literal|"true"
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|snprintf
argument_list|(
name|branch_top
argument_list|,
sizeof|sizeof
argument_list|(
name|branch_top
argument_list|)
argument_list|,
literal|"refs/remotes/%s/"
argument_list|,
name|option_origin
argument_list|)
expr_stmt|;
comment|/* Configure the remote */
name|snprintf
argument_list|(
name|key
argument_list|,
sizeof|sizeof
argument_list|(
name|key
argument_list|)
argument_list|,
literal|"remote.%s.url"
argument_list|,
name|option_origin
argument_list|)
expr_stmt|;
name|git_config_set
argument_list|(
name|key
argument_list|,
name|repo
argument_list|)
expr_stmt|;
name|snprintf
argument_list|(
name|key
argument_list|,
sizeof|sizeof
argument_list|(
name|key
argument_list|)
argument_list|,
literal|"remote.%s.fetch"
argument_list|,
name|option_origin
argument_list|)
expr_stmt|;
name|snprintf
argument_list|(
name|value
argument_list|,
sizeof|sizeof
argument_list|(
name|value
argument_list|)
argument_list|,
literal|"+refs/heads/*:%s*"
argument_list|,
name|branch_top
argument_list|)
expr_stmt|;
name|git_config_set_multivar
argument_list|(
name|key
argument_list|,
name|value
argument_list|,
literal|"^$"
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
name|refspec
operator|.
name|force
operator|=
literal|0
expr_stmt|;
name|refspec
operator|.
name|pattern
operator|=
literal|1
expr_stmt|;
name|refspec
operator|.
name|src
operator|=
literal|"refs/heads/"
expr_stmt|;
name|refspec
operator|.
name|dst
operator|=
name|branch_top
expr_stmt|;
if|if
condition|(
name|path
operator|&&
operator|!
name|is_bundle
condition|)
name|refs
operator|=
name|clone_local
argument_list|(
name|path
argument_list|,
name|git_dir
argument_list|)
expr_stmt|;
else|else
block|{
name|struct
name|remote
modifier|*
name|remote
init|=
name|remote_get
argument_list|(
name|argv
index|[
literal|0
index|]
argument_list|)
decl_stmt|;
name|struct
name|transport
modifier|*
name|transport
init|=
name|transport_get
argument_list|(
name|remote
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
decl_stmt|;
name|transport_set_option
argument_list|(
name|transport
argument_list|,
name|TRANS_OPT_KEEP
argument_list|,
literal|"yes"
argument_list|)
expr_stmt|;
if|if
condition|(
name|option_depth
condition|)
name|transport_set_option
argument_list|(
name|transport
argument_list|,
name|TRANS_OPT_DEPTH
argument_list|,
name|option_depth
argument_list|)
expr_stmt|;
if|if
condition|(
name|option_quiet
condition|)
name|transport
operator|->
name|verbose
operator|=
operator|-
literal|1
expr_stmt|;
name|refs
operator|=
name|transport_get_remote_refs
argument_list|(
name|transport
argument_list|)
expr_stmt|;
name|transport_fetch_refs
argument_list|(
name|transport
argument_list|,
name|refs
argument_list|)
expr_stmt|;
block|}
name|clear_extra_refs
argument_list|()
expr_stmt|;
name|mapped_refs
operator|=
name|write_remote_refs
argument_list|(
name|refs
argument_list|,
operator|&
name|refspec
argument_list|,
name|reflog_msg
operator|.
name|buf
argument_list|)
expr_stmt|;
name|head_points_at
operator|=
name|locate_head
argument_list|(
name|refs
argument_list|,
name|mapped_refs
argument_list|,
operator|&
name|remote_head
argument_list|)
expr_stmt|;
if|if
condition|(
name|head_points_at
condition|)
block|{
comment|/* Local default branch link */
name|create_symref
argument_list|(
literal|"HEAD"
argument_list|,
name|head_points_at
operator|->
name|name
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|option_bare
condition|)
block|{
name|struct
name|strbuf
name|head_ref
decl_stmt|;
specifier|const
name|char
modifier|*
name|head
init|=
name|head_points_at
operator|->
name|name
decl_stmt|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|head
argument_list|,
literal|"refs/heads/"
argument_list|)
condition|)
name|head
operator|+=
literal|11
expr_stmt|;
comment|/* Set up the initial local branch */
comment|/* Local branch initial value */
name|update_ref
argument_list|(
name|reflog_msg
operator|.
name|buf
argument_list|,
literal|"HEAD"
argument_list|,
name|head_points_at
operator|->
name|old_sha1
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
name|DIE_ON_ERR
argument_list|)
expr_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|head_ref
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|head_ref
argument_list|,
name|branch_top
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|head_ref
argument_list|,
literal|"HEAD"
argument_list|)
expr_stmt|;
comment|/* Remote branch link */
name|create_symref
argument_list|(
name|head_ref
operator|.
name|buf
argument_list|,
name|head_points_at
operator|->
name|peer_ref
operator|->
name|name
argument_list|,
name|reflog_msg
operator|.
name|buf
argument_list|)
expr_stmt|;
name|snprintf
argument_list|(
name|key
argument_list|,
sizeof|sizeof
argument_list|(
name|key
argument_list|)
argument_list|,
literal|"branch.%s.remote"
argument_list|,
name|head
argument_list|)
expr_stmt|;
name|git_config_set
argument_list|(
name|key
argument_list|,
name|option_origin
argument_list|)
expr_stmt|;
name|snprintf
argument_list|(
name|key
argument_list|,
sizeof|sizeof
argument_list|(
name|key
argument_list|)
argument_list|,
literal|"branch.%s.merge"
argument_list|,
name|head
argument_list|)
expr_stmt|;
name|git_config_set
argument_list|(
name|key
argument_list|,
name|head_points_at
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
block|}
elseif|else
if|if
condition|(
name|remote_head
condition|)
block|{
comment|/* Source had detached HEAD pointing somewhere. */
if|if
condition|(
operator|!
name|option_bare
condition|)
name|update_ref
argument_list|(
name|reflog_msg
operator|.
name|buf
argument_list|,
literal|"HEAD"
argument_list|,
name|remote_head
operator|->
name|old_sha1
argument_list|,
name|NULL
argument_list|,
name|REF_NODEREF
argument_list|,
name|DIE_ON_ERR
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/* Nothing to checkout out */
if|if
condition|(
operator|!
name|option_no_checkout
condition|)
name|warning
argument_list|(
literal|"remote HEAD refers to nonexistent ref, "
literal|"unable to checkout.\n"
argument_list|)
expr_stmt|;
name|option_no_checkout
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|option_no_checkout
condition|)
block|{
name|struct
name|lock_file
modifier|*
name|lock_file
init|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|lock_file
argument_list|)
argument_list|)
decl_stmt|;
name|struct
name|unpack_trees_options
name|opts
decl_stmt|;
name|struct
name|tree
modifier|*
name|tree
decl_stmt|;
name|struct
name|tree_desc
name|t
decl_stmt|;
name|int
name|fd
decl_stmt|;
comment|/* We need to be in the new work tree for the checkout */
name|setup_work_tree
argument_list|()
expr_stmt|;
name|fd
operator|=
name|hold_locked_index
argument_list|(
name|lock_file
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|opts
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
name|opts
argument_list|)
expr_stmt|;
name|opts
operator|.
name|update
operator|=
literal|1
expr_stmt|;
name|opts
operator|.
name|merge
operator|=
literal|1
expr_stmt|;
name|opts
operator|.
name|fn
operator|=
name|oneway_merge
expr_stmt|;
name|opts
operator|.
name|verbose_update
operator|=
operator|!
name|option_quiet
expr_stmt|;
name|opts
operator|.
name|src_index
operator|=
operator|&
name|the_index
expr_stmt|;
name|opts
operator|.
name|dst_index
operator|=
operator|&
name|the_index
expr_stmt|;
name|tree
operator|=
name|parse_tree_indirect
argument_list|(
name|remote_head
operator|->
name|old_sha1
argument_list|)
expr_stmt|;
name|parse_tree
argument_list|(
name|tree
argument_list|)
expr_stmt|;
name|init_tree_desc
argument_list|(
operator|&
name|t
argument_list|,
name|tree
operator|->
name|buffer
argument_list|,
name|tree
operator|->
name|size
argument_list|)
expr_stmt|;
name|unpack_trees
argument_list|(
literal|1
argument_list|,
operator|&
name|t
argument_list|,
operator|&
name|opts
argument_list|)
expr_stmt|;
if|if
condition|(
name|write_cache
argument_list|(
name|fd
argument_list|,
name|active_cache
argument_list|,
name|active_nr
argument_list|)
operator|||
name|commit_locked_index
argument_list|(
name|lock_file
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to write new index file"
argument_list|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|reflog_msg
argument_list|)
expr_stmt|;
name|junk_pid
operator|=
literal|0
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

