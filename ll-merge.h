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

begin_endif
endif|#
directive|endif
end_endif

end_unit

