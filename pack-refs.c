begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
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
file|"tag.h"
end_include

begin_include
include|#
directive|include
file|"pack-refs.h"
end_include

begin_struct
DECL|struct|ref_to_prune
struct|struct
name|ref_to_prune
block|{
DECL|member|next
name|struct
name|ref_to_prune
modifier|*
name|next
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|name
name|char
name|name
index|[
name|FLEX_ARRAY
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|pack_refs_cb_data
struct|struct
name|pack_refs_cb_data
block|{
DECL|member|flags
name|unsigned
name|int
name|flags
decl_stmt|;
DECL|member|ref_to_prune
name|struct
name|ref_to_prune
modifier|*
name|ref_to_prune
decl_stmt|;
DECL|member|refs_file
name|FILE
modifier|*
name|refs_file
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|do_not_prune
specifier|static
name|int
name|do_not_prune
parameter_list|(
name|int
name|flags
parameter_list|)
block|{
comment|/* If it is already packed or if it is a symref, 	 * do not prune it. 	 */
return|return
operator|(
name|flags
operator|&
operator|(
name|REF_ISSYMREF
operator||
name|REF_ISPACKED
operator|)
operator|)
return|;
block|}
end_function

begin_function
DECL|function|handle_one_ref
specifier|static
name|int
name|handle_one_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|flags
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
block|{
name|struct
name|pack_refs_cb_data
modifier|*
name|cb
init|=
name|cb_data
decl_stmt|;
name|int
name|is_tag_ref
decl_stmt|;
comment|/* Do not pack the symbolic refs */
if|if
condition|(
operator|(
name|flags
operator|&
name|REF_ISSYMREF
operator|)
condition|)
return|return
literal|0
return|;
name|is_tag_ref
operator|=
operator|!
name|prefixcmp
argument_list|(
name|path
argument_list|,
literal|"refs/tags/"
argument_list|)
expr_stmt|;
comment|/* ALWAYS pack refs that were already packed or are tags */
if|if
condition|(
operator|!
operator|(
name|cb
operator|->
name|flags
operator|&
name|PACK_REFS_ALL
operator|)
operator|&&
operator|!
name|is_tag_ref
operator|&&
operator|!
operator|(
name|flags
operator|&
name|REF_ISPACKED
operator|)
condition|)
return|return
literal|0
return|;
name|fprintf
argument_list|(
name|cb
operator|->
name|refs_file
argument_list|,
literal|"%s %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|if
condition|(
name|is_tag_ref
condition|)
block|{
name|struct
name|object
modifier|*
name|o
init|=
name|parse_object
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
name|o
operator|->
name|type
operator|==
name|OBJ_TAG
condition|)
block|{
name|o
operator|=
name|deref_tag
argument_list|(
name|o
argument_list|,
name|path
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|o
condition|)
name|fprintf
argument_list|(
name|cb
operator|->
name|refs_file
argument_list|,
literal|"^%s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|o
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|(
name|cb
operator|->
name|flags
operator|&
name|PACK_REFS_PRUNE
operator|)
operator|&&
operator|!
name|do_not_prune
argument_list|(
name|flags
argument_list|)
condition|)
block|{
name|int
name|namelen
init|=
name|strlen
argument_list|(
name|path
argument_list|)
operator|+
literal|1
decl_stmt|;
name|struct
name|ref_to_prune
modifier|*
name|n
init|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|n
argument_list|)
operator|+
name|namelen
argument_list|)
decl_stmt|;
name|hashcpy
argument_list|(
name|n
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|n
operator|->
name|name
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|n
operator|->
name|next
operator|=
name|cb
operator|->
name|ref_to_prune
expr_stmt|;
name|cb
operator|->
name|ref_to_prune
operator|=
name|n
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * Remove empty parents, but spare refs/ and immediate subdirs.  * Note: munges *name.  */
end_comment

begin_function
DECL|function|try_remove_empty_parents
specifier|static
name|void
name|try_remove_empty_parents
parameter_list|(
name|char
modifier|*
name|name
parameter_list|)
block|{
name|char
modifier|*
name|p
decl_stmt|,
modifier|*
name|q
decl_stmt|;
name|int
name|i
decl_stmt|;
name|p
operator|=
name|name
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|2
condition|;
name|i
operator|++
control|)
block|{
comment|/* refs/{heads,tags,...}/ */
while|while
condition|(
operator|*
name|p
operator|&&
operator|*
name|p
operator|!=
literal|'/'
condition|)
name|p
operator|++
expr_stmt|;
comment|/* tolerate duplicate slashes; see check_refname_format() */
while|while
condition|(
operator|*
name|p
operator|==
literal|'/'
condition|)
name|p
operator|++
expr_stmt|;
block|}
for|for
control|(
name|q
operator|=
name|p
init|;
operator|*
name|q
condition|;
name|q
operator|++
control|)
empty_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
while|while
condition|(
name|q
operator|>
name|p
operator|&&
operator|*
name|q
operator|!=
literal|'/'
condition|)
name|q
operator|--
expr_stmt|;
while|while
condition|(
name|q
operator|>
name|p
operator|&&
operator|*
operator|(
name|q
operator|-
literal|1
operator|)
operator|==
literal|'/'
condition|)
name|q
operator|--
expr_stmt|;
if|if
condition|(
name|q
operator|==
name|p
condition|)
break|break;
operator|*
name|q
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
name|rmdir
argument_list|(
name|git_path
argument_list|(
literal|"%s"
argument_list|,
name|name
argument_list|)
argument_list|)
condition|)
break|break;
block|}
block|}
end_function

begin_comment
comment|/* make sure nobody touched the ref, and unlink */
end_comment

begin_function
DECL|function|prune_ref
specifier|static
name|void
name|prune_ref
parameter_list|(
name|struct
name|ref_to_prune
modifier|*
name|r
parameter_list|)
block|{
name|struct
name|ref_lock
modifier|*
name|lock
init|=
name|lock_ref_sha1
argument_list|(
name|r
operator|->
name|name
operator|+
literal|5
argument_list|,
name|r
operator|->
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
name|lock
condition|)
block|{
name|unlink_or_warn
argument_list|(
name|git_path
argument_list|(
literal|"%s"
argument_list|,
name|r
operator|->
name|name
argument_list|)
argument_list|)
expr_stmt|;
name|unlock_ref
argument_list|(
name|lock
argument_list|)
expr_stmt|;
name|try_remove_empty_parents
argument_list|(
name|r
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|prune_refs
specifier|static
name|void
name|prune_refs
parameter_list|(
name|struct
name|ref_to_prune
modifier|*
name|r
parameter_list|)
block|{
while|while
condition|(
name|r
condition|)
block|{
name|prune_ref
argument_list|(
name|r
argument_list|)
expr_stmt|;
name|r
operator|=
name|r
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_decl_stmt
DECL|variable|packed
specifier|static
name|struct
name|lock_file
name|packed
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|pack_refs
name|int
name|pack_refs
parameter_list|(
name|unsigned
name|int
name|flags
parameter_list|)
block|{
name|int
name|fd
decl_stmt|;
name|struct
name|pack_refs_cb_data
name|cbdata
decl_stmt|;
name|memset
argument_list|(
operator|&
name|cbdata
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|cbdata
argument_list|)
argument_list|)
expr_stmt|;
name|cbdata
operator|.
name|flags
operator|=
name|flags
expr_stmt|;
name|fd
operator|=
name|hold_lock_file_for_update
argument_list|(
operator|&
name|packed
argument_list|,
name|git_path
argument_list|(
literal|"packed-refs"
argument_list|)
argument_list|,
name|LOCK_DIE_ON_ERROR
argument_list|)
expr_stmt|;
name|cbdata
operator|.
name|refs_file
operator|=
name|fdopen
argument_list|(
name|fd
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cbdata
operator|.
name|refs_file
condition|)
name|die_errno
argument_list|(
literal|"unable to create ref-pack file structure"
argument_list|)
expr_stmt|;
comment|/* perhaps other traits later as well */
name|fprintf
argument_list|(
name|cbdata
operator|.
name|refs_file
argument_list|,
literal|"# pack-refs with: peeled \n"
argument_list|)
expr_stmt|;
name|for_each_ref
argument_list|(
name|handle_one_ref
argument_list|,
operator|&
name|cbdata
argument_list|)
expr_stmt|;
if|if
condition|(
name|ferror
argument_list|(
name|cbdata
operator|.
name|refs_file
argument_list|)
condition|)
name|die
argument_list|(
literal|"failed to write ref-pack file"
argument_list|)
expr_stmt|;
if|if
condition|(
name|fflush
argument_list|(
name|cbdata
operator|.
name|refs_file
argument_list|)
operator|||
name|fsync
argument_list|(
name|fd
argument_list|)
operator|||
name|fclose
argument_list|(
name|cbdata
operator|.
name|refs_file
argument_list|)
condition|)
name|die_errno
argument_list|(
literal|"failed to write ref-pack file"
argument_list|)
expr_stmt|;
comment|/* 	 * Since the lock file was fdopen()'ed and then fclose()'ed above, 	 * assign -1 to the lock file descriptor so that commit_lock_file() 	 * won't try to close() it. 	 */
name|packed
operator|.
name|fd
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|commit_lock_file
argument_list|(
operator|&
name|packed
argument_list|)
operator|<
literal|0
condition|)
name|die_errno
argument_list|(
literal|"unable to overwrite old ref-pack file"
argument_list|)
expr_stmt|;
if|if
condition|(
name|cbdata
operator|.
name|flags
operator|&
name|PACK_REFS_PRUNE
condition|)
name|prune_refs
argument_list|(
name|cbdata
operator|.
name|ref_to_prune
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

