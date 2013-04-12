begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|LINE_LOG_H
end_ifndef

begin_define
DECL|macro|LINE_LOG_H
define|#
directive|define
name|LINE_LOG_H
end_define

begin_include
include|#
directive|include
file|"diffcore.h"
end_include

begin_struct_decl
struct_decl|struct
name|rev_info
struct_decl|;
end_struct_decl

begin_struct_decl
struct_decl|struct
name|commit
struct_decl|;
end_struct_decl

begin_comment
comment|/* A range [start,end].  Lines are numbered starting at 0, and the  * ranges include start but exclude end. */
end_comment

begin_struct
DECL|struct|range
struct|struct
name|range
block|{
DECL|member|start
DECL|member|end
name|long
name|start
decl_stmt|,
name|end
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* A set of ranges.  The ranges must always be disjoint and sorted. */
end_comment

begin_struct
DECL|struct|range_set
struct|struct
name|range_set
block|{
DECL|member|alloc
DECL|member|nr
name|int
name|alloc
decl_stmt|,
name|nr
decl_stmt|;
DECL|member|ranges
name|struct
name|range
modifier|*
name|ranges
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* A diff, encoded as the set of pre- and post-image ranges where the  * files differ. A pair of ranges corresponds to a hunk. */
end_comment

begin_struct
DECL|struct|diff_ranges
struct|struct
name|diff_ranges
block|{
DECL|member|parent
name|struct
name|range_set
name|parent
decl_stmt|;
DECL|member|target
name|struct
name|range_set
name|target
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* Linked list of interesting files and their associated ranges.  The  * list must be kept sorted by path.  *  * For simplicity, even though this is highly redundant, each  * line_log_data owns its 'path'.  */
end_comment

begin_struct
DECL|struct|line_log_data
struct|struct
name|line_log_data
block|{
DECL|member|next
name|struct
name|line_log_data
modifier|*
name|next
decl_stmt|;
DECL|member|path
name|char
modifier|*
name|path
decl_stmt|;
DECL|member|status
name|char
name|status
decl_stmt|;
DECL|member|ranges
name|struct
name|range_set
name|ranges
decl_stmt|;
DECL|member|arg_alloc
DECL|member|arg_nr
name|int
name|arg_alloc
decl_stmt|,
name|arg_nr
decl_stmt|;
DECL|member|args
specifier|const
name|char
modifier|*
modifier|*
name|args
decl_stmt|;
DECL|member|pair
name|struct
name|diff_filepair
modifier|*
name|pair
decl_stmt|;
DECL|member|diff
name|struct
name|diff_ranges
name|diff
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|void
name|line_log_data_init
parameter_list|(
name|struct
name|line_log_data
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|line_log_init
parameter_list|(
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|struct
name|string_list
modifier|*
name|args
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|line_log_filter
parameter_list|(
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|line_log_print
parameter_list|(
name|struct
name|rev_info
modifier|*
name|rev
parameter_list|,
name|struct
name|commit
modifier|*
name|commit
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* LINE_LOG_H */
end_comment

end_unit

