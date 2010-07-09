begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|GIT_UTF8_H
end_ifndef

begin_define
DECL|macro|GIT_UTF8_H
define|#
directive|define
name|GIT_UTF8_H
end_define

begin_typedef
DECL|typedef|ucs_char_t
typedef|typedef
name|unsigned
name|int
name|ucs_char_t
typedef|;
end_typedef

begin_comment
DECL|typedef|ucs_char_t
comment|/* assuming 32bit int */
end_comment

begin_function_decl
name|int
name|utf8_width
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|start
parameter_list|,
name|size_t
modifier|*
name|remainder_p
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|utf8_strwidth
parameter_list|(
specifier|const
name|char
modifier|*
name|string
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|is_utf8
parameter_list|(
specifier|const
name|char
modifier|*
name|text
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|is_encoding_utf8
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|strbuf_add_wrapped_text
parameter_list|(
name|struct
name|strbuf
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|text
parameter_list|,
name|int
name|indent
parameter_list|,
name|int
name|indent2
parameter_list|,
name|int
name|width
parameter_list|)
function_decl|;
end_function_decl

begin_ifndef
ifndef|#
directive|ifndef
name|NO_ICONV
end_ifndef

begin_function_decl
name|char
modifier|*
name|reencode_string
parameter_list|(
specifier|const
name|char
modifier|*
name|in
parameter_list|,
specifier|const
name|char
modifier|*
name|out_encoding
parameter_list|,
specifier|const
name|char
modifier|*
name|in_encoding
parameter_list|)
function_decl|;
end_function_decl

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|reencode_string
define|#
directive|define
name|reencode_string
parameter_list|(
name|a
parameter_list|,
name|b
parameter_list|,
name|c
parameter_list|)
value|NULL
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

end_unit

