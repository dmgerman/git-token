begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  *  LibXDiff by Davide Libenzi ( File Differential Library )  *  Copyright (C) 2003	Davide Libenzi  *  *  This library is free software; you can redistribute it and/or  *  modify it under the terms of the GNU Lesser General Public  *  License as published by the Free Software Foundation; either  *  version 2.1 of the License, or (at your option) any later version.  *  *  This library is distributed in the hope that it will be useful,  *  but WITHOUT ANY WARRANTY; without even the implied warranty of  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *  Lesser General Public License for more details.  *  *  You should have received a copy of the GNU Lesser General Public  *  License along with this library; if not, write to the Free Software  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *  *  Davide Libenzi<davidel@xmailserver.org>  *  */
end_comment

begin_include
include|#
directive|include
file|"xinclude.h"
end_include

begin_function_decl
specifier|static
name|long
name|xdl_get_rec
parameter_list|(
name|xdfile_t
modifier|*
name|xdf
parameter_list|,
name|long
name|ri
parameter_list|,
name|char
specifier|const
modifier|*
modifier|*
name|rec
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|xdl_emit_record
parameter_list|(
name|xdfile_t
modifier|*
name|xdf
parameter_list|,
name|long
name|ri
parameter_list|,
name|char
specifier|const
modifier|*
name|pre
parameter_list|,
name|xdemitcb_t
modifier|*
name|ecb
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|xdl_get_rec
specifier|static
name|long
name|xdl_get_rec
parameter_list|(
name|xdfile_t
modifier|*
name|xdf
parameter_list|,
name|long
name|ri
parameter_list|,
name|char
specifier|const
modifier|*
modifier|*
name|rec
parameter_list|)
block|{
operator|*
name|rec
operator|=
name|xdf
operator|->
name|recs
index|[
name|ri
index|]
operator|->
name|ptr
expr_stmt|;
return|return
name|xdf
operator|->
name|recs
index|[
name|ri
index|]
operator|->
name|size
return|;
block|}
end_function

begin_function
DECL|function|xdl_emit_record
specifier|static
name|int
name|xdl_emit_record
parameter_list|(
name|xdfile_t
modifier|*
name|xdf
parameter_list|,
name|long
name|ri
parameter_list|,
name|char
specifier|const
modifier|*
name|pre
parameter_list|,
name|xdemitcb_t
modifier|*
name|ecb
parameter_list|)
block|{
name|long
name|size
decl_stmt|,
name|psize
init|=
name|strlen
argument_list|(
name|pre
argument_list|)
decl_stmt|;
name|char
specifier|const
modifier|*
name|rec
decl_stmt|;
name|size
operator|=
name|xdl_get_rec
argument_list|(
name|xdf
argument_list|,
name|ri
argument_list|,
operator|&
name|rec
argument_list|)
expr_stmt|;
if|if
condition|(
name|xdl_emit_diffrec
argument_list|(
name|rec
argument_list|,
name|size
argument_list|,
name|pre
argument_list|,
name|psize
argument_list|,
name|ecb
argument_list|)
operator|<
literal|0
condition|)
block|{
return|return
operator|-
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * Starting at the passed change atom, find the latest change atom to be included  * inside the differential hunk according to the specified configuration.  */
end_comment

begin_function
DECL|function|xdl_get_hunk
name|xdchange_t
modifier|*
name|xdl_get_hunk
parameter_list|(
name|xdchange_t
modifier|*
name|xscr
parameter_list|,
name|xdemitconf_t
specifier|const
modifier|*
name|xecfg
parameter_list|)
block|{
name|xdchange_t
modifier|*
name|xch
decl_stmt|,
modifier|*
name|xchp
decl_stmt|;
name|long
name|max_common
init|=
literal|2
operator|*
name|xecfg
operator|->
name|ctxlen
operator|+
name|xecfg
operator|->
name|interhunkctxlen
decl_stmt|;
for|for
control|(
name|xchp
operator|=
name|xscr
operator|,
name|xch
operator|=
name|xscr
operator|->
name|next
init|;
name|xch
condition|;
name|xchp
operator|=
name|xch
operator|,
name|xch
operator|=
name|xch
operator|->
name|next
control|)
if|if
condition|(
name|xch
operator|->
name|i1
operator|-
operator|(
name|xchp
operator|->
name|i1
operator|+
name|xchp
operator|->
name|chg1
operator|)
operator|>
name|max_common
condition|)
break|break;
return|return
name|xchp
return|;
block|}
end_function

begin_function
DECL|function|def_ff
specifier|static
name|long
name|def_ff
parameter_list|(
specifier|const
name|char
modifier|*
name|rec
parameter_list|,
name|long
name|len
parameter_list|,
name|char
modifier|*
name|buf
parameter_list|,
name|long
name|sz
parameter_list|,
name|void
modifier|*
name|priv
parameter_list|)
block|{
if|if
condition|(
name|len
operator|>
literal|0
operator|&&
operator|(
name|isalpha
argument_list|(
operator|(
name|unsigned
name|char
operator|)
operator|*
name|rec
argument_list|)
operator|||
comment|/* identifier? */
operator|*
name|rec
operator|==
literal|'_'
operator|||
comment|/* also identifier? */
operator|*
name|rec
operator|==
literal|'$'
operator|)
condition|)
block|{
comment|/* identifiers from VMS and other esoterico */
if|if
condition|(
name|len
operator|>
name|sz
condition|)
name|len
operator|=
name|sz
expr_stmt|;
while|while
condition|(
literal|0
operator|<
name|len
operator|&&
name|isspace
argument_list|(
operator|(
name|unsigned
name|char
operator|)
name|rec
index|[
name|len
operator|-
literal|1
index|]
argument_list|)
condition|)
name|len
operator|--
expr_stmt|;
name|memcpy
argument_list|(
name|buf
argument_list|,
name|rec
argument_list|,
name|len
argument_list|)
expr_stmt|;
return|return
name|len
return|;
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|xdl_emit_common
specifier|static
name|int
name|xdl_emit_common
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
block|{
name|xdfile_t
modifier|*
name|xdf
init|=
operator|&
name|xe
operator|->
name|xdf1
decl_stmt|;
specifier|const
name|char
modifier|*
name|rchg
init|=
name|xdf
operator|->
name|rchg
decl_stmt|;
name|long
name|ix
decl_stmt|;
for|for
control|(
name|ix
operator|=
literal|0
init|;
name|ix
operator|<
name|xdf
operator|->
name|nrec
condition|;
name|ix
operator|++
control|)
block|{
if|if
condition|(
name|rchg
index|[
name|ix
index|]
condition|)
continue|continue;
if|if
condition|(
name|xdl_emit_record
argument_list|(
name|xdf
argument_list|,
name|ix
argument_list|,
literal|""
argument_list|,
name|ecb
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_struct
DECL|struct|func_line
struct|struct
name|func_line
block|{
DECL|member|len
name|long
name|len
decl_stmt|;
DECL|member|buf
name|char
name|buf
index|[
literal|80
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_function
DECL|function|get_func_line
specifier|static
name|long
name|get_func_line
parameter_list|(
name|xdfenv_t
modifier|*
name|xe
parameter_list|,
name|xdemitconf_t
specifier|const
modifier|*
name|xecfg
parameter_list|,
name|struct
name|func_line
modifier|*
name|func_line
parameter_list|,
name|long
name|start
parameter_list|,
name|long
name|limit
parameter_list|)
block|{
name|find_func_t
name|ff
init|=
name|xecfg
operator|->
name|find_func
condition|?
name|xecfg
operator|->
name|find_func
else|:
name|def_ff
decl_stmt|;
name|long
name|l
decl_stmt|,
name|size
decl_stmt|,
name|step
init|=
operator|(
name|start
operator|>
name|limit
operator|)
condition|?
operator|-
literal|1
else|:
literal|1
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|,
name|dummy
index|[
literal|1
index|]
decl_stmt|;
name|buf
operator|=
name|func_line
condition|?
name|func_line
operator|->
name|buf
else|:
name|dummy
expr_stmt|;
name|size
operator|=
name|func_line
condition|?
sizeof|sizeof
argument_list|(
name|func_line
operator|->
name|buf
argument_list|)
else|:
sizeof|sizeof
argument_list|(
name|dummy
argument_list|)
expr_stmt|;
for|for
control|(
name|l
operator|=
name|start
init|;
name|l
operator|!=
name|limit
operator|&&
literal|0
operator|<=
name|l
operator|&&
name|l
operator|<
name|xe
operator|->
name|xdf1
operator|.
name|nrec
condition|;
name|l
operator|+=
name|step
control|)
block|{
specifier|const
name|char
modifier|*
name|rec
decl_stmt|;
name|long
name|reclen
init|=
name|xdl_get_rec
argument_list|(
operator|&
name|xe
operator|->
name|xdf1
argument_list|,
name|l
argument_list|,
operator|&
name|rec
argument_list|)
decl_stmt|;
name|long
name|len
init|=
name|ff
argument_list|(
name|rec
argument_list|,
name|reclen
argument_list|,
name|buf
argument_list|,
name|size
argument_list|,
name|xecfg
operator|->
name|find_func_priv
argument_list|)
decl_stmt|;
if|if
condition|(
name|len
operator|>=
literal|0
condition|)
block|{
if|if
condition|(
name|func_line
condition|)
name|func_line
operator|->
name|len
operator|=
name|len
expr_stmt|;
return|return
name|l
return|;
block|}
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|xdl_emit_diff
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
block|{
name|long
name|s1
decl_stmt|,
name|s2
decl_stmt|,
name|e1
decl_stmt|,
name|e2
decl_stmt|,
name|lctx
decl_stmt|;
name|xdchange_t
modifier|*
name|xch
decl_stmt|,
modifier|*
name|xche
decl_stmt|;
name|long
name|funclineprev
init|=
operator|-
literal|1
decl_stmt|;
name|struct
name|func_line
name|func_line
init|=
block|{
literal|0
block|}
decl_stmt|;
if|if
condition|(
name|xecfg
operator|->
name|flags
operator|&
name|XDL_EMIT_COMMON
condition|)
return|return
name|xdl_emit_common
argument_list|(
name|xe
argument_list|,
name|xscr
argument_list|,
name|ecb
argument_list|,
name|xecfg
argument_list|)
return|;
for|for
control|(
name|xch
operator|=
name|xscr
init|;
name|xch
condition|;
name|xch
operator|=
name|xche
operator|->
name|next
control|)
block|{
name|xche
operator|=
name|xdl_get_hunk
argument_list|(
name|xch
argument_list|,
name|xecfg
argument_list|)
expr_stmt|;
name|s1
operator|=
name|XDL_MAX
argument_list|(
name|xch
operator|->
name|i1
operator|-
name|xecfg
operator|->
name|ctxlen
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|s2
operator|=
name|XDL_MAX
argument_list|(
name|xch
operator|->
name|i2
operator|-
name|xecfg
operator|->
name|ctxlen
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|xecfg
operator|->
name|flags
operator|&
name|XDL_EMIT_FUNCCONTEXT
condition|)
block|{
name|long
name|fs1
init|=
name|get_func_line
argument_list|(
name|xe
argument_list|,
name|xecfg
argument_list|,
name|NULL
argument_list|,
name|xch
operator|->
name|i1
argument_list|,
operator|-
literal|1
argument_list|)
decl_stmt|;
if|if
condition|(
name|fs1
operator|<
literal|0
condition|)
name|fs1
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|fs1
operator|<
name|s1
condition|)
block|{
name|s2
operator|-=
name|s1
operator|-
name|fs1
expr_stmt|;
name|s1
operator|=
name|fs1
expr_stmt|;
block|}
block|}
name|again
label|:
name|lctx
operator|=
name|xecfg
operator|->
name|ctxlen
expr_stmt|;
name|lctx
operator|=
name|XDL_MIN
argument_list|(
name|lctx
argument_list|,
name|xe
operator|->
name|xdf1
operator|.
name|nrec
operator|-
operator|(
name|xche
operator|->
name|i1
operator|+
name|xche
operator|->
name|chg1
operator|)
argument_list|)
expr_stmt|;
name|lctx
operator|=
name|XDL_MIN
argument_list|(
name|lctx
argument_list|,
name|xe
operator|->
name|xdf2
operator|.
name|nrec
operator|-
operator|(
name|xche
operator|->
name|i2
operator|+
name|xche
operator|->
name|chg2
operator|)
argument_list|)
expr_stmt|;
name|e1
operator|=
name|xche
operator|->
name|i1
operator|+
name|xche
operator|->
name|chg1
operator|+
name|lctx
expr_stmt|;
name|e2
operator|=
name|xche
operator|->
name|i2
operator|+
name|xche
operator|->
name|chg2
operator|+
name|lctx
expr_stmt|;
if|if
condition|(
name|xecfg
operator|->
name|flags
operator|&
name|XDL_EMIT_FUNCCONTEXT
condition|)
block|{
name|long
name|fe1
init|=
name|get_func_line
argument_list|(
name|xe
argument_list|,
name|xecfg
argument_list|,
name|NULL
argument_list|,
name|xche
operator|->
name|i1
operator|+
name|xche
operator|->
name|chg1
argument_list|,
name|xe
operator|->
name|xdf1
operator|.
name|nrec
argument_list|)
decl_stmt|;
if|if
condition|(
name|fe1
operator|<
literal|0
condition|)
name|fe1
operator|=
name|xe
operator|->
name|xdf1
operator|.
name|nrec
expr_stmt|;
if|if
condition|(
name|fe1
operator|>
name|e1
condition|)
block|{
name|e2
operator|+=
name|fe1
operator|-
name|e1
expr_stmt|;
name|e1
operator|=
name|fe1
expr_stmt|;
block|}
comment|/* 			 * Overlap with next change?  Then include it 			 * in the current hunk and start over to find 			 * its new end. 			 */
if|if
condition|(
name|xche
operator|->
name|next
condition|)
block|{
name|long
name|l
init|=
name|xche
operator|->
name|next
operator|->
name|i1
decl_stmt|;
if|if
condition|(
name|l
operator|<=
name|e1
operator|||
name|get_func_line
argument_list|(
name|xe
argument_list|,
name|xecfg
argument_list|,
name|NULL
argument_list|,
name|l
argument_list|,
name|e1
argument_list|)
operator|<
literal|0
condition|)
block|{
name|xche
operator|=
name|xche
operator|->
name|next
expr_stmt|;
goto|goto
name|again
goto|;
block|}
block|}
block|}
comment|/* 		 * Emit current hunk header. 		 */
if|if
condition|(
name|xecfg
operator|->
name|flags
operator|&
name|XDL_EMIT_FUNCNAMES
condition|)
block|{
name|get_func_line
argument_list|(
name|xe
argument_list|,
name|xecfg
argument_list|,
operator|&
name|func_line
argument_list|,
name|s1
operator|-
literal|1
argument_list|,
name|funclineprev
argument_list|)
expr_stmt|;
name|funclineprev
operator|=
name|s1
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|xdl_emit_hunk_hdr
argument_list|(
name|s1
operator|+
literal|1
argument_list|,
name|e1
operator|-
name|s1
argument_list|,
name|s2
operator|+
literal|1
argument_list|,
name|e2
operator|-
name|s2
argument_list|,
name|func_line
operator|.
name|buf
argument_list|,
name|func_line
operator|.
name|len
argument_list|,
name|ecb
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
comment|/* 		 * Emit pre-context. 		 */
for|for
control|(
init|;
name|s1
operator|<
name|xch
operator|->
name|i1
condition|;
name|s1
operator|++
control|)
if|if
condition|(
name|xdl_emit_record
argument_list|(
operator|&
name|xe
operator|->
name|xdf1
argument_list|,
name|s1
argument_list|,
literal|" "
argument_list|,
name|ecb
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
for|for
control|(
name|s1
operator|=
name|xch
operator|->
name|i1
operator|,
name|s2
operator|=
name|xch
operator|->
name|i2
init|;
condition|;
name|xch
operator|=
name|xch
operator|->
name|next
control|)
block|{
comment|/* 			 * Merge previous with current change atom. 			 */
for|for
control|(
init|;
name|s1
operator|<
name|xch
operator|->
name|i1
operator|&&
name|s2
operator|<
name|xch
operator|->
name|i2
condition|;
name|s1
operator|++
operator|,
name|s2
operator|++
control|)
if|if
condition|(
name|xdl_emit_record
argument_list|(
operator|&
name|xe
operator|->
name|xdf1
argument_list|,
name|s1
argument_list|,
literal|" "
argument_list|,
name|ecb
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
comment|/* 			 * Removes lines from the first file. 			 */
for|for
control|(
name|s1
operator|=
name|xch
operator|->
name|i1
init|;
name|s1
operator|<
name|xch
operator|->
name|i1
operator|+
name|xch
operator|->
name|chg1
condition|;
name|s1
operator|++
control|)
if|if
condition|(
name|xdl_emit_record
argument_list|(
operator|&
name|xe
operator|->
name|xdf1
argument_list|,
name|s1
argument_list|,
literal|"-"
argument_list|,
name|ecb
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
comment|/* 			 * Adds lines from the second file. 			 */
for|for
control|(
name|s2
operator|=
name|xch
operator|->
name|i2
init|;
name|s2
operator|<
name|xch
operator|->
name|i2
operator|+
name|xch
operator|->
name|chg2
condition|;
name|s2
operator|++
control|)
if|if
condition|(
name|xdl_emit_record
argument_list|(
operator|&
name|xe
operator|->
name|xdf2
argument_list|,
name|s2
argument_list|,
literal|"+"
argument_list|,
name|ecb
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|xch
operator|==
name|xche
condition|)
break|break;
name|s1
operator|=
name|xch
operator|->
name|i1
operator|+
name|xch
operator|->
name|chg1
expr_stmt|;
name|s2
operator|=
name|xch
operator|->
name|i2
operator|+
name|xch
operator|->
name|chg2
expr_stmt|;
block|}
comment|/* 		 * Emit post-context. 		 */
for|for
control|(
name|s1
operator|=
name|xche
operator|->
name|i1
operator|+
name|xche
operator|->
name|chg1
init|;
name|s1
operator|<
name|e1
condition|;
name|s1
operator|++
control|)
if|if
condition|(
name|xdl_emit_record
argument_list|(
operator|&
name|xe
operator|->
name|xdf1
argument_list|,
name|s1
argument_list|,
literal|" "
argument_list|,
name|ecb
argument_list|)
operator|<
literal|0
condition|)
return|return
operator|-
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

