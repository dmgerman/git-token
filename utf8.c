begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_include
include|#
directive|include
file|"utf8.h"
end_include

begin_comment
comment|/* This code is originally from http://www.cl.cam.ac.uk/~mgk25/ucs/ */
end_comment

begin_typedef
DECL|typedef|ucs_char_t
typedef|typedef
name|unsigned
name|int
name|ucs_char_t
typedef|;
end_typedef

begin_comment
DECL|typedef|ucs_char_t
comment|/* assuming 32bit int */
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
comment|/* 	 * Sorted list of non-overlapping intervals of non-spacing characters, 	 * generated by 	 *   "uniset +cat=Me +cat=Mn +cat=Cf -00AD +1160-11FF +200B c". 	 */
specifier|static
specifier|const
name|struct
name|interval
name|combining
index|[]
init|=
block|{
block|{
literal|0x0300
block|,
literal|0x0357
block|}
block|,
block|{
literal|0x035D
block|,
literal|0x036F
block|}
block|,
block|{
literal|0x0483
block|,
literal|0x0486
block|}
block|,
block|{
literal|0x0488
block|,
literal|0x0489
block|}
block|,
block|{
literal|0x0591
block|,
literal|0x05A1
block|}
block|,
block|{
literal|0x05A3
block|,
literal|0x05B9
block|}
block|,
block|{
literal|0x05BB
block|,
literal|0x05BD
block|}
block|,
block|{
literal|0x05BF
block|,
literal|0x05BF
block|}
block|,
block|{
literal|0x05C1
block|,
literal|0x05C2
block|}
block|,
block|{
literal|0x05C4
block|,
literal|0x05C4
block|}
block|,
block|{
literal|0x0600
block|,
literal|0x0603
block|}
block|,
block|{
literal|0x0610
block|,
literal|0x0615
block|}
block|,
block|{
literal|0x064B
block|,
literal|0x0658
block|}
block|,
block|{
literal|0x0670
block|,
literal|0x0670
block|}
block|,
block|{
literal|0x06D6
block|,
literal|0x06E4
block|}
block|,
block|{
literal|0x06E7
block|,
literal|0x06E8
block|}
block|,
block|{
literal|0x06EA
block|,
literal|0x06ED
block|}
block|,
block|{
literal|0x070F
block|,
literal|0x070F
block|}
block|,
block|{
literal|0x0711
block|,
literal|0x0711
block|}
block|,
block|{
literal|0x0730
block|,
literal|0x074A
block|}
block|,
block|{
literal|0x07A6
block|,
literal|0x07B0
block|}
block|,
block|{
literal|0x0901
block|,
literal|0x0902
block|}
block|,
block|{
literal|0x093C
block|,
literal|0x093C
block|}
block|,
block|{
literal|0x0941
block|,
literal|0x0948
block|}
block|,
block|{
literal|0x094D
block|,
literal|0x094D
block|}
block|,
block|{
literal|0x0951
block|,
literal|0x0954
block|}
block|,
block|{
literal|0x0962
block|,
literal|0x0963
block|}
block|,
block|{
literal|0x0981
block|,
literal|0x0981
block|}
block|,
block|{
literal|0x09BC
block|,
literal|0x09BC
block|}
block|,
block|{
literal|0x09C1
block|,
literal|0x09C4
block|}
block|,
block|{
literal|0x09CD
block|,
literal|0x09CD
block|}
block|,
block|{
literal|0x09E2
block|,
literal|0x09E3
block|}
block|,
block|{
literal|0x0A01
block|,
literal|0x0A02
block|}
block|,
block|{
literal|0x0A3C
block|,
literal|0x0A3C
block|}
block|,
block|{
literal|0x0A41
block|,
literal|0x0A42
block|}
block|,
block|{
literal|0x0A47
block|,
literal|0x0A48
block|}
block|,
block|{
literal|0x0A4B
block|,
literal|0x0A4D
block|}
block|,
block|{
literal|0x0A70
block|,
literal|0x0A71
block|}
block|,
block|{
literal|0x0A81
block|,
literal|0x0A82
block|}
block|,
block|{
literal|0x0ABC
block|,
literal|0x0ABC
block|}
block|,
block|{
literal|0x0AC1
block|,
literal|0x0AC5
block|}
block|,
block|{
literal|0x0AC7
block|,
literal|0x0AC8
block|}
block|,
block|{
literal|0x0ACD
block|,
literal|0x0ACD
block|}
block|,
block|{
literal|0x0AE2
block|,
literal|0x0AE3
block|}
block|,
block|{
literal|0x0B01
block|,
literal|0x0B01
block|}
block|,
block|{
literal|0x0B3C
block|,
literal|0x0B3C
block|}
block|,
block|{
literal|0x0B3F
block|,
literal|0x0B3F
block|}
block|,
block|{
literal|0x0B41
block|,
literal|0x0B43
block|}
block|,
block|{
literal|0x0B4D
block|,
literal|0x0B4D
block|}
block|,
block|{
literal|0x0B56
block|,
literal|0x0B56
block|}
block|,
block|{
literal|0x0B82
block|,
literal|0x0B82
block|}
block|,
block|{
literal|0x0BC0
block|,
literal|0x0BC0
block|}
block|,
block|{
literal|0x0BCD
block|,
literal|0x0BCD
block|}
block|,
block|{
literal|0x0C3E
block|,
literal|0x0C40
block|}
block|,
block|{
literal|0x0C46
block|,
literal|0x0C48
block|}
block|,
block|{
literal|0x0C4A
block|,
literal|0x0C4D
block|}
block|,
block|{
literal|0x0C55
block|,
literal|0x0C56
block|}
block|,
block|{
literal|0x0CBC
block|,
literal|0x0CBC
block|}
block|,
block|{
literal|0x0CBF
block|,
literal|0x0CBF
block|}
block|,
block|{
literal|0x0CC6
block|,
literal|0x0CC6
block|}
block|,
block|{
literal|0x0CCC
block|,
literal|0x0CCD
block|}
block|,
block|{
literal|0x0D41
block|,
literal|0x0D43
block|}
block|,
block|{
literal|0x0D4D
block|,
literal|0x0D4D
block|}
block|,
block|{
literal|0x0DCA
block|,
literal|0x0DCA
block|}
block|,
block|{
literal|0x0DD2
block|,
literal|0x0DD4
block|}
block|,
block|{
literal|0x0DD6
block|,
literal|0x0DD6
block|}
block|,
block|{
literal|0x0E31
block|,
literal|0x0E31
block|}
block|,
block|{
literal|0x0E34
block|,
literal|0x0E3A
block|}
block|,
block|{
literal|0x0E47
block|,
literal|0x0E4E
block|}
block|,
block|{
literal|0x0EB1
block|,
literal|0x0EB1
block|}
block|,
block|{
literal|0x0EB4
block|,
literal|0x0EB9
block|}
block|,
block|{
literal|0x0EBB
block|,
literal|0x0EBC
block|}
block|,
block|{
literal|0x0EC8
block|,
literal|0x0ECD
block|}
block|,
block|{
literal|0x0F18
block|,
literal|0x0F19
block|}
block|,
block|{
literal|0x0F35
block|,
literal|0x0F35
block|}
block|,
block|{
literal|0x0F37
block|,
literal|0x0F37
block|}
block|,
block|{
literal|0x0F39
block|,
literal|0x0F39
block|}
block|,
block|{
literal|0x0F71
block|,
literal|0x0F7E
block|}
block|,
block|{
literal|0x0F80
block|,
literal|0x0F84
block|}
block|,
block|{
literal|0x0F86
block|,
literal|0x0F87
block|}
block|,
block|{
literal|0x0F90
block|,
literal|0x0F97
block|}
block|,
block|{
literal|0x0F99
block|,
literal|0x0FBC
block|}
block|,
block|{
literal|0x0FC6
block|,
literal|0x0FC6
block|}
block|,
block|{
literal|0x102D
block|,
literal|0x1030
block|}
block|,
block|{
literal|0x1032
block|,
literal|0x1032
block|}
block|,
block|{
literal|0x1036
block|,
literal|0x1037
block|}
block|,
block|{
literal|0x1039
block|,
literal|0x1039
block|}
block|,
block|{
literal|0x1058
block|,
literal|0x1059
block|}
block|,
block|{
literal|0x1160
block|,
literal|0x11FF
block|}
block|,
block|{
literal|0x1712
block|,
literal|0x1714
block|}
block|,
block|{
literal|0x1732
block|,
literal|0x1734
block|}
block|,
block|{
literal|0x1752
block|,
literal|0x1753
block|}
block|,
block|{
literal|0x1772
block|,
literal|0x1773
block|}
block|,
block|{
literal|0x17B4
block|,
literal|0x17B5
block|}
block|,
block|{
literal|0x17B7
block|,
literal|0x17BD
block|}
block|,
block|{
literal|0x17C6
block|,
literal|0x17C6
block|}
block|,
block|{
literal|0x17C9
block|,
literal|0x17D3
block|}
block|,
block|{
literal|0x17DD
block|,
literal|0x17DD
block|}
block|,
block|{
literal|0x180B
block|,
literal|0x180D
block|}
block|,
block|{
literal|0x18A9
block|,
literal|0x18A9
block|}
block|,
block|{
literal|0x1920
block|,
literal|0x1922
block|}
block|,
block|{
literal|0x1927
block|,
literal|0x1928
block|}
block|,
block|{
literal|0x1932
block|,
literal|0x1932
block|}
block|,
block|{
literal|0x1939
block|,
literal|0x193B
block|}
block|,
block|{
literal|0x200B
block|,
literal|0x200F
block|}
block|,
block|{
literal|0x202A
block|,
literal|0x202E
block|}
block|,
block|{
literal|0x2060
block|,
literal|0x2063
block|}
block|,
block|{
literal|0x206A
block|,
literal|0x206F
block|}
block|,
block|{
literal|0x20D0
block|,
literal|0x20EA
block|}
block|,
block|{
literal|0x302A
block|,
literal|0x302F
block|}
block|,
block|{
literal|0x3099
block|,
literal|0x309A
block|}
block|,
block|{
literal|0xFB1E
block|,
literal|0xFB1E
block|}
block|,
block|{
literal|0xFE00
block|,
literal|0xFE0F
block|}
block|,
block|{
literal|0xFE20
block|,
literal|0xFE23
block|}
block|,
block|{
literal|0xFEFF
block|,
literal|0xFEFF
block|}
block|,
block|{
literal|0xFFF9
block|,
literal|0xFFFB
block|}
block|,
block|{
literal|0x1D167
block|,
literal|0x1D169
block|}
block|,
block|{
literal|0x1D173
block|,
literal|0x1D182
block|}
block|,
block|{
literal|0x1D185
block|,
literal|0x1D18B
block|}
block|,
block|{
literal|0x1D1AA
block|,
literal|0x1D1AD
block|}
block|,
block|{
literal|0xE0001
block|,
literal|0xE0001
block|}
block|,
block|{
literal|0xE0020
block|,
literal|0xE007F
block|}
block|,
block|{
literal|0xE0100
block|,
literal|0xE01EF
block|}
block|}
decl_stmt|;
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
name|combining
argument_list|,
sizeof|sizeof
argument_list|(
name|combining
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
comment|/* 	 * If we arrive here, ch is neither a combining nor a C0/C1 	 * control character. 	 */
return|return
literal|1
operator|+
operator|(
name|ch
operator|>=
literal|0x1100
operator|&&
comment|/* Hangul Jamo init. consonants */
operator|(
name|ch
operator|<=
literal|0x115f
operator|||
name|ch
operator|==
literal|0x2329
operator|||
name|ch
operator|==
literal|0x232a
operator|||
comment|/* CJK ... Yi */
operator|(
name|ch
operator|>=
literal|0x2e80
operator|&&
name|ch
operator|<=
literal|0xa4cf
operator|&&
name|ch
operator|!=
literal|0x303f
operator|)
operator|||
comment|/* Hangul Syllables */
operator|(
name|ch
operator|>=
literal|0xac00
operator|&&
name|ch
operator|<=
literal|0xd7a3
operator|)
operator|||
comment|/* CJK Compatibility Ideographs */
operator|(
name|ch
operator|>=
literal|0xf900
operator|&&
name|ch
operator|<=
literal|0xfaff
operator|)
operator|||
comment|/* CJK Compatibility Forms */
operator|(
name|ch
operator|>=
literal|0xfe30
operator|&&
name|ch
operator|<=
literal|0xfe6f
operator|)
operator|||
comment|/* Fullwidth Forms */
operator|(
name|ch
operator|>=
literal|0xff00
operator|&&
name|ch
operator|<=
literal|0xff60
operator|)
operator|||
operator|(
name|ch
operator|>=
literal|0xffe0
operator|&&
name|ch
operator|<=
literal|0xffe6
operator|)
operator|||
operator|(
name|ch
operator|>=
literal|0x20000
operator|&&
name|ch
operator|<=
literal|0x2fffd
operator|)
operator|||
operator|(
name|ch
operator|>=
literal|0x30000
operator|&&
name|ch
operator|<=
literal|0x3fffd
operator|)
operator|)
operator|)
return|;
block|}
end_function

begin_comment
comment|/*  * This function returns the number of columns occupied by the character  * pointed to by the variable start. The pointer is updated to point at  * the next character. If it was not valid UTF-8, the pointer is set to NULL.  */
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
operator|*
name|start
operator|+=
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
comment|/* overlong? */
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
operator|*
name|start
operator|+=
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
operator|*
name|start
operator|+=
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
operator|*
name|start
operator|+=
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
return|return
name|git_wcwidth
argument_list|(
name|ch
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
DECL|function|print_spaces
specifier|static
name|void
name|print_spaces
parameter_list|(
name|int
name|count
parameter_list|)
block|{
specifier|static
specifier|const
name|char
name|s
index|[]
init|=
literal|"                    "
decl_stmt|;
while|while
condition|(
name|count
operator|>=
sizeof|sizeof
argument_list|(
name|s
argument_list|)
condition|)
block|{
name|fwrite
argument_list|(
name|s
argument_list|,
sizeof|sizeof
argument_list|(
name|s
argument_list|)
operator|-
literal|1
argument_list|,
literal|1
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
name|count
operator|-=
sizeof|sizeof
argument_list|(
name|s
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
name|fwrite
argument_list|(
name|s
argument_list|,
name|count
argument_list|,
literal|1
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * Wrap the text, if necessary. The variable indent is the indent for the  * first line, indent2 is the indent for all other lines.  * If indent is negative, assume that already -indent columns have been  * consumed (and no extra indent is necessary for the first line).  */
end_comment

begin_function
DECL|function|print_wrapped_text
name|int
name|print_wrapped_text
parameter_list|(
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
parameter_list|,
name|int
name|width
parameter_list|)
block|{
name|int
name|w
init|=
name|indent
decl_stmt|,
name|assume_utf8
init|=
name|is_utf8
argument_list|(
name|text
argument_list|)
decl_stmt|;
specifier|const
name|char
modifier|*
name|bol
init|=
name|text
decl_stmt|,
modifier|*
name|space
init|=
name|NULL
decl_stmt|;
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
init|=
operator|*
name|text
decl_stmt|;
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
operator|<
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
name|space
condition|)
name|start
operator|=
name|space
expr_stmt|;
else|else
name|print_spaces
argument_list|(
name|indent
argument_list|)
expr_stmt|;
name|fwrite
argument_list|(
name|start
argument_list|,
name|text
operator|-
name|start
argument_list|,
literal|1
argument_list|,
name|stdout
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|c
condition|)
return|return
name|w
return|;
elseif|else
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
name|space
operator|=
name|text
expr_stmt|;
name|w
operator|++
expr_stmt|;
name|text
operator|++
expr_stmt|;
block|}
else|else
block|{
name|putchar
argument_list|(
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
name|w
operator|+=
name|utf8_width
argument_list|(
operator|&
name|text
argument_list|)
expr_stmt|;
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

begin_comment
comment|/*  * Given a buffer and its encoding, return it re-encoded  * with iconv.  If the conversion fails, returns NULL.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|NO_ICONV
end_ifndef

begin_ifdef
ifdef|#
directive|ifdef
name|OLD_ICONV
end_ifdef

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
DECL|function|reencode_string
name|char
modifier|*
name|reencode_string
parameter_list|(
specifier|const
name|char
modifier|*
name|in
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
parameter_list|)
block|{
name|iconv_t
name|conv
decl_stmt|;
name|size_t
name|insz
decl_stmt|,
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
return|return
name|NULL
return|;
name|insz
operator|=
name|strlen
argument_list|(
name|in
argument_list|)
expr_stmt|;
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
name|iconv_close
argument_list|(
name|conv
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
break|break;
block|}
block|}
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

end_unit

