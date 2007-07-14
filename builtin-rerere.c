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
file|"path-list.h"
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

begin_include
include|#
directive|include
file|<time.h>
end_include

begin_decl_stmt
DECL|variable|git_rerere_usage
specifier|static
specifier|const
name|char
name|git_rerere_usage
index|[]
init|=
literal|"git-rerere [clear | status | diff | gc]"
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* these values are days */
end_comment

begin_decl_stmt
DECL|variable|cutoff_noresolve
specifier|static
name|int
name|cutoff_noresolve
init|=
literal|15
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cutoff_resolve
specifier|static
name|int
name|cutoff_resolve
init|=
literal|60
decl_stmt|;
end_decl_stmt

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

begin_decl_stmt
DECL|variable|merge_rr_path
specifier|static
name|char
modifier|*
name|merge_rr_path
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|rr_path
specifier|static
specifier|const
name|char
modifier|*
name|rr_path
parameter_list|(
specifier|const
name|char
modifier|*
name|name
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
name|name
argument_list|,
name|file
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
name|path_list
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
name|path_list_insert
argument_list|(
name|buf
argument_list|,
name|rr
argument_list|)
operator|->
name|util
operator|=
name|xstrdup
argument_list|(
name|name
argument_list|)
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
name|path_list
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
init|=
name|rr
operator|->
name|items
index|[
name|i
index|]
operator|.
name|path
decl_stmt|;
name|int
name|length
init|=
name|strlen
argument_list|(
name|path
argument_list|)
operator|+
literal|1
decl_stmt|;
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
name|write_in_full
argument_list|(
name|out_fd
argument_list|,
literal|"\t"
argument_list|,
literal|1
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
name|close
argument_list|(
name|out_fd
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
name|commit_lock_file
argument_list|(
operator|&
name|write_lock
argument_list|)
return|;
block|}
end_function

begin_struct
DECL|struct|buffer
struct|struct
name|buffer
block|{
DECL|member|ptr
name|char
modifier|*
name|ptr
decl_stmt|;
DECL|member|nr
DECL|member|alloc
name|int
name|nr
decl_stmt|,
name|alloc
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|append_line
specifier|static
name|void
name|append_line
parameter_list|(
name|struct
name|buffer
modifier|*
name|buffer
parameter_list|,
specifier|const
name|char
modifier|*
name|line
parameter_list|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|line
argument_list|)
decl_stmt|;
if|if
condition|(
name|buffer
operator|->
name|nr
operator|+
name|len
operator|>
name|buffer
operator|->
name|alloc
condition|)
block|{
name|buffer
operator|->
name|alloc
operator|=
name|alloc_nr
argument_list|(
name|buffer
operator|->
name|nr
operator|+
name|len
argument_list|)
expr_stmt|;
name|buffer
operator|->
name|ptr
operator|=
name|xrealloc
argument_list|(
name|buffer
operator|->
name|ptr
argument_list|,
name|buffer
operator|->
name|alloc
argument_list|)
expr_stmt|;
block|}
name|memcpy
argument_list|(
name|buffer
operator|->
name|ptr
operator|+
name|buffer
operator|->
name|nr
argument_list|,
name|line
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|buffer
operator|->
name|nr
operator|+=
name|len
expr_stmt|;
block|}
end_function

begin_function
DECL|function|clear_buffer
specifier|static
name|void
name|clear_buffer
parameter_list|(
name|struct
name|buffer
modifier|*
name|buffer
parameter_list|)
block|{
name|free
argument_list|(
name|buffer
operator|->
name|ptr
argument_list|)
expr_stmt|;
name|buffer
operator|->
name|ptr
operator|=
name|NULL
expr_stmt|;
name|buffer
operator|->
name|nr
operator|=
name|buffer
operator|->
name|alloc
operator|=
literal|0
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
name|SHA_CTX
name|ctx
decl_stmt|;
name|char
name|buf
index|[
literal|1024
index|]
decl_stmt|;
name|int
name|hunk
init|=
literal|0
decl_stmt|,
name|hunk_no
init|=
literal|0
decl_stmt|;
name|struct
name|buffer
name|minus
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|,
name|plus
init|=
block|{
name|NULL
block|,
literal|0
block|,
literal|0
block|}
decl_stmt|;
name|struct
name|buffer
modifier|*
name|one
init|=
operator|&
name|minus
decl_stmt|,
modifier|*
name|two
init|=
operator|&
name|plus
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
else|else
name|out
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|sha1
condition|)
name|SHA1_Init
argument_list|(
operator|&
name|ctx
argument_list|)
expr_stmt|;
while|while
condition|(
name|fgets
argument_list|(
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
name|f
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
argument_list|,
literal|"<<<<<<< "
argument_list|)
condition|)
name|hunk
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
argument_list|,
literal|"======="
argument_list|)
condition|)
name|hunk
operator|=
literal|2
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|buf
argument_list|,
literal|">>>>>>> "
argument_list|)
condition|)
block|{
name|int
name|one_is_longer
init|=
operator|(
name|one
operator|->
name|nr
operator|>
name|two
operator|->
name|nr
operator|)
decl_stmt|;
name|int
name|common_len
init|=
name|one_is_longer
condition|?
name|two
operator|->
name|nr
else|:
name|one
operator|->
name|nr
decl_stmt|;
name|int
name|cmp
init|=
name|memcmp
argument_list|(
name|one
operator|->
name|ptr
argument_list|,
name|two
operator|->
name|ptr
argument_list|,
name|common_len
argument_list|)
decl_stmt|;
name|hunk_no
operator|++
expr_stmt|;
name|hunk
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|(
name|cmp
operator|>
literal|0
operator|)
operator|||
operator|(
operator|(
name|cmp
operator|==
literal|0
operator|)
operator|&&
name|one_is_longer
operator|)
condition|)
block|{
name|struct
name|buffer
modifier|*
name|swap
init|=
name|one
decl_stmt|;
name|one
operator|=
name|two
expr_stmt|;
name|two
operator|=
name|swap
expr_stmt|;
block|}
if|if
condition|(
name|out
condition|)
block|{
name|fputs
argument_list|(
literal|"<<<<<<<\n"
argument_list|,
name|out
argument_list|)
expr_stmt|;
name|fwrite
argument_list|(
name|one
operator|->
name|ptr
argument_list|,
name|one
operator|->
name|nr
argument_list|,
literal|1
argument_list|,
name|out
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
literal|"=======\n"
argument_list|,
name|out
argument_list|)
expr_stmt|;
name|fwrite
argument_list|(
name|two
operator|->
name|ptr
argument_list|,
name|two
operator|->
name|nr
argument_list|,
literal|1
argument_list|,
name|out
argument_list|)
expr_stmt|;
name|fputs
argument_list|(
literal|">>>>>>>\n"
argument_list|,
name|out
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|sha1
condition|)
block|{
name|SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
name|one
operator|->
name|ptr
argument_list|,
name|one
operator|->
name|nr
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
literal|"\0"
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
name|two
operator|->
name|ptr
argument_list|,
name|two
operator|->
name|nr
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|ctx
argument_list|,
literal|"\0"
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
name|clear_buffer
argument_list|(
name|one
argument_list|)
expr_stmt|;
name|clear_buffer
argument_list|(
name|two
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|hunk
operator|==
literal|1
condition|)
name|append_line
argument_list|(
name|one
argument_list|,
name|buf
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|hunk
operator|==
literal|2
condition|)
name|append_line
argument_list|(
name|two
argument_list|,
name|buf
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|out
condition|)
name|fputs
argument_list|(
name|buf
argument_list|,
name|out
argument_list|)
expr_stmt|;
block|}
name|fclose
argument_list|(
name|f
argument_list|)
expr_stmt|;
if|if
condition|(
name|out
condition|)
name|fclose
argument_list|(
name|out
argument_list|)
expr_stmt|;
if|if
condition|(
name|sha1
condition|)
name|SHA1_Final
argument_list|(
name|sha1
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
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
name|path_list
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
name|ntohl
argument_list|(
name|e2
operator|->
name|ce_mode
argument_list|)
argument_list|)
operator|&&
name|S_ISREG
argument_list|(
name|ntohl
argument_list|(
name|e3
operator|->
name|ce_mode
argument_list|)
argument_list|)
condition|)
block|{
name|path_list_insert
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
name|rr_path
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
name|rr_path
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
name|rr_path
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
name|rr_path
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
literal|"Could not write to %s"
argument_list|,
name|path
argument_list|)
return|;
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
expr_stmt|;
name|fclose
argument_list|(
name|f
argument_list|)
expr_stmt|;
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

begin_function
DECL|function|unlink_rr_item
specifier|static
name|void
name|unlink_rr_item
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
name|unlink
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"thisimage"
argument_list|)
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"preimage"
argument_list|)
argument_list|)
expr_stmt|;
name|unlink
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"postimage"
argument_list|)
argument_list|)
expr_stmt|;
name|rmdir
argument_list|(
name|git_path
argument_list|(
literal|"rr-cache/%s"
argument_list|,
name|name
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|garbage_collect
specifier|static
name|void
name|garbage_collect
parameter_list|(
name|struct
name|path_list
modifier|*
name|rr
parameter_list|)
block|{
name|struct
name|path_list
name|to_remove
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
name|char
name|buf
index|[
literal|1024
index|]
decl_stmt|;
name|DIR
modifier|*
name|dir
decl_stmt|;
name|struct
name|dirent
modifier|*
name|e
decl_stmt|;
name|int
name|len
decl_stmt|,
name|i
decl_stmt|,
name|cutoff
decl_stmt|;
name|time_t
name|now
init|=
name|time
argument_list|(
name|NULL
argument_list|)
decl_stmt|,
name|then
decl_stmt|;
name|strlcpy
argument_list|(
name|buf
argument_list|,
name|git_path
argument_list|(
literal|"rr-cache"
argument_list|)
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|)
expr_stmt|;
name|len
operator|=
name|strlen
argument_list|(
name|buf
argument_list|)
expr_stmt|;
name|dir
operator|=
name|opendir
argument_list|(
name|buf
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|buf
operator|+
name|len
operator|++
argument_list|,
literal|"/"
argument_list|)
expr_stmt|;
while|while
condition|(
operator|(
name|e
operator|=
name|readdir
argument_list|(
name|dir
argument_list|)
operator|)
condition|)
block|{
specifier|const
name|char
modifier|*
name|name
init|=
name|e
operator|->
name|d_name
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
if|if
condition|(
name|name
index|[
literal|0
index|]
operator|==
literal|'.'
operator|&&
operator|(
name|name
index|[
literal|1
index|]
operator|==
literal|'\0'
operator|||
operator|(
name|name
index|[
literal|1
index|]
operator|==
literal|'.'
operator|&&
name|name
index|[
literal|2
index|]
operator|==
literal|'\0'
operator|)
operator|)
condition|)
continue|continue;
name|i
operator|=
name|snprintf
argument_list|(
name|buf
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
operator|-
name|len
argument_list|,
literal|"%s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
name|strlcpy
argument_list|(
name|buf
operator|+
name|len
operator|+
name|i
argument_list|,
literal|"/preimage"
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
operator|-
name|len
operator|-
name|i
argument_list|)
expr_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|buf
argument_list|,
operator|&
name|st
argument_list|)
condition|)
continue|continue;
name|then
operator|=
name|st
operator|.
name|st_mtime
expr_stmt|;
name|strlcpy
argument_list|(
name|buf
operator|+
name|len
operator|+
name|i
argument_list|,
literal|"/postimage"
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
operator|-
name|len
operator|-
name|i
argument_list|)
expr_stmt|;
name|cutoff
operator|=
name|stat
argument_list|(
name|buf
argument_list|,
operator|&
name|st
argument_list|)
condition|?
name|cutoff_noresolve
else|:
name|cutoff_resolve
expr_stmt|;
if|if
condition|(
name|then
operator|<
name|now
operator|-
name|cutoff
operator|*
literal|86400
condition|)
block|{
name|buf
index|[
name|len
operator|+
name|i
index|]
operator|=
literal|'\0'
expr_stmt|;
name|path_list_insert
argument_list|(
name|xstrdup
argument_list|(
name|name
argument_list|)
argument_list|,
operator|&
name|to_remove
argument_list|)
expr_stmt|;
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
name|to_remove
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|unlink_rr_item
argument_list|(
name|to_remove
operator|.
name|items
index|[
name|i
index|]
operator|.
name|path
argument_list|)
expr_stmt|;
name|path_list_clear
argument_list|(
operator|&
name|to_remove
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|outf
specifier|static
name|int
name|outf
parameter_list|(
name|void
modifier|*
name|dummy
parameter_list|,
name|mmbuffer_t
modifier|*
name|ptr
parameter_list|,
name|int
name|nbuf
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
name|nbuf
condition|;
name|i
operator|++
control|)
if|if
condition|(
name|write_in_full
argument_list|(
literal|1
argument_list|,
name|ptr
index|[
name|i
index|]
operator|.
name|ptr
argument_list|,
name|ptr
index|[
name|i
index|]
operator|.
name|size
argument_list|)
operator|!=
name|ptr
index|[
name|i
index|]
operator|.
name|size
condition|)
return|return
operator|-
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|diff_two
specifier|static
name|int
name|diff_two
parameter_list|(
specifier|const
name|char
modifier|*
name|file1
parameter_list|,
specifier|const
name|char
modifier|*
name|label1
parameter_list|,
specifier|const
name|char
modifier|*
name|file2
parameter_list|,
specifier|const
name|char
modifier|*
name|label2
parameter_list|)
block|{
name|xpparam_t
name|xpp
decl_stmt|;
name|xdemitconf_t
name|xecfg
decl_stmt|;
name|xdemitcb_t
name|ecb
decl_stmt|;
name|mmfile_t
name|minus
decl_stmt|,
name|plus
decl_stmt|;
if|if
condition|(
name|read_mmfile
argument_list|(
operator|&
name|minus
argument_list|,
name|file1
argument_list|)
operator|||
name|read_mmfile
argument_list|(
operator|&
name|plus
argument_list|,
name|file2
argument_list|)
condition|)
return|return
literal|1
return|;
name|printf
argument_list|(
literal|"--- a/%s\n+++ b/%s\n"
argument_list|,
name|label1
argument_list|,
name|label2
argument_list|)
expr_stmt|;
name|fflush
argument_list|(
name|stdout
argument_list|)
expr_stmt|;
name|xpp
operator|.
name|flags
operator|=
name|XDF_NEED_MINIMAL
expr_stmt|;
name|memset
argument_list|(
operator|&
name|xecfg
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|xecfg
argument_list|)
argument_list|)
expr_stmt|;
name|xecfg
operator|.
name|ctxlen
operator|=
literal|3
expr_stmt|;
name|ecb
operator|.
name|outf
operator|=
name|outf
expr_stmt|;
name|xdl_diff
argument_list|(
operator|&
name|minus
argument_list|,
operator|&
name|plus
argument_list|,
operator|&
name|xpp
argument_list|,
operator|&
name|xecfg
argument_list|,
operator|&
name|ecb
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|minus
operator|.
name|ptr
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|plus
operator|.
name|ptr
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|copy_file
specifier|static
name|int
name|copy_file
parameter_list|(
specifier|const
name|char
modifier|*
name|src
parameter_list|,
specifier|const
name|char
modifier|*
name|dest
parameter_list|)
block|{
name|FILE
modifier|*
name|in
decl_stmt|,
modifier|*
name|out
decl_stmt|;
name|char
name|buffer
index|[
literal|32768
index|]
decl_stmt|;
name|int
name|count
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|in
operator|=
name|fopen
argument_list|(
name|src
argument_list|,
literal|"r"
argument_list|)
operator|)
condition|)
return|return
name|error
argument_list|(
literal|"Could not open %s"
argument_list|,
name|src
argument_list|)
return|;
if|if
condition|(
operator|!
operator|(
name|out
operator|=
name|fopen
argument_list|(
name|dest
argument_list|,
literal|"w"
argument_list|)
operator|)
condition|)
return|return
name|error
argument_list|(
literal|"Could not open %s"
argument_list|,
name|dest
argument_list|)
return|;
while|while
condition|(
operator|(
name|count
operator|=
name|fread
argument_list|(
name|buffer
argument_list|,
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
argument_list|,
name|in
argument_list|)
operator|)
condition|)
name|fwrite
argument_list|(
name|buffer
argument_list|,
literal|1
argument_list|,
name|count
argument_list|,
name|out
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|in
argument_list|)
expr_stmt|;
name|fclose
argument_list|(
name|out
argument_list|)
expr_stmt|;
return|return
literal|0
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
name|path_list
modifier|*
name|rr
parameter_list|,
name|int
name|fd
parameter_list|)
block|{
name|struct
name|path_list
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
name|path
decl_stmt|;
if|if
condition|(
operator|!
name|path_list_has_path
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
name|path_list_insert
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
empty_stmt|;
name|handle_file
argument_list|(
name|path
argument_list|,
name|NULL
argument_list|,
name|rr_path
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
name|struct
name|stat
name|st
decl_stmt|;
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
name|path
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
operator|!
name|stat
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"preimage"
argument_list|)
argument_list|,
operator|&
name|st
argument_list|)
operator|&&
operator|!
name|stat
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"postimage"
argument_list|)
argument_list|,
operator|&
name|st
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
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Resolved '%s' using "
literal|"previous resolution.\n"
argument_list|,
name|path
argument_list|)
expr_stmt|;
goto|goto
name|tail_optimization
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
name|path
argument_list|,
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"postimage"
argument_list|)
argument_list|)
expr_stmt|;
name|tail_optimization
label|:
if|if
condition|(
name|i
operator|<
name|rr
operator|->
name|nr
operator|-
literal|1
condition|)
name|memmove
argument_list|(
name|rr
operator|->
name|items
operator|+
name|i
argument_list|,
name|rr
operator|->
name|items
operator|+
name|i
operator|+
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
name|rr
operator|->
name|items
index|[
literal|0
index|]
argument_list|)
operator|*
operator|(
name|rr
operator|->
name|nr
operator|-
name|i
operator|-
literal|1
operator|)
argument_list|)
expr_stmt|;
name|rr
operator|->
name|nr
operator|--
expr_stmt|;
name|i
operator|--
expr_stmt|;
block|}
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
parameter_list|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"gc.rerereresolved"
argument_list|)
condition|)
name|cutoff_resolve
operator|=
name|git_config_int
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
literal|"gc.rerereunresolved"
argument_list|)
condition|)
name|cutoff_noresolve
operator|=
name|git_config_int
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
else|else
return|return
name|git_default_config
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
DECL|function|is_rerere_enabled
specifier|static
name|int
name|is_rerere_enabled
parameter_list|(
name|void
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
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
operator|!
name|stat
argument_list|(
name|rr_cache
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
DECL|function|cmd_rerere
name|int
name|cmd_rerere
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
name|struct
name|path_list
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
name|i
decl_stmt|,
name|fd
init|=
operator|-
literal|1
decl_stmt|;
name|git_config
argument_list|(
name|git_rerere_config
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|is_rerere_enabled
argument_list|()
condition|)
return|return
literal|0
return|;
name|merge_rr_path
operator|=
name|xstrdup
argument_list|(
name|git_path
argument_list|(
literal|"rr-cache/MERGE_RR"
argument_list|)
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
literal|1
argument_list|)
expr_stmt|;
name|read_rr
argument_list|(
operator|&
name|merge_rr
argument_list|)
expr_stmt|;
if|if
condition|(
name|argc
operator|<
literal|2
condition|)
return|return
name|do_plain_rerere
argument_list|(
operator|&
name|merge_rr
argument_list|,
name|fd
argument_list|)
return|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"clear"
argument_list|)
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
name|merge_rr
operator|.
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|stat
name|st
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
name|merge_rr
operator|.
name|items
index|[
name|i
index|]
operator|.
name|util
decl_stmt|;
if|if
condition|(
operator|!
name|stat
argument_list|(
name|git_path
argument_list|(
literal|"rr-cache/%s"
argument_list|,
name|name
argument_list|)
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
operator|&&
name|stat
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"postimage"
argument_list|)
argument_list|,
operator|&
name|st
argument_list|)
condition|)
name|unlink_rr_item
argument_list|(
name|name
argument_list|)
expr_stmt|;
block|}
name|unlink
argument_list|(
name|merge_rr_path
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"gc"
argument_list|)
condition|)
name|garbage_collect
argument_list|(
operator|&
name|merge_rr
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"status"
argument_list|)
condition|)
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|merge_rr
operator|.
name|nr
condition|;
name|i
operator|++
control|)
name|printf
argument_list|(
literal|"%s\n"
argument_list|,
name|merge_rr
operator|.
name|items
index|[
name|i
index|]
operator|.
name|path
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
literal|"diff"
argument_list|)
condition|)
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|merge_rr
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
name|merge_rr
operator|.
name|items
index|[
name|i
index|]
operator|.
name|path
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
name|merge_rr
operator|.
name|items
index|[
name|i
index|]
operator|.
name|util
decl_stmt|;
name|diff_two
argument_list|(
name|rr_path
argument_list|(
name|name
argument_list|,
literal|"preimage"
argument_list|)
argument_list|,
name|path
argument_list|,
name|path
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
else|else
name|usage
argument_list|(
name|git_rerere_usage
argument_list|)
expr_stmt|;
name|path_list_clear
argument_list|(
operator|&
name|merge_rr
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

