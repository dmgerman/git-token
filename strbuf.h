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
comment|/**  * strbuf's are meant to be used with all the usual C string and memory  * APIs. Given that the length of the buffer is known, it's often better to  * use the mem* functions than a str* one (memchr vs. strchr e.g.).  * Though, one has to be careful about the fact that str* functions often  * stop on NULs and that strbufs may have embedded NULs.  *  * A strbuf is NUL terminated for convenience, but no function in the  * strbuf API actually relies on the string being free of NULs.  *  * strbufs have some invariants that are very important to keep in mind:  *  *  - The `buf` member is never NULL, so it can be used in any usual C  *    string operations safely. strbuf's _have_ to be initialized either by  *    `strbuf_init()` or by `= STRBUF_INIT` before the invariants, though.  *  *    Do *not* assume anything on what `buf` really is (e.g. if it is  *    allocated memory or not), use `strbuf_detach()` to unwrap a memory  *    buffer from its strbuf shell in a safe way. That is the sole supported  *    way. This will give you a malloced buffer that you can later `free()`.  *  *    However, it is totally safe to modify anything in the string pointed by  *    the `buf` member, between the indices `0` and `len-1` (inclusive).  *  *  - The `buf` member is a byte array that has at least `len + 1` bytes  *    allocated. The extra byte is used to store a `'\0'`, allowing the  *    `buf` member to be a valid C-string. Every strbuf function ensure this  *    invariant is preserved.  *  *    NOTE: It is OK to "play" with the buffer directly if you work it this  *    way:  *  *        strbuf_grow(sb, SOME_SIZE);<1>  *        strbuf_setlen(sb, sb->len + SOME_OTHER_SIZE);  *  *<1> Here, the memory array starting at `sb->buf`, and of length  *    `strbuf_avail(sb)` is all yours, and you can be sure that  *    `strbuf_avail(sb)` is at least `SOME_SIZE`.  *  *    NOTE: `SOME_OTHER_SIZE` must be smaller or equal to `strbuf_avail(sb)`.  *  *    Doing so is safe, though if it has to be done in many places, adding the  *    missing API to the strbuf module is the way to go.  *  *    WARNING: Do _not_ assume that the area that is yours is of size `alloc  *    - 1` even if it's true in the current implementation. Alloc is somehow a  *    "private" member that should not be messed with. Use `strbuf_avail()`  *    instead. */
end_comment

begin_comment
comment|/**  * Data Structures  * ---------------  */
end_comment

begin_comment
comment|/**  * This is the string buffer structure. The `len` member can be used to  * determine the current length of the string, and `buf` member provides  * access to the string itself.  */
end_comment

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

begin_decl_stmt
specifier|extern
name|char
name|strbuf_slopbuf
index|[]
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|STRBUF_INIT
define|#
directive|define
name|STRBUF_INIT
value|{ 0, 0, strbuf_slopbuf }
end_define

begin_comment
comment|/**  * Life Cycle Functions  * --------------------  */
end_comment

begin_comment
comment|/**  * Initialize the structure. The second parameter can be zero or a bigger  * number to allocate memory, in case you want to prevent further reallocs.  */
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

begin_comment
comment|/**  * Release a string buffer and the memory it used. You should not use the  * string buffer after using this function, unless you initialize it again.  */
end_comment

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

begin_comment
comment|/**  * Detach the string from the strbuf and returns it; you now own the  * storage the string occupies and it is your responsibility from then on  * to release it with `free(3)` when you are done with it.  */
end_comment

begin_function_decl
specifier|extern
name|char
modifier|*
name|strbuf_detach
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|size_t
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Attach a string to a buffer. You should specify the string to attach,  * the current length of the string and the amount of allocated memory.  * The amount must be larger than the string length, because the string you  * pass is supposed to be a NUL-terminated string.  This string _must_ be  * malloc()ed, and after attaching, the pointer cannot be relied upon  * anymore, and neither be free()d directly.  */
end_comment

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

begin_comment
comment|/**  * Swap the contents of two string buffers.  */
end_comment

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
comment|/**  * Functions related to the size of the buffer  * -------------------------------------------  */
end_comment

begin_comment
comment|/**  * Determine the amount of allocated but unused memory.  */
end_comment

begin_function
DECL|function|strbuf_avail
specifier|static
specifier|inline
name|size_t
name|strbuf_avail
parameter_list|(
specifier|const
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

begin_comment
comment|/**  * Ensure that at least this amount of unused memory is available after  * `len`. This is used when you know a typical size for what you will add  * and want to avoid repetitive automatic resizing of the underlying buffer.  * This is never a needed operation, but can be critical for performance in  * some cases.  */
end_comment

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

begin_comment
comment|/**  * Set the length of the buffer to a given value. This function does *not*  * allocate new memory, so you should not perform a `strbuf_setlen()` to a  * length that is larger than `len + strbuf_avail()`. `strbuf_setlen()` is  * just meant as a 'please fix invariants from this strbuf I just messed  * with'.  */
end_comment

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
name|len
operator|>
operator|(
name|sb
operator|->
name|alloc
condition|?
name|sb
operator|->
name|alloc
operator|-
literal|1
else|:
literal|0
operator|)
condition|)
name|die
argument_list|(
literal|"BUG: strbuf_setlen() beyond buffer"
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
comment|/**  * Empty the buffer by setting the size of it to zero.  */
end_comment

begin_define
DECL|macro|strbuf_reset
define|#
directive|define
name|strbuf_reset
parameter_list|(
name|sb
parameter_list|)
value|strbuf_setlen(sb, 0)
end_define

begin_comment
comment|/**  * Functions related to the contents of the buffer  * -----------------------------------------------  */
end_comment

begin_comment
comment|/**  * Strip whitespace from the beginning (`ltrim`), end (`rtrim`), or both side  * (`trim`) of a string.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_trim
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
name|void
name|strbuf_ltrim
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Replace the contents of the strbuf with a reencoded form.  Returns -1  * on error, 0 on success.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|strbuf_reencode
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|from
parameter_list|,
specifier|const
name|char
modifier|*
name|to
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Lowercase each character in the buffer using `tolower`.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_tolower
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Compare two buffers. Returns an integer less than, equal to, or greater  * than zero if the first buffer is found, respectively, to be less than,  * to match, or be greater than the second buffer.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|strbuf_cmp
parameter_list|(
specifier|const
name|struct
name|strbuf
modifier|*
parameter_list|,
specifier|const
name|struct
name|strbuf
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Adding data to the buffer  * -------------------------  *  * NOTE: All of the functions in this section will grow the buffer as  * necessary.  If they fail for some reason other than memory shortage and the  * buffer hadn't been allocated before (i.e. the `struct strbuf` was set to  * `STRBUF_INIT`), then they will free() it.  */
end_comment

begin_comment
comment|/**  * Add a single character to the buffer.  */
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
if|if
condition|(
operator|!
name|strbuf_avail
argument_list|(
name|sb
argument_list|)
condition|)
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

begin_comment
comment|/**  * Add a character the specified number of times to the buffer.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_addchars
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|int
name|c
parameter_list|,
name|size_t
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Insert data to the given position of the buffer. The remaining contents  * will be shifted, not overwritten.  */
end_comment

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

begin_comment
comment|/**  * Remove given amount of data from a given position of the buffer.  */
end_comment

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
comment|/**  * Remove the bytes between `pos..pos+len` and replace it with the given  * data.  */
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

begin_comment
comment|/**  * Add a NUL-terminated string to the buffer. Each line will be prepended  * by a comment character and a blank.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_add_commented_lines
parameter_list|(
name|struct
name|strbuf
modifier|*
name|out
parameter_list|,
specifier|const
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Add data of given length to the buffer.  */
end_comment

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

begin_comment
comment|/**  * Add a NUL-terminated string to the buffer.  *  * NOTE: This function will *always* be implemented as an inline or a macro  * using strlen, meaning that this is efficient to write things like:  *  *     strbuf_addstr(sb, "immediate string");  *  */
end_comment

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

begin_comment
comment|/**  * Copy the contents of another buffer at the end of the current one.  */
end_comment

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
specifier|const
name|struct
name|strbuf
modifier|*
name|sb2
parameter_list|)
block|{
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
name|sb2
operator|->
name|len
argument_list|)
expr_stmt|;
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

begin_comment
comment|/**  * Copy part of the buffer from a given position till a given length to the  * end of the buffer.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_adddup
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
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
comment|/**  * This function can be used to expand a format string containing  * placeholders. To that end, it parses the string and calls the specified  * function for every percent sign found.  *  * The callback function is given a pointer to the character after the `%`  * and a pointer to the struct strbuf.  It is expected to add the expanded  * version of the placeholder to the strbuf, e.g. to add a newline  * character if the letter `n` appears after a `%`.  The function returns  * the length of the placeholder recognized and `strbuf_expand()` skips  * over it.  *  * The format `%%` is automatically expanded to a single `%` as a quoting  * mechanism; callers do not need to handle the `%` placeholder themselves,  * and the callback function will not be invoked for this placeholder.  *  * All other characters (non-percent and not skipped ones) are copied  * verbatim to the strbuf.  If the callback returned zero, meaning that the  * placeholder is unknown, then the percent sign is copied, too.  *  * In order to facilitate caching and to make it possible to give  * parameters to the callback, `strbuf_expand()` passes a context pointer,  * which can be used by the programmer of the callback as she sees fit.  */
end_comment

begin_typedef
DECL|typedef|expand_fn_t
typedef|typedef
name|size_t
function_decl|(
modifier|*
name|expand_fn_t
function_decl|)
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|placeholder
parameter_list|,
name|void
modifier|*
name|context
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
specifier|extern
name|void
name|strbuf_expand
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
name|expand_fn_t
name|fn
parameter_list|,
name|void
modifier|*
name|context
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Used as callback for `strbuf_expand()`, expects an array of  * struct strbuf_expand_dict_entry as context, i.e. pairs of  * placeholder and replacement string.  The array needs to be  * terminated by an entry with placeholder set to NULL.  */
end_comment

begin_struct
DECL|struct|strbuf_expand_dict_entry
struct|struct
name|strbuf_expand_dict_entry
block|{
DECL|member|placeholder
specifier|const
name|char
modifier|*
name|placeholder
decl_stmt|;
DECL|member|value
specifier|const
name|char
modifier|*
name|value
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|size_t
name|strbuf_expand_dict_cb
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|placeholder
parameter_list|,
name|void
modifier|*
name|context
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Append the contents of one strbuf to another, quoting any  * percent signs ("%") into double-percents ("%%") in the  * destination. This is useful for literal data to be fed to either  * strbuf_expand or to the *printf family of functions.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_addbuf_percentquote
parameter_list|(
name|struct
name|strbuf
modifier|*
name|dst
parameter_list|,
specifier|const
name|struct
name|strbuf
modifier|*
name|src
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Append the given byte size as a human-readable string (i.e. 12.23 KiB,  * 3.50 MiB).  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_humanise_bytes
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|off_t
name|bytes
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Add a formatted string to the buffer.  */
end_comment

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
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

begin_comment
comment|/**  * Add a formatted string prepended by a comment character and a  * blank to the buffer.  */
end_comment

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|2
argument|,
literal|3
argument|))
argument_list|)
end_macro

begin_function_decl
specifier|extern
name|void
name|strbuf_commented_addf
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

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|2
argument|,
literal|0
argument|))
argument_list|)
end_macro

begin_function_decl
specifier|extern
name|void
name|strbuf_vaddf
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
name|va_list
name|ap
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Add the time specified by `tm`, as formatted by `strftime`.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_addftime
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
specifier|const
name|struct
name|tm
modifier|*
name|tm
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Read a given size of data from a FILE* pointer to the buffer.  *  * NOTE: The buffer is rewound if the read fails. If -1 is returned,  * `errno` must be consulted, like you would do for `read(3)`.  * `strbuf_read()`, `strbuf_read_file()` and `strbuf_getline()` has the  * same behaviour as well.  */
end_comment

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
comment|/**  * Read the contents of a given file descriptor. The third argument can be  * used to give a hint about the file size, to avoid reallocs.  If read fails,  * any partial read is undone.  */
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

begin_comment
comment|/**  * Read the contents of a file, specified by its path. The third argument  * can be used to give a hint about the file size, to avoid reallocs.  */
end_comment

begin_function_decl
specifier|extern
name|ssize_t
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
parameter_list|,
name|size_t
name|hint
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Read the target of a symbolic link, specified by its path.  The third  * argument can be used to give a hint about the size, to avoid reallocs.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|strbuf_readlink
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
parameter_list|,
name|size_t
name|hint
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Read a line from a FILE *, overwriting the existing contents  * of the strbuf. The second argument specifies the line  * terminator character, typically `'\n'`.  * Reading stops after the terminator or at EOF.  The terminator  * is removed from the buffer before returning.  Returns 0 unless  * there was nothing left before EOF, in which case it returns `EOF`.  */
end_comment

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

begin_comment
comment|/**  * Like `strbuf_getline`, but keeps the trailing terminator (if  * any) in the buffer.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|strbuf_getwholeline
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

begin_comment
comment|/**  * Like `strbuf_getwholeline`, but operates on a file descriptor.  * It reads one character at a time, so it is very slow.  Do not  * use it unless you need the correct position in the file  * descriptor.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|strbuf_getwholeline_fd
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
name|int
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Set the buffer to the path of the current working directory.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|strbuf_getcwd
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Add a path to a buffer, converting a relative path to an  * absolute one in the process.  Symbolic links are not  * resolved.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_add_absolute_path
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

begin_comment
comment|/**  * Strip whitespace from a buffer. The second parameter controls if  * comments are considered contents to be removed or not.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|stripspace
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
name|int
name|skip_comments
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|strbuf_strip_suffix
specifier|static
specifier|inline
name|int
name|strbuf_strip_suffix
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|suffix
parameter_list|)
block|{
if|if
condition|(
name|strip_suffix_mem
argument_list|(
name|sb
operator|->
name|buf
argument_list|,
operator|&
name|sb
operator|->
name|len
argument_list|,
name|suffix
argument_list|)
condition|)
block|{
name|strbuf_setlen
argument_list|(
name|sb
argument_list|,
name|sb
operator|->
name|len
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
else|else
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/**  * Split str (of length slen) at the specified terminator character.  * Return a null-terminated array of pointers to strbuf objects  * holding the substrings.  The substrings include the terminator,  * except for the last substring, which might be unterminated if the  * original string did not end with a terminator.  If max is positive,  * then split the string into at most max substrings (with the last  * substring containing everything following the (max-1)th terminator  * character).  *  * The most generic form is `strbuf_split_buf`, which takes an arbitrary  * pointer/len buffer. The `_str` variant takes a NUL-terminated string,  * the `_max` variant takes a strbuf, and just `strbuf_split` is a convenience  * wrapper to drop the `max` parameter.  *  * For lighter-weight alternatives, see string_list_split() and  * string_list_split_in_place().  */
end_comment

begin_function_decl
specifier|extern
name|struct
name|strbuf
modifier|*
modifier|*
name|strbuf_split_buf
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|size_t
parameter_list|,
name|int
name|terminator
parameter_list|,
name|int
name|max
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|strbuf_split_str
specifier|static
specifier|inline
name|struct
name|strbuf
modifier|*
modifier|*
name|strbuf_split_str
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
name|int
name|terminator
parameter_list|,
name|int
name|max
parameter_list|)
block|{
return|return
name|strbuf_split_buf
argument_list|(
name|str
argument_list|,
name|strlen
argument_list|(
name|str
argument_list|)
argument_list|,
name|terminator
argument_list|,
name|max
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|strbuf_split_max
specifier|static
specifier|inline
name|struct
name|strbuf
modifier|*
modifier|*
name|strbuf_split_max
parameter_list|(
specifier|const
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|int
name|terminator
parameter_list|,
name|int
name|max
parameter_list|)
block|{
return|return
name|strbuf_split_buf
argument_list|(
name|sb
operator|->
name|buf
argument_list|,
name|sb
operator|->
name|len
argument_list|,
name|terminator
argument_list|,
name|max
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|strbuf_split
specifier|static
specifier|inline
name|struct
name|strbuf
modifier|*
modifier|*
name|strbuf_split
parameter_list|(
specifier|const
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|int
name|terminator
parameter_list|)
block|{
return|return
name|strbuf_split_max
argument_list|(
name|sb
argument_list|,
name|terminator
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/**  * Free a NULL-terminated list of strbufs (for example, the return  * values of the strbuf_split*() functions).  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_list_free
parameter_list|(
name|struct
name|strbuf
modifier|*
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Add the abbreviation, as generated by find_unique_abbrev, of `sha1` to  * the strbuf `sb`.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_add_unique_abbrev
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
name|abbrev_len
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Launch the user preferred editor to edit a file and fill the buffer  * with the file's contents upon the user completing their editing. The  * third argument can be used to set the environment which the editor is  * run in. If the buffer is NULL the editor is launched as usual but the  * file's contents are not read into the buffer upon completion.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|launch_editor
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|struct
name|strbuf
modifier|*
name|buffer
parameter_list|,
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|env
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|strbuf_add_lines
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Append s to sb, with the characters '<', '>', '&' and '"' converted  * into XML entities.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|strbuf_addstr_xml_quoted
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
function_decl|;
end_function_decl

begin_comment
comment|/**  * "Complete" the contents of `sb` by ensuring that either it ends with the  * character `term`, or it is empty.  This can be used, for example,  * to ensure that text ends with a newline, but without creating an empty  * blank line if there is no content in the first place.  */
end_comment

begin_function
DECL|function|strbuf_complete
specifier|static
specifier|inline
name|void
name|strbuf_complete
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
name|char
name|term
parameter_list|)
block|{
if|if
condition|(
name|sb
operator|->
name|len
operator|&&
name|sb
operator|->
name|buf
index|[
name|sb
operator|->
name|len
operator|-
literal|1
index|]
operator|!=
name|term
condition|)
name|strbuf_addch
argument_list|(
name|sb
argument_list|,
name|term
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_complete_line
specifier|static
specifier|inline
name|void
name|strbuf_complete_line
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
block|{
name|strbuf_complete
argument_list|(
name|sb
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
block|}
end_function

begin_function_decl
specifier|extern
name|int
name|strbuf_branchname
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|strbuf_check_branch_ref
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|strbuf_addstr_urlencode
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|int
name|reserved
parameter_list|)
function_decl|;
end_function_decl

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|1
argument|,
literal|2
argument|))
argument_list|)
end_macro

begin_function_decl
specifier|extern
name|int
name|printf_ln
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|2
argument|,
literal|3
argument|))
argument_list|)
end_macro

begin_function_decl
specifier|extern
name|int
name|fprintf_ln
parameter_list|(
name|FILE
modifier|*
name|fp
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
name|char
modifier|*
name|xstrdup_tolower
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Create a newly allocated string using printf format. You can do this easily  * with a strbuf, but this provides a shortcut to save a few lines.  */
end_comment

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|1
argument|,
literal|0
argument|))
argument_list|)
end_macro

begin_function_decl
name|char
modifier|*
name|xstrvfmt
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
name|va_list
name|ap
parameter_list|)
function_decl|;
end_function_decl

begin_macro
name|__attribute__
argument_list|(
argument|(format (printf,
literal|1
argument|,
literal|2
argument|))
argument_list|)
end_macro

begin_function_decl
name|char
modifier|*
name|xstrfmt
parameter_list|(
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
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

