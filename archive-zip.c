begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (c) 2006 Rene Scharfe  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"archive.h"
end_include

begin_include
include|#
directive|include
file|"streaming.h"
end_include

begin_include
include|#
directive|include
file|"utf8.h"
end_include

begin_include
include|#
directive|include
file|"userdiff.h"
end_include

begin_include
include|#
directive|include
file|"xdiff-interface.h"
end_include

begin_decl_stmt
DECL|variable|zip_date
specifier|static
name|int
name|zip_date
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|zip_time
specifier|static
name|int
name|zip_time
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|zip_dir
specifier|static
name|unsigned
name|char
modifier|*
name|zip_dir
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|zip_dir_size
specifier|static
name|unsigned
name|int
name|zip_dir_size
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|zip_offset
specifier|static
name|unsigned
name|int
name|zip_offset
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|zip_dir_offset
specifier|static
name|unsigned
name|int
name|zip_dir_offset
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|zip_dir_entries
specifier|static
name|unsigned
name|int
name|zip_dir_entries
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|ZIP_DIRECTORY_MIN_SIZE
define|#
directive|define
name|ZIP_DIRECTORY_MIN_SIZE
value|(1024 * 1024)
end_define

begin_define
DECL|macro|ZIP_STREAM
define|#
directive|define
name|ZIP_STREAM
value|(1<<  3)
end_define

begin_define
DECL|macro|ZIP_UTF8
define|#
directive|define
name|ZIP_UTF8
value|(1<< 11)
end_define

begin_struct
DECL|struct|zip_local_header
struct|struct
name|zip_local_header
block|{
DECL|member|magic
name|unsigned
name|char
name|magic
index|[
literal|4
index|]
decl_stmt|;
DECL|member|version
name|unsigned
name|char
name|version
index|[
literal|2
index|]
decl_stmt|;
DECL|member|flags
name|unsigned
name|char
name|flags
index|[
literal|2
index|]
decl_stmt|;
DECL|member|compression_method
name|unsigned
name|char
name|compression_method
index|[
literal|2
index|]
decl_stmt|;
DECL|member|mtime
name|unsigned
name|char
name|mtime
index|[
literal|2
index|]
decl_stmt|;
DECL|member|mdate
name|unsigned
name|char
name|mdate
index|[
literal|2
index|]
decl_stmt|;
DECL|member|crc32
name|unsigned
name|char
name|crc32
index|[
literal|4
index|]
decl_stmt|;
DECL|member|compressed_size
name|unsigned
name|char
name|compressed_size
index|[
literal|4
index|]
decl_stmt|;
DECL|member|size
name|unsigned
name|char
name|size
index|[
literal|4
index|]
decl_stmt|;
DECL|member|filename_length
name|unsigned
name|char
name|filename_length
index|[
literal|2
index|]
decl_stmt|;
DECL|member|extra_length
name|unsigned
name|char
name|extra_length
index|[
literal|2
index|]
decl_stmt|;
DECL|member|_end
name|unsigned
name|char
name|_end
index|[
literal|1
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|zip_data_desc
struct|struct
name|zip_data_desc
block|{
DECL|member|magic
name|unsigned
name|char
name|magic
index|[
literal|4
index|]
decl_stmt|;
DECL|member|crc32
name|unsigned
name|char
name|crc32
index|[
literal|4
index|]
decl_stmt|;
DECL|member|compressed_size
name|unsigned
name|char
name|compressed_size
index|[
literal|4
index|]
decl_stmt|;
DECL|member|size
name|unsigned
name|char
name|size
index|[
literal|4
index|]
decl_stmt|;
DECL|member|_end
name|unsigned
name|char
name|_end
index|[
literal|1
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|zip_dir_header
struct|struct
name|zip_dir_header
block|{
DECL|member|magic
name|unsigned
name|char
name|magic
index|[
literal|4
index|]
decl_stmt|;
DECL|member|creator_version
name|unsigned
name|char
name|creator_version
index|[
literal|2
index|]
decl_stmt|;
DECL|member|version
name|unsigned
name|char
name|version
index|[
literal|2
index|]
decl_stmt|;
DECL|member|flags
name|unsigned
name|char
name|flags
index|[
literal|2
index|]
decl_stmt|;
DECL|member|compression_method
name|unsigned
name|char
name|compression_method
index|[
literal|2
index|]
decl_stmt|;
DECL|member|mtime
name|unsigned
name|char
name|mtime
index|[
literal|2
index|]
decl_stmt|;
DECL|member|mdate
name|unsigned
name|char
name|mdate
index|[
literal|2
index|]
decl_stmt|;
DECL|member|crc32
name|unsigned
name|char
name|crc32
index|[
literal|4
index|]
decl_stmt|;
DECL|member|compressed_size
name|unsigned
name|char
name|compressed_size
index|[
literal|4
index|]
decl_stmt|;
DECL|member|size
name|unsigned
name|char
name|size
index|[
literal|4
index|]
decl_stmt|;
DECL|member|filename_length
name|unsigned
name|char
name|filename_length
index|[
literal|2
index|]
decl_stmt|;
DECL|member|extra_length
name|unsigned
name|char
name|extra_length
index|[
literal|2
index|]
decl_stmt|;
DECL|member|comment_length
name|unsigned
name|char
name|comment_length
index|[
literal|2
index|]
decl_stmt|;
DECL|member|disk
name|unsigned
name|char
name|disk
index|[
literal|2
index|]
decl_stmt|;
DECL|member|attr1
name|unsigned
name|char
name|attr1
index|[
literal|2
index|]
decl_stmt|;
DECL|member|attr2
name|unsigned
name|char
name|attr2
index|[
literal|4
index|]
decl_stmt|;
DECL|member|offset
name|unsigned
name|char
name|offset
index|[
literal|4
index|]
decl_stmt|;
DECL|member|_end
name|unsigned
name|char
name|_end
index|[
literal|1
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|zip_dir_trailer
struct|struct
name|zip_dir_trailer
block|{
DECL|member|magic
name|unsigned
name|char
name|magic
index|[
literal|4
index|]
decl_stmt|;
DECL|member|disk
name|unsigned
name|char
name|disk
index|[
literal|2
index|]
decl_stmt|;
DECL|member|directory_start_disk
name|unsigned
name|char
name|directory_start_disk
index|[
literal|2
index|]
decl_stmt|;
DECL|member|entries_on_this_disk
name|unsigned
name|char
name|entries_on_this_disk
index|[
literal|2
index|]
decl_stmt|;
DECL|member|entries
name|unsigned
name|char
name|entries
index|[
literal|2
index|]
decl_stmt|;
DECL|member|size
name|unsigned
name|char
name|size
index|[
literal|4
index|]
decl_stmt|;
DECL|member|offset
name|unsigned
name|char
name|offset
index|[
literal|4
index|]
decl_stmt|;
DECL|member|comment_length
name|unsigned
name|char
name|comment_length
index|[
literal|2
index|]
decl_stmt|;
DECL|member|_end
name|unsigned
name|char
name|_end
index|[
literal|1
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|zip_extra_mtime
struct|struct
name|zip_extra_mtime
block|{
DECL|member|magic
name|unsigned
name|char
name|magic
index|[
literal|2
index|]
decl_stmt|;
DECL|member|extra_size
name|unsigned
name|char
name|extra_size
index|[
literal|2
index|]
decl_stmt|;
DECL|member|flags
name|unsigned
name|char
name|flags
index|[
literal|1
index|]
decl_stmt|;
DECL|member|mtime
name|unsigned
name|char
name|mtime
index|[
literal|4
index|]
decl_stmt|;
DECL|member|_end
name|unsigned
name|char
name|_end
index|[
literal|1
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * On ARM, padding is added at the end of the struct, so a simple  * sizeof(struct ...) reports two bytes more than the payload size  * we're interested in.  */
end_comment

begin_define
DECL|macro|ZIP_LOCAL_HEADER_SIZE
define|#
directive|define
name|ZIP_LOCAL_HEADER_SIZE
value|offsetof(struct zip_local_header, _end)
end_define

begin_define
DECL|macro|ZIP_DATA_DESC_SIZE
define|#
directive|define
name|ZIP_DATA_DESC_SIZE
value|offsetof(struct zip_data_desc, _end)
end_define

begin_define
DECL|macro|ZIP_DIR_HEADER_SIZE
define|#
directive|define
name|ZIP_DIR_HEADER_SIZE
value|offsetof(struct zip_dir_header, _end)
end_define

begin_define
DECL|macro|ZIP_DIR_TRAILER_SIZE
define|#
directive|define
name|ZIP_DIR_TRAILER_SIZE
value|offsetof(struct zip_dir_trailer, _end)
end_define

begin_define
DECL|macro|ZIP_EXTRA_MTIME_SIZE
define|#
directive|define
name|ZIP_EXTRA_MTIME_SIZE
value|offsetof(struct zip_extra_mtime, _end)
end_define

begin_define
DECL|macro|ZIP_EXTRA_MTIME_PAYLOAD_SIZE
define|#
directive|define
name|ZIP_EXTRA_MTIME_PAYLOAD_SIZE
define|\
value|(ZIP_EXTRA_MTIME_SIZE - offsetof(struct zip_extra_mtime, flags))
end_define

begin_function
DECL|function|copy_le16
specifier|static
name|void
name|copy_le16
parameter_list|(
name|unsigned
name|char
modifier|*
name|dest
parameter_list|,
name|unsigned
name|int
name|n
parameter_list|)
block|{
name|dest
index|[
literal|0
index|]
operator|=
literal|0xff
operator|&
name|n
expr_stmt|;
name|dest
index|[
literal|1
index|]
operator|=
literal|0xff
operator|&
operator|(
name|n
operator|>>
literal|010
operator|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|copy_le32
specifier|static
name|void
name|copy_le32
parameter_list|(
name|unsigned
name|char
modifier|*
name|dest
parameter_list|,
name|unsigned
name|int
name|n
parameter_list|)
block|{
name|dest
index|[
literal|0
index|]
operator|=
literal|0xff
operator|&
name|n
expr_stmt|;
name|dest
index|[
literal|1
index|]
operator|=
literal|0xff
operator|&
operator|(
name|n
operator|>>
literal|010
operator|)
expr_stmt|;
name|dest
index|[
literal|2
index|]
operator|=
literal|0xff
operator|&
operator|(
name|n
operator|>>
literal|020
operator|)
expr_stmt|;
name|dest
index|[
literal|3
index|]
operator|=
literal|0xff
operator|&
operator|(
name|n
operator|>>
literal|030
operator|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|zlib_deflate_raw
specifier|static
name|void
modifier|*
name|zlib_deflate_raw
parameter_list|(
name|void
modifier|*
name|data
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|int
name|compression_level
parameter_list|,
name|unsigned
name|long
modifier|*
name|compressed_size
parameter_list|)
block|{
name|git_zstream
name|stream
decl_stmt|;
name|unsigned
name|long
name|maxsize
decl_stmt|;
name|void
modifier|*
name|buffer
decl_stmt|;
name|int
name|result
decl_stmt|;
name|git_deflate_init_raw
argument_list|(
operator|&
name|stream
argument_list|,
name|compression_level
argument_list|)
expr_stmt|;
name|maxsize
operator|=
name|git_deflate_bound
argument_list|(
operator|&
name|stream
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|buffer
operator|=
name|xmalloc
argument_list|(
name|maxsize
argument_list|)
expr_stmt|;
name|stream
operator|.
name|next_in
operator|=
name|data
expr_stmt|;
name|stream
operator|.
name|avail_in
operator|=
name|size
expr_stmt|;
name|stream
operator|.
name|next_out
operator|=
name|buffer
expr_stmt|;
name|stream
operator|.
name|avail_out
operator|=
name|maxsize
expr_stmt|;
do|do
block|{
name|result
operator|=
name|git_deflate
argument_list|(
operator|&
name|stream
argument_list|,
name|Z_FINISH
argument_list|)
expr_stmt|;
block|}
do|while
condition|(
name|result
operator|==
name|Z_OK
condition|)
do|;
if|if
condition|(
name|result
operator|!=
name|Z_STREAM_END
condition|)
block|{
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|git_deflate_end
argument_list|(
operator|&
name|stream
argument_list|)
expr_stmt|;
operator|*
name|compressed_size
operator|=
name|stream
operator|.
name|total_out
expr_stmt|;
return|return
name|buffer
return|;
block|}
end_function

begin_function
DECL|function|write_zip_data_desc
specifier|static
name|void
name|write_zip_data_desc
parameter_list|(
name|unsigned
name|long
name|size
parameter_list|,
name|unsigned
name|long
name|compressed_size
parameter_list|,
name|unsigned
name|long
name|crc
parameter_list|)
block|{
name|struct
name|zip_data_desc
name|trailer
decl_stmt|;
name|copy_le32
argument_list|(
name|trailer
operator|.
name|magic
argument_list|,
literal|0x08074b50
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|trailer
operator|.
name|crc32
argument_list|,
name|crc
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|trailer
operator|.
name|compressed_size
argument_list|,
name|compressed_size
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|trailer
operator|.
name|size
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
operator|&
name|trailer
argument_list|,
name|ZIP_DATA_DESC_SIZE
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|set_zip_dir_data_desc
specifier|static
name|void
name|set_zip_dir_data_desc
parameter_list|(
name|struct
name|zip_dir_header
modifier|*
name|header
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|unsigned
name|long
name|compressed_size
parameter_list|,
name|unsigned
name|long
name|crc
parameter_list|)
block|{
name|copy_le32
argument_list|(
name|header
operator|->
name|crc32
argument_list|,
name|crc
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|header
operator|->
name|compressed_size
argument_list|,
name|compressed_size
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|header
operator|->
name|size
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|set_zip_header_data_desc
specifier|static
name|void
name|set_zip_header_data_desc
parameter_list|(
name|struct
name|zip_local_header
modifier|*
name|header
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|,
name|unsigned
name|long
name|compressed_size
parameter_list|,
name|unsigned
name|long
name|crc
parameter_list|)
block|{
name|copy_le32
argument_list|(
name|header
operator|->
name|crc32
argument_list|,
name|crc
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|header
operator|->
name|compressed_size
argument_list|,
name|compressed_size
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|header
operator|->
name|size
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|has_only_ascii
specifier|static
name|int
name|has_only_ascii
parameter_list|(
specifier|const
name|char
modifier|*
name|s
parameter_list|)
block|{
for|for
control|(
init|;
condition|;
control|)
block|{
name|int
name|c
init|=
operator|*
name|s
operator|++
decl_stmt|;
if|if
condition|(
name|c
operator|==
literal|'\0'
condition|)
return|return
literal|1
return|;
if|if
condition|(
operator|!
name|isascii
argument_list|(
name|c
argument_list|)
condition|)
return|return
literal|0
return|;
block|}
block|}
end_function

begin_function
DECL|function|entry_is_binary
specifier|static
name|int
name|entry_is_binary
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|void
modifier|*
name|buffer
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|struct
name|userdiff_driver
modifier|*
name|driver
init|=
name|userdiff_find_by_path
argument_list|(
name|path
argument_list|)
decl_stmt|;
if|if
condition|(
operator|!
name|driver
condition|)
name|driver
operator|=
name|userdiff_find_by_name
argument_list|(
literal|"default"
argument_list|)
expr_stmt|;
if|if
condition|(
name|driver
operator|->
name|binary
operator|!=
operator|-
literal|1
condition|)
return|return
name|driver
operator|->
name|binary
return|;
return|return
name|buffer_is_binary
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|)
return|;
block|}
end_function

begin_define
DECL|macro|STREAM_BUFFER_SIZE
define|#
directive|define
name|STREAM_BUFFER_SIZE
value|(1024 * 16)
end_define

begin_function
DECL|function|write_zip_entry
specifier|static
name|int
name|write_zip_entry
parameter_list|(
name|struct
name|archiver_args
modifier|*
name|args
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|size_t
name|pathlen
parameter_list|,
name|unsigned
name|int
name|mode
parameter_list|)
block|{
name|struct
name|zip_local_header
name|header
decl_stmt|;
name|struct
name|zip_dir_header
name|dirent
decl_stmt|;
name|struct
name|zip_extra_mtime
name|extra
decl_stmt|;
name|unsigned
name|long
name|attr2
decl_stmt|;
name|unsigned
name|long
name|compressed_size
decl_stmt|;
name|unsigned
name|long
name|crc
decl_stmt|;
name|unsigned
name|long
name|direntsize
decl_stmt|;
name|int
name|method
decl_stmt|;
name|unsigned
name|char
modifier|*
name|out
decl_stmt|;
name|void
modifier|*
name|deflated
init|=
name|NULL
decl_stmt|;
name|void
modifier|*
name|buffer
decl_stmt|;
name|struct
name|git_istream
modifier|*
name|stream
init|=
name|NULL
decl_stmt|;
name|unsigned
name|long
name|flags
init|=
literal|0
decl_stmt|;
name|unsigned
name|long
name|size
decl_stmt|;
name|int
name|is_binary
init|=
operator|-
literal|1
decl_stmt|;
specifier|const
name|char
modifier|*
name|path_without_prefix
init|=
name|path
operator|+
name|args
operator|->
name|baselen
decl_stmt|;
name|crc
operator|=
name|crc32
argument_list|(
literal|0
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|has_only_ascii
argument_list|(
name|path
argument_list|)
condition|)
block|{
if|if
condition|(
name|is_utf8
argument_list|(
name|path
argument_list|)
condition|)
name|flags
operator||=
name|ZIP_UTF8
expr_stmt|;
else|else
name|warning
argument_list|(
literal|"Path is not valid UTF-8: %s"
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|pathlen
operator|>
literal|0xffff
condition|)
block|{
return|return
name|error
argument_list|(
literal|"path too long (%d chars, SHA1: %s): %s"
argument_list|,
operator|(
name|int
operator|)
name|pathlen
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|path
argument_list|)
return|;
block|}
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
name|method
operator|=
literal|0
expr_stmt|;
name|attr2
operator|=
literal|16
expr_stmt|;
name|out
operator|=
name|NULL
expr_stmt|;
name|size
operator|=
literal|0
expr_stmt|;
name|compressed_size
operator|=
literal|0
expr_stmt|;
name|buffer
operator|=
name|NULL
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|S_ISREG
argument_list|(
name|mode
argument_list|)
operator|||
name|S_ISLNK
argument_list|(
name|mode
argument_list|)
condition|)
block|{
name|enum
name|object_type
name|type
init|=
name|sha1_object_info
argument_list|(
name|sha1
argument_list|,
operator|&
name|size
argument_list|)
decl_stmt|;
name|method
operator|=
literal|0
expr_stmt|;
name|attr2
operator|=
name|S_ISLNK
argument_list|(
name|mode
argument_list|)
condition|?
operator|(
operator|(
name|mode
operator||
literal|0777
operator|)
operator|<<
literal|16
operator|)
else|:
operator|(
name|mode
operator|&
literal|0111
operator|)
condition|?
operator|(
operator|(
name|mode
operator|)
operator|<<
literal|16
operator|)
else|:
literal|0
expr_stmt|;
if|if
condition|(
name|S_ISREG
argument_list|(
name|mode
argument_list|)
operator|&&
name|args
operator|->
name|compression_level
operator|!=
literal|0
operator|&&
name|size
operator|>
literal|0
condition|)
name|method
operator|=
literal|8
expr_stmt|;
if|if
condition|(
name|S_ISREG
argument_list|(
name|mode
argument_list|)
operator|&&
name|type
operator|==
name|OBJ_BLOB
operator|&&
operator|!
name|args
operator|->
name|convert
operator|&&
name|size
operator|>
name|big_file_threshold
condition|)
block|{
name|stream
operator|=
name|open_istream
argument_list|(
name|sha1
argument_list|,
operator|&
name|type
argument_list|,
operator|&
name|size
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|stream
condition|)
return|return
name|error
argument_list|(
literal|"cannot stream blob %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
return|;
name|flags
operator||=
name|ZIP_STREAM
expr_stmt|;
name|out
operator|=
name|buffer
operator|=
name|NULL
expr_stmt|;
block|}
else|else
block|{
name|buffer
operator|=
name|sha1_file_to_archive
argument_list|(
name|args
argument_list|,
name|path
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
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|buffer
condition|)
return|return
name|error
argument_list|(
literal|"cannot read %s"
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|)
return|;
name|crc
operator|=
name|crc32
argument_list|(
name|crc
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|is_binary
operator|=
name|entry_is_binary
argument_list|(
name|path_without_prefix
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|out
operator|=
name|buffer
expr_stmt|;
block|}
name|compressed_size
operator|=
operator|(
name|method
operator|==
literal|0
operator|)
condition|?
name|size
else|:
literal|0
expr_stmt|;
block|}
else|else
block|{
return|return
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
return|;
block|}
if|if
condition|(
name|buffer
operator|&&
name|method
operator|==
literal|8
condition|)
block|{
name|out
operator|=
name|deflated
operator|=
name|zlib_deflate_raw
argument_list|(
name|buffer
argument_list|,
name|size
argument_list|,
name|args
operator|->
name|compression_level
argument_list|,
operator|&
name|compressed_size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|out
operator|||
name|compressed_size
operator|>=
name|size
condition|)
block|{
name|out
operator|=
name|buffer
expr_stmt|;
name|method
operator|=
literal|0
expr_stmt|;
name|compressed_size
operator|=
name|size
expr_stmt|;
block|}
block|}
name|copy_le16
argument_list|(
name|extra
operator|.
name|magic
argument_list|,
literal|0x5455
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|extra
operator|.
name|extra_size
argument_list|,
name|ZIP_EXTRA_MTIME_PAYLOAD_SIZE
argument_list|)
expr_stmt|;
name|extra
operator|.
name|flags
index|[
literal|0
index|]
operator|=
literal|1
expr_stmt|;
comment|/* just mtime */
name|copy_le32
argument_list|(
name|extra
operator|.
name|mtime
argument_list|,
name|args
operator|->
name|time
argument_list|)
expr_stmt|;
comment|/* make sure we have enough free space in the dictionary */
name|direntsize
operator|=
name|ZIP_DIR_HEADER_SIZE
operator|+
name|pathlen
operator|+
name|ZIP_EXTRA_MTIME_SIZE
expr_stmt|;
while|while
condition|(
name|zip_dir_size
operator|<
name|zip_dir_offset
operator|+
name|direntsize
condition|)
block|{
name|zip_dir_size
operator|+=
name|ZIP_DIRECTORY_MIN_SIZE
expr_stmt|;
name|zip_dir
operator|=
name|xrealloc
argument_list|(
name|zip_dir
argument_list|,
name|zip_dir_size
argument_list|)
expr_stmt|;
block|}
name|copy_le32
argument_list|(
name|dirent
operator|.
name|magic
argument_list|,
literal|0x02014b50
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|creator_version
argument_list|,
name|S_ISLNK
argument_list|(
name|mode
argument_list|)
operator|||
operator|(
name|S_ISREG
argument_list|(
name|mode
argument_list|)
operator|&&
operator|(
name|mode
operator|&
literal|0111
operator|)
operator|)
condition|?
literal|0x0317
else|:
literal|0
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|version
argument_list|,
literal|10
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|flags
argument_list|,
name|flags
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|compression_method
argument_list|,
name|method
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|mtime
argument_list|,
name|zip_time
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|mdate
argument_list|,
name|zip_date
argument_list|)
expr_stmt|;
name|set_zip_dir_data_desc
argument_list|(
operator|&
name|dirent
argument_list|,
name|size
argument_list|,
name|compressed_size
argument_list|,
name|crc
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|filename_length
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|extra_length
argument_list|,
name|ZIP_EXTRA_MTIME_SIZE
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|comment_length
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|disk
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|dirent
operator|.
name|attr2
argument_list|,
name|attr2
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|dirent
operator|.
name|offset
argument_list|,
name|zip_offset
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|header
operator|.
name|magic
argument_list|,
literal|0x04034b50
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|header
operator|.
name|version
argument_list|,
literal|10
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|header
operator|.
name|flags
argument_list|,
name|flags
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|header
operator|.
name|compression_method
argument_list|,
name|method
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|header
operator|.
name|mtime
argument_list|,
name|zip_time
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|header
operator|.
name|mdate
argument_list|,
name|zip_date
argument_list|)
expr_stmt|;
name|set_zip_header_data_desc
argument_list|(
operator|&
name|header
argument_list|,
name|size
argument_list|,
name|compressed_size
argument_list|,
name|crc
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|header
operator|.
name|filename_length
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|header
operator|.
name|extra_length
argument_list|,
name|ZIP_EXTRA_MTIME_SIZE
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
operator|&
name|header
argument_list|,
name|ZIP_LOCAL_HEADER_SIZE
argument_list|)
expr_stmt|;
name|zip_offset
operator|+=
name|ZIP_LOCAL_HEADER_SIZE
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|path
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
name|zip_offset
operator|+=
name|pathlen
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
operator|&
name|extra
argument_list|,
name|ZIP_EXTRA_MTIME_SIZE
argument_list|)
expr_stmt|;
name|zip_offset
operator|+=
name|ZIP_EXTRA_MTIME_SIZE
expr_stmt|;
if|if
condition|(
name|stream
operator|&&
name|method
operator|==
literal|0
condition|)
block|{
name|unsigned
name|char
name|buf
index|[
name|STREAM_BUFFER_SIZE
index|]
decl_stmt|;
name|ssize_t
name|readlen
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|readlen
operator|=
name|read_istream
argument_list|(
name|stream
argument_list|,
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|readlen
operator|<=
literal|0
condition|)
break|break;
name|crc
operator|=
name|crc32
argument_list|(
name|crc
argument_list|,
name|buf
argument_list|,
name|readlen
argument_list|)
expr_stmt|;
if|if
condition|(
name|is_binary
operator|==
operator|-
literal|1
condition|)
name|is_binary
operator|=
name|entry_is_binary
argument_list|(
name|path_without_prefix
argument_list|,
name|buf
argument_list|,
name|readlen
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|buf
argument_list|,
name|readlen
argument_list|)
expr_stmt|;
block|}
name|close_istream
argument_list|(
name|stream
argument_list|)
expr_stmt|;
if|if
condition|(
name|readlen
condition|)
return|return
name|readlen
return|;
name|compressed_size
operator|=
name|size
expr_stmt|;
name|zip_offset
operator|+=
name|compressed_size
expr_stmt|;
name|write_zip_data_desc
argument_list|(
name|size
argument_list|,
name|compressed_size
argument_list|,
name|crc
argument_list|)
expr_stmt|;
name|zip_offset
operator|+=
name|ZIP_DATA_DESC_SIZE
expr_stmt|;
name|set_zip_dir_data_desc
argument_list|(
operator|&
name|dirent
argument_list|,
name|size
argument_list|,
name|compressed_size
argument_list|,
name|crc
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|stream
operator|&&
name|method
operator|==
literal|8
condition|)
block|{
name|unsigned
name|char
name|buf
index|[
name|STREAM_BUFFER_SIZE
index|]
decl_stmt|;
name|ssize_t
name|readlen
decl_stmt|;
name|git_zstream
name|zstream
decl_stmt|;
name|int
name|result
decl_stmt|;
name|size_t
name|out_len
decl_stmt|;
name|unsigned
name|char
name|compressed
index|[
name|STREAM_BUFFER_SIZE
operator|*
literal|2
index|]
decl_stmt|;
name|git_deflate_init_raw
argument_list|(
operator|&
name|zstream
argument_list|,
name|args
operator|->
name|compression_level
argument_list|)
expr_stmt|;
name|compressed_size
operator|=
literal|0
expr_stmt|;
name|zstream
operator|.
name|next_out
operator|=
name|compressed
expr_stmt|;
name|zstream
operator|.
name|avail_out
operator|=
sizeof|sizeof
argument_list|(
name|compressed
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|readlen
operator|=
name|read_istream
argument_list|(
name|stream
argument_list|,
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|readlen
operator|<=
literal|0
condition|)
break|break;
name|crc
operator|=
name|crc32
argument_list|(
name|crc
argument_list|,
name|buf
argument_list|,
name|readlen
argument_list|)
expr_stmt|;
if|if
condition|(
name|is_binary
operator|==
operator|-
literal|1
condition|)
name|is_binary
operator|=
name|entry_is_binary
argument_list|(
name|path_without_prefix
argument_list|,
name|buf
argument_list|,
name|readlen
argument_list|)
expr_stmt|;
name|zstream
operator|.
name|next_in
operator|=
name|buf
expr_stmt|;
name|zstream
operator|.
name|avail_in
operator|=
name|readlen
expr_stmt|;
name|result
operator|=
name|git_deflate
argument_list|(
operator|&
name|zstream
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|result
operator|!=
name|Z_OK
condition|)
name|die
argument_list|(
literal|"deflate error (%d)"
argument_list|,
name|result
argument_list|)
expr_stmt|;
name|out_len
operator|=
name|zstream
operator|.
name|next_out
operator|-
name|compressed
expr_stmt|;
if|if
condition|(
name|out_len
operator|>
literal|0
condition|)
block|{
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|compressed
argument_list|,
name|out_len
argument_list|)
expr_stmt|;
name|compressed_size
operator|+=
name|out_len
expr_stmt|;
name|zstream
operator|.
name|next_out
operator|=
name|compressed
expr_stmt|;
name|zstream
operator|.
name|avail_out
operator|=
sizeof|sizeof
argument_list|(
name|compressed
argument_list|)
expr_stmt|;
block|}
block|}
name|close_istream
argument_list|(
name|stream
argument_list|)
expr_stmt|;
if|if
condition|(
name|readlen
condition|)
return|return
name|readlen
return|;
name|zstream
operator|.
name|next_in
operator|=
name|buf
expr_stmt|;
name|zstream
operator|.
name|avail_in
operator|=
literal|0
expr_stmt|;
name|result
operator|=
name|git_deflate
argument_list|(
operator|&
name|zstream
argument_list|,
name|Z_FINISH
argument_list|)
expr_stmt|;
if|if
condition|(
name|result
operator|!=
name|Z_STREAM_END
condition|)
name|die
argument_list|(
literal|"deflate error (%d)"
argument_list|,
name|result
argument_list|)
expr_stmt|;
name|git_deflate_end
argument_list|(
operator|&
name|zstream
argument_list|)
expr_stmt|;
name|out_len
operator|=
name|zstream
operator|.
name|next_out
operator|-
name|compressed
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|compressed
argument_list|,
name|out_len
argument_list|)
expr_stmt|;
name|compressed_size
operator|+=
name|out_len
expr_stmt|;
name|zip_offset
operator|+=
name|compressed_size
expr_stmt|;
name|write_zip_data_desc
argument_list|(
name|size
argument_list|,
name|compressed_size
argument_list|,
name|crc
argument_list|)
expr_stmt|;
name|zip_offset
operator|+=
name|ZIP_DATA_DESC_SIZE
expr_stmt|;
name|set_zip_dir_data_desc
argument_list|(
operator|&
name|dirent
argument_list|,
name|size
argument_list|,
name|compressed_size
argument_list|,
name|crc
argument_list|)
expr_stmt|;
block|}
elseif|else
if|if
condition|(
name|compressed_size
operator|>
literal|0
condition|)
block|{
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|out
argument_list|,
name|compressed_size
argument_list|)
expr_stmt|;
name|zip_offset
operator|+=
name|compressed_size
expr_stmt|;
block|}
name|free
argument_list|(
name|deflated
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|dirent
operator|.
name|attr1
argument_list|,
operator|!
name|is_binary
argument_list|)
expr_stmt|;
name|memcpy
argument_list|(
name|zip_dir
operator|+
name|zip_dir_offset
argument_list|,
operator|&
name|dirent
argument_list|,
name|ZIP_DIR_HEADER_SIZE
argument_list|)
expr_stmt|;
name|zip_dir_offset
operator|+=
name|ZIP_DIR_HEADER_SIZE
expr_stmt|;
name|memcpy
argument_list|(
name|zip_dir
operator|+
name|zip_dir_offset
argument_list|,
name|path
argument_list|,
name|pathlen
argument_list|)
expr_stmt|;
name|zip_dir_offset
operator|+=
name|pathlen
expr_stmt|;
name|memcpy
argument_list|(
name|zip_dir
operator|+
name|zip_dir_offset
argument_list|,
operator|&
name|extra
argument_list|,
name|ZIP_EXTRA_MTIME_SIZE
argument_list|)
expr_stmt|;
name|zip_dir_offset
operator|+=
name|ZIP_EXTRA_MTIME_SIZE
expr_stmt|;
name|zip_dir_entries
operator|++
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|write_zip_trailer
specifier|static
name|void
name|write_zip_trailer
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
block|{
name|struct
name|zip_dir_trailer
name|trailer
decl_stmt|;
name|copy_le32
argument_list|(
name|trailer
operator|.
name|magic
argument_list|,
literal|0x06054b50
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|trailer
operator|.
name|disk
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|trailer
operator|.
name|directory_start_disk
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|trailer
operator|.
name|entries_on_this_disk
argument_list|,
name|zip_dir_entries
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|trailer
operator|.
name|entries
argument_list|,
name|zip_dir_entries
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|trailer
operator|.
name|size
argument_list|,
name|zip_dir_offset
argument_list|)
expr_stmt|;
name|copy_le32
argument_list|(
name|trailer
operator|.
name|offset
argument_list|,
name|zip_offset
argument_list|)
expr_stmt|;
name|copy_le16
argument_list|(
name|trailer
operator|.
name|comment_length
argument_list|,
name|sha1
condition|?
name|GIT_SHA1_HEXSZ
else|:
literal|0
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|zip_dir
argument_list|,
name|zip_dir_offset
argument_list|)
expr_stmt|;
name|write_or_die
argument_list|(
literal|1
argument_list|,
operator|&
name|trailer
argument_list|,
name|ZIP_DIR_TRAILER_SIZE
argument_list|)
expr_stmt|;
if|if
condition|(
name|sha1
condition|)
name|write_or_die
argument_list|(
literal|1
argument_list|,
name|sha1_to_hex
argument_list|(
name|sha1
argument_list|)
argument_list|,
name|GIT_SHA1_HEXSZ
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|dos_time
specifier|static
name|void
name|dos_time
parameter_list|(
name|time_t
modifier|*
name|time
parameter_list|,
name|int
modifier|*
name|dos_date
parameter_list|,
name|int
modifier|*
name|dos_time
parameter_list|)
block|{
name|struct
name|tm
modifier|*
name|t
init|=
name|localtime
argument_list|(
name|time
argument_list|)
decl_stmt|;
operator|*
name|dos_date
operator|=
name|t
operator|->
name|tm_mday
operator|+
operator|(
name|t
operator|->
name|tm_mon
operator|+
literal|1
operator|)
operator|*
literal|32
operator|+
operator|(
name|t
operator|->
name|tm_year
operator|+
literal|1900
operator|-
literal|1980
operator|)
operator|*
literal|512
expr_stmt|;
operator|*
name|dos_time
operator|=
name|t
operator|->
name|tm_sec
operator|/
literal|2
operator|+
name|t
operator|->
name|tm_min
operator|*
literal|32
operator|+
name|t
operator|->
name|tm_hour
operator|*
literal|2048
expr_stmt|;
block|}
end_function

begin_function
DECL|function|write_zip_archive
specifier|static
name|int
name|write_zip_archive
parameter_list|(
specifier|const
name|struct
name|archiver
modifier|*
name|ar
parameter_list|,
name|struct
name|archiver_args
modifier|*
name|args
parameter_list|)
block|{
name|int
name|err
decl_stmt|;
name|dos_time
argument_list|(
operator|&
name|args
operator|->
name|time
argument_list|,
operator|&
name|zip_date
argument_list|,
operator|&
name|zip_time
argument_list|)
expr_stmt|;
name|zip_dir
operator|=
name|xmalloc
argument_list|(
name|ZIP_DIRECTORY_MIN_SIZE
argument_list|)
expr_stmt|;
name|zip_dir_size
operator|=
name|ZIP_DIRECTORY_MIN_SIZE
expr_stmt|;
name|err
operator|=
name|write_archive_entries
argument_list|(
name|args
argument_list|,
name|write_zip_entry
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|err
condition|)
name|write_zip_trailer
argument_list|(
name|args
operator|->
name|commit_sha1
argument_list|)
expr_stmt|;
name|free
argument_list|(
name|zip_dir
argument_list|)
expr_stmt|;
return|return
name|err
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|zip_archiver
specifier|static
name|struct
name|archiver
name|zip_archiver
init|=
block|{
literal|"zip"
block|,
name|write_zip_archive
block|,
name|ARCHIVER_WANT_COMPRESSION_LEVELS
operator||
name|ARCHIVER_REMOTE
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|init_zip_archiver
name|void
name|init_zip_archiver
parameter_list|(
name|void
parameter_list|)
block|{
name|register_archiver
argument_list|(
operator|&
name|zip_archiver
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

