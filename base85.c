begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_undef
DECL|macro|DEBUG_85
undef|#
directive|undef
name|DEBUG_85
end_undef

begin_ifdef
ifdef|#
directive|ifdef
name|DEBUG_85
end_ifdef

begin_define
DECL|macro|say
define|#
directive|define
name|say
parameter_list|(
name|a
parameter_list|)
value|fprintf(stderr, a)
end_define

begin_define
DECL|macro|say1
define|#
directive|define
name|say1
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
value|fprintf(stderr, a, b)
end_define

begin_define
DECL|macro|say2
define|#
directive|define
name|say2
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|,
name|c
parameter_list|)
value|fprintf(stderr, a, b, c)
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|say
define|#
directive|define
name|say
parameter_list|(
name|a
parameter_list|)
value|do {} while(0)
end_define

begin_define
DECL|macro|say1
define|#
directive|define
name|say1
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
value|do {} while(0)
end_define

begin_define
DECL|macro|say2
define|#
directive|define
name|say2
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|,
name|c
parameter_list|)
value|do {} while(0)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|en85
specifier|static
specifier|const
name|char
name|en85
index|[]
init|=
block|{
literal|'0'
block|,
literal|'1'
block|,
literal|'2'
block|,
literal|'3'
block|,
literal|'4'
block|,
literal|'5'
block|,
literal|'6'
block|,
literal|'7'
block|,
literal|'8'
block|,
literal|'9'
block|,
literal|'A'
block|,
literal|'B'
block|,
literal|'C'
block|,
literal|'D'
block|,
literal|'E'
block|,
literal|'F'
block|,
literal|'G'
block|,
literal|'H'
block|,
literal|'I'
block|,
literal|'J'
block|,
literal|'K'
block|,
literal|'L'
block|,
literal|'M'
block|,
literal|'N'
block|,
literal|'O'
block|,
literal|'P'
block|,
literal|'Q'
block|,
literal|'R'
block|,
literal|'S'
block|,
literal|'T'
block|,
literal|'U'
block|,
literal|'V'
block|,
literal|'W'
block|,
literal|'X'
block|,
literal|'Y'
block|,
literal|'Z'
block|,
literal|'a'
block|,
literal|'b'
block|,
literal|'c'
block|,
literal|'d'
block|,
literal|'e'
block|,
literal|'f'
block|,
literal|'g'
block|,
literal|'h'
block|,
literal|'i'
block|,
literal|'j'
block|,
literal|'k'
block|,
literal|'l'
block|,
literal|'m'
block|,
literal|'n'
block|,
literal|'o'
block|,
literal|'p'
block|,
literal|'q'
block|,
literal|'r'
block|,
literal|'s'
block|,
literal|'t'
block|,
literal|'u'
block|,
literal|'v'
block|,
literal|'w'
block|,
literal|'x'
block|,
literal|'y'
block|,
literal|'z'
block|,
literal|'!'
block|,
literal|'#'
block|,
literal|'$'
block|,
literal|'%'
block|,
literal|'&'
block|,
literal|'('
block|,
literal|')'
block|,
literal|'*'
block|,
literal|'+'
block|,
literal|'-'
block|,
literal|';'
block|,
literal|'<'
block|,
literal|'='
block|,
literal|'>'
block|,
literal|'?'
block|,
literal|'@'
block|,
literal|'^'
block|,
literal|'_'
block|,
literal|'`'
block|,
literal|'{'
block|,
literal|'|'
block|,
literal|'}'
block|,
literal|'~'
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|de85
specifier|static
name|char
name|de85
index|[
literal|256
index|]
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|prep_base85
specifier|static
name|void
name|prep_base85
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
if|if
condition|(
name|de85
index|[
literal|'Z'
index|]
condition|)
return|return;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ARRAY_SIZE
argument_list|(
name|en85
argument_list|)
condition|;
name|i
operator|++
control|)
block|{
name|int
name|ch
init|=
name|en85
index|[
name|i
index|]
decl_stmt|;
name|de85
index|[
name|ch
index|]
operator|=
name|i
operator|+
literal|1
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|decode_85
name|int
name|decode_85
parameter_list|(
name|char
modifier|*
name|dst
parameter_list|,
name|char
modifier|*
name|buffer
parameter_list|,
name|int
name|len
parameter_list|)
block|{
name|prep_base85
argument_list|()
expr_stmt|;
name|say2
argument_list|(
literal|"decode 85<%.*s>"
argument_list|,
name|len
operator|/
literal|4
operator|*
literal|5
argument_list|,
name|buffer
argument_list|)
expr_stmt|;
while|while
condition|(
name|len
condition|)
block|{
name|unsigned
name|acc
init|=
literal|0
decl_stmt|;
name|int
name|cnt
decl_stmt|;
for|for
control|(
name|cnt
operator|=
literal|0
init|;
name|cnt
operator|<
literal|5
condition|;
name|cnt
operator|++
operator|,
name|buffer
operator|++
control|)
block|{
name|int
name|ch
init|=
operator|*
operator|(
operator|(
name|unsigned
name|char
operator|*
operator|)
name|buffer
operator|)
decl_stmt|;
name|int
name|de
init|=
name|de85
index|[
name|ch
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|de
condition|)
return|return
name|error
argument_list|(
literal|"invalid base85 alphabet %c"
argument_list|,
name|ch
argument_list|)
return|;
name|de
operator|--
expr_stmt|;
if|if
condition|(
name|cnt
operator|==
literal|4
condition|)
block|{
comment|/* 				 * Detect overflow.  The largest 				 * 5-letter possible is "|NsC0" to 				 * encode 0xffffffff, and "|NsC" gives 				 * 0x03030303 at this point (i.e. 				 * 0xffffffff = 0x03030303 * 85). 				 */
if|if
condition|(
literal|0x03030303
operator|<
name|acc
operator|||
operator|(
literal|0x03030303
operator|==
name|acc
operator|&&
name|de
operator|)
condition|)
name|error
argument_list|(
literal|"invalid base85 sequence %.5s"
argument_list|,
name|buffer
operator|-
literal|3
argument_list|)
expr_stmt|;
block|}
name|acc
operator|=
name|acc
operator|*
literal|85
operator|+
name|de
expr_stmt|;
name|say1
argument_list|(
literal|"<%08x>"
argument_list|,
name|acc
argument_list|)
expr_stmt|;
block|}
name|say1
argument_list|(
literal|" %08x"
argument_list|,
name|acc
argument_list|)
expr_stmt|;
for|for
control|(
name|cnt
operator|=
literal|0
init|;
name|cnt
operator|<
literal|4
operator|&&
name|len
condition|;
name|cnt
operator|++
operator|,
name|len
operator|--
control|)
block|{
operator|*
name|dst
operator|++
operator|=
operator|(
name|acc
operator|>>
literal|24
operator|)
operator|&
literal|0xff
expr_stmt|;
name|acc
operator|=
name|acc
operator|<<
literal|8
expr_stmt|;
block|}
block|}
name|say
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|encode_85
name|void
name|encode_85
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|char
modifier|*
name|data
parameter_list|,
name|int
name|bytes
parameter_list|)
block|{
name|prep_base85
argument_list|()
expr_stmt|;
name|say
argument_list|(
literal|"encode 85"
argument_list|)
expr_stmt|;
while|while
condition|(
name|bytes
condition|)
block|{
name|unsigned
name|acc
init|=
literal|0
decl_stmt|;
name|int
name|cnt
decl_stmt|;
for|for
control|(
name|cnt
operator|=
literal|0
init|;
name|cnt
operator|<
literal|4
operator|&&
name|bytes
condition|;
name|cnt
operator|++
operator|,
name|bytes
operator|--
control|)
block|{
name|int
name|ch
init|=
operator|*
name|data
operator|++
decl_stmt|;
name|acc
operator||=
name|ch
operator|<<
operator|(
operator|(
literal|3
operator|-
name|cnt
operator|)
operator|*
literal|8
operator|)
expr_stmt|;
block|}
name|say1
argument_list|(
literal|" %08x"
argument_list|,
name|acc
argument_list|)
expr_stmt|;
for|for
control|(
name|cnt
operator|=
literal|0
init|;
name|cnt
operator|<
literal|5
condition|;
name|cnt
operator|++
control|)
block|{
name|int
name|val
init|=
name|acc
operator|%
literal|85
decl_stmt|;
name|acc
operator|/=
literal|85
expr_stmt|;
name|buf
index|[
literal|4
operator|-
name|cnt
index|]
operator|=
name|en85
index|[
name|val
index|]
expr_stmt|;
block|}
name|buf
operator|+=
literal|5
expr_stmt|;
block|}
name|say
argument_list|(
literal|"\n"
argument_list|)
expr_stmt|;
operator|*
name|buf
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_ifdef
ifdef|#
directive|ifdef
name|DEBUG_85
end_ifdef

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|ac
parameter_list|,
name|char
modifier|*
modifier|*
name|av
parameter_list|)
block|{
name|char
name|buf
index|[
literal|1024
index|]
decl_stmt|;
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|av
index|[
literal|1
index|]
argument_list|,
literal|"-e"
argument_list|)
condition|)
block|{
name|int
name|len
init|=
name|strlen
argument_list|(
name|av
index|[
literal|2
index|]
argument_list|)
decl_stmt|;
name|encode_85
argument_list|(
name|buf
argument_list|,
name|av
index|[
literal|2
index|]
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|<=
literal|26
condition|)
name|len
operator|=
name|len
operator|+
literal|'A'
operator|-
literal|1
expr_stmt|;
else|else
name|len
operator|=
name|len
operator|+
literal|'a'
operator|-
literal|26
operator|+
literal|1
expr_stmt|;
name|printf
argument_list|(
literal|"encoded: %c%s\n"
argument_list|,
name|len
argument_list|,
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|av
index|[
literal|1
index|]
argument_list|,
literal|"-d"
argument_list|)
condition|)
block|{
name|int
name|len
init|=
operator|*
name|av
index|[
literal|2
index|]
decl_stmt|;
if|if
condition|(
literal|'A'
operator|<=
name|len
operator|&&
name|len
operator|<=
literal|'Z'
condition|)
name|len
operator|=
name|len
operator|-
literal|'A'
operator|+
literal|1
expr_stmt|;
else|else
name|len
operator|=
name|len
operator|-
literal|'a'
operator|+
literal|26
operator|+
literal|1
expr_stmt|;
name|decode_85
argument_list|(
name|buf
argument_list|,
name|av
index|[
literal|2
index|]
operator|+
literal|1
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"decoded: %.*s\n"
argument_list|,
name|len
argument_list|,
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|av
index|[
literal|1
index|]
argument_list|,
literal|"-t"
argument_list|)
condition|)
block|{
name|char
name|t
index|[
literal|4
index|]
init|=
block|{
operator|-
literal|1
block|,
operator|-
literal|1
block|,
operator|-
literal|1
block|,
operator|-
literal|1
block|}
decl_stmt|;
name|encode_85
argument_list|(
name|buf
argument_list|,
name|t
argument_list|,
literal|4
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"encoded: D%s\n"
argument_list|,
name|buf
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

end_unit

