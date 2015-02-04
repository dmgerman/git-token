begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/**  * Copyright 2013, GitHub, Inc  * Copyright 2009-2013, Daniel Lemire, Cliff Moon,  *	David McIntosh, Robert Becho, Google Inc. and Veronika Zenz  *  * This program is free software; you can redistribute it and/or  * modify it under the terms of the GNU General Public License  * as published by the Free Software Foundation; either version 2  * of the License, or (at your option) any later version.  *  * This program is distributed in the hope that it will be useful,  * but WITHOUT ANY WARRANTY; without even the implied warranty of  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  * GNU General Public License for more details.  *  * You should have received a copy of the GNU General Public License  * along with this program; if not, write to the Free Software  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|__EWOK_BITMAP_H__
end_ifndef

begin_define
DECL|macro|__EWOK_BITMAP_H__
define|#
directive|define
name|__EWOK_BITMAP_H__
end_define

begin_ifndef
ifndef|#
directive|ifndef
name|ewah_malloc
end_ifndef

begin_define
DECL|macro|ewah_malloc
define|#
directive|define
name|ewah_malloc
value|xmalloc
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|ewah_realloc
end_ifndef

begin_define
DECL|macro|ewah_realloc
define|#
directive|define
name|ewah_realloc
value|xrealloc
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|ewah_calloc
end_ifndef

begin_define
DECL|macro|ewah_calloc
define|#
directive|define
name|ewah_calloc
value|xcalloc
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_typedef
DECL|typedef|eword_t
typedef|typedef
name|uint64_t
name|eword_t
typedef|;
end_typedef

begin_define
DECL|macro|BITS_IN_WORD
define|#
directive|define
name|BITS_IN_WORD
value|(sizeof(eword_t) * 8)
end_define

begin_comment
comment|/**  * Do not use __builtin_popcountll. The GCC implementation  * is notoriously slow on all platforms.  *  * See: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=36041  */
end_comment

begin_function
DECL|function|ewah_bit_popcount64
specifier|static
specifier|inline
name|uint32_t
name|ewah_bit_popcount64
parameter_list|(
name|uint64_t
name|x
parameter_list|)
block|{
name|x
operator|=
operator|(
name|x
operator|&
literal|0x5555555555555555ULL
operator|)
operator|+
operator|(
operator|(
name|x
operator|>>
literal|1
operator|)
operator|&
literal|0x5555555555555555ULL
operator|)
expr_stmt|;
name|x
operator|=
operator|(
name|x
operator|&
literal|0x3333333333333333ULL
operator|)
operator|+
operator|(
operator|(
name|x
operator|>>
literal|2
operator|)
operator|&
literal|0x3333333333333333ULL
operator|)
expr_stmt|;
name|x
operator|=
operator|(
name|x
operator|&
literal|0x0F0F0F0F0F0F0F0FULL
operator|)
operator|+
operator|(
operator|(
name|x
operator|>>
literal|4
operator|)
operator|&
literal|0x0F0F0F0F0F0F0F0FULL
operator|)
expr_stmt|;
return|return
operator|(
name|x
operator|*
literal|0x0101010101010101ULL
operator|)
operator|>>
literal|56
return|;
block|}
end_function

begin_comment
comment|/* __builtin_ctzll was not available until 3.4.0 */
end_comment

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__GNUC__
argument_list|)
operator|&&
operator|(
name|__GNUC__
operator|>
literal|3
operator|||
operator|(
name|__GNUC__
operator|==
literal|3
operator|&&
name|__GNUC_MINOR
operator|>
literal|3
operator|)
operator|)
end_if

begin_define
DECL|macro|ewah_bit_ctz64
define|#
directive|define
name|ewah_bit_ctz64
parameter_list|(
name|x
parameter_list|)
value|__builtin_ctzll(x)
end_define

begin_else
else|#
directive|else
end_else

begin_function
DECL|function|ewah_bit_ctz64
specifier|static
specifier|inline
name|int
name|ewah_bit_ctz64
parameter_list|(
name|uint64_t
name|x
parameter_list|)
block|{
name|int
name|n
init|=
literal|0
decl_stmt|;
if|if
condition|(
operator|(
name|x
operator|&
literal|0xffffffff
operator|)
operator|==
literal|0
condition|)
block|{
name|x
operator|>>=
literal|32
expr_stmt|;
name|n
operator|+=
literal|32
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|x
operator|&
literal|0xffff
operator|)
operator|==
literal|0
condition|)
block|{
name|x
operator|>>=
literal|16
expr_stmt|;
name|n
operator|+=
literal|16
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|x
operator|&
literal|0xff
operator|)
operator|==
literal|0
condition|)
block|{
name|x
operator|>>=
literal|8
expr_stmt|;
name|n
operator|+=
literal|8
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|x
operator|&
literal|0xf
operator|)
operator|==
literal|0
condition|)
block|{
name|x
operator|>>=
literal|4
expr_stmt|;
name|n
operator|+=
literal|4
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|x
operator|&
literal|0x3
operator|)
operator|==
literal|0
condition|)
block|{
name|x
operator|>>=
literal|2
expr_stmt|;
name|n
operator|+=
literal|2
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|x
operator|&
literal|0x1
operator|)
operator|==
literal|0
condition|)
block|{
name|x
operator|>>=
literal|1
expr_stmt|;
name|n
operator|+=
literal|1
expr_stmt|;
block|}
return|return
name|n
operator|+
operator|!
name|x
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_struct
DECL|struct|ewah_bitmap
struct|struct
name|ewah_bitmap
block|{
DECL|member|buffer
name|eword_t
modifier|*
name|buffer
decl_stmt|;
DECL|member|buffer_size
name|size_t
name|buffer_size
decl_stmt|;
DECL|member|alloc_size
name|size_t
name|alloc_size
decl_stmt|;
DECL|member|bit_size
name|size_t
name|bit_size
decl_stmt|;
DECL|member|rlw
name|eword_t
modifier|*
name|rlw
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|typedef|ewah_callback
typedef|typedef
name|void
function_decl|(
modifier|*
name|ewah_callback
function_decl|)
parameter_list|(
name|size_t
name|pos
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|struct
name|ewah_bitmap
modifier|*
name|ewah_pool_new
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ewah_pool_free
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Allocate a new EWAH Compressed bitmap  */
end_comment

begin_function_decl
name|struct
name|ewah_bitmap
modifier|*
name|ewah_new
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Clear all the bits in the bitmap. Does not free or resize  * memory.  */
end_comment

begin_function_decl
name|void
name|ewah_clear
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Free all the memory of the bitmap  */
end_comment

begin_function_decl
name|void
name|ewah_free
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ewah_serialize_to
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
name|int
function_decl|(
modifier|*
name|write_fun
function_decl|)
parameter_list|(
name|void
modifier|*
name|out
parameter_list|,
specifier|const
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
parameter_list|,
name|void
modifier|*
name|out
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ewah_serialize
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
name|int
name|fd
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ewah_serialize_native
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
name|int
name|fd
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ewah_deserialize
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
name|int
name|fd
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ewah_read_mmap
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
name|void
modifier|*
name|map
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ewah_read_mmap_native
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
name|void
modifier|*
name|map
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|uint32_t
name|ewah_checksum
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Logical not (bitwise negation) in-place on the bitmap  *  * This operation is linear time based on the size of the bitmap.  */
end_comment

begin_function_decl
name|void
name|ewah_not
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Call the given callback with the position of every single bit  * that has been set on the bitmap.  *  * This is an efficient operation that does not fully decompress  * the bitmap.  */
end_comment

begin_function_decl
name|void
name|ewah_each_bit
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
name|ewah_callback
name|callback
parameter_list|,
name|void
modifier|*
name|payload
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Set a given bit on the bitmap.  *  * The bit at position `pos` will be set to true. Because of the  * way that the bitmap is compressed, a set bit cannot be unset  * later on.  *  * Furthermore, since the bitmap uses streaming compression, bits  * can only set incrementally.  *  * E.g.  *		ewah_set(bitmap, 1); // ok  *		ewah_set(bitmap, 76); // ok  *		ewah_set(bitmap, 77); // ok  *		ewah_set(bitmap, 8712800127); // ok  *		ewah_set(bitmap, 25); // failed, assert raised  */
end_comment

begin_function_decl
name|void
name|ewah_set
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
name|size_t
name|i
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|ewah_iterator
struct|struct
name|ewah_iterator
block|{
DECL|member|buffer
specifier|const
name|eword_t
modifier|*
name|buffer
decl_stmt|;
DECL|member|buffer_size
name|size_t
name|buffer_size
decl_stmt|;
DECL|member|pointer
name|size_t
name|pointer
decl_stmt|;
DECL|member|compressed
DECL|member|literals
name|eword_t
name|compressed
decl_stmt|,
name|literals
decl_stmt|;
DECL|member|rl
DECL|member|lw
name|eword_t
name|rl
decl_stmt|,
name|lw
decl_stmt|;
DECL|member|b
name|int
name|b
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/**  * Initialize a new iterator to run through the bitmap in uncompressed form.  *  * The iterator can be stack allocated. The underlying bitmap must not be freed  * before the iteration is over.  *  * E.g.  *  *		struct ewah_bitmap *bitmap = ewah_new();  *		struct ewah_iterator it;  *  *		ewah_iterator_init(&it, bitmap);  */
end_comment

begin_function_decl
name|void
name|ewah_iterator_init
parameter_list|(
name|struct
name|ewah_iterator
modifier|*
name|it
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|parent
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Yield every single word in the bitmap in uncompressed form. This is:  * yield single words (32-64 bits) where each bit represents an actual  * bit from the bitmap.  *  * Return: true if a word was yield, false if there are no words left  */
end_comment

begin_function_decl
name|int
name|ewah_iterator_next
parameter_list|(
name|eword_t
modifier|*
name|next
parameter_list|,
name|struct
name|ewah_iterator
modifier|*
name|it
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ewah_or
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|ewah_i
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|ewah_j
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|out
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ewah_and_not
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|ewah_i
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|ewah_j
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|out
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ewah_xor
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|ewah_i
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|ewah_j
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|out
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ewah_and
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|ewah_i
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|ewah_j
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|out
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Direct word access  */
end_comment

begin_function_decl
name|size_t
name|ewah_add_empty_words
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
name|int
name|v
parameter_list|,
name|size_t
name|number
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ewah_add_dirty_words
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
specifier|const
name|eword_t
modifier|*
name|buffer
parameter_list|,
name|size_t
name|number
parameter_list|,
name|int
name|negate
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|size_t
name|ewah_add
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|self
parameter_list|,
name|eword_t
name|word
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Uncompressed, old-school bitmap that can be efficiently compressed  * into an `ewah_bitmap`.  */
end_comment

begin_struct
DECL|struct|bitmap
struct|struct
name|bitmap
block|{
DECL|member|words
name|eword_t
modifier|*
name|words
decl_stmt|;
DECL|member|word_alloc
name|size_t
name|word_alloc
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|bitmap
modifier|*
name|bitmap_new
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|bitmap_set
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|,
name|size_t
name|pos
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|bitmap_clear
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|,
name|size_t
name|pos
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|bitmap_get
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|,
name|size_t
name|pos
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|bitmap_reset
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|bitmap_free
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|bitmap_equals
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|,
name|struct
name|bitmap
modifier|*
name|other
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|bitmap_is_subset
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|,
name|struct
name|bitmap
modifier|*
name|super
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|ewah_bitmap
modifier|*
name|bitmap_to_ewah
parameter_list|(
name|struct
name|bitmap
modifier|*
name|bitmap
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|bitmap
modifier|*
name|ewah_to_bitmap
parameter_list|(
name|struct
name|ewah_bitmap
modifier|*
name|ewah
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|bitmap_and_not
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|,
name|struct
name|bitmap
modifier|*
name|other
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|bitmap_or_ewah
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|other
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|bitmap_or
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|,
specifier|const
name|struct
name|bitmap
modifier|*
name|other
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|bitmap_each_bit
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|,
name|ewah_callback
name|callback
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|size_t
name|bitmap_popcount
parameter_list|(
name|struct
name|bitmap
modifier|*
name|self
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

