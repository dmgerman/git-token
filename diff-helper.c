begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2005 Junio C Hamano  */
end_comment

begin_include
include|#
directive|include
file|<limits.h>
end_include

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_include
include|#
directive|include
file|"diff.h"
end_include

begin_function
DECL|function|matches_pathspec
specifier|static
name|int
name|matches_pathspec
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|spec
parameter_list|,
name|int
name|cnt
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
name|namelen
init|=
name|strlen
argument_list|(
name|name
argument_list|)
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|cnt
condition|;
name|i
operator|++
control|)
block|{
name|int
name|speclen
init|=
name|strlen
argument_list|(
name|spec
index|[
name|i
index|]
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|strncmp
argument_list|(
name|spec
index|[
name|i
index|]
argument_list|,
name|name
argument_list|,
name|speclen
argument_list|)
operator|&&
name|speclen
operator|<=
name|namelen
operator|&&
operator|(
name|name
index|[
name|speclen
index|]
operator|==
literal|0
operator|||
name|name
index|[
name|speclen
index|]
operator|==
literal|'/'
operator|)
condition|)
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|detect_rename
specifier|static
name|int
name|detect_rename
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * We do not detect circular renames.  Just hold created and deleted  * entries and later attempt to match them up.  If they do not match,  * then spit them out as deletes or creates as original.  */
end_comment

begin_struct
DECL|struct|diff_spec_hold
specifier|static
struct|struct
name|diff_spec_hold
block|{
DECL|member|next
name|struct
name|diff_spec_hold
modifier|*
name|next
decl_stmt|;
DECL|member|matched
name|struct
name|diff_spec_hold
modifier|*
name|matched
decl_stmt|;
DECL|member|old
DECL|member|new
name|struct
name|diff_spec
name|old
decl_stmt|,
name|new
decl_stmt|;
DECL|member|path
name|char
name|path
index|[
literal|1
index|]
decl_stmt|;
DECL|variable|createdfile
DECL|variable|deletedfile
block|}
modifier|*
name|createdfile
struct|,
modifier|*
name|deletedfile
struct|;
end_struct

begin_function
DECL|function|hold_spec
specifier|static
name|void
name|hold_spec
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|struct
name|diff_spec
modifier|*
name|old
parameter_list|,
name|struct
name|diff_spec
modifier|*
name|new
parameter_list|)
block|{
name|struct
name|diff_spec_hold
modifier|*
modifier|*
name|list
decl_stmt|,
modifier|*
name|elem
decl_stmt|;
name|list
operator|=
operator|(
operator|!
name|old
operator|->
name|file_valid
operator|)
condition|?
operator|&
name|createdfile
else|:
operator|&
name|deletedfile
expr_stmt|;
name|elem
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|elem
argument_list|)
operator|+
name|strlen
argument_list|(
name|path
argument_list|)
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|elem
operator|->
name|path
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|elem
operator|->
name|next
operator|=
operator|*
name|list
expr_stmt|;
operator|*
name|list
operator|=
name|elem
expr_stmt|;
name|elem
operator|->
name|old
operator|=
operator|*
name|old
expr_stmt|;
name|elem
operator|->
name|new
operator|=
operator|*
name|new
expr_stmt|;
name|elem
operator|->
name|matched
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_define
DECL|macro|MINIMUM_SCORE
define|#
directive|define
name|MINIMUM_SCORE
value|7000
end_define

begin_function
DECL|function|estimate_similarity
name|int
name|estimate_similarity
parameter_list|(
name|struct
name|diff_spec
modifier|*
name|one
parameter_list|,
name|struct
name|diff_spec
modifier|*
name|two
parameter_list|)
block|{
comment|/* Return how similar they are, representing the score as an 	 * integer between 0 and 10000. 	 * 	 * This version is very dumb and detects exact matches only. 	 * Wnen Nico's delta stuff gets in, I'll use the delta 	 * algorithm to estimate the similarity score in core. 	 */
if|if
condition|(
name|one
operator|->
name|sha1_valid
operator|&&
name|two
operator|->
name|sha1_valid
operator|&&
operator|!
name|memcmp
argument_list|(
name|one
operator|->
name|blob_sha1
argument_list|,
name|two
operator|->
name|blob_sha1
argument_list|,
literal|20
argument_list|)
condition|)
return|return
literal|10000
return|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|flush_renames
specifier|static
name|void
name|flush_renames
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|spec
parameter_list|,
name|int
name|cnt
parameter_list|,
name|int
name|reverse
parameter_list|)
block|{
name|struct
name|diff_spec_hold
modifier|*
name|rename_src
decl_stmt|,
modifier|*
name|rename_dst
decl_stmt|,
modifier|*
name|elem
decl_stmt|;
name|struct
name|diff_spec_hold
modifier|*
name|leftover
init|=
name|NULL
decl_stmt|;
name|int
name|score
decl_stmt|,
name|best_score
decl_stmt|;
while|while
condition|(
name|createdfile
condition|)
block|{
name|rename_dst
operator|=
name|createdfile
expr_stmt|;
name|createdfile
operator|=
name|rename_dst
operator|->
name|next
expr_stmt|;
name|best_score
operator|=
name|MINIMUM_SCORE
expr_stmt|;
name|rename_src
operator|=
name|NULL
expr_stmt|;
for|for
control|(
name|elem
operator|=
name|deletedfile
init|;
name|elem
condition|;
name|elem
operator|=
name|elem
operator|->
name|next
control|)
block|{
if|if
condition|(
name|elem
operator|->
name|matched
condition|)
continue|continue;
name|score
operator|=
name|estimate_similarity
argument_list|(
operator|&
name|elem
operator|->
name|old
argument_list|,
operator|&
name|rename_dst
operator|->
name|new
argument_list|)
expr_stmt|;
if|if
condition|(
name|best_score
operator|<
name|score
condition|)
block|{
name|rename_src
operator|=
name|elem
expr_stmt|;
name|best_score
operator|=
name|score
expr_stmt|;
block|}
block|}
if|if
condition|(
name|rename_src
condition|)
block|{
name|rename_src
operator|->
name|matched
operator|=
name|rename_dst
expr_stmt|;
name|rename_dst
operator|->
name|matched
operator|=
name|rename_src
expr_stmt|;
if|if
condition|(
operator|!
name|cnt
operator|||
name|matches_pathspec
argument_list|(
name|rename_src
operator|->
name|path
argument_list|,
name|spec
argument_list|,
name|cnt
argument_list|)
operator|||
name|matches_pathspec
argument_list|(
name|rename_dst
operator|->
name|path
argument_list|,
name|spec
argument_list|,
name|cnt
argument_list|)
condition|)
block|{
if|if
condition|(
name|reverse
condition|)
name|run_external_diff
argument_list|(
name|rename_dst
operator|->
name|path
argument_list|,
name|rename_src
operator|->
name|path
argument_list|,
operator|&
name|rename_dst
operator|->
name|new
argument_list|,
operator|&
name|rename_src
operator|->
name|old
argument_list|)
expr_stmt|;
else|else
name|run_external_diff
argument_list|(
name|rename_src
operator|->
name|path
argument_list|,
name|rename_dst
operator|->
name|path
argument_list|,
operator|&
name|rename_src
operator|->
name|old
argument_list|,
operator|&
name|rename_dst
operator|->
name|new
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
name|rename_dst
operator|->
name|next
operator|=
name|leftover
expr_stmt|;
name|leftover
operator|=
name|rename_dst
expr_stmt|;
block|}
block|}
comment|/* unmatched deletes */
for|for
control|(
name|elem
operator|=
name|deletedfile
init|;
name|elem
condition|;
name|elem
operator|=
name|elem
operator|->
name|next
control|)
block|{
if|if
condition|(
name|elem
operator|->
name|matched
condition|)
continue|continue;
if|if
condition|(
operator|!
name|cnt
operator|||
name|matches_pathspec
argument_list|(
name|elem
operator|->
name|path
argument_list|,
name|spec
argument_list|,
name|cnt
argument_list|)
condition|)
block|{
if|if
condition|(
name|reverse
condition|)
name|run_external_diff
argument_list|(
name|elem
operator|->
name|path
argument_list|,
name|NULL
argument_list|,
operator|&
name|elem
operator|->
name|new
argument_list|,
operator|&
name|elem
operator|->
name|old
argument_list|)
expr_stmt|;
else|else
name|run_external_diff
argument_list|(
name|elem
operator|->
name|path
argument_list|,
name|NULL
argument_list|,
operator|&
name|elem
operator|->
name|old
argument_list|,
operator|&
name|elem
operator|->
name|new
argument_list|)
expr_stmt|;
block|}
block|}
comment|/* unmatched creates */
for|for
control|(
name|elem
operator|=
name|leftover
init|;
name|elem
condition|;
name|elem
operator|=
name|elem
operator|->
name|next
control|)
block|{
if|if
condition|(
operator|!
name|cnt
operator|||
name|matches_pathspec
argument_list|(
name|elem
operator|->
name|path
argument_list|,
name|spec
argument_list|,
name|cnt
argument_list|)
condition|)
block|{
if|if
condition|(
name|reverse
condition|)
name|run_external_diff
argument_list|(
name|elem
operator|->
name|path
argument_list|,
name|NULL
argument_list|,
operator|&
name|elem
operator|->
name|new
argument_list|,
operator|&
name|elem
operator|->
name|old
argument_list|)
expr_stmt|;
else|else
name|run_external_diff
argument_list|(
name|elem
operator|->
name|path
argument_list|,
name|NULL
argument_list|,
operator|&
name|elem
operator|->
name|old
argument_list|,
operator|&
name|elem
operator|->
name|new
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|parse_oneside_change
specifier|static
name|int
name|parse_oneside_change
parameter_list|(
specifier|const
name|char
modifier|*
name|cp
parameter_list|,
name|struct
name|diff_spec
modifier|*
name|one
parameter_list|,
name|char
modifier|*
name|path
parameter_list|)
block|{
name|int
name|ch
decl_stmt|;
name|one
operator|->
name|file_valid
operator|=
name|one
operator|->
name|sha1_valid
operator|=
literal|1
expr_stmt|;
name|one
operator|->
name|mode
operator|=
literal|0
expr_stmt|;
while|while
condition|(
operator|(
name|ch
operator|=
operator|*
name|cp
operator|)
operator|&&
literal|'0'
operator|<=
name|ch
operator|&&
name|ch
operator|<=
literal|'7'
condition|)
block|{
name|one
operator|->
name|mode
operator|=
operator|(
name|one
operator|->
name|mode
operator|<<
literal|3
operator|)
operator||
operator|(
name|ch
operator|-
literal|'0'
operator|)
expr_stmt|;
name|cp
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|strncmp
argument_list|(
name|cp
argument_list|,
literal|"\tblob\t"
argument_list|,
literal|6
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|cp
operator|+=
literal|6
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|cp
argument_list|,
name|one
operator|->
name|blob_sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|cp
operator|+=
literal|40
expr_stmt|;
if|if
condition|(
operator|*
name|cp
operator|++
operator|!=
literal|'\t'
condition|)
return|return
operator|-
literal|1
return|;
name|strcpy
argument_list|(
name|path
argument_list|,
name|cp
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|parse_diff_raw_output
specifier|static
name|int
name|parse_diff_raw_output
parameter_list|(
specifier|const
name|char
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|spec
parameter_list|,
name|int
name|cnt
parameter_list|,
name|int
name|reverse
parameter_list|)
block|{
name|struct
name|diff_spec
name|old
decl_stmt|,
name|new
decl_stmt|;
name|char
name|path
index|[
name|PATH_MAX
index|]
decl_stmt|;
specifier|const
name|char
modifier|*
name|cp
init|=
name|buf
decl_stmt|;
name|int
name|ch
decl_stmt|;
switch|switch
condition|(
operator|*
name|cp
operator|++
condition|)
block|{
case|case
literal|'U'
case|:
if|if
condition|(
operator|!
name|cnt
operator|||
name|matches_pathspec
argument_list|(
name|cp
operator|+
literal|1
argument_list|,
name|spec
argument_list|,
name|cnt
argument_list|)
condition|)
name|diff_unmerge
argument_list|(
name|cp
operator|+
literal|1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
case|case
literal|'+'
case|:
name|old
operator|.
name|file_valid
operator|=
literal|0
expr_stmt|;
name|parse_oneside_change
argument_list|(
name|cp
argument_list|,
operator|&
name|new
argument_list|,
name|path
argument_list|)
expr_stmt|;
break|break;
case|case
literal|'-'
case|:
name|new
operator|.
name|file_valid
operator|=
literal|0
expr_stmt|;
name|parse_oneside_change
argument_list|(
name|cp
argument_list|,
operator|&
name|old
argument_list|,
name|path
argument_list|)
expr_stmt|;
break|break;
case|case
literal|'*'
case|:
name|old
operator|.
name|file_valid
operator|=
name|old
operator|.
name|sha1_valid
operator|=
name|new
operator|.
name|file_valid
operator|=
name|new
operator|.
name|sha1_valid
operator|=
literal|1
expr_stmt|;
name|old
operator|.
name|mode
operator|=
name|new
operator|.
name|mode
operator|=
literal|0
expr_stmt|;
while|while
condition|(
operator|(
name|ch
operator|=
operator|*
name|cp
operator|)
operator|&&
operator|(
literal|'0'
operator|<=
name|ch
operator|&&
name|ch
operator|<=
literal|'7'
operator|)
condition|)
block|{
name|old
operator|.
name|mode
operator|=
operator|(
name|old
operator|.
name|mode
operator|<<
literal|3
operator|)
operator||
operator|(
name|ch
operator|-
literal|'0'
operator|)
expr_stmt|;
name|cp
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|strncmp
argument_list|(
name|cp
argument_list|,
literal|"->"
argument_list|,
literal|2
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|cp
operator|+=
literal|2
expr_stmt|;
while|while
condition|(
operator|(
name|ch
operator|=
operator|*
name|cp
operator|)
operator|&&
operator|(
literal|'0'
operator|<=
name|ch
operator|&&
name|ch
operator|<=
literal|'7'
operator|)
condition|)
block|{
name|new
operator|.
name|mode
operator|=
operator|(
name|new
operator|.
name|mode
operator|<<
literal|3
operator|)
operator||
operator|(
name|ch
operator|-
literal|'0'
operator|)
expr_stmt|;
name|cp
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|strncmp
argument_list|(
name|cp
argument_list|,
literal|"\tblob\t"
argument_list|,
literal|6
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|cp
operator|+=
literal|6
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|cp
argument_list|,
name|old
operator|.
name|blob_sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|cp
operator|+=
literal|40
expr_stmt|;
if|if
condition|(
name|strncmp
argument_list|(
name|cp
argument_list|,
literal|"->"
argument_list|,
literal|2
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|cp
operator|+=
literal|2
expr_stmt|;
if|if
condition|(
name|get_sha1_hex
argument_list|(
name|cp
argument_list|,
name|new
operator|.
name|blob_sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|cp
operator|+=
literal|40
expr_stmt|;
if|if
condition|(
operator|*
name|cp
operator|++
operator|!=
literal|'\t'
condition|)
return|return
operator|-
literal|1
return|;
name|strcpy
argument_list|(
name|path
argument_list|,
name|cp
argument_list|)
expr_stmt|;
break|break;
default|default:
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|detect_rename
operator|&&
name|old
operator|.
name|file_valid
operator|!=
name|new
operator|.
name|file_valid
condition|)
block|{
comment|/* hold these */
name|hold_spec
argument_list|(
name|path
argument_list|,
operator|&
name|old
argument_list|,
operator|&
name|new
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|cnt
operator|||
name|matches_pathspec
argument_list|(
name|path
argument_list|,
name|spec
argument_list|,
name|cnt
argument_list|)
condition|)
block|{
if|if
condition|(
name|reverse
condition|)
name|run_external_diff
argument_list|(
name|path
argument_list|,
name|NULL
argument_list|,
operator|&
name|new
argument_list|,
operator|&
name|old
argument_list|)
expr_stmt|;
else|else
name|run_external_diff
argument_list|(
name|path
argument_list|,
name|NULL
argument_list|,
operator|&
name|old
argument_list|,
operator|&
name|new
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|diff_helper_usage
specifier|static
specifier|const
name|char
modifier|*
name|diff_helper_usage
init|=
literal|"git-diff-helper [-r] [-R] [-z] paths..."
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|ac
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|av
parameter_list|)
block|{
name|struct
name|strbuf
name|sb
decl_stmt|;
name|int
name|reverse
init|=
literal|0
decl_stmt|;
name|int
name|line_termination
init|=
literal|'\n'
decl_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|sb
argument_list|)
expr_stmt|;
while|while
condition|(
literal|1
operator|<
name|ac
operator|&&
name|av
index|[
literal|1
index|]
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
if|if
condition|(
name|av
index|[
literal|1
index|]
index|[
literal|1
index|]
operator|==
literal|'R'
condition|)
name|reverse
operator|=
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
name|av
index|[
literal|1
index|]
index|[
literal|1
index|]
operator|==
literal|'z'
condition|)
name|line_termination
operator|=
literal|0
expr_stmt|;
elseif|else
if|if
condition|(
name|av
index|[
literal|1
index|]
index|[
literal|1
index|]
operator|==
literal|'r'
condition|)
name|detect_rename
operator|=
literal|1
expr_stmt|;
else|else
name|usage
argument_list|(
name|diff_helper_usage
argument_list|)
expr_stmt|;
name|ac
operator|--
expr_stmt|;
name|av
operator|++
expr_stmt|;
block|}
comment|/* the remaining parameters are paths patterns */
while|while
condition|(
literal|1
condition|)
block|{
name|int
name|status
decl_stmt|;
name|read_line
argument_list|(
operator|&
name|sb
argument_list|,
name|stdin
argument_list|,
name|line_termination
argument_list|)
expr_stmt|;
if|if
condition|(
name|sb
operator|.
name|eof
condition|)
break|break;
name|status
operator|=
name|parse_diff_raw_output
argument_list|(
name|sb
operator|.
name|buf
argument_list|,
name|av
operator|+
literal|1
argument_list|,
name|ac
operator|-
literal|1
argument_list|,
name|reverse
argument_list|)
expr_stmt|;
if|if
condition|(
name|status
condition|)
block|{
name|flush_renames
argument_list|(
name|av
operator|+
literal|1
argument_list|,
name|ac
operator|-
literal|1
argument_list|,
name|reverse
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%s%c"
argument_list|,
name|sb
operator|.
name|buf
argument_list|,
name|line_termination
argument_list|)
expr_stmt|;
block|}
block|}
name|flush_renames
argument_list|(
name|av
operator|+
literal|1
argument_list|,
name|ac
operator|-
literal|1
argument_list|,
name|reverse
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

