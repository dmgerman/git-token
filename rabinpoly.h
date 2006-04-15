begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  *  * Copyright (C) 2000 David Mazieres (dm@uun.org)  *  * This program is free software; you can redistribute it and/or  * modify it under the terms of the GNU General Public License as  * published by the Free Software Foundation; either version 2, or (at  * your option) any later version.  *  * This program is distributed in the hope that it will be useful, but  * WITHOUT ANY WARRANTY; without even the implied warranty of  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  * General Public License for more details.  *  * You should have received a copy of the GNU General Public License  * along with this program; if not, write to the Free Software  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  * USA  *  * Translated to C and simplified by Geert Bosch (bosch@gnat.com)  */
end_comment

begin_include
include|#
directive|include
file|<assert.h>
end_include

begin_include
include|#
directive|include
file|<strings.h>
end_include

begin_include
include|#
directive|include
file|<sys/types.h>
end_include

begin_ifndef
ifndef|#
directive|ifndef
name|RABIN_WINDOW_SIZE
end_ifndef

begin_define
DECL|macro|RABIN_WINDOW_SIZE
define|#
directive|define
name|RABIN_WINDOW_SIZE
value|48
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
name|void
name|rabin_reset
parameter_list|()
function_decl|;
end_function_decl

begin_function_decl
name|u_int64_t
name|rabin_slide8
parameter_list|(
name|u_char
name|c
parameter_list|)
function_decl|;
end_function_decl

end_unit

