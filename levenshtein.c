begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"levenshtein.h"
end_include

begin_comment
comment|/*  * This function implements the Damerau-Levenshtein algorithm to  * calculate a distance between strings.  *  * Basically, it says how many letters need to be swapped, substituted,  * deleted from, or added to string1, at least, to get string2.  *  * The idea is to build a distance matrix for the substrings of both  * strings.  To avoid a large space complexity, only the last three rows  * are kept in memory (if swaps had the same or higher cost as one deletion  * plus one insertion, only two rows would be needed).  *  * At any stage, "i + 1" denotes the length of the current substring of  * string1 that the distance is calculated for.  *  * row2 holds the current row, row1 the previous row (i.e. for the substring  * of string1 of length "i"), and row0 the row before that.  *  * In other words, at the start of the big loop, row2[j + 1] contains the  * Damerau-Levenshtein distance between the substring of string1 of length  * "i" and the substring of string2 of length "j + 1".  *  * All the big loop does is determine the partial minimum-cost paths.  *  * It does so by calculating the costs of the path ending in characters  * i (in string1) and j (in string2), respectively, given that the last  * operation is a substitution, a swap, a deletion, or an insertion.  *  * This implementation allows the costs to be weighted:  *  * - w (as in "sWap")  * - s (as in "Substitution")  * - a (for insertion, AKA "Add")  * - d (as in "Deletion")  *  * Note that this algorithm calculates a distance _iff_ d == a.  */
end_comment

begin_function
DECL|function|levenshtein
name|int
name|levenshtein
parameter_list|(
specifier|const
name|char
modifier|*
name|string1
parameter_list|,
specifier|const
name|char
modifier|*
name|string2
parameter_list|,
name|int
name|w
parameter_list|,
name|int
name|s
parameter_list|,
name|int
name|a
parameter_list|,
name|int
name|d
parameter_list|)
block|{
name|int
name|len1
init|=
name|strlen
argument_list|(
name|string1
argument_list|)
decl_stmt|,
name|len2
init|=
name|strlen
argument_list|(
name|string2
argument_list|)
decl_stmt|;
name|int
modifier|*
name|row0
decl_stmt|,
modifier|*
name|row1
decl_stmt|,
modifier|*
name|row2
decl_stmt|;
name|int
name|i
decl_stmt|,
name|j
decl_stmt|;
name|ALLOC_ARRAY
argument_list|(
name|row0
argument_list|,
name|len2
operator|+
literal|1
argument_list|)
expr_stmt|;
name|ALLOC_ARRAY
argument_list|(
name|row1
argument_list|,
name|len2
operator|+
literal|1
argument_list|)
expr_stmt|;
name|ALLOC_ARRAY
argument_list|(
name|row2
argument_list|,
name|len2
operator|+
literal|1
argument_list|)
expr_stmt|;
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<=
name|len2
condition|;
name|j
operator|++
control|)
name|row1
index|[
name|j
index|]
operator|=
name|j
operator|*
name|a
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|len1
condition|;
name|i
operator|++
control|)
block|{
name|int
modifier|*
name|dummy
decl_stmt|;
name|row2
index|[
literal|0
index|]
operator|=
operator|(
name|i
operator|+
literal|1
operator|)
operator|*
name|d
expr_stmt|;
for|for
control|(
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|len2
condition|;
name|j
operator|++
control|)
block|{
comment|/* substitution */
name|row2
index|[
name|j
operator|+
literal|1
index|]
operator|=
name|row1
index|[
name|j
index|]
operator|+
name|s
operator|*
operator|(
name|string1
index|[
name|i
index|]
operator|!=
name|string2
index|[
name|j
index|]
operator|)
expr_stmt|;
comment|/* swap */
if|if
condition|(
name|i
operator|>
literal|0
operator|&&
name|j
operator|>
literal|0
operator|&&
name|string1
index|[
name|i
operator|-
literal|1
index|]
operator|==
name|string2
index|[
name|j
index|]
operator|&&
name|string1
index|[
name|i
index|]
operator|==
name|string2
index|[
name|j
operator|-
literal|1
index|]
operator|&&
name|row2
index|[
name|j
operator|+
literal|1
index|]
operator|>
name|row0
index|[
name|j
operator|-
literal|1
index|]
operator|+
name|w
condition|)
name|row2
index|[
name|j
operator|+
literal|1
index|]
operator|=
name|row0
index|[
name|j
operator|-
literal|1
index|]
operator|+
name|w
expr_stmt|;
comment|/* deletion */
if|if
condition|(
name|row2
index|[
name|j
operator|+
literal|1
index|]
operator|>
name|row1
index|[
name|j
operator|+
literal|1
index|]
operator|+
name|d
condition|)
name|row2
index|[
name|j
operator|+
literal|1
index|]
operator|=
name|row1
index|[
name|j
operator|+
literal|1
index|]
operator|+
name|d
expr_stmt|;
comment|/* insertion */
if|if
condition|(
name|row2
index|[
name|j
operator|+
literal|1
index|]
operator|>
name|row2
index|[
name|j
index|]
operator|+
name|a
condition|)
name|row2
index|[
name|j
operator|+
literal|1
index|]
operator|=
name|row2
index|[
name|j
index|]
operator|+
name|a
expr_stmt|;
block|}
name|dummy
operator|=
name|row0
expr_stmt|;
name|row0
operator|=
name|row1
expr_stmt|;
name|row1
operator|=
name|row2
expr_stmt|;
name|row2
operator|=
name|dummy
expr_stmt|;
block|}
name|i
operator|=
name|row1
index|[
name|len2
index|]
expr_stmt|;
name|free
argument_list|(
name|row0
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|row1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|row2
argument_list|)
expr_stmt|;
return|return
name|i
return|;
block|}
end_function

end_unit

