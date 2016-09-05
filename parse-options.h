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
comment|/* special types */
DECL|enumerator|OPTION_END
name|OPTION_END
block|,
DECL|enumerator|OPTION_ARGUMENT
name|OPTION_ARGUMENT
block|,
DECL|enumerator|OPTION_GROUP
name|OPTION_GROUP
block|,
DECL|enumerator|OPTION_NUMBER
name|OPTION_NUMBER
block|,
comment|/* options with no arguments */
DECL|enumerator|OPTION_BIT
name|OPTION_BIT
block|,
DECL|enumerator|OPTION_NEGBIT
name|OPTION_NEGBIT
block|,
DECL|enumerator|OPTION_COUNTUP
name|OPTION_COUNTUP
block|,
DECL|enumerator|OPTION_SET_INT
name|OPTION_SET_INT
block|,
DECL|enumerator|OPTION_CMDMODE
name|OPTION_CMDMODE
block|,
comment|/* options with arguments (usually) */
DECL|enumerator|OPTION_STRING
name|OPTION_STRING
block|,
DECL|enumerator|OPTION_INTEGER
name|OPTION_INTEGER
block|,
DECL|enumerator|OPTION_MAGNITUDE
name|OPTION_MAGNITUDE
block|,
DECL|enumerator|OPTION_CALLBACK
name|OPTION_CALLBACK
block|,
DECL|enumerator|OPTION_LOWLEVEL_CALLBACK
name|OPTION_LOWLEVEL_CALLBACK
block|,
DECL|enumerator|OPTION_FILENAME
name|OPTION_FILENAME
block|}
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
block|,
DECL|enumerator|PARSE_OPT_STOP_AT_NON_OPTION
name|PARSE_OPT_STOP_AT_NON_OPTION
init|=
literal|2
block|,
DECL|enumerator|PARSE_OPT_KEEP_ARGV0
name|PARSE_OPT_KEEP_ARGV0
init|=
literal|4
block|,
DECL|enumerator|PARSE_OPT_KEEP_UNKNOWN
name|PARSE_OPT_KEEP_UNKNOWN
init|=
literal|8
block|,
DECL|enumerator|PARSE_OPT_NO_INTERNAL_HELP
name|PARSE_OPT_NO_INTERNAL_HELP
init|=
literal|16
block|}
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
block|,
DECL|enumerator|PARSE_OPT_NONEG
name|PARSE_OPT_NONEG
init|=
literal|4
block|,
DECL|enumerator|PARSE_OPT_HIDDEN
name|PARSE_OPT_HIDDEN
init|=
literal|8
block|,
DECL|enumerator|PARSE_OPT_LASTARG_DEFAULT
name|PARSE_OPT_LASTARG_DEFAULT
init|=
literal|16
block|,
DECL|enumerator|PARSE_OPT_NODASH
name|PARSE_OPT_NODASH
init|=
literal|32
block|,
DECL|enumerator|PARSE_OPT_LITERAL_ARGHELP
name|PARSE_OPT_LITERAL_ARGHELP
init|=
literal|64
block|,
DECL|enumerator|PARSE_OPT_SHELL_EVAL
name|PARSE_OPT_SHELL_EVAL
init|=
literal|256
block|}
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

begin_struct_decl
struct_decl|struct
name|parse_opt_ctx_t
struct_decl|;
end_struct_decl

begin_typedef
DECL|typedef|parse_opt_ll_cb
typedef|typedef
name|int
name|parse_opt_ll_cb
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|ctx
parameter_list|,
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
name|int
name|unset
parameter_list|)
function_decl|;
end_typedef

begin_comment
comment|/*  * `type`::  *   holds the type of the option, you must have an OPTION_END last in your  *   array.  *  * `short_name`::  *   the character to use as a short option name, '\0' if none.  *  * `long_name`::  *   the long option name, without the leading dashes, NULL if none.  *  * `value`::  *   stores pointers to the values to be filled.  *  * `argh`::  *   token to explain the kind of argument this option wants. Keep it  *   homogeneous across the repository. Should be wrapped by N_() for  *   translation.  *  * `help`::  *   the short help associated to what the option does.  *   Must never be NULL (except for OPTION_END).  *   OPTION_GROUP uses this pointer to store the group header.  *   Should be wrapped by N_() for translation.  *  * `flags`::  *   mask of parse_opt_option_flags.  *   PARSE_OPT_OPTARG: says that the argument is optional (not for BOOLEANs)  *   PARSE_OPT_NOARG: says that this option does not take an argument  *   PARSE_OPT_NONEG: says that this option cannot be negated  *   PARSE_OPT_HIDDEN: this option is skipped in the default usage, and  *                     shown only in the full usage.  *   PARSE_OPT_LASTARG_DEFAULT: says that this option will take the default  *				value if no argument is given when the option  *				is last on the command line. If the option is  *				not last it will require an argument.  *				Should not be used with PARSE_OPT_OPTARG.  *   PARSE_OPT_NODASH: this option doesn't start with a dash.  *   PARSE_OPT_LITERAL_ARGHELP: says that argh shouldn't be enclosed in brackets  *				(i.e. '<argh>') in the help message.  *				Useful for options with multiple parameters.  *  * `callback`::  *   pointer to the callback to use for OPTION_CALLBACK or  *   OPTION_LOWLEVEL_CALLBACK.  *  * `defval`::  *   default value to fill (*->value) with for PARSE_OPT_OPTARG.  *   OPTION_{BIT,SET_INT} store the {mask,integer} to put in the value when met.  *   CALLBACKS can use it like they want.  */
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
DECL|macro|OPT_ARGUMENT
define|#
directive|define
name|OPT_ARGUMENT
parameter_list|(
name|l
parameter_list|,
name|h
parameter_list|)
value|{ OPTION_ARGUMENT, 0, (l), NULL, NULL, \ 				      (h), PARSE_OPT_NOARG}
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
DECL|macro|OPT_BIT
define|#
directive|define
name|OPT_BIT
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|,
name|b
parameter_list|)
value|{ OPTION_BIT, (s), (l), (v), NULL, (h), \ 				      PARSE_OPT_NOARG, NULL, (b) }
end_define

begin_define
DECL|macro|OPT_NEGBIT
define|#
directive|define
name|OPT_NEGBIT
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|,
name|b
parameter_list|)
value|{ OPTION_NEGBIT, (s), (l), (v), NULL, \ 				      (h), PARSE_OPT_NOARG, NULL, (b) }
end_define

begin_define
DECL|macro|OPT_COUNTUP
define|#
directive|define
name|OPT_COUNTUP
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
value|{ OPTION_COUNTUP, (s), (l), (v), NULL, \ 				      (h), PARSE_OPT_NOARG }
end_define

begin_define
DECL|macro|OPT_SET_INT
define|#
directive|define
name|OPT_SET_INT
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|,
name|i
parameter_list|)
value|{ OPTION_SET_INT, (s), (l), (v), NULL, \ 				      (h), PARSE_OPT_NOARG, NULL, (i) }
end_define

begin_define
DECL|macro|OPT_BOOL
define|#
directive|define
name|OPT_BOOL
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
value|OPT_SET_INT(s, l, v, h, 1)
end_define

begin_define
DECL|macro|OPT_HIDDEN_BOOL
define|#
directive|define
name|OPT_HIDDEN_BOOL
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
value|{ OPTION_SET_INT, (s), (l), (v), NULL, \ 				      (h), PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, NULL, 1}
end_define

begin_define
DECL|macro|OPT_CMDMODE
define|#
directive|define
name|OPT_CMDMODE
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|,
name|i
parameter_list|)
value|{ OPTION_CMDMODE, (s), (l), (v), NULL, \ 				      (h), PARSE_OPT_NOARG|PARSE_OPT_NONEG, NULL, (i) }
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
value|{ OPTION_INTEGER, (s), (l), (v), N_("n"), (h) }
end_define

begin_define
DECL|macro|OPT_MAGNITUDE
define|#
directive|define
name|OPT_MAGNITUDE
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
value|{ OPTION_MAGNITUDE, (s), (l), (v), \ 				      N_("n"), (h), PARSE_OPT_NONEG }
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
DECL|macro|OPT_STRING_LIST
define|#
directive|define
name|OPT_STRING_LIST
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
define|\
value|{ OPTION_CALLBACK, (s), (l), (v), (a), \ 				      (h), 0,&parse_opt_string_list }
end_define

begin_define
DECL|macro|OPT_UYN
define|#
directive|define
name|OPT_UYN
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
value|{ OPTION_CALLBACK, (s), (l), (v), NULL, \ 				      (h), PARSE_OPT_NOARG,&parse_opt_tertiary }
end_define

begin_define
DECL|macro|OPT_DATE
define|#
directive|define
name|OPT_DATE
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
define|\
value|{ OPTION_CALLBACK, (s), (l), (v), N_("time"),(h), 0,	\ 	  parse_opt_approxidate_cb }
end_define

begin_define
DECL|macro|OPT_EXPIRY_DATE
define|#
directive|define
name|OPT_EXPIRY_DATE
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
define|\
value|{ OPTION_CALLBACK, (s), (l), (v), N_("expiry-date"),(h), 0,	\ 	  parse_opt_expiry_date_cb }
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

begin_define
DECL|macro|OPT_NUMBER_CALLBACK
define|#
directive|define
name|OPT_NUMBER_CALLBACK
parameter_list|(
name|v
parameter_list|,
name|h
parameter_list|,
name|f
parameter_list|)
define|\
value|{ OPTION_NUMBER, 0, NULL, (v), NULL, (h), \ 	  PARSE_OPT_NOARG | PARSE_OPT_NONEG, (f) }
end_define

begin_define
DECL|macro|OPT_FILENAME
define|#
directive|define
name|OPT_FILENAME
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
value|{ OPTION_FILENAME, (s), (l), (v), \ 				       N_("file"), (h) }
end_define

begin_define
DECL|macro|OPT_COLOR_FLAG
define|#
directive|define
name|OPT_COLOR_FLAG
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
define|\
value|{ OPTION_CALLBACK, (s), (l), (v), N_("when"), (h), PARSE_OPT_OPTARG, \ 		parse_opt_color_flag_cb, (intptr_t)"always" }
end_define

begin_define
DECL|macro|OPT_NOOP_NOARG
define|#
directive|define
name|OPT_NOOP_NOARG
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|)
define|\
value|{ OPTION_CALLBACK, (s), (l), NULL, NULL, \ 	  N_("no-op (backward compatibility)"),		\ 	  PARSE_OPT_HIDDEN | PARSE_OPT_NOARG, parse_opt_noop_cb }
end_define

begin_comment
comment|/* parse_options() will filter out the processed options and leave the  * non-option arguments in argv[]. usagestr strings should be marked  * for translation with N_().  * Returns the number of arguments left in argv[].  */
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
name|char
modifier|*
name|prefix
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

begin_function_decl
specifier|extern
name|NORETURN
name|void
name|usage_msg_opt
parameter_list|(
specifier|const
name|char
modifier|*
name|msg
parameter_list|,
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

begin_function_decl
specifier|extern
name|int
name|optbug
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|reason
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|opterror
parameter_list|(
specifier|const
name|struct
name|option
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|reason
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_if
if|#
directive|if
name|defined
argument_list|(
name|__GNUC__
argument_list|)
end_if

begin_define
DECL|macro|opterror
define|#
directive|define
name|opterror
parameter_list|(
name|o
parameter_list|,
name|r
parameter_list|,
name|f
parameter_list|)
value|(opterror((o),(r),(f)), const_error())
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*----- incremental advanced APIs -----*/
end_comment

begin_enum
enum|enum
block|{
DECL|enumerator|PARSE_OPT_HELP
name|PARSE_OPT_HELP
init|=
operator|-
literal|1
block|,
DECL|enumerator|PARSE_OPT_DONE
name|PARSE_OPT_DONE
block|,
DECL|enumerator|PARSE_OPT_NON_OPTION
name|PARSE_OPT_NON_OPTION
block|,
DECL|enumerator|PARSE_OPT_UNKNOWN
name|PARSE_OPT_UNKNOWN
block|}
enum|;
end_enum

begin_comment
comment|/*  * It's okay for the caller to consume argv/argc in the usual way.  * Other fields of that structure are private to parse-options and should not  * be modified in any way.  */
end_comment

begin_struct
DECL|struct|parse_opt_ctx_t
struct|struct
name|parse_opt_ctx_t
block|{
DECL|member|argv
specifier|const
name|char
modifier|*
modifier|*
name|argv
decl_stmt|;
DECL|member|out
specifier|const
name|char
modifier|*
modifier|*
name|out
decl_stmt|;
DECL|member|argc
DECL|member|cpidx
DECL|member|total
name|int
name|argc
decl_stmt|,
name|cpidx
decl_stmt|,
name|total
decl_stmt|;
DECL|member|opt
specifier|const
name|char
modifier|*
name|opt
decl_stmt|;
DECL|member|flags
name|int
name|flags
decl_stmt|;
DECL|member|prefix
specifier|const
name|char
modifier|*
name|prefix
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|void
name|parse_options_start
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|ctx
parameter_list|,
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
specifier|const
name|struct
name|option
modifier|*
name|options
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|parse_options_step
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|ctx
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
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|parse_options_end
parameter_list|(
name|struct
name|parse_opt_ctx_t
modifier|*
name|ctx
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|parse_options_concat
parameter_list|(
name|struct
name|option
modifier|*
name|dst
parameter_list|,
name|size_t
parameter_list|,
name|struct
name|option
modifier|*
name|src
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_approxidate_cb
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_expiry_date_cb
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_color_flag_cb
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_verbosity_cb
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_object_name
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_commits
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_tertiary
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_string_list
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_noop_cb
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_unknown_cb
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_passthru
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

begin_function_decl
specifier|extern
name|int
name|parse_opt_passthru_argv
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
parameter_list|,
name|h
parameter_list|)
value|OPT_COUNTUP('v', "verbose", (var), (h))
end_define

begin_define
DECL|macro|OPT__QUIET
define|#
directive|define
name|OPT__QUIET
parameter_list|(
name|var
parameter_list|,
name|h
parameter_list|)
value|OPT_COUNTUP('q', "quiet",   (var), (h))
end_define

begin_define
DECL|macro|OPT__VERBOSITY
define|#
directive|define
name|OPT__VERBOSITY
parameter_list|(
name|var
parameter_list|)
define|\
value|{ OPTION_CALLBACK, 'v', "verbose", (var), NULL, N_("be more verbose"), \ 	  PARSE_OPT_NOARG,&parse_opt_verbosity_cb, 0 }, \ 	{ OPTION_CALLBACK, 'q', "quiet", (var), NULL, N_("be more quiet"), \ 	  PARSE_OPT_NOARG,&parse_opt_verbosity_cb, 0 }
end_define

begin_define
DECL|macro|OPT__DRY_RUN
define|#
directive|define
name|OPT__DRY_RUN
parameter_list|(
name|var
parameter_list|,
name|h
parameter_list|)
value|OPT_BOOL('n', "dry-run", (var), (h))
end_define

begin_define
DECL|macro|OPT__FORCE
define|#
directive|define
name|OPT__FORCE
parameter_list|(
name|var
parameter_list|,
name|h
parameter_list|)
value|OPT_COUNTUP('f', "force",   (var), (h))
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
value|{ OPTION_CALLBACK, 0, "abbrev", (var), N_("n"),	\ 	  N_("use<n> digits to display SHA-1s"),	\ 	  PARSE_OPT_OPTARG,&parse_opt_abbrev_cb, 0 }
end_define

begin_define
DECL|macro|OPT__COLOR
define|#
directive|define
name|OPT__COLOR
parameter_list|(
name|var
parameter_list|,
name|h
parameter_list|)
define|\
value|OPT_COLOR_FLAG(0, "color", (var), (h))
end_define

begin_define
DECL|macro|OPT_COLUMN
define|#
directive|define
name|OPT_COLUMN
parameter_list|(
name|s
parameter_list|,
name|l
parameter_list|,
name|v
parameter_list|,
name|h
parameter_list|)
define|\
value|{ OPTION_CALLBACK, (s), (l), (v), N_("style"), (h), PARSE_OPT_OPTARG, parseopt_column_callback }
end_define

begin_define
DECL|macro|OPT_PASSTHRU
define|#
directive|define
name|OPT_PASSTHRU
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
value|{ OPTION_CALLBACK, (s), (l), (v), (a), (h), (f), parse_opt_passthru }
end_define

begin_define
DECL|macro|OPT_PASSTHRU_ARGV
define|#
directive|define
name|OPT_PASSTHRU_ARGV
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
value|{ OPTION_CALLBACK, (s), (l), (v), (a), (h), (f), parse_opt_passthru_argv }
end_define

begin_define
DECL|macro|_OPT_CONTAINS_OR_WITH
define|#
directive|define
name|_OPT_CONTAINS_OR_WITH
parameter_list|(
name|name
parameter_list|,
name|variable
parameter_list|,
name|help
parameter_list|,
name|flag
parameter_list|)
define|\
value|{ OPTION_CALLBACK, 0, name, (variable), N_("commit"), (help), \ 	  PARSE_OPT_LASTARG_DEFAULT | flag, \ 	  parse_opt_commits, (intptr_t) "HEAD" \ 	}
end_define

begin_define
DECL|macro|OPT_CONTAINS
define|#
directive|define
name|OPT_CONTAINS
parameter_list|(
name|v
parameter_list|,
name|h
parameter_list|)
value|_OPT_CONTAINS_OR_WITH("contains", v, h, 0)
end_define

begin_define
DECL|macro|OPT_WITH
define|#
directive|define
name|OPT_WITH
parameter_list|(
name|v
parameter_list|,
name|h
parameter_list|)
value|_OPT_CONTAINS_OR_WITH("with", v, h, PARSE_OPT_HIDDEN)
end_define

begin_endif
endif|#
directive|endif
end_endif

end_unit

