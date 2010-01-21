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
name|XDIFF_H
argument_list|)
end_if

begin_define
DECL|macro|XDIFF_H
define|#
directive|define
name|XDIFF_H
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
comment|/* #ifdef __cplusplus */
DECL|macro|XDF_NEED_MINIMAL
define|#
directive|define
name|XDF_NEED_MINIMAL
value|(1<< 1)
DECL|macro|XDF_IGNORE_WHITESPACE
define|#
directive|define
name|XDF_IGNORE_WHITESPACE
value|(1<< 2)
DECL|macro|XDF_IGNORE_WHITESPACE_CHANGE
define|#
directive|define
name|XDF_IGNORE_WHITESPACE_CHANGE
value|(1<< 3)
DECL|macro|XDF_IGNORE_WHITESPACE_AT_EOL
define|#
directive|define
name|XDF_IGNORE_WHITESPACE_AT_EOL
value|(1<< 4)
DECL|macro|XDF_PATIENCE_DIFF
define|#
directive|define
name|XDF_PATIENCE_DIFF
value|(1<< 5)
DECL|macro|XDF_WHITESPACE_FLAGS
define|#
directive|define
name|XDF_WHITESPACE_FLAGS
value|(XDF_IGNORE_WHITESPACE | XDF_IGNORE_WHITESPACE_CHANGE | XDF_IGNORE_WHITESPACE_AT_EOL)
DECL|macro|XDL_PATCH_NORMAL
define|#
directive|define
name|XDL_PATCH_NORMAL
value|'-'
DECL|macro|XDL_PATCH_REVERSE
define|#
directive|define
name|XDL_PATCH_REVERSE
value|'+'
DECL|macro|XDL_PATCH_MODEMASK
define|#
directive|define
name|XDL_PATCH_MODEMASK
value|((1<< 8) - 1)
DECL|macro|XDL_PATCH_IGNOREBSPACE
define|#
directive|define
name|XDL_PATCH_IGNOREBSPACE
value|(1<< 8)
DECL|macro|XDL_EMIT_FUNCNAMES
define|#
directive|define
name|XDL_EMIT_FUNCNAMES
value|(1<< 0)
DECL|macro|XDL_EMIT_COMMON
define|#
directive|define
name|XDL_EMIT_COMMON
value|(1<< 1)
DECL|macro|XDL_MMB_READONLY
define|#
directive|define
name|XDL_MMB_READONLY
value|(1<< 0)
DECL|macro|XDL_MMF_ATOMIC
define|#
directive|define
name|XDL_MMF_ATOMIC
value|(1<< 0)
DECL|macro|XDL_BDOP_INS
define|#
directive|define
name|XDL_BDOP_INS
value|1
DECL|macro|XDL_BDOP_CPY
define|#
directive|define
name|XDL_BDOP_CPY
value|2
DECL|macro|XDL_BDOP_INSB
define|#
directive|define
name|XDL_BDOP_INSB
value|3
comment|/* merge simplification levels */
DECL|macro|XDL_MERGE_MINIMAL
define|#
directive|define
name|XDL_MERGE_MINIMAL
value|0
DECL|macro|XDL_MERGE_EAGER
define|#
directive|define
name|XDL_MERGE_EAGER
value|1
DECL|macro|XDL_MERGE_ZEALOUS
define|#
directive|define
name|XDL_MERGE_ZEALOUS
value|2
DECL|macro|XDL_MERGE_ZEALOUS_ALNUM
define|#
directive|define
name|XDL_MERGE_ZEALOUS_ALNUM
value|3
DECL|macro|XDL_MERGE_LEVEL_MASK
define|#
directive|define
name|XDL_MERGE_LEVEL_MASK
value|0x0f
comment|/* merge favor modes */
DECL|macro|XDL_MERGE_FAVOR_OURS
define|#
directive|define
name|XDL_MERGE_FAVOR_OURS
value|1
DECL|macro|XDL_MERGE_FAVOR_THEIRS
define|#
directive|define
name|XDL_MERGE_FAVOR_THEIRS
value|2
DECL|macro|XDL_MERGE_FAVOR
define|#
directive|define
name|XDL_MERGE_FAVOR
parameter_list|(
name|flags
parameter_list|)
value|(((flags)>>4)& 3)
DECL|macro|XDL_MERGE_FLAGS
define|#
directive|define
name|XDL_MERGE_FLAGS
parameter_list|(
name|level
parameter_list|,
name|style
parameter_list|,
name|favor
parameter_list|)
value|((level)|(style)|((favor)<<4))
comment|/* merge output styles */
DECL|macro|XDL_MERGE_DIFF3
define|#
directive|define
name|XDL_MERGE_DIFF3
value|0x8000
DECL|macro|XDL_MERGE_STYLE_MASK
define|#
directive|define
name|XDL_MERGE_STYLE_MASK
value|0x8000
DECL|struct|s_mmfile
typedef|typedef
struct|struct
name|s_mmfile
block|{
DECL|member|ptr
name|char
modifier|*
name|ptr
decl_stmt|;
DECL|member|size
name|long
name|size
decl_stmt|;
DECL|typedef|mmfile_t
block|}
name|mmfile_t
typedef|;
DECL|struct|s_mmbuffer
typedef|typedef
struct|struct
name|s_mmbuffer
block|{
DECL|member|ptr
name|char
modifier|*
name|ptr
decl_stmt|;
DECL|member|size
name|long
name|size
decl_stmt|;
DECL|typedef|mmbuffer_t
block|}
name|mmbuffer_t
typedef|;
DECL|struct|s_xpparam
typedef|typedef
struct|struct
name|s_xpparam
block|{
DECL|member|flags
name|unsigned
name|long
name|flags
decl_stmt|;
DECL|typedef|xpparam_t
block|}
name|xpparam_t
typedef|;
DECL|struct|s_xdemitcb
typedef|typedef
struct|struct
name|s_xdemitcb
block|{
DECL|member|priv
name|void
modifier|*
name|priv
decl_stmt|;
DECL|member|outf
name|int
function_decl|(
modifier|*
name|outf
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|,
name|mmbuffer_t
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
DECL|typedef|xdemitcb_t
block|}
name|xdemitcb_t
typedef|;
DECL|typedef|find_func_t
typedef|typedef
name|long
function_decl|(
modifier|*
name|find_func_t
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
name|line
parameter_list|,
name|long
name|line_len
parameter_list|,
name|char
modifier|*
name|buffer
parameter_list|,
name|long
name|buffer_size
parameter_list|,
name|void
modifier|*
name|priv
parameter_list|)
function_decl|;
DECL|struct|s_xdemitconf
typedef|typedef
struct|struct
name|s_xdemitconf
block|{
DECL|member|ctxlen
name|long
name|ctxlen
decl_stmt|;
DECL|member|interhunkctxlen
name|long
name|interhunkctxlen
decl_stmt|;
DECL|member|flags
name|unsigned
name|long
name|flags
decl_stmt|;
DECL|member|find_func
name|find_func_t
name|find_func
decl_stmt|;
DECL|member|find_func_priv
name|void
modifier|*
name|find_func_priv
decl_stmt|;
DECL|member|emit_func
name|void
function_decl|(
modifier|*
name|emit_func
function_decl|)
parameter_list|()
function_decl|;
DECL|typedef|xdemitconf_t
block|}
name|xdemitconf_t
typedef|;
DECL|struct|s_bdiffparam
typedef|typedef
struct|struct
name|s_bdiffparam
block|{
DECL|member|bsize
name|long
name|bsize
decl_stmt|;
DECL|typedef|bdiffparam_t
block|}
name|bdiffparam_t
typedef|;
DECL|macro|xdl_malloc
define|#
directive|define
name|xdl_malloc
parameter_list|(
name|x
parameter_list|)
value|malloc(x)
DECL|macro|xdl_free
define|#
directive|define
name|xdl_free
parameter_list|(
name|ptr
parameter_list|)
value|free(ptr)
DECL|macro|xdl_realloc
define|#
directive|define
name|xdl_realloc
parameter_list|(
name|ptr
parameter_list|,
name|x
parameter_list|)
value|realloc(ptr,x)
name|void
modifier|*
name|xdl_mmfile_first
parameter_list|(
name|mmfile_t
modifier|*
name|mmf
parameter_list|,
name|long
modifier|*
name|size
parameter_list|)
function_decl|;
name|void
modifier|*
name|xdl_mmfile_next
parameter_list|(
name|mmfile_t
modifier|*
name|mmf
parameter_list|,
name|long
modifier|*
name|size
parameter_list|)
function_decl|;
name|long
name|xdl_mmfile_size
parameter_list|(
name|mmfile_t
modifier|*
name|mmf
parameter_list|)
function_decl|;
name|int
name|xdl_diff
parameter_list|(
name|mmfile_t
modifier|*
name|mf1
parameter_list|,
name|mmfile_t
modifier|*
name|mf2
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|xdemitconf_t
specifier|const
modifier|*
name|xecfg
parameter_list|,
name|xdemitcb_t
modifier|*
name|ecb
parameter_list|)
function_decl|;
name|int
name|xdl_merge
parameter_list|(
name|mmfile_t
modifier|*
name|orig
parameter_list|,
name|mmfile_t
modifier|*
name|mf1
parameter_list|,
specifier|const
name|char
modifier|*
name|name1
parameter_list|,
name|mmfile_t
modifier|*
name|mf2
parameter_list|,
specifier|const
name|char
modifier|*
name|name2
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|int
name|flags
parameter_list|,
name|mmbuffer_t
modifier|*
name|result
parameter_list|)
function_decl|;
ifdef|#
directive|ifdef
name|__cplusplus
block|}
end_extern

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* #ifdef __cplusplus */
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* #if !defined(XDIFF_H) */
end_comment

end_unit

