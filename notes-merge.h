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

begin_include
include|#
directive|include
file|"notes-utils.h"
end_include

begin_define
DECL|macro|NOTES_MERGE_WORKTREE
define|#
directive|define
name|NOTES_MERGE_WORKTREE
value|"NOTES_MERGE_WORKTREE"
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
DECL|member|commit_msg
name|struct
name|strbuf
name|commit_msg
decl_stmt|;
DECL|member|verbosity
name|int
name|verbosity
decl_stmt|;
DECL|member|strategy
name|enum
name|notes_merge_strategy
name|strategy
decl_stmt|;
DECL|member|has_worktree
name|unsigned
name|has_worktree
range|:
literal|1
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
comment|/*  * Merge notes from o->remote_ref into o->local_ref  *  * The given notes_tree 'local_tree' must be the notes_tree referenced by the  * o->local_ref. This is the notes_tree in which the object-level merge is  * performed.  *  * The commits given by the two refs are merged, producing one of the following  * outcomes:  *  * 1. The merge trivially results in an existing commit (e.g. fast-forward or  *    already-up-to-date). 'local_tree' is untouched, the SHA1 of the result  *    is written into 'result_sha1' and 0 is returned.  * 2. The merge successfully completes, producing a merge commit. local_tree  *    contains the updated notes tree, the SHA1 of the resulting commit is  *    written into 'result_sha1', and 1 is returned.  * 3. The merge results in conflicts. This is similar to #2 in that the  *    partial merge result (i.e. merge result minus the unmerged entries)  *    are stored in 'local_tree', and the SHA1 or the resulting commit  *    (to be amended when the conflicts have been resolved) is written into  *    'result_sha1'. The unmerged entries are written into the  *    .git/NOTES_MERGE_WORKTREE directory with conflict markers.  *    -1 is returned.  *  * Both o->local_ref and o->remote_ref must be given (non-NULL), but either ref  * (although not both) may refer to a non-existing notes ref, in which case  * that notes ref is interpreted as an empty notes tree, and the merge  * trivially results in what the other ref points to.  */
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
name|struct
name|notes_tree
modifier|*
name|local_tree
parameter_list|,
name|unsigned
name|char
modifier|*
name|result_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Finalize conflict resolution from an earlier notes_merge()  *  * The given notes tree 'partial_tree' must be the notes_tree corresponding to  * the given 'partial_commit', the partial result commit created by a previous  * call to notes_merge().  *  * This function will add the (now resolved) notes in .git/NOTES_MERGE_WORKTREE  * to 'partial_tree', and create a final notes merge commit, the SHA1 of which  * will be stored in 'result_sha1'.  */
end_comment

begin_function_decl
name|int
name|notes_merge_commit
parameter_list|(
name|struct
name|notes_merge_options
modifier|*
name|o
parameter_list|,
name|struct
name|notes_tree
modifier|*
name|partial_tree
parameter_list|,
name|struct
name|commit
modifier|*
name|partial_commit
parameter_list|,
name|unsigned
name|char
modifier|*
name|result_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Abort conflict resolution from an earlier notes_merge()  *  * Removes the notes merge worktree in .git/NOTES_MERGE_WORKTREE.  */
end_comment

begin_function_decl
name|int
name|notes_merge_abort
parameter_list|(
name|struct
name|notes_merge_options
modifier|*
name|o
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

