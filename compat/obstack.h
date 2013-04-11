begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/* obstack.h - object stack macros    Copyright (C) 1988-1994,1996-1999,2003,2004,2005,2009 	Free Software Foundation, Inc.    This file is part of the GNU C Library.     The GNU C Library is free software; you can redistribute it and/or    modify it under the terms of the GNU Lesser General Public    License as published by the Free Software Foundation; either    version 2.1 of the License, or (at your option) any later version.     The GNU C Library is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    Lesser General Public License for more details.     You should have received a copy of the GNU Lesser General Public    License along with the GNU C Library; if not, write to the Free    Software Foundation, Inc., 51 Franklin Street, Fifth Floor,    Boston, MA 02110-1301, USA.  */
end_comment

begin_comment
comment|/* Summary:  All the apparent functions defined here are macros. The idea is that you would use these pre-tested macros to solve a very specific set of problems, and they would run fast. Caution: no side-effects in arguments please!! They may be evaluated MANY times!!  These macros operate a stack of objects.  Each object starts life small, and may grow to maturity.  (Consider building a word syllable by syllable.)  An object can move while it is growing.  Once it has been "finished" it never changes address again.  So the "top of the stack" is typically an immature growing object, while the rest of the stack is of mature, fixed size and fixed address objects.  These routines grab large chunks of memory, using a function you supply, called `obstack_chunk_alloc'.  On occasion, they free chunks, by calling `obstack_chunk_free'.  You must define them and declare them before using any obstack macros.  Each independent stack is represented by a `struct obstack'. Each of the obstack macros expects a pointer to such a structure as the first argument.  One motivation for this package is the problem of growing char strings in symbol tables.  Unless you are "fascist pig with a read-only mind" --Gosper's immortal quote from HAKMEM item 154, out of context--you would not like to put any arbitrary upper limit on the length of your symbols.  In practice this often means you will build many short symbols and a few long symbols.  At the time you are reading a symbol you don't know how long it is.  One traditional method is to read a symbol into a buffer, realloc()ating the buffer every time you try to read a symbol that is longer than the buffer.  This is beaut, but you still will want to copy the symbol from the buffer to a more permanent symbol-table entry say about half the time.  With obstacks, you can work differently.  Use one obstack for all symbol names.  As you read a symbol, grow the name in the obstack gradually. When the name is complete, finalize it.  Then, if the symbol exists already, free the newly read name.  The way we do this is to take a large chunk, allocating memory from low addresses.  When you want to build a symbol in the chunk you just add chars above the current "high water mark" in the chunk.  When you have finished adding chars, because you got to the end of the symbol, you know how long the chars are, and you can create a new object. Mostly the chars will not burst over the highest address of the chunk, because you would typically expect a chunk to be (say) 100 times as long as an average object.  In case that isn't clear, when we have enough chars to make up the object, THEY ARE ALREADY CONTIGUOUS IN THE CHUNK (guaranteed) so we just point to it where it lies.  No moving of chars is needed and this is the second win: potentially long strings need never be explicitly shuffled. Once an object is formed, it does not change its address during its lifetime.  When the chars burst over a chunk boundary, we allocate a larger chunk, and then copy the partly formed object from the end of the old chunk to the beginning of the new larger chunk.  We then carry on accreting characters to the end of the object as we normally would.  A special macro is provided to add a single char at a time to a growing object.  This allows the use of register variables, which break the ordinary 'growth' macro.  Summary: 	We allocate large chunks. 	We carve out one object at a time from the current chunk. 	Once carved, an object never moves. 	We are free to append data of any size to the currently 	  growing object. 	Exactly one object is growing in an obstack at any one time. 	You can run one obstack per control block. 	You may have as many control blocks as you dare. 	Because of the way we do it, you can `unwind' an obstack 	  back to a previous state. (You may remove objects much 	  as you would with a stack.) */
end_comment

begin_comment
comment|/* Don't do the contents of this file more than once.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_OBSTACK_H
end_ifndef

begin_define
DECL|macro|_OBSTACK_H
define|#
directive|define
name|_OBSTACK_H
value|1
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|__cplusplus
end_ifdef

begin_extern
extern|extern
literal|"C"
block|{
endif|#
directive|endif
comment|/* We need the type of a pointer subtraction.  If __PTRDIFF_TYPE__ is    defined, as with GNU C, use that; that way we don't pollute the    namespace with<stddef.h>'s symbols.  Otherwise, include<stddef.h>    and use ptrdiff_t.  */
ifdef|#
directive|ifdef
name|__PTRDIFF_TYPE__
DECL|macro|PTR_INT_TYPE
define|#
directive|define
name|PTR_INT_TYPE
value|__PTRDIFF_TYPE__
else|#
directive|else
include|#
directive|include
file|<stddef.h>
DECL|macro|PTR_INT_TYPE
define|#
directive|define
name|PTR_INT_TYPE
value|ptrdiff_t
endif|#
directive|endif
comment|/* If B is the base of an object addressed by P, return the result of    aligning P to the next multiple of A + 1.  B and P must be of type    char *.  A + 1 must be a power of 2.  */
DECL|macro|__BPTR_ALIGN
define|#
directive|define
name|__BPTR_ALIGN
parameter_list|(
name|B
parameter_list|,
name|P
parameter_list|,
name|A
parameter_list|)
value|((B) + (((P) - (B) + (A))& ~(A)))
comment|/* Similar to _BPTR_ALIGN (B, P, A), except optimize the common case    where pointers can be converted to integers, aligned as integers,    and converted back again.  If PTR_INT_TYPE is narrower than a    pointer (e.g., the AS/400), play it safe and compute the alignment    relative to B.  Otherwise, use the faster strategy of computing the    alignment relative to 0.  */
DECL|macro|__PTR_ALIGN
define|#
directive|define
name|__PTR_ALIGN
parameter_list|(
name|B
parameter_list|,
name|P
parameter_list|,
name|A
parameter_list|)
define|\
value|__BPTR_ALIGN (sizeof (PTR_INT_TYPE)< sizeof (void *) ? (B) : (char *) 0, \ 		P, A)
include|#
directive|include
file|<string.h>
DECL|struct|_obstack_chunk
struct|struct
name|_obstack_chunk
comment|/* Lives at front of each chunk. */
block|{
DECL|member|limit
name|char
modifier|*
name|limit
decl_stmt|;
comment|/* 1 past end of this chunk */
DECL|member|prev
name|struct
name|_obstack_chunk
modifier|*
name|prev
decl_stmt|;
comment|/* address of prior chunk or NULL */
DECL|member|contents
name|char
name|contents
index|[
literal|4
index|]
decl_stmt|;
comment|/* objects begin here */
block|}
struct|;
DECL|struct|obstack
struct|struct
name|obstack
comment|/* control current object in current chunk */
block|{
DECL|member|chunk_size
name|long
name|chunk_size
decl_stmt|;
comment|/* preferred size to allocate chunks in */
DECL|member|chunk
name|struct
name|_obstack_chunk
modifier|*
name|chunk
decl_stmt|;
comment|/* address of current struct obstack_chunk */
DECL|member|object_base
name|char
modifier|*
name|object_base
decl_stmt|;
comment|/* address of object we are building */
DECL|member|next_free
name|char
modifier|*
name|next_free
decl_stmt|;
comment|/* where to add next char to current object */
DECL|member|chunk_limit
name|char
modifier|*
name|chunk_limit
decl_stmt|;
comment|/* address of char after current chunk */
union|union
block|{
DECL|member|tempint
name|PTR_INT_TYPE
name|tempint
decl_stmt|;
DECL|member|tempptr
name|void
modifier|*
name|tempptr
decl_stmt|;
DECL|member|temp
block|}
name|temp
union|;
comment|/* Temporary for some macros.  */
DECL|member|alignment_mask
name|int
name|alignment_mask
decl_stmt|;
comment|/* Mask of alignment for each object. */
comment|/* These prototypes vary based on `use_extra_arg', and we use      casts to the prototypeless function type in all assignments,      but having prototypes here quiets -Wstrict-prototypes.  */
DECL|member|chunkfun
name|struct
name|_obstack_chunk
modifier|*
function_decl|(
modifier|*
name|chunkfun
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|,
name|long
parameter_list|)
function_decl|;
DECL|member|freefun
name|void
function_decl|(
modifier|*
name|freefun
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|,
name|struct
name|_obstack_chunk
modifier|*
parameter_list|)
function_decl|;
DECL|member|extra_arg
name|void
modifier|*
name|extra_arg
decl_stmt|;
comment|/* first arg for chunk alloc/dealloc funcs */
DECL|member|use_extra_arg
name|unsigned
name|use_extra_arg
range|:
literal|1
decl_stmt|;
comment|/* chunk alloc/dealloc funcs take extra arg */
DECL|member|maybe_empty_object
name|unsigned
name|maybe_empty_object
range|:
literal|1
decl_stmt|;
comment|/* There is a possibility that the current 				   chunk contains a zero-length object.  This 				   prevents freeing the chunk if we allocate 				   a bigger chunk to replace it. */
DECL|member|alloc_failed
name|unsigned
name|alloc_failed
range|:
literal|1
decl_stmt|;
comment|/* No longer used, as we now call the failed 				   handler on error, but retained for binary 				   compatibility.  */
block|}
struct|;
comment|/* Declare the external functions we use; they are in obstack.c.  */
specifier|extern
name|void
name|_obstack_newchunk
parameter_list|(
name|struct
name|obstack
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
specifier|extern
name|int
name|_obstack_begin
parameter_list|(
name|struct
name|obstack
modifier|*
parameter_list|,
name|int
parameter_list|,
name|int
parameter_list|,
name|void
modifier|*
function_decl|(
modifier|*
function_decl|)
parameter_list|(
name|long
parameter_list|)
parameter_list|,
name|void
function_decl|(
modifier|*
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|)
parameter_list|)
function_decl|;
specifier|extern
name|int
name|_obstack_begin_1
parameter_list|(
name|struct
name|obstack
modifier|*
parameter_list|,
name|int
parameter_list|,
name|int
parameter_list|,
name|void
modifier|*
function_decl|(
modifier|*
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|,
name|long
parameter_list|)
parameter_list|,
name|void
function_decl|(
modifier|*
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|)
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
specifier|extern
name|int
name|_obstack_memory_used
parameter_list|(
name|struct
name|obstack
modifier|*
parameter_list|)
function_decl|;
name|void
name|obstack_free
parameter_list|(
name|struct
name|obstack
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
comment|/* Error handler called when `obstack_chunk_alloc' failed to allocate    more memory.  This can be set to a user defined function which    should either abort gracefully or use longjump - but shouldn't    return.  The default action is to print a message and abort.  */
specifier|extern
name|void
function_decl|(
modifier|*
name|obstack_alloc_failed_handler
function_decl|)
parameter_list|(
name|void
parameter_list|)
function_decl|;
comment|/* Pointer to beginning of object being allocated or to be allocated next.    Note that this might not be the final address of the object    because a new chunk might be needed to hold the final size.  */
DECL|macro|obstack_base
define|#
directive|define
name|obstack_base
parameter_list|(
name|h
parameter_list|)
value|((void *) (h)->object_base)
comment|/* Size for allocating ordinary chunks.  */
DECL|macro|obstack_chunk_size
define|#
directive|define
name|obstack_chunk_size
parameter_list|(
name|h
parameter_list|)
value|((h)->chunk_size)
comment|/* Pointer to next byte not yet allocated in current chunk.  */
DECL|macro|obstack_next_free
define|#
directive|define
name|obstack_next_free
parameter_list|(
name|h
parameter_list|)
value|((h)->next_free)
comment|/* Mask specifying low bits that should be clear in address of an object.  */
DECL|macro|obstack_alignment_mask
define|#
directive|define
name|obstack_alignment_mask
parameter_list|(
name|h
parameter_list|)
value|((h)->alignment_mask)
comment|/* To prevent prototype warnings provide complete argument list.  */
DECL|macro|obstack_init
define|#
directive|define
name|obstack_init
parameter_list|(
name|h
parameter_list|)
define|\
value|_obstack_begin ((h), 0, 0,					\ 		  (void *(*) (long)) obstack_chunk_alloc,	\ 		  (void (*) (void *)) obstack_chunk_free)
DECL|macro|obstack_begin
define|#
directive|define
name|obstack_begin
parameter_list|(
name|h
parameter_list|,
name|size
parameter_list|)
define|\
value|_obstack_begin ((h), (size), 0,				\ 		  (void *(*) (long)) obstack_chunk_alloc,	\ 		  (void (*) (void *)) obstack_chunk_free)
DECL|macro|obstack_specify_allocation
define|#
directive|define
name|obstack_specify_allocation
parameter_list|(
name|h
parameter_list|,
name|size
parameter_list|,
name|alignment
parameter_list|,
name|chunkfun
parameter_list|,
name|freefun
parameter_list|)
define|\
value|_obstack_begin ((h), (size), (alignment),				   \ 		  (void *(*) (long)) (chunkfun),			   \ 		  (void (*) (void *)) (freefun))
DECL|macro|obstack_specify_allocation_with_arg
define|#
directive|define
name|obstack_specify_allocation_with_arg
parameter_list|(
name|h
parameter_list|,
name|size
parameter_list|,
name|alignment
parameter_list|,
name|chunkfun
parameter_list|,
name|freefun
parameter_list|,
name|arg
parameter_list|)
define|\
value|_obstack_begin_1 ((h), (size), (alignment),				\ 		    (void *(*) (void *, long)) (chunkfun),		\ 		    (void (*) (void *, void *)) (freefun), (arg))
DECL|macro|obstack_chunkfun
define|#
directive|define
name|obstack_chunkfun
parameter_list|(
name|h
parameter_list|,
name|newchunkfun
parameter_list|)
define|\
value|((h) -> chunkfun = (struct _obstack_chunk *(*)(void *, long)) (newchunkfun))
DECL|macro|obstack_freefun
define|#
directive|define
name|obstack_freefun
parameter_list|(
name|h
parameter_list|,
name|newfreefun
parameter_list|)
define|\
value|((h) -> freefun = (void (*)(void *, struct _obstack_chunk *)) (newfreefun))
DECL|macro|obstack_1grow_fast
define|#
directive|define
name|obstack_1grow_fast
parameter_list|(
name|h
parameter_list|,
name|achar
parameter_list|)
value|(*((h)->next_free)++ = (achar))
DECL|macro|obstack_blank_fast
define|#
directive|define
name|obstack_blank_fast
parameter_list|(
name|h
parameter_list|,
name|n
parameter_list|)
value|((h)->next_free += (n))
DECL|macro|obstack_memory_used
define|#
directive|define
name|obstack_memory_used
parameter_list|(
name|h
parameter_list|)
value|_obstack_memory_used (h)
if|#
directive|if
name|defined
name|__GNUC__
operator|&&
name|defined
name|__STDC__
operator|&&
name|__STDC__
comment|/* NextStep 2.0 cc is really gcc 1.93 but it defines __GNUC__ = 2 and    does not implement __extension__.  But that compiler doesn't define    __GNUC_MINOR__.  */
if|#
directive|if
name|__GNUC__
operator|<
literal|2
operator|||
operator|(
name|__NeXT__
operator|&&
operator|!
name|__GNUC_MINOR__
operator|)
DECL|macro|__extension__
define|#
directive|define
name|__extension__
endif|#
directive|endif
comment|/* For GNU C, if not -traditional,    we can define these macros to compute all args only once    without using a global variable.    Also, we can avoid using the `temp' slot, to make faster code.  */
DECL|macro|obstack_object_size
define|#
directive|define
name|obstack_object_size
parameter_list|(
name|OBSTACK
parameter_list|)
define|\
value|__extension__								\   ({ struct obstack const *__o = (OBSTACK);				\      (unsigned) (__o->next_free - __o->object_base); })
DECL|macro|obstack_room
define|#
directive|define
name|obstack_room
parameter_list|(
name|OBSTACK
parameter_list|)
define|\
value|__extension__								\   ({ struct obstack const *__o = (OBSTACK);				\      (unsigned) (__o->chunk_limit - __o->next_free); })
DECL|macro|obstack_make_room
define|#
directive|define
name|obstack_make_room
parameter_list|(
name|OBSTACK
parameter_list|,
name|length
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__o = (OBSTACK);					\    int __len = (length);						\    if (__o->chunk_limit - __o->next_free< __len)			\      _obstack_newchunk (__o, __len);					\    (void) 0; })
DECL|macro|obstack_empty_p
define|#
directive|define
name|obstack_empty_p
parameter_list|(
name|OBSTACK
parameter_list|)
define|\
value|__extension__								\   ({ struct obstack const *__o = (OBSTACK);				\      (__o->chunk->prev == 0						\&& __o->next_free == __PTR_ALIGN ((char *) __o->chunk,		\ 					__o->chunk->contents,		\ 					__o->alignment_mask)); })
DECL|macro|obstack_grow
define|#
directive|define
name|obstack_grow
parameter_list|(
name|OBSTACK
parameter_list|,
name|where
parameter_list|,
name|length
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__o = (OBSTACK);					\    int __len = (length);						\    if (__o->next_free + __len> __o->chunk_limit)			\      _obstack_newchunk (__o, __len);					\    memcpy (__o->next_free, where, __len);				\    __o->next_free += __len;						\    (void) 0; })
DECL|macro|obstack_grow0
define|#
directive|define
name|obstack_grow0
parameter_list|(
name|OBSTACK
parameter_list|,
name|where
parameter_list|,
name|length
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__o = (OBSTACK);					\    int __len = (length);						\    if (__o->next_free + __len + 1> __o->chunk_limit)			\      _obstack_newchunk (__o, __len + 1);				\    memcpy (__o->next_free, where, __len);				\    __o->next_free += __len;						\    *(__o->next_free)++ = 0;						\    (void) 0; })
DECL|macro|obstack_1grow
define|#
directive|define
name|obstack_1grow
parameter_list|(
name|OBSTACK
parameter_list|,
name|datum
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__o = (OBSTACK);					\    if (__o->next_free + 1> __o->chunk_limit)				\      _obstack_newchunk (__o, 1);					\    obstack_1grow_fast (__o, datum);					\    (void) 0; })
comment|/* These assume that the obstack alignment is good enough for pointers    or ints, and that the data added so far to the current object    shares that much alignment.  */
DECL|macro|obstack_ptr_grow
define|#
directive|define
name|obstack_ptr_grow
parameter_list|(
name|OBSTACK
parameter_list|,
name|datum
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__o = (OBSTACK);					\    if (__o->next_free + sizeof (void *)> __o->chunk_limit)		\      _obstack_newchunk (__o, sizeof (void *));				\    obstack_ptr_grow_fast (__o, datum); })				\  # define obstack_int_grow(OBSTACK,datum)				\ __extension__								\ ({ struct obstack *__o = (OBSTACK);					\    if (__o->next_free + sizeof (int)> __o->chunk_limit)		\      _obstack_newchunk (__o, sizeof (int));				\    obstack_int_grow_fast (__o, datum); })
DECL|macro|obstack_ptr_grow_fast
define|#
directive|define
name|obstack_ptr_grow_fast
parameter_list|(
name|OBSTACK
parameter_list|,
name|aptr
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__o1 = (OBSTACK);					\    *(const void **) __o1->next_free = (aptr);				\    __o1->next_free += sizeof (const void *);				\    (void) 0; })
DECL|macro|obstack_int_grow_fast
define|#
directive|define
name|obstack_int_grow_fast
parameter_list|(
name|OBSTACK
parameter_list|,
name|aint
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__o1 = (OBSTACK);					\    *(int *) __o1->next_free = (aint);					\    __o1->next_free += sizeof (int);					\    (void) 0; })
DECL|macro|obstack_blank
define|#
directive|define
name|obstack_blank
parameter_list|(
name|OBSTACK
parameter_list|,
name|length
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__o = (OBSTACK);					\    int __len = (length);						\    if (__o->chunk_limit - __o->next_free< __len)			\      _obstack_newchunk (__o, __len);					\    obstack_blank_fast (__o, __len);					\    (void) 0; })
DECL|macro|obstack_alloc
define|#
directive|define
name|obstack_alloc
parameter_list|(
name|OBSTACK
parameter_list|,
name|length
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__h = (OBSTACK);					\    obstack_blank (__h, (length));					\    obstack_finish (__h); })
DECL|macro|obstack_copy
define|#
directive|define
name|obstack_copy
parameter_list|(
name|OBSTACK
parameter_list|,
name|where
parameter_list|,
name|length
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__h = (OBSTACK);					\    obstack_grow (__h, (where), (length));				\    obstack_finish (__h); })
DECL|macro|obstack_copy0
define|#
directive|define
name|obstack_copy0
parameter_list|(
name|OBSTACK
parameter_list|,
name|where
parameter_list|,
name|length
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__h = (OBSTACK);					\    obstack_grow0 (__h, (where), (length));				\    obstack_finish (__h); })
comment|/* The local variable is named __o1 to avoid a name conflict    when obstack_blank is called.  */
DECL|macro|obstack_finish
define|#
directive|define
name|obstack_finish
parameter_list|(
name|OBSTACK
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__o1 = (OBSTACK);					\    void *__value = (void *) __o1->object_base;				\    if (__o1->next_free == __value)					\      __o1->maybe_empty_object = 1;					\    __o1->next_free							\      = __PTR_ALIGN (__o1->object_base, __o1->next_free,			\ 		    __o1->alignment_mask);				\    if (__o1->next_free - (char *)__o1->chunk				\> __o1->chunk_limit - (char *)__o1->chunk)			\      __o1->next_free = __o1->chunk_limit;				\    __o1->object_base = __o1->next_free;					\    __value; })
DECL|macro|obstack_free
define|#
directive|define
name|obstack_free
parameter_list|(
name|OBSTACK
parameter_list|,
name|OBJ
parameter_list|)
define|\
value|__extension__								\ ({ struct obstack *__o = (OBSTACK);					\    void *__obj = (OBJ);							\    if (__obj> (void *)__o->chunk&& __obj< (void *)__o->chunk_limit)  \      __o->next_free = __o->object_base = (char *)__obj;			\    else (obstack_free) (__o, __obj); })
else|#
directive|else
comment|/* not __GNUC__ or not __STDC__ */
DECL|macro|obstack_object_size
define|#
directive|define
name|obstack_object_size
parameter_list|(
name|h
parameter_list|)
define|\
value|(unsigned) ((h)->next_free - (h)->object_base)
DECL|macro|obstack_room
define|#
directive|define
name|obstack_room
parameter_list|(
name|h
parameter_list|)
define|\
value|(unsigned) ((h)->chunk_limit - (h)->next_free)
DECL|macro|obstack_empty_p
define|#
directive|define
name|obstack_empty_p
parameter_list|(
name|h
parameter_list|)
define|\
value|((h)->chunk->prev == 0							\&& (h)->next_free == __PTR_ALIGN ((char *) (h)->chunk,		\ 				    (h)->chunk->contents,		\ 				    (h)->alignment_mask))
comment|/* Note that the call to _obstack_newchunk is enclosed in (..., 0)    so that we can avoid having void expressions    in the arms of the conditional expression.    Casting the third operand to void was tried before,    but some compilers won't accept it.  */
DECL|macro|obstack_make_room
define|#
directive|define
name|obstack_make_room
parameter_list|(
name|h
parameter_list|,
name|length
parameter_list|)
define|\
value|( (h)->temp.tempint = (length),						\   (((h)->next_free + (h)->temp.tempint> (h)->chunk_limit)		\    ? (_obstack_newchunk ((h), (h)->temp.tempint), 0) : 0))
DECL|macro|obstack_grow
define|#
directive|define
name|obstack_grow
parameter_list|(
name|h
parameter_list|,
name|where
parameter_list|,
name|length
parameter_list|)
define|\
value|( (h)->temp.tempint = (length),						\   (((h)->next_free + (h)->temp.tempint> (h)->chunk_limit)		\    ? (_obstack_newchunk ((h), (h)->temp.tempint), 0) : 0),		\   memcpy ((h)->next_free, where, (h)->temp.tempint),			\   (h)->next_free += (h)->temp.tempint)
DECL|macro|obstack_grow0
define|#
directive|define
name|obstack_grow0
parameter_list|(
name|h
parameter_list|,
name|where
parameter_list|,
name|length
parameter_list|)
define|\
value|( (h)->temp.tempint = (length),						\   (((h)->next_free + (h)->temp.tempint + 1> (h)->chunk_limit)		\    ? (_obstack_newchunk ((h), (h)->temp.tempint + 1), 0) : 0),		\   memcpy ((h)->next_free, where, (h)->temp.tempint),			\   (h)->next_free += (h)->temp.tempint,					\   *((h)->next_free)++ = 0)
DECL|macro|obstack_1grow
define|#
directive|define
name|obstack_1grow
parameter_list|(
name|h
parameter_list|,
name|datum
parameter_list|)
define|\
value|( (((h)->next_free + 1> (h)->chunk_limit)				\    ? (_obstack_newchunk ((h), 1), 0) : 0),				\   obstack_1grow_fast (h, datum))
DECL|macro|obstack_ptr_grow
define|#
directive|define
name|obstack_ptr_grow
parameter_list|(
name|h
parameter_list|,
name|datum
parameter_list|)
define|\
value|( (((h)->next_free + sizeof (char *)> (h)->chunk_limit)		\    ? (_obstack_newchunk ((h), sizeof (char *)), 0) : 0),		\   obstack_ptr_grow_fast (h, datum))
DECL|macro|obstack_int_grow
define|#
directive|define
name|obstack_int_grow
parameter_list|(
name|h
parameter_list|,
name|datum
parameter_list|)
define|\
value|( (((h)->next_free + sizeof (int)> (h)->chunk_limit)			\    ? (_obstack_newchunk ((h), sizeof (int)), 0) : 0),			\   obstack_int_grow_fast (h, datum))
DECL|macro|obstack_ptr_grow_fast
define|#
directive|define
name|obstack_ptr_grow_fast
parameter_list|(
name|h
parameter_list|,
name|aptr
parameter_list|)
define|\
value|(((const void **) ((h)->next_free += sizeof (void *)))[-1] = (aptr))
DECL|macro|obstack_int_grow_fast
define|#
directive|define
name|obstack_int_grow_fast
parameter_list|(
name|h
parameter_list|,
name|aint
parameter_list|)
define|\
value|(((int *) ((h)->next_free += sizeof (int)))[-1] = (aint))
DECL|macro|obstack_blank
define|#
directive|define
name|obstack_blank
parameter_list|(
name|h
parameter_list|,
name|length
parameter_list|)
define|\
value|( (h)->temp.tempint = (length),						\   (((h)->chunk_limit - (h)->next_free< (h)->temp.tempint)		\    ? (_obstack_newchunk ((h), (h)->temp.tempint), 0) : 0),		\   obstack_blank_fast (h, (h)->temp.tempint))
DECL|macro|obstack_alloc
define|#
directive|define
name|obstack_alloc
parameter_list|(
name|h
parameter_list|,
name|length
parameter_list|)
define|\
value|(obstack_blank ((h), (length)), obstack_finish ((h)))
DECL|macro|obstack_copy
define|#
directive|define
name|obstack_copy
parameter_list|(
name|h
parameter_list|,
name|where
parameter_list|,
name|length
parameter_list|)
define|\
value|(obstack_grow ((h), (where), (length)), obstack_finish ((h)))
DECL|macro|obstack_copy0
define|#
directive|define
name|obstack_copy0
parameter_list|(
name|h
parameter_list|,
name|where
parameter_list|,
name|length
parameter_list|)
define|\
value|(obstack_grow0 ((h), (where), (length)), obstack_finish ((h)))
DECL|macro|obstack_finish
define|#
directive|define
name|obstack_finish
parameter_list|(
name|h
parameter_list|)
define|\
value|( ((h)->next_free == (h)->object_base					\    ? (((h)->maybe_empty_object = 1), 0)					\    : 0),								\   (h)->temp.tempptr = (h)->object_base,					\   (h)->next_free							\     = __PTR_ALIGN ((h)->object_base, (h)->next_free,			\ 		   (h)->alignment_mask),				\   (((h)->next_free - (char *) (h)->chunk				\> (h)->chunk_limit - (char *) (h)->chunk)				\    ? ((h)->next_free = (h)->chunk_limit) : 0),				\   (h)->object_base = (h)->next_free,					\   (h)->temp.tempptr)
DECL|macro|obstack_free
define|#
directive|define
name|obstack_free
parameter_list|(
name|h
parameter_list|,
name|obj
parameter_list|)
define|\
value|( (h)->temp.tempint = (char *) (obj) - (char *) (h)->chunk,		\   ((((h)->temp.tempint> 0						\&& (h)->temp.tempint< (h)->chunk_limit - (char *) (h)->chunk))	\    ? (int) ((h)->next_free = (h)->object_base				\ 	    = (h)->temp.tempint + (char *) (h)->chunk)			\    : (((obstack_free) ((h), (h)->temp.tempint + (char *) (h)->chunk), 0), 0)))
endif|#
directive|endif
comment|/* not __GNUC__ or not __STDC__ */
ifdef|#
directive|ifdef
name|__cplusplus
block|}
end_extern

begin_comment
comment|/* C++ */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* obstack.h */
end_comment

end_unit

