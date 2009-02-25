begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|REMOTE_H
end_ifndef

begin_define
DECL|macro|REMOTE_H
define|#
directive|define
name|REMOTE_H
end_define

begin_enum
enum|enum
block|{
DECL|enumerator|REMOTE_CONFIG
name|REMOTE_CONFIG
block|,
DECL|enumerator|REMOTE_REMOTES
name|REMOTE_REMOTES
block|,
DECL|enumerator|REMOTE_BRANCHES
name|REMOTE_BRANCHES
block|}
enum|;
end_enum

begin_struct
DECL|struct|remote
struct|struct
name|remote
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|origin
name|int
name|origin
decl_stmt|;
DECL|member|url
specifier|const
name|char
modifier|*
modifier|*
name|url
decl_stmt|;
DECL|member|url_nr
name|int
name|url_nr
decl_stmt|;
DECL|member|url_alloc
name|int
name|url_alloc
decl_stmt|;
DECL|member|push_refspec
specifier|const
name|char
modifier|*
modifier|*
name|push_refspec
decl_stmt|;
DECL|member|push
name|struct
name|refspec
modifier|*
name|push
decl_stmt|;
DECL|member|push_refspec_nr
name|int
name|push_refspec_nr
decl_stmt|;
DECL|member|push_refspec_alloc
name|int
name|push_refspec_alloc
decl_stmt|;
DECL|member|fetch_refspec
specifier|const
name|char
modifier|*
modifier|*
name|fetch_refspec
decl_stmt|;
DECL|member|fetch
name|struct
name|refspec
modifier|*
name|fetch
decl_stmt|;
DECL|member|fetch_refspec_nr
name|int
name|fetch_refspec_nr
decl_stmt|;
DECL|member|fetch_refspec_alloc
name|int
name|fetch_refspec_alloc
decl_stmt|;
comment|/* 	 * -1 to never fetch tags 	 * 0 to auto-follow tags on heuristic (default) 	 * 1 to always auto-follow tags 	 * 2 to always fetch tags 	 */
DECL|member|fetch_tags
name|int
name|fetch_tags
decl_stmt|;
DECL|member|skip_default_update
name|int
name|skip_default_update
decl_stmt|;
DECL|member|mirror
name|int
name|mirror
decl_stmt|;
DECL|member|receivepack
specifier|const
name|char
modifier|*
name|receivepack
decl_stmt|;
DECL|member|uploadpack
specifier|const
name|char
modifier|*
name|uploadpack
decl_stmt|;
comment|/* 	 * for curl remotes only 	 */
DECL|member|http_proxy
name|char
modifier|*
name|http_proxy
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|remote
modifier|*
name|remote_get
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_typedef
DECL|typedef|each_remote_fn
typedef|typedef
name|int
name|each_remote_fn
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
name|void
modifier|*
name|priv
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|int
name|for_each_remote
parameter_list|(
name|each_remote_fn
name|fn
parameter_list|,
name|void
modifier|*
name|priv
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|remote_has_url
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
specifier|const
name|char
modifier|*
name|url
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|refspec
struct|struct
name|refspec
block|{
DECL|member|force
name|unsigned
name|force
range|:
literal|1
decl_stmt|;
DECL|member|pattern
name|unsigned
name|pattern
range|:
literal|1
decl_stmt|;
DECL|member|matching
name|unsigned
name|matching
range|:
literal|1
decl_stmt|;
DECL|member|src
name|char
modifier|*
name|src
decl_stmt|;
DECL|member|dst
name|char
modifier|*
name|dst
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
specifier|extern
specifier|const
name|struct
name|refspec
modifier|*
name|tag_refspec
decl_stmt|;
end_decl_stmt

begin_function_decl
name|struct
name|ref
modifier|*
name|alloc_ref
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|ref
modifier|*
name|copy_ref_list
parameter_list|(
specifier|const
name|struct
name|ref
modifier|*
name|ref
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|check_ref_type
parameter_list|(
specifier|const
name|struct
name|ref
modifier|*
name|ref
parameter_list|,
name|int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Frees the entire list and peers of elements.  */
end_comment

begin_function_decl
name|void
name|free_refs
parameter_list|(
name|struct
name|ref
modifier|*
name|ref
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|resolve_remote_symref
parameter_list|(
name|struct
name|ref
modifier|*
name|ref
parameter_list|,
name|struct
name|ref
modifier|*
name|list
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ref_newer
parameter_list|(
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
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Removes and frees any duplicate refs in the map.  */
end_comment

begin_function_decl
name|void
name|ref_remove_duplicates
parameter_list|(
name|struct
name|ref
modifier|*
name|ref_map
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|valid_fetch_refspec
parameter_list|(
specifier|const
name|char
modifier|*
name|refspec
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|refspec
modifier|*
name|parse_fetch_refspec
parameter_list|(
name|int
name|nr_refspec
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspec
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|match_refs
parameter_list|(
name|struct
name|ref
modifier|*
name|src
parameter_list|,
name|struct
name|ref
modifier|*
name|dst
parameter_list|,
name|struct
name|ref
modifier|*
modifier|*
modifier|*
name|dst_tail
parameter_list|,
name|int
name|nr_refspec
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|refspec
parameter_list|,
name|int
name|all
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Given a list of the remote refs and the specification of things to  * fetch, makes a (separate) list of the refs to fetch and the local  * refs to store into.  *  * *tail is the pointer to the tail pointer of the list of results  * beforehand, and will be set to the tail pointer of the list of  * results afterward.  *  * missing_ok is usually false, but when we are adding branch.$name.merge  * it is Ok if the branch is not at the remote anymore.  */
end_comment

begin_function_decl
name|int
name|get_fetch_map
parameter_list|(
specifier|const
name|struct
name|ref
modifier|*
name|remote_refs
parameter_list|,
specifier|const
name|struct
name|refspec
modifier|*
name|refspec
parameter_list|,
name|struct
name|ref
modifier|*
modifier|*
modifier|*
name|tail
parameter_list|,
name|int
name|missing_ok
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|ref
modifier|*
name|get_remote_ref
parameter_list|(
specifier|const
name|struct
name|ref
modifier|*
name|remote_refs
parameter_list|,
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * For the given remote, reads the refspec's src and sets the other fields.  */
end_comment

begin_function_decl
name|int
name|remote_find_tracking
parameter_list|(
name|struct
name|remote
modifier|*
name|remote
parameter_list|,
name|struct
name|refspec
modifier|*
name|refspec
parameter_list|)
function_decl|;
end_function_decl

begin_struct
DECL|struct|branch
struct|struct
name|branch
block|{
DECL|member|name
specifier|const
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|refname
specifier|const
name|char
modifier|*
name|refname
decl_stmt|;
DECL|member|remote_name
specifier|const
name|char
modifier|*
name|remote_name
decl_stmt|;
DECL|member|remote
name|struct
name|remote
modifier|*
name|remote
decl_stmt|;
DECL|member|merge_name
specifier|const
name|char
modifier|*
modifier|*
name|merge_name
decl_stmt|;
DECL|member|merge
name|struct
name|refspec
modifier|*
modifier|*
name|merge
decl_stmt|;
DECL|member|merge_nr
name|int
name|merge_nr
decl_stmt|;
DECL|member|merge_alloc
name|int
name|merge_alloc
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|struct
name|branch
modifier|*
name|branch_get
parameter_list|(
specifier|const
name|char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|branch_has_merge_config
parameter_list|(
name|struct
name|branch
modifier|*
name|branch
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|branch_merge_matches
parameter_list|(
name|struct
name|branch
modifier|*
parameter_list|,
name|int
name|n
parameter_list|,
specifier|const
name|char
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* Flags to match_refs. */
end_comment

begin_enum
DECL|enum|match_refs_flags
enum|enum
name|match_refs_flags
block|{
DECL|enumerator|MATCH_REFS_NONE
name|MATCH_REFS_NONE
init|=
literal|0
block|,
DECL|enumerator|MATCH_REFS_ALL
name|MATCH_REFS_ALL
init|=
operator|(
literal|1
operator|<<
literal|0
operator|)
block|,
DECL|enumerator|MATCH_REFS_MIRROR
name|MATCH_REFS_MIRROR
init|=
operator|(
literal|1
operator|<<
literal|1
operator|)
block|, }
enum|;
end_enum

begin_comment
comment|/* Reporting of tracking info */
end_comment

begin_function_decl
name|int
name|stat_tracking_info
parameter_list|(
name|struct
name|branch
modifier|*
name|branch
parameter_list|,
name|int
modifier|*
name|num_ours
parameter_list|,
name|int
modifier|*
name|num_theirs
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|format_tracking_info
parameter_list|(
name|struct
name|branch
modifier|*
name|branch
parameter_list|,
name|struct
name|strbuf
modifier|*
name|sb
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|struct
name|ref
modifier|*
name|get_local_heads
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * Look in refs for HEAD. Then look for a matching SHA1 in mapped_refs,  * first checking if refs/heads/master matches. Return NULL if nothing matches  * or if there is no HEAD in refs. remote_head_p is assigned HEAD if not NULL.  */
end_comment

begin_function_decl
specifier|const
name|struct
name|ref
modifier|*
name|guess_remote_head
parameter_list|(
specifier|const
name|struct
name|ref
modifier|*
name|refs
parameter_list|,
specifier|const
name|struct
name|ref
modifier|*
name|mapped_refs
parameter_list|,
specifier|const
name|struct
name|ref
modifier|*
modifier|*
name|remote_head_p
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

end_unit

