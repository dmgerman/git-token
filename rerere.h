begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|RERERE_H
end_ifndef

begin_define
DECL|macro|RERERE_H
define|#
directive|define
name|RERERE_H
end_define

begin_include
include|#
directive|include
file|"string-list.h"
end_include

begin_define
DECL|macro|RERERE_AUTOUPDATE
define|#
directive|define
name|RERERE_AUTOUPDATE
value|01
end_define

begin_define
DECL|macro|RERERE_NOAUTOUPDATE
define|#
directive|define
name|RERERE_NOAUTOUPDATE
value|02
end_define

begin_comment
comment|/*  * Marks paths that have been hand-resolved and added to the  * index. Set in the util field of such paths after calling  * rerere_remaining.  */
end_comment

begin_decl_stmt
specifier|extern
name|void
modifier|*
name|RERERE_RESOLVED
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|extern
name|int
name|setup_rerere
parameter_list|(
name|struct
name|string_list
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|rerere
parameter_list|(
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|rerere_path
parameter_list|(
specifier|const
name|char
modifier|*
name|hex
parameter_list|,
specifier|const
name|char
modifier|*
name|file
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|has_rerere_resolution
parameter_list|(
specifier|const
name|char
modifier|*
name|hex
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|rerere_forget
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|rerere_remaining
parameter_list|(
name|struct
name|string_list
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|OPT_RERERE_AUTOUPDATE
define|#
directive|define
name|OPT_RERERE_AUTOUPDATE
parameter_list|(
name|v
parameter_list|)
value|OPT_UYN(0, "rerere-autoupdate", (v), \ 	"update the index with reused conflict resolution if possible")
end_define

begin_endif
endif|#
directive|endif
end_endif

end_unit

