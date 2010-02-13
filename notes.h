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
comment|/*  * Flags controlling behaviour of notes tree initialization  *  * Default behaviour is to initialize the notes tree from the tree object  * specified by the given (or default) notes ref.  */
end_comment

begin_define
DECL|macro|NOTES_INIT_EMPTY
define|#
directive|define
name|NOTES_INIT_EMPTY
value|1
end_define

begin_comment
comment|/*  * Initialize internal notes tree structure with the notes tree at the given  * ref. If given ref is NULL, the value of the $GIT_NOTES_REF environment  * variable is used, and if that is missing, the default notes ref is used  * ("refs/notes/commits").  *  * If you need to re-intialize the internal notes tree structure (e.g. loading  * from a different notes ref), please first de-initialize the current notes  * tree by calling free_notes().  */
end_comment

begin_function_decl
name|void
name|init_notes
parameter_list|(
specifier|const
name|char
modifier|*
name|notes_ref
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Add the given note object to the internal notes tree structure */
end_comment

begin_function_decl
name|void
name|add_note
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|object_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|note_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Remove the given note object from the internal notes tree structure */
end_comment

begin_function_decl
name|void
name|remove_note
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|object_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Get the note object SHA1 containing the note data for the given object  *  * Return NULL if the given object has no notes.  */
end_comment

begin_function_decl
specifier|const
name|unsigned
name|char
modifier|*
name|get_note
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|object_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Flags controlling behaviour of for_each_note()  *  * Default behaviour of for_each_note() is to traverse every single note object  * in the notes tree, unpacking subtree entries along the way.  * The following flags can be used to alter the default behaviour:  *  * - DONT_UNPACK_SUBTREES causes for_each_note() NOT to unpack and recurse into  *   subtree entries while traversing the notes tree. This causes notes within  *   those subtrees NOT to be passed to the callback. Use this flag if you  *   don't want to traverse _all_ notes, but only want to traverse the parts  *   of the notes tree that have already been unpacked (this includes at least  *   all notes that have been added/changed).  *  * - YIELD_SUBTREES causes any subtree entries that are encountered to be  *   passed to the callback, before recursing into them. Subtree entries are  *   not note objects, but represent intermediate directories in the notes  *   tree. When passed to the callback, subtree entries will have a trailing  *   slash in their path, which the callback may use to differentiate between  *   note entries and subtree entries. Note that already-unpacked subtree  *   entries are not part of the notes tree, and will therefore not be yielded.  *   If this flag is used together with DONT_UNPACK_SUBTREES, for_each_note()  *   will yield the subtree entry, but not recurse into it.  */
end_comment

begin_define
DECL|macro|FOR_EACH_NOTE_DONT_UNPACK_SUBTREES
define|#
directive|define
name|FOR_EACH_NOTE_DONT_UNPACK_SUBTREES
value|1
end_define

begin_define
DECL|macro|FOR_EACH_NOTE_YIELD_SUBTREES
define|#
directive|define
name|FOR_EACH_NOTE_YIELD_SUBTREES
value|2
end_define

begin_comment
comment|/*  * Invoke the specified callback function for each note  *  * If the callback returns nonzero, the note walk is aborted, and the return  * value from the callback is returned from for_each_note(). Hence, a zero  * return value from for_each_note() indicates that all notes were walked  * successfully.  *  * IMPORTANT: The callback function is NOT allowed to change the notes tree.  * In other words, the following functions can NOT be invoked (on the current  * notes tree) from within the callback:  * - add_note()  * - remove_note()  * - free_notes()  */
end_comment

begin_typedef
DECL|typedef|each_note_fn
typedef|typedef
name|int
name|each_note_fn
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|object_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|note_sha1
parameter_list|,
name|char
modifier|*
name|note_path
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|int
name|for_each_note
parameter_list|(
name|int
name|flags
parameter_list|,
name|each_note_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

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

