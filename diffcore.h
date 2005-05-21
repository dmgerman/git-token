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
value|10000
end_define

begin_define
DECL|macro|DEFAULT_MINIMUM_SCORE
define|#
directive|define
name|DEFAULT_MINIMUM_SCORE
value|5000
end_define

begin_define
DECL|macro|RENAME_DST_MATCHED
define|#
directive|define
name|RENAME_DST_MATCHED
value|01
end_define

begin_define
DECL|macro|RENAME_SRC_GONE
define|#
directive|define
name|RENAME_SRC_GONE
value|02
end_define

begin_define
DECL|macro|RENAME_SCORE_SHIFT
define|#
directive|define
name|RENAME_SCORE_SHIFT
value|8
end_define

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
DECL|member|file_valid
name|unsigned
name|file_valid
range|:
literal|1
decl_stmt|;
comment|/* if false the file does not exist */
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
DECL|member|xfrm_msg
name|char
modifier|*
name|xfrm_msg
decl_stmt|;
DECL|member|orig_order
name|int
name|orig_order
decl_stmt|;
comment|/* the original order of insertion into the queue */
DECL|member|xfrm_work
name|int
name|xfrm_work
decl_stmt|;
comment|/* for use by tramsformers, not by diffcore */
block|}
struct|;
end_struct

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
name|diff_detect_rename
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
parameter_list|,
name|int
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|diff_pickaxe
parameter_list|(
name|struct
name|diff_queue_struct
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

