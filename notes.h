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

begin_function_decl
name|void
name|get_commit_notes
parameter_list|(
specifier|const
name|struct
name|commit
modifier|*
name|commit
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

