begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PARSE_OPTIONS_H
end_ifndef

begin_define
DECL|macro|PARSE_OPTIONS_H
define|#
directive|define
name|PARSE_OPTIONS_H
end_define

begin_enum
DECL|enum|parse_opt_type
enum|enum
name|parse_opt_type
block|{
DECL|enumerator|OPTION_END
name|OPTION_END
block|,
DECL|enumerator|OPTION_BOOLEAN
name|OPTION_BOOLEAN
block|,
DECL|enumerator|OPTION_STRING
name|OPTION_STRING
block|,
DECL|enumerator|OPTION_INTEGER
name|OPTION_INTEGER
block|, }
enum|;
end_enum

begin_enum
DECL|enum|parse_opt_flags
enum|enum
name|parse_opt_flags
block|{
DECL|enumerator|PARSE_OPT_KEEP_DASHDASH
name|PARSE_OPT_KEEP_DASHDASH
init|=
literal|1
block|, }
enum|;
end_enum

begin_struct
DECL|struct|option
struct|struct
name|option
block|{
DECL|member|type
name|enum
name|parse_opt_type
name|type
decl_stmt|;
DECL|member|short_name
name|int
name|short_name
decl_stmt|;
DECL|member|long_name
specifier|const
name|char
modifier|*
name|long_name
decl_stmt|;
DECL|member|value
name|void
modifier|*
name|value
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|OPT_END
define|#
directive|define
name|OPT_END
parameter_list|()
value|{ OPTION_END }
end_define

begin_define
DECL|macro|OPT_BOOLEAN
define|#
directive|define
name|OPT_BOOLEAN
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
value|{ OPTION_BOOLEAN, (s), (l), (v) }
end_define

begin_define
DECL|macro|OPT_INTEGER
define|#
directive|define
name|OPT_INTEGER
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
value|{ OPTION_INTEGER, (s), (l), (v) }
end_define

begin_define
DECL|macro|OPT_STRING
define|#
directive|define
name|OPT_STRING
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|a
parameter_list|,
name|h
parameter_list|)
value|{ OPTION_STRING,  (s), (l), (v) }
end_define

begin_comment
comment|/* parse_options() will filter out the processed options and leave the  * non-option argments in argv[].  * Returns the number of arguments left in argv[].  */
end_comment

begin_function_decl
specifier|extern
name|int
name|parse_options
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
name|struct
name|option
modifier|*
name|options
parameter_list|,
specifier|const
name|char
modifier|*
name|usagestr
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

