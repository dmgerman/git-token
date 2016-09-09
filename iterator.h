begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|ITERATOR_H
end_ifndef

begin_define
DECL|macro|ITERATOR_H
define|#
directive|define
name|ITERATOR_H
end_define

begin_comment
comment|/*  * Generic constants related to iterators.  */
end_comment

begin_comment
comment|/*  * The attempt to advance the iterator was successful; the iterator  * reflects the new current entry.  */
end_comment

begin_define
DECL|macro|ITER_OK
define|#
directive|define
name|ITER_OK
value|0
end_define

begin_comment
comment|/*  * The iterator is exhausted and has been freed.  */
end_comment

begin_define
DECL|macro|ITER_DONE
define|#
directive|define
name|ITER_DONE
value|-1
end_define

begin_comment
comment|/*  * The iterator experienced an error. The iteration has been aborted  * and the iterator has been freed.  */
end_comment

begin_define
DECL|macro|ITER_ERROR
define|#
directive|define
name|ITER_ERROR
value|-2
end_define

begin_comment
comment|/*  * Return values for selector functions for merge iterators. The  * numerical values of these constants are important and must be  * compatible with ITER_DONE and ITER_ERROR.  */
end_comment

begin_enum
DECL|enum|iterator_selection
enum|enum
name|iterator_selection
block|{
comment|/* End the iteration without an error: */
DECL|enumerator|ITER_SELECT_DONE
name|ITER_SELECT_DONE
init|=
name|ITER_DONE
block|,
comment|/* Report an error and abort the iteration: */
DECL|enumerator|ITER_SELECT_ERROR
name|ITER_SELECT_ERROR
init|=
name|ITER_ERROR
block|,
comment|/* 	 * The next group of constants are masks that are useful 	 * mainly internally. 	 */
comment|/* The LSB selects whether iter0/iter1 is the "current" iterator: */
DECL|enumerator|ITER_CURRENT_SELECTION_MASK
name|ITER_CURRENT_SELECTION_MASK
init|=
literal|0x01
block|,
comment|/* iter0 is the "current" iterator this round: */
DECL|enumerator|ITER_CURRENT_SELECTION_0
name|ITER_CURRENT_SELECTION_0
init|=
literal|0x00
block|,
comment|/* iter1 is the "current" iterator this round: */
DECL|enumerator|ITER_CURRENT_SELECTION_1
name|ITER_CURRENT_SELECTION_1
init|=
literal|0x01
block|,
comment|/* Yield the value from the current iterator? */
DECL|enumerator|ITER_YIELD_CURRENT
name|ITER_YIELD_CURRENT
init|=
literal|0x02
block|,
comment|/* Discard the value from the secondary iterator? */
DECL|enumerator|ITER_SKIP_SECONDARY
name|ITER_SKIP_SECONDARY
init|=
literal|0x04
block|,
comment|/* 	 * The constants that a selector function should usually 	 * return. 	 */
comment|/* Yield the value from iter0: */
DECL|enumerator|ITER_SELECT_0
name|ITER_SELECT_0
init|=
name|ITER_CURRENT_SELECTION_0
operator||
name|ITER_YIELD_CURRENT
block|,
comment|/* Yield the value from iter0 and discard the one from iter1: */
DECL|enumerator|ITER_SELECT_0_SKIP_1
name|ITER_SELECT_0_SKIP_1
init|=
name|ITER_SELECT_0
operator||
name|ITER_SKIP_SECONDARY
block|,
comment|/* Discard the value from iter0 without yielding anything this round: */
DECL|enumerator|ITER_SKIP_0
name|ITER_SKIP_0
init|=
name|ITER_CURRENT_SELECTION_1
operator||
name|ITER_SKIP_SECONDARY
block|,
comment|/* Yield the value from iter1: */
DECL|enumerator|ITER_SELECT_1
name|ITER_SELECT_1
init|=
name|ITER_CURRENT_SELECTION_1
operator||
name|ITER_YIELD_CURRENT
block|,
comment|/* Yield the value from iter1 and discard the one from iter0: */
DECL|enumerator|ITER_SELECT_1_SKIP_0
name|ITER_SELECT_1_SKIP_0
init|=
name|ITER_SELECT_1
operator||
name|ITER_SKIP_SECONDARY
block|,
comment|/* Discard the value from iter1 without yielding anything this round: */
DECL|enumerator|ITER_SKIP_1
name|ITER_SKIP_1
init|=
name|ITER_CURRENT_SELECTION_0
operator||
name|ITER_SKIP_SECONDARY
block|}
enum|;
end_enum

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* ITERATOR_H */
end_comment

end_unit

