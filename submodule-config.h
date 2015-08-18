begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|SUBMODULE_CONFIG_CACHE_H
end_ifndef

begin_define
DECL|macro|SUBMODULE_CONFIG_CACHE_H
define|#
directive|define
name|SUBMODULE_CONFIG_CACHE_H
end_define

begin_include
include|#
directive|include
file|"hashmap.h"
end_include

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_comment
comment|/*  * Submodule entry containing the information about a certain submodule  * in a certain revision.  */
end_comment

begin_struct
DECL|struct|submodule
struct|struct
name|submodule
block|{
DECL|member|path
specifier|const
name|char
modifier|*
name|path
decl_stmt|;
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|url
specifier|const
name|char
modifier|*
name|url
decl_stmt|;
DECL|member|fetch_recurse
name|int
name|fetch_recurse
decl_stmt|;
DECL|member|ignore
specifier|const
name|char
modifier|*
name|ignore
decl_stmt|;
comment|/* the sha1 blob id of the responsible .gitmodules file */
DECL|member|gitmodules_sha1
name|unsigned
name|char
name|gitmodules_sha1
index|[
literal|20
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|parse_submodule_config_option
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|const
name|struct
name|submodule
modifier|*
name|submodule_from_name
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|commit_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|const
name|struct
name|submodule
modifier|*
name|submodule_from_path
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|commit_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|submodule_free
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* SUBMODULE_CONFIG_H */
end_comment

end_unit

