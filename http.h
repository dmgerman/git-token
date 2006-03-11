begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|HTTP_H
end_ifndef

begin_define
DECL|macro|HTTP_H
define|#
directive|define
name|HTTP_H
end_define

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|<curl/curl.h>
end_include

begin_include
include|#
directive|include
file|<curl/easy.h>
end_include

begin_if
if|#
directive|if
name|LIBCURL_VERSION_NUM
operator|>=
literal|0x070908
end_if

begin_define
DECL|macro|USE_CURL_MULTI
define|#
directive|define
name|USE_CURL_MULTI
end_define

begin_define
DECL|macro|DEFAULT_MAX_REQUESTS
define|#
directive|define
name|DEFAULT_MAX_REQUESTS
value|5
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|LIBCURL_VERSION_NUM
operator|<
literal|0x070704
end_if

begin_define
DECL|macro|curl_global_cleanup
define|#
directive|define
name|curl_global_cleanup
parameter_list|()
value|do {
comment|/* nothing */
value|} while(0)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|LIBCURL_VERSION_NUM
operator|<
literal|0x070800
end_if

begin_define
DECL|macro|curl_global_init
define|#
directive|define
name|curl_global_init
parameter_list|(
name|a
parameter_list|)
value|do {
comment|/* nothing */
value|} while(0)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|LIBCURL_VERSION_NUM
operator|<
literal|0x070c04
end_if

begin_define
DECL|macro|NO_CURL_EASY_DUPHANDLE
define|#
directive|define
name|NO_CURL_EASY_DUPHANDLE
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_struct
DECL|struct|slot_results
struct|struct
name|slot_results
block|{
DECL|member|curl_result
name|CURLcode
name|curl_result
decl_stmt|;
DECL|member|http_code
name|long
name|http_code
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|active_request_slot
struct|struct
name|active_request_slot
block|{
DECL|member|curl
name|CURL
modifier|*
name|curl
decl_stmt|;
DECL|member|local
name|FILE
modifier|*
name|local
decl_stmt|;
DECL|member|in_use
name|int
name|in_use
decl_stmt|;
DECL|member|curl_result
name|CURLcode
name|curl_result
decl_stmt|;
DECL|member|http_code
name|long
name|http_code
decl_stmt|;
DECL|member|finished
name|int
modifier|*
name|finished
decl_stmt|;
DECL|member|results
name|struct
name|slot_results
modifier|*
name|results
decl_stmt|;
DECL|member|callback_data
name|void
modifier|*
name|callback_data
decl_stmt|;
DECL|member|callback_func
name|void
function_decl|(
modifier|*
name|callback_func
function_decl|)
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
DECL|member|next
name|struct
name|active_request_slot
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|buffer
struct|struct
name|buffer
block|{
DECL|member|posn
name|size_t
name|posn
decl_stmt|;
DECL|member|size
name|size_t
name|size
decl_stmt|;
DECL|member|buffer
name|void
modifier|*
name|buffer
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/* Curl request read/write callbacks */
end_comment

begin_function_decl
specifier|extern
name|size_t
name|fread_buffer
parameter_list|(
name|void
modifier|*
name|ptr
parameter_list|,
name|size_t
name|eltsize
parameter_list|,
name|size_t
name|nmemb
parameter_list|,
name|struct
name|buffer
modifier|*
name|buffer
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|size_t
name|fwrite_buffer
parameter_list|(
specifier|const
name|void
modifier|*
name|ptr
parameter_list|,
name|size_t
name|eltsize
parameter_list|,
name|size_t
name|nmemb
parameter_list|,
name|struct
name|buffer
modifier|*
name|buffer
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|size_t
name|fwrite_null
parameter_list|(
specifier|const
name|void
modifier|*
name|ptr
parameter_list|,
name|size_t
name|eltsize
parameter_list|,
name|size_t
name|nmemb
parameter_list|,
name|struct
name|buffer
modifier|*
name|buffer
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Slot lifecycle functions */
end_comment

begin_function_decl
specifier|extern
name|struct
name|active_request_slot
modifier|*
name|get_active_slot
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|start_active_slot
parameter_list|(
name|struct
name|active_request_slot
modifier|*
name|slot
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|run_active_slot
parameter_list|(
name|struct
name|active_request_slot
modifier|*
name|slot
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|finish_all_active_slots
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|release_active_slot
parameter_list|(
name|struct
name|active_request_slot
modifier|*
name|slot
parameter_list|)
function_decl|;
end_function_decl

begin_ifdef
ifdef|#
directive|ifdef
name|USE_CURL_MULTI
end_ifdef

begin_function_decl
specifier|extern
name|void
name|fill_active_slots
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|step_active_slots
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
specifier|extern
name|void
name|http_init
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|http_cleanup
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
name|int
name|data_received
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|active_requests
decl_stmt|;
end_decl_stmt

begin_ifdef
ifdef|#
directive|ifdef
name|USE_CURL_MULTI
end_ifdef

begin_decl_stmt
specifier|extern
name|int
name|max_requests
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|CURLM
modifier|*
name|curlm
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_ifndef
ifndef|#
directive|ifndef
name|NO_CURL_EASY_DUPHANDLE
end_ifndef

begin_decl_stmt
specifier|extern
name|CURL
modifier|*
name|curl_default
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
specifier|extern
name|char
name|curl_errorstr
index|[
name|CURL_ERROR_SIZE
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|int
name|curl_ssl_verify
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|char
modifier|*
name|ssl_cert
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
name|LIBCURL_VERSION_NUM
operator|>=
literal|0x070902
end_if

begin_decl_stmt
specifier|extern
name|char
modifier|*
name|ssl_key
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
name|LIBCURL_VERSION_NUM
operator|>=
literal|0x070908
end_if

begin_decl_stmt
specifier|extern
name|char
modifier|*
name|ssl_capath
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
specifier|extern
name|char
modifier|*
name|ssl_cainfo
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|long
name|curl_low_speed_limit
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|long
name|curl_low_speed_time
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|struct
name|curl_slist
modifier|*
name|pragma_header
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|struct
name|curl_slist
modifier|*
name|no_range_header
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|struct
name|active_request_slot
modifier|*
name|active_queue_head
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* HTTP_H */
end_comment

end_unit

