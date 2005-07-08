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

begin_comment
comment|/* Help to copy the thing properly quoted for the shell safety.  * any single quote is replaced with '\'', and the whole thing  * is enclosed in a single quote pair.  *  * For example, if you are passing the result to system() as an  * argument:  *  * sprintf(cmd, "foobar %s %s", sq_quote(arg0), sq_quote(arg1))  *  * would be appropriate.  If the system() is going to call ssh to  * run the command on the other side:  *  * sprintf(cmd, "git-diff-tree %s %s", sq_quote(arg0), sq_quote(arg1));  * sprintf(rcmd, "ssh %s %s", sq_quote(host), sq_quote(cmd));  *  * Note that the above examples leak memory!  Remember to free result from  * sq_quote() in a real application.  */
end_comment

begin_function_decl
name|char
modifier|*
name|sq_quote
parameter_list|(
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

