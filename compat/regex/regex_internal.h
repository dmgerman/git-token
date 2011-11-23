begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/* Extended regular expression matching and search library.    Copyright (C) 2002-2005, 2007, 2008, 2010 Free Software Foundation, Inc.    This file is part of the GNU C Library.    Contributed by Isamu Hasegawa<isamu@yamato.ibm.com>.     The GNU C Library is free software; you can redistribute it and/or    modify it under the terms of the GNU Lesser General Public    License as published by the Free Software Foundation; either    version 2.1 of the License, or (at your option) any later version.     The GNU C Library is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    Lesser General Public License for more details.     You should have received a copy of the GNU Lesser General Public    License along with the GNU C Library; if not, write to the Free    Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA    02111-1307 USA.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_REGEX_INTERNAL_H
end_ifndef

begin_define
DECL|macro|_REGEX_INTERNAL_H
define|#
directive|define
name|_REGEX_INTERNAL_H
value|1
end_define

begin_include
include|#
directive|include
file|<assert.h>
end_include

begin_include
include|#
directive|include
file|<ctype.h>
end_include

begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_if
if|#
directive|if
name|defined
name|HAVE_LANGINFO_H
operator|||
name|defined
name|HAVE_LANGINFO_CODESET
operator|||
name|defined
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<langinfo.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|defined
name|HAVE_LOCALE_H
operator|||
name|defined
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<locale.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|defined
name|HAVE_WCHAR_H
operator|||
name|defined
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<wchar.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* HAVE_WCHAR_H || _LIBC */
end_comment

begin_if
if|#
directive|if
name|defined
name|HAVE_WCTYPE_H
operator|||
name|defined
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<wctype.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* HAVE_WCTYPE_H || _LIBC */
end_comment

begin_if
if|#
directive|if
name|defined
name|HAVE_STDBOOL_H
operator|||
name|defined
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<stdbool.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* HAVE_STDBOOL_H || _LIBC */
end_comment

begin_if
if|#
directive|if
operator|!
name|defined
argument_list|(
name|ZOS_USS
argument_list|)
end_if

begin_if
if|#
directive|if
name|defined
name|HAVE_STDINT_H
operator|||
name|defined
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<stdint.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* HAVE_STDINT_H || _LIBC */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* !ZOS_USS */
end_comment

begin_if
if|#
directive|if
name|defined
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<bits/libc-lock.h>
end_include

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|__libc_lock_define
define|#
directive|define
name|__libc_lock_define
parameter_list|(
name|CLASS
parameter_list|,
name|NAME
parameter_list|)
end_define

begin_define
DECL|macro|__libc_lock_init
define|#
directive|define
name|__libc_lock_init
parameter_list|(
name|NAME
parameter_list|)
value|do { } while (0)
end_define

begin_define
DECL|macro|__libc_lock_lock
define|#
directive|define
name|__libc_lock_lock
parameter_list|(
name|NAME
parameter_list|)
value|do { } while (0)
end_define

begin_define
DECL|macro|__libc_lock_unlock
define|#
directive|define
name|__libc_lock_unlock
parameter_list|(
name|NAME
parameter_list|)
value|do { } while (0)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|GAWK
end_ifndef

begin_comment
comment|/* In case that the system doesn't have isblank().  */
end_comment

begin_if
if|#
directive|if
operator|!
name|defined
name|_LIBC
operator|&&
operator|!
name|defined
name|HAVE_ISBLANK
operator|&&
operator|!
name|defined
name|isblank
end_if

begin_define
DECL|macro|isblank
define|#
directive|define
name|isblank
parameter_list|(
name|ch
parameter_list|)
value|((ch) == ' ' || (ch) == '\t')
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_else
else|#
directive|else
end_else

begin_comment
comment|/* GAWK */
end_comment

begin_comment
comment|/*  * This is a freaking mess. On glibc systems you have to define  * a magic constant to get isblank() out of<ctype.h>, since it's  * a C99 function.  To heck with all that and borrow a page from  * dfa.c's book.  */
end_comment

begin_function
specifier|static
name|int
DECL|function|is_blank
name|is_blank
parameter_list|(
name|int
name|c
parameter_list|)
block|{
return|return
operator|(
name|c
operator|==
literal|' '
operator|||
name|c
operator|==
literal|'\t'
operator|)
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* GAWK */
end_comment

begin_ifdef
ifdef|#
directive|ifdef
name|_LIBC
end_ifdef

begin_ifndef
ifndef|#
directive|ifndef
name|_RE_DEFINE_LOCALE_FUNCTIONS
end_ifndef

begin_define
DECL|macro|_RE_DEFINE_LOCALE_FUNCTIONS
define|#
directive|define
name|_RE_DEFINE_LOCALE_FUNCTIONS
value|1
end_define

begin_include
include|#
directive|include
file|<locale/localeinfo.h>
end_include

begin_include
include|#
directive|include
file|<locale/elem-hash.h>
end_include

begin_include
include|#
directive|include
file|<locale/coll-lookup.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* This is for other GNU distributions with internationalized messages.  */
end_comment

begin_if
if|#
directive|if
operator|(
name|HAVE_LIBINTL_H
operator|&&
name|ENABLE_NLS
operator|)
operator|||
name|defined
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<libintl.h>
end_include

begin_ifdef
ifdef|#
directive|ifdef
name|_LIBC
end_ifdef

begin_undef
DECL|macro|gettext
undef|#
directive|undef
name|gettext
end_undef

begin_define
DECL|macro|gettext
define|#
directive|define
name|gettext
parameter_list|(
name|msgid
parameter_list|)
define|\
value|INTUSE(__dcgettext) (_libc_intl_domainname, msgid, LC_MESSAGES)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|gettext
define|#
directive|define
name|gettext
parameter_list|(
name|msgid
parameter_list|)
value|(msgid)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|gettext_noop
end_ifndef

begin_comment
comment|/* This define is so xgettext can find the internationalizable    strings.  */
end_comment

begin_define
DECL|macro|gettext_noop
define|#
directive|define
name|gettext_noop
parameter_list|(
name|String
parameter_list|)
value|String
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* For loser systems without the definition.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|SIZE_MAX
end_ifndef

begin_define
DECL|macro|SIZE_MAX
define|#
directive|define
name|SIZE_MAX
value|((size_t) -1)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|NO_MBSUPPORT
end_ifndef

begin_include
include|#
directive|include
file|"mbsupport.h"
end_include

begin_comment
comment|/* gawk */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|MB_CUR_MAX
end_ifndef

begin_define
DECL|macro|MB_CUR_MAX
define|#
directive|define
name|MB_CUR_MAX
value|1
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
operator|(
name|defined
name|MBS_SUPPORT
operator|)
operator|||
name|_LIBC
end_if

begin_define
DECL|macro|RE_ENABLE_I18N
define|#
directive|define
name|RE_ENABLE_I18N
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|__GNUC__
operator|>=
literal|3
end_if

begin_define
DECL|macro|BE
define|#
directive|define
name|BE
parameter_list|(
name|expr
parameter_list|,
name|val
parameter_list|)
value|__builtin_expect (expr, val)
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|BE
define|#
directive|define
name|BE
parameter_list|(
name|expr
parameter_list|,
name|val
parameter_list|)
value|(expr)
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|inline
end_ifdef

begin_undef
DECL|macro|inline
undef|#
directive|undef
name|inline
end_undef

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|inline
define|#
directive|define
name|inline
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* Number of single byte character.  */
end_comment

begin_define
DECL|macro|SBC_MAX
define|#
directive|define
name|SBC_MAX
value|256
end_define

begin_define
DECL|macro|COLL_ELEM_LEN_MAX
define|#
directive|define
name|COLL_ELEM_LEN_MAX
value|8
end_define

begin_comment
comment|/* The character which represents newline.  */
end_comment

begin_define
DECL|macro|NEWLINE_CHAR
define|#
directive|define
name|NEWLINE_CHAR
value|'\n'
end_define

begin_define
DECL|macro|WIDE_NEWLINE_CHAR
define|#
directive|define
name|WIDE_NEWLINE_CHAR
value|L'\n'
end_define

begin_comment
comment|/* Rename to standard API for using out of glibc.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_LIBC
end_ifndef

begin_ifdef
ifdef|#
directive|ifdef
name|__wctype
end_ifdef

begin_undef
DECL|macro|__wctype
undef|#
directive|undef
name|__wctype
end_undef

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|__wctype
define|#
directive|define
name|__wctype
value|wctype
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|__iswctype
end_ifdef

begin_undef
DECL|macro|__iswctype
undef|#
directive|undef
name|__iswctype
end_undef

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|__iswctype
define|#
directive|define
name|__iswctype
value|iswctype
end_define

begin_define
DECL|macro|__btowc
define|#
directive|define
name|__btowc
value|btowc
end_define

begin_define
DECL|macro|__mbrtowc
define|#
directive|define
name|__mbrtowc
value|mbrtowc
end_define

begin_undef
DECL|macro|__mempcpy
undef|#
directive|undef
name|__mempcpy
end_undef

begin_comment
DECL|macro|__mempcpy
comment|/* GAWK */
end_comment

begin_define
DECL|macro|__mempcpy
define|#
directive|define
name|__mempcpy
value|mempcpy
end_define

begin_define
DECL|macro|__wcrtomb
define|#
directive|define
name|__wcrtomb
value|wcrtomb
end_define

begin_define
DECL|macro|__regfree
define|#
directive|define
name|__regfree
value|regfree
end_define

begin_define
DECL|macro|attribute_hidden
define|#
directive|define
name|attribute_hidden
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* not _LIBC */
end_comment

begin_ifdef
ifdef|#
directive|ifdef
name|__GNUC__
end_ifdef

begin_define
DECL|macro|__attribute
define|#
directive|define
name|__attribute
parameter_list|(
name|arg
parameter_list|)
value|__attribute__ (arg)
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|__attribute
define|#
directive|define
name|__attribute
parameter_list|(
name|arg
parameter_list|)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
specifier|extern
specifier|const
name|char
name|__re_error_msgid
index|[]
name|attribute_hidden
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
specifier|const
name|size_t
name|__re_error_msgid_idx
index|[]
name|attribute_hidden
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* An integer used to represent a set of bits.  It must be unsigned,    and must be at least as wide as unsigned int.  */
end_comment

begin_typedef
DECL|typedef|bitset_word_t
typedef|typedef
name|unsigned
name|long
name|int
name|bitset_word_t
typedef|;
end_typedef

begin_comment
comment|/* All bits set in a bitset_word_t.  */
end_comment

begin_define
DECL|macro|BITSET_WORD_MAX
define|#
directive|define
name|BITSET_WORD_MAX
value|ULONG_MAX
end_define

begin_comment
comment|/* Number of bits in a bitset_word_t.  */
end_comment

begin_define
DECL|macro|BITSET_WORD_BITS
define|#
directive|define
name|BITSET_WORD_BITS
value|(sizeof (bitset_word_t) * CHAR_BIT)
end_define

begin_comment
comment|/* Number of bitset_word_t in a bit_set.  */
end_comment

begin_define
DECL|macro|BITSET_WORDS
define|#
directive|define
name|BITSET_WORDS
value|(SBC_MAX / BITSET_WORD_BITS)
end_define

begin_typedef
DECL|typedef|bitset_t
typedef|typedef
name|bitset_word_t
name|bitset_t
index|[
name|BITSET_WORDS
index|]
typedef|;
end_typedef

begin_typedef
DECL|typedef|re_bitset_ptr_t
typedef|typedef
name|bitset_word_t
modifier|*
name|re_bitset_ptr_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|re_const_bitset_ptr_t
typedef|typedef
specifier|const
name|bitset_word_t
modifier|*
name|re_const_bitset_ptr_t
typedef|;
end_typedef

begin_define
DECL|macro|bitset_set
define|#
directive|define
name|bitset_set
parameter_list|(
name|set
parameter_list|,
name|i
parameter_list|)
define|\
value|(set[i / BITSET_WORD_BITS] |= (bitset_word_t) 1<< i % BITSET_WORD_BITS)
end_define

begin_define
DECL|macro|bitset_clear
define|#
directive|define
name|bitset_clear
parameter_list|(
name|set
parameter_list|,
name|i
parameter_list|)
define|\
value|(set[i / BITSET_WORD_BITS]&= ~((bitset_word_t) 1<< i % BITSET_WORD_BITS))
end_define

begin_define
DECL|macro|bitset_contain
define|#
directive|define
name|bitset_contain
parameter_list|(
name|set
parameter_list|,
name|i
parameter_list|)
define|\
value|(set[i / BITSET_WORD_BITS]& ((bitset_word_t) 1<< i % BITSET_WORD_BITS))
end_define

begin_define
DECL|macro|bitset_empty
define|#
directive|define
name|bitset_empty
parameter_list|(
name|set
parameter_list|)
value|memset (set, '\0', sizeof (bitset_t))
end_define

begin_define
DECL|macro|bitset_set_all
define|#
directive|define
name|bitset_set_all
parameter_list|(
name|set
parameter_list|)
value|memset (set, '\xff', sizeof (bitset_t))
end_define

begin_define
DECL|macro|bitset_copy
define|#
directive|define
name|bitset_copy
parameter_list|(
name|dest
parameter_list|,
name|src
parameter_list|)
value|memcpy (dest, src, sizeof (bitset_t))
end_define

begin_define
DECL|macro|PREV_WORD_CONSTRAINT
define|#
directive|define
name|PREV_WORD_CONSTRAINT
value|0x0001
end_define

begin_define
DECL|macro|PREV_NOTWORD_CONSTRAINT
define|#
directive|define
name|PREV_NOTWORD_CONSTRAINT
value|0x0002
end_define

begin_define
DECL|macro|NEXT_WORD_CONSTRAINT
define|#
directive|define
name|NEXT_WORD_CONSTRAINT
value|0x0004
end_define

begin_define
DECL|macro|NEXT_NOTWORD_CONSTRAINT
define|#
directive|define
name|NEXT_NOTWORD_CONSTRAINT
value|0x0008
end_define

begin_define
DECL|macro|PREV_NEWLINE_CONSTRAINT
define|#
directive|define
name|PREV_NEWLINE_CONSTRAINT
value|0x0010
end_define

begin_define
DECL|macro|NEXT_NEWLINE_CONSTRAINT
define|#
directive|define
name|NEXT_NEWLINE_CONSTRAINT
value|0x0020
end_define

begin_define
DECL|macro|PREV_BEGBUF_CONSTRAINT
define|#
directive|define
name|PREV_BEGBUF_CONSTRAINT
value|0x0040
end_define

begin_define
DECL|macro|NEXT_ENDBUF_CONSTRAINT
define|#
directive|define
name|NEXT_ENDBUF_CONSTRAINT
value|0x0080
end_define

begin_define
DECL|macro|WORD_DELIM_CONSTRAINT
define|#
directive|define
name|WORD_DELIM_CONSTRAINT
value|0x0100
end_define

begin_define
DECL|macro|NOT_WORD_DELIM_CONSTRAINT
define|#
directive|define
name|NOT_WORD_DELIM_CONSTRAINT
value|0x0200
end_define

begin_typedef
typedef|typedef
enum|enum
block|{
DECL|enumerator|INSIDE_WORD
name|INSIDE_WORD
init|=
name|PREV_WORD_CONSTRAINT
operator||
name|NEXT_WORD_CONSTRAINT
block|,
DECL|enumerator|WORD_FIRST
name|WORD_FIRST
init|=
name|PREV_NOTWORD_CONSTRAINT
operator||
name|NEXT_WORD_CONSTRAINT
block|,
DECL|enumerator|WORD_LAST
name|WORD_LAST
init|=
name|PREV_WORD_CONSTRAINT
operator||
name|NEXT_NOTWORD_CONSTRAINT
block|,
DECL|enumerator|INSIDE_NOTWORD
name|INSIDE_NOTWORD
init|=
name|PREV_NOTWORD_CONSTRAINT
operator||
name|NEXT_NOTWORD_CONSTRAINT
block|,
DECL|enumerator|LINE_FIRST
name|LINE_FIRST
init|=
name|PREV_NEWLINE_CONSTRAINT
block|,
DECL|enumerator|LINE_LAST
name|LINE_LAST
init|=
name|NEXT_NEWLINE_CONSTRAINT
block|,
DECL|enumerator|BUF_FIRST
name|BUF_FIRST
init|=
name|PREV_BEGBUF_CONSTRAINT
block|,
DECL|enumerator|BUF_LAST
name|BUF_LAST
init|=
name|NEXT_ENDBUF_CONSTRAINT
block|,
DECL|enumerator|WORD_DELIM
name|WORD_DELIM
init|=
name|WORD_DELIM_CONSTRAINT
block|,
DECL|enumerator|NOT_WORD_DELIM
name|NOT_WORD_DELIM
init|=
name|NOT_WORD_DELIM_CONSTRAINT
DECL|typedef|re_context_type
block|}
name|re_context_type
typedef|;
end_typedef

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|nelem
name|int
name|nelem
decl_stmt|;
DECL|member|elems
name|int
modifier|*
name|elems
decl_stmt|;
DECL|typedef|re_node_set
block|}
name|re_node_set
typedef|;
end_typedef

begin_typedef
typedef|typedef
enum|enum
block|{
DECL|enumerator|NON_TYPE
name|NON_TYPE
init|=
literal|0
block|,
comment|/* Node type, These are used by token, node, tree.  */
DECL|enumerator|CHARACTER
name|CHARACTER
init|=
literal|1
block|,
DECL|enumerator|END_OF_RE
name|END_OF_RE
init|=
literal|2
block|,
DECL|enumerator|SIMPLE_BRACKET
name|SIMPLE_BRACKET
init|=
literal|3
block|,
DECL|enumerator|OP_BACK_REF
name|OP_BACK_REF
init|=
literal|4
block|,
DECL|enumerator|OP_PERIOD
name|OP_PERIOD
init|=
literal|5
block|,
ifdef|#
directive|ifdef
name|RE_ENABLE_I18N
DECL|enumerator|COMPLEX_BRACKET
name|COMPLEX_BRACKET
init|=
literal|6
block|,
DECL|enumerator|OP_UTF8_PERIOD
name|OP_UTF8_PERIOD
init|=
literal|7
block|,
endif|#
directive|endif
comment|/* RE_ENABLE_I18N */
comment|/* We define EPSILON_BIT as a macro so that OP_OPEN_SUBEXP is used      when the debugger shows values of this enum type.  */
DECL|macro|EPSILON_BIT
define|#
directive|define
name|EPSILON_BIT
value|8
DECL|enumerator|OP_OPEN_SUBEXP
name|OP_OPEN_SUBEXP
init|=
name|EPSILON_BIT
operator||
literal|0
block|,
DECL|enumerator|OP_CLOSE_SUBEXP
name|OP_CLOSE_SUBEXP
init|=
name|EPSILON_BIT
operator||
literal|1
block|,
DECL|enumerator|OP_ALT
name|OP_ALT
init|=
name|EPSILON_BIT
operator||
literal|2
block|,
DECL|enumerator|OP_DUP_ASTERISK
name|OP_DUP_ASTERISK
init|=
name|EPSILON_BIT
operator||
literal|3
block|,
DECL|enumerator|ANCHOR
name|ANCHOR
init|=
name|EPSILON_BIT
operator||
literal|4
block|,
comment|/* Tree type, these are used only by tree. */
DECL|enumerator|CONCAT
name|CONCAT
init|=
literal|16
block|,
DECL|enumerator|SUBEXP
name|SUBEXP
init|=
literal|17
block|,
comment|/* Token type, these are used only by token.  */
DECL|enumerator|OP_DUP_PLUS
name|OP_DUP_PLUS
init|=
literal|18
block|,
DECL|enumerator|OP_DUP_QUESTION
name|OP_DUP_QUESTION
block|,
DECL|enumerator|OP_OPEN_BRACKET
name|OP_OPEN_BRACKET
block|,
DECL|enumerator|OP_CLOSE_BRACKET
name|OP_CLOSE_BRACKET
block|,
DECL|enumerator|OP_CHARSET_RANGE
name|OP_CHARSET_RANGE
block|,
DECL|enumerator|OP_OPEN_DUP_NUM
name|OP_OPEN_DUP_NUM
block|,
DECL|enumerator|OP_CLOSE_DUP_NUM
name|OP_CLOSE_DUP_NUM
block|,
DECL|enumerator|OP_NON_MATCH_LIST
name|OP_NON_MATCH_LIST
block|,
DECL|enumerator|OP_OPEN_COLL_ELEM
name|OP_OPEN_COLL_ELEM
block|,
DECL|enumerator|OP_CLOSE_COLL_ELEM
name|OP_CLOSE_COLL_ELEM
block|,
DECL|enumerator|OP_OPEN_EQUIV_CLASS
name|OP_OPEN_EQUIV_CLASS
block|,
DECL|enumerator|OP_CLOSE_EQUIV_CLASS
name|OP_CLOSE_EQUIV_CLASS
block|,
DECL|enumerator|OP_OPEN_CHAR_CLASS
name|OP_OPEN_CHAR_CLASS
block|,
DECL|enumerator|OP_CLOSE_CHAR_CLASS
name|OP_CLOSE_CHAR_CLASS
block|,
DECL|enumerator|OP_WORD
name|OP_WORD
block|,
DECL|enumerator|OP_NOTWORD
name|OP_NOTWORD
block|,
DECL|enumerator|OP_SPACE
name|OP_SPACE
block|,
DECL|enumerator|OP_NOTSPACE
name|OP_NOTSPACE
block|,
DECL|enumerator|BACK_SLASH
name|BACK_SLASH
DECL|typedef|re_token_type_t
block|}
name|re_token_type_t
typedef|;
end_typedef

begin_ifdef
ifdef|#
directive|ifdef
name|RE_ENABLE_I18N
end_ifdef

begin_typedef
typedef|typedef
struct|struct
block|{
comment|/* Multibyte characters.  */
DECL|member|mbchars
name|wchar_t
modifier|*
name|mbchars
decl_stmt|;
comment|/* Collating symbols.  */
ifdef|#
directive|ifdef
name|_LIBC
DECL|member|coll_syms
name|int32_t
modifier|*
name|coll_syms
decl_stmt|;
endif|#
directive|endif
comment|/* Equivalence classes. */
ifdef|#
directive|ifdef
name|_LIBC
DECL|member|equiv_classes
name|int32_t
modifier|*
name|equiv_classes
decl_stmt|;
endif|#
directive|endif
comment|/* Range expressions. */
ifdef|#
directive|ifdef
name|_LIBC
DECL|member|range_starts
name|uint32_t
modifier|*
name|range_starts
decl_stmt|;
DECL|member|range_ends
name|uint32_t
modifier|*
name|range_ends
decl_stmt|;
else|#
directive|else
comment|/* not _LIBC */
DECL|member|range_starts
name|wchar_t
modifier|*
name|range_starts
decl_stmt|;
DECL|member|range_ends
name|wchar_t
modifier|*
name|range_ends
decl_stmt|;
endif|#
directive|endif
comment|/* not _LIBC */
comment|/* Character classes. */
DECL|member|char_classes
name|wctype_t
modifier|*
name|char_classes
decl_stmt|;
comment|/* If this character set is the non-matching list.  */
DECL|member|non_match
name|unsigned
name|int
name|non_match
range|:
literal|1
decl_stmt|;
comment|/* # of multibyte characters.  */
DECL|member|nmbchars
name|int
name|nmbchars
decl_stmt|;
comment|/* # of collating symbols.  */
DECL|member|ncoll_syms
name|int
name|ncoll_syms
decl_stmt|;
comment|/* # of equivalence classes. */
DECL|member|nequiv_classes
name|int
name|nequiv_classes
decl_stmt|;
comment|/* # of range expressions. */
DECL|member|nranges
name|int
name|nranges
decl_stmt|;
comment|/* # of character classes. */
DECL|member|nchar_classes
name|int
name|nchar_classes
decl_stmt|;
DECL|typedef|re_charset_t
block|}
name|re_charset_t
typedef|;
end_typedef

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* RE_ENABLE_I18N */
end_comment

begin_typedef
typedef|typedef
struct|struct
block|{
union|union
block|{
DECL|member|c
name|unsigned
name|char
name|c
decl_stmt|;
comment|/* for CHARACTER */
DECL|member|sbcset
name|re_bitset_ptr_t
name|sbcset
decl_stmt|;
comment|/* for SIMPLE_BRACKET */
ifdef|#
directive|ifdef
name|RE_ENABLE_I18N
DECL|member|mbcset
name|re_charset_t
modifier|*
name|mbcset
decl_stmt|;
comment|/* for COMPLEX_BRACKET */
endif|#
directive|endif
comment|/* RE_ENABLE_I18N */
DECL|member|idx
name|int
name|idx
decl_stmt|;
comment|/* for BACK_REF */
DECL|member|ctx_type
name|re_context_type
name|ctx_type
decl_stmt|;
comment|/* for ANCHOR */
DECL|member|opr
block|}
name|opr
union|;
if|#
directive|if
name|__GNUC__
operator|>=
literal|2
DECL|member|type
name|re_token_type_t
name|type
range|:
literal|8
decl_stmt|;
else|#
directive|else
DECL|member|type
name|re_token_type_t
name|type
decl_stmt|;
endif|#
directive|endif
DECL|member|constraint
name|unsigned
name|int
name|constraint
range|:
literal|10
decl_stmt|;
comment|/* context constraint */
DECL|member|duplicated
name|unsigned
name|int
name|duplicated
range|:
literal|1
decl_stmt|;
DECL|member|opt_subexp
name|unsigned
name|int
name|opt_subexp
range|:
literal|1
decl_stmt|;
ifdef|#
directive|ifdef
name|RE_ENABLE_I18N
DECL|member|accept_mb
name|unsigned
name|int
name|accept_mb
range|:
literal|1
decl_stmt|;
comment|/* These 2 bits can be moved into the union if needed (e.g. if running out      of bits; move opr.c to opr.c.c and move the flags to opr.c.flags).  */
DECL|member|mb_partial
name|unsigned
name|int
name|mb_partial
range|:
literal|1
decl_stmt|;
endif|#
directive|endif
DECL|member|word_char
name|unsigned
name|int
name|word_char
range|:
literal|1
decl_stmt|;
DECL|typedef|re_token_t
block|}
name|re_token_t
typedef|;
end_typedef

begin_define
DECL|macro|IS_EPSILON_NODE
define|#
directive|define
name|IS_EPSILON_NODE
parameter_list|(
name|type
parameter_list|)
value|((type)& EPSILON_BIT)
end_define

begin_struct
DECL|struct|re_string_t
struct|struct
name|re_string_t
block|{
comment|/* Indicate the raw buffer which is the original string passed as an      argument of regexec(), re_search(), etc..  */
DECL|member|raw_mbs
specifier|const
name|unsigned
name|char
modifier|*
name|raw_mbs
decl_stmt|;
comment|/* Store the multibyte string.  In case of "case insensitive mode" like      REG_ICASE, upper cases of the string are stored, otherwise MBS points      the same address that RAW_MBS points.  */
DECL|member|mbs
name|unsigned
name|char
modifier|*
name|mbs
decl_stmt|;
ifdef|#
directive|ifdef
name|RE_ENABLE_I18N
comment|/* Store the wide character string which is corresponding to MBS.  */
DECL|member|wcs
name|wint_t
modifier|*
name|wcs
decl_stmt|;
DECL|member|offsets
name|int
modifier|*
name|offsets
decl_stmt|;
DECL|member|cur_state
name|mbstate_t
name|cur_state
decl_stmt|;
endif|#
directive|endif
comment|/* Index in RAW_MBS.  Each character mbs[i] corresponds to      raw_mbs[raw_mbs_idx + i].  */
DECL|member|raw_mbs_idx
name|int
name|raw_mbs_idx
decl_stmt|;
comment|/* The length of the valid characters in the buffers.  */
DECL|member|valid_len
name|int
name|valid_len
decl_stmt|;
comment|/* The corresponding number of bytes in raw_mbs array.  */
DECL|member|valid_raw_len
name|int
name|valid_raw_len
decl_stmt|;
comment|/* The length of the buffers MBS and WCS.  */
DECL|member|bufs_len
name|int
name|bufs_len
decl_stmt|;
comment|/* The index in MBS, which is updated by re_string_fetch_byte.  */
DECL|member|cur_idx
name|int
name|cur_idx
decl_stmt|;
comment|/* length of RAW_MBS array.  */
DECL|member|raw_len
name|int
name|raw_len
decl_stmt|;
comment|/* This is RAW_LEN - RAW_MBS_IDX + VALID_LEN - VALID_RAW_LEN.  */
DECL|member|len
name|int
name|len
decl_stmt|;
comment|/* End of the buffer may be shorter than its length in the cases such      as re_match_2, re_search_2.  Then, we use STOP for end of the buffer      instead of LEN.  */
DECL|member|raw_stop
name|int
name|raw_stop
decl_stmt|;
comment|/* This is RAW_STOP - RAW_MBS_IDX adjusted through OFFSETS.  */
DECL|member|stop
name|int
name|stop
decl_stmt|;
comment|/* The context of mbs[0].  We store the context independently, since      the context of mbs[0] may be different from raw_mbs[0], which is      the beginning of the input string.  */
DECL|member|tip_context
name|unsigned
name|int
name|tip_context
decl_stmt|;
comment|/* The translation passed as a part of an argument of re_compile_pattern.  */
DECL|member|trans
name|RE_TRANSLATE_TYPE
name|trans
decl_stmt|;
comment|/* Copy of re_dfa_t's word_char.  */
DECL|member|word_char
name|re_const_bitset_ptr_t
name|word_char
decl_stmt|;
comment|/* 1 if REG_ICASE.  */
DECL|member|icase
name|unsigned
name|char
name|icase
decl_stmt|;
DECL|member|is_utf8
name|unsigned
name|char
name|is_utf8
decl_stmt|;
DECL|member|map_notascii
name|unsigned
name|char
name|map_notascii
decl_stmt|;
DECL|member|mbs_allocated
name|unsigned
name|char
name|mbs_allocated
decl_stmt|;
DECL|member|offsets_needed
name|unsigned
name|char
name|offsets_needed
decl_stmt|;
DECL|member|newline_anchor
name|unsigned
name|char
name|newline_anchor
decl_stmt|;
DECL|member|word_ops_used
name|unsigned
name|char
name|word_ops_used
decl_stmt|;
DECL|member|mb_cur_max
name|int
name|mb_cur_max
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|typedef|re_string_t
typedef|typedef
name|struct
name|re_string_t
name|re_string_t
typedef|;
end_typedef

begin_struct_decl
struct_decl|struct
name|re_dfa_t
struct_decl|;
end_struct_decl

begin_typedef
DECL|typedef|re_dfa_t
typedef|typedef
name|struct
name|re_dfa_t
name|re_dfa_t
typedef|;
end_typedef

begin_ifndef
ifndef|#
directive|ifndef
name|_LIBC
end_ifndef

begin_ifdef
ifdef|#
directive|ifdef
name|__i386__
end_ifdef

begin_define
DECL|macro|internal_function
define|#
directive|define
name|internal_function
value|__attribute ((regparm (3), stdcall))
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|internal_function
define|#
directive|define
name|internal_function
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|NOT_IN_libc
end_ifndef

begin_decl_stmt
specifier|static
name|reg_errcode_t
name|re_string_realloc_buffers
argument_list|(
name|re_string_t
operator|*
name|pstr
argument_list|,
name|int
name|new_buf_len
argument_list|)
name|internal_function
decl_stmt|;
end_decl_stmt

begin_ifdef
ifdef|#
directive|ifdef
name|RE_ENABLE_I18N
end_ifdef

begin_decl_stmt
specifier|static
name|void
name|build_wcs_buffer
argument_list|(
name|re_string_t
operator|*
name|pstr
argument_list|)
name|internal_function
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|static
name|reg_errcode_t
name|build_wcs_upper_buffer
argument_list|(
name|re_string_t
operator|*
name|pstr
argument_list|)
name|internal_function
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* RE_ENABLE_I18N */
end_comment

begin_decl_stmt
specifier|static
name|void
name|build_upper_buffer
argument_list|(
name|re_string_t
operator|*
name|pstr
argument_list|)
name|internal_function
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|static
name|void
name|re_string_translate_buffer
argument_list|(
name|re_string_t
operator|*
name|pstr
argument_list|)
name|internal_function
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|static
name|unsigned
name|int
name|re_string_context_at
argument_list|(
specifier|const
name|re_string_t
operator|*
name|input
argument_list|,
name|int
name|idx
argument_list|,
name|int
name|eflags
argument_list|)
name|internal_function
name|__attribute
argument_list|(
operator|(
name|pure
operator|)
argument_list|)
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|re_string_peek_byte
define|#
directive|define
name|re_string_peek_byte
parameter_list|(
name|pstr
parameter_list|,
name|offset
parameter_list|)
define|\
value|((pstr)->mbs[(pstr)->cur_idx + offset])
end_define

begin_define
DECL|macro|re_string_fetch_byte
define|#
directive|define
name|re_string_fetch_byte
parameter_list|(
name|pstr
parameter_list|)
define|\
value|((pstr)->mbs[(pstr)->cur_idx++])
end_define

begin_define
DECL|macro|re_string_first_byte
define|#
directive|define
name|re_string_first_byte
parameter_list|(
name|pstr
parameter_list|,
name|idx
parameter_list|)
define|\
value|((idx) == (pstr)->valid_len || (pstr)->wcs[idx] != WEOF)
end_define

begin_define
DECL|macro|re_string_is_single_byte_char
define|#
directive|define
name|re_string_is_single_byte_char
parameter_list|(
name|pstr
parameter_list|,
name|idx
parameter_list|)
define|\
value|((pstr)->wcs[idx] != WEOF&& ((pstr)->valid_len == (idx) + 1 \ 				|| (pstr)->wcs[(idx) + 1] != WEOF))
end_define

begin_define
DECL|macro|re_string_eoi
define|#
directive|define
name|re_string_eoi
parameter_list|(
name|pstr
parameter_list|)
value|((pstr)->stop<= (pstr)->cur_idx)
end_define

begin_define
DECL|macro|re_string_cur_idx
define|#
directive|define
name|re_string_cur_idx
parameter_list|(
name|pstr
parameter_list|)
value|((pstr)->cur_idx)
end_define

begin_define
DECL|macro|re_string_get_buffer
define|#
directive|define
name|re_string_get_buffer
parameter_list|(
name|pstr
parameter_list|)
value|((pstr)->mbs)
end_define

begin_define
DECL|macro|re_string_length
define|#
directive|define
name|re_string_length
parameter_list|(
name|pstr
parameter_list|)
value|((pstr)->len)
end_define

begin_define
DECL|macro|re_string_byte_at
define|#
directive|define
name|re_string_byte_at
parameter_list|(
name|pstr
parameter_list|,
name|idx
parameter_list|)
value|((pstr)->mbs[idx])
end_define

begin_define
DECL|macro|re_string_skip_bytes
define|#
directive|define
name|re_string_skip_bytes
parameter_list|(
name|pstr
parameter_list|,
name|idx
parameter_list|)
value|((pstr)->cur_idx += (idx))
end_define

begin_define
DECL|macro|re_string_set_index
define|#
directive|define
name|re_string_set_index
parameter_list|(
name|pstr
parameter_list|,
name|idx
parameter_list|)
value|((pstr)->cur_idx = (idx))
end_define

begin_ifndef
ifndef|#
directive|ifndef
name|_LIBC
end_ifndef

begin_if
if|#
directive|if
name|HAVE_ALLOCA
end_if

begin_if
if|#
directive|if
operator|(
name|_MSC_VER
operator|)
end_if

begin_include
include|#
directive|include
file|<malloc.h>
end_include

begin_define
DECL|macro|__libc_use_alloca
define|#
directive|define
name|__libc_use_alloca
parameter_list|(
name|n
parameter_list|)
value|0
end_define

begin_else
else|#
directive|else
end_else

begin_include
include|#
directive|include
file|<alloca.h>
end_include

begin_comment
comment|/* The OS usually guarantees only one guard page at the bottom of the stack,    and a page size can be as small as 4096 bytes.  So we cannot safely    allocate anything larger than 4096 bytes.  Also care for the possibility    of a few compiler-allocated temporary stack slots.  */
end_comment

begin_define
DECL|macro|__libc_use_alloca
define|#
directive|define
name|__libc_use_alloca
parameter_list|(
name|n
parameter_list|)
value|((n)< 4032)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_else
else|#
directive|else
end_else

begin_comment
comment|/* alloca is implemented with malloc, so just use malloc.  */
end_comment

begin_define
DECL|macro|__libc_use_alloca
define|#
directive|define
name|__libc_use_alloca
parameter_list|(
name|n
parameter_list|)
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|re_malloc
define|#
directive|define
name|re_malloc
parameter_list|(
name|t
parameter_list|,
name|n
parameter_list|)
value|((t *) malloc ((n) * sizeof (t)))
end_define

begin_comment
comment|/* SunOS 4.1.x realloc doesn't accept null pointers: pre-Standard C. Sigh. */
end_comment

begin_define
DECL|macro|re_realloc
define|#
directive|define
name|re_realloc
parameter_list|(
name|p
parameter_list|,
name|t
parameter_list|,
name|n
parameter_list|)
value|((p != NULL) ? (t *) realloc (p,(n)*sizeof(t)) : (t *) calloc(n,sizeof(t)))
end_define

begin_define
DECL|macro|re_free
define|#
directive|define
name|re_free
parameter_list|(
name|p
parameter_list|)
value|free (p)
end_define

begin_struct
DECL|struct|bin_tree_t
struct|struct
name|bin_tree_t
block|{
DECL|member|parent
name|struct
name|bin_tree_t
modifier|*
name|parent
decl_stmt|;
DECL|member|left
name|struct
name|bin_tree_t
modifier|*
name|left
decl_stmt|;
DECL|member|right
name|struct
name|bin_tree_t
modifier|*
name|right
decl_stmt|;
DECL|member|first
name|struct
name|bin_tree_t
modifier|*
name|first
decl_stmt|;
DECL|member|next
name|struct
name|bin_tree_t
modifier|*
name|next
decl_stmt|;
DECL|member|token
name|re_token_t
name|token
decl_stmt|;
comment|/* `node_idx' is the index in dfa->nodes, if `type' == 0.      Otherwise `type' indicate the type of this node.  */
DECL|member|node_idx
name|int
name|node_idx
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|typedef|bin_tree_t
typedef|typedef
name|struct
name|bin_tree_t
name|bin_tree_t
typedef|;
end_typedef

begin_define
DECL|macro|BIN_TREE_STORAGE_SIZE
define|#
directive|define
name|BIN_TREE_STORAGE_SIZE
define|\
value|((1024 - sizeof (void *)) / sizeof (bin_tree_t))
end_define

begin_struct
DECL|struct|bin_tree_storage_t
struct|struct
name|bin_tree_storage_t
block|{
DECL|member|next
name|struct
name|bin_tree_storage_t
modifier|*
name|next
decl_stmt|;
DECL|member|data
name|bin_tree_t
name|data
index|[
name|BIN_TREE_STORAGE_SIZE
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|typedef|bin_tree_storage_t
typedef|typedef
name|struct
name|bin_tree_storage_t
name|bin_tree_storage_t
typedef|;
end_typedef

begin_define
DECL|macro|CONTEXT_WORD
define|#
directive|define
name|CONTEXT_WORD
value|1
end_define

begin_define
DECL|macro|CONTEXT_NEWLINE
define|#
directive|define
name|CONTEXT_NEWLINE
value|(CONTEXT_WORD<< 1)
end_define

begin_define
DECL|macro|CONTEXT_BEGBUF
define|#
directive|define
name|CONTEXT_BEGBUF
value|(CONTEXT_NEWLINE<< 1)
end_define

begin_define
DECL|macro|CONTEXT_ENDBUF
define|#
directive|define
name|CONTEXT_ENDBUF
value|(CONTEXT_BEGBUF<< 1)
end_define

begin_define
DECL|macro|IS_WORD_CONTEXT
define|#
directive|define
name|IS_WORD_CONTEXT
parameter_list|(
name|c
parameter_list|)
value|((c)& CONTEXT_WORD)
end_define

begin_define
DECL|macro|IS_NEWLINE_CONTEXT
define|#
directive|define
name|IS_NEWLINE_CONTEXT
parameter_list|(
name|c
parameter_list|)
value|((c)& CONTEXT_NEWLINE)
end_define

begin_define
DECL|macro|IS_BEGBUF_CONTEXT
define|#
directive|define
name|IS_BEGBUF_CONTEXT
parameter_list|(
name|c
parameter_list|)
value|((c)& CONTEXT_BEGBUF)
end_define

begin_define
DECL|macro|IS_ENDBUF_CONTEXT
define|#
directive|define
name|IS_ENDBUF_CONTEXT
parameter_list|(
name|c
parameter_list|)
value|((c)& CONTEXT_ENDBUF)
end_define

begin_define
DECL|macro|IS_ORDINARY_CONTEXT
define|#
directive|define
name|IS_ORDINARY_CONTEXT
parameter_list|(
name|c
parameter_list|)
value|((c) == 0)
end_define

begin_define
DECL|macro|IS_WORD_CHAR
define|#
directive|define
name|IS_WORD_CHAR
parameter_list|(
name|ch
parameter_list|)
value|(isalnum (ch) || (ch) == '_')
end_define

begin_define
DECL|macro|IS_NEWLINE
define|#
directive|define
name|IS_NEWLINE
parameter_list|(
name|ch
parameter_list|)
value|((ch) == NEWLINE_CHAR)
end_define

begin_define
DECL|macro|IS_WIDE_WORD_CHAR
define|#
directive|define
name|IS_WIDE_WORD_CHAR
parameter_list|(
name|ch
parameter_list|)
value|(iswalnum (ch) || (ch) == L'_')
end_define

begin_define
DECL|macro|IS_WIDE_NEWLINE
define|#
directive|define
name|IS_WIDE_NEWLINE
parameter_list|(
name|ch
parameter_list|)
value|((ch) == WIDE_NEWLINE_CHAR)
end_define

begin_define
DECL|macro|NOT_SATISFY_PREV_CONSTRAINT
define|#
directive|define
name|NOT_SATISFY_PREV_CONSTRAINT
parameter_list|(
name|constraint
parameter_list|,
name|context
parameter_list|)
define|\
value|((((constraint)& PREV_WORD_CONSTRAINT)&& !IS_WORD_CONTEXT (context)) \   || ((constraint& PREV_NOTWORD_CONSTRAINT)&& IS_WORD_CONTEXT (context)) \   || ((constraint& PREV_NEWLINE_CONSTRAINT)&& !IS_NEWLINE_CONTEXT (context))\   || ((constraint& PREV_BEGBUF_CONSTRAINT)&& !IS_BEGBUF_CONTEXT (context)))
end_define

begin_define
DECL|macro|NOT_SATISFY_NEXT_CONSTRAINT
define|#
directive|define
name|NOT_SATISFY_NEXT_CONSTRAINT
parameter_list|(
name|constraint
parameter_list|,
name|context
parameter_list|)
define|\
value|((((constraint)& NEXT_WORD_CONSTRAINT)&& !IS_WORD_CONTEXT (context)) \   || (((constraint)& NEXT_NOTWORD_CONSTRAINT)&& IS_WORD_CONTEXT (context)) \   || (((constraint)& NEXT_NEWLINE_CONSTRAINT)&& !IS_NEWLINE_CONTEXT (context)) \   || (((constraint)& NEXT_ENDBUF_CONSTRAINT)&& !IS_ENDBUF_CONTEXT (context)))
end_define

begin_struct
DECL|struct|re_dfastate_t
struct|struct
name|re_dfastate_t
block|{
DECL|member|hash
name|unsigned
name|int
name|hash
decl_stmt|;
DECL|member|nodes
name|re_node_set
name|nodes
decl_stmt|;
DECL|member|non_eps_nodes
name|re_node_set
name|non_eps_nodes
decl_stmt|;
DECL|member|inveclosure
name|re_node_set
name|inveclosure
decl_stmt|;
DECL|member|entrance_nodes
name|re_node_set
modifier|*
name|entrance_nodes
decl_stmt|;
DECL|member|trtable
DECL|member|word_trtable
name|struct
name|re_dfastate_t
modifier|*
modifier|*
name|trtable
decl_stmt|,
modifier|*
modifier|*
name|word_trtable
decl_stmt|;
DECL|member|context
name|unsigned
name|int
name|context
range|:
literal|4
decl_stmt|;
DECL|member|halt
name|unsigned
name|int
name|halt
range|:
literal|1
decl_stmt|;
comment|/* If this state can accept `multi byte'.      Note that we refer to multibyte characters, and multi character      collating elements as `multi byte'.  */
DECL|member|accept_mb
name|unsigned
name|int
name|accept_mb
range|:
literal|1
decl_stmt|;
comment|/* If this state has backreference node(s).  */
DECL|member|has_backref
name|unsigned
name|int
name|has_backref
range|:
literal|1
decl_stmt|;
DECL|member|has_constraint
name|unsigned
name|int
name|has_constraint
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|typedef|re_dfastate_t
typedef|typedef
name|struct
name|re_dfastate_t
name|re_dfastate_t
typedef|;
end_typedef

begin_struct
DECL|struct|re_state_table_entry
struct|struct
name|re_state_table_entry
block|{
DECL|member|num
name|int
name|num
decl_stmt|;
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|array
name|re_dfastate_t
modifier|*
modifier|*
name|array
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* Array type used in re_sub_match_last_t and re_sub_match_top_t.  */
end_comment

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|next_idx
name|int
name|next_idx
decl_stmt|;
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|array
name|re_dfastate_t
modifier|*
modifier|*
name|array
decl_stmt|;
DECL|typedef|state_array_t
block|}
name|state_array_t
typedef|;
end_typedef

begin_comment
comment|/* Store information about the node NODE whose type is OP_CLOSE_SUBEXP.  */
end_comment

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|node
name|int
name|node
decl_stmt|;
DECL|member|str_idx
name|int
name|str_idx
decl_stmt|;
comment|/* The position NODE match at.  */
DECL|member|path
name|state_array_t
name|path
decl_stmt|;
DECL|typedef|re_sub_match_last_t
block|}
name|re_sub_match_last_t
typedef|;
end_typedef

begin_comment
comment|/* Store information about the node NODE whose type is OP_OPEN_SUBEXP.    And information about the node, whose type is OP_CLOSE_SUBEXP,    corresponding to NODE is stored in LASTS.  */
end_comment

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|str_idx
name|int
name|str_idx
decl_stmt|;
DECL|member|node
name|int
name|node
decl_stmt|;
DECL|member|path
name|state_array_t
modifier|*
name|path
decl_stmt|;
DECL|member|alasts
name|int
name|alasts
decl_stmt|;
comment|/* Allocation size of LASTS.  */
DECL|member|nlasts
name|int
name|nlasts
decl_stmt|;
comment|/* The number of LASTS.  */
DECL|member|lasts
name|re_sub_match_last_t
modifier|*
modifier|*
name|lasts
decl_stmt|;
DECL|typedef|re_sub_match_top_t
block|}
name|re_sub_match_top_t
typedef|;
end_typedef

begin_struct
DECL|struct|re_backref_cache_entry
struct|struct
name|re_backref_cache_entry
block|{
DECL|member|node
name|int
name|node
decl_stmt|;
DECL|member|str_idx
name|int
name|str_idx
decl_stmt|;
DECL|member|subexp_from
name|int
name|subexp_from
decl_stmt|;
DECL|member|subexp_to
name|int
name|subexp_to
decl_stmt|;
DECL|member|more
name|char
name|more
decl_stmt|;
DECL|member|unused
name|char
name|unused
decl_stmt|;
DECL|member|eps_reachable_subexps_map
name|unsigned
name|short
name|int
name|eps_reachable_subexps_map
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
typedef|typedef
struct|struct
block|{
comment|/* The string object corresponding to the input string.  */
DECL|member|input
name|re_string_t
name|input
decl_stmt|;
if|#
directive|if
name|defined
name|_LIBC
operator|||
operator|(
name|defined
name|__STDC_VERSION__
operator|&&
name|__STDC_VERSION__
operator|>=
literal|199901L
operator|)
DECL|member|dfa
specifier|const
name|re_dfa_t
modifier|*
specifier|const
name|dfa
decl_stmt|;
else|#
directive|else
DECL|member|dfa
specifier|const
name|re_dfa_t
modifier|*
name|dfa
decl_stmt|;
endif|#
directive|endif
comment|/* EFLAGS of the argument of regexec.  */
DECL|member|eflags
name|int
name|eflags
decl_stmt|;
comment|/* Where the matching ends.  */
DECL|member|match_last
name|int
name|match_last
decl_stmt|;
DECL|member|last_node
name|int
name|last_node
decl_stmt|;
comment|/* The state log used by the matcher.  */
DECL|member|state_log
name|re_dfastate_t
modifier|*
modifier|*
name|state_log
decl_stmt|;
DECL|member|state_log_top
name|int
name|state_log_top
decl_stmt|;
comment|/* Back reference cache.  */
DECL|member|nbkref_ents
name|int
name|nbkref_ents
decl_stmt|;
DECL|member|abkref_ents
name|int
name|abkref_ents
decl_stmt|;
DECL|member|bkref_ents
name|struct
name|re_backref_cache_entry
modifier|*
name|bkref_ents
decl_stmt|;
DECL|member|max_mb_elem_len
name|int
name|max_mb_elem_len
decl_stmt|;
DECL|member|nsub_tops
name|int
name|nsub_tops
decl_stmt|;
DECL|member|asub_tops
name|int
name|asub_tops
decl_stmt|;
DECL|member|sub_tops
name|re_sub_match_top_t
modifier|*
modifier|*
name|sub_tops
decl_stmt|;
DECL|typedef|re_match_context_t
block|}
name|re_match_context_t
typedef|;
end_typedef

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|sifted_states
name|re_dfastate_t
modifier|*
modifier|*
name|sifted_states
decl_stmt|;
DECL|member|limited_states
name|re_dfastate_t
modifier|*
modifier|*
name|limited_states
decl_stmt|;
DECL|member|last_node
name|int
name|last_node
decl_stmt|;
DECL|member|last_str_idx
name|int
name|last_str_idx
decl_stmt|;
DECL|member|limits
name|re_node_set
name|limits
decl_stmt|;
DECL|typedef|re_sift_context_t
block|}
name|re_sift_context_t
typedef|;
end_typedef

begin_struct
DECL|struct|re_fail_stack_ent_t
struct|struct
name|re_fail_stack_ent_t
block|{
DECL|member|idx
name|int
name|idx
decl_stmt|;
DECL|member|node
name|int
name|node
decl_stmt|;
DECL|member|regs
name|regmatch_t
modifier|*
name|regs
decl_stmt|;
DECL|member|eps_via_nodes
name|re_node_set
name|eps_via_nodes
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|re_fail_stack_t
struct|struct
name|re_fail_stack_t
block|{
DECL|member|num
name|int
name|num
decl_stmt|;
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|stack
name|struct
name|re_fail_stack_ent_t
modifier|*
name|stack
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|re_dfa_t
struct|struct
name|re_dfa_t
block|{
DECL|member|nodes
name|re_token_t
modifier|*
name|nodes
decl_stmt|;
DECL|member|nodes_alloc
name|size_t
name|nodes_alloc
decl_stmt|;
DECL|member|nodes_len
name|size_t
name|nodes_len
decl_stmt|;
DECL|member|nexts
name|int
modifier|*
name|nexts
decl_stmt|;
DECL|member|org_indices
name|int
modifier|*
name|org_indices
decl_stmt|;
DECL|member|edests
name|re_node_set
modifier|*
name|edests
decl_stmt|;
DECL|member|eclosures
name|re_node_set
modifier|*
name|eclosures
decl_stmt|;
DECL|member|inveclosures
name|re_node_set
modifier|*
name|inveclosures
decl_stmt|;
DECL|member|state_table
name|struct
name|re_state_table_entry
modifier|*
name|state_table
decl_stmt|;
DECL|member|init_state
name|re_dfastate_t
modifier|*
name|init_state
decl_stmt|;
DECL|member|init_state_word
name|re_dfastate_t
modifier|*
name|init_state_word
decl_stmt|;
DECL|member|init_state_nl
name|re_dfastate_t
modifier|*
name|init_state_nl
decl_stmt|;
DECL|member|init_state_begbuf
name|re_dfastate_t
modifier|*
name|init_state_begbuf
decl_stmt|;
DECL|member|str_tree
name|bin_tree_t
modifier|*
name|str_tree
decl_stmt|;
DECL|member|str_tree_storage
name|bin_tree_storage_t
modifier|*
name|str_tree_storage
decl_stmt|;
DECL|member|sb_char
name|re_bitset_ptr_t
name|sb_char
decl_stmt|;
DECL|member|str_tree_storage_idx
name|int
name|str_tree_storage_idx
decl_stmt|;
comment|/* number of subexpressions `re_nsub' is in regex_t.  */
DECL|member|state_hash_mask
name|unsigned
name|int
name|state_hash_mask
decl_stmt|;
DECL|member|init_node
name|int
name|init_node
decl_stmt|;
DECL|member|nbackref
name|int
name|nbackref
decl_stmt|;
comment|/* The number of backreference in this dfa.  */
comment|/* Bitmap expressing which backreference is used.  */
DECL|member|used_bkref_map
name|bitset_word_t
name|used_bkref_map
decl_stmt|;
DECL|member|completed_bkref_map
name|bitset_word_t
name|completed_bkref_map
decl_stmt|;
DECL|member|has_plural_match
name|unsigned
name|int
name|has_plural_match
range|:
literal|1
decl_stmt|;
comment|/* If this dfa has "multibyte node", which is a backreference or      a node which can accept multibyte character or multi character      collating element.  */
DECL|member|has_mb_node
name|unsigned
name|int
name|has_mb_node
range|:
literal|1
decl_stmt|;
DECL|member|is_utf8
name|unsigned
name|int
name|is_utf8
range|:
literal|1
decl_stmt|;
DECL|member|map_notascii
name|unsigned
name|int
name|map_notascii
range|:
literal|1
decl_stmt|;
DECL|member|word_ops_used
name|unsigned
name|int
name|word_ops_used
range|:
literal|1
decl_stmt|;
DECL|member|mb_cur_max
name|int
name|mb_cur_max
decl_stmt|;
DECL|member|word_char
name|bitset_t
name|word_char
decl_stmt|;
DECL|member|syntax
name|reg_syntax_t
name|syntax
decl_stmt|;
DECL|member|subexp_map
name|int
modifier|*
name|subexp_map
decl_stmt|;
ifdef|#
directive|ifdef
name|DEBUG
DECL|member|re_str
name|char
modifier|*
name|re_str
decl_stmt|;
endif|#
directive|endif
if|#
directive|if
name|defined
name|_LIBC
name|__libc_lock_define
argument_list|(
argument_list|,
argument|lock
argument_list|)
endif|#
directive|endif
block|}
struct|;
end_struct

begin_define
DECL|macro|re_node_set_init_empty
define|#
directive|define
name|re_node_set_init_empty
parameter_list|(
name|set
parameter_list|)
value|memset (set, '\0', sizeof (re_node_set))
end_define

begin_define
DECL|macro|re_node_set_remove
define|#
directive|define
name|re_node_set_remove
parameter_list|(
name|set
parameter_list|,
name|id
parameter_list|)
define|\
value|(re_node_set_remove_at (set, re_node_set_contains (set, id) - 1))
end_define

begin_define
DECL|macro|re_node_set_empty
define|#
directive|define
name|re_node_set_empty
parameter_list|(
name|p
parameter_list|)
value|((p)->nelem = 0)
end_define

begin_define
DECL|macro|re_node_set_free
define|#
directive|define
name|re_node_set_free
parameter_list|(
name|set
parameter_list|)
value|re_free ((set)->elems)
end_define

begin_escape
end_escape

begin_typedef
typedef|typedef
enum|enum
block|{
DECL|enumerator|SB_CHAR
name|SB_CHAR
block|,
DECL|enumerator|MB_CHAR
name|MB_CHAR
block|,
DECL|enumerator|EQUIV_CLASS
name|EQUIV_CLASS
block|,
DECL|enumerator|COLL_SYM
name|COLL_SYM
block|,
DECL|enumerator|CHAR_CLASS
name|CHAR_CLASS
DECL|typedef|bracket_elem_type
block|}
name|bracket_elem_type
typedef|;
end_typedef

begin_typedef
typedef|typedef
struct|struct
block|{
DECL|member|type
name|bracket_elem_type
name|type
decl_stmt|;
union|union
block|{
DECL|member|ch
name|unsigned
name|char
name|ch
decl_stmt|;
DECL|member|name
name|unsigned
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|wch
name|wchar_t
name|wch
decl_stmt|;
DECL|member|opr
block|}
name|opr
union|;
DECL|typedef|bracket_elem_t
block|}
name|bracket_elem_t
typedef|;
end_typedef

begin_comment
comment|/* Inline functions for bitset operation.  */
end_comment

begin_function
specifier|static
specifier|inline
name|void
DECL|function|bitset_not
name|bitset_not
parameter_list|(
name|bitset_t
name|set
parameter_list|)
block|{
name|int
name|bitset_i
decl_stmt|;
for|for
control|(
name|bitset_i
operator|=
literal|0
init|;
name|bitset_i
operator|<
name|BITSET_WORDS
condition|;
operator|++
name|bitset_i
control|)
name|set
index|[
name|bitset_i
index|]
operator|=
operator|~
name|set
index|[
name|bitset_i
index|]
expr_stmt|;
block|}
end_function

begin_function
specifier|static
specifier|inline
name|void
DECL|function|bitset_merge
name|bitset_merge
parameter_list|(
name|bitset_t
name|dest
parameter_list|,
specifier|const
name|bitset_t
name|src
parameter_list|)
block|{
name|int
name|bitset_i
decl_stmt|;
for|for
control|(
name|bitset_i
operator|=
literal|0
init|;
name|bitset_i
operator|<
name|BITSET_WORDS
condition|;
operator|++
name|bitset_i
control|)
name|dest
index|[
name|bitset_i
index|]
operator||=
name|src
index|[
name|bitset_i
index|]
expr_stmt|;
block|}
end_function

begin_function
specifier|static
specifier|inline
name|void
DECL|function|bitset_mask
name|bitset_mask
parameter_list|(
name|bitset_t
name|dest
parameter_list|,
specifier|const
name|bitset_t
name|src
parameter_list|)
block|{
name|int
name|bitset_i
decl_stmt|;
for|for
control|(
name|bitset_i
operator|=
literal|0
init|;
name|bitset_i
operator|<
name|BITSET_WORDS
condition|;
operator|++
name|bitset_i
control|)
name|dest
index|[
name|bitset_i
index|]
operator|&=
name|src
index|[
name|bitset_i
index|]
expr_stmt|;
block|}
end_function

begin_ifdef
ifdef|#
directive|ifdef
name|RE_ENABLE_I18N
end_ifdef

begin_comment
comment|/* Inline functions for re_string.  */
end_comment

begin_decl_stmt
specifier|static
specifier|inline
name|int
name|internal_function
name|__attribute
argument_list|(
operator|(
name|pure
operator|)
argument_list|)
DECL|function|re_string_char_size_at
name|re_string_char_size_at
argument_list|(
specifier|const
name|re_string_t
operator|*
name|pstr
argument_list|,
name|int
name|idx
argument_list|)
block|{
name|int
name|byte_idx
decl_stmt|;
if|if
condition|(
name|pstr
operator|->
name|mb_cur_max
operator|==
literal|1
condition|)
return|return
literal|1
return|;
for|for
control|(
name|byte_idx
operator|=
literal|1
init|;
name|idx
operator|+
name|byte_idx
operator|<
name|pstr
operator|->
name|valid_len
condition|;
operator|++
name|byte_idx
control|)
if|if
condition|(
name|pstr
operator|->
name|wcs
index|[
name|idx
operator|+
name|byte_idx
index|]
operator|!=
name|WEOF
condition|)
break|break;
return|return
name|byte_idx
return|;
block|}
end_decl_stmt

begin_decl_stmt
specifier|static
specifier|inline
name|wint_t
name|internal_function
name|__attribute
argument_list|(
operator|(
name|pure
operator|)
argument_list|)
DECL|function|re_string_wchar_at
name|re_string_wchar_at
argument_list|(
specifier|const
name|re_string_t
operator|*
name|pstr
argument_list|,
name|int
name|idx
argument_list|)
block|{
if|if
condition|(
name|pstr
operator|->
name|mb_cur_max
operator|==
literal|1
condition|)
return|return
operator|(
name|wint_t
operator|)
name|pstr
operator|->
name|mbs
index|[
name|idx
index|]
return|;
return|return
operator|(
name|wint_t
operator|)
name|pstr
operator|->
name|wcs
index|[
name|idx
index|]
return|;
block|}
end_decl_stmt

begin_ifndef
ifndef|#
directive|ifndef
name|NOT_IN_libc
end_ifndef

begin_decl_stmt
specifier|static
name|int
name|internal_function
name|__attribute
argument_list|(
operator|(
name|pure
operator|)
argument_list|)
DECL|function|re_string_elem_size_at
name|re_string_elem_size_at
argument_list|(
specifier|const
name|re_string_t
operator|*
name|pstr
argument_list|,
name|int
name|idx
argument_list|)
block|{
ifdef|#
directive|ifdef
name|_LIBC
specifier|const
name|unsigned
name|char
modifier|*
name|p
decl_stmt|,
modifier|*
name|extra
decl_stmt|;
specifier|const
name|int32_t
modifier|*
name|table
decl_stmt|,
modifier|*
name|indirect
decl_stmt|;
name|int32_t
name|tmp
decl_stmt|;
include|#
directive|include
file|<locale/weight.h>
name|uint_fast32_t
name|nrules
init|=
name|_NL_CURRENT_WORD
argument_list|(
name|LC_COLLATE
argument_list|,
name|_NL_COLLATE_NRULES
argument_list|)
decl_stmt|;
if|if
condition|(
name|nrules
operator|!=
literal|0
condition|)
block|{
name|table
operator|=
operator|(
specifier|const
name|int32_t
operator|*
operator|)
name|_NL_CURRENT
argument_list|(
name|LC_COLLATE
argument_list|,
name|_NL_COLLATE_TABLEMB
argument_list|)
expr_stmt|;
name|extra
operator|=
operator|(
specifier|const
name|unsigned
name|char
operator|*
operator|)
name|_NL_CURRENT
argument_list|(
name|LC_COLLATE
argument_list|,
name|_NL_COLLATE_EXTRAMB
argument_list|)
expr_stmt|;
name|indirect
operator|=
operator|(
specifier|const
name|int32_t
operator|*
operator|)
name|_NL_CURRENT
argument_list|(
name|LC_COLLATE
argument_list|,
name|_NL_COLLATE_INDIRECTMB
argument_list|)
expr_stmt|;
name|p
operator|=
name|pstr
operator|->
name|mbs
operator|+
name|idx
expr_stmt|;
name|tmp
operator|=
name|findidx
argument_list|(
operator|&
name|p
argument_list|)
expr_stmt|;
return|return
name|p
operator|-
name|pstr
operator|->
name|mbs
operator|-
name|idx
return|;
block|}
else|else
endif|#
directive|endif
comment|/* _LIBC */
return|return
literal|1
return|;
block|}
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* RE_ENABLE_I18N */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  _REGEX_INTERNAL_H */
end_comment

end_unit

