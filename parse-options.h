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
DECL|enumerator|OPTION_GROUP
name|OPTION_GROUP
block|,
DECL|enumerator|OPTION_BOOLEAN
name|OPTION_BOOLEAN
block|,
DECL|enumerator|OPTION_STRING
name|OPTION_STRING
block|,
DECL|enumerator|OPTION_INTEGER
name|OPTION_INTEGER
block|,
DECL|enumerator|OPTION_CALLBACK
name|OPTION_CALLBACK
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

begin_enum
DECL|enum|parse_opt_option_flags
enum|enum
name|parse_opt_option_flags
block|{
DECL|enumerator|PARSE_OPT_OPTARG
name|PARSE_OPT_OPTARG
init|=
literal|1
block|,
DECL|enumerator|PARSE_OPT_NOARG
name|PARSE_OPT_NOARG
init|=
literal|2
block|, }
enum|;
end_enum

begin_struct_decl
struct_decl|struct
name|option
struct_decl|;
end_struct_decl

begin_typedef
DECL|typedef|parse_opt_cb
typedef|typedef
name|int
name|parse_opt_cb
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|int
name|unset
parameter_list|)
function_decl|;
end_typedef

begin_comment
comment|/*  * `type`::  *   holds the type of the option, you must have an OPTION_END last in your  *   array.  *  * `short_name`::  *   the character to use as a short option name, '\0' if none.  *  * `long_name`::  *   the long option name, without the leading dashes, NULL if none.  *  * `value`::  *   stores pointers to the values to be filled.  *  * `argh`::  *   token to explain the kind of argument this option wants. Keep it  *   homogenous across the repository.  *  * `help`::  *   the short help associated to what the option does.  *   Must never be NULL (except for OPTION_END).  *   OPTION_GROUP uses this pointer to store the group header.  *  * `flags`::  *   mask of parse_opt_option_flags.  *   PARSE_OPT_OPTARG: says that the argument is optionnal (not for BOOLEANs)  *   PARSE_OPT_NOARG: says that this option takes no argument, for CALLBACKs  *  * `callback`::  *   pointer to the callback to use for OPTION_CALLBACK.  *  * `defval`::  *   default value to fill (*->value) with for PARSE_OPT_OPTARG.  *   CALLBACKS can use it like they want.  */
end_comment

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
DECL|member|argh
specifier|const
name|char
modifier|*
name|argh
decl_stmt|;
DECL|member|help
specifier|const
name|char
modifier|*
name|help
decl_stmt|;
DECL|member|flags
name|int
name|flags
decl_stmt|;
DECL|member|callback
name|parse_opt_cb
modifier|*
name|callback
decl_stmt|;
DECL|member|defval
name|intptr_t
name|defval
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
DECL|macro|OPT_GROUP
define|#
directive|define
name|OPT_GROUP
parameter_list|(
name|h
parameter_list|)
value|{ OPTION_GROUP, 0, NULL, NULL, NULL, (h) }
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
value|{ OPTION_BOOLEAN, (s), (l), (v), NULL, (h) }
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
value|{ OPTION_INTEGER, (s), (l), (v), NULL, (h) }
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
value|{ OPTION_STRING,  (s), (l), (v), (a), (h) }
end_define

begin_define
DECL|macro|OPT_CALLBACK
define|#
directive|define
name|OPT_CALLBACK
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
parameter_list|,
name|f
parameter_list|)
define|\
value|{ OPTION_CALLBACK, (s), (l), (v), (a), (h), 0, (f) }
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
specifier|const
name|usagestr
index|[]
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|NORETURN
name|void
name|usage_with_options
parameter_list|(
specifier|const
name|char
modifier|*
specifier|const
modifier|*
name|usagestr
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|options
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*----- some often used options -----*/
end_comment

begin_function_decl
specifier|extern
name|int
name|parse_opt_abbrev_cb
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|int
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|OPT__VERBOSE
define|#
directive|define
name|OPT__VERBOSE
parameter_list|(
name|var
parameter_list|)
value|OPT_BOOLEAN('v', "verbose", (var), "be verbose")
end_define

begin_define
DECL|macro|OPT__QUIET
define|#
directive|define
name|OPT__QUIET
parameter_list|(
name|var
parameter_list|)
value|OPT_BOOLEAN('q', "quiet",   (var), "be quiet")
end_define

begin_define
DECL|macro|OPT__DRY_RUN
define|#
directive|define
name|OPT__DRY_RUN
parameter_list|(
name|var
parameter_list|)
value|OPT_BOOLEAN('n', "dry-run", (var), "dry run")
end_define

begin_define
DECL|macro|OPT__ABBREV
define|#
directive|define
name|OPT__ABBREV
parameter_list|(
name|var
parameter_list|)
define|\
value|{ OPTION_CALLBACK, 0, "abbrev", (var), "n", \ 	  "use<n> digits to display SHA-1s", \ 	  PARSE_OPT_OPTARG,&parse_opt_abbrev_cb, 0 }
end_define

begin_endif
endif|#
directive|endif
end_endif

end_unit

