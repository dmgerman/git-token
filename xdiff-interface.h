begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|XDIFF_INTERFACE_H
end_ifndef

begin_define
DECL|macro|XDIFF_INTERFACE_H
define|#
directive|define
name|XDIFF_INTERFACE_H
end_define

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"xdiff/xdiff.h"
end_include

begin_comment
comment|/*  * xdiff isn't equipped to handle content over a gigabyte;  * we make the cutoff 1GB - 1MB to give some breathing  * room for constant-sized additions (e.g., merge markers)  */
end_comment

begin_define
DECL|macro|MAX_XDIFF_SIZE
define|#
directive|define
name|MAX_XDIFF_SIZE
value|(1024UL * 1024 * 1023)
end_define

begin_typedef
DECL|typedef|xdiff_emit_consume_fn
typedef|typedef
name|void
function_decl|(
modifier|*
name|xdiff_emit_consume_fn
function_decl|)
parameter_list|(
name|void
modifier|*
parameter_list|,
name|char
modifier|*
parameter_list|,
name|unsigned
name|long
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|int
name|xdi_diff
parameter_list|(
name|mmfile_t
modifier|*
name|mf1
parameter_list|,
name|mmfile_t
modifier|*
name|mf2
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|xdemitconf_t
specifier|const
modifier|*
name|xecfg
parameter_list|,
name|xdemitcb_t
modifier|*
name|ecb
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|xdi_diff_outf
parameter_list|(
name|mmfile_t
modifier|*
name|mf1
parameter_list|,
name|mmfile_t
modifier|*
name|mf2
parameter_list|,
name|xdiff_emit_consume_fn
name|fn
parameter_list|,
name|void
modifier|*
name|consume_callback_data
parameter_list|,
name|xpparam_t
specifier|const
modifier|*
name|xpp
parameter_list|,
name|xdemitconf_t
specifier|const
modifier|*
name|xecfg
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_hunk_header
parameter_list|(
name|char
modifier|*
name|line
parameter_list|,
name|int
name|len
parameter_list|,
name|int
modifier|*
name|ob
parameter_list|,
name|int
modifier|*
name|on
parameter_list|,
name|int
modifier|*
name|nb
parameter_list|,
name|int
modifier|*
name|nn
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|read_mmfile
parameter_list|(
name|mmfile_t
modifier|*
name|ptr
parameter_list|,
specifier|const
name|char
modifier|*
name|filename
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|read_mmblob
parameter_list|(
name|mmfile_t
modifier|*
name|ptr
parameter_list|,
specifier|const
name|struct
name|object_id
modifier|*
name|oid
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|buffer_is_binary
parameter_list|(
specifier|const
name|char
modifier|*
name|ptr
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|xdiff_set_find_func
parameter_list|(
name|xdemitconf_t
modifier|*
name|xecfg
parameter_list|,
specifier|const
name|char
modifier|*
name|line
parameter_list|,
name|int
name|cflags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|xdiff_clear_find_func
parameter_list|(
name|xdemitconf_t
modifier|*
name|xecfg
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|git_xmerge_config
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
parameter_list|,
name|void
modifier|*
name|cb
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
name|int
name|git_xmerge_style
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

end_unit

