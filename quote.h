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

begin_include
include|#
directive|include
file|<stddef.h>
end_include

begin_include
include|#
directive|include
file|<stdio.h>
end_include

begin_comment
comment|/* Help to copy the thing properly quoted for the shell safety.  * any single quote is replaced with '\'', any exclamation point  * is replaced with '\!', and the whole thing is enclosed in a  * single quote pair.  *  * For example, if you are passing the result to system() as an  * argument:  *  * sprintf(cmd, "foobar %s %s", sq_quote(arg0), sq_quote(arg1))  *  * would be appropriate.  If the system() is going to call ssh to  * run the command on the other side:  *  * sprintf(cmd, "git-diff-tree %s %s", sq_quote(arg0), sq_quote(arg1));  * sprintf(rcmd, "ssh %s %s", sq_quote(host), sq_quote(cmd));  *  * Note that the above examples leak memory!  Remember to free result from  * sq_quote() in a real application.  *  * sq_quote_buf() writes to an existing buffer of specified size; it  * will return the number of characters that would have been written  * excluding the final null regardless of the buffer size.  */
end_comment

begin_function_decl
specifier|extern
name|void
name|sq_quote_print
parameter_list|(
name|FILE
modifier|*
name|stream
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
name|char
modifier|*
name|sq_quote_argv
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|argv
parameter_list|,
name|int
name|count
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Append a string to a string buffer, with or without shell quoting.  * Return true if the buffer overflowed.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|add_to_string
parameter_list|(
name|char
modifier|*
modifier|*
name|ptrp
parameter_list|,
name|int
modifier|*
name|sizep
parameter_list|,
specifier|const
name|char
modifier|*
name|str
parameter_list|,
name|int
name|quote
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
name|int
name|quote_c_style
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|char
modifier|*
name|outbuf
parameter_list|,
name|FILE
modifier|*
name|outfp
parameter_list|,
name|int
name|nodq
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
name|prefix
parameter_list|,
name|int
name|prefix_len
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
name|int
name|quote
parameter_list|,
name|FILE
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
name|perl_quote_print
parameter_list|(
name|FILE
modifier|*
name|stream
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
name|python_quote_print
parameter_list|(
name|FILE
modifier|*
name|stream
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
name|tcl_quote_print
parameter_list|(
name|FILE
modifier|*
name|stream
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

