begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"submodule.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"diffcore.h"
end_include

begin_include
include|#
directive|include
file|"refs.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_decl_stmt
DECL|variable|config_name_for_path
name|struct
name|string_list
name|config_name_for_path
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|config_ignore_for_name
name|struct
name|string_list
name|config_ignore_for_name
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|add_submodule_odb
specifier|static
name|int
name|add_submodule_odb
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|struct
name|strbuf
name|objects_directory
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|alternate_object_database
modifier|*
name|alt_odb
decl_stmt|;
name|int
name|ret
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|git_dir
decl_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|objects_directory
argument_list|,
literal|"%s/.git"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|git_dir
operator|=
name|read_gitfile_gently
argument_list|(
name|objects_directory
operator|.
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|git_dir
condition|)
block|{
name|strbuf_reset
argument_list|(
operator|&
name|objects_directory
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|objects_directory
argument_list|,
name|git_dir
argument_list|)
expr_stmt|;
block|}
name|strbuf_addstr
argument_list|(
operator|&
name|objects_directory
argument_list|,
literal|"/objects/"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_directory
argument_list|(
name|objects_directory
operator|.
name|buf
argument_list|)
condition|)
block|{
name|ret
operator|=
operator|-
literal|1
expr_stmt|;
goto|goto
name|done
goto|;
block|}
comment|/* avoid adding it twice */
for|for
control|(
name|alt_odb
operator|=
name|alt_odb_list
init|;
name|alt_odb
condition|;
name|alt_odb
operator|=
name|alt_odb
operator|->
name|next
control|)
if|if
condition|(
name|alt_odb
operator|->
name|name
operator|-
name|alt_odb
operator|->
name|base
operator|==
name|objects_directory
operator|.
name|len
operator|&&
operator|!
name|strncmp
argument_list|(
name|alt_odb
operator|->
name|base
argument_list|,
name|objects_directory
operator|.
name|buf
argument_list|,
name|objects_directory
operator|.
name|len
argument_list|)
condition|)
goto|goto
name|done
goto|;
name|alt_odb
operator|=
name|xmalloc
argument_list|(
name|objects_directory
operator|.
name|len
operator|+
literal|42
operator|+
sizeof|sizeof
argument_list|(
operator|*
name|alt_odb
argument_list|)
argument_list|)
expr_stmt|;
name|alt_odb
operator|->
name|next
operator|=
name|alt_odb_list
expr_stmt|;
name|strcpy
argument_list|(
name|alt_odb
operator|->
name|base
argument_list|,
name|objects_directory
operator|.
name|buf
argument_list|)
expr_stmt|;
name|alt_odb
operator|->
name|name
operator|=
name|alt_odb
operator|->
name|base
operator|+
name|objects_directory
operator|.
name|len
expr_stmt|;
name|alt_odb
operator|->
name|name
index|[
literal|2
index|]
operator|=
literal|'/'
expr_stmt|;
name|alt_odb
operator|->
name|name
index|[
literal|40
index|]
operator|=
literal|'\0'
expr_stmt|;
name|alt_odb
operator|->
name|name
index|[
literal|41
index|]
operator|=
literal|'\0'
expr_stmt|;
name|alt_odb_list
operator|=
name|alt_odb
expr_stmt|;
name|prepare_alt_odb
argument_list|()
expr_stmt|;
name|done
label|:
name|strbuf_release
argument_list|(
operator|&
name|objects_directory
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|set_diffopt_flags_from_submodule_config
name|void
name|set_diffopt_flags_from_submodule_config
parameter_list|(
name|struct
name|diff_options
modifier|*
name|diffopt
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|struct
name|string_list_item
modifier|*
name|path_option
decl_stmt|,
modifier|*
name|ignore_option
decl_stmt|;
name|path_option
operator|=
name|unsorted_string_list_lookup
argument_list|(
operator|&
name|config_name_for_path
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|path_option
condition|)
block|{
name|ignore_option
operator|=
name|unsorted_string_list_lookup
argument_list|(
operator|&
name|config_ignore_for_name
argument_list|,
name|path_option
operator|->
name|util
argument_list|)
expr_stmt|;
if|if
condition|(
name|ignore_option
condition|)
name|handle_ignore_submodules_arg
argument_list|(
name|diffopt
argument_list|,
name|ignore_option
operator|->
name|util
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|submodule_config
specifier|static
name|int
name|submodule_config
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
name|prefixcmp
argument_list|(
name|var
argument_list|,
literal|"submodule."
argument_list|)
condition|)
return|return
name|parse_submodule_config_option
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|gitmodules_config
name|void
name|gitmodules_config
parameter_list|(
name|void
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|work_tree
init|=
name|get_git_work_tree
argument_list|()
decl_stmt|;
if|if
condition|(
name|work_tree
condition|)
block|{
name|struct
name|strbuf
name|gitmodules_path
init|=
name|STRBUF_INIT
decl_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|gitmodules_path
argument_list|,
name|work_tree
argument_list|)
expr_stmt|;
name|strbuf_addstr
argument_list|(
operator|&
name|gitmodules_path
argument_list|,
literal|"/.gitmodules"
argument_list|)
expr_stmt|;
name|git_config_from_file
argument_list|(
name|submodule_config
argument_list|,
name|gitmodules_path
operator|.
name|buf
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|gitmodules_path
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|parse_submodule_config_option
name|int
name|parse_submodule_config_option
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
parameter_list|)
block|{
name|int
name|len
decl_stmt|;
name|struct
name|string_list_item
modifier|*
name|config
decl_stmt|;
name|struct
name|strbuf
name|submodname
init|=
name|STRBUF_INIT
decl_stmt|;
name|var
operator|+=
literal|10
expr_stmt|;
comment|/* Skip "submodule." */
name|len
operator|=
name|strlen
argument_list|(
name|var
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|len
operator|>
literal|5
operator|)
operator|&&
operator|!
name|strcmp
argument_list|(
name|var
operator|+
name|len
operator|-
literal|5
argument_list|,
literal|".path"
argument_list|)
condition|)
block|{
name|strbuf_add
argument_list|(
operator|&
name|submodname
argument_list|,
name|var
argument_list|,
name|len
operator|-
literal|5
argument_list|)
expr_stmt|;
name|config
operator|=
name|unsorted_string_list_lookup
argument_list|(
operator|&
name|config_name_for_path
argument_list|,
name|value
argument_list|)
expr_stmt|;
if|if
condition|(
name|config
condition|)
name|free
argument_list|(
name|config
operator|->
name|util
argument_list|)
expr_stmt|;
else|else
name|config
operator|=
name|string_list_append
argument_list|(
operator|&
name|config_name_for_path
argument_list|,
name|xstrdup
argument_list|(
name|value
argument_list|)
argument_list|)
expr_stmt|;
name|config
operator|->
name|util
operator|=
name|strbuf_detach
argument_list|(
operator|&
name|submodname
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|submodname
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|(
name|len
operator|>
literal|7
operator|)
operator|&&
operator|!
name|strcmp
argument_list|(
name|var
operator|+
name|len
operator|-
literal|7
argument_list|,
literal|".ignore"
argument_list|)
condition|)
block|{
if|if
condition|(
name|strcmp
argument_list|(
name|value
argument_list|,
literal|"untracked"
argument_list|)
operator|&&
name|strcmp
argument_list|(
name|value
argument_list|,
literal|"dirty"
argument_list|)
operator|&&
name|strcmp
argument_list|(
name|value
argument_list|,
literal|"all"
argument_list|)
operator|&&
name|strcmp
argument_list|(
name|value
argument_list|,
literal|"none"
argument_list|)
condition|)
block|{
name|warning
argument_list|(
literal|"Invalid parameter \"%s\" for config option \"submodule.%s.ignore\""
argument_list|,
name|value
argument_list|,
name|var
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|strbuf_add
argument_list|(
operator|&
name|submodname
argument_list|,
name|var
argument_list|,
name|len
operator|-
literal|7
argument_list|)
expr_stmt|;
name|config
operator|=
name|unsorted_string_list_lookup
argument_list|(
operator|&
name|config_ignore_for_name
argument_list|,
name|submodname
operator|.
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|config
condition|)
name|free
argument_list|(
name|config
operator|->
name|util
argument_list|)
expr_stmt|;
else|else
name|config
operator|=
name|string_list_append
argument_list|(
operator|&
name|config_ignore_for_name
argument_list|,
name|strbuf_detach
argument_list|(
operator|&
name|submodname
argument_list|,
name|NULL
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|submodname
argument_list|)
expr_stmt|;
name|config
operator|->
name|util
operator|=
name|xstrdup
argument_list|(
name|value
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|handle_ignore_submodules_arg
name|void
name|handle_ignore_submodules_arg
parameter_list|(
name|struct
name|diff_options
modifier|*
name|diffopt
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|)
block|{
name|DIFF_OPT_CLR
argument_list|(
name|diffopt
argument_list|,
name|IGNORE_SUBMODULES
argument_list|)
expr_stmt|;
name|DIFF_OPT_CLR
argument_list|(
name|diffopt
argument_list|,
name|IGNORE_UNTRACKED_IN_SUBMODULES
argument_list|)
expr_stmt|;
name|DIFF_OPT_CLR
argument_list|(
name|diffopt
argument_list|,
name|IGNORE_DIRTY_SUBMODULES
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"all"
argument_list|)
condition|)
name|DIFF_OPT_SET
argument_list|(
name|diffopt
argument_list|,
name|IGNORE_SUBMODULES
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"untracked"
argument_list|)
condition|)
name|DIFF_OPT_SET
argument_list|(
name|diffopt
argument_list|,
name|IGNORE_UNTRACKED_IN_SUBMODULES
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"dirty"
argument_list|)
condition|)
name|DIFF_OPT_SET
argument_list|(
name|diffopt
argument_list|,
name|IGNORE_DIRTY_SUBMODULES
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"none"
argument_list|)
condition|)
name|die
argument_list|(
literal|"bad --ignore-submodules argument: %s"
argument_list|,
name|arg
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_submodule_summary
name|void
name|show_submodule_summary
parameter_list|(
name|FILE
modifier|*
name|f
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|char
name|one
index|[
literal|20
index|]
parameter_list|,
name|unsigned
name|char
name|two
index|[
literal|20
index|]
parameter_list|,
name|unsigned
name|dirty_submodule
parameter_list|,
specifier|const
name|char
modifier|*
name|del
parameter_list|,
specifier|const
name|char
modifier|*
name|add
parameter_list|,
specifier|const
name|char
modifier|*
name|reset
parameter_list|)
block|{
name|struct
name|rev_info
name|rev
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|,
modifier|*
name|left
init|=
name|left
decl_stmt|,
modifier|*
name|right
init|=
name|right
decl_stmt|;
name|struct
name|commit_list
modifier|*
name|merge_bases
decl_stmt|,
modifier|*
name|list
decl_stmt|;
specifier|const
name|char
modifier|*
name|message
init|=
name|NULL
decl_stmt|;
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
specifier|static
specifier|const
name|char
modifier|*
name|format
init|=
literal|"  %m %s"
decl_stmt|;
name|int
name|fast_forward
init|=
literal|0
decl_stmt|,
name|fast_backward
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|is_null_sha1
argument_list|(
name|two
argument_list|)
condition|)
name|message
operator|=
literal|"(submodule deleted)"
expr_stmt|;
elseif|else
if|if
condition|(
name|add_submodule_odb
argument_list|(
name|path
argument_list|)
condition|)
name|message
operator|=
literal|"(not checked out)"
expr_stmt|;
elseif|else
if|if
condition|(
name|is_null_sha1
argument_list|(
name|one
argument_list|)
condition|)
name|message
operator|=
literal|"(new submodule)"
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
operator|(
name|left
operator|=
name|lookup_commit_reference
argument_list|(
name|one
argument_list|)
operator|)
operator|||
operator|!
operator|(
name|right
operator|=
name|lookup_commit_reference
argument_list|(
name|two
argument_list|)
operator|)
condition|)
name|message
operator|=
literal|"(commits not present)"
expr_stmt|;
if|if
condition|(
operator|!
name|message
condition|)
block|{
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|setup_revisions
argument_list|(
literal|0
argument_list|,
name|NULL
argument_list|,
operator|&
name|rev
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|rev
operator|.
name|left_right
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|first_parent_only
operator|=
literal|1
expr_stmt|;
name|left
operator|->
name|object
operator|.
name|flags
operator||=
name|SYMMETRIC_LEFT
expr_stmt|;
name|add_pending_object
argument_list|(
operator|&
name|rev
argument_list|,
operator|&
name|left
operator|->
name|object
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|add_pending_object
argument_list|(
operator|&
name|rev
argument_list|,
operator|&
name|right
operator|->
name|object
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|merge_bases
operator|=
name|get_merge_bases
argument_list|(
name|left
argument_list|,
name|right
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|merge_bases
condition|)
block|{
if|if
condition|(
name|merge_bases
operator|->
name|item
operator|==
name|left
condition|)
name|fast_forward
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
name|merge_bases
operator|->
name|item
operator|==
name|right
condition|)
name|fast_backward
operator|=
literal|1
expr_stmt|;
block|}
for|for
control|(
name|list
operator|=
name|merge_bases
init|;
name|list
condition|;
name|list
operator|=
name|list
operator|->
name|next
control|)
block|{
name|list
operator|->
name|item
operator|->
name|object
operator|.
name|flags
operator||=
name|UNINTERESTING
expr_stmt|;
name|add_pending_object
argument_list|(
operator|&
name|rev
argument_list|,
operator|&
name|list
operator|->
name|item
operator|->
name|object
argument_list|,
name|sha1_to_hex
argument_list|(
name|list
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|prepare_revision_walk
argument_list|(
operator|&
name|rev
argument_list|)
condition|)
name|message
operator|=
literal|"(revision walker failed)"
expr_stmt|;
block|}
if|if
condition|(
name|dirty_submodule
operator|&
name|DIRTY_SUBMODULE_UNTRACKED
condition|)
name|fprintf
argument_list|(
name|f
argument_list|,
literal|"Submodule %s contains untracked content\n"
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|dirty_submodule
operator|&
name|DIRTY_SUBMODULE_MODIFIED
condition|)
name|fprintf
argument_list|(
name|f
argument_list|,
literal|"Submodule %s contains modified content\n"
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|hashcmp
argument_list|(
name|one
argument_list|,
name|two
argument_list|)
condition|)
block|{
name|strbuf_release
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
return|return;
block|}
name|strbuf_addf
argument_list|(
operator|&
name|sb
argument_list|,
literal|"Submodule %s %s.."
argument_list|,
name|path
argument_list|,
name|find_unique_abbrev
argument_list|(
name|one
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|fast_backward
operator|&&
operator|!
name|fast_forward
condition|)
name|strbuf_addch
argument_list|(
operator|&
name|sb
argument_list|,
literal|'.'
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|sb
argument_list|,
literal|"%s"
argument_list|,
name|find_unique_abbrev
argument_list|(
name|two
argument_list|,
name|DEFAULT_ABBREV
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|message
condition|)
name|strbuf_addf
argument_list|(
operator|&
name|sb
argument_list|,
literal|" %s\n"
argument_list|,
name|message
argument_list|)
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
operator|&
name|sb
argument_list|,
literal|"%s:\n"
argument_list|,
name|fast_backward
condition|?
literal|" (rewind)"
else|:
literal|""
argument_list|)
expr_stmt|;
name|fwrite
argument_list|(
name|sb
operator|.
name|buf
argument_list|,
name|sb
operator|.
name|len
argument_list|,
literal|1
argument_list|,
name|f
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|message
condition|)
block|{
while|while
condition|(
operator|(
name|commit
operator|=
name|get_revision
argument_list|(
operator|&
name|rev
argument_list|)
operator|)
condition|)
block|{
name|struct
name|pretty_print_context
name|ctx
init|=
block|{
literal|0
block|}
decl_stmt|;
name|ctx
operator|.
name|date_mode
operator|=
name|rev
operator|.
name|date_mode
expr_stmt|;
name|strbuf_setlen
argument_list|(
operator|&
name|sb
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|commit
operator|->
name|object
operator|.
name|flags
operator|&
name|SYMMETRIC_LEFT
condition|)
block|{
if|if
condition|(
name|del
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|sb
argument_list|,
name|del
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|add
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|sb
argument_list|,
name|add
argument_list|)
expr_stmt|;
name|format_commit_message
argument_list|(
name|commit
argument_list|,
name|format
argument_list|,
operator|&
name|sb
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
if|if
condition|(
name|reset
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|sb
argument_list|,
name|reset
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
operator|&
name|sb
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|f
argument_list|,
literal|"%s"
argument_list|,
name|sb
operator|.
name|buf
argument_list|)
expr_stmt|;
block|}
name|clear_commit_marks
argument_list|(
name|left
argument_list|,
operator|~
literal|0
argument_list|)
expr_stmt|;
name|clear_commit_marks
argument_list|(
name|right
argument_list|,
operator|~
literal|0
argument_list|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|is_submodule_modified
name|unsigned
name|is_submodule_modified
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|int
name|ignore_untracked
parameter_list|)
block|{
name|ssize_t
name|len
decl_stmt|;
name|struct
name|child_process
name|cp
decl_stmt|;
specifier|const
name|char
modifier|*
name|argv
index|[]
init|=
block|{
literal|"status"
block|,
literal|"--porcelain"
block|,
name|NULL
block|,
name|NULL
block|, 	}
decl_stmt|;
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|unsigned
name|dirty_submodule
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|line
decl_stmt|,
modifier|*
name|next_line
decl_stmt|;
specifier|const
name|char
modifier|*
name|git_dir
decl_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"%s/.git"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|git_dir
operator|=
name|read_gitfile_gently
argument_list|(
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|git_dir
condition|)
name|git_dir
operator|=
name|buf
operator|.
name|buf
expr_stmt|;
if|if
condition|(
operator|!
name|is_directory
argument_list|(
name|git_dir
argument_list|)
condition|)
block|{
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
comment|/* The submodule is not checked out, so it is not modified */
return|return
literal|0
return|;
block|}
name|strbuf_reset
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|ignore_untracked
condition|)
name|argv
index|[
literal|2
index|]
operator|=
literal|"-uno"
expr_stmt|;
name|memset
argument_list|(
operator|&
name|cp
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|cp
argument_list|)
argument_list|)
expr_stmt|;
name|cp
operator|.
name|argv
operator|=
name|argv
expr_stmt|;
name|cp
operator|.
name|env
operator|=
name|local_repo_env
expr_stmt|;
name|cp
operator|.
name|git_cmd
operator|=
literal|1
expr_stmt|;
name|cp
operator|.
name|no_stdin
operator|=
literal|1
expr_stmt|;
name|cp
operator|.
name|out
operator|=
operator|-
literal|1
expr_stmt|;
name|cp
operator|.
name|dir
operator|=
name|path
expr_stmt|;
if|if
condition|(
name|start_command
argument_list|(
operator|&
name|cp
argument_list|)
condition|)
name|die
argument_list|(
literal|"Could not run git status --porcelain"
argument_list|)
expr_stmt|;
name|len
operator|=
name|strbuf_read
argument_list|(
operator|&
name|buf
argument_list|,
name|cp
operator|.
name|out
argument_list|,
literal|1024
argument_list|)
expr_stmt|;
name|line
operator|=
name|buf
operator|.
name|buf
expr_stmt|;
while|while
condition|(
name|len
operator|>
literal|2
condition|)
block|{
if|if
condition|(
operator|(
name|line
index|[
literal|0
index|]
operator|==
literal|'?'
operator|)
operator|&&
operator|(
name|line
index|[
literal|1
index|]
operator|==
literal|'?'
operator|)
condition|)
block|{
name|dirty_submodule
operator||=
name|DIRTY_SUBMODULE_UNTRACKED
expr_stmt|;
if|if
condition|(
name|dirty_submodule
operator|&
name|DIRTY_SUBMODULE_MODIFIED
condition|)
break|break;
block|}
else|else
block|{
name|dirty_submodule
operator||=
name|DIRTY_SUBMODULE_MODIFIED
expr_stmt|;
if|if
condition|(
name|ignore_untracked
operator|||
operator|(
name|dirty_submodule
operator|&
name|DIRTY_SUBMODULE_UNTRACKED
operator|)
condition|)
break|break;
block|}
name|next_line
operator|=
name|strchr
argument_list|(
name|line
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|next_line
condition|)
break|break;
name|next_line
operator|++
expr_stmt|;
name|len
operator|-=
operator|(
name|next_line
operator|-
name|line
operator|)
expr_stmt|;
name|line
operator|=
name|next_line
expr_stmt|;
block|}
name|close
argument_list|(
name|cp
operator|.
name|out
argument_list|)
expr_stmt|;
if|if
condition|(
name|finish_command
argument_list|(
operator|&
name|cp
argument_list|)
condition|)
name|die
argument_list|(
literal|"git status --porcelain failed"
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
return|return
name|dirty_submodule
return|;
block|}
end_function

begin_function
DECL|function|find_first_merges
specifier|static
name|int
name|find_first_merges
parameter_list|(
name|struct
name|object_array
modifier|*
name|result
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|struct
name|commit
modifier|*
name|a
parameter_list|,
name|struct
name|commit
modifier|*
name|b
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|j
decl_stmt|;
name|struct
name|object_array
name|merges
decl_stmt|;
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
name|int
name|contains_another
decl_stmt|;
name|char
name|merged_revision
index|[
literal|42
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|rev_args
index|[]
init|=
block|{
literal|"rev-list"
block|,
literal|"--merges"
block|,
literal|"--ancestry-path"
block|,
literal|"--all"
block|,
name|merged_revision
block|,
name|NULL
block|}
decl_stmt|;
name|struct
name|rev_info
name|revs
decl_stmt|;
name|struct
name|setup_revision_opt
name|rev_opts
decl_stmt|;
name|memset
argument_list|(
operator|&
name|merges
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|merges
argument_list|)
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|result
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|object_array
argument_list|)
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|rev_opts
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|rev_opts
argument_list|)
argument_list|)
expr_stmt|;
comment|/* get all revisions that merge commit a */
name|snprintf
argument_list|(
name|merged_revision
argument_list|,
sizeof|sizeof
argument_list|(
name|merged_revision
argument_list|)
argument_list|,
literal|"^%s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|a
operator|->
name|object
operator|.
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|init_revisions
argument_list|(
operator|&
name|revs
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|rev_opts
operator|.
name|submodule
operator|=
name|path
expr_stmt|;
name|setup_revisions
argument_list|(
sizeof|sizeof
argument_list|(
name|rev_args
argument_list|)
operator|/
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
operator|-
literal|1
argument_list|,
name|rev_args
argument_list|,
operator|&
name|revs
argument_list|,
operator|&
name|rev_opts
argument_list|)
expr_stmt|;
comment|/* save all revisions from the above list that contain b */
if|if
condition|(
name|prepare_revision_walk
argument_list|(
operator|&
name|revs
argument_list|)
condition|)
name|die
argument_list|(
literal|"revision walk setup failed"
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|commit
operator|=
name|get_revision
argument_list|(
operator|&
name|revs
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|struct
name|object
modifier|*
name|o
init|=
operator|&
operator|(
name|commit
operator|->
name|object
operator|)
decl_stmt|;
if|if
condition|(
name|in_merge_bases
argument_list|(
name|b
argument_list|,
operator|&
name|commit
argument_list|,
literal|1
argument_list|)
condition|)
name|add_object_array
argument_list|(
name|o
argument_list|,
name|NULL
argument_list|,
operator|&
name|merges
argument_list|)
expr_stmt|;
block|}
comment|/* Now we've got all merges that contain a and b. Prune all 	 * merges that contain another found merge and save them in 	 * result. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|merges
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|commit
modifier|*
name|m1
init|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|merges
operator|.
name|objects
index|[
name|i
index|]
operator|.
name|item
decl_stmt|;
name|contains_another
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|merges
operator|.
name|nr
condition|;
name|j
operator|++
control|)
block|{
name|struct
name|commit
modifier|*
name|m2
init|=
operator|(
expr|struct
name|commit
operator|*
operator|)
name|merges
operator|.
name|objects
index|[
name|j
index|]
operator|.
name|item
decl_stmt|;
if|if
condition|(
name|i
operator|!=
name|j
operator|&&
name|in_merge_bases
argument_list|(
name|m2
argument_list|,
operator|&
name|m1
argument_list|,
literal|1
argument_list|)
condition|)
block|{
name|contains_another
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
operator|!
name|contains_another
condition|)
name|add_object_array
argument_list|(
name|merges
operator|.
name|objects
index|[
name|i
index|]
operator|.
name|item
argument_list|,
name|merges
operator|.
name|objects
index|[
name|i
index|]
operator|.
name|name
argument_list|,
name|result
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|merges
operator|.
name|objects
argument_list|)
expr_stmt|;
return|return
name|result
operator|->
name|nr
return|;
block|}
end_function

begin_function
DECL|function|print_commit
specifier|static
name|void
name|print_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
block|{
name|struct
name|strbuf
name|sb
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|pretty_print_context
name|ctx
init|=
block|{
literal|0
block|}
decl_stmt|;
name|ctx
operator|.
name|date_mode
operator|=
name|DATE_NORMAL
expr_stmt|;
name|format_commit_message
argument_list|(
name|commit
argument_list|,
literal|" %h: %m %s"
argument_list|,
operator|&
name|sb
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s\n"
argument_list|,
name|sb
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
block|}
end_function

begin_define
DECL|macro|MERGE_WARNING
define|#
directive|define
name|MERGE_WARNING
parameter_list|(
name|path
parameter_list|,
name|msg
parameter_list|)
define|\
value|warning("Failed to merge submodule %s (%s)", path, msg);
end_define

begin_function
DECL|function|merge_submodule
name|int
name|merge_submodule
parameter_list|(
name|unsigned
name|char
name|result
index|[
literal|20
index|]
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
name|base
index|[
literal|20
index|]
parameter_list|,
specifier|const
name|unsigned
name|char
name|a
index|[
literal|20
index|]
parameter_list|,
specifier|const
name|unsigned
name|char
name|b
index|[
literal|20
index|]
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|commit_base
decl_stmt|,
modifier|*
name|commit_a
decl_stmt|,
modifier|*
name|commit_b
decl_stmt|;
name|int
name|parent_count
decl_stmt|;
name|struct
name|object_array
name|merges
decl_stmt|;
name|int
name|i
decl_stmt|;
comment|/* store a in result in case we fail */
name|hashcpy
argument_list|(
name|result
argument_list|,
name|a
argument_list|)
expr_stmt|;
comment|/* we can not handle deletion conflicts */
if|if
condition|(
name|is_null_sha1
argument_list|(
name|base
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|is_null_sha1
argument_list|(
name|a
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|is_null_sha1
argument_list|(
name|b
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|add_submodule_odb
argument_list|(
name|path
argument_list|)
condition|)
block|{
name|MERGE_WARNING
argument_list|(
name|path
argument_list|,
literal|"not checked out"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
operator|(
name|commit_base
operator|=
name|lookup_commit_reference
argument_list|(
name|base
argument_list|)
operator|)
operator|||
operator|!
operator|(
name|commit_a
operator|=
name|lookup_commit_reference
argument_list|(
name|a
argument_list|)
operator|)
operator|||
operator|!
operator|(
name|commit_b
operator|=
name|lookup_commit_reference
argument_list|(
name|b
argument_list|)
operator|)
condition|)
block|{
name|MERGE_WARNING
argument_list|(
name|path
argument_list|,
literal|"commits not present"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
comment|/* check whether both changes are forward */
if|if
condition|(
operator|!
name|in_merge_bases
argument_list|(
name|commit_base
argument_list|,
operator|&
name|commit_a
argument_list|,
literal|1
argument_list|)
operator|||
operator|!
name|in_merge_bases
argument_list|(
name|commit_base
argument_list|,
operator|&
name|commit_b
argument_list|,
literal|1
argument_list|)
condition|)
block|{
name|MERGE_WARNING
argument_list|(
name|path
argument_list|,
literal|"commits don't follow merge-base"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
comment|/* Case #1: a is contained in b or vice versa */
if|if
condition|(
name|in_merge_bases
argument_list|(
name|commit_a
argument_list|,
operator|&
name|commit_b
argument_list|,
literal|1
argument_list|)
condition|)
block|{
name|hashcpy
argument_list|(
name|result
argument_list|,
name|b
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
if|if
condition|(
name|in_merge_bases
argument_list|(
name|commit_b
argument_list|,
operator|&
name|commit_a
argument_list|,
literal|1
argument_list|)
condition|)
block|{
name|hashcpy
argument_list|(
name|result
argument_list|,
name|a
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
comment|/* 	 * Case #2: There are one or more merges that contain a and b in 	 * the submodule. If there is only one, then present it as a 	 * suggestion to the user, but leave it marked unmerged so the 	 * user needs to confirm the resolution. 	 */
comment|/* find commit which merges them */
name|parent_count
operator|=
name|find_first_merges
argument_list|(
operator|&
name|merges
argument_list|,
name|path
argument_list|,
name|commit_a
argument_list|,
name|commit_b
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|parent_count
condition|)
block|{
case|case
literal|0
case|:
name|MERGE_WARNING
argument_list|(
name|path
argument_list|,
literal|"merge following commits not found"
argument_list|)
expr_stmt|;
break|break;
case|case
literal|1
case|:
name|MERGE_WARNING
argument_list|(
name|path
argument_list|,
literal|"not fast-forward"
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Found a possible merge resolution "
literal|"for the submodule:\n"
argument_list|)
expr_stmt|;
name|print_commit
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|merges
operator|.
name|objects
index|[
literal|0
index|]
operator|.
name|item
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"If this is correct simply add it to the index "
literal|"for example\n"
literal|"by using:\n\n"
literal|"  git update-index --cacheinfo 160000 %s \"%s\"\n\n"
literal|"which will accept this suggestion.\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|merges
operator|.
name|objects
index|[
literal|0
index|]
operator|.
name|item
operator|->
name|sha1
argument_list|)
argument_list|,
name|path
argument_list|)
expr_stmt|;
break|break;
default|default:
name|MERGE_WARNING
argument_list|(
name|path
argument_list|,
literal|"multiple merges found"
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
name|merges
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|print_commit
argument_list|(
operator|(
expr|struct
name|commit
operator|*
operator|)
name|merges
operator|.
name|objects
index|[
name|i
index|]
operator|.
name|item
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|merges
operator|.
name|objects
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

