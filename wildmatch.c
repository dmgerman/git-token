begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/* **  Do shell-style pattern matching for ?, \, [], and * characters. **  It is 8bit clean. ** **  Written by Rich $alz, mirror!rs, Wed Nov 26 19:03:17 EST 1986. **  Rich $alz is now<rsalz@bbn.com>. ** **  Modified by Wayne Davison to special-case '/' matching, to make '**' **  work differently than '*', and to fix the character-class code. */
end_comment

begin_include
include|#
directive|include
file|"rsync.h"
end_include

begin_comment
comment|/* What character marks an inverted character class? */
end_comment

begin_define
DECL|macro|NEGATE_CLASS
define|#
directive|define
name|NEGATE_CLASS
value|'!'
end_define

begin_define
DECL|macro|NEGATE_CLASS2
define|#
directive|define
name|NEGATE_CLASS2
value|'^'
end_define

begin_define
DECL|macro|FALSE
define|#
directive|define
name|FALSE
value|0
end_define

begin_define
DECL|macro|TRUE
define|#
directive|define
name|TRUE
value|1
end_define

begin_define
DECL|macro|ABORT_ALL
define|#
directive|define
name|ABORT_ALL
value|-1
end_define

begin_define
DECL|macro|ABORT_TO_STARSTAR
define|#
directive|define
name|ABORT_TO_STARSTAR
value|-2
end_define

begin_define
DECL|macro|CC_EQ
define|#
directive|define
name|CC_EQ
parameter_list|(
name|class
parameter_list|,
name|len
parameter_list|,
name|litmatch
parameter_list|)
value|((len) == sizeof (litmatch)-1 \&& *(class) == *(litmatch) \&& strncmp((char*)class, litmatch, len) == 0)
end_define

begin_if
if|#
directive|if
name|defined
name|STDC_HEADERS
operator|||
operator|!
name|defined
name|isascii
end_if

begin_define
DECL|macro|ISASCII
define|#
directive|define
name|ISASCII
parameter_list|(
name|c
parameter_list|)
value|1
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|ISASCII
define|#
directive|define
name|ISASCII
parameter_list|(
name|c
parameter_list|)
value|isascii(c)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|isblank
end_ifdef

begin_define
DECL|macro|ISBLANK
define|#
directive|define
name|ISBLANK
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& isblank(c))
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|ISBLANK
define|#
directive|define
name|ISBLANK
parameter_list|(
name|c
parameter_list|)
value|((c) == ' ' || (c) == '\t')
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|isgraph
end_ifdef

begin_define
DECL|macro|ISGRAPH
define|#
directive|define
name|ISGRAPH
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& isgraph(c))
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|ISGRAPH
define|#
directive|define
name|ISGRAPH
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& isprint(c)&& !isspace(c))
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|ISPRINT
define|#
directive|define
name|ISPRINT
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& isprint(c))
end_define

begin_define
DECL|macro|ISDIGIT
define|#
directive|define
name|ISDIGIT
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& isdigit(c))
end_define

begin_define
DECL|macro|ISALNUM
define|#
directive|define
name|ISALNUM
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& isalnum(c))
end_define

begin_define
DECL|macro|ISALPHA
define|#
directive|define
name|ISALPHA
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& isalpha(c))
end_define

begin_define
DECL|macro|ISCNTRL
define|#
directive|define
name|ISCNTRL
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& iscntrl(c))
end_define

begin_define
DECL|macro|ISLOWER
define|#
directive|define
name|ISLOWER
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& islower(c))
end_define

begin_define
DECL|macro|ISPUNCT
define|#
directive|define
name|ISPUNCT
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& ispunct(c))
end_define

begin_define
DECL|macro|ISSPACE
define|#
directive|define
name|ISSPACE
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& isspace(c))
end_define

begin_define
DECL|macro|ISUPPER
define|#
directive|define
name|ISUPPER
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& isupper(c))
end_define

begin_define
DECL|macro|ISXDIGIT
define|#
directive|define
name|ISXDIGIT
parameter_list|(
name|c
parameter_list|)
value|(ISASCII(c)&& isxdigit(c))
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|WILD_TEST_ITERATIONS
end_ifdef

begin_decl_stmt
DECL|variable|wildmatch_iteration_count
name|int
name|wildmatch_iteration_count
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|force_lower_case
specifier|static
name|int
name|force_lower_case
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* Match pattern "p" against the a virtually-joined string consisting  * of "text" and any strings in array "a". */
end_comment

begin_function
DECL|function|dowild
specifier|static
name|int
name|dowild
parameter_list|(
specifier|const
name|uchar
modifier|*
name|p
parameter_list|,
specifier|const
name|uchar
modifier|*
name|text
parameter_list|,
specifier|const
name|uchar
modifier|*
specifier|const
modifier|*
name|a
parameter_list|)
block|{
name|uchar
name|p_ch
decl_stmt|;
ifdef|#
directive|ifdef
name|WILD_TEST_ITERATIONS
name|wildmatch_iteration_count
operator|++
expr_stmt|;
endif|#
directive|endif
for|for
control|(
init|;
operator|(
name|p_ch
operator|=
operator|*
name|p
operator|)
operator|!=
literal|'\0'
condition|;
name|text
operator|++
operator|,
name|p
operator|++
control|)
block|{
name|int
name|matched
decl_stmt|,
name|special
decl_stmt|;
name|uchar
name|t_ch
decl_stmt|,
name|prev_ch
decl_stmt|;
while|while
condition|(
operator|(
name|t_ch
operator|=
operator|*
name|text
operator|)
operator|==
literal|'\0'
condition|)
block|{
if|if
condition|(
operator|*
name|a
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|p_ch
operator|!=
literal|'*'
condition|)
return|return
name|ABORT_ALL
return|;
break|break;
block|}
name|text
operator|=
operator|*
name|a
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|force_lower_case
operator|&&
name|ISUPPER
argument_list|(
name|t_ch
argument_list|)
condition|)
name|t_ch
operator|=
name|tolower
argument_list|(
name|t_ch
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|p_ch
condition|)
block|{
case|case
literal|'\\'
case|:
comment|/* Literal match with following character.  Note that the test 	     * in "default" handles the p[1] == '\0' failure case. */
name|p_ch
operator|=
operator|*
operator|++
name|p
expr_stmt|;
comment|/* FALLTHROUGH */
default|default:
if|if
condition|(
name|t_ch
operator|!=
name|p_ch
condition|)
return|return
name|FALSE
return|;
continue|continue;
case|case
literal|'?'
case|:
comment|/* Match anything but '/'. */
if|if
condition|(
name|t_ch
operator|==
literal|'/'
condition|)
return|return
name|FALSE
return|;
continue|continue;
case|case
literal|'*'
case|:
if|if
condition|(
operator|*
operator|++
name|p
operator|==
literal|'*'
condition|)
block|{
while|while
condition|(
operator|*
operator|++
name|p
operator|==
literal|'*'
condition|)
block|{}
name|special
operator|=
name|TRUE
expr_stmt|;
block|}
else|else
name|special
operator|=
name|FALSE
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|==
literal|'\0'
condition|)
block|{
comment|/* Trailing "**" matches everything.  Trailing "*" matches 		 * only if there are no more slash characters. */
if|if
condition|(
operator|!
name|special
condition|)
block|{
do|do
block|{
if|if
condition|(
name|strchr
argument_list|(
operator|(
name|char
operator|*
operator|)
name|text
argument_list|,
literal|'/'
argument_list|)
operator|!=
name|NULL
condition|)
return|return
name|FALSE
return|;
block|}
do|while
condition|(
operator|(
name|text
operator|=
operator|*
name|a
operator|++
operator|)
operator|!=
name|NULL
condition|)
do|;
block|}
return|return
name|TRUE
return|;
block|}
while|while
condition|(
literal|1
condition|)
block|{
if|if
condition|(
name|t_ch
operator|==
literal|'\0'
condition|)
block|{
if|if
condition|(
operator|(
name|text
operator|=
operator|*
name|a
operator|++
operator|)
operator|==
name|NULL
condition|)
break|break;
name|t_ch
operator|=
operator|*
name|text
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|(
name|matched
operator|=
name|dowild
argument_list|(
name|p
argument_list|,
name|text
argument_list|,
name|a
argument_list|)
operator|)
operator|!=
name|FALSE
condition|)
block|{
if|if
condition|(
operator|!
name|special
operator|||
name|matched
operator|!=
name|ABORT_TO_STARSTAR
condition|)
return|return
name|matched
return|;
block|}
elseif|else
if|if
condition|(
operator|!
name|special
operator|&&
name|t_ch
operator|==
literal|'/'
condition|)
return|return
name|ABORT_TO_STARSTAR
return|;
name|t_ch
operator|=
operator|*
operator|++
name|text
expr_stmt|;
block|}
return|return
name|ABORT_ALL
return|;
case|case
literal|'['
case|:
name|p_ch
operator|=
operator|*
operator|++
name|p
expr_stmt|;
ifdef|#
directive|ifdef
name|NEGATE_CLASS2
if|if
condition|(
name|p_ch
operator|==
name|NEGATE_CLASS2
condition|)
name|p_ch
operator|=
name|NEGATE_CLASS
expr_stmt|;
endif|#
directive|endif
comment|/* Assign literal TRUE/FALSE because of "matched" comparison. */
name|special
operator|=
name|p_ch
operator|==
name|NEGATE_CLASS
condition|?
name|TRUE
else|:
name|FALSE
expr_stmt|;
if|if
condition|(
name|special
condition|)
block|{
comment|/* Inverted character class. */
name|p_ch
operator|=
operator|*
operator|++
name|p
expr_stmt|;
block|}
name|prev_ch
operator|=
literal|0
expr_stmt|;
name|matched
operator|=
name|FALSE
expr_stmt|;
do|do
block|{
if|if
condition|(
operator|!
name|p_ch
condition|)
return|return
name|ABORT_ALL
return|;
if|if
condition|(
name|p_ch
operator|==
literal|'\\'
condition|)
block|{
name|p_ch
operator|=
operator|*
operator|++
name|p
expr_stmt|;
if|if
condition|(
operator|!
name|p_ch
condition|)
return|return
name|ABORT_ALL
return|;
if|if
condition|(
name|t_ch
operator|==
name|p_ch
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|p_ch
operator|==
literal|'-'
operator|&&
name|prev_ch
operator|&&
name|p
index|[
literal|1
index|]
operator|&&
name|p
index|[
literal|1
index|]
operator|!=
literal|']'
condition|)
block|{
name|p_ch
operator|=
operator|*
operator|++
name|p
expr_stmt|;
if|if
condition|(
name|p_ch
operator|==
literal|'\\'
condition|)
block|{
name|p_ch
operator|=
operator|*
operator|++
name|p
expr_stmt|;
if|if
condition|(
operator|!
name|p_ch
condition|)
return|return
name|ABORT_ALL
return|;
block|}
if|if
condition|(
name|t_ch
operator|<=
name|p_ch
operator|&&
name|t_ch
operator|>=
name|prev_ch
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
name|p_ch
operator|=
literal|0
expr_stmt|;
comment|/* This makes "prev_ch" get set to 0. */
block|}
elseif|else
if|if
condition|(
name|p_ch
operator|==
literal|'['
operator|&&
name|p
index|[
literal|1
index|]
operator|==
literal|':'
condition|)
block|{
specifier|const
name|uchar
modifier|*
name|s
decl_stmt|;
name|int
name|i
decl_stmt|;
for|for
control|(
name|s
operator|=
name|p
operator|+=
literal|2
init|;
operator|(
name|p_ch
operator|=
operator|*
name|p
operator|)
operator|&&
name|p_ch
operator|!=
literal|']'
condition|;
name|p
operator|++
control|)
block|{}
comment|/*SHARED ITERATOR*/
if|if
condition|(
operator|!
name|p_ch
condition|)
return|return
name|ABORT_ALL
return|;
name|i
operator|=
name|p
operator|-
name|s
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|i
operator|<
literal|0
operator|||
name|p
index|[
operator|-
literal|1
index|]
operator|!=
literal|':'
condition|)
block|{
comment|/* Didn't find ":]", so treat like a normal set. */
name|p
operator|=
name|s
operator|-
literal|2
expr_stmt|;
name|p_ch
operator|=
literal|'['
expr_stmt|;
if|if
condition|(
name|t_ch
operator|==
name|p_ch
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"alnum"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISALNUM
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"alpha"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISALPHA
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"blank"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISBLANK
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"cntrl"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISCNTRL
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"digit"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISDIGIT
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"graph"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISGRAPH
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"lower"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISLOWER
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"print"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISPRINT
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"punct"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISPUNCT
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"space"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISSPACE
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"upper"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISUPPER
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|CC_EQ
argument_list|(
name|s
argument_list|,
name|i
argument_list|,
literal|"xdigit"
argument_list|)
condition|)
block|{
if|if
condition|(
name|ISXDIGIT
argument_list|(
name|t_ch
argument_list|)
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
else|else
comment|/* malformed [:class:] string */
return|return
name|ABORT_ALL
return|;
name|p_ch
operator|=
literal|0
expr_stmt|;
comment|/* This makes "prev_ch" get set to 0. */
block|}
elseif|else
if|if
condition|(
name|t_ch
operator|==
name|p_ch
condition|)
name|matched
operator|=
name|TRUE
expr_stmt|;
block|}
do|while
condition|(
name|prev_ch
operator|=
name|p_ch
operator|,
operator|(
name|p_ch
operator|=
operator|*
operator|++
name|p
operator|)
operator|!=
literal|']'
condition|)
do|;
if|if
condition|(
name|matched
operator|==
name|special
operator|||
name|t_ch
operator|==
literal|'/'
condition|)
return|return
name|FALSE
return|;
continue|continue;
block|}
block|}
do|do
block|{
if|if
condition|(
operator|*
name|text
condition|)
return|return
name|FALSE
return|;
block|}
do|while
condition|(
operator|(
name|text
operator|=
operator|*
name|a
operator|++
operator|)
operator|!=
name|NULL
condition|)
do|;
return|return
name|TRUE
return|;
block|}
end_function

begin_comment
comment|/* Match literal string "s" against the a virtually-joined string consisting  * of "text" and any strings in array "a". */
end_comment

begin_function
DECL|function|doliteral
specifier|static
name|int
name|doliteral
parameter_list|(
specifier|const
name|uchar
modifier|*
name|s
parameter_list|,
specifier|const
name|uchar
modifier|*
name|text
parameter_list|,
specifier|const
name|uchar
modifier|*
specifier|const
modifier|*
name|a
parameter_list|)
block|{
for|for
control|(
init|;
operator|*
name|s
operator|!=
literal|'\0'
condition|;
name|text
operator|++
operator|,
name|s
operator|++
control|)
block|{
while|while
condition|(
operator|*
name|text
operator|==
literal|'\0'
condition|)
block|{
if|if
condition|(
operator|(
name|text
operator|=
operator|*
name|a
operator|++
operator|)
operator|==
name|NULL
condition|)
return|return
name|FALSE
return|;
block|}
if|if
condition|(
operator|*
name|text
operator|!=
operator|*
name|s
condition|)
return|return
name|FALSE
return|;
block|}
do|do
block|{
if|if
condition|(
operator|*
name|text
condition|)
return|return
name|FALSE
return|;
block|}
do|while
condition|(
operator|(
name|text
operator|=
operator|*
name|a
operator|++
operator|)
operator|!=
name|NULL
condition|)
do|;
return|return
name|TRUE
return|;
block|}
end_function

begin_comment
comment|/* Return the last "count" path elements from the concatenated string.  * We return a string pointer to the start of the string, and update the  * array pointer-pointer to point to any remaining string elements. */
end_comment

begin_function
DECL|function|trailing_N_elements
specifier|static
specifier|const
name|uchar
modifier|*
name|trailing_N_elements
parameter_list|(
specifier|const
name|uchar
modifier|*
specifier|const
modifier|*
modifier|*
name|a_ptr
parameter_list|,
name|int
name|count
parameter_list|)
block|{
specifier|const
name|uchar
modifier|*
specifier|const
modifier|*
name|a
init|=
operator|*
name|a_ptr
decl_stmt|;
specifier|const
name|uchar
modifier|*
specifier|const
modifier|*
name|first_a
init|=
name|a
decl_stmt|;
while|while
condition|(
operator|*
name|a
condition|)
name|a
operator|++
expr_stmt|;
while|while
condition|(
name|a
operator|!=
name|first_a
condition|)
block|{
specifier|const
name|uchar
modifier|*
name|s
init|=
operator|*
operator|--
name|a
decl_stmt|;
name|s
operator|+=
name|strlen
argument_list|(
operator|(
name|char
operator|*
operator|)
name|s
argument_list|)
expr_stmt|;
while|while
condition|(
operator|--
name|s
operator|>=
operator|*
name|a
condition|)
block|{
if|if
condition|(
operator|*
name|s
operator|==
literal|'/'
operator|&&
operator|!
operator|--
name|count
condition|)
block|{
operator|*
name|a_ptr
operator|=
name|a
operator|+
literal|1
expr_stmt|;
return|return
name|s
operator|+
literal|1
return|;
block|}
block|}
block|}
if|if
condition|(
name|count
operator|==
literal|1
condition|)
block|{
operator|*
name|a_ptr
operator|=
name|a
operator|+
literal|1
expr_stmt|;
return|return
operator|*
name|a
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_comment
comment|/* Match the "pattern" against the "text" string. */
end_comment

begin_function
DECL|function|wildmatch
name|int
name|wildmatch
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|text
parameter_list|)
block|{
specifier|static
specifier|const
name|uchar
modifier|*
name|nomore
index|[
literal|1
index|]
decl_stmt|;
comment|/* A NULL pointer. */
ifdef|#
directive|ifdef
name|WILD_TEST_ITERATIONS
name|wildmatch_iteration_count
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
return|return
name|dowild
argument_list|(
operator|(
specifier|const
name|uchar
operator|*
operator|)
name|pattern
argument_list|,
operator|(
specifier|const
name|uchar
operator|*
operator|)
name|text
argument_list|,
name|nomore
argument_list|)
operator|==
name|TRUE
return|;
block|}
end_function

begin_comment
comment|/* Match the "pattern" against the forced-to-lower-case "text" string. */
end_comment

begin_function
DECL|function|iwildmatch
name|int
name|iwildmatch
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|text
parameter_list|)
block|{
specifier|static
specifier|const
name|uchar
modifier|*
name|nomore
index|[
literal|1
index|]
decl_stmt|;
comment|/* A NULL pointer. */
name|int
name|ret
decl_stmt|;
ifdef|#
directive|ifdef
name|WILD_TEST_ITERATIONS
name|wildmatch_iteration_count
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
name|force_lower_case
operator|=
literal|1
expr_stmt|;
name|ret
operator|=
name|dowild
argument_list|(
operator|(
specifier|const
name|uchar
operator|*
operator|)
name|pattern
argument_list|,
operator|(
specifier|const
name|uchar
operator|*
operator|)
name|text
argument_list|,
name|nomore
argument_list|)
operator|==
name|TRUE
expr_stmt|;
name|force_lower_case
operator|=
literal|0
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_comment
comment|/* Match pattern "p" against the a virtually-joined string consisting  * of all the pointers in array "texts" (which has a NULL pointer at the  * end).  The int "where" can be 0 (normal matching),> 0 (match only  * the trailing N slash-separated filename components of "texts"), or< 0  * (match the "pattern" at the start or after any slash in "texts"). */
end_comment

begin_function
DECL|function|wildmatch_array
name|int
name|wildmatch_array
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|texts
parameter_list|,
name|int
name|where
parameter_list|)
block|{
specifier|const
name|uchar
modifier|*
name|p
init|=
operator|(
specifier|const
name|uchar
operator|*
operator|)
name|pattern
decl_stmt|;
specifier|const
name|uchar
modifier|*
specifier|const
modifier|*
name|a
init|=
operator|(
specifier|const
name|uchar
operator|*
specifier|const
operator|*
operator|)
name|texts
decl_stmt|;
specifier|const
name|uchar
modifier|*
name|text
decl_stmt|;
name|int
name|matched
decl_stmt|;
ifdef|#
directive|ifdef
name|WILD_TEST_ITERATIONS
name|wildmatch_iteration_count
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|where
operator|>
literal|0
condition|)
name|text
operator|=
name|trailing_N_elements
argument_list|(
operator|&
name|a
argument_list|,
name|where
argument_list|)
expr_stmt|;
else|else
name|text
operator|=
operator|*
name|a
operator|++
expr_stmt|;
if|if
condition|(
operator|!
name|text
condition|)
return|return
name|FALSE
return|;
if|if
condition|(
operator|(
name|matched
operator|=
name|dowild
argument_list|(
name|p
argument_list|,
name|text
argument_list|,
name|a
argument_list|)
operator|)
operator|!=
name|TRUE
operator|&&
name|where
operator|<
literal|0
operator|&&
name|matched
operator|!=
name|ABORT_ALL
condition|)
block|{
while|while
condition|(
literal|1
condition|)
block|{
if|if
condition|(
operator|*
name|text
operator|==
literal|'\0'
condition|)
block|{
if|if
condition|(
operator|(
name|text
operator|=
operator|(
name|uchar
operator|*
operator|)
operator|*
name|a
operator|++
operator|)
operator|==
name|NULL
condition|)
return|return
name|FALSE
return|;
continue|continue;
block|}
if|if
condition|(
operator|*
name|text
operator|++
operator|==
literal|'/'
operator|&&
operator|(
name|matched
operator|=
name|dowild
argument_list|(
name|p
argument_list|,
name|text
argument_list|,
name|a
argument_list|)
operator|)
operator|!=
name|FALSE
operator|&&
name|matched
operator|!=
name|ABORT_TO_STARSTAR
condition|)
break|break;
block|}
block|}
return|return
name|matched
operator|==
name|TRUE
return|;
block|}
end_function

begin_comment
comment|/* Match literal string "s" against the a virtually-joined string consisting  * of all the pointers in array "texts" (which has a NULL pointer at the  * end).  The int "where" can be 0 (normal matching), or> 0 (match  * only the trailing N slash-separated filename components of "texts"). */
end_comment

begin_function
DECL|function|litmatch_array
name|int
name|litmatch_array
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|,
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|texts
parameter_list|,
name|int
name|where
parameter_list|)
block|{
specifier|const
name|uchar
modifier|*
name|s
init|=
operator|(
specifier|const
name|uchar
operator|*
operator|)
name|string
decl_stmt|;
specifier|const
name|uchar
modifier|*
specifier|const
modifier|*
name|a
init|=
operator|(
specifier|const
name|uchar
operator|*
specifier|const
operator|*
operator|)
name|texts
decl_stmt|;
specifier|const
name|uchar
modifier|*
name|text
decl_stmt|;
if|if
condition|(
name|where
operator|>
literal|0
condition|)
name|text
operator|=
name|trailing_N_elements
argument_list|(
operator|&
name|a
argument_list|,
name|where
argument_list|)
expr_stmt|;
else|else
name|text
operator|=
operator|*
name|a
operator|++
expr_stmt|;
if|if
condition|(
operator|!
name|text
condition|)
return|return
name|FALSE
return|;
return|return
name|doliteral
argument_list|(
name|s
argument_list|,
name|text
argument_list|,
name|a
argument_list|)
operator|==
name|TRUE
return|;
block|}
end_function

end_unit

