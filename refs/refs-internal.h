begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|REFS_REFS_INTERNAL_H
end_ifndef

begin_define
DECL|macro|REFS_REFS_INTERNAL_H
define|#
directive|define
name|REFS_REFS_INTERNAL_H
end_define

begin_comment
comment|/*  * Data structures and functions for the internal use of the refs  * module. Code outside of the refs module should use only the public  * functions defined in "refs.h", and should *not* include this file.  */
end_comment

begin_comment
comment|/*  * Flag passed to lock_ref_sha1_basic() telling it to tolerate broken  * refs (i.e., because the reference is about to be deleted anyway).  */
end_comment

begin_define
DECL|macro|REF_DELETING
define|#
directive|define
name|REF_DELETING
value|0x02
end_define

begin_comment
comment|/*  * Used as a flag in ref_update::flags when a loose ref is being  * pruned. This flag must only be used when REF_NODEREF is set.  */
end_comment

begin_define
DECL|macro|REF_ISPRUNING
define|#
directive|define
name|REF_ISPRUNING
value|0x04
end_define

begin_comment
comment|/*  * Used as a flag in ref_update::flags when the reference should be  * updated to new_sha1.  */
end_comment

begin_define
DECL|macro|REF_HAVE_NEW
define|#
directive|define
name|REF_HAVE_NEW
value|0x08
end_define

begin_comment
comment|/*  * Used as a flag in ref_update::flags when old_sha1 should be  * checked.  */
end_comment

begin_define
DECL|macro|REF_HAVE_OLD
define|#
directive|define
name|REF_HAVE_OLD
value|0x10
end_define

begin_comment
comment|/*  * Used as a flag in ref_update::flags when the lockfile needs to be  * committed.  */
end_comment

begin_define
DECL|macro|REF_NEEDS_COMMIT
define|#
directive|define
name|REF_NEEDS_COMMIT
value|0x20
end_define

begin_comment
comment|/*  * 0x40 is REF_FORCE_CREATE_REFLOG, so skip it if you're adding a  * value to ref_update::flags  */
end_comment

begin_comment
comment|/*  * Used as a flag in ref_update::flags when we want to log a ref  * update but not actually perform it.  This is used when a symbolic  * ref update is split up.  */
end_comment

begin_define
DECL|macro|REF_LOG_ONLY
define|#
directive|define
name|REF_LOG_ONLY
value|0x80
end_define

begin_comment
comment|/*  * Internal flag, meaning that the containing ref_update was via an  * update to HEAD.  */
end_comment

begin_define
DECL|macro|REF_UPDATE_VIA_HEAD
define|#
directive|define
name|REF_UPDATE_VIA_HEAD
value|0x100
end_define

begin_comment
comment|/*  * Return true iff refname is minimally safe. "Safe" here means that  * deleting a loose reference by this name will not do any damage, for  * example by causing a file that is not a reference to be deleted.  * This function does not check that the reference name is legal; for  * that, use check_refname_format().  *  * We consider a refname that starts with "refs/" to be safe as long  * as any ".." components that it might contain do not escape "refs/".  * Names that do not start with "refs/" are considered safe iff they  * consist entirely of upper case characters and '_' (like "HEAD" and  * "MERGE_HEAD" but not "config" or "FOO/BAR").  */
end_comment

begin_function_decl
name|int
name|refname_is_safe
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|)
function_decl|;
end_function_decl

begin_enum
DECL|enum|peel_status
enum|enum
name|peel_status
block|{
comment|/* object was peeled successfully: */
DECL|enumerator|PEEL_PEELED
name|PEEL_PEELED
init|=
literal|0
block|,
comment|/* 	 * object cannot be peeled because the named object (or an 	 * object referred to by a tag in the peel chain), does not 	 * exist. 	 */
DECL|enumerator|PEEL_INVALID
name|PEEL_INVALID
init|=
operator|-
literal|1
block|,
comment|/* object cannot be peeled because it is not a tag: */
DECL|enumerator|PEEL_NON_TAG
name|PEEL_NON_TAG
init|=
operator|-
literal|2
block|,
comment|/* ref_entry contains no peeled value because it is a symref: */
DECL|enumerator|PEEL_IS_SYMREF
name|PEEL_IS_SYMREF
init|=
operator|-
literal|3
block|,
comment|/* 	 * ref_entry cannot be peeled because it is broken (i.e., the 	 * symbolic reference cannot even be resolved to an object 	 * name): 	 */
DECL|enumerator|PEEL_BROKEN
name|PEEL_BROKEN
init|=
operator|-
literal|4
block|}
enum|;
end_enum

begin_comment
comment|/*  * Peel the named object; i.e., if the object is a tag, resolve the  * tag recursively until a non-tag is found.  If successful, store the  * result to sha1 and return PEEL_PEELED.  If the object is not a tag  * or is not valid, return PEEL_NON_TAG or PEEL_INVALID, respectively,  * and leave sha1 unchanged.  */
end_comment

begin_function_decl
name|enum
name|peel_status
name|peel_object
parameter_list|(
specifier|const
name|unsigned
name|char
modifier|*
name|name
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Return 0 if a reference named refname could be created without  * conflicting with the name of an existing reference. Otherwise,  * return a negative value and write an explanation to err. If extras  * is non-NULL, it is a list of additional refnames with which refname  * is not allowed to conflict. If skip is non-NULL, ignore potential  * conflicts with refs in skip (e.g., because they are scheduled for  * deletion in the same operation). Behavior is undefined if the same  * name is listed in both extras and skip.  *  * Two reference names conflict if one of them exactly matches the  * leading components of the other; e.g., "foo/bar" conflicts with  * both "foo" and with "foo/bar/baz" but not with "foo/bar" or  * "foo/barbados".  *  * extras and skip must be sorted.  */
end_comment

begin_function_decl
name|int
name|verify_refname_available
parameter_list|(
specifier|const
name|char
modifier|*
name|newname
parameter_list|,
specifier|const
name|struct
name|string_list
modifier|*
name|extras
parameter_list|,
specifier|const
name|struct
name|string_list
modifier|*
name|skip
parameter_list|,
name|struct
name|strbuf
modifier|*
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Copy the reflog message msg to buf, which has been allocated sufficiently  * large, while cleaning up the whitespaces.  Especially, convert LF to space,  * because reflog file is one line per entry.  */
end_comment

begin_function_decl
name|int
name|copy_reflog_msg
parameter_list|(
name|char
modifier|*
name|buf
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|should_autocreate_reflog
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Information needed for a single ref update. Set new_sha1 to the new  * value or to null_sha1 to delete the ref. To check the old value  * while the ref is locked, set (flags& REF_HAVE_OLD) and set  * old_sha1 to the old value, or to null_sha1 to ensure the ref does  * not exist before update.  */
end_comment

begin_struct
DECL|struct|ref_update
struct|struct
name|ref_update
block|{
comment|/* 	 * If (flags& REF_HAVE_NEW), set the reference to this value: 	 */
DECL|member|new_sha1
name|unsigned
name|char
name|new_sha1
index|[
literal|20
index|]
decl_stmt|;
comment|/* 	 * If (flags& REF_HAVE_OLD), check that the reference 	 * previously had this value: 	 */
DECL|member|old_sha1
name|unsigned
name|char
name|old_sha1
index|[
literal|20
index|]
decl_stmt|;
comment|/* 	 * One or more of REF_HAVE_NEW, REF_HAVE_OLD, REF_NODEREF, 	 * REF_DELETING, REF_ISPRUNING, REF_LOG_ONLY, and 	 * REF_UPDATE_VIA_HEAD: 	 */
DECL|member|flags
name|unsigned
name|int
name|flags
decl_stmt|;
DECL|member|lock
name|struct
name|ref_lock
modifier|*
name|lock
decl_stmt|;
DECL|member|type
name|unsigned
name|int
name|type
decl_stmt|;
DECL|member|msg
name|char
modifier|*
name|msg
decl_stmt|;
comment|/* 	 * If this ref_update was split off of a symref update via 	 * split_symref_update(), then this member points at that 	 * update. This is used for two purposes: 	 * 1. When reporting errors, we report the refname under which 	 *    the update was originally requested. 	 * 2. When we read the old value of this reference, we 	 *    propagate it back to its parent update for recording in 	 *    the latter's reflog. 	 */
DECL|member|parent_update
name|struct
name|ref_update
modifier|*
name|parent_update
decl_stmt|;
DECL|member|refname
specifier|const
name|char
name|refname
index|[
name|FLEX_ARRAY
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * Add a ref_update with the specified properties to transaction, and  * return a pointer to the new object. This function does not verify  * that refname is well-formed. new_sha1 and old_sha1 are only  * dereferenced if the REF_HAVE_NEW and REF_HAVE_OLD bits,  * respectively, are set in flags.  */
end_comment

begin_function_decl
name|struct
name|ref_update
modifier|*
name|ref_transaction_add_update
parameter_list|(
name|struct
name|ref_transaction
modifier|*
name|transaction
parameter_list|,
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Transaction states.  * OPEN:   The transaction is in a valid state and can accept new updates.  *         An OPEN transaction can be committed.  * CLOSED: A closed transaction is no longer active and no other operations  *         than free can be used on it in this state.  *         A transaction can either become closed by successfully committing  *         an active transaction or if there is a failure while building  *         the transaction thus rendering it failed/inactive.  */
end_comment

begin_enum
DECL|enum|ref_transaction_state
enum|enum
name|ref_transaction_state
block|{
DECL|enumerator|REF_TRANSACTION_OPEN
name|REF_TRANSACTION_OPEN
init|=
literal|0
block|,
DECL|enumerator|REF_TRANSACTION_CLOSED
name|REF_TRANSACTION_CLOSED
init|=
literal|1
block|}
enum|;
end_enum

begin_comment
comment|/*  * Data structure for holding a reference transaction, which can  * consist of checks and updates to multiple references, carried out  * as atomically as possible.  This structure is opaque to callers.  */
end_comment

begin_struct
DECL|struct|ref_transaction
struct|struct
name|ref_transaction
block|{
DECL|member|updates
name|struct
name|ref_update
modifier|*
modifier|*
name|updates
decl_stmt|;
DECL|member|alloc
name|size_t
name|alloc
decl_stmt|;
DECL|member|nr
name|size_t
name|nr
decl_stmt|;
DECL|member|state
name|enum
name|ref_transaction_state
name|state
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|files_log_ref_write
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|,
name|int
name|flags
parameter_list|,
name|struct
name|strbuf
modifier|*
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Check for entries in extras that are within the specified  * directory, where dirname is a reference directory name including  * the trailing slash (e.g., "refs/heads/foo/"). Ignore any  * conflicting references that are found in skip. If there is a  * conflicting reference, return its name.  *  * extras and skip must be sorted lists of reference names. Either one  * can be NULL, signifying the empty list.  */
end_comment

begin_function_decl
specifier|const
name|char
modifier|*
name|find_descendant_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|dirname
parameter_list|,
specifier|const
name|struct
name|string_list
modifier|*
name|extras
parameter_list|,
specifier|const
name|struct
name|string_list
modifier|*
name|skip
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Check whether an attempt to rename old_refname to new_refname would  * cause a D/F conflict with any existing reference (other than  * possibly old_refname). If there would be a conflict, emit an error  * message and return false; otherwise, return true.  *  * Note that this function is not safe against all races with other  * processes (though rename_ref() catches some races that might get by  * this check).  */
end_comment

begin_function_decl
name|int
name|rename_ref_available
parameter_list|(
specifier|const
name|char
modifier|*
name|old_refname
parameter_list|,
specifier|const
name|char
modifier|*
name|new_refname
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* We allow "recursive" symbolic refs. Only within reason, though */
end_comment

begin_define
DECL|macro|SYMREF_MAXDEPTH
define|#
directive|define
name|SYMREF_MAXDEPTH
value|5
end_define

begin_comment
comment|/* Include broken references in a do_for_each_ref*() iteration: */
end_comment

begin_define
DECL|macro|DO_FOR_EACH_INCLUDE_BROKEN
define|#
directive|define
name|DO_FOR_EACH_INCLUDE_BROKEN
value|0x01
end_define

begin_comment
comment|/*  * Reference iterators  *  * A reference iterator encapsulates the state of an in-progress  * iteration over references. Create an instance of `struct  * ref_iterator` via one of the functions in this module.  *  * A freshly-created ref_iterator doesn't yet point at a reference. To  * advance the iterator, call ref_iterator_advance(). If successful,  * this sets the iterator's refname, oid, and flags fields to describe  * the next reference and returns ITER_OK. The data pointed at by  * refname and oid belong to the iterator; if you want to retain them  * after calling ref_iterator_advance() again or calling  * ref_iterator_abort(), you must make a copy. When the iteration has  * been exhausted, ref_iterator_advance() releases any resources  * assocated with the iteration, frees the ref_iterator object, and  * returns ITER_DONE. If you want to abort the iteration early, call  * ref_iterator_abort(), which also frees the ref_iterator object and  * any associated resources. If there was an internal error advancing  * to the next entry, ref_iterator_advance() aborts the iteration,  * frees the ref_iterator, and returns ITER_ERROR.  *  * The reference currently being looked at can be peeled by calling  * ref_iterator_peel(). This function is often faster than peel_ref(),  * so it should be preferred when iterating over references.  *  * Putting it all together, a typical iteration looks like this:  *  *     int ok;  *     struct ref_iterator *iter = ...;  *  *     while ((ok = ref_iterator_advance(iter)) == ITER_OK) {  *             if (want_to_stop_iteration()) {  *                     ok = ref_iterator_abort(iter);  *                     break;  *             }  *  *             // Access information about the current reference:  *             if (!(iter->flags& REF_ISSYMREF))  *                     printf("%s is %s\n", iter->refname, oid_to_hex(&iter->oid));  *  *             // If you need to peel the reference:  *             ref_iterator_peel(iter,&oid);  *     }  *  *     if (ok != ITER_DONE)  *             handle_error();  */
end_comment

begin_struct
DECL|struct|ref_iterator
struct|struct
name|ref_iterator
block|{
DECL|member|vtable
name|struct
name|ref_iterator_vtable
modifier|*
name|vtable
decl_stmt|;
DECL|member|refname
specifier|const
name|char
modifier|*
name|refname
decl_stmt|;
DECL|member|oid
specifier|const
name|struct
name|object_id
modifier|*
name|oid
decl_stmt|;
DECL|member|flags
name|unsigned
name|int
name|flags
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * Advance the iterator to the first or next item and return ITER_OK.  * If the iteration is exhausted, free the resources associated with  * the ref_iterator and return ITER_DONE. On errors, free the iterator  * resources and return ITER_ERROR. It is a bug to use ref_iterator or  * call this function again after it has returned ITER_DONE or  * ITER_ERROR.  */
end_comment

begin_function_decl
name|int
name|ref_iterator_advance
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|ref_iterator
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * If possible, peel the reference currently being viewed by the  * iterator. Return 0 on success.  */
end_comment

begin_function_decl
name|int
name|ref_iterator_peel
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|ref_iterator
parameter_list|,
name|struct
name|object_id
modifier|*
name|peeled
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * End the iteration before it has been exhausted, freeing the  * reference iterator and any associated resources and returning  * ITER_DONE. If the abort itself failed, return ITER_ERROR.  */
end_comment

begin_function_decl
name|int
name|ref_iterator_abort
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|ref_iterator
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * An iterator over nothing (its first ref_iterator_advance() call  * returns ITER_DONE).  */
end_comment

begin_function_decl
name|struct
name|ref_iterator
modifier|*
name|empty_ref_iterator_begin
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Return true iff ref_iterator is an empty_ref_iterator.  */
end_comment

begin_function_decl
name|int
name|is_empty_ref_iterator
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|ref_iterator
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * A callback function used to instruct merge_ref_iterator how to  * interleave the entries from iter0 and iter1. The function should  * return one of the constants defined in enum iterator_selection. It  * must not advance either of the iterators itself.  *  * The function must be prepared to handle the case that iter0 and/or  * iter1 is NULL, which indicates that the corresponding sub-iterator  * has been exhausted. Its return value must be consistent with the  * current states of the iterators; e.g., it must not return  * ITER_SKIP_1 if iter1 has already been exhausted.  */
end_comment

begin_typedef
DECL|typedef|ref_iterator_select_fn
typedef|typedef
name|enum
name|iterator_selection
name|ref_iterator_select_fn
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|iter0
parameter_list|,
name|struct
name|ref_iterator
modifier|*
name|iter1
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_typedef

begin_comment
comment|/*  * Iterate over the entries from iter0 and iter1, with the values  * interleaved as directed by the select function. The iterator takes  * ownership of iter0 and iter1 and frees them when the iteration is  * over.  */
end_comment

begin_function_decl
name|struct
name|ref_iterator
modifier|*
name|merge_ref_iterator_begin
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|iter0
parameter_list|,
name|struct
name|ref_iterator
modifier|*
name|iter1
parameter_list|,
name|ref_iterator_select_fn
modifier|*
name|select
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * An iterator consisting of the union of the entries from front and  * back. If there are entries common to the two sub-iterators, use the  * one from front. Each iterator must iterate over its entries in  * strcmp() order by refname for this to work.  *  * The new iterator takes ownership of its arguments and frees them  * when the iteration is over. As a convenience to callers, if front  * or back is an empty_ref_iterator, then abort that one immediately  * and return the other iterator directly, without wrapping it.  */
end_comment

begin_function_decl
name|struct
name|ref_iterator
modifier|*
name|overlay_ref_iterator_begin
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|front
parameter_list|,
name|struct
name|ref_iterator
modifier|*
name|back
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Wrap iter0, only letting through the references whose names start  * with prefix. If trim is set, set iter->refname to the name of the  * reference with that many characters trimmed off the front;  * otherwise set it to the full refname. The new iterator takes over  * ownership of iter0 and frees it when iteration is over. It makes  * its own copy of prefix.  *  * As an convenience to callers, if prefix is the empty string and  * trim is zero, this function returns iter0 directly, without  * wrapping it.  */
end_comment

begin_function_decl
name|struct
name|ref_iterator
modifier|*
name|prefix_ref_iterator_begin
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|iter0
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|int
name|trim
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Iterate over the packed and loose references in the specified  * submodule that are within find_containing_dir(prefix). If prefix is  * NULL or the empty string, iterate over all references in the  * submodule.  */
end_comment

begin_function_decl
name|struct
name|ref_iterator
modifier|*
name|files_ref_iterator_begin
parameter_list|(
specifier|const
name|char
modifier|*
name|submodule
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Iterate over the references in the main ref_store that have a  * reflog. The paths within a directory are iterated over in arbitrary  * order.  */
end_comment

begin_function_decl
name|struct
name|ref_iterator
modifier|*
name|files_reflog_iterator_begin
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Internal implementation of reference iteration: */
end_comment

begin_comment
comment|/*  * Base class constructor for ref_iterators. Initialize the  * ref_iterator part of iter, setting its vtable pointer as specified.  * This is meant to be called only by the initializers of derived  * classes.  */
end_comment

begin_function_decl
name|void
name|base_ref_iterator_init
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|iter
parameter_list|,
name|struct
name|ref_iterator_vtable
modifier|*
name|vtable
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Base class destructor for ref_iterators. Destroy the ref_iterator  * part of iter and shallow-free the object. This is meant to be  * called only by the destructors of derived classes.  */
end_comment

begin_function_decl
name|void
name|base_ref_iterator_free
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|iter
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Virtual function declarations for ref_iterators: */
end_comment

begin_typedef
DECL|typedef|ref_iterator_advance_fn
typedef|typedef
name|int
name|ref_iterator_advance_fn
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|ref_iterator
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ref_iterator_peel_fn
typedef|typedef
name|int
name|ref_iterator_peel_fn
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|ref_iterator
parameter_list|,
name|struct
name|object_id
modifier|*
name|peeled
parameter_list|)
function_decl|;
end_typedef

begin_comment
comment|/*  * Implementations of this function should free any resources specific  * to the derived class, then call base_ref_iterator_free() to clean  * up and free the ref_iterator object.  */
end_comment

begin_typedef
DECL|typedef|ref_iterator_abort_fn
typedef|typedef
name|int
name|ref_iterator_abort_fn
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|ref_iterator
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|ref_iterator_vtable
struct|struct
name|ref_iterator_vtable
block|{
DECL|member|advance
name|ref_iterator_advance_fn
modifier|*
name|advance
decl_stmt|;
DECL|member|peel
name|ref_iterator_peel_fn
modifier|*
name|peel
decl_stmt|;
DECL|member|abort
name|ref_iterator_abort_fn
modifier|*
name|abort
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * current_ref_iter is a performance hack: when iterating over  * references using the for_each_ref*() functions, current_ref_iter is  * set to the reference iterator before calling the callback function.  * If the callback function calls peel_ref(), then peel_ref() first  * checks whether the reference to be peeled is the one referred to by  * the iterator (it usually is) and if so, asks the iterator for the  * peeled version of the reference if it is available. This avoids a  * refname lookup in a common case. current_ref_iter is set to NULL  * when the iteration is over.  */
end_comment

begin_decl_stmt
specifier|extern
name|struct
name|ref_iterator
modifier|*
name|current_ref_iter
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * The common backend for the for_each_*ref* functions. Call fn for  * each reference in iter. If the iterator itself ever returns  * ITER_ERROR, return -1. If fn ever returns a non-zero value, stop  * the iteration and return that value. Otherwise, return 0. In any  * case, free the iterator when done. This function is basically an  * adapter between the callback style of reference iteration and the  * iterator style.  */
end_comment

begin_function_decl
name|int
name|do_for_each_ref_iterator
parameter_list|(
name|struct
name|ref_iterator
modifier|*
name|iter
parameter_list|,
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Read the specified reference from the filesystem or packed refs  * file, non-recursively. Set type to describe the reference, and:  *  * - If refname is the name of a normal reference, fill in sha1  *   (leaving referent unchanged).  *  * - If refname is the name of a symbolic reference, write the full  *   name of the reference to which it refers (e.g.  *   "refs/heads/master") to referent and set the REF_ISSYMREF bit in  *   type (leaving sha1 unchanged). The caller is responsible for  *   validating that referent is a valid reference name.  *  * WARNING: refname might be used as part of a filename, so it is  * important from a security standpoint that it be safe in the sense  * of refname_is_safe(). Moreover, for symrefs this function sets  * referent to whatever the repository says, which might not be a  * properly-formatted or even safe reference name. NEITHER INPUT NOR  * OUTPUT REFERENCE NAMES ARE VALIDATED WITHIN THIS FUNCTION.  *  * Return 0 on success. If the ref doesn't exist, set errno to ENOENT  * and return -1. If the ref exists but is neither a symbolic ref nor  * a sha1, it is broken; set REF_ISBROKEN in type, set errno to  * EINVAL, and return -1. If there is another error reading the ref,  * set errno appropriately and return -1.  *  * Backend-specific flags might be set in type as well, regardless of  * outcome.  *  * It is OK for refname to point into referent. If so:  *  * - if the function succeeds with REF_ISSYMREF, referent will be  *   overwritten and the memory formerly pointed to by it might be  *   changed or even freed.  *  * - in all other cases, referent will be untouched, and therefore  *   refname will still be valid and unchanged.  */
end_comment

begin_function_decl
name|int
name|read_raw_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|struct
name|strbuf
modifier|*
name|referent
parameter_list|,
name|unsigned
name|int
modifier|*
name|type
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* refs backends */
end_comment

begin_comment
comment|/*  * Initialize the ref_store for the specified submodule, or for the  * main repository if submodule == NULL. These functions should call  * base_ref_store_init() to initialize the shared part of the  * ref_store and to record the ref_store for later lookup.  */
end_comment

begin_typedef
DECL|typedef|ref_store_init_fn
typedef|typedef
name|struct
name|ref_store
modifier|*
name|ref_store_init_fn
parameter_list|(
specifier|const
name|char
modifier|*
name|submodule
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ref_transaction_commit_fn
typedef|typedef
name|int
name|ref_transaction_commit_fn
parameter_list|(
name|struct
name|ref_store
modifier|*
name|refs
parameter_list|,
name|struct
name|ref_transaction
modifier|*
name|transaction
parameter_list|,
name|struct
name|strbuf
modifier|*
name|err
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|ref_storage_be
struct|struct
name|ref_storage_be
block|{
DECL|member|next
name|struct
name|ref_storage_be
modifier|*
name|next
decl_stmt|;
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|init
name|ref_store_init_fn
modifier|*
name|init
decl_stmt|;
DECL|member|transaction_commit
name|ref_transaction_commit_fn
modifier|*
name|transaction_commit
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
specifier|extern
name|struct
name|ref_storage_be
name|refs_be_files
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * A representation of the reference store for the main repository or  * a submodule. The ref_store instances for submodules are kept in a  * linked list.  */
end_comment

begin_struct
DECL|struct|ref_store
struct|struct
name|ref_store
block|{
comment|/* The backend describing this ref_store's storage scheme: */
DECL|member|be
specifier|const
name|struct
name|ref_storage_be
modifier|*
name|be
decl_stmt|;
comment|/* 	 * The name of the submodule represented by this object, or 	 * the empty string if it represents the main repository's 	 * reference store: 	 */
DECL|member|submodule
specifier|const
name|char
modifier|*
name|submodule
decl_stmt|;
comment|/* 	 * Submodule reference store instances are stored in a linked 	 * list using this pointer. 	 */
DECL|member|next
name|struct
name|ref_store
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/*  * Fill in the generic part of refs for the specified submodule and  * add it to our collection of reference stores.  */
end_comment

begin_function_decl
name|void
name|base_ref_store_init
parameter_list|(
name|struct
name|ref_store
modifier|*
name|refs
parameter_list|,
specifier|const
name|struct
name|ref_storage_be
modifier|*
name|be
parameter_list|,
specifier|const
name|char
modifier|*
name|submodule
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Create, record, and return a ref_store instance for the specified  * submodule (or the main repository if submodule is NULL).  *  * For backwards compatibility, submodule=="" is treated the same as  * submodule==NULL.  */
end_comment

begin_function_decl
name|struct
name|ref_store
modifier|*
name|ref_store_init
parameter_list|(
specifier|const
name|char
modifier|*
name|submodule
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Return the ref_store instance for the specified submodule (or the  * main repository if submodule is NULL). If that ref_store hasn't  * been initialized yet, return NULL.  *  * For backwards compatibility, submodule=="" is treated the same as  * submodule==NULL.  */
end_comment

begin_function_decl
name|struct
name|ref_store
modifier|*
name|lookup_ref_store
parameter_list|(
specifier|const
name|char
modifier|*
name|submodule
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Return the ref_store instance for the specified submodule. For the  * main repository, use submodule==NULL; such a call cannot fail. For  * a submodule, the submodule must exist and be a nonbare repository,  * otherwise return NULL. If the requested reference store has not yet  * been initialized, initialize it first.  *  * For backwards compatibility, submodule=="" is treated the same as  * submodule==NULL.  */
end_comment

begin_function_decl
name|struct
name|ref_store
modifier|*
name|get_ref_store
parameter_list|(
specifier|const
name|char
modifier|*
name|submodule
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Die if refs is for a submodule (i.e., not for the main repository).  * caller is used in any necessary error messages.  */
end_comment

begin_function_decl
name|void
name|assert_main_repository
parameter_list|(
name|struct
name|ref_store
modifier|*
name|refs
parameter_list|,
specifier|const
name|char
modifier|*
name|caller
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* REFS_REFS_INTERNAL_H */
end_comment

end_unit

