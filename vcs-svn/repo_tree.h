begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|REPO_TREE_H_
end_ifndef

begin_define
DECL|macro|REPO_TREE_H_
define|#
directive|define
name|REPO_TREE_H_
end_define

begin_include
include|#
directive|include
file|"git-compat-util.h"
end_include

begin_define
DECL|macro|REPO_MODE_DIR
define|#
directive|define
name|REPO_MODE_DIR
value|0040000
end_define

begin_define
DECL|macro|REPO_MODE_BLB
define|#
directive|define
name|REPO_MODE_BLB
value|0100644
end_define

begin_define
DECL|macro|REPO_MODE_EXE
define|#
directive|define
name|REPO_MODE_EXE
value|0100755
end_define

begin_define
DECL|macro|REPO_MODE_LNK
define|#
directive|define
name|REPO_MODE_LNK
value|0120000
end_define

begin_define
DECL|macro|REPO_MAX_PATH_LEN
define|#
directive|define
name|REPO_MAX_PATH_LEN
value|4096
end_define

begin_define
DECL|macro|REPO_MAX_PATH_DEPTH
define|#
directive|define
name|REPO_MAX_PATH_DEPTH
value|1000
end_define

begin_function_decl
name|uint32_t
name|next_blob_mark
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|repo_copy
parameter_list|(
name|uint32_t
name|revision
parameter_list|,
specifier|const
name|uint32_t
modifier|*
name|src
parameter_list|,
specifier|const
name|uint32_t
modifier|*
name|dst
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|repo_add
parameter_list|(
name|uint32_t
modifier|*
name|path
parameter_list|,
name|uint32_t
name|mode
parameter_list|,
name|uint32_t
name|blob_mark
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|uint32_t
name|repo_read_path
parameter_list|(
specifier|const
name|uint32_t
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|uint32_t
name|repo_read_mode
parameter_list|(
specifier|const
name|uint32_t
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|repo_delete
parameter_list|(
name|uint32_t
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|repo_commit
parameter_list|(
name|uint32_t
name|revision
parameter_list|,
name|uint32_t
name|author
parameter_list|,
name|char
modifier|*
name|log
parameter_list|,
name|uint32_t
name|uuid
parameter_list|,
name|uint32_t
name|url
parameter_list|,
name|long
name|unsigned
name|timestamp
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|repo_diff
parameter_list|(
name|uint32_t
name|r1
parameter_list|,
name|uint32_t
name|r2
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|repo_init
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|repo_reset
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

