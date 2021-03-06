begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|QUOTE_H
end_ifndef

begin_define
DECL|macro|QUOTE_H
define|#
directive|define
name|QUOTE_H
end_define

begin_struct_decl
struct_decl|struct
name|strbuf
struct_decl|;
end_struct_decl

begin_comment
comment|/* Help to copy the thing properly quoted for the shell safety.  * any single quote is replaced with '\'', any exclamation point  * is replaced with '\!', and the whole thing is enclosed in a  * single quote pair.  *  * For example, if you are passing the result to system() as an  * argument:  *  * sprintf(cmd, "foobar %s %s", sq_quote(arg0), sq_quote(arg1))  *  * would be appropriate.  If the system() is going to call ssh to  * run the command on the other side:  *  * sprintf(cmd, "git-diff-tree %s %s", sq_quote(arg0), sq_quote(arg1));  * sprintf(rcmd, "ssh %s %s", sq_quote(host), sq_quote(cmd));  *  * Note that the above examples leak memory!  Remember to free result from  * sq_quote() in a real application.  *  * sq_quote_buf() writes to an existing buffer of specified size; it  * will return the number of characters that would have been written  * excluding the final null regardless of the buffer size.  *  * sq_quotef() quotes the entire formatted string as a single result.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|sq_quote_buf
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|sq_quote_argv
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|size_t
name|maxlen
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|sq_quotef
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* This unwraps what sq_quote() produces in place, but returns  * NULL if the input does not look like what sq_quote would have  * produced.  */
end_comment

begin_function_decl
specifier|extern
name|char
modifier|*
name|sq_dequote
parameter_list|(
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Same as the above, but can be used to unwrap many arguments in the  * same string separated by space. Like sq_quote, it works in place,  * modifying arg and appending pointers into it to argv.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|sq_dequote_to_argv
parameter_list|(
name|char
modifier|*
name|arg
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
modifier|*
name|argv
parameter_list|,
name|int
modifier|*
name|nr
parameter_list|,
name|int
modifier|*
name|alloc
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Same as above, but store the unquoted strings in an argv_array. We will  * still modify arg in place, but unlike sq_dequote_to_argv, the argv_array  * will duplicate and take ownership of the strings.  */
end_comment

begin_struct_decl
struct_decl|struct
name|argv_array
struct_decl|;
end_struct_decl

begin_function_decl
specifier|extern
name|int
name|sq_dequote_to_argv_array
parameter_list|(
name|char
modifier|*
name|arg
parameter_list|,
name|struct
name|argv_array
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|unquote_c_style
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
name|quoted
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|endp
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|size_t
name|quote_c_style
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|struct
name|strbuf
modifier|*
parameter_list|,
name|FILE
modifier|*
parameter_list|,
name|int
name|no_dq
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|quote_two_c_style
parameter_list|(
name|struct
name|strbuf
modifier|*
parameter_list|,
specifier|const
name|char
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
name|void
name|write_name_quoted
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|FILE
modifier|*
parameter_list|,
name|int
name|terminator
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|write_name_quoted_relative
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|FILE
modifier|*
name|fp
parameter_list|,
name|int
name|terminator
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* quote path as relative to the given prefix */
end_comment

begin_function_decl
specifier|extern
name|char
modifier|*
name|quote_path_relative
parameter_list|(
specifier|const
name|char
modifier|*
name|in
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|struct
name|strbuf
modifier|*
name|out
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* quoting as a string literal for other languages */
end_comment

begin_function_decl
specifier|extern
name|void
name|perl_quote_buf
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|python_quote_buf
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|tcl_quote_buf
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|basic_regex_quote_buf
parameter_list|(
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|src
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

