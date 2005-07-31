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
file|"pull.h"
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
DECL|variable|base
specifier|static
name|char
modifier|*
name|base
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
modifier|*
name|url
decl_stmt|;
name|char
modifier|*
name|posn
decl_stmt|;
name|local
operator|=
name|open
argument_list|(
name|filename
argument_list|,
name|O_WRONLY
operator||
name|O_CREAT
operator||
name|O_EXCL
argument_list|,
literal|0666
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
literal|"Couldn't open %s\n"
argument_list|,
name|filename
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
name|url
operator|=
name|xmalloc
argument_list|(
name|strlen
argument_list|(
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
return|return
name|error
argument_list|(
literal|"Couldn't get %s for %s\n"
argument_list|,
name|url
argument_list|,
name|hex
argument_list|)
return|;
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
name|filename
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
name|filename
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
literal|"Couldn't get %s for %s\n"
argument_list|,
name|url
argument_list|,
name|ref
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
literal|"git-http-pull [-c] [-t] [-a] [-d] [-v] [--recover] [-w ref] commit-id url"
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
name|curl_ssl_verify
operator|=
name|gitenv
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
name|base
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
name|curl_global_cleanup
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

