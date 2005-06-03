begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2005 Junio C Hamano  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_DIFFCORE_H_
end_ifndef

begin_define
DECL|macro|_DIFFCORE_H_
define|#
directive|define
name|_DIFFCORE_H_
end_define

begin_comment
comment|/* This header file is internal between diff.c and its diff transformers  * (e.g. diffcore-rename, diffcore-pickaxe).  Never include this header  * in anything else.  */
end_comment

begin_define
DECL|macro|MAX_SCORE
define|#
directive|define
name|MAX_SCORE
value|60000
end_define

begin_define
DECL|macro|DEFAULT_RENAME_SCORE
define|#
directive|define
name|DEFAULT_RENAME_SCORE
value|30000
end_define

begin_comment
DECL|macro|DEFAULT_RENAME_SCORE
comment|/* rename/copy similarity minimum (50%) */
end_comment

begin_define
DECL|macro|DEFAULT_BREAK_SCORE
define|#
directive|define
name|DEFAULT_BREAK_SCORE
value|59400
end_define

begin_comment
DECL|macro|DEFAULT_BREAK_SCORE
comment|/* minimum for break to happen (99%)*/
end_comment

begin_struct
DECL|struct|diff_filespec
struct|struct
name|diff_filespec
block|{
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|path
name|char
modifier|*
name|path
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|size
name|unsigned
name|long
name|size
decl_stmt|;
DECL|member|xfrm_flags
name|int
name|xfrm_flags
decl_stmt|;
comment|/* for use by the xfrm */
DECL|member|mode
name|unsigned
name|short
name|mode
decl_stmt|;
comment|/* file mode */
DECL|member|sha1_valid
name|unsigned
name|sha1_valid
range|:
literal|1
decl_stmt|;
comment|/* if true, use sha1 and trust mode; 				  * if false, use the name and read from 				  * the filesystem. 				  */
DECL|macro|DIFF_FILE_VALID
define|#
directive|define
name|DIFF_FILE_VALID
parameter_list|(
name|spec
parameter_list|)
value|(((spec)->mode) != 0)
DECL|member|should_free
name|unsigned
name|should_free
range|:
literal|1
decl_stmt|;
comment|/* data should be free()'ed */
DECL|member|should_munmap
name|unsigned
name|should_munmap
range|:
literal|1
decl_stmt|;
comment|/* data should be munmap()'ed */
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|struct
name|diff_filespec
modifier|*
name|alloc_filespec
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|fill_filespec
parameter_list|(
name|struct
name|diff_filespec
modifier|*
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
parameter_list|,
name|unsigned
name|short
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|diff_populate_filespec
parameter_list|(
name|struct
name|diff_filespec
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_free_filespec_data
parameter_list|(
name|struct
name|diff_filespec
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|diff_filepair
struct|struct
name|diff_filepair
block|{
DECL|member|one
name|struct
name|diff_filespec
modifier|*
name|one
decl_stmt|;
DECL|member|two
name|struct
name|diff_filespec
modifier|*
name|two
decl_stmt|;
DECL|member|score
name|unsigned
name|short
name|int
name|score
decl_stmt|;
DECL|member|status
name|char
name|status
decl_stmt|;
comment|/* M C R N D U (see Documentation/diff-format.txt) */
DECL|member|source_stays
name|unsigned
name|source_stays
range|:
literal|1
decl_stmt|;
comment|/* all of R/C are copies */
DECL|member|broken_pair
name|unsigned
name|broken_pair
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|DIFF_PAIR_UNMERGED
define|#
directive|define
name|DIFF_PAIR_UNMERGED
parameter_list|(
name|p
parameter_list|)
define|\
value|(!DIFF_FILE_VALID((p)->one)&& !DIFF_FILE_VALID((p)->two))
end_define

begin_define
DECL|macro|DIFF_PAIR_RENAME
define|#
directive|define
name|DIFF_PAIR_RENAME
parameter_list|(
name|p
parameter_list|)
value|(strcmp((p)->one->path, (p)->two->path))
end_define

begin_define
DECL|macro|DIFF_PAIR_BROKEN
define|#
directive|define
name|DIFF_PAIR_BROKEN
parameter_list|(
name|p
parameter_list|)
define|\
value|( (!DIFF_FILE_VALID((p)->one) != !DIFF_FILE_VALID((p)->two))&& \ 	  ((p)->broken_pair != 0) )
end_define

begin_define
DECL|macro|DIFF_PAIR_TYPE_CHANGED
define|#
directive|define
name|DIFF_PAIR_TYPE_CHANGED
parameter_list|(
name|p
parameter_list|)
define|\
value|((S_IFMT& (p)->one->mode) != (S_IFMT& (p)->two->mode))
end_define

begin_define
DECL|macro|DIFF_PAIR_MODE_CHANGED
define|#
directive|define
name|DIFF_PAIR_MODE_CHANGED
parameter_list|(
name|p
parameter_list|)
value|((p)->one->mode != (p)->two->mode)
end_define

begin_function_decl
specifier|extern
name|void
name|diff_free_filepair
parameter_list|(
name|struct
name|diff_filepair
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|diff_unmodified_pair
parameter_list|(
name|struct
name|diff_filepair
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|diff_queue_struct
struct|struct
name|diff_queue_struct
block|{
DECL|member|queue
name|struct
name|diff_filepair
modifier|*
modifier|*
name|queue
decl_stmt|;
DECL|member|alloc
name|int
name|alloc
decl_stmt|;
DECL|member|nr
name|int
name|nr
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
specifier|extern
name|struct
name|diff_queue_struct
name|diff_queued_diff
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|extern
name|struct
name|diff_filepair
modifier|*
name|diff_queue
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
parameter_list|,
name|struct
name|diff_filespec
modifier|*
parameter_list|,
name|struct
name|diff_filespec
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_q
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
parameter_list|,
name|struct
name|diff_filepair
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|DIFF_DEBUG
define|#
directive|define
name|DIFF_DEBUG
value|0
end_define

begin_if
if|#
directive|if
name|DIFF_DEBUG
end_if

begin_function_decl
name|void
name|diff_debug_filespec
parameter_list|(
name|struct
name|diff_filespec
modifier|*
parameter_list|,
name|int
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|diff_debug_filepair
parameter_list|(
specifier|const
name|struct
name|diff_filepair
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|diff_debug_queue
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|struct
name|diff_queue_struct
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|diff_debug_filespec
define|#
directive|define
name|diff_debug_filespec
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|,
name|c
parameter_list|)
value|do {} while(0)
end_define

begin_define
DECL|macro|diff_debug_filepair
define|#
directive|define
name|diff_debug_filepair
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
value|do {} while(0)
end_define

begin_define
DECL|macro|diff_debug_queue
define|#
directive|define
name|diff_debug_queue
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|)
value|do {} while(0)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

end_unit

