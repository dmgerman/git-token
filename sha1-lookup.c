begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"sha1-lookup.h"
end_include

begin_function
DECL|function|take2
specifier|static
name|uint32_t
name|take2
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
return|return
operator|(
operator|(
name|sha1
index|[
literal|0
index|]
operator|<<
literal|8
operator|)
operator||
name|sha1
index|[
literal|1
index|]
operator|)
return|;
block|}
end_function

begin_comment
comment|/*  * Conventional binary search loop looks like this:  *  *      do {  *              int mi = (lo + hi) / 2;  *              int cmp = "entry pointed at by mi" minus "target";  *              if (!cmp)  *                      return (mi is the wanted one)  *              if (cmp> 0)  *                      hi = mi; "mi is larger than target"  *              else  *                      lo = mi+1; "mi is smaller than target"  *      } while (lo< hi);  *  * The invariants are:  *  * - When entering the loop, lo points at a slot that is never  *   above the target (it could be at the target), hi points at a  *   slot that is guaranteed to be above the target (it can never  *   be at the target).  *  * - We find a point 'mi' between lo and hi (mi could be the same  *   as lo, but never can be the same as hi), and check if it hits  *   the target.  There are three cases:  *  *    - if it is a hit, we are happy.  *  *    - if it is strictly higher than the target, we update hi with  *      it.  *  *    - if it is strictly lower than the target, we update lo to be  *      one slot after it, because we allow lo to be at the target.  *  * When choosing 'mi', we do not have to take the "middle" but  * anywhere in between lo and hi, as long as lo<= mi< hi is  * satisfied.  When we somehow know that the distance between the  * target and lo is much shorter than the target and hi, we could  * pick mi that is much closer to lo than the midway.  */
end_comment

begin_comment
comment|/*  * The table should contain "nr" elements.  * The sha1 of element i (between 0 and nr - 1) should be returned  * by "fn(i, table)".  */
end_comment

begin_function
DECL|function|sha1_pos
name|int
name|sha1_pos
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|void
modifier|*
name|table
parameter_list|,
name|size_t
name|nr
parameter_list|,
name|sha1_access_fn
name|fn
parameter_list|)
block|{
name|size_t
name|hi
init|=
name|nr
decl_stmt|;
name|size_t
name|lo
init|=
literal|0
decl_stmt|;
name|size_t
name|mi
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|!
name|nr
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|nr
operator|!=
literal|1
condition|)
block|{
name|size_t
name|lov
decl_stmt|,
name|hiv
decl_stmt|,
name|miv
decl_stmt|,
name|ofs
decl_stmt|;
for|for
control|(
name|ofs
operator|=
literal|0
init|;
name|ofs
operator|<
literal|18
condition|;
name|ofs
operator|+=
literal|2
control|)
block|{
name|lov
operator|=
name|take2
argument_list|(
name|fn
argument_list|(
literal|0
argument_list|,
name|table
argument_list|)
operator|+
name|ofs
argument_list|)
expr_stmt|;
name|hiv
operator|=
name|take2
argument_list|(
name|fn
argument_list|(
name|nr
operator|-
literal|1
argument_list|,
name|table
argument_list|)
operator|+
name|ofs
argument_list|)
expr_stmt|;
name|miv
operator|=
name|take2
argument_list|(
name|sha1
operator|+
name|ofs
argument_list|)
expr_stmt|;
if|if
condition|(
name|miv
operator|<
name|lov
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|hiv
operator|<
name|miv
condition|)
return|return
operator|-
literal|1
operator|-
name|nr
return|;
if|if
condition|(
name|lov
operator|!=
name|hiv
condition|)
block|{
comment|/* 				 * At this point miv could be equal 				 * to hiv (but sha1 could still be higher); 				 * the invariant of (mi< hi) should be 				 * kept. 				 */
name|mi
operator|=
operator|(
name|nr
operator|-
literal|1
operator|)
operator|*
operator|(
name|miv
operator|-
name|lov
operator|)
operator|/
operator|(
name|hiv
operator|-
name|lov
operator|)
expr_stmt|;
if|if
condition|(
name|lo
operator|<=
name|mi
operator|&&
name|mi
operator|<
name|hi
condition|)
break|break;
name|die
argument_list|(
literal|"BUG: assertion failed in binary search"
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
literal|18
operator|<=
name|ofs
condition|)
name|die
argument_list|(
literal|"cannot happen -- lo and hi are identical"
argument_list|)
expr_stmt|;
block|}
do|do
block|{
name|int
name|cmp
decl_stmt|;
name|cmp
operator|=
name|hashcmp
argument_list|(
name|fn
argument_list|(
name|mi
argument_list|,
name|table
argument_list|)
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cmp
condition|)
return|return
name|mi
return|;
if|if
condition|(
name|cmp
operator|>
literal|0
condition|)
name|hi
operator|=
name|mi
expr_stmt|;
else|else
name|lo
operator|=
name|mi
operator|+
literal|1
expr_stmt|;
name|mi
operator|=
operator|(
name|hi
operator|+
name|lo
operator|)
operator|/
literal|2
expr_stmt|;
block|}
do|while
condition|(
name|lo
operator|<
name|hi
condition|)
do|;
return|return
operator|-
name|lo
operator|-
literal|1
return|;
block|}
end_function

begin_comment
comment|/*  * Conventional binary search loop looks like this:  *  *	unsigned lo, hi;  *      do {  *              unsigned mi = (lo + hi) / 2;  *              int cmp = "entry pointed at by mi" minus "target";  *              if (!cmp)  *                      return (mi is the wanted one)  *              if (cmp> 0)  *                      hi = mi; "mi is larger than target"  *              else  *                      lo = mi+1; "mi is smaller than target"  *      } while (lo< hi);  *  * The invariants are:  *  * - When entering the loop, lo points at a slot that is never  *   above the target (it could be at the target), hi points at a  *   slot that is guaranteed to be above the target (it can never  *   be at the target).  *  * - We find a point 'mi' between lo and hi (mi could be the same  *   as lo, but never can be as same as hi), and check if it hits  *   the target.  There are three cases:  *  *    - if it is a hit, we are happy.  *  *    - if it is strictly higher than the target, we set it to hi,  *      and repeat the search.  *  *    - if it is strictly lower than the target, we update lo to  *      one slot after it, because we allow lo to be at the target.  *  *   If the loop exits, there is no matching entry.  *  * When choosing 'mi', we do not have to take the "middle" but  * anywhere in between lo and hi, as long as lo<= mi< hi is  * satisfied.  When we somehow know that the distance between the  * target and lo is much shorter than the target and hi, we could  * pick mi that is much closer to lo than the midway.  *  * Now, we can take advantage of the fact that SHA-1 is a good hash  * function, and as long as there are enough entries in the table, we  * can expect uniform distribution.  An entry that begins with for  * example "deadbeef..." is much likely to appear much later than in  * the midway of the table.  It can reasonably be expected to be near  * 87% (222/256) from the top of the table.  *  * However, we do not want to pick "mi" too precisely.  If the entry at  * the 87% in the above example turns out to be higher than the target  * we are looking for, we would end up narrowing the search space down  * only by 13%, instead of 50% we would get if we did a simple binary  * search.  So we would want to hedge our bets by being less aggressive.  *  * The table at "table" holds at least "nr" entries of "elem_size"  * bytes each.  Each entry has the SHA-1 key at "key_offset".  The  * table is sorted by the SHA-1 key of the entries.  The caller wants  * to find the entry with "key", and knows that the entry at "lo" is  * not higher than the entry it is looking for, and that the entry at  * "hi" is higher than the entry it is looking for.  */
end_comment

begin_function
DECL|function|sha1_entry_pos
name|int
name|sha1_entry_pos
parameter_list|(
specifier|const
name|void
modifier|*
name|table
parameter_list|,
name|size_t
name|elem_size
parameter_list|,
name|size_t
name|key_offset
parameter_list|,
name|unsigned
name|lo
parameter_list|,
name|unsigned
name|hi
parameter_list|,
name|unsigned
name|nr
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|key
parameter_list|)
block|{
specifier|const
name|unsigned
name|char
modifier|*
name|base
init|=
name|table
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|hi_key
decl_stmt|,
modifier|*
name|lo_key
decl_stmt|;
name|unsigned
name|ofs_0
decl_stmt|;
specifier|static
name|int
name|debug_lookup
init|=
operator|-
literal|1
decl_stmt|;
if|if
condition|(
name|debug_lookup
operator|<
literal|0
condition|)
name|debug_lookup
operator|=
operator|!
operator|!
name|getenv
argument_list|(
literal|"GIT_DEBUG_LOOKUP"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|nr
operator|||
name|lo
operator|>=
name|hi
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|nr
operator|==
name|hi
condition|)
name|hi_key
operator|=
name|NULL
expr_stmt|;
else|else
name|hi_key
operator|=
name|base
operator|+
name|elem_size
operator|*
name|hi
operator|+
name|key_offset
expr_stmt|;
name|lo_key
operator|=
name|base
operator|+
name|elem_size
operator|*
name|lo
operator|+
name|key_offset
expr_stmt|;
name|ofs_0
operator|=
literal|0
expr_stmt|;
do|do
block|{
name|int
name|cmp
decl_stmt|;
name|unsigned
name|ofs
decl_stmt|,
name|mi
decl_stmt|,
name|range
decl_stmt|;
name|unsigned
name|lov
decl_stmt|,
name|hiv
decl_stmt|,
name|kyv
decl_stmt|;
specifier|const
name|unsigned
name|char
modifier|*
name|mi_key
decl_stmt|;
name|range
operator|=
name|hi
operator|-
name|lo
expr_stmt|;
if|if
condition|(
name|hi_key
condition|)
block|{
for|for
control|(
name|ofs
operator|=
name|ofs_0
init|;
name|ofs
operator|<
literal|20
condition|;
name|ofs
operator|++
control|)
if|if
condition|(
name|lo_key
index|[
name|ofs
index|]
operator|!=
name|hi_key
index|[
name|ofs
index|]
condition|)
break|break;
name|ofs_0
operator|=
name|ofs
expr_stmt|;
comment|/* 			 * byte 0 thru (ofs-1) are the same between 			 * lo and hi; ofs is the first byte that is 			 * different. 			 * 			 * If ofs==20, then no bytes are different, 			 * meaning we have entries with duplicate 			 * keys. We know that we are in a solid run 			 * of this entry (because the entries are 			 * sorted, and our lo and hi are the same, 			 * there can be nothing but this single key 			 * in between). So we can stop the search. 			 * Either one of these entries is it (and 			 * we do not care which), or we do not have 			 * it. 			 * 			 * Furthermore, we know that one of our 			 * endpoints must be the edge of the run of 			 * duplicates. For example, given this 			 * sequence: 			 * 			 *     idx 0 1 2 3 4 5 			 *     key A C C C C D 			 * 			 * If we are searching for "B", we might 			 * hit the duplicate run at lo=1, hi=3 			 * (e.g., by first mi=3, then mi=0). But we 			 * can never have lo> 1, because B< C. 			 * That is, if our key is less than the 			 * run, we know that "lo" is the edge, but 			 * we can say nothing of "hi". Similarly, 			 * if our key is greater than the run, we 			 * know that "hi" is the edge, but we can 			 * say nothing of "lo". 			 * 			 * Therefore if we do not find it, we also 			 * know where it would go if it did exist: 			 * just on the far side of the edge that we 			 * know about. 			 */
if|if
condition|(
name|ofs
operator|==
literal|20
condition|)
block|{
name|mi
operator|=
name|lo
expr_stmt|;
name|mi_key
operator|=
name|base
operator|+
name|elem_size
operator|*
name|mi
operator|+
name|key_offset
expr_stmt|;
name|cmp
operator|=
name|memcmp
argument_list|(
name|mi_key
argument_list|,
name|key
argument_list|,
literal|20
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cmp
condition|)
return|return
name|mi
return|;
if|if
condition|(
name|cmp
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
operator|-
name|hi
return|;
else|else
return|return
operator|-
literal|1
operator|-
name|lo
return|;
block|}
name|hiv
operator|=
name|hi_key
index|[
name|ofs_0
index|]
expr_stmt|;
if|if
condition|(
name|ofs_0
operator|<
literal|19
condition|)
name|hiv
operator|=
operator|(
name|hiv
operator|<<
literal|8
operator|)
operator||
name|hi_key
index|[
name|ofs_0
operator|+
literal|1
index|]
expr_stmt|;
block|}
else|else
block|{
name|hiv
operator|=
literal|256
expr_stmt|;
if|if
condition|(
name|ofs_0
operator|<
literal|19
condition|)
name|hiv
operator|<<=
literal|8
expr_stmt|;
block|}
name|lov
operator|=
name|lo_key
index|[
name|ofs_0
index|]
expr_stmt|;
name|kyv
operator|=
name|key
index|[
name|ofs_0
index|]
expr_stmt|;
if|if
condition|(
name|ofs_0
operator|<
literal|19
condition|)
block|{
name|lov
operator|=
operator|(
name|lov
operator|<<
literal|8
operator|)
operator||
name|lo_key
index|[
name|ofs_0
operator|+
literal|1
index|]
expr_stmt|;
name|kyv
operator|=
operator|(
name|kyv
operator|<<
literal|8
operator|)
operator||
name|key
index|[
name|ofs_0
operator|+
literal|1
index|]
expr_stmt|;
block|}
name|assert
argument_list|(
name|lov
operator|<
name|hiv
argument_list|)
expr_stmt|;
if|if
condition|(
name|kyv
operator|<
name|lov
condition|)
return|return
operator|-
literal|1
operator|-
name|lo
return|;
if|if
condition|(
name|hiv
operator|<
name|kyv
condition|)
return|return
operator|-
literal|1
operator|-
name|hi
return|;
comment|/* 		 * Even if we know the target is much closer to 'hi' 		 * than 'lo', if we pick too precisely and overshoot 		 * (e.g. when we know 'mi' is closer to 'hi' than to 		 * 'lo', pick 'mi' that is higher than the target), we 		 * end up narrowing the search space by a smaller 		 * amount (i.e. the distance between 'mi' and 'hi') 		 * than what we would have (i.e. about half of 'lo' 		 * and 'hi').  Hedge our bets to pick 'mi' less 		 * aggressively, i.e. make 'mi' a bit closer to the 		 * middle than we would otherwise pick. 		 */
name|kyv
operator|=
operator|(
name|kyv
operator|*
literal|6
operator|+
name|lov
operator|+
name|hiv
operator|)
operator|/
literal|8
expr_stmt|;
if|if
condition|(
name|lov
operator|<
name|hiv
operator|-
literal|1
condition|)
block|{
if|if
condition|(
name|kyv
operator|==
name|lov
condition|)
name|kyv
operator|++
expr_stmt|;
elseif|else
if|if
condition|(
name|kyv
operator|==
name|hiv
condition|)
name|kyv
operator|--
expr_stmt|;
block|}
name|mi
operator|=
operator|(
name|range
operator|-
literal|1
operator|)
operator|*
operator|(
name|kyv
operator|-
name|lov
operator|)
operator|/
operator|(
name|hiv
operator|-
name|lov
operator|)
operator|+
name|lo
expr_stmt|;
if|if
condition|(
name|debug_lookup
condition|)
block|{
name|printf
argument_list|(
literal|"lo %u hi %u rg %u mi %u "
argument_list|,
name|lo
argument_list|,
name|hi
argument_list|,
name|range
argument_list|,
name|mi
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"ofs %u lov %x, hiv %x, kyv %x\n"
argument_list|,
name|ofs_0
argument_list|,
name|lov
argument_list|,
name|hiv
argument_list|,
name|kyv
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
operator|(
name|lo
operator|<=
name|mi
operator|&&
name|mi
operator|<
name|hi
operator|)
condition|)
name|die
argument_list|(
literal|"assertion failure lo %u mi %u hi %u %s"
argument_list|,
name|lo
argument_list|,
name|mi
argument_list|,
name|hi
argument_list|,
name|sha1_to_hex
argument_list|(
name|key
argument_list|)
argument_list|)
expr_stmt|;
name|mi_key
operator|=
name|base
operator|+
name|elem_size
operator|*
name|mi
operator|+
name|key_offset
expr_stmt|;
name|cmp
operator|=
name|memcmp
argument_list|(
name|mi_key
operator|+
name|ofs_0
argument_list|,
name|key
operator|+
name|ofs_0
argument_list|,
literal|20
operator|-
name|ofs_0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|cmp
condition|)
return|return
name|mi
return|;
if|if
condition|(
name|cmp
operator|>
literal|0
condition|)
block|{
name|hi
operator|=
name|mi
expr_stmt|;
name|hi_key
operator|=
name|mi_key
expr_stmt|;
block|}
else|else
block|{
name|lo
operator|=
name|mi
operator|+
literal|1
expr_stmt|;
name|lo_key
operator|=
name|mi_key
operator|+
name|elem_size
expr_stmt|;
block|}
block|}
do|while
condition|(
name|lo
operator|<
name|hi
condition|)
do|;
return|return
operator|-
name|lo
operator|-
literal|1
return|;
block|}
end_function

end_unit

