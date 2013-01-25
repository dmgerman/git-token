begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|WILDMATCH_H
end_ifndef

begin_define
DECL|macro|WILDMATCH_H
define|#
directive|define
name|WILDMATCH_H
end_define

begin_define
DECL|macro|WM_CASEFOLD
define|#
directive|define
name|WM_CASEFOLD
value|1
end_define

begin_define
DECL|macro|WM_PATHNAME
define|#
directive|define
name|WM_PATHNAME
value|2
end_define

begin_define
DECL|macro|WM_ABORT_MALFORMED
define|#
directive|define
name|WM_ABORT_MALFORMED
value|2
end_define

begin_define
DECL|macro|WM_NOMATCH
define|#
directive|define
name|WM_NOMATCH
value|1
end_define

begin_define
DECL|macro|WM_MATCH
define|#
directive|define
name|WM_MATCH
value|0
end_define

begin_define
DECL|macro|WM_ABORT_ALL
define|#
directive|define
name|WM_ABORT_ALL
value|-1
end_define

begin_define
DECL|macro|WM_ABORT_TO_STARSTAR
define|#
directive|define
name|WM_ABORT_TO_STARSTAR
value|-2
end_define

begin_struct_decl
struct_decl|struct
name|wildopts
struct_decl|;
end_struct_decl

begin_function_decl
name|int
name|wildmatch
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|text
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|,
name|struct
name|wildopts
modifier|*
name|wo
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

