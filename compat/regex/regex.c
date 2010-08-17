begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/* Extended regular expression matching and search library.    Copyright (C) 2002, 2003, 2005 Free Software Foundation, Inc.    This file is part of the GNU C Library.    Contributed by Isamu Hasegawa<isamu@yamato.ibm.com>.     The GNU C Library is free software; you can redistribute it and/or    modify it under the terms of the GNU Lesser General Public    License as published by the Free Software Foundation; either    version 2.1 of the License, or (at your option) any later version.     The GNU C Library is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    Lesser General Public License for more details.     You should have received a copy of the GNU Lesser General Public    License along with the GNU C Library; if not, write to the Free    Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA    02110-1301 USA.  */
end_comment

begin_ifdef
ifdef|#
directive|ifdef
name|HAVE_CONFIG_H
end_ifdef

begin_include
include|#
directive|include
file|"config.h"
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* Make sure noone compiles this code with a C++ compiler.  */
end_comment

begin_ifdef
ifdef|#
directive|ifdef
name|__cplusplus
end_ifdef

begin_error
error|#
directive|error
literal|"This is C code, use a C compiler"
end_error

begin_endif
endif|#
directive|endif
end_endif

begin_ifdef
ifdef|#
directive|ifdef
name|_LIBC
end_ifdef

begin_comment
comment|/* We have to keep the namespace clean.  */
end_comment

begin_define
DECL|macro|regfree
define|#
directive|define
name|regfree
parameter_list|(
name|preg
parameter_list|)
value|__regfree (preg)
end_define

begin_define
DECL|macro|regexec
define|#
directive|define
name|regexec
parameter_list|(
name|pr
parameter_list|,
name|st
parameter_list|,
name|nm
parameter_list|,
name|pm
parameter_list|,
name|ef
parameter_list|)
value|__regexec (pr, st, nm, pm, ef)
end_define

begin_define
DECL|macro|regcomp
define|#
directive|define
name|regcomp
parameter_list|(
name|preg
parameter_list|,
name|pattern
parameter_list|,
name|cflags
parameter_list|)
value|__regcomp (preg, pattern, cflags)
end_define

begin_define
DECL|macro|regerror
define|#
directive|define
name|regerror
parameter_list|(
name|errcode
parameter_list|,
name|preg
parameter_list|,
name|errbuf
parameter_list|,
name|errbuf_size
parameter_list|)
define|\
value|__regerror(errcode, preg, errbuf, errbuf_size)
end_define

begin_define
DECL|macro|re_set_registers
define|#
directive|define
name|re_set_registers
parameter_list|(
name|bu
parameter_list|,
name|re
parameter_list|,
name|nu
parameter_list|,
name|st
parameter_list|,
name|en
parameter_list|)
define|\
value|__re_set_registers (bu, re, nu, st, en)
end_define

begin_define
DECL|macro|re_match_2
define|#
directive|define
name|re_match_2
parameter_list|(
name|bufp
parameter_list|,
name|string1
parameter_list|,
name|size1
parameter_list|,
name|string2
parameter_list|,
name|size2
parameter_list|,
name|pos
parameter_list|,
name|regs
parameter_list|,
name|stop
parameter_list|)
define|\
value|__re_match_2 (bufp, string1, size1, string2, size2, pos, regs, stop)
end_define

begin_define
DECL|macro|re_match
define|#
directive|define
name|re_match
parameter_list|(
name|bufp
parameter_list|,
name|string
parameter_list|,
name|size
parameter_list|,
name|pos
parameter_list|,
name|regs
parameter_list|)
define|\
value|__re_match (bufp, string, size, pos, regs)
end_define

begin_define
DECL|macro|re_search
define|#
directive|define
name|re_search
parameter_list|(
name|bufp
parameter_list|,
name|string
parameter_list|,
name|size
parameter_list|,
name|startpos
parameter_list|,
name|range
parameter_list|,
name|regs
parameter_list|)
define|\
value|__re_search (bufp, string, size, startpos, range, regs)
end_define

begin_define
DECL|macro|re_compile_pattern
define|#
directive|define
name|re_compile_pattern
parameter_list|(
name|pattern
parameter_list|,
name|length
parameter_list|,
name|bufp
parameter_list|)
define|\
value|__re_compile_pattern (pattern, length, bufp)
end_define

begin_define
DECL|macro|re_set_syntax
define|#
directive|define
name|re_set_syntax
parameter_list|(
name|syntax
parameter_list|)
value|__re_set_syntax (syntax)
end_define

begin_define
DECL|macro|re_search_2
define|#
directive|define
name|re_search_2
parameter_list|(
name|bufp
parameter_list|,
name|st1
parameter_list|,
name|s1
parameter_list|,
name|st2
parameter_list|,
name|s2
parameter_list|,
name|startpos
parameter_list|,
name|range
parameter_list|,
name|regs
parameter_list|,
name|stop
parameter_list|)
define|\
value|__re_search_2 (bufp, st1, s1, st2, s2, startpos, range, regs, stop)
end_define

begin_define
DECL|macro|re_compile_fastmap
define|#
directive|define
name|re_compile_fastmap
parameter_list|(
name|bufp
parameter_list|)
value|__re_compile_fastmap (bufp)
end_define

begin_include
include|#
directive|include
file|"../locale/localeinfo.h"
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|defined
argument_list|(
name|_MSC_VER
argument_list|)
end_if

begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_comment
comment|/* for size_t */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* On some systems, limits.h sets RE_DUP_MAX to a lower value than    GNU regex allows.  Include it before<regex.h>, which correctly    #undefs RE_DUP_MAX and sets it to the right value.  */
end_comment

begin_include
include|#
directive|include
file|<limits.h>
end_include

begin_ifdef
ifdef|#
directive|ifdef
name|GAWK
end_ifdef

begin_undef
DECL|macro|alloca
undef|#
directive|undef
name|alloca
end_undef

begin_define
DECL|macro|alloca
define|#
directive|define
name|alloca
value|alloca_is_bad_you_should_never_use_it
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_include
include|#
directive|include
file|<regex.h>
end_include

begin_include
include|#
directive|include
file|"regex_internal.h"
end_include

begin_include
include|#
directive|include
file|"regex_internal.c"
end_include

begin_ifdef
ifdef|#
directive|ifdef
name|GAWK
end_ifdef

begin_define
DECL|macro|bool
define|#
directive|define
name|bool
value|int
end_define

begin_define
DECL|macro|true
define|#
directive|define
name|true
value|(1)
end_define

begin_define
DECL|macro|false
define|#
directive|define
name|false
value|(0)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_include
include|#
directive|include
file|"regcomp.c"
end_include

begin_include
include|#
directive|include
file|"regexec.c"
end_include

begin_comment
comment|/* Binary backward compatibility.  */
end_comment

begin_if
if|#
directive|if
name|_LIBC
end_if

begin_include
include|#
directive|include
file|<shlib-compat.h>
end_include

begin_if
if|#
directive|if
name|SHLIB_COMPAT
argument_list|(
name|libc
operator|,
name|GLIBC_2_0
operator|,
name|GLIBC_2_3
argument_list|)
end_if

begin_macro
name|link_warning
argument_list|(
argument|re_max_failures
argument_list|,
literal|"the 're_max_failures' variable is obsolete and will go away."
argument_list|)
end_macro

begin_decl_stmt
DECL|variable|re_max_failures
name|int
name|re_max_failures
init|=
literal|2000
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

end_unit

