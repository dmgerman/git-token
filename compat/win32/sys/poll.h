begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/* Header for poll(2) emulation    Contributed by Paolo Bonzini.     Copyright 2001, 2002, 2003, 2007, 2009, 2010 Free Software Foundation, Inc.     This file is part of gnulib.     This program is free software; you can redistribute it and/or modify    it under the terms of the GNU General Public License as published by    the Free Software Foundation; either version 2, or (at your option)    any later version.     This program is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    GNU General Public License for more details.     You should have received a copy of the GNU General Public License along    with this program; if not, write to the Free Software Foundation,    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_GL_POLL_H
end_ifndef

begin_define
DECL|macro|_GL_POLL_H
define|#
directive|define
name|_GL_POLL_H
end_define

begin_comment
comment|/* fake a poll(2) environment */
end_comment

begin_define
DECL|macro|POLLIN
define|#
directive|define
name|POLLIN
value|0x0001
end_define

begin_comment
DECL|macro|POLLIN
comment|/* any readable data available   */
end_comment

begin_define
DECL|macro|POLLPRI
define|#
directive|define
name|POLLPRI
value|0x0002
end_define

begin_comment
DECL|macro|POLLPRI
comment|/* OOB/Urgent readable data      */
end_comment

begin_define
DECL|macro|POLLOUT
define|#
directive|define
name|POLLOUT
value|0x0004
end_define

begin_comment
DECL|macro|POLLOUT
comment|/* file descriptor is writeable  */
end_comment

begin_define
DECL|macro|POLLERR
define|#
directive|define
name|POLLERR
value|0x0008
end_define

begin_comment
DECL|macro|POLLERR
comment|/* some poll error occurred      */
end_comment

begin_define
DECL|macro|POLLHUP
define|#
directive|define
name|POLLHUP
value|0x0010
end_define

begin_comment
DECL|macro|POLLHUP
comment|/* file descriptor was "hung up" */
end_comment

begin_define
DECL|macro|POLLNVAL
define|#
directive|define
name|POLLNVAL
value|0x0020
end_define

begin_comment
DECL|macro|POLLNVAL
comment|/* requested events "invalid"    */
end_comment

begin_define
DECL|macro|POLLRDNORM
define|#
directive|define
name|POLLRDNORM
value|0x0040
end_define

begin_define
DECL|macro|POLLRDBAND
define|#
directive|define
name|POLLRDBAND
value|0x0080
end_define

begin_define
DECL|macro|POLLWRNORM
define|#
directive|define
name|POLLWRNORM
value|0x0100
end_define

begin_define
DECL|macro|POLLWRBAND
define|#
directive|define
name|POLLWRBAND
value|0x0200
end_define

begin_struct
DECL|struct|pollfd
struct|struct
name|pollfd
block|{
DECL|member|fd
name|int
name|fd
decl_stmt|;
comment|/* which file descriptor to poll */
DECL|member|events
name|short
name|events
decl_stmt|;
comment|/* events we are interested in   */
DECL|member|revents
name|short
name|revents
decl_stmt|;
comment|/* events found on return        */
block|}
struct|;
end_struct

begin_typedef
DECL|typedef|nfds_t
typedef|typedef
name|unsigned
name|long
name|nfds_t
typedef|;
end_typedef

begin_function_decl
specifier|extern
name|int
name|poll
parameter_list|(
name|struct
name|pollfd
modifier|*
name|pfd
parameter_list|,
name|nfds_t
name|nfd
parameter_list|,
name|int
name|timeout
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Define INFTIM only if doing so conforms to POSIX.  */
end_comment

begin_if
if|#
directive|if
operator|!
name|defined
argument_list|(
name|_POSIX_C_SOURCE
argument_list|)
operator|&&
operator|!
name|defined
argument_list|(
name|_XOPEN_SOURCE
argument_list|)
end_if

begin_define
DECL|macro|INFTIM
define|#
directive|define
name|INFTIM
value|(-1)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _GL_POLL_H */
end_comment

end_unit

