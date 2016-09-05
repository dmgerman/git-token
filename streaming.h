begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2011, Google Inc.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|STREAMING_H
end_ifndef

begin_define
DECL|macro|STREAMING_H
define|#
directive|define
name|STREAMING_H
value|1
end_define

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_comment
comment|/* opaque */
end_comment

begin_struct_decl
struct_decl|struct
name|git_istream
struct_decl|;
end_struct_decl

begin_function_decl
specifier|extern
name|struct
name|git_istream
modifier|*
name|open_istream
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
parameter_list|,
name|enum
name|object_type
modifier|*
parameter_list|,
name|unsigned
name|long
modifier|*
parameter_list|,
name|struct
name|stream_filter
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|close_istream
parameter_list|(
name|struct
name|git_istream
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|ssize_t
name|read_istream
parameter_list|(
name|struct
name|git_istream
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|,
name|size_t
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|stream_blob_to_fd
parameter_list|(
name|int
name|fd
parameter_list|,
specifier|const
name|struct
name|object_id
modifier|*
parameter_list|,
name|struct
name|stream_filter
modifier|*
parameter_list|,
name|int
name|can_seek
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* STREAMING_H */
end_comment

end_unit

