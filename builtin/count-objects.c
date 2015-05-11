begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Builtin "git count-objects".  *  * Copyright (c) 2006 Junio C Hamano  */
end_comment

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"dir.h"
end_include

begin_include
include|#
directive|include
file|"builtin.h"
end_include

begin_include
include|#
directive|include
file|"parse-options.h"
end_include

begin_decl_stmt
DECL|variable|garbage
specifier|static
name|unsigned
name|long
name|garbage
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|size_garbage
specifier|static
name|off_t
name|size_garbage
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
DECL|variable|loose
DECL|variable|packed
DECL|variable|packed_loose
specifier|static
name|unsigned
name|long
name|loose
decl_stmt|,
name|packed
decl_stmt|,
name|packed_loose
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|loose_size
specifier|static
name|off_t
name|loose_size
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|real_report_garbage
specifier|static
name|void
name|real_report_garbage
parameter_list|(
specifier|const
name|char
modifier|*
name|desc
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
if|if
condition|(
operator|!
name|stat
argument_list|(
name|path
argument_list|,
operator|&
name|st
argument_list|)
condition|)
name|size_garbage
operator|+=
name|st
operator|.
name|st_size
expr_stmt|;
name|warning
argument_list|(
literal|"%s: %s"
argument_list|,
name|desc
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|garbage
operator|++
expr_stmt|;
block|}
end_function

begin_function
DECL|function|loose_garbage
specifier|static
name|void
name|loose_garbage
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
block|{
if|if
condition|(
name|verbose
condition|)
name|report_garbage
argument_list|(
literal|"garbage found"
argument_list|,
name|path
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|count_loose
specifier|static
name|int
name|count_loose
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
name|path
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|struct
name|stat
name|st
decl_stmt|;
if|if
condition|(
name|lstat
argument_list|(
name|path
argument_list|,
operator|&
name|st
argument_list|)
operator|||
operator|!
name|S_ISREG
argument_list|(
name|st
operator|.
name|st_mode
argument_list|)
condition|)
name|loose_garbage
argument_list|(
name|path
argument_list|)
expr_stmt|;
else|else
block|{
name|loose_size
operator|+=
name|on_disk_bytes
argument_list|(
name|st
argument_list|)
expr_stmt|;
name|loose
operator|++
expr_stmt|;
if|if
condition|(
name|verbose
operator|&&
name|has_sha1_pack
argument_list|(
name|sha1
argument_list|)
condition|)
name|packed_loose
operator|++
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|count_cruft
specifier|static
name|int
name|count_cruft
parameter_list|(
specifier|const
name|char
modifier|*
name|basename
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|loose_garbage
argument_list|(
name|path
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
end_function

begin_decl_stmt
DECL|variable|count_objects_usage
specifier|static
name|char
specifier|const
modifier|*
specifier|const
name|count_objects_usage
index|[]
init|=
block|{
name|N_
argument_list|(
literal|"git count-objects [-v] [-H | --human-readable]"
argument_list|)
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|cmd_count_objects
name|int
name|cmd_count_objects
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
parameter_list|)
block|{
name|int
name|human_readable
init|=
literal|0
decl_stmt|;
name|struct
name|option
name|opts
index|[]
init|=
block|{
name|OPT__VERBOSE
argument_list|(
operator|&
name|verbose
argument_list|,
name|N_
argument_list|(
literal|"be verbose"
argument_list|)
argument_list|)
block|,
name|OPT_BOOL
argument_list|(
literal|'H'
argument_list|,
literal|"human-readable"
argument_list|,
operator|&
name|human_readable
argument_list|,
name|N_
argument_list|(
literal|"print sizes in human readable format"
argument_list|)
argument_list|)
block|,
name|OPT_END
argument_list|()
block|, 	}
decl_stmt|;
name|argc
operator|=
name|parse_options
argument_list|(
name|argc
argument_list|,
name|argv
argument_list|,
name|prefix
argument_list|,
name|opts
argument_list|,
name|count_objects_usage
argument_list|,
literal|0
argument_list|)
expr_stmt|;
comment|/* we do not take arguments other than flags for now */
if|if
condition|(
name|argc
condition|)
name|usage_with_options
argument_list|(
name|count_objects_usage
argument_list|,
name|opts
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
block|{
name|report_garbage
operator|=
name|real_report_garbage
expr_stmt|;
name|report_linked_checkout_garbage
argument_list|()
expr_stmt|;
block|}
name|for_each_loose_file_in_objdir
argument_list|(
name|get_object_directory
argument_list|()
argument_list|,
name|count_loose
argument_list|,
name|count_cruft
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|verbose
condition|)
block|{
name|struct
name|packed_git
modifier|*
name|p
decl_stmt|;
name|unsigned
name|long
name|num_pack
init|=
literal|0
decl_stmt|;
name|off_t
name|size_pack
init|=
literal|0
decl_stmt|;
name|struct
name|strbuf
name|loose_buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|pack_buf
init|=
name|STRBUF_INIT
decl_stmt|;
name|struct
name|strbuf
name|garbage_buf
init|=
name|STRBUF_INIT
decl_stmt|;
if|if
condition|(
operator|!
name|packed_git
condition|)
name|prepare_packed_git
argument_list|()
expr_stmt|;
for|for
control|(
name|p
operator|=
name|packed_git
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|next
control|)
block|{
if|if
condition|(
operator|!
name|p
operator|->
name|pack_local
condition|)
continue|continue;
if|if
condition|(
name|open_pack_index
argument_list|(
name|p
argument_list|)
condition|)
continue|continue;
name|packed
operator|+=
name|p
operator|->
name|num_objects
expr_stmt|;
name|size_pack
operator|+=
name|p
operator|->
name|pack_size
operator|+
name|p
operator|->
name|index_size
expr_stmt|;
name|num_pack
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|human_readable
condition|)
block|{
name|strbuf_humanise_bytes
argument_list|(
operator|&
name|loose_buf
argument_list|,
name|loose_size
argument_list|)
expr_stmt|;
name|strbuf_humanise_bytes
argument_list|(
operator|&
name|pack_buf
argument_list|,
name|size_pack
argument_list|)
expr_stmt|;
name|strbuf_humanise_bytes
argument_list|(
operator|&
name|garbage_buf
argument_list|,
name|size_garbage
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|strbuf_addf
argument_list|(
operator|&
name|loose_buf
argument_list|,
literal|"%lu"
argument_list|,
call|(
name|unsigned
name|long
call|)
argument_list|(
name|loose_size
operator|/
literal|1024
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|pack_buf
argument_list|,
literal|"%lu"
argument_list|,
call|(
name|unsigned
name|long
call|)
argument_list|(
name|size_pack
operator|/
literal|1024
argument_list|)
argument_list|)
expr_stmt|;
name|strbuf_addf
argument_list|(
operator|&
name|garbage_buf
argument_list|,
literal|"%lu"
argument_list|,
call|(
name|unsigned
name|long
call|)
argument_list|(
name|size_garbage
operator|/
literal|1024
argument_list|)
argument_list|)
expr_stmt|;
block|}
name|printf
argument_list|(
literal|"count: %lu\n"
argument_list|,
name|loose
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"size: %s\n"
argument_list|,
name|loose_buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"in-pack: %lu\n"
argument_list|,
name|packed
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"packs: %lu\n"
argument_list|,
name|num_pack
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"size-pack: %s\n"
argument_list|,
name|pack_buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"prune-packable: %lu\n"
argument_list|,
name|packed_loose
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"garbage: %lu\n"
argument_list|,
name|garbage
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"size-garbage: %s\n"
argument_list|,
name|garbage_buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|loose_buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|pack_buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|garbage_buf
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|struct
name|strbuf
name|buf
init|=
name|STRBUF_INIT
decl_stmt|;
if|if
condition|(
name|human_readable
condition|)
name|strbuf_humanise_bytes
argument_list|(
operator|&
name|buf
argument_list|,
name|loose_size
argument_list|)
expr_stmt|;
else|else
name|strbuf_addf
argument_list|(
operator|&
name|buf
argument_list|,
literal|"%lu kilobytes"
argument_list|,
call|(
name|unsigned
name|long
call|)
argument_list|(
name|loose_size
operator|/
literal|1024
argument_list|)
argument_list|)
expr_stmt|;
name|printf
argument_list|(
literal|"%lu objects, %s\n"
argument_list|,
name|loose
argument_list|,
name|buf
operator|.
name|buf
argument_list|)
expr_stmt|;
name|strbuf_release
argument_list|(
operator|&
name|buf
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

end_unit

