begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|STATUS_H
end_ifndef

begin_define
DECL|macro|STATUS_H
define|#
directive|define
name|STATUS_H
end_define

begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_include
include|#
directive|include
file|"color.h"
end_include

begin_enum
DECL|enum|color_wt_status
enum|enum
name|color_wt_status
block|{
DECL|enumerator|WT_STATUS_HEADER
name|WT_STATUS_HEADER
init|=
literal|0
block|,
DECL|enumerator|WT_STATUS_UPDATED
name|WT_STATUS_UPDATED
block|,
DECL|enumerator|WT_STATUS_CHANGED
name|WT_STATUS_CHANGED
block|,
DECL|enumerator|WT_STATUS_UNTRACKED
name|WT_STATUS_UNTRACKED
block|,
DECL|enumerator|WT_STATUS_NOBRANCH
name|WT_STATUS_NOBRANCH
block|,
DECL|enumerator|WT_STATUS_UNMERGED
name|WT_STATUS_UNMERGED
block|, }
enum|;
end_enum

begin_enum
DECL|enum|untracked_status_type
enum|enum
name|untracked_status_type
block|{
DECL|enumerator|SHOW_NO_UNTRACKED_FILES
name|SHOW_NO_UNTRACKED_FILES
block|,
DECL|enumerator|SHOW_NORMAL_UNTRACKED_FILES
name|SHOW_NORMAL_UNTRACKED_FILES
block|,
DECL|enumerator|SHOW_ALL_UNTRACKED_FILES
name|SHOW_ALL_UNTRACKED_FILES
block|}
enum|;
end_enum

begin_struct
DECL|struct|wt_status_change_data
struct|struct
name|wt_status_change_data
block|{
DECL|member|worktree_status
name|int
name|worktree_status
decl_stmt|;
DECL|member|index_status
name|int
name|index_status
decl_stmt|;
DECL|member|stagemask
name|int
name|stagemask
decl_stmt|;
DECL|member|head_path
name|char
modifier|*
name|head_path
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|wt_status
struct|struct
name|wt_status
block|{
DECL|member|is_initial
name|int
name|is_initial
decl_stmt|;
DECL|member|branch
name|char
modifier|*
name|branch
decl_stmt|;
DECL|member|reference
specifier|const
name|char
modifier|*
name|reference
decl_stmt|;
DECL|member|verbose
name|int
name|verbose
decl_stmt|;
DECL|member|amend
name|int
name|amend
decl_stmt|;
DECL|member|nowarn
name|int
name|nowarn
decl_stmt|;
DECL|member|use_color
name|int
name|use_color
decl_stmt|;
DECL|member|relative_paths
name|int
name|relative_paths
decl_stmt|;
DECL|member|submodule_summary
name|int
name|submodule_summary
decl_stmt|;
DECL|member|show_untracked_files
name|enum
name|untracked_status_type
name|show_untracked_files
decl_stmt|;
DECL|member|color_palette
name|char
name|color_palette
index|[
name|WT_STATUS_UNMERGED
operator|+
literal|1
index|]
index|[
name|COLOR_MAXLEN
index|]
decl_stmt|;
comment|/* These are computed during processing of the individual sections */
DECL|member|commitable
name|int
name|commitable
decl_stmt|;
DECL|member|workdir_dirty
name|int
name|workdir_dirty
decl_stmt|;
DECL|member|workdir_untracked
name|int
name|workdir_untracked
decl_stmt|;
DECL|member|index_file
specifier|const
name|char
modifier|*
name|index_file
decl_stmt|;
DECL|member|fp
name|FILE
modifier|*
name|fp
decl_stmt|;
DECL|member|prefix
specifier|const
name|char
modifier|*
name|prefix
decl_stmt|;
DECL|member|change
name|struct
name|string_list
name|change
decl_stmt|;
DECL|member|untracked
name|struct
name|string_list
name|untracked
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|void
name|wt_status_prepare
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|wt_status_print
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|wt_status_collect
parameter_list|(
name|struct
name|wt_status
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* STATUS_H */
end_comment

end_unit

