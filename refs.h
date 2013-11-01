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

begin_struct
DECL|struct|ref_lock
struct|struct
name|ref_lock
block|{
DECL|member|ref_name
name|char
modifier|*
name|ref_name
decl_stmt|;
DECL|member|orig_ref_name
name|char
modifier|*
name|orig_ref_name
decl_stmt|;
DECL|member|lk
name|struct
name|lock_file
modifier|*
name|lk
decl_stmt|;
DECL|member|old_sha1
name|unsigned
name|char
name|old_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|lock_fd
name|int
name|lock_fd
decl_stmt|;
DECL|member|force_write
name|int
name|force_write
decl_stmt|;
block|}
struct|;
end_struct

begin_comment
comment|/**  * Information needed for a single ref update.  Set new_sha1 to the  * new value or to zero to delete the ref.  To check the old value  * while locking the ref, set have_old to 1 and set old_sha1 to the  * value or to zero to ensure the ref does not exist before update.  */
end_comment

begin_struct
DECL|struct|ref_update
struct|struct
name|ref_update
block|{
DECL|member|ref_name
specifier|const
name|char
modifier|*
name|ref_name
decl_stmt|;
DECL|member|new_sha1
name|unsigned
name|char
name|new_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|old_sha1
name|unsigned
name|char
name|old_sha1
index|[
literal|20
index|]
decl_stmt|;
DECL|member|flags
name|int
name|flags
decl_stmt|;
comment|/* REF_NODEREF? */
DECL|member|have_old
name|int
name|have_old
decl_stmt|;
comment|/* 1 if old_sha1 is valid, 0 otherwise */
block|}
struct|;
end_struct

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
comment|/*  * Reference cannot be resolved to an object name: dangling symbolic  * reference (directly or indirectly), corrupt reference file, or  * symbolic reference refers to ill-formatted reference name.  */
end_comment

begin_define
DECL|macro|REF_ISBROKEN
define|#
directive|define
name|REF_ISBROKEN
value|0x04
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
name|unsigned
name|char
modifier|*
name|sha1
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
specifier|extern
name|int
name|head_ref
parameter_list|(
name|each_ref_fn
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|for_each_ref
parameter_list|(
name|each_ref_fn
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|for_each_ref_in
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|,
name|each_ref_fn
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|for_each_tag_ref
parameter_list|(
name|each_ref_fn
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|for_each_branch_ref
parameter_list|(
name|each_ref_fn
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|for_each_remote_ref
parameter_list|(
name|each_ref_fn
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|for_each_replace_ref
parameter_list|(
name|each_ref_fn
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|for_each_glob_ref
parameter_list|(
name|each_ref_fn
parameter_list|,
specifier|const
name|char
modifier|*
name|pattern
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|for_each_glob_ref_in
parameter_list|(
name|each_ref_fn
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
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
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
specifier|extern
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
specifier|extern
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
specifier|extern
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
specifier|extern
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
specifier|extern
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
specifier|extern
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
specifier|extern
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

begin_comment
comment|/* can be used to learn about broken ref and symref */
end_comment

begin_function_decl
specifier|extern
name|int
name|for_each_rawref
parameter_list|(
name|each_ref_fn
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
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

begin_comment
comment|/*  * Lock the packed-refs file for writing.  Flags is passed to  * hold_lock_file_for_update().  Return 0 on success.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|lock_packed_refs
parameter_list|(
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Add a reference to the in-memory packed reference cache.  This may  * only be called while the packed-refs file is locked (see  * lock_packed_refs()).  To actually write the packed-refs file, call  * commit_packed_refs().  */
end_comment

begin_function_decl
specifier|extern
name|void
name|add_packed_ref
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
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Write the current version of the packed refs cache from memory to  * disk.  The packed-refs file must already be locked for writing (see  * lock_packed_refs()).  Return zero on success.  */
end_comment

begin_function_decl
specifier|extern
name|int
name|commit_packed_refs
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Rollback the lockfile for the packed-refs file, and discard the  * in-memory packed reference cache.  (The packed-refs file will be  * read anew if it is needed again after this function is called.)  */
end_comment

begin_function_decl
specifier|extern
name|void
name|rollback_packed_refs
parameter_list|(
name|void
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

begin_function_decl
specifier|extern
name|int
name|ref_exists
parameter_list|(
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * If refname is a non-symbolic reference that refers to a tag object,  * and the tag can be (recursively) dereferenced to a non-tag object,  * store the SHA1 of the referred-to object to sha1 and return 0.  If  * any of these conditions are not met, return a non-zero value.  * Symbolic references are considered unpeelable, even if they  * ultimately resolve to a peelable tag.  */
end_comment

begin_function_decl
specifier|extern
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
comment|/** Locks a "refs/" ref returning the lock on success and NULL on failure. **/
end_comment

begin_function_decl
specifier|extern
name|struct
name|ref_lock
modifier|*
name|lock_ref_sha1
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
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Locks any ref (for 'HEAD' type refs). */
end_comment

begin_define
DECL|macro|REF_NODEREF
define|#
directive|define
name|REF_NODEREF
value|0x01
end_define

begin_function_decl
specifier|extern
name|struct
name|ref_lock
modifier|*
name|lock_any_ref_for_update
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
name|int
name|flags
parameter_list|,
name|int
modifier|*
name|type_p
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Close the file descriptor owned by a lock and return the status */
end_comment

begin_function_decl
specifier|extern
name|int
name|close_ref
parameter_list|(
name|struct
name|ref_lock
modifier|*
name|lock
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Close and commit the ref locked by the lock */
end_comment

begin_function_decl
specifier|extern
name|int
name|commit_ref
parameter_list|(
name|struct
name|ref_lock
modifier|*
name|lock
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Release any lock taken but not written. **/
end_comment

begin_function_decl
specifier|extern
name|void
name|unlock_ref
parameter_list|(
name|struct
name|ref_lock
modifier|*
name|lock
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Writes sha1 into the ref specified by the lock. **/
end_comment

begin_function_decl
specifier|extern
name|int
name|write_ref_sha1
parameter_list|(
name|struct
name|ref_lock
modifier|*
name|lock
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|sha1
parameter_list|,
specifier|const
name|char
modifier|*
name|msg
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Setup reflog before using. **/
end_comment

begin_function_decl
name|int
name|log_ref_setup
parameter_list|(
specifier|const
name|char
modifier|*
name|ref_name
parameter_list|,
name|char
modifier|*
name|logfile
parameter_list|,
name|int
name|bufsize
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/** Reads log for the value of ref during at_time. **/
end_comment

begin_function_decl
specifier|extern
name|int
name|read_ref_at
parameter_list|(
specifier|const
name|char
modifier|*
name|refname
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
parameter_list|,
name|unsigned
name|long
parameter_list|,
name|int
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|,
name|void
modifier|*
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
specifier|extern
name|int
name|for_each_reflog
parameter_list|(
name|each_ref_fn
parameter_list|,
name|void
modifier|*
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

begin_define
DECL|macro|REFNAME_DOT_COMPONENT
define|#
directive|define
name|REFNAME_DOT_COMPONENT
value|4
end_define

begin_comment
comment|/*  * Return 0 iff refname has the correct format for a refname according  * to the rules described in Documentation/git-check-ref-format.txt.  * If REFNAME_ALLOW_ONELEVEL is set in flags, then accept one-level  * reference names.  If REFNAME_REFSPEC_PATTERN is set in flags, then  * allow a "*" wildcard character in place of one of the name  * components.  No leading or repeated slashes are accepted.  If  * REFNAME_DOT_COMPONENT is set in flags, then allow refname  * components to start with "." (but not a whole component equal to  * "." or "..").  */
end_comment

begin_function_decl
specifier|extern
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
specifier|extern
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
specifier|extern
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
specifier|extern
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

begin_comment
comment|/**  * Resolve refname in the nested "gitlink" repository that is located  * at path.  If the resolution is successful, return 0 and set sha1 to  * the name of the object; otherwise, return a non-zero value.  */
end_comment

begin_function_decl
specifier|extern
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
comment|/** lock a ref and then write its file */
end_comment

begin_enum
DECL|enum|action_on_err
DECL|enumerator|MSG_ON_ERR
DECL|enumerator|DIE_ON_ERR
DECL|enumerator|QUIET_ON_ERR
enum|enum
name|action_on_err
block|{
name|MSG_ON_ERR
block|,
name|DIE_ON_ERR
block|,
name|QUIET_ON_ERR
block|}
enum|;
end_enum

begin_function_decl
name|int
name|update_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|action
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
name|sha1
parameter_list|,
specifier|const
name|unsigned
name|char
modifier|*
name|oldval
parameter_list|,
name|int
name|flags
parameter_list|,
name|enum
name|action_on_err
name|onerr
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**  * Lock all refs and then perform all modifications.  */
end_comment

begin_function_decl
name|int
name|update_refs
parameter_list|(
specifier|const
name|char
modifier|*
name|action
parameter_list|,
specifier|const
name|struct
name|ref_update
modifier|*
modifier|*
name|updates
parameter_list|,
name|int
name|n
parameter_list|,
name|enum
name|action_on_err
name|onerr
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
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

begin_function_decl
specifier|extern
name|int
name|ref_is_hidden
parameter_list|(
specifier|const
name|char
modifier|*
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

