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
comment|/*  * Used as a flag in ref_update::flags when a loose ref is being  * pruned.  */
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
name|struct
name|string_list
modifier|*
name|extras
parameter_list|,
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
comment|/* 	 * One or more of REF_HAVE_NEW, REF_HAVE_OLD, REF_NODEREF, 	 * REF_DELETING, and REF_ISPRUNING: 	 */
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
name|int
name|type
decl_stmt|;
DECL|member|msg
name|char
modifier|*
name|msg
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

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* REFS_REFS_INTERNAL_H */
end_comment

end_unit

