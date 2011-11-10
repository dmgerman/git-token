begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<sys/types.h>
end_include

begin_include
include|#
directive|include
file|<sys/stat.h>
end_include

begin_typedef
DECL|typedef|stat_fn_t
typedef|typedef
name|int
function_decl|(
modifier|*
name|stat_fn_t
function_decl|)
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|struct
name|stat
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_decl_stmt
specifier|extern
name|stat_fn_t
name|cygwin_stat_fn
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|stat_fn_t
name|cygwin_lstat_fn
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|stat
define|#
directive|define
name|stat
parameter_list|(
name|path
parameter_list|,
name|buf
parameter_list|)
value|(*cygwin_stat_fn)(path, buf)
end_define

begin_define
DECL|macro|lstat
define|#
directive|define
name|lstat
parameter_list|(
name|path
parameter_list|,
name|buf
parameter_list|)
value|(*cygwin_lstat_fn)(path, buf)
end_define

end_unit

