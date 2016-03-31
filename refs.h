begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|REFS_H
end_ifndef

begin_define
DECL|macro|REFS_H
define|#
directive|define
name|REFS_H
end_define

begin_comment
comment|/*  * Resolve a reference, recursively following symbolic refererences.  *  * Store the referred-to object's name in sha1 and return the name of  * the non-symbolic reference that ultimately pointed at it.  The  * return value, if not NULL, is a pointer into either a static buffer  * or the input ref.  *  * If the reference cannot be resolved to an object, the behavior  * depends on the RESOLVE_REF_READING flag:  *  * - If RESOLVE_REF_READING is set, return NULL.  *  * - If RESOLVE_REF_READING is not set, clear sha1 and return the name of  *   the last reference name in the chain, which will either be a non-symbolic  *   reference or an undefined reference.  If this is a prelude to  *   "writing" to the ref, the return value is the name of the ref  *   that will actually be created or changed.  *  * If the RESOLVE_REF_NO_RECURSE flag is passed, only resolves one  * level of symbolic reference.  The value stored in sha1 for a symbolic  * reference will always be null_sha1 in this case, and the return  * value is the reference that the symref refers to directly.  *  * If flags is non-NULL, set the value that it points to the  * combination of REF_ISPACKED (if the reference was found among the  * packed references), REF_ISSYMREF (if the initial reference was a  * symbolic reference), REF_BAD_NAME (if the reference name is ill  * formed --- see RESOLVE_REF_ALLOW_BAD_NAME below), and REF_ISBROKEN  * (if the ref is malformed or has a bad name). See refs.h for more detail  * on each flag.  *  * If ref is not a properly-formatted, normalized reference, return  * NULL.  If more than MAXDEPTH recursive symbolic lookups are needed,  * give up and return NULL.  *  * RESOLVE_REF_ALLOW_BAD_NAME allows resolving refs even when their  * name is invalid according to git-check-ref-format(1).  If the name  * is bad then the value stored in sha1 will be null_sha1 and the two  * flags REF_ISBROKEN and REF_BAD_NAME will be set.  *  * Even with RESOLVE_REF_ALLOW_BAD_NAME, names that escape the refs/  * directory and do not consist of all caps and underscores cannot be  * resolved. The function returns NULL for such ref names.  * Caps and underscores refers to the special refs, such as HEAD,  * FETCH_HEAD and friends, that all live outside of the refs/ directory.  */
end_comment

begin_define
DECL|macro|RESOLVE_REF_READING
define|#
directive|define
name|RESOLVE_REF_READING
value|0x01
end_define

begin_define
DECL|macro|RESOLVE_REF_NO_RECURSE
define|#
directive|define
name|RESOLVE_REF_NO_RECURSE
value|0x02
end_define

begin_define
DECL|macro|RESOLVE_REF_ALLOW_BAD_NAME
define|#
directive|define
name|RESOLVE_REF_ALLOW_BAD_NAME
value|0x04
end_define

begin_function_decl
specifier|const
name|char
modifier|*
name|resolve_ref_unsafe
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|int
name|resolve_flags
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
modifier|*
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|char
modifier|*
name|resolve_refdup
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|int
name|resolve_flags
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
modifier|*
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|read_ref_full
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|int
name|resolve_flags
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|int
modifier|*
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|read_ref
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
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ref_exists
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|is_branch
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * If refname is a non-symbolic reference that refers to a tag object,  * and the tag can be (recursively) dereferenced to a non-tag object,  * store the SHA1 of the referred-to object to sha1 and return 0.  If  * any of these conditions are not met, return a non-zero value.  * Symbolic references are considered unpeelable, even if they  * ultimately resolve to a peelable tag.  */
end_comment

begin_function_decl
name|int
name|peel_ref
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
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Resolve refname in the nested "gitlink" repository that is located  * at path.  If the resolution is successful, return 0 and set sha1 to  * the name of the object; otherwise, return a non-zero value.  */
end_comment

begin_function_decl
name|int
name|resolve_gitlink_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Return true iff abbrev_name is a possible abbreviation for  * full_name according to the rules defined by ref_rev_parse_rules in  * refs.c.  */
end_comment

begin_function_decl
name|int
name|refname_match
parameter_list|(
specifier|const
name|char
modifier|*
name|abbrev_name
parameter_list|,
specifier|const
name|char
modifier|*
name|full_name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|dwim_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
name|int
name|len
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|char
modifier|*
modifier|*
name|ref
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|dwim_log
parameter_list|(
specifier|const
name|char
modifier|*
name|str
parameter_list|,
name|int
name|len
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|char
modifier|*
modifier|*
name|ref
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * A ref_transaction represents a collection of ref updates  * that should succeed or fail together.  *  * Calling sequence  * ----------------  * - Allocate and initialize a `struct ref_transaction` by calling  *   `ref_transaction_begin()`.  *  * - List intended ref updates by calling functions like  *   `ref_transaction_update()` and `ref_transaction_create()`.  *  * - Call `ref_transaction_commit()` to execute the transaction.  *   If this succeeds, the ref updates will have taken place and  *   the transaction cannot be rolled back.  *  * - Instead of `ref_transaction_commit`, use  *   `initial_ref_transaction_commit()` if the ref database is known  *   to be empty (e.g. during clone).  This is likely to be much  *   faster.  *  * - At any time call `ref_transaction_free()` to discard the  *   transaction and free associated resources.  In particular,  *   this rolls back the transaction if it has not been  *   successfully committed.  *  * Error handling  * --------------  *  * On error, transaction functions append a message about what  * went wrong to the 'err' argument.  The message mentions what  * ref was being updated (if any) when the error occurred so it  * can be passed to 'die' or 'error' as-is.  *  * The message is appended to err without first clearing err.  * err will not be '\n' terminated.  *  * Caveats  * -------  *  * Note that no locks are taken, and no refs are read, until  * `ref_transaction_commit` is called.  So `ref_transaction_verify`  * won't report a verification failure until the commit is attempted.  */
end_comment

begin_struct_decl
struct_decl|struct
name|ref_transaction
struct_decl|;
end_struct_decl

begin_comment
comment|/*  * Bit values set in the flags argument passed to each_ref_fn():  */
end_comment

begin_comment
comment|/* Reference is a symbolic reference. */
end_comment

begin_define
DECL|macro|REF_ISSYMREF
define|#
directive|define
name|REF_ISSYMREF
value|0x01
end_define

begin_comment
comment|/* Reference is a packed reference. */
end_comment

begin_define
DECL|macro|REF_ISPACKED
define|#
directive|define
name|REF_ISPACKED
value|0x02
end_define

begin_comment
comment|/*  * Reference cannot be resolved to an object name: dangling symbolic  * reference (directly or indirectly), corrupt reference file,  * reference exists but name is bad, or symbolic reference refers to  * ill-formatted reference name.  */
end_comment

begin_define
DECL|macro|REF_ISBROKEN
define|#
directive|define
name|REF_ISBROKEN
value|0x04
end_define

begin_comment
comment|/*  * Reference name is not well formed.  *  * See git-check-ref-format(1) for the definition of well formed ref names.  */
end_comment

begin_define
DECL|macro|REF_BAD_NAME
define|#
directive|define
name|REF_BAD_NAME
value|0x08
end_define

begin_comment
comment|/*  * The signature for the callback function for the for_each_*()  * functions below.  The memory pointed to by the refname and sha1  * arguments is only guaranteed to be valid for the duration of a  * single callback invocation.  */
end_comment

begin_typedef
DECL|typedef|each_ref_fn
typedef|typedef
name|int
name|each_ref_fn
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
specifier|const
name|struct
name|object_id
modifier|*
name|oid
parameter_list|,
name|int
name|flags
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_typedef

begin_comment
comment|/*  * The following functions invoke the specified callback function for  * each reference indicated.  If the function ever returns a nonzero  * value, stop the iteration and return that value.  Please note that  * it is not safe to modify references while an iteration is in  * progress, unless the same callback function invocation that  * modifies the reference also returns a nonzero value to immediately  * stop the iteration.  */
end_comment

begin_function_decl
name|int
name|head_ref
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_ref
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_ref_in
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
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

begin_function_decl
name|int
name|for_each_fullref_in
parameter_list|(
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|,
name|unsigned
name|int
name|broken
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_tag_ref
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_branch_ref
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_remote_ref
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_replace_ref
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_glob_ref
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_glob_ref_in
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|head_ref_submodule
parameter_list|(
specifier|const
name|char
modifier|*
name|submodule
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

begin_function_decl
name|int
name|for_each_ref_submodule
parameter_list|(
specifier|const
name|char
modifier|*
name|submodule
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

begin_function_decl
name|int
name|for_each_ref_in_submodule
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
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_tag_ref_submodule
parameter_list|(
specifier|const
name|char
modifier|*
name|submodule
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

begin_function_decl
name|int
name|for_each_branch_ref_submodule
parameter_list|(
specifier|const
name|char
modifier|*
name|submodule
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

begin_function_decl
name|int
name|for_each_remote_ref_submodule
parameter_list|(
specifier|const
name|char
modifier|*
name|submodule
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

begin_function_decl
name|int
name|head_ref_namespaced
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_namespaced_ref
parameter_list|(
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
comment|/* can be used to learn about broken ref and symref */
end_comment

begin_function_decl
name|int
name|for_each_rawref
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|has_glob_specials
specifier|static
specifier|inline
specifier|const
name|char
modifier|*
name|has_glob_specials
parameter_list|(
specifier|const
name|char
modifier|*
name|pattern
parameter_list|)
block|{
return|return
name|strpbrk
argument_list|(
name|pattern
argument_list|,
literal|"?*["
argument_list|)
return|;
block|}
end_function

begin_function_decl
name|void
name|warn_dangling_symref
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|msg_fmt
parameter_list|,
specifier|const
name|char
modifier|*
name|refname
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|warn_dangling_symrefs
parameter_list|(
name|FILE
modifier|*
name|fp
parameter_list|,
specifier|const
name|char
modifier|*
name|msg_fmt
parameter_list|,
specifier|const
name|struct
name|string_list
modifier|*
name|refnames
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Flags for controlling behaviour of pack_refs()  * PACK_REFS_PRUNE: Prune loose refs after packing  * PACK_REFS_ALL:   Pack _all_ refs, not just tags and already packed refs  */
end_comment

begin_define
DECL|macro|PACK_REFS_PRUNE
define|#
directive|define
name|PACK_REFS_PRUNE
value|0x0001
end_define

begin_define
DECL|macro|PACK_REFS_ALL
define|#
directive|define
name|PACK_REFS_ALL
value|0x0002
end_define

begin_comment
comment|/*  * Write a packed-refs file for the current repository.  * flags: Combination of the above PACK_REFS_* flags.  */
end_comment

begin_function_decl
name|int
name|pack_refs
parameter_list|(
name|unsigned
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Flags controlling ref_transaction_update(), ref_transaction_create(), etc.  * REF_NODEREF: act on the ref directly, instead of dereferencing  *              symbolic references.  *  * Other flags are reserved for internal use.  */
end_comment

begin_define
DECL|macro|REF_NODEREF
define|#
directive|define
name|REF_NODEREF
value|0x01
end_define

begin_define
DECL|macro|REF_FORCE_CREATE_REFLOG
define|#
directive|define
name|REF_FORCE_CREATE_REFLOG
value|0x40
end_define

begin_comment
comment|/*  * Setup reflog before using. Fill in err and return -1 on failure.  */
end_comment

begin_function_decl
name|int
name|safe_create_reflog
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|int
name|force_create
parameter_list|,
name|struct
name|strbuf
modifier|*
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Reads log for the value of ref during at_time. **/
end_comment

begin_function_decl
name|int
name|read_ref_at
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|,
name|unsigned
name|long
name|at_time
parameter_list|,
name|int
name|cnt
parameter_list|,
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
name|char
modifier|*
modifier|*
name|msg
parameter_list|,
name|unsigned
name|long
modifier|*
name|cutoff_time
parameter_list|,
name|int
modifier|*
name|cutoff_tz
parameter_list|,
name|int
modifier|*
name|cutoff_cnt
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Check if a particular reflog exists */
end_comment

begin_function_decl
name|int
name|reflog_exists
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Delete the specified reference. If old_sha1 is non-NULL, then  * verify that the current value of the reference is old_sha1 before  * deleting it. If old_sha1 is NULL, delete the reference if it  * exists, regardless of its old value. It is an error for old_sha1 to  * be NULL_SHA1. flags is passed through to ref_transaction_delete().  */
end_comment

begin_function_decl
name|int
name|delete_ref
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
name|unsigned
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Delete the specified references. If there are any problems, emit  * errors but attempt to keep going (i.e., the deletes are not done in  * an all-or-nothing transaction).  */
end_comment

begin_function_decl
name|int
name|delete_refs
parameter_list|(
name|struct
name|string_list
modifier|*
name|refnames
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Delete a reflog */
end_comment

begin_function_decl
name|int
name|delete_reflog
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* iterate over reflog entries */
end_comment

begin_typedef
DECL|typedef|each_reflog_ent_fn
typedef|typedef
name|int
name|each_reflog_ent_fn
parameter_list|(
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|committer
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|,
name|int
name|tz
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|int
name|for_each_reflog_ent
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|each_reflog_ent_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|for_each_reflog_ent_reverse
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|each_reflog_ent_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Calls the specified function for each reflog file until it returns nonzero,  * and returns the value  */
end_comment

begin_function_decl
name|int
name|for_each_reflog
parameter_list|(
name|each_ref_fn
name|fn
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|REFNAME_ALLOW_ONELEVEL
define|#
directive|define
name|REFNAME_ALLOW_ONELEVEL
value|1
end_define

begin_define
DECL|macro|REFNAME_REFSPEC_PATTERN
define|#
directive|define
name|REFNAME_REFSPEC_PATTERN
value|2
end_define

begin_comment
comment|/*  * Return 0 iff refname has the correct format for a refname according  * to the rules described in Documentation/git-check-ref-format.txt.  * If REFNAME_ALLOW_ONELEVEL is set in flags, then accept one-level  * reference names.  If REFNAME_REFSPEC_PATTERN is set in flags, then  * allow a single "*" wildcard character in the refspec. No leading or  * repeated slashes are accepted.  */
end_comment

begin_function_decl
name|int
name|check_refname_format
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|const
name|char
modifier|*
name|prettify_refname
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|char
modifier|*
name|shorten_unambiguous_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
name|int
name|strict
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** rename ref, return 0 on success **/
end_comment

begin_function_decl
name|int
name|rename_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|oldref
parameter_list|,
specifier|const
name|char
modifier|*
name|newref
parameter_list|,
specifier|const
name|char
modifier|*
name|logmsg
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|create_symref
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|,
specifier|const
name|char
modifier|*
name|target
parameter_list|,
specifier|const
name|char
modifier|*
name|logmsg
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Update HEAD of the specified gitdir.  * Similar to create_symref("relative-git-dir/HEAD", target, NULL), but  * this can update the main working tree's HEAD regardless of where  * $GIT_DIR points to.  * Return 0 if successful, non-zero otherwise.  * */
end_comment

begin_function_decl
name|int
name|set_worktree_head_symref
parameter_list|(
specifier|const
name|char
modifier|*
name|gitdir
parameter_list|,
specifier|const
name|char
modifier|*
name|target
parameter_list|)
function_decl|;
end_function_decl

begin_enum
DECL|enum|action_on_err
enum|enum
name|action_on_err
block|{
DECL|enumerator|UPDATE_REFS_MSG_ON_ERR
name|UPDATE_REFS_MSG_ON_ERR
block|,
DECL|enumerator|UPDATE_REFS_DIE_ON_ERR
name|UPDATE_REFS_DIE_ON_ERR
block|,
DECL|enumerator|UPDATE_REFS_QUIET_ON_ERR
name|UPDATE_REFS_QUIET_ON_ERR
block|}
enum|;
end_enum

begin_comment
comment|/*  * Begin a reference transaction.  The reference transaction must  * be freed by calling ref_transaction_free().  */
end_comment

begin_function_decl
name|struct
name|ref_transaction
modifier|*
name|ref_transaction_begin
parameter_list|(
name|struct
name|strbuf
modifier|*
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Reference transaction updates  *  * The following four functions add a reference check or update to a  * ref_transaction.  They have some common similar parameters:  *  *     transaction -- a pointer to an open ref_transaction, obtained  *         from ref_transaction_begin().  *  *     refname -- the name of the reference to be affected.  *  *     flags -- flags affecting the update, passed to  *         update_ref_lock(). Can be REF_NODEREF, which means that  *         symbolic references should not be followed.  *  *     msg -- a message describing the change (for the reflog).  *  *     err -- a strbuf for receiving a description of any error that  *         might have occured.  *  * The functions make internal copies of refname and msg, so the  * caller retains ownership of these parameters.  *  * The functions return 0 on success and non-zero on failure. A  * failure means that the transaction as a whole has failed and needs  * to be rolled back.  */
end_comment

begin_comment
comment|/*  * Add a reference update to transaction. new_sha1 is the value that  * the reference should have after the update, or null_sha1 if it  * should be deleted. If new_sha1 is NULL, then the reference is not  * changed at all. old_sha1 is the value that the reference must have  * before the update, or null_sha1 if it must not have existed  * beforehand. The old value is checked after the lock is taken to  * prevent races. If the old value doesn't agree with old_sha1, the  * whole transaction fails. If old_sha1 is NULL, then the previous  * value is not checked.  *  * See the above comment "Reference transaction updates" for more  * information.  */
end_comment

begin_function_decl
name|int
name|ref_transaction_update
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
name|unsigned
name|int
name|flags
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|,
name|struct
name|strbuf
modifier|*
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Add a reference creation to transaction. new_sha1 is the value that  * the reference should have after the update; it must not be  * null_sha1. It is verified that the reference does not exist  * already.  *  * See the above comment "Reference transaction updates" for more  * information.  */
end_comment

begin_function_decl
name|int
name|ref_transaction_create
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
specifier|const
name|unsigned
name|char
modifier|*
name|new_sha1
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|,
name|struct
name|strbuf
modifier|*
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Add a reference deletion to transaction. If old_sha1 is non-NULL,  * then it holds the value that the reference should have had before  * the update (which must not be null_sha1).  *  * See the above comment "Reference transaction updates" for more  * information.  */
end_comment

begin_function_decl
name|int
name|ref_transaction_delete
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
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|,
name|struct
name|strbuf
modifier|*
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Verify, within a transaction, that refname has the value old_sha1,  * or, if old_sha1 is null_sha1, then verify that the reference  * doesn't exist. old_sha1 must be non-NULL.  *  * See the above comment "Reference transaction updates" for more  * information.  */
end_comment

begin_function_decl
name|int
name|ref_transaction_verify
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
specifier|const
name|unsigned
name|char
modifier|*
name|old_sha1
parameter_list|,
name|unsigned
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
comment|/*  * Commit all of the changes that have been queued in transaction, as  * atomically as possible.  *  * Returns 0 for success, or one of the below error codes for errors.  */
end_comment

begin_comment
comment|/* Naming conflict (for example, the ref names A and A/B conflict). */
end_comment

begin_define
DECL|macro|TRANSACTION_NAME_CONFLICT
define|#
directive|define
name|TRANSACTION_NAME_CONFLICT
value|-1
end_define

begin_comment
comment|/* All other errors. */
end_comment

begin_define
DECL|macro|TRANSACTION_GENERIC_ERROR
define|#
directive|define
name|TRANSACTION_GENERIC_ERROR
value|-2
end_define

begin_function_decl
name|int
name|ref_transaction_commit
parameter_list|(
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
end_function_decl

begin_comment
comment|/*  * Like ref_transaction_commit(), but optimized for creating  * references when originally initializing a repository (e.g., by "git  * clone"). It writes the new references directly to packed-refs  * without locking the individual references.  *  * It is a bug to call this function when there might be other  * processes accessing the repository or if there are existing  * references that might conflict with the ones being created. All  * old_sha1 values must either be absent or NULL_SHA1.  */
end_comment

begin_function_decl
name|int
name|initial_ref_transaction_commit
parameter_list|(
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
end_function_decl

begin_comment
comment|/*  * Free an existing transaction and all associated data.  */
end_comment

begin_function_decl
name|void
name|ref_transaction_free
parameter_list|(
name|struct
name|ref_transaction
modifier|*
name|transaction
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Lock, update, and unlock a single reference. This function  * basically does a transaction containing a single call to  * ref_transaction_update(). The parameters to this function have the  * same meaning as the corresponding parameters to  * ref_transaction_update(). Handle errors as requested by the `onerr`  * argument.  */
end_comment

begin_function_decl
name|int
name|update_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|msg
parameter_list|,
specifier|const
name|char
modifier|*
name|refname
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
name|unsigned
name|int
name|flags
parameter_list|,
name|enum
name|action_on_err
name|onerr
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|parse_hide_refs_config
parameter_list|(
specifier|const
name|char
modifier|*
name|var
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Check whether a ref is hidden. If no namespace is set, both the first and  * the second parameter point to the full ref name. If a namespace is set and  * the ref is inside that namespace, the first parameter is a pointer to the  * name of the ref with the namespace prefix removed. If a namespace is set and  * the ref is outside that namespace, the first parameter is NULL. The second  * parameter always points to the full ref name.  */
end_comment

begin_function_decl
name|int
name|ref_is_hidden
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_enum
DECL|enum|ref_type
enum|enum
name|ref_type
block|{
DECL|enumerator|REF_TYPE_PER_WORKTREE
name|REF_TYPE_PER_WORKTREE
block|,
DECL|enumerator|REF_TYPE_PSEUDOREF
name|REF_TYPE_PSEUDOREF
block|,
DECL|enumerator|REF_TYPE_NORMAL
name|REF_TYPE_NORMAL
block|, }
enum|;
end_enum

begin_function_decl
name|enum
name|ref_type
name|ref_type
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
parameter_list|)
function_decl|;
end_function_decl

begin_enum
DECL|enum|expire_reflog_flags
enum|enum
name|expire_reflog_flags
block|{
DECL|enumerator|EXPIRE_REFLOGS_DRY_RUN
name|EXPIRE_REFLOGS_DRY_RUN
init|=
literal|1
operator|<<
literal|0
block|,
DECL|enumerator|EXPIRE_REFLOGS_UPDATE_REF
name|EXPIRE_REFLOGS_UPDATE_REF
init|=
literal|1
operator|<<
literal|1
block|,
DECL|enumerator|EXPIRE_REFLOGS_VERBOSE
name|EXPIRE_REFLOGS_VERBOSE
init|=
literal|1
operator|<<
literal|2
block|,
DECL|enumerator|EXPIRE_REFLOGS_REWRITE
name|EXPIRE_REFLOGS_REWRITE
init|=
literal|1
operator|<<
literal|3
block|}
enum|;
end_enum

begin_comment
comment|/*  * The following interface is used for reflog expiration. The caller  * calls reflog_expire(), supplying it with three callback functions,  * of the following types. The callback functions define the  * expiration policy that is desired.  *  * reflog_expiry_prepare_fn -- Called once after the reference is  *     locked.  *  * reflog_expiry_should_prune_fn -- Called once for each entry in the  *     existing reflog. It should return true iff that entry should be  *     pruned.  *  * reflog_expiry_cleanup_fn -- Called once before the reference is  *     unlocked again.  */
end_comment

begin_typedef
DECL|typedef|reflog_expiry_prepare_fn
typedef|typedef
name|void
name|reflog_expiry_prepare_fn
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
name|sha1
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|reflog_expiry_should_prune_fn
typedef|typedef
name|int
name|reflog_expiry_should_prune_fn
parameter_list|(
name|unsigned
name|char
modifier|*
name|osha1
parameter_list|,
name|unsigned
name|char
modifier|*
name|nsha1
parameter_list|,
specifier|const
name|char
modifier|*
name|email
parameter_list|,
name|unsigned
name|long
name|timestamp
parameter_list|,
name|int
name|tz
parameter_list|,
specifier|const
name|char
modifier|*
name|message
parameter_list|,
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|reflog_expiry_cleanup_fn
typedef|typedef
name|void
name|reflog_expiry_cleanup_fn
parameter_list|(
name|void
modifier|*
name|cb_data
parameter_list|)
function_decl|;
end_typedef

begin_comment
comment|/*  * Expire reflog entries for the specified reference. sha1 is the old  * value of the reference. flags is a combination of the constants in  * enum expire_reflog_flags. The three function pointers are described  * above. On success, return zero.  */
end_comment

begin_function_decl
name|int
name|reflog_expire
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
name|sha1
parameter_list|,
name|unsigned
name|int
name|flags
parameter_list|,
name|reflog_expiry_prepare_fn
name|prepare_fn
parameter_list|,
name|reflog_expiry_should_prune_fn
name|should_prune_fn
parameter_list|,
name|reflog_expiry_cleanup_fn
name|cleanup_fn
parameter_list|,
name|void
modifier|*
name|policy_cb_data
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* REFS_H */
end_comment

end_unit

