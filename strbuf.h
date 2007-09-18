begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|STRBUF_H
end_ifndef

begin_define
DECL|macro|STRBUF_H
define|#
directive|define
name|STRBUF_H
end_define

begin_comment
comment|/*  * Strbuf's can be use in many ways: as a byte array, or to store arbitrary  * long, overflow safe strings.  *  * Strbufs has some invariants that are very important to keep in mind:  *  * 1. the ->buf member is always malloc-ed, hence strbuf's can be used to  *    build complex strings/buffers whose final size isn't easily known.  *  *    It is legal to copy the ->buf pointer away. Though if you want to reuse  *    the strbuf after that, setting ->buf to NULL isn't legal.  *    `strbuf_detach' is the operation that detachs a buffer from its shell  *    while keeping the shell valid wrt its invariants.  *  * 2. the ->buf member is a byte array that has at least ->len + 1 bytes  *    allocated. The extra byte is used to store a '\0', allowing the ->buf  *    member to be a valid C-string. Every strbuf function ensure this  *    invariant is preserved.  *  *    Note that it is OK to "play" with the buffer directly if you work it  *    that way:  *  *    strbuf_grow(sb, SOME_SIZE);  *    // ... here the memory areay starting at sb->buf, and of length  *    // sb_avail(sb) is all yours, and you are sure that sb_avail(sb) is at  *    // least SOME_SIZE  *    strbuf_setlen(sb, sb->len + SOME_OTHER_SIZE);  *  *    Of course, SOME_OTHER_SIZE must be smaller or equal to sb_avail(sb).  *  *    Doing so is safe, though if it has to be done in many places, adding the  *    missing API to the strbuf module is the way to go.  *  *    XXX: do _not_ assume that the area that is yours is of size ->alloc - 1  *         even if it's true in the current implementation. Alloc is somehow a  *         "private" member that should not be messed with.  */
end_comment

begin_include
include|#
directive|include
file|<assert.h>
end_include

begin_struct
DECL|struct|strbuf
struct|struct
name|strbuf
block|{
DECL|member|alloc
name|size_t
name|alloc
decl_stmt|;
DECL|member|len
name|size_t
name|len
decl_stmt|;
DECL|member|buf
name|char
modifier|*
name|buf
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|STRBUF_INIT
define|#
directive|define
name|STRBUF_INIT
value|{ 0, 0, NULL }
end_define

begin_comment
comment|/*----- strbuf life cycle -----*/
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_init
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|size_t
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|strbuf_release
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|strbuf_reset
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|strbuf_detach
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|strbuf_attach
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|,
name|size_t
parameter_list|,
name|size_t
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|strbuf_swap
specifier|static
specifier|inline
name|void
name|strbuf_swap
parameter_list|(
name|struct
name|strbuf
modifier|*
name|a
parameter_list|,
name|struct
name|strbuf
modifier|*
name|b
parameter_list|)
block|{
name|struct
name|strbuf
name|tmp
init|=
operator|*
name|a
decl_stmt|;
operator|*
name|a
operator|=
operator|*
name|b
expr_stmt|;
operator|*
name|b
operator|=
name|tmp
expr_stmt|;
block|}
end_function

begin_comment
comment|/*----- strbuf size related -----*/
end_comment

begin_function
DECL|function|strbuf_avail
specifier|static
specifier|inline
name|size_t
name|strbuf_avail
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
block|{
return|return
name|sb
operator|->
name|alloc
condition|?
name|sb
operator|->
name|alloc
operator|-
name|sb
operator|->
name|len
operator|-
literal|1
else|:
literal|0
return|;
block|}
end_function

begin_function_decl
specifier|extern
name|void
name|strbuf_grow
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|size_t
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|strbuf_setlen
specifier|static
specifier|inline
name|void
name|strbuf_setlen
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
if|if
condition|(
operator|!
name|sb
operator|->
name|alloc
condition|)
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|assert
argument_list|(
name|len
operator|<
name|sb
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|sb
operator|->
name|len
operator|=
name|len
expr_stmt|;
name|sb
operator|->
name|buf
index|[
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
block|}
end_function

begin_comment
comment|/*----- content related -----*/
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_rtrim
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|strbuf_cmp
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|struct
name|strbuf
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*----- add data in your buffer -----*/
end_comment

begin_function
DECL|function|strbuf_addch
specifier|static
specifier|inline
name|void
name|strbuf_addch
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|int
name|c
parameter_list|)
block|{
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|sb
operator|->
name|buf
index|[
name|sb
operator|->
name|len
operator|++
index|]
operator|=
name|c
expr_stmt|;
name|sb
operator|->
name|buf
index|[
name|sb
operator|->
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
block|}
end_function

begin_function_decl
specifier|extern
name|void
name|strbuf_insert
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|size_t
name|pos
parameter_list|,
specifier|const
name|void
modifier|*
parameter_list|,
name|size_t
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|strbuf_remove
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|size_t
name|pos
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* splice pos..pos+len with given data */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_splice
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|size_t
name|pos
parameter_list|,
name|size_t
name|len
parameter_list|,
specifier|const
name|void
modifier|*
parameter_list|,
name|size_t
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|strbuf_add
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
specifier|const
name|void
modifier|*
parameter_list|,
name|size_t
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|strbuf_addstr
specifier|static
specifier|inline
name|void
name|strbuf_addstr
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|s
parameter_list|)
block|{
name|strbuf_add
argument_list|(
name|sb
argument_list|,
name|s
argument_list|,
name|strlen
argument_list|(
name|s
argument_list|)
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_addbuf
specifier|static
specifier|inline
name|void
name|strbuf_addbuf
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb2
parameter_list|)
block|{
name|strbuf_add
argument_list|(
name|sb
argument_list|,
name|sb2
operator|->
name|buf
argument_list|,
name|sb2
operator|->
name|len
argument_list|)
expr_stmt|;
block|}
end_function

begin_macro
name|__attribute__
argument_list|(
argument|(format(printf,
literal|2
argument|,
literal|3
argument|))
argument_list|)
end_macro

begin_function_decl
specifier|extern
name|void
name|strbuf_addf
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|size_t
name|strbuf_fread
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|size_t
parameter_list|,
name|FILE
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* XXX: if read fails, any partial read is undone */
end_comment

begin_function_decl
specifier|extern
name|ssize_t
name|strbuf_read
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|int
name|fd
parameter_list|,
name|size_t
name|hint
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|strbuf_read_file
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|strbuf_getline
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|FILE
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* STRBUF_H */
end_comment

end_unit

