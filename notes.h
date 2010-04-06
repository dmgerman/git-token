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
comment|/*  * Function type for combining two notes annotating the same object.  *  * When adding a new note annotating the same object as an existing note, it is  * up to the caller to decide how to combine the two notes. The decision is  * made by passing in a function of the following form. The function accepts  * two SHA1s -- of the existing note and the new note, respectively. The  * function then combines the notes in whatever way it sees fit, and writes the  * resulting SHA1 into the first SHA1 argument (cur_sha1). A non-zero return  * value indicates failure.  *  * The two given SHA1s must both be non-NULL and different from each other.  *  * The default combine_notes function (you get this when passing NULL) is  * combine_notes_concatenate(), which appends the contents of the new note to  * the contents of the existing note.  */
end_comment

begin_typedef
DECL|typedef|combine_notes_fn
typedef|typedef
name|int
name|combine_notes_fn
parameter_list|(
name|unsigned
name|char
modifier|*
name|cur_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|)
function_decl|;
end_typedef

begin_comment
comment|/* Common notes combinators */
end_comment

begin_function_decl
name|int
name|combine_notes_concatenate
parameter_list|(
name|unsigned
name|char
modifier|*
name|cur_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|combine_notes_overwrite
parameter_list|(
name|unsigned
name|char
modifier|*
name|cur_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|combine_notes_ignore
parameter_list|(
name|unsigned
name|char
modifier|*
name|cur_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Notes tree object  *  * Encapsulates the internal notes tree structure associated with a notes ref.  * Whenever a struct notes_tree pointer is required below, you may pass NULL in  * order to use the default/internal notes tree. E.g. you only need to pass a  * non-NULL value if you need to refer to several different notes trees  * simultaneously.  */
end_comment

begin_struct
DECL|struct|notes_tree
specifier|extern
struct|struct
name|notes_tree
block|{
DECL|member|root
name|struct
name|int_node
modifier|*
name|root
decl_stmt|;
DECL|member|first_non_note
DECL|member|prev_non_note
name|struct
name|non_note
modifier|*
name|first_non_note
decl_stmt|,
modifier|*
name|prev_non_note
decl_stmt|;
DECL|member|ref
name|char
modifier|*
name|ref
decl_stmt|;
DECL|member|combine_notes
name|combine_notes_fn
modifier|*
name|combine_notes
decl_stmt|;
DECL|member|initialized
name|int
name|initialized
decl_stmt|;
DECL|member|dirty
name|int
name|dirty
decl_stmt|;
block|}
name|default_notes_tree
struct|;
end_struct

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
comment|/*  * Initialize the given notes_tree with the notes tree structure at the given  * ref. If given ref is NULL, the value of the $GIT_NOTES_REF environment  * variable is used, and if that is missing, the default notes ref is used  * ("refs/notes/commits").  *  * If you need to re-intialize a notes_tree structure (e.g. when switching from  * one notes ref to another), you must first de-initialize the notes_tree  * structure by calling free_notes(struct notes_tree *).  *  * If you pass t == NULL, the default internal notes_tree will be initialized.  *  * The combine_notes function that is passed becomes the default combine_notes  * function for the given notes_tree. If NULL is passed, the default  * combine_notes function is combine_notes_concatenate().  *  * Precondition: The notes_tree structure is zeroed (this can be achieved with  * memset(t, 0, sizeof(struct notes_tree)))  */
end_comment

begin_function_decl
name|void
name|init_notes
parameter_list|(
name|struct
name|notes_tree
modifier|*
name|t
parameter_list|,
specifier|const
name|char
modifier|*
name|notes_ref
parameter_list|,
name|combine_notes_fn
name|combine_notes
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Add the given note object to the given notes_tree structure  *  * IMPORTANT: The changes made by add_note() to the given notes_tree structure  * are not persistent until a subsequent call to write_notes_tree() returns  * zero.  */
end_comment

begin_function_decl
name|void
name|add_note
parameter_list|(
name|struct
name|notes_tree
modifier|*
name|t
parameter_list|,
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
name|combine_notes_fn
name|combine_notes
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Remove the given note object from the given notes_tree structure  *  * IMPORTANT: The changes made by remove_note() to the given notes_tree  * structure are not persistent until a subsequent call to write_notes_tree()  * returns zero.  */
end_comment

begin_function_decl
name|void
name|remove_note
parameter_list|(
name|struct
name|notes_tree
modifier|*
name|t
parameter_list|,
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
name|struct
name|notes_tree
modifier|*
name|t
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|object_sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Copy a note from one object to another in the given notes_tree.  *  * Fails if the to_obj already has a note unless 'force' is true.  */
end_comment

begin_function_decl
name|int
name|copy_note
parameter_list|(
name|struct
name|notes_tree
modifier|*
name|t
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|from_obj
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|to_obj
parameter_list|,
name|int
name|force
parameter_list|,
name|combine_notes_fn
name|combine_fn
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Flags controlling behaviour of for_each_note()  *  * Default behaviour of for_each_note() is to traverse every single note object  * in the given notes tree, unpacking subtree entries along the way.  * The following flags can be used to alter the default behaviour:  *  * - DONT_UNPACK_SUBTREES causes for_each_note() NOT to unpack and recurse into  *   subtree entries while traversing the notes tree. This causes notes within  *   those subtrees NOT to be passed to the callback. Use this flag if you  *   don't want to traverse _all_ notes, but only want to traverse the parts  *   of the notes tree that have already been unpacked (this includes at least  *   all notes that have been added/changed).  *  * - YIELD_SUBTREES causes any subtree entries that are encountered to be  *   passed to the callback, before recursing into them. Subtree entries are  *   not note objects, but represent intermediate directories in the notes  *   tree. When passed to the callback, subtree entries will have a trailing  *   slash in their path, which the callback may use to differentiate between  *   note entries and subtree entries. Note that already-unpacked subtree  *   entries are not part of the notes tree, and will therefore not be yielded.  *   If this flag is used together with DONT_UNPACK_SUBTREES, for_each_note()  *   will yield the subtree entry, but not recurse into it.  */
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
comment|/*  * Invoke the specified callback function for each note in the given notes_tree  *  * If the callback returns nonzero, the note walk is aborted, and the return  * value from the callback is returned from for_each_note(). Hence, a zero  * return value from for_each_note() indicates that all notes were walked  * successfully.  *  * IMPORTANT: The callback function is NOT allowed to change the notes tree.  * In other words, the following functions can NOT be invoked (on the current  * notes tree) from within the callback:  * - add_note()  * - remove_note()  * - free_notes()  */
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
name|struct
name|notes_tree
modifier|*
name|t
parameter_list|,
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
comment|/*  * Write the given notes_tree structure to the object database  *  * Creates a new tree object encapsulating the current state of the given  * notes_tree, and stores its SHA1 into the 'result' argument.  *  * Returns zero on success, non-zero on failure.  *  * IMPORTANT: Changes made to the given notes_tree are not persistent until  * this function has returned zero. Please also remember to create a  * corresponding commit object, and update the appropriate notes ref.  */
end_comment

begin_function_decl
name|int
name|write_notes_tree
parameter_list|(
name|struct
name|notes_tree
modifier|*
name|t
parameter_list|,
name|unsigned
name|char
modifier|*
name|result
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Remove all notes annotating non-existing objects from the given notes tree  *  * All notes in the given notes_tree that are associated with objects that no  * longer exist in the database, are removed from the notes tree.  *  * IMPORTANT: The changes made by prune_notes() to the given notes_tree  * structure are not persistent until a subsequent call to write_notes_tree()  * returns zero.  */
end_comment

begin_function_decl
name|void
name|prune_notes
parameter_list|(
name|struct
name|notes_tree
modifier|*
name|t
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Free (and de-initialize) the given notes_tree structure  *  * IMPORTANT: Changes made to the given notes_tree since the last, successful  * call to write_notes_tree() will be lost.  */
end_comment

begin_function_decl
name|void
name|free_notes
parameter_list|(
name|struct
name|notes_tree
modifier|*
name|t
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
comment|/*  * Fill the given strbuf with the notes associated with the given object.  *  * If the given notes_tree structure is not initialized, it will be auto-  * initialized to the default value (see documentation for init_notes() above).  * If the given notes_tree is NULL, the internal/default notes_tree will be  * used instead.  *  * 'flags' is a bitwise combination of the above formatting flags.  */
end_comment

begin_function_decl
name|void
name|format_note
parameter_list|(
name|struct
name|notes_tree
modifier|*
name|t
parameter_list|,
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

begin_struct_decl
struct_decl|struct
name|string_list
struct_decl|;
end_struct_decl

begin_struct
DECL|struct|display_notes_opt
struct|struct
name|display_notes_opt
block|{
DECL|member|suppress_default_notes
name|unsigned
name|int
name|suppress_default_notes
range|:
literal|1
decl_stmt|;
DECL|member|extra_notes_refs
name|struct
name|string_list
modifier|*
name|extra_notes_refs
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * Load the notes machinery for displaying several notes trees.  *  * If 'opt' is not NULL, then it specifies additional settings for the  * displaying:  *  * - suppress_default_notes indicates that the notes from  *   core.notesRef and notes.displayRef should not be loaded.  *  * - extra_notes_refs may contain a list of globs (in the same style  *   as notes.displayRef) where notes should be loaded from.  */
end_comment

begin_function_decl
name|void
name|init_display_notes
parameter_list|(
name|struct
name|display_notes_opt
modifier|*
name|opt
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Append notes for the given 'object_sha1' from all trees set up by  * init_display_notes() to 'sb'.  The 'flags' are a bitwise  * combination of  *  * - NOTES_SHOW_HEADER: add a 'Notes (refname):' header  *  * - NOTES_INDENT: indent the notes by 4 places  *  * You *must* call init_display_notes() before using this function.  */
end_comment

begin_function_decl
name|void
name|format_display_notes
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

begin_comment
comment|/*  * Load the notes tree from each ref listed in 'refs'.  The output is  * an array of notes_tree*, terminated by a NULL.  */
end_comment

begin_function_decl
name|struct
name|notes_tree
modifier|*
modifier|*
name|load_notes_trees
parameter_list|(
name|struct
name|string_list
modifier|*
name|refs
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Add all refs that match 'glob' to the 'list'.  */
end_comment

begin_function_decl
name|void
name|string_list_add_refs_by_glob
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|glob
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Add all refs from a colon-separated glob list 'globs' to the end of  * 'list'.  Empty components are ignored.  This helper is used to  * parse GIT_NOTES_DISPLAY_REF style environment variables.  */
end_comment

begin_function_decl
name|void
name|string_list_add_refs_from_colon_sep
parameter_list|(
name|struct
name|string_list
modifier|*
name|list
parameter_list|,
specifier|const
name|char
modifier|*
name|globs
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

