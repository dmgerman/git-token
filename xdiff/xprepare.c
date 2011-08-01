begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  *  LibXDiff by Davide Libenzi ( File Differential Library )  *  Copyright (C) 2003  Davide Libenzi  *  *  This library is free software; you can redistribute it and/or  *  modify it under the terms of the GNU Lesser General Public  *  License as published by the Free Software Foundation; either  *  version 2.1 of the License, or (at your option) any later version.  *  *  This library is distributed in the hope that it will be useful,  *  but WITHOUT ANY WARRANTY; without even the implied warranty of  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *  Lesser General Public License for more details.  *  *  You should have received a copy of the GNU Lesser General Public  *  License along with this library; if not, write to the Free Software  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *  *  Davide Libenzi<davidel@xmailserver.org>  *  */
end_comment

begin_include
include|#
directive|include
file|"xinclude.h"
end_include

begin_define
DECL|macro|XDL_KPDIS_RUN
define|#
directive|define
name|XDL_KPDIS_RUN
value|4
end_define

begin_define
DECL|macro|XDL_MAX_EQLIMIT
define|#
directive|define
name|XDL_MAX_EQLIMIT
value|1024
end_define

begin_define
DECL|macro|XDL_SIMSCAN_WINDOW
define|#
directive|define
name|XDL_SIMSCAN_WINDOW
value|100
end_define

begin_define
DECL|macro|XDL_GUESS_NLINES1
define|#
directive|define
name|XDL_GUESS_NLINES1
value|256
end_define

begin_define
DECL|macro|XDL_GUESS_NLINES2
define|#
directive|define
name|XDL_GUESS_NLINES2
value|20
end_define

begin_typedef
DECL|struct|s_xdlclass
typedef|typedef
struct|struct
name|s_xdlclass
block|{
DECL|member|next
name|struct
name|s_xdlclass
modifier|*
name|next
decl_stmt|;
DECL|member|ha
name|unsigned
name|long
name|ha
decl_stmt|;
DECL|member|line
name|char
specifier|const
modifier|*
name|line
decl_stmt|;
DECL|member|size
name|long
name|size
decl_stmt|;
DECL|member|idx
name|long
name|idx
decl_stmt|;
DECL|typedef|xdlclass_t
block|}
name|xdlclass_t
typedef|;
end_typedef

begin_typedef
DECL|struct|s_xdlclassifier
typedef|typedef
struct|struct
name|s_xdlclassifier
block|{
DECL|member|hbits
name|unsigned
name|int
name|hbits
decl_stmt|;
DECL|member|hsize
name|long
name|hsize
decl_stmt|;
DECL|member|rchash
name|xdlclass_t
modifier|*
modifier|*
name|rchash
decl_stmt|;
DECL|member|ncha
name|chastore_t
name|ncha
decl_stmt|;
DECL|member|count
name|long
name|count
decl_stmt|;
DECL|member|flags
name|long
name|flags
decl_stmt|;
DECL|typedef|xdlclassifier_t
block|}
name|xdlclassifier_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|int
name|xdl_init_classifier
parameter_list|(
name|xdlclassifier_t
modifier|*
name|cf
parameter_list|,
name|long
name|size
parameter_list|,
name|long
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|xdl_free_classifier
parameter_list|(
name|xdlclassifier_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|xdl_classify_record
parameter_list|(
name|xdlclassifier_t
modifier|*
name|cf
parameter_list|,
name|xrecord_t
modifier|*
modifier|*
name|rhash
parameter_list|,
name|unsigned
name|int
name|hbits
parameter_list|,
name|xrecord_t
modifier|*
name|rec
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|xdl_prepare_ctx
parameter_list|(
name|mmfile_t
modifier|*
name|mf
parameter_list|,
name|long
name|narec
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|xdlclassifier_t
modifier|*
name|cf
parameter_list|,
name|xdfile_t
modifier|*
name|xdf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|xdl_free_ctx
parameter_list|(
name|xdfile_t
modifier|*
name|xdf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|xdl_clean_mmatch
parameter_list|(
name|char
specifier|const
modifier|*
name|dis
parameter_list|,
name|long
name|i
parameter_list|,
name|long
name|s
parameter_list|,
name|long
name|e
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|xdl_cleanup_records
parameter_list|(
name|xdfile_t
modifier|*
name|xdf1
parameter_list|,
name|xdfile_t
modifier|*
name|xdf2
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|xdl_trim_ends
parameter_list|(
name|xdfile_t
modifier|*
name|xdf1
parameter_list|,
name|xdfile_t
modifier|*
name|xdf2
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|xdl_optimize_ctxs
parameter_list|(
name|xdfile_t
modifier|*
name|xdf1
parameter_list|,
name|xdfile_t
modifier|*
name|xdf2
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|xdl_init_classifier
specifier|static
name|int
name|xdl_init_classifier
parameter_list|(
name|xdlclassifier_t
modifier|*
name|cf
parameter_list|,
name|long
name|size
parameter_list|,
name|long
name|flags
parameter_list|)
block|{
name|cf
operator|->
name|flags
operator|=
name|flags
expr_stmt|;
name|cf
operator|->
name|hbits
operator|=
name|xdl_hashbits
argument_list|(
operator|(
name|unsigned
name|int
operator|)
name|size
argument_list|)
expr_stmt|;
name|cf
operator|->
name|hsize
operator|=
literal|1
operator|<<
name|cf
operator|->
name|hbits
expr_stmt|;
if|if
condition|(
name|xdl_cha_init
argument_list|(
operator|&
name|cf
operator|->
name|ncha
argument_list|,
sizeof|sizeof
argument_list|(
name|xdlclass_t
argument_list|)
argument_list|,
name|size
operator|/
literal|4
operator|+
literal|1
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
if|if
condition|(
operator|!
operator|(
name|cf
operator|->
name|rchash
operator|=
operator|(
name|xdlclass_t
operator|*
operator|*
operator|)
name|xdl_malloc
argument_list|(
name|cf
operator|->
name|hsize
operator|*
sizeof|sizeof
argument_list|(
name|xdlclass_t
operator|*
argument_list|)
argument_list|)
operator|)
condition|)
block|{
name|xdl_cha_free
argument_list|(
operator|&
name|cf
operator|->
name|ncha
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
name|memset
argument_list|(
name|cf
operator|->
name|rchash
argument_list|,
literal|0
argument_list|,
name|cf
operator|->
name|hsize
operator|*
sizeof|sizeof
argument_list|(
name|xdlclass_t
operator|*
argument_list|)
argument_list|)
expr_stmt|;
name|cf
operator|->
name|count
operator|=
literal|0
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|xdl_free_classifier
specifier|static
name|void
name|xdl_free_classifier
parameter_list|(
name|xdlclassifier_t
modifier|*
name|cf
parameter_list|)
block|{
name|xdl_free
argument_list|(
name|cf
operator|->
name|rchash
argument_list|)
expr_stmt|;
name|xdl_cha_free
argument_list|(
operator|&
name|cf
operator|->
name|ncha
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|xdl_classify_record
specifier|static
name|int
name|xdl_classify_record
parameter_list|(
name|xdlclassifier_t
modifier|*
name|cf
parameter_list|,
name|xrecord_t
modifier|*
modifier|*
name|rhash
parameter_list|,
name|unsigned
name|int
name|hbits
parameter_list|,
name|xrecord_t
modifier|*
name|rec
parameter_list|)
block|{
name|long
name|hi
decl_stmt|;
name|char
specifier|const
modifier|*
name|line
decl_stmt|;
name|xdlclass_t
modifier|*
name|rcrec
decl_stmt|;
name|line
operator|=
name|rec
operator|->
name|ptr
expr_stmt|;
name|hi
operator|=
operator|(
name|long
operator|)
name|XDL_HASHLONG
argument_list|(
name|rec
operator|->
name|ha
argument_list|,
name|cf
operator|->
name|hbits
argument_list|)
expr_stmt|;
for|for
control|(
name|rcrec
operator|=
name|cf
operator|->
name|rchash
index|[
name|hi
index|]
init|;
name|rcrec
condition|;
name|rcrec
operator|=
name|rcrec
operator|->
name|next
control|)
if|if
condition|(
name|rcrec
operator|->
name|ha
operator|==
name|rec
operator|->
name|ha
operator|&&
name|xdl_recmatch
argument_list|(
name|rcrec
operator|->
name|line
argument_list|,
name|rcrec
operator|->
name|size
argument_list|,
name|rec
operator|->
name|ptr
argument_list|,
name|rec
operator|->
name|size
argument_list|,
name|cf
operator|->
name|flags
argument_list|)
condition|)
break|break;
if|if
condition|(
operator|!
name|rcrec
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|rcrec
operator|=
name|xdl_cha_alloc
argument_list|(
operator|&
name|cf
operator|->
name|ncha
argument_list|)
operator|)
condition|)
block|{
return|return
operator|-
literal|1
return|;
block|}
name|rcrec
operator|->
name|idx
operator|=
name|cf
operator|->
name|count
operator|++
expr_stmt|;
name|rcrec
operator|->
name|line
operator|=
name|line
expr_stmt|;
name|rcrec
operator|->
name|size
operator|=
name|rec
operator|->
name|size
expr_stmt|;
name|rcrec
operator|->
name|ha
operator|=
name|rec
operator|->
name|ha
expr_stmt|;
name|rcrec
operator|->
name|next
operator|=
name|cf
operator|->
name|rchash
index|[
name|hi
index|]
expr_stmt|;
name|cf
operator|->
name|rchash
index|[
name|hi
index|]
operator|=
name|rcrec
expr_stmt|;
block|}
name|rec
operator|->
name|ha
operator|=
operator|(
name|unsigned
name|long
operator|)
name|rcrec
operator|->
name|idx
expr_stmt|;
name|hi
operator|=
operator|(
name|long
operator|)
name|XDL_HASHLONG
argument_list|(
name|rec
operator|->
name|ha
argument_list|,
name|hbits
argument_list|)
expr_stmt|;
name|rec
operator|->
name|next
operator|=
name|rhash
index|[
name|hi
index|]
expr_stmt|;
name|rhash
index|[
name|hi
index|]
operator|=
name|rec
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|xdl_prepare_ctx
specifier|static
name|int
name|xdl_prepare_ctx
parameter_list|(
name|mmfile_t
modifier|*
name|mf
parameter_list|,
name|long
name|narec
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|xdlclassifier_t
modifier|*
name|cf
parameter_list|,
name|xdfile_t
modifier|*
name|xdf
parameter_list|)
block|{
name|unsigned
name|int
name|hbits
decl_stmt|;
name|long
name|nrec
decl_stmt|,
name|hsize
decl_stmt|,
name|bsize
decl_stmt|;
name|unsigned
name|long
name|hav
decl_stmt|;
name|char
specifier|const
modifier|*
name|blk
decl_stmt|,
modifier|*
name|cur
decl_stmt|,
modifier|*
name|top
decl_stmt|,
modifier|*
name|prev
decl_stmt|;
name|xrecord_t
modifier|*
name|crec
decl_stmt|;
name|xrecord_t
modifier|*
modifier|*
name|recs
decl_stmt|,
modifier|*
modifier|*
name|rrecs
decl_stmt|;
name|xrecord_t
modifier|*
modifier|*
name|rhash
decl_stmt|;
name|unsigned
name|long
modifier|*
name|ha
decl_stmt|;
name|char
modifier|*
name|rchg
decl_stmt|;
name|long
modifier|*
name|rindex
decl_stmt|;
name|ha
operator|=
name|NULL
expr_stmt|;
name|rindex
operator|=
name|NULL
expr_stmt|;
name|rchg
operator|=
name|NULL
expr_stmt|;
name|rhash
operator|=
name|NULL
expr_stmt|;
name|recs
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|xdl_cha_init
argument_list|(
operator|&
name|xdf
operator|->
name|rcha
argument_list|,
sizeof|sizeof
argument_list|(
name|xrecord_t
argument_list|)
argument_list|,
name|narec
operator|/
literal|4
operator|+
literal|1
argument_list|)
operator|<
literal|0
condition|)
goto|goto
name|abort
goto|;
if|if
condition|(
operator|!
operator|(
name|recs
operator|=
operator|(
name|xrecord_t
operator|*
operator|*
operator|)
name|xdl_malloc
argument_list|(
name|narec
operator|*
sizeof|sizeof
argument_list|(
name|xrecord_t
operator|*
argument_list|)
argument_list|)
operator|)
condition|)
goto|goto
name|abort
goto|;
if|if
condition|(
name|xpp
operator|->
name|flags
operator|&
name|XDF_HISTOGRAM_DIFF
condition|)
name|hbits
operator|=
name|hsize
operator|=
literal|0
expr_stmt|;
else|else
block|{
name|hbits
operator|=
name|xdl_hashbits
argument_list|(
operator|(
name|unsigned
name|int
operator|)
name|narec
argument_list|)
expr_stmt|;
name|hsize
operator|=
literal|1
operator|<<
name|hbits
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|rhash
operator|=
operator|(
name|xrecord_t
operator|*
operator|*
operator|)
name|xdl_malloc
argument_list|(
name|hsize
operator|*
sizeof|sizeof
argument_list|(
name|xrecord_t
operator|*
argument_list|)
argument_list|)
operator|)
condition|)
goto|goto
name|abort
goto|;
name|memset
argument_list|(
name|rhash
argument_list|,
literal|0
argument_list|,
name|hsize
operator|*
sizeof|sizeof
argument_list|(
name|xrecord_t
operator|*
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|nrec
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|(
name|cur
operator|=
name|blk
operator|=
name|xdl_mmfile_first
argument_list|(
name|mf
argument_list|,
operator|&
name|bsize
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
for|for
control|(
name|top
operator|=
name|blk
operator|+
name|bsize
init|;
name|cur
operator|<
name|top
condition|;
control|)
block|{
name|prev
operator|=
name|cur
expr_stmt|;
name|hav
operator|=
name|xdl_hash_record
argument_list|(
operator|&
name|cur
argument_list|,
name|top
argument_list|,
name|xpp
operator|->
name|flags
argument_list|)
expr_stmt|;
if|if
condition|(
name|nrec
operator|>=
name|narec
condition|)
block|{
name|narec
operator|*=
literal|2
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|rrecs
operator|=
operator|(
name|xrecord_t
operator|*
operator|*
operator|)
name|xdl_realloc
argument_list|(
name|recs
argument_list|,
name|narec
operator|*
sizeof|sizeof
argument_list|(
name|xrecord_t
operator|*
argument_list|)
argument_list|)
operator|)
condition|)
goto|goto
name|abort
goto|;
name|recs
operator|=
name|rrecs
expr_stmt|;
block|}
if|if
condition|(
operator|!
operator|(
name|crec
operator|=
name|xdl_cha_alloc
argument_list|(
operator|&
name|xdf
operator|->
name|rcha
argument_list|)
operator|)
condition|)
goto|goto
name|abort
goto|;
name|crec
operator|->
name|ptr
operator|=
name|prev
expr_stmt|;
name|crec
operator|->
name|size
operator|=
call|(
name|long
call|)
argument_list|(
name|cur
operator|-
name|prev
argument_list|)
expr_stmt|;
name|crec
operator|->
name|ha
operator|=
name|hav
expr_stmt|;
name|recs
index|[
name|nrec
operator|++
index|]
operator|=
name|crec
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|xpp
operator|->
name|flags
operator|&
name|XDF_HISTOGRAM_DIFF
operator|)
operator|&&
name|xdl_classify_record
argument_list|(
name|cf
argument_list|,
name|rhash
argument_list|,
name|hbits
argument_list|,
name|crec
argument_list|)
operator|<
literal|0
condition|)
goto|goto
name|abort
goto|;
block|}
block|}
if|if
condition|(
operator|!
operator|(
name|rchg
operator|=
operator|(
name|char
operator|*
operator|)
name|xdl_malloc
argument_list|(
operator|(
name|nrec
operator|+
literal|2
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|char
argument_list|)
argument_list|)
operator|)
condition|)
goto|goto
name|abort
goto|;
name|memset
argument_list|(
name|rchg
argument_list|,
literal|0
argument_list|,
operator|(
name|nrec
operator|+
literal|2
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|char
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|rindex
operator|=
operator|(
name|long
operator|*
operator|)
name|xdl_malloc
argument_list|(
operator|(
name|nrec
operator|+
literal|1
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|long
argument_list|)
argument_list|)
operator|)
condition|)
goto|goto
name|abort
goto|;
if|if
condition|(
operator|!
operator|(
name|ha
operator|=
operator|(
name|unsigned
name|long
operator|*
operator|)
name|xdl_malloc
argument_list|(
operator|(
name|nrec
operator|+
literal|1
operator|)
operator|*
expr|sizeof
operator|(
name|unsigned
name|long
operator|)
argument_list|)
operator|)
condition|)
goto|goto
name|abort
goto|;
name|xdf
operator|->
name|nrec
operator|=
name|nrec
expr_stmt|;
name|xdf
operator|->
name|recs
operator|=
name|recs
expr_stmt|;
name|xdf
operator|->
name|hbits
operator|=
name|hbits
expr_stmt|;
name|xdf
operator|->
name|rhash
operator|=
name|rhash
expr_stmt|;
name|xdf
operator|->
name|rchg
operator|=
name|rchg
operator|+
literal|1
expr_stmt|;
name|xdf
operator|->
name|rindex
operator|=
name|rindex
expr_stmt|;
name|xdf
operator|->
name|nreff
operator|=
literal|0
expr_stmt|;
name|xdf
operator|->
name|ha
operator|=
name|ha
expr_stmt|;
name|xdf
operator|->
name|dstart
operator|=
literal|0
expr_stmt|;
name|xdf
operator|->
name|dend
operator|=
name|nrec
operator|-
literal|1
expr_stmt|;
return|return
literal|0
return|;
name|abort
label|:
name|xdl_free
argument_list|(
name|ha
argument_list|)
expr_stmt|;
name|xdl_free
argument_list|(
name|rindex
argument_list|)
expr_stmt|;
name|xdl_free
argument_list|(
name|rchg
argument_list|)
expr_stmt|;
name|xdl_free
argument_list|(
name|rhash
argument_list|)
expr_stmt|;
name|xdl_free
argument_list|(
name|recs
argument_list|)
expr_stmt|;
name|xdl_cha_free
argument_list|(
operator|&
name|xdf
operator|->
name|rcha
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|xdl_free_ctx
specifier|static
name|void
name|xdl_free_ctx
parameter_list|(
name|xdfile_t
modifier|*
name|xdf
parameter_list|)
block|{
name|xdl_free
argument_list|(
name|xdf
operator|->
name|rhash
argument_list|)
expr_stmt|;
name|xdl_free
argument_list|(
name|xdf
operator|->
name|rindex
argument_list|)
expr_stmt|;
name|xdl_free
argument_list|(
name|xdf
operator|->
name|rchg
operator|-
literal|1
argument_list|)
expr_stmt|;
name|xdl_free
argument_list|(
name|xdf
operator|->
name|ha
argument_list|)
expr_stmt|;
name|xdl_free
argument_list|(
name|xdf
operator|->
name|recs
argument_list|)
expr_stmt|;
name|xdl_cha_free
argument_list|(
operator|&
name|xdf
operator|->
name|rcha
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|xdl_prepare_env
name|int
name|xdl_prepare_env
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
name|xdfenv_t
modifier|*
name|xe
parameter_list|)
block|{
name|long
name|enl1
decl_stmt|,
name|enl2
decl_stmt|,
name|sample
decl_stmt|;
name|xdlclassifier_t
name|cf
decl_stmt|;
comment|/* 	 * For histogram diff, we can afford a smaller sample size and 	 * thus a poorer estimate of the number of lines, as the hash 	 * table (rhash) won't be filled up/grown. The number of lines 	 * (nrecs) will be updated correctly anyway by 	 * xdl_prepare_ctx(). 	 */
name|sample
operator|=
name|xpp
operator|->
name|flags
operator|&
name|XDF_HISTOGRAM_DIFF
condition|?
name|XDL_GUESS_NLINES2
else|:
name|XDL_GUESS_NLINES1
expr_stmt|;
name|enl1
operator|=
name|xdl_guess_lines
argument_list|(
name|mf1
argument_list|,
name|sample
argument_list|)
operator|+
literal|1
expr_stmt|;
name|enl2
operator|=
name|xdl_guess_lines
argument_list|(
name|mf2
argument_list|,
name|sample
argument_list|)
operator|+
literal|1
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|xpp
operator|->
name|flags
operator|&
name|XDF_HISTOGRAM_DIFF
operator|)
operator|&&
name|xdl_init_classifier
argument_list|(
operator|&
name|cf
argument_list|,
name|enl1
operator|+
name|enl2
operator|+
literal|1
argument_list|,
name|xpp
operator|->
name|flags
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
if|if
condition|(
name|xdl_prepare_ctx
argument_list|(
name|mf1
argument_list|,
name|enl1
argument_list|,
name|xpp
argument_list|,
operator|&
name|cf
argument_list|,
operator|&
name|xe
operator|->
name|xdf1
argument_list|)
operator|<
literal|0
condition|)
block|{
name|xdl_free_classifier
argument_list|(
operator|&
name|cf
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|xdl_prepare_ctx
argument_list|(
name|mf2
argument_list|,
name|enl2
argument_list|,
name|xpp
argument_list|,
operator|&
name|cf
argument_list|,
operator|&
name|xe
operator|->
name|xdf2
argument_list|)
operator|<
literal|0
condition|)
block|{
name|xdl_free_ctx
argument_list|(
operator|&
name|xe
operator|->
name|xdf1
argument_list|)
expr_stmt|;
name|xdl_free_classifier
argument_list|(
operator|&
name|cf
argument_list|)
expr_stmt|;
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
operator|!
operator|(
name|xpp
operator|->
name|flags
operator|&
name|XDF_HISTOGRAM_DIFF
operator|)
condition|)
name|xdl_free_classifier
argument_list|(
operator|&
name|cf
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|xpp
operator|->
name|flags
operator|&
name|XDF_PATIENCE_DIFF
operator|)
operator|&&
operator|!
operator|(
name|xpp
operator|->
name|flags
operator|&
name|XDF_HISTOGRAM_DIFF
operator|)
operator|&&
name|xdl_optimize_ctxs
argument_list|(
operator|&
name|xe
operator|->
name|xdf1
argument_list|,
operator|&
name|xe
operator|->
name|xdf2
argument_list|)
operator|<
literal|0
condition|)
block|{
name|xdl_free_ctx
argument_list|(
operator|&
name|xe
operator|->
name|xdf2
argument_list|)
expr_stmt|;
name|xdl_free_ctx
argument_list|(
operator|&
name|xe
operator|->
name|xdf1
argument_list|)
expr_stmt|;
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

begin_function
DECL|function|xdl_free_env
name|void
name|xdl_free_env
parameter_list|(
name|xdfenv_t
modifier|*
name|xe
parameter_list|)
block|{
name|xdl_free_ctx
argument_list|(
operator|&
name|xe
operator|->
name|xdf2
argument_list|)
expr_stmt|;
name|xdl_free_ctx
argument_list|(
operator|&
name|xe
operator|->
name|xdf1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|xdl_clean_mmatch
specifier|static
name|int
name|xdl_clean_mmatch
parameter_list|(
name|char
specifier|const
modifier|*
name|dis
parameter_list|,
name|long
name|i
parameter_list|,
name|long
name|s
parameter_list|,
name|long
name|e
parameter_list|)
block|{
name|long
name|r
decl_stmt|,
name|rdis0
decl_stmt|,
name|rpdis0
decl_stmt|,
name|rdis1
decl_stmt|,
name|rpdis1
decl_stmt|;
comment|/* 	 * Limits the window the is examined during the similar-lines 	 * scan. The loops below stops when dis[i - r] == 1 (line that 	 * has no match), but there are corner cases where the loop 	 * proceed all the way to the extremities by causing huge 	 * performance penalties in case of big files. 	 */
if|if
condition|(
name|i
operator|-
name|s
operator|>
name|XDL_SIMSCAN_WINDOW
condition|)
name|s
operator|=
name|i
operator|-
name|XDL_SIMSCAN_WINDOW
expr_stmt|;
if|if
condition|(
name|e
operator|-
name|i
operator|>
name|XDL_SIMSCAN_WINDOW
condition|)
name|e
operator|=
name|i
operator|+
name|XDL_SIMSCAN_WINDOW
expr_stmt|;
comment|/* 	 * Scans the lines before 'i' to find a run of lines that either 	 * have no match (dis[j] == 0) or have multiple matches (dis[j]> 1). 	 * Note that we always call this function with dis[i]> 1, so the 	 * current line (i) is already a multimatch line. 	 */
for|for
control|(
name|r
operator|=
literal|1
operator|,
name|rdis0
operator|=
literal|0
operator|,
name|rpdis0
operator|=
literal|1
init|;
operator|(
name|i
operator|-
name|r
operator|)
operator|>=
name|s
condition|;
name|r
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|dis
index|[
name|i
operator|-
name|r
index|]
condition|)
name|rdis0
operator|++
expr_stmt|;
elseif|else
if|if
condition|(
name|dis
index|[
name|i
operator|-
name|r
index|]
operator|==
literal|2
condition|)
name|rpdis0
operator|++
expr_stmt|;
else|else
break|break;
block|}
comment|/* 	 * If the run before the line 'i' found only multimatch lines, we 	 * return 0 and hence we don't make the current line (i) discarded. 	 * We want to discard multimatch lines only when they appear in the 	 * middle of runs with nomatch lines (dis[j] == 0). 	 */
if|if
condition|(
name|rdis0
operator|==
literal|0
condition|)
return|return
literal|0
return|;
for|for
control|(
name|r
operator|=
literal|1
operator|,
name|rdis1
operator|=
literal|0
operator|,
name|rpdis1
operator|=
literal|1
init|;
operator|(
name|i
operator|+
name|r
operator|)
operator|<=
name|e
condition|;
name|r
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|dis
index|[
name|i
operator|+
name|r
index|]
condition|)
name|rdis1
operator|++
expr_stmt|;
elseif|else
if|if
condition|(
name|dis
index|[
name|i
operator|+
name|r
index|]
operator|==
literal|2
condition|)
name|rpdis1
operator|++
expr_stmt|;
else|else
break|break;
block|}
comment|/* 	 * If the run after the line 'i' found only multimatch lines, we 	 * return 0 and hence we don't make the current line (i) discarded. 	 */
if|if
condition|(
name|rdis1
operator|==
literal|0
condition|)
return|return
literal|0
return|;
name|rdis1
operator|+=
name|rdis0
expr_stmt|;
name|rpdis1
operator|+=
name|rpdis0
expr_stmt|;
return|return
name|rpdis1
operator|*
name|XDL_KPDIS_RUN
operator|<
operator|(
name|rpdis1
operator|+
name|rdis1
operator|)
return|;
block|}
end_function

begin_comment
comment|/*  * Try to reduce the problem complexity, discard records that have no  * matches on the other file. Also, lines that have multiple matches  * might be potentially discarded if they happear in a run of discardable.  */
end_comment

begin_function
DECL|function|xdl_cleanup_records
specifier|static
name|int
name|xdl_cleanup_records
parameter_list|(
name|xdfile_t
modifier|*
name|xdf1
parameter_list|,
name|xdfile_t
modifier|*
name|xdf2
parameter_list|)
block|{
name|long
name|i
decl_stmt|,
name|nm
decl_stmt|,
name|rhi
decl_stmt|,
name|nreff
decl_stmt|,
name|mlim
decl_stmt|;
name|unsigned
name|long
name|hav
decl_stmt|;
name|xrecord_t
modifier|*
modifier|*
name|recs
decl_stmt|;
name|xrecord_t
modifier|*
name|rec
decl_stmt|;
name|char
modifier|*
name|dis
decl_stmt|,
modifier|*
name|dis1
decl_stmt|,
modifier|*
name|dis2
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|dis
operator|=
operator|(
name|char
operator|*
operator|)
name|xdl_malloc
argument_list|(
name|xdf1
operator|->
name|nrec
operator|+
name|xdf2
operator|->
name|nrec
operator|+
literal|2
argument_list|)
operator|)
condition|)
block|{
return|return
operator|-
literal|1
return|;
block|}
name|memset
argument_list|(
name|dis
argument_list|,
literal|0
argument_list|,
name|xdf1
operator|->
name|nrec
operator|+
name|xdf2
operator|->
name|nrec
operator|+
literal|2
argument_list|)
expr_stmt|;
name|dis1
operator|=
name|dis
expr_stmt|;
name|dis2
operator|=
name|dis1
operator|+
name|xdf1
operator|->
name|nrec
operator|+
literal|1
expr_stmt|;
if|if
condition|(
operator|(
name|mlim
operator|=
name|xdl_bogosqrt
argument_list|(
name|xdf1
operator|->
name|nrec
argument_list|)
operator|)
operator|>
name|XDL_MAX_EQLIMIT
condition|)
name|mlim
operator|=
name|XDL_MAX_EQLIMIT
expr_stmt|;
for|for
control|(
name|i
operator|=
name|xdf1
operator|->
name|dstart
operator|,
name|recs
operator|=
operator|&
name|xdf1
operator|->
name|recs
index|[
name|xdf1
operator|->
name|dstart
index|]
init|;
name|i
operator|<=
name|xdf1
operator|->
name|dend
condition|;
name|i
operator|++
operator|,
name|recs
operator|++
control|)
block|{
name|hav
operator|=
operator|(
operator|*
name|recs
operator|)
operator|->
name|ha
expr_stmt|;
name|rhi
operator|=
operator|(
name|long
operator|)
name|XDL_HASHLONG
argument_list|(
name|hav
argument_list|,
name|xdf2
operator|->
name|hbits
argument_list|)
expr_stmt|;
for|for
control|(
name|nm
operator|=
literal|0
operator|,
name|rec
operator|=
name|xdf2
operator|->
name|rhash
index|[
name|rhi
index|]
init|;
name|rec
condition|;
name|rec
operator|=
name|rec
operator|->
name|next
control|)
if|if
condition|(
name|rec
operator|->
name|ha
operator|==
name|hav
operator|&&
operator|++
name|nm
operator|==
name|mlim
condition|)
break|break;
name|dis1
index|[
name|i
index|]
operator|=
operator|(
name|nm
operator|==
literal|0
operator|)
condition|?
literal|0
else|:
operator|(
name|nm
operator|>=
name|mlim
operator|)
condition|?
literal|2
else|:
literal|1
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|mlim
operator|=
name|xdl_bogosqrt
argument_list|(
name|xdf2
operator|->
name|nrec
argument_list|)
operator|)
operator|>
name|XDL_MAX_EQLIMIT
condition|)
name|mlim
operator|=
name|XDL_MAX_EQLIMIT
expr_stmt|;
for|for
control|(
name|i
operator|=
name|xdf2
operator|->
name|dstart
operator|,
name|recs
operator|=
operator|&
name|xdf2
operator|->
name|recs
index|[
name|xdf2
operator|->
name|dstart
index|]
init|;
name|i
operator|<=
name|xdf2
operator|->
name|dend
condition|;
name|i
operator|++
operator|,
name|recs
operator|++
control|)
block|{
name|hav
operator|=
operator|(
operator|*
name|recs
operator|)
operator|->
name|ha
expr_stmt|;
name|rhi
operator|=
operator|(
name|long
operator|)
name|XDL_HASHLONG
argument_list|(
name|hav
argument_list|,
name|xdf1
operator|->
name|hbits
argument_list|)
expr_stmt|;
for|for
control|(
name|nm
operator|=
literal|0
operator|,
name|rec
operator|=
name|xdf1
operator|->
name|rhash
index|[
name|rhi
index|]
init|;
name|rec
condition|;
name|rec
operator|=
name|rec
operator|->
name|next
control|)
if|if
condition|(
name|rec
operator|->
name|ha
operator|==
name|hav
operator|&&
operator|++
name|nm
operator|==
name|mlim
condition|)
break|break;
name|dis2
index|[
name|i
index|]
operator|=
operator|(
name|nm
operator|==
literal|0
operator|)
condition|?
literal|0
else|:
operator|(
name|nm
operator|>=
name|mlim
operator|)
condition|?
literal|2
else|:
literal|1
expr_stmt|;
block|}
for|for
control|(
name|nreff
operator|=
literal|0
operator|,
name|i
operator|=
name|xdf1
operator|->
name|dstart
operator|,
name|recs
operator|=
operator|&
name|xdf1
operator|->
name|recs
index|[
name|xdf1
operator|->
name|dstart
index|]
init|;
name|i
operator|<=
name|xdf1
operator|->
name|dend
condition|;
name|i
operator|++
operator|,
name|recs
operator|++
control|)
block|{
if|if
condition|(
name|dis1
index|[
name|i
index|]
operator|==
literal|1
operator|||
operator|(
name|dis1
index|[
name|i
index|]
operator|==
literal|2
operator|&&
operator|!
name|xdl_clean_mmatch
argument_list|(
name|dis1
argument_list|,
name|i
argument_list|,
name|xdf1
operator|->
name|dstart
argument_list|,
name|xdf1
operator|->
name|dend
argument_list|)
operator|)
condition|)
block|{
name|xdf1
operator|->
name|rindex
index|[
name|nreff
index|]
operator|=
name|i
expr_stmt|;
name|xdf1
operator|->
name|ha
index|[
name|nreff
index|]
operator|=
operator|(
operator|*
name|recs
operator|)
operator|->
name|ha
expr_stmt|;
name|nreff
operator|++
expr_stmt|;
block|}
else|else
name|xdf1
operator|->
name|rchg
index|[
name|i
index|]
operator|=
literal|1
expr_stmt|;
block|}
name|xdf1
operator|->
name|nreff
operator|=
name|nreff
expr_stmt|;
for|for
control|(
name|nreff
operator|=
literal|0
operator|,
name|i
operator|=
name|xdf2
operator|->
name|dstart
operator|,
name|recs
operator|=
operator|&
name|xdf2
operator|->
name|recs
index|[
name|xdf2
operator|->
name|dstart
index|]
init|;
name|i
operator|<=
name|xdf2
operator|->
name|dend
condition|;
name|i
operator|++
operator|,
name|recs
operator|++
control|)
block|{
if|if
condition|(
name|dis2
index|[
name|i
index|]
operator|==
literal|1
operator|||
operator|(
name|dis2
index|[
name|i
index|]
operator|==
literal|2
operator|&&
operator|!
name|xdl_clean_mmatch
argument_list|(
name|dis2
argument_list|,
name|i
argument_list|,
name|xdf2
operator|->
name|dstart
argument_list|,
name|xdf2
operator|->
name|dend
argument_list|)
operator|)
condition|)
block|{
name|xdf2
operator|->
name|rindex
index|[
name|nreff
index|]
operator|=
name|i
expr_stmt|;
name|xdf2
operator|->
name|ha
index|[
name|nreff
index|]
operator|=
operator|(
operator|*
name|recs
operator|)
operator|->
name|ha
expr_stmt|;
name|nreff
operator|++
expr_stmt|;
block|}
else|else
name|xdf2
operator|->
name|rchg
index|[
name|i
index|]
operator|=
literal|1
expr_stmt|;
block|}
name|xdf2
operator|->
name|nreff
operator|=
name|nreff
expr_stmt|;
name|xdl_free
argument_list|(
name|dis
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_comment
comment|/*  * Early trim initial and terminal matching records.  */
end_comment

begin_function
DECL|function|xdl_trim_ends
specifier|static
name|int
name|xdl_trim_ends
parameter_list|(
name|xdfile_t
modifier|*
name|xdf1
parameter_list|,
name|xdfile_t
modifier|*
name|xdf2
parameter_list|)
block|{
name|long
name|i
decl_stmt|,
name|lim
decl_stmt|;
name|xrecord_t
modifier|*
modifier|*
name|recs1
decl_stmt|,
modifier|*
modifier|*
name|recs2
decl_stmt|;
name|recs1
operator|=
name|xdf1
operator|->
name|recs
expr_stmt|;
name|recs2
operator|=
name|xdf2
operator|->
name|recs
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
operator|,
name|lim
operator|=
name|XDL_MIN
argument_list|(
name|xdf1
operator|->
name|nrec
argument_list|,
name|xdf2
operator|->
name|nrec
argument_list|)
init|;
name|i
operator|<
name|lim
condition|;
name|i
operator|++
operator|,
name|recs1
operator|++
operator|,
name|recs2
operator|++
control|)
if|if
condition|(
operator|(
operator|*
name|recs1
operator|)
operator|->
name|ha
operator|!=
operator|(
operator|*
name|recs2
operator|)
operator|->
name|ha
condition|)
break|break;
name|xdf1
operator|->
name|dstart
operator|=
name|xdf2
operator|->
name|dstart
operator|=
name|i
expr_stmt|;
name|recs1
operator|=
name|xdf1
operator|->
name|recs
operator|+
name|xdf1
operator|->
name|nrec
operator|-
literal|1
expr_stmt|;
name|recs2
operator|=
name|xdf2
operator|->
name|recs
operator|+
name|xdf2
operator|->
name|nrec
operator|-
literal|1
expr_stmt|;
for|for
control|(
name|lim
operator|-=
name|i
operator|,
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|lim
condition|;
name|i
operator|++
operator|,
name|recs1
operator|--
operator|,
name|recs2
operator|--
control|)
if|if
condition|(
operator|(
operator|*
name|recs1
operator|)
operator|->
name|ha
operator|!=
operator|(
operator|*
name|recs2
operator|)
operator|->
name|ha
condition|)
break|break;
name|xdf1
operator|->
name|dend
operator|=
name|xdf1
operator|->
name|nrec
operator|-
name|i
operator|-
literal|1
expr_stmt|;
name|xdf2
operator|->
name|dend
operator|=
name|xdf2
operator|->
name|nrec
operator|-
name|i
operator|-
literal|1
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|xdl_optimize_ctxs
specifier|static
name|int
name|xdl_optimize_ctxs
parameter_list|(
name|xdfile_t
modifier|*
name|xdf1
parameter_list|,
name|xdfile_t
modifier|*
name|xdf2
parameter_list|)
block|{
if|if
condition|(
name|xdl_trim_ends
argument_list|(
name|xdf1
argument_list|,
name|xdf2
argument_list|)
operator|<
literal|0
operator|||
name|xdl_cleanup_records
argument_list|(
name|xdf1
argument_list|,
name|xdf2
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

end_unit

