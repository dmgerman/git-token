begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|CACHE_H
end_ifndef

begin_define
DECL|macro|CACHE_H
define|#
directive|define
name|CACHE_H
end_define

begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_include
include|#
directive|include
file|<sys/stat.h>
end_include

begin_include
include|#
directive|include
file|<fcntl.h>
end_include

begin_include
include|#
directive|include
file|<stddef.h>
end_include

begin_include
include|#
directive|include
file|<stdlib.h>
end_include

begin_include
include|#
directive|include
file|<stdarg.h>
end_include

begin_include
include|#
directive|include
file|<string.h>
end_include

begin_include
include|#
directive|include
file|<errno.h>
end_include

begin_include
include|#
directive|include
file|<sys/mman.h>
end_include

begin_include
include|#
directive|include
file|<openssl/sha.h>
end_include

begin_include
include|#
directive|include
file|<zlib.h>
end_include

begin_comment
comment|/*  * Basic data structures for the directory cache  *  * NOTE NOTE NOTE! This is all in the native CPU byte format. It's  * not even trying to be portable. It's trying to be efficient. It's  * just a cache, after all.  */
end_comment

begin_define
DECL|macro|CACHE_SIGNATURE
define|#
directive|define
name|CACHE_SIGNATURE
value|0x44495243
end_define

begin_comment
DECL|macro|CACHE_SIGNATURE
comment|/* "DIRC" */
end_comment

begin_struct
DECL|struct|cache_header
struct|struct
name|cache_header
block|{
DECL|member|signature
name|unsigned
name|int
name|signature
decl_stmt|;
DECL|member|version
name|unsigned
name|int
name|version
decl_stmt|;
DECL|member|entries
name|unsigned
name|int
name|entries
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * The "cache_time" is just the low 32 bits of the  * time. It doesn't matter if it overflows - we only  * check it for equality in the 32 bits we save.  */
end_comment

begin_struct
DECL|struct|cache_time
struct|struct
name|cache_time
block|{
DECL|member|sec
name|unsigned
name|int
name|sec
decl_stmt|;
DECL|member|nsec
name|unsigned
name|int
name|nsec
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * dev/ino/uid/gid/size are also just tracked to the low 32 bits  * Again - this is just a (very strong in practice) heuristic that  * the inode hasn't changed.  */
end_comment

begin_struct
DECL|struct|cache_entry
struct|struct
name|cache_entry
block|{
DECL|member|ctime
name|struct
name|cache_time
name|ctime
decl_stmt|;
DECL|member|mtime
name|struct
name|cache_time
name|mtime
decl_stmt|;
DECL|member|st_dev
name|unsigned
name|int
name|st_dev
decl_stmt|;
DECL|member|st_ino
name|unsigned
name|int
name|st_ino
decl_stmt|;
DECL|member|st_mode
name|unsigned
name|int
name|st_mode
decl_stmt|;
DECL|member|st_uid
name|unsigned
name|int
name|st_uid
decl_stmt|;
DECL|member|st_gid
name|unsigned
name|int
name|st_gid
decl_stmt|;
DECL|member|st_size
name|unsigned
name|int
name|st_size
decl_stmt|;
DECL|member|sha1
name|unsigned
name|char
name|sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|namelen
name|unsigned
name|short
name|namelen
decl_stmt|;
DECL|member|name
name|char
name|name
index|[
literal|0
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
DECL|variable|sha1_file_directory
specifier|const
name|char
modifier|*
name|sha1_file_directory
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|active_cache
name|struct
name|cache_entry
modifier|*
modifier|*
name|active_cache
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|active_nr
DECL|variable|active_alloc
name|unsigned
name|int
name|active_nr
decl_stmt|,
name|active_alloc
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|DB_ENVIRONMENT
define|#
directive|define
name|DB_ENVIRONMENT
value|"SHA1_FILE_DIRECTORY"
end_define

begin_define
DECL|macro|DEFAULT_DB_ENVIRONMENT
define|#
directive|define
name|DEFAULT_DB_ENVIRONMENT
value|".dircache/objects"
end_define

begin_define
DECL|macro|cache_entry_size
define|#
directive|define
name|cache_entry_size
parameter_list|(
name|len
parameter_list|)
value|((offsetof(struct cache_entry,name) + (len) + 8)& ~7)
end_define

begin_define
DECL|macro|ce_size
define|#
directive|define
name|ce_size
parameter_list|(
name|ce
parameter_list|)
value|cache_entry_size((ce)->namelen)
end_define

begin_define
DECL|macro|alloc_nr
define|#
directive|define
name|alloc_nr
parameter_list|(
name|x
parameter_list|)
value|(((x)+16)*3/2)
end_define

begin_comment
comment|/* Initialize and use the cache information */
end_comment

begin_function_decl
specifier|extern
name|int
name|read_cache
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|write_cache
parameter_list|(
name|int
name|newfd
parameter_list|,
name|struct
name|cache_entry
modifier|*
modifier|*
name|cache
parameter_list|,
name|int
name|entries
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|cache_name_pos
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|namelen
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|add_cache_entry
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|remove_file_from_cache
parameter_list|(
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|cache_match_stat
parameter_list|(
name|struct
name|cache_entry
modifier|*
name|ce
parameter_list|,
name|struct
name|stat
modifier|*
name|st
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|MTIME_CHANGED
define|#
directive|define
name|MTIME_CHANGED
value|0x0001
end_define

begin_define
DECL|macro|CTIME_CHANGED
define|#
directive|define
name|CTIME_CHANGED
value|0x0002
end_define

begin_define
DECL|macro|OWNER_CHANGED
define|#
directive|define
name|OWNER_CHANGED
value|0x0004
end_define

begin_define
DECL|macro|MODE_CHANGED
define|#
directive|define
name|MODE_CHANGED
value|0x0008
end_define

begin_define
DECL|macro|INODE_CHANGED
define|#
directive|define
name|INODE_CHANGED
value|0x0010
end_define

begin_define
DECL|macro|DATA_CHANGED
define|#
directive|define
name|DATA_CHANGED
value|0x0020
end_define

begin_comment
comment|/* Return a statically allocated filename matching the sha1 signature */
end_comment

begin_function_decl
specifier|extern
name|char
modifier|*
name|sha1_file_name
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Write a memory buffer out to the sha file */
end_comment

begin_function_decl
specifier|extern
name|int
name|write_sha1_buffer
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|unsigned
name|int
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Read and unpack a sha1 file into memory, write memory to a sha1 file */
end_comment

begin_function_decl
specifier|extern
name|void
modifier|*
name|map_sha1_file
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|unsigned
name|long
modifier|*
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|unpack_sha1_file
parameter_list|(
name|void
modifier|*
name|map
parameter_list|,
name|unsigned
name|long
name|mapsize
parameter_list|,
name|char
modifier|*
name|type
parameter_list|,
name|unsigned
name|long
modifier|*
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
modifier|*
name|read_sha1_file
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|char
modifier|*
name|type
parameter_list|,
name|unsigned
name|long
modifier|*
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|write_sha1_file
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|check_sha1_signature
parameter_list|(
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Convert to/from hex/sha1 representation */
end_comment

begin_function_decl
specifier|extern
name|int
name|get_sha1_hex
parameter_list|(
specifier|const
name|char
modifier|*
name|hex
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
name|char
modifier|*
name|sha1_to_hex
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* static buffer result! */
end_comment

begin_comment
comment|/* General helper functions */
end_comment

begin_function_decl
specifier|extern
name|void
name|usage
parameter_list|(
specifier|const
name|char
modifier|*
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|cache_name_compare
parameter_list|(
specifier|const
name|char
modifier|*
name|name1
parameter_list|,
name|int
name|len1
parameter_list|,
specifier|const
name|char
modifier|*
name|name2
parameter_list|,
name|int
name|len2
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* CACHE_H */
end_comment

end_unit

