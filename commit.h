begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|COMMIT_H
end_ifndef

begin_define
DECL|macro|COMMIT_H
define|#
directive|define
name|COMMIT_H
end_define

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_include
include|#
directive|include
file|"tree.h"
end_include

begin_struct
DECL|struct|commit_list
struct|struct
name|commit_list
block|{
DECL|member|item
name|struct
name|commit
modifier|*
name|item
decl_stmt|;
DECL|member|next
name|struct
name|commit_list
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|commit
struct|struct
name|commit
block|{
DECL|member|object
name|struct
name|object
name|object
decl_stmt|;
DECL|member|date
name|unsigned
name|long
name|date
decl_stmt|;
DECL|member|parents
name|struct
name|commit_list
modifier|*
name|parents
decl_stmt|;
DECL|member|tree
name|struct
name|tree
modifier|*
name|tree
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
specifier|extern
specifier|const
name|char
modifier|*
name|commit_type
decl_stmt|;
end_decl_stmt

begin_function_decl
name|struct
name|commit
modifier|*
name|lookup_commit
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_commit
parameter_list|(
name|struct
name|commit
modifier|*
name|item
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|commit_list_insert
parameter_list|(
name|struct
name|commit
modifier|*
name|item
parameter_list|,
name|struct
name|commit_list
modifier|*
modifier|*
name|list_p
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|free_commit_list
parameter_list|(
name|struct
name|commit_list
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|sort_by_date
parameter_list|(
name|struct
name|commit_list
modifier|*
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Removes the first commit from a list sorted by date, and adds all  * of its parents.  **/
end_comment

begin_function_decl
name|struct
name|commit
modifier|*
name|pop_most_recent_commit
parameter_list|(
name|struct
name|commit_list
modifier|*
modifier|*
name|list
parameter_list|,
name|unsigned
name|int
name|mark
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* COMMIT_H */
end_comment

end_unit

