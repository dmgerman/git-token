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
name|XEMIT_H
argument_list|)
end_if

begin_define
DECL|macro|XEMIT_H
define|#
directive|define
name|XEMIT_H
end_define

begin_typedef
DECL|typedef|emit_func_t
typedef|typedef
name|int
function_decl|(
modifier|*
name|emit_func_t
function_decl|)
parameter_list|(
name|xdfenv_t
modifier|*
name|xe
parameter_list|,
name|xdchange_t
modifier|*
name|xscr
parameter_list|,
name|xdemitcb_t
modifier|*
name|ecb
parameter_list|,
name|xdemitconf_t
specifier|const
modifier|*
name|xecfg
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|xdchange_t
modifier|*
name|xdl_get_hunk
parameter_list|(
name|xdchange_t
modifier|*
modifier|*
name|xscr
parameter_list|,
name|xdemitconf_t
specifier|const
modifier|*
name|xecfg
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|xdl_emit_diff
parameter_list|(
name|xdfenv_t
modifier|*
name|xe
parameter_list|,
name|xdchange_t
modifier|*
name|xscr
parameter_list|,
name|xdemitcb_t
modifier|*
name|ecb
parameter_list|,
name|xdemitconf_t
specifier|const
modifier|*
name|xecfg
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* #if !defined(XEMIT_H) */
end_comment

end_unit

