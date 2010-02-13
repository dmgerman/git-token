begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|NOTES_H
end_ifndef

begin_define
DECL|macro|NOTES_H
define|#
directive|define
name|NOTES_H
end_define

begin_comment
comment|/* Free (and de-initialize) the internal notes tree structure */
end_comment

begin_function_decl
name|void
name|free_notes
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Flags controlling how notes are formatted */
end_comment

begin_define
DECL|macro|NOTES_SHOW_HEADER
define|#
directive|define
name|NOTES_SHOW_HEADER
value|1
end_define

begin_define
DECL|macro|NOTES_INDENT
define|#
directive|define
name|NOTES_INDENT
value|2
end_define

begin_comment
comment|/*  * Fill the given strbuf with the notes associated with the given object.  *  * If the internal notes structure is not initialized, it will be auto-  * initialized to the default value (see documentation for init_notes() above).  *  * 'flags' is a bitwise combination of the above formatting flags.  */
end_comment

begin_function_decl
name|void
name|format_note
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|object_sha1
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|,
specifier|const
name|char
modifier|*
name|output_encoding
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

