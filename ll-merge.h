begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Low level 3-way in-core file merge.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|LL_MERGE_H
end_ifndef

begin_define
DECL|macro|LL_MERGE_H
define|#
directive|define
name|LL_MERGE_H
end_define

begin_define
DECL|macro|LL_OPT_VIRTUAL_ANCESTOR
define|#
directive|define
name|LL_OPT_VIRTUAL_ANCESTOR
value|(1<< 0)
end_define

begin_define
DECL|macro|LL_OPT_FAVOR_MASK
define|#
directive|define
name|LL_OPT_FAVOR_MASK
value|((1<< 1) | (1<< 2))
end_define

begin_define
DECL|macro|LL_OPT_FAVOR_SHIFT
define|#
directive|define
name|LL_OPT_FAVOR_SHIFT
value|1
end_define

begin_function
DECL|function|ll_opt_favor
specifier|static
specifier|inline
name|int
name|ll_opt_favor
parameter_list|(
name|int
name|flag
parameter_list|)
block|{
return|return
operator|(
name|flag
operator|&
name|LL_OPT_FAVOR_MASK
operator|)
operator|>>
name|LL_OPT_FAVOR_SHIFT
return|;
block|}
end_function

begin_function
DECL|function|create_ll_flag
specifier|static
specifier|inline
name|int
name|create_ll_flag
parameter_list|(
name|int
name|favor
parameter_list|)
block|{
return|return
operator|(
operator|(
name|favor
operator|<<
name|LL_OPT_FAVOR_SHIFT
operator|)
operator|&
name|LL_OPT_FAVOR_MASK
operator|)
return|;
block|}
end_function

begin_function_decl
name|int
name|ll_merge
parameter_list|(
name|mmbuffer_t
modifier|*
name|result_buf
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|mmfile_t
modifier|*
name|ancestor
parameter_list|,
specifier|const
name|char
modifier|*
name|ancestor_label
parameter_list|,
name|mmfile_t
modifier|*
name|ours
parameter_list|,
specifier|const
name|char
modifier|*
name|our_label
parameter_list|,
name|mmfile_t
modifier|*
name|theirs
parameter_list|,
specifier|const
name|char
modifier|*
name|their_label
parameter_list|,
name|int
name|flag
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ll_merge_marker_size
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

