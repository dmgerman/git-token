begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  *  LibXDiff by Davide Libenzi ( File Differential Library )  *  Copyright (C) 2003  Davide Libenzi  *  *  This library is free software; you can redistribute it and/or  *  modify it under the terms of the GNU Lesser General Public  *  License as published by the Free Software Foundation; either  *  version 2.1 of the License, or (at your option) any later version.  *  *  This library is distributed in the hope that it will be useful,  *  but WITHOUT ANY WARRANTY; without even the implied warranty of  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *  Lesser General Public License for more details.  *  *  You should have received a copy of the GNU Lesser General Public  *  License along with this library; if not, write to the Free Software  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *  *  Davide Libenzi<davidel@xmailserver.org>  *  */
end_comment

begin_if
if|#
directive|if
operator|!
name|defined
argument_list|(
name|XMACROS_H
argument_list|)
end_if

begin_define
DECL|macro|XMACROS_H
define|#
directive|define
name|XMACROS_H
end_define

begin_define
DECL|macro|GR_PRIME
define|#
directive|define
name|GR_PRIME
value|0x9e370001UL
end_define

begin_define
DECL|macro|XDL_MIN
define|#
directive|define
name|XDL_MIN
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
value|((a)< (b) ? (a): (b))
end_define

begin_define
DECL|macro|XDL_MAX
define|#
directive|define
name|XDL_MAX
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
value|((a)> (b) ? (a): (b))
end_define

begin_define
DECL|macro|XDL_ABS
define|#
directive|define
name|XDL_ABS
parameter_list|(
name|v
parameter_list|)
value|((v)>= 0 ? (v): -(v))
end_define

begin_define
DECL|macro|XDL_ISDIGIT
define|#
directive|define
name|XDL_ISDIGIT
parameter_list|(
name|c
parameter_list|)
value|((c)>= '0'&& (c)<= '9')
end_define

begin_define
DECL|macro|XDL_HASHLONG
define|#
directive|define
name|XDL_HASHLONG
parameter_list|(
name|v
parameter_list|,
name|b
parameter_list|)
value|(((unsigned long)(v) * GR_PRIME)>> ((CHAR_BIT * sizeof(unsigned long)) - (b)))
end_define

begin_define
DECL|macro|XDL_PTRFREE
define|#
directive|define
name|XDL_PTRFREE
parameter_list|(
name|p
parameter_list|)
value|do { if (p) { xdl_free(p); (p) = NULL; } } while (0)
end_define

begin_define
DECL|macro|XDL_LE32_PUT
define|#
directive|define
name|XDL_LE32_PUT
parameter_list|(
name|p
parameter_list|,
name|v
parameter_list|)
define|\
value|do { \ 	unsigned char *__p = (unsigned char *) (p); \ 	*__p++ = (unsigned char) (v); \ 	*__p++ = (unsigned char) ((v)>> 8); \ 	*__p++ = (unsigned char) ((v)>> 16); \ 	*__p = (unsigned char) ((v)>> 24); \ } while (0)
end_define

begin_define
DECL|macro|XDL_LE32_GET
define|#
directive|define
name|XDL_LE32_GET
parameter_list|(
name|p
parameter_list|,
name|v
parameter_list|)
define|\
value|do { \ 	unsigned char const *__p = (unsigned char const *) (p); \ 	(v) = (unsigned long) __p[0] | ((unsigned long) __p[1])<< 8 | \ 		((unsigned long) __p[2])<< 16 | ((unsigned long) __p[3])<< 24; \ } while (0)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* #if !defined(XMACROS_H) */
end_comment

end_unit

