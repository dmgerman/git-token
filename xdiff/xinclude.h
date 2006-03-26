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
name|XINCLUDE_H
argument_list|)
end_if

begin_define
DECL|macro|XINCLUDE_H
define|#
directive|define
name|XINCLUDE_H
end_define

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
file|<unistd.h>
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_include
include|#
directive|include
file|<limits.h>
end_include

begin_include
include|#
directive|include
file|"xmacros.h"
end_include

begin_include
include|#
directive|include
file|"xdiff.h"
end_include

begin_include
include|#
directive|include
file|"xtypes.h"
end_include

begin_include
include|#
directive|include
file|"xutils.h"
end_include

begin_include
include|#
directive|include
file|"xprepare.h"
end_include

begin_include
include|#
directive|include
file|"xdiffi.h"
end_include

begin_include
include|#
directive|include
file|"xemit.h"
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* #if !defined(XINCLUDE_H) */
end_comment

end_unit

