begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|ARCHIVE_H
end_ifndef

begin_define
DECL|macro|ARCHIVE_H
define|#
directive|define
name|ARCHIVE_H
end_define

begin_struct
DECL|struct|archiver_args
struct|struct
name|archiver_args
block|{
DECL|member|base
specifier|const
name|char
modifier|*
name|base
decl_stmt|;
DECL|member|baselen
name|size_t
name|baselen
decl_stmt|;
DECL|member|tree
name|struct
name|tree
modifier|*
name|tree
decl_stmt|;
DECL|member|commit_sha1
specifier|const
name|unsigned
name|char
modifier|*
name|commit_sha1
decl_stmt|;
DECL|member|commit
specifier|const
name|struct
name|commit
modifier|*
name|commit
decl_stmt|;
DECL|member|time
name|time_t
name|time
decl_stmt|;
DECL|member|pathspec
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
decl_stmt|;
DECL|member|verbose
name|unsigned
name|int
name|verbose
range|:
literal|1
decl_stmt|;
DECL|member|worktree_attributes
name|unsigned
name|int
name|worktree_attributes
range|:
literal|1
decl_stmt|;
DECL|member|compression_level
name|int
name|compression_level
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|ARCHIVER_WANT_COMPRESSION_LEVELS
define|#
directive|define
name|ARCHIVER_WANT_COMPRESSION_LEVELS
value|1
end_define

begin_struct
DECL|struct|archiver
struct|struct
name|archiver
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|write_archive
name|int
function_decl|(
modifier|*
name|write_archive
function_decl|)
parameter_list|(
name|struct
name|archiver_args
modifier|*
parameter_list|)
function_decl|;
DECL|member|flags
name|unsigned
name|flags
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|void
name|register_archiver
parameter_list|(
name|struct
name|archiver
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|init_tar_archiver
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|init_zip_archiver
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_typedef
DECL|typedef|write_archive_entry_fn_t
typedef|typedef
name|int
function_decl|(
modifier|*
name|write_archive_entry_fn_t
function_decl|)
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
parameter_list|,
name|void
modifier|*
name|buffer
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
specifier|extern
name|int
name|write_archive_entries
parameter_list|(
name|struct
name|archiver_args
modifier|*
name|args
parameter_list|,
name|write_archive_entry_fn_t
name|write_entry
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|write_archive
parameter_list|(
name|int
name|argc
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|int
name|setup_prefix
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* ARCHIVE_H */
end_comment

end_unit

