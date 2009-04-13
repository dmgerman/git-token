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

begin_function_decl
specifier|extern
name|int
name|setup_rerere
parameter_list|(
name|struct
name|string_list
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|rerere
parameter_list|(
name|void
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

begin_endif
endif|#
directive|endif
end_endif

end_unit

