begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  *  LibXDiff by Davide Libenzi ( File Differential Library )  *  Copyright (C) 2003	Davide Libenzi  *  *  This library is free software; you can redistribute it and/or  *  modify it under the terms of the GNU Lesser General Public  *  License as published by the Free Software Foundation; either  *  version 2.1 of the License, or (at your option) any later version.  *  *  This library is distributed in the hope that it will be useful,  *  but WITHOUT ANY WARRANTY; without even the implied warranty of  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *  Lesser General Public License for more details.  *  *  You should have received a copy of the GNU Lesser General Public  *  License along with this library; if not, write to the Free Software  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *  *  Davide Libenzi<davidel@xmailserver.org>  *  */
end_comment

begin_include
include|#
directive|include
file|"xinclude.h"
end_include

begin_define
DECL|macro|XDL_MAX_COST_MIN
define|#
directive|define
name|XDL_MAX_COST_MIN
value|256
end_define

begin_define
DECL|macro|XDL_HEUR_MIN_COST
define|#
directive|define
name|XDL_HEUR_MIN_COST
value|256
end_define

begin_define
DECL|macro|XDL_LINE_MAX
define|#
directive|define
name|XDL_LINE_MAX
value|(long)((1UL<< (8 * sizeof(long) - 1)) - 1)
end_define

begin_define
DECL|macro|XDL_SNAKE_CNT
define|#
directive|define
name|XDL_SNAKE_CNT
value|20
end_define

begin_define
DECL|macro|XDL_K_HEUR
define|#
directive|define
name|XDL_K_HEUR
value|4
end_define

begin_typedef
DECL|struct|s_xdpsplit
typedef|typedef
struct|struct
name|s_xdpsplit
block|{
DECL|member|i1
DECL|member|i2
name|long
name|i1
decl_stmt|,
name|i2
decl_stmt|;
DECL|member|min_lo
DECL|member|min_hi
name|int
name|min_lo
decl_stmt|,
name|min_hi
decl_stmt|;
DECL|typedef|xdpsplit_t
block|}
name|xdpsplit_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|long
name|xdl_split
parameter_list|(
name|unsigned
name|long
specifier|const
modifier|*
name|ha1
parameter_list|,
name|long
name|off1
parameter_list|,
name|long
name|lim1
parameter_list|,
name|unsigned
name|long
specifier|const
modifier|*
name|ha2
parameter_list|,
name|long
name|off2
parameter_list|,
name|long
name|lim2
parameter_list|,
name|long
modifier|*
name|kvdf
parameter_list|,
name|long
modifier|*
name|kvdb
parameter_list|,
name|int
name|need_min
parameter_list|,
name|xdpsplit_t
modifier|*
name|spl
parameter_list|,
name|xdalgoenv_t
modifier|*
name|xenv
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|xdchange_t
modifier|*
name|xdl_add_change
parameter_list|(
name|xdchange_t
modifier|*
name|xscr
parameter_list|,
name|long
name|i1
parameter_list|,
name|long
name|i2
parameter_list|,
name|long
name|chg1
parameter_list|,
name|long
name|chg2
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|xdl_change_compact
parameter_list|(
name|xdfile_t
modifier|*
name|xdf
parameter_list|,
name|xdfile_t
modifier|*
name|xdfo
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * See "An O(ND) Difference Algorithm and its Variations", by Eugene Myers.  * Basically considers a "box" (off1, off2, lim1, lim2) and scan from both  * the forward diagonal starting from (off1, off2) and the backward diagonal  * starting from (lim1, lim2). If the K values on the same diagonal crosses  * returns the furthest point of reach. We might end up having to expensive  * cases using this algorithm is full, so a little bit of heuristic is needed  * to cut the search and to return a suboptimal point.  */
end_comment

begin_function
DECL|function|xdl_split
specifier|static
name|long
name|xdl_split
parameter_list|(
name|unsigned
name|long
specifier|const
modifier|*
name|ha1
parameter_list|,
name|long
name|off1
parameter_list|,
name|long
name|lim1
parameter_list|,
name|unsigned
name|long
specifier|const
modifier|*
name|ha2
parameter_list|,
name|long
name|off2
parameter_list|,
name|long
name|lim2
parameter_list|,
name|long
modifier|*
name|kvdf
parameter_list|,
name|long
modifier|*
name|kvdb
parameter_list|,
name|int
name|need_min
parameter_list|,
name|xdpsplit_t
modifier|*
name|spl
parameter_list|,
name|xdalgoenv_t
modifier|*
name|xenv
parameter_list|)
block|{
name|long
name|dmin
init|=
name|off1
operator|-
name|lim2
decl_stmt|,
name|dmax
init|=
name|lim1
operator|-
name|off2
decl_stmt|;
name|long
name|fmid
init|=
name|off1
operator|-
name|off2
decl_stmt|,
name|bmid
init|=
name|lim1
operator|-
name|lim2
decl_stmt|;
name|long
name|odd
init|=
operator|(
name|fmid
operator|-
name|bmid
operator|)
operator|&
literal|1
decl_stmt|;
name|long
name|fmin
init|=
name|fmid
decl_stmt|,
name|fmax
init|=
name|fmid
decl_stmt|;
name|long
name|bmin
init|=
name|bmid
decl_stmt|,
name|bmax
init|=
name|bmid
decl_stmt|;
name|long
name|ec
decl_stmt|,
name|d
decl_stmt|,
name|i1
decl_stmt|,
name|i2
decl_stmt|,
name|prev1
decl_stmt|,
name|best
decl_stmt|,
name|dd
decl_stmt|,
name|v
decl_stmt|,
name|k
decl_stmt|;
comment|/* 	 * Set initial diagonal values for both forward and backward path. 	 */
name|kvdf
index|[
name|fmid
index|]
operator|=
name|off1
expr_stmt|;
name|kvdb
index|[
name|bmid
index|]
operator|=
name|lim1
expr_stmt|;
for|for
control|(
name|ec
operator|=
literal|1
init|;
condition|;
name|ec
operator|++
control|)
block|{
name|int
name|got_snake
init|=
literal|0
decl_stmt|;
comment|/* 		 * We need to extent the diagonal "domain" by one. If the next 		 * values exits the box boundaries we need to change it in the 		 * opposite direction because (max - min) must be a power of two. 		 * Also we initialize the extenal K value to -1 so that we can 		 * avoid extra conditions check inside the core loop. 		 */
if|if
condition|(
name|fmin
operator|>
name|dmin
condition|)
name|kvdf
index|[
operator|--
name|fmin
operator|-
literal|1
index|]
operator|=
operator|-
literal|1
expr_stmt|;
else|else
operator|++
name|fmin
expr_stmt|;
if|if
condition|(
name|fmax
operator|<
name|dmax
condition|)
name|kvdf
index|[
operator|++
name|fmax
operator|+
literal|1
index|]
operator|=
operator|-
literal|1
expr_stmt|;
else|else
operator|--
name|fmax
expr_stmt|;
for|for
control|(
name|d
operator|=
name|fmax
init|;
name|d
operator|>=
name|fmin
condition|;
name|d
operator|-=
literal|2
control|)
block|{
if|if
condition|(
name|kvdf
index|[
name|d
operator|-
literal|1
index|]
operator|>=
name|kvdf
index|[
name|d
operator|+
literal|1
index|]
condition|)
name|i1
operator|=
name|kvdf
index|[
name|d
operator|-
literal|1
index|]
operator|+
literal|1
expr_stmt|;
else|else
name|i1
operator|=
name|kvdf
index|[
name|d
operator|+
literal|1
index|]
expr_stmt|;
name|prev1
operator|=
name|i1
expr_stmt|;
name|i2
operator|=
name|i1
operator|-
name|d
expr_stmt|;
for|for
control|(
init|;
name|i1
operator|<
name|lim1
operator|&&
name|i2
operator|<
name|lim2
operator|&&
name|ha1
index|[
name|i1
index|]
operator|==
name|ha2
index|[
name|i2
index|]
condition|;
name|i1
operator|++
operator|,
name|i2
operator|++
control|)
empty_stmt|;
if|if
condition|(
name|i1
operator|-
name|prev1
operator|>
name|xenv
operator|->
name|snake_cnt
condition|)
name|got_snake
operator|=
literal|1
expr_stmt|;
name|kvdf
index|[
name|d
index|]
operator|=
name|i1
expr_stmt|;
if|if
condition|(
name|odd
operator|&&
name|bmin
operator|<=
name|d
operator|&&
name|d
operator|<=
name|bmax
operator|&&
name|kvdb
index|[
name|d
index|]
operator|<=
name|i1
condition|)
block|{
name|spl
operator|->
name|i1
operator|=
name|i1
expr_stmt|;
name|spl
operator|->
name|i2
operator|=
name|i2
expr_stmt|;
name|spl
operator|->
name|min_lo
operator|=
name|spl
operator|->
name|min_hi
operator|=
literal|1
expr_stmt|;
return|return
name|ec
return|;
block|}
block|}
comment|/* 		 * We need to extent the diagonal "domain" by one. If the next 		 * values exits the box boundaries we need to change it in the 		 * opposite direction because (max - min) must be a power of two. 		 * Also we initialize the extenal K value to -1 so that we can 		 * avoid extra conditions check inside the core loop. 		 */
if|if
condition|(
name|bmin
operator|>
name|dmin
condition|)
name|kvdb
index|[
operator|--
name|bmin
operator|-
literal|1
index|]
operator|=
name|XDL_LINE_MAX
expr_stmt|;
else|else
operator|++
name|bmin
expr_stmt|;
if|if
condition|(
name|bmax
operator|<
name|dmax
condition|)
name|kvdb
index|[
operator|++
name|bmax
operator|+
literal|1
index|]
operator|=
name|XDL_LINE_MAX
expr_stmt|;
else|else
operator|--
name|bmax
expr_stmt|;
for|for
control|(
name|d
operator|=
name|bmax
init|;
name|d
operator|>=
name|bmin
condition|;
name|d
operator|-=
literal|2
control|)
block|{
if|if
condition|(
name|kvdb
index|[
name|d
operator|-
literal|1
index|]
operator|<
name|kvdb
index|[
name|d
operator|+
literal|1
index|]
condition|)
name|i1
operator|=
name|kvdb
index|[
name|d
operator|-
literal|1
index|]
expr_stmt|;
else|else
name|i1
operator|=
name|kvdb
index|[
name|d
operator|+
literal|1
index|]
operator|-
literal|1
expr_stmt|;
name|prev1
operator|=
name|i1
expr_stmt|;
name|i2
operator|=
name|i1
operator|-
name|d
expr_stmt|;
for|for
control|(
init|;
name|i1
operator|>
name|off1
operator|&&
name|i2
operator|>
name|off2
operator|&&
name|ha1
index|[
name|i1
operator|-
literal|1
index|]
operator|==
name|ha2
index|[
name|i2
operator|-
literal|1
index|]
condition|;
name|i1
operator|--
operator|,
name|i2
operator|--
control|)
empty_stmt|;
if|if
condition|(
name|prev1
operator|-
name|i1
operator|>
name|xenv
operator|->
name|snake_cnt
condition|)
name|got_snake
operator|=
literal|1
expr_stmt|;
name|kvdb
index|[
name|d
index|]
operator|=
name|i1
expr_stmt|;
if|if
condition|(
operator|!
name|odd
operator|&&
name|fmin
operator|<=
name|d
operator|&&
name|d
operator|<=
name|fmax
operator|&&
name|i1
operator|<=
name|kvdf
index|[
name|d
index|]
condition|)
block|{
name|spl
operator|->
name|i1
operator|=
name|i1
expr_stmt|;
name|spl
operator|->
name|i2
operator|=
name|i2
expr_stmt|;
name|spl
operator|->
name|min_lo
operator|=
name|spl
operator|->
name|min_hi
operator|=
literal|1
expr_stmt|;
return|return
name|ec
return|;
block|}
block|}
if|if
condition|(
name|need_min
condition|)
continue|continue;
comment|/* 		 * If the edit cost is above the heuristic trigger and if 		 * we got a good snake, we sample current diagonals to see 		 * if some of the, have reached an "interesting" path. Our 		 * measure is a function of the distance from the diagonal 		 * corner (i1 + i2) penalized with the distance from the 		 * mid diagonal itself. If this value is above the current 		 * edit cost times a magic factor (XDL_K_HEUR) we consider 		 * it interesting. 		 */
if|if
condition|(
name|got_snake
operator|&&
name|ec
operator|>
name|xenv
operator|->
name|heur_min
condition|)
block|{
for|for
control|(
name|best
operator|=
literal|0
operator|,
name|d
operator|=
name|fmax
init|;
name|d
operator|>=
name|fmin
condition|;
name|d
operator|-=
literal|2
control|)
block|{
name|dd
operator|=
name|d
operator|>
name|fmid
condition|?
name|d
operator|-
name|fmid
else|:
name|fmid
operator|-
name|d
expr_stmt|;
name|i1
operator|=
name|kvdf
index|[
name|d
index|]
expr_stmt|;
name|i2
operator|=
name|i1
operator|-
name|d
expr_stmt|;
name|v
operator|=
operator|(
name|i1
operator|-
name|off1
operator|)
operator|+
operator|(
name|i2
operator|-
name|off2
operator|)
operator|-
name|dd
expr_stmt|;
if|if
condition|(
name|v
operator|>
name|XDL_K_HEUR
operator|*
name|ec
operator|&&
name|v
operator|>
name|best
operator|&&
name|off1
operator|+
name|xenv
operator|->
name|snake_cnt
operator|<=
name|i1
operator|&&
name|i1
operator|<
name|lim1
operator|&&
name|off2
operator|+
name|xenv
operator|->
name|snake_cnt
operator|<=
name|i2
operator|&&
name|i2
operator|<
name|lim2
condition|)
block|{
for|for
control|(
name|k
operator|=
literal|1
init|;
name|ha1
index|[
name|i1
operator|-
name|k
index|]
operator|==
name|ha2
index|[
name|i2
operator|-
name|k
index|]
condition|;
name|k
operator|++
control|)
if|if
condition|(
name|k
operator|==
name|xenv
operator|->
name|snake_cnt
condition|)
block|{
name|best
operator|=
name|v
expr_stmt|;
name|spl
operator|->
name|i1
operator|=
name|i1
expr_stmt|;
name|spl
operator|->
name|i2
operator|=
name|i2
expr_stmt|;
break|break;
block|}
block|}
block|}
if|if
condition|(
name|best
operator|>
literal|0
condition|)
block|{
name|spl
operator|->
name|min_lo
operator|=
literal|1
expr_stmt|;
name|spl
operator|->
name|min_hi
operator|=
literal|0
expr_stmt|;
return|return
name|ec
return|;
block|}
for|for
control|(
name|best
operator|=
literal|0
operator|,
name|d
operator|=
name|bmax
init|;
name|d
operator|>=
name|bmin
condition|;
name|d
operator|-=
literal|2
control|)
block|{
name|dd
operator|=
name|d
operator|>
name|bmid
condition|?
name|d
operator|-
name|bmid
else|:
name|bmid
operator|-
name|d
expr_stmt|;
name|i1
operator|=
name|kvdb
index|[
name|d
index|]
expr_stmt|;
name|i2
operator|=
name|i1
operator|-
name|d
expr_stmt|;
name|v
operator|=
operator|(
name|lim1
operator|-
name|i1
operator|)
operator|+
operator|(
name|lim2
operator|-
name|i2
operator|)
operator|-
name|dd
expr_stmt|;
if|if
condition|(
name|v
operator|>
name|XDL_K_HEUR
operator|*
name|ec
operator|&&
name|v
operator|>
name|best
operator|&&
name|off1
operator|<
name|i1
operator|&&
name|i1
operator|<=
name|lim1
operator|-
name|xenv
operator|->
name|snake_cnt
operator|&&
name|off2
operator|<
name|i2
operator|&&
name|i2
operator|<=
name|lim2
operator|-
name|xenv
operator|->
name|snake_cnt
condition|)
block|{
for|for
control|(
name|k
operator|=
literal|0
init|;
name|ha1
index|[
name|i1
operator|+
name|k
index|]
operator|==
name|ha2
index|[
name|i2
operator|+
name|k
index|]
condition|;
name|k
operator|++
control|)
if|if
condition|(
name|k
operator|==
name|xenv
operator|->
name|snake_cnt
operator|-
literal|1
condition|)
block|{
name|best
operator|=
name|v
expr_stmt|;
name|spl
operator|->
name|i1
operator|=
name|i1
expr_stmt|;
name|spl
operator|->
name|i2
operator|=
name|i2
expr_stmt|;
break|break;
block|}
block|}
block|}
if|if
condition|(
name|best
operator|>
literal|0
condition|)
block|{
name|spl
operator|->
name|min_lo
operator|=
literal|0
expr_stmt|;
name|spl
operator|->
name|min_hi
operator|=
literal|1
expr_stmt|;
return|return
name|ec
return|;
block|}
block|}
comment|/* 		 * Enough is enough. We spent too much time here and now we collect 		 * the furthest reaching path using the (i1 + i2) measure. 		 */
if|if
condition|(
name|ec
operator|>=
name|xenv
operator|->
name|mxcost
condition|)
block|{
name|long
name|fbest
decl_stmt|,
name|fbest1
decl_stmt|,
name|bbest
decl_stmt|,
name|bbest1
decl_stmt|;
name|fbest
operator|=
name|fbest1
operator|=
operator|-
literal|1
expr_stmt|;
for|for
control|(
name|d
operator|=
name|fmax
init|;
name|d
operator|>=
name|fmin
condition|;
name|d
operator|-=
literal|2
control|)
block|{
name|i1
operator|=
name|XDL_MIN
argument_list|(
name|kvdf
index|[
name|d
index|]
argument_list|,
name|lim1
argument_list|)
expr_stmt|;
name|i2
operator|=
name|i1
operator|-
name|d
expr_stmt|;
if|if
condition|(
name|lim2
operator|<
name|i2
condition|)
name|i1
operator|=
name|lim2
operator|+
name|d
operator|,
name|i2
operator|=
name|lim2
expr_stmt|;
if|if
condition|(
name|fbest
operator|<
name|i1
operator|+
name|i2
condition|)
block|{
name|fbest
operator|=
name|i1
operator|+
name|i2
expr_stmt|;
name|fbest1
operator|=
name|i1
expr_stmt|;
block|}
block|}
name|bbest
operator|=
name|bbest1
operator|=
name|XDL_LINE_MAX
expr_stmt|;
for|for
control|(
name|d
operator|=
name|bmax
init|;
name|d
operator|>=
name|bmin
condition|;
name|d
operator|-=
literal|2
control|)
block|{
name|i1
operator|=
name|XDL_MAX
argument_list|(
name|off1
argument_list|,
name|kvdb
index|[
name|d
index|]
argument_list|)
expr_stmt|;
name|i2
operator|=
name|i1
operator|-
name|d
expr_stmt|;
if|if
condition|(
name|i2
operator|<
name|off2
condition|)
name|i1
operator|=
name|off2
operator|+
name|d
operator|,
name|i2
operator|=
name|off2
expr_stmt|;
if|if
condition|(
name|i1
operator|+
name|i2
operator|<
name|bbest
condition|)
block|{
name|bbest
operator|=
name|i1
operator|+
name|i2
expr_stmt|;
name|bbest1
operator|=
name|i1
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|(
name|lim1
operator|+
name|lim2
operator|)
operator|-
name|bbest
operator|<
name|fbest
operator|-
operator|(
name|off1
operator|+
name|off2
operator|)
condition|)
block|{
name|spl
operator|->
name|i1
operator|=
name|fbest1
expr_stmt|;
name|spl
operator|->
name|i2
operator|=
name|fbest
operator|-
name|fbest1
expr_stmt|;
name|spl
operator|->
name|min_lo
operator|=
literal|1
expr_stmt|;
name|spl
operator|->
name|min_hi
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|spl
operator|->
name|i1
operator|=
name|bbest1
expr_stmt|;
name|spl
operator|->
name|i2
operator|=
name|bbest
operator|-
name|bbest1
expr_stmt|;
name|spl
operator|->
name|min_lo
operator|=
literal|0
expr_stmt|;
name|spl
operator|->
name|min_hi
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|ec
return|;
block|}
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_comment
comment|/*  * Rule: "Divide et Impera". Recursively split the box in sub-boxes by calling  * the box splitting function. Note that the real job (marking changed lines)  * is done in the two boundary reaching checks.  */
end_comment

begin_function
DECL|function|xdl_recs_cmp
name|int
name|xdl_recs_cmp
parameter_list|(
name|diffdata_t
modifier|*
name|dd1
parameter_list|,
name|long
name|off1
parameter_list|,
name|long
name|lim1
parameter_list|,
name|diffdata_t
modifier|*
name|dd2
parameter_list|,
name|long
name|off2
parameter_list|,
name|long
name|lim2
parameter_list|,
name|long
modifier|*
name|kvdf
parameter_list|,
name|long
modifier|*
name|kvdb
parameter_list|,
name|int
name|need_min
parameter_list|,
name|xdalgoenv_t
modifier|*
name|xenv
parameter_list|)
block|{
name|unsigned
name|long
specifier|const
modifier|*
name|ha1
init|=
name|dd1
operator|->
name|ha
decl_stmt|,
modifier|*
name|ha2
init|=
name|dd2
operator|->
name|ha
decl_stmt|;
comment|/* 	 * Shrink the box by walking through each diagonal snake (SW and NE). 	 */
for|for
control|(
init|;
name|off1
operator|<
name|lim1
operator|&&
name|off2
operator|<
name|lim2
operator|&&
name|ha1
index|[
name|off1
index|]
operator|==
name|ha2
index|[
name|off2
index|]
condition|;
name|off1
operator|++
operator|,
name|off2
operator|++
control|)
empty_stmt|;
for|for
control|(
init|;
name|off1
operator|<
name|lim1
operator|&&
name|off2
operator|<
name|lim2
operator|&&
name|ha1
index|[
name|lim1
operator|-
literal|1
index|]
operator|==
name|ha2
index|[
name|lim2
operator|-
literal|1
index|]
condition|;
name|lim1
operator|--
operator|,
name|lim2
operator|--
control|)
empty_stmt|;
comment|/* 	 * If one dimension is empty, then all records on the other one must 	 * be obviously changed. 	 */
if|if
condition|(
name|off1
operator|==
name|lim1
condition|)
block|{
name|char
modifier|*
name|rchg2
init|=
name|dd2
operator|->
name|rchg
decl_stmt|;
name|long
modifier|*
name|rindex2
init|=
name|dd2
operator|->
name|rindex
decl_stmt|;
for|for
control|(
init|;
name|off2
operator|<
name|lim2
condition|;
name|off2
operator|++
control|)
name|rchg2
index|[
name|rindex2
index|[
name|off2
index|]
index|]
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|off2
operator|==
name|lim2
condition|)
block|{
name|char
modifier|*
name|rchg1
init|=
name|dd1
operator|->
name|rchg
decl_stmt|;
name|long
modifier|*
name|rindex1
init|=
name|dd1
operator|->
name|rindex
decl_stmt|;
for|for
control|(
init|;
name|off1
operator|<
name|lim1
condition|;
name|off1
operator|++
control|)
name|rchg1
index|[
name|rindex1
index|[
name|off1
index|]
index|]
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|long
name|ec
decl_stmt|;
name|xdpsplit_t
name|spl
decl_stmt|;
name|spl
operator|.
name|i1
operator|=
name|spl
operator|.
name|i2
operator|=
literal|0
expr_stmt|;
comment|/* 		 * Divide ... 		 */
if|if
condition|(
operator|(
name|ec
operator|=
name|xdl_split
argument_list|(
name|ha1
argument_list|,
name|off1
argument_list|,
name|lim1
argument_list|,
name|ha2
argument_list|,
name|off2
argument_list|,
name|lim2
argument_list|,
name|kvdf
argument_list|,
name|kvdb
argument_list|,
name|need_min
argument_list|,
operator|&
name|spl
argument_list|,
name|xenv
argument_list|)
operator|)
operator|<
literal|0
condition|)
block|{
return|return
operator|-
literal|1
return|;
block|}
comment|/* 		 * ... et Impera. 		 */
if|if
condition|(
name|xdl_recs_cmp
argument_list|(
name|dd1
argument_list|,
name|off1
argument_list|,
name|spl
operator|.
name|i1
argument_list|,
name|dd2
argument_list|,
name|off2
argument_list|,
name|spl
operator|.
name|i2
argument_list|,
name|kvdf
argument_list|,
name|kvdb
argument_list|,
name|spl
operator|.
name|min_lo
argument_list|,
name|xenv
argument_list|)
operator|<
literal|0
operator|||
name|xdl_recs_cmp
argument_list|(
name|dd1
argument_list|,
name|spl
operator|.
name|i1
argument_list|,
name|lim1
argument_list|,
name|dd2
argument_list|,
name|spl
operator|.
name|i2
argument_list|,
name|lim2
argument_list|,
name|kvdf
argument_list|,
name|kvdb
argument_list|,
name|spl
operator|.
name|min_hi
argument_list|,
name|xenv
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
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|xdl_do_diff
name|int
name|xdl_do_diff
parameter_list|(
name|mmfile_t
modifier|*
name|mf1
parameter_list|,
name|mmfile_t
modifier|*
name|mf2
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|xdfenv_t
modifier|*
name|xe
parameter_list|)
block|{
name|long
name|ndiags
decl_stmt|;
name|long
modifier|*
name|kvd
decl_stmt|,
modifier|*
name|kvdf
decl_stmt|,
modifier|*
name|kvdb
decl_stmt|;
name|xdalgoenv_t
name|xenv
decl_stmt|;
name|diffdata_t
name|dd1
decl_stmt|,
name|dd2
decl_stmt|;
if|if
condition|(
name|xdl_prepare_env
argument_list|(
name|mf1
argument_list|,
name|mf2
argument_list|,
name|xpp
argument_list|,
name|xe
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
comment|/* 	 * Allocate and setup K vectors to be used by the differential algorithm. 	 * One is to store the forward path and one to store the backward path. 	 */
name|ndiags
operator|=
name|xe
operator|->
name|xdf1
operator|.
name|nreff
operator|+
name|xe
operator|->
name|xdf2
operator|.
name|nreff
operator|+
literal|3
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|kvd
operator|=
operator|(
name|long
operator|*
operator|)
name|xdl_malloc
argument_list|(
operator|(
literal|2
operator|*
name|ndiags
operator|+
literal|2
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|long
argument_list|)
argument_list|)
operator|)
condition|)
block|{
name|xdl_free_env
argument_list|(
name|xe
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|kvdf
operator|=
name|kvd
expr_stmt|;
name|kvdb
operator|=
name|kvdf
operator|+
name|ndiags
expr_stmt|;
name|kvdf
operator|+=
name|xe
operator|->
name|xdf2
operator|.
name|nreff
operator|+
literal|1
expr_stmt|;
name|kvdb
operator|+=
name|xe
operator|->
name|xdf2
operator|.
name|nreff
operator|+
literal|1
expr_stmt|;
name|xenv
operator|.
name|mxcost
operator|=
name|xdl_bogosqrt
argument_list|(
name|ndiags
argument_list|)
expr_stmt|;
if|if
condition|(
name|xenv
operator|.
name|mxcost
operator|<
name|XDL_MAX_COST_MIN
condition|)
name|xenv
operator|.
name|mxcost
operator|=
name|XDL_MAX_COST_MIN
expr_stmt|;
name|xenv
operator|.
name|snake_cnt
operator|=
name|XDL_SNAKE_CNT
expr_stmt|;
name|xenv
operator|.
name|heur_min
operator|=
name|XDL_HEUR_MIN_COST
expr_stmt|;
name|dd1
operator|.
name|nrec
operator|=
name|xe
operator|->
name|xdf1
operator|.
name|nreff
expr_stmt|;
name|dd1
operator|.
name|ha
operator|=
name|xe
operator|->
name|xdf1
operator|.
name|ha
expr_stmt|;
name|dd1
operator|.
name|rchg
operator|=
name|xe
operator|->
name|xdf1
operator|.
name|rchg
expr_stmt|;
name|dd1
operator|.
name|rindex
operator|=
name|xe
operator|->
name|xdf1
operator|.
name|rindex
expr_stmt|;
name|dd2
operator|.
name|nrec
operator|=
name|xe
operator|->
name|xdf2
operator|.
name|nreff
expr_stmt|;
name|dd2
operator|.
name|ha
operator|=
name|xe
operator|->
name|xdf2
operator|.
name|ha
expr_stmt|;
name|dd2
operator|.
name|rchg
operator|=
name|xe
operator|->
name|xdf2
operator|.
name|rchg
expr_stmt|;
name|dd2
operator|.
name|rindex
operator|=
name|xe
operator|->
name|xdf2
operator|.
name|rindex
expr_stmt|;
if|if
condition|(
name|xdl_recs_cmp
argument_list|(
operator|&
name|dd1
argument_list|,
literal|0
argument_list|,
name|dd1
operator|.
name|nrec
argument_list|,
operator|&
name|dd2
argument_list|,
literal|0
argument_list|,
name|dd2
operator|.
name|nrec
argument_list|,
name|kvdf
argument_list|,
name|kvdb
argument_list|,
operator|(
name|xpp
operator|->
name|flags
operator|&
name|XDF_NEED_MINIMAL
operator|)
operator|!=
literal|0
argument_list|,
operator|&
name|xenv
argument_list|)
operator|<
literal|0
condition|)
block|{
name|xdl_free
argument_list|(
name|kvd
argument_list|)
expr_stmt|;
name|xdl_free_env
argument_list|(
name|xe
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|xdl_free
argument_list|(
name|kvd
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|xdl_add_change
specifier|static
name|xdchange_t
modifier|*
name|xdl_add_change
parameter_list|(
name|xdchange_t
modifier|*
name|xscr
parameter_list|,
name|long
name|i1
parameter_list|,
name|long
name|i2
parameter_list|,
name|long
name|chg1
parameter_list|,
name|long
name|chg2
parameter_list|)
block|{
name|xdchange_t
modifier|*
name|xch
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|xch
operator|=
operator|(
name|xdchange_t
operator|*
operator|)
name|xdl_malloc
argument_list|(
sizeof|sizeof
argument_list|(
name|xdchange_t
argument_list|)
argument_list|)
operator|)
condition|)
return|return
name|NULL
return|;
name|xch
operator|->
name|next
operator|=
name|xscr
expr_stmt|;
name|xch
operator|->
name|i1
operator|=
name|i1
expr_stmt|;
name|xch
operator|->
name|i2
operator|=
name|i2
expr_stmt|;
name|xch
operator|->
name|chg1
operator|=
name|chg1
expr_stmt|;
name|xch
operator|->
name|chg2
operator|=
name|chg2
expr_stmt|;
return|return
name|xch
return|;
block|}
end_function

begin_function
DECL|function|xdl_change_compact
specifier|static
name|int
name|xdl_change_compact
parameter_list|(
name|xdfile_t
modifier|*
name|xdf
parameter_list|,
name|xdfile_t
modifier|*
name|xdfo
parameter_list|)
block|{
name|long
name|ix
decl_stmt|,
name|ixo
decl_stmt|,
name|ixs
decl_stmt|,
name|ixref
decl_stmt|,
name|grpsiz
decl_stmt|,
name|nrec
init|=
name|xdf
operator|->
name|nrec
decl_stmt|;
name|char
modifier|*
name|rchg
init|=
name|xdf
operator|->
name|rchg
decl_stmt|,
modifier|*
name|rchgo
init|=
name|xdfo
operator|->
name|rchg
decl_stmt|;
name|xrecord_t
modifier|*
modifier|*
name|recs
init|=
name|xdf
operator|->
name|recs
decl_stmt|;
comment|/* 	 * This is the same of what GNU diff does. Move back and forward 	 * change groups for a consistent and pretty diff output. This also 	 * helps in finding joineable change groups and reduce the diff size. 	 */
for|for
control|(
name|ix
operator|=
name|ixo
operator|=
literal|0
init|;
condition|;
control|)
block|{
comment|/* 		 * Find the first changed line in the to-be-compacted file. 		 * We need to keep track of both indexes, so if we find a 		 * changed lines group on the other file, while scanning the 		 * to-be-compacted file, we need to skip it properly. Note 		 * that loops that are testing for changed lines on rchg* do 		 * not need index bounding since the array is prepared with 		 * a zero at position -1 and N. 		 */
for|for
control|(
init|;
name|ix
operator|<
name|nrec
operator|&&
operator|!
name|rchg
index|[
name|ix
index|]
condition|;
name|ix
operator|++
control|)
while|while
condition|(
name|rchgo
index|[
name|ixo
operator|++
index|]
condition|)
empty_stmt|;
if|if
condition|(
name|ix
operator|==
name|nrec
condition|)
break|break;
comment|/* 		 * Record the start of a changed-group in the to-be-compacted file 		 * and find the end of it, on both to-be-compacted and other file 		 * indexes (ix and ixo). 		 */
name|ixs
operator|=
name|ix
expr_stmt|;
for|for
control|(
name|ix
operator|++
init|;
name|rchg
index|[
name|ix
index|]
condition|;
name|ix
operator|++
control|)
empty_stmt|;
for|for
control|(
init|;
name|rchgo
index|[
name|ixo
index|]
condition|;
name|ixo
operator|++
control|)
empty_stmt|;
do|do
block|{
name|grpsiz
operator|=
name|ix
operator|-
name|ixs
expr_stmt|;
comment|/* 			 * If the line before the current change group, is equal to 			 * the last line of the current change group, shift backward 			 * the group. 			 */
while|while
condition|(
name|ixs
operator|>
literal|0
operator|&&
name|recs
index|[
name|ixs
operator|-
literal|1
index|]
operator|->
name|ha
operator|==
name|recs
index|[
name|ix
operator|-
literal|1
index|]
operator|->
name|ha
operator|&&
name|XDL_RECMATCH
argument_list|(
name|recs
index|[
name|ixs
operator|-
literal|1
index|]
argument_list|,
name|recs
index|[
name|ix
operator|-
literal|1
index|]
argument_list|)
condition|)
block|{
name|rchg
index|[
operator|--
name|ixs
index|]
operator|=
literal|1
expr_stmt|;
name|rchg
index|[
operator|--
name|ix
index|]
operator|=
literal|0
expr_stmt|;
comment|/* 				 * This change might have joined two change groups, 				 * so we try to take this scenario in account by moving 				 * the start index accordingly (and so the other-file 				 * end-of-group index). 				 */
for|for
control|(
init|;
name|rchg
index|[
name|ixs
operator|-
literal|1
index|]
condition|;
name|ixs
operator|--
control|)
empty_stmt|;
while|while
condition|(
name|rchgo
index|[
operator|--
name|ixo
index|]
condition|)
empty_stmt|;
block|}
comment|/* 			 * Record the end-of-group position in case we are matched 			 * with a group of changes in the other file (that is, the 			 * change record before the enf-of-group index in the other 			 * file is set). 			 */
name|ixref
operator|=
name|rchgo
index|[
name|ixo
operator|-
literal|1
index|]
condition|?
name|ix
else|:
name|nrec
expr_stmt|;
comment|/* 			 * If the first line of the current change group, is equal to 			 * the line next of the current change group, shift forward 			 * the group. 			 */
while|while
condition|(
name|ix
operator|<
name|nrec
operator|&&
name|recs
index|[
name|ixs
index|]
operator|->
name|ha
operator|==
name|recs
index|[
name|ix
index|]
operator|->
name|ha
operator|&&
name|XDL_RECMATCH
argument_list|(
name|recs
index|[
name|ixs
index|]
argument_list|,
name|recs
index|[
name|ix
index|]
argument_list|)
condition|)
block|{
name|rchg
index|[
name|ixs
operator|++
index|]
operator|=
literal|0
expr_stmt|;
name|rchg
index|[
name|ix
operator|++
index|]
operator|=
literal|1
expr_stmt|;
comment|/* 				 * This change might have joined two change groups, 				 * so we try to take this scenario in account by moving 				 * the start index accordingly (and so the other-file 				 * end-of-group index). Keep tracking the reference 				 * index in case we are shifting together with a 				 * corresponding group of changes in the other file. 				 */
for|for
control|(
init|;
name|rchg
index|[
name|ix
index|]
condition|;
name|ix
operator|++
control|)
empty_stmt|;
while|while
condition|(
name|rchgo
index|[
operator|++
name|ixo
index|]
condition|)
name|ixref
operator|=
name|ix
expr_stmt|;
block|}
block|}
do|while
condition|(
name|grpsiz
operator|!=
name|ix
operator|-
name|ixs
condition|)
do|;
comment|/* 		 * Try to move back the possibly merged group of changes, to match 		 * the recorded postion in the other file. 		 */
while|while
condition|(
name|ixref
operator|<
name|ix
condition|)
block|{
name|rchg
index|[
operator|--
name|ixs
index|]
operator|=
literal|1
expr_stmt|;
name|rchg
index|[
operator|--
name|ix
index|]
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|rchgo
index|[
operator|--
name|ixo
index|]
condition|)
empty_stmt|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|xdl_build_script
name|int
name|xdl_build_script
parameter_list|(
name|xdfenv_t
modifier|*
name|xe
parameter_list|,
name|xdchange_t
modifier|*
modifier|*
name|xscr
parameter_list|)
block|{
name|xdchange_t
modifier|*
name|cscr
init|=
name|NULL
decl_stmt|,
modifier|*
name|xch
decl_stmt|;
name|char
modifier|*
name|rchg1
init|=
name|xe
operator|->
name|xdf1
operator|.
name|rchg
decl_stmt|,
modifier|*
name|rchg2
init|=
name|xe
operator|->
name|xdf2
operator|.
name|rchg
decl_stmt|;
name|long
name|i1
decl_stmt|,
name|i2
decl_stmt|,
name|l1
decl_stmt|,
name|l2
decl_stmt|;
comment|/* 	 * Trivial. Collects "groups" of changes and creates an edit script. 	 */
for|for
control|(
name|i1
operator|=
name|xe
operator|->
name|xdf1
operator|.
name|nrec
operator|,
name|i2
operator|=
name|xe
operator|->
name|xdf2
operator|.
name|nrec
init|;
name|i1
operator|>=
literal|0
operator|||
name|i2
operator|>=
literal|0
condition|;
name|i1
operator|--
operator|,
name|i2
operator|--
control|)
if|if
condition|(
name|rchg1
index|[
name|i1
operator|-
literal|1
index|]
operator|||
name|rchg2
index|[
name|i2
operator|-
literal|1
index|]
condition|)
block|{
for|for
control|(
name|l1
operator|=
name|i1
init|;
name|rchg1
index|[
name|i1
operator|-
literal|1
index|]
condition|;
name|i1
operator|--
control|)
empty_stmt|;
for|for
control|(
name|l2
operator|=
name|i2
init|;
name|rchg2
index|[
name|i2
operator|-
literal|1
index|]
condition|;
name|i2
operator|--
control|)
empty_stmt|;
if|if
condition|(
operator|!
operator|(
name|xch
operator|=
name|xdl_add_change
argument_list|(
name|cscr
argument_list|,
name|i1
argument_list|,
name|i2
argument_list|,
name|l1
operator|-
name|i1
argument_list|,
name|l2
operator|-
name|i2
argument_list|)
operator|)
condition|)
block|{
name|xdl_free_script
argument_list|(
name|cscr
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|cscr
operator|=
name|xch
expr_stmt|;
block|}
operator|*
name|xscr
operator|=
name|cscr
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|xdl_free_script
name|void
name|xdl_free_script
parameter_list|(
name|xdchange_t
modifier|*
name|xscr
parameter_list|)
block|{
name|xdchange_t
modifier|*
name|xch
decl_stmt|;
while|while
condition|(
operator|(
name|xch
operator|=
name|xscr
operator|)
operator|!=
name|NULL
condition|)
block|{
name|xscr
operator|=
name|xscr
operator|->
name|next
expr_stmt|;
name|xdl_free
argument_list|(
name|xch
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|xdl_diff
name|int
name|xdl_diff
parameter_list|(
name|mmfile_t
modifier|*
name|mf1
parameter_list|,
name|mmfile_t
modifier|*
name|mf2
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|xdemitconf_t
specifier|const
modifier|*
name|xecfg
parameter_list|,
name|xdemitcb_t
modifier|*
name|ecb
parameter_list|)
block|{
name|xdchange_t
modifier|*
name|xscr
decl_stmt|;
name|xdfenv_t
name|xe
decl_stmt|;
if|if
condition|(
name|xdl_do_diff
argument_list|(
name|mf1
argument_list|,
name|mf2
argument_list|,
name|xpp
argument_list|,
operator|&
name|xe
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
name|xe
operator|.
name|xdf1
argument_list|,
operator|&
name|xe
operator|.
name|xdf2
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
name|xscr
condition|)
block|{
if|if
condition|(
name|xdl_emit_diff
argument_list|(
operator|&
name|xe
argument_list|,
name|xscr
argument_list|,
name|ecb
argument_list|,
name|xecfg
argument_list|)
operator|<
literal|0
condition|)
block|{
name|xdl_free_script
argument_list|(
name|xscr
argument_list|)
expr_stmt|;
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
name|xdl_free_script
argument_list|(
name|xscr
argument_list|)
expr_stmt|;
block|}
name|xdl_free_env
argument_list|(
operator|&
name|xe
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

