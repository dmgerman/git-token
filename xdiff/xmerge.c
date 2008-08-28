begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  *  LibXDiff by Davide Libenzi ( File Differential Library )  *  Copyright (C) 2003-2006 Davide Libenzi, Johannes E. Schindelin  *  *  This library is free software; you can redistribute it and/or  *  modify it under the terms of the GNU Lesser General Public  *  License as published by the Free Software Foundation; either  *  version 2.1 of the License, or (at your option) any later version.  *  *  This library is distributed in the hope that it will be useful,  *  but WITHOUT ANY WARRANTY; without even the implied warranty of  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *  Lesser General Public License for more details.  *  *  You should have received a copy of the GNU Lesser General Public  *  License along with this library; if not, write to the Free Software  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *  *  Davide Libenzi<davidel@xmailserver.org>  *  */
end_comment

begin_include
include|#
directive|include
file|"xinclude.h"
end_include

begin_typedef
DECL|struct|s_xdmerge
typedef|typedef
struct|struct
name|s_xdmerge
block|{
DECL|member|next
name|struct
name|s_xdmerge
modifier|*
name|next
decl_stmt|;
comment|/* 	 * 0 = conflict, 	 * 1 = no conflict, take first, 	 * 2 = no conflict, take second. 	 */
DECL|member|mode
name|int
name|mode
decl_stmt|;
DECL|member|i1
DECL|member|i2
name|long
name|i1
decl_stmt|,
name|i2
decl_stmt|;
DECL|member|chg1
DECL|member|chg2
name|long
name|chg1
decl_stmt|,
name|chg2
decl_stmt|;
DECL|typedef|xdmerge_t
block|}
name|xdmerge_t
typedef|;
end_typedef

begin_function
DECL|function|xdl_append_merge
specifier|static
name|int
name|xdl_append_merge
parameter_list|(
name|xdmerge_t
modifier|*
modifier|*
name|merge
parameter_list|,
name|int
name|mode
parameter_list|,
name|long
name|i1
parameter_list|,
name|long
name|chg1
parameter_list|,
name|long
name|i2
parameter_list|,
name|long
name|chg2
parameter_list|)
block|{
name|xdmerge_t
modifier|*
name|m
init|=
operator|*
name|merge
decl_stmt|;
if|if
condition|(
name|m
operator|&&
operator|(
name|i1
operator|<=
name|m
operator|->
name|i1
operator|+
name|m
operator|->
name|chg1
operator|||
name|i2
operator|<=
name|m
operator|->
name|i2
operator|+
name|m
operator|->
name|chg2
operator|)
condition|)
block|{
if|if
condition|(
name|mode
operator|!=
name|m
operator|->
name|mode
condition|)
name|m
operator|->
name|mode
operator|=
literal|0
expr_stmt|;
name|m
operator|->
name|chg1
operator|=
name|i1
operator|+
name|chg1
operator|-
name|m
operator|->
name|i1
expr_stmt|;
name|m
operator|->
name|chg2
operator|=
name|i2
operator|+
name|chg2
operator|-
name|m
operator|->
name|i2
expr_stmt|;
block|}
else|else
block|{
name|m
operator|=
name|xdl_malloc
argument_list|(
sizeof|sizeof
argument_list|(
name|xdmerge_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|m
condition|)
return|return
operator|-
literal|1
return|;
name|m
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|m
operator|->
name|mode
operator|=
name|mode
expr_stmt|;
name|m
operator|->
name|i1
operator|=
name|i1
expr_stmt|;
name|m
operator|->
name|chg1
operator|=
name|chg1
expr_stmt|;
name|m
operator|->
name|i2
operator|=
name|i2
expr_stmt|;
name|m
operator|->
name|chg2
operator|=
name|chg2
expr_stmt|;
if|if
condition|(
operator|*
name|merge
condition|)
operator|(
operator|*
name|merge
operator|)
operator|->
name|next
operator|=
name|m
expr_stmt|;
operator|*
name|merge
operator|=
name|m
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|xdl_cleanup_merge
specifier|static
name|int
name|xdl_cleanup_merge
parameter_list|(
name|xdmerge_t
modifier|*
name|c
parameter_list|)
block|{
name|int
name|count
init|=
literal|0
decl_stmt|;
name|xdmerge_t
modifier|*
name|next_c
decl_stmt|;
comment|/* were there conflicts? */
for|for
control|(
init|;
name|c
condition|;
name|c
operator|=
name|next_c
control|)
block|{
if|if
condition|(
name|c
operator|->
name|mode
operator|==
literal|0
condition|)
name|count
operator|++
expr_stmt|;
name|next_c
operator|=
name|c
operator|->
name|next
expr_stmt|;
name|free
argument_list|(
name|c
argument_list|)
expr_stmt|;
block|}
return|return
name|count
return|;
block|}
end_function

begin_function
DECL|function|xdl_merge_cmp_lines
specifier|static
name|int
name|xdl_merge_cmp_lines
parameter_list|(
name|xdfenv_t
modifier|*
name|xe1
parameter_list|,
name|int
name|i1
parameter_list|,
name|xdfenv_t
modifier|*
name|xe2
parameter_list|,
name|int
name|i2
parameter_list|,
name|int
name|line_count
parameter_list|,
name|long
name|flags
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|xrecord_t
modifier|*
modifier|*
name|rec1
init|=
name|xe1
operator|->
name|xdf2
operator|.
name|recs
operator|+
name|i1
decl_stmt|;
name|xrecord_t
modifier|*
modifier|*
name|rec2
init|=
name|xe2
operator|->
name|xdf2
operator|.
name|recs
operator|+
name|i2
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|line_count
condition|;
name|i
operator|++
control|)
block|{
name|int
name|result
init|=
name|xdl_recmatch
argument_list|(
name|rec1
index|[
name|i
index|]
operator|->
name|ptr
argument_list|,
name|rec1
index|[
name|i
index|]
operator|->
name|size
argument_list|,
name|rec2
index|[
name|i
index|]
operator|->
name|ptr
argument_list|,
name|rec2
index|[
name|i
index|]
operator|->
name|size
argument_list|,
name|flags
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|result
condition|)
return|return
operator|-
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|xdl_recs_copy
specifier|static
name|int
name|xdl_recs_copy
parameter_list|(
name|xdfenv_t
modifier|*
name|xe
parameter_list|,
name|int
name|i
parameter_list|,
name|int
name|count
parameter_list|,
name|int
name|add_nl
parameter_list|,
name|char
modifier|*
name|dest
parameter_list|)
block|{
name|xrecord_t
modifier|*
modifier|*
name|recs
init|=
name|xe
operator|->
name|xdf2
operator|.
name|recs
operator|+
name|i
decl_stmt|;
name|int
name|size
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|count
operator|<
literal|1
condition|)
return|return
literal|0
return|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|count
condition|;
name|size
operator|+=
name|recs
index|[
name|i
operator|++
index|]
operator|->
name|size
control|)
if|if
condition|(
name|dest
condition|)
name|memcpy
argument_list|(
name|dest
operator|+
name|size
argument_list|,
name|recs
index|[
name|i
index|]
operator|->
name|ptr
argument_list|,
name|recs
index|[
name|i
index|]
operator|->
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|add_nl
condition|)
block|{
name|i
operator|=
name|recs
index|[
name|count
operator|-
literal|1
index|]
operator|->
name|size
expr_stmt|;
if|if
condition|(
name|i
operator|==
literal|0
operator|||
name|recs
index|[
name|count
operator|-
literal|1
index|]
operator|->
name|ptr
index|[
name|i
operator|-
literal|1
index|]
operator|!=
literal|'\n'
condition|)
block|{
if|if
condition|(
name|dest
condition|)
name|dest
index|[
name|size
index|]
operator|=
literal|'\n'
expr_stmt|;
name|size
operator|++
expr_stmt|;
block|}
block|}
return|return
name|size
return|;
block|}
end_function

begin_function
DECL|function|fill_conflict_hunk
specifier|static
name|int
name|fill_conflict_hunk
parameter_list|(
name|xdfenv_t
modifier|*
name|xe1
parameter_list|,
specifier|const
name|char
modifier|*
name|name1
parameter_list|,
name|xdfenv_t
modifier|*
name|xe2
parameter_list|,
specifier|const
name|char
modifier|*
name|name2
parameter_list|,
name|int
name|size
parameter_list|,
name|int
name|i
parameter_list|,
name|xdmerge_t
modifier|*
name|m
parameter_list|,
name|char
modifier|*
name|dest
parameter_list|)
block|{
specifier|const
name|int
name|marker_size
init|=
literal|7
decl_stmt|;
name|int
name|marker1_size
init|=
operator|(
name|name1
condition|?
name|strlen
argument_list|(
name|name1
argument_list|)
operator|+
literal|1
else|:
literal|0
operator|)
decl_stmt|;
name|int
name|marker2_size
init|=
operator|(
name|name2
condition|?
name|strlen
argument_list|(
name|name2
argument_list|)
operator|+
literal|1
else|:
literal|0
operator|)
decl_stmt|;
name|int
name|j
decl_stmt|;
comment|/* Before conflicting part */
name|size
operator|+=
name|xdl_recs_copy
argument_list|(
name|xe1
argument_list|,
name|i
argument_list|,
name|m
operator|->
name|i1
operator|-
name|i
argument_list|,
literal|0
argument_list|,
name|dest
condition|?
name|dest
operator|+
name|size
else|:
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dest
condition|)
block|{
name|size
operator|+=
name|marker_size
operator|+
literal|1
operator|+
name|marker1_size
expr_stmt|;
block|}
else|else
block|{
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|marker_size
condition|;
name|j
operator|++
control|)
name|dest
index|[
name|size
operator|++
index|]
operator|=
literal|'<'
expr_stmt|;
if|if
condition|(
name|marker1_size
condition|)
block|{
name|dest
index|[
name|size
index|]
operator|=
literal|' '
expr_stmt|;
name|memcpy
argument_list|(
name|dest
operator|+
name|size
operator|+
literal|1
argument_list|,
name|name1
argument_list|,
name|marker1_size
operator|-
literal|1
argument_list|)
expr_stmt|;
name|size
operator|+=
name|marker1_size
expr_stmt|;
block|}
name|dest
index|[
name|size
operator|++
index|]
operator|=
literal|'\n'
expr_stmt|;
block|}
comment|/* Postimage from side #1 */
name|size
operator|+=
name|xdl_recs_copy
argument_list|(
name|xe1
argument_list|,
name|m
operator|->
name|i1
argument_list|,
name|m
operator|->
name|chg1
argument_list|,
literal|1
argument_list|,
name|dest
condition|?
name|dest
operator|+
name|size
else|:
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dest
condition|)
block|{
name|size
operator|+=
name|marker_size
operator|+
literal|1
expr_stmt|;
block|}
else|else
block|{
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|marker_size
condition|;
name|j
operator|++
control|)
name|dest
index|[
name|size
operator|++
index|]
operator|=
literal|'='
expr_stmt|;
name|dest
index|[
name|size
operator|++
index|]
operator|=
literal|'\n'
expr_stmt|;
block|}
comment|/* Postimage from side #2 */
name|size
operator|+=
name|xdl_recs_copy
argument_list|(
name|xe2
argument_list|,
name|m
operator|->
name|i2
argument_list|,
name|m
operator|->
name|chg2
argument_list|,
literal|1
argument_list|,
name|dest
condition|?
name|dest
operator|+
name|size
else|:
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dest
condition|)
block|{
name|size
operator|+=
name|marker_size
operator|+
literal|1
operator|+
name|marker2_size
expr_stmt|;
block|}
else|else
block|{
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|marker_size
condition|;
name|j
operator|++
control|)
name|dest
index|[
name|size
operator|++
index|]
operator|=
literal|'>'
expr_stmt|;
if|if
condition|(
name|marker2_size
condition|)
block|{
name|dest
index|[
name|size
index|]
operator|=
literal|' '
expr_stmt|;
name|memcpy
argument_list|(
name|dest
operator|+
name|size
operator|+
literal|1
argument_list|,
name|name2
argument_list|,
name|marker2_size
operator|-
literal|1
argument_list|)
expr_stmt|;
name|size
operator|+=
name|marker2_size
expr_stmt|;
block|}
name|dest
index|[
name|size
operator|++
index|]
operator|=
literal|'\n'
expr_stmt|;
block|}
return|return
name|size
return|;
block|}
end_function

begin_function
DECL|function|xdl_fill_merge_buffer
specifier|static
name|int
name|xdl_fill_merge_buffer
parameter_list|(
name|xdfenv_t
modifier|*
name|xe1
parameter_list|,
specifier|const
name|char
modifier|*
name|name1
parameter_list|,
name|xdfenv_t
modifier|*
name|xe2
parameter_list|,
specifier|const
name|char
modifier|*
name|name2
parameter_list|,
name|xdmerge_t
modifier|*
name|m
parameter_list|,
name|char
modifier|*
name|dest
parameter_list|)
block|{
name|int
name|size
decl_stmt|,
name|i
decl_stmt|;
for|for
control|(
name|size
operator|=
name|i
operator|=
literal|0
init|;
name|m
condition|;
name|m
operator|=
name|m
operator|->
name|next
control|)
block|{
if|if
condition|(
name|m
operator|->
name|mode
operator|==
literal|0
condition|)
name|size
operator|=
name|fill_conflict_hunk
argument_list|(
name|xe1
argument_list|,
name|name1
argument_list|,
name|xe2
argument_list|,
name|name2
argument_list|,
name|size
argument_list|,
name|i
argument_list|,
name|m
argument_list|,
name|dest
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|m
operator|->
name|mode
operator|==
literal|1
condition|)
name|size
operator|+=
name|xdl_recs_copy
argument_list|(
name|xe1
argument_list|,
name|i
argument_list|,
name|m
operator|->
name|i1
operator|+
name|m
operator|->
name|chg1
operator|-
name|i
argument_list|,
literal|0
argument_list|,
name|dest
condition|?
name|dest
operator|+
name|size
else|:
name|NULL
argument_list|)
expr_stmt|;
elseif|else
if|if
condition|(
name|m
operator|->
name|mode
operator|==
literal|2
condition|)
name|size
operator|+=
name|xdl_recs_copy
argument_list|(
name|xe2
argument_list|,
name|m
operator|->
name|i2
operator|-
name|m
operator|->
name|i1
operator|+
name|i
argument_list|,
name|m
operator|->
name|i1
operator|+
name|m
operator|->
name|chg2
operator|-
name|i
argument_list|,
literal|0
argument_list|,
name|dest
condition|?
name|dest
operator|+
name|size
else|:
name|NULL
argument_list|)
expr_stmt|;
else|else
continue|continue;
name|i
operator|=
name|m
operator|->
name|i1
operator|+
name|m
operator|->
name|chg1
expr_stmt|;
block|}
name|size
operator|+=
name|xdl_recs_copy
argument_list|(
name|xe1
argument_list|,
name|i
argument_list|,
name|xe1
operator|->
name|xdf2
operator|.
name|nrec
operator|-
name|i
argument_list|,
literal|0
argument_list|,
name|dest
condition|?
name|dest
operator|+
name|size
else|:
name|NULL
argument_list|)
expr_stmt|;
return|return
name|size
return|;
block|}
end_function

begin_comment
comment|/*  * Sometimes, changes are not quite identical, but differ in only a few  * lines. Try hard to show only these few lines as conflicting.  */
end_comment

begin_function
DECL|function|xdl_refine_conflicts
specifier|static
name|int
name|xdl_refine_conflicts
parameter_list|(
name|xdfenv_t
modifier|*
name|xe1
parameter_list|,
name|xdfenv_t
modifier|*
name|xe2
parameter_list|,
name|xdmerge_t
modifier|*
name|m
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|)
block|{
for|for
control|(
init|;
name|m
condition|;
name|m
operator|=
name|m
operator|->
name|next
control|)
block|{
name|mmfile_t
name|t1
decl_stmt|,
name|t2
decl_stmt|;
name|xdfenv_t
name|xe
decl_stmt|;
name|xdchange_t
modifier|*
name|xscr
decl_stmt|,
modifier|*
name|x
decl_stmt|;
name|int
name|i1
init|=
name|m
operator|->
name|i1
decl_stmt|,
name|i2
init|=
name|m
operator|->
name|i2
decl_stmt|;
comment|/* let's handle just the conflicts */
if|if
condition|(
name|m
operator|->
name|mode
condition|)
continue|continue;
comment|/* no sense refining a conflict when one side is empty */
if|if
condition|(
name|m
operator|->
name|chg1
operator|==
literal|0
operator|||
name|m
operator|->
name|chg2
operator|==
literal|0
condition|)
continue|continue;
comment|/* 		 * This probably does not work outside git, since 		 * we have a very simple mmfile structure. 		 */
name|t1
operator|.
name|ptr
operator|=
operator|(
name|char
operator|*
operator|)
name|xe1
operator|->
name|xdf2
operator|.
name|recs
index|[
name|m
operator|->
name|i1
index|]
operator|->
name|ptr
expr_stmt|;
name|t1
operator|.
name|size
operator|=
name|xe1
operator|->
name|xdf2
operator|.
name|recs
index|[
name|m
operator|->
name|i1
operator|+
name|m
operator|->
name|chg1
operator|-
literal|1
index|]
operator|->
name|ptr
operator|+
name|xe1
operator|->
name|xdf2
operator|.
name|recs
index|[
name|m
operator|->
name|i1
operator|+
name|m
operator|->
name|chg1
operator|-
literal|1
index|]
operator|->
name|size
operator|-
name|t1
operator|.
name|ptr
expr_stmt|;
name|t2
operator|.
name|ptr
operator|=
operator|(
name|char
operator|*
operator|)
name|xe2
operator|->
name|xdf2
operator|.
name|recs
index|[
name|m
operator|->
name|i2
index|]
operator|->
name|ptr
expr_stmt|;
name|t2
operator|.
name|size
operator|=
name|xe2
operator|->
name|xdf2
operator|.
name|recs
index|[
name|m
operator|->
name|i2
operator|+
name|m
operator|->
name|chg2
operator|-
literal|1
index|]
operator|->
name|ptr
operator|+
name|xe2
operator|->
name|xdf2
operator|.
name|recs
index|[
name|m
operator|->
name|i2
operator|+
name|m
operator|->
name|chg2
operator|-
literal|1
index|]
operator|->
name|size
operator|-
name|t2
operator|.
name|ptr
expr_stmt|;
if|if
condition|(
name|xdl_do_diff
argument_list|(
operator|&
name|t1
argument_list|,
operator|&
name|t2
argument_list|,
name|xpp
argument_list|,
operator|&
name|xe
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|xdl_change_compact
argument_list|(
operator|&
name|xe
operator|.
name|xdf1
argument_list|,
operator|&
name|xe
operator|.
name|xdf2
argument_list|,
name|xpp
operator|->
name|flags
argument_list|)
operator|<
literal|0
operator|||
name|xdl_change_compact
argument_list|(
operator|&
name|xe
operator|.
name|xdf2
argument_list|,
operator|&
name|xe
operator|.
name|xdf1
argument_list|,
name|xpp
operator|->
name|flags
argument_list|)
operator|<
literal|0
operator|||
name|xdl_build_script
argument_list|(
operator|&
name|xe
argument_list|,
operator|&
name|xscr
argument_list|)
operator|<
literal|0
condition|)
block|{
name|xdl_free_env
argument_list|(
operator|&
name|xe
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
operator|!
name|xscr
condition|)
block|{
comment|/* If this happens, the changes are identical. */
name|xdl_free_env
argument_list|(
operator|&
name|xe
argument_list|)
expr_stmt|;
name|m
operator|->
name|mode
operator|=
literal|4
expr_stmt|;
continue|continue;
block|}
name|x
operator|=
name|xscr
expr_stmt|;
name|m
operator|->
name|i1
operator|=
name|xscr
operator|->
name|i1
operator|+
name|i1
expr_stmt|;
name|m
operator|->
name|chg1
operator|=
name|xscr
operator|->
name|chg1
expr_stmt|;
name|m
operator|->
name|i2
operator|=
name|xscr
operator|->
name|i2
operator|+
name|i2
expr_stmt|;
name|m
operator|->
name|chg2
operator|=
name|xscr
operator|->
name|chg2
expr_stmt|;
while|while
condition|(
name|xscr
operator|->
name|next
condition|)
block|{
name|xdmerge_t
modifier|*
name|m2
init|=
name|xdl_malloc
argument_list|(
sizeof|sizeof
argument_list|(
name|xdmerge_t
argument_list|)
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|m2
condition|)
block|{
name|xdl_free_env
argument_list|(
operator|&
name|xe
argument_list|)
expr_stmt|;
name|xdl_free_script
argument_list|(
name|x
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|xscr
operator|=
name|xscr
operator|->
name|next
expr_stmt|;
name|m2
operator|->
name|next
operator|=
name|m
operator|->
name|next
expr_stmt|;
name|m
operator|->
name|next
operator|=
name|m2
expr_stmt|;
name|m
operator|=
name|m2
expr_stmt|;
name|m
operator|->
name|mode
operator|=
literal|0
expr_stmt|;
name|m
operator|->
name|i1
operator|=
name|xscr
operator|->
name|i1
operator|+
name|i1
expr_stmt|;
name|m
operator|->
name|chg1
operator|=
name|xscr
operator|->
name|chg1
expr_stmt|;
name|m
operator|->
name|i2
operator|=
name|xscr
operator|->
name|i2
operator|+
name|i2
expr_stmt|;
name|m
operator|->
name|chg2
operator|=
name|xscr
operator|->
name|chg2
expr_stmt|;
block|}
name|xdl_free_env
argument_list|(
operator|&
name|xe
argument_list|)
expr_stmt|;
name|xdl_free_script
argument_list|(
name|x
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|line_contains_alnum
specifier|static
name|int
name|line_contains_alnum
parameter_list|(
specifier|const
name|char
modifier|*
name|ptr
parameter_list|,
name|long
name|size
parameter_list|)
block|{
while|while
condition|(
name|size
operator|--
condition|)
if|if
condition|(
name|isalnum
argument_list|(
operator|*
operator|(
name|ptr
operator|++
operator|)
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

begin_function
DECL|function|lines_contain_alnum
specifier|static
name|int
name|lines_contain_alnum
parameter_list|(
name|xdfenv_t
modifier|*
name|xe
parameter_list|,
name|int
name|i
parameter_list|,
name|int
name|chg
parameter_list|)
block|{
for|for
control|(
init|;
name|chg
condition|;
name|chg
operator|--
operator|,
name|i
operator|++
control|)
if|if
condition|(
name|line_contains_alnum
argument_list|(
name|xe
operator|->
name|xdf2
operator|.
name|recs
index|[
name|i
index|]
operator|->
name|ptr
argument_list|,
name|xe
operator|->
name|xdf2
operator|.
name|recs
index|[
name|i
index|]
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

begin_comment
comment|/*  * This function merges m and m->next, marking everything between those hunks  * as conflicting, too.  */
end_comment

begin_function
DECL|function|xdl_merge_two_conflicts
specifier|static
name|void
name|xdl_merge_two_conflicts
parameter_list|(
name|xdmerge_t
modifier|*
name|m
parameter_list|)
block|{
name|xdmerge_t
modifier|*
name|next_m
init|=
name|m
operator|->
name|next
decl_stmt|;
name|m
operator|->
name|chg1
operator|=
name|next_m
operator|->
name|i1
operator|+
name|next_m
operator|->
name|chg1
operator|-
name|m
operator|->
name|i1
expr_stmt|;
name|m
operator|->
name|chg2
operator|=
name|next_m
operator|->
name|i2
operator|+
name|next_m
operator|->
name|chg2
operator|-
name|m
operator|->
name|i2
expr_stmt|;
name|m
operator|->
name|next
operator|=
name|next_m
operator|->
name|next
expr_stmt|;
name|free
argument_list|(
name|next_m
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * If there are less than 3 non-conflicting lines between conflicts,  * it appears simpler -- because it takes up less (or as many) lines --  * if the lines are moved into the conflicts.  */
end_comment

begin_function
DECL|function|xdl_simplify_non_conflicts
specifier|static
name|int
name|xdl_simplify_non_conflicts
parameter_list|(
name|xdfenv_t
modifier|*
name|xe1
parameter_list|,
name|xdmerge_t
modifier|*
name|m
parameter_list|,
name|int
name|simplify_if_no_alnum
parameter_list|)
block|{
name|int
name|result
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|m
condition|)
return|return
name|result
return|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|xdmerge_t
modifier|*
name|next_m
init|=
name|m
operator|->
name|next
decl_stmt|;
name|int
name|begin
decl_stmt|,
name|end
decl_stmt|;
if|if
condition|(
operator|!
name|next_m
condition|)
return|return
name|result
return|;
name|begin
operator|=
name|m
operator|->
name|i1
operator|+
name|m
operator|->
name|chg1
expr_stmt|;
name|end
operator|=
name|next_m
operator|->
name|i1
expr_stmt|;
if|if
condition|(
name|m
operator|->
name|mode
operator|!=
literal|0
operator|||
name|next_m
operator|->
name|mode
operator|!=
literal|0
operator|||
operator|(
name|end
operator|-
name|begin
operator|>
literal|3
operator|&&
operator|(
operator|!
name|simplify_if_no_alnum
operator|||
name|lines_contain_alnum
argument_list|(
name|xe1
argument_list|,
name|begin
argument_list|,
name|end
operator|-
name|begin
argument_list|)
operator|)
operator|)
condition|)
block|{
name|m
operator|=
name|next_m
expr_stmt|;
block|}
else|else
block|{
name|result
operator|++
expr_stmt|;
name|xdl_merge_two_conflicts
argument_list|(
name|m
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_comment
comment|/*  * level == 0: mark all overlapping changes as conflict  * level == 1: mark overlapping changes as conflict only if not identical  * level == 2: analyze non-identical changes for minimal conflict set  * level == 3: analyze non-identical changes for minimal conflict set, but  *             treat hunks not containing any letter or number as conflicting  *  * returns< 0 on error, == 0 for no conflicts, else number of conflicts  */
end_comment

begin_function
DECL|function|xdl_do_merge
specifier|static
name|int
name|xdl_do_merge
parameter_list|(
name|xdfenv_t
modifier|*
name|xe1
parameter_list|,
name|xdchange_t
modifier|*
name|xscr1
parameter_list|,
specifier|const
name|char
modifier|*
name|name1
parameter_list|,
name|xdfenv_t
modifier|*
name|xe2
parameter_list|,
name|xdchange_t
modifier|*
name|xscr2
parameter_list|,
specifier|const
name|char
modifier|*
name|name2
parameter_list|,
name|int
name|level
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|mmbuffer_t
modifier|*
name|result
parameter_list|)
block|{
name|xdmerge_t
modifier|*
name|changes
decl_stmt|,
modifier|*
name|c
decl_stmt|;
name|int
name|i1
decl_stmt|,
name|i2
decl_stmt|,
name|chg1
decl_stmt|,
name|chg2
decl_stmt|;
name|c
operator|=
name|changes
operator|=
name|NULL
expr_stmt|;
while|while
condition|(
name|xscr1
operator|&&
name|xscr2
condition|)
block|{
if|if
condition|(
operator|!
name|changes
condition|)
name|changes
operator|=
name|c
expr_stmt|;
if|if
condition|(
name|xscr1
operator|->
name|i1
operator|+
name|xscr1
operator|->
name|chg1
operator|<
name|xscr2
operator|->
name|i1
condition|)
block|{
name|i1
operator|=
name|xscr1
operator|->
name|i2
expr_stmt|;
name|i2
operator|=
name|xscr2
operator|->
name|i2
operator|-
name|xscr2
operator|->
name|i1
operator|+
name|xscr1
operator|->
name|i1
expr_stmt|;
name|chg1
operator|=
name|xscr1
operator|->
name|chg2
expr_stmt|;
name|chg2
operator|=
name|xscr1
operator|->
name|chg1
expr_stmt|;
if|if
condition|(
name|xdl_append_merge
argument_list|(
operator|&
name|c
argument_list|,
literal|1
argument_list|,
name|i1
argument_list|,
name|chg1
argument_list|,
name|i2
argument_list|,
name|chg2
argument_list|)
condition|)
block|{
name|xdl_cleanup_merge
argument_list|(
name|changes
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|xscr1
operator|=
name|xscr1
operator|->
name|next
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|xscr2
operator|->
name|i1
operator|+
name|xscr2
operator|->
name|chg1
operator|<
name|xscr1
operator|->
name|i1
condition|)
block|{
name|i1
operator|=
name|xscr1
operator|->
name|i2
operator|-
name|xscr1
operator|->
name|i1
operator|+
name|xscr2
operator|->
name|i1
expr_stmt|;
name|i2
operator|=
name|xscr2
operator|->
name|i2
expr_stmt|;
name|chg1
operator|=
name|xscr2
operator|->
name|chg1
expr_stmt|;
name|chg2
operator|=
name|xscr2
operator|->
name|chg2
expr_stmt|;
if|if
condition|(
name|xdl_append_merge
argument_list|(
operator|&
name|c
argument_list|,
literal|2
argument_list|,
name|i1
argument_list|,
name|chg1
argument_list|,
name|i2
argument_list|,
name|chg2
argument_list|)
condition|)
block|{
name|xdl_cleanup_merge
argument_list|(
name|changes
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|xscr2
operator|=
name|xscr2
operator|->
name|next
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|level
operator|<
literal|1
operator|||
name|xscr1
operator|->
name|i1
operator|!=
name|xscr2
operator|->
name|i1
operator|||
name|xscr1
operator|->
name|chg1
operator|!=
name|xscr2
operator|->
name|chg1
operator|||
name|xscr1
operator|->
name|chg2
operator|!=
name|xscr2
operator|->
name|chg2
operator|||
name|xdl_merge_cmp_lines
argument_list|(
name|xe1
argument_list|,
name|xscr1
operator|->
name|i2
argument_list|,
name|xe2
argument_list|,
name|xscr2
operator|->
name|i2
argument_list|,
name|xscr1
operator|->
name|chg2
argument_list|,
name|xpp
operator|->
name|flags
argument_list|)
condition|)
block|{
comment|/* conflict */
name|int
name|off
init|=
name|xscr1
operator|->
name|i1
operator|-
name|xscr2
operator|->
name|i1
decl_stmt|;
name|int
name|ffo
init|=
name|off
operator|+
name|xscr1
operator|->
name|chg1
operator|-
name|xscr2
operator|->
name|chg1
decl_stmt|;
name|i1
operator|=
name|xscr1
operator|->
name|i2
expr_stmt|;
name|i2
operator|=
name|xscr2
operator|->
name|i2
expr_stmt|;
if|if
condition|(
name|off
operator|>
literal|0
condition|)
name|i1
operator|-=
name|off
expr_stmt|;
else|else
name|i2
operator|+=
name|off
expr_stmt|;
name|chg1
operator|=
name|xscr1
operator|->
name|i2
operator|+
name|xscr1
operator|->
name|chg2
operator|-
name|i1
expr_stmt|;
name|chg2
operator|=
name|xscr2
operator|->
name|i2
operator|+
name|xscr2
operator|->
name|chg2
operator|-
name|i2
expr_stmt|;
if|if
condition|(
name|ffo
operator|>
literal|0
condition|)
name|chg2
operator|+=
name|ffo
expr_stmt|;
else|else
name|chg1
operator|-=
name|ffo
expr_stmt|;
if|if
condition|(
name|xdl_append_merge
argument_list|(
operator|&
name|c
argument_list|,
literal|0
argument_list|,
name|i1
argument_list|,
name|chg1
argument_list|,
name|i2
argument_list|,
name|chg2
argument_list|)
condition|)
block|{
name|xdl_cleanup_merge
argument_list|(
name|changes
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
block|}
name|i1
operator|=
name|xscr1
operator|->
name|i1
operator|+
name|xscr1
operator|->
name|chg1
expr_stmt|;
name|i2
operator|=
name|xscr2
operator|->
name|i1
operator|+
name|xscr2
operator|->
name|chg1
expr_stmt|;
if|if
condition|(
name|i1
operator|>=
name|i2
condition|)
name|xscr2
operator|=
name|xscr2
operator|->
name|next
expr_stmt|;
if|if
condition|(
name|i2
operator|>=
name|i1
condition|)
name|xscr1
operator|=
name|xscr1
operator|->
name|next
expr_stmt|;
block|}
while|while
condition|(
name|xscr1
condition|)
block|{
if|if
condition|(
operator|!
name|changes
condition|)
name|changes
operator|=
name|c
expr_stmt|;
name|i1
operator|=
name|xscr1
operator|->
name|i2
expr_stmt|;
name|i2
operator|=
name|xscr1
operator|->
name|i1
operator|+
name|xe2
operator|->
name|xdf2
operator|.
name|nrec
operator|-
name|xe2
operator|->
name|xdf1
operator|.
name|nrec
expr_stmt|;
name|chg1
operator|=
name|xscr1
operator|->
name|chg2
expr_stmt|;
name|chg2
operator|=
name|xscr1
operator|->
name|chg1
expr_stmt|;
if|if
condition|(
name|xdl_append_merge
argument_list|(
operator|&
name|c
argument_list|,
literal|1
argument_list|,
name|i1
argument_list|,
name|chg1
argument_list|,
name|i2
argument_list|,
name|chg2
argument_list|)
condition|)
block|{
name|xdl_cleanup_merge
argument_list|(
name|changes
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|xscr1
operator|=
name|xscr1
operator|->
name|next
expr_stmt|;
block|}
while|while
condition|(
name|xscr2
condition|)
block|{
if|if
condition|(
operator|!
name|changes
condition|)
name|changes
operator|=
name|c
expr_stmt|;
name|i1
operator|=
name|xscr2
operator|->
name|i1
operator|+
name|xe1
operator|->
name|xdf2
operator|.
name|nrec
operator|-
name|xe1
operator|->
name|xdf1
operator|.
name|nrec
expr_stmt|;
name|i2
operator|=
name|xscr2
operator|->
name|i2
expr_stmt|;
name|chg1
operator|=
name|xscr2
operator|->
name|chg1
expr_stmt|;
name|chg2
operator|=
name|xscr2
operator|->
name|chg2
expr_stmt|;
if|if
condition|(
name|xdl_append_merge
argument_list|(
operator|&
name|c
argument_list|,
literal|2
argument_list|,
name|i1
argument_list|,
name|chg1
argument_list|,
name|i2
argument_list|,
name|chg2
argument_list|)
condition|)
block|{
name|xdl_cleanup_merge
argument_list|(
name|changes
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|xscr2
operator|=
name|xscr2
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|changes
condition|)
name|changes
operator|=
name|c
expr_stmt|;
comment|/* refine conflicts */
if|if
condition|(
name|level
operator|>
literal|1
operator|&&
operator|(
name|xdl_refine_conflicts
argument_list|(
name|xe1
argument_list|,
name|xe2
argument_list|,
name|changes
argument_list|,
name|xpp
argument_list|)
operator|<
literal|0
operator|||
name|xdl_simplify_non_conflicts
argument_list|(
name|xe1
argument_list|,
name|changes
argument_list|,
name|level
operator|>
literal|2
argument_list|)
operator|<
literal|0
operator|)
condition|)
block|{
name|xdl_cleanup_merge
argument_list|(
name|changes
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
comment|/* output */
if|if
condition|(
name|result
condition|)
block|{
name|int
name|size
init|=
name|xdl_fill_merge_buffer
argument_list|(
name|xe1
argument_list|,
name|name1
argument_list|,
name|xe2
argument_list|,
name|name2
argument_list|,
name|changes
argument_list|,
name|NULL
argument_list|)
decl_stmt|;
name|result
operator|->
name|ptr
operator|=
name|xdl_malloc
argument_list|(
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|result
operator|->
name|ptr
condition|)
block|{
name|xdl_cleanup_merge
argument_list|(
name|changes
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|result
operator|->
name|size
operator|=
name|size
expr_stmt|;
name|xdl_fill_merge_buffer
argument_list|(
name|xe1
argument_list|,
name|name1
argument_list|,
name|xe2
argument_list|,
name|name2
argument_list|,
name|changes
argument_list|,
name|result
operator|->
name|ptr
argument_list|)
expr_stmt|;
block|}
return|return
name|xdl_cleanup_merge
argument_list|(
name|changes
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|xdl_merge
name|int
name|xdl_merge
parameter_list|(
name|mmfile_t
modifier|*
name|orig
parameter_list|,
name|mmfile_t
modifier|*
name|mf1
parameter_list|,
specifier|const
name|char
modifier|*
name|name1
parameter_list|,
name|mmfile_t
modifier|*
name|mf2
parameter_list|,
specifier|const
name|char
modifier|*
name|name2
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|int
name|level
parameter_list|,
name|mmbuffer_t
modifier|*
name|result
parameter_list|)
block|{
name|xdchange_t
modifier|*
name|xscr1
decl_stmt|,
modifier|*
name|xscr2
decl_stmt|;
name|xdfenv_t
name|xe1
decl_stmt|,
name|xe2
decl_stmt|;
name|int
name|status
decl_stmt|;
name|result
operator|->
name|ptr
operator|=
name|NULL
expr_stmt|;
name|result
operator|->
name|size
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|xdl_do_diff
argument_list|(
name|orig
argument_list|,
name|mf1
argument_list|,
name|xpp
argument_list|,
operator|&
name|xe1
argument_list|)
operator|<
literal|0
operator|||
name|xdl_do_diff
argument_list|(
name|orig
argument_list|,
name|mf2
argument_list|,
name|xpp
argument_list|,
operator|&
name|xe2
argument_list|)
operator|<
literal|0
condition|)
block|{
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|xdl_change_compact
argument_list|(
operator|&
name|xe1
operator|.
name|xdf1
argument_list|,
operator|&
name|xe1
operator|.
name|xdf2
argument_list|,
name|xpp
operator|->
name|flags
argument_list|)
operator|<
literal|0
operator|||
name|xdl_change_compact
argument_list|(
operator|&
name|xe1
operator|.
name|xdf2
argument_list|,
operator|&
name|xe1
operator|.
name|xdf1
argument_list|,
name|xpp
operator|->
name|flags
argument_list|)
operator|<
literal|0
operator|||
name|xdl_build_script
argument_list|(
operator|&
name|xe1
argument_list|,
operator|&
name|xscr1
argument_list|)
operator|<
literal|0
condition|)
block|{
name|xdl_free_env
argument_list|(
operator|&
name|xe1
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|xdl_change_compact
argument_list|(
operator|&
name|xe2
operator|.
name|xdf1
argument_list|,
operator|&
name|xe2
operator|.
name|xdf2
argument_list|,
name|xpp
operator|->
name|flags
argument_list|)
operator|<
literal|0
operator|||
name|xdl_change_compact
argument_list|(
operator|&
name|xe2
operator|.
name|xdf2
argument_list|,
operator|&
name|xe2
operator|.
name|xdf1
argument_list|,
name|xpp
operator|->
name|flags
argument_list|)
operator|<
literal|0
operator|||
name|xdl_build_script
argument_list|(
operator|&
name|xe2
argument_list|,
operator|&
name|xscr2
argument_list|)
operator|<
literal|0
condition|)
block|{
name|xdl_free_env
argument_list|(
operator|&
name|xe2
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|status
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|xscr1
operator|||
name|xscr2
condition|)
block|{
if|if
condition|(
operator|!
name|xscr1
condition|)
block|{
name|result
operator|->
name|ptr
operator|=
name|xdl_malloc
argument_list|(
name|mf2
operator|->
name|size
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|result
operator|->
name|ptr
argument_list|,
name|mf2
operator|->
name|ptr
argument_list|,
name|mf2
operator|->
name|size
argument_list|)
expr_stmt|;
name|result
operator|->
name|size
operator|=
name|mf2
operator|->
name|size
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|xscr2
condition|)
block|{
name|result
operator|->
name|ptr
operator|=
name|xdl_malloc
argument_list|(
name|mf1
operator|->
name|size
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|result
operator|->
name|ptr
argument_list|,
name|mf1
operator|->
name|ptr
argument_list|,
name|mf1
operator|->
name|size
argument_list|)
expr_stmt|;
name|result
operator|->
name|size
operator|=
name|mf1
operator|->
name|size
expr_stmt|;
block|}
else|else
block|{
name|status
operator|=
name|xdl_do_merge
argument_list|(
operator|&
name|xe1
argument_list|,
name|xscr1
argument_list|,
name|name1
argument_list|,
operator|&
name|xe2
argument_list|,
name|xscr2
argument_list|,
name|name2
argument_list|,
name|level
argument_list|,
name|xpp
argument_list|,
name|result
argument_list|)
expr_stmt|;
block|}
name|xdl_free_script
argument_list|(
name|xscr1
argument_list|)
expr_stmt|;
name|xdl_free_script
argument_list|(
name|xscr2
argument_list|)
expr_stmt|;
block|}
name|xdl_free_env
argument_list|(
operator|&
name|xe1
argument_list|)
expr_stmt|;
name|xdl_free_env
argument_list|(
operator|&
name|xe2
argument_list|)
expr_stmt|;
return|return
name|status
return|;
block|}
end_function

end_unit

