begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_decl_stmt
DECL|variable|cached_only
specifier|static
name|int
name|cached_only
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|generate_patch
specifier|static
name|int
name|generate_patch
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|line_termination
specifier|static
name|int
name|line_termination
init|=
literal|'\n'
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* A file entry went away or appeared */
end_comment

begin_function
DECL|function|show_file
specifier|static
name|void
name|show_file
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
name|int
name|mode
parameter_list|)
block|{
if|if
condition|(
name|generate_patch
condition|)
name|diff_addremove
argument_list|(
name|prefix
index|[
literal|0
index|]
argument_list|,
name|ntohl
argument_list|(
name|mode
argument_list|)
argument_list|,
name|sha1
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"%s%06o\tblob\t%s\t%s%c"
argument_list|,
name|prefix
argument_list|,
name|ntohl
argument_list|(
name|mode
argument_list|)
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|line_termination
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|get_stat_data
specifier|static
name|int
name|get_stat_data
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|unsigned
name|char
modifier|*
modifier|*
name|sha1p
parameter_list|,
name|unsigned
name|int
modifier|*
name|modep
parameter_list|)
block|{
name|unsigned
name|char
modifier|*
name|sha1
init|=
name|ce
operator|->
name|sha1
decl_stmt|;
name|unsigned
name|int
name|mode
init|=
name|ce
operator|->
name|ce_mode
decl_stmt|;
if|if
condition|(
operator|!
name|cached_only
condition|)
block|{
specifier|static
name|unsigned
name|char
name|no_sha1
index|[
literal|20
index|]
decl_stmt|;
name|int
name|changed
decl_stmt|;
name|struct
name|stat
name|st
decl_stmt|;
if|if
condition|(
name|stat
argument_list|(
name|ce
operator|->
name|name
argument_list|,
operator|&
name|st
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
name|changed
operator|=
name|cache_match_stat
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
if|if
condition|(
name|changed
condition|)
block|{
name|mode
operator|=
name|create_ce_mode
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
expr_stmt|;
name|sha1
operator|=
name|no_sha1
expr_stmt|;
block|}
block|}
operator|*
name|sha1p
operator|=
name|sha1
expr_stmt|;
operator|*
name|modep
operator|=
name|mode
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|show_new_file
specifier|static
name|void
name|show_new_file
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|new
parameter_list|)
block|{
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|unsigned
name|int
name|mode
decl_stmt|;
comment|/* New file in the index: it might actually be different in the working copy */
if|if
condition|(
name|get_stat_data
argument_list|(
name|new
argument_list|,
operator|&
name|sha1
argument_list|,
operator|&
name|mode
argument_list|)
operator|<
literal|0
condition|)
return|return;
name|show_file
argument_list|(
literal|"+"
argument_list|,
name|new
argument_list|,
name|sha1
argument_list|,
name|mode
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_modified
specifier|static
name|int
name|show_modified
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|old
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|new
parameter_list|)
block|{
name|unsigned
name|int
name|mode
decl_stmt|,
name|oldmode
decl_stmt|;
name|unsigned
name|char
modifier|*
name|sha1
decl_stmt|;
name|unsigned
name|char
name|old_sha1_hex
index|[
literal|60
index|]
decl_stmt|;
if|if
condition|(
name|get_stat_data
argument_list|(
name|new
argument_list|,
operator|&
name|sha1
argument_list|,
operator|&
name|mode
argument_list|)
operator|<
literal|0
condition|)
block|{
name|show_file
argument_list|(
literal|"-"
argument_list|,
name|old
argument_list|,
name|old
operator|->
name|sha1
argument_list|,
name|old
operator|->
name|ce_mode
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|oldmode
operator|=
name|old
operator|->
name|ce_mode
expr_stmt|;
if|if
condition|(
name|mode
operator|==
name|oldmode
operator|&&
operator|!
name|memcmp
argument_list|(
name|sha1
argument_list|,
name|old
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
condition|)
return|return
literal|0
return|;
name|mode
operator|=
name|ntohl
argument_list|(
name|mode
argument_list|)
expr_stmt|;
name|oldmode
operator|=
name|ntohl
argument_list|(
name|oldmode
argument_list|)
expr_stmt|;
if|if
condition|(
name|generate_patch
condition|)
name|diff_change
argument_list|(
name|oldmode
argument_list|,
name|mode
argument_list|,
name|old
operator|->
name|sha1
argument_list|,
name|sha1
argument_list|,
name|old
operator|->
name|name
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
else|else
block|{
name|strcpy
argument_list|(
name|old_sha1_hex
argument_list|,
name|sha1_to_hex
argument_list|(
name|old
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"*%06o->%06o\tblob\t%s->%s\t%s%c"
argument_list|,
name|oldmode
argument_list|,
name|mode
argument_list|,
name|old_sha1_hex
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|old
operator|->
name|name
argument_list|,
name|line_termination
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|diff_cache
specifier|static
name|int
name|diff_cache
parameter_list|(
name|struct
name|cache_entry
modifier|*
modifier|*
name|ac
parameter_list|,
name|int
name|entries
parameter_list|)
block|{
while|while
condition|(
name|entries
condition|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
operator|*
name|ac
decl_stmt|;
name|int
name|same
init|=
operator|(
name|entries
operator|>
literal|1
operator|)
operator|&&
name|same_name
argument_list|(
name|ce
argument_list|,
name|ac
index|[
literal|1
index|]
argument_list|)
decl_stmt|;
switch|switch
condition|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|)
block|{
case|case
literal|0
case|:
comment|/* No stage 1 entry? That means it's a new file */
if|if
condition|(
operator|!
name|same
condition|)
block|{
name|show_new_file
argument_list|(
name|ce
argument_list|)
expr_stmt|;
break|break;
block|}
comment|/* Show difference between old and new */
name|show_modified
argument_list|(
name|ac
index|[
literal|1
index|]
argument_list|,
name|ce
argument_list|)
expr_stmt|;
break|break;
case|case
literal|1
case|:
comment|/* No stage 3 (merge) entry? That means it's been deleted */
if|if
condition|(
operator|!
name|same
condition|)
block|{
name|show_file
argument_list|(
literal|"-"
argument_list|,
name|ce
argument_list|,
name|ce
operator|->
name|sha1
argument_list|,
name|ce
operator|->
name|ce_mode
argument_list|)
expr_stmt|;
break|break;
block|}
comment|/* Otherwise we fall through to the "unmerged" case */
case|case
literal|3
case|:
if|if
condition|(
name|generate_patch
condition|)
name|diff_unmerge
argument_list|(
name|ce
operator|->
name|name
argument_list|)
expr_stmt|;
else|else
name|printf
argument_list|(
literal|"U %s%c"
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|line_termination
argument_list|)
expr_stmt|;
break|break;
default|default:
name|die
argument_list|(
literal|"impossible cache entry stage"
argument_list|)
expr_stmt|;
block|}
comment|/* 		 * Ignore all the different stages for this file, 		 * we've handled the relevant cases now. 		 */
do|do
block|{
name|ac
operator|++
expr_stmt|;
name|entries
operator|--
expr_stmt|;
block|}
do|while
condition|(
name|entries
operator|&&
name|same_name
argument_list|(
name|ce
argument_list|,
name|ac
index|[
literal|0
index|]
argument_list|)
condition|)
do|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * This turns all merge entries into "stage 3". That guarantees that  * when we read in the new tree (into "stage 1"), we won't lose sight  * of the fact that we had unmerged entries.  */
end_comment

begin_function
DECL|function|mark_merge_entries
specifier|static
name|void
name|mark_merge_entries
parameter_list|(
name|void
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
name|active_nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|active_cache
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|)
continue|continue;
name|ce
operator|->
name|ce_flags
operator||=
name|htons
argument_list|(
name|CE_STAGEMASK
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_decl_stmt
DECL|variable|diff_cache_usage
specifier|static
name|char
modifier|*
name|diff_cache_usage
init|=
literal|"diff-cache [-r] [-z] [-p] [--cached]<tree sha1>"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|unsigned
name|char
name|tree_sha1
index|[
literal|20
index|]
decl_stmt|;
name|void
modifier|*
name|tree
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|read_cache
argument_list|()
expr_stmt|;
while|while
condition|(
name|argc
operator|>
literal|2
condition|)
block|{
name|char
modifier|*
name|arg
init|=
name|argv
index|[
literal|1
index|]
decl_stmt|;
name|argv
operator|++
expr_stmt|;
name|argc
operator|--
expr_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-r"
argument_list|)
condition|)
block|{
comment|/* We accept the -r flag just to look like diff-tree */
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-p"
argument_list|)
condition|)
block|{
name|generate_patch
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-z"
argument_list|)
condition|)
block|{
name|line_termination
operator|=
literal|'\0'
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--cached"
argument_list|)
condition|)
block|{
name|cached_only
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|usage
argument_list|(
name|diff_cache_usage
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|!=
literal|2
operator|||
name|get_sha1
argument_list|(
name|argv
index|[
literal|1
index|]
argument_list|,
name|tree_sha1
argument_list|)
condition|)
name|usage
argument_list|(
name|diff_cache_usage
argument_list|)
expr_stmt|;
name|mark_merge_entries
argument_list|()
expr_stmt|;
name|tree
operator|=
name|read_object_with_reference
argument_list|(
name|tree_sha1
argument_list|,
literal|"tree"
argument_list|,
operator|&
name|size
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|tree
condition|)
name|die
argument_list|(
literal|"bad tree object %s"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|read_tree
argument_list|(
name|tree
argument_list|,
name|size
argument_list|,
literal|1
argument_list|)
condition|)
name|die
argument_list|(
literal|"unable to read tree object %s"
argument_list|,
name|argv
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
return|return
name|diff_cache
argument_list|(
name|active_cache
argument_list|,
name|active_nr
argument_list|)
return|;
block|}
end_function

end_unit

