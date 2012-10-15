begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_comment
comment|/* wildmatch.h */
end_comment

begin_define
DECL|macro|ABORT_MALFORMED
define|#
directive|define
name|ABORT_MALFORMED
value|2
end_define

begin_define
DECL|macro|NOMATCH
define|#
directive|define
name|NOMATCH
value|1
end_define

begin_define
DECL|macro|MATCH
define|#
directive|define
name|MATCH
value|0
end_define

begin_define
DECL|macro|ABORT_ALL
define|#
directive|define
name|ABORT_ALL
value|-1
end_define

begin_define
DECL|macro|ABORT_TO_STARSTAR
define|#
directive|define
name|ABORT_TO_STARSTAR
value|-2
end_define

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
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

end_unit

