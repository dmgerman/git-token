begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|TRANSPORT_H
end_ifndef

begin_define
DECL|macro|TRANSPORT_H
define|#
directive|define
name|TRANSPORT_H
end_define

begin_include
include|#
directive|include
file|"cache.h"
end_include

begin_include
include|#
directive|include
file|"run-command.h"
end_include

begin_include
include|#
directive|include
file|"remote.h"
end_include

begin_struct_decl
struct_decl|struct
name|string_list
struct_decl|;
end_struct_decl

begin_struct
DECL|struct|git_transport_options
struct|struct
name|git_transport_options
block|{
DECL|member|thin
name|unsigned
name|thin
range|:
literal|1
decl_stmt|;
DECL|member|keep
name|unsigned
name|keep
range|:
literal|1
decl_stmt|;
DECL|member|followtags
name|unsigned
name|followtags
range|:
literal|1
decl_stmt|;
DECL|member|check_self_contained_and_connected
name|unsigned
name|check_self_contained_and_connected
range|:
literal|1
decl_stmt|;
DECL|member|self_contained_and_connected
name|unsigned
name|self_contained_and_connected
range|:
literal|1
decl_stmt|;
DECL|member|update_shallow
name|unsigned
name|update_shallow
range|:
literal|1
decl_stmt|;
DECL|member|deepen_relative
name|unsigned
name|deepen_relative
range|:
literal|1
decl_stmt|;
DECL|member|depth
name|int
name|depth
decl_stmt|;
DECL|member|deepen_since
specifier|const
name|char
modifier|*
name|deepen_since
decl_stmt|;
DECL|member|deepen_not
specifier|const
name|struct
name|string_list
modifier|*
name|deepen_not
decl_stmt|;
DECL|member|uploadpack
specifier|const
name|char
modifier|*
name|uploadpack
decl_stmt|;
DECL|member|receivepack
specifier|const
name|char
modifier|*
name|receivepack
decl_stmt|;
DECL|member|cas
name|struct
name|push_cas_option
modifier|*
name|cas
decl_stmt|;
block|}
struct|;
end_struct

begin_enum
DECL|enum|transport_family
enum|enum
name|transport_family
block|{
DECL|enumerator|TRANSPORT_FAMILY_ALL
name|TRANSPORT_FAMILY_ALL
init|=
literal|0
block|,
DECL|enumerator|TRANSPORT_FAMILY_IPV4
name|TRANSPORT_FAMILY_IPV4
block|,
DECL|enumerator|TRANSPORT_FAMILY_IPV6
name|TRANSPORT_FAMILY_IPV6
block|}
enum|;
end_enum

begin_struct
DECL|struct|transport
struct|struct
name|transport
block|{
DECL|member|remote
name|struct
name|remote
modifier|*
name|remote
decl_stmt|;
DECL|member|url
specifier|const
name|char
modifier|*
name|url
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|remote_refs
specifier|const
name|struct
name|ref
modifier|*
name|remote_refs
decl_stmt|;
comment|/** 	 * Indicates whether we already called get_refs_list(); set by 	 * transport.c::transport_get_remote_refs(). 	 */
DECL|member|got_remote_refs
name|unsigned
name|got_remote_refs
range|:
literal|1
decl_stmt|;
comment|/* 	 * Transports that call take-over destroys the data specific to 	 * the transport type while doing so, and cannot be reused. 	 */
DECL|member|cannot_reuse
name|unsigned
name|cannot_reuse
range|:
literal|1
decl_stmt|;
comment|/* 	 * A hint from caller that it will be performing a clone, not 	 * normal fetch. IOW the repository is guaranteed empty. 	 */
DECL|member|cloning
name|unsigned
name|cloning
range|:
literal|1
decl_stmt|;
comment|/* 	 * These strings will be passed to the {pre, post}-receive hook, 	 * on the remote side, if both sides support the push options capability. 	 */
DECL|member|push_options
specifier|const
name|struct
name|string_list
modifier|*
name|push_options
decl_stmt|;
comment|/** 	 * Returns 0 if successful, positive if the option is not 	 * recognized or is inapplicable, and negative if the option 	 * is applicable but the value is invalid. 	 **/
DECL|member|set_option
name|int
function_decl|(
modifier|*
name|set_option
function_decl|)
parameter_list|(
name|struct
name|transport
modifier|*
name|connection
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
function_decl|;
comment|/** 	 * Returns a list of the remote side's refs. In order to allow 	 * the transport to try to share connections, for_push is a 	 * hint as to whether the ultimate operation is a push or a fetch. 	 * 	 * If the transport is able to determine the remote hash for 	 * the ref without a huge amount of effort, it should store it 	 * in the ref's old_sha1 field; otherwise it should be all 0. 	 **/
DECL|member|get_refs_list
name|struct
name|ref
modifier|*
function_decl|(
modifier|*
name|get_refs_list
function_decl|)
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|for_push
parameter_list|)
function_decl|;
comment|/** 	 * Fetch the objects for the given refs. Note that this gets 	 * an array, and should ignore the list structure. 	 * 	 * If the transport did not get hashes for refs in 	 * get_refs_list(), it should set the old_sha1 fields in the 	 * provided refs now. 	 **/
DECL|member|fetch
name|int
function_decl|(
modifier|*
name|fetch
function_decl|)
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|refs_nr
parameter_list|,
name|struct
name|ref
modifier|*
modifier|*
name|refs
parameter_list|)
function_decl|;
comment|/** 	 * Push the objects and refs. Send the necessary objects, and 	 * then, for any refs where peer_ref is set and 	 * peer_ref->new_oid is different from old_oid, tell the 	 * remote side to update each ref in the list from old_oid to 	 * peer_ref->new_oid. 	 * 	 * Where possible, set the status for each ref appropriately. 	 * 	 * The transport must modify new_sha1 in the ref to the new 	 * value if the remote accepted the change. Note that this 	 * could be a different value from peer_ref->new_oid if the 	 * process involved generating new commits. 	 **/
DECL|member|push_refs
name|int
function_decl|(
modifier|*
name|push_refs
function_decl|)
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|struct
name|ref
modifier|*
name|refs
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
DECL|member|push
name|int
function_decl|(
modifier|*
name|push
function_decl|)
parameter_list|(
name|struct
name|transport
modifier|*
name|connection
parameter_list|,
name|int
name|refspec_nr
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspec
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
DECL|member|connect
name|int
function_decl|(
modifier|*
name|connect
function_decl|)
parameter_list|(
name|struct
name|transport
modifier|*
name|connection
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|executable
parameter_list|,
name|int
name|fd
index|[
literal|2
index|]
parameter_list|)
function_decl|;
comment|/** get_refs_list(), fetch(), and push_refs() can keep 	 * resources (such as a connection) reserved for further 	 * use. disconnect() releases these resources. 	 **/
DECL|member|disconnect
name|int
function_decl|(
modifier|*
name|disconnect
function_decl|)
parameter_list|(
name|struct
name|transport
modifier|*
name|connection
parameter_list|)
function_decl|;
DECL|member|pack_lockfile
name|char
modifier|*
name|pack_lockfile
decl_stmt|;
DECL|member|verbose
name|signed
name|verbose
range|:
literal|3
decl_stmt|;
comment|/** 	 * Transports should not set this directly, and should use this 	 * value without having to check isatty(2), -q/--quiet 	 * (transport->verbose< 0), etc. - checking has already been done 	 * in transport_set_verbosity(). 	 **/
DECL|member|progress
name|unsigned
name|progress
range|:
literal|1
decl_stmt|;
comment|/* 	 * If transport is at least potentially smart, this points to 	 * git_transport_options structure to use in case transport 	 * actually turns out to be smart. 	 */
DECL|member|smart_options
name|struct
name|git_transport_options
modifier|*
name|smart_options
decl_stmt|;
DECL|member|family
name|enum
name|transport_family
name|family
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|TRANSPORT_PUSH_ALL
define|#
directive|define
name|TRANSPORT_PUSH_ALL
value|1
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_FORCE
define|#
directive|define
name|TRANSPORT_PUSH_FORCE
value|2
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_DRY_RUN
define|#
directive|define
name|TRANSPORT_PUSH_DRY_RUN
value|4
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_MIRROR
define|#
directive|define
name|TRANSPORT_PUSH_MIRROR
value|8
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_PORCELAIN
define|#
directive|define
name|TRANSPORT_PUSH_PORCELAIN
value|16
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_SET_UPSTREAM
define|#
directive|define
name|TRANSPORT_PUSH_SET_UPSTREAM
value|32
end_define

begin_define
DECL|macro|TRANSPORT_RECURSE_SUBMODULES_CHECK
define|#
directive|define
name|TRANSPORT_RECURSE_SUBMODULES_CHECK
value|64
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_PRUNE
define|#
directive|define
name|TRANSPORT_PUSH_PRUNE
value|128
end_define

begin_define
DECL|macro|TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND
define|#
directive|define
name|TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND
value|256
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_NO_HOOK
define|#
directive|define
name|TRANSPORT_PUSH_NO_HOOK
value|512
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_FOLLOW_TAGS
define|#
directive|define
name|TRANSPORT_PUSH_FOLLOW_TAGS
value|1024
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_CERT_ALWAYS
define|#
directive|define
name|TRANSPORT_PUSH_CERT_ALWAYS
value|2048
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_CERT_IF_ASKED
define|#
directive|define
name|TRANSPORT_PUSH_CERT_IF_ASKED
value|4096
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_ATOMIC
define|#
directive|define
name|TRANSPORT_PUSH_ATOMIC
value|8192
end_define

begin_define
DECL|macro|TRANSPORT_PUSH_OPTIONS
define|#
directive|define
name|TRANSPORT_PUSH_OPTIONS
value|16384
end_define

begin_define
DECL|macro|TRANSPORT_SUMMARY_WIDTH
define|#
directive|define
name|TRANSPORT_SUMMARY_WIDTH
value|(2 * DEFAULT_ABBREV + 3)
end_define

begin_define
DECL|macro|TRANSPORT_SUMMARY
define|#
directive|define
name|TRANSPORT_SUMMARY
parameter_list|(
name|x
parameter_list|)
value|(int)(TRANSPORT_SUMMARY_WIDTH + strlen(x) - gettext_width(x)), (x)
end_define

begin_comment
comment|/* Returns a transport suitable for the url */
end_comment

begin_function_decl
name|struct
name|transport
modifier|*
name|transport_get
parameter_list|(
name|struct
name|remote
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Check whether a transport is allowed by the environment. Type should  * generally be the URL scheme, as described in Documentation/git.txt  */
end_comment

begin_function_decl
name|int
name|is_transport_allowed
parameter_list|(
specifier|const
name|char
modifier|*
name|type
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Check whether a transport is allowed by the environment,  * and die otherwise.  */
end_comment

begin_function_decl
name|void
name|transport_check_allowed
parameter_list|(
specifier|const
name|char
modifier|*
name|type
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Returns true if the user has attempted to turn on protocol  * restrictions at all.  */
end_comment

begin_function_decl
name|int
name|transport_restrict_protocols
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Transport options which apply to git:// and scp-style URLs */
end_comment

begin_comment
comment|/* The program to use on the remote side to send a pack */
end_comment

begin_define
DECL|macro|TRANS_OPT_UPLOADPACK
define|#
directive|define
name|TRANS_OPT_UPLOADPACK
value|"uploadpack"
end_define

begin_comment
comment|/* The program to use on the remote side to receive a pack */
end_comment

begin_define
DECL|macro|TRANS_OPT_RECEIVEPACK
define|#
directive|define
name|TRANS_OPT_RECEIVEPACK
value|"receivepack"
end_define

begin_comment
comment|/* Transfer the data as a thin pack if not null */
end_comment

begin_define
DECL|macro|TRANS_OPT_THIN
define|#
directive|define
name|TRANS_OPT_THIN
value|"thin"
end_define

begin_comment
comment|/* Check the current value of the remote ref */
end_comment

begin_define
DECL|macro|TRANS_OPT_CAS
define|#
directive|define
name|TRANS_OPT_CAS
value|"cas"
end_define

begin_comment
comment|/* Keep the pack that was transferred if not null */
end_comment

begin_define
DECL|macro|TRANS_OPT_KEEP
define|#
directive|define
name|TRANS_OPT_KEEP
value|"keep"
end_define

begin_comment
comment|/* Limit the depth of the fetch if not null */
end_comment

begin_define
DECL|macro|TRANS_OPT_DEPTH
define|#
directive|define
name|TRANS_OPT_DEPTH
value|"depth"
end_define

begin_comment
comment|/* Limit the depth of the fetch based on time if not null */
end_comment

begin_define
DECL|macro|TRANS_OPT_DEEPEN_SINCE
define|#
directive|define
name|TRANS_OPT_DEEPEN_SINCE
value|"deepen-since"
end_define

begin_comment
comment|/* Limit the depth of the fetch based on revs if not null */
end_comment

begin_define
DECL|macro|TRANS_OPT_DEEPEN_NOT
define|#
directive|define
name|TRANS_OPT_DEEPEN_NOT
value|"deepen-not"
end_define

begin_comment
comment|/* Limit the deepen of the fetch if not null */
end_comment

begin_define
DECL|macro|TRANS_OPT_DEEPEN_RELATIVE
define|#
directive|define
name|TRANS_OPT_DEEPEN_RELATIVE
value|"deepen-relative"
end_define

begin_comment
comment|/* Aggressively fetch annotated tags if possible */
end_comment

begin_define
DECL|macro|TRANS_OPT_FOLLOWTAGS
define|#
directive|define
name|TRANS_OPT_FOLLOWTAGS
value|"followtags"
end_define

begin_comment
comment|/* Accept refs that may update .git/shallow without --depth */
end_comment

begin_define
DECL|macro|TRANS_OPT_UPDATE_SHALLOW
define|#
directive|define
name|TRANS_OPT_UPDATE_SHALLOW
value|"updateshallow"
end_define

begin_comment
comment|/* Send push certificates */
end_comment

begin_define
DECL|macro|TRANS_OPT_PUSH_CERT
define|#
directive|define
name|TRANS_OPT_PUSH_CERT
value|"pushcert"
end_define

begin_comment
comment|/**  * Returns 0 if the option was used, non-zero otherwise. Prints a  * message to stderr if the option is not used.  **/
end_comment

begin_function_decl
name|int
name|transport_set_option
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|transport_set_verbosity
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|int
name|verbosity
parameter_list|,
name|int
name|force_progress
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|REJECT_NON_FF_HEAD
define|#
directive|define
name|REJECT_NON_FF_HEAD
value|0x01
end_define

begin_define
DECL|macro|REJECT_NON_FF_OTHER
define|#
directive|define
name|REJECT_NON_FF_OTHER
value|0x02
end_define

begin_define
DECL|macro|REJECT_ALREADY_EXISTS
define|#
directive|define
name|REJECT_ALREADY_EXISTS
value|0x04
end_define

begin_define
DECL|macro|REJECT_FETCH_FIRST
define|#
directive|define
name|REJECT_FETCH_FIRST
value|0x08
end_define

begin_define
DECL|macro|REJECT_NEEDS_FORCE
define|#
directive|define
name|REJECT_NEEDS_FORCE
value|0x10
end_define

begin_function_decl
name|int
name|transport_push
parameter_list|(
name|struct
name|transport
modifier|*
name|connection
parameter_list|,
name|int
name|refspec_nr
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspec
parameter_list|,
name|int
name|flags
parameter_list|,
name|unsigned
name|int
modifier|*
name|reject_reasons
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|const
name|struct
name|ref
modifier|*
name|transport_get_remote_refs
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|transport_fetch_refs
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|struct
name|ref
modifier|*
name|refs
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|transport_unlock_pack
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|transport_disconnect
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|char
modifier|*
name|transport_anonymize_url
parameter_list|(
specifier|const
name|char
modifier|*
name|url
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|transport_take_over
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
name|struct
name|child_process
modifier|*
name|child
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|transport_connect
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|,
specifier|const
name|char
modifier|*
name|exec
parameter_list|,
name|int
name|fd
index|[
literal|2
index|]
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Transport methods defined outside transport.c */
end_comment

begin_function_decl
name|int
name|transport_helper_init
parameter_list|(
name|struct
name|transport
modifier|*
name|transport
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|bidirectional_transfer_loop
parameter_list|(
name|int
name|input
parameter_list|,
name|int
name|output
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* common methods used by transport.c and builtin/send-pack.c */
end_comment

begin_function_decl
name|void
name|transport_verify_remote_names
parameter_list|(
name|int
name|nr_heads
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|heads
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|transport_update_tracking_ref
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
name|struct
name|ref
modifier|*
name|ref
parameter_list|,
name|int
name|verbose
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|transport_refs_pushed
parameter_list|(
name|struct
name|ref
modifier|*
name|ref
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|transport_print_push_status
parameter_list|(
specifier|const
name|char
modifier|*
name|dest
parameter_list|,
name|struct
name|ref
modifier|*
name|refs
parameter_list|,
name|int
name|verbose
parameter_list|,
name|int
name|porcelain
parameter_list|,
name|unsigned
name|int
modifier|*
name|reject_reasons
parameter_list|)
function_decl|;
end_function_decl

begin_typedef
DECL|typedef|alternate_ref_fn
typedef|typedef
name|void
name|alternate_ref_fn
parameter_list|(
specifier|const
name|struct
name|ref
modifier|*
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
specifier|extern
name|void
name|for_each_alternate_ref
parameter_list|(
name|alternate_ref_fn
parameter_list|,
name|void
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

