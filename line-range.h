begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|LINE_RANGE_H
end_ifndef

begin_define
DECL|macro|LINE_RANGE_H
define|#
directive|define
name|LINE_RANGE_H
end_define

begin_comment
comment|/*  * Parse one item in an -L begin,end option w.r.t. the notional file  * object 'cb_data' consisting of 'lines' lines.  *  * The 'nth_line_cb' callback is used to determine the start of the  * line 'lno' inside the 'cb_data'.  The caller is expected to already  * have a suitable map at hand to make this a constant-time lookup.  *  * 'anchor' is the 1-based line at which relative range specifications  * should be anchored. Absolute ranges are unaffected by this value.  *  * Returns 0 in case of success and -1 if there was an error.  The  * actual range is stored in *begin and *end.  The counting starts  * at 1!  In case of error, the caller should show usage message.  */
end_comment

begin_typedef
DECL|typedef|nth_line_fn_t
typedef|typedef
specifier|const
name|char
modifier|*
function_decl|(
modifier|*
name|nth_line_fn_t
function_decl|)
parameter_list|(
name|void
modifier|*
name|data
parameter_list|,
name|long
name|lno
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
specifier|extern
name|int
name|parse_range_arg
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|,
name|nth_line_fn_t
name|nth_line_cb
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|,
name|long
name|lines
parameter_list|,
name|long
name|anchor
parameter_list|,
name|long
modifier|*
name|begin
parameter_list|,
name|long
modifier|*
name|end
parameter_list|,
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Scan past a range argument that could be parsed by  * 'parse_range_arg', to help the caller determine the start of the  * filename in '-L n,m:file' syntax.  *  * Returns a pointer to the first character after the 'n,m' part, or  * NULL in case the argument is obviously malformed.  */
end_comment

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|skip_range_arg
parameter_list|(
specifier|const
name|char
modifier|*
name|arg
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* LINE_RANGE_H */
end_comment

end_unit

