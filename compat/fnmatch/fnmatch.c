begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/* Copyright (C) 1991, 92, 93, 96, 97, 98, 99 Free Software Foundation, Inc.    This file is part of the GNU C Library.     This library is free software; you can redistribute it and/or    modify it under the terms of the GNU Library General Public License as    published by the Free Software Foundation; either version 2 of the    License, or (at your option) any later version.     This library is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    Library General Public License for more details.     You should have received a copy of the GNU Library General Public    License along with this library; see the file COPYING.LIB.  If not,    write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,    Boston, MA 02111-1307, USA.  */
end_comment

begin_if
if|#
directive|if
name|HAVE_CONFIG_H
end_if

begin_include
include|#
directive|include
file|<config.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* Enable GNU extensions in fnmatch.h.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_GNU_SOURCE
end_ifndef

begin_define
DECL|macro|_GNU_SOURCE
define|#
directive|define
name|_GNU_SOURCE
value|1
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_include
include|#
directive|include
file|<errno.h>
end_include

begin_include
include|#
directive|include
file|<fnmatch.h>
end_include

begin_include
include|#
directive|include
file|<ctype.h>
end_include

begin_if
if|#
directive|if
name|HAVE_STRING_H
operator|||
name|defined
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_else
else|#
directive|else
end_else

begin_include
include|#
directive|include
file|<strings.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|defined
name|STDC_HEADERS
operator|||
name|defined
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* For platforms which support the ISO C amendment 1 functionality we    support user defined character classes.  */
end_comment

begin_if
if|#
directive|if
name|defined
name|_LIBC
operator|||
operator|(
name|defined
name|HAVE_WCTYPE_H
operator|&&
name|defined
name|HAVE_WCHAR_H
operator|)
end_if

begin_comment
comment|/* Solaris 2.5 has a bug:<wchar.h> must be included before<wctype.h>.  */
end_comment

begin_include
include|#
directive|include
file|<wchar.h>
end_include

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
comment|/* Comment out all this code if we are using the GNU C Library, and are not    actually compiling the library itself.  This code is part of the GNU C    Library, but also included in many other GNU distributions.  Compiling    and linking in this code is a waste when using the GNU C library    (especially if it is a shared library).  Rather than having every GNU    program understand `configure --with-gnu-libc' and omit the object files,    it is simpler to just do this in the source for each such file.  */
end_comment

begin_if
if|#
directive|if
name|defined
name|NO_FNMATCH
operator|||
name|defined
name|NO_FNMATCH_CASEFOLD
operator|||
expr|\
name|defined
name|_LIBC
operator|||
operator|!
name|defined
name|__GNU_LIBRARY__
end_if

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
value|(ISASCII (c)&& isblank (c))
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
value|(ISASCII (c)&& isgraph (c))
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
value|(ISASCII (c)&& isprint (c)&& !isspace (c))
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
value|(ISASCII (c)&& isprint (c))
end_define

begin_define
DECL|macro|ISDIGIT
define|#
directive|define
name|ISDIGIT
parameter_list|(
name|c
parameter_list|)
value|(ISASCII (c)&& isdigit (c))
end_define

begin_define
DECL|macro|ISALNUM
define|#
directive|define
name|ISALNUM
parameter_list|(
name|c
parameter_list|)
value|(ISASCII (c)&& isalnum (c))
end_define

begin_define
DECL|macro|ISALPHA
define|#
directive|define
name|ISALPHA
parameter_list|(
name|c
parameter_list|)
value|(ISASCII (c)&& isalpha (c))
end_define

begin_define
DECL|macro|ISCNTRL
define|#
directive|define
name|ISCNTRL
parameter_list|(
name|c
parameter_list|)
value|(ISASCII (c)&& iscntrl (c))
end_define

begin_define
DECL|macro|ISLOWER
define|#
directive|define
name|ISLOWER
parameter_list|(
name|c
parameter_list|)
value|(ISASCII (c)&& islower (c))
end_define

begin_define
DECL|macro|ISPUNCT
define|#
directive|define
name|ISPUNCT
parameter_list|(
name|c
parameter_list|)
value|(ISASCII (c)&& ispunct (c))
end_define

begin_define
DECL|macro|ISSPACE
define|#
directive|define
name|ISSPACE
parameter_list|(
name|c
parameter_list|)
value|(ISASCII (c)&& isspace (c))
end_define

begin_define
DECL|macro|ISUPPER
define|#
directive|define
name|ISUPPER
parameter_list|(
name|c
parameter_list|)
value|(ISASCII (c)&& isupper (c))
end_define

begin_define
DECL|macro|ISXDIGIT
define|#
directive|define
name|ISXDIGIT
parameter_list|(
name|c
parameter_list|)
value|(ISASCII (c)&& isxdigit (c))
end_define

begin_define
DECL|macro|STREQ
define|#
directive|define
name|STREQ
parameter_list|(
name|s1
parameter_list|,
name|s2
parameter_list|)
value|((strcmp (s1, s2) == 0))
end_define

begin_if
if|#
directive|if
name|defined
name|_LIBC
operator|||
operator|(
name|defined
name|HAVE_WCTYPE_H
operator|&&
name|defined
name|HAVE_WCHAR_H
operator|)
end_if

begin_comment
comment|/* The GNU C library provides support for user-defined character classes    and the functions from ISO C amendment 1.  */
end_comment

begin_ifdef
ifdef|#
directive|ifdef
name|CHARCLASS_NAME_MAX
end_ifdef

begin_define
DECL|macro|CHAR_CLASS_MAX_LENGTH
define|#
directive|define
name|CHAR_CLASS_MAX_LENGTH
value|CHARCLASS_NAME_MAX
end_define

begin_else
else|#
directive|else
end_else

begin_comment
comment|/* This shouldn't happen but some implementation might still have this    problem.  Use a reasonable default value.  */
end_comment

begin_define
DECL|macro|CHAR_CLASS_MAX_LENGTH
define|#
directive|define
name|CHAR_CLASS_MAX_LENGTH
value|256
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|_LIBC
end_ifdef

begin_define
DECL|macro|IS_CHAR_CLASS
define|#
directive|define
name|IS_CHAR_CLASS
parameter_list|(
name|string
parameter_list|)
value|__wctype (string)
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|IS_CHAR_CLASS
define|#
directive|define
name|IS_CHAR_CLASS
parameter_list|(
name|string
parameter_list|)
value|wctype (string)
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
DECL|macro|CHAR_CLASS_MAX_LENGTH
define|#
directive|define
name|CHAR_CLASS_MAX_LENGTH
value|6
end_define

begin_comment
DECL|macro|CHAR_CLASS_MAX_LENGTH
comment|/* Namely, `xdigit'.  */
end_comment

begin_define
DECL|macro|IS_CHAR_CLASS
define|#
directive|define
name|IS_CHAR_CLASS
parameter_list|(
name|string
parameter_list|)
define|\
value|(STREQ (string, "alpha") || STREQ (string, "upper")			      \     || STREQ (string, "lower") || STREQ (string, "digit")		      \     || STREQ (string, "alnum") || STREQ (string, "xdigit")		      \     || STREQ (string, "space") || STREQ (string, "print")		      \     || STREQ (string, "punct") || STREQ (string, "graph")		      \     || STREQ (string, "cntrl") || STREQ (string, "blank"))
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* Avoid depending on library functions or files    whose names are inconsistent.  */
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
name|getenv
end_if

begin_function_decl
specifier|extern
name|char
modifier|*
name|getenv
parameter_list|()
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|errno
end_ifndef

begin_decl_stmt
specifier|extern
name|int
name|errno
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|NULL
end_ifndef

begin_define
DECL|macro|NULL
define|#
directive|define
name|NULL
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* This function doesn't exist on most systems.  */
end_comment

begin_if
if|#
directive|if
operator|!
name|defined
name|HAVE___STRCHRNUL
operator|&&
operator|!
name|defined
name|_LIBC
end_if

begin_function
specifier|static
name|char
modifier|*
DECL|function|__strchrnul
name|__strchrnul
parameter_list|(
specifier|const
name|char
modifier|*
name|s
parameter_list|,
name|int
name|c
parameter_list|)
block|{
name|char
modifier|*
name|result
init|=
name|strchr
argument_list|(
name|s
argument_list|,
name|c
argument_list|)
decl_stmt|;
if|if
condition|(
name|result
operator|==
name|NULL
condition|)
name|result
operator|=
name|strchr
argument_list|(
name|s
argument_list|,
literal|'\0'
argument_list|)
expr_stmt|;
return|return
name|result
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|internal_function
end_ifndef

begin_comment
comment|/* Inside GNU libc we mark some function in a special way.  In other    environments simply ignore the marking.  */
end_comment

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

begin_comment
comment|/* Match STRING against the filename pattern PATTERN, returning zero if    it matches, nonzero if not.  */
end_comment

begin_decl_stmt
specifier|static
name|int
name|internal_fnmatch
name|__P
argument_list|(
operator|(
specifier|const
name|char
operator|*
name|pattern
operator|,
specifier|const
name|char
operator|*
name|string
operator|,
name|int
name|no_leading_period
operator|,
name|int
name|flags
operator|)
argument_list|)
name|internal_function
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|int
name|internal_function
DECL|function|internal_fnmatch
name|internal_fnmatch
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|int
name|no_leading_period
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
specifier|register
specifier|const
name|char
modifier|*
name|p
init|=
name|pattern
decl_stmt|,
modifier|*
name|n
init|=
name|string
decl_stmt|;
specifier|register
name|unsigned
name|char
name|c
decl_stmt|;
comment|/* Note that this evaluates C many times.  */
ifdef|#
directive|ifdef
name|_LIBC
DECL|macro|FOLD
define|#
directive|define
name|FOLD
parameter_list|(
name|c
parameter_list|)
value|((flags& FNM_CASEFOLD) ? tolower (c) : (c))
else|#
directive|else
define|#
directive|define
name|FOLD
parameter_list|(
name|c
parameter_list|)
value|((flags& FNM_CASEFOLD)&& ISUPPER (c) ? tolower (c) : (c))
endif|#
directive|endif
while|while
condition|(
operator|(
name|c
operator|=
operator|*
name|p
operator|++
operator|)
operator|!=
literal|'\0'
condition|)
block|{
name|c
operator|=
name|FOLD
argument_list|(
name|c
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|c
condition|)
block|{
case|case
literal|'?'
case|:
if|if
condition|(
operator|*
name|n
operator|==
literal|'\0'
condition|)
return|return
name|FNM_NOMATCH
return|;
elseif|else
if|if
condition|(
operator|*
name|n
operator|==
literal|'/'
operator|&&
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
condition|)
return|return
name|FNM_NOMATCH
return|;
elseif|else
if|if
condition|(
operator|*
name|n
operator|==
literal|'.'
operator|&&
name|no_leading_period
operator|&&
operator|(
name|n
operator|==
name|string
operator|||
operator|(
name|n
index|[
operator|-
literal|1
index|]
operator|==
literal|'/'
operator|&&
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
operator|)
operator|)
condition|)
return|return
name|FNM_NOMATCH
return|;
break|break;
case|case
literal|'\\'
case|:
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|FNM_NOESCAPE
operator|)
condition|)
block|{
name|c
operator|=
operator|*
name|p
operator|++
expr_stmt|;
if|if
condition|(
name|c
operator|==
literal|'\0'
condition|)
comment|/* Trailing \ loses.  */
return|return
name|FNM_NOMATCH
return|;
name|c
operator|=
name|FOLD
argument_list|(
name|c
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|FOLD
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|!=
name|c
condition|)
return|return
name|FNM_NOMATCH
return|;
break|break;
case|case
literal|'*'
case|:
if|if
condition|(
operator|*
name|n
operator|==
literal|'.'
operator|&&
name|no_leading_period
operator|&&
operator|(
name|n
operator|==
name|string
operator|||
operator|(
name|n
index|[
operator|-
literal|1
index|]
operator|==
literal|'/'
operator|&&
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
operator|)
operator|)
condition|)
return|return
name|FNM_NOMATCH
return|;
for|for
control|(
name|c
operator|=
operator|*
name|p
operator|++
init|;
name|c
operator|==
literal|'?'
operator|||
name|c
operator|==
literal|'*'
condition|;
name|c
operator|=
operator|*
name|p
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|n
operator|==
literal|'/'
operator|&&
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
condition|)
comment|/* A slash does not match a wildcard under FNM_FILE_NAME.  */
return|return
name|FNM_NOMATCH
return|;
elseif|else
if|if
condition|(
name|c
operator|==
literal|'?'
condition|)
block|{
comment|/* A ? needs to match one character.  */
if|if
condition|(
operator|*
name|n
operator|==
literal|'\0'
condition|)
comment|/* There isn't another character; no match.  */
return|return
name|FNM_NOMATCH
return|;
else|else
comment|/* One character of the string is consumed in matching 		       this ? wildcard, so *??? won't match if there are 		       less than three characters.  */
operator|++
name|n
expr_stmt|;
block|}
block|}
if|if
condition|(
name|c
operator|==
literal|'\0'
condition|)
comment|/* The wildcard(s) is/are the last element of the pattern. 	       If the name is a file name and contains another slash 	       this does mean it cannot match.  */
return|return
operator|(
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
operator|&&
name|strchr
argument_list|(
name|n
argument_list|,
literal|'/'
argument_list|)
operator|!=
name|NULL
condition|?
name|FNM_NOMATCH
else|:
literal|0
operator|)
return|;
else|else
block|{
specifier|const
name|char
modifier|*
name|endp
decl_stmt|;
name|endp
operator|=
name|__strchrnul
argument_list|(
name|n
argument_list|,
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
condition|?
literal|'/'
else|:
literal|'\0'
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|==
literal|'['
condition|)
block|{
name|int
name|flags2
init|=
operator|(
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
condition|?
name|flags
else|:
operator|(
name|flags
operator|&
operator|~
name|FNM_PERIOD
operator|)
operator|)
decl_stmt|;
for|for
control|(
operator|--
name|p
init|;
name|n
operator|<
name|endp
condition|;
operator|++
name|n
control|)
if|if
condition|(
name|internal_fnmatch
argument_list|(
name|p
argument_list|,
name|n
argument_list|,
operator|(
name|no_leading_period
operator|&&
operator|(
name|n
operator|==
name|string
operator|||
operator|(
name|n
index|[
operator|-
literal|1
index|]
operator|==
literal|'/'
operator|&&
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
operator|)
operator|)
operator|)
argument_list|,
name|flags2
argument_list|)
operator|==
literal|0
condition|)
return|return
literal|0
return|;
block|}
elseif|else
if|if
condition|(
name|c
operator|==
literal|'/'
operator|&&
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
condition|)
block|{
while|while
condition|(
operator|*
name|n
operator|!=
literal|'\0'
operator|&&
operator|*
name|n
operator|!=
literal|'/'
condition|)
operator|++
name|n
expr_stmt|;
if|if
condition|(
operator|*
name|n
operator|==
literal|'/'
operator|&&
operator|(
name|internal_fnmatch
argument_list|(
name|p
argument_list|,
name|n
operator|+
literal|1
argument_list|,
name|flags
operator|&
name|FNM_PERIOD
argument_list|,
name|flags
argument_list|)
operator|==
literal|0
operator|)
condition|)
return|return
literal|0
return|;
block|}
else|else
block|{
name|int
name|flags2
init|=
operator|(
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
condition|?
name|flags
else|:
operator|(
name|flags
operator|&
operator|~
name|FNM_PERIOD
operator|)
operator|)
decl_stmt|;
if|if
condition|(
name|c
operator|==
literal|'\\'
operator|&&
operator|!
operator|(
name|flags
operator|&
name|FNM_NOESCAPE
operator|)
condition|)
name|c
operator|=
operator|*
name|p
expr_stmt|;
name|c
operator|=
name|FOLD
argument_list|(
name|c
argument_list|)
expr_stmt|;
for|for
control|(
operator|--
name|p
init|;
name|n
operator|<
name|endp
condition|;
operator|++
name|n
control|)
if|if
condition|(
name|FOLD
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|==
name|c
operator|&&
operator|(
name|internal_fnmatch
argument_list|(
name|p
argument_list|,
name|n
argument_list|,
operator|(
name|no_leading_period
operator|&&
operator|(
name|n
operator|==
name|string
operator|||
operator|(
name|n
index|[
operator|-
literal|1
index|]
operator|==
literal|'/'
operator|&&
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
operator|)
operator|)
operator|)
argument_list|,
name|flags2
argument_list|)
operator|==
literal|0
operator|)
condition|)
return|return
literal|0
return|;
block|}
block|}
comment|/* If we come here no match is possible with the wildcard.  */
return|return
name|FNM_NOMATCH
return|;
case|case
literal|'['
case|:
block|{
comment|/* Nonzero if the sense of the character class is inverted.  */
specifier|static
name|int
name|posixly_correct
decl_stmt|;
specifier|register
name|int
name|not
decl_stmt|;
name|char
name|cold
decl_stmt|;
if|if
condition|(
name|posixly_correct
operator|==
literal|0
condition|)
name|posixly_correct
operator|=
name|getenv
argument_list|(
literal|"POSIXLY_CORRECT"
argument_list|)
operator|!=
name|NULL
condition|?
literal|1
else|:
operator|-
literal|1
expr_stmt|;
if|if
condition|(
operator|*
name|n
operator|==
literal|'\0'
condition|)
return|return
name|FNM_NOMATCH
return|;
if|if
condition|(
operator|*
name|n
operator|==
literal|'.'
operator|&&
name|no_leading_period
operator|&&
operator|(
name|n
operator|==
name|string
operator|||
operator|(
name|n
index|[
operator|-
literal|1
index|]
operator|==
literal|'/'
operator|&&
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
operator|)
operator|)
condition|)
return|return
name|FNM_NOMATCH
return|;
if|if
condition|(
operator|*
name|n
operator|==
literal|'/'
operator|&&
operator|(
name|flags
operator|&
name|FNM_FILE_NAME
operator|)
condition|)
comment|/* `/' cannot be matched.  */
return|return
name|FNM_NOMATCH
return|;
name|not
operator|=
operator|(
operator|*
name|p
operator|==
literal|'!'
operator|||
operator|(
name|posixly_correct
operator|<
literal|0
operator|&&
operator|*
name|p
operator|==
literal|'^'
operator|)
operator|)
expr_stmt|;
if|if
condition|(
name|not
condition|)
operator|++
name|p
expr_stmt|;
name|c
operator|=
operator|*
name|p
operator|++
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|unsigned
name|char
name|fn
init|=
name|FOLD
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|FNM_NOESCAPE
operator|)
operator|&&
name|c
operator|==
literal|'\\'
condition|)
block|{
if|if
condition|(
operator|*
name|p
operator|==
literal|'\0'
condition|)
return|return
name|FNM_NOMATCH
return|;
name|c
operator|=
name|FOLD
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|p
argument_list|)
expr_stmt|;
operator|++
name|p
expr_stmt|;
if|if
condition|(
name|c
operator|==
name|fn
condition|)
goto|goto
name|matched
goto|;
block|}
elseif|else
if|if
condition|(
name|c
operator|==
literal|'['
operator|&&
operator|*
name|p
operator|==
literal|':'
condition|)
block|{
comment|/* Leave room for the null.  */
name|char
name|str
index|[
name|CHAR_CLASS_MAX_LENGTH
operator|+
literal|1
index|]
decl_stmt|;
name|size_t
name|c1
init|=
literal|0
decl_stmt|;
if|#
directive|if
name|defined
name|_LIBC
operator|||
operator|(
name|defined
name|HAVE_WCTYPE_H
operator|&&
name|defined
name|HAVE_WCHAR_H
operator|)
name|wctype_t
name|wt
decl_stmt|;
endif|#
directive|endif
specifier|const
name|char
modifier|*
name|startp
init|=
name|p
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|c1
operator|>
name|CHAR_CLASS_MAX_LENGTH
condition|)
comment|/* The name is too long and therefore the pattern 			     is ill-formed.  */
return|return
name|FNM_NOMATCH
return|;
name|c
operator|=
operator|*
operator|++
name|p
expr_stmt|;
if|if
condition|(
name|c
operator|==
literal|':'
operator|&&
name|p
index|[
literal|1
index|]
operator|==
literal|']'
condition|)
block|{
name|p
operator|+=
literal|2
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|c
operator|<
literal|'a'
operator|||
name|c
operator|>=
literal|'z'
condition|)
block|{
comment|/* This cannot possibly be a character class name. 			       Match it as a normal range.  */
name|p
operator|=
name|startp
expr_stmt|;
name|c
operator|=
literal|'['
expr_stmt|;
goto|goto
name|normal_bracket
goto|;
block|}
name|str
index|[
name|c1
operator|++
index|]
operator|=
name|c
expr_stmt|;
block|}
name|str
index|[
name|c1
index|]
operator|=
literal|'\0'
expr_stmt|;
if|#
directive|if
name|defined
name|_LIBC
operator|||
operator|(
name|defined
name|HAVE_WCTYPE_H
operator|&&
name|defined
name|HAVE_WCHAR_H
operator|)
name|wt
operator|=
name|IS_CHAR_CLASS
argument_list|(
name|str
argument_list|)
expr_stmt|;
if|if
condition|(
name|wt
operator|==
literal|0
condition|)
comment|/* Invalid character class name.  */
return|return
name|FNM_NOMATCH
return|;
if|if
condition|(
name|__iswctype
argument_list|(
name|__btowc
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
argument_list|,
name|wt
argument_list|)
condition|)
goto|goto
name|matched
goto|;
else|#
directive|else
if|if
condition|(
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"alnum"
argument_list|)
operator|&&
name|ISALNUM
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"alpha"
argument_list|)
operator|&&
name|ISALPHA
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"blank"
argument_list|)
operator|&&
name|ISBLANK
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"cntrl"
argument_list|)
operator|&&
name|ISCNTRL
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"digit"
argument_list|)
operator|&&
name|ISDIGIT
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"graph"
argument_list|)
operator|&&
name|ISGRAPH
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"lower"
argument_list|)
operator|&&
name|ISLOWER
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"print"
argument_list|)
operator|&&
name|ISPRINT
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"punct"
argument_list|)
operator|&&
name|ISPUNCT
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"space"
argument_list|)
operator|&&
name|ISSPACE
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"upper"
argument_list|)
operator|&&
name|ISUPPER
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
operator|||
operator|(
name|STREQ
argument_list|(
name|str
argument_list|,
literal|"xdigit"
argument_list|)
operator|&&
name|ISXDIGIT
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
operator|)
condition|)
goto|goto
name|matched
goto|;
endif|#
directive|endif
block|}
elseif|else
if|if
condition|(
name|c
operator|==
literal|'\0'
condition|)
comment|/* [ (unterminated) loses.  */
return|return
name|FNM_NOMATCH
return|;
else|else
block|{
name|normal_bracket
label|:
if|if
condition|(
name|FOLD
argument_list|(
name|c
argument_list|)
operator|==
name|fn
condition|)
goto|goto
name|matched
goto|;
name|cold
operator|=
name|c
expr_stmt|;
name|c
operator|=
operator|*
name|p
operator|++
expr_stmt|;
if|if
condition|(
name|c
operator|==
literal|'-'
operator|&&
operator|*
name|p
operator|!=
literal|']'
condition|)
block|{
comment|/* It is a range.  */
name|unsigned
name|char
name|cend
init|=
operator|*
name|p
operator|++
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|FNM_NOESCAPE
operator|)
operator|&&
name|cend
operator|==
literal|'\\'
condition|)
name|cend
operator|=
operator|*
name|p
operator|++
expr_stmt|;
if|if
condition|(
name|cend
operator|==
literal|'\0'
condition|)
return|return
name|FNM_NOMATCH
return|;
if|if
condition|(
name|cold
operator|<=
name|fn
operator|&&
name|fn
operator|<=
name|FOLD
argument_list|(
name|cend
argument_list|)
condition|)
goto|goto
name|matched
goto|;
name|c
operator|=
operator|*
name|p
operator|++
expr_stmt|;
block|}
block|}
if|if
condition|(
name|c
operator|==
literal|']'
condition|)
break|break;
block|}
if|if
condition|(
operator|!
name|not
condition|)
return|return
name|FNM_NOMATCH
return|;
break|break;
name|matched
label|:
comment|/* Skip the rest of the [...] that already matched.  */
while|while
condition|(
name|c
operator|!=
literal|']'
condition|)
block|{
if|if
condition|(
name|c
operator|==
literal|'\0'
condition|)
comment|/* [... (unterminated) loses.  */
return|return
name|FNM_NOMATCH
return|;
name|c
operator|=
operator|*
name|p
operator|++
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|flags
operator|&
name|FNM_NOESCAPE
operator|)
operator|&&
name|c
operator|==
literal|'\\'
condition|)
block|{
if|if
condition|(
operator|*
name|p
operator|==
literal|'\0'
condition|)
return|return
name|FNM_NOMATCH
return|;
comment|/* XXX 1003.2d11 is unclear if this is right.  */
operator|++
name|p
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|c
operator|==
literal|'['
operator|&&
operator|*
name|p
operator|==
literal|':'
condition|)
block|{
do|do
if|if
condition|(
operator|*
operator|++
name|p
operator|==
literal|'\0'
condition|)
return|return
name|FNM_NOMATCH
return|;
do|while
condition|(
operator|*
name|p
operator|!=
literal|':'
operator|||
name|p
index|[
literal|1
index|]
operator|==
literal|']'
condition|)
do|;
name|p
operator|+=
literal|2
expr_stmt|;
name|c
operator|=
operator|*
name|p
expr_stmt|;
block|}
block|}
if|if
condition|(
name|not
condition|)
return|return
name|FNM_NOMATCH
return|;
block|}
break|break;
default|default:
if|if
condition|(
name|c
operator|!=
name|FOLD
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|n
argument_list|)
condition|)
return|return
name|FNM_NOMATCH
return|;
block|}
operator|++
name|n
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|n
operator|==
literal|'\0'
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|(
name|flags
operator|&
name|FNM_LEADING_DIR
operator|)
operator|&&
operator|*
name|n
operator|==
literal|'/'
condition|)
comment|/* The FNM_LEADING_DIR flag says that "foo*" matches "foobar/frobozz".  */
return|return
literal|0
return|;
return|return
name|FNM_NOMATCH
return|;
DECL|macro|FOLD
undef|#
directive|undef
name|FOLD
block|}
end_function

begin_function
name|int
DECL|function|fnmatch
name|fnmatch
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|string
parameter_list|,
name|int
name|flags
parameter_list|)
block|{
return|return
name|internal_fnmatch
argument_list|(
name|pattern
argument_list|,
name|string
argument_list|,
name|flags
operator|&
name|FNM_PERIOD
argument_list|,
name|flags
argument_list|)
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _LIBC or not __GNU_LIBRARY__.  */
end_comment

end_unit

