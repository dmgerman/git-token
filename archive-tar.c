begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2005, 2006 Rene Scharfe  */
end_comment

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
file|"tar.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"archive.h"
end_include

begin_define
DECL|macro|RECORDSIZE
define|#
directive|define
name|RECORDSIZE
value|(512)
end_define

begin_define
DECL|macro|BLOCKSIZE
define|#
directive|define
name|BLOCKSIZE
value|(RECORDSIZE * 20)
end_define

begin_decl_stmt
DECL|variable|block
specifier|static
name|char
name|block
index|[
name|BLOCKSIZE
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|offset
specifier|static
name|unsigned
name|long
name|offset
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|archive_time
specifier|static
name|time_t
name|archive_time
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tar_umask
specifier|static
name|int
name|tar_umask
init|=
literal|002
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|verbose
specifier|static
name|int
name|verbose
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|commit
specifier|static
specifier|const
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* writes out the whole block, but only if it is full */
end_comment

begin_function
DECL|function|write_if_needed
specifier|static
name|void
name|write_if_needed
parameter_list|(
name|void
parameter_list|)
block|{
if|if
condition|(
name|offset
operator|==
name|BLOCKSIZE
condition|)
block|{
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|block
argument_list|,
name|BLOCKSIZE
argument_list|)
expr_stmt|;
name|offset
operator|=
literal|0
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/*  * queues up writes, so that all our write(2) calls write exactly one  * full block; pads writes to RECORDSIZE  */
end_comment

begin_function
DECL|function|write_blocked
specifier|static
name|void
name|write_blocked
parameter_list|(
specifier|const
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
specifier|const
name|char
modifier|*
name|buf
init|=
name|data
decl_stmt|;
name|unsigned
name|long
name|tail
decl_stmt|;
if|if
condition|(
name|offset
condition|)
block|{
name|unsigned
name|long
name|chunk
init|=
name|BLOCKSIZE
operator|-
name|offset
decl_stmt|;
if|if
condition|(
name|size
operator|<
name|chunk
condition|)
name|chunk
operator|=
name|size
expr_stmt|;
name|memcpy
argument_list|(
name|block
operator|+
name|offset
argument_list|,
name|buf
argument_list|,
name|chunk
argument_list|)
expr_stmt|;
name|size
operator|-=
name|chunk
expr_stmt|;
name|offset
operator|+=
name|chunk
expr_stmt|;
name|buf
operator|+=
name|chunk
expr_stmt|;
name|write_if_needed
argument_list|()
expr_stmt|;
block|}
while|while
condition|(
name|size
operator|>=
name|BLOCKSIZE
condition|)
block|{
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|buf
argument_list|,
name|BLOCKSIZE
argument_list|)
expr_stmt|;
name|size
operator|-=
name|BLOCKSIZE
expr_stmt|;
name|buf
operator|+=
name|BLOCKSIZE
expr_stmt|;
block|}
if|if
condition|(
name|size
condition|)
block|{
name|memcpy
argument_list|(
name|block
operator|+
name|offset
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|offset
operator|+=
name|size
expr_stmt|;
block|}
name|tail
operator|=
name|offset
operator|%
name|RECORDSIZE
expr_stmt|;
if|if
condition|(
name|tail
condition|)
block|{
name|memset
argument_list|(
name|block
operator|+
name|offset
argument_list|,
literal|0
argument_list|,
name|RECORDSIZE
operator|-
name|tail
argument_list|)
expr_stmt|;
name|offset
operator|+=
name|RECORDSIZE
operator|-
name|tail
expr_stmt|;
block|}
name|write_if_needed
argument_list|()
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * The end of tar archives is marked by 2*512 nul bytes and after that  * follows the rest of the block (if any).  */
end_comment

begin_function
DECL|function|write_trailer
specifier|static
name|void
name|write_trailer
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|tail
init|=
name|BLOCKSIZE
operator|-
name|offset
decl_stmt|;
name|memset
argument_list|(
name|block
operator|+
name|offset
argument_list|,
literal|0
argument_list|,
name|tail
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|block
argument_list|,
name|BLOCKSIZE
argument_list|)
expr_stmt|;
if|if
condition|(
name|tail
operator|<
literal|2
operator|*
name|RECORDSIZE
condition|)
block|{
name|memset
argument_list|(
name|block
argument_list|,
literal|0
argument_list|,
name|offset
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|block
argument_list|,
name|BLOCKSIZE
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/*  * pax extended header records have the format "%u %s=%s\n".  %u contains  * the size of the whole string (including the %u), the first %s is the  * keyword, the second one is the value.  This function constructs such a  * string and appends it to a struct strbuf.  */
end_comment

begin_function
DECL|function|strbuf_append_ext_header
specifier|static
name|void
name|strbuf_append_ext_header
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|keyword
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|,
name|unsigned
name|int
name|valuelen
parameter_list|)
block|{
name|int
name|len
decl_stmt|,
name|tmp
decl_stmt|;
comment|/* "%u %s=%s\n" */
name|len
operator|=
literal|1
operator|+
literal|1
operator|+
name|strlen
argument_list|(
name|keyword
argument_list|)
operator|+
literal|1
operator|+
name|valuelen
operator|+
literal|1
expr_stmt|;
for|for
control|(
name|tmp
operator|=
name|len
init|;
name|tmp
operator|>
literal|9
condition|;
name|tmp
operator|/=
literal|10
control|)
name|len
operator|++
expr_stmt|;
name|strbuf_grow
argument_list|(
name|sb
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
name|sb
argument_list|,
literal|"%u %s="
argument_list|,
name|len
argument_list|,
name|keyword
argument_list|)
expr_stmt|;
name|strbuf_add
argument_list|(
name|sb
argument_list|,
name|value
argument_list|,
name|valuelen
argument_list|)
expr_stmt|;
name|strbuf_addch
argument_list|(
name|sb
argument_list|,
literal|'\n'
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ustar_header_chksum
specifier|static
name|unsigned
name|int
name|ustar_header_chksum
parameter_list|(
specifier|const
name|struct
name|ustar_header
modifier|*
name|header
parameter_list|)
block|{
name|char
modifier|*
name|p
init|=
operator|(
name|char
operator|*
operator|)
name|header
decl_stmt|;
name|unsigned
name|int
name|chksum
init|=
literal|0
decl_stmt|;
while|while
condition|(
name|p
operator|<
name|header
operator|->
name|chksum
condition|)
name|chksum
operator|+=
operator|*
name|p
operator|++
expr_stmt|;
name|chksum
operator|+=
sizeof|sizeof
argument_list|(
name|header
operator|->
name|chksum
argument_list|)
operator|*
literal|' '
expr_stmt|;
name|p
operator|+=
sizeof|sizeof
argument_list|(
name|header
operator|->
name|chksum
argument_list|)
expr_stmt|;
while|while
condition|(
name|p
operator|<
operator|(
name|char
operator|*
operator|)
name|header
operator|+
sizeof|sizeof
argument_list|(
expr|struct
name|ustar_header
argument_list|)
condition|)
name|chksum
operator|+=
operator|*
name|p
operator|++
expr_stmt|;
return|return
name|chksum
return|;
block|}
end_function

begin_function
DECL|function|get_path_prefix
specifier|static
name|int
name|get_path_prefix
parameter_list|(
specifier|const
name|struct
name|strbuf
modifier|*
name|path
parameter_list|,
name|int
name|maxlen
parameter_list|)
block|{
name|int
name|i
init|=
name|path
operator|->
name|len
decl_stmt|;
if|if
condition|(
name|i
operator|>
name|maxlen
condition|)
name|i
operator|=
name|maxlen
expr_stmt|;
do|do
block|{
name|i
operator|--
expr_stmt|;
block|}
do|while
condition|(
name|i
operator|>
literal|0
operator|&&
name|path
operator|->
name|buf
index|[
name|i
index|]
operator|!=
literal|'/'
condition|)
do|;
return|return
name|i
return|;
block|}
end_function

begin_function
DECL|function|write_entry
specifier|static
name|void
name|write_entry
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|struct
name|strbuf
modifier|*
name|path
parameter_list|,
name|unsigned
name|int
name|mode
parameter_list|,
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
block|{
name|struct
name|ustar_header
name|header
decl_stmt|;
name|struct
name|strbuf
name|ext_header
decl_stmt|;
name|memset
argument_list|(
operator|&
name|header
argument_list|,
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|header
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|ext_header
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|sha1
condition|)
block|{
operator|*
name|header
operator|.
name|typeflag
operator|=
name|TYPEFLAG_GLOBAL_HEADER
expr_stmt|;
name|mode
operator|=
literal|0100666
expr_stmt|;
name|strcpy
argument_list|(
name|header
operator|.
name|name
argument_list|,
literal|"pax_global_header"
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
operator|!
name|path
condition|)
block|{
operator|*
name|header
operator|.
name|typeflag
operator|=
name|TYPEFLAG_EXT_HEADER
expr_stmt|;
name|mode
operator|=
literal|0100666
expr_stmt|;
name|sprintf
argument_list|(
name|header
operator|.
name|name
argument_list|,
literal|"%s.paxheader"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|verbose
condition|)
name|fprintf
argument_list|(
name|stderr
argument_list|,
literal|"%.*s\n"
argument_list|,
operator|(
name|int
operator|)
name|path
operator|->
name|len
argument_list|,
name|path
operator|->
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
operator|||
name|S_ISGITLINK
argument_list|(
name|mode
argument_list|)
condition|)
block|{
operator|*
name|header
operator|.
name|typeflag
operator|=
name|TYPEFLAG_DIR
expr_stmt|;
name|mode
operator|=
operator|(
name|mode
operator||
literal|0777
operator|)
operator|&
operator|~
name|tar_umask
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|S_ISLNK
argument_list|(
name|mode
argument_list|)
condition|)
block|{
operator|*
name|header
operator|.
name|typeflag
operator|=
name|TYPEFLAG_LNK
expr_stmt|;
name|mode
operator||=
literal|0777
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|S_ISREG
argument_list|(
name|mode
argument_list|)
condition|)
block|{
operator|*
name|header
operator|.
name|typeflag
operator|=
name|TYPEFLAG_REG
expr_stmt|;
name|mode
operator|=
operator|(
name|mode
operator||
operator|(
operator|(
name|mode
operator|&
literal|0100
operator|)
condition|?
literal|0777
else|:
literal|0666
operator|)
operator|)
operator|&
operator|~
name|tar_umask
expr_stmt|;
block|}
else|else
block|{
name|error
argument_list|(
literal|"unsupported file mode: 0%o (SHA1: %s)"
argument_list|,
name|mode
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|path
operator|->
name|len
operator|>
sizeof|sizeof
argument_list|(
name|header
operator|.
name|name
argument_list|)
condition|)
block|{
name|int
name|plen
init|=
name|get_path_prefix
argument_list|(
name|path
argument_list|,
sizeof|sizeof
argument_list|(
name|header
operator|.
name|prefix
argument_list|)
argument_list|)
decl_stmt|;
name|int
name|rest
init|=
name|path
operator|->
name|len
operator|-
name|plen
operator|-
literal|1
decl_stmt|;
if|if
condition|(
name|plen
operator|>
literal|0
operator|&&
name|rest
operator|<=
sizeof|sizeof
argument_list|(
name|header
operator|.
name|name
argument_list|)
condition|)
block|{
name|memcpy
argument_list|(
name|header
operator|.
name|prefix
argument_list|,
name|path
operator|->
name|buf
argument_list|,
name|plen
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|header
operator|.
name|name
argument_list|,
name|path
operator|->
name|buf
operator|+
name|plen
operator|+
literal|1
argument_list|,
name|rest
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|sprintf
argument_list|(
name|header
operator|.
name|name
argument_list|,
literal|"%s.data"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_append_ext_header
argument_list|(
operator|&
name|ext_header
argument_list|,
literal|"path"
argument_list|,
name|path
operator|->
name|buf
argument_list|,
name|path
operator|->
name|len
argument_list|)
expr_stmt|;
block|}
block|}
else|else
name|memcpy
argument_list|(
name|header
operator|.
name|name
argument_list|,
name|path
operator|->
name|buf
argument_list|,
name|path
operator|->
name|len
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|S_ISLNK
argument_list|(
name|mode
argument_list|)
operator|&&
name|buffer
condition|)
block|{
if|if
condition|(
name|size
operator|>
sizeof|sizeof
argument_list|(
name|header
operator|.
name|linkname
argument_list|)
condition|)
block|{
name|sprintf
argument_list|(
name|header
operator|.
name|linkname
argument_list|,
literal|"see %s.paxheader"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_append_ext_header
argument_list|(
operator|&
name|ext_header
argument_list|,
literal|"linkpath"
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
else|else
name|memcpy
argument_list|(
name|header
operator|.
name|linkname
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
name|sprintf
argument_list|(
name|header
operator|.
name|mode
argument_list|,
literal|"%07o"
argument_list|,
name|mode
operator|&
literal|07777
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|header
operator|.
name|size
argument_list|,
literal|"%011lo"
argument_list|,
name|S_ISREG
argument_list|(
name|mode
argument_list|)
condition|?
name|size
else|:
literal|0
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|header
operator|.
name|mtime
argument_list|,
literal|"%011lo"
argument_list|,
name|archive_time
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|header
operator|.
name|uid
argument_list|,
literal|"%07o"
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|header
operator|.
name|gid
argument_list|,
literal|"%07o"
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|strlcpy
argument_list|(
name|header
operator|.
name|uname
argument_list|,
literal|"root"
argument_list|,
sizeof|sizeof
argument_list|(
name|header
operator|.
name|uname
argument_list|)
argument_list|)
expr_stmt|;
name|strlcpy
argument_list|(
name|header
operator|.
name|gname
argument_list|,
literal|"root"
argument_list|,
sizeof|sizeof
argument_list|(
name|header
operator|.
name|gname
argument_list|)
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|header
operator|.
name|devmajor
argument_list|,
literal|"%07o"
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|header
operator|.
name|devminor
argument_list|,
literal|"%07o"
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|header
operator|.
name|magic
argument_list|,
literal|"ustar"
argument_list|,
literal|6
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|header
operator|.
name|version
argument_list|,
literal|"00"
argument_list|,
literal|2
argument_list|)
expr_stmt|;
name|sprintf
argument_list|(
name|header
operator|.
name|chksum
argument_list|,
literal|"%07o"
argument_list|,
name|ustar_header_chksum
argument_list|(
operator|&
name|header
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ext_header
operator|.
name|len
operator|>
literal|0
condition|)
block|{
name|write_entry
argument_list|(
name|sha1
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
name|ext_header
operator|.
name|buf
argument_list|,
name|ext_header
operator|.
name|len
argument_list|)
expr_stmt|;
block|}
name|strbuf_release
argument_list|(
operator|&
name|ext_header
argument_list|)
expr_stmt|;
name|write_blocked
argument_list|(
operator|&
name|header
argument_list|,
sizeof|sizeof
argument_list|(
name|header
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|S_ISREG
argument_list|(
name|mode
argument_list|)
operator|&&
name|buffer
operator|&&
name|size
operator|>
literal|0
condition|)
name|write_blocked
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|write_global_extended_header
specifier|static
name|void
name|write_global_extended_header
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|strbuf
name|ext_header
decl_stmt|;
name|strbuf_init
argument_list|(
operator|&
name|ext_header
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|strbuf_append_ext_header
argument_list|(
operator|&
name|ext_header
argument_list|,
literal|"comment"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
literal|40
argument_list|)
expr_stmt|;
name|write_entry
argument_list|(
name|NULL
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
name|ext_header
operator|.
name|buf
argument_list|,
name|ext_header
operator|.
name|len
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|ext_header
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|git_tar_config
specifier|static
name|int
name|git_tar_config
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
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|var
argument_list|,
literal|"tar.umask"
argument_list|)
condition|)
block|{
if|if
condition|(
operator|!
name|strcmp
argument_list|(
name|value
argument_list|,
literal|"user"
argument_list|)
condition|)
block|{
name|tar_umask
operator|=
name|umask
argument_list|(
literal|0
argument_list|)
expr_stmt|;
name|umask
argument_list|(
name|tar_umask
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|tar_umask
operator|=
name|git_config_int
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
return|return
name|git_default_config
argument_list|(
name|var
argument_list|,
name|value
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|write_tar_entry
specifier|static
name|int
name|write_tar_entry
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|base
parameter_list|,
name|int
name|baselen
parameter_list|,
specifier|const
name|char
modifier|*
name|filename
parameter_list|,
name|unsigned
name|mode
parameter_list|,
name|int
name|stage
parameter_list|)
block|{
specifier|static
name|struct
name|strbuf
name|path
init|=
name|STRBUF_INIT
decl_stmt|;
name|int
name|filenamelen
init|=
name|strlen
argument_list|(
name|filename
argument_list|)
decl_stmt|;
name|void
modifier|*
name|buffer
decl_stmt|;
name|enum
name|object_type
name|type
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|strbuf_grow
argument_list|(
operator|&
name|path
argument_list|,
name|MAX
argument_list|(
name|PATH_MAX
argument_list|,
name|baselen
operator|+
name|filenamelen
operator|+
literal|1
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_reset
argument_list|(
operator|&
name|path
argument_list|)
expr_stmt|;
name|strbuf_add
argument_list|(
operator|&
name|path
argument_list|,
name|base
argument_list|,
name|baselen
argument_list|)
expr_stmt|;
name|strbuf_add
argument_list|(
operator|&
name|path
argument_list|,
name|filename
argument_list|,
name|filenamelen
argument_list|)
expr_stmt|;
if|if
condition|(
name|S_ISDIR
argument_list|(
name|mode
argument_list|)
operator|||
name|S_ISGITLINK
argument_list|(
name|mode
argument_list|)
condition|)
block|{
name|strbuf_addch
argument_list|(
operator|&
name|path
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
name|buffer
operator|=
name|NULL
expr_stmt|;
name|size
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|buffer
operator|=
name|sha1_file_to_archive
argument_list|(
name|path
operator|.
name|buf
argument_list|,
name|sha1
argument_list|,
name|mode
argument_list|,
operator|&
name|type
argument_list|,
operator|&
name|size
argument_list|,
name|commit
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buffer
condition|)
name|die
argument_list|(
literal|"cannot read %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|write_entry
argument_list|(
name|sha1
argument_list|,
operator|&
name|path
argument_list|,
name|mode
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
return|return
name|READ_TREE_RECURSIVE
return|;
block|}
end_function

begin_function
DECL|function|write_tar_archive
name|int
name|write_tar_archive
parameter_list|(
name|struct
name|archiver_args
modifier|*
name|args
parameter_list|)
block|{
name|int
name|plen
init|=
name|args
operator|->
name|base
condition|?
name|strlen
argument_list|(
name|args
operator|->
name|base
argument_list|)
else|:
literal|0
decl_stmt|;
name|git_config
argument_list|(
name|git_tar_config
argument_list|)
expr_stmt|;
name|archive_time
operator|=
name|args
operator|->
name|time
expr_stmt|;
name|verbose
operator|=
name|args
operator|->
name|verbose
expr_stmt|;
name|commit
operator|=
name|args
operator|->
name|commit
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|commit_sha1
condition|)
name|write_global_extended_header
argument_list|(
name|args
operator|->
name|commit_sha1
argument_list|)
expr_stmt|;
if|if
condition|(
name|args
operator|->
name|base
operator|&&
name|plen
operator|>
literal|0
operator|&&
name|args
operator|->
name|base
index|[
name|plen
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
block|{
name|char
modifier|*
name|base
init|=
name|xstrdup
argument_list|(
name|args
operator|->
name|base
argument_list|)
decl_stmt|;
name|int
name|baselen
init|=
name|strlen
argument_list|(
name|base
argument_list|)
decl_stmt|;
while|while
condition|(
name|baselen
operator|>
literal|0
operator|&&
name|base
index|[
name|baselen
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
name|base
index|[
operator|--
name|baselen
index|]
operator|=
literal|'\0'
expr_stmt|;
name|write_tar_entry
argument_list|(
name|args
operator|->
name|tree
operator|->
name|object
operator|.
name|sha1
argument_list|,
literal|""
argument_list|,
literal|0
argument_list|,
name|base
argument_list|,
literal|040777
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|base
argument_list|)
expr_stmt|;
block|}
name|read_tree_recursive
argument_list|(
name|args
operator|->
name|tree
argument_list|,
name|args
operator|->
name|base
argument_list|,
name|plen
argument_list|,
literal|0
argument_list|,
name|args
operator|->
name|pathspec
argument_list|,
name|write_tar_entry
argument_list|)
expr_stmt|;
name|write_trailer
argument_list|()
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

end_unit

