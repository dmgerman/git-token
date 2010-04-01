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

begin_include
include|#
directive|include
file|"strbuf.h"
end_include

begin_include
include|#
directive|include
file|"remote.h"
end_include

begin_comment
comment|/*  * We detect based on the cURL version if multi-transfer is  * usable in this implementation and define this symbol accordingly.  * This is not something Makefile should set nor users should pass  * via CFLAGS.  */
end_comment

begin_undef
DECL|macro|USE_CURL_MULTI
undef|#
directive|undef
name|USE_CURL_MULTI
end_undef

begin_if
if|#
directive|if
name|LIBCURL_VERSION_NUM
operator|>=
literal|0x071000
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
operator|(
name|LIBCURL_VERSION_NUM
operator|<
literal|0x070c04
operator|)
operator|||
operator|(
name|LIBCURL_VERSION_NUM
operator|==
literal|0x071000
operator|)
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

begin_if
if|#
directive|if
name|LIBCURL_VERSION_NUM
operator|<
literal|0x070a03
end_if

begin_define
DECL|macro|CURLE_HTTP_RETURNED_ERROR
define|#
directive|define
name|CURLE_HTTP_RETURNED_ERROR
value|CURLE_HTTP_NOT_FOUND
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
literal|0x070c03
end_if

begin_define
DECL|macro|NO_CURL_IOCTL
define|#
directive|define
name|NO_CURL_IOCTL
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
DECL|member|buf
name|struct
name|strbuf
name|buf
decl_stmt|;
DECL|member|posn
name|size_t
name|posn
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
name|void
modifier|*
name|strbuf
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
name|void
modifier|*
name|strbuf
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
name|void
modifier|*
name|strbuf
parameter_list|)
function_decl|;
end_function_decl

begin_ifndef
ifndef|#
directive|ifndef
name|NO_CURL_IOCTL
end_ifndef

begin_function_decl
specifier|extern
name|curlioerr
name|ioctl_buffer
parameter_list|(
name|CURL
modifier|*
name|handle
parameter_list|,
name|int
name|cmd
parameter_list|,
name|void
modifier|*
name|clientp
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

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
name|finish_active_slot
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
name|add_fill_function
parameter_list|(
name|void
modifier|*
name|data
parameter_list|,
name|int
function_decl|(
modifier|*
name|fill
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|)
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
name|struct
name|remote
modifier|*
name|remote
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

begin_decl_stmt
specifier|extern
name|int
name|http_is_verbose
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|size_t
name|http_post_buffer
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|char
name|curl_errorstr
index|[
name|CURL_ERROR_SIZE
index|]
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|missing__target
specifier|static
specifier|inline
name|int
name|missing__target
parameter_list|(
name|int
name|code
parameter_list|,
name|int
name|result
parameter_list|)
block|{
return|return
comment|/* file:// URL -- do we ever use one??? */
operator|(
name|result
operator|==
name|CURLE_FILE_COULDNT_READ_FILE
operator|)
operator|||
comment|/* http:// and https:// URL */
operator|(
name|code
operator|==
literal|404
operator|&&
name|result
operator|==
name|CURLE_HTTP_RETURNED_ERROR
operator|)
operator|||
comment|/* ftp:// URL */
operator|(
name|code
operator|==
literal|550
operator|&&
name|result
operator|==
name|CURLE_FTP_COULDNT_RETR_FILE
operator|)
return|;
block|}
end_function

begin_define
DECL|macro|missing_target
define|#
directive|define
name|missing_target
parameter_list|(
name|a
parameter_list|)
value|missing__target((a)->http_code, (a)->curl_result)
end_define

begin_comment
comment|/* Helpers for modifying and creating URLs */
end_comment

begin_function_decl
specifier|extern
name|void
name|append_remote_object_url
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|url
parameter_list|,
specifier|const
name|char
modifier|*
name|hex
parameter_list|,
name|int
name|only_two_digit_prefix
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|get_remote_object_url
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|,
specifier|const
name|char
modifier|*
name|hex
parameter_list|,
name|int
name|only_two_digit_prefix
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Options for http_request_*() */
end_comment

begin_define
DECL|macro|HTTP_NO_CACHE
define|#
directive|define
name|HTTP_NO_CACHE
value|1
end_define

begin_comment
comment|/* Return values for http_request_*() */
end_comment

begin_define
DECL|macro|HTTP_OK
define|#
directive|define
name|HTTP_OK
value|0
end_define

begin_define
DECL|macro|HTTP_MISSING_TARGET
define|#
directive|define
name|HTTP_MISSING_TARGET
value|1
end_define

begin_define
DECL|macro|HTTP_ERROR
define|#
directive|define
name|HTTP_ERROR
value|2
end_define

begin_define
DECL|macro|HTTP_START_FAILED
define|#
directive|define
name|HTTP_START_FAILED
value|3
end_define

begin_define
DECL|macro|HTTP_REAUTH
define|#
directive|define
name|HTTP_REAUTH
value|4
end_define

begin_define
DECL|macro|HTTP_NOAUTH
define|#
directive|define
name|HTTP_NOAUTH
value|5
end_define

begin_comment
comment|/*  * Requests an url and stores the result in a strbuf.  *  * If the result pointer is NULL, a HTTP HEAD request is made instead of GET.  */
end_comment

begin_function_decl
name|int
name|http_get_strbuf
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|,
name|struct
name|strbuf
modifier|*
name|result
parameter_list|,
name|int
name|options
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Prints an error message using error() containing url and curl_errorstr,  * and returns ret.  */
end_comment

begin_function_decl
name|int
name|http_error
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|,
name|int
name|ret
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|http_fetch_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|struct
name|ref
modifier|*
name|ref
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Helpers for fetching packs */
end_comment

begin_function_decl
specifier|extern
name|int
name|http_get_info_packs
parameter_list|(
specifier|const
name|char
modifier|*
name|base_url
parameter_list|,
name|struct
name|packed_git
modifier|*
modifier|*
name|packs_head
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|http_pack_request
struct|struct
name|http_pack_request
block|{
DECL|member|url
name|char
modifier|*
name|url
decl_stmt|;
DECL|member|target
name|struct
name|packed_git
modifier|*
name|target
decl_stmt|;
DECL|member|lst
name|struct
name|packed_git
modifier|*
modifier|*
name|lst
decl_stmt|;
DECL|member|packfile
name|FILE
modifier|*
name|packfile
decl_stmt|;
DECL|member|filename
name|char
name|filename
index|[
name|PATH_MAX
index|]
decl_stmt|;
DECL|member|tmpfile
name|char
name|tmpfile
index|[
name|PATH_MAX
index|]
decl_stmt|;
DECL|member|range_header
name|struct
name|curl_slist
modifier|*
name|range_header
decl_stmt|;
DECL|member|slot
name|struct
name|active_request_slot
modifier|*
name|slot
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|struct
name|http_pack_request
modifier|*
name|new_http_pack_request
parameter_list|(
name|struct
name|packed_git
modifier|*
name|target
parameter_list|,
specifier|const
name|char
modifier|*
name|base_url
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|finish_http_pack_request
parameter_list|(
name|struct
name|http_pack_request
modifier|*
name|preq
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|release_http_pack_request
parameter_list|(
name|struct
name|http_pack_request
modifier|*
name|preq
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Helpers for fetching object */
end_comment

begin_struct
DECL|struct|http_object_request
struct|struct
name|http_object_request
block|{
DECL|member|url
name|char
modifier|*
name|url
decl_stmt|;
DECL|member|filename
name|char
name|filename
index|[
name|PATH_MAX
index|]
decl_stmt|;
DECL|member|tmpfile
name|char
name|tmpfile
index|[
name|PATH_MAX
index|]
decl_stmt|;
DECL|member|localfile
name|int
name|localfile
decl_stmt|;
DECL|member|curl_result
name|CURLcode
name|curl_result
decl_stmt|;
DECL|member|errorstr
name|char
name|errorstr
index|[
name|CURL_ERROR_SIZE
index|]
decl_stmt|;
DECL|member|http_code
name|long
name|http_code
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|real_sha1
name|unsigned
name|char
name|real_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|c
name|git_SHA_CTX
name|c
decl_stmt|;
DECL|member|stream
name|z_stream
name|stream
decl_stmt|;
DECL|member|zret
name|int
name|zret
decl_stmt|;
DECL|member|rename
name|int
name|rename
decl_stmt|;
DECL|member|slot
name|struct
name|active_request_slot
modifier|*
name|slot
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|struct
name|http_object_request
modifier|*
name|new_http_object_request
parameter_list|(
specifier|const
name|char
modifier|*
name|base_url
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|process_http_object_request
parameter_list|(
name|struct
name|http_object_request
modifier|*
name|freq
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|finish_http_object_request
parameter_list|(
name|struct
name|http_object_request
modifier|*
name|freq
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|abort_http_object_request
parameter_list|(
name|struct
name|http_object_request
modifier|*
name|freq
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|release_http_object_request
parameter_list|(
name|struct
name|http_object_request
modifier|*
name|freq
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* HTTP_H */
end_comment

end_unit

