begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/**  * Copyright 2013, GitHub, Inc  * Copyright 2009-2013, Daniel Lemire, Cliff Moon,  *	David McIntosh, Robert Becho, Google Inc. and Veronika Zenz  *  * This program is free software; you can redistribute it and/or  * modify it under the terms of the GNU General Public License  * as published by the Free Software Foundation; either version 2  * of the License, or (at your option) any later version.  *  * This program is distributed in the hope that it will be useful,  * but WITHOUT ANY WARRANTY; without even the implied warranty of  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  * GNU General Public License for more details.  *  * You should have received a copy of the GNU General Public License  * along with this program; if not, write to the Free Software  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  */
end_comment

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"ewok.h"
end_include

begin_include
include|#
directive|include
file|"ewok_rlw.h"
end_include

begin_function
DECL|function|next_word
specifier|static
specifier|inline
name|int
name|next_word
parameter_list|(
name|struct
name|rlw_iterator
modifier|*
name|it
parameter_list|)
block|{
if|if
condition|(
name|it
operator|->
name|pointer
operator|>=
name|it
operator|->
name|size
condition|)
return|return
literal|0
return|;
name|it
operator|->
name|rlw
operator|.
name|word
operator|=
operator|&
name|it
operator|->
name|buffer
index|[
name|it
operator|->
name|pointer
index|]
expr_stmt|;
name|it
operator|->
name|pointer
operator|+=
name|rlw_get_literal_words
argument_list|(
name|it
operator|->
name|rlw
operator|.
name|word
argument_list|)
operator|+
literal|1
expr_stmt|;
name|it
operator|->
name|rlw
operator|.
name|literal_words
operator|=
name|rlw_get_literal_words
argument_list|(
name|it
operator|->
name|rlw
operator|.
name|word
argument_list|)
expr_stmt|;
name|it
operator|->
name|rlw
operator|.
name|running_len
operator|=
name|rlw_get_running_len
argument_list|(
name|it
operator|->
name|rlw
operator|.
name|word
argument_list|)
expr_stmt|;
name|it
operator|->
name|rlw
operator|.
name|running_bit
operator|=
name|rlw_get_run_bit
argument_list|(
name|it
operator|->
name|rlw
operator|.
name|word
argument_list|)
expr_stmt|;
name|it
operator|->
name|rlw
operator|.
name|literal_word_offset
operator|=
literal|0
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|rlwit_init
name|void
name|rlwit_init
parameter_list|(
name|struct
name|rlw_iterator
modifier|*
name|it
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|from_ewah
parameter_list|)
block|{
name|it
operator|->
name|buffer
operator|=
name|from_ewah
operator|->
name|buffer
expr_stmt|;
name|it
operator|->
name|size
operator|=
name|from_ewah
operator|->
name|buffer_size
expr_stmt|;
name|it
operator|->
name|pointer
operator|=
literal|0
expr_stmt|;
name|next_word
argument_list|(
name|it
argument_list|)
expr_stmt|;
name|it
operator|->
name|literal_word_start
operator|=
name|rlwit_literal_words
argument_list|(
name|it
argument_list|)
operator|+
name|it
operator|->
name|rlw
operator|.
name|literal_word_offset
expr_stmt|;
block|}
end_function

begin_function
DECL|function|rlwit_discard_first_words
name|void
name|rlwit_discard_first_words
parameter_list|(
name|struct
name|rlw_iterator
modifier|*
name|it
parameter_list|,
name|size_t
name|x
parameter_list|)
block|{
while|while
condition|(
name|x
operator|>
literal|0
condition|)
block|{
name|size_t
name|discard
decl_stmt|;
if|if
condition|(
name|it
operator|->
name|rlw
operator|.
name|running_len
operator|>
name|x
condition|)
block|{
name|it
operator|->
name|rlw
operator|.
name|running_len
operator|-=
name|x
expr_stmt|;
return|return;
block|}
name|x
operator|-=
name|it
operator|->
name|rlw
operator|.
name|running_len
expr_stmt|;
name|it
operator|->
name|rlw
operator|.
name|running_len
operator|=
literal|0
expr_stmt|;
name|discard
operator|=
operator|(
name|x
operator|>
name|it
operator|->
name|rlw
operator|.
name|literal_words
operator|)
condition|?
name|it
operator|->
name|rlw
operator|.
name|literal_words
else|:
name|x
expr_stmt|;
name|it
operator|->
name|literal_word_start
operator|+=
name|discard
expr_stmt|;
name|it
operator|->
name|rlw
operator|.
name|literal_words
operator|-=
name|discard
expr_stmt|;
name|x
operator|-=
name|discard
expr_stmt|;
if|if
condition|(
name|x
operator|>
literal|0
operator|||
name|rlwit_word_size
argument_list|(
name|it
argument_list|)
operator|==
literal|0
condition|)
block|{
if|if
condition|(
operator|!
name|next_word
argument_list|(
name|it
argument_list|)
condition|)
break|break;
name|it
operator|->
name|literal_word_start
operator|=
name|rlwit_literal_words
argument_list|(
name|it
argument_list|)
operator|+
name|it
operator|->
name|rlw
operator|.
name|literal_word_offset
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|rlwit_discharge
name|size_t
name|rlwit_discharge
parameter_list|(
name|struct
name|rlw_iterator
modifier|*
name|it
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|out
parameter_list|,
name|size_t
name|max
parameter_list|,
name|int
name|negate
parameter_list|)
block|{
name|size_t
name|index
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|index
operator|<
name|max
operator|&&
name|rlwit_word_size
argument_list|(
name|it
argument_list|)
operator|>
literal|0
condition|)
block|{
name|size_t
name|pd
decl_stmt|,
name|pl
init|=
name|it
operator|->
name|rlw
operator|.
name|running_len
decl_stmt|;
if|if
condition|(
name|index
operator|+
name|pl
operator|>
name|max
condition|)
name|pl
operator|=
name|max
operator|-
name|index
expr_stmt|;
name|ewah_add_empty_words
argument_list|(
name|out
argument_list|,
name|it
operator|->
name|rlw
operator|.
name|running_bit
operator|^
name|negate
argument_list|,
name|pl
argument_list|)
expr_stmt|;
name|index
operator|+=
name|pl
expr_stmt|;
name|pd
operator|=
name|it
operator|->
name|rlw
operator|.
name|literal_words
expr_stmt|;
if|if
condition|(
name|pd
operator|+
name|index
operator|>
name|max
condition|)
name|pd
operator|=
name|max
operator|-
name|index
expr_stmt|;
name|ewah_add_dirty_words
argument_list|(
name|out
argument_list|,
name|it
operator|->
name|buffer
operator|+
name|it
operator|->
name|literal_word_start
argument_list|,
name|pd
argument_list|,
name|negate
argument_list|)
expr_stmt|;
name|rlwit_discard_first_words
argument_list|(
name|it
argument_list|,
name|pd
operator|+
name|pl
argument_list|)
expr_stmt|;
name|index
operator|+=
name|pd
expr_stmt|;
block|}
return|return
name|index
return|;
block|}
end_function

begin_function
DECL|function|rlwit_discharge_empty
name|void
name|rlwit_discharge_empty
parameter_list|(
name|struct
name|rlw_iterator
modifier|*
name|it
parameter_list|,
name|struct
name|ewah_bitmap
modifier|*
name|out
parameter_list|)
block|{
while|while
condition|(
name|rlwit_word_size
argument_list|(
name|it
argument_list|)
operator|>
literal|0
condition|)
block|{
name|ewah_add_empty_words
argument_list|(
name|out
argument_list|,
literal|0
argument_list|,
name|rlwit_word_size
argument_list|(
name|it
argument_list|)
argument_list|)
expr_stmt|;
name|rlwit_discard_first_words
argument_list|(
name|it
argument_list|,
name|rlwit_word_size
argument_list|(
name|it
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit
