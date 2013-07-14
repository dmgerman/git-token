begin_unit|revision:0.9.5;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|PATHSPEC_H
end_ifndef

begin_define
DECL|macro|PATHSPEC_H
define|#
directive|define
name|PATHSPEC_H
end_define

begin_comment
comment|/* Pathspec magic */
end_comment

begin_define
DECL|macro|PATHSPEC_FROMTOP
define|#
directive|define
name|PATHSPEC_FROMTOP
value|(1<<0)
end_define

begin_define
DECL|macro|PATHSPEC_MAXDEPTH
define|#
directive|define
name|PATHSPEC_MAXDEPTH
value|(1<<1)
end_define

begin_define
DECL|macro|PATHSPEC_ALL_MAGIC
define|#
directive|define
name|PATHSPEC_ALL_MAGIC
define|\
value|(PATHSPEC_FROMTOP	| \ 	 PATHSPEC_MAXDEPTH)
end_define

begin_define
DECL|macro|PATHSPEC_ONESTAR
define|#
directive|define
name|PATHSPEC_ONESTAR
value|1
end_define

begin_comment
DECL|macro|PATHSPEC_ONESTAR
comment|/* the pathspec pattern sastisfies GFNM_ONESTAR */
end_comment

begin_struct
DECL|struct|pathspec
struct|struct
name|pathspec
block|{
DECL|member|raw
specifier|const
name|char
modifier|*
modifier|*
name|raw
decl_stmt|;
comment|/* get_pathspec() result, not freed by free_pathspec() */
DECL|member|nr
name|int
name|nr
decl_stmt|;
DECL|member|has_wildcard
name|unsigned
name|int
name|has_wildcard
range|:
literal|1
decl_stmt|;
DECL|member|recursive
name|unsigned
name|int
name|recursive
range|:
literal|1
decl_stmt|;
DECL|member|magic
name|unsigned
name|magic
decl_stmt|;
DECL|member|max_depth
name|int
name|max_depth
decl_stmt|;
DECL|struct|pathspec_item
struct|struct
name|pathspec_item
block|{
DECL|member|match
specifier|const
name|char
modifier|*
name|match
decl_stmt|;
DECL|member|original
specifier|const
name|char
modifier|*
name|original
decl_stmt|;
DECL|member|magic
name|unsigned
name|magic
decl_stmt|;
DECL|member|len
name|int
name|len
decl_stmt|;
DECL|member|nowildcard_len
name|int
name|nowildcard_len
decl_stmt|;
DECL|member|flags
name|int
name|flags
decl_stmt|;
DECL|member|items
block|}
modifier|*
name|items
struct|;
block|}
struct|;
end_struct

begin_define
DECL|macro|GUARD_PATHSPEC
define|#
directive|define
name|GUARD_PATHSPEC
parameter_list|(
name|ps
parameter_list|,
name|mask
parameter_list|)
define|\
value|do { \ 		if ((ps)->magic& ~(mask))	       \ 			die("BUG:%s:%d: unsupported magic %x",	\ 			    __FILE__, __LINE__, (ps)->magic& ~(mask)); \ 	} while (0)
end_define

begin_comment
comment|/* parse_pathspec flags */
end_comment

begin_define
DECL|macro|PATHSPEC_PREFER_CWD
define|#
directive|define
name|PATHSPEC_PREFER_CWD
value|(1<<0)
end_define

begin_comment
DECL|macro|PATHSPEC_PREFER_CWD
comment|/* No args means match cwd */
end_comment

begin_define
DECL|macro|PATHSPEC_PREFER_FULL
define|#
directive|define
name|PATHSPEC_PREFER_FULL
value|(1<<1)
end_define

begin_comment
DECL|macro|PATHSPEC_PREFER_FULL
comment|/* No args means match everything */
end_comment

begin_define
DECL|macro|PATHSPEC_MAXDEPTH_VALID
define|#
directive|define
name|PATHSPEC_MAXDEPTH_VALID
value|(1<<2)
end_define

begin_comment
DECL|macro|PATHSPEC_MAXDEPTH_VALID
comment|/* max_depth field is valid */
end_comment

begin_comment
comment|/* strip the trailing slash if the given path is a gitlink */
end_comment

begin_define
DECL|macro|PATHSPEC_STRIP_SUBMODULE_SLASH_CHEAP
define|#
directive|define
name|PATHSPEC_STRIP_SUBMODULE_SLASH_CHEAP
value|(1<<3)
end_define

begin_comment
comment|/* die if a symlink is part of the given path's directory */
end_comment

begin_define
DECL|macro|PATHSPEC_SYMLINK_LEADING_PATH
define|#
directive|define
name|PATHSPEC_SYMLINK_LEADING_PATH
value|(1<<4)
end_define

begin_comment
comment|/*  * This is like a combination of ..LEADING_PATH and .._SLASH_CHEAP  * (but not the same): it strips the trailing slash if the given path  * is a gitlink but also checks and dies if gitlink is part of the  * leading path (i.e. the given path goes beyond a submodule). It's  * safer than _SLASH_CHEAP and also more expensive.  */
end_comment

begin_define
DECL|macro|PATHSPEC_STRIP_SUBMODULE_SLASH_EXPENSIVE
define|#
directive|define
name|PATHSPEC_STRIP_SUBMODULE_SLASH_EXPENSIVE
value|(1<<5)
end_define

begin_define
DECL|macro|PATHSPEC_PREFIX_ORIGIN
define|#
directive|define
name|PATHSPEC_PREFIX_ORIGIN
value|(1<<6)
end_define

begin_function_decl
specifier|extern
name|int
name|init_pathspec
parameter_list|(
name|struct
name|pathspec
modifier|*
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|parse_pathspec
parameter_list|(
name|struct
name|pathspec
modifier|*
name|pathspec
parameter_list|,
name|unsigned
name|magic_mask
parameter_list|,
name|unsigned
name|flags
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|,
specifier|const
name|char
modifier|*
modifier|*
name|args
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|copy_pathspec
parameter_list|(
name|struct
name|pathspec
modifier|*
name|dst
parameter_list|,
specifier|const
name|struct
name|pathspec
modifier|*
name|src
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|free_pathspec
parameter_list|(
name|struct
name|pathspec
modifier|*
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
name|limit_pathspec_to_literal
parameter_list|(
name|void
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|char
modifier|*
name|find_pathspecs_matching_against_index
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|add_pathspec_matches_against_index
parameter_list|(
specifier|const
name|char
modifier|*
modifier|*
name|pathspec
parameter_list|,
name|char
modifier|*
name|seen
parameter_list|,
name|int
name|specs
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
specifier|const
name|char
modifier|*
name|check_path_for_gitlink
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|void
name|die_if_path_beyond_symlink
parameter_list|(
specifier|const
name|char
modifier|*
name|path
parameter_list|,
specifier|const
name|char
modifier|*
name|prefix
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* PATHSPEC_H */
end_comment

end_unit

