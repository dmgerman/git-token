begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_include
include|#
directive|include
file|"utf8.h"
end_include

begin_comment
comment|/* This code is originally from http://www.cl.cam.ac.uk/~mgk25/ucs/ */
end_comment

begin_struct
DECL|struct|interval
struct|struct
name|interval
block|{
DECL|member|first
name|int
name|first
decl_stmt|;
DECL|member|last
name|int
name|last
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|display_mode_esc_sequence_len
name|size_t
name|display_mode_esc_sequence_len
parameter_list|(
specifier|const
name|char
modifier|*
name|s
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|p
init|=
name|s
decl_stmt|;
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|'\033'
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|'['
condition|)
return|return
literal|0
return|;
while|while
condition|(
name|isdigit
argument_list|(
operator|*
name|p
argument_list|)
operator|||
operator|*
name|p
operator|==
literal|';'
condition|)
name|p
operator|++
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|'m'
condition|)
return|return
literal|0
return|;
return|return
name|p
operator|-
name|s
return|;
block|}
end_function

begin_comment
comment|/* auxiliary function for binary search in interval table */
end_comment

begin_function
DECL|function|bisearch
specifier|static
name|int
name|bisearch
parameter_list|(
name|ucs_char_t
name|ucs
parameter_list|,
specifier|const
name|struct
name|interval
modifier|*
name|table
parameter_list|,
name|int
name|max
parameter_list|)
block|{
name|int
name|min
init|=
literal|0
decl_stmt|;
name|int
name|mid
decl_stmt|;
if|if
condition|(
name|ucs
operator|<
name|table
index|[
literal|0
index|]
operator|.
name|first
operator|||
name|ucs
operator|>
name|table
index|[
name|max
index|]
operator|.
name|last
condition|)
return|return
literal|0
return|;
while|while
condition|(
name|max
operator|>=
name|min
condition|)
block|{
name|mid
operator|=
operator|(
name|min
operator|+
name|max
operator|)
operator|/
literal|2
expr_stmt|;
if|if
condition|(
name|ucs
operator|>
name|table
index|[
name|mid
index|]
operator|.
name|last
condition|)
name|min
operator|=
name|mid
operator|+
literal|1
expr_stmt|;
elseif|else
if|if
condition|(
name|ucs
operator|<
name|table
index|[
name|mid
index|]
operator|.
name|first
condition|)
name|max
operator|=
name|mid
operator|-
literal|1
expr_stmt|;
else|else
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/* The following two functions define the column width of an ISO 10646  * character as follows:  *  *    - The null character (U+0000) has a column width of 0.  *  *    - Other C0/C1 control characters and DEL will lead to a return  *      value of -1.  *  *    - Non-spacing and enclosing combining characters (general  *      category code Mn or Me in the Unicode database) have a  *      column width of 0.  *  *    - SOFT HYPHEN (U+00AD) has a column width of 1.  *  *    - Other format characters (general category code Cf in the Unicode  *      database) and ZERO WIDTH SPACE (U+200B) have a column width of 0.  *  *    - Hangul Jamo medial vowels and final consonants (U+1160-U+11FF)  *      have a column width of 0.  *  *    - Spacing characters in the East Asian Wide (W) or East Asian  *      Full-width (F) category as defined in Unicode Technical  *      Report #11 have a column width of 2.  *  *    - All remaining characters (including all printable  *      ISO 8859-1 and WGL4 characters, Unicode control characters,  *      etc.) have a column width of 1.  *  * This implementation assumes that ucs_char_t characters are encoded  * in ISO 10646.  */
end_comment

begin_function
DECL|function|git_wcwidth
specifier|static
name|int
name|git_wcwidth
parameter_list|(
name|ucs_char_t
name|ch
parameter_list|)
block|{
comment|/* 	 * Sorted list of non-overlapping intervals of non-spacing characters, 	 */
include|#
directive|include
file|"unicode_width.h"
comment|/* test for 8-bit control characters */
if|if
condition|(
name|ch
operator|==
literal|0
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|ch
operator|<
literal|32
operator|||
operator|(
name|ch
operator|>=
literal|0x7f
operator|&&
name|ch
operator|<
literal|0xa0
operator|)
condition|)
return|return
operator|-
literal|1
return|;
comment|/* binary search in table of non-spacing characters */
if|if
condition|(
name|bisearch
argument_list|(
name|ch
argument_list|,
name|zero_width
argument_list|,
sizeof|sizeof
argument_list|(
name|zero_width
argument_list|)
operator|/
sizeof|sizeof
argument_list|(
expr|struct
name|interval
argument_list|)
operator|-
literal|1
argument_list|)
condition|)
return|return
literal|0
return|;
comment|/* binary search in table of double width characters */
if|if
condition|(
name|bisearch
argument_list|(
name|ch
argument_list|,
name|double_width
argument_list|,
sizeof|sizeof
argument_list|(
name|double_width
argument_list|)
operator|/
sizeof|sizeof
argument_list|(
expr|struct
name|interval
argument_list|)
operator|-
literal|1
argument_list|)
condition|)
return|return
literal|2
return|;
return|return
literal|1
return|;
block|}
end_function

begin_comment
comment|/*  * Pick one ucs character starting from the location *start points at,  * and return it, while updating the *start pointer to point at the  * end of that character.  When remainder_p is not NULL, the location  * holds the number of bytes remaining in the string that we are allowed  * to pick from.  Otherwise we are allowed to pick up to the NUL that  * would eventually appear in the string.  *remainder_p is also reduced  * by the number of bytes we have consumed.  *  * If the string was not a valid UTF-8, *start pointer is set to NULL  * and the return value is undefined.  */
end_comment

begin_function
DECL|function|pick_one_utf8_char
specifier|static
name|ucs_char_t
name|pick_one_utf8_char
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|start
parameter_list|,
name|size_t
modifier|*
name|remainder_p
parameter_list|)
block|{
name|unsigned
name|char
modifier|*
name|s
init|=
operator|(
name|unsigned
name|char
operator|*
operator|)
operator|*
name|start
decl_stmt|;
name|ucs_char_t
name|ch
decl_stmt|;
name|size_t
name|remainder
decl_stmt|,
name|incr
decl_stmt|;
comment|/* 	 * A caller that assumes NUL terminated text can choose 	 * not to bother with the remainder length.  We will 	 * stop at the first NUL. 	 */
name|remainder
operator|=
operator|(
name|remainder_p
condition|?
operator|*
name|remainder_p
else|:
literal|999
operator|)
expr_stmt|;
if|if
condition|(
name|remainder
operator|<
literal|1
condition|)
block|{
goto|goto
name|invalid
goto|;
block|}
elseif|else
if|if
condition|(
operator|*
name|s
operator|<
literal|0x80
condition|)
block|{
comment|/* 0xxxxxxx */
name|ch
operator|=
operator|*
name|s
expr_stmt|;
name|incr
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|(
name|s
index|[
literal|0
index|]
operator|&
literal|0xe0
operator|)
operator|==
literal|0xc0
condition|)
block|{
comment|/* 110XXXXx 10xxxxxx */
if|if
condition|(
name|remainder
operator|<
literal|2
operator|||
operator|(
name|s
index|[
literal|1
index|]
operator|&
literal|0xc0
operator|)
operator|!=
literal|0x80
operator|||
operator|(
name|s
index|[
literal|0
index|]
operator|&
literal|0xfe
operator|)
operator|==
literal|0xc0
condition|)
goto|goto
name|invalid
goto|;
name|ch
operator|=
operator|(
operator|(
name|s
index|[
literal|0
index|]
operator|&
literal|0x1f
operator|)
operator|<<
literal|6
operator|)
operator||
operator|(
name|s
index|[
literal|1
index|]
operator|&
literal|0x3f
operator|)
expr_stmt|;
name|incr
operator|=
literal|2
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|(
name|s
index|[
literal|0
index|]
operator|&
literal|0xf0
operator|)
operator|==
literal|0xe0
condition|)
block|{
comment|/* 1110XXXX 10Xxxxxx 10xxxxxx */
if|if
condition|(
name|remainder
operator|<
literal|3
operator|||
operator|(
name|s
index|[
literal|1
index|]
operator|&
literal|0xc0
operator|)
operator|!=
literal|0x80
operator|||
operator|(
name|s
index|[
literal|2
index|]
operator|&
literal|0xc0
operator|)
operator|!=
literal|0x80
operator|||
comment|/* overlong? */
operator|(
name|s
index|[
literal|0
index|]
operator|==
literal|0xe0
operator|&&
operator|(
name|s
index|[
literal|1
index|]
operator|&
literal|0xe0
operator|)
operator|==
literal|0x80
operator|)
operator|||
comment|/* surrogate? */
operator|(
name|s
index|[
literal|0
index|]
operator|==
literal|0xed
operator|&&
operator|(
name|s
index|[
literal|1
index|]
operator|&
literal|0xe0
operator|)
operator|==
literal|0xa0
operator|)
operator|||
comment|/* U+FFFE or U+FFFF? */
operator|(
name|s
index|[
literal|0
index|]
operator|==
literal|0xef
operator|&&
name|s
index|[
literal|1
index|]
operator|==
literal|0xbf
operator|&&
operator|(
name|s
index|[
literal|2
index|]
operator|&
literal|0xfe
operator|)
operator|==
literal|0xbe
operator|)
condition|)
goto|goto
name|invalid
goto|;
name|ch
operator|=
operator|(
operator|(
name|s
index|[
literal|0
index|]
operator|&
literal|0x0f
operator|)
operator|<<
literal|12
operator|)
operator||
operator|(
operator|(
name|s
index|[
literal|1
index|]
operator|&
literal|0x3f
operator|)
operator|<<
literal|6
operator|)
operator||
operator|(
name|s
index|[
literal|2
index|]
operator|&
literal|0x3f
operator|)
expr_stmt|;
name|incr
operator|=
literal|3
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|(
name|s
index|[
literal|0
index|]
operator|&
literal|0xf8
operator|)
operator|==
literal|0xf0
condition|)
block|{
comment|/* 11110XXX 10XXxxxx 10xxxxxx 10xxxxxx */
if|if
condition|(
name|remainder
operator|<
literal|4
operator|||
operator|(
name|s
index|[
literal|1
index|]
operator|&
literal|0xc0
operator|)
operator|!=
literal|0x80
operator|||
operator|(
name|s
index|[
literal|2
index|]
operator|&
literal|0xc0
operator|)
operator|!=
literal|0x80
operator|||
operator|(
name|s
index|[
literal|3
index|]
operator|&
literal|0xc0
operator|)
operator|!=
literal|0x80
operator|||
comment|/* overlong? */
operator|(
name|s
index|[
literal|0
index|]
operator|==
literal|0xf0
operator|&&
operator|(
name|s
index|[
literal|1
index|]
operator|&
literal|0xf0
operator|)
operator|==
literal|0x80
operator|)
operator|||
comment|/*> U+10FFFF? */
operator|(
name|s
index|[
literal|0
index|]
operator|==
literal|0xf4
operator|&&
name|s
index|[
literal|1
index|]
operator|>
literal|0x8f
operator|)
operator|||
name|s
index|[
literal|0
index|]
operator|>
literal|0xf4
condition|)
goto|goto
name|invalid
goto|;
name|ch
operator|=
operator|(
operator|(
name|s
index|[
literal|0
index|]
operator|&
literal|0x07
operator|)
operator|<<
literal|18
operator|)
operator||
operator|(
operator|(
name|s
index|[
literal|1
index|]
operator|&
literal|0x3f
operator|)
operator|<<
literal|12
operator|)
operator||
operator|(
operator|(
name|s
index|[
literal|2
index|]
operator|&
literal|0x3f
operator|)
operator|<<
literal|6
operator|)
operator||
operator|(
name|s
index|[
literal|3
index|]
operator|&
literal|0x3f
operator|)
expr_stmt|;
name|incr
operator|=
literal|4
expr_stmt|;
block|}
else|else
block|{
name|invalid
label|:
operator|*
name|start
operator|=
name|NULL
expr_stmt|;
return|return
literal|0
return|;
block|}
operator|*
name|start
operator|+=
name|incr
expr_stmt|;
if|if
condition|(
name|remainder_p
condition|)
operator|*
name|remainder_p
operator|=
name|remainder
operator|-
name|incr
expr_stmt|;
return|return
name|ch
return|;
block|}
end_function

begin_comment
comment|/*  * This function returns the number of columns occupied by the character  * pointed to by the variable start. The pointer is updated to point at  * the next character. When remainder_p is not NULL, it points at the  * location that stores the number of remaining bytes we can use to pick  * a character (see pick_one_utf8_char() above).  */
end_comment

begin_function
DECL|function|utf8_width
name|int
name|utf8_width
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|start
parameter_list|,
name|size_t
modifier|*
name|remainder_p
parameter_list|)
block|{
name|ucs_char_t
name|ch
init|=
name|pick_one_utf8_char
argument_list|(
name|start
argument_list|,
name|remainder_p
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
operator|*
name|start
condition|)
return|return
literal|0
return|;
return|return
name|git_wcwidth
argument_list|(
name|ch
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * Returns the total number of columns required by a null-terminated  * string, assuming that the string is utf8.  Returns strlen() instead  * if the string does not look like a valid utf8 string.  */
end_comment

begin_function
DECL|function|utf8_strnwidth
name|int
name|utf8_strnwidth
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|int
name|len
parameter_list|,
name|int
name|skip_ansi
parameter_list|)
block|{
name|int
name|width
init|=
literal|0
decl_stmt|;
specifier|const
name|char
modifier|*
name|orig
init|=
name|string
decl_stmt|;
if|if
condition|(
name|len
operator|==
operator|-
literal|1
condition|)
name|len
operator|=
name|strlen
argument_list|(
name|string
argument_list|)
expr_stmt|;
while|while
condition|(
name|string
operator|&&
name|string
operator|<
name|orig
operator|+
name|len
condition|)
block|{
name|int
name|skip
decl_stmt|;
while|while
condition|(
name|skip_ansi
operator|&&
operator|(
name|skip
operator|=
name|display_mode_esc_sequence_len
argument_list|(
name|string
argument_list|)
operator|)
operator|!=
literal|0
condition|)
name|string
operator|+=
name|skip
expr_stmt|;
name|width
operator|+=
name|utf8_width
argument_list|(
operator|&
name|string
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
return|return
name|string
condition|?
name|width
else|:
name|len
return|;
block|}
end_function

begin_function
DECL|function|utf8_strwidth
name|int
name|utf8_strwidth
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|)
block|{
return|return
name|utf8_strnwidth
argument_list|(
name|string
argument_list|,
operator|-
literal|1
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|is_utf8
name|int
name|is_utf8
parameter_list|(
specifier|const
name|char
modifier|*
name|text
parameter_list|)
block|{
while|while
condition|(
operator|*
name|text
condition|)
block|{
if|if
condition|(
operator|*
name|text
operator|==
literal|'\n'
operator|||
operator|*
name|text
operator|==
literal|'\t'
operator|||
operator|*
name|text
operator|==
literal|'\r'
condition|)
block|{
name|text
operator|++
expr_stmt|;
continue|continue;
block|}
name|utf8_width
argument_list|(
operator|&
name|text
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|text
condition|)
return|return
literal|0
return|;
block|}
return|return
literal|1
return|;
block|}
end_function

begin_function
DECL|function|strbuf_addchars
specifier|static
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
block|{
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|memset
argument_list|(
name|sb
operator|->
name|buf
operator|+
name|sb
operator|->
name|len
argument_list|,
name|c
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|strbuf_setlen
argument_list|(
name|sb
argument_list|,
name|sb
operator|->
name|len
operator|+
name|n
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_add_indented_text
specifier|static
name|void
name|strbuf_add_indented_text
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|text
parameter_list|,
name|int
name|indent
parameter_list|,
name|int
name|indent2
parameter_list|)
block|{
if|if
condition|(
name|indent
operator|<
literal|0
condition|)
name|indent
operator|=
literal|0
expr_stmt|;
while|while
condition|(
operator|*
name|text
condition|)
block|{
specifier|const
name|char
modifier|*
name|eol
init|=
name|strchrnul
argument_list|(
name|text
argument_list|,
literal|'\n'
argument_list|)
decl_stmt|;
if|if
condition|(
operator|*
name|eol
operator|==
literal|'\n'
condition|)
name|eol
operator|++
expr_stmt|;
name|strbuf_addchars
argument_list|(
name|buf
argument_list|,
literal|' '
argument_list|,
name|indent
argument_list|)
expr_stmt|;
name|strbuf_add
argument_list|(
name|buf
argument_list|,
name|text
argument_list|,
name|eol
operator|-
name|text
argument_list|)
expr_stmt|;
name|text
operator|=
name|eol
expr_stmt|;
name|indent
operator|=
name|indent2
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/*  * Wrap the text, if necessary. The variable indent is the indent for the  * first line, indent2 is the indent for all other lines.  * If indent is negative, assume that already -indent columns have been  * consumed (and no extra indent is necessary for the first line).  */
end_comment

begin_function
DECL|function|strbuf_add_wrapped_text
name|void
name|strbuf_add_wrapped_text
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|text
parameter_list|,
name|int
name|indent1
parameter_list|,
name|int
name|indent2
parameter_list|,
name|int
name|width
parameter_list|)
block|{
name|int
name|indent
decl_stmt|,
name|w
decl_stmt|,
name|assume_utf8
init|=
literal|1
decl_stmt|;
specifier|const
name|char
modifier|*
name|bol
decl_stmt|,
modifier|*
name|space
decl_stmt|,
modifier|*
name|start
init|=
name|text
decl_stmt|;
name|size_t
name|orig_len
init|=
name|buf
operator|->
name|len
decl_stmt|;
if|if
condition|(
name|width
operator|<=
literal|0
condition|)
block|{
name|strbuf_add_indented_text
argument_list|(
name|buf
argument_list|,
name|text
argument_list|,
name|indent1
argument_list|,
name|indent2
argument_list|)
expr_stmt|;
return|return;
block|}
name|retry
label|:
name|bol
operator|=
name|text
expr_stmt|;
name|w
operator|=
name|indent
operator|=
name|indent1
expr_stmt|;
name|space
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|indent
operator|<
literal|0
condition|)
block|{
name|w
operator|=
operator|-
name|indent
expr_stmt|;
name|space
operator|=
name|text
expr_stmt|;
block|}
for|for
control|(
init|;
condition|;
control|)
block|{
name|char
name|c
decl_stmt|;
name|size_t
name|skip
decl_stmt|;
while|while
condition|(
operator|(
name|skip
operator|=
name|display_mode_esc_sequence_len
argument_list|(
name|text
argument_list|)
operator|)
condition|)
name|text
operator|+=
name|skip
expr_stmt|;
name|c
operator|=
operator|*
name|text
expr_stmt|;
if|if
condition|(
operator|!
name|c
operator|||
name|isspace
argument_list|(
name|c
argument_list|)
condition|)
block|{
if|if
condition|(
name|w
operator|<=
name|width
operator|||
operator|!
name|space
condition|)
block|{
specifier|const
name|char
modifier|*
name|start
init|=
name|bol
decl_stmt|;
if|if
condition|(
operator|!
name|c
operator|&&
name|text
operator|==
name|start
condition|)
return|return;
if|if
condition|(
name|space
condition|)
name|start
operator|=
name|space
expr_stmt|;
else|else
name|strbuf_addchars
argument_list|(
name|buf
argument_list|,
literal|' '
argument_list|,
name|indent
argument_list|)
expr_stmt|;
name|strbuf_add
argument_list|(
name|buf
argument_list|,
name|start
argument_list|,
name|text
operator|-
name|start
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|c
condition|)
return|return;
name|space
operator|=
name|text
expr_stmt|;
if|if
condition|(
name|c
operator|==
literal|'\t'
condition|)
name|w
operator||=
literal|0x07
expr_stmt|;
elseif|else
if|if
condition|(
name|c
operator|==
literal|'\n'
condition|)
block|{
name|space
operator|++
expr_stmt|;
if|if
condition|(
operator|*
name|space
operator|==
literal|'\n'
condition|)
block|{
name|strbuf_addch
argument_list|(
name|buf
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
goto|goto
name|new_line
goto|;
block|}
elseif|else
if|if
condition|(
operator|!
name|isalnum
argument_list|(
operator|*
name|space
argument_list|)
condition|)
goto|goto
name|new_line
goto|;
else|else
name|strbuf_addch
argument_list|(
name|buf
argument_list|,
literal|' '
argument_list|)
expr_stmt|;
block|}
name|w
operator|++
expr_stmt|;
name|text
operator|++
expr_stmt|;
block|}
else|else
block|{
name|new_line
label|:
name|strbuf_addch
argument_list|(
name|buf
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
name|text
operator|=
name|bol
operator|=
name|space
operator|+
name|isspace
argument_list|(
operator|*
name|space
argument_list|)
expr_stmt|;
name|space
operator|=
name|NULL
expr_stmt|;
name|w
operator|=
name|indent
operator|=
name|indent2
expr_stmt|;
block|}
continue|continue;
block|}
if|if
condition|(
name|assume_utf8
condition|)
block|{
name|w
operator|+=
name|utf8_width
argument_list|(
operator|&
name|text
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|text
condition|)
block|{
name|assume_utf8
operator|=
literal|0
expr_stmt|;
name|text
operator|=
name|start
expr_stmt|;
name|strbuf_setlen
argument_list|(
name|buf
argument_list|,
name|orig_len
argument_list|)
expr_stmt|;
goto|goto
name|retry
goto|;
block|}
block|}
else|else
block|{
name|w
operator|++
expr_stmt|;
name|text
operator|++
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|strbuf_add_wrapped_bytes
name|void
name|strbuf_add_wrapped_bytes
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|data
parameter_list|,
name|int
name|len
parameter_list|,
name|int
name|indent
parameter_list|,
name|int
name|indent2
parameter_list|,
name|int
name|width
parameter_list|)
block|{
name|char
modifier|*
name|tmp
init|=
name|xstrndup
argument_list|(
name|data
argument_list|,
name|len
argument_list|)
decl_stmt|;
name|strbuf_add_wrapped_text
argument_list|(
name|buf
argument_list|,
name|tmp
argument_list|,
name|indent
argument_list|,
name|indent2
argument_list|,
name|width
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|tmp
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|strbuf_utf8_replace
name|void
name|strbuf_utf8_replace
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb_src
parameter_list|,
name|int
name|pos
parameter_list|,
name|int
name|width
parameter_list|,
specifier|const
name|char
modifier|*
name|subst
parameter_list|)
block|{
name|struct
name|strbuf
name|sb_dst
init|=
name|STRBUF_INIT
decl_stmt|;
name|char
modifier|*
name|src
init|=
name|sb_src
operator|->
name|buf
decl_stmt|;
name|char
modifier|*
name|end
init|=
name|src
operator|+
name|sb_src
operator|->
name|len
decl_stmt|;
name|char
modifier|*
name|dst
decl_stmt|;
name|int
name|w
init|=
literal|0
decl_stmt|,
name|subst_len
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|subst
condition|)
name|subst_len
operator|=
name|strlen
argument_list|(
name|subst
argument_list|)
expr_stmt|;
name|strbuf_grow
argument_list|(
operator|&
name|sb_dst
argument_list|,
name|sb_src
operator|->
name|len
operator|+
name|subst_len
argument_list|)
expr_stmt|;
name|dst
operator|=
name|sb_dst
operator|.
name|buf
expr_stmt|;
while|while
condition|(
name|src
operator|<
name|end
condition|)
block|{
name|char
modifier|*
name|old
decl_stmt|;
name|size_t
name|n
decl_stmt|;
while|while
condition|(
operator|(
name|n
operator|=
name|display_mode_esc_sequence_len
argument_list|(
name|src
argument_list|)
operator|)
condition|)
block|{
name|memcpy
argument_list|(
name|dst
argument_list|,
name|src
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|src
operator|+=
name|n
expr_stmt|;
name|dst
operator|+=
name|n
expr_stmt|;
block|}
name|old
operator|=
name|src
expr_stmt|;
name|n
operator|=
name|utf8_width
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|*
operator|)
operator|&
name|src
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|src
condition|)
comment|/* broken utf-8, do nothing */
return|return;
if|if
condition|(
name|n
operator|&&
name|w
operator|>=
name|pos
operator|&&
name|w
operator|<
name|pos
operator|+
name|width
condition|)
block|{
if|if
condition|(
name|subst
condition|)
block|{
name|memcpy
argument_list|(
name|dst
argument_list|,
name|subst
argument_list|,
name|subst_len
argument_list|)
expr_stmt|;
name|dst
operator|+=
name|subst_len
expr_stmt|;
name|subst
operator|=
name|NULL
expr_stmt|;
block|}
name|w
operator|+=
name|n
expr_stmt|;
continue|continue;
block|}
name|memcpy
argument_list|(
name|dst
argument_list|,
name|old
argument_list|,
name|src
operator|-
name|old
argument_list|)
expr_stmt|;
name|dst
operator|+=
name|src
operator|-
name|old
expr_stmt|;
name|w
operator|+=
name|n
expr_stmt|;
block|}
name|strbuf_setlen
argument_list|(
operator|&
name|sb_dst
argument_list|,
name|dst
operator|-
name|sb_dst
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_swap
argument_list|(
name|sb_src
argument_list|,
operator|&
name|sb_dst
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|sb_dst
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|is_encoding_utf8
name|int
name|is_encoding_utf8
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
block|{
if|if
condition|(
operator|!
name|name
condition|)
return|return
literal|1
return|;
if|if
condition|(
operator|!
name|strcasecmp
argument_list|(
name|name
argument_list|,
literal|"utf-8"
argument_list|)
operator|||
operator|!
name|strcasecmp
argument_list|(
name|name
argument_list|,
literal|"utf8"
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
DECL|function|same_encoding
name|int
name|same_encoding
parameter_list|(
specifier|const
name|char
modifier|*
name|src
parameter_list|,
specifier|const
name|char
modifier|*
name|dst
parameter_list|)
block|{
if|if
condition|(
name|is_encoding_utf8
argument_list|(
name|src
argument_list|)
operator|&&
name|is_encoding_utf8
argument_list|(
name|dst
argument_list|)
condition|)
return|return
literal|1
return|;
return|return
operator|!
name|strcasecmp
argument_list|(
name|src
argument_list|,
name|dst
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * Wrapper for fprintf and returns the total number of columns required  * for the printed string, assuming that the string is utf8.  */
end_comment

begin_function
DECL|function|utf8_fprintf
name|int
name|utf8_fprintf
parameter_list|(
name|FILE
modifier|*
name|stream
parameter_list|,
specifier|const
name|char
modifier|*
name|format
parameter_list|,
modifier|...
parameter_list|)
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|va_list
name|arg
decl_stmt|;
name|int
name|columns
decl_stmt|;
name|va_start
argument_list|(
name|arg
argument_list|,
name|format
argument_list|)
expr_stmt|;
name|strbuf_vaddf
argument_list|(
operator|&
name|buf
argument_list|,
name|format
argument_list|,
name|arg
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|arg
argument_list|)
expr_stmt|;
name|columns
operator|=
name|fputs
argument_list|(
name|buf
operator|.
name|buf
argument_list|,
name|stream
argument_list|)
expr_stmt|;
if|if
condition|(
literal|0
operator|<=
name|columns
condition|)
comment|/* keep the error from the I/O */
name|columns
operator|=
name|utf8_strwidth
argument_list|(
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
return|return
name|columns
return|;
block|}
end_function

begin_comment
comment|/*  * Given a buffer and its encoding, return it re-encoded  * with iconv.  If the conversion fails, returns NULL.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|NO_ICONV
end_ifndef

begin_if
if|#
directive|if
name|defined
argument_list|(
name|OLD_ICONV
argument_list|)
operator|||
operator|(
name|defined
argument_list|(
name|__sun__
argument_list|)
operator|&&
operator|!
name|defined
argument_list|(
name|_XPG6
argument_list|)
operator|)
end_if

begin_typedef
DECL|typedef|iconv_ibp
typedef|typedef
specifier|const
name|char
modifier|*
name|iconv_ibp
typedef|;
end_typedef

begin_else
else|#
directive|else
end_else

begin_typedef
DECL|typedef|iconv_ibp
typedef|typedef
name|char
modifier|*
name|iconv_ibp
typedef|;
end_typedef

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|reencode_string_iconv
name|char
modifier|*
name|reencode_string_iconv
parameter_list|(
specifier|const
name|char
modifier|*
name|in
parameter_list|,
name|size_t
name|insz
parameter_list|,
name|iconv_t
name|conv
parameter_list|,
name|int
modifier|*
name|outsz_p
parameter_list|)
block|{
name|size_t
name|outsz
decl_stmt|,
name|outalloc
decl_stmt|;
name|char
modifier|*
name|out
decl_stmt|,
modifier|*
name|outpos
decl_stmt|;
name|iconv_ibp
name|cp
decl_stmt|;
name|outsz
operator|=
name|insz
expr_stmt|;
name|outalloc
operator|=
name|outsz
operator|+
literal|1
expr_stmt|;
comment|/* for terminating NUL */
name|out
operator|=
name|xmalloc
argument_list|(
name|outalloc
argument_list|)
expr_stmt|;
name|outpos
operator|=
name|out
expr_stmt|;
name|cp
operator|=
operator|(
name|iconv_ibp
operator|)
name|in
expr_stmt|;
while|while
condition|(
literal|1
condition|)
block|{
name|size_t
name|cnt
init|=
name|iconv
argument_list|(
name|conv
argument_list|,
operator|&
name|cp
argument_list|,
operator|&
name|insz
argument_list|,
operator|&
name|outpos
argument_list|,
operator|&
name|outsz
argument_list|)
decl_stmt|;
if|if
condition|(
name|cnt
operator|==
operator|-
literal|1
condition|)
block|{
name|size_t
name|sofar
decl_stmt|;
if|if
condition|(
name|errno
operator|!=
name|E2BIG
condition|)
block|{
name|free
argument_list|(
name|out
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
comment|/* insz has remaining number of bytes. 			 * since we started outsz the same as insz, 			 * it is likely that insz is not enough for 			 * converting the rest. 			 */
name|sofar
operator|=
name|outpos
operator|-
name|out
expr_stmt|;
name|outalloc
operator|=
name|sofar
operator|+
name|insz
operator|*
literal|2
operator|+
literal|32
expr_stmt|;
name|out
operator|=
name|xrealloc
argument_list|(
name|out
argument_list|,
name|outalloc
argument_list|)
expr_stmt|;
name|outpos
operator|=
name|out
operator|+
name|sofar
expr_stmt|;
name|outsz
operator|=
name|outalloc
operator|-
name|sofar
operator|-
literal|1
expr_stmt|;
block|}
else|else
block|{
operator|*
name|outpos
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
name|outsz_p
condition|)
operator|*
name|outsz_p
operator|=
name|outpos
operator|-
name|out
expr_stmt|;
break|break;
block|}
block|}
return|return
name|out
return|;
block|}
end_function

begin_function
DECL|function|reencode_string_len
name|char
modifier|*
name|reencode_string_len
parameter_list|(
specifier|const
name|char
modifier|*
name|in
parameter_list|,
name|int
name|insz
parameter_list|,
specifier|const
name|char
modifier|*
name|out_encoding
parameter_list|,
specifier|const
name|char
modifier|*
name|in_encoding
parameter_list|,
name|int
modifier|*
name|outsz
parameter_list|)
block|{
name|iconv_t
name|conv
decl_stmt|;
name|char
modifier|*
name|out
decl_stmt|;
if|if
condition|(
operator|!
name|in_encoding
condition|)
return|return
name|NULL
return|;
name|conv
operator|=
name|iconv_open
argument_list|(
name|out_encoding
argument_list|,
name|in_encoding
argument_list|)
expr_stmt|;
if|if
condition|(
name|conv
operator|==
operator|(
name|iconv_t
operator|)
operator|-
literal|1
condition|)
block|{
comment|/* 		 * Some platforms do not have the variously spelled variants of 		 * UTF-8, so let's fall back to trying the most official 		 * spelling. We do so only as a fallback in case the platform 		 * does understand the user's spelling, but not our official 		 * one. 		 */
if|if
condition|(
name|is_encoding_utf8
argument_list|(
name|in_encoding
argument_list|)
condition|)
name|in_encoding
operator|=
literal|"UTF-8"
expr_stmt|;
if|if
condition|(
name|is_encoding_utf8
argument_list|(
name|out_encoding
argument_list|)
condition|)
name|out_encoding
operator|=
literal|"UTF-8"
expr_stmt|;
name|conv
operator|=
name|iconv_open
argument_list|(
name|out_encoding
argument_list|,
name|in_encoding
argument_list|)
expr_stmt|;
if|if
condition|(
name|conv
operator|==
operator|(
name|iconv_t
operator|)
operator|-
literal|1
condition|)
return|return
name|NULL
return|;
block|}
name|out
operator|=
name|reencode_string_iconv
argument_list|(
name|in
argument_list|,
name|insz
argument_list|,
name|conv
argument_list|,
name|outsz
argument_list|)
expr_stmt|;
name|iconv_close
argument_list|(
name|conv
argument_list|)
expr_stmt|;
return|return
name|out
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  * Returns first character length in bytes for multi-byte `text` according to  * `encoding`.  *  * - The `text` pointer is updated to point at the next character.  * - When `remainder_p` is not NULL, on entry `*remainder_p` is how much bytes  *   we can consume from text, and on exit `*remainder_p` is reduced by returned  *   character length. Otherwise `text` is treated as limited by NUL.  */
end_comment

begin_function
DECL|function|mbs_chrlen
name|int
name|mbs_chrlen
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|text
parameter_list|,
name|size_t
modifier|*
name|remainder_p
parameter_list|,
specifier|const
name|char
modifier|*
name|encoding
parameter_list|)
block|{
name|int
name|chrlen
decl_stmt|;
specifier|const
name|char
modifier|*
name|p
init|=
operator|*
name|text
decl_stmt|;
name|size_t
name|r
init|=
operator|(
name|remainder_p
condition|?
operator|*
name|remainder_p
else|:
name|SIZE_MAX
operator|)
decl_stmt|;
if|if
condition|(
name|r
operator|<
literal|1
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|is_encoding_utf8
argument_list|(
name|encoding
argument_list|)
condition|)
block|{
name|pick_one_utf8_char
argument_list|(
operator|&
name|p
argument_list|,
operator|&
name|r
argument_list|)
expr_stmt|;
name|chrlen
operator|=
name|p
condition|?
operator|(
name|p
operator|-
operator|*
name|text
operator|)
else|:
literal|1
comment|/* not valid UTF-8 -> raw byte sequence */
expr_stmt|;
block|}
else|else
block|{
comment|/* 		 * TODO use iconv to decode one char and obtain its chrlen 		 * for now, let's treat encodings != UTF-8 as one-byte 		 */
name|chrlen
operator|=
literal|1
expr_stmt|;
block|}
operator|*
name|text
operator|+=
name|chrlen
expr_stmt|;
if|if
condition|(
name|remainder_p
condition|)
operator|*
name|remainder_p
operator|-=
name|chrlen
expr_stmt|;
return|return
name|chrlen
return|;
block|}
end_function

end_unit

