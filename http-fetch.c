begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"commit.h"
end_include

begin_include
include|#
directive|include
file|"fetch.h"
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

begin_decl_stmt
DECL|variable|curl
specifier|static
name|CURL
modifier|*
name|curl
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|no_pragma_header
specifier|static
name|struct
name|curl_slist
modifier|*
name|no_pragma_header
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|curl_errorstr
specifier|static
name|char
name|curl_errorstr
index|[
name|CURL_ERROR_SIZE
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|initial_base
specifier|static
name|char
modifier|*
name|initial_base
decl_stmt|;
end_decl_stmt

begin_struct
DECL|struct|alt_base
struct|struct
name|alt_base
block|{
DECL|member|base
name|char
modifier|*
name|base
decl_stmt|;
DECL|member|got_indices
name|int
name|got_indices
decl_stmt|;
DECL|member|packs
name|struct
name|packed_git
modifier|*
name|packs
decl_stmt|;
DECL|member|next
name|struct
name|alt_base
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|alt
name|struct
name|alt_base
modifier|*
name|alt
init|=
name|NULL
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|c
specifier|static
name|SHA_CTX
name|c
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|stream
specifier|static
name|z_stream
name|stream
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|local
specifier|static
name|int
name|local
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|zret
specifier|static
name|int
name|zret
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|curl_ssl_verify
specifier|static
name|int
name|curl_ssl_verify
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ssl_cert
specifier|static
name|char
modifier|*
name|ssl_cert
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ssl_key
specifier|static
name|char
modifier|*
name|ssl_key
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ssl_capath
specifier|static
name|char
modifier|*
name|ssl_capath
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ssl_cainfo
specifier|static
name|char
modifier|*
name|ssl_cainfo
decl_stmt|;
end_decl_stmt

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

begin_function
DECL|function|fwrite_buffer
specifier|static
name|size_t
name|fwrite_buffer
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
block|{
name|size_t
name|size
init|=
name|eltsize
operator|*
name|nmemb
decl_stmt|;
if|if
condition|(
name|size
operator|>
name|buffer
operator|->
name|size
operator|-
name|buffer
operator|->
name|posn
condition|)
name|size
operator|=
name|buffer
operator|->
name|size
operator|-
name|buffer
operator|->
name|posn
expr_stmt|;
name|memcpy
argument_list|(
name|buffer
operator|->
name|buffer
operator|+
name|buffer
operator|->
name|posn
argument_list|,
name|ptr
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|buffer
operator|->
name|posn
operator|+=
name|size
expr_stmt|;
return|return
name|size
return|;
block|}
end_function

begin_function
DECL|function|fwrite_sha1_file
specifier|static
name|size_t
name|fwrite_sha1_file
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
name|data
parameter_list|)
block|{
name|unsigned
name|char
name|expn
index|[
literal|4096
index|]
decl_stmt|;
name|size_t
name|size
init|=
name|eltsize
operator|*
name|nmemb
decl_stmt|;
name|int
name|posn
init|=
literal|0
decl_stmt|;
do|do
block|{
name|ssize_t
name|retval
init|=
name|write
argument_list|(
name|local
argument_list|,
name|ptr
operator|+
name|posn
argument_list|,
name|size
operator|-
name|posn
argument_list|)
decl_stmt|;
if|if
condition|(
name|retval
operator|<
literal|0
condition|)
return|return
name|posn
return|;
name|posn
operator|+=
name|retval
expr_stmt|;
block|}
do|while
condition|(
name|posn
operator|<
name|size
condition|)
do|;
name|stream
operator|.
name|avail_in
operator|=
name|size
expr_stmt|;
name|stream
operator|.
name|next_in
operator|=
name|ptr
expr_stmt|;
do|do
block|{
name|stream
operator|.
name|next_out
operator|=
name|expn
expr_stmt|;
name|stream
operator|.
name|avail_out
operator|=
sizeof|sizeof
argument_list|(
name|expn
argument_list|)
expr_stmt|;
name|zret
operator|=
name|inflate
argument_list|(
operator|&
name|stream
argument_list|,
name|Z_SYNC_FLUSH
argument_list|)
expr_stmt|;
name|SHA1_Update
argument_list|(
operator|&
name|c
argument_list|,
name|expn
argument_list|,
sizeof|sizeof
argument_list|(
name|expn
argument_list|)
operator|-
name|stream
operator|.
name|avail_out
argument_list|)
expr_stmt|;
block|}
do|while
condition|(
name|stream
operator|.
name|avail_in
operator|&&
name|zret
operator|==
name|Z_OK
condition|)
do|;
return|return
name|size
return|;
block|}
end_function

begin_function
DECL|function|prefetch
name|void
name|prefetch
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{ }
end_function

begin_decl_stmt
DECL|variable|got_alternates
specifier|static
name|int
name|got_alternates
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|fetch_index
specifier|static
name|int
name|fetch_index
parameter_list|(
name|struct
name|alt_base
modifier|*
name|repo
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|char
modifier|*
name|filename
decl_stmt|;
name|char
modifier|*
name|url
decl_stmt|;
name|FILE
modifier|*
name|indexfile
decl_stmt|;
if|if
condition|(
name|has_pack_index
argument_list|(
name|sha1
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|get_verbosely
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Getting index for pack %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|url
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|repo
operator|->
name|base
argument_list|)
operator|+
literal|64
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|url
argument_list|,
literal|"%s/objects/pack/pack-%s.idx"
argument_list|,
name|repo
operator|->
name|base
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|filename
operator|=
name|sha1_pack_index_name
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|indexfile
operator|=
name|fopen
argument_list|(
name|filename
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|indexfile
condition|)
return|return
name|error
argument_list|(
literal|"Unable to open local file %s for pack index"
argument_list|,
name|filename
argument_list|)
return|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_FILE
argument_list|,
name|indexfile
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_WRITEFUNCTION
argument_list|,
name|fwrite
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_URL
argument_list|,
name|url
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_HTTPHEADER
argument_list|,
name|no_pragma_header
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_ERRORBUFFER
argument_list|,
name|curl_errorstr
argument_list|)
expr_stmt|;
if|if
condition|(
name|curl_easy_perform
argument_list|(
name|curl
argument_list|)
condition|)
block|{
name|fclose
argument_list|(
name|indexfile
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"Unable to get pack index %s\n%s"
argument_list|,
name|url
argument_list|,
name|curl_errorstr
argument_list|)
return|;
block|}
name|fclose
argument_list|(
name|indexfile
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|setup_index
specifier|static
name|int
name|setup_index
parameter_list|(
name|struct
name|alt_base
modifier|*
name|repo
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|packed_git
modifier|*
name|new_pack
decl_stmt|;
if|if
condition|(
name|has_pack_file
argument_list|(
name|sha1
argument_list|)
condition|)
return|return
literal|0
return|;
comment|// don't list this as something we can get
if|if
condition|(
name|fetch_index
argument_list|(
name|repo
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|new_pack
operator|=
name|parse_pack_index
argument_list|(
name|sha1
argument_list|)
expr_stmt|;
name|new_pack
operator|->
name|next
operator|=
name|repo
operator|->
name|packs
expr_stmt|;
name|repo
operator|->
name|packs
operator|=
name|new_pack
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fetch_alternates
specifier|static
name|int
name|fetch_alternates
parameter_list|(
name|char
modifier|*
name|base
parameter_list|)
block|{
name|int
name|ret
init|=
literal|0
decl_stmt|;
name|struct
name|buffer
name|buffer
decl_stmt|;
name|char
modifier|*
name|url
decl_stmt|;
name|char
modifier|*
name|data
decl_stmt|;
name|int
name|i
init|=
literal|0
decl_stmt|;
name|int
name|http_specific
init|=
literal|1
decl_stmt|;
if|if
condition|(
name|got_alternates
condition|)
return|return
literal|0
return|;
name|data
operator|=
name|xmalloc
argument_list|(
literal|4096
argument_list|)
expr_stmt|;
name|buffer
operator|.
name|size
operator|=
literal|4095
expr_stmt|;
name|buffer
operator|.
name|posn
operator|=
literal|0
expr_stmt|;
name|buffer
operator|.
name|buffer
operator|=
name|data
expr_stmt|;
if|if
condition|(
name|get_verbosely
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Getting alternates list\n"
argument_list|)
expr_stmt|;
name|url
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|base
argument_list|)
operator|+
literal|31
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|url
argument_list|,
literal|"%s/objects/info/http-alternates"
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_FILE
argument_list|,
operator|&
name|buffer
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_WRITEFUNCTION
argument_list|,
name|fwrite_buffer
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_URL
argument_list|,
name|url
argument_list|)
expr_stmt|;
if|if
condition|(
name|curl_easy_perform
argument_list|(
name|curl
argument_list|)
operator|||
operator|!
name|buffer
operator|.
name|posn
condition|)
block|{
name|http_specific
operator|=
literal|0
expr_stmt|;
name|sprintf
argument_list|(
name|url
argument_list|,
literal|"%s/objects/info/alternates"
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_FILE
argument_list|,
operator|&
name|buffer
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_WRITEFUNCTION
argument_list|,
name|fwrite_buffer
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_URL
argument_list|,
name|url
argument_list|)
expr_stmt|;
if|if
condition|(
name|curl_easy_perform
argument_list|(
name|curl
argument_list|)
condition|)
block|{
return|return
literal|0
return|;
block|}
block|}
name|data
index|[
name|buffer
operator|.
name|posn
index|]
operator|=
literal|'\0'
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|buffer
operator|.
name|posn
condition|)
block|{
name|int
name|posn
init|=
name|i
decl_stmt|;
while|while
condition|(
name|posn
operator|<
name|buffer
operator|.
name|posn
operator|&&
name|data
index|[
name|posn
index|]
operator|!=
literal|'\n'
condition|)
name|posn
operator|++
expr_stmt|;
if|if
condition|(
name|data
index|[
name|posn
index|]
operator|==
literal|'\n'
condition|)
block|{
name|int
name|okay
init|=
literal|0
decl_stmt|;
name|int
name|serverlen
init|=
literal|0
decl_stmt|;
name|struct
name|alt_base
modifier|*
name|newalt
decl_stmt|;
name|char
modifier|*
name|target
init|=
name|NULL
decl_stmt|;
if|if
condition|(
name|data
index|[
name|i
index|]
operator|==
literal|'/'
condition|)
block|{
name|serverlen
operator|=
name|strchr
argument_list|(
name|base
operator|+
literal|8
argument_list|,
literal|'/'
argument_list|)
operator|-
name|base
expr_stmt|;
name|okay
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|memcmp
argument_list|(
name|data
operator|+
name|i
argument_list|,
literal|"../"
argument_list|,
literal|3
argument_list|)
condition|)
block|{
name|i
operator|+=
literal|3
expr_stmt|;
name|serverlen
operator|=
name|strlen
argument_list|(
name|base
argument_list|)
expr_stmt|;
while|while
condition|(
name|i
operator|+
literal|2
operator|<
name|posn
operator|&&
operator|!
name|memcmp
argument_list|(
name|data
operator|+
name|i
argument_list|,
literal|"../"
argument_list|,
literal|3
argument_list|)
condition|)
block|{
do|do
block|{
name|serverlen
operator|--
expr_stmt|;
block|}
do|while
condition|(
name|serverlen
operator|&&
name|base
index|[
name|serverlen
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
do|;
name|i
operator|+=
literal|3
expr_stmt|;
block|}
comment|// If the server got removed, give up.
name|okay
operator|=
name|strchr
argument_list|(
name|base
argument_list|,
literal|':'
argument_list|)
operator|-
name|base
operator|+
literal|3
operator|<
name|serverlen
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|http_specific
condition|)
block|{
name|char
modifier|*
name|colon
init|=
name|strchr
argument_list|(
name|data
operator|+
name|i
argument_list|,
literal|':'
argument_list|)
decl_stmt|;
name|char
modifier|*
name|slash
init|=
name|strchr
argument_list|(
name|data
operator|+
name|i
argument_list|,
literal|'/'
argument_list|)
decl_stmt|;
if|if
condition|(
name|colon
operator|&&
name|slash
operator|&&
name|colon
operator|<
name|data
operator|+
name|posn
operator|&&
name|slash
operator|<
name|data
operator|+
name|posn
operator|&&
name|colon
operator|<
name|slash
condition|)
block|{
name|okay
operator|=
literal|1
expr_stmt|;
block|}
block|}
comment|// skip 'objects' at end
if|if
condition|(
name|okay
condition|)
block|{
name|target
operator|=
name|xmalloc
argument_list|(
name|serverlen
operator|+
name|posn
operator|-
name|i
operator|-
literal|6
argument_list|)
expr_stmt|;
name|strncpy
argument_list|(
name|target
argument_list|,
name|base
argument_list|,
name|serverlen
argument_list|)
expr_stmt|;
name|strncpy
argument_list|(
name|target
operator|+
name|serverlen
argument_list|,
name|data
operator|+
name|i
argument_list|,
name|posn
operator|-
name|i
operator|-
literal|7
argument_list|)
expr_stmt|;
name|target
index|[
name|serverlen
operator|+
name|posn
operator|-
name|i
operator|-
literal|7
index|]
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
name|get_verbosely
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Also look at %s\n"
argument_list|,
name|target
argument_list|)
expr_stmt|;
name|newalt
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|newalt
argument_list|)
argument_list|)
expr_stmt|;
name|newalt
operator|->
name|next
operator|=
name|alt
expr_stmt|;
name|newalt
operator|->
name|base
operator|=
name|target
expr_stmt|;
name|newalt
operator|->
name|got_indices
operator|=
literal|0
expr_stmt|;
name|newalt
operator|->
name|packs
operator|=
name|NULL
expr_stmt|;
name|alt
operator|=
name|newalt
expr_stmt|;
name|ret
operator|++
expr_stmt|;
block|}
block|}
name|i
operator|=
name|posn
operator|+
literal|1
expr_stmt|;
block|}
name|got_alternates
operator|=
literal|1
expr_stmt|;
return|return
name|ret
return|;
block|}
end_function

begin_function
DECL|function|fetch_indices
specifier|static
name|int
name|fetch_indices
parameter_list|(
name|struct
name|alt_base
modifier|*
name|repo
parameter_list|)
block|{
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
modifier|*
name|url
decl_stmt|;
name|struct
name|buffer
name|buffer
decl_stmt|;
name|char
modifier|*
name|data
decl_stmt|;
name|int
name|i
init|=
literal|0
decl_stmt|;
if|if
condition|(
name|repo
operator|->
name|got_indices
condition|)
return|return
literal|0
return|;
name|data
operator|=
name|xmalloc
argument_list|(
literal|4096
argument_list|)
expr_stmt|;
name|buffer
operator|.
name|size
operator|=
literal|4096
expr_stmt|;
name|buffer
operator|.
name|posn
operator|=
literal|0
expr_stmt|;
name|buffer
operator|.
name|buffer
operator|=
name|data
expr_stmt|;
if|if
condition|(
name|get_verbosely
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Getting pack list\n"
argument_list|)
expr_stmt|;
name|url
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|repo
operator|->
name|base
argument_list|)
operator|+
literal|21
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|url
argument_list|,
literal|"%s/objects/info/packs"
argument_list|,
name|repo
operator|->
name|base
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_FILE
argument_list|,
operator|&
name|buffer
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_WRITEFUNCTION
argument_list|,
name|fwrite_buffer
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_URL
argument_list|,
name|url
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_HTTPHEADER
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_ERRORBUFFER
argument_list|,
name|curl_errorstr
argument_list|)
expr_stmt|;
if|if
condition|(
name|curl_easy_perform
argument_list|(
name|curl
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"%s"
argument_list|,
name|curl_errorstr
argument_list|)
return|;
while|while
condition|(
name|i
operator|<
name|buffer
operator|.
name|posn
condition|)
block|{
switch|switch
condition|(
name|data
index|[
name|i
index|]
condition|)
block|{
case|case
literal|'P'
case|:
name|i
operator|++
expr_stmt|;
if|if
condition|(
name|i
operator|+
literal|52
operator|<
name|buffer
operator|.
name|posn
operator|&&
operator|!
name|strncmp
argument_list|(
name|data
operator|+
name|i
argument_list|,
literal|" pack-"
argument_list|,
literal|6
argument_list|)
operator|&&
operator|!
name|strncmp
argument_list|(
name|data
operator|+
name|i
operator|+
literal|46
argument_list|,
literal|".pack\n"
argument_list|,
literal|6
argument_list|)
condition|)
block|{
name|get_sha1_hex
argument_list|(
name|data
operator|+
name|i
operator|+
literal|6
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|setup_index
argument_list|(
name|repo
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
name|i
operator|+=
literal|51
expr_stmt|;
break|break;
block|}
default|default:
while|while
condition|(
name|data
index|[
name|i
index|]
operator|!=
literal|'\n'
condition|)
name|i
operator|++
expr_stmt|;
block|}
name|i
operator|++
expr_stmt|;
block|}
name|repo
operator|->
name|got_indices
operator|=
literal|1
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fetch_pack
specifier|static
name|int
name|fetch_pack
parameter_list|(
name|struct
name|alt_base
modifier|*
name|repo
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|char
modifier|*
name|url
decl_stmt|;
name|struct
name|packed_git
modifier|*
name|target
decl_stmt|;
name|struct
name|packed_git
modifier|*
modifier|*
name|lst
decl_stmt|;
name|FILE
modifier|*
name|packfile
decl_stmt|;
name|char
modifier|*
name|filename
decl_stmt|;
if|if
condition|(
name|fetch_indices
argument_list|(
name|repo
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
name|target
operator|=
name|find_sha1_pack
argument_list|(
name|sha1
argument_list|,
name|repo
operator|->
name|packs
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|target
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|get_verbosely
condition|)
block|{
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"Getting pack %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|target
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|" which contains %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|url
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|repo
operator|->
name|base
argument_list|)
operator|+
literal|65
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|url
argument_list|,
literal|"%s/objects/pack/pack-%s.pack"
argument_list|,
name|repo
operator|->
name|base
argument_list|,
name|sha1_to_hex
argument_list|(
name|target
operator|->
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|filename
operator|=
name|sha1_pack_name
argument_list|(
name|target
operator|->
name|sha1
argument_list|)
expr_stmt|;
name|packfile
operator|=
name|fopen
argument_list|(
name|filename
argument_list|,
literal|"w"
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|packfile
condition|)
return|return
name|error
argument_list|(
literal|"Unable to open local file %s for pack"
argument_list|,
name|filename
argument_list|)
return|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_FILE
argument_list|,
name|packfile
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_WRITEFUNCTION
argument_list|,
name|fwrite
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_URL
argument_list|,
name|url
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_HTTPHEADER
argument_list|,
name|no_pragma_header
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_ERRORBUFFER
argument_list|,
name|curl_errorstr
argument_list|)
expr_stmt|;
if|if
condition|(
name|curl_easy_perform
argument_list|(
name|curl
argument_list|)
condition|)
block|{
name|fclose
argument_list|(
name|packfile
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"Unable to get pack file %s\n%s"
argument_list|,
name|url
argument_list|,
name|curl_errorstr
argument_list|)
return|;
block|}
name|fclose
argument_list|(
name|packfile
argument_list|)
expr_stmt|;
name|lst
operator|=
operator|&
name|repo
operator|->
name|packs
expr_stmt|;
while|while
condition|(
operator|*
name|lst
operator|!=
name|target
condition|)
name|lst
operator|=
operator|&
operator|(
operator|(
operator|*
name|lst
operator|)
operator|->
name|next
operator|)
expr_stmt|;
operator|*
name|lst
operator|=
operator|(
operator|*
name|lst
operator|)
operator|->
name|next
expr_stmt|;
name|install_packed_git
argument_list|(
name|target
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fetch_object
name|int
name|fetch_object
parameter_list|(
name|struct
name|alt_base
modifier|*
name|repo
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|char
modifier|*
name|hex
init|=
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|char
modifier|*
name|filename
init|=
name|sha1_file_name
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|unsigned
name|char
name|real_sha1
index|[
literal|20
index|]
decl_stmt|;
name|char
name|tmpfile
index|[
name|PATH_MAX
index|]
decl_stmt|;
name|int
name|ret
decl_stmt|;
name|char
modifier|*
name|url
decl_stmt|;
name|char
modifier|*
name|posn
decl_stmt|;
name|snprintf
argument_list|(
name|tmpfile
argument_list|,
sizeof|sizeof
argument_list|(
name|tmpfile
argument_list|)
argument_list|,
literal|"%s/obj_XXXXXX"
argument_list|,
name|get_object_directory
argument_list|()
argument_list|)
expr_stmt|;
name|local
operator|=
name|mkstemp
argument_list|(
name|tmpfile
argument_list|)
expr_stmt|;
if|if
condition|(
name|local
operator|<
literal|0
condition|)
return|return
name|error
argument_list|(
literal|"Couldn't create temporary file %s for %s: %s\n"
argument_list|,
name|tmpfile
argument_list|,
name|filename
argument_list|,
name|strerror
argument_list|(
name|errno
argument_list|)
argument_list|)
return|;
name|memset
argument_list|(
operator|&
name|stream
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|stream
argument_list|)
argument_list|)
expr_stmt|;
name|inflateInit
argument_list|(
operator|&
name|stream
argument_list|)
expr_stmt|;
name|SHA1_Init
argument_list|(
operator|&
name|c
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_FAILONERROR
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_FILE
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_WRITEFUNCTION
argument_list|,
name|fwrite_sha1_file
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_HTTPHEADER
argument_list|,
name|no_pragma_header
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_ERRORBUFFER
argument_list|,
name|curl_errorstr
argument_list|)
expr_stmt|;
name|url
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|repo
operator|->
name|base
argument_list|)
operator|+
literal|50
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|url
argument_list|,
name|repo
operator|->
name|base
argument_list|)
expr_stmt|;
name|posn
operator|=
name|url
operator|+
name|strlen
argument_list|(
name|repo
operator|->
name|base
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|posn
argument_list|,
literal|"objects/"
argument_list|)
expr_stmt|;
name|posn
operator|+=
literal|8
expr_stmt|;
name|memcpy
argument_list|(
name|posn
argument_list|,
name|hex
argument_list|,
literal|2
argument_list|)
expr_stmt|;
name|posn
operator|+=
literal|2
expr_stmt|;
operator|*
operator|(
name|posn
operator|++
operator|)
operator|=
literal|'/'
expr_stmt|;
name|strcpy
argument_list|(
name|posn
argument_list|,
name|hex
operator|+
literal|2
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_URL
argument_list|,
name|url
argument_list|)
expr_stmt|;
if|if
condition|(
name|curl_easy_perform
argument_list|(
name|curl
argument_list|)
condition|)
block|{
name|unlink
argument_list|(
name|filename
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"%s"
argument_list|,
name|curl_errorstr
argument_list|)
return|;
block|}
name|fchmod
argument_list|(
name|local
argument_list|,
literal|0444
argument_list|)
expr_stmt|;
name|close
argument_list|(
name|local
argument_list|)
expr_stmt|;
name|inflateEnd
argument_list|(
operator|&
name|stream
argument_list|)
expr_stmt|;
name|SHA1_Final
argument_list|(
name|real_sha1
argument_list|,
operator|&
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|zret
operator|!=
name|Z_STREAM_END
condition|)
block|{
name|unlink
argument_list|(
name|tmpfile
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"File %s (%s) corrupt\n"
argument_list|,
name|hex
argument_list|,
name|url
argument_list|)
return|;
block|}
if|if
condition|(
name|memcmp
argument_list|(
name|sha1
argument_list|,
name|real_sha1
argument_list|,
literal|20
argument_list|)
condition|)
block|{
name|unlink
argument_list|(
name|tmpfile
argument_list|)
expr_stmt|;
return|return
name|error
argument_list|(
literal|"File %s has bad hash\n"
argument_list|,
name|hex
argument_list|)
return|;
block|}
name|ret
operator|=
name|link
argument_list|(
name|tmpfile
argument_list|,
name|filename
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
operator|<
literal|0
condition|)
block|{
comment|/* Same Coda hack as in write_sha1_file(sha1_file.c) */
name|ret
operator|=
name|errno
expr_stmt|;
if|if
condition|(
name|ret
operator|==
name|EXDEV
operator|&&
operator|!
name|rename
argument_list|(
name|tmpfile
argument_list|,
name|filename
argument_list|)
condition|)
goto|goto
name|out
goto|;
block|}
name|unlink
argument_list|(
name|tmpfile
argument_list|)
expr_stmt|;
if|if
condition|(
name|ret
condition|)
block|{
if|if
condition|(
name|ret
operator|!=
name|EEXIST
condition|)
return|return
name|error
argument_list|(
literal|"unable to write sha1 filename %s: %s"
argument_list|,
name|filename
argument_list|,
name|strerror
argument_list|(
name|ret
argument_list|)
argument_list|)
return|;
block|}
name|out
label|:
name|pull_say
argument_list|(
literal|"got %s\n"
argument_list|,
name|hex
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|fetch
name|int
name|fetch
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|alt_base
modifier|*
name|altbase
init|=
name|alt
decl_stmt|;
while|while
condition|(
name|altbase
condition|)
block|{
if|if
condition|(
operator|!
name|fetch_object
argument_list|(
name|altbase
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
operator|!
name|fetch_pack
argument_list|(
name|altbase
argument_list|,
name|sha1
argument_list|)
condition|)
return|return
literal|0
return|;
if|if
condition|(
name|fetch_alternates
argument_list|(
name|altbase
operator|->
name|base
argument_list|)
operator|>
literal|0
condition|)
block|{
name|altbase
operator|=
name|alt
expr_stmt|;
continue|continue;
block|}
name|altbase
operator|=
name|altbase
operator|->
name|next
expr_stmt|;
block|}
return|return
name|error
argument_list|(
literal|"Unable to find %s under %s\n"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|initial_base
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|fetch_ref
name|int
name|fetch_ref
parameter_list|(
name|char
modifier|*
name|ref
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|char
modifier|*
name|url
decl_stmt|,
modifier|*
name|posn
decl_stmt|;
name|char
name|hex
index|[
literal|42
index|]
decl_stmt|;
name|struct
name|buffer
name|buffer
decl_stmt|;
name|char
modifier|*
name|base
init|=
name|initial_base
decl_stmt|;
name|buffer
operator|.
name|size
operator|=
literal|41
expr_stmt|;
name|buffer
operator|.
name|posn
operator|=
literal|0
expr_stmt|;
name|buffer
operator|.
name|buffer
operator|=
name|hex
expr_stmt|;
name|hex
index|[
literal|41
index|]
operator|=
literal|'\0'
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_FILE
argument_list|,
operator|&
name|buffer
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_WRITEFUNCTION
argument_list|,
name|fwrite_buffer
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_HTTPHEADER
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_ERRORBUFFER
argument_list|,
name|curl_errorstr
argument_list|)
expr_stmt|;
name|url
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
name|base
argument_list|)
operator|+
literal|6
operator|+
name|strlen
argument_list|(
name|ref
argument_list|)
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|url
argument_list|,
name|base
argument_list|)
expr_stmt|;
name|posn
operator|=
name|url
operator|+
name|strlen
argument_list|(
name|base
argument_list|)
expr_stmt|;
name|strcpy
argument_list|(
name|posn
argument_list|,
literal|"refs/"
argument_list|)
expr_stmt|;
name|posn
operator|+=
literal|5
expr_stmt|;
name|strcpy
argument_list|(
name|posn
argument_list|,
name|ref
argument_list|)
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_URL
argument_list|,
name|url
argument_list|)
expr_stmt|;
if|if
condition|(
name|curl_easy_perform
argument_list|(
name|curl
argument_list|)
condition|)
return|return
name|error
argument_list|(
literal|"Couldn't get %s for %s\n%s"
argument_list|,
name|url
argument_list|,
name|ref
argument_list|,
name|curl_errorstr
argument_list|)
return|;
name|hex
index|[
literal|40
index|]
operator|=
literal|'\0'
expr_stmt|;
name|get_sha1_hex
argument_list|(
name|hex
argument_list|,
name|sha1
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|main
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|char
modifier|*
name|commit_id
decl_stmt|;
name|char
modifier|*
name|url
decl_stmt|;
name|int
name|arg
init|=
literal|1
decl_stmt|;
while|while
condition|(
name|arg
operator|<
name|argc
operator|&&
name|argv
index|[
name|arg
index|]
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'t'
condition|)
block|{
name|get_tree
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'c'
condition|)
block|{
name|get_history
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'a'
condition|)
block|{
name|get_all
operator|=
literal|1
expr_stmt|;
name|get_tree
operator|=
literal|1
expr_stmt|;
name|get_history
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'v'
condition|)
block|{
name|get_verbosely
operator|=
literal|1
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|argv
index|[
name|arg
index|]
index|[
literal|1
index|]
operator|==
literal|'w'
condition|)
block|{
name|write_ref
operator|=
name|argv
index|[
name|arg
operator|+
literal|1
index|]
expr_stmt|;
name|arg
operator|++
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|argv
index|[
name|arg
index|]
argument_list|,
literal|"--recover"
argument_list|)
condition|)
block|{
name|get_recover
operator|=
literal|1
expr_stmt|;
block|}
name|arg
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|argc
operator|<
name|arg
operator|+
literal|2
condition|)
block|{
name|usage
argument_list|(
literal|"git-http-fetch [-c] [-t] [-a] [-d] [-v] [--recover] [-w ref] commit-id url"
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
name|commit_id
operator|=
name|argv
index|[
name|arg
index|]
expr_stmt|;
name|url
operator|=
name|argv
index|[
name|arg
operator|+
literal|1
index|]
expr_stmt|;
name|curl_global_init
argument_list|(
name|CURL_GLOBAL_ALL
argument_list|)
expr_stmt|;
name|curl
operator|=
name|curl_easy_init
argument_list|()
expr_stmt|;
name|no_pragma_header
operator|=
name|curl_slist_append
argument_list|(
name|no_pragma_header
argument_list|,
literal|"Pragma:"
argument_list|)
expr_stmt|;
name|curl_ssl_verify
operator|=
name|getenv
argument_list|(
literal|"GIT_SSL_NO_VERIFY"
argument_list|)
condition|?
literal|0
else|:
literal|1
expr_stmt|;
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_SSL_VERIFYPEER
argument_list|,
name|curl_ssl_verify
argument_list|)
expr_stmt|;
if|#
directive|if
name|LIBCURL_VERSION_NUM
operator|>=
literal|0x070907
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_NETRC
argument_list|,
name|CURL_NETRC_OPTIONAL
argument_list|)
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
operator|(
name|ssl_cert
operator|=
name|getenv
argument_list|(
literal|"GIT_SSL_CERT"
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_SSLCERT
argument_list|,
name|ssl_cert
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|ssl_key
operator|=
name|getenv
argument_list|(
literal|"GIT_SSL_KEY"
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_SSLKEY
argument_list|,
name|ssl_key
argument_list|)
expr_stmt|;
block|}
if|#
directive|if
name|LIBCURL_VERSION_NUM
operator|>=
literal|0x070908
if|if
condition|(
operator|(
name|ssl_capath
operator|=
name|getenv
argument_list|(
literal|"GIT_SSL_CAPATH"
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_CAPATH
argument_list|,
name|ssl_capath
argument_list|)
expr_stmt|;
block|}
endif|#
directive|endif
if|if
condition|(
operator|(
name|ssl_cainfo
operator|=
name|getenv
argument_list|(
literal|"GIT_SSL_CAINFO"
argument_list|)
operator|)
operator|!=
name|NULL
condition|)
block|{
name|curl_easy_setopt
argument_list|(
name|curl
argument_list|,
name|CURLOPT_CAINFO
argument_list|,
name|ssl_cainfo
argument_list|)
expr_stmt|;
block|}
name|alt
operator|=
name|xmalloc
argument_list|(
sizeof|sizeof
argument_list|(
operator|*
name|alt
argument_list|)
argument_list|)
expr_stmt|;
name|alt
operator|->
name|base
operator|=
name|url
expr_stmt|;
name|alt
operator|->
name|got_indices
operator|=
literal|0
expr_stmt|;
name|alt
operator|->
name|packs
operator|=
name|NULL
expr_stmt|;
name|alt
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|initial_base
operator|=
name|url
expr_stmt|;
if|if
condition|(
name|pull
argument_list|(
name|commit_id
argument_list|)
condition|)
return|return
literal|1
return|;
name|curl_slist_free_all
argument_list|(
name|no_pragma_header
argument_list|)
expr_stmt|;
name|curl_global_cleanup
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

