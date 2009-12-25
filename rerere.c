begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_include
include|#
directive|include
file|"rerere.h"
end_include

begin_include
include|#
directive|include
file|"xdiff/xdiff.h"
end_include

begin_include
include|#
directive|include
file|"xdiff-interface.h"
end_include

begin_comment
comment|/* if rerere_enabled == -1, fall back to detection of .git/rr-cache */
end_comment

begin_decl_stmt
DECL|variable|rerere_enabled
specifier|static
name|int
name|rerere_enabled
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* automatically update cleanly resolved paths to the index */
end_comment

begin_decl_stmt
DECL|variable|rerere_autoupdate
specifier|static
name|int
name|rerere_autoupdate
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|merge_rr_path
specifier|static
name|char
modifier|*
name|merge_rr_path
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|rerere_path
specifier|const
name|char
modifier|*
name|rerere_path
parameter_list|(
specifier|const
name|char
modifier|*
name|hex
parameter_list|,
specifier|const
name|char
modifier|*
name|file
parameter_list|)
block|{
return|return
name|git_path
argument_list|(
literal|"rr-cache/%s/%s"
argument_list|,
name|hex
argument_list|,
name|file
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|has_rerere_resolution
name|int
name|has_rerere_resolution
parameter_list|(
specifier|const
name|char
modifier|*
name|hex
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
return|return
operator|!
name|stat
argument_list|(
name|rerere_path
argument_list|(
name|hex
argument_list|,
literal|"postimage"
argument_list|)
argument_list|,
operator|&
name|st
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|read_rr
specifier|static
name|void
name|read_rr
parameter_list|(
name|struct
name|string_list
modifier|*
name|rr
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
name|buf
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|FILE
modifier|*
name|in
init|=
name|fopen
argument_list|(
name|merge_rr_path
argument_list|,
literal|"r"
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|in
condition|)
return|return;
while|while
condition|(
name|fread
argument_list|(
name|buf
argument_list|,
literal|40
argument_list|,
literal|1
argument_list|,
name|in
argument_list|)
operator|==
literal|1
condition|)
block|{
name|int
name|i
decl_stmt|;
name|char
modifier|*
name|name
decl_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|buf
argument_list|,
name|sha1
argument_list|)
condition|)
name|die
argument_list|(
literal|"corrupt MERGE_RR"
argument_list|)
expr_stmt|;
name|buf
index|[
literal|40
index|]
operator|=
literal|'\0'
expr_stmt|;
name|name
operator|=
name|xstrdup
argument_list|(
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|fgetc
argument_list|(
name|in
argument_list|)
operator|!=
literal|'\t'
condition|)
name|die
argument_list|(
literal|"corrupt MERGE_RR"
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
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
operator|&&
operator|(
name|buf
index|[
name|i
index|]
operator|=
name|fgetc
argument_list|(
name|in
argument_list|)
operator|)
condition|;
name|i
operator|++
control|)
empty_stmt|;
comment|/* do nothing */
if|if
condition|(
name|i
operator|==
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
condition|)
name|die
argument_list|(
literal|"filename too long"
argument_list|)
expr_stmt|;
name|string_list_insert
argument_list|(
name|buf
argument_list|,
name|rr
argument_list|)
operator|->
name|util
operator|=
name|name
expr_stmt|;
block|}
name|fclose
argument_list|(
name|in
argument_list|)
expr_stmt|;
block|}
end_function

begin_decl_stmt
DECL|variable|write_lock
specifier|static
name|struct
name|lock_file
name|write_lock
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|write_rr
specifier|static
name|int
name|write_rr
parameter_list|(
name|struct
name|string_list
modifier|*
name|rr
parameter_list|,
name|int
name|out_fd
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
name|rr
operator|->
name|nr
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
name|int
name|length
decl_stmt|;
if|if
condition|(
operator|!
name|rr
operator|->
name|items
index|[
name|i
index|]
operator|.
name|util
condition|)
continue|continue;
name|path
operator|=
name|rr
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
expr_stmt|;
name|length
operator|=
name|strlen
argument_list|(
name|path
argument_list|)
operator|+
literal|1
expr_stmt|;
if|if
condition|(
name|write_in_full
argument_list|(
name|out_fd
argument_list|,
name|rr
operator|->
name|items
index|[
name|i
index|]
operator|.
name|util
argument_list|,
literal|40
argument_list|)
operator|!=
literal|40
operator|||
name|write_str_in_full
argument_list|(
name|out_fd
argument_list|,
literal|"\t"
argument_list|)
operator|!=
literal|1
operator|||
name|write_in_full
argument_list|(
name|out_fd
argument_list|,
name|path
argument_list|,
name|length
argument_list|)
operator|!=
name|length
condition|)
name|die
argument_list|(
literal|"unable to write rerere record"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|commit_lock_file
argument_list|(
operator|&
name|write_lock
argument_list|)
operator|!=
literal|0
condition|)
name|die
argument_list|(
literal|"unable to write rerere record"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|ferr_write
specifier|static
name|void
name|ferr_write
parameter_list|(
specifier|const
name|void
modifier|*
name|p
parameter_list|,
name|size_t
name|count
parameter_list|,
name|FILE
modifier|*
name|fp
parameter_list|,
name|int
modifier|*
name|err
parameter_list|)
block|{
if|if
condition|(
operator|!
name|count
operator|||
operator|*
name|err
condition|)
return|return;
if|if
condition|(
name|fwrite
argument_list|(
name|p
argument_list|,
name|count
argument_list|,
literal|1
argument_list|,
name|fp
argument_list|)
operator|!=
literal|1
condition|)
operator|*
name|err
operator|=
name|errno
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ferr_puts
specifier|static
specifier|inline
name|void
name|ferr_puts
parameter_list|(
specifier|const
name|char
modifier|*
name|s
parameter_list|,
name|FILE
modifier|*
name|fp
parameter_list|,
name|int
modifier|*
name|err
parameter_list|)
block|{
name|ferr_write
argument_list|(
name|s
argument_list|,
name|strlen
argument_list|(
name|s
argument_list|)
argument_list|,
name|fp
argument_list|,
name|err
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|handle_file
specifier|static
name|int
name|handle_file
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|output
parameter_list|)
block|{
name|git_SHA_CTX
name|ctx
decl_stmt|;
name|int
name|hunk_no
init|=
literal|0
decl_stmt|;
enum|enum
block|{
name|RR_CONTEXT
init|=
literal|0
block|,
name|RR_SIDE_1
block|,
name|RR_SIDE_2
block|,
name|RR_ORIGINAL
block|, 	}
name|hunk
init|=
name|RR_CONTEXT
enum|;
name|struct
name|strbuf
name|one
init|=
name|STRBUF_INIT
decl_stmt|,
name|two
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|FILE
modifier|*
name|f
init|=
name|fopen
argument_list|(
name|path
argument_list|,
literal|"r"
argument_list|)
decl_stmt|;
name|FILE
modifier|*
name|out
init|=
name|NULL
decl_stmt|;
name|int
name|wrerror
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|f
condition|)
return|return
name|error
argument_list|(
literal|"Could not open %s"
argument_list|,
name|path
argument_list|)
return|;
if|if
condition|(
name|output
condition|)
block|{
name|out
operator|=
name|fopen
argument_list|(
name|output
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|out
condition|)
block|{
name|fclose
argument_list|(
name|f
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"Could not write %s"
argument_list|,
name|output
argument_list|)
return|;
block|}
block|}
if|if
condition|(
name|sha1
condition|)
name|git_SHA1_Init
argument_list|(
operator|&
name|ctx
argument_list|)
expr_stmt|;
while|while
condition|(
operator|!
name|strbuf_getwholeline
argument_list|(
operator|&
name|buf
argument_list|,
name|f
argument_list|,
literal|'\n'
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"<<<<<<< "
argument_list|)
condition|)
block|{
if|if
condition|(
name|hunk
operator|!=
name|RR_CONTEXT
condition|)
goto|goto
name|bad
goto|;
name|hunk
operator|=
name|RR_SIDE_1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"|||||||"
argument_list|)
operator|&&
name|isspace
argument_list|(
name|buf
operator|.
name|buf
index|[
literal|7
index|]
argument_list|)
condition|)
block|{
if|if
condition|(
name|hunk
operator|!=
name|RR_SIDE_1
condition|)
goto|goto
name|bad
goto|;
name|hunk
operator|=
name|RR_ORIGINAL
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|"======="
argument_list|)
operator|&&
name|isspace
argument_list|(
name|buf
operator|.
name|buf
index|[
literal|7
index|]
argument_list|)
condition|)
block|{
if|if
condition|(
name|hunk
operator|!=
name|RR_SIDE_1
operator|&&
name|hunk
operator|!=
name|RR_ORIGINAL
condition|)
goto|goto
name|bad
goto|;
name|hunk
operator|=
name|RR_SIDE_2
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
literal|">>>>>>> "
argument_list|)
condition|)
block|{
if|if
condition|(
name|hunk
operator|!=
name|RR_SIDE_2
condition|)
goto|goto
name|bad
goto|;
if|if
condition|(
name|strbuf_cmp
argument_list|(
operator|&
name|one
argument_list|,
operator|&
name|two
argument_list|)
operator|>
literal|0
condition|)
name|strbuf_swap
argument_list|(
operator|&
name|one
argument_list|,
operator|&
name|two
argument_list|)
expr_stmt|;
name|hunk_no
operator|++
expr_stmt|;
name|hunk
operator|=
name|RR_CONTEXT
expr_stmt|;
if|if
condition|(
name|out
condition|)
block|{
name|ferr_puts
argument_list|(
literal|"<<<<<<<\n"
argument_list|,
name|out
argument_list|,
operator|&
name|wrerror
argument_list|)
expr_stmt|;
name|ferr_write
argument_list|(
name|one
operator|.
name|buf
argument_list|,
name|one
operator|.
name|len
argument_list|,
name|out
argument_list|,
operator|&
name|wrerror
argument_list|)
expr_stmt|;
name|ferr_puts
argument_list|(
literal|"=======\n"
argument_list|,
name|out
argument_list|,
operator|&
name|wrerror
argument_list|)
expr_stmt|;
name|ferr_write
argument_list|(
name|two
operator|.
name|buf
argument_list|,
name|two
operator|.
name|len
argument_list|,
name|out
argument_list|,
operator|&
name|wrerror
argument_list|)
expr_stmt|;
name|ferr_puts
argument_list|(
literal|">>>>>>>\n"
argument_list|,
name|out
argument_list|,
operator|&
name|wrerror
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|sha1
condition|)
block|{
name|git_SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
name|one
operator|.
name|buf
condition|?
name|one
operator|.
name|buf
else|:
literal|""
argument_list|,
name|one
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|git_SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
name|two
operator|.
name|buf
condition|?
name|two
operator|.
name|buf
else|:
literal|""
argument_list|,
name|two
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
block|}
name|strbuf_reset
argument_list|(
operator|&
name|one
argument_list|)
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|two
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|hunk
operator|==
name|RR_SIDE_1
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|one
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|hunk
operator|==
name|RR_ORIGINAL
condition|)
empty_stmt|;
comment|/* discard */
elseif|else
if|if
condition|(
name|hunk
operator|==
name|RR_SIDE_2
condition|)
name|strbuf_addstr
argument_list|(
operator|&
name|two
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|out
condition|)
name|ferr_puts
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
name|out
argument_list|,
operator|&
name|wrerror
argument_list|)
expr_stmt|;
continue|continue;
name|bad
label|:
name|hunk
operator|=
literal|99
expr_stmt|;
comment|/* force error exit */
break|break;
block|}
name|strbuf_release
argument_list|(
operator|&
name|one
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|two
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|f
argument_list|)
expr_stmt|;
if|if
condition|(
name|wrerror
condition|)
name|error
argument_list|(
literal|"There were errors while writing %s (%s)"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|wrerror
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|out
operator|&&
name|fclose
argument_list|(
name|out
argument_list|)
condition|)
name|wrerror
operator|=
name|error
argument_list|(
literal|"Failed to flush %s: %s"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|sha1
condition|)
name|git_SHA1_Final
argument_list|(
name|sha1
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
if|if
condition|(
name|hunk
operator|!=
name|RR_CONTEXT
condition|)
block|{
if|if
condition|(
name|output
condition|)
name|unlink_or_warn
argument_list|(
name|output
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"Could not parse conflict hunks in %s"
argument_list|,
name|path
argument_list|)
return|;
block|}
if|if
condition|(
name|wrerror
condition|)
return|return
operator|-
literal|1
return|;
return|return
name|hunk_no
return|;
block|}
end_function

begin_function
DECL|function|find_conflict
specifier|static
name|int
name|find_conflict
parameter_list|(
name|struct
name|string_list
modifier|*
name|conflict
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
name|read_cache
argument_list|()
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"Could not read index"
argument_list|)
return|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|+
literal|1
operator|<
name|active_nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|e2
init|=
name|active_cache
index|[
name|i
index|]
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|e3
init|=
name|active_cache
index|[
name|i
operator|+
literal|1
index|]
decl_stmt|;
if|if
condition|(
name|ce_stage
argument_list|(
name|e2
argument_list|)
operator|==
literal|2
operator|&&
name|ce_stage
argument_list|(
name|e3
argument_list|)
operator|==
literal|3
operator|&&
name|ce_same_name
argument_list|(
name|e2
argument_list|,
name|e3
argument_list|)
operator|&&
name|S_ISREG
argument_list|(
name|e2
operator|->
name|ce_mode
argument_list|)
operator|&&
name|S_ISREG
argument_list|(
name|e3
operator|->
name|ce_mode
argument_list|)
condition|)
block|{
name|string_list_insert
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|e2
operator|->
name|name
argument_list|,
name|conflict
argument_list|)
expr_stmt|;
name|i
operator|++
expr_stmt|;
comment|/* skip over both #2 and #3 */
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|merge
specifier|static
name|int
name|merge
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|int
name|ret
decl_stmt|;
name|mmfile_t
name|cur
decl_stmt|,
name|base
decl_stmt|,
name|other
decl_stmt|;
name|mmbuffer_t
name|result
init|=
block|{
name|NULL
block|,
literal|0
block|}
decl_stmt|;
name|xpparam_t
name|xpp
init|=
block|{
name|XDF_NEED_MINIMAL
block|}
decl_stmt|;
if|if
condition|(
name|handle_file
argument_list|(
name|path
argument_list|,
name|NULL
argument_list|,
name|rerere_path
argument_list|(
name|name
argument_list|,
literal|"thisimage"
argument_list|)
argument_list|)
operator|<
literal|0
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|read_mmfile
argument_list|(
operator|&
name|cur
argument_list|,
name|rerere_path
argument_list|(
name|name
argument_list|,
literal|"thisimage"
argument_list|)
argument_list|)
operator|||
name|read_mmfile
argument_list|(
operator|&
name|base
argument_list|,
name|rerere_path
argument_list|(
name|name
argument_list|,
literal|"preimage"
argument_list|)
argument_list|)
operator|||
name|read_mmfile
argument_list|(
operator|&
name|other
argument_list|,
name|rerere_path
argument_list|(
name|name
argument_list|,
literal|"postimage"
argument_list|)
argument_list|)
condition|)
return|return
literal|1
return|;
name|ret
operator|=
name|xdl_merge
argument_list|(
operator|&
name|base
argument_list|,
operator|&
name|cur
argument_list|,
literal|""
argument_list|,
operator|&
name|other
argument_list|,
literal|""
argument_list|,
operator|&
name|xpp
argument_list|,
name|XDL_MERGE_ZEALOUS
argument_list|,
operator|&
name|result
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ret
condition|)
block|{
name|FILE
modifier|*
name|f
init|=
name|fopen
argument_list|(
name|path
argument_list|,
literal|"w"
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|f
condition|)
return|return
name|error
argument_list|(
literal|"Could not open %s: %s"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
if|if
condition|(
name|fwrite
argument_list|(
name|result
operator|.
name|ptr
argument_list|,
name|result
operator|.
name|size
argument_list|,
literal|1
argument_list|,
name|f
argument_list|)
operator|!=
literal|1
condition|)
name|error
argument_list|(
literal|"Could not write %s: %s"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|fclose
argument_list|(
name|f
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"Writing %s failed: %s"
argument_list|,
name|path
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
block|}
name|free
argument_list|(
name|cur
operator|.
name|ptr
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|base
operator|.
name|ptr
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|other
operator|.
name|ptr
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|result
operator|.
name|ptr
argument_list|)
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|index_lock
specifier|static
name|struct
name|lock_file
name|index_lock
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|update_paths
specifier|static
name|int
name|update_paths
parameter_list|(
name|struct
name|string_list
modifier|*
name|update
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|fd
init|=
name|hold_locked_index
argument_list|(
operator|&
name|index_lock
argument_list|,
literal|0
argument_list|)
decl_stmt|;
name|int
name|status
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|update
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|string_list_item
modifier|*
name|item
init|=
operator|&
name|update
operator|->
name|items
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|add_file_to_cache
argument_list|(
name|item
operator|->
name|string
argument_list|,
name|ADD_CACHE_IGNORE_ERRORS
argument_list|)
condition|)
name|status
operator|=
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|status
operator|&&
name|active_cache_changed
condition|)
block|{
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
operator|&
name|index_lock
argument_list|)
condition|)
name|die
argument_list|(
literal|"Unable to write new index file"
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|fd
operator|>=
literal|0
condition|)
name|rollback_lock_file
argument_list|(
operator|&
name|index_lock
argument_list|)
expr_stmt|;
return|return
name|status
return|;
block|}
end_function

begin_function
DECL|function|do_plain_rerere
specifier|static
name|int
name|do_plain_rerere
parameter_list|(
name|struct
name|string_list
modifier|*
name|rr
parameter_list|,
name|int
name|fd
parameter_list|)
block|{
name|struct
name|string_list
name|conflict
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|1
block|}
decl_stmt|;
name|struct
name|string_list
name|update
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|1
block|}
decl_stmt|;
name|int
name|i
decl_stmt|;
name|find_conflict
argument_list|(
operator|&
name|conflict
argument_list|)
expr_stmt|;
comment|/* 	 * MERGE_RR records paths with conflicts immediately after merge 	 * failed.  Some of the conflicted paths might have been hand resolved 	 * in the working tree since then, but the initial run would catch all 	 * and register their preimages. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|conflict
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
name|conflict
operator|.
name|items
index|[
name|i
index|]
operator|.
name|string
decl_stmt|;
if|if
condition|(
operator|!
name|string_list_has_string
argument_list|(
name|rr
argument_list|,
name|path
argument_list|)
condition|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|hex
decl_stmt|;
name|int
name|ret
decl_stmt|;
name|ret
operator|=
name|handle_file
argument_list|(
name|path
argument_list|,
name|sha1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|<
literal|1
condition|)
continue|continue;
name|hex
operator|=
name|xstrdup
argument_list|(
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|string_list_insert
argument_list|(
name|path
argument_list|,
name|rr
argument_list|)
operator|->
name|util
operator|=
name|hex
expr_stmt|;
if|if
condition|(
name|mkdir
argument_list|(
name|git_path
argument_list|(
literal|"rr-cache/%s"
argument_list|,
name|hex
argument_list|)
argument_list|,
literal|0755
argument_list|)
condition|)
continue|continue;
name|handle_file
argument_list|(
name|path
argument_list|,
name|NULL
argument_list|,
name|rerere_path
argument_list|(
name|hex
argument_list|,
literal|"preimage"
argument_list|)
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Recorded preimage for '%s'\n"
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
block|}
comment|/* 	 * Now some of the paths that had conflicts earlier might have been 	 * hand resolved.  Others may be similar to a conflict already that 	 * was resolved before. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|rr
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|int
name|ret
decl_stmt|;
specifier|const
name|char
modifier|*
name|path
init|=
name|rr
operator|->
name|items
index|[
name|i
index|]
operator|.
name|string
decl_stmt|;
specifier|const
name|char
modifier|*
name|name
init|=
operator|(
specifier|const
name|char
operator|*
operator|)
name|rr
operator|->
name|items
index|[
name|i
index|]
operator|.
name|util
decl_stmt|;
if|if
condition|(
name|has_rerere_resolution
argument_list|(
name|name
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|merge
argument_list|(
name|name
argument_list|,
name|path
argument_list|)
condition|)
block|{
if|if
condition|(
name|rerere_autoupdate
condition|)
name|string_list_insert
argument_list|(
name|path
argument_list|,
operator|&
name|update
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s '%s' using previous resolution.\n"
argument_list|,
name|rerere_autoupdate
condition|?
literal|"Staged"
else|:
literal|"Resolved"
argument_list|,
name|path
argument_list|)
expr_stmt|;
goto|goto
name|mark_resolved
goto|;
block|}
block|}
comment|/* Let's see if we have resolved it. */
name|ret
operator|=
name|handle_file
argument_list|(
name|path
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
continue|continue;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Recorded resolution for '%s'.\n"
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|copy_file
argument_list|(
name|rerere_path
argument_list|(
name|name
argument_list|,
literal|"postimage"
argument_list|)
argument_list|,
name|path
argument_list|,
literal|0666
argument_list|)
expr_stmt|;
name|mark_resolved
label|:
name|rr
operator|->
name|items
index|[
name|i
index|]
operator|.
name|util
operator|=
name|NULL
expr_stmt|;
block|}
if|if
condition|(
name|update
operator|.
name|nr
condition|)
name|update_paths
argument_list|(
operator|&
name|update
argument_list|)
expr_stmt|;
return|return
name|write_rr
argument_list|(
name|rr
argument_list|,
name|fd
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|git_rerere_config
specifier|static
name|int
name|git_rerere_config
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
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"rerere.enabled"
argument_list|)
condition|)
name|rerere_enabled
operator|=
name|git_config_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"rerere.autoupdate"
argument_list|)
condition|)
name|rerere_autoupdate
operator|=
name|git_config_bool
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
else|else
return|return
name|git_default_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|,
name|cb
argument_list|)
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|is_rerere_enabled
specifier|static
name|int
name|is_rerere_enabled
parameter_list|(
name|void
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|rr_cache
decl_stmt|;
name|int
name|rr_cache_exists
decl_stmt|;
if|if
condition|(
operator|!
name|rerere_enabled
condition|)
return|return
literal|0
return|;
name|rr_cache
operator|=
name|git_path
argument_list|(
literal|"rr-cache"
argument_list|)
expr_stmt|;
name|rr_cache_exists
operator|=
name|is_directory
argument_list|(
name|rr_cache
argument_list|)
expr_stmt|;
if|if
condition|(
name|rerere_enabled
operator|<
literal|0
condition|)
return|return
name|rr_cache_exists
return|;
if|if
condition|(
operator|!
name|rr_cache_exists
operator|&&
operator|(
name|mkdir
argument_list|(
name|rr_cache
argument_list|,
literal|0777
argument_list|)
operator|||
name|adjust_shared_perm
argument_list|(
name|rr_cache
argument_list|)
operator|)
condition|)
name|die
argument_list|(
literal|"Could not create directory %s"
argument_list|,
name|rr_cache
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|setup_rerere
name|int
name|setup_rerere
parameter_list|(
name|struct
name|string_list
modifier|*
name|merge_rr
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|git_config
argument_list|(
name|git_rerere_config
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_rerere_enabled
argument_list|()
condition|)
return|return
operator|-
literal|1
return|;
name|merge_rr_path
operator|=
name|git_pathdup
argument_list|(
literal|"MERGE_RR"
argument_list|)
expr_stmt|;
name|fd
operator|=
name|hold_lock_file_for_update
argument_list|(
operator|&
name|write_lock
argument_list|,
name|merge_rr_path
argument_list|,
name|LOCK_DIE_ON_ERROR
argument_list|)
expr_stmt|;
name|read_rr
argument_list|(
name|merge_rr
argument_list|)
expr_stmt|;
return|return
name|fd
return|;
block|}
end_function

begin_function
DECL|function|rerere
name|int
name|rerere
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|string_list
name|merge_rr
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|1
block|}
decl_stmt|;
name|int
name|fd
decl_stmt|;
name|fd
operator|=
name|setup_rerere
argument_list|(
operator|&
name|merge_rr
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|<
literal|0
condition|)
return|return
literal|0
return|;
return|return
name|do_plain_rerere
argument_list|(
operator|&
name|merge_rr
argument_list|,
name|fd
argument_list|)
return|;
block|}
end_function

end_unit

