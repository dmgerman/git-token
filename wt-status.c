begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"wt-status.h"
end_include

begin_include
include|#
directive|include
file|"color.h"
end_include

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_include
include|#
directive|include
file|"revision.h"
end_include

begin_include
include|#
directive|include
file|"diffcore.h"
end_include

begin_decl_stmt
DECL|variable|wt_status_use_color
name|int
name|wt_status_use_color
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|wt_status_colors
specifier|static
name|char
name|wt_status_colors
index|[]
index|[
name|COLOR_MAXLEN
index|]
init|=
block|{
literal|""
block|,
comment|/* WT_STATUS_HEADER: normal */
literal|"\033[32m"
block|,
comment|/* WT_STATUS_UPDATED: green */
literal|"\033[31m"
block|,
comment|/* WT_STATUS_CHANGED: red */
literal|"\033[31m"
block|,
comment|/* WT_STATUS_UNTRACKED: red */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|use_add_msg
specifier|static
specifier|const
name|char
name|use_add_msg
index|[]
init|=
literal|"use \"git add<file>...\" to update what will be committed"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|use_add_rm_msg
specifier|static
specifier|const
name|char
name|use_add_rm_msg
index|[]
init|=
literal|"use \"git add/rm<file>...\" to update what will be committed"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|use_add_to_include_msg
specifier|static
specifier|const
name|char
name|use_add_to_include_msg
index|[]
init|=
literal|"use \"git add<file>...\" to include in what will be committed"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|parse_status_slot
specifier|static
name|int
name|parse_status_slot
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
name|int
name|offset
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|var
operator|+
name|offset
argument_list|,
literal|"header"
argument_list|)
condition|)
return|return
name|WT_STATUS_HEADER
return|;
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|var
operator|+
name|offset
argument_list|,
literal|"updated"
argument_list|)
operator|||
operator|!
name|strcasecmp
argument_list|(
name|var
operator|+
name|offset
argument_list|,
literal|"added"
argument_list|)
condition|)
return|return
name|WT_STATUS_UPDATED
return|;
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|var
operator|+
name|offset
argument_list|,
literal|"changed"
argument_list|)
condition|)
return|return
name|WT_STATUS_CHANGED
return|;
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|var
operator|+
name|offset
argument_list|,
literal|"untracked"
argument_list|)
condition|)
return|return
name|WT_STATUS_UNTRACKED
return|;
name|die
argument_list|(
literal|"bad config variable '%s'"
argument_list|,
name|var
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|color
specifier|static
specifier|const
name|char
modifier|*
name|color
parameter_list|(
name|int
name|slot
parameter_list|)
block|{
return|return
name|wt_status_use_color
condition|?
name|wt_status_colors
index|[
name|slot
index|]
else|:
literal|""
return|;
block|}
end_function

begin_function
DECL|function|wt_status_prepare
name|void
name|wt_status_prepare
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|head
decl_stmt|;
name|memset
argument_list|(
name|s
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|s
argument_list|)
argument_list|)
expr_stmt|;
name|head
operator|=
name|resolve_ref
argument_list|(
literal|"HEAD"
argument_list|,
name|sha1
argument_list|,
literal|0
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|s
operator|->
name|branch
operator|=
name|head
condition|?
name|xstrdup
argument_list|(
name|head
argument_list|)
else|:
name|NULL
expr_stmt|;
name|s
operator|->
name|reference
operator|=
literal|"HEAD"
expr_stmt|;
name|s
operator|->
name|fp
operator|=
name|stdout
expr_stmt|;
name|s
operator|->
name|index_file
operator|=
name|get_index_file
argument_list|()
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wt_status_print_cached_header
specifier|static
name|void
name|wt_status_print_cached_header
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|c
init|=
name|color
argument_list|(
name|WT_STATUS_HEADER
argument_list|)
decl_stmt|;
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"# Changes to be committed:"
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|reference
condition|)
block|{
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"#   (use \"git reset %s<file>...\" to unstage)"
argument_list|,
name|s
operator|->
name|reference
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"#   (use \"git rm --cached<file>...\" to unstage)"
argument_list|)
expr_stmt|;
block|}
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"#"
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|function|wt_status_print_header
specifier|static
name|void
name|wt_status_print_header
argument_list|(
expr|struct
name|wt_status
operator|*
name|s
argument_list|,
specifier|const
name|char
operator|*
expr|main
argument_list|,
specifier|const
name|char
operator|*
name|sub
argument_list|)
block|{
specifier|const
name|char
modifier|*
name|c
init|=
name|color
argument_list|(
name|WT_STATUS_HEADER
argument_list|)
decl_stmt|;
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"# %s:"
argument_list|,
expr|main
argument_list|)
expr_stmt|;
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"#   (%s)"
argument_list|,
name|sub
argument_list|)
expr_stmt|;
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"#"
argument_list|)
expr_stmt|;
block|}
end_decl_stmt

begin_function
DECL|function|wt_status_print_trailer
specifier|static
name|void
name|wt_status_print_trailer
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
block|{
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|color
argument_list|(
name|WT_STATUS_HEADER
argument_list|)
argument_list|,
literal|"#"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|quote_path
specifier|static
name|char
modifier|*
name|quote_path
parameter_list|(
specifier|const
name|char
modifier|*
name|in
parameter_list|,
name|int
name|len
parameter_list|,
name|struct
name|strbuf
modifier|*
name|out
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
if|if
condition|(
name|len
operator|<
literal|0
condition|)
name|len
operator|=
name|strlen
argument_list|(
name|in
argument_list|)
expr_stmt|;
name|strbuf_grow
argument_list|(
name|out
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|strbuf_setlen
argument_list|(
name|out
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|prefix
condition|)
block|{
name|int
name|off
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|prefix
index|[
name|off
index|]
operator|&&
name|off
operator|<
name|len
operator|&&
name|prefix
index|[
name|off
index|]
operator|==
name|in
index|[
name|off
index|]
condition|)
if|if
condition|(
name|prefix
index|[
name|off
index|]
operator|==
literal|'/'
condition|)
block|{
name|prefix
operator|+=
name|off
operator|+
literal|1
expr_stmt|;
name|in
operator|+=
name|off
operator|+
literal|1
expr_stmt|;
name|len
operator|-=
name|off
operator|+
literal|1
expr_stmt|;
name|off
operator|=
literal|0
expr_stmt|;
block|}
else|else
name|off
operator|++
expr_stmt|;
for|for
control|(
init|;
operator|*
name|prefix
condition|;
name|prefix
operator|++
control|)
if|if
condition|(
operator|*
name|prefix
operator|==
literal|'/'
condition|)
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
literal|"../"
argument_list|)
expr_stmt|;
block|}
for|for
control|(
init|;
name|len
operator|>
literal|0
condition|;
name|in
operator|++
operator|,
name|len
operator|--
control|)
block|{
name|int
name|ch
init|=
operator|*
name|in
decl_stmt|;
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'\n'
case|:
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
literal|"\\n"
argument_list|)
expr_stmt|;
break|break;
case|case
literal|'\r'
case|:
name|strbuf_addstr
argument_list|(
name|out
argument_list|,
literal|"\\r"
argument_list|)
expr_stmt|;
break|break;
default|default:
name|strbuf_addch
argument_list|(
name|out
argument_list|,
name|ch
argument_list|)
expr_stmt|;
continue|continue;
block|}
block|}
return|return
name|out
operator|->
name|buf
return|;
block|}
end_function

begin_function
DECL|function|wt_status_print_filepair
specifier|static
name|void
name|wt_status_print_filepair
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|,
name|int
name|t
parameter_list|,
name|struct
name|diff_filepair
modifier|*
name|p
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|c
init|=
name|color
argument_list|(
name|t
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|one
decl_stmt|,
modifier|*
name|two
decl_stmt|;
name|struct
name|strbuf
name|onebuf
decl_stmt|,
name|twobuf
decl_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|onebuf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|twobuf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|one
operator|=
name|quote_path
argument_list|(
name|p
operator|->
name|one
operator|->
name|path
argument_list|,
operator|-
literal|1
argument_list|,
operator|&
name|onebuf
argument_list|,
name|s
operator|->
name|prefix
argument_list|)
expr_stmt|;
name|two
operator|=
name|quote_path
argument_list|(
name|p
operator|->
name|two
operator|->
name|path
argument_list|,
operator|-
literal|1
argument_list|,
operator|&
name|twobuf
argument_list|,
name|s
operator|->
name|prefix
argument_list|)
expr_stmt|;
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|color
argument_list|(
name|WT_STATUS_HEADER
argument_list|)
argument_list|,
literal|"#\t"
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|p
operator|->
name|status
condition|)
block|{
case|case
name|DIFF_STATUS_ADDED
case|:
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"new file:   %s"
argument_list|,
name|one
argument_list|)
expr_stmt|;
break|break;
case|case
name|DIFF_STATUS_COPIED
case|:
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"copied:     %s -> %s"
argument_list|,
name|one
argument_list|,
name|two
argument_list|)
expr_stmt|;
break|break;
case|case
name|DIFF_STATUS_DELETED
case|:
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"deleted:    %s"
argument_list|,
name|one
argument_list|)
expr_stmt|;
break|break;
case|case
name|DIFF_STATUS_MODIFIED
case|:
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"modified:   %s"
argument_list|,
name|one
argument_list|)
expr_stmt|;
break|break;
case|case
name|DIFF_STATUS_RENAMED
case|:
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"renamed:    %s -> %s"
argument_list|,
name|one
argument_list|,
name|two
argument_list|)
expr_stmt|;
break|break;
case|case
name|DIFF_STATUS_TYPE_CHANGED
case|:
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"typechange: %s"
argument_list|,
name|one
argument_list|)
expr_stmt|;
break|break;
case|case
name|DIFF_STATUS_UNKNOWN
case|:
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"unknown:    %s"
argument_list|,
name|one
argument_list|)
expr_stmt|;
break|break;
case|case
name|DIFF_STATUS_UNMERGED
case|:
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|c
argument_list|,
literal|"unmerged:   %s"
argument_list|,
name|one
argument_list|)
expr_stmt|;
break|break;
default|default:
name|die
argument_list|(
literal|"bug: unhandled diff status %c"
argument_list|,
name|p
operator|->
name|status
argument_list|)
expr_stmt|;
block|}
name|fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
literal|"\n"
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|onebuf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|twobuf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wt_status_print_updated_cb
specifier|static
name|void
name|wt_status_print_updated_cb
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
name|q
parameter_list|,
name|struct
name|diff_options
modifier|*
name|options
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|struct
name|wt_status
modifier|*
name|s
init|=
name|data
decl_stmt|;
name|int
name|shown_header
init|=
literal|0
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
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|q
operator|->
name|queue
index|[
name|i
index|]
operator|->
name|status
operator|==
literal|'U'
condition|)
continue|continue;
if|if
condition|(
operator|!
name|shown_header
condition|)
block|{
name|wt_status_print_cached_header
argument_list|(
name|s
argument_list|)
expr_stmt|;
name|s
operator|->
name|commitable
operator|=
literal|1
expr_stmt|;
name|shown_header
operator|=
literal|1
expr_stmt|;
block|}
name|wt_status_print_filepair
argument_list|(
name|s
argument_list|,
name|WT_STATUS_UPDATED
argument_list|,
name|q
operator|->
name|queue
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|shown_header
condition|)
name|wt_status_print_trailer
argument_list|(
name|s
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wt_status_print_changed_cb
specifier|static
name|void
name|wt_status_print_changed_cb
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
name|q
parameter_list|,
name|struct
name|diff_options
modifier|*
name|options
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|struct
name|wt_status
modifier|*
name|s
init|=
name|data
decl_stmt|;
name|int
name|i
decl_stmt|;
if|if
condition|(
name|q
operator|->
name|nr
condition|)
block|{
specifier|const
name|char
modifier|*
name|msg
init|=
name|use_add_msg
decl_stmt|;
name|s
operator|->
name|workdir_dirty
operator|=
literal|1
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|q
operator|->
name|queue
index|[
name|i
index|]
operator|->
name|status
operator|==
name|DIFF_STATUS_DELETED
condition|)
block|{
name|msg
operator|=
name|use_add_rm_msg
expr_stmt|;
break|break;
block|}
name|wt_status_print_header
argument_list|(
name|s
argument_list|,
literal|"Changed but not updated"
argument_list|,
name|msg
argument_list|)
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|q
operator|->
name|nr
condition|;
name|i
operator|++
control|)
name|wt_status_print_filepair
argument_list|(
name|s
argument_list|,
name|WT_STATUS_CHANGED
argument_list|,
name|q
operator|->
name|queue
index|[
name|i
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|q
operator|->
name|nr
condition|)
name|wt_status_print_trailer
argument_list|(
name|s
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wt_read_cache
specifier|static
name|void
name|wt_read_cache
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
block|{
name|discard_cache
argument_list|()
expr_stmt|;
name|read_cache_from
argument_list|(
name|s
operator|->
name|index_file
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wt_status_print_initial
specifier|static
name|void
name|wt_status_print_initial
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|struct
name|strbuf
name|buf
decl_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|buf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|wt_read_cache
argument_list|(
name|s
argument_list|)
expr_stmt|;
if|if
condition|(
name|active_nr
condition|)
block|{
name|s
operator|->
name|commitable
operator|=
literal|1
expr_stmt|;
name|wt_status_print_cached_header
argument_list|(
name|s
argument_list|)
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|active_nr
condition|;
name|i
operator|++
control|)
block|{
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|color
argument_list|(
name|WT_STATUS_HEADER
argument_list|)
argument_list|,
literal|"#\t"
argument_list|)
expr_stmt|;
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|color
argument_list|(
name|WT_STATUS_UPDATED
argument_list|)
argument_list|,
literal|"new file: %s"
argument_list|,
name|quote_path
argument_list|(
name|active_cache
index|[
name|i
index|]
operator|->
name|name
argument_list|,
operator|-
literal|1
argument_list|,
operator|&
name|buf
argument_list|,
name|s
operator|->
name|prefix
argument_list|)
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|active_nr
condition|)
name|wt_status_print_trailer
argument_list|(
name|s
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wt_status_print_updated
specifier|static
name|void
name|wt_status_print_updated
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
block|{
name|struct
name|rev_info
name|rev
decl_stmt|;
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
name|s
operator|->
name|reference
argument_list|)
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|output_format
operator||=
name|DIFF_FORMAT_CALLBACK
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|format_callback
operator|=
name|wt_status_print_updated_cb
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|format_callback_data
operator|=
name|s
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|detect_rename
operator|=
literal|1
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|rename_limit
operator|=
literal|100
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|break_opt
operator|=
literal|0
expr_stmt|;
name|wt_read_cache
argument_list|(
name|s
argument_list|)
expr_stmt|;
name|run_diff_index
argument_list|(
operator|&
name|rev
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wt_status_print_changed
specifier|static
name|void
name|wt_status_print_changed
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
block|{
name|struct
name|rev_info
name|rev
decl_stmt|;
name|init_revisions
argument_list|(
operator|&
name|rev
argument_list|,
literal|""
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
name|diffopt
operator|.
name|output_format
operator||=
name|DIFF_FORMAT_CALLBACK
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|format_callback
operator|=
name|wt_status_print_changed_cb
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|format_callback_data
operator|=
name|s
expr_stmt|;
name|wt_read_cache
argument_list|(
name|s
argument_list|)
expr_stmt|;
name|run_diff_files
argument_list|(
operator|&
name|rev
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wt_status_print_untracked
specifier|static
name|void
name|wt_status_print_untracked
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
block|{
name|struct
name|dir_struct
name|dir
decl_stmt|;
name|int
name|i
decl_stmt|;
name|int
name|shown_header
init|=
literal|0
decl_stmt|;
name|struct
name|strbuf
name|buf
decl_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|buf
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|memset
argument_list|(
operator|&
name|dir
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|dir
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|s
operator|->
name|untracked
condition|)
block|{
name|dir
operator|.
name|show_other_directories
operator|=
literal|1
expr_stmt|;
name|dir
operator|.
name|hide_empty_directories
operator|=
literal|1
expr_stmt|;
block|}
name|setup_standard_excludes
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
name|read_directory
argument_list|(
operator|&
name|dir
argument_list|,
literal|"."
argument_list|,
literal|""
argument_list|,
literal|0
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
name|dir
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
comment|/* check for matching entry, which is unmerged; lifted from 		 * builtin-ls-files:show_other_files */
name|struct
name|dir_entry
modifier|*
name|ent
init|=
name|dir
operator|.
name|entries
index|[
name|i
index|]
decl_stmt|;
name|int
name|pos
init|=
name|cache_name_pos
argument_list|(
name|ent
operator|->
name|name
argument_list|,
name|ent
operator|->
name|len
argument_list|)
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
decl_stmt|;
if|if
condition|(
literal|0
operator|<=
name|pos
condition|)
name|die
argument_list|(
literal|"bug in wt_status_print_untracked"
argument_list|)
expr_stmt|;
name|pos
operator|=
operator|-
name|pos
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|pos
operator|<
name|active_nr
condition|)
block|{
name|ce
operator|=
name|active_cache
index|[
name|pos
index|]
expr_stmt|;
if|if
condition|(
name|ce_namelen
argument_list|(
name|ce
argument_list|)
operator|==
name|ent
operator|->
name|len
operator|&&
operator|!
name|memcmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|ent
operator|->
name|name
argument_list|,
name|ent
operator|->
name|len
argument_list|)
condition|)
continue|continue;
block|}
if|if
condition|(
operator|!
name|shown_header
condition|)
block|{
name|s
operator|->
name|workdir_untracked
operator|=
literal|1
expr_stmt|;
name|wt_status_print_header
argument_list|(
name|s
argument_list|,
literal|"Untracked files"
argument_list|,
name|use_add_to_include_msg
argument_list|)
expr_stmt|;
name|shown_header
operator|=
literal|1
expr_stmt|;
block|}
name|color_fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|color
argument_list|(
name|WT_STATUS_HEADER
argument_list|)
argument_list|,
literal|"#\t"
argument_list|)
expr_stmt|;
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|color
argument_list|(
name|WT_STATUS_UNTRACKED
argument_list|)
argument_list|,
literal|"%s"
argument_list|,
name|quote_path
argument_list|(
name|ent
operator|->
name|name
argument_list|,
name|ent
operator|->
name|len
argument_list|,
operator|&
name|buf
argument_list|,
name|s
operator|->
name|prefix
argument_list|)
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
end_function

begin_function
DECL|function|wt_status_print_verbose
specifier|static
name|void
name|wt_status_print_verbose
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
block|{
name|struct
name|rev_info
name|rev
decl_stmt|;
name|int
name|saved_stdout
decl_stmt|;
name|fflush
argument_list|(
name|s
operator|->
name|fp
argument_list|)
expr_stmt|;
comment|/* Sigh, the entire diff machinery is hardcoded to output to 	 * stdout.  Do the dup-dance...*/
name|saved_stdout
operator|=
name|dup
argument_list|(
name|STDOUT_FILENO
argument_list|)
expr_stmt|;
if|if
condition|(
name|saved_stdout
operator|<
literal|0
operator|||
name|dup2
argument_list|(
name|fileno
argument_list|(
name|s
operator|->
name|fp
argument_list|)
argument_list|,
name|STDOUT_FILENO
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"couldn't redirect stdout\n"
argument_list|)
expr_stmt|;
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
name|s
operator|->
name|reference
argument_list|)
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|output_format
operator||=
name|DIFF_FORMAT_PATCH
expr_stmt|;
name|rev
operator|.
name|diffopt
operator|.
name|detect_rename
operator|=
literal|1
expr_stmt|;
name|wt_read_cache
argument_list|(
name|s
argument_list|)
expr_stmt|;
name|run_diff_index
argument_list|(
operator|&
name|rev
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
name|dup2
argument_list|(
name|saved_stdout
argument_list|,
name|STDOUT_FILENO
argument_list|)
operator|<
literal|0
condition|)
name|die
argument_list|(
literal|"couldn't restore stdout\n"
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|saved_stdout
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|wt_status_print
name|void
name|wt_status_print
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|s
operator|->
name|is_initial
operator|=
name|get_sha1
argument_list|(
name|s
operator|->
name|reference
argument_list|,
name|sha1
argument_list|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|branch
condition|)
block|{
specifier|const
name|char
modifier|*
name|on_what
init|=
literal|"On branch "
decl_stmt|;
specifier|const
name|char
modifier|*
name|branch_name
init|=
name|s
operator|->
name|branch
decl_stmt|;
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|branch_name
argument_list|,
literal|"refs/heads/"
argument_list|)
condition|)
name|branch_name
operator|+=
literal|11
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|branch_name
argument_list|,
literal|"HEAD"
argument_list|)
condition|)
block|{
name|branch_name
operator|=
literal|""
expr_stmt|;
name|on_what
operator|=
literal|"Not currently on any branch."
expr_stmt|;
block|}
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|color
argument_list|(
name|WT_STATUS_HEADER
argument_list|)
argument_list|,
literal|"# %s%s"
argument_list|,
name|on_what
argument_list|,
name|branch_name
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|s
operator|->
name|is_initial
condition|)
block|{
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|color
argument_list|(
name|WT_STATUS_HEADER
argument_list|)
argument_list|,
literal|"#"
argument_list|)
expr_stmt|;
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|color
argument_list|(
name|WT_STATUS_HEADER
argument_list|)
argument_list|,
literal|"# Initial commit"
argument_list|)
expr_stmt|;
name|color_fprintf_ln
argument_list|(
name|s
operator|->
name|fp
argument_list|,
name|color
argument_list|(
name|WT_STATUS_HEADER
argument_list|)
argument_list|,
literal|"#"
argument_list|)
expr_stmt|;
name|wt_status_print_initial
argument_list|(
name|s
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|wt_status_print_updated
argument_list|(
name|s
argument_list|)
expr_stmt|;
block|}
name|wt_status_print_changed
argument_list|(
name|s
argument_list|)
expr_stmt|;
name|wt_status_print_untracked
argument_list|(
name|s
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|verbose
operator|&&
operator|!
name|s
operator|->
name|is_initial
condition|)
name|wt_status_print_verbose
argument_list|(
name|s
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|s
operator|->
name|commitable
condition|)
block|{
if|if
condition|(
name|s
operator|->
name|amend
condition|)
name|fprintf
argument_list|(
name|s
operator|->
name|fp
argument_list|,
literal|"# No changes\n"
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|s
operator|->
name|workdir_dirty
condition|)
name|printf
argument_list|(
literal|"no changes added to commit (use \"git add\" and/or \"git commit -a\")\n"
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|s
operator|->
name|workdir_untracked
condition|)
name|printf
argument_list|(
literal|"nothing added to commit but untracked files present (use \"git add\" to track)\n"
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|s
operator|->
name|is_initial
condition|)
name|printf
argument_list|(
literal|"nothing to commit (create/copy files and use \"git add\" to track)\n"
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"nothing to commit (working directory clean)\n"
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|git_status_config
name|int
name|git_status_config
parameter_list|(
specifier|const
name|char
modifier|*
name|k
parameter_list|,
specifier|const
name|char
modifier|*
name|v
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|k
argument_list|,
literal|"status.color"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|k
argument_list|,
literal|"color.status"
argument_list|)
condition|)
block|{
name|wt_status_use_color
operator|=
name|git_config_colorbool
argument_list|(
name|k
argument_list|,
name|v
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|k
argument_list|,
literal|"status.color."
argument_list|)
operator|||
operator|!
name|prefixcmp
argument_list|(
name|k
argument_list|,
literal|"color.status."
argument_list|)
condition|)
block|{
name|int
name|slot
init|=
name|parse_status_slot
argument_list|(
name|k
argument_list|,
literal|13
argument_list|)
decl_stmt|;
name|color_parse
argument_list|(
name|v
argument_list|,
name|k
argument_list|,
name|wt_status_colors
index|[
name|slot
index|]
argument_list|)
expr_stmt|;
block|}
return|return
name|git_default_config
argument_list|(
name|k
argument_list|,
name|v
argument_list|)
return|;
block|}
end_function

end_unit

