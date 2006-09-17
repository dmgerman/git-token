begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|GREP_H
end_ifndef

begin_define
DECL|macro|GREP_H
define|#
directive|define
name|GREP_H
end_define

begin_enum
DECL|enum|grep_pat_token
enum|enum
name|grep_pat_token
block|{
DECL|enumerator|GREP_PATTERN
name|GREP_PATTERN
block|,
DECL|enumerator|GREP_AND
name|GREP_AND
block|,
DECL|enumerator|GREP_OPEN_PAREN
name|GREP_OPEN_PAREN
block|,
DECL|enumerator|GREP_CLOSE_PAREN
name|GREP_CLOSE_PAREN
block|,
DECL|enumerator|GREP_NOT
name|GREP_NOT
block|,
DECL|enumerator|GREP_OR
name|GREP_OR
block|, }
enum|;
end_enum

begin_struct
DECL|struct|grep_pat
struct|struct
name|grep_pat
block|{
DECL|member|next
name|struct
name|grep_pat
modifier|*
name|next
decl_stmt|;
DECL|member|origin
specifier|const
name|char
modifier|*
name|origin
decl_stmt|;
DECL|member|no
name|int
name|no
decl_stmt|;
DECL|member|token
name|enum
name|grep_pat_token
name|token
decl_stmt|;
DECL|member|pattern
specifier|const
name|char
modifier|*
name|pattern
decl_stmt|;
DECL|member|regexp
name|regex_t
name|regexp
decl_stmt|;
block|}
struct|;
end_struct

begin_enum
DECL|enum|grep_expr_node
enum|enum
name|grep_expr_node
block|{
DECL|enumerator|GREP_NODE_ATOM
name|GREP_NODE_ATOM
block|,
DECL|enumerator|GREP_NODE_NOT
name|GREP_NODE_NOT
block|,
DECL|enumerator|GREP_NODE_AND
name|GREP_NODE_AND
block|,
DECL|enumerator|GREP_NODE_OR
name|GREP_NODE_OR
block|, }
enum|;
end_enum

begin_struct
DECL|struct|grep_expr
struct|struct
name|grep_expr
block|{
DECL|member|node
name|enum
name|grep_expr_node
name|node
decl_stmt|;
union|union
block|{
DECL|member|atom
name|struct
name|grep_pat
modifier|*
name|atom
decl_stmt|;
DECL|member|unary
name|struct
name|grep_expr
modifier|*
name|unary
decl_stmt|;
struct|struct
block|{
DECL|member|left
name|struct
name|grep_expr
modifier|*
name|left
decl_stmt|;
DECL|member|right
name|struct
name|grep_expr
modifier|*
name|right
decl_stmt|;
DECL|member|binary
block|}
name|binary
struct|;
DECL|member|u
block|}
name|u
union|;
block|}
struct|;
end_struct

begin_struct
DECL|struct|grep_opt
struct|struct
name|grep_opt
block|{
DECL|member|pattern_list
name|struct
name|grep_pat
modifier|*
name|pattern_list
decl_stmt|;
DECL|member|pattern_tail
name|struct
name|grep_pat
modifier|*
modifier|*
name|pattern_tail
decl_stmt|;
DECL|member|pattern_expression
name|struct
name|grep_expr
modifier|*
name|pattern_expression
decl_stmt|;
DECL|member|prefix_length
name|int
name|prefix_length
decl_stmt|;
DECL|member|regexp
name|regex_t
name|regexp
decl_stmt|;
DECL|member|linenum
name|unsigned
name|linenum
range|:
literal|1
decl_stmt|;
DECL|member|invert
name|unsigned
name|invert
range|:
literal|1
decl_stmt|;
DECL|member|status_only
name|unsigned
name|status_only
range|:
literal|1
decl_stmt|;
DECL|member|name_only
name|unsigned
name|name_only
range|:
literal|1
decl_stmt|;
DECL|member|unmatch_name_only
name|unsigned
name|unmatch_name_only
range|:
literal|1
decl_stmt|;
DECL|member|count
name|unsigned
name|count
range|:
literal|1
decl_stmt|;
DECL|member|word_regexp
name|unsigned
name|word_regexp
range|:
literal|1
decl_stmt|;
DECL|member|fixed
name|unsigned
name|fixed
range|:
literal|1
decl_stmt|;
DECL|macro|GREP_BINARY_DEFAULT
define|#
directive|define
name|GREP_BINARY_DEFAULT
value|0
DECL|macro|GREP_BINARY_NOMATCH
define|#
directive|define
name|GREP_BINARY_NOMATCH
value|1
DECL|macro|GREP_BINARY_TEXT
define|#
directive|define
name|GREP_BINARY_TEXT
value|2
DECL|member|binary
name|unsigned
name|binary
range|:
literal|2
decl_stmt|;
DECL|member|extended
name|unsigned
name|extended
range|:
literal|1
decl_stmt|;
DECL|member|relative
name|unsigned
name|relative
range|:
literal|1
decl_stmt|;
DECL|member|pathname
name|unsigned
name|pathname
range|:
literal|1
decl_stmt|;
DECL|member|regflags
name|int
name|regflags
decl_stmt|;
DECL|member|pre_context
name|unsigned
name|pre_context
decl_stmt|;
DECL|member|post_context
name|unsigned
name|post_context
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|extern
name|void
name|append_grep_pattern
parameter_list|(
name|struct
name|grep_opt
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|pat
parameter_list|,
specifier|const
name|char
modifier|*
name|origin
parameter_list|,
name|int
name|no
parameter_list|,
name|enum
name|grep_pat_token
name|t
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|compile_grep_patterns
parameter_list|(
name|struct
name|grep_opt
modifier|*
name|opt
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|grep_buffer
parameter_list|(
name|struct
name|grep_opt
modifier|*
name|opt
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|char
modifier|*
name|buf
parameter_list|,
name|unsigned
name|long
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

