begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|URL_MATCH_H
end_ifndef

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_struct
DECL|struct|url_info
struct|struct
name|url_info
block|{
comment|/* normalized url on success, must be freed, otherwise NULL */
DECL|member|url
name|char
modifier|*
name|url
decl_stmt|;
comment|/* if !url, a brief reason for the failure, otherwise NULL */
DECL|member|err
specifier|const
name|char
modifier|*
name|err
decl_stmt|;
comment|/* the rest of the fields are only set if url != NULL */
DECL|member|url_len
name|size_t
name|url_len
decl_stmt|;
comment|/* total length of url (which is now normalized) */
DECL|member|scheme_len
name|size_t
name|scheme_len
decl_stmt|;
comment|/* length of scheme name (excluding final :) */
DECL|member|user_off
name|size_t
name|user_off
decl_stmt|;
comment|/* offset into url to start of user name (0 => none) */
DECL|member|user_len
name|size_t
name|user_len
decl_stmt|;
comment|/* length of user name; if user_off != 0 but 				   user_len == 0, an empty user name was given */
DECL|member|passwd_off
name|size_t
name|passwd_off
decl_stmt|;
comment|/* offset into url to start of passwd (0 => none) */
DECL|member|passwd_len
name|size_t
name|passwd_len
decl_stmt|;
comment|/* length of passwd; if passwd_off != 0 but 				   passwd_len == 0, an empty passwd was given */
DECL|member|host_off
name|size_t
name|host_off
decl_stmt|;
comment|/* offset into url to start of host name (0 => none) */
DECL|member|host_len
name|size_t
name|host_len
decl_stmt|;
comment|/* length of host name; this INCLUDES any ':portnum'; 				 * file urls may have host_len == 0 */
DECL|member|port_len
name|size_t
name|port_len
decl_stmt|;
comment|/* if a portnum is present (port_len != 0), it has 				 * this length (excluding the leading ':') at the 				 * end of the host name (always 0 for file urls) */
DECL|member|path_off
name|size_t
name|path_off
decl_stmt|;
comment|/* offset into url to the start of the url path; 				 * this will always point to a '/' character 				 * after the url has been normalized */
DECL|member|path_len
name|size_t
name|path_len
decl_stmt|;
comment|/* length of path portion excluding any trailing 				 * '?...' and '#...' portion; will always be>= 1 */
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|char
modifier|*
name|url_normalize
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|struct
name|url_info
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|match_urls
parameter_list|(
specifier|const
name|struct
name|url_info
modifier|*
name|url
parameter_list|,
specifier|const
name|struct
name|url_info
modifier|*
name|url_prefix
parameter_list|,
name|int
modifier|*
name|exactusermatch
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|urlmatch_item
struct|struct
name|urlmatch_item
block|{
DECL|member|matched_len
name|size_t
name|matched_len
decl_stmt|;
DECL|member|user_matched
name|char
name|user_matched
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|urlmatch_config
struct|struct
name|urlmatch_config
block|{
DECL|member|vars
name|struct
name|string_list
name|vars
decl_stmt|;
DECL|member|url
name|struct
name|url_info
name|url
decl_stmt|;
DECL|member|section
specifier|const
name|char
modifier|*
name|section
decl_stmt|;
DECL|member|key
specifier|const
name|char
modifier|*
name|key
decl_stmt|;
DECL|member|cb
name|void
modifier|*
name|cb
decl_stmt|;
DECL|member|collect_fn
name|int
function_decl|(
modifier|*
name|collect_fn
function_decl|)
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
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
function_decl|;
DECL|member|cascade_fn
name|int
function_decl|(
modifier|*
name|cascade_fn
function_decl|)
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
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
function_decl|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|int
name|urlmatch_config_entry
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
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* URL_MATCH_H */
end_comment

end_unit

