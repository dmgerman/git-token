begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2005 Junio C Hamano  */
end_comment

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

begin_include
include|#
directive|include
file|"diffcore.h"
end_include

begin_include
include|#
directive|include
file|"delta.h"
end_include

begin_include
include|#
directive|include
file|"count-delta.h"
end_include

begin_comment
comment|/* Table of rename/copy destinations */
end_comment

begin_struct
DECL|struct|diff_rename_dst
specifier|static
struct|struct
name|diff_rename_dst
block|{
DECL|member|two
name|struct
name|diff_filespec
modifier|*
name|two
decl_stmt|;
DECL|member|pair
name|struct
name|diff_filepair
modifier|*
name|pair
decl_stmt|;
DECL|variable|rename_dst
block|}
modifier|*
name|rename_dst
struct|;
end_struct

begin_decl_stmt
DECL|variable|rename_dst_nr
DECL|variable|rename_dst_alloc
specifier|static
name|int
name|rename_dst_nr
decl_stmt|,
name|rename_dst_alloc
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|locate_rename_dst
specifier|static
name|struct
name|diff_rename_dst
modifier|*
name|locate_rename_dst
parameter_list|(
name|struct
name|diff_filespec
modifier|*
name|two
parameter_list|,
name|int
name|insert_ok
parameter_list|)
block|{
name|int
name|first
decl_stmt|,
name|last
decl_stmt|;
name|first
operator|=
literal|0
expr_stmt|;
name|last
operator|=
name|rename_dst_nr
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
name|diff_rename_dst
modifier|*
name|dst
init|=
operator|&
operator|(
name|rename_dst
index|[
name|next
index|]
operator|)
decl_stmt|;
name|int
name|cmp
init|=
name|strcmp
argument_list|(
name|two
operator|->
name|path
argument_list|,
name|dst
operator|->
name|two
operator|->
name|path
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|cmp
condition|)
return|return
name|dst
return|;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
block|{
name|last
operator|=
name|next
expr_stmt|;
continue|continue;
block|}
name|first
operator|=
name|next
operator|+
literal|1
expr_stmt|;
block|}
comment|/* not found */
if|if
condition|(
operator|!
name|insert_ok
condition|)
return|return
name|NULL
return|;
comment|/* insert to make it at "first" */
if|if
condition|(
name|rename_dst_alloc
operator|<=
name|rename_dst_nr
condition|)
block|{
name|rename_dst_alloc
operator|=
name|alloc_nr
argument_list|(
name|rename_dst_alloc
argument_list|)
expr_stmt|;
name|rename_dst
operator|=
name|xrealloc
argument_list|(
name|rename_dst
argument_list|,
name|rename_dst_alloc
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|rename_dst
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|rename_dst_nr
operator|++
expr_stmt|;
if|if
condition|(
name|first
operator|<
name|rename_dst_nr
condition|)
name|memmove
argument_list|(
name|rename_dst
operator|+
name|first
operator|+
literal|1
argument_list|,
name|rename_dst
operator|+
name|first
argument_list|,
operator|(
name|rename_dst_nr
operator|-
name|first
operator|-
literal|1
operator|)
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|rename_dst
argument_list|)
argument_list|)
expr_stmt|;
name|rename_dst
index|[
name|first
index|]
operator|.
name|two
operator|=
name|two
expr_stmt|;
name|rename_dst
index|[
name|first
index|]
operator|.
name|pair
operator|=
name|NULL
expr_stmt|;
return|return
operator|&
operator|(
name|rename_dst
index|[
name|first
index|]
operator|)
return|;
block|}
end_function

begin_struct
DECL|struct|diff_rename_src
specifier|static
struct|struct
name|diff_rename_src
block|{
DECL|member|one
name|struct
name|diff_filespec
modifier|*
name|one
decl_stmt|;
DECL|member|src_used
name|unsigned
name|src_used
range|:
literal|1
decl_stmt|;
DECL|variable|rename_src
block|}
modifier|*
name|rename_src
struct|;
end_struct

begin_decl_stmt
DECL|variable|rename_src_nr
DECL|variable|rename_src_alloc
specifier|static
name|int
name|rename_src_nr
decl_stmt|,
name|rename_src_alloc
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|locate_rename_src
specifier|static
name|struct
name|diff_rename_src
modifier|*
name|locate_rename_src
parameter_list|(
name|struct
name|diff_filespec
modifier|*
name|one
parameter_list|,
name|int
name|insert_ok
parameter_list|)
block|{
name|int
name|first
decl_stmt|,
name|last
decl_stmt|;
name|first
operator|=
literal|0
expr_stmt|;
name|last
operator|=
name|rename_src_nr
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
name|diff_rename_src
modifier|*
name|src
init|=
operator|&
operator|(
name|rename_src
index|[
name|next
index|]
operator|)
decl_stmt|;
name|int
name|cmp
init|=
name|strcmp
argument_list|(
name|one
operator|->
name|path
argument_list|,
name|src
operator|->
name|one
operator|->
name|path
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|cmp
condition|)
return|return
name|src
return|;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
block|{
name|last
operator|=
name|next
expr_stmt|;
continue|continue;
block|}
name|first
operator|=
name|next
operator|+
literal|1
expr_stmt|;
block|}
comment|/* not found */
if|if
condition|(
operator|!
name|insert_ok
condition|)
return|return
name|NULL
return|;
comment|/* insert to make it at "first" */
if|if
condition|(
name|rename_src_alloc
operator|<=
name|rename_src_nr
condition|)
block|{
name|rename_src_alloc
operator|=
name|alloc_nr
argument_list|(
name|rename_src_alloc
argument_list|)
expr_stmt|;
name|rename_src
operator|=
name|xrealloc
argument_list|(
name|rename_src
argument_list|,
name|rename_src_alloc
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|rename_src
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|rename_src_nr
operator|++
expr_stmt|;
if|if
condition|(
name|first
operator|<
name|rename_src_nr
condition|)
name|memmove
argument_list|(
name|rename_src
operator|+
name|first
operator|+
literal|1
argument_list|,
name|rename_src
operator|+
name|first
argument_list|,
operator|(
name|rename_src_nr
operator|-
name|first
operator|-
literal|1
operator|)
operator|*
sizeof|sizeof
argument_list|(
operator|*
name|rename_src
argument_list|)
argument_list|)
expr_stmt|;
name|rename_src
index|[
name|first
index|]
operator|.
name|one
operator|=
name|one
expr_stmt|;
name|rename_src
index|[
name|first
index|]
operator|.
name|src_used
operator|=
literal|0
expr_stmt|;
return|return
operator|&
operator|(
name|rename_src
index|[
name|first
index|]
operator|)
return|;
block|}
end_function

begin_function
DECL|function|is_exact_match
specifier|static
name|int
name|is_exact_match
parameter_list|(
name|struct
name|diff_filespec
modifier|*
name|src
parameter_list|,
name|struct
name|diff_filespec
modifier|*
name|dst
parameter_list|)
block|{
if|if
condition|(
name|src
operator|->
name|sha1_valid
operator|&&
name|dst
operator|->
name|sha1_valid
operator|&&
operator|!
name|memcmp
argument_list|(
name|src
operator|->
name|sha1
argument_list|,
name|dst
operator|->
name|sha1
argument_list|,
literal|20
argument_list|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|diff_populate_filespec
argument_list|(
name|src
argument_list|)
operator|||
name|diff_populate_filespec
argument_list|(
name|dst
argument_list|)
condition|)
comment|/* this is an error but will be caught downstream */
return|return
literal|0
return|;
if|if
condition|(
name|src
operator|->
name|size
operator|==
name|dst
operator|->
name|size
operator|&&
operator|!
name|memcmp
argument_list|(
name|src
operator|->
name|data
argument_list|,
name|dst
operator|->
name|data
argument_list|,
name|src
operator|->
name|size
argument_list|)
condition|)
return|return
literal|1
return|;
return|return
literal|0
return|;
block|}
end_function

begin_struct
DECL|struct|diff_score
struct|struct
name|diff_score
block|{
DECL|member|src
name|int
name|src
decl_stmt|;
comment|/* index in rename_src */
DECL|member|dst
name|int
name|dst
decl_stmt|;
comment|/* index in rename_dst */
DECL|member|score
name|int
name|score
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|estimate_similarity
specifier|static
name|int
name|estimate_similarity
parameter_list|(
name|struct
name|diff_filespec
modifier|*
name|src
parameter_list|,
name|struct
name|diff_filespec
modifier|*
name|dst
parameter_list|,
name|int
name|minimum_score
parameter_list|)
block|{
comment|/* src points at a file that existed in the original tree (or 	 * optionally a file in the destination tree) and dst points 	 * at a newly created file.  They may be quite similar, in which 	 * case we want to say src is renamed to dst or src is copied into 	 * dst, and then some edit has been applied to dst. 	 * 	 * Compare them and return how similar they are, representing 	 * the score as an integer between 0 and 10000, except 	 * where they match exactly it is considered better than anything 	 * else. 	 */
name|void
modifier|*
name|delta
decl_stmt|;
name|unsigned
name|long
name|delta_size
decl_stmt|,
name|base_size
decl_stmt|;
name|int
name|score
decl_stmt|;
comment|/* We deal only with regular files.  Symlink renames are handled 	 * only when they are exact matches --- in other words, no edits 	 * after renaming. 	 */
if|if
condition|(
operator|!
name|S_ISREG
argument_list|(
name|src
operator|->
name|mode
argument_list|)
operator|||
operator|!
name|S_ISREG
argument_list|(
name|dst
operator|->
name|mode
argument_list|)
condition|)
return|return
literal|0
return|;
name|delta_size
operator|=
operator|(
operator|(
name|src
operator|->
name|size
operator|<
name|dst
operator|->
name|size
operator|)
condition|?
operator|(
name|dst
operator|->
name|size
operator|-
name|src
operator|->
name|size
operator|)
else|:
operator|(
name|src
operator|->
name|size
operator|-
name|dst
operator|->
name|size
operator|)
operator|)
expr_stmt|;
name|base_size
operator|=
operator|(
operator|(
name|src
operator|->
name|size
operator|<
name|dst
operator|->
name|size
operator|)
condition|?
name|src
operator|->
name|size
else|:
name|dst
operator|->
name|size
operator|)
expr_stmt|;
comment|/* We would not consider edits that change the file size so 	 * drastically.  delta_size must be smaller than 	 * (MAX_SCORE-minimum_score)/MAX_SCORE * min(src->size, dst->size). 	 * Note that base_size == 0 case is handled here already 	 * and the final score computation below would not have a 	 * divide-by-zero issue. 	 */
if|if
condition|(
name|base_size
operator|*
operator|(
name|MAX_SCORE
operator|-
name|minimum_score
operator|)
operator|<
name|delta_size
operator|*
name|MAX_SCORE
condition|)
return|return
literal|0
return|;
name|delta
operator|=
name|diff_delta
argument_list|(
name|src
operator|->
name|data
argument_list|,
name|src
operator|->
name|size
argument_list|,
name|dst
operator|->
name|data
argument_list|,
name|dst
operator|->
name|size
argument_list|,
operator|&
name|delta_size
argument_list|)
expr_stmt|;
comment|/* A delta that has a lot of literal additions would have 	 * big delta_size no matter what else it does. 	 */
if|if
condition|(
name|base_size
operator|*
operator|(
name|MAX_SCORE
operator|-
name|minimum_score
operator|)
operator|<
name|delta_size
operator|*
name|MAX_SCORE
condition|)
return|return
literal|0
return|;
comment|/* Estimate the edit size by interpreting delta. */
name|delta_size
operator|=
name|count_delta
argument_list|(
name|delta
argument_list|,
name|delta_size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|delta
argument_list|)
expr_stmt|;
if|if
condition|(
name|delta_size
operator|==
name|UINT_MAX
condition|)
return|return
literal|0
return|;
comment|/* 	 * Now we will give some score to it.  100% edit gets 0 points 	 * and 0% edit gets MAX_SCORE points. 	 */
name|score
operator|=
name|MAX_SCORE
operator|-
operator|(
name|MAX_SCORE
operator|*
name|delta_size
operator|/
name|base_size
operator|)
expr_stmt|;
if|if
condition|(
name|score
operator|<
literal|0
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|MAX_SCORE
operator|<
name|score
condition|)
return|return
name|MAX_SCORE
return|;
return|return
name|score
return|;
block|}
end_function

begin_function
DECL|function|record_rename_pair
specifier|static
name|void
name|record_rename_pair
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
name|renq
parameter_list|,
name|int
name|dst_index
parameter_list|,
name|int
name|src_index
parameter_list|,
name|int
name|score
parameter_list|)
block|{
name|struct
name|diff_filespec
modifier|*
name|one
decl_stmt|,
modifier|*
name|two
decl_stmt|,
modifier|*
name|src
decl_stmt|,
modifier|*
name|dst
decl_stmt|;
name|struct
name|diff_filepair
modifier|*
name|dp
decl_stmt|;
if|if
condition|(
name|rename_dst
index|[
name|dst_index
index|]
operator|.
name|pair
condition|)
name|die
argument_list|(
literal|"internal error: dst already matched."
argument_list|)
expr_stmt|;
name|src
operator|=
name|rename_src
index|[
name|src_index
index|]
operator|.
name|one
expr_stmt|;
name|one
operator|=
name|alloc_filespec
argument_list|(
name|src
operator|->
name|path
argument_list|)
expr_stmt|;
name|fill_filespec
argument_list|(
name|one
argument_list|,
name|src
operator|->
name|sha1
argument_list|,
name|src
operator|->
name|mode
argument_list|)
expr_stmt|;
name|dst
operator|=
name|rename_dst
index|[
name|dst_index
index|]
operator|.
name|two
expr_stmt|;
name|two
operator|=
name|alloc_filespec
argument_list|(
name|dst
operator|->
name|path
argument_list|)
expr_stmt|;
name|fill_filespec
argument_list|(
name|two
argument_list|,
name|dst
operator|->
name|sha1
argument_list|,
name|dst
operator|->
name|mode
argument_list|)
expr_stmt|;
name|dp
operator|=
name|diff_queue
argument_list|(
name|renq
argument_list|,
name|one
argument_list|,
name|two
argument_list|)
expr_stmt|;
name|dp
operator|->
name|score
operator|=
name|score
expr_stmt|;
name|rename_src
index|[
name|src_index
index|]
operator|.
name|src_used
operator|=
literal|1
expr_stmt|;
name|rename_dst
index|[
name|dst_index
index|]
operator|.
name|pair
operator|=
name|dp
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * We sort the rename similarity matrix with the score, in descending  * order (more similar first).  */
end_comment

begin_function
DECL|function|score_compare
specifier|static
name|int
name|score_compare
parameter_list|(
specifier|const
name|void
modifier|*
name|a_
parameter_list|,
specifier|const
name|void
modifier|*
name|b_
parameter_list|)
block|{
specifier|const
name|struct
name|diff_score
modifier|*
name|a
init|=
name|a_
decl_stmt|,
modifier|*
name|b
init|=
name|b_
decl_stmt|;
return|return
name|b
operator|->
name|score
operator|-
name|a
operator|->
name|score
return|;
block|}
end_function

begin_function
DECL|function|diff_scoreopt_parse
name|int
name|diff_scoreopt_parse
parameter_list|(
specifier|const
name|char
modifier|*
name|opt
parameter_list|)
block|{
name|int
name|diglen
decl_stmt|,
name|num
decl_stmt|,
name|scale
decl_stmt|,
name|i
decl_stmt|;
if|if
condition|(
name|opt
index|[
literal|0
index|]
operator|!=
literal|'-'
operator|||
operator|(
name|opt
index|[
literal|1
index|]
operator|!=
literal|'M'
operator|&&
name|opt
index|[
literal|1
index|]
operator|!=
literal|'C'
operator|)
condition|)
return|return
operator|-
literal|1
return|;
comment|/* that is not a -M nor -C option */
name|diglen
operator|=
name|strspn
argument_list|(
name|opt
operator|+
literal|2
argument_list|,
literal|"0123456789"
argument_list|)
expr_stmt|;
if|if
condition|(
name|diglen
operator|==
literal|0
operator|||
name|strlen
argument_list|(
name|opt
operator|+
literal|2
argument_list|)
operator|!=
name|diglen
condition|)
return|return
literal|0
return|;
comment|/* use default */
name|sscanf
argument_list|(
name|opt
operator|+
literal|2
argument_list|,
literal|"%d"
argument_list|,
operator|&
name|num
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
operator|,
name|scale
operator|=
literal|1
init|;
name|i
operator|<
name|diglen
condition|;
name|i
operator|++
control|)
name|scale
operator|*=
literal|10
expr_stmt|;
comment|/* user says num divided by scale and we say internally that 	 * is MAX_SCORE * num / scale. 	 */
return|return
name|MAX_SCORE
operator|*
name|num
operator|/
name|scale
return|;
block|}
end_function

begin_function
DECL|function|diffcore_rename
name|void
name|diffcore_rename
parameter_list|(
name|int
name|detect_rename
parameter_list|,
name|int
name|minimum_score
parameter_list|)
block|{
name|struct
name|diff_queue_struct
modifier|*
name|q
init|=
operator|&
name|diff_queued_diff
decl_stmt|;
name|struct
name|diff_queue_struct
name|renq
decl_stmt|,
name|outq
decl_stmt|;
name|struct
name|diff_score
modifier|*
name|mx
decl_stmt|;
name|int
name|i
decl_stmt|,
name|j
decl_stmt|;
name|int
name|num_create
decl_stmt|,
name|num_src
decl_stmt|,
name|dst_cnt
decl_stmt|;
if|if
condition|(
operator|!
name|minimum_score
condition|)
name|minimum_score
operator|=
name|DEFAULT_MINIMUM_SCORE
expr_stmt|;
name|renq
operator|.
name|queue
operator|=
name|NULL
expr_stmt|;
name|renq
operator|.
name|nr
operator|=
name|renq
operator|.
name|alloc
operator|=
literal|0
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
block|{
name|struct
name|diff_filepair
modifier|*
name|p
init|=
name|q
operator|->
name|queue
index|[
name|i
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|DIFF_FILE_VALID
argument_list|(
name|p
operator|->
name|one
argument_list|)
condition|)
if|if
condition|(
operator|!
name|DIFF_FILE_VALID
argument_list|(
name|p
operator|->
name|two
argument_list|)
condition|)
continue|continue;
comment|/* unmerged */
else|else
name|locate_rename_dst
argument_list|(
name|p
operator|->
name|two
argument_list|,
literal|1
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
operator|!
name|DIFF_FILE_VALID
argument_list|(
name|p
operator|->
name|two
argument_list|)
condition|)
name|locate_rename_src
argument_list|(
name|p
operator|->
name|one
argument_list|,
literal|1
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
literal|1
operator|<
name|detect_rename
condition|)
comment|/* find copy, too */
name|locate_rename_src
argument_list|(
name|p
operator|->
name|one
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|rename_dst_nr
operator|==
literal|0
condition|)
goto|goto
name|cleanup
goto|;
comment|/* nothing to do */
comment|/* We really want to cull the candidates list early 	 * with cheap tests in order to avoid doing deltas. 	 */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|rename_dst_nr
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|diff_filespec
modifier|*
name|two
init|=
name|rename_dst
index|[
name|i
index|]
operator|.
name|two
decl_stmt|;
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|rename_src_nr
condition|;
name|j
operator|++
control|)
block|{
name|struct
name|diff_filespec
modifier|*
name|one
init|=
name|rename_src
index|[
name|j
index|]
operator|.
name|one
decl_stmt|;
if|if
condition|(
operator|!
name|is_exact_match
argument_list|(
name|one
argument_list|,
name|two
argument_list|)
condition|)
continue|continue;
name|record_rename_pair
argument_list|(
operator|&
name|renq
argument_list|,
name|i
argument_list|,
name|j
argument_list|,
name|MAX_SCORE
argument_list|)
expr_stmt|;
break|break;
comment|/* we are done with this entry */
block|}
block|}
name|diff_debug_queue
argument_list|(
literal|"done detecting exact"
argument_list|,
operator|&
name|renq
argument_list|)
expr_stmt|;
comment|/* Have we run out the created file pool?  If so we can avoid 	 * doing the delta matrix altogether. 	 */
if|if
condition|(
name|renq
operator|.
name|nr
operator|==
name|rename_dst_nr
condition|)
goto|goto
name|flush_rest
goto|;
name|num_create
operator|=
operator|(
name|rename_dst_nr
operator|-
name|renq
operator|.
name|nr
operator|)
expr_stmt|;
name|num_src
operator|=
name|rename_src_nr
expr_stmt|;
name|mx
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|mx
argument_list|)
operator|*
name|num_create
operator|*
name|num_src
argument_list|)
expr_stmt|;
for|for
control|(
name|dst_cnt
operator|=
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|rename_dst_nr
condition|;
name|i
operator|++
control|)
block|{
name|int
name|base
init|=
name|dst_cnt
operator|*
name|num_src
decl_stmt|;
name|struct
name|diff_filespec
modifier|*
name|two
init|=
name|rename_dst
index|[
name|i
index|]
operator|.
name|two
decl_stmt|;
if|if
condition|(
name|rename_dst
index|[
name|i
index|]
operator|.
name|pair
condition|)
continue|continue;
comment|/* dealt with exact match already. */
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|rename_src_nr
condition|;
name|j
operator|++
control|)
block|{
name|struct
name|diff_filespec
modifier|*
name|one
init|=
name|rename_src
index|[
name|j
index|]
operator|.
name|one
decl_stmt|;
name|struct
name|diff_score
modifier|*
name|m
init|=
operator|&
name|mx
index|[
name|base
operator|+
name|j
index|]
decl_stmt|;
name|m
operator|->
name|src
operator|=
name|j
expr_stmt|;
name|m
operator|->
name|dst
operator|=
name|i
expr_stmt|;
name|m
operator|->
name|score
operator|=
name|estimate_similarity
argument_list|(
name|one
argument_list|,
name|two
argument_list|,
name|minimum_score
argument_list|)
expr_stmt|;
block|}
name|dst_cnt
operator|++
expr_stmt|;
block|}
comment|/* cost matrix sorted by most to least similar pair */
name|qsort
argument_list|(
name|mx
argument_list|,
name|num_create
operator|*
name|num_src
argument_list|,
sizeof|sizeof
argument_list|(
operator|*
name|mx
argument_list|)
argument_list|,
name|score_compare
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
name|num_create
operator|*
name|num_src
condition|;
name|i
operator|++
control|)
block|{
name|struct
name|diff_rename_dst
modifier|*
name|dst
init|=
operator|&
name|rename_dst
index|[
name|mx
index|[
name|i
index|]
operator|.
name|dst
index|]
decl_stmt|;
if|if
condition|(
name|dst
operator|->
name|pair
condition|)
continue|continue;
comment|/* already done, either exact or fuzzy. */
if|if
condition|(
name|mx
index|[
name|i
index|]
operator|.
name|score
operator|<
name|minimum_score
condition|)
break|break;
comment|/* there is not any more diffs applicable. */
name|record_rename_pair
argument_list|(
operator|&
name|renq
argument_list|,
name|mx
index|[
name|i
index|]
operator|.
name|dst
argument_list|,
name|mx
index|[
name|i
index|]
operator|.
name|src
argument_list|,
name|mx
index|[
name|i
index|]
operator|.
name|score
argument_list|)
expr_stmt|;
block|}
name|free
argument_list|(
name|mx
argument_list|)
expr_stmt|;
name|diff_debug_queue
argument_list|(
literal|"done detecting fuzzy"
argument_list|,
operator|&
name|renq
argument_list|)
expr_stmt|;
name|flush_rest
label|:
comment|/* At this point, we have found some renames and copies and they 	 * are kept in renq.  The original list is still in *q. 	 * 	 * Scan the original list and move them into the outq; we will sort 	 * outq and swap it into the queue supplied to pass that to 	 * downstream, so we assign the sort keys in this loop. 	 * 	 * See comments at the top of record_rename_pair for numbers used 	 * to assign rename_rank. 	 */
name|outq
operator|.
name|queue
operator|=
name|NULL
expr_stmt|;
name|outq
operator|.
name|nr
operator|=
name|outq
operator|.
name|alloc
operator|=
literal|0
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
block|{
name|struct
name|diff_filepair
modifier|*
name|p
init|=
name|q
operator|->
name|queue
index|[
name|i
index|]
decl_stmt|;
name|struct
name|diff_rename_src
modifier|*
name|src
init|=
name|locate_rename_src
argument_list|(
name|p
operator|->
name|one
argument_list|,
literal|0
argument_list|)
decl_stmt|;
name|struct
name|diff_rename_dst
modifier|*
name|dst
init|=
name|locate_rename_dst
argument_list|(
name|p
operator|->
name|two
argument_list|,
literal|0
argument_list|)
decl_stmt|;
name|struct
name|diff_filepair
modifier|*
name|pair_to_free
init|=
name|NULL
decl_stmt|;
if|if
condition|(
name|dst
condition|)
block|{
comment|/* creation */
if|if
condition|(
name|dst
operator|->
name|pair
condition|)
block|{
comment|/* renq has rename/copy already to produce 				 * this file, so we do not emit the creation 				 * record in the output. 				 */
name|diff_q
argument_list|(
operator|&
name|outq
argument_list|,
name|dst
operator|->
name|pair
argument_list|)
expr_stmt|;
name|pair_to_free
operator|=
name|p
expr_stmt|;
block|}
else|else
comment|/* no matching rename/copy source, so record 				 * this as a creation. 				 */
name|diff_q
argument_list|(
operator|&
name|outq
argument_list|,
name|p
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|diff_unmodified_pair
argument_list|(
name|p
argument_list|)
condition|)
comment|/* all the other cases need to be recorded as is */
name|diff_q
argument_list|(
operator|&
name|outq
argument_list|,
name|p
argument_list|)
expr_stmt|;
else|else
block|{
comment|/* unmodified pair needs to be recorded only if 			 * it is used as the source of rename/copy 			 */
if|if
condition|(
name|src
operator|&&
name|src
operator|->
name|src_used
condition|)
name|diff_q
argument_list|(
operator|&
name|outq
argument_list|,
name|p
argument_list|)
expr_stmt|;
else|else
name|pair_to_free
operator|=
name|p
expr_stmt|;
block|}
if|if
condition|(
name|pair_to_free
condition|)
name|diff_free_filepair
argument_list|(
name|pair_to_free
argument_list|)
expr_stmt|;
block|}
name|diff_debug_queue
argument_list|(
literal|"done copying original"
argument_list|,
operator|&
name|outq
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|renq
operator|.
name|queue
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|q
operator|->
name|queue
argument_list|)
expr_stmt|;
operator|*
name|q
operator|=
name|outq
expr_stmt|;
name|diff_debug_queue
argument_list|(
literal|"done collapsing"
argument_list|,
name|q
argument_list|)
expr_stmt|;
name|cleanup
label|:
name|free
argument_list|(
name|rename_dst
argument_list|)
expr_stmt|;
name|rename_dst
operator|=
name|NULL
expr_stmt|;
name|rename_dst_nr
operator|=
name|rename_dst_alloc
operator|=
literal|0
expr_stmt|;
name|free
argument_list|(
name|rename_src
argument_list|)
expr_stmt|;
name|rename_src
operator|=
name|NULL
expr_stmt|;
name|rename_src_nr
operator|=
name|rename_src_alloc
operator|=
literal|0
expr_stmt|;
return|return;
block|}
end_function

end_unit

