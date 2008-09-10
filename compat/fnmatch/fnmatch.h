begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/* Copyright (C) 1991, 92, 93, 96, 97, 98, 99 Free Software Foundation, Inc.    This file is part of the GNU C Library.     The GNU C Library is free software; you can redistribute it and/or    modify it under the terms of the GNU Library General Public License as    published by the Free Software Foundation; either version 2 of the    License, or (at your option) any later version.     The GNU C Library is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    Library General Public License for more details.     You should have received a copy of the GNU Library General Public    License along with the GNU C Library; see the file COPYING.LIB.  If not,    write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,    Boston, MA 02111-1307, USA.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_FNMATCH_H
end_ifndef

begin_define
DECL|macro|_FNMATCH_H
define|#
directive|define
name|_FNMATCH_H
value|1
end_define

begin_ifdef
ifdef|#
directive|ifdef
name|__cplusplus
end_ifdef

begin_extern
extern|extern
literal|"C"
block|{
endif|#
directive|endif
if|#
directive|if
name|defined
name|__cplusplus
operator|||
operator|(
name|defined
name|__STDC__
operator|&&
name|__STDC__
operator|)
operator|||
name|defined
name|WINDOWS32
if|#
directive|if
operator|!
name|defined
name|__GLIBC__
operator|||
operator|!
name|defined
name|__P
DECL|macro|__P
undef|#
directive|undef
name|__P
DECL|macro|__P
define|#
directive|define
name|__P
parameter_list|(
name|protos
parameter_list|)
value|protos
endif|#
directive|endif
else|#
directive|else
comment|/* Not C++ or ANSI C.  */
DECL|macro|__P
undef|#
directive|undef
name|__P
DECL|macro|__P
define|#
directive|define
name|__P
parameter_list|(
name|protos
parameter_list|)
value|()
comment|/* We can get away without defining `const' here only because in this file    it is used only inside the prototype for `fnmatch', which is elided in    non-ANSI C where `const' is problematical.  */
endif|#
directive|endif
comment|/* C++ or ANSI C.  */
ifndef|#
directive|ifndef
name|const
if|#
directive|if
operator|(
name|defined
name|__STDC__
operator|&&
name|__STDC__
operator|)
operator|||
name|defined
name|__cplusplus
DECL|macro|__const
define|#
directive|define
name|__const
value|const
else|#
directive|else
DECL|macro|__const
define|#
directive|define
name|__const
endif|#
directive|endif
endif|#
directive|endif
comment|/* We #undef these before defining them because some losing systems    (HP-UX A.08.07 for example) define these in<unistd.h>.  */
DECL|macro|FNM_PATHNAME
undef|#
directive|undef
name|FNM_PATHNAME
DECL|macro|FNM_NOESCAPE
undef|#
directive|undef
name|FNM_NOESCAPE
DECL|macro|FNM_PERIOD
undef|#
directive|undef
name|FNM_PERIOD
comment|/* Bits set in the FLAGS argument to `fnmatch'.  */
DECL|macro|FNM_PATHNAME
define|#
directive|define
name|FNM_PATHNAME
value|(1<< 0)
comment|/* No wildcard can ever match `/'.  */
DECL|macro|FNM_NOESCAPE
define|#
directive|define
name|FNM_NOESCAPE
value|(1<< 1)
comment|/* Backslashes don't quote special chars.  */
DECL|macro|FNM_PERIOD
define|#
directive|define
name|FNM_PERIOD
value|(1<< 2)
comment|/* Leading `.' is matched only explicitly.  */
if|#
directive|if
operator|!
name|defined
name|_POSIX_C_SOURCE
operator|||
name|_POSIX_C_SOURCE
operator|<
literal|2
operator|||
name|defined
name|_GNU_SOURCE
DECL|macro|FNM_FILE_NAME
define|#
directive|define
name|FNM_FILE_NAME
value|FNM_PATHNAME
comment|/* Preferred GNU name.  */
DECL|macro|FNM_LEADING_DIR
define|#
directive|define
name|FNM_LEADING_DIR
value|(1<< 3)
comment|/* Ignore `/...' after a match.  */
DECL|macro|FNM_CASEFOLD
define|#
directive|define
name|FNM_CASEFOLD
value|(1<< 4)
comment|/* Compare without regard to case.  */
endif|#
directive|endif
comment|/* Value returned by `fnmatch' if STRING does not match PATTERN.  */
DECL|macro|FNM_NOMATCH
define|#
directive|define
name|FNM_NOMATCH
value|1
comment|/* This value is returned if the implementation does not support    `fnmatch'.  Since this is not the case here it will never be    returned but the conformance test suites still require the symbol    to be defined.  */
ifdef|#
directive|ifdef
name|_XOPEN_SOURCE
DECL|macro|FNM_NOSYS
define|#
directive|define
name|FNM_NOSYS
value|(-1)
endif|#
directive|endif
comment|/* Match NAME against the filename pattern PATTERN,    returning zero if it matches, FNM_NOMATCH if not.  */
specifier|extern
name|int
name|fnmatch
name|__P
argument_list|(
operator|(
name|__const
name|char
operator|*
name|__pattern
operator|,
name|__const
name|char
operator|*
name|__name
operator|,
name|int
name|__flags
operator|)
argument_list|)
decl_stmt|;
ifdef|#
directive|ifdef
name|__cplusplus
block|}
end_extern

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* fnmatch.h */
end_comment

end_unit

