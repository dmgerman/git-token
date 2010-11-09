begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|NOTES_MERGE_H
end_ifndef

begin_define
DECL|macro|NOTES_MERGE_H
define|#
directive|define
name|NOTES_MERGE_H
end_define

begin_enum
DECL|enum|notes_merge_verbosity
enum|enum
name|notes_merge_verbosity
block|{
DECL|enumerator|NOTES_MERGE_VERBOSITY_DEFAULT
name|NOTES_MERGE_VERBOSITY_DEFAULT
init|=
literal|2
block|,
DECL|enumerator|NOTES_MERGE_VERBOSITY_MAX
name|NOTES_MERGE_VERBOSITY_MAX
init|=
literal|5
block|}
enum|;
end_enum

begin_struct
DECL|struct|notes_merge_options
struct|struct
name|notes_merge_options
block|{
DECL|member|local_ref
specifier|const
name|char
modifier|*
name|local_ref
decl_stmt|;
DECL|member|remote_ref
specifier|const
name|char
modifier|*
name|remote_ref
decl_stmt|;
DECL|member|verbosity
name|int
name|verbosity
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|void
name|init_notes_merge_options
parameter_list|(
name|struct
name|notes_merge_options
modifier|*
name|o
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Merge notes from o->remote_ref into o->local_ref  *  * The commits given by the two refs are merged, producing one of the following  * outcomes:  *  * 1. The merge trivially results in an existing commit (e.g. fast-forward or  *    already-up-to-date). The SHA1 of the result is written into 'result_sha1'  *    and 0 is returned.  * 2. The merge fails. result_sha1 is set to null_sha1, and non-zero returned.  *  * Both o->local_ref and o->remote_ref must be given (non-NULL), but either ref  * (although not both) may refer to a non-existing notes ref, in which case  * that notes ref is interpreted as an empty notes tree, and the merge  * trivially results in what the other ref points to.  */
end_comment

begin_function_decl
name|int
name|notes_merge
parameter_list|(
name|struct
name|notes_merge_options
modifier|*
name|o
parameter_list|,
name|unsigned
name|char
modifier|*
name|result_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

