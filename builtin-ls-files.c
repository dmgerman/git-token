begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * This merges the file listing in the directory cache index  * with the actual working directory list, and shows different  * combinations of the two.  *  * Copyright (C) Linus Torvalds, 2005  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"quote.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_decl_stmt
DECL|variable|abbrev
specifier|static
name|int
name|abbrev
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_deleted
specifier|static
name|int
name|show_deleted
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_cached
specifier|static
name|int
name|show_cached
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_others
specifier|static
name|int
name|show_others
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_stage
specifier|static
name|int
name|show_stage
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_unmerged
specifier|static
name|int
name|show_unmerged
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_modified
specifier|static
name|int
name|show_modified
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_killed
specifier|static
name|int
name|show_killed
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|show_valid_bit
specifier|static
name|int
name|show_valid_bit
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|line_terminator
specifier|static
name|int
name|line_terminator
init|=
literal|'\n'
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|prefix_len
specifier|static
name|int
name|prefix_len
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|prefix_offset
specifier|static
name|int
name|prefix_offset
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pathspec
specifier|static
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|error_unmatch
specifier|static
name|int
name|error_unmatch
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ps_matched
specifier|static
name|char
modifier|*
name|ps_matched
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tag_cached
specifier|static
specifier|const
name|char
modifier|*
name|tag_cached
init|=
literal|""
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tag_unmerged
specifier|static
specifier|const
name|char
modifier|*
name|tag_unmerged
init|=
literal|""
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tag_removed
specifier|static
specifier|const
name|char
modifier|*
name|tag_removed
init|=
literal|""
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tag_other
specifier|static
specifier|const
name|char
modifier|*
name|tag_other
init|=
literal|""
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tag_killed
specifier|static
specifier|const
name|char
modifier|*
name|tag_killed
init|=
literal|""
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tag_modified
specifier|static
specifier|const
name|char
modifier|*
name|tag_modified
init|=
literal|""
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Match a pathspec against a filename. The first "len" characters  * are the common prefix  */
end_comment

begin_function
DECL|function|match
specifier|static
name|int
name|match
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|spec
parameter_list|,
name|char
modifier|*
name|ps_matched
parameter_list|,
specifier|const
name|char
modifier|*
name|filename
parameter_list|,
name|int
name|len
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|m
decl_stmt|;
while|while
condition|(
operator|(
name|m
operator|=
operator|*
name|spec
operator|++
operator|)
operator|!=
name|NULL
condition|)
block|{
name|int
name|matchlen
init|=
name|strlen
argument_list|(
name|m
operator|+
name|len
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|matchlen
condition|)
goto|goto
name|matched
goto|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|m
operator|+
name|len
argument_list|,
name|filename
operator|+
name|len
argument_list|,
name|matchlen
argument_list|)
condition|)
block|{
if|if
condition|(
name|m
index|[
name|len
operator|+
name|matchlen
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
goto|goto
name|matched
goto|;
switch|switch
condition|(
name|filename
index|[
name|len
operator|+
name|matchlen
index|]
condition|)
block|{
case|case
literal|'/'
case|:
case|case
literal|'\0'
case|:
goto|goto
name|matched
goto|;
block|}
block|}
if|if
condition|(
operator|!
name|fnmatch
argument_list|(
name|m
operator|+
name|len
argument_list|,
name|filename
operator|+
name|len
argument_list|,
literal|0
argument_list|)
condition|)
goto|goto
name|matched
goto|;
if|if
condition|(
name|ps_matched
condition|)
name|ps_matched
operator|++
expr_stmt|;
continue|continue;
name|matched
label|:
if|if
condition|(
name|ps_matched
condition|)
operator|*
name|ps_matched
operator|=
literal|1
expr_stmt|;
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
DECL|function|show_dir_entry
specifier|static
name|void
name|show_dir_entry
parameter_list|(
specifier|const
name|char
modifier|*
name|tag
parameter_list|,
name|struct
name|dir_entry
modifier|*
name|ent
parameter_list|)
block|{
name|int
name|len
init|=
name|prefix_len
decl_stmt|;
name|int
name|offset
init|=
name|prefix_offset
decl_stmt|;
if|if
condition|(
name|len
operator|>=
name|ent
operator|->
name|len
condition|)
name|die
argument_list|(
literal|"git-ls-files: internal error - directory entry not superset of prefix"
argument_list|)
expr_stmt|;
if|if
condition|(
name|pathspec
operator|&&
operator|!
name|match
argument_list|(
name|pathspec
argument_list|,
name|ps_matched
argument_list|,
name|ent
operator|->
name|name
argument_list|,
name|len
argument_list|)
condition|)
return|return;
name|fputs
argument_list|(
name|tag
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|write_name_quoted
argument_list|(
literal|""
argument_list|,
literal|0
argument_list|,
name|ent
operator|->
name|name
operator|+
name|offset
argument_list|,
name|line_terminator
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
name|line_terminator
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|show_other_files
specifier|static
name|void
name|show_other_files
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
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
name|dir
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
comment|/* We should not have a matching entry, but we 		 * may have an unmerged entry for this path. 		 */
name|struct
name|dir_entry
modifier|*
name|ent
init|=
name|dir
operator|->
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
literal|"bug in show-other-files"
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
comment|/* Yup, this one exists unmerged */
block|}
name|show_dir_entry
argument_list|(
name|tag_other
argument_list|,
name|ent
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|show_killed_files
specifier|static
name|void
name|show_killed_files
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
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
name|dir
operator|->
name|nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|dir_entry
modifier|*
name|ent
init|=
name|dir
operator|->
name|entries
index|[
name|i
index|]
decl_stmt|;
name|char
modifier|*
name|cp
decl_stmt|,
modifier|*
name|sp
decl_stmt|;
name|int
name|pos
decl_stmt|,
name|len
decl_stmt|,
name|killed
init|=
literal|0
decl_stmt|;
for|for
control|(
name|cp
operator|=
name|ent
operator|->
name|name
init|;
name|cp
operator|-
name|ent
operator|->
name|name
operator|<
name|ent
operator|->
name|len
condition|;
name|cp
operator|=
name|sp
operator|+
literal|1
control|)
block|{
name|sp
operator|=
name|strchr
argument_list|(
name|cp
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|sp
condition|)
block|{
comment|/* If ent->name is prefix of an entry in the 				 * cache, it will be killed. 				 */
name|pos
operator|=
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
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|pos
condition|)
name|die
argument_list|(
literal|"bug in show-killed-files"
argument_list|)
expr_stmt|;
name|pos
operator|=
operator|-
name|pos
operator|-
literal|1
expr_stmt|;
while|while
condition|(
name|pos
operator|<
name|active_nr
operator|&&
name|ce_stage
argument_list|(
name|active_cache
index|[
name|pos
index|]
argument_list|)
condition|)
name|pos
operator|++
expr_stmt|;
comment|/* skip unmerged */
if|if
condition|(
name|active_nr
operator|<=
name|pos
condition|)
break|break;
comment|/* pos points at a name immediately after 				 * ent->name in the cache.  Does it expect 				 * ent->name to be a directory? 				 */
name|len
operator|=
name|ce_namelen
argument_list|(
name|active_cache
index|[
name|pos
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|ent
operator|->
name|len
operator|<
name|len
operator|)
operator|&&
operator|!
name|strncmp
argument_list|(
name|active_cache
index|[
name|pos
index|]
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
operator|&&
name|active_cache
index|[
name|pos
index|]
operator|->
name|name
index|[
name|ent
operator|->
name|len
index|]
operator|==
literal|'/'
condition|)
name|killed
operator|=
literal|1
expr_stmt|;
break|break;
block|}
if|if
condition|(
literal|0
operator|<=
name|cache_name_pos
argument_list|(
name|ent
operator|->
name|name
argument_list|,
name|sp
operator|-
name|ent
operator|->
name|name
argument_list|)
condition|)
block|{
comment|/* If any of the leading directories in 				 * ent->name is registered in the cache, 				 * ent->name will be killed. 				 */
name|killed
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
name|killed
condition|)
name|show_dir_entry
argument_list|(
name|tag_killed
argument_list|,
name|dir
operator|->
name|entries
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|show_ce_entry
specifier|static
name|void
name|show_ce_entry
parameter_list|(
specifier|const
name|char
modifier|*
name|tag
parameter_list|,
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|)
block|{
name|int
name|len
init|=
name|prefix_len
decl_stmt|;
name|int
name|offset
init|=
name|prefix_offset
decl_stmt|;
if|if
condition|(
name|len
operator|>=
name|ce_namelen
argument_list|(
name|ce
argument_list|)
condition|)
name|die
argument_list|(
literal|"git-ls-files: internal error - cache entry not superset of prefix"
argument_list|)
expr_stmt|;
if|if
condition|(
name|pathspec
operator|&&
operator|!
name|match
argument_list|(
name|pathspec
argument_list|,
name|ps_matched
argument_list|,
name|ce
operator|->
name|name
argument_list|,
name|len
argument_list|)
condition|)
return|return;
if|if
condition|(
name|tag
operator|&&
operator|*
name|tag
operator|&&
name|show_valid_bit
operator|&&
operator|(
name|ce
operator|->
name|ce_flags
operator|&
name|htons
argument_list|(
name|CE_VALID
argument_list|)
operator|)
condition|)
block|{
specifier|static
name|char
name|alttag
index|[
literal|4
index|]
decl_stmt|;
name|memcpy
argument_list|(
name|alttag
argument_list|,
name|tag
argument_list|,
literal|3
argument_list|)
expr_stmt|;
if|if
condition|(
name|isalpha
argument_list|(
name|tag
index|[
literal|0
index|]
argument_list|)
condition|)
name|alttag
index|[
literal|0
index|]
operator|=
name|tolower
argument_list|(
name|tag
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|tag
index|[
literal|0
index|]
operator|==
literal|'?'
condition|)
name|alttag
index|[
literal|0
index|]
operator|=
literal|'!'
expr_stmt|;
else|else
block|{
name|alttag
index|[
literal|0
index|]
operator|=
literal|'v'
expr_stmt|;
name|alttag
index|[
literal|1
index|]
operator|=
name|tag
index|[
literal|0
index|]
expr_stmt|;
name|alttag
index|[
literal|2
index|]
operator|=
literal|' '
expr_stmt|;
name|alttag
index|[
literal|3
index|]
operator|=
literal|0
expr_stmt|;
block|}
name|tag
operator|=
name|alttag
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|show_stage
condition|)
block|{
name|fputs
argument_list|(
name|tag
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|write_name_quoted
argument_list|(
literal|""
argument_list|,
literal|0
argument_list|,
name|ce
operator|->
name|name
operator|+
name|offset
argument_list|,
name|line_terminator
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
name|line_terminator
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|printf
argument_list|(
literal|"%s%06o %s %d\t"
argument_list|,
name|tag
argument_list|,
name|ntohl
argument_list|(
name|ce
operator|->
name|ce_mode
argument_list|)
argument_list|,
name|abbrev
condition|?
name|find_unique_abbrev
argument_list|(
name|ce
operator|->
name|sha1
argument_list|,
name|abbrev
argument_list|)
else|:
name|sha1_to_hex
argument_list|(
name|ce
operator|->
name|sha1
argument_list|)
argument_list|,
name|ce_stage
argument_list|(
name|ce
argument_list|)
argument_list|)
expr_stmt|;
name|write_name_quoted
argument_list|(
literal|""
argument_list|,
literal|0
argument_list|,
name|ce
operator|->
name|name
operator|+
name|offset
argument_list|,
name|line_terminator
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|putchar
argument_list|(
name|line_terminator
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|show_files
specifier|static
name|void
name|show_files
parameter_list|(
name|struct
name|dir_struct
modifier|*
name|dir
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
comment|/* For cached/deleted files we don't need to even do the readdir */
if|if
condition|(
name|show_others
operator|||
name|show_killed
condition|)
block|{
specifier|const
name|char
modifier|*
name|path
init|=
literal|"."
decl_stmt|,
modifier|*
name|base
init|=
literal|""
decl_stmt|;
name|int
name|baselen
init|=
name|prefix_len
decl_stmt|;
if|if
condition|(
name|baselen
condition|)
name|path
operator|=
name|base
operator|=
name|prefix
expr_stmt|;
name|read_directory
argument_list|(
name|dir
argument_list|,
name|path
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|,
name|pathspec
argument_list|)
expr_stmt|;
if|if
condition|(
name|show_others
condition|)
name|show_other_files
argument_list|(
name|dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|show_killed
condition|)
name|show_killed_files
argument_list|(
name|dir
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|show_cached
operator||
name|show_stage
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
name|excluded
argument_list|(
name|dir
argument_list|,
name|ce
operator|->
name|name
argument_list|)
operator|!=
name|dir
operator|->
name|show_ignored
condition|)
continue|continue;
if|if
condition|(
name|show_unmerged
operator|&&
operator|!
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|)
continue|continue;
name|show_ce_entry
argument_list|(
name|ce_stage
argument_list|(
name|ce
argument_list|)
condition|?
name|tag_unmerged
else|:
name|tag_cached
argument_list|,
name|ce
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|show_deleted
operator||
name|show_modified
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
name|struct
name|stat
name|st
decl_stmt|;
name|int
name|err
decl_stmt|;
if|if
condition|(
name|excluded
argument_list|(
name|dir
argument_list|,
name|ce
operator|->
name|name
argument_list|)
operator|!=
name|dir
operator|->
name|show_ignored
condition|)
continue|continue;
name|err
operator|=
name|lstat
argument_list|(
name|ce
operator|->
name|name
argument_list|,
operator|&
name|st
argument_list|)
expr_stmt|;
if|if
condition|(
name|show_deleted
operator|&&
name|err
condition|)
name|show_ce_entry
argument_list|(
name|tag_removed
argument_list|,
name|ce
argument_list|)
expr_stmt|;
if|if
condition|(
name|show_modified
operator|&&
name|ce_modified
argument_list|(
name|ce
argument_list|,
operator|&
name|st
argument_list|,
literal|0
argument_list|)
condition|)
name|show_ce_entry
argument_list|(
name|tag_modified
argument_list|,
name|ce
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_comment
comment|/*  * Prune the index to only contain stuff starting with "prefix"  */
end_comment

begin_function
DECL|function|prune_cache
specifier|static
name|void
name|prune_cache
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
name|int
name|pos
init|=
name|cache_name_pos
argument_list|(
name|prefix
argument_list|,
name|prefix_len
argument_list|)
decl_stmt|;
name|unsigned
name|int
name|first
decl_stmt|,
name|last
decl_stmt|;
if|if
condition|(
name|pos
operator|<
literal|0
condition|)
name|pos
operator|=
operator|-
name|pos
operator|-
literal|1
expr_stmt|;
name|active_cache
operator|+=
name|pos
expr_stmt|;
name|active_nr
operator|-=
name|pos
expr_stmt|;
name|first
operator|=
literal|0
expr_stmt|;
name|last
operator|=
name|active_nr
expr_stmt|;
while|while
condition|(
name|last
operator|>
name|first
condition|)
block|{
name|int
name|next
init|=
operator|(
name|last
operator|+
name|first
operator|)
operator|>>
literal|1
decl_stmt|;
name|struct
name|cache_entry
modifier|*
name|ce
init|=
name|active_cache
index|[
name|next
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|ce
operator|->
name|name
argument_list|,
name|prefix
argument_list|,
name|prefix_len
argument_list|)
condition|)
block|{
name|first
operator|=
name|next
operator|+
literal|1
expr_stmt|;
continue|continue;
block|}
name|last
operator|=
name|next
expr_stmt|;
block|}
name|active_nr
operator|=
name|last
expr_stmt|;
block|}
end_function

begin_function
DECL|function|verify_pathspec
specifier|static
specifier|const
name|char
modifier|*
name|verify_pathspec
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
block|{
specifier|const
name|char
modifier|*
modifier|*
name|p
decl_stmt|,
modifier|*
name|n
decl_stmt|,
modifier|*
name|prev
decl_stmt|;
name|char
modifier|*
name|real_prefix
decl_stmt|;
name|unsigned
name|long
name|max
decl_stmt|;
name|prev
operator|=
name|NULL
expr_stmt|;
name|max
operator|=
name|PATH_MAX
expr_stmt|;
for|for
control|(
name|p
operator|=
name|pathspec
init|;
operator|(
name|n
operator|=
operator|*
name|p
operator|)
operator|!=
name|NULL
condition|;
name|p
operator|++
control|)
block|{
name|int
name|i
decl_stmt|,
name|len
init|=
literal|0
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|max
condition|;
name|i
operator|++
control|)
block|{
name|char
name|c
init|=
name|n
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
name|prev
operator|&&
name|prev
index|[
name|i
index|]
operator|!=
name|c
condition|)
break|break;
if|if
condition|(
operator|!
name|c
operator|||
name|c
operator|==
literal|'*'
operator|||
name|c
operator|==
literal|'?'
condition|)
break|break;
if|if
condition|(
name|c
operator|==
literal|'/'
condition|)
name|len
operator|=
name|i
operator|+
literal|1
expr_stmt|;
block|}
name|prev
operator|=
name|n
expr_stmt|;
if|if
condition|(
name|len
operator|<
name|max
condition|)
block|{
name|max
operator|=
name|len
expr_stmt|;
if|if
condition|(
operator|!
name|max
condition|)
break|break;
block|}
block|}
if|if
condition|(
name|prefix_offset
operator|>
name|max
operator|||
name|memcmp
argument_list|(
name|prev
argument_list|,
name|prefix
argument_list|,
name|prefix_offset
argument_list|)
condition|)
name|die
argument_list|(
literal|"git-ls-files: cannot generate relative filenames containing '..'"
argument_list|)
expr_stmt|;
name|real_prefix
operator|=
name|NULL
expr_stmt|;
name|prefix_len
operator|=
name|max
expr_stmt|;
if|if
condition|(
name|max
condition|)
block|{
name|real_prefix
operator|=
name|xmalloc
argument_list|(
name|max
operator|+
literal|1
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|real_prefix
argument_list|,
name|prev
argument_list|,
name|max
argument_list|)
expr_stmt|;
name|real_prefix
index|[
name|max
index|]
operator|=
literal|0
expr_stmt|;
block|}
return|return
name|real_prefix
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|ls_files_usage
specifier|static
specifier|const
name|char
name|ls_files_usage
index|[]
init|=
literal|"git-ls-files [-z] [-t] [-v] (--[cached|deleted|others|stage|unmerged|killed|modified])* "
literal|"[ --ignored ] [--exclude=<pattern>] [--exclude-from=<file>] "
literal|"[ --exclude-per-directory=<filename> ] [--full-name] [--abbrev] "
literal|"[--] [<file>]*"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_ls_files
name|int
name|cmd_ls_files
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
name|i
decl_stmt|;
name|int
name|exc_given
init|=
literal|0
decl_stmt|,
name|require_work_tree
init|=
literal|0
decl_stmt|;
name|struct
name|dir_struct
name|dir
decl_stmt|;
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
name|prefix
condition|)
name|prefix_offset
operator|=
name|strlen
argument_list|(
name|prefix
argument_list|)
expr_stmt|;
name|git_config
argument_list|(
name|git_default_config
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|argc
condition|;
name|i
operator|++
control|)
block|{
specifier|const
name|char
modifier|*
name|arg
init|=
name|argv
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--"
argument_list|)
condition|)
block|{
name|i
operator|++
expr_stmt|;
break|break;
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
name|line_terminator
operator|=
literal|0
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
literal|"-t"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"-v"
argument_list|)
condition|)
block|{
name|tag_cached
operator|=
literal|"H "
expr_stmt|;
name|tag_unmerged
operator|=
literal|"M "
expr_stmt|;
name|tag_removed
operator|=
literal|"R "
expr_stmt|;
name|tag_modified
operator|=
literal|"C "
expr_stmt|;
name|tag_other
operator|=
literal|"? "
expr_stmt|;
name|tag_killed
operator|=
literal|"K "
expr_stmt|;
if|if
condition|(
name|arg
index|[
literal|1
index|]
operator|==
literal|'v'
condition|)
name|show_valid_bit
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
literal|"-c"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--cached"
argument_list|)
condition|)
block|{
name|show_cached
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
literal|"-d"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--deleted"
argument_list|)
condition|)
block|{
name|show_deleted
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
literal|"-m"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--modified"
argument_list|)
condition|)
block|{
name|show_modified
operator|=
literal|1
expr_stmt|;
name|require_work_tree
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
literal|"-o"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--others"
argument_list|)
condition|)
block|{
name|show_others
operator|=
literal|1
expr_stmt|;
name|require_work_tree
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
literal|"-i"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--ignored"
argument_list|)
condition|)
block|{
name|dir
operator|.
name|show_ignored
operator|=
literal|1
expr_stmt|;
name|require_work_tree
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
literal|"-s"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--stage"
argument_list|)
condition|)
block|{
name|show_stage
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
literal|"-k"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--killed"
argument_list|)
condition|)
block|{
name|show_killed
operator|=
literal|1
expr_stmt|;
name|require_work_tree
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
literal|"--directory"
argument_list|)
condition|)
block|{
name|dir
operator|.
name|show_other_directories
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
literal|"--no-empty-directory"
argument_list|)
condition|)
block|{
name|dir
operator|.
name|hide_empty_directories
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
literal|"-u"
argument_list|)
operator|||
operator|!
name|strcmp
argument_list|(
name|arg
argument_list|,
literal|"--unmerged"
argument_list|)
condition|)
block|{
comment|/* There's no point in showing unmerged unless 			 * you also show the stage information. 			 */
name|show_stage
operator|=
literal|1
expr_stmt|;
name|show_unmerged
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
literal|"-x"
argument_list|)
operator|&&
name|i
operator|+
literal|1
operator|<
name|argc
condition|)
block|{
name|exc_given
operator|=
literal|1
expr_stmt|;
name|add_exclude
argument_list|(
name|argv
index|[
operator|++
name|i
index|]
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|,
operator|&
name|dir
operator|.
name|exclude_list
index|[
name|EXC_CMDL
index|]
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--exclude="
argument_list|)
condition|)
block|{
name|exc_given
operator|=
literal|1
expr_stmt|;
name|add_exclude
argument_list|(
name|arg
operator|+
literal|10
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|,
operator|&
name|dir
operator|.
name|exclude_list
index|[
name|EXC_CMDL
index|]
argument_list|)
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
literal|"-X"
argument_list|)
operator|&&
name|i
operator|+
literal|1
operator|<
name|argc
condition|)
block|{
name|exc_given
operator|=
literal|1
expr_stmt|;
name|add_excludes_from_file
argument_list|(
operator|&
name|dir
argument_list|,
name|argv
index|[
operator|++
name|i
index|]
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--exclude-from="
argument_list|)
condition|)
block|{
name|exc_given
operator|=
literal|1
expr_stmt|;
name|add_excludes_from_file
argument_list|(
operator|&
name|dir
argument_list|,
name|arg
operator|+
literal|15
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--exclude-per-directory="
argument_list|)
condition|)
block|{
name|exc_given
operator|=
literal|1
expr_stmt|;
name|dir
operator|.
name|exclude_per_dir
operator|=
name|arg
operator|+
literal|24
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
literal|"--full-name"
argument_list|)
condition|)
block|{
name|prefix_offset
operator|=
literal|0
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
literal|"--error-unmatch"
argument_list|)
condition|)
block|{
name|error_unmatch
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|!
name|prefixcmp
argument_list|(
name|arg
argument_list|,
literal|"--abbrev="
argument_list|)
condition|)
block|{
name|abbrev
operator|=
name|strtoul
argument_list|(
name|arg
operator|+
literal|9
argument_list|,
name|NULL
argument_list|,
literal|10
argument_list|)
expr_stmt|;
if|if
condition|(
name|abbrev
operator|&&
name|abbrev
operator|<
name|MINIMUM_ABBREV
condition|)
name|abbrev
operator|=
name|MINIMUM_ABBREV
expr_stmt|;
elseif|else
if|if
condition|(
name|abbrev
operator|>
literal|40
condition|)
name|abbrev
operator|=
literal|40
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
literal|"--abbrev"
argument_list|)
condition|)
block|{
name|abbrev
operator|=
name|DEFAULT_ABBREV
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|*
name|arg
operator|==
literal|'-'
condition|)
name|usage
argument_list|(
name|ls_files_usage
argument_list|)
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|require_work_tree
operator|&&
operator|(
name|is_bare_repository
argument_list|()
operator|||
name|is_inside_git_dir
argument_list|()
operator|)
condition|)
name|die
argument_list|(
literal|"This operation must be run in a work tree"
argument_list|)
expr_stmt|;
name|pathspec
operator|=
name|get_pathspec
argument_list|(
name|prefix
argument_list|,
name|argv
operator|+
name|i
argument_list|)
expr_stmt|;
comment|/* Verify that the pathspec matches the prefix */
if|if
condition|(
name|pathspec
condition|)
name|prefix
operator|=
name|verify_pathspec
argument_list|(
name|prefix
argument_list|)
expr_stmt|;
comment|/* Treat unmatching pathspec elements as errors */
if|if
condition|(
name|pathspec
operator|&&
name|error_unmatch
condition|)
block|{
name|int
name|num
decl_stmt|;
for|for
control|(
name|num
operator|=
literal|0
init|;
name|pathspec
index|[
name|num
index|]
condition|;
name|num
operator|++
control|)
empty_stmt|;
name|ps_matched
operator|=
name|xcalloc
argument_list|(
literal|1
argument_list|,
name|num
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|dir
operator|.
name|show_ignored
operator|&&
operator|!
name|exc_given
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%s: --ignored needs some exclude pattern\n"
argument_list|,
name|argv
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
comment|/* With no flags, we default to showing the cached files */
if|if
condition|(
operator|!
operator|(
name|show_stage
operator||
name|show_deleted
operator||
name|show_others
operator||
name|show_unmerged
operator||
name|show_killed
operator||
name|show_modified
operator|)
condition|)
name|show_cached
operator|=
literal|1
expr_stmt|;
name|read_cache
argument_list|()
expr_stmt|;
if|if
condition|(
name|prefix
condition|)
name|prune_cache
argument_list|(
name|prefix
argument_list|)
expr_stmt|;
name|show_files
argument_list|(
operator|&
name|dir
argument_list|,
name|prefix
argument_list|)
expr_stmt|;
if|if
condition|(
name|ps_matched
condition|)
block|{
comment|/* We need to make sure all pathspec matched otherwise 		 * it is an error. 		 */
name|int
name|num
decl_stmt|,
name|errors
init|=
literal|0
decl_stmt|;
for|for
control|(
name|num
operator|=
literal|0
init|;
name|pathspec
index|[
name|num
index|]
condition|;
name|num
operator|++
control|)
block|{
if|if
condition|(
name|ps_matched
index|[
name|num
index|]
condition|)
continue|continue;
name|error
argument_list|(
literal|"pathspec '%s' did not match any file(s) known to git."
argument_list|,
name|pathspec
index|[
name|num
index|]
operator|+
name|prefix_offset
argument_list|)
expr_stmt|;
name|errors
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|errors
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Did you forget to 'git add'?\n"
argument_list|)
expr_stmt|;
return|return
name|errors
condition|?
literal|1
else|:
literal|0
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

