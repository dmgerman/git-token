begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|TAG_H
end_ifndef

begin_define
DECL|macro|TAG_H
define|#
directive|define
name|TAG_H
end_define

begin_include
include|#
directive|include
file|"object.h"
end_include

begin_decl_stmt
specifier|extern
specifier|const
name|char
modifier|*
name|tag_type
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|tag
struct|struct
name|tag
block|{
DECL|member|object
name|struct
name|object
name|object
decl_stmt|;
DECL|member|tagged
name|struct
name|object
modifier|*
name|tagged
decl_stmt|;
DECL|member|tag
name|char
modifier|*
name|tag
decl_stmt|;
DECL|member|signature
name|char
modifier|*
name|signature
decl_stmt|;
comment|/* not actually implemented */
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|struct
name|tag
modifier|*
name|lookup_tag
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|parse_tag_buffer
parameter_list|(
name|struct
name|tag
modifier|*
name|item
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|parse_tag
parameter_list|(
name|struct
name|tag
modifier|*
name|item
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* TAG_H */
end_comment

end_unit

