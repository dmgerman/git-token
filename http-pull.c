begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<fcntl.h>
end_include

begin_include
include|#
directive|include
file|<unistd.h>
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

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
file|<errno.h>
end_include

begin_include
include|#
directive|include
file|<stdio.h>
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
DECL|variable|tree
specifier|static
name|int
name|tree
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|commits
specifier|static
name|int
name|commits
init|=
literal|0
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|all
specifier|static
name|int
name|all
init|=
literal|0
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
specifier|static
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
if|if
condition|(
name|has_sha1_file
argument_list|(
name|sha1
argument_list|)
condition|)
block|{
return|return
literal|0
return|;
block|}
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
comment|/*printf("Getting %s\n", hex);*/
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
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|process_tree
specifier|static
name|int
name|process_tree
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|tree
modifier|*
name|tree
init|=
name|lookup_tree
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
name|struct
name|tree_entry_list
modifier|*
name|entries
decl_stmt|;
if|if
condition|(
name|parse_tree
argument_list|(
name|tree
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
for|for
control|(
name|entries
operator|=
name|tree
operator|->
name|entries
init|;
name|entries
condition|;
name|entries
operator|=
name|entries
operator|->
name|next
control|)
block|{
if|if
condition|(
name|fetch
argument_list|(
name|entries
operator|->
name|item
operator|.
name|tree
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|entries
operator|->
name|directory
condition|)
block|{
if|if
condition|(
name|process_tree
argument_list|(
name|entries
operator|->
name|item
operator|.
name|tree
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
block|}
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|process_commit
specifier|static
name|int
name|process_commit
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|commit
modifier|*
name|obj
init|=
name|lookup_commit
argument_list|(
name|sha1
argument_list|)
decl_stmt|;
if|if
condition|(
name|fetch
argument_list|(
name|sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|parse_commit
argument_list|(
name|obj
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|tree
condition|)
block|{
if|if
condition|(
name|fetch
argument_list|(
name|obj
operator|->
name|tree
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
name|process_tree
argument_list|(
name|obj
operator|->
name|tree
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
if|if
condition|(
operator|!
name|all
condition|)
name|tree
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|commits
condition|)
block|{
name|struct
name|commit_list
modifier|*
name|parents
init|=
name|obj
operator|->
name|parents
decl_stmt|;
for|for
control|(
init|;
name|parents
condition|;
name|parents
operator|=
name|parents
operator|->
name|next
control|)
block|{
if|if
condition|(
name|has_sha1_file
argument_list|(
name|parents
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
continue|continue;
if|if
condition|(
name|fetch
argument_list|(
name|parents
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
block|{
comment|/* The server might not have it, and 				 * we don't mind.  				 */
continue|continue;
block|}
if|if
condition|(
name|process_commit
argument_list|(
name|parents
operator|->
name|item
operator|->
name|object
operator|.
name|sha1
argument_list|)
condition|)
return|return
operator|-
literal|1
return|;
block|}
block|}
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
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
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
name|tree
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
name|commits
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
name|all
operator|=
literal|1
expr_stmt|;
name|tree
operator|=
literal|1
expr_stmt|;
name|commits
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
literal|"http-pull [-c] [-t] [-a] commit-id url"
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
name|get_sha1_hex
argument_list|(
name|commit_id
argument_list|,
name|sha1
argument_list|)
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
name|base
operator|=
name|url
expr_stmt|;
if|if
condition|(
name|fetch
argument_list|(
name|sha1
argument_list|)
condition|)
return|return
literal|1
return|;
if|if
condition|(
name|process_commit
argument_list|(
name|sha1
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

